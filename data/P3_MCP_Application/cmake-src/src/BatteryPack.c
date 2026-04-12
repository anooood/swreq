/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <BatteryPack.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control between the BatteryPack System and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - BatteryPackInit:             Initializes the battery pack controller interface
 *  - BatteryPackSetCommand:       Sets the command message sent to the battery pack controller
 *  - BatteryPackSetLog:           Sets the battery pack log to transmit
 *  - BatteryPackRequestLog:       Request the battery pack log
 *  - BatteryPackSend:             Transmits battery pack message
 *  - BatteryPackSendAuxToCsv:     Battery Info in CSV format sent to AUX PORT
 *  - BatteryPackSendAuxToCsv:     Battery Info sent to AUX PORT
 *  - BatteryPackLogReportSendAux: Battery Log Report sent to AUX PORT
 *  - BatteryPackService:          Extract and handle battery pack received messages
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include "TypeDefines.h"
#include "FPGA.h"
#include "Timer.h"
#include "MCP.h"
#include "UART.h"
#include "CRC.h"
#include "Timer.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WeaponSettings.h"
#include "BatteryPack.h"

#define BATTERY_PACK_SYNC_1                 0xEB
#define BATTERY_PACK_SYNC_2                 0x90

#define BATTERY_PACK_COMMAND_MSG_ID         0x00
#define BATTERY_PACK_STATUS_MSG_ID          0x00
#define BATTERY_PACK_LOG_MSG_ID             0x01
#define BATTERY_PACK_LOG_REQUEST_MSG_ID     0x02
#define BATTERY_PACK_LOG_REPORT_MSG_ID      0x02

#define BATTERY_PART_NUMBER_LENGTH          (unsigned char)(5)
#define BATTERY_ISSUE_NUMBER_LENGTH         (unsigned char)(3)
#define BATTERY_SERIAL_NUMBER_LENGTH        (unsigned char)(8)

typedef enum
{
  bsGET_SYNC_1,
  bsGET_SYNC_2,
  bsGET_CRC,
  bsGET_DATA
} eBateryPackStates;


#pragma pack(1)

typedef struct
{
  unsigned char Sync1;
  unsigned char Sync2;
  unsigned char Checksum;
} sBatteryPackHeader;

typedef struct
{
  sBatteryPackHeader Header;
  unsigned char CommandId;
  unsigned char CommandValue;
} sBatteryPackCommandMsg;

typedef struct
{
  sBatteryPackHeader Header;
  unsigned char Reserved;
  sBatteryPackLog Log;
} sBatteryPackLogDataMsg;

typedef struct
{
  unsigned char Sync1;
  unsigned char Sync2;
} sBatteryPackRequestLogMsg;

typedef struct
{
  unsigned char CtrlMode      : 2;
  unsigned char State         : 1;
  unsigned char Spare         : 5;
} sBatteryHeaterStatus;

typedef struct
{
  sBatteryHeaterStatus HeaterStatus;
  signed char TemperatureSetPoint;
  unsigned char Capacity;
  unsigned short CumulativeUsage;
  unsigned char ActivationCnt;
  signed char Temperature1;
  signed char Temperature2;
  signed char Temperature3;
  signed char Temperature4;
  signed char Temperature5;
  signed char Temperature6;
  signed char Temperature7;
  signed char Temperature8;
  unsigned char AveCurrent_0_10ms;
  unsigned char AveCurrent_10_20ms;
  unsigned char AveCurrent_20_30ms;
  unsigned char AveCurrent_30_40ms;
  unsigned char AveCurrent_40_50ms;
} sBatteryPackStatusData;

typedef struct
{
  unsigned char Reserved;
  sBatteryPackLog Log;
  unsigned short SW_Version;
  unsigned char Spare[2];
} sBatteryPackLogReportData;

typedef struct
{
  sBatteryPackHeader Header;
  union
  {
    unsigned char Data[60];
    sBatteryPackStatusData StatusData;
    sBatteryPackLogReportData LogData;
  };
} sBatteryPackRxMsg;

#pragma pack()

sBATTERY_StateSpace BATT_SS;

sBatteryPack BatteryPack;

