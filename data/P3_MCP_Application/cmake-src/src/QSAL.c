/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <QSAL.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control data between the QSAL and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - CRC_Initialise                           : Initializes CRC calculation parameters and tables.
 *  - Calculate_CRC                            : Computes CRC value over a given data buffer.
 *  - ResetRxStates                            : Resets the reception state machine and related flags.
 *  - qsal_Isrcb                               : Interrupt Service Routine callback for QSAL data reception.
 *  - QSAL_SeekerInit                          : Initializes the QSAL seeker communication and state.
 *  - QSAL_SeekerSend                          : Sends a message or command to the QSAL seeker.
 *  - QSAL_HandleSBIT_Msg                      : Processes Seeker Built-In Test (SBIT) messages.
 *  - QSAL_HandleDetectionMsg                  : Handles detection event messages from the QSAL seeker.
 *  - ProcessrxByte                            : Processes a single received byte within the QSAL protocol.
 *  - QSAL_SeekerHandleRxDataBytes             : Processes raw incoming data bytes from the seeker.
 *  - QSAL_SeekerHandleRxDataMsgs              : Parses and dispatches complete seeker messages.
 *  - QSAL_RxService                           : Manages reception and processing of all QSAL data traffic.
 *  - QSAL_SeekerSetPRI                        : Sets the seeker's PRI configuration.
 *  - QSAL_SeekerSetAccuracy                   : Updates the seeker's accuracy parameters.
 *  - QSAL_SeekerSetJitter                     : Configures seeker timing jitter parameters.
 *  - QSAL_SeekerSetNewParams                  : Applies a full set of new seeker configuration parameters.
 *  - QSAL_SeekerSetEstTargetClosingVelocity   : Sets estimated target closing velocity for seeker tracking.
 *  - QSAL_SeekerSetEstTargetRange             : Sets estimated range to the target for seeker engagement logic.
 *  - QSAL_SeekerSetEstTargetPitchYaw          : Sets estimated target pitch and yaw angles for seeker guidance.
 *  - QSAL_SeekerSetSeekerToHostOffsets        : Configures physical offsets between seeker and host platform.
 *  - QSAL_GetFrequency                        : Retrieves the current operating frequency setting from the seeker.
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "TypeDefines.h"
#include "FPGA.h"
#include "Interrupts.h"
#include "UART.h"
#include "FIFO.h"
#include "UART_PR.h"
#include "Timer.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Wind.h"
#include "Timer.h"
#include "Fuze.h"
#include "QSAL.h"
#include "MLAN.h"
#include "MCP.h"
#include "AircraftInterface.h"
#include "Weapon.h"
#include "WeaponSettings.h"

#define MS_TO_NS(ms) (ms*1000000)
#define MS_TO_US(ms) (ms*1000)
#define US_TO_NS(us) (us*1000)
#define NS_TO_MS(ns) (ns/1000000)

#define POINT1_MRAD_S16_TO_RAD_DOUBLE(POINT1_MRAD)	 ((double)POINT1_MRAD/10000.0)
#define POINT1_MRAD_S16_TO_RAD_FLOAT32(POINT1_MRAD)	 ((float)POINT1_MRAD/10000.0)
#define POINT2_MRAD_U8_TO_RAD_FLOAT32(POINT1_MRAD)	 ((float)POINT1_MRAD/5000.0)

//packet receiver states KKK - Add states
#define RX_STATE_WAIT_SYNC_WORD1	 0  //exit when bytes == EF BE
#define RX_STATE_WAIT_SYNC_WORD2 	 1  //exit when bytes == BE BA -> go back to first state on error

#define RX_STATE_WAIT_SYNC_BYTE1	 0  // Waiting for sync byte 1: 0xEF
#define RX_STATE_WAIT_SYNC_BYTE2 	 1  // Waiting for sync byte 2: 0xBE
#define RX_STATE_WAIT_SYNC_BYTE3	 2  // Waiting for sync byte 3: 0xBE
#define RX_STATE_WAIT_SYNC_BYTE4 	 3  // Waiting for sync byte 4: 0xBA

#define RX_STATE_WAIT_REST_OF_HEADER 4  // Waiting for remaining bytes of header (after sync bytes)
#define RX_STATE_WAIT_MSG_BODY       5  // exit when bytecount = payloadlength + 12


//detection message est. time. 1 start 1 stop per byte and 36 bytes/message
#define STATUS_MESSAGE_TIME_SEC ((double)(10*36)/3125000.0)

#pragma pack(1)

sQSAL_StateSpace QSAL_SS;
sUART QSALPackUART;

#pragma pack()
#define  CRC_GEN 0x8408         // CRC generator polynomial
unsigned short CRC_LUT[256];    // Lookup table to speed up calculating 16-bit CRC


