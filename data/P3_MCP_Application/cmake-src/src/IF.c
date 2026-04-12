/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <IF.c>
 *
 *  Description:
 *  ------------
 *  Interface module for control data between the Interface Card and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - IF_Init:                   Initializes the IF interface
 *  - IF_SendMsg:                Send a message to the IF
 *  - IF_Service:                Send a fuze command message
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "Timer.h"
#include "IF.h"
#include "UART.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "FPGA.h"
#include "CRC.h"
#include "WeaponSettings.h"
#include "Fuze.h"

typedef enum
{
  if_stateGET_SYNC_1,
  if_stateGET_SYNC_2,
  if_stateGET_HEADER,
  if_stateGET_PAYLOAD
} eIF_State;

typedef struct
{
  sIF_Header Header;
  uint8_t Payload[IF_MAX_MSG_PAYLOAD_SIZE];
} sIF_Msg;

sIF IF;

static uint32_t IF100msCnt;
static uint8_t IF_RxMsgIndex;
sIF_Msg IF_RxMsg;
static eIF_State IF_State;
static sUART IF_UART;

void IF_Init(char *newAuxStr)
{
	IF_RxMsgIndex = 0;
  	IF_State = if_stateGET_SYNC_1;
  	IF100msCnt = 0;

  	UART_Init(&IF_UART, FPGA_UART_INTERFACE_CARD_ADDR, br115200, parNONE, 1, 0);

  	if (WeaponSettings.DataFileValid == 0U) {
		AUXCTRL_SS.IFControls.AuxOn 			= 1U;
		AUXCTRL_SS.IFControls.SchedulerAuxOn	= 0U;
		AUXCTRL_SS.IFControls.RxAuxOn			= 0U;
		AUXCTRL_SS.IFControls.TxAuxOn			= 0U;
		AUXCTRL_SS.IFControls.ErrDetectOn		= 1U;
  	}

  	if (AUXCTRL_SS.HOBSControls.AuxOn != 0U) {
		sprintf(newAuxStr,"%s%u:IF Initialized\n",newAuxStr,GetTimeUs());
	}
}

void IF_SendMsg(uint8_t IFMsgId, const uint8_t *Data, uint8_t IFDataSize)
{
  sIF_Msg msg;
  uint8_t validatedIFDataSize = IFDataSize;

  if (validatedIFDataSize > IF_MAX_MSG_PAYLOAD_SIZE) {
	  validatedIFDataSize = IF_MAX_MSG_PAYLOAD_SIZE;
  }

  msg.Header.Sync1 = IF_SYNC_1;
  msg.Header.Sync2 = IF_SYNC_2;
  msg.Header.MsgId = IFMsgId;
  msg.Header.DataSize = validatedIFDataSize;
  msg.Header.Spare1 = 0;
  msg.Header.Spare2 = 0;
  msg.Header.Spare3 = 0;

  void * temp = memcpy(msg.Payload, Data, (size_t)validatedIFDataSize);
  (void)temp; /* MISRA deviation: return value not used */

  msg.Header.Checksum = CalculateXOR_Checksum((uint8_t *) &msg.Header.MsgId, sizeof(sIF_Header) + msg.Header.DataSize - IF_H_CHECKSUM);

  if (UART_Write(&IF_UART, (uint8_t *) &msg, sizeof(sIF_Header) + msg.Header.DataSize) == uartSUCCESS) {
	  IF.TxCnt++;

	  if (AUXCTRL_SS.IFControls.TxAuxOn != 0U) {
		  AUXsprintf("%d:IF Tx Message Sent %d\n",GetTimeUs(),IF.TxCnt);
	  }
  }
}

void IF_Service(void)
{
  uint8_t msg_received = 0U; /* FALSE */
  uint8_t rx_ifcnt;
  uint8_t offset = 0;
  uint8_t calculated_checksum;
  uint8_t rx_data[sizeof(sIF_Msg)];
  uint8_t *msg_data;

  rx_ifcnt = UART_Read(&IF_UART, rx_data, sizeof(rx_data));

  msg_data = (uint8_t *) &IF_RxMsg;

  while (offset < rx_ifcnt) {
    switch (IF_State) {
      case if_stateGET_SYNC_1 : {
        if (rx_data[offset] == IF_SYNC_1) {
          msg_data[0] = IF_SYNC_1;
          IF_RxMsgIndex = 1U;
          IF_State = if_stateGET_SYNC_2;
        }
      } break;
      case if_stateGET_SYNC_2 : {
        if (rx_data[offset] == IF_SYNC_2) {
          msg_data[1] = IF_SYNC_2;
          IF_RxMsgIndex = 2U;
          IF_State = if_stateGET_HEADER;
        }
        else {
          IF_State = if_stateGET_SYNC_1;
          offset--;
        }
      } break;
      case if_stateGET_HEADER : {
        msg_data[IF_RxMsgIndex] = rx_data[offset];
        IF_RxMsgIndex++;

        if (IF_RxMsgIndex == sizeof(sIF_Header)) {
          if (IF_RxMsg.Header.DataSize == 0U) {
            msg_received = 1U; /* TRUE */
          }
          else if (IF_RxMsg.Header.DataSize <= IF_MAX_MSG_PAYLOAD_SIZE) {
            IF_State = if_stateGET_PAYLOAD;
          }
          else {
            IF_State = if_stateGET_SYNC_1;
          }
        }
      } break;
      case if_stateGET_PAYLOAD : {
        msg_data[IF_RxMsgIndex] = rx_data[offset];
        IF_RxMsgIndex++;

        if (IF_RxMsgIndex == (sizeof(sIF_Header) + IF_RxMsg.Header.DataSize)) {
          msg_received = 1U; /* TRUE */
        }
      } break;
      default : IF_State = if_stateGET_SYNC_1; break;
    }

    if (msg_received == 1U) {
		  msg_received = 0U; /* FALSE */

		  calculated_checksum = CalculateXOR_Checksum(&msg_data[3], sizeof(sIF_Header) + IF_RxMsg.Header.DataSize - IF_H_CHECKSUM);

		  if (calculated_checksum == IF_RxMsg.Header.Checksum) {
			  IF.RxCnt++;
			  IF.RxTimstamp = GetSystemTime();

			  if (AUXCTRL_SS.IFControls.RxAuxOn != 0U){
				  AUXsprintf("%d:IF Message Received %d\n",GetTimeUs(),IF.RxCnt);
			  }

			  FuzeHandleRxData((uint8_t *) &IF_RxMsg);
		  }
		  else {
			  IF.ErrCnt++;

			  if (AUXCTRL_SS.IFControls.ErrDetectOn != 0U){
				  AUXsprintf("%d:IF Rx Message Timeout ERROR %d\n",GetTimeUs(),IF.ErrCnt);
			  }
		  }
		  IF_State = if_stateGET_SYNC_1;
    }
    offset++;
  }

  IF100msCnt++;

  if ((IF100msCnt % IF_RATE_100MS) == 0U) {
	  IF_SendMsg(FUZE_SETTINGS_ID, (uint8_t *) &SS_FUZE.FuzeSettings, sizeof(SS_FUZE.FuzeSettings));
  }
}