static unsigned char BatteryPackSendLogFlag;
static unsigned char BatteryPackRequestLogFlag;
static unsigned char BateryPackRxIndex;
static unsigned char BatteryRxMsgId;
static unsigned short BateryPackRxMsgLenth;
static unsigned int Battery10msCnt;
static eBateryPackStates BateryPackState;
static sBatteryPackCommandMsg BatteryPackCommandMsg;
static sBatteryPackLogDataMsg BatteryPackLogDataMsg;
static sBatteryPackRequestLogMsg BatteryPackRequestLogMsg;
static sBatteryPackRxMsg BatteryPackRxMsg;
static sUART BatteryPackUART;

void BatteryPackInit(char *AuxStr)
{
//  memset(&BATT_SS, 0, sizeof(sBATTERY_StateSpace));

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.BatteryControls.AuxOn 				= 1;
	  AUXCTRL_SS.BatteryControls.SchedulerAuxOn		= 0;
	  AUXCTRL_SS.BatteryControls.AuxToCsv 			= 0;
	  AUXCTRL_SS.BatteryControls.RxAuxOn			= 0;
	  AUXCTRL_SS.BatteryControls.RxDetailAuxOn		= 0;
	  AUXCTRL_SS.BatteryControls.TxAuxOn			= 0;
	  AUXCTRL_SS.BatteryControls.TxDetailAuxOn 		= 0;
	  AUXCTRL_SS.BatteryControls.ErrDetectOn    	= 1;
  }

  BatteryPack.BatteryWaitForRxMsg = FALSE;
  BatteryPack.isBatteryConfigChecked = FALSE;
  BatteryPack.SBIT.Word = 0;

  BATT_SS.AuxStr[0]='\0'; // clear string

  if (AUXCTRL_SS.BatteryControls.AuxOn) {
	sprintf(AuxStr,"%s%u:BATTERY PACK Parameters Initialized\n",AuxStr,GetTimeUs());
  }

  memset(&BatteryPack, 0, sizeof(sBatteryPack));

  BatteryPackSendLogFlag = FALSE;
  BatteryPackRequestLogFlag = FALSE;
  Battery10msCnt = 0;
  BateryPackRxIndex = 0;
  BateryPackState = bsGET_SYNC_1;

  BatteryPackCommandMsg.Header.Sync1 = BATTERY_PACK_SYNC_1;
  BatteryPackCommandMsg.Header.Sync2 = BATTERY_PACK_SYNC_2 | BATTERY_PACK_COMMAND_MSG_ID;
  BatteryPackCommandMsg.CommandId = batSET_CONTROL_MODE;
  BatteryPackCommandMsg.CommandValue = btcAUTO;
  BatteryPackCommandMsg.Header.Checksum = BatteryPackCommandMsg.CommandId ^ BatteryPackCommandMsg.CommandValue;

  BatteryPackLogDataMsg.Header.Sync1 = BATTERY_PACK_SYNC_1;
  BatteryPackLogDataMsg.Header.Sync2 = BATTERY_PACK_SYNC_2 | BATTERY_PACK_LOG_MSG_ID;
  BatteryPackLogDataMsg.Reserved = 0;

  BatteryPackRequestLogMsg.Sync1 = BATTERY_PACK_SYNC_1;
  BatteryPackRequestLogMsg.Sync2 = BATTERY_PACK_SYNC_2 | BATTERY_PACK_LOG_REQUEST_MSG_ID;

  BatteryPackSetCommand(batSET_CONTROL_MODE, btcAUTO);

  UART_Init(&BatteryPackUART, FPGA_UART_BATTERY_PACK_ADDR, br115200, parNONE, 1, 0);
}

void BatteryPackSetCommand(eBatteryPackCommands CommandId, unsigned char CommandValue)
{
  BatteryPackCommandMsg.CommandId = CommandId;
  BatteryPackCommandMsg.CommandValue = CommandValue;
  BatteryPackCommandMsg.Header.Checksum  = BatteryPackCommandMsg.CommandId ^ BatteryPackCommandMsg.CommandValue;

  if (AUXCTRL_SS.BatteryControls.TxAuxOn) {
	  sprintf(BATT_SS.AuxStr,"%s%u:Battery Command (%s) CommandValue: %d  (0-Auto 1-On 2-Off)\n",BATT_SS.AuxStr,GetTimeUs(),
			  printBatteryCommand(BatteryPackCommandMsg.CommandId),BatteryPackCommandMsg.CommandValue);
  }
}

void BatteryPackSetLog(sBatteryPackLog *Log)
{
  BatteryPackLogDataMsg.Log = *Log;
  BatteryPackLogDataMsg.Header.Checksum = CalculateXOR_Checksum(&BatteryPackLogDataMsg.Reserved, sizeof(sBatteryPackLog) + 1);

  BatteryPackSendLogFlag = TRUE;
}