//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
//
// FUNCTION    : CRC_Initialise
//
// DESCRIPTION : This function initialises the 256-entry lookup table for CRC calculation
//
// INPUTS      : None.
//
// OUTPUTS     : CRC_LUT[256] - CRC lookup table, used to speed up CRC calculation
//
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
void CRC_Initialise(void)
{
  int            index;   // Index into LUT
  int            bit;     // Bit indicator
  unsigned short CRC;     // Value to be written in LUT

  for (index = 0; index < 256; index++)
  {
    CRC = 0x0000;
    for (bit = 0; bit < 8; bit++)
    {
      if (((CRC & 1) ^ ((index >> bit) & 1)) != 0)    { CRC = (CRC >> 1) ^ CRC_GEN; }
      else                                            { CRC =  CRC >> 1;            }
    }
    CRC_LUT[index] = CRC;
  } // for (index = 0; index < 256; index++)
}
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
//
// FUNCTION    : Calculate_CRC
//
// DESCRIPTION : Calculates the 16 bit CRC on an array of bytes.
//
// INPUTS      : Bytes     – Array of bytes
//               NoOfBytes – Number of bytes to calculate CRC on
//
// OUTPUTS     : Return value : 16-bit CRC
//
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
unsigned short Calculate_CRC(unsigned char Bytes[], int NoOfBytes)
{
  unsigned short CRC;   // Cumulative CRC
  int            i;     // Index in array of bytes

  //********* First Reset CRC, And Then Incorporate Each Byte (Four Per Word) Into CRC **********/
  CRC  = 0x0000;
  for (i=0; i<NoOfBytes; i++)
  {
    CRC = CRC_LUT[(Bytes[i] & 0xFF) ^ (CRC & 0xFF)] ^ (CRC >> 8);
  }
  return(CRC);
}
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
//
// FUNCTIONS   : ResetRxStates
//
// DESCRIPTION : This function clears the QSAL seeker message reception statuses, so that the 
//               function 'ProcessRxByte' will start looking for a new sync sequence again
//
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*
void ResetRxStates(void)
{
	QSAL_SS.StateMachine.RxState=0;
	QSAL_SS.StateMachine.RxStateSync1Idx=0;
	QSAL_SS.StateMachine.RxStateSync2Idx=0;
	QSAL_SS.StateMachine.RxStateRxBufferIdx=0;
}
void qsal_Isrcb(unsigned int i)
{
	/*get timestamp
	 *copy msg from uart to fifo
	 *append ts
	 * */
	TIME msgts=GetSystemTime();
	unsigned char* pb=(unsigned char*)&msgts;
	sCharFIFO* pfifo=&QSAL_SS.CommsControls.RxFIFO;
	UART_PR_Read(&QSALPackUART, &QSAL_SS.CommsControls.RxFIFO,500,QSAL_SS.CommsControls.PacketMode);
	PUT_FIFO_Ptr(pfifo,pb[0]);
	PUT_FIFO_Ptr(pfifo,pb[1]);
	PUT_FIFO_Ptr(pfifo,pb[2]);
	PUT_FIFO_Ptr(pfifo,pb[3]);
	PUT_FIFO_Ptr(pfifo,pb[4]);
	PUT_FIFO_Ptr(pfifo,pb[5]);
	PUT_FIFO_Ptr(pfifo,pb[6]);
	PUT_FIFO_Ptr(pfifo,pb[7]);


}
/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerInit                                              */
/*                                                                           */
/*                                                   						 */
/*                                                                           */
/* CODED BY    :                                                 			 */
/*                                                                           */
/* DESCRIPTION : Initialize SAL seeker interface                             */
/*                                                                           */
/* INPUTS      : None                                                        */
/*                                                                           */
/* RETURNS     : 0 - Initialisation OK                                       */
/*               1 - Firmware version does not match                         */
/*                                                                           */
/*****************************************************************************/
unsigned int QSAL_SeekerInit(char *AuxStr, unsigned int FW_Version)
{

//	memset(&QSAL_SS, 0, sizeof(sQSAL_StateSpace));
	CRC_Initialise();
	//populate send msg structs
	//sbit req
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.SyncWord1		=	0xBEEF;
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.SyncWord2		=	0xBABE;
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.MsgId			=	QSAL_SEEKER_ID_SBIT_REQ_ID;
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.PayloadLength	=	0;
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.MsgCount		=	0;
	QSAL_SS.TxMsgs.SBITReqCmd.Hdr.CRC           = 	Calculate_CRC((unsigned char*)&(QSAL_SS.TxMsgs.SBITReqCmd.Hdr.MsgId),6);

	//clear cbit
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.SyncWord1		=	0xBEEF;
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.SyncWord2		=	0xBABE;
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.MsgId			=	QSAL_SEEKER_ID_CLEAR_CBIT;
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.PayloadLength	=	0;
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.MsgCount		=	0;
	QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.CRC           	= 	Calculate_CRC((unsigned char*)&(QSAL_SS.TxMsgs.ClearCBITCmd.Hdr.MsgId),6);

	//standby
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.SyncWord1		=	0xBEEF;
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.SyncWord2		=	0xBABE;
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.MsgId			=	QSAL_SEEKER_ID_STANDBY_CMD;
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.PayloadLength	=	0;
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.MsgCount		=	0;
	QSAL_SS.TxMsgs.StandbyCmd.Hdr.CRC           = 	Calculate_CRC((unsigned char*)&(QSAL_SS.TxMsgs.StandbyCmd.Hdr.MsgId),6);

	//activation
	QSAL_SS.TxMsgs.ActivationCmd.Hdr.SyncWord1		=	0xBEEF;
	QSAL_SS.TxMsgs.ActivationCmd.Hdr.SyncWord2		=	0xBABE;
	QSAL_SS.TxMsgs.ActivationCmd.Hdr.MsgId			=	QSAL_SEEKER_ID_ACTIVATION_CMD;
	QSAL_SS.TxMsgs.ActivationCmd.Hdr.PayloadLength	=	sizeof(QSAL_SS.TxMsgs.ActivationCmd)-12;
	QSAL_SS.TxMsgs.ActivationCmd.Hdr.MsgCount		=	0;
	QSAL_SS.TxMsgs.ActivationCmd.DesigPRI			=	MS_TO_NS(100);//40ms
	QSAL_SS.TxMsgs.ActivationCmd.DesigJitter		=	100; //100ns
	QSAL_SS.TxMsgs.ActivationCmd.DesigAccuracy		=	US_TO_NS(2);//2us
	QSAL_SS.TxMsgs.ActivationCmd.TargetRange		=	10000;
	QSAL_SS.TxMsgs.ActivationCmd.TgtGateLead		=	0;//target gate rising edge lead time wrt detected laser pulse
	QSAL_SS.TxMsgs.ActivationCmd.TgtGateWidth		=	10;//target gate pulse with set to 10x10us=100us default

#ifdef MPSAL_RX_TEST
	QSAL_SS.StateMachine.SendSBITReqCmd			=	0;
	QSAL_SS.StateMachine.SendActiveCmd			=	1;
#else
	QSAL_SS.StateMachine.SendSBITReqCmd			=	1;
	QSAL_SS.StateMachine.SendActiveCmd			=	1;
#endif

//	QSAL_SS.ServiceControl.Cnt10ms 				= 	0;

	QSAL_SS.CommsControls.PacketMode			= 1;//1;
	QSAL_SS.CommsControls.RxInteruptEnable		= 1; // KKK
	QSAL_SS.CommsControls.TxMsgEnabled			= 1;

	if (!WeaponSettings.DataFileValid)
	{
		AUXCTRL_SS.QSALControls.AuxOn 			= 1;
		AUXCTRL_SS.QSALControls.SchedulerAuxOn 	= 0;
		AUXCTRL_SS.QSALControls.RxAuxOn 		= 0;
		AUXCTRL_SS.QSALControls.RxDetailAuxOn 	= 0;
		AUXCTRL_SS.QSALControls.TxAuxOn 		= 0;
		AUXCTRL_SS.QSALControls.ErrDetectOn		= 1;
	}

	//QSAL_SS.Detection.CBITStatus = 1; // Initialise to fail

	INIT_FIFO(QSAL_SS.CommsControls.RxFIFO);
	QSAL_SS.AuxStr[0] = '\0'; //clear buffer

	if (AUXCTRL_SS.QSALControls.AuxOn) {
		sprintf(AuxStr,"%s%u:QSAL_SS Parameters Initialized ",AuxStr,GetTimeUs());
		sprintf(AuxStr,"%s UART BASE ADDRESS = 0x%x\n",AuxStr,FPGA_UART_MPSAL_ADDR);
	}

	UART_PR_Init((sUART *) &QSALPackUART, FPGA_UART_MPSAL_ADDR, br3125000, parNONE, 1, QSAL_SS.CommsControls.PacketMode, 0, 0, QSAL_SS.CommsControls.RxInteruptEnable);
	*((long*)0x43001800)|=(1<<27);//set bit 27 to enable rx while tx active otherwise msgs lost now and then.
//	*((long*)0x43001810) =0xABCD1234;//0xBABEBEEF; //pkt_sync;
//	*((long*)0x43001814) =4;//8;          //pkt_len_offset;
//	*((long*)0x43001818) =4;//12;         //pkt_payload_offset
	*((long*)0x43001810) =0xBABEBEEF; //pkt_sync;
	*((long*)0x43001814) =8;          //pkt_len_offset;
	*((long*)0x43001818) =12;         //pkt_payload_offset


	EnableHardwareInterrupt(qsal_Isrcb, 0, intUART_4, TRUE);

	return(UART_CheckFW_Version((sUART *) &QSALPackUART, FW_Version));

}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSend                                              */
/*                                                                           */
/*                                                   						 */
/*                                                                           */
/* CODED BY    :                                                 			 */
/*                                                                           */
/* DESCRIPTION : Send command message to seeker                              */
/*                                                                           */
/* INPUTS      : None                                                        */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/* NOTE: Should be called every 10 ms                                        */
/*                                                                           */
/*****************************************************************************/
unsigned int QSAL_SeekerSend(void)
{
	unsigned int DataToSend;
	unsigned int msglen;
	sQSAL_MsgHeader *pMsg;

	DataToSend = 0;

	if(QSAL_SS.StateMachine.SendClearCBITCmd==1)
	{
		DataToSend = 1;
		QSAL_SS.StateMachine.SendClearCBITCmd=0;
		pMsg=&QSAL_SS.TxMsgs.ClearCBITCmd.Hdr;
		msglen=sizeof(QSAL_SS.TxMsgs.ClearCBITCmd);
	}
	else if(QSAL_SS.StateMachine.SendSBITReqCmd==1)
	{
		DataToSend = 1;
		pMsg=&QSAL_SS.TxMsgs.SBITReqCmd.Hdr;
		msglen=sizeof(QSAL_SS.TxMsgs.SBITReqCmd);
	}
	else if(QSAL_SS.StateMachine.SendStandbyCmd==1)
	{
		DataToSend = 1;
		pMsg=&QSAL_SS.TxMsgs.StandbyCmd.Hdr;
		msglen=sizeof(QSAL_SS.TxMsgs.StandbyCmd);
	}
	else if(QSAL_SS.StateMachine.SendActiveCmd==1)
	{
		DataToSend = 1;
		pMsg=&QSAL_SS.TxMsgs.ActivationCmd.Hdr;
		msglen=sizeof(QSAL_SS.TxMsgs.ActivationCmd);
	}


	if (DataToSend)
	{
		pMsg->MsgCount++;
		QSAL_SS.SharedData.TxCnt++;
		if(pMsg->MsgId==QSAL_SEEKER_ID_SBIT_REQ_ID){
			QSAL_SS.TxCounters.TxSBITRequestCnt++;
			if (AUXCTRL_SS.QSALControls.TxAuxOn) {
				AUXsprintf("%u:QSAL - Tx SBIT Request Sent - %d\n",GetTimeUs(),QSAL_SS.TxCounters.TxSBITRequestCnt);
			}
		}

		if(pMsg->MsgId==QSAL_SEEKER_ID_CLEAR_CBIT){
			QSAL_SS.TxCounters.TxClearCBITCnt++;
			if (AUXCTRL_SS.QSALControls.TxAuxOn) {
				AUXsprintf("%u:QSAL - Tx Clear CBIT Request Sent - %d\n",GetTimeUs(),QSAL_SS.TxCounters.TxClearCBITCnt);
			}
		}

		if(pMsg->MsgId==QSAL_SEEKER_ID_STANDBY_CMD){
			QSAL_SS.TxCounters.TxStandbyCmdCnt++;
			if (AUXCTRL_SS.QSALControls.TxAuxOn) {
				AUXsprintf("%u:QSAL - Tx Standby Command Sent - %d\n",GetTimeUs(),QSAL_SS.TxCounters.TxStandbyCmdCnt);
			}
		}

		if(pMsg->MsgId==QSAL_SEEKER_ID_ACTIVATION_CMD){
			QSAL_SS.TxCounters.TxActiveCmd++;
			if (AUXCTRL_SS.QSALControls.TxAuxOn) {
				AUXsprintf("%u:QSAL - Tx Active Command Sent - %d\n",GetTimeUs(),QSAL_SS.TxCounters.TxActiveCmd);
			}
		}

		pMsg->CRC=Calculate_CRC((unsigned char*)&pMsg->MsgId,msglen-6);

		UART_PR_Write(&QSALPackUART, (char*)pMsg, msglen);
	}

	return(SAL_SKR_NO_ERROR);
}