void BatteryPackRequestLog(void)
{
  BatteryPackRequestLogFlag = TRUE;
}

void BatteryPackSendMsg(void)
{
  Battery10msCnt++;

  if ((Battery10msCnt % 5) == 0) {
    BatteryPackService();

    if (BatteryPackSendLogFlag) {
      if (AUXCTRL_SS.BatteryControls.TxAuxOn) {
       	sprintf(BATT_SS.AuxStr,"%s%u:Battery Pack Log Data Tx Message Sent \n",BATT_SS.AuxStr,GetTimeUs());
      }
      BatteryPackSendLogFlag = FALSE;
      UART_Write(&BatteryPackUART, (unsigned char *) &BatteryPackLogDataMsg, sizeof(sBatteryPackLogDataMsg));
    }
    else if (BatteryPackRequestLogFlag) {
      if (AUXCTRL_SS.BatteryControls.TxAuxOn) {
    	sprintf(BATT_SS.AuxStr,"%s%u:Battery Pack Request Log Data Tx Message Sent \n",BATT_SS.AuxStr,GetTimeUs());
      }
      BatteryPackRequestLogFlag = FALSE;
      UART_Write(&BatteryPackUART, (unsigned char *) &BatteryPackRequestLogMsg, sizeof(sBatteryPackRequestLogMsg));
    }
    else {
      if (AUXCTRL_SS.BatteryControls.TxAuxOn) {
        sprintf(BATT_SS.AuxStr,"%s%u:Battery Pack Command Tx Message Sent \n",BATT_SS.AuxStr,GetTimeUs());
      }
      UART_Write(&BatteryPackUART, (unsigned char *) &BatteryPackCommandMsg, sizeof(sBatteryPackCommandMsg));
    }

    BatteryPack.BatteryWaitForRxMsg = TRUE;
    BatteryPack.TxCnt++;
  }
}

void BatteryPackSendAuxToCsv(void){

	sprintf(BATT_SS.AuxStr,"%s%u,",BATT_SS.AuxStr,GetTimeUs());
	sprintf(BATT_SS.AuxStr,"%sBATTERY_PACK,",BATT_SS.AuxStr);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.Capacity);
	sprintf(BATT_SS.AuxStr,"%s,",BATT_SS.AuxStr);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.CumulativeUsage);
	sprintf(BATT_SS.AuxStr,"%s,",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.HeaterCtrl);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.CurrentHeaterState);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.ActivationCnt);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawCapacity);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawCumulativeUsage);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_0_10ms);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_10_20ms);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_20_30ms);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_30_40ms);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_40_50ms);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature1);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature2);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature3);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature4);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature5);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature6);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature7);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.Temperature8);
	sprintf(BATT_SS.AuxStr,"%s%d,",BATT_SS.AuxStr,BatteryPack.TemperatureSetPoint);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.AverageCurrent);
	sprintf(BATT_SS.AuxStr,"%s,",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
}

void BatteryPackSendAux(void){

	sprintf(BATT_SS.AuxStr,"%s Capacity: ",BATT_SS.AuxStr);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.Capacity);
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Cumulative Usage: ",BATT_SS.AuxStr);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.CumulativeUsage);
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Heater Control: %d\n",BATT_SS.AuxStr,BatteryPack.HeaterCtrl);
	sprintf(BATT_SS.AuxStr,"%s Current Heater State: %d\n",BATT_SS.AuxStr,BatteryPack.CurrentHeaterState);
	sprintf(BATT_SS.AuxStr,"%s Activation Cnt: %d\n",BATT_SS.AuxStr,BatteryPack.ActivationCnt);
	sprintf(BATT_SS.AuxStr,"%s Raw Capacity: %d\n",BATT_SS.AuxStr,BatteryPack.RawCapacity);
	sprintf(BATT_SS.AuxStr,"%s Raw Cumulative Usage: %d\n",BATT_SS.AuxStr,BatteryPack.RawCumulativeUsage);
	sprintf(BATT_SS.AuxStr,"%s Raw AveCurrent 0_10ms: %d\n",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_0_10ms);
	sprintf(BATT_SS.AuxStr,"%s Raw AveCurrent 10_20ms: %d\n",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_10_20ms);
	sprintf(BATT_SS.AuxStr,"%s Raw AveCurrent 20_30ms: %d\n",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_20_30ms);
	sprintf(BATT_SS.AuxStr,"%s Raw AveCurrent 30_40ms: %d\n",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_30_40ms);
	sprintf(BATT_SS.AuxStr,"%s Raw AveCurrent 40_50ms: %d\n",BATT_SS.AuxStr,BatteryPack.RawAveCurrent_40_50ms);
	sprintf(BATT_SS.AuxStr,"%s Temperature 1: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature1);
	sprintf(BATT_SS.AuxStr,"%s Temperature 2: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature2);
	sprintf(BATT_SS.AuxStr,"%s Temperature 3: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature3);
	sprintf(BATT_SS.AuxStr,"%s Temperature 4: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature4);
	sprintf(BATT_SS.AuxStr,"%s Temperature 5: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature5);
	sprintf(BATT_SS.AuxStr,"%s Temperature 6: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature6);
	sprintf(BATT_SS.AuxStr,"%s Temperature 7: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature7);
	sprintf(BATT_SS.AuxStr,"%s Temperature 8: %d\n",BATT_SS.AuxStr,BatteryPack.Temperature8);
	sprintf(BATT_SS.AuxStr,"%s Temperature Set Point: %d\n",BATT_SS.AuxStr,BatteryPack.TemperatureSetPoint);
	sprintf(BATT_SS.AuxStr,"%s Avg Current: ",BATT_SS.AuxStr);
	AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.AverageCurrent);
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
}

void BatteryPackLogReportSendAux(void){

	sprintf(BATT_SS.AuxStr,"%s Battery Part Number Left: ",BATT_SS.AuxStr);
	for (int i = 0; i < BATTERY_PART_NUMBER_LENGTH; i++) {

        if (BatteryPack.LogReport.PartNoLeft[i] != '\0') {
            sprintf(BATT_SS.AuxStr,"%s%c",BATT_SS.AuxStr,BatteryPack.LogReport.PartNoLeft[i]);
        } else {
            break;
        }

	}
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Battery Part Number Right: ",BATT_SS.AuxStr);
	for (int i = 0; i < BATTERY_PART_NUMBER_LENGTH; i++) {

        if (BatteryPack.LogReport.PartNoRight[i] != '\0') {
            sprintf(BATT_SS.AuxStr,"%s%c",BATT_SS.AuxStr,BatteryPack.LogReport.PartNoRight[i]);
        } else {
            break;
        }

	}
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Battery Issue Number: ",BATT_SS.AuxStr);
	for (int i = 0; i < BATTERY_ISSUE_NUMBER_LENGTH; i++) {

        if (BatteryPack.LogReport.IssueNo[i] != '\0') {
            sprintf(BATT_SS.AuxStr,"%s%c",BATT_SS.AuxStr,BatteryPack.LogReport.IssueNo[i]);
        } else {
            break;
        }

	}
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Battery Serial Number: ",BATT_SS.AuxStr);
	for (int i = 0; i < BATTERY_SERIAL_NUMBER_LENGTH; i++) {

        if (BatteryPack.LogReport.SerialNo[i] != '\0') {
            sprintf(BATT_SS.AuxStr,"%s%c",BATT_SS.AuxStr,BatteryPack.LogReport.SerialNo[i]);
        } else {
            break;
        }

	}
	sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
	sprintf(BATT_SS.AuxStr,"%s Manufactured Year - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.ManufacturedYear);
	sprintf(BATT_SS.AuxStr,"%s Manufactured Month - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.ManufacturedMonth);
	sprintf(BATT_SS.AuxStr,"%s Manufactured Day - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.ManufacturedDay);
	sprintf(BATT_SS.AuxStr,"%s Last Test Year - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.LastTestYear);
	sprintf(BATT_SS.AuxStr,"%s Last Test Month - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.LastTestMonth);
	sprintf(BATT_SS.AuxStr,"%s Last Test Day - %d\n",BATT_SS.AuxStr,BatteryPack.LogReport.LastTestDay);
	sprintf(BATT_SS.AuxStr,"%s CRC - 0x%x\n",BATT_SS.AuxStr,BatteryPack.LogReport.CRC);
}

void BatteryPackService(void)
{
  volatile unsigned short rx_cnt;
  unsigned short offset;
  unsigned char *msg_data;
  unsigned char rx_data[sizeof(sBatteryPackRxMsg)];


  // Check for new data from batter pack controller
  if ((rx_cnt = UART_Read(&BatteryPackUART, rx_data, sizeof(rx_data))) > 0) {
    // Build message with received data
    msg_data = (unsigned char *) &BatteryPackRxMsg;

    offset = 0;
    BatteryPack.BatteryWaitForRxMsg = FALSE;

    do {
      switch (BateryPackState) {
        case bsGET_SYNC_1 : {
          if (rx_data[offset] == BATTERY_PACK_SYNC_1) {
            msg_data[0] = rx_data[offset];
            BateryPackRxIndex = 1;
            BateryPackState = bsGET_SYNC_2;
          }
        } break;
        case bsGET_SYNC_2 : {
          if ((rx_data[offset] & 0xF0) == BATTERY_PACK_SYNC_2) {
            BatteryRxMsgId = rx_data[offset] & 0x0F;

            if (BatteryRxMsgId == BATTERY_PACK_STATUS_MSG_ID) {
              BateryPackRxMsgLenth = sizeof(sBatteryPackHeader) + sizeof(sBatteryPackStatusData);
            }
            else if (BatteryRxMsgId == BATTERY_PACK_LOG_REPORT_MSG_ID) {
              BateryPackRxMsgLenth = sizeof(sBatteryPackHeader) + sizeof(sBatteryPackLogReportData);
            }
            else {
              if (AUXCTRL_SS.BatteryControls.ErrDetectOn) {
                sprintf(BATT_SS.AuxStr,"%s%u:Battery ERROR UNKNOWN Message ID - 0x%x \n",BATT_SS.AuxStr,GetTimeUs(), BatteryRxMsgId);
              }
              // Unknown message ID
              BatteryPack.ErrCnt++;
              BateryPackRxIndex = 0;
              BateryPackState = bsGET_SYNC_1;
              break;
            }

            msg_data[1] = rx_data[offset];
            BateryPackRxIndex = 2;
            BateryPackState = bsGET_CRC;
          }
        } break;
        case bsGET_CRC : {
          msg_data[2] = rx_data[offset];
          BateryPackRxIndex = 3;
          BateryPackState = bsGET_DATA;
        } break;
        case bsGET_DATA : {
          msg_data[BateryPackRxIndex] = rx_data[offset];

          BateryPackRxIndex++;

          if (BateryPackRxIndex == BateryPackRxMsgLenth) {
            // Verify checksum
            if (BatteryPackRxMsg.Header.Checksum == CalculateXOR_Checksum(BatteryPackRxMsg.Data, BateryPackRxMsgLenth)) {
              BatteryPack.RxCnt++;

              if (BatteryRxMsgId == BATTERY_PACK_STATUS_MSG_ID) {
                BatteryPack.RxTimestamp = GetSystemTime();

                BatteryPack.Capacity = (float) BatteryPackRxMsg.StatusData.Capacity * 0.04;
                BatteryPack.CumulativeUsage = (float) BatteryPackRxMsg.StatusData.CumulativeUsage * 0.0002;

                BatteryPack.HeaterCtrl = BatteryPackRxMsg.StatusData.HeaterStatus.CtrlMode;
                BatteryPack.CurrentHeaterState = BatteryPackRxMsg.StatusData.HeaterStatus.State;
                BatteryPack.ActivationCnt = BatteryPackRxMsg.StatusData.ActivationCnt;
                BatteryPack.RawCapacity = BatteryPackRxMsg.StatusData.Capacity;
                BatteryPack.RawCumulativeUsage = BatteryPackRxMsg.StatusData.CumulativeUsage;
                BatteryPack.RawAveCurrent_0_10ms = BatteryPackRxMsg.StatusData.AveCurrent_0_10ms;
                BatteryPack.RawAveCurrent_10_20ms = BatteryPackRxMsg.StatusData.AveCurrent_10_20ms;
                BatteryPack.RawAveCurrent_20_30ms = BatteryPackRxMsg.StatusData.AveCurrent_20_30ms;
                BatteryPack.RawAveCurrent_30_40ms = BatteryPackRxMsg.StatusData.AveCurrent_30_40ms;
                BatteryPack.RawAveCurrent_40_50ms = BatteryPackRxMsg.StatusData.AveCurrent_40_50ms;
                BatteryPack.Temperature1 = BatteryPackRxMsg.StatusData.Temperature1;
                BatteryPack.Temperature2 = BatteryPackRxMsg.StatusData.Temperature2;
                BatteryPack.Temperature3 = BatteryPackRxMsg.StatusData.Temperature3;
                BatteryPack.Temperature4 = BatteryPackRxMsg.StatusData.Temperature4;
                BatteryPack.Temperature5 = BatteryPackRxMsg.StatusData.Temperature5;
                BatteryPack.Temperature6 = BatteryPackRxMsg.StatusData.Temperature6;
                BatteryPack.Temperature7 = BatteryPackRxMsg.StatusData.Temperature7;
                BatteryPack.Temperature8 = BatteryPackRxMsg.StatusData.Temperature8;
                BatteryPack.TemperatureSetPoint = BatteryPackRxMsg.StatusData.TemperatureSetPoint;

                if(!BatteryPack.isBatteryConfigChecked)
                {
                	BatteryPack.SBIT.Bits.BatteryConfig_Fail = (BatteryPack.TemperatureSetPoint < 35) ? 1 : 0;
                	BatteryPack.isBatteryConfigChecked = TRUE;
                }

                BatteryPack.AverageCurrent = (float) (BatteryPack.RawAveCurrent_0_10ms + BatteryPack.RawAveCurrent_10_20ms +
                      BatteryPack.RawAveCurrent_20_30ms + BatteryPack.RawAveCurrent_30_40ms + BatteryPack.RawAveCurrent_40_50ms) / 50.0;

                if (AUXCTRL_SS.BatteryControls.AuxToCsv) {
                	BatteryPackSendAuxToCsv();
                }
                if (AUXCTRL_SS.BatteryControls.RxAuxOn) {
                	sprintf(BATT_SS.AuxStr,"%s%u:Battery Pack Status Message Received - 0x%x \n",BATT_SS.AuxStr,GetTimeUs(), BatteryRxMsgId);
                	if (AUXCTRL_SS.BatteryControls.RxDetailAuxOn) {
                		BatteryPackSendAux();
                	}
                }
            	if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence) {
                	sprintf(BATT_SS.AuxStr,"%s%u:BATTERY STATUS (WIAC82) \n",BATT_SS.AuxStr,GetTimeUs());
            		sprintf(BATT_SS.AuxStr,"%s Capacity: ",BATT_SS.AuxStr);
            		AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.Capacity);
            		sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
            		sprintf(BATT_SS.AuxStr,"%s Cumulative Usage: ",BATT_SS.AuxStr);
            		AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.CumulativeUsage);
            		sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);
            		sprintf(BATT_SS.AuxStr,"%s Avg Current: ",BATT_SS.AuxStr);
            		AUXformatFloatToStr(BATT_SS.AuxStr, BatteryPack.AverageCurrent);
            		sprintf(BATT_SS.AuxStr,"%s\n",BATT_SS.AuxStr);

            	}
              }
              else if (BatteryRxMsgId == BATTERY_PACK_LOG_REPORT_MSG_ID) {
                BatteryPack.LogReport = BatteryPackRxMsg.LogData.Log;
                BatteryPack.LogReportValid = TRUE;

                if (AUXCTRL_SS.BatteryControls.RxAuxOn) {
                	sprintf(BATT_SS.AuxStr,"%s%u:Battery Pack Log Report Message Received - 0x%x \n",BATT_SS.AuxStr,GetTimeUs(), BatteryRxMsgId);
                	if (AUXCTRL_SS.BatteryControls.RxDetailAuxOn) {
                		BatteryPackLogReportSendAux();
                	}
            	}
              }
            }
            else {
              BatteryPack.ErrCnt++;
              if (AUXCTRL_SS.BatteryControls.ErrDetectOn) {
            	  sprintf(BATT_SS.AuxStr,"%s%u:Battery ERROR CHECKSUM Message - 0x%x \n",BATT_SS.AuxStr,GetTimeUs(), BatteryPackRxMsg.Header.Checksum);
              }
            }

            BateryPackRxIndex = 0;
            BateryPackState = bsGET_SYNC_1;
          }
        } break;
        default : {
          BateryPackRxIndex = 0;
          BateryPackState = bsGET_SYNC_1;
        } break;
      }

      offset++;
    } while (offset < rx_cnt);
  }
  else {
	  if (BatteryPack.BatteryWaitForRxMsg)
	  {
		  BatteryPack.ErrCnt++;
		  BatteryPack.BatteryWaitForRxMsg = FALSE;
		  if (AUXCTRL_SS.BatteryControls.ErrDetectOn){
			  sprintf(BATT_SS.AuxStr,"%s%u:Battery Rx Message Timeout ERROR \n",BATT_SS.AuxStr,GetTimeUs());
		  }
	  }
  }
}