void QSAL_HandleSBIT_Msg(void)
{
	QSAL_SS.RxCounters.RxSBITReportMsgCnt++;
	unsigned char *pb=(unsigned char *)&QSAL_SS.CommsControls.RxBuffer[sizeof(sMCP_QSAL_SBITReportMessage)];
	memcpy(&QSAL_SS.TimeStamps.RxSBITTimestamp,pb,8);

	sMCP_QSAL_SBITReportMessage* pMsg = (sMCP_QSAL_SBITReportMessage *)(QSAL_SS.CommsControls.RxBuffer);

	QSAL_SS.SharedData.SBIT 							= pMsg->SBITStatus;
	QSAL_SS.SharedData.CalDataCRC						= pMsg->CalDataCRC;
	QSAL_SS.SharedData.FW_CRC							= pMsg->AppFWCrc;
	QSAL_SS.SharedData.SW_CRC							= pMsg->AppSWCrc;
	QSAL_SS.SharedData.AppFWVersion						= pMsg->AppFWVersion;
	QSAL_SS.SharedData.AppSWVersion						= pMsg->AppSWVersion;

	if (AUXCTRL_SS.QSALControls.RxAuxOn) {
		AUXsprintf("%u:QSAL - Rx SBIT Report Received - %d\n",GetTimeUs(),QSAL_SS.RxCounters.RxSBITReportMsgCnt);
		if (AUXCTRL_SS.QSALControls.RxDetailAuxOn) {
			AUXsprintf("    SBIT = 0x%x\n",QSAL_SS.SharedData.SBIT.DWord);
			AUXsprintf("    CalDataCRC = 0x%x\n",QSAL_SS.SharedData.CalDataCRC);
			AUXsprintf("    FW_CRC = 0x%x\n",QSAL_SS.SharedData.FW_CRC);
			AUXsprintf("    SW_CRC = 0x%x\n",QSAL_SS.SharedData.SW_CRC);
			AUXsprintf("    AppFWVersion = %d\n",QSAL_SS.SharedData.AppFWVersion);
			AUXsprintf("    AppSWVersion = %d\n",QSAL_SS.SharedData.AppSWVersion);
		}
	}

	QSAL_SS.StateMachine.SendSBITReqCmd = 0;
}
void QSAL_HandleDetectionMsg(void)
{

	sMCP_QSAL_StatusMessage* pMsg = (sMCP_QSAL_StatusMessage *)(QSAL_SS.CommsControls.RxBuffer);
	unsigned char *pb=(unsigned char *)&QSAL_SS.CommsControls.RxBuffer[sizeof(sMCP_QSAL_StatusMessage)];
	memcpy(&QSAL_SS.TimeStamps.RxStatusTimestamp,pb,8);

	double TsInSeconds=(double)QSAL_SS.TimeStamps.RxStatusTimestamp/1000000.0;
	double TotalMsgLatency = STATUS_MESSAGE_TIME_SEC;
	if(QSAL_SS.RxCounters.RxStatusMessageQSalMsgCnt!=0)
	{
		if(pMsg->Hdr.MsgCount != (QSAL_SS.RxCounters.RxStatusMessageQSalMsgCnt+1))
		{
			QSAL_SS.RxCounters.RxStatusMessageMissCnt++;
		}
	}
	QSAL_SS.RxCounters.RxStatusMessageQSalMsgCnt			= pMsg->Hdr.MsgCount;
	QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt			= QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt+1;
	QSAL_SS.SharedData.TargetYaw 						    = POINT1_MRAD_S16_TO_RAD_DOUBLE(pMsg->TargetYaw);
	QSAL_SS.SharedData.TargetPitch 						    = POINT1_MRAD_S16_TO_RAD_DOUBLE(pMsg->TargetPitch);
	QSAL_SS.SharedData.DetectionTime					    = TsInSeconds-TotalMsgLatency;
	QSAL_SS.SharedData.EstPRI							    = NS_TO_MS(pMsg->SeekerPRI);
	QSAL_SS.SharedData.RangeCmd							    = QSAL_SS.TxMsgs.ActivationCmd.TargetRange;
	QSAL_SS.SharedData.CBIT								    = pMsg->CBITStatus;
	QSAL_SS.SharedData.Status							    = pMsg->ModesAndStates;
	QSAL_SS.SharedData.ProcessorTemperature				    = pMsg->ProcessorTemp;
	QSAL_SS.SharedData.DetectorTemperature				    = pMsg->DetectorTemp;
	QSAL_SS.SharedData.JitterCmd						    = QSAL_SS.TxMsgs.ActivationCmd.DesigJitter;
	QSAL_SS.SharedData.AccuracyCmd						    = QSAL_SS.TxMsgs.ActivationCmd.DesigAccuracy;

	if (AUXCTRL_SS.QSALControls.RxAuxOn) {
		AUXsprintf("%u:QSAL - Rx Status Message Received - %d ",GetTimeUs(),QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt);
		AUXsprintf("Total Missed %d\n",QSAL_SS.RxCounters.RxStatusMessageMissCnt);
		if (AUXCTRL_SS.QSALControls.RxDetailAuxOn) {
			AUXsprintf("    TargetYaw = %s mrads\n",AUXsprintfDouble(QSAL_SS.SharedData.TargetYaw * 1000.0));
			AUXsprintf("    TargetPitch = %s mrads\n",AUXsprintfDouble(QSAL_SS.SharedData.TargetPitch * 1000.0));
			AUXsprintf("    DetectionTime = %s us\n",AUXsprintfDouble(QSAL_SS.SharedData.DetectionTime * 1000000.0));
			AUXsprintf("    EstPRI = %d ms\n",QSAL_SS.SharedData.EstPRI);
			AUXsprintf("    RangeCmd = %d m\n",QSAL_SS.SharedData.RangeCmd);
			AUXsprintf("    CBIT = 0x%x\n",QSAL_SS.SharedData.CBIT);
			AUXsprintf("    Status = 0x%x\n",QSAL_SS.SharedData.Status);
			AUXsprintf("    Status.SeekerMode = %d\n",QSAL_SS.SharedData.Status.Bits.SeekerMode);
			AUXsprintf("    Status.Tracking = %d\n",QSAL_SS.SharedData.Status.Bits.Tracking);
			AUXsprintf("    Status.TargetPulse = %d\n",QSAL_SS.SharedData.Status.Bits.TargetPulse);
			AUXsprintf("    Status.AnglesValid = %d\n",QSAL_SS.SharedData.Status.Bits.AnglesValid);
			AUXsprintf("    Status.AnglesAccurate = %d\n",QSAL_SS.SharedData.Status.Bits.AnglesAccurate);
			AUXsprintf("    Status.GainStage = %d\n",QSAL_SS.SharedData.Status.Bits.GainStage);
			AUXsprintf("    Status.TempGood = %d\n",QSAL_SS.SharedData.Status.Bits.TempGood);
			AUXsprintf("    Status.Heaters = %d\n",QSAL_SS.SharedData.Status.Bits.Heaters);
			AUXsprintf("    Status.Spare = %d\n",QSAL_SS.SharedData.Status.Bits.Spare);
			AUXsprintf("    ProcessorTemperature = %d degC\n",QSAL_SS.SharedData.ProcessorTemperature);
			AUXsprintf("    DetectorTemperature = %d degC\n",QSAL_SS.SharedData.DetectorTemperature);
			AUXsprintf("    JitterCmd = %d ns\n",QSAL_SS.SharedData.JitterCmd);
			AUXsprintf("    AccuracyCmd = %d ns\n",QSAL_SS.SharedData.AccuracyCmd);
		}
	}
}
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F
//                                                                           
// FUNCTION    : ProcessrxByte                                              	 
//                                                                           
// CREATED     : 2024/09/24                                                  
//                                                                           
// CODED BY    : Regan Govender
//                                                                           
// DESCRIPTION : This function processes the next byte as extracted from the serial receive FIFO (from the QSAL seeker).
//               It first identifies and saves the four synchronization bytes (two 16-bit words), where after it identifies
//               and saves the rest of the header bytes, where after it saves the rest of the message payload.
//
//               On any of the following error conditions, it aborts the extraction and construction of the current 
//               message, and start to look for a new Sync Word sequence again (from scratch):
//
//               - Any of the four sync bytes does not have the expected value
//               - The message ID is not that of either the SEEKER STATUS or the SEEKER SBIT REPORT messages
//               - The message payload length does not match the message ID
//
//              After a full message has been extracted and saved, the CRC of the message is checked. If it is correct, the
//              corresponding higher message handing function is called, given the assembled message buffer (RxBuffer).  
//                                                                           
// INPUTS      : - The next byte as extracted from the serial receive FIFO buffer (function parameter 'rxByte')
//               - The receive state of the current message being constructed (QSAL_SS.StateMachine.RxState)
//                                                                           
// OUTPUTS     : - The receive state of the current message (QSAL_SS.StateMachine.RxState) is updated
//               - The receive message buffer (QSAL_SS.CommsControls.RxBuffer)
//               - The various QSAL receive counters and receive error counters                                                   
//                                                                           
// NOTES       : - Called at 600Hz from TaskManager600HzCommon                   
//                                                                           
//*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F*F
void ProcessrxByte(unsigned char rxByte)
{
	sQSAL_MsgHeader *phdr = (sQSAL_MsgHeader*)QSAL_SS.CommsControls.RxBuffer;

    //********************* Check This Byte's Validity, In Accordance To the Message Reception State *******************
    switch (QSAL_SS.StateMachine.RxState)
	{
		case RX_STATE_WAIT_SYNC_BYTE1:	
				if (rxByte == 0xEF)		
				{ 
					QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE2; 	
				}
				else		   			
				{ 
					ResetRxStates();											
				}
				break;
		case RX_STATE_WAIT_SYNC_BYTE2:	
				if (rxByte == 0xBE)		
				{ 
					phdr->SyncWord1 			 = 0xBEEF;
					QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE3; 	
				}
				else		   			
				{ 
					ResetRxStates();											
				}
				break;
		case RX_STATE_WAIT_SYNC_BYTE3:	
				if (rxByte == 0xBE)		
				{ 
					QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE4; 	
				}
				else		   			
				{ 
					ResetRxStates();											
				}
				break;
		case RX_STATE_WAIT_SYNC_BYTE4:	
				if (rxByte == 0xBA)		
				{ 
					phdr->SyncWord2 			 			= 0xBABE;
					QSAL_SS.StateMachine.RxState 			= RX_STATE_WAIT_REST_OF_HEADER;
					QSAL_SS.StateMachine.RxStateRxBufferIdx	= 4;
				}
				else		   			
				{ 
					ResetRxStates();											
				}
				break;
		case RX_STATE_WAIT_REST_OF_HEADER: 
				QSAL_SS.CommsControls.RxBuffer[QSAL_SS.StateMachine.RxStateRxBufferIdx++] = rxByte;
				if (QSAL_SS.StateMachine.RxStateRxBufferIdx == 12)
				{
					if (((phdr->MsgId == QSAL_SEEKER_ID_SBIT_REPORT_ID) && (phdr->PayloadLength != 88)) ||
						((phdr->MsgId == QSAL_SEEKER_ID_STATUS)         && (phdr->PayloadLength != 24)))
					{
						QSAL_SS.RxCounters.RxMsgSizeErrCnt++;
						ResetRxStates();
						break;
					}
					if ((phdr->MsgId != QSAL_SEEKER_ID_SBIT_REPORT_ID) && (phdr->MsgId != QSAL_SEEKER_ID_STATUS))
					{
						QSAL_SS.RxCounters.RxMsgIdErrCnt++;
						ResetRxStates();
						break;
					}
					QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_MSG_BODY;
				}	
				break;
				
		case RX_STATE_WAIT_MSG_BODY:
				QSAL_SS.CommsControls.RxBuffer[QSAL_SS.StateMachine.RxStateRxBufferIdx++] = rxByte;
				if (QSAL_SS.StateMachine.RxStateRxBufferIdx == (phdr->PayloadLength+12) )
				{
					if (Calculate_CRC((unsigned char*)&phdr->MsgId, phdr->PayloadLength+6) == phdr->CRC)
					{
						QSAL_SS.SharedData.RxTimestamp	= Weapon.SystemTime;
						QSAL_SS.SharedData.RxCnt++;
						if (phdr->MsgId == QSAL_SEEKER_ID_SBIT_REPORT_ID) { QSAL_HandleSBIT_Msg();     }
						if (phdr->MsgId == QSAL_SEEKER_ID_STATUS        ) { QSAL_HandleDetectionMsg(); }
					}
					else
					{
						QSAL_SS.RxCounters.RxCrcErrCnt++;
					}
					ResetRxStates();
				}
				break;

	} // switch (QSAL_SS.StateMachine.RxState)
}

#ifdef MPSAL_RX_TEST
typedef struct
{
	Uint16 SyncWord1;
	Uint16 SyncWord2;
	Uint16 PayloadLength;
	Uint16 MsgId;
} sMPSAL_MsgHeader;
typedef struct
{
	sMPSAL_MsgHeader hdr;
	unsigned char data[22];
	Uint16 Crc;
} sMPSAL_DetMsg;
void ProcessrxByteMPSAL(unsigned char rxByte)
{
	sMPSAL_MsgHeader *phdr=(sMPSAL_MsgHeader*)QSAL_SS.CommsControls.RxBuffer;
	sMPSAL_DetMsg 	 *pMsg=(sMPSAL_DetMsg*)QSAL_SS.CommsControls.RxBuffer;
	if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_SYNC_BYTE1)
	{
		if (rxByte == 0x34)
		{
			QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE2;
		}
		else
		{
			ResetRxStates();
		}
	}
	else if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_SYNC_BYTE2)
	{
		if (rxByte == 0x12)
		{
			QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE3;
			phdr->SyncWord1=0x1234;
		}
		else
		{
			ResetRxStates();
		}
	}
	else if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_SYNC_BYTE3)
	{
		if (rxByte == 0xcd)
		{
			QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_SYNC_BYTE4;
		}
		else
		{
			ResetRxStates();
		}
	}
	else if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_SYNC_BYTE4)
	{
		if (rxByte == 0xab)
		{
			QSAL_SS.StateMachine.RxState = RX_STATE_WAIT_REST_OF_HEADER;
			phdr->SyncWord2=0xabcd;
			QSAL_SS.StateMachine.RxStateRxBufferIdx=4;
		}
		else
		{
			ResetRxStates();
		}
	}
	else if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_REST_OF_HEADER)
	{
		QSAL_SS.CommsControls.RxBuffer[QSAL_SS.StateMachine.RxStateRxBufferIdx++]=rxByte;
		if(QSAL_SS.StateMachine.RxStateRxBufferIdx==8)
		{

			if((phdr->MsgId==0x12)&&(phdr->PayloadLength!=196))//tel msg
			{
				QSAL_SS.RxCounters.RxMsgIdErrCnt++;
				ResetRxStates();
				return;
			}
			if((phdr->MsgId==0x11)&&(phdr->PayloadLength!=28))//det msg
			{
				QSAL_SS.RxCounters.RxMsgSizeErrCnt++;
				ResetRxStates();
				return;
			}
			QSAL_SS.StateMachine.RxState=RX_STATE_WAIT_MSG_BODY;

		}
	}
	else if(QSAL_SS.StateMachine.RxState==RX_STATE_WAIT_MSG_BODY)
	{

		QSAL_SS.CommsControls.RxBuffer[QSAL_SS.StateMachine.RxStateRxBufferIdx++]=rxByte;
		if(QSAL_SS.StateMachine.RxStateRxBufferIdx==(phdr->PayloadLength+4))
		{
			if(utCrc16Skr((unsigned char*)&phdr->PayloadLength,phdr->PayloadLength-2)==*(Uint16*)(&QSAL_SS.CommsControls.RxBuffer[phdr->PayloadLength+2]))
			{
				QSAL_SS.SharedData.RxTimestamp=Weapon.SystemTime;
				QSAL_SS.SharedData.RxCnt++;
				QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt			= QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt+1;
			}
			else
			{
				QSAL_SS.RxCounters.RxCrcErrCnt++;
			}


			ResetRxStates();
		}

	}

}
#endif //MPSAL_RX_TEST
/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerHandleRxData                                      */
/*                                                                           */
/*                                                   						*/
/*                                                                           */
/* CODED BY    :                                                 			*/
/*                                                                           */
/* DESCRIPTION : Handle received seeker data                                 */
/*                                                                           */
/* INPUTS      : unsigned char* - received seeker data                       */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void QSAL_SeekerHandleRxDataBytes(void)
{
	int BytesInFIFO;
	int Cnt;
	unsigned char rxByte;
	static int CommsTimeoutCheckCounter;

	/* get bytecount
	 * for all bytes
	 *  get byte
	 *  process byte
	 * if timeout
	 *  reset state vars
	 **/

	NUMBER_BYTES_IN_FIFO(QSAL_SS.CommsControls.RxFIFO, BytesInFIFO);
	for(Cnt=0;Cnt<BytesInFIFO;Cnt++)
	{
		GET_FIFO_Ptr(((sCharFIFO*)&QSAL_SS.CommsControls.RxFIFO), rxByte);
#ifdef MPSAL_RX_TEST
		ProcessrxByteMPSAL(rxByte);
#else
		ProcessrxByte(rxByte);
#endif

	}
	if(CommsTimeoutCheckCounter++%300==0) //Checked at @2hz = 500000us
		if ((Weapon.SystemTime - QSAL_SS.SharedData.RxTimestamp) >= 400000)
		{
//KKK 		ResetRxStates();
			QSAL_SS.RxCounters.RxTimeoutErrCnt++;
		}

}
void QSAL_SeekerHandleRxDataMsgs(void)
{
	static int CommsTimeoutCheckCounter;
	int Cnt,BytesInFIFO;
	unsigned char rxByte;
	NUMBER_BYTES_IN_FIFO(QSAL_SS.CommsControls.RxFIFO, BytesInFIFO);
	sQSAL_MsgHeader *phdr = (sQSAL_MsgHeader*)QSAL_SS.CommsControls.RxBuffer;

	if (BytesInFIFO>0)
	{
		if(BytesInFIFO<sizeof(sQSAL_MsgHeader))
		{
			QSAL_SS.RxCounters.RxMsgSizeErrCnt++;
			return;
		}
		for(Cnt=0;Cnt<sizeof(sQSAL_MsgHeader);Cnt++)
		{
			GET_FIFO_Ptr(((sCharFIFO*)&QSAL_SS.CommsControls.RxFIFO), rxByte);
			QSAL_SS.CommsControls.RxBuffer[Cnt]=rxByte;
		}
		if(phdr->SyncWord1!=0xBEEF)
		{
			QSAL_SS.RxCounters.RxCrcErrCnt++;
			INIT_FIFO(QSAL_SS.CommsControls.RxFIFO);
			return;
		}
		if((BytesInFIFO-sizeof(sQSAL_MsgHeader))<(phdr->PayloadLength+sizeof(TIME)))
		{
			QSAL_SS.RxCounters.RxMsgSizeErrCnt++;
			return;

		}
		if (((phdr->MsgId == QSAL_SEEKER_ID_SBIT_REPORT_ID) && (phdr->PayloadLength != 88)) ||
			((phdr->MsgId == QSAL_SEEKER_ID_STATUS)         && (phdr->PayloadLength != 24)))
		{
			QSAL_SS.RxCounters.RxMsgSizeErrCnt++;
			return;
		}
		if ((phdr->MsgId != QSAL_SEEKER_ID_SBIT_REPORT_ID) && (phdr->MsgId != QSAL_SEEKER_ID_STATUS))
		{
			QSAL_SS.RxCounters.RxMsgIdErrCnt++;
			return;
		}
		for(Cnt=0;Cnt<(phdr->PayloadLength+8);Cnt++)
		{
			GET_FIFO_Ptr(((sCharFIFO*)&QSAL_SS.CommsControls.RxFIFO), rxByte);
			QSAL_SS.CommsControls.RxBuffer[sizeof(sQSAL_MsgHeader)+Cnt]=rxByte;
		}

		if (Calculate_CRC((unsigned char*)&phdr->MsgId, phdr->PayloadLength+6) == phdr->CRC)
		{
			QSAL_SS.SharedData.RxTimestamp	= Weapon.SystemTime;
			QSAL_SS.SharedData.RxCnt++;
			if (phdr->MsgId == QSAL_SEEKER_ID_SBIT_REPORT_ID)
			{
				QSAL_HandleSBIT_Msg();
			}
			if (phdr->MsgId == QSAL_SEEKER_ID_STATUS)
			{
				QSAL_HandleDetectionMsg();
			}
		}
		else
		{
			QSAL_SS.RxCounters.RxCrcErrCnt++;

			if(AUXCTRL_SS.QSALControls.ErrDetectOn)
				AUXsprintf("%u:QSAL Rx CRC ERROR - %d Opcode: 0x%x \n",GetTimeUs(),QSAL_SS.RxCounters.RxTimeoutErrCnt,phdr->MsgId);
		}
	}

	if(CommsTimeoutCheckCounter++%300==0) //Checked at @2hz = 500000us
		if ((Weapon.SystemTime - QSAL_SS.SharedData.RxTimestamp) >= 400000)   
		{
			QSAL_SS.RxCounters.RxTimeoutErrCnt++;

			if(AUXCTRL_SS.QSALControls.ErrDetectOn)
				AUXsprintf("%u:QSAL Rx Timeout ERROR - %d \n",GetTimeUs(),QSAL_SS.RxCounters.RxTimeoutErrCnt);
		}

}
/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_Service                                              	 */
/*                                                                           */
/* CREATED     : 2024/09/24                                                  */
/*                                                                           */
/* CODED BY    :                                                 	 		 */
/*                                                                           */
/* DESCRIPTION : Polled routine to service QSAL Received messages            */
/*                                                                           */
/* INPUTS      : Scheduler cycle counter (for Aux purposes only              */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/* NOTE: Called at 600Hz from TaskManager600HzCommon                   */
/*                                                                           */
/*****************************************************************************/
void QSAL_RxService(unsigned char Cycle)
{
	QSAL_SeekerHandleRxDataMsgs();
}
void QSAL_SeekerSetPRI(double Frequency)
{
  double period;

  if((Frequency >= 8.0) && (Frequency <= 25.0))
  {
    period = 1000000000.0 / Frequency;
    QSAL_SS.TxMsgs.ActivationCmd.DesigPRI = (unsigned int) period;
  }
}
void QSAL_SeekerSetAccuracy(unsigned short Accuracy)
{
	QSAL_SS.TxMsgs.ActivationCmd.DesigAccuracy = Accuracy;
}
void QSAL_SeekerSetJitter(unsigned short Jitter)
{
	QSAL_SS.TxMsgs.ActivationCmd.DesigJitter = Jitter;
}
void QSAL_SeekerSetNewParms(double Frequency, unsigned short Accuracy, unsigned short Jitter)
{
	QSAL_SeekerSetPRI(Frequency);QSAL_SeekerSetAccuracy(Accuracy);QSAL_SeekerSetJitter(Jitter);
}
void QSAL_SeekerSetEstTargetClosingVelocity(unsigned int ClosingVel,unsigned int VelocityValid)
{
	QSAL_SS.TxMsgs.ActivationCmd.ClosingVelocity = ClosingVel;
	QSAL_SS.TxMsgs.ActivationCmd.ValidityFlags.Bits.VelocityValid = VelocityValid;
}
void QSAL_SeekerSetEstTargetRange(unsigned short TargetRange,unsigned int RangeValid)
{
	QSAL_SS.TxMsgs.ActivationCmd.TargetRange = TargetRange;
	QSAL_SS.TxMsgs.ActivationCmd.ValidityFlags.Bits.RangeValid = RangeValid;
}
void QSAL_SeekerSetEstTargetPitchYaw(short TargetPitch,short TargetYaw,unsigned int AnglesValid)
{
	QSAL_SS.TxMsgs.ActivationCmd.EstTgtPitch = TargetPitch;
	QSAL_SS.TxMsgs.ActivationCmd.EstTgtYaw = TargetYaw;
	QSAL_SS.TxMsgs.ActivationCmd.ValidityFlags.Bits.AnglesValid = AnglesValid;
}
void QSAL_SeekerSetSeekerToHostOffsets(short SkrOffYaw,short SkrOffPitch,short SkrOffRoll)
{
	QSAL_SS.TxMsgs.ActivationCmd.SkrOffYaw = SkrOffYaw;
	QSAL_SS.TxMsgs.ActivationCmd.SkrOffPitch = SkrOffPitch;
	QSAL_SS.TxMsgs.ActivationCmd.SkrOffPitch = SkrOffRoll;
}
/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_GetFrequency                                            */
/*                                                                           */
/*                                                   						 */
/*                                                                           */
/* CODED BY    :                                                			 */
/*                                                                           */
/* DESCRIPTION : Get the seeker laser frequency from NATO code               */
/*                                                                           */
/* INPUTS      : float* - frequency [Hz]                                     */
/*               unsigned int - NATO code                                    */
/*                                                                           */
/* RETURNS     : result - TRUE = successful                                  */
/*                                                                           */
/*****************************************************************************/
unsigned char QSAL_GetFrequency(float *Frequency, unsigned int Code)
{
  unsigned int d1;
  unsigned int d2;
  unsigned int d3;
  unsigned int d4;
  unsigned int n;
  float period = 100.0;           // Default period is 100 ms (10 Hz)
  unsigned char result = TRUE;    // Default is successful


  d1 = Code % 10;
  d2 = (Code / 10) % 10;
  d3 = (Code / 100) % 10;
  d4 = (Code / 1000) % 10;

  if ((d1 > 0) && (d1 < 9) && (d2 > 0) && (d2 < 9) &&
      (d3 > 0) && (d3 < 5) && (d4 == 1)               ) {
    // Band 2 code
    n = d1 + ((d2 - 1) * 8) + ((d3 - 1) * 64);

    if ((n > 0) && (n < 257)) {
      period = 50.0 + (0.1 * n);
    }
    else {
      result = FALSE;
    }
  }
  else if ((d1 > 0) && (d1 < 9) && (d2 > 0) && (d2 < 9) &&
           (d3 > 4) && (d3 < 8) && (d4 == 1)               ) {
    // Band 1 code
    n = d1 + ((d2 - 1) * 8) + ((d3 - 5) * 64);

    if (n > 129) {
      n = n + (n - 129);
    }

    if ((n > 0) && (n < 256)) {
      period = 87.2 + (0.1 * n);
    }
    else {
      result = FALSE;
    }
  }
  else {
    result = FALSE;
  }

  *Frequency = 1000.0 / period;

  // Convert period [ms] to frequency [Hz] and return
  return result;
}
