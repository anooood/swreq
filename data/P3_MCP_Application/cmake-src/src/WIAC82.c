/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WIAC82.c>
 *
 *  Description:
 *  ------------
 *  The WIAC82 module manages communication and control data between the WIAC82 and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - WIAC82_Init                              : Initializes the WIAC82 system and its communication interfaces.
 *  - RT_CheckAddressParity                    : Verifies parity of a remote terminal address.
 *  - RT_GetAddress                            : Retrieves the RT address from a received message.
 *  - RT_GetParity                             : Extracts the parity bit from a message.
 *  - WIAC82_MLAN_SetHeader                    : Sets up a MLAN message header.
 *  - WIAC82_MLAN_Set1553Header                : Configures a MIL-STD-1553 message header.
 *  - WIAC82_PrnMLANHeader                     : Prints or logs a MLAN header for debugging.
 *  - WIAC82_Prn1553Header                     : Prints or logs a 1553 header for debugging.
 *  - WIAC82_SendSBIT_Request                  : Sends a request to perform Startup BIT (SBIT).
 *  - WIAC82_SendStatus_Request                : Sends a status request to the weapon/store.
 *  - WIAC82_SendExecuteArmingBattery          : Sends a command to activate the arming battery.
 *  - WIAC82_SendBatteryDeactivation           : Sends a command to deactivate the battery.
 *  - WIAC82_SendNormalWrap                    : Sends a message with a standard operational wrap.
 *  - WIAC82_SendBrokenWrap                    : Sends a message indicating a failure or error wrap.
 *  - WIAC82_CheckFuzeType                     : Verifies the type of fuze installed.
 *  - WIAC82_VersionNumber                     : Retrieves the version number.
 *  - WIAC82_SendStoreDescriptionReport1T      : Sends the store description report (type 1T).
 *  - PositionStatusValidation                 : Validates the position status of the weapon/adaptor.
 *  - WIAC82_SendWeaponStatusWord8T            : Sends the weapon status word (type 8T).
 *  - WIAC82_SendStoreMonitorReport11T         : Sends store monitoring report (type 11T).
 *  - WIAC82_SendFunctionalExchange3M_17T      : Sends functional exchange message 3M (type 17T).
 *  - WIAC82_SendFunctionalExchange1M_23T      : Sends functional exchange message 1M (type 23T).
 *  - WIAC82_ServiceSBIT_Req                   : Services a received Startup BIT request.
 *  - WIAC82_ServiceStatusReq                  : Processes a status request message.
 *  - WIAC82_ServiceStoreDescriptionReport1T   : Processes store description report (type 1T).
 *  - WIAC82_ServiceWeaponStatusWord8T         : Handles weapon status word message (type 8T).
 *  - WIAC82_ServiceStoreMonitorReport11T      : Handles store monitor report message (type 11T).
 *  - WIAC82_ServiceFunctionalExchange3M_17T   : Handles functional exchange 3M message (type 17T).
 *  - WIAC82_ServiceFunctionalExchange1M_23T   : Handles functional exchange 1M message (type 23T).
 *  - CalculateLatency                         : Calculates communication or system latency.
 *  - WIAC82_ServiceTransferAlignment          : Processes transfer alignment service message.
 *  - WIAC82_Send                              : Sends a generic message over the WIAC82 bus.
 *  - WIAC82_MisfiredWeapon                    : Handles detection or reporting of a misfire event.
 *  - WIAC82_HandleSBITmsg                     : Handles incoming Startup BIT message.
 *  - WIAC82_HandleSTATUSmsg                   : Handles incoming status message.
 *  - GetStationNumber                         : Retrieves the station number for the current adapter.
 *  - GetAdaptorIdentity                       : Gets the identity/profile of the adapter.
 *  - GetAdaptorPosition                       : Gets the physical mounting position of the adapter.
 *  - GetPositionAdaptorConfig                 : Retrieves configuration data for the position adapter.
 *  - WIAC82_HandleAC_DescriptionMsg1R         : Handles aircraft configuration description message (1R).
 *  - WIAC82_HandleAC_SystemTimeMsg2R          : Processes system time sync message (2R).
 *  - WIAC82_HandleGeneralRefusalMsg3R         : Handles a general refusal message (3R).
 *  - GetAirplaneTypeConfig                    : Retrieves configuration based on the aircraft type.
 *  - GetCP1                                   : Retrieves configuration parameter CP1.
 *  - WIAC82_HandleWeaponConfigMsg10R          : Handles weapon configuration message (10R).
 *  - DecodeSCM                                : Decodes the Store Configuration Matrix (SCM).
 *  - DecodeFlag                               : Extracts and interprets specific status flags.
 *  - DecodeValidity                           : Validates decoded data from message fields.
 *  - WIAC82_UnpackCriticalCotrolMsg11R        : Unpacks critical control message (11R).
 *  - WIAC82_HandleWeaponStoreControlMsg11R    : Processes weapon store control message (11R).
 *  - Pack14R_TransferAlignmentData            : Packs transfer alignment data into 14R format.
 *  - WIAC82_HandleStateVector50HzMsg14R       : Processes state vector message at 50Hz (14R).
 *  - DecodeWIAC82_IMU_Number                  : Decodes the IMU number from the message.
 *  - Pack16R_TransferAlignmentData            : Packs transfer alignment data into 16R format.
 *  - WIAC82_HandleStateVector6p25HzMsg16R     : Handles state vector data at 6.25Hz (16R).
 *  - Pack21R_TransferAlignmentData            : Packs transfer alignment data into 21R format.
 *  - WIAC82_HandleFunctionalExchange1DMsg21R  : Processes functional exchange message (21R).
 *  - DecodeWIAC82_GNSS_Mode                   : Decodes GNSS operating mode.
 *  - DecodeWIAC82_FuseSettings                : Decodes fuse configuration settings.
 *  - DecodeWIAC82_SeekerSettings              : Decodes seeker configuration settings.
 *  - WIAC82_UnpackFunctionalExchange3DMsg22R  : Unpacks message 3D (22R) from functional exchange.
 *  - WIAC82_HandleFunctionalExchange3DMsg22R  : Processes functional exchange message 3D (22R).
 *  - DecodeSatUsed                            : Decodes which satellites are in use.
 *  - DecodeSatStatus                          : Decodes the operational status of satellites.
 *  - DecodeSatEphemerisStatus                 : Interprets satellite ephemeris data validity.
 *  - WIAC82_HandleSatteliteUseMsg26R          : Handles satellite usage data (26R).
 *  - WIAC82_HandleGPS_InitializationALMANAC_Msg29R : Handles GPS initialization with ALMANAC data (29R).
 *  - WIAC82_HandleGPS_InitializationEPHEMERIDES_Msg29R : Handles initialization with EPHEMERIS data (29R).
 *  - WIAC82_HandleGPS_Initialization_Msg      : General handler for GPS initialization messages.
 *  - WIAC82_HandleEquipmentDialogueMsg30R     : Handles equipment dialogue exchange message (30R).
 *  - WIAC82_HandleBC_WPNmsg                   : Handles broadcast weapon-related messages.
 *  - WIAC82_HandleBCDataReqmsg                : Handles broadcast data request messages.
 *  - WIAC82_HandleBC_WPN_Cmdmsg               : Handles broadcast weapon command messages.
 *  - WIAC82HandleRxData                       : Central handler for all received WIAC82 messages.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "TypeDefines.h"
#include "ScaleFactors.h"
#include "M1553Convert.h"
#include "MIL1553.h"
#include "CRC.h"
#include "ScheduleControl.h"
#include "Timer.h"
#include "FIFO.h"
#include "UART.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "MPSAL.h"
#include "SAL_Seeker.h"
#include "MLAN.h"
#include "Servos.h"
#include "MCP.h"
#include "BIT.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "Fuze.h"
#include "UBLOX.h"
#include "Mission.h"
#include "Nav.h"
#include "EGM96.h"

sWIAC82_StateSpace WIAC82_SS;

const uint8_t MLAN_Lookup[] = {0,255,254,251,251,249,243,253,246,249,249,242,248,248,250,247,241};

void WIAC82_Init(char *AuxStr)
{

	WIAC82_SS.CommsControls.SBIT_NotSent = 1;
	WIAC82_SS.CommsControls.SBIT_Received = 0;
	WIAC82_SS.CommsControls.Trigger23T = 0;
	WIAC82_SS.CommsControls.Trigger17T = 0;

	WIAC82_SS.TxCounters.SBITReqMsgCnt = 0;
	WIAC82_SS.TxCounters.StatusReqMsgCnt = 0;

	if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.WIAC82Controls.AuxOn 				= 1;			// Module Initialization printout
		AUXCTRL_SS.WIAC82Controls.SchedulerAuxOn		= 0;			// Module scheduler printout
		AUXCTRL_SS.WIAC82Controls.RxAuxOn				= 0;			// All received messages printout
		AUXCTRL_SS.WIAC82Controls.RxDetailAuxOn			= 0;
		AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn		= 0;			// First level of Received details
		AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn		= 0;			// Second level of Received details
		AUXCTRL_SS.WIAC82Controls.TxAuxOn				= 0; 			// All Transmitted messages printout
		AUXCTRL_SS.WIAC82Controls.TxDetailAuxOn 		= 0;
		AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn 		= 0;            // First level of Transmitted details
		AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn 		= 0;			// Second level of Transmitted details
		AUXCTRL_SS.WIAC82Controls.ErrDetectOn			= 1;			// Printout when an exception error detected

		AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq			= 0;			// Printout for WIAC82 SBIT Request transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxStatusReq		= 0;			// Printout for WIAC82 Status Request transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxBCMessage		= 0;			// Printout for Bus Controller Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxNormalWrap		= 0;			// Printout for Normal Wrap Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxBrokenWrap		= 0;			// Printout for Broken Wrap Message transmissions
		AUXCTRL_SS.WIAC82Controls.RxAuxSBIT				= 0;			// Printout for WIAC82 SBIT Message Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxStatus			= 0;			// Printout for WIAC82 Status Message Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxBC_WPN			= 0;			// Printout for BC WPN Message Message Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxBCDataReq		= 0;			// Printout for BC Data  Message Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxBCWPNcmd			= 0;			// Printout for BC WPN Command Receipts

		AUXCTRL_SS.WIAC82Controls.RxAuxAC_Description	= 0;			// Printout for AIRCRAFT DESCRIPTION MESSAGE (1R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxAC_Time			= 0;			// Printout for SYSTEM TIME MESSAGE (2R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxRefusal			= 0;			// Printout for GENERAL REFUSAL MESSAGE (3R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxConfiguration	= 0;			// Printout for CONFIGURATION MESSAGE (10R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxStoresCtl		= 0;			// Printout for STORE CONTROL MESSAGE (11R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxVector50Hz		= 0;			// Printout for STATE VECTOR 50Hz MESSAGE (14R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxVector6p25Hz		= 0;			// Printout for STATE VECTOR 6.25HZ MESSAGE (16R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxFunction1D		= 0;			// Printout for FUNCTIONAL EXCHANGE MESSAGE 1 (21R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxFunction3D		= 0;			// Printout for FUNCTIONAL EXCHANGE MESSAGE 2 (22R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxSatUse			= 0;			// Printout for SATELLITE USE PARAMETERS (26R) Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxALMAC			= 0;			// Printout for GPS INITIALIZATION, ALMANAC (29R) CUSTOM DASSAULT MESSAGE Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxEPHEMERIDES		= 0;			// Printout for GPS INITIALIZATION, EPHEMERIDES (29R) CUSTOM DASSAULT MESSAGE Receipts
		AUXCTRL_SS.WIAC82Controls.RxAuxBC_TEST			= 0;			// Printout for EQUIPMENT DIALOGUE TEST (30R) Receipts

		AUXCTRL_SS.WIAC82Controls.TxAuxStoresRep		= 0;			// Printout for STORE DESCRIPTION REPORT (1T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxStatusWord		= 0;			// Printout for STATUS WORD WEAPON (8T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxMonitorRep		= 0;			// Printout for STORE MONITOR REPORT (11T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxFunction3M		= 0;			// Printout for FUNCTIONAL EXCHANGE 3M (17T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxFunction1M		= 0;			// Printout for FUNCTIONAL EXCHANGE 1M (23T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.TxAuxWPN_TEST			= 0;			// Printout for RETURN DIALOGUE TEST (30T) Message transmissions
		AUXCTRL_SS.WIAC82Controls.PrnMLANHeaderOn		= 0;			// Printout MLAN Header contents
		AUXCTRL_SS.WIAC82Controls.Prn1553HeaderOn		= 0;			// Printout 1553 Header contents
		AUXCTRL_SS.WIAC82Controls.ReleaseSequence       = 0;
		AUXCTRL_SS.WIAC82Controls.MissionData           = 0;
	}
	WIAC82_SS.TxCounters.TxServiceCnt 			= 0;
	WIAC82_SS.TxCounters.SBITReqMsgCnt 			= 0;
	WIAC82_SS.TxCounters.BCMsgCnt		 		= 0;
	WIAC82_SS.TxCounters.NormalWrapMsgCnt 		= 0;
	WIAC82_SS.TxCounters.BrokenWrapMsgCnt 		= 0;
	WIAC82_SS.TxCounters.StoresDescriptionMsgCnt= 0;
	WIAC82_SS.TxCounters.WeaponStsMsgCnt		= 0;
	WIAC82_SS.TxCounters.StoresMonRepMsgCnt		= 0;

	WIAC82_SS.RxCounters.Rx1RMsgCnt 			= 0;
	WIAC82_SS.RxCounters.Rx2RMsgCnt 			= 0;
	WIAC82_SS.RxCounters.Rx3RMsgCnt		 		= 0;
	WIAC82_SS.RxCounters.Rx10RMsgCnt 			= 0;
	WIAC82_SS.RxCounters.Rx11RMsgCnt 	 		= 0;
	WIAC82_SS.RxCounters.Rx14RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx16RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx21RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx22RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx26RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx29RMsgCnt			= 0;
	WIAC82_SS.RxCounters.Rx30RMsgCnt			= 0;

	WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU1 = FALSE;
	WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU2 = FALSE;

	WIAC82_SS.ServiceControls.WpnStatusWordSum = 0;
	WIAC82_SS.RTAddr = 0x3d;

	WIAC82_SS.RxTimestamp = 0;
	WIAC82_SS.RxCnt       = 0;
	WIAC82_SS.TxTimestamp = 0;

	WIAC82_SS.WIAC82ExecuteArming = FALSE;
	WIAC82_SS.PreviousReleaseConsent       = 0;
	WIAC82_SS.PreviousCriticalMonitorArmed = 0;

	WIAC82_SS.AuxStr[0]=0; //Clear Buffer
	if (AUXCTRL_SS.WIAC82Controls.AuxOn) {
		sprintf(AuxStr,"%s%u:WIAC82 Parameters Initialized\n",AuxStr,GetTimeUs());
		sprintf(AuxStr,"%s WIAC82 Node Address (ITUC_NODE_ID) = %d 0x%2.2x\n",AuxStr,WIAC82_MLAN_ITUC_NODE_ID,WIAC82_MLAN_ITUC_NODE_ID);
	}

}
uint8_t RT_CheckAddressParity(uint8_t RTAddr)
{
  uint8_t RT_Address = RTAddr&0x1F;
  uint8_t RT_AddrPar = (RTAddr&0x20)>>5;
  uint8_t BitCnt;

  BitCnt = (RT_AddrPar & 0x1);
  for (int i = 0; i < 5; i++) {
    BitCnt = BitCnt + ((RT_Address >> i) & 0x1);
  }

  return (BitCnt % 2);
}

uint8_t RT_GetAddress(uint8_t RTAddr)
{
  return(RTAddr&0x1F);
}
uint8_t RT_GetParity(uint8_t RTAddr)
{
  return((RTAddr&0x20)>>5);
}

void WIAC82_MLAN_SetHeader (sWIAC82_MLAN_Header *MLANHeader, uint8_t SIAddr, uint8_t DIAddr,
        uint8_t Opcode, uint8_t len, unsigned int *Cnt) {
	MLANHeader->SourceAddr     = SIAddr;
	MLANHeader->DestAddr       = DIAddr;
	MLANHeader->Opcode         = Opcode;
	MLANHeader->Version        = WIAC82_MLAN_VERSION;
	MLANHeader->DataLength     = len - sizeof(sWIAC82_MLAN_Header);
	MLANHeader->SequenceNo     = 1;
	MLANHeader->SequenceCnt    = 1;
	MLANHeader->FrameCnt       = ++(*Cnt);
}
void WIAC82_MLAN_Set1553Header (sWIAC82_1553_Header *Header1553, uint8_t RTAddr, uint8_t SubAddr){
	Header1553->SubAddr     = SubAddr;
//	Header1553->RTAddr      = RTAddr;
	Header1553->RTAddr      = WIAC82_SS.RTAddr;
	Header1553->Spare[0]    = 0;
	Header1553->Spare[1]    = 0;
	Header1553->DiffTime	= 0;
}
void WIAC82_PrnMLANHeader (sWIAC82_MLAN_Header *HeaderMLAN,unsigned int Lookup) {

	if (AUXCTRL_SS.WIAC82Controls.PrnMLANHeaderOn) {
		sprintf(WIAC82_SS.AuxStr,"%s MLAN Header: \n",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s  [From Node:%d 0x%2.2x] ",WIAC82_SS.AuxStr,HeaderMLAN->SourceAddr,HeaderMLAN->SourceAddr);
		if (Lookup)
			sprintf(WIAC82_SS.AuxStr,"%s  [Lookup:%d 0x%2.2x]\n",WIAC82_SS.AuxStr,MLAN_Lookup[HeaderMLAN->SourceAddr],MLAN_Lookup[HeaderMLAN->SourceAddr]);
		else
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s  [To Node:%d 0x%2.2x] ",WIAC82_SS.AuxStr,HeaderMLAN->DestAddr,HeaderMLAN->DestAddr);
		if (Lookup)
			sprintf(WIAC82_SS.AuxStr,"%s  [Lookup:%d 0x%2.2x]\n",WIAC82_SS.AuxStr,MLAN_Lookup[HeaderMLAN->DestAddr],MLAN_Lookup[HeaderMLAN->DestAddr]);
		else
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s  [Op Code=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,HeaderMLAN->Opcode,HeaderMLAN->Opcode);
		sprintf(WIAC82_SS.AuxStr,"%s  [Version=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,HeaderMLAN->Version,HeaderMLAN->Version);
		sprintf(WIAC82_SS.AuxStr,"%s  [Msg Size=%d 0x%4.4x]\n",WIAC82_SS.AuxStr,HeaderMLAN->DataLength,HeaderMLAN->DataLength);
		sprintf(WIAC82_SS.AuxStr,"%s  [Seq No=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,HeaderMLAN->SequenceNo,HeaderMLAN->SequenceNo);
		sprintf(WIAC82_SS.AuxStr,"%s  [Seq Cnt=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,HeaderMLAN->SequenceCnt,HeaderMLAN->SequenceCnt);
		sprintf(WIAC82_SS.AuxStr,"%s  [Frame Cnt=%d 0x%8.8x]\n",WIAC82_SS.AuxStr,(int)HeaderMLAN->FrameCnt,(int)HeaderMLAN->FrameCnt);
	}
}
void WIAC82_Prn1553Header (sWIAC82_1553_Header *Header1553) {
	if (AUXCTRL_SS.WIAC82Controls.Prn1553HeaderOn) {
		sprintf(WIAC82_SS.AuxStr,"%s 1553 Header: \n",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s  SubAddr=[%d:0x%2.2x]\n",WIAC82_SS.AuxStr,Header1553->SubAddr,Header1553->SubAddr);
		sprintf(WIAC82_SS.AuxStr,"%s  RTAddr=[%d:0x%2.2x] Parity=%d\n",WIAC82_SS.AuxStr,RT_GetAddress(Header1553->RTAddr),RT_GetAddress(Header1553->RTAddr),RT_GetParity(Header1553->RTAddr));
		sprintf(WIAC82_SS.AuxStr,"%s  Spare[0]=[%d:0x%4.4x]\n",WIAC82_SS.AuxStr,Header1553->Spare[0],Header1553->Spare[0]);
		sprintf(WIAC82_SS.AuxStr,"%s  Spare[1]=[%d:0x%4.4x]\n",WIAC82_SS.AuxStr,Header1553->Spare[1],Header1553->Spare[1]);
		sprintf(WIAC82_SS.AuxStr,"%s  DiffTime=[0x%8.8x]\n",WIAC82_SS.AuxStr,(unsigned int)Header1553->DiffTime);
	}
}


void WIAC82_SendSBIT_Request(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.SBIT_Request.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID, WIAC82_TX_ID_SBIT_REQ, sizeof(sWIAC82_SBIT_Request_Msg), &WIAC82_SS.TxCounters.SBITReqMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *)(&WIAC82_SS.SBIT_Request), sizeof(sWIAC82_SBIT_Request_Msg));

	WIAC82_SS.WIAC82WaitForRxMsg = TRUE;

	if (AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 SBIT Request Message sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.SBITReqMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.SBIT_Request.MLANHeader,0);
	}

}
void WIAC82_SendStatus_Request(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.status_req_msg.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_ID_STATUS_REQ, sizeof(sWIAC82_StatusRequest_Msg), &WIAC82_SS.TxCounters.StatusReqMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.status_req_msg, sizeof(sWIAC82_StatusRequest_Msg));

	WIAC82_SS.WIAC82WaitForRxMsg = TRUE;

	if (AUXCTRL_SS.WIAC82Controls.TxAuxStatusReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Status Request Message sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.StatusReqMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.status_req_msg.MLANHeader,0);
	}

}
void WIAC82_SendExecuteArmingBattery(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.SBIT_Request.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID, WIAC82_TX_ID_BATTERY_ACT_REQ, sizeof(sWIAC82_SBIT_Request_Msg), &WIAC82_SS.TxCounters.BatteryActReqMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *)(&WIAC82_SS.SBIT_Request), sizeof(sWIAC82_SBIT_Request_Msg));
	if (AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Battery Activation (Execute Arming 1) message sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.BatteryActReqMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.SBIT_Request.MLANHeader,0);
	}

}
void WIAC82_SendBatteryDeactivation(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.SBIT_Request.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID, WIAC82_TX_ID_BATTERY_DEACT_REQ, sizeof(sWIAC82_SBIT_Request_Msg), &WIAC82_SS.TxCounters.BatteryDeactReqMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *)(&WIAC82_SS.SBIT_Request), sizeof(sWIAC82_SBIT_Request_Msg));
	if (AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Battery DeActivation (Execute Arming 0) message sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.BatteryDeactReqMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.SBIT_Request.MLANHeader,0);
	}

}
/*
void WIAC82_SendBCMessage(void) {

	int Cnt;
	int LineCnt;

    MLAN_SetHeader(&WIAC82_SS.bc_msg.Header, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_Message), &WIAC82_SS.TxCounters.BCMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.bc_msg, sizeof(sWIAC82_1553_MLAN_Message));
	if (AUXCTRL_SS.WIAC82Controls.TxAuxBCMessage) {
		sprintf(WIAC82_SS.AuxStr,"%sBC Message sent to WIAC82\n",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s [From Node:%d 0x%2.2x] \n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Header.SourceAddr,WIAC82_SS.bc_msg.Header.SourceAddr);
		sprintf(WIAC82_SS.AuxStr,"%s [To Node:%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Header.DestAddr,WIAC82_SS.bc_msg.Header.DestAddr);
		sprintf(WIAC82_SS.AuxStr,"%s [Op Code=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Header.Opcode,WIAC82_SS.bc_msg.Header.Opcode);
		sprintf(WIAC82_SS.AuxStr,"%s Message Size=%d\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Header.DataLength);
		sprintf(WIAC82_SS.AuxStr,"%s [Sub Address=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.SubAddr,WIAC82_SS.bc_msg.SubAddr);
		sprintf(WIAC82_SS.AuxStr,"%s [RT Address=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.SubAddr,WIAC82_SS.bc_msg.SubAddr);
		sprintf(WIAC82_SS.AuxStr,"%s [Sub Address=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.RTAddr,WIAC82_SS.bc_msg.RTAddr);
		sprintf(WIAC82_SS.AuxStr,"%s [Spare[0]=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Spare[0],WIAC82_SS.bc_msg.Spare[0]);
		sprintf(WIAC82_SS.AuxStr,"%s [Spare[1]=%d 0x%2.2x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.Spare[1],WIAC82_SS.bc_msg.Spare[1]);
		sprintf(WIAC82_SS.AuxStr,"%s [DiffTime=%d 0x%4.4x]\n",WIAC82_SS.AuxStr,WIAC82_SS.bc_msg.SubAddr,WIAC82_SS.bc_msg.SubAddr);
		LineCnt = 0;
		sprintf(WIAC82_SS.AuxStr,"%s Data Sent:\n",WIAC82_SS.AuxStr);
		for (Cnt=0;Cnt<WIAC82_BC_MESSAGE_SIZE_WORDS;Cnt++) {
			sprintf(WIAC82_SS.AuxStr,"%s %4.4x",WIAC82_SS.AuxStr, WIAC82_SS.bc_msg.buf.Words[Cnt]);
			LineCnt++;
			if ((LineCnt & 0xf)  == 0) {
				sprintf(WIAC82_SS.AuxStr,"%s \n",WIAC82_SS.AuxStr);
			}
		}
	}

}
*/
void WIAC82_SendNormalWrap(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.normal_wrap_msg.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_ID_NORMAL_WRAP, sizeof(sWIAC82_NormalWrap_Msg), &WIAC82_SS.TxCounters.NormalWrapMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.normal_wrap_msg, sizeof(sWIAC82_NormalWrap_Msg));
	if (AUXCTRL_SS.WIAC82Controls.TxAuxNormalWrap||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Normal Wrap Message sent to WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs());
		WIAC82_PrnMLANHeader(&WIAC82_SS.normal_wrap_msg.MLANHeader,0);
	}

}
void WIAC82_SendBrokenWrap(void) {

	WIAC82_MLAN_SetHeader(&WIAC82_SS.broken_wrap_msg.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_ID_BROKEN_WRAP, sizeof(sWIAC82_BrokenWrap_Msg), &WIAC82_SS.TxCounters.BrokenWrapMsgCnt);
	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.broken_wrap_msg, sizeof(sWIAC82_BrokenWrap_Msg));
	if (AUXCTRL_SS.WIAC82Controls.TxAuxBrokenWrap||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Broken Wrap Message sent to WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs());
		WIAC82_PrnMLANHeader(&WIAC82_SS.broken_wrap_msg.MLANHeader,0);
	}

}
uint16_t WIAC82_CheckFuzeType(void) {
	uint16_t FuzeType;

	switch (SS_FUZE.Fuze.Mode) {
		case fmNON_PROGRAMMABLE: {
			FuzeType = 0x2020; // " "
		} break;
		case fmDELAY: {
			FuzeType = 0x5020; // "P "
		} break;
		case fmHEIGHT: {
			if (WeaponSettings.WeaponConfig.HOBS_Present)
				FuzeType = 0x2048;  // " H"
			else
				FuzeType = 0x2020;
		} break;
		case fmHEIGHT_AND_DELAY: {
			if (WeaponSettings.WeaponConfig.HOBS_Present)
				FuzeType = 0x5048;  // "PH"
			else
				FuzeType = 0x2020;
		} break;
		default: {
			FuzeType = 0x2020; // " "
		} break;
	}
	return (FuzeType);
}

uint16_t WIAC82_VersionNumber(unsigned short value) {
	uint16_t StoreConfigID;
	char firstValue;
	char secondValue;

	if (value < 10){
		firstValue = '0'; // First value must be 0 (ASCII)
		secondValue = '0' + value; // Second value (ASCII)
	}else{
		firstValue = '0' + (value/10);
		secondValue = '0' + (value%10);
	}

	StoreConfigID = (firstValue << 8) | secondValue;

	return (StoreConfigID);
}

void WIAC82_SendStoreDescriptionReport1T(void) {
	int Count;

	char *CharData;

	WIAC82_MLAN_SetHeader(&WIAC82_SS.StoresRep.HeaderMLAN, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_StoreDescriptionReport_Message), &WIAC82_SS.TxCounters.StoresDescriptionMsgCnt);
	WIAC82_MLAN_Set1553Header(&WIAC82_SS.StoresRep.Header1553, WIAC82_SS.ServiceControls.RTAddr ,WIAC82_STORES_DESCRIPTION_REPORT_MESSAGE_1T);
    // Header Word 1
	WIAC82_SS.StoresRep.Store.Header				= WIAC82_STORE_DESCRIPTION_REPORT_HEADER;	// 0x0421
	// Country Code Word 2
    WIAC82_SS.StoresRep.Store.CountryCode			= WIAC82_STORE_DESCRIPTION_COUNTRY_CODE;	// ASCII ='AE'
    // Identification Word 3
	WIAC82_SS.StoresRep.Store.StoreIdentityBinary	= 0;										// Default=0
	// Store Identity ASCII 1 Word 4
	WIAC82_SS.StoresRep.Store.IDWeaponType			= WIAC82_STORE_DESCRIPTION_ID_WEAPON_TYPE;	// 'P3'
	// Store Identity ASCII 2 Word 5
	WIAC82_SS.StoresRep.Store.IDWeaponRange			= WIAC82_STORE_DESCRIPTION_ID_WEAPON_RANGE;	// '  '
	// Store Identity ASCII 3 Word 6
	CharData = (char *)&WIAC82_SS.StoresRep.Store.ID_BombGuidance;
	if (WeaponSettings.WeaponConfig.BombType == bombMK82)
		CharData[1] = '2';
	else
		CharData[1] = '1';
	if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
		CharData[0] = 'L';
	else
		CharData[0] = 'G';
	// Store Identity ASCII 4 Word 7
	WIAC82_SS.StoresRep.Store.ID_FuzeType			= WIAC82_CheckFuzeType();
	// Store Identity ASCII 5 Word 8
	WIAC82_SS.StoresRep.Store.StoreID5				= 0x2020;									// '  '
	// Store Identity ASCII 6 Word 9
	WIAC82_SS.StoresRep.Store.StoreID6				= 0x2020;									// '  '
	// Store Identity ASCII 7 Word 10
	WIAC82_SS.StoresRep.Store.StoreID7				= 0x2020;									// '  '
	// Store Identity ASCII 8 Word 11
	WIAC82_SS.StoresRep.Store.StoreID8				= 0x2020;									// '  '
	// Maximum Interruptive BIT Time Word 12
	WIAC82_SS.StoresRep.Store.MaxInterruptiveBIT_Time = WIAC82_STORE_DESCRIPTION_MAX_IBIT_TIME;	// Maximum interruptive BIT is unused by the aircraft (Logical ICD Page. 101)

	// Store Configuration ID1 Word 13
	WIAC82_SS.StoresRep.Store.StoreConfigID1		= WIAC82_VersionNumber(MCP.SW_Version%100);	// SW ICD Minor Version '01'
	// Store Configuration ID2 Word 14
	WIAC82_SS.StoresRep.Store.StoreConfigID2		= WIAC82_VersionNumber(MCP.SW_Version/100);	// SW ICD Major Version '11'
	// Store Configuration ID3 Word 15
	WIAC82_SS.StoresRep.Store.StoreConfigID3		= 0x2031;        		   					// SW major using LAR Tables '1'
	// Reserved Words 16..29
	for (Count=0;Count<WIAC82_STORE_DESCRIPTION_RESERVED_SIZE;Count++){
		WIAC82_SS.StoresRep.Store.Reserved[Count]=0;
	}
	WIAC82_SS.StoresRep.Store.CRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.StoresRep.Store.Header,((sizeof(sWIAC82_1553_StoreDescriptionReport_Message)/2)-1));

    MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.StoresRep, sizeof(sWIAC82_1553_MLAN_StoreDescriptionReport_Message));

    // Print-out Auxiliary Controls
	if (AUXCTRL_SS.WIAC82Controls.TxAuxStoresRep||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (1T) STORE DESCRIPTION REPORT sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.StoresDescriptionMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.StoresRep.HeaderMLAN,0);
		WIAC82_Prn1553Header(&WIAC82_SS.StoresRep.Header1553);
		if (AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Header=0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.Header);							// 0x0421
			CharData = (char *)&WIAC82_SS.StoresRep.Store.CountryCode;
			sprintf(WIAC82_SS.AuxStr,"%s  CountryCode=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.CountryCode,CharData[1],CharData[0]);				// ASCII ='AE'
			sprintf(WIAC82_SS.AuxStr,"%s  StoreIdentityBinary=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreIdentityBinary);// Default=0
			CharData = (char *)&WIAC82_SS.StoresRep.Store.IDWeaponType;
			sprintf(WIAC82_SS.AuxStr,"%s  IDWeaponType=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.IDWeaponType,CharData[1],CharData[0]);				// 'P3'
			CharData = (char *)&WIAC82_SS.StoresRep.Store.IDWeaponRange;
			sprintf(WIAC82_SS.AuxStr,"%s  IDWeaponRange=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.IDWeaponRange,CharData[1],CharData[0]);			// '  '
			CharData = (char *)&WIAC82_SS.StoresRep.Store.ID_BombGuidance;
			sprintf(WIAC82_SS.AuxStr,"%s  ID_BombGuidance=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.ID_BombGuidance,CharData[1],CharData[0]);		// Default='3L'
			CharData = (char *)&WIAC82_SS.StoresRep.Store.ID_FuzeType;
			sprintf(WIAC82_SS.AuxStr,"%s  ID_FuzeType=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.ID_FuzeType,CharData[1],CharData[0]);				// Default='F0'
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreID5;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreID5=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreID5,CharData[1],CharData[0]);						// Default='  '
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreID6;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreID6=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreID6,CharData[1],CharData[0]);						// Default='  '
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreID7;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreID7=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreID7,CharData[1],CharData[0]);						// Default='  '
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreID8;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreID8=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreID8,CharData[1],CharData[0]);						// Default='  '
			sprintf(WIAC82_SS.AuxStr,"%s  MaxInterruptiveBIT_Time=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.MaxInterruptiveBIT_Time);	//
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreConfigID1;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreConfigID1=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreConfigID1,CharData[1],CharData[0]);			// Default='  '
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreConfigID2;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreConfigID2=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreConfigID2,CharData[1],CharData[0]);			// SW ICD Minor Version
			CharData = (char *)&WIAC82_SS.StoresRep.Store.StoreConfigID3;
			sprintf(WIAC82_SS.AuxStr,"%s  StoreConfigID3=0x%x (%c%c)\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.StoreConfigID3,CharData[1],CharData[0]);			// SW ICD Major Version
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved = ",	WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_STORE_DESCRIPTION_RESERVED_SIZE;Count++){
				sprintf(WIAC82_SS.AuxStr,"%s[0x%x]",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StoresRep.Store.CRC);
		}
	}
}

uint16_t PositionStatusValidation(sWIAC82_Pylon_Bay_ID *PBI, sWIAC82_Position_Adaptor *PositionAdaptor) {

	switch (PositionAdaptor->Position){
		// CENTER LINE FUSELAGE
		case 1 :{
			// ADAPTOR IDENTITY: CLB900 &&
			// POSITION UNDER ADAPTER: (LEFT || RIGHT) &&
			// STATION NUMBER: CENTERLINE
			if ((PBI->admAdaptorIdentity == 1) &&
					((PBI->admPositionUnderAdaptor == 2) || (PBI->admPositionUnderAdaptor == 3)) &&
					(PBI->admStationNumber == 0x8))
			{
				return 1;
			} else {
				return 0;
			}
		}break;

		// WING 2 LEFT
		case 2 :{
			// ADAPTOR IDENTITY: (CLB900 || AT730) &&
			// POSITION UNDER ADAPTER: (LEFT || CENTRAL || RIGHT) &&
			// STATION NUMBER: WING 2 LEFT (P2G)
			if ((PBI->admAdaptorIdentity == 2) && (PBI->admStationNumber == 0xD)){

				if ((PBI->admPositionUnderAdaptor == 1) || (PBI->admPositionUnderAdaptor == 2) || (PBI->admPositionUnderAdaptor == 3)){
					return 1;
				} else {
					return 0;
				}
			} else if ((PBI->admAdaptorIdentity == 1) && (PBI->admStationNumber == 0xD)){

				if ((PBI->admPositionUnderAdaptor == 2) || (PBI->admPositionUnderAdaptor == 3)){
					return 1;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		}break;

		// WING 2 RIGHT
		case 3 :{
			// ADAPTOR IDENTITY: (CLB900 || AT730) &&
			// POSITION UNDER ADAPTER: (LEFT || CENTRAL || RIGHT) &&
			// STATION NUMBER: WING 2 RIGHT (P2D)
			if ((PBI->admAdaptorIdentity == 2) && (PBI->admStationNumber == 0xE)){

				if ((PBI->admPositionUnderAdaptor == 1) || (PBI->admPositionUnderAdaptor == 2) || (PBI->admPositionUnderAdaptor == 3)){
					return 1;
				} else {
					return 0;
				}
			} else if ((PBI->admAdaptorIdentity == 1) && (PBI->admStationNumber == 0xE)){

				if ((PBI->admPositionUnderAdaptor == 2) || (PBI->admPositionUnderAdaptor == 3)){
					return 1;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		}break;

		default:{
			return 0;
		}break;
	}
}

void WIAC82_SendWeaponStatusWord8T(void) {
	sWIAC82_StatusWordWeapon_Validity *Validity;
	sWIAC82_Status1P32 *Status1P32;
	sWIAC82_Status2P32 *Status2P32;
	sWIAC82_StatusPBIT *OverallPBIT;
	sWIAC82_StatusMode1 *Mode1;
	sWIAC82_StatusMode2 *Mode2;
	sWIAC82_StatusMode3 *Mode3;
	sWIAC82_Pylon_Bay_ID *PBI;
	sWIAC82_Position_Adaptor *PositionAdaptor;
	uint16_t NewWpnStatusWordSum;
	int Count;

	// Save previous CRC
	WIAC82_SS.ServiceControls.WpnStatusWordSum = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.StatusWord.Status.ValidityStatusWord,((sizeof(sWIAC82_1553_StatusWordWeapon_Message)/2)-1));
	WIAC82_MLAN_SetHeader(&WIAC82_SS.StatusWord.HeaderMLAN, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_StatusWordWeapon_Message), &WIAC82_SS.TxCounters.WeaponStsMsgCnt);
	WIAC82_MLAN_Set1553Header(&WIAC82_SS.StatusWord.Header1553, WIAC82_SS.ServiceControls.RTAddr ,WIAC82_STATUS_WORD_WEAPON_MESSAGE_8T);

	// Validity Status Word 2 Al-Tariq P32
	Validity = (sWIAC82_StatusWordWeapon_Validity *)&WIAC82_SS.StatusWord.Status.ValidityStatusWord;
	// Weapon is set for P32
	Validity->StatusP32Valid 	  = 1;
	Validity->StatusAlTariqValid  = 0;
	// The weapon will report Battery Capacity INVALID when the spare capacity goes to 1%.
	if (Weapon.BatteryCapacity > 1)
		Validity->BatterySpareCapacityValid	= 1;
	else
		Validity->BatterySpareCapacityValid	= 0;
	// The weapon is busy doing the PBIT. (Once is completed, the Status PBIT will be valid)
	if (BIT.SBIT_Complete)
		Validity->StatusPBITvalid 			= 1;
	else
		Validity->StatusPBITvalid 			= 0;

	/* The weapon verify coherence between physical address (RT) and the info received in:
	 * AIRCRAFT_DESCRIPTION_MESSAGE (1R) - (Adapter Identity, Store Station and Position Under Adapter)
	 * CONFIGURATION_MESSAGE (10R) - (Charge Position Number and  Position Adapter)
	 *
	 * Deviation: The weapon does not verify the coherence between the physical address (bit reading on the 1760 connector)
	 * and the information given by the AIRCRAFT DESCRIPTION MESSAGE (ADM) and the CONFIGURATION MESSAGE
	 *
	 * The weapon will still return in the STATUS WORD a Store Station Number, Adapter Identity and Position Under Adapter
	 * in conformity with the received information (Adapter Identity, Store station number, Position under adapter
	 * from AIRCRAFT_DESCRIPTION_MESSAGE (1R) and Position Adapter from CONFIGURATION_MESSAGE (10R))
	 */
	PBI = (sWIAC82_Pylon_Bay_ID *)&WIAC82_SS.AC_Description.AC_Descr_Msg.PBI;
	PositionAdaptor = (sWIAC82_Position_Adaptor *)&WIAC82_SS.Configuration.Config.Mixed2;
	Validity->PositionStatusValid = PositionStatusValidation(PBI,PositionAdaptor);

	// Reserved 1..15
	Validity->Reserved 			  = 0;

	// Status P32 Word 3
	Status1P32 = (sWIAC82_Status1P32 *)&WIAC82_SS.StatusWord.Status.Status1P32;
	Status1P32->SWExecutionOverrun 	= MCP.CBIT.Bits.ExecutionOverrunFail;
	Status1P32->IMU_PBIT 			= Weapon.SBIT.Bits.IMU_CommsFail;  // IMU Power-up BIT
	Status1P32->GAINS_PBIT 			= Weapon.SBIT.Bits.GNSS_Fail;
	Status1P32->GCA_BIT	 			= Weapon.SBIT.Bits.GCA_Fail;
	//HOBS Comms Error Power-up BIT && Continuous Build-In Testing
	Status1P32->HOBS_PBIT 			= (Weapon.SBIT.Bits.HOBS_CommsFail || Weapon.CBIT.Bits.HOBS_CommsFail) ? 1 : 0;
	Status1P32->Config_BIT 			= Weapon.SBIT.Bits.ConfigFail;
	//Servo Comms Error Power-up BIT && Continuous Build-In Testing
	Status1P32->ServoPBITandCBIT 	= (Weapon.SBIT.Bits.ServoCommsFail || Weapon.CBIT.Bits.ServoCommsFail) ? 1 : 0;
	Status1P32->Reserved1 			= 0;
	//Seeker Comms Error Power-up BIT && Continuous Build-In Testing
	Status1P32->SAL_PBITandCBIT 	= (Weapon.SBIT.Bits.SeekerCommsFail || Weapon.CBIT.Bits.SeekerCommsFail) ? 1 : 0;
	//Battery Comms Error Power-up BIT && Continuous Build-In Testing
	Status1P32->BatteryPBITandCBIT	= (Weapon.SBIT.Bits.BatteryCommsFail || Weapon.CBIT.Bits.BatteryCommsFail) ? 1 : 0;
	Status1P32->Reserved2 			= 0;
	Status1P32->FinLockPBIT 		= Weapon.CBIT.Bits.FinLockFail; // Servo is Finlocked
	Status1P32->IMU 				= Weapon.CBIT.Bits.IMU_CommsFail; // IMU Continuous Build-In Testing (IMU messages on time)
	Status1P32->GNSS_Receiver 		= Weapon.CBIT.Bits.GNSS_Fail;
	Status1P32->Reserved3 			= 0;
	//HOBS System Status (any CBIT Error)
	Status1P32->HOBS 				= Weapon.CBIT.Bits.HOBS_Fail;

	// Status P32 Word 4
	Status2P32 = (sWIAC82_Status2P32 *)&WIAC82_SS.StatusWord.Status.Status2P32;
	Status2P32->Reserved1			= 0;
	Status2P32->Servo				= Weapon.CBIT.Bits.ServoFail;
	Status2P32->Reserved2			= 0;
	Status2P32->SAL					= Weapon.CBIT.Bits.SeekerFail;
	Status2P32->ArcnetCBITErr		= Weapon.CBIT.Bits.ArcnetReconFail;
	Status2P32->Reserved3			= 0;
	Status2P32->Battery				= Weapon.CBIT.Bits.BatteryFail;
	Status2P32->TailSensor			= Weapon.CBIT.Bits.TailFinFail;
	Status2P32->Reserved4			= 0;
	if (SS_FUZE.Fuze.Status.Bits.CommsOk)
		Status2P32->IF_PCB = 0;
	else
		Status2P32->IF_PCB = 1;
	Status2P32->NAV					= Weapon.CBIT.Bits.NavFail;
	Status2P32->WeaponDegraded		= Weapon.CBIT.Bits.DegradedFail;
	Status2P32->Reserved5			= 0;

	// Status Al-Tariq Word 5
	WIAC82_SS.StatusWord.Status.Status1AlTariq = 0;
	// Status Al-Tariq Word 6
	WIAC82_SS.StatusWord.Status.Status2AlTariq = 0;

	// Status PBIT Word 7
	OverallPBIT = (sWIAC82_StatusPBIT *)&WIAC82_SS.StatusWord.Status.StatusPBIT;
	OverallPBIT->InProgress = (!(BIT.SBIT_Complete));
	if (!(BIT.SBIT_Complete))
		OverallPBIT->PBIT_Result	=			WIAC82_OVERALL_PBIT_NO_RESULT;
	else if (Weapon.CBIT.Bits.CriticalFail)
		OverallPBIT->PBIT_Result	=			WIAC82_OVERALL_PBIT_NO_GO;
	else if (Weapon.CBIT.Bits.DegradedFail)
		OverallPBIT->PBIT_Result	=			WIAC82_OVERALL_PBIT_DEGRADED;
	else
		OverallPBIT->PBIT_Result	=			WIAC82_OVERALL_PBIT_GO;
	OverallPBIT->Reserved			= 0;

	// Status Mode Word 8
	Mode1 = (sWIAC82_StatusMode1 *)&WIAC82_SS.StatusWord.Status.StatusMode1;
	Mode1->WepaonReady			= Weapon.Flags.Bits.Ready;
	Mode1->BatterySpareCapacity	= Weapon.BatteryCapacity;

	if(!Weapon.GNSS_Usage.GPS_NotUsed && Weapon.GNSS_Usage.GlonassNotUsed)
		Mode1->GNSS_Locked			= WIAC82_GPS_LOCKED;
	else if (Weapon.GNSS_Usage.GPS_NotUsed && !Weapon.GNSS_Usage.GlonassNotUsed)
		Mode1->GNSS_Locked			= WIAC82_GLONASS_LOCKED;
	else if (!Weapon.GNSS_Usage.GPS_NotUsed && !Weapon.GNSS_Usage.GlonassNotUsed)
		Mode1->GNSS_Locked			= WIAC82_GPS_GLONASS_LOCKED;
	else
		Mode1->GNSS_Locked			= WIAC82_NO_GNSS_LOCK;

	Mode1->Reserved				= 0;

	// Status Mode Word 9
	Mode2 = (sWIAC82_StatusMode2 *)&WIAC82_SS.StatusWord.Status.StatusMode2;
	//Number of satellites locked
	Mode2->GPS_SAT_Locked		=  UBLOX.GPS_SatUsedCnt;
	Mode2->GLONASS_SAT_Locked	=  UBLOX.GLO_SatUsedCnt;

	// Status Mode Word 10
	Mode3 = (sWIAC82_StatusMode3 *)&WIAC82_SS.StatusWord.Status.StatusMode3;
	Mode3->WeaponAlignment		= Weapon.Status.Bits.NAV_Aligned;
	Mode3->Reserved				= 0;

	if (AUXCTRL_SS.WIAC82Controls.TransferAlignment) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82TransferAlignment: SBIT_Complete: = %d - NAV Aligned = %d - NAV Mode = %d\n",WIAC82_SS.AuxStr,GetTimeUs(),
				BIT.SBIT_Complete,Weapon.Status.Bits.NAV_Aligned,Weapon.Status.Bits.NAV_Mode);
	}

	// Position Status Word 11
	WIAC82_SS.StatusWord.Status.PositionStatus = WIAC82_SS.AC_Description.AC_Descr_Msg.PBI;
	// Position Status Word 12..16
	for (Count=0;Count<WIAC82_STATUS_WORD_WEAON_RESERVED_WORDS;Count++){
		WIAC82_SS.StatusWord.Status.Reserved[Count]	=	0;
	}

	// Change Indication in Data Word 1
	NewWpnStatusWordSum = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.StatusWord.Status.ValidityStatusWord,((sizeof(sWIAC82_1553_StatusWordWeapon_Message)/2)-1));
	if (WIAC82_SS.ServiceControls.WpnStatusWordSum == NewWpnStatusWordSum){
		// Only allow set to 0 when it has been at least 200ms since last change (1553 ICD)
		if (WIAC82_SS.StatusWord.Status.DataChanged == 1){
			if (GetElapsedTime(WIAC82_SS.DataChangedTimer) > 200000)
				WIAC82_SS.StatusWord.Status.DataChanged = 0;
		}
	} else {
		WIAC82_SS.DataChangedTimer = GetSystemTime();
		WIAC82_SS.StatusWord.Status.DataChanged = 1;
	}

    MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.StatusWord, sizeof(sWIAC82_1553_MLAN_StatusWordWeapon_Message));

    // Print-out Auxiliary Controls
	if (AUXCTRL_SS.WIAC82Controls.TxAuxStatusWord||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (8T) WEAPON STATUS MESSAGE sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.WeaponStsMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.StatusWord.HeaderMLAN,0);
		WIAC82_Prn1553Header(&WIAC82_SS.StatusWord.Header1553);
		if (AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  DataChanged=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.DataChanged);
			sprintf(WIAC82_SS.AuxStr,"%s  ValidityStatusWord=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.ValidityStatusWord);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   StatusP32Valid=0x%x\n",	WIAC82_SS.AuxStr,Validity->StatusP32Valid);
				sprintf(WIAC82_SS.AuxStr,"%s   StatusAlTariqValid=0x%x\n",	WIAC82_SS.AuxStr,Validity->StatusAlTariqValid);
				sprintf(WIAC82_SS.AuxStr,"%s   BatterySpareCapacityValid=0x%x\n",	WIAC82_SS.AuxStr,Validity->BatterySpareCapacityValid);
				sprintf(WIAC82_SS.AuxStr,"%s   StatusPBITvalid=0x%x\n",	WIAC82_SS.AuxStr,Validity->StatusPBITvalid);
				sprintf(WIAC82_SS.AuxStr,"%s   PositionStatusValid=0x%x\n",	WIAC82_SS.AuxStr,Validity->PositionStatusValid);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved=0x%x\n",	WIAC82_SS.AuxStr,Validity->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Status1P32=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.Status1P32);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   SWExecutionOverrun=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->SWExecutionOverrun);
				sprintf(WIAC82_SS.AuxStr,"%s   IMU_PBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->IMU_PBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   GAINS_PBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->GAINS_PBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   GCA_BIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->GCA_BIT);
				sprintf(WIAC82_SS.AuxStr,"%s   HOBS_PBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->HOBS_PBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   Config_BIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->Config_BIT);
				sprintf(WIAC82_SS.AuxStr,"%s   ServoPBITandCBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->ServoPBITandCBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved1=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->Reserved1);
				sprintf(WIAC82_SS.AuxStr,"%s   SAL_PBITandCBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->SAL_PBITandCBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   BatteryPBITandCBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->BatteryPBITandCBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved2=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->Reserved2);
				sprintf(WIAC82_SS.AuxStr,"%s   FinLockPBIT=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->FinLockPBIT);
				sprintf(WIAC82_SS.AuxStr,"%s   IMU=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->IMU);
				sprintf(WIAC82_SS.AuxStr,"%s   GNSS_Receiver=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->GNSS_Receiver);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved3=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->Reserved3);
				sprintf(WIAC82_SS.AuxStr,"%s   HOBS=0x%x\n",	WIAC82_SS.AuxStr,Status1P32->HOBS);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Status2P32=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.Status2P32);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved1=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Reserved1);
				sprintf(WIAC82_SS.AuxStr,"%s   Servo=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Servo);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved2=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Reserved2);
				sprintf(WIAC82_SS.AuxStr,"%s   SAL=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->SAL);
				sprintf(WIAC82_SS.AuxStr,"%s   ArcnetCBITErr=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->ArcnetCBITErr);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved3=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Reserved3);
				sprintf(WIAC82_SS.AuxStr,"%s   Battery=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Battery);
				sprintf(WIAC82_SS.AuxStr,"%s   TailSensor=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->TailSensor);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved4=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Reserved4);
				sprintf(WIAC82_SS.AuxStr,"%s   IF_PCB=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->IF_PCB);
				sprintf(WIAC82_SS.AuxStr,"%s   AV=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->NAV);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved5=0x%x\n",	WIAC82_SS.AuxStr,Status2P32->Reserved5);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Status1AlTariq=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.Status1AlTariq);
			sprintf(WIAC82_SS.AuxStr,"%s  Status2AlTariq=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.Status2AlTariq);
			sprintf(WIAC82_SS.AuxStr,"%s  StatusPBIT=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.StatusPBIT);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   InProgress=0x%x\n",	WIAC82_SS.AuxStr,OverallPBIT->InProgress);
				sprintf(WIAC82_SS.AuxStr,"%s   PBIT_Result=0x%x\n",	WIAC82_SS.AuxStr,OverallPBIT->PBIT_Result);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved=0x%x\n",	WIAC82_SS.AuxStr,OverallPBIT->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  StatusMode1=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.StatusMode1);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s    WeaponReady=0x%x\n",	WIAC82_SS.AuxStr,Mode1->WepaonReady);
				sprintf(WIAC82_SS.AuxStr,"%s    BatterySpareCapacity=0x%x\n",	WIAC82_SS.AuxStr,Mode1->BatterySpareCapacity);
				sprintf(WIAC82_SS.AuxStr,"%s    GNSS_Locked=0x%x\n",	WIAC82_SS.AuxStr,Mode1->GNSS_Locked);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved=0x%x\n",	WIAC82_SS.AuxStr,Mode1->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  StatusMode2=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.StatusMode2);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s    GPS_SAT_Locked=0x%x\n",	WIAC82_SS.AuxStr,Mode2->GPS_SAT_Locked);
				sprintf(WIAC82_SS.AuxStr,"%s    GLONASS_SAT_Locked=0x%x\n",	WIAC82_SS.AuxStr,Mode2->GLONASS_SAT_Locked);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  StatusMode3=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.StatusMode3);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s    WeaponAlignment=0x%x\n",	WIAC82_SS.AuxStr,Mode3->WeaponAlignment);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved=0x%x\n",	WIAC82_SS.AuxStr,Mode3->Reserved);;
			}
			sprintf(WIAC82_SS.AuxStr,"%s  PositionStatus=0x%x\n",	WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.PositionStatus);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				PBI = (sWIAC82_Pylon_Bay_ID *)&WIAC82_SS.StatusWord.Status.PositionStatus;
				sprintf(WIAC82_SS.AuxStr,"%s    StationNumber=0x%x\n",	WIAC82_SS.AuxStr,PBI->admStationNumber);
				sprintf(WIAC82_SS.AuxStr,"%s    AdaptorIdentity=0x%x\n",	WIAC82_SS.AuxStr,PBI->admAdaptorIdentity);
				sprintf(WIAC82_SS.AuxStr,"%s    PositionUnderAdaptor=0x%x\n",	WIAC82_SS.AuxStr,PBI->admPositionUnderAdaptor);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved=0x%x\n",	WIAC82_SS.AuxStr,PBI->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved=",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_STATUS_WORD_WEAON_RESERVED_WORDS;Count++){
				sprintf(WIAC82_SS.AuxStr,"%s [0x%x]",WIAC82_SS.AuxStr,WIAC82_SS.StatusWord.Status.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		}
	}
}

void WIAC82_SendStoreMonitorReport11T(void) {
	sWIAC82_1553_StoreMonitorReportValidity1	*InValidity1;
	sWIAC82_1553_StoreMonitorReportValidity2	*InValidity2;
	sWIAC82_1553_StoreMonitorReportCriticalMonitor1 *CriticalMonitor1;
	sWIAC82_1553_StoreMonitorReportCriticalMonitor2 *CriticalMonitor2;
	sWIAC82_1553_StoreMonitorReportProtocolStatus *ProtocolStatus;
	sWIAC82_Store_Critical_Control_1 *CriticalCtrl1;

	int Count;

	WIAC82_MLAN_SetHeader(&WIAC82_SS.MonitorRep.HeaderMLAN, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_StoreMonitorReport_Message), &WIAC82_SS.TxCounters.StoresMonRepMsgCnt);
	WIAC82_MLAN_Set1553Header(&WIAC82_SS.MonitorRep.Header1553, WIAC82_SS.ServiceControls.RTAddr ,WIAC82_STORE_MONITOR_REPORT_MESSAGE_11T);
    // Header Word 1
	WIAC82_SS.MonitorRep.Stores.Header				= WIAC82_STORE_MONITOR_REPORT_HEADER;	// 0x0420

	// Invalidity for Words 1-16 (Word 2)
    InValidity1 = (sWIAC82_1553_StoreMonitorReportValidity1	*)&WIAC82_SS.MonitorRep.Stores.InValidity1;
    InValidity1->ValHeader				= 0;
    InValidity1->InValidity1			= 0;
    InValidity1->InValidity2			= 0;
    InValidity1->ValCriticalMonitor1	= 0;
    InValidity1->ValCriticalMonitor2	= 0;
    InValidity1->ValReserved1			= 0;
    InValidity1->ValProtocolStatus		= 0;
    InValidity1->ValReserved2			= 0;

    // Invalidity for Words 17-30 (Word 3)
	InValidity2 = (sWIAC82_1553_StoreMonitorReportValidity2	*)&WIAC82_SS.MonitorRep.Stores.InValidity2;
	InValidity2->ValReserved2		= 0;
	InValidity2->ValCRC				= 0;
	InValidity2->NU					= 0;

	// Critical Monitor 1 - Store State & Demanded State Word 4
	CriticalCtrl1 = (sWIAC82_Store_Critical_Control_1 *)&WIAC82_SS.StoresCtl.Store.Control1;
	CriticalMonitor1 = (sWIAC82_1553_StoreMonitorReportCriticalMonitor1	*)&WIAC82_SS.MonitorRep.Stores.CriticalMonitor1;
	CriticalMonitor1->ReleaseModeD3 		= 	CriticalCtrl1->ReleaseModeD3;
	CriticalMonitor1->InitiateIBitD4		= 	CriticalCtrl1->InitiateIBitD4;
	CriticalMonitor1->SelectStoreD5			= 	CriticalCtrl1->SelectStoreD5;
	CriticalMonitor1->PreSetArmingD6		= 	CriticalCtrl1->PreSetArmingD6;
	CriticalMonitor1->ExecuteArmingD7		= 	CriticalCtrl1->ExecuteArmingD7;
	CriticalMonitor1->CommitToSeperateD8	= 	CriticalCtrl1->CommitToSeperateD8;
	CriticalMonitor1->JettisonD9			= 	CriticalCtrl1->JettisonD9;
	CriticalMonitor1->ReleaseD10			= 	CriticalCtrl1->ReleaseD10;		// Fire, Launch and Release

	CriticalMonitor1->LaunchModeR			= 0;
	CriticalMonitor1->InitiateIBitR			= 0;
	CriticalMonitor1->SelectStoreR			= 0;
	CriticalMonitor1->PresetArmingR			= 0;

	// ARMED status remains at �1� unless the weapon detects a battery voltage level below pre-set threshold.
	//         In such case it will set ARMED to �0�.
	// Armed - Set to '1' if battery activated and Weapon is ARMED - Logical ICD DATA ITEM NUMBER 0-7
	CriticalMonitor1->ArmedR					= Weapon.BatteryFirstActivation;
	CriticalMonitor1->CommittedToStoreSeparateR	= Weapon.Flags.Bits.WeaponCDSS; // Committed to Separate

	CriticalMonitor1->JettisonR				= 0;
	CriticalMonitor1->ReleasedR				= 0;

	// Critical Monitor 2 Word 5
	CriticalMonitor2 = (sWIAC82_1553_StoreMonitorReportCriticalMonitor2	*)&WIAC82_SS.MonitorRep.Stores.CriticalMonitor2;
	CriticalMonitor2->CommandedStoreState	= 0;
	if ((WIAC82_SS.Critical.CritCtrl2Result == 0) && (WIAC82_SS.Critical.EraseStart == 1)) {
		CriticalMonitor2->EraseCommand = 1;		// Done
		WIAC82_SS.Critical.EraseStart = 0;
	}
	else {
		CriticalMonitor2->EraseCommand = 0;		// Not activated
	}
	// Reserved Word 6
	WIAC82_SS.MonitorRep.Stores.Reserved1 = 0;

	// Protocol Status Word 7
	ProtocolStatus = (sWIAC82_1553_StoreMonitorReportProtocolStatus *)&WIAC82_SS.MonitorRep.Stores.ProtocolStatus;
	ProtocolStatus->CheckSumError = WIAC82_SS.Critical.Msg11R_CRC_NOK;
	ProtocolStatus->IllegalHeaderSubAddress = WIAC82_SS.Critical.Msg11R_Header_NOK;
	ProtocolStatus->SubAddressOfError = WIAC82_SS.Critical.Msg11R_RTAddress;
	ProtocolStatus->CriticalControl1Error = WIAC82_SS.Critical.CritCtrl1Result;
	ProtocolStatus->CriticalControl2Error = WIAC82_SS.Critical.CritCtrl2Result;

	// Reserved Word 8..29
	for (Count=0;Count<WIAC82_STORE_MONITOR_REPORT_RESERVED2_WORDS;Count++) {
		WIAC82_SS.MonitorRep.Stores.Reserved2[Count] = 0;
	}
	// CheckSum Word 30
	WIAC82_SS.MonitorRep.Stores.CRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.MonitorRep.Stores,((sizeof(sWIAC82_1553_StoreMonitorReport_Message)/2)-1));

	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.MonitorRep, sizeof(sWIAC82_1553_MLAN_StoreMonitorReport_Message));

	if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence){
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (11T) STORES MONITOR REPORT\n",WIAC82_SS.AuxStr,GetTimeUs());
		sprintf(WIAC82_SS.AuxStr,"%s   CommittedToStoreSeparateR = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->CommittedToStoreSeparateR);
		sprintf(WIAC82_SS.AuxStr,"%s   ArmedR      				 = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ArmedR);
		sprintf(WIAC82_SS.AuxStr,"%s   CommitToSeperateD8  		 = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->CommitToSeperateD8);
		sprintf(WIAC82_SS.AuxStr,"%s   ExecuteArmingD7           = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ExecuteArmingD7);
	}

	// Print-out Auxiliary Controls
	if (AUXCTRL_SS.WIAC82Controls.TxAuxMonitorRep||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (11T) STORES MONITOR REPORT sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.StoresMonRepMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.MonitorRep.HeaderMLAN,0);
		WIAC82_Prn1553Header(&WIAC82_SS.MonitorRep.Header1553);
		if (AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Header           = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.Header);
			sprintf(WIAC82_SS.AuxStr,"%s  InValidity1      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.InValidity1);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   Header              = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValHeader,DecodeValidity(InValidity1->ValHeader,1));
				sprintf(WIAC82_SS.AuxStr,"%s   InValidity1Flags    = %d %s\n",WIAC82_SS.AuxStr,InValidity1->InValidity1,DecodeValidity(InValidity1->InValidity1,1));
				sprintf(WIAC82_SS.AuxStr,"%s   InValidity2	       = %d %s\n",WIAC82_SS.AuxStr,InValidity1->InValidity2,DecodeValidity(InValidity1->InValidity2,1));
				sprintf(WIAC82_SS.AuxStr,"%s   CriticalMonitor1    = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValCriticalMonitor1,DecodeValidity(InValidity1->ValCriticalMonitor1,1));
				sprintf(WIAC82_SS.AuxStr,"%s   CriticalMonitor2    = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValCriticalMonitor2,DecodeValidity(InValidity1->ValCriticalMonitor2,1));
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved1	       = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValReserved1,DecodeValidity(InValidity1->ValReserved1,1));
				sprintf(WIAC82_SS.AuxStr,"%s   ProtocolStatus      = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValProtocolStatus,DecodeValidity(InValidity1->ValProtocolStatus,1));
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved2	       = %d %s\n",WIAC82_SS.AuxStr,InValidity1->ValReserved2,DecodeValidity(InValidity1->ValReserved2,1));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  InValidity2      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.InValidity2);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved2	       = %d %s\n",WIAC82_SS.AuxStr,InValidity2->ValReserved2,DecodeValidity(InValidity2->ValReserved2,1));
				sprintf(WIAC82_SS.AuxStr,"%s   CRC                 = %d %s\n",WIAC82_SS.AuxStr,InValidity2->ValCRC,DecodeValidity(InValidity2->ValCRC,1));
				sprintf(WIAC82_SS.AuxStr,"%s   NU                  = %d %s\n",WIAC82_SS.AuxStr,InValidity2->NU,DecodeValidity(InValidity2->NU,1));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  CriticalMonitor1 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.CriticalMonitor1);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   Released            = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ReleasedR);
				sprintf(WIAC82_SS.AuxStr,"%s   JettisonR           = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->JettisonR);
				sprintf(WIAC82_SS.AuxStr,"%s   CommittedToStoreSeparateR = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->CommittedToStoreSeparateR);
				sprintf(WIAC82_SS.AuxStr,"%s   ArmedR      		   = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ArmedR);
				sprintf(WIAC82_SS.AuxStr,"%s   PresetArmingR       = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->PresetArmingR);
				sprintf(WIAC82_SS.AuxStr,"%s   SelectStoreR	       = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->SelectStoreR);
				sprintf(WIAC82_SS.AuxStr,"%s   InitiateIBitR       = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->InitiateIBitR);
				sprintf(WIAC82_SS.AuxStr,"%s   LaunchMode          = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->LaunchModeR);

				sprintf(WIAC82_SS.AuxStr,"%s   ReleaseD10          = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ReleaseD10);		// Fire, Launch and Release
				sprintf(WIAC82_SS.AuxStr,"%s   JettisonD9          = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->JettisonD9);
				sprintf(WIAC82_SS.AuxStr,"%s   CommitToSeperateD8  = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->CommitToSeperateD8);
				sprintf(WIAC82_SS.AuxStr,"%s   ExecuteArmingD7     = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ExecuteArmingD7);
				sprintf(WIAC82_SS.AuxStr,"%s   PreSetArmingD6      = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->PreSetArmingD6);
				sprintf(WIAC82_SS.AuxStr,"%s   SelectStoreD5       = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->SelectStoreD5);
				sprintf(WIAC82_SS.AuxStr,"%s   InitiateIBitD4      = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->InitiateIBitD4);
				sprintf(WIAC82_SS.AuxStr,"%s   ReleaseModeD3       = %d\n",WIAC82_SS.AuxStr,CriticalMonitor1->ReleaseModeD3);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  CriticalMonitor2 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.CriticalMonitor2);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   EraseCommand	            = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->EraseCommand);
				sprintf(WIAC82_SS.AuxStr,"%s   RFjamCommand	            = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->RFjamCommand);
				sprintf(WIAC82_SS.AuxStr,"%s   RFemissionCommand        = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->RFemissionCommand);
				sprintf(WIAC82_SS.AuxStr,"%s   ControlSurfaceInhibit    = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->ControlSurfaceInhibit);
				sprintf(WIAC82_SS.AuxStr,"%s   AbortRelease	            = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->AbortRelease);
				sprintf(WIAC82_SS.AuxStr,"%s   ActivateReleaseFunctions = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->ActivateReleaseFunctions);
				sprintf(WIAC82_SS.AuxStr,"%s   GlobalDeletionReport     = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->GlobalDeletionReport);
				sprintf(WIAC82_SS.AuxStr,"%s   CommandedStoreState      = %d\n",WIAC82_SS.AuxStr,CriticalMonitor2->CommandedStoreState);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved1        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.Reserved1);
			sprintf(WIAC82_SS.AuxStr,"%s  ProtocolStatus   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.ProtocolStatus);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   NoCommandWordCount      = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->NoCommandWordCount);
				sprintf(WIAC82_SS.AuxStr,"%s   IllegalHeaderSubAddress = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->IllegalHeaderSubAddress);
				sprintf(WIAC82_SS.AuxStr,"%s   CheckSumError           = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->CheckSumError);
				sprintf(WIAC82_SS.AuxStr,"%s   CriticalControl1Error   = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->CriticalControl1Error);
				sprintf(WIAC82_SS.AuxStr,"%s   CriticalControl2Error   = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->CriticalControl2Error);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved                = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->Reserved);
				sprintf(WIAC82_SS.AuxStr,"%s   SubAddressOfError       = %d\n",WIAC82_SS.AuxStr,ProtocolStatus->SubAddressOfError);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved2=",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_STORE_MONITOR_REPORT_RESERVED2_WORDS;Count++) {
				sprintf(WIAC82_SS.AuxStr,"%s[0x%2.2x]",	WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.Reserved2[Count]);
				if (Count==((WIAC82_STORE_MONITOR_REPORT_RESERVED2_WORDS/2)-1)) {
					sprintf(WIAC82_SS.AuxStr,"%s\n            ",	WIAC82_SS.AuxStr);
				}
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC              = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.MonitorRep.Stores.CRC);
		}
	}
}
/**/
void WIAC82_SendFunctionalExchange3M_17T(void){
	sWIAC82_FunctionalExchange3MValidity *Validity3M;
	int Count;

	WIAC82_MLAN_SetHeader(&WIAC82_SS.Function3M.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_FunctionalExchange3M_Message), &WIAC82_SS.TxCounters.FunctionalExchane3MMsgCnt);
	WIAC82_MLAN_Set1553Header(&WIAC82_SS.Function3M.MIL1553Header, WIAC82_SS.ServiceControls.RTAddr ,WIAC82_FUNCTIONAL_EXCHANGE_3M_MESSAGE_17T);
	// Word 1
	WIAC82_SS.Function3M.Funct3M.Identifier				= WIAC82_FUNCTIONAL_EXCHANGE_3M_HEADER;
	// Word 2
	WIAC82_SS.Function3M.Funct3M.UsefulLength			= WIAC82_FUNCTIONAL_EXCHANGE_3M_USEFUL_LENGTH;

	// Functional Exchange 3M Validity Word 3
	Validity3M = (sWIAC82_FunctionalExchange3MValidity *)&WIAC82_SS.Function3M.Funct3M.Validity;
	Validity3M->ImpactHeading	= 0; // Logical ICD Data Item Number 66 (Only AT) - INVALID Always
	Validity3M->ImpactSpeed		= 0; // Logical ICD Data Item Number 67 (Only AT) - INVALID Always
	Validity3M->Reserved 		= 0;

	// Word 4
	WIAC82_SS.Function3M.Funct3M.ImpactHeading  = 0; // Set value to default (Only AT)
	// Word 5
	WIAC82_SS.Function3M.Funct3M.ImpactSpeed    = 0; // Set value to default (Only AT)

	// Reserved Words 6..9
	for (Count=0;Count<WIAC82_FUNCTIONAL_EXCHANGE_3M_RESERVED_SIZE;Count++) {
		WIAC82_SS.Function3M.Funct3M.Reserved[Count] = 0;
	}
	// CheckSum Word 10
	WIAC82_SS.Function3M.Funct3M.CRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.Function3M.Funct3M.Validity,((sizeof(sWIAC82_1553_FunctionalExchange3M_Message)/2)-3));

	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.Function3M, sizeof(sWIAC82_1553_MLAN_FunctionalExchange3M_Message));

	// Print-out Auxiliary Controls
	if (AUXCTRL_SS.WIAC82Controls.TxAuxFunction3M||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (17T) FUNCTIONAL EXCHANGE 3M sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.FunctionalExchane3MMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.MonitorRep.HeaderMLAN,0);
		WIAC82_Prn1553Header(&WIAC82_SS.MonitorRep.Header1553);
		if (AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier=0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.Identifier);
			sprintf(WIAC82_SS.AuxStr,"%s  Useful Length=%d\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.UsefulLength);
			sprintf(WIAC82_SS.AuxStr,"%s  Validity=0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.Validity);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   ImpactHeading=%d\n",WIAC82_SS.AuxStr,Validity3M->ImpactHeading);
				sprintf(WIAC82_SS.AuxStr,"%s   ImpactSpeed=%d\n",WIAC82_SS.AuxStr,Validity3M->ImpactSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved=%d\n",WIAC82_SS.AuxStr,Validity3M->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  ImpactHeading=0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.ImpactHeading);
			sprintf(WIAC82_SS.AuxStr,"%s  ImpactSpeed=0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.ImpactSpeed);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved= ",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_FUNCTIONAL_EXCHANGE_3M_RESERVED_SIZE;Count++) {
				sprintf(WIAC82_SS.AuxStr,"%s[0x%x]",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC=0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function3M.Funct3M.CRC);
		}
	}
}
/**/
void WIAC82_SendFunctionalExchange1M_23T (void) {
	sWIAC82_FunctionalExchange1MValidity *Validity1M;
	sWIAC82_FunctionalExchange1MWeaponSettings *WIAC82WeaponSettings;

	int Count;
	unsigned char SAL_CodeValid;
	float SAL_Freq;

	WIAC82_MLAN_SetHeader(&WIAC82_SS.Function1M.MLANHeader, WIAC82_MLAN_MCP_NODE_ID, WIAC82_MLAN_ITUC_NODE_ID,	WIAC82_TX_MLAN_WPN_BC_MESSAGE, sizeof(sWIAC82_1553_MLAN_FunctionalExchange1M_Message), &WIAC82_SS.TxCounters.FunctionalExchane1MMsgCnt);
	WIAC82_MLAN_Set1553Header(&WIAC82_SS.Function1M.MIL1553Header, WIAC82_SS.ServiceControls.RTAddr ,WIAC82_FUNCTIONAL_EXCHANGE_1M_MESSAGE_23T);
	// Word 1
	WIAC82_SS.Function1M.Funct1M.Identifier				= WIAC82_FUNCTIONAL_EXCHANGE_1M_HEADER;
	// Word 2
	WIAC82_SS.Function1M.Funct1M.UsefulLength			= WIAC82_FUNCTIONAL_EXCHANGE_1M_USEFUL_LENGTH;
	// Functional Exchange 1M Validity Word 3
	Validity1M = (sWIAC82_FunctionalExchange1MValidity *)&WIAC82_SS.Function1M.Funct1M.Validity;
	Validity1M->Target			= MISS_SS.Control.TargetPositionOK;
	Validity1M->DiveAngle		= MISS_SS.Control.DiveAngleOK;

	if(MISS_SS.WIAC82_Mission.MissionType != seekerSAL)
		Validity1M->SAL_Code 		= 0; // Validity = Invalid
	else
		Validity1M->SAL_Code 		= MISS_SS.Control.SALCodeOK;

	Validity1M->ImageCRC		= 0; // Al Tariq Weapon
	Validity1M->SeekerType		= MISS_SS.Control.MissionTypeOK; // Logical ICD (Data Item Number 71 Validity)
	Validity1M->FuzeSetting		= MISS_SS.WIAC82_Mission.ValidityFlags.FuzeSettings; // Logical ICD (Data Item Number 72 Validity)
	Validity1M->Reserved2		= 0;
	Validity1M->TimeOfFlight	= 1;
	Validity1M->Reserved1		= 0;

	// Word 4
	WIAC82_SS.Function1M.Funct1M.TargetLatitude1 	= WIAC82_SS.Func3D.FuncExch.TargetLatitude1;
	// Word 5
	WIAC82_SS.Function1M.Funct1M.TargetLatitude2 	= WIAC82_SS.Func3D.FuncExch.TargetLatitude2;
	// Word 6
	WIAC82_SS.Function1M.Funct1M.TargetLongitude1 	= WIAC82_SS.Func3D.FuncExch.TargetLongitude1;
	// Word 7
	WIAC82_SS.Function1M.Funct1M.TargetLongitude2 	= WIAC82_SS.Func3D.FuncExch.TargetLongitude2;
	// Word 8
	WIAC82_SS.Function1M.Funct1M.TargetAltitude1 	= WIAC82_SS.Func3D.FuncExch.TargetAltitude1;
	// Word 9
	WIAC82_SS.Function1M.Funct1M.TargetAltitude2 	= WIAC82_SS.Func3D.FuncExch.TargetAltitude2;
	// Word 10
	WIAC82_SS.Function1M.Funct1M.FuzeDelay 			= MISS_SS.WIAC82_Mission.FuzeDelay;
	// Word 11
	WIAC82_SS.Function1M.Funct1M.FuzeHeight 		= MISS_SS.WIAC82_Mission.FuzingDistance;
	// Guidance Information Word 12
	WIAC82_SS.Function1M.Funct1M.GuidanceInformation = EncodeDiveAngle(MISS_SS.WIAC82_Mission.DiveAngle);
	// Word 13
	WIAC82_SS.Function1M.Funct1M.SALCode 			= MISS_SS.WIAC82_Mission.SAL_Code;

	// Weapon Settings Word 14
	WIAC82WeaponSettings = (sWIAC82_FunctionalExchange1MWeaponSettings *)&WIAC82_SS.Function1M.Funct1M.WeaponSettings;
	WIAC82WeaponSettings->ImageChecksum = WIAC82_FUNCTIONAL_EXCHANGE_1M_IMAGE_CHECKSUM_NO_IMAGE;
	if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
		WIAC82WeaponSettings->SeekerType = WIAC82_FUNCTIONAL_EXCHANGE_1M_SEEKER_TYPE_SAL;
	else
		WIAC82WeaponSettings->SeekerType = WIAC82_FUNCTIONAL_EXCHANGE_1M_SEEKER_TYPE_GNSS;

	if (MISS_SS.WIAC82_Mission.UpdatingFuze)
	{
		WIAC82WeaponSettings->FuzeSettings = WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_IN_PROGRAMMING;
	} else {
		WIAC82WeaponSettings->FuzeSettings = WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_IMPACT;
		if (Weapon.Mission.FuzeMode == fmNON_PROGRAMMABLE)
			WIAC82WeaponSettings->FuzeSettings = WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_IMPACT;
		if (Weapon.Mission.FuzeMode == fmDELAY)
			WIAC82WeaponSettings->FuzeSettings = WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_DELAY;
		if (Weapon.Mission.FuzeMode == fmHEIGHT)
			WIAC82WeaponSettings->FuzeSettings = WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_AIRBURST;
	}

	WIAC82WeaponSettings->Reserved = 0;

	// Time of Flight Word 15

	WIAC82_SS.Function1M.Funct1M.TimeOfFlight = 0;

	// Reserved Words 16..30
	for (Count=0;Count<WIAC82_FUNCTIONAL_EXCHANGE_1M_RESERVED_SIZE;Count++) {
		WIAC82_SS.Function1M.Funct1M.Reserved[Count] = 0;
	}
	// CheckSum Word 31
	WIAC82_SS.Function1M.Funct1M.CRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.Function1M.Funct1M.Validity,((sizeof(sWIAC82_1553_FunctionalExchange1M_Message)/2)-3));

	MLAN_Send(WIAC82_MLAN_ITUC_NODE_ID,  (uint8_t *) &WIAC82_SS.Function1M, sizeof(sWIAC82_1553_MLAN_FunctionalExchange1M_Message));

	if (AUXCTRL_SS.WIAC82Controls.MissionData) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (23T) FUNCTIONAL EXCHANGE 1M \n",WIAC82_SS.AuxStr,GetTimeUs());
		sprintf(WIAC82_SS.AuxStr,"%s Target Latitude %8s [turn] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetLatitude1,FLOAT_2POWER_32)),Validity1M->Target);
		sprintf(WIAC82_SS.AuxStr,"%s Target Longitude %8s [turn] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetLongitude1,FLOAT_2POWER_32)),Validity1M->Target);
		sprintf(WIAC82_SS.AuxStr,"%s Target Altitude %8s [m] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetAltitude1,FLOAT_2POWER_16)),Validity1M->Target);
		sprintf(WIAC82_SS.AuxStr,"%s Fuze Delay %8s [ms] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Function1M.Funct1M.FuzeDelay,1)),Validity1M->FuzeSetting);
		sprintf(WIAC82_SS.AuxStr,"%s Fuze Height %8s [m] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Function1M.Funct1M.FuzeHeight,1)),Validity1M->FuzeSetting);
		sprintf(WIAC82_SS.AuxStr,"%s Dive Angle  %8s [turn] Valid: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Function1M.Funct1M.GuidanceInformation,FLOAT_2POWER_17)),Validity1M->DiveAngle);
		SAL_CodeValid = SAL_GetFrequency(&SAL_Freq,WIAC82_SS.Function1M.Funct1M.SALCode);
		sprintf(WIAC82_SS.AuxStr,"%s SAL Code  %8s [Hz] Valid = %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(SAL_Freq),SAL_CodeValid);
	}

	// Print-out Auxiliary Controls
	if (AUXCTRL_SS.WIAC82Controls.TxAuxFunction1M||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 TX (23T) FUNCTIONAL EXCHANGE 1M sent to WIAC82 - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.FunctionalExchane1MMsgCnt);
		WIAC82_PrnMLANHeader(&WIAC82_SS.MonitorRep.HeaderMLAN,0);
		WIAC82_Prn1553Header(&WIAC82_SS.MonitorRep.Header1553);
		if (AUXCTRL_SS.WIAC82Controls.TxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier       =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.Identifier);
			sprintf(WIAC82_SS.AuxStr,"%s  Useful Length    =  %4d\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.UsefulLength);
			sprintf(WIAC82_SS.AuxStr,"%s  Validity         =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.Validity);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   TargetValid      =%d\n",WIAC82_SS.AuxStr,Validity1M->Target);
				sprintf(WIAC82_SS.AuxStr,"%s   DiveAngleValid   =%d\n",WIAC82_SS.AuxStr,Validity1M->DiveAngle);
				sprintf(WIAC82_SS.AuxStr,"%s   SAL_CodeValid    =%d\n",WIAC82_SS.AuxStr,Validity1M->SAL_Code);
				sprintf(WIAC82_SS.AuxStr,"%s   ImageCRCValid    =%d\n",WIAC82_SS.AuxStr,Validity1M->ImageCRC);
				sprintf(WIAC82_SS.AuxStr,"%s   SeekerTypeValid  =%d\n",WIAC82_SS.AuxStr,Validity1M->SeekerType);
				sprintf(WIAC82_SS.AuxStr,"%s   FuzeSettingValid =%d\n",WIAC82_SS.AuxStr,Validity1M->FuzeSetting);
				sprintf(WIAC82_SS.AuxStr,"%s   ReservedValid    =%d\n",WIAC82_SS.AuxStr,Validity1M->Reserved2);
				sprintf(WIAC82_SS.AuxStr,"%s   TimeOfFlightValid=%d\n",WIAC82_SS.AuxStr,Validity1M->TimeOfFlight);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved         =%d\n",WIAC82_SS.AuxStr,Validity1M->Reserved1);
			}
//			sprintf(WIAC82_SS.AuxStr,"%s  Impact Heading   =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.ImpactHeading);
//			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloat(WIAC82_SS.Function1M.Funct1M.ImpactHeading,FLOAT_2POWER_16)));

			sprintf(WIAC82_SS.AuxStr,"%s  TargetLatitude1  =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetLatitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLatitude2  =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetLatitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetLatitude1,FLOAT_2POWER_32)));

			sprintf(WIAC82_SS.AuxStr,"%s  TargetLongitude1 =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetLongitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLongitude2 =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetLongitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetLongitude1,FLOAT_2POWER_32)));

			sprintf(WIAC82_SS.AuxStr,"%s  TargetAltitude1  =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetAltitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetAltitude2  =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TargetAltitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Function1M.Funct1M.TargetAltitude1,FLOAT_2POWER_16)));

			sprintf(WIAC82_SS.AuxStr,"%s  FuzeDelay        =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.FuzeDelay);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [ms]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Function1M.Funct1M.FuzeDelay,1)));

			sprintf(WIAC82_SS.AuxStr,"%s  FuzeHeight       =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.FuzeHeight);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Function1M.Funct1M.FuzeHeight,1)));

			sprintf(WIAC82_SS.AuxStr,"%s  GuidanceInformation   =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.GuidanceInformation);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn] \n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Function1M.Funct1M.GuidanceInformation,FLOAT_2POWER_17)));

			sprintf(WIAC82_SS.AuxStr,"%s  SALCode          =0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.SALCode);
			SAL_CodeValid = SAL_GetFrequency(&SAL_Freq,WIAC82_SS.Function1M.Funct1M.SALCode);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [Hz] Valid = %d\n",WIAC82_SS.AuxStr,AUXsprintfFloat(SAL_Freq),SAL_CodeValid);

			sprintf(WIAC82_SS.AuxStr,"%s  WeaponSettings   =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.WeaponSettings);
			if (AUXCTRL_SS.WIAC82Controls.TxDetail2AuxOn) {
				sprintf(WIAC82_SS.AuxStr,"%s   ImageChecksum   =%d\n",WIAC82_SS.AuxStr,WIAC82WeaponSettings->ImageChecksum);
				sprintf(WIAC82_SS.AuxStr,"%s   SeekerType      =%d\n",WIAC82_SS.AuxStr,WIAC82WeaponSettings->SeekerType);
				sprintf(WIAC82_SS.AuxStr,"%s   FuzeSettings    =%d\n",WIAC82_SS.AuxStr,WIAC82WeaponSettings->FuzeSettings);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved        =%d\n",WIAC82_SS.AuxStr,WIAC82WeaponSettings->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  TimeOfFlight     =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.TimeOfFlight);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved= ",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_FUNCTIONAL_EXCHANGE_1M_RESERVED_SIZE;Count++) {
				sprintf(WIAC82_SS.AuxStr,"%s[0x%x]",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC              =0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Function1M.Funct1M.CRC);
		}
	}
}
int WIAC82_ServiceSBIT_Req(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	if (WIAC82_SS.CommsControls.SBIT_NotSent) {
		WIAC82_SendSBIT_Request();
		if (AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 SBIT Request Message sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),(int)WIAC82_SS.TxCounters.SBITReqMsgCnt);
		}
		WIAC82_SS.CommsControls.SBIT_NotSent = 0;
		WIAC82_SS.ServiceControls.SBIT_Service.StartTime = GetTimeUs();
		MessageNotSent = 0;
	}
	else if ((WIAC82_SS.CommsControls.SBIT_Received == 0)||(WIAC82_SS.TxCounters.SBITReqMsgCnt<10)) {
		WIAC82_SS.ServiceControls.SBIT_Service.TimeNow = GetTimeUs();
		WIAC82_SS.ServiceControls.SBIT_Service.TimeDiff = WIAC82_SS.ServiceControls.SBIT_Service.TimeNow - WIAC82_SS.ServiceControls.SBIT_Service.StartTime;
		if (WIAC82_SS.ServiceControls.SBIT_Service.TimeDiff > WIAC82_SBIT_SERVICE_INTERVAL) {
			WIAC82_SS.ServiceControls.SBIT_Service.StartTime = GetTimeUs();
			WIAC82_SendSBIT_Request();
			MessageNotSent = 0;
		}
	}
	return(MessageNotSent);

}
int WIAC82_ServiceStatusReq(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	WIAC82_SS.ServiceControls.Status_Service.TimeNow = GetTimeUs();
	WIAC82_SS.ServiceControls.Status_Service.TimeDiff = WIAC82_SS.ServiceControls.Status_Service.TimeNow - WIAC82_SS.ServiceControls.Status_Service.StartTime;
	if (WIAC82_SS.ServiceControls.Status_Service.TimeDiff > WIAC82_STATUS_SERVICE_INTERVAL) {
		WIAC82_SS.ServiceControls.Status_Service.StartTime = GetTimeUs();
		WIAC82_SendStatus_Request();
		MessageNotSent = 0;
	}
	return(MessageNotSent);
}
int WIAC82_ServiceStoreDescriptionReport1T(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	if (SS_FUZE.Fuze.InitComplete) {
		if (WIAC82_SS.TxCounters.StoresDescriptionMsgCnt<30){
			WIAC82_SS.ServiceControls.StoreDescription1T.TimeNow = GetTimeUs();
			WIAC82_SS.ServiceControls.StoreDescription1T.TimeDiff = WIAC82_SS.ServiceControls.StoreDescription1T.TimeNow - WIAC82_SS.ServiceControls.StoreDescription1T.StartTime;
			if (WIAC82_SS.ServiceControls.StoreDescription1T.TimeDiff > WIAC82_STORE_DESCRIPTION_1T_SERVICE_INTERVAL) {
				WIAC82_SS.ServiceControls.StoreDescription1T.StartTime = GetTimeUs();
				WIAC82_SendStoreDescriptionReport1T();
				MessageNotSent = 0;
			}
		}
	}
	return(MessageNotSent);
}

int WIAC82_ServiceWeaponStatusWord8T(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	WIAC82_SS.ServiceControls.WeaponStatusWord8T.TimeNow = GetTimeUs();
	WIAC82_SS.ServiceControls.WeaponStatusWord8T.TimeDiff = WIAC82_SS.ServiceControls.WeaponStatusWord8T.TimeNow - WIAC82_SS.ServiceControls.WeaponStatusWord8T.StartTime;
	if (WIAC82_SS.ServiceControls.WeaponStatusWord8T.TimeDiff > WIAC82_WEAPON_STATUS_WORD_8T_SERVICE_INTERVAL) {
		WIAC82_SS.ServiceControls.WeaponStatusWord8T.StartTime = GetTimeUs();
		WIAC82_SendWeaponStatusWord8T();
		MessageNotSent = 0;
	}
	return(MessageNotSent);
}
int WIAC82_ServiceStoreMonitorReport11T(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	WIAC82_SS.ServiceControls.StoreMonitorReport11T.TimeNow = GetTimeUs();
	WIAC82_SS.ServiceControls.StoreMonitorReport11T.TimeDiff = WIAC82_SS.ServiceControls.StoreMonitorReport11T.TimeNow - WIAC82_SS.ServiceControls.StoreMonitorReport11T.StartTime;
	if (WIAC82_SS.ServiceControls.StoreMonitorReport11T.TimeDiff > WIAC82_WEAPON_STORE_MONITOR_REPORT_11T_SERVICE_INTERVAL) {
		WIAC82_SS.ServiceControls.StoreMonitorReport11T.StartTime = GetTimeUs();
		WIAC82_SendStoreMonitorReport11T();
		MessageNotSent = 0;
	}
	return(MessageNotSent);
}
int WIAC82_ServiceFunctionalExchange3M_17T(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	if (WIAC82_SS.CommsControls.Trigger17T) {
		WIAC82_SS.CommsControls.Trigger17T = 0;
		WIAC82_SendFunctionalExchange3M_17T();
		MessageNotSent = 0;
	}
/*
	WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.TimeNow = GetTimeUs();
	WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.TimeDiff = WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.TimeNow - WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.StartTime;
	if (WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.TimeDiff > WIAC82_WEAPON_FUNCTIONAL_EXCHANGE_3M_17T_SERVICE_INTERVAL) {
		WIAC82_SS.ServiceControls.FunctionalExchange3M_17T.StartTime = GetTimeUs();
		WIAC82_SendFunctionalExchange3M_17T();
		MessageNotSent = 0;
	}
*/
	return(MessageNotSent);
}
int WIAC82_ServiceFunctionalExchange1M_23T(void)
{
	int MessageNotSent;

	MessageNotSent = 1;
	if (WIAC82_SS.CommsControls.Trigger23T) {
		WIAC82_SS.CommsControls.Trigger23T = 0;
		WIAC82_SendFunctionalExchange1M_23T();
		MessageNotSent = 0;
	}
/*
	WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.TimeNow = GetTimeUs();
	WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.TimeDiff = WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.TimeNow - WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.StartTime;
	if (WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.TimeDiff > WIAC82_WEAPON_FUNCTIONAL_EXCHANGE_1M_23T_SERVICE_INTERVAL) {
		WIAC82_SS.ServiceControls.FunctionalExchange1M_23T.StartTime = GetTimeUs();
		WIAC82_SendFunctionalExchange1M_23T();
		MessageNotSent = 0;
	}
*/
	return(MessageNotSent);
}

real_T CalculateLatency(TIME Latency){

	real_T calculatedLatency, latencyAux, SystemAux, OffsetAux;

	// All values must be real before calculation (Unsigned Long Long issue)
	latencyAux = Latency;
	SystemAux = Weapon.SystemTime;
	OffsetAux = WIAC82_SS.OffsetTimeLatency;

	calculatedLatency = (real_T) (((SystemAux + OffsetAux) - latencyAux) * 0.000001);

	return calculatedLatency;
}

int WIAC82_ServiceTransferAlignment()
{
  int MessageNotSent;
  float altitude;
  T_RTAC_TransferAlignment ta;

  MessageNotSent = 1;
  ta.ValidityWord = 0x7FFF; //WIAC82_SS.TransferAlignmentData.Validity.DWord; (RTDP is not sending right values, hardcoded for now)
  ta.Roll = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Roll,FLOAT_2POWER_15);
  ta.Pitch = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Pitch,FLOAT_2POWER_16);
  ta.Heading = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Yaw,FLOAT_2POWER_15);
  ta.RollRate = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.RollRate,FLOAT_2POWER_13);
  ta.PitchRate = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.PitchRate,FLOAT_2POWER_14);
  ta.YawRate = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.YawRate,FLOAT_2POWER_14);
  ta.VelocityNorth = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityNorth,FLOAT_10POWER_2);
  ta.VelocityEast = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityEast,FLOAT_10POWER_2);
  ta.VelocityDown = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityDown,FLOAT_10POWER_2);
  ta.Latitude = (double)GetFloatInt(WIAC82_SS.TransferAlignmentData.Latitude,FLOAT_2POWER_32);
  ta.Longitude = (double)GetFloatInt(WIAC82_SS.TransferAlignmentData.Longitude,FLOAT_2POWER_31);
  ta.Altitude = Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Altitude,FLOAT_2POWER_0);
  ta.LeverArmX = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmX,FLOAT_2POWER_10);
  ta.LeverArmY = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmY,FLOAT_2POWER_10);
  ta.LeverArmZ = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmZ,FLOAT_2POWER_10);
  ta.Latency1 = CalculateLatency(WIAC82_SS.AttitudeLatency); 	// Attitude Latency
  ta.Latency2 = CalculateLatency(WIAC82_SS.RatesLatency); 		// Rate Latency
  ta.Latency3 = CalculateLatency(WIAC82_SS.VelocityLatency);	// Velocity Latency
  ta.Latency4 = CalculateLatency(WIAC82_SS.PositionLatency); 	// Position Latency
  ta.TrueAirspeed = Get1WordFloatU(WIAC82_SS.TransferAlignmentData.TrueAirspeed,FLOAT_10POWER_2);
  ta.CalibratedAirspeed = Get1WordFloatU(WIAC82_SS.TransferAlignmentData.CalibratedAirspeed,FLOAT_10POWER_2);
  ta.StaticPressure = MCP.PressSensPressure;
  ta.AngleOfAttack = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.AngleOfAttack,FLOAT_2POWER_16);
  ta.AngleOfSideslip = Get1WordFloatS(WIAC82_SS.TransferAlignmentData.AngleOfSideslip,FLOAT_2POWER_16);

  // Convert Altitude in EGM96 to WGS84
  if (GetEGM96GeoidHeight(&altitude, ta.Latitude, ta.Longitude)) {
    ta.Altitude = ta.Altitude + altitude;
  }

  NavStart50Hz(&ta);

  /*static int NotAligned;

  if (!NotAligned){
	  NotAligned = 1;
	  NavForceFullNav(0);
  } */

  WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg14RPacked = FALSE;
  WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg16RPacked = TRUE;
  WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg21RPacked = TRUE;

  MessageNotSent = 0;
  return(MessageNotSent);
}

void WIAC82_Send(void)
{
	int SendAvailable;

	SendAvailable = WIAC82_ServiceSBIT_Req();

	// SBIT & STATUS SERVICE INTERVAL
	if (((Weapon.SystemTime - WIAC82_SS.TxTimestamp) >= 300000) && (BIT.SBIT_Complete)) {
		WIAC82_SS.TxTimestamp = Weapon.SystemTime;
	    if (WIAC82_SS.WIAC82WaitForRxMsg){
	    	WIAC82_SS.ErrCnt++;

			if (AUXCTRL_SS.WIAC82Controls.ErrDetectOn) {
				AUXsprintf("%u:WIAC82 Receive Message Timeout ERROR - %d \n",GetTimeUs(),WIAC82_SS.ErrCnt);
			}
	    }
	}

	if (WIAC82_SS.CommsControls.SBIT_Received == 1) // WIAC SBIT must be completed
	{
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceStatusReq();
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceStoreDescriptionReport1T();
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceWeaponStatusWord8T();
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceStoreMonitorReport11T();
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceFunctionalExchange3M_17T();
		if (SendAvailable)
			SendAvailable = WIAC82_ServiceFunctionalExchange1M_23T();
		if (SendAvailable)
			if(WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg14RPacked) //&& WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg16RPacked &&
					//WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg21RPacked)
				SendAvailable = WIAC82_ServiceTransferAlignment();
	}

/*
	int Cycle;

	WIAC82_SS.ServiceControls.SystemTime = GetTimeUs();
	WIAC82_SS.ServiceControls.TimeDiff = WIAC82_SS.ServiceControls.SystemTime - WIAC82_SS.ServiceControls.OldSystemTime;
	if ((WIAC82_SS.ServiceControls.TimeDiff > WIAC82_TX_SERVICE)||(WIAC82_SS.CommsControls.StartupCycle)) {
		WIAC82_SS.ServiceControls.OldSystemTime = WIAC82_SS.ServiceControls.SystemTime;
		if (AUXCTRL_SS.WIAC82Controls.SchedulerAuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s%u:Servicing WIAC82 Scheduled Transmitter %d - %d\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.TxCounters.TxServiceCnt,WIAC82_SS.CommsControls.StartupCycle);
		}
		WIAC82_SS.CommsControls.StartupCycle = 0;
		WIAC82_SS.TxCounters.TxServiceCnt++;
		Cycle = WIAC82_SS.TxCounters.TxServiceCnt & 0x7;
		switch (Cycle) {
			case 0:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxSBITReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 SBIT Request Message sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				WIAC82_SendSBIT_Request();
		    	break;
			case 1:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxStatusReq||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 STATUS Request Message sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				WIAC82_SendStatus_Request();
				break;
			case 2:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxStoresRep||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 (1T) STORES DESCRIPTION REPORT MESSAGE sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				WIAC82_SendStoreDescriptionReport();
				break;
			case 3:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxStatusWord||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 (8T) STATUS WORD MESSAGE sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				WIAC82_SendWeaponStatusWord();
				break;
			case 4:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxFunction3M||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 (17T) FUNCTIONAL EXCHANGE 3M MESSAGE sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				break;
			case 5:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxMonitorRep||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 (11T) STORE MONITOR REPORT MESSAGE sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				WIAC82_SendStoreMonitorReport();
				break;
			case 6:
				if (AUXCTRL_SS.WIAC82Controls.TxAuxFunction1M||AUXCTRL_SS.WIAC82Controls.TxAuxOn) {
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 (23T) FUNCTIONAL EXCHANGE 1M MESSAGE sent to WIAC82 %d\n",WIAC82_SS.AuxStr,GetTimeUs(),Cycle);
				}
				break;
		    default : break;
		}
	}
*/
}
void WIAC82_MisfiredWeapon(void)
{
	  ClearMission(msRTAC);

  	  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
  		  AUXsprintf("%u:Weapon ---- MISFIRED ----\n",GetTimeUs());

  	  WIAC82_SendBatteryDeactivation();
  	  WIAC82_SS.WIAC82ExecuteArming = FALSE; // To avoid activation again of the weapon after Standby
}

void WIAC82_HandleSBITmsg (unsigned char *RxData, unsigned int Timestamp){
	sWIAC82_SBIT_Msg *SBIT_Message;

	SBIT_Message = (sWIAC82_SBIT_Msg *)RxData;
	WIAC82_SS.SBIT_Message.SerialNo			  = SBIT_Message->SerialNo;
	WIAC82_SS.SBIT_Message.LoaderVer		  = SBIT_Message->LoaderVer;
	WIAC82_SS.SBIT_Message.LoaderCRC		  = SBIT_Message->LoaderCRC;
	WIAC82_SS.SBIT_Message.AppVer			  = SBIT_Message->AppVer;
	WIAC82_SS.SBIT_Message.AppCRC			  = SBIT_Message->AppCRC;
	WIAC82_SS.SBIT_Message.RTAddr			  = SBIT_Message->RTAddr;
	WIAC82_SS.SBIT_Message.Spare1			  = SBIT_Message->Spare1;
	WIAC82_SS.SBIT_Message.Spare2			  = SBIT_Message->Spare2;
	WIAC82_SS.SBIT_Message.SBIT.Bits.ErrBusy  = SBIT_Message->SBIT.Bits.ErrBusy;
	WIAC82_SS.SBIT_Message.SBIT.Bits.ErrSpare = SBIT_Message->SBIT.Bits.ErrSpare;
	WIAC82_SS.SBIT_Message.SBIT.Bits.ParErr   = SBIT_Message->SBIT.Bits.ParErr;
	WIAC82_SS.SBIT_Message.SBIT.Bits.WrnBusy  = SBIT_Message->SBIT.Bits.WrnBusy;
	WIAC82_SS.SBIT_Message.SBIT.Bits.WrnSpare = SBIT_Message->SBIT.Bits.WrnSpare;
	WIAC82_SS.SBIT_Message.Spare3			  = SBIT_Message->Spare3;

	WIAC82_SS.CommsControls.SBIT_Received = 1;
	WIAC82_SS.RTAddr = WIAC82_SS.SBIT_Message.RTAddr;

	if (AUXCTRL_SS.WIAC82Controls.RxAuxSBIT||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 SBIT Message RECEIVED from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs());
		WIAC82_PrnMLANHeader(&SBIT_Message->MLANHeader,1);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s SerialNo    = %8.8lu \n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SerialNo);
			sprintf(WIAC82_SS.AuxStr,"%s LoaderVer   = %d \n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.LoaderVer);
			sprintf(WIAC82_SS.AuxStr,"%s LoaderCRC   = 0x%4.4x \n",WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.LoaderCRC);
			sprintf(WIAC82_SS.AuxStr,"%s AppVer      = %d \n",		WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.AppVer);
			sprintf(WIAC82_SS.AuxStr,"%s AppCRC      = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.AppCRC);
			sprintf(WIAC82_SS.AuxStr,"%s RTAddr      = 0x%4.4x %d\n",WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.RTAddr,WIAC82_SS.SBIT_Message.RTAddr);
			sprintf(WIAC82_SS.AuxStr,"%s Spare1      = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.Spare1);
			sprintf(WIAC82_SS.AuxStr,"%s Spare2      = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.Spare2);
			sprintf(WIAC82_SS.AuxStr,"%s ErrBusy     = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SBIT.Bits.ErrBusy);
			sprintf(WIAC82_SS.AuxStr,"%s ErrSpare    = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SBIT.Bits.ErrSpare);
			sprintf(WIAC82_SS.AuxStr,"%s ParErr      = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SBIT.Bits.ParErr);
			sprintf(WIAC82_SS.AuxStr,"%s WrnBusy     = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SBIT.Bits.WrnBusy);
			sprintf(WIAC82_SS.AuxStr,"%s WrnSpare    = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.SBIT.Bits.WrnSpare);
			sprintf(WIAC82_SS.AuxStr,"%s Spare3      = 0x%4.4x\n",	WIAC82_SS.AuxStr,WIAC82_SS.SBIT_Message.Spare3);
		}
	}
}
void WIAC82_HandleSTATUSmsg (unsigned char *RxData, unsigned int Timestamp){
	sWIAC82_StatusMsg *STATUS_Message;

	STATUS_Message = (sWIAC82_StatusMsg *)RxData;
	WIAC82_SS.STATUS_Message.Temperature							=	STATUS_Message->Temperature;
	WIAC82_SS.STATUS_Message.ActivationCnt							=	STATUS_Message->ActivationCnt;
	WIAC82_SS.STATUS_Message.ArcnetRxMsgErrCnt  					=	STATUS_Message->ArcnetRxMsgErrCnt;
	WIAC82_SS.STATUS_Message.ArcnetTxMsgErrCnt						=	STATUS_Message->ArcnetTxMsgErrCnt;
	WIAC82_SS.STATUS_Message.ArcnetMyReconCnt						=	STATUS_Message->ArcnetMyReconCnt;
	WIAC82_SS.STATUS_Message.NotUsed1								=	STATUS_Message->NotUsed1;
	WIAC82_SS.STATUS_Message.NotUsed2								=	STATUS_Message->NotUsed2;
	WIAC82_SS.STATUS_Message.NotUsed3								=	STATUS_Message->NotUsed3;
	WIAC82_SS.STATUS_Message.NotUsed4								=	STATUS_Message->NotUsed4;
	WIAC82_SS.STATUS_Message.NotUsed5								=	STATUS_Message->NotUsed5;
	WIAC82_SS.STATUS_Message.NotUsed6								=	STATUS_Message->NotUsed6;
	WIAC82_SS.STATUS_Message.FuzeId									=	STATUS_Message->FuzeId;
	WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.Address			=	STATUS_Message->MIL1553_Discretes.Bits.Address;
	WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.AddressParity	=	STATUS_Message->MIL1553_Discretes.Bits.AddressParity;
	WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.ReleaseConsent	=	STATUS_Message->MIL1553_Discretes.Bits.ReleaseConsent;
	WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.WpnActivate		=	STATUS_Message->MIL1553_Discretes.Bits.WpnActivate;
	WIAC82_SS.STATUS_Message.Spare									=	STATUS_Message->Spare;

	WIAC82_SS.WIAC82ReleaseConsent = WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.ReleaseConsent;

	if(WIAC82_SS.PreviousReleaseConsent != WIAC82_SS.WIAC82ReleaseConsent) {
		if (WIAC82_SS.WIAC82ReleaseConsent) {
			// Weapon must be in Armed Mode to proceed with Battery Activation with the Release Consent Command
			if (Weapon.Flags.Bits.WeaponArmed) {
				if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- RELEASE CONSENT ---------- \n",WIAC82_SS.AuxStr,GetTimeUs());
				WIAC82_SendExecuteArmingBattery();
				WIAC82_SS.PreviousReleaseConsent = WIAC82_SS.WIAC82ReleaseConsent;
			} else {
				if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- RELEASE ACTIVATED WITHOUT BEING ARMED ---------- \n",WIAC82_SS.AuxStr,GetTimeUs());
			}
		} else {
			if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
				sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- RELEASE DEACTIVATED ---------- \n",WIAC82_SS.AuxStr,GetTimeUs());
			WIAC82_SS.PreviousReleaseConsent = WIAC82_SS.WIAC82ReleaseConsent;
		}
	}

	if (AUXCTRL_SS.WIAC82Controls.RxAuxStatus||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 STATUS Message RECEIVED from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs());
		WIAC82_PrnMLANHeader(&STATUS_Message->MLANHeader,1);
		if (AUXCTRL_SS.WIAC82Controls.RxDetailAuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s Temperature=%d \n",			WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.Temperature);
			sprintf(WIAC82_SS.AuxStr,"%s ActivationCnt=%d \n",			WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.ActivationCnt);
			sprintf(WIAC82_SS.AuxStr,"%s ArcnetRxMsgErrCnt=%d \n",		WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.ArcnetRxMsgErrCnt);
			sprintf(WIAC82_SS.AuxStr,"%s ArcnetTxMsgErrCnt=%d \n",		WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.ArcnetTxMsgErrCnt);
			sprintf(WIAC82_SS.AuxStr,"%s ArcnetMyReconCnt=%d \n",		WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.ArcnetMyReconCnt);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed1=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed1);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed2=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed2);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed3=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed3);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed4=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed4);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed5=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed5);
			sprintf(WIAC82_SS.AuxStr,"%s NotUsed6=%d \n",				WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.NotUsed6);
			sprintf(WIAC82_SS.AuxStr,"%s FuzeId=%d \n",					WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.FuzeId);
			sprintf(WIAC82_SS.AuxStr,"%s MIL1553 Address=%d \n",		WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.Address);
			sprintf(WIAC82_SS.AuxStr,"%s MIL1553 AddressParity=%d \n",	WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.AddressParity);
			sprintf(WIAC82_SS.AuxStr,"%s MIL1553 ReleaseConsent=%d \n",	WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.ReleaseConsent);
			sprintf(WIAC82_SS.AuxStr,"%s MIL1553 WpnActivate=%d \n",	WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.MIL1553_Discretes.Bits.WpnActivate);
			sprintf(WIAC82_SS.AuxStr,"%s Spare=%d \n",					WIAC82_SS.AuxStr,WIAC82_SS.STATUS_Message.Spare);
		}
	}
}
char *GetStationNumber(uint16_t StationNumber){
	static char StationName[20];
	if (StationNumber == 0x8) {
		sprintf(StationName,"Center Line");
	}
	else if (StationNumber == 0xD){
		sprintf(StationName,"Left Wing (P2G)");
	}
	else if (StationNumber == 0xE){
		sprintf(StationName,"Right Wing (P2D)");
	}
	else {
		sprintf(StationName,"Invalid");
	}
	return(StationName);
}
char *GetAdaptorIdentity(uint16_t AdaptorID){
	static char AdaptorIdentity[20];
	if (AdaptorID == 1) {
		sprintf(AdaptorIdentity,"CLB900");
	}
	else if (AdaptorID == 2){
		sprintf(AdaptorIdentity,"AT730");
	}
	else {
		sprintf(AdaptorIdentity,"Invalid");
	}
	return(AdaptorIdentity);
}
char *GetAdaptorPosition(uint16_t AdaptorPos){
	static char AdaptorPosition[20];
	if (AdaptorPos == 1) {
		sprintf(AdaptorPosition,"Center");
	}
	else if (AdaptorPos == 2){
		sprintf(AdaptorPosition,"Left");
	}
	else if (AdaptorPos == 3){
		sprintf(AdaptorPosition,"Right");
	}
	else {
		sprintf(AdaptorPosition,"Invalid");
	}
	return(AdaptorPosition);
}

char *GetPositionAdaptorConfig(uint16_t AdaptorPos){
	static char AdaptorPosition[25];
	if (AdaptorPos == 1) {
		sprintf(AdaptorPosition,"Center Line Fuselage");
	}
	else if (AdaptorPos == 2){
		sprintf(AdaptorPosition,"Wing 2 Left");
	}
	else if (AdaptorPos == 3){
		sprintf(AdaptorPosition,"Wing 2 Right");
	}
	else {
		sprintf(AdaptorPosition,"Invalid");
	}
	return(AdaptorPosition);
}

void WIAC82_HandleAC_DescriptionMsg1R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Aircraft_Description_Message *AC_Description;
	sWIAC82_Pylon_Bay_ID *PBI;
	unsigned short CalcCRC;
	char *AC_ID;
	int Count;

	AC_Description = (sWIAC82_1553_MLAN_Aircraft_Description_Message *)RxData;
	// Word 1 Header
	WIAC82_SS.AC_Description.AC_Descr_Msg.Header			= AC_Description->AC_Descr_Msg.Header;						 	// 0x0421
	// Word 2 Invalidity for words 1-16
	WIAC82_SS.AC_Description.AC_Descr_Msg.admInvalidity1	= AC_Description->AC_Descr_Msg.admInvalidity1;					// Invalidity for words 1 - 16
	// Word 3 Invalidity for words 17-30
	WIAC82_SS.AC_Description.AC_Descr_Msg.admInvalidity2	= AC_Description->AC_Descr_Msg.admInvalidity2;					// Invalidity for words 17 - 30
	// Word 4 Country Code
	WIAC82_SS.AC_Description.AC_Descr_Msg.CountryCode		= AC_Description->AC_Descr_Msg.CountryCode;
	// Word 5 Aircraft Identity (ASCII) 1
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID1	= AC_Description->AC_Descr_Msg.admAircraftID1;					// ASCII ='RA'
	// Word 6 Aircraft Identity (ASCII) 2
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID2	= AC_Description->AC_Descr_Msg.admAircraftID2;					// ASCII ='FA'
	// Word 7 Aircraft Identity (ASCII) 3
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID3	= AC_Description->AC_Descr_Msg.admAircraftID3;					// ASCII ='LE'
	// Word 8 Aircraft Identity (ASCII) 4
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID4	= AC_Description->AC_Descr_Msg.admAircraftID4;					// ASCII ='  '
	// Word 9 Aircraft Identity (ASCII) 5
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID5	= AC_Description->AC_Descr_Msg.admAircraftID5;					// ASCII ='  '
	// Word 10 Aircraft Identity (ASCII) 6
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID6	= AC_Description->AC_Descr_Msg.admAircraftID6;					// ASCII ='  '
	// Word 11 Aircraft Identity (ASCII) 7
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID7	= AC_Description->AC_Descr_Msg.admAircraftID7;					// ASCII ='  '
	// Word 12 Aircraft Identity (ASCII) 8
	WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID8	= AC_Description->AC_Descr_Msg.admAircraftID8;					// ASCII ='  '
	// Word 13 Station number and pylon/bay identity
	WIAC82_SS.AC_Description.AC_Descr_Msg.PBI				= AC_Description->AC_Descr_Msg.PBI;								// sWIAC82_Pylon_Bay_ID
	// Word 14..29 Reserved
	for (Count=0;Count<WIAC82_AIRCRAFT_DESCRIPTION_MESSAGE_RESERVED_SIZE;Count++){
		WIAC82_SS.AC_Description.AC_Descr_Msg.Reserved[Count] = AC_Description->AC_Descr_Msg.Reserved[Count];
	}
	// Word 30 Checksum
	WIAC82_SS.AC_Description.AC_Descr_Msg.CRC			= AC_Description->AC_Descr_Msg.CRC;

	CalcCRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.AC_Description.AC_Descr_Msg,((sizeof(sWIAC82_1553_Aircraft_Description_Message)/2)-1));

	if (AUXCTRL_SS.WIAC82Controls.RxAuxAC_Description||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (1R) AC DESCRIPT - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx1RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,AC_Description->MLANHeader.DataLength);
		if (AC_Description->MLANHeader.DataLength == sizeof(sWIAC82_1553_MLAN_Message)-sizeof(sWIAC82_MLAN_Header))
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]",WIAC82_SS.AuxStr,sizeof(sWIAC82_1553_MLAN_Aircraft_Description_Message));
		sprintf(WIAC82_SS.AuxStr,"%s [CRC=0x%4.4x",WIAC82_SS.AuxStr,AC_Description->AC_Descr_Msg.CRC);
		if (AC_Description->AC_Descr_Msg.CRC == CalcCRC)
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%x)]",WIAC82_SS.AuxStr,CalcCRC);
		sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		WIAC82_PrnMLANHeader(&AC_Description->MLANHeader,0);
		WIAC82_Prn1553Header(&AC_Description->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Header      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.Header);
			sprintf(WIAC82_SS.AuxStr,"%s  Invalidity1 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admInvalidity1);
			sprintf(WIAC82_SS.AuxStr,"%s  Invalidity2 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admInvalidity2);
			AC_ID=(char *)(&WIAC82_SS.AC_Description.AC_Descr_Msg.CountryCode);
			sprintf(WIAC82_SS.AuxStr,"%s  CountryCode = 0x%4.4x %c%c\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.CountryCode,AC_ID[1],AC_ID[0]);
			AC_ID=(char *)(&WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID1);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID1 = 0x%4.4x %c%c\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID1,AC_ID[1],AC_ID[0]);
			AC_ID=(char *)(&WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID2);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID2 = 0x%4.4x %c%c\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID2,AC_ID[1],AC_ID[0]);
			AC_ID=(char *)(&WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID3);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID3 = 0x%4.4x %c%c\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID3,AC_ID[1],AC_ID[0]);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID4 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID4);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID5 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID5);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID6 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID6);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID7 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID7);
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftID8 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.admAircraftID8);
			sprintf(WIAC82_SS.AuxStr,"%s  PBI         = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.PBI);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				PBI = (sWIAC82_Pylon_Bay_ID *)&WIAC82_SS.AC_Description.AC_Descr_Msg.PBI;
				sprintf(WIAC82_SS.AuxStr,"%s    Station Number         = 0x%2.2x:%2.2d  %s\n",WIAC82_SS.AuxStr,PBI->admStationNumber,PBI->admStationNumber,GetStationNumber(PBI->admStationNumber));
				sprintf(WIAC82_SS.AuxStr,"%s    Adaptor Identity       = 0x%2.2x:%2.2d  %s\n",WIAC82_SS.AuxStr,PBI->admAdaptorIdentity,PBI->admAdaptorIdentity,GetAdaptorIdentity(PBI->admAdaptorIdentity));
				sprintf(WIAC82_SS.AuxStr,"%s    Position Under Adaptor = 0x%2.2x:%2.2d  %s\n",WIAC82_SS.AuxStr,PBI->admPositionUnderAdaptor,PBI->admPositionUnderAdaptor,GetAdaptorPosition(PBI->admPositionUnderAdaptor));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved = 0x%x\n",WIAC82_SS.AuxStr, PBI->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved= ",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_AIRCRAFT_DESCRIPTION_MESSAGE_RESERVED_SIZE;Count++){
				sprintf(WIAC82_SS.AuxStr,"%s[0x%x}",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC         = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Description.AC_Descr_Msg.CRC);
		}
	}

}
void WIAC82_HandleAC_SystemTimeMsg2R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_System_Time_Message *AC_Time;
	TIME AircraftSystemTimeUs;

	AC_Time = (sWIAC82_1553_MLAN_System_Time_Message *)RxData;
	// Word 1 System Time
	WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime1			= AC_Time->AC_1553_System_Time.SystemTime1;
	// Word 2 System Time
	WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime2			= AC_Time->AC_1553_System_Time.SystemTime2;

	AircraftSystemTimeUs = Get2WordFloatU(&WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime1,FLOAT_10POWER_5) * 1000000; // microseconds

	WIAC82_SS.OffsetTimeLatency = AircraftSystemTimeUs - Weapon.SystemTime; //us

	if (AUXCTRL_SS.WIAC82Controls.RxAuxAC_Time||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (2R) SYSTEM_TIME_MESSAGE - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx2RMsgCnt);
		WIAC82_PrnMLANHeader(&AC_Time->MLANHeader,0);
		WIAC82_Prn1553Header(&AC_Time->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  SystemTime1 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  SystemTime2 = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.AC_Time.AC_1553_System_Time.SystemTime1,FLOAT_10POWER_5)));
		}
	}
}
void WIAC82_HandleGeneralRefusalMsg3R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Refusal_Message *Refusal;

	Refusal = (sWIAC82_1553_MLAN_Refusal_Message *)RxData;
	// Word 1 Random Refusal
	WIAC82_SS.Refusal.General.Reserved			= Refusal->General.Reserved;
	// Word 2 Random Refusal
	WIAC82_SS.Refusal.General.Cause				= Refusal->General.Cause;
	// Word 3 Global Refusal Identifier
	WIAC82_SS.Refusal.General.Global_ID			= Refusal->General.Global_ID;

	if (AUXCTRL_SS.WIAC82Controls.RxAuxRefusal||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (3R) GENERAL_REFUSAL_MESSAGE - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx3RMsgCnt);
		WIAC82_PrnMLANHeader(&Refusal->MLANHeader,0);
		WIAC82_Prn1553Header(&Refusal->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetailAuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Refusal.General.Reserved);
			sprintf(WIAC82_SS.AuxStr,"%s  Cause     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Refusal.General.Cause);
			sprintf(WIAC82_SS.AuxStr,"%s  Global_ID = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Refusal.General.Global_ID);
		}
	}
}

char *GetAirplaneTypeConfig(uint16_t AirplaneType){
	static char Airplane[10];
	if (AirplaneType == 1) {
		sprintf(Airplane,"Rafale");
	}
	else if (AirplaneType == 4){
		sprintf(Airplane,"M2000");
	}
	else {
		sprintf(Airplane,"Invalid");
	}
	return(Airplane);
}

char *GetCP1(uint16_t CP1){
	static char ChargePosition1[25];
	if (CP1 == 0) {
		sprintf(ChargePosition1,"Undefined Address");
	}
	else if (CP1 == 1){
		sprintf(ChargePosition1,"Rear Left Side");
	}
	else if (CP1 == 2){
		sprintf(ChargePosition1,"Rear Right Side");
	}
	else if (CP1 == 3){
		sprintf(ChargePosition1,"Front Left Side");
	}
	else if (CP1 == 4){
		sprintf(ChargePosition1,"Front Right Side");
	}
	else if (CP1 == 5){
		sprintf(ChargePosition1,"Left Adapter Charge");
	}
	else if (CP1 == 6){
		sprintf(ChargePosition1,"Central Adapter Charge");
	}
	else if (CP1 == 7){
		sprintf(ChargePosition1,"Right Adapter Charge");
	}
	else {
		sprintf(ChargePosition1,"Unused");
	}
	return(ChargePosition1);
}

void WIAC82_HandleWeaponConfigMsg10R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Configuration_Message *Configuration;
	sWIAC82_Position_Adaptor *PositionAdaptor;
	sWIAC82_Mixed_Config *ValidityMixedConfig;
	unsigned short CalcCRC;

	Configuration = (sWIAC82_1553_MLAN_Configuration_Message *)RxData;
	// Word 1
	WIAC82_SS.Configuration.Config.Identifier = Configuration->Config.Identifier;
	// Word 2
	WIAC82_SS.Configuration.Config.Length = Configuration->Config.Length;
	// Word 3 Validity/AC Type/Charge Position Address 1_2
	WIAC82_SS.Configuration.Config.Mixed1 = Configuration->Config.Mixed1;		// sWIAC82_Mixed_Config
	// Word 4 Charge Position Address 3_6
	WIAC82_SS.Configuration.Config.CP3_6  = Configuration->Config.CP3_6;		// sWIAC82_Charge_Position_Address
	// Word 5 Charge Position Address 7_10
	WIAC82_SS.Configuration.Config.CP7_10  = Configuration->Config.CP7_10;
	// Word 6 Charge Position Address 11_14
	WIAC82_SS.Configuration.Config.CP11_14 = Configuration->Config.CP11_14;
	// Word 7 Charge Position Address 15_18
	WIAC82_SS.Configuration.Config.CP15_18 = Configuration->Config.CP15_18;
	// Word 8 Charge Position Address 19_22
	WIAC82_SS.Configuration.Config.CP19_22 = Configuration->Config.CP19_22;
	// Word 9 Charge Position Address 23_26
	WIAC82_SS.Configuration.Config.CP23_26 = Configuration->Config.CP23_26;
	// Word 10 Charge Position Address 27_30
	WIAC82_SS.Configuration.Config.CP27_30 = Configuration->Config.CP27_30;
	// Word 11 Position Adapter/Ground Integrity Test Order
	WIAC82_SS.Configuration.Config.Mixed2 = Configuration->Config.Mixed2;		// sWIAC82_Position_Adaptor
	// Word 12 Reserved
	WIAC82_SS.Configuration.Config.Reserved = Configuration->Config.Reserved;
	// Word 13 Checksum
	WIAC82_SS.Configuration.Config.CRC = Configuration->Config.CRC;				// From word 3 to word 12

	CalcCRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&Configuration->Config.Mixed1,((sizeof(sWIAC82_1553_Configuration_Message)/2)-3));

	if (AUXCTRL_SS.WIAC82Controls.RxAuxConfiguration||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (10R) WEAPON_CONFIGURATION_MESSAGE - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx10RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,Configuration->MLANHeader.DataLength);
		if (Configuration->MLANHeader.DataLength == sizeof(sWIAC82_1553_MLAN_Message)-sizeof(sWIAC82_MLAN_Header))
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]",WIAC82_SS.AuxStr,sizeof(sWIAC82_1553_MLAN_Configuration_Message));
		sprintf(WIAC82_SS.AuxStr,"%s [CRC=0x%4.4x",WIAC82_SS.AuxStr,Configuration->Config.CRC);
		if (Configuration->Config.CRC == CalcCRC)
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%x)]",WIAC82_SS.AuxStr,CalcCRC);
		sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		WIAC82_PrnMLANHeader(&Configuration->MLANHeader,0);
		WIAC82_Prn1553Header(&Configuration->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.Identifier);
			sprintf(WIAC82_SS.AuxStr,"%s  Length = %d\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.Length);							// bytes default 22
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ValidityMixedConfig = (sWIAC82_Mixed_Config *)&WIAC82_SS.Configuration.Config.Mixed1;
				sprintf(WIAC82_SS.AuxStr,"%s    Validity_Setup          = %d\n",WIAC82_SS.AuxStr,ValidityMixedConfig->Validity_Setup);
				sprintf(WIAC82_SS.AuxStr,"%s    Validity_Integrity_Test = %d\n",WIAC82_SS.AuxStr,ValidityMixedConfig->Validity_Integrity_Test);
				sprintf(WIAC82_SS.AuxStr,"%s    Airplane_Type           = 0x%x %s\n",WIAC82_SS.AuxStr,ValidityMixedConfig->Airplane_Type,
						GetAirplaneTypeConfig(ValidityMixedConfig->Airplane_Type));
				sprintf(WIAC82_SS.AuxStr,"%s    Charge Position 1       = 0x%x %s\n",WIAC82_SS.AuxStr,ValidityMixedConfig->CP1,
						GetCP1(ValidityMixedConfig->CP1));
				sprintf(WIAC82_SS.AuxStr,"%s    Charge Position 2       = 0x%x\n",WIAC82_SS.AuxStr,ValidityMixedConfig->CP2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Mixed1 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.Mixed1);							// sWIAC82_Mixed_Config
			sprintf(WIAC82_SS.AuxStr,"%s  CP3_6  = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP3_6);								// sWIAC82_Charge_Position_Address
			sprintf(WIAC82_SS.AuxStr,"%s  CP7_10  = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP7_10);
			sprintf(WIAC82_SS.AuxStr,"%s  CP11_14 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP11_14);
			sprintf(WIAC82_SS.AuxStr,"%s  CP15_18 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP15_18);
			sprintf(WIAC82_SS.AuxStr,"%s  CP19_22 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP19_22);
			sprintf(WIAC82_SS.AuxStr,"%s  CP23_26 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP23_26);
			sprintf(WIAC82_SS.AuxStr,"%s  CP27_30 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CP27_30);
			sprintf(WIAC82_SS.AuxStr,"%s  Mixed2 = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.Mixed2);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				PositionAdaptor = (sWIAC82_Position_Adaptor *)&WIAC82_SS.Configuration.Config.Mixed2;
				sprintf(WIAC82_SS.AuxStr,"%s    Position Adaptor = 0x%2.2x %s\n",WIAC82_SS.AuxStr,PositionAdaptor->Position,
						GetPositionAdaptorConfig(PositionAdaptor->Position));
				sprintf(WIAC82_SS.AuxStr,"%s    Test Order       = 0x%2.2x \n",WIAC82_SS.AuxStr,PositionAdaptor->TestOrder);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved         = 0x%x\n",WIAC82_SS.AuxStr,PositionAdaptor->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.Reserved);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.Configuration.Config.CRC);								// From word 3 to word 12
		}
	}
}
char *DecodeSCM(uint SCM){
	static char strSCM[20];

	if (SCM==WIAC82_SCM_MISSION_STORE){
		sprintf(strSCM,"MISSION STORE");
	}
	else if (SCM==WIAC82_SCM_CARRIAGE_STORE){
		sprintf(strSCM,"CARRIAGE STORE");
	}
	else {
		sprintf(strSCM,"UNKNOWN SCM");
	}
	return(strSCM);
}
#define WIAC82_CRITICAL_FLAG_INVERTED		1
#define WIAC82_CRITICAL_FLAG_NOT_INVERTED	0

char *DecodeFlag(uint16_t Flag, uint16_t Inverted){
	static char strFlagState[20];

	if (Inverted) {
		if (Flag){
			sprintf(strFlagState,"NOK");
		}
		else {
			sprintf(strFlagState,"OK");
		}
	}
	else {
		if (Flag){
			sprintf(strFlagState,"OK");
		}
		else {
			sprintf(strFlagState,"NOK");
		}

	}
	return(strFlagState);
}
char *DecodeValidity(uint16_t Flag, uint16_t Inverted){
	static char strValidity[20];

	if (Inverted) {
		if (Flag){
			sprintf(strValidity,"INVALID");
		}
		else {
			sprintf(strValidity,"VALID");
		}
	}
	else {
		if (Flag){
			sprintf(strValidity,"VALID");
		}
		else {
			sprintf(strValidity,"INVALID");
		}

	}
	return(strValidity);
}
void WIAC82_UnpackCriticalCotrolMsg11R(unsigned char *RxData, unsigned int Timestamp) {
//	sWIAC82_1553_MLAN_Store_Control_Message *StoresCtl;
	sWIAC82_Store_Critical_Control_1 *CriticalControl1;
	uMIL1553_CriticalCtrl1 *CriticalCtrl1;
	uMIL1553_CriticalCtrl2 *CriticalCtrl2;
	uMIL1553_CriticalAuthority Authority1;
	uMIL1553_CriticalAuthority Authority2;

//	StoresCtl = (sWIAC82_1553_MLAN_Store_Control_Message *)RxData;
	CriticalCtrl1 = (uMIL1553_CriticalCtrl1 *)&WIAC82_SS.StoresCtl.Store.Control1;
	Authority1.C14 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D0;
	Authority1.C13 = CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D0;
	Authority1.C12 = CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D4;
	Authority1.C11 = CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D3;
	Authority1.C10 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D2 + CriticalCtrl1->Ctrl.D1 + CriticalCtrl1->Ctrl.D0;
	Authority1.C9 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D4;
	Authority1.C8 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D3 + CriticalCtrl1->Ctrl.D1 + CriticalCtrl1->Ctrl.D0;
	Authority1.C7 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D4 + CriticalCtrl1->Ctrl.D2 + CriticalCtrl1->Ctrl.D1;
	Authority1.C6 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D4 + CriticalCtrl1->Ctrl.D3;
	Authority1.C5 = CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D3 + CriticalCtrl1->Ctrl.D2;
	Authority1.C4 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D6 + CriticalCtrl1->Ctrl.D4 + CriticalCtrl1->Ctrl.D2 + CriticalCtrl1->Ctrl.D0;
	Authority1.C3 = CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D5 + CriticalCtrl1->Ctrl.D3 + CriticalCtrl1->Ctrl.D1;
	Authority1.C2 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D9 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D4 + CriticalCtrl1->Ctrl.D2 + CriticalCtrl1->Ctrl.D1;
	Authority1.C1 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D8 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D3;
	Authority1.C0 = CriticalCtrl1->Ctrl.D10 + CriticalCtrl1->Ctrl.D7 + CriticalCtrl1->Ctrl.D2 + CriticalCtrl1->Ctrl.D1 + CriticalCtrl1->Ctrl.D0;
	Authority1.NotUsed = 0;

	CriticalCtrl2 = (uMIL1553_CriticalCtrl2 *)&WIAC82_SS.StoresCtl.Store.Control2;
	Authority2.C14 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D0;
	Authority2.C13 = CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D0;
	Authority2.C12 = CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D4;
	Authority2.C11 = CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D3;
	Authority2.C10 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D2 + CriticalCtrl2->Ctrl.D1 + CriticalCtrl2->Ctrl.D0;
	Authority2.C9 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D4;
	Authority2.C8 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D3 + CriticalCtrl2->Ctrl.D1 + CriticalCtrl2->Ctrl.D0;
	Authority2.C7 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D4 + CriticalCtrl2->Ctrl.D2 + CriticalCtrl2->Ctrl.D1;
	Authority2.C6 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D4 + CriticalCtrl2->Ctrl.D3;
	Authority2.C5 = CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D3 + CriticalCtrl2->Ctrl.D2;
	Authority2.C4 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D6 + CriticalCtrl2->Ctrl.D4 + CriticalCtrl2->Ctrl.D2 + CriticalCtrl2->Ctrl.D0;
	Authority2.C3 = CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D5 + CriticalCtrl2->Ctrl.D3 + CriticalCtrl2->Ctrl.D1;
	Authority2.C2 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D9 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D4 + CriticalCtrl2->Ctrl.D2 + CriticalCtrl2->Ctrl.D1;
	Authority2.C1 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D8 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D3;
	Authority2.C0 = CriticalCtrl2->Ctrl.D10 + CriticalCtrl2->Ctrl.D7 + CriticalCtrl2->Ctrl.D2 + CriticalCtrl2->Ctrl.D1 + CriticalCtrl2->Ctrl.D0;
	Authority2.NotUsed = 0;

	WIAC82_SS.Critical.CalcCriticalAuthority1 = Authority1.Word;
	if (Authority1.Word == WIAC82_SS.StoresCtl.Store.scmCCCA1) {
		WIAC82_SS.Critical.Msg11R_CAuthority1_OK = 1;
	}
	else {
		WIAC82_SS.Critical.Msg11R_CAuthority1_OK = 0;
	}

	WIAC82_SS.Critical.CalcCriticalAuthority2 = Authority2.Word;
	if (Authority2.Word == WIAC82_SS.StoresCtl.Store.scmCCCA2) {
		WIAC82_SS.Critical.Msg11R_CAuthority2_OK = 1;
	}
	else {
		WIAC82_SS.Critical.Msg11R_CAuthority2_OK = 0;
	}

	if ((WIAC82_SS.StoresCtl.Store.InValidity1to16 & 0xFF80) == 0) {
		WIAC82_SS.Critical.Msg11R_CC1_NOK = 0;
	}
	else {
		WIAC82_SS.Critical.Msg11R_CC1_NOK = 1;
	}

	if ((WIAC82_SS.StoresCtl.Store.InValidity17t30 & 0x1FFF) == 0) {
		WIAC82_SS.Critical.Msg11R_CC2_NOK = 0;
	}
	else {
		WIAC82_SS.Critical.Msg11R_CC2_NOK = 1;
	}

    if (WIAC82_SS.StoresCtl.Store.Header == WIAC82_STORES_CONTROL_HEADER){
    	WIAC82_SS.Critical.Msg11R_Header_NOK = 0;
    }
    else {
    	WIAC82_SS.Critical.Msg11R_Header_NOK = 1;
    }
	CriticalControl1 = (sWIAC82_Store_Critical_Control_1 *)&WIAC82_SS.StoresCtl.Store.Control1;
    WIAC82_SS.Critical.Msg11R_RTAddress = CriticalControl1->RTaddress;

	if ((WIAC82_SS.Critical.Msg11R_CRC_NOK == 0) &&
	    (WIAC82_SS.Critical.Msg11R_CC1_NOK == 0) &&
	    (WIAC82_SS.Critical.Msg11R_CC2_NOK == 0) &&
	    (WIAC82_SS.Critical.Msg11R_Header_NOK == 0))
	{
		if (WIAC82_SS.Critical.Msg11R_CAuthority1_OK){
			WIAC82_SS.Critical.ARM_Weapon = CriticalCtrl1->Bits.Arm;
			WIAC82_SS.Critical.CommitToSeparate = CriticalCtrl1->Bits.CommitToSeparate;
	    	WIAC82_SS.Critical.CritCtrl1Result = 0;
		}
		else {
	    	WIAC82_SS.Critical.CritCtrl1Result = 1;
		}
		if (WIAC82_SS.Critical.Msg11R_CAuthority2_OK){
	    	WIAC82_SS.Critical.CritCtrl2Result = 0;
	    	//WIAC82_SS.Critical.EraseStart = 1;
	    	//WIAC82_SS.Critical.EraseStartTime = Weapon.SystemTime;
	    	//ClearMission(msRTAC);
		}
		else {
	    	WIAC82_SS.Critical.CritCtrl2Result = 1;
		}
	}
	else {
		WIAC82_SS.Critical.Rx11R_ErrCnt++;

	    if (WIAC82_SS.Critical.Msg11R_CRC_NOK) {
	    	WIAC82_SS.Critical.CritCtrl1Result = 2;
	    	WIAC82_SS.Critical.CritCtrl2Result = 2;
	    }
	    else if (WIAC82_SS.Critical.Msg11R_Header_NOK) {
	    	WIAC82_SS.Critical.CritCtrl1Result = 3;
	    	WIAC82_SS.Critical.CritCtrl2Result = 3;
	    }
	    else if (WIAC82_SS.Critical.Msg11R_CC1_NOK || WIAC82_SS.Critical.Msg11R_CC2_NOK) {
	    	WIAC82_SS.Critical.CritCtrl1Result = 4;
	    	WIAC82_SS.Critical.CritCtrl2Result = 4;
	    }
	}
	if (AUXCTRL_SS.WIAC82Controls.RxAuxStoresCtl||AUXCTRL_SS.WIAC82Controls.RxAuxOn){
		AUXsprintf("%u:Unpacking (11R) Critical Control Message\n",GetTimeUs());
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			AUXsprintf("  Message Header                    = %s\n",DecodeFlag(WIAC82_SS.Critical.Msg11R_Header_NOK,WIAC82_CRITICAL_FLAG_INVERTED));
			AUXsprintf("  Message CRC                       = %s\n",DecodeFlag(WIAC82_SS.Critical.Msg11R_CRC_NOK,WIAC82_CRITICAL_FLAG_INVERTED));
			AUXsprintf("  Calculated Critical Authority1    = 0x%4.4x %s\n",WIAC82_SS.Critical.CalcCriticalAuthority1,DecodeFlag(WIAC82_SS.Critical.Msg11R_CAuthority1_OK,WIAC82_CRITICAL_FLAG_NOT_INVERTED));
			AUXsprintf("  Calculated Critical Authority2    = 0x%4.4x %s\n",WIAC82_SS.Critical.CalcCriticalAuthority2,DecodeFlag(WIAC82_SS.Critical.Msg11R_CAuthority2_OK,WIAC82_CRITICAL_FLAG_NOT_INVERTED));
			AUXsprintf("  Validity Flags 1                  = %s\n",DecodeFlag(WIAC82_SS.Critical.Msg11R_CC1_NOK,WIAC82_CRITICAL_FLAG_INVERTED));
			AUXsprintf("  Validity Flags 2                  = %s\n",DecodeFlag(WIAC82_SS.Critical.Msg11R_CC2_NOK,WIAC82_CRITICAL_FLAG_INVERTED));
			AUXsprintf("  Error Count                       = %d\n",WIAC82_SS.Critical.Rx11R_ErrCnt);
			AUXsprintf("  RT Address                        = %d\n",WIAC82_SS.Critical.Msg11R_RTAddress);
			AUXsprintf("  ARM Weapon                        = %d\n",WIAC82_SS.Critical.ARM_Weapon);
			AUXsprintf("  CommitToSeparate                  = %d\n",WIAC82_SS.Critical.CommitToSeparate);
			AUXsprintf("  Erase Start                       = %d\n",WIAC82_SS.Critical.EraseStart);
		}
	}
}
void WIAC82_HandleWeaponStoreControlMsg11R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Store_Control_Message *StoresCtl;
	sWIAC82_Store_Critical_Control_1 *CriticalControl1;
	sWIAC82_Store_Critical_Control_2 *CriticalControl2;
	sWIAC82_Store_Critical_Control_1 *CriticalControl2_WIAC_Battery;
	sWIAC82_Satellite_Used1 *InValidity;

	int Count;

	StoresCtl = (sWIAC82_1553_MLAN_Store_Control_Message *)RxData;
	// Word 1 Header
	WIAC82_SS.StoresCtl.Store.Header			= StoresCtl->Store.Header;
	// Word 2 Invalidity 1-16
	WIAC82_SS.StoresCtl.Store.InValidity1to16	= StoresCtl->Store.InValidity1to16;
	// Word 3 Invalidity 17-30
	WIAC82_SS.StoresCtl.Store.InValidity17t30	= StoresCtl->Store.InValidity17t30;
	// Word 4 Critical Control 1
	WIAC82_SS.StoresCtl.Store.Control1			= StoresCtl->Store.Control1;							// sWIAC82_Store_Critical_Control_1
	// Word 5 Critical Authority 1
	WIAC82_SS.StoresCtl.Store.scmCCCA1			= StoresCtl->Store.scmCCCA1;							// Coded check for Critical Control
	// Word 6 Critical Control 2
	WIAC82_SS.StoresCtl.Store.Control2			= StoresCtl->Store.Control2;							// sWIAC82_Store_Critical_Control_2
	// Word 7 Critical Authority 2
	WIAC82_SS.StoresCtl.Store.scmCCCA2			= StoresCtl->Store.scmCCCA2;							// Coded check for Critical Control
	// Word 8..29 Reserved
	for (Count=0;Count<WIAC82_STORE_CONTROL_MESSAGE_RESERVED_SIZE;Count++) {
		WIAC82_SS.StoresCtl.Store.Reserved[Count]		= StoresCtl->Store.Reserved[Count];
	}
	// Word 30 Checksum
	WIAC82_SS.StoresCtl.Store.CRC				= StoresCtl->Store.CRC;

	WIAC82_SS.Critical.Msg11R_CalcCRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.StoresCtl.Store,((sizeof(sWIAC82_1553_Store_Control_Message)/2)-1));

	if (StoresCtl->Store.CRC == WIAC82_SS.Critical.Msg11R_CalcCRC){
		WIAC82_SS.Critical.Msg11R_CRC_NOK = 0; // OK
	}
	else {
		WIAC82_SS.Critical.Msg11R_CRC_NOK = 1; // NOT OK
	}

	WIAC82_UnpackCriticalCotrolMsg11R(RxData, Timestamp);

	CriticalControl2_WIAC_Battery = (sWIAC82_Store_Critical_Control_1 *)&WIAC82_SS.StoresCtl.Store.Control1;

	if (CriticalControl2_WIAC_Battery->ExecuteArmingD7 != WIAC82_SS.PreviousCriticalMonitorArmed)
	{
		if(CriticalControl2_WIAC_Battery->ExecuteArmingD7)
		{
			if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
				sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- EXECUTE ARMING (ON) ----- RX (11R) STORE CONTROL\n",WIAC82_SS.AuxStr,GetTimeUs());

			WIAC82_SS.WIAC82ExecuteArming = TRUE;

			if (Weapon.Flags.Bits.Ready) {
				if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- WEAPON READY ---------- \n",WIAC82_SS.AuxStr,GetTimeUs());
				WIAC82_SendExecuteArmingBattery();
				WIAC82_SS.PreviousCriticalMonitorArmed = CriticalControl2_WIAC_Battery->ExecuteArmingD7;
			} else {
				if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- WEAPON NOT READY -------- \n",WIAC82_SS.AuxStr,GetTimeUs());
			}
		}else{
			if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
				sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- EXECUTE ARMING (OFF) ----- RX (11R) STORE CONTROL\n",WIAC82_SS.AuxStr,GetTimeUs());
			WIAC82_SS.WIAC82ExecuteArming = FALSE;
			WIAC82_SendBatteryDeactivation();
			WIAC82_SS.PreviousCriticalMonitorArmed = CriticalControl2_WIAC_Battery->ExecuteArmingD7;
		}
	}

	if(CriticalControl2_WIAC_Battery->CommitToSeperateD8){
		if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 ----- COMMIT TO SEPARATE ----- RX (11R) STORE CONTROL\n",WIAC82_SS.AuxStr,GetTimeUs());
		WIAC82_SS.WIAC82CommitToSeparate = TRUE;
	}

	if (AUXCTRL_SS.WIAC82Controls.RxAuxStoresCtl||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (11R) STORE CONTROL - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx11RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,StoresCtl->MLANHeader.DataLength);
		if (StoresCtl->MLANHeader.DataLength == (sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)))
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]",WIAC82_SS.AuxStr,sizeof(sWIAC82_1553_MLAN_Aircraft_Description_Message));
		sprintf(WIAC82_SS.AuxStr,"%s [CRC=0x%4.4x",WIAC82_SS.AuxStr,StoresCtl->Store.CRC);
		if (WIAC82_SS.Critical.Msg11R_CRC_NOK){
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%x)]",WIAC82_SS.AuxStr,WIAC82_SS.Critical.Msg11R_CalcCRC);
		}
		else {
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		}
		sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		WIAC82_PrnMLANHeader(&StoresCtl->MLANHeader,0);
		WIAC82_Prn1553Header(&StoresCtl->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Header          = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.Header);
			sprintf(WIAC82_SS.AuxStr,"%s  InValidity1to16 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.InValidity1to16);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				InValidity = (sWIAC82_Satellite_Used1 *)(&WIAC82_SS.StoresCtl.Store.InValidity1to16);
				sprintf(WIAC82_SS.AuxStr,"%s    Header            = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite01, DecodeValidity(InValidity->Satellite01,1));
				sprintf(WIAC82_SS.AuxStr,"%s    InValidity1to16   = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite02, DecodeValidity(InValidity->Satellite02,1));
				sprintf(WIAC82_SS.AuxStr,"%s    InValidity17t30   = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite03, DecodeValidity(InValidity->Satellite03,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Critical Control1 = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite04, DecodeValidity(InValidity->Satellite04,1));
				sprintf(WIAC82_SS.AuxStr,"%s    scmCCCA1          = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite05, DecodeValidity(InValidity->Satellite05,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Critical Control2 = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite06, DecodeValidity(InValidity->Satellite06,1));
				sprintf(WIAC82_SS.AuxStr,"%s    scmCCCA2          = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite07, DecodeValidity(InValidity->Satellite07,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved1         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite08, DecodeValidity(InValidity->Satellite08,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved2         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite09, DecodeValidity(InValidity->Satellite09,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved3         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite10, DecodeValidity(InValidity->Satellite10,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved4         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite11, DecodeValidity(InValidity->Satellite11,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved5         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite12, DecodeValidity(InValidity->Satellite12,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved6         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite13, DecodeValidity(InValidity->Satellite13,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved7         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite14, DecodeValidity(InValidity->Satellite14,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved8         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite15, DecodeValidity(InValidity->Satellite15,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved9         = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite16, DecodeValidity(InValidity->Satellite01,1));
			}

			sprintf(WIAC82_SS.AuxStr,"%s  InValidity17t30 = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.InValidity17t30);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				InValidity = (sWIAC82_Satellite_Used1 *)(&WIAC82_SS.StoresCtl.Store.InValidity17t30);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved10        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite01, DecodeValidity(InValidity->Satellite01,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved11        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite02, DecodeValidity(InValidity->Satellite02,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved12        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite03, DecodeValidity(InValidity->Satellite03,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved13        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite04, DecodeValidity(InValidity->Satellite04,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved14        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite05, DecodeValidity(InValidity->Satellite05,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved15        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite06, DecodeValidity(InValidity->Satellite06,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved16        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite07, DecodeValidity(InValidity->Satellite07,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved17        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite08, DecodeValidity(InValidity->Satellite08,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved18        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite09, DecodeValidity(InValidity->Satellite09,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved19        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite10, DecodeValidity(InValidity->Satellite10,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved20        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite11, DecodeValidity(InValidity->Satellite11,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved21        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite12, DecodeValidity(InValidity->Satellite12,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved22        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite13, DecodeValidity(InValidity->Satellite13,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved23        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite14, DecodeValidity(InValidity->Satellite14,1));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved24        = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite15, DecodeValidity(InValidity->Satellite15,1));
				sprintf(WIAC82_SS.AuxStr,"%s    CRC               = %d %s\n",WIAC82_SS.AuxStr, InValidity->Satellite16, DecodeValidity(InValidity->Satellite16,1));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  Control1        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.Control1);							// sWIAC82_Store_Critical_Control_1
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				CriticalControl1 = (sWIAC82_Store_Critical_Control_1 *)&WIAC82_SS.StoresCtl.Store.Control1;
				sprintf(WIAC82_SS.AuxStr,"%s   ReleaseD10          = %d\n",WIAC82_SS.AuxStr,CriticalControl1->ReleaseD10);		// Fire, Launch and Release
				sprintf(WIAC82_SS.AuxStr,"%s   JettisonD9          = %d\n",WIAC82_SS.AuxStr,CriticalControl1->JettisonD9);
				sprintf(WIAC82_SS.AuxStr,"%s   CommitToSeperateD8  = %d\n",WIAC82_SS.AuxStr,CriticalControl1->CommitToSeperateD8);
				sprintf(WIAC82_SS.AuxStr,"%s   ExecuteArmingD7     = %d\n",WIAC82_SS.AuxStr,CriticalControl1->ExecuteArmingD7);
				sprintf(WIAC82_SS.AuxStr,"%s   PreSetArmingD6      = %d\n",WIAC82_SS.AuxStr,CriticalControl1->PreSetArmingD6);
				sprintf(WIAC82_SS.AuxStr,"%s   SelectStoreD5       = %d\n",WIAC82_SS.AuxStr,CriticalControl1->SelectStoreD5);
				sprintf(WIAC82_SS.AuxStr,"%s   InitiateIBITD4      = %d\n",WIAC82_SS.AuxStr,CriticalControl1->InitiateIBitD4);
				sprintf(WIAC82_SS.AuxStr,"%s   ReleaseModeD3       = %d\n",WIAC82_SS.AuxStr,CriticalControl1->ReleaseModeD3);
				sprintf(WIAC82_SS.AuxStr,"%s   SCM IdentifierD0-D2 = 0x%3.3x %s\n",WIAC82_SS.AuxStr,CriticalControl1->scmIdentifier,DecodeSCM(CriticalControl1->scmIdentifier));
				sprintf(WIAC82_SS.AuxStr,"%s   RTaddress           = %d 0x%2.2x\n",WIAC82_SS.AuxStr,CriticalControl1->RTaddress,CriticalControl1->RTaddress);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  scmCCCA1       = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.scmCCCA1);							// Coded check for Critical Control
			sprintf(WIAC82_SS.AuxStr,"%s  Control2       = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.Control2);							// sWIAC82_Store_Critical_Control_2
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				CriticalControl2 = (sWIAC82_Store_Critical_Control_2 *)&WIAC82_SS.StoresCtl.Store.Control2;
				sprintf(WIAC82_SS.AuxStr,"%s   EraseD10                   = %d\n",WIAC82_SS.AuxStr,CriticalControl2->EraseD10);		//
				sprintf(WIAC82_SS.AuxStr,"%s   RFjamD9                    = %d\n",WIAC82_SS.AuxStr,CriticalControl2->RFjamD9);
				sprintf(WIAC82_SS.AuxStr,"%s   RFemissionD8               = %d\n",WIAC82_SS.AuxStr,CriticalControl2->RFemissionD8);
				sprintf(WIAC82_SS.AuxStr,"%s   PreLaunchInhibitD7         = %d\n",WIAC82_SS.AuxStr,CriticalControl2->PreLaunchInhibitD7);
				sprintf(WIAC82_SS.AuxStr,"%s   AbortReleaseD6             = %d\n",WIAC82_SS.AuxStr,CriticalControl2->AbortReleaseD6);
				sprintf(WIAC82_SS.AuxStr,"%s   ActivateReleaseFunctionsD5 = %d\n",WIAC82_SS.AuxStr,CriticalControl2->ActivateReleaseFunctionsD5);
				sprintf(WIAC82_SS.AuxStr,"%s   ReservedD4                 = %d\n",WIAC82_SS.AuxStr,CriticalControl2->ReservedD4);
				sprintf(WIAC82_SS.AuxStr,"%s   ReservedD3                 = %d\n",WIAC82_SS.AuxStr,CriticalControl2->ReservedD3);
				sprintf(WIAC82_SS.AuxStr,"%s   SCM Identifier             = 0x%3.3x %s\n",WIAC82_SS.AuxStr,CriticalControl2->scmIdentifier,DecodeSCM(CriticalControl2->scmIdentifier));
				sprintf(WIAC82_SS.AuxStr,"%s   RTaddress                  = %d 0x%2.2x\n",WIAC82_SS.AuxStr,CriticalControl2->RTaddress,CriticalControl2->RTaddress);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  scmCCCA2       = 0x%x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.scmCCCA2);							// Coded check for Critical Control
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved=",WIAC82_SS.AuxStr);
			for (Count=0;Count<WIAC82_STORE_CONTROL_MESSAGE_RESERVED_SIZE;Count++) {
				sprintf(WIAC82_SS.AuxStr,"%s[%d]",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.Reserved[Count]);
			}
			sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC            = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.StoresCtl.Store.CRC);
		}
	}
}

void Pack14R_TransferAlignmentData(void)
{
	sWIAC82_ValidityAndIMUNumber *Validity;

	WIAC82_SS.TransferAlignmentData.Roll 		 	= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollAngle,FLOAT_2POWER_16, FLOAT_TURNS_2_RAD,FLOAT_2POWER_15);
	WIAC82_SS.TransferAlignmentData.Pitch 			= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchAngle,FLOAT_2POWER_16,FLOAT_TURNS_2_RAD, FLOAT_2POWER_16);
	WIAC82_SS.TransferAlignmentData.Yaw 			= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_TrueHeading,FLOAT_2POWER_16,FLOAT_TURNS_2_RAD, FLOAT_2POWER_15);
	WIAC82_SS.TransferAlignmentData.RollRate 		= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollRate,FLOAT_2POWER_6,FLOAT_DEG_2_RAD, FLOAT_2POWER_13);
	WIAC82_SS.TransferAlignmentData.PitchRate 		= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchRate,FLOAT_2POWER_6,FLOAT_DEG_2_RAD, FLOAT_2POWER_14);
	WIAC82_SS.TransferAlignmentData.YawRate 		= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_YawRate,FLOAT_2POWER_6,FLOAT_DEG_2_RAD, FLOAT_2POWER_14);
	WIAC82_SS.TransferAlignmentData.VelocityNorth 	= Convert2WordToShortS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed1,FLOAT_2POWER_21,FLOAT_10POWER_0, FLOAT_10POWER_2);
	WIAC82_SS.TransferAlignmentData.VelocityEast 	= Convert2WordToShortS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed1,FLOAT_2POWER_21,FLOAT_10POWER_0, FLOAT_10POWER_2);
	WIAC82_SS.TransferAlignmentData.VelocityDown 	= Convert2WordToShortS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed1,FLOAT_2POWER_21,FLOAT_10POWER_0, FLOAT_10POWER_2);
	WIAC82_SS.TransferAlignmentData.Reserved1 		= 0;

	WIAC82_SS.TransferAlignmentData.AngleOfAttack 	= Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfAttack,FLOAT_2POWER_16,FLOAT_TURNS_2_RAD, FLOAT_2POWER_16);
	WIAC82_SS.TransferAlignmentData.AngleOfSideslip = Convert1WordToShortS(WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfSideslip,FLOAT_2POWER_16,FLOAT_TURNS_2_RAD, FLOAT_2POWER_16);
	WIAC82_SS.TransferAlignmentData.Latency1 		= Convert2WordToShortU(&WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime1,FLOAT_10POWER_5,FLOAT_SEC_2_MILLISEC, FLOAT_10POWER_1);
	WIAC82_SS.TransferAlignmentData.Latency2 		= Convert2WordToShortU(&WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime1,FLOAT_10POWER_5,FLOAT_SEC_2_MILLISEC, FLOAT_10POWER_1);
	WIAC82_SS.TransferAlignmentData.Latency3 		= Convert2WordToShortU(&WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime1,FLOAT_10POWER_5,FLOAT_SEC_2_MILLISEC, FLOAT_10POWER_1);
	WIAC82_SS.TransferAlignmentData.Latency4 		= Convert2WordToShortU(&WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime1,FLOAT_10POWER_5,FLOAT_SEC_2_MILLISEC, FLOAT_10POWER_1);

	Validity = (sWIAC82_ValidityAndIMUNumber *)(&WIAC82_SS.Vector50Hz.Vec50Hz.ValidityAndACIMUNumber);
    WIAC82_SS.TransferAlignmentData.Validity.INS_TrueHeading	= Validity->INS_TrueHeading;
    WIAC82_SS.TransferAlignmentData.Validity.INS_RollPitchAngle	= Validity->INS_RollPitchAngle;
    WIAC82_SS.TransferAlignmentData.Validity.INS_PQR   			= Validity->INS_PQR;
    WIAC82_SS.TransferAlignmentData.Validity.VelocityNorth		= Validity->FighterNorthSpeed;
    WIAC82_SS.TransferAlignmentData.Validity.VelocityEast		= Validity->FighterEastSpeed;
    WIAC82_SS.TransferAlignmentData.Validity.VelocityDown		= Validity->FighterDownSpeed;
    WIAC82_SS.TransferAlignmentData.Validity.AngleOfAttack     	= Validity->AngleOfAttack;
    WIAC82_SS.TransferAlignmentData.Validity.AngleOfSideslip   	= Validity->AngleOfSideslip;

    if(Validity->AC_IMU_ReferenceNumber == 0)
    {
    	WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU1 = TRUE;
    } else {
    	WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU2 = TRUE;
    }

    WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg14RPacked = TRUE;

    // ADD for IMU1 or IMU2

	if (AUXCTRL_SS.WIAC82Controls.TransferAlignment){
		AUXsprintf("%u:Packing (14R) Transfer Alignment Data\n",GetTimeUs());
		AUXsprintf("   Validity INS_TrueHeading    = %d\n",WIAC82_SS.TransferAlignmentData.Validity.INS_TrueHeading);
		AUXsprintf("   Validity INS_RollPitchAngle = %d\n",WIAC82_SS.TransferAlignmentData.Validity.INS_RollPitchAngle);
		AUXsprintf("   Validity INS_PQR            = %d\n",WIAC82_SS.TransferAlignmentData.Validity.INS_PQR);
		AUXsprintf("   Validity VelocityNorth      = %d\n",WIAC82_SS.TransferAlignmentData.Validity.VelocityNorth);
		AUXsprintf("   Validity VelocityEast       = %d\n",WIAC82_SS.TransferAlignmentData.Validity.VelocityEast);
		AUXsprintf("   Validity VelocityDown       = %d\n",WIAC82_SS.TransferAlignmentData.Validity.VelocityDown);
		AUXsprintf("   Validity AngleOfAttack      = %d\n",WIAC82_SS.TransferAlignmentData.Validity.AngleOfAttack);
		AUXsprintf("   Validity AngleOfSideslip    = %d\n",WIAC82_SS.TransferAlignmentData.Validity.AngleOfSideslip);
		AUXsprintf("   Validity Reserved           = %d\n",WIAC82_SS.TransferAlignmentData.Validity.Reserved);
		AUXsprintf("   IMU1           			   = %d\n",WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU1);
		AUXsprintf("   IMU2           			   = %d\n",WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU2);

		AUXsprintf("  Roll                = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.Roll));
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Roll,FLOAT_2POWER_15)));
		AUXsprintf("  Pitch               = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.Pitch));
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Pitch,FLOAT_2POWER_16)));
		AUXsprintf("  Yaw                 = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.Yaw));
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.Yaw,FLOAT_2POWER_15)));

		AUXsprintf("  RollRate            = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.RollRate));
		AUXsprintf("  %8s [rad/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.RollRate,FLOAT_2POWER_13)));
		AUXsprintf("  PitchRate           = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.PitchRate));
		AUXsprintf("  %8s [rad/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.PitchRate,FLOAT_2POWER_14)));
		AUXsprintf("  YawRate             = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.YawRate));
		AUXsprintf("  %8s [rad/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.YawRate,FLOAT_2POWER_14)));

		AUXsprintf("  VelocityNorth       = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.VelocityNorth));
		AUXsprintf("  %8s [m/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityNorth,FLOAT_10POWER_2)));
		AUXsprintf("  VelocityEast        = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.VelocityEast));
		AUXsprintf("  %8s [m/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityEast,FLOAT_10POWER_2)));
		AUXsprintf("  VelocityDown        = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.VelocityDown));
		AUXsprintf("  %8s [m/s]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.VelocityDown,FLOAT_10POWER_2)));

		AUXsprintf("  AngleOfAttack       = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.AngleOfAttack));
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.AngleOfAttack,FLOAT_2POWER_16)));

		AUXsprintf("  AngleOfSideslip     = %s",AUXsprintfHexSignedShort(WIAC82_SS.TransferAlignmentData.AngleOfSideslip));
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.AngleOfSideslip,FLOAT_2POWER_16)));
		AUXsprintf("  Latency1            = 0x%4.4x",WIAC82_SS.TransferAlignmentData.Latency1);
		AUXsprintf("  %8s [ms]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Latency1,FLOAT_10POWER_1)));
		AUXsprintf("  Latency2            = 0x%4.4x",WIAC82_SS.TransferAlignmentData.Latency2);
		AUXsprintf("  %8s [ms]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Latency2,FLOAT_10POWER_1)));
		AUXsprintf("  Latency3            = 0x%4.4x",WIAC82_SS.TransferAlignmentData.Latency3);
		AUXsprintf("  %8s [ms]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Latency3,FLOAT_10POWER_1)));
		AUXsprintf("  Latency4            = 0x%4.4x",WIAC82_SS.TransferAlignmentData.Latency4);
		AUXsprintf("  %8s [ms]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Latency4,FLOAT_10POWER_1)));

	}
}
void WIAC82_HandleStateVector50HzMsg14R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_StateVector50Hz_Message *Vector50Hz;
	sWIAC82_ValidityAndIMUNumber *Validity;


	Vector50Hz = (sWIAC82_1553_MLAN_StateVector50Hz_Message *)RxData;
	// Word 1..2 Heading Time Tag
	WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime1			= Vector50Hz->Vec50Hz.HeadingTime1;
	WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime2			= Vector50Hz->Vec50Hz.HeadingTime2;
	// Word 3..4 Roll-Pitch Time Tag
	WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime1			= Vector50Hz->Vec50Hz.RollPitchTime1;
	WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime2			= Vector50Hz->Vec50Hz.RollPitchTime2;
	// Word 5..6 Horizontal Speed Time Tag
	WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime1			= Vector50Hz->Vec50Hz.HorzSpeedTime1;
	WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime2			= Vector50Hz->Vec50Hz.HorzSpeedTime2;
	// Word 7..8 Vertical Speed Time Tag
	WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime1			= Vector50Hz->Vec50Hz.VertSpeedTime1;
	WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime2			= Vector50Hz->Vec50Hz.VertSpeedTime2;
	// Word 9..10 Alpha Beta Time Tag
	WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime1			= Vector50Hz->Vec50Hz.ABSpeedTime1;
	WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime2			= Vector50Hz->Vec50Hz.ABSpeedTime2;
	// Word 11 Validity and AC IMU Number
	WIAC82_SS.Vector50Hz.Vec50Hz.ValidityAndACIMUNumber	= Vector50Hz->Vec50Hz.ValidityAndACIMUNumber;			// sWIAC82_ValidityAndIMUNumber
	// Word 12 INS True Heading
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_TrueHeading		= Vector50Hz->Vec50Hz.INS_TrueHeading;
	// Word 13 INS Roll Angle
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollAngle			= Vector50Hz->Vec50Hz.INS_RollAngle;
	// Word 14 INS Pitch Angle
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchAngle			= Vector50Hz->Vec50Hz.INS_PitchAngle;
	// Word 15 INS Roll Rate
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollRate			= Vector50Hz->Vec50Hz.INS_RollRate;
	// Word 16 INS Pitch Rate
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchRate			= Vector50Hz->Vec50Hz.INS_PitchRate;
	// Word 17 INS Yaw Rate
	WIAC82_SS.Vector50Hz.Vec50Hz.INS_YawRate			= Vector50Hz->Vec50Hz.INS_YawRate;
	// Word 18..19 Fighter North Speed
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed1		= Vector50Hz->Vec50Hz.FighterNorthSpeed1;
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed2		= Vector50Hz->Vec50Hz.FighterNorthSpeed2;
	// Word 20..21 Fighter East Speed
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed1		= Vector50Hz->Vec50Hz.FighterEastSpeed1;
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed2		= Vector50Hz->Vec50Hz.FighterEastSpeed2;
	// Word 22..23 Fighter Down Speed
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed1		= Vector50Hz->Vec50Hz.FighterDownSpeed1;
	WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed2		= Vector50Hz->Vec50Hz.FighterDownSpeed2;
	// Word 24
	WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfAttack			= Vector50Hz->Vec50Hz.AngleOfAttack;
	// Word 25
	WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfSideslip		= Vector50Hz->Vec50Hz.AngleOfSideslip;

	WIAC82_SS.VelocityLatency = Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime1,FLOAT_10POWER_5) * 1000000; // microseconds
	WIAC82_SS.AttitudeLatency = Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime1,FLOAT_10POWER_5) * 1000000;   // microseconds
	WIAC82_SS.RatesLatency 	  = Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime1,FLOAT_10POWER_5) * 1000000;   // microseconds

	if (AUXCTRL_SS.WIAC82Controls.RxAuxVector50Hz||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (14R) STATE_VECTOR_50HZ - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx14RMsgCnt);
		WIAC82_PrnMLANHeader(&Vector50Hz->MLANHeader,0);
		WIAC82_Prn1553Header(&Vector50Hz->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  HeadingTime1        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  HeadingTime2        = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.HeadingTime1,FLOAT_10POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  RollPitchTime1      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  RollPitchTime2      = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.RollPitchTime1,FLOAT_10POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  HorzSpeedTime1      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  HorzSpeedTime2      = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.HorzSpeedTime1,FLOAT_10POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  VertSpeedTime1      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  VertSpeedTime2      = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.VertSpeedTime1,FLOAT_10POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  ABSpeedTime1        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  ABSpeedTime2        = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector50Hz.Vec50Hz.ABSpeedTime1,FLOAT_10POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  Validity&CIMUNumber = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.ValidityAndACIMUNumber);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Validity = (sWIAC82_ValidityAndIMUNumber *)(&WIAC82_SS.Vector50Hz.Vec50Hz.ValidityAndACIMUNumber);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity INS_TrueHeading    = %d\n",WIAC82_SS.AuxStr,Validity->INS_TrueHeading);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity INS_RollPitchAngle = %d\n",WIAC82_SS.AuxStr,Validity->INS_RollPitchAngle);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity INS_PQR            = %d\n",WIAC82_SS.AuxStr,Validity->INS_PQR);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity FighterNorthSpeed  = %d\n",WIAC82_SS.AuxStr,Validity->FighterNorthSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity FighterEastSpeed   = %d\n",WIAC82_SS.AuxStr,Validity->FighterEastSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity FighterDownSpeed   = %d\n",WIAC82_SS.AuxStr,Validity->FighterDownSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity AngleOfAttack      = %d\n",WIAC82_SS.AuxStr,Validity->AngleOfAttack);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity AngleOfSideslip    = %d\n",WIAC82_SS.AuxStr,Validity->AngleOfSideslip);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved                    = 0x%2.2x\n",WIAC82_SS.AuxStr,Validity->Reserved);
				sprintf(WIAC82_SS.AuxStr,"%s   AC_IMU_ReferenceNumber      = 0x%2.2x\n",WIAC82_SS.AuxStr,Validity->AC_IMU_ReferenceNumber);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  INS_TrueHeading     = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_TrueHeading);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_TrueHeading,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  INS_RollAngle       = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollAngle);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollAngle,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  INS_PitchAngle      = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchAngle);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchAngle,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  INS_RollRate        = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollRate);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [deg/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_RollRate,FLOAT_2POWER_6)));
			sprintf(WIAC82_SS.AuxStr,"%s  INS_PitchRate       = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchRate);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [deg/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_PitchRate,FLOAT_2POWER_6)));
			sprintf(WIAC82_SS.AuxStr,"%s  INS_YawRate         = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.INS_YawRate);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [deg/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.INS_YawRate,FLOAT_2POWER_6)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterNorthSpeed1  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterNorthSpeed2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfDouble(Get2WordFloatS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterNorthSpeed1,FLOAT_2POWER_21)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterEastSpeed1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterEastSpeed2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterEastSpeed1,FLOAT_2POWER_21)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterDownSpeed1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterDownSpeed2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Vector50Hz.Vec50Hz.FighterDownSpeed1,FLOAT_2POWER_21)));
			sprintf(WIAC82_SS.AuxStr,"%s  AngleOfAttack       = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfAttack);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfAttack,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  AngleOfSideslip     = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfSideslip);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector50Hz.Vec50Hz.AngleOfSideslip,FLOAT_2POWER_16)));
		}
	}
	Pack14R_TransferAlignmentData();
}
char *DecodeWIAC82_IMU_Number(uint16_t IMU_Number) {
	static char strIMU_Number[50];

	if (IMU_Number == WIAC82_IMU_REFERENCE_NUMBER1){
		sprintf(strIMU_Number,"%s","IMU 1");
	}
	else if (IMU_Number == WIAC82_IMU_REFERENCE_NUMBER2){
		sprintf(strIMU_Number,"%s","IMU 2");
	}
	else {
		sprintf(strIMU_Number,"%s","UKNOWN IMU");
	}

	return(strIMU_Number);
}
void Pack16R_TransferAlignmentData(void)
{
	sWIAC82_StateVector6p25HzValidity *Validity;

	Validity = (sWIAC82_StateVector6p25HzValidity *)(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Validity);
	WIAC82_SS.TransferAlignmentData.Validity.FighterLatitude	= Validity->FighterLatitude;
	WIAC82_SS.TransferAlignmentData.Validity.FighterLongitude	= Validity->FighterLongitude;
	WIAC82_SS.TransferAlignmentData.Validity.FighterAltitude	= Validity->FighterAltitude;
	WIAC82_SS.TransferAlignmentData.Validity.TrueAirspeed      	= Validity->AircraftTrueAirspeed;
	WIAC82_SS.TransferAlignmentData.Validity.CalibratedAirspeed = Validity->AircraftCalibratedAirspeed;


	WIAC82_SS.TransferAlignmentData.Latitude 		 	= Convert2WordToIntS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude1,FLOAT_2POWER_32, FLOAT_TURNS_2_RAD,FLOAT_2POWER_31);
	WIAC82_SS.TransferAlignmentData.Longitude 		 	= Convert2WordToIntS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude1,FLOAT_2POWER_32, FLOAT_TURNS_2_RAD,FLOAT_2POWER_31);
	WIAC82_SS.TransferAlignmentData.Altitude 		 	= Convert2WordToShortS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude1,FLOAT_2POWER_16, FLOAT_10POWER_0,FLOAT_2POWER_0);
	WIAC82_SS.TransferAlignmentData.TrueAirspeed 	 	= Convert1WordToShortU(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftTrueAirspeed,FLOAT_2POWER_5, FLOAT_10POWER_0,FLOAT_10POWER_2);
	WIAC82_SS.TransferAlignmentData.CalibratedAirspeed 	= Convert1WordToShortU(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftCalibratedAirspeed,FLOAT_2POWER_5, FLOAT_10POWER_0,FLOAT_10POWER_2);

	WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg16RPacked = TRUE;

	if (AUXCTRL_SS.WIAC82Controls.TransferAlignment){
		AUXsprintf("%u:Packing (16R) Transfer Alignment Data\n",GetTimeUs());
		AUXsprintf("    Latitude           = %d\n",WIAC82_SS.TransferAlignmentData.Validity.FighterLatitude);
		AUXsprintf("    Longitude          = %d\n",WIAC82_SS.TransferAlignmentData.Validity.FighterLongitude);
		AUXsprintf("    Altitude           = %d\n",WIAC82_SS.TransferAlignmentData.Validity.FighterAltitude);
		AUXsprintf("    TrueAirspeed       = %d\n",WIAC82_SS.TransferAlignmentData.Validity.TrueAirspeed);
		AUXsprintf("    CalibratedAirspeed = %d\n",WIAC82_SS.TransferAlignmentData.Validity.CalibratedAirspeed);
		AUXsprintf("  Latitude              = 0x%8.8x",WIAC82_SS.TransferAlignmentData.Latitude);
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(GetFloatInt(WIAC82_SS.TransferAlignmentData.Latitude,FLOAT_2POWER_31)));
		AUXsprintf("  Longitude             = 0x%8.8x",WIAC82_SS.TransferAlignmentData.Longitude);
		AUXsprintf("  %8s [rad]\n",AUXsprintfFloat(GetFloatInt(WIAC82_SS.TransferAlignmentData.Longitude,FLOAT_2POWER_31)));
		AUXsprintf("  Altitude              = 0x%4.4x",WIAC82_SS.TransferAlignmentData.Altitude);
		AUXsprintf("  %8s [m]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.Altitude,FLOAT_2POWER_0)));
		AUXsprintf("  TrueAirspeed          = 0x%4.4x",WIAC82_SS.TransferAlignmentData.TrueAirspeed);
		AUXsprintf("  %8s [m/s]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.TrueAirspeed,FLOAT_10POWER_2)));
		AUXsprintf("  CalibratedAirspeed    = 0x%4.4x",WIAC82_SS.TransferAlignmentData.CalibratedAirspeed);
		AUXsprintf("  %8s [m/s]\n",AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.TransferAlignmentData.CalibratedAirspeed,FLOAT_10POWER_2)));
	}
}
void WIAC82_HandleStateVector6p25HzMsg16R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_StateVector6p25Hz_Message *Vector6p25Hz;
	sWIAC82_StateVector6p25HzValidity *Validity;

	Vector6p25Hz = (sWIAC82_1553_MLAN_StateVector6p25Hz_Message *)RxData;
	// Word 1..2 Aero Time Tag
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AeroTimeTag1					= Vector6p25Hz->Vec6p25Hz.AeroTimeTag1;					// LSB=1E-5 Max Val =42949.67295
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AeroTimeTag2					= Vector6p25Hz->Vec6p25Hz.AeroTimeTag2;
	// Word 3..4 Best Position and Wind Time Tag
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag1	= Vector6p25Hz->Vec6p25Hz.BestPositionAndWindTimeTag1; 	// LSB=1E-5 Max Val =42949.67295
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag2	= Vector6p25Hz->Vec6p25Hz.BestPositionAndWindTimeTag2; 	// LSB=1E-5 Max Val =42949.67295
	// Word 5 Validity
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Validity						= Vector6p25Hz->Vec6p25Hz.Validity; 					// sWIAC82_StateVector6p25HzValidity
	// Word 6 Aircraft Calibrated Airpeed
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftCalibratedAirspeed		= Vector6p25Hz->Vec6p25Hz.AircraftCalibratedAirspeed;
	// Word 7 Aircraft True Airspeed
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftTrueAirspeed			= Vector6p25Hz->Vec6p25Hz.AircraftTrueAirspeed;
	// Word 8..9 Fighter Latitude
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude1				= Vector6p25Hz->Vec6p25Hz.FighterLatitude1;
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude2				= Vector6p25Hz->Vec6p25Hz.FighterLatitude2;
	// Word 10..11 Fighter Longitude
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude1				= Vector6p25Hz->Vec6p25Hz.FighterLongitude1;
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude2				= Vector6p25Hz->Vec6p25Hz.FighterLongitude2;
	// Word 12..13 Fighter Altitude
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude1				= Vector6p25Hz->Vec6p25Hz.FighterAltitude1;
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude2				= Vector6p25Hz->Vec6p25Hz.FighterAltitude2;
	// Word 14 Wind Speed
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindSpeed						= Vector6p25Hz->Vec6p25Hz.WindSpeed;
	// Word 15 Wind Direction
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindDirection					= Vector6p25Hz->Vec6p25Hz.WindDirection;
	// Word 16..17 Reserved
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Reserved1						= Vector6p25Hz->Vec6p25Hz.Reserved1;
	WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Reserved2						= Vector6p25Hz->Vec6p25Hz.Reserved2;

	WIAC82_SS.PositionLatency = Get2WordFloatU(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag1,FLOAT_10POWER_5) * 1000000; // microseconds

	if (AUXCTRL_SS.WIAC82Controls.RxAuxVector6p25Hz||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (16R) STATE_VECTOR_6P25HZ_MESSAGE - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx16RMsgCnt);
		WIAC82_PrnMLANHeader(&Vector6p25Hz->MLANHeader,0);
		WIAC82_Prn1553Header(&Vector6p25Hz->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  AeroTimeTag1                  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AeroTimeTag1);
			sprintf(WIAC82_SS.AuxStr,"%s  AeroTimeTag2                  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AeroTimeTag2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AeroTimeTag1,FLOAT_10POWER_5)));

			sprintf(WIAC82_SS.AuxStr,"%s  BestPositionAndWindTimeTag1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag1);
			sprintf(WIAC82_SS.AuxStr,"%s  BestPositionAndWindTimeTag2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.BestPositionAndWindTimeTag1,FLOAT_10POWER_5)));

			sprintf(WIAC82_SS.AuxStr,"%s  Validity                      = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Validity);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Validity = (sWIAC82_StateVector6p25HzValidity *)(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Validity);
				sprintf(WIAC82_SS.AuxStr,"%s    AircraftCalibratedAirspeed = %d\n",WIAC82_SS.AuxStr,Validity->AircraftCalibratedAirspeed);
				sprintf(WIAC82_SS.AuxStr,"%s    AircraftTrueAirspeed       = %d\n",WIAC82_SS.AuxStr,Validity->AircraftTrueAirspeed);
				sprintf(WIAC82_SS.AuxStr,"%s    FighterLatitude            = %d\n",WIAC82_SS.AuxStr,Validity->FighterLatitude);
				sprintf(WIAC82_SS.AuxStr,"%s    FighterLongitude           = %d\n",WIAC82_SS.AuxStr,Validity->FighterLongitude);
				sprintf(WIAC82_SS.AuxStr,"%s    FighterAltitude            = %d\n",WIAC82_SS.AuxStr,Validity->FighterAltitude);
				sprintf(WIAC82_SS.AuxStr,"%s    WindSpeed                  = %d\n",WIAC82_SS.AuxStr,Validity->WindSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s    WindDirection              = %d\n",WIAC82_SS.AuxStr,Validity->WindDirection);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                   = %d\n",WIAC82_SS.AuxStr,Validity->Reserved);
				sprintf(WIAC82_SS.AuxStr,"%s    AC_IMU_ReferenceNumber     = %d %s\n",WIAC82_SS.AuxStr,Validity->AC_IMU_ReferenceNumber,DecodeWIAC82_IMU_Number(Validity->AC_IMU_ReferenceNumber));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftCalibratedAirspeed    = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftCalibratedAirspeed);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftCalibratedAirspeed,FLOAT_2POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  AircraftTrueAirspeed          = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftTrueAirspeed);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.AircraftTrueAirspeed,FLOAT_2POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterLatitude1              = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterLatitude2              = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLatitude1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterLongitude1             = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterLongitude2             = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterLongitude1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  FighterAltitude1              = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  FighterAltitude2              = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Vector6p25Hz.Vec6p25Hz.FighterAltitude1,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  WindSpeed                     = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindSpeed);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindSpeed,FLOAT_2POWER_7)));
			sprintf(WIAC82_SS.AuxStr,"%s  WindDirection                 = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindDirection);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Vector6p25Hz.Vec6p25Hz.WindDirection,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved1                     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Reserved1);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved2                     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Vector6p25Hz.Vec6p25Hz.Reserved2);
		}
	}
	Pack16R_TransferAlignmentData();

}

void Pack21R_TransferAlignmentData(void)
{
	FunctionalExchange1ValidityWord *Validity;

	Validity = (FunctionalExchange1ValidityWord *)(&WIAC82_SS.Func1D.FuncExch.Validity);
	WIAC82_SS.TransferAlignmentData.Validity.AngleOffset	= Validity->AngleOffsetValid;
	WIAC82_SS.TransferAlignmentData.Validity.LeverArm		= Validity->LeverArmValid;

	if (WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU1) {
		// IMU1
		WIAC82_SS.TransferAlignmentData.LeverArmX       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU1,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
		WIAC82_SS.TransferAlignmentData.LeverArmY       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU1,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
		WIAC82_SS.TransferAlignmentData.LeverArmZ       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU1,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
	}
	else if (WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU2){
		// IMU2
		WIAC82_SS.TransferAlignmentData.LeverArmX       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU2,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
		WIAC82_SS.TransferAlignmentData.LeverArmY       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU2,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
		WIAC82_SS.TransferAlignmentData.LeverArmZ       = Convert1WordToShortS(WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU2,FLOAT_2POWER_10,FLOAT_10POWER_0,FLOAT_2POWER_0);
	} else {
		WIAC82_SS.TransferAlignmentData.LeverArmX       = 0;
		WIAC82_SS.TransferAlignmentData.LeverArmY       = 0;
		WIAC82_SS.TransferAlignmentData.LeverArmZ       = 0;
	}

	WIAC82_SS.TransferAlignmentData.WICA82Flag.Msg21RPacked = TRUE;

	if (AUXCTRL_SS.WIAC82Controls.TransferAlignment){
		AUXsprintf("%u:Packing (21R) Transfer Alignment Data\n",GetTimeUs());
		Validity = (FunctionalExchange1ValidityWord *)(&WIAC82_SS.Func1D.FuncExch.Validity);
		AUXsprintf("  Validity Angle Offset   = %d\n",Validity->AngleOffsetValid);
		AUXsprintf("  Validity Lever Arm      = %d\n",Validity->LeverArmValid);
		AUXsprintf("  Reserved                = %d\n",Validity->Reserved);
		AUXsprintf("   IMU1           		  = %d\n",WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU1);
		AUXsprintf("   IMU2           		  = %d\n",WIAC82_SS.TransferAlignmentData.WICA82Flag.IMU2);

		AUXsprintf("  LeverArmX   = 0x%4.4x",WIAC82_SS.TransferAlignmentData.LeverArmX);
		AUXsprintf("  %8s [m]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmX,FLOAT_2POWER_10)));
		AUXsprintf("  LeverArmY   = 0x%4.4x",WIAC82_SS.TransferAlignmentData.LeverArmY);
		AUXsprintf("  %8s [m]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmY,FLOAT_2POWER_10)));
		AUXsprintf("  LeverArmZ   = 0x%4.4x",WIAC82_SS.TransferAlignmentData.LeverArmZ);
		AUXsprintf("  %8s [m]\n",AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.TransferAlignmentData.LeverArmZ,FLOAT_2POWER_10)));
	}
}

void WIAC82_HandleFunctionalExchange1DMsg21R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_FunctionalExchange1D_Message *Func1D;
	FunctionalExchange1ValidityWord *Validity;
	unsigned short CalcCRC;

	Func1D = (sWIAC82_1553_MLAN_FunctionalExchange1D_Message *)RxData;
	// Word 1 Identifier
	WIAC82_SS.Func1D.FuncExch.Identifier				= Func1D->FuncExch.Identifier;				// default 1
	// Word 2 Useful Length
	WIAC82_SS.Func1D.FuncExch.UsefulLength				= Func1D->FuncExch.UsefulLength;			// Wagon header not to be considered as part of CRC
	// Word 3 Validity
	WIAC82_SS.Func1D.FuncExch.Validity					= Func1D->FuncExch.Validity; 				// FunctionalExchange1ValidityWord
	// Word 4..5 AC Reference Lever Arm Roll Angle Offset
	WIAC82_SS.Func1D.FuncExch.ACLevArmRollAngOffset1	= Func1D->FuncExch.ACLevArmRollAngOffset1;
	WIAC82_SS.Func1D.FuncExch.ACLevArmRollAngOffset2	= Func1D->FuncExch.ACLevArmRollAngOffset2;
	// Word 6..7 AC Reference Lever Arm Pitch Angle Offset
	WIAC82_SS.Func1D.FuncExch.ACLevArmPitchAngOffset1	= Func1D->FuncExch.ACLevArmPitchAngOffset1;
	WIAC82_SS.Func1D.FuncExch.ACLevArmPitchAngOffset2	= Func1D->FuncExch.ACLevArmPitchAngOffset2;
	// Word 8..9 AC Reference Lever Arm Yaw Angle Offset
	WIAC82_SS.Func1D.FuncExch.ACLevArmYawAngOffset1		= Func1D->FuncExch.ACLevArmYawAngOffset1;
	WIAC82_SS.Func1D.FuncExch.ACLevArmYawAngOffset2		= Func1D->FuncExch.ACLevArmYawAngOffset2;
	// Word 10 AC Ref Lever Arm X Axis offset IMU 1
	WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU1		= Func1D->FuncExch.ACLeverArmXoffsetlMU1;
	// Word 11 AC Ref Lever Arm Y Axis offset IMU 1
	WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU1		= Func1D->FuncExch.ACLeverArmYoffsetlMU1;
	// Word 12 AC Ref Lever Arm Z Axis offset IMU 1
	WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU1		= Func1D->FuncExch.ACLeverArmZoffsetlMU1;
	// Word 13..18
	WIAC82_SS.Func1D.FuncExch.Reserve1					= Func1D->FuncExch.Reserve1;
	WIAC82_SS.Func1D.FuncExch.Reserve2					= Func1D->FuncExch.Reserve2;
	WIAC82_SS.Func1D.FuncExch.Reserve3					= Func1D->FuncExch.Reserve3;
	WIAC82_SS.Func1D.FuncExch.Reserve4					= Func1D->FuncExch.Reserve4;
	WIAC82_SS.Func1D.FuncExch.Reserve5					= Func1D->FuncExch.Reserve5;
	WIAC82_SS.Func1D.FuncExch.Reserve6					= Func1D->FuncExch.Reserve6;
	// Word 19 AC Ref Lever Arm X Axis offset IMU 2
	WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU2		= Func1D->FuncExch.ACLeverArmXoffsetlMU2;
	// Word 20 AC Ref Lever Arm Y Axis offset IMU 2
	WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU2		= Func1D->FuncExch.ACLeverArmYoffsetlMU2;
	// Word 21 AC Ref Lever Arm Z Axis offset IMU 2
	WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU2		= Func1D->FuncExch.ACLeverArmZoffsetlMU2;
	// Word 22 GPS Ref Lever Arm X Axis offset
	WIAC82_SS.Func1D.FuncExch.GPSLeverArmXoffset		= Func1D->FuncExch.GPSLeverArmXoffset;
	// Word 23 GPS Ref Lever Arm Y Axis offset
	WIAC82_SS.Func1D.FuncExch.GPSLeverArmYoffset		= Func1D->FuncExch.GPSLeverArmYoffset;
	// Word 24 GPS Ref Lever Arm Z Axis offset
	WIAC82_SS.Func1D.FuncExch.GPSLeverArmZoffset		= Func1D->FuncExch.GPSLeverArmZoffset;
	// Word 25 Checksum
	WIAC82_SS.Func1D.FuncExch.CRC						= Func1D->FuncExch.CRC;

	CalcCRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&WIAC82_SS.Func1D.FuncExch.Validity,(sizeof(sWIAC82_1553_FunctionalExchange1D_Message)/2)-3);

	if (AUXCTRL_SS.WIAC82Controls.RxAuxFunction1D||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (21R) FUNCTIONAL_EXCHANGE 1D - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx21RMsgCnt);

		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,Func1D->MLANHeader.DataLength);
		if (Func1D->MLANHeader.DataLength == (sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)))
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]",WIAC82_SS.AuxStr,(sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)));
		sprintf(WIAC82_SS.AuxStr,"%s [CRC=0x%4.4x",WIAC82_SS.AuxStr,Func1D->FuncExch.CRC);
		if (Func1D->FuncExch.CRC == CalcCRC)
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%x)]",WIAC82_SS.AuxStr,CalcCRC);
		sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);
		WIAC82_PrnMLANHeader(&Func1D->MLANHeader,0);
		WIAC82_Prn1553Header(&Func1D->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier       = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Identifier);
			sprintf(WIAC82_SS.AuxStr,"%s  UsefulLength     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.UsefulLength);
			sprintf(WIAC82_SS.AuxStr,"%s  Validity         = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Validity);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Validity = (FunctionalExchange1ValidityWord *)(&WIAC82_SS.Func1D.FuncExch.Validity);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity Angle Offset   = %d\n",WIAC82_SS.AuxStr,Validity->AngleOffsetValid);
				sprintf(WIAC82_SS.AuxStr,"%s   Validity Lever Arm      = %d\n",WIAC82_SS.AuxStr,Validity->LeverArmValid);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved = %d\n",WIAC82_SS.AuxStr,Validity->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmRollAngOffset1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmRollAngOffset1);
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmRollAngOffset2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmRollAngOffset2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func1D.FuncExch.ACLevArmRollAngOffset1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmPitchAngOffset1  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmPitchAngOffset1);
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmPitchAngOffset2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmPitchAngOffset2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func1D.FuncExch.ACLevArmPitchAngOffset1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmYawAngOffset1    = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmYawAngOffset1);
			sprintf(WIAC82_SS.AuxStr,"%s  ACLevArmYawAngOffset2    = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLevArmYawAngOffset2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func1D.FuncExch.ACLevArmYawAngOffset1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmXoffsetlMU1    = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU1);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU1,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmYoffsetlMU1    = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU1);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU1,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmZoffsetlMU1    = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU1);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU1,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve1  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve1);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve2  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve2);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve3  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve3);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve4  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve4);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve5  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve5);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserve6  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.Reserve6);
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmXoffsetlMU2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmXoffsetlMU2,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmYoffsetlMU2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmYoffsetlMU2,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  ACLeverArmZoffsetlMU2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.ACLeverArmZoffsetlMU2,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  GPSLeverArmXoffset     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.GPSLeverArmXoffset);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.GPSLeverArmXoffset,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  GPSLeverArmYoffset     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.GPSLeverArmYoffset);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.GPSLeverArmYoffset,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  GPSLeverArmZoffset     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.GPSLeverArmZoffset);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func1D.FuncExch.GPSLeverArmZoffset,FLOAT_2POWER_10)));
			sprintf(WIAC82_SS.AuxStr,"%s  CRC                    = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func1D.FuncExch.CRC);
		}
	}
	Pack21R_TransferAlignmentData();
}

char *DecodeWIAC82_GNSS_Mode(uint16_t GNSS_Mode) {
	static char strGNSS_Mode[50];

	if (GNSS_Mode == WIAC82_GNSS_MODE_ALL_OFF){
		sprintf(strGNSS_Mode,"%s","All Constellations OFF");
	}
	else if (GNSS_Mode == WIAC82_GNSS_MODE_GLONASS_OFF_GPS_ON){
		sprintf(strGNSS_Mode,"%s","GLONASS OFF GPS ON");
	}
	else if (GNSS_Mode == WIAC82_GNSS_MODE_GLONASS_ON_GPS_OFF){
		sprintf(strGNSS_Mode,"%s","GLONASS ON, GPS OFF");
	}
	else if (GNSS_Mode == WIAC82_GNSS_MODE_GLONASS_ON_GPS_ON){
		sprintf(strGNSS_Mode,"%s","GLONASS ON, GPS ON");
	}
	else {
		sprintf(strGNSS_Mode,"%s","Reserved");
	}

	return(strGNSS_Mode);
}
char *DecodeWIAC82_FuseSettings(uint16_t FuzeSetting) {
	static char strFuzeSetting[50];

	if (FuzeSetting == WIAC82_FUZE_SETTING_AT_IMPACT){
		sprintf(strFuzeSetting,"%s","At Impact");
	}
	else if (FuzeSetting == WIAC82_FUZE_SETTING_TIME_AFTER_IMPACT){
		sprintf(strFuzeSetting,"%s","Time after Impact (Delay)");
	}
	else if (FuzeSetting == WIAC82_FUZE_SETTING_AT_ALTITUDE){
		sprintf(strFuzeSetting,"%s","At Altitude (Airburst)");
	}
	else if (FuzeSetting == WIAC82_FUZE_SETTING_IN_PROGRAMMING){
		sprintf(strFuzeSetting,"%s","In programming");
	}
	else {
		sprintf(strFuzeSetting,"%s","Unknown");
	}

	return(strFuzeSetting);
}
char *DecodeWIAC82_SeekerSettings(uint16_t SeekerSetting) {
	static char strSeekerSetting[50];

	if (SeekerSetting == WIAC82_SEEKER_SETTING_GNSS_ONLY){
		sprintf(strSeekerSetting,"GNSS Only");
	}
	else if (SeekerSetting == WIAC82_SEEKER_SETTING_SAL){
		sprintf(strSeekerSetting,"SAL");
	}
	else if (SeekerSetting == WIAC82_SEEKER_SETTING_IIR){
		sprintf(strSeekerSetting,"IIR (Al Tariq)");
	}
	else {
		sprintf(strSeekerSetting,"Unknown");
	}

	return(strSeekerSetting);
}
void WIAC82_UnpackFunctionalExchange3DMsg22R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_FunctionalExchange3D_Message *Func3D;
	bFunctionalExchange2ValidityWord *FuncValid;
	bFunctionalExchange2MissionSettings *MissionSettings;
	unsigned char SAL_CodeValid;
	float SAL_Freq;

	Func3D = (sWIAC82_1553_MLAN_FunctionalExchange3D_Message *)RxData;
	// Word 1 Identifier
	WIAC82_SS.Func3D.FuncExch.Identifier		= Func3D->FuncExch.Identifier;		// default 1
	// Word 2 Useful Length
	WIAC82_SS.Func3D.FuncExch.UsefulLength		= Func3D->FuncExch.UsefulLength;	// Wagon header not to be considered as part of CRC
	// Word 3 Validity
	WIAC82_SS.Func3D.FuncExch.Validity			= Func3D->FuncExch.Validity;  		// FunctionalExchange2ValidityWord
	// Word 4..5 Target Latitude
	WIAC82_SS.Func3D.FuncExch.TargetLatitude1	= Func3D->FuncExch.TargetLatitude1;  // WGS84 coordinates
	WIAC82_SS.Func3D.FuncExch.TargetLatitude2	= Func3D->FuncExch.TargetLatitude2;  // WGS84 coordinates
	// Word 6..7 Target Longitude
	WIAC82_SS.Func3D.FuncExch.TargetLongitude1	= Func3D->FuncExch.TargetLongitude1; // WGS84 coordinates
	WIAC82_SS.Func3D.FuncExch.TargetLongitude2	= Func3D->FuncExch.TargetLongitude2; // WGS84 coordinates
	// Word 8..9 Target Altitude
	WIAC82_SS.Func3D.FuncExch.TargetAltitude1	= Func3D->FuncExch.TargetAltitude1;  // MSL in geoid EGM96
	WIAC82_SS.Func3D.FuncExch.TargetAltitude2	= Func3D->FuncExch.TargetAltitude2;  // MSL in geoid EGM96.
	// Word 10 Dive Angle
	WIAC82_SS.Func3D.FuncExch.DiveAngle			= Func3D->FuncExch.DiveAngle;
	// Word 11 Impact Heading
	WIAC82_SS.Func3D.FuncExch.ImpactHeading		= Func3D->FuncExch.ImpactHeading;   // Only for AL TARIQ (Logical ICD page 37)
	// Word 12 Impact Speed
	WIAC82_SS.Func3D.FuncExch.ImpactSpeed		= Func3D->FuncExch.ImpactSpeed;     // Only for AL TARIQ (Logical ICD page 37)
	// Word 13 SAL Code
	WIAC82_SS.Func3D.FuncExch.SALCode			= Func3D->FuncExch.SALCode;
	// Word 14 Image Index
	WIAC82_SS.Func3D.FuncExch.ImageIndex		= Func3D->FuncExch.ImageIndex;
	// Word 15 Image Checksum
	WIAC82_SS.Func3D.FuncExch.ImageChecksum		= Func3D->FuncExch.ImageChecksum;
	// Word 16 Mission Settings
	WIAC82_SS.Func3D.FuncExch.MissionSettings	= Func3D->FuncExch.MissionSettings;
	// Word 17
	WIAC82_SS.Func3D.FuncExch.FuzeDelay			= Func3D->FuncExch.FuzeDelay;		// [ms] Max = 240
	// Word 18
	WIAC82_SS.Func3D.FuncExch.FuzeHeight		= Func3D->FuncExch.FuzeHeight;		// [m] Max = 200
	// Word 19..20 Reserved
	WIAC82_SS.Func3D.FuncExch.Reserved1			= Func3D->FuncExch.Reserved1;
	WIAC82_SS.Func3D.FuncExch.Reserved2			= Func3D->FuncExch.Reserved2;
	// Word 21 Checksum
	WIAC82_SS.Func3D.FuncExch.CRC				= Func3D->FuncExch.CRC;				// Calculated from word 3 to Word 20

	if (AUXCTRL_SS.WIAC82Controls.MissionData) {
		FuncValid = (bFunctionalExchange2ValidityWord *)&WIAC82_SS.Func3D.FuncExch.Validity;
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 FUNCTIONAL_EXCHANGE_3D_MESSAGE (22R) \n",WIAC82_SS.AuxStr,GetTimeUs());
		sprintf(WIAC82_SS.AuxStr,"%s Target Latitude %8s [turn] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetLatitude1,FLOAT_2POWER_32)),FuncValid->TargetLongLatAlt);
		sprintf(WIAC82_SS.AuxStr,"%s Target Longitude %8s [turn] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetLongitude1,FLOAT_2POWER_32)),FuncValid->TargetLongLatAlt);
		sprintf(WIAC82_SS.AuxStr,"%s Target Altitude %8s [m] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetAltitude1,FLOAT_2POWER_16)),FuncValid->TargetLongLatAlt);
		sprintf(WIAC82_SS.AuxStr,"%s Moving Target Mode %d\n",
						WIAC82_SS.AuxStr,FuncValid->MovingTargetMode);
		sprintf(WIAC82_SS.AuxStr,"%s Dive Angle %8s [turn] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.DiveAngle,FLOAT_2POWER_17)),FuncValid->DiveAngle);
		SAL_CodeValid = SAL_GetFrequency(&SAL_Freq,WIAC82_SS.Func3D.FuncExch.SALCode);
		sprintf(WIAC82_SS.AuxStr,"%s SAL Code %8s [Hz] VALID = %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(SAL_Freq),SAL_CodeValid);
		sprintf(WIAC82_SS.AuxStr,"%s Fuze Delay %8s [ms] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.FuzeDelay,1)),FuncValid->FuzeSettingDelayHeight);
		sprintf(WIAC82_SS.AuxStr,"%s Fuze Height %8s [m] VALID: %d\n",
				WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.FuzeHeight,1)),FuncValid->FuzeSettingDelayHeight);
	}

	if (AUXCTRL_SS.WIAC82Controls.RxAuxFunction3D||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.Identifier);				// default 1
			sprintf(WIAC82_SS.AuxStr,"%s  UsefulLength      =   %4d\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.UsefulLength);			// Wagon header not to be considered as part of CRC
			sprintf(WIAC82_SS.AuxStr,"%s  Validity          = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.Validity);  				// FunctionalExchange2ValidityWord
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				FuncValid = (bFunctionalExchange2ValidityWord *)&WIAC82_SS.Func3D.FuncExch.Validity;
				sprintf(WIAC82_SS.AuxStr,"%s   TargetLongLatAlt           = %d\n",WIAC82_SS.AuxStr,FuncValid->TargetLongLatAlt);
				sprintf(WIAC82_SS.AuxStr,"%s   DiveAngle                  = %d\n",WIAC82_SS.AuxStr,FuncValid->DiveAngle);
				sprintf(WIAC82_SS.AuxStr,"%s   ImpactHeading              = %d\n",WIAC82_SS.AuxStr,FuncValid->ImpactHeading);
				sprintf(WIAC82_SS.AuxStr,"%s   ImpactSpeed                = %d\n",WIAC82_SS.AuxStr,FuncValid->ImpactSpeed);
				sprintf(WIAC82_SS.AuxStr,"%s   SALCode                    = %d\n",WIAC82_SS.AuxStr,FuncValid->SALCode);
				sprintf(WIAC82_SS.AuxStr,"%s   ImageIndexCRC              = %d\n",WIAC82_SS.AuxStr,FuncValid->ImageIndexCRC);
				sprintf(WIAC82_SS.AuxStr,"%s   MovingTargetMode           = %d\n",WIAC82_SS.AuxStr,FuncValid->MovingTargetMode);
				sprintf(WIAC82_SS.AuxStr,"%s   FuzeSettingDelayHeight     = %d\n",WIAC82_SS.AuxStr,FuncValid->FuzeSettingDelayHeight);
				sprintf(WIAC82_SS.AuxStr,"%s   GNSS_HybridationActivation = %d\n",WIAC82_SS.AuxStr,FuncValid->GNSS_HybridationActivation);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved                   = 0x%x\n",WIAC82_SS.AuxStr,FuncValid->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLatitude1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetLatitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLatitude2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetLatitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetLatitude1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLongitude1  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetLongitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetLongitude2  = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetLongitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetLongitude1,FLOAT_2POWER_32)));
			sprintf(WIAC82_SS.AuxStr,"%s  TargetAltitude1   = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetAltitude1);
			sprintf(WIAC82_SS.AuxStr,"%s  TargetAltitude2   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.TargetAltitude2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.Func3D.FuncExch.TargetAltitude1,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  DiveAngle         = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.DiveAngle);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.DiveAngle,FLOAT_2POWER_17)));
			sprintf(WIAC82_SS.AuxStr,"%s  ImpactHeading (Only AT) = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.ImpactHeading);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [turn]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatS(WIAC82_SS.Func3D.FuncExch.ImpactHeading,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  ImpactSpeed (Only AT)   = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.ImpactSpeed);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m/s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.ImpactSpeed,1)));
			sprintf(WIAC82_SS.AuxStr,"%s  SALCode           = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.SALCode);
			SAL_CodeValid = SAL_GetFrequency(&SAL_Freq,WIAC82_SS.Func3D.FuncExch.SALCode);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [Hz] Valid = %d\n",WIAC82_SS.AuxStr,AUXsprintfFloat(SAL_Freq),SAL_CodeValid);
			sprintf(WIAC82_SS.AuxStr,"%s  ImageIndex        = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.ImageIndex);
			sprintf(WIAC82_SS.AuxStr,"%s  ImageChecksum     = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.ImageChecksum);
			sprintf(WIAC82_SS.AuxStr,"%s  Mission_Settings  = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.MissionSettings); 			// FunctionalExchange2MissionSettings
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				MissionSettings = (bFunctionalExchange2MissionSettings *)&WIAC82_SS.Func3D.FuncExch.MissionSettings;
				sprintf(WIAC82_SS.AuxStr,"%s   SeekerSetting      = %d %s\n",WIAC82_SS.AuxStr,MissionSettings->SeekerSetting,DecodeWIAC82_SeekerSettings(MissionSettings->SeekerSetting));
				sprintf(WIAC82_SS.AuxStr,"%s   GNSSMode           = %d %s\n",WIAC82_SS.AuxStr,MissionSettings->GNSSMode,DecodeWIAC82_GNSS_Mode(MissionSettings->GNSSMode));
				sprintf(WIAC82_SS.AuxStr,"%s   MovingTargetActive = %d\n",WIAC82_SS.AuxStr,MissionSettings->MovingTargetActive);
				sprintf(WIAC82_SS.AuxStr,"%s   FuzeSetting        = %d %s\n",WIAC82_SS.AuxStr,MissionSettings->FuzeSetting,DecodeWIAC82_FuseSettings(MissionSettings->FuzeSetting));
				sprintf(WIAC82_SS.AuxStr,"%s   TargetRelative     = %d\n",WIAC82_SS.AuxStr,MissionSettings->TargetRelative);
				sprintf(WIAC82_SS.AuxStr,"%s   Reserved           = %d\n",WIAC82_SS.AuxStr,MissionSettings->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  FuzeDelay         = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.FuzeDelay);				// [ms] Max = 240
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [ms]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.FuzeDelay,1)));
			sprintf(WIAC82_SS.AuxStr,"%s  FuzeHeight        = 0x%4.4x",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.FuzeHeight);				// [m] Max = 200
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [m]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.Func3D.FuncExch.FuzeHeight,1)));
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved1         = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.Reserved1);
			sprintf(WIAC82_SS.AuxStr,"%s  Reserved2         = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.Reserved2);
			sprintf(WIAC82_SS.AuxStr,"%s  CRC               = 0x%4.4x\n",WIAC82_SS.AuxStr,WIAC82_SS.Func3D.FuncExch.CRC);							// Calculated from word 3 to Word 20
		}
	}
}
void WIAC82_HandleFunctionalExchange3DMsg22R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_FunctionalExchange3D_Message *Func3D;
	unsigned short CalcCRC;
	unsigned short OldCRC;
	unsigned short NewData;
	unsigned short CRC_OK;

	OldCRC = WIAC82_SS.Func3D.FuncExch.CRC;

	Func3D = (sWIAC82_1553_MLAN_FunctionalExchange3D_Message *)RxData;
	CalcCRC = CalculateMIL_STD_1760D_Crc((unsigned short *)&Func3D->FuncExch.Validity,((sizeof(sWIAC82_1553_FunctionalExchange3D_Message)/2)-3));
	if (OldCRC == CalcCRC)
		NewData = 0;
	else
		NewData = 1;
	if (CalcCRC == Func3D->FuncExch.CRC)
		CRC_OK = 1;
	else
		CRC_OK = 0;
	if (AUXCTRL_SS.WIAC82Controls.RxAuxFunction3D||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (22R) FUNCTIONAL_EXCHANGE_3D_MESSAGE RX - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx22RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,Func3D->MLANHeader.DataLength);
		if (Func3D->MLANHeader.DataLength == (sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)))
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]",WIAC82_SS.AuxStr,(sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)));
		sprintf(WIAC82_SS.AuxStr,"%s [CRC=0x%4.4x",WIAC82_SS.AuxStr,Func3D->FuncExch.CRC);
		if (CRC_OK)
			sprintf(WIAC82_SS.AuxStr,"%s OK]",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%x)]",WIAC82_SS.AuxStr,CalcCRC);
		if (NewData)
			sprintf(WIAC82_SS.AuxStr,"%s Data Changed",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s No Data Change",WIAC82_SS.AuxStr);
		sprintf(WIAC82_SS.AuxStr,"%s\n",WIAC82_SS.AuxStr);

		WIAC82_PrnMLANHeader(&Func3D->MLANHeader,0);
		WIAC82_Prn1553Header(&Func3D->MIL1553Header);
	}
	//if ((NewData) && (CRC_OK)) {
	if (CRC_OK) {
		WIAC82_UnpackFunctionalExchange3DMsg22R(RxData,Timestamp);
		SetMissionFromWIAC82(Func3D,CRC_OK);
		WIAC82_SS.CommsControls.Trigger23T = 1;
		WIAC82_SS.CommsControls.Trigger17T = 1;

	}
}
char *DecodeSatUsed(uint16_t Used) {
	static char strUsage[10];
	if (Used==0)
		sprintf(strUsage,"NOT USED");
	else
		sprintf(strUsage,"USED");
	return(strUsage);
}

char *DecodeSatStatus(uint16_t Status) {
	static char strStatus[30];

	if (Status == WAIC82_SATELLITE_STATUS_UNUSED) {
		sprintf(strStatus,"UNUSED");
	}
	else if (Status == WAIC82_SATELLITE_STATUS_SEARCHING){
		sprintf(strStatus,"SEARCHING");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_BIT_SYNC){
		sprintf(strStatus,"BIT SYNC");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_SUB_FRAME_SYNC){
		sprintf(strStatus,"SUB FRAME SYNC");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_LOCKED_C_A){
		sprintf(strStatus,"LOCKED C A");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_NEW_ACQUISITION_C_A){
		sprintf(strStatus,"NEW ACQUISITION C A");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_LOCKED_P_Y){
		sprintf(strStatus,"LOCKED P(Y)");
	}
	else if (Status== WAIC82_SATELLITE_STATUS_NEW_ACQUISITION_P_Y){
		sprintf(strStatus,"NEW ACQUISITION p(Y)");
	}
	else{
		sprintf(strStatus,"INVALID");
	}
	return(strStatus);
}
char *DecodeSatEphemerisStatus(uint16_t EphemerisStatus) {
	static char strEphemerisStatus[30];

	if (EphemerisStatus == WAIC82_SATELLITE_EPHEMERIS_STATUS_INVALID) {
		sprintf(strEphemerisStatus,"INVALID EPHEMERIS");
	}
	else if (EphemerisStatus == WAIC82_SATELLITE_EPHEMERIS_STATUS_VALID_UNUSED){
		sprintf(strEphemerisStatus,"VALID EPHEMERIS UNUSED");
	}
	else if (EphemerisStatus== WAIC82_SATELLITE_EPHEMERIS_STATUS_VALID_IN_USE){
		sprintf(strEphemerisStatus,"VALID EPHEMERIS IN USE");
	}
	else if (EphemerisStatus== WAIC82_SATELLITE_EPHEMERIS_STATUS_UNKNOWN){
		sprintf(strEphemerisStatus,"EPHEMERIS STATUS UNKNOWN");
	}
	else{
		sprintf(strEphemerisStatus,"INVALID VALUE");
	}
	return(strEphemerisStatus);
}
void WIAC82_HandleSatteliteUseMsg26R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Satellite_Use_Parameters_Message *SatUse;
	sWIAC82_Satellite_Use_Parameters_Validity *Validity;
	sWIAC82_Byte12 *AB_ID;
	sWIAC82_Satellite_Used1 *Used1;
	sWIAC82_Satellite_Used1 *Used2;
	sWIAC82_SatelliteAB_Status *ReceptionHealth;
	sWIAC82_Byte12 *SN;
	sWIAC82_Byte12 *DayMonth;
	sWIAC82_Byte12 *YearReserve;

	SatUse = (sWIAC82_1553_MLAN_Satellite_Use_Parameters_Message *)RxData;
	// Word 1 Validity
	WIAC82_SS.SatUse.SatParms.SatelliteValidity		= SatUse->SatParms.SatelliteValidity;
	// Word 2..3
	WIAC82_SS.SatUse.SatParms.UsedSatellites1		= SatUse->SatParms.UsedSatellites1;
	WIAC82_SS.SatUse.SatParms.UsedSatellites2		= SatUse->SatParms.UsedSatellites2;
	// Word 4 Satellite ID using channel 01 and 02
	WIAC82_SS.SatUse.SatParms.SatChannelsID01p02	= SatUse->SatParms.SatChannelsID01p02;
	// Word 5 Reception Health using channel 01 and 02
	WIAC82_SS.SatUse.SatParms.ReceptionHealth01p02	= SatUse->SatParms.ReceptionHealth01p02;
	// Word 6 Signal/Noise using channel 01 and 02
	WIAC82_SS.SatUse.SatParms.SignalNoise01p02		= SatUse->SatParms.SignalNoise01p02;
	// Word 7 Satellite ID using channel 03 and 04
	WIAC82_SS.SatUse.SatParms.SatChannelsID03p04	= SatUse->SatParms.SatChannelsID03p04;
	// Word 8 Reception Health using channel 03 and 04
	WIAC82_SS.SatUse.SatParms.ReceptionHealth03p04	= SatUse->SatParms.ReceptionHealth03p04;
	// Word 9 Signal/Noise using channel 03 and 04
	WIAC82_SS.SatUse.SatParms.SignalNoise03p04		= SatUse->SatParms.SignalNoise03p04;
	// Word 10 Satellite ID using channel 05 and 06
	WIAC82_SS.SatUse.SatParms.SatChannelsID05p06	= SatUse->SatParms.SatChannelsID05p06;
	// Word 11 Reception Health using channel 05 and 06
	WIAC82_SS.SatUse.SatParms.ReceptionHealth05p06	= SatUse->SatParms.ReceptionHealth05p06;
	// Word 12 Signal/Noise using channel 05 and 06
	WIAC82_SS.SatUse.SatParms.SignalNoise05p06		= SatUse->SatParms.SignalNoise05p06;
	// Word 13 Satellite ID using channel 07 and 08
	WIAC82_SS.SatUse.SatParms.SatChannelsID07p08	= SatUse->SatParms.SatChannelsID07p08;
	// Word 14 Reception Health using channel 07 and 08
	WIAC82_SS.SatUse.SatParms.ReceptionHealth07p08	= SatUse->SatParms.ReceptionHealth07p08;
	// Word 15 Signal/Noise using channel 07 and 08
	WIAC82_SS.SatUse.SatParms.SignalNoise07p08		= SatUse->SatParms.SignalNoise07p08;
	// Word 16 Satellite ID using channel 09 and 10
	WIAC82_SS.SatUse.SatParms.SatChannelsID09p10	= SatUse->SatParms.SatChannelsID09p10;
	// Word 17 Reception Health using channel 00 and 10
	WIAC82_SS.SatUse.SatParms.ReceptionHealth09p10	= SatUse->SatParms.ReceptionHealth09p10;
	// Word 18 Signal/Noise using channel 09 and 10
	WIAC82_SS.SatUse.SatParms.SignalNoise09p10		= SatUse->SatParms.SignalNoise09p10;
	// Word 19 Satellite ID using channel 11 and 12
	WIAC82_SS.SatUse.SatParms.SatChannelsID11p12	= SatUse->SatParms.SatChannelsID11p12;
	// Word 20 Reception Health using channel 11 and 12
	WIAC82_SS.SatUse.SatParms.ReceptionHealth11p12	= SatUse->SatParms.ReceptionHealth11p12;
	// Word 21 Signal/Noise using channel 11 and 12
	WIAC82_SS.SatUse.SatParms.SignalNoise11p12		= SatUse->SatParms.SignalNoise11p12;
	// Word 22..23
	WIAC82_SS.SatUse.SatParms.TimeTagGPSInfo1		= SatUse->SatParms.TimeTagGPSInfo1;
	WIAC82_SS.SatUse.SatParms.TimeTagGPSInfo2		= SatUse->SatParms.TimeTagGPSInfo2;
	// Word 24..25
	WIAC82_SS.SatUse.SatParms.GPSHour1				= SatUse->SatParms.GPSHour1;
	WIAC82_SS.SatUse.SatParms.GPSHour2				= SatUse->SatParms.GPSHour2;
	// Word 26
	WIAC82_SS.SatUse.SatParms.GPSHourFraction		= SatUse->SatParms.GPSHourFraction;
	// Word 27..28 GPS Day Month Year
	WIAC82_SS.SatUse.SatParms.DayMonth				= SatUse->SatParms.DayMonth;
	WIAC82_SS.SatUse.SatParms.YearReserve			= SatUse->SatParms.YearReserve;

	if (AUXCTRL_SS.WIAC82Controls.RxAuxSatUse||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (26R) SATELLITE_USE_PARAMETERS RX - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx26RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,SatUse->MLANHeader.DataLength);
		if (SatUse->MLANHeader.DataLength == (sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)))
			sprintf(WIAC82_SS.AuxStr,"%s OK]\n",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]\n",WIAC82_SS.AuxStr,(sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)));

		WIAC82_PrnMLANHeader(&SatUse->MLANHeader,0);
		WIAC82_Prn1553Header(&SatUse->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  SatelliteValidity     = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatelliteValidity);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Validity = (sWIAC82_Satellite_Use_Parameters_Validity *)(&WIAC82_SS.SatUse.SatParms.SatelliteValidity);
				sprintf(WIAC82_SS.AuxStr,"%s    NumSAT      = %d\n",WIAC82_SS.AuxStr, Validity->valNumSAT);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC01_02  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC01_02);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC03_04  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC03_04);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC05_06  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC05_06);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC07_08  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC07_08);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC09_10  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC09_10);
				sprintf(WIAC82_SS.AuxStr,"%s    NSATC11_12  = %d\n",WIAC82_SS.AuxStr, Validity->valNSATC11_12);
				sprintf(WIAC82_SS.AuxStr,"%s    DPACI       = %d\n",WIAC82_SS.AuxStr, Validity->valDPACI);
				sprintf(WIAC82_SS.AuxStr,"%s    HGPS        = %d\n",WIAC82_SS.AuxStr, Validity->valHGPS);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved    = 0x%x\n",WIAC82_SS.AuxStr, Validity->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  UsedSatellites1       = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.UsedSatellites1);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Used1 = (sWIAC82_Satellite_Used1 *)(&WIAC82_SS.SatUse.SatParms.UsedSatellites1);
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite01 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite01,DecodeSatUsed(Used1->Satellite01));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite02 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite02,DecodeSatUsed(Used1->Satellite02));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite03 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite03,DecodeSatUsed(Used1->Satellite03));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite04 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite04,DecodeSatUsed(Used1->Satellite04));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite05 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite05,DecodeSatUsed(Used1->Satellite05));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite06 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite06,DecodeSatUsed(Used1->Satellite06));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite07 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite07,DecodeSatUsed(Used1->Satellite07));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite08 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite08,DecodeSatUsed(Used1->Satellite08));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite09 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite09,DecodeSatUsed(Used1->Satellite09));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite10 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite10,DecodeSatUsed(Used1->Satellite10));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite11 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite11,DecodeSatUsed(Used1->Satellite11));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite12 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite12,DecodeSatUsed(Used1->Satellite12));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite13 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite13,DecodeSatUsed(Used1->Satellite13));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite14 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite14,DecodeSatUsed(Used1->Satellite14));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite15 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite15,DecodeSatUsed(Used1->Satellite15));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite16 = %d %s\n",WIAC82_SS.AuxStr, Used1->Satellite16,DecodeSatUsed(Used1->Satellite16));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  UsedSatellites2       = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.UsedSatellites2);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				Used2 = (sWIAC82_Satellite_Used1 *)(&WIAC82_SS.SatUse.SatParms.UsedSatellites2);
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite17 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite01,DecodeSatUsed(Used2->Satellite01));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite18 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite02,DecodeSatUsed(Used2->Satellite02));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite19 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite03,DecodeSatUsed(Used2->Satellite03));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite20 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite04,DecodeSatUsed(Used2->Satellite04));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite21 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite05,DecodeSatUsed(Used2->Satellite05));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite22 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite06,DecodeSatUsed(Used2->Satellite06));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite23 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite07,DecodeSatUsed(Used2->Satellite07));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite24 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite08,DecodeSatUsed(Used2->Satellite08));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite25 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite09,DecodeSatUsed(Used2->Satellite09));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite26 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite10,DecodeSatUsed(Used2->Satellite10));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite27 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite11,DecodeSatUsed(Used2->Satellite11));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite28 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite12,DecodeSatUsed(Used2->Satellite12));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite29 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite13,DecodeSatUsed(Used2->Satellite13));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite30 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite14,DecodeSatUsed(Used2->Satellite14));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite31 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite15,DecodeSatUsed(Used2->Satellite15));
				sprintf(WIAC82_SS.AuxStr,"%s    Satellite32 = %d %s\n",WIAC82_SS.AuxStr, Used2->Satellite16,DecodeSatUsed(Used2->Satellite16));
			}
			sprintf(WIAC82_SS.AuxStr,"%s  ID Channel 01 & 02	= 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID01p02);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID01p02);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 01 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 02 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth01p02  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth01p02);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth01p02);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 01 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 01 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 02 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 02 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise01p02      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise01p02);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise01p02);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 01 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 02 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SatChannelsID03p04    = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID03p04);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID03p04);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}

			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth03p04  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth03p04);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth03p04);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise03p04      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise03p04);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise03p04);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SatChannelsID05p06    = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID05p06);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID05p06);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 05 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 06 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}

			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth05p06  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth05p06);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth03p04);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 03 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 04 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise05p06      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise05p06);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise05p06);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 05 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 06 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SatChannelsID07p08    = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID07p08);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID07p08);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 07 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 08 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}

			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth07p08  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth07p08);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth07p08);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 07 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 07 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 08 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 08 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise07p08      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise07p08);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise07p08);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 07 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 08 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SatChannelsID09p10    = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID09p10);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID09p10);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 09 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 10 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}

			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth09p10  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth09p10);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth09p10);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 09 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 09 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 10 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 10 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise09p10      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise09p10);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise09p10);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 09 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 10 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SatChannelsID11p12    = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SatChannelsID11p12);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				AB_ID = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SatChannelsID11p12);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 11 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte1, AB_ID->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 12 ID = %1.2d 0x%2.2x\n",WIAC82_SS.AuxStr, AB_ID->Byte2, AB_ID->Byte2);
			}

			sprintf(WIAC82_SS.AuxStr,"%s  ReceptionHealth11p12  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.ReceptionHealth11p12);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				ReceptionHealth = (sWIAC82_SatelliteAB_Status *)(&WIAC82_SS.SatUse.SatParms.ReceptionHealth11p12);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 11 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAStatus, DecodeSatStatus(ReceptionHealth->ChanAStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 11 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanAEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanAEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 12 Status           = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBStatus, DecodeSatStatus(ReceptionHealth->ChanBStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 12 Ephemeris Status = 0x%2.2x %s\n",WIAC82_SS.AuxStr, ReceptionHealth->ChanBEphemerisStatus, DecodeSatEphemerisStatus(ReceptionHealth->ChanBEphemerisStatus));
				sprintf(WIAC82_SS.AuxStr,"%s    Reserved                 = 0x%2.2x\n",WIAC82_SS.AuxStr, ReceptionHealth->Reserved);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  SignalNoise11p12      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.SignalNoise11p12);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				SN = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.SignalNoise11p12);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 11 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Chan 12 SN = %d [db/Hz]\n",WIAC82_SS.AuxStr, SN->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  TimeTagGPSInfo1       = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.TimeTagGPSInfo1);
			sprintf(WIAC82_SS.AuxStr,"%s  TimeTagGPSInfo2       = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.TimeTagGPSInfo2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.SatUse.SatParms.TimeTagGPSInfo1,FLOAT_2POWER_5)));
			sprintf(WIAC82_SS.AuxStr,"%s  GPSHour1              = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.GPSHour1);
			sprintf(WIAC82_SS.AuxStr,"%s  GPSHour2              = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.GPSHour2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatU(&WIAC82_SS.SatUse.SatParms.GPSHour1,FLOAT_2POWER_0)));
			sprintf(WIAC82_SS.AuxStr,"%s  GPSHourFraction       = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.GPSHourFraction);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.SatUse.SatParms.GPSHourFraction,FLOAT_2POWER_16)));
			sprintf(WIAC82_SS.AuxStr,"%s  DayMonth              = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.DayMonth);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				DayMonth = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.DayMonth);
				sprintf(WIAC82_SS.AuxStr,"%s    Day   = %d\n",WIAC82_SS.AuxStr, DayMonth->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Month = %d\n",WIAC82_SS.AuxStr, DayMonth->Byte2);
			}
			sprintf(WIAC82_SS.AuxStr,"%s  YearReserve           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.SatUse.SatParms.YearReserve);
			if (AUXCTRL_SS.WIAC82Controls.RxDetail2AuxOn) {
				YearReserve = (sWIAC82_Byte12 *)(&WIAC82_SS.SatUse.SatParms.YearReserve);
				sprintf(WIAC82_SS.AuxStr,"%s    Year    = %d\n",WIAC82_SS.AuxStr, YearReserve->Byte1);
				sprintf(WIAC82_SS.AuxStr,"%s    Reserve = %d\n",WIAC82_SS.AuxStr, YearReserve->Byte2);
			}
		}
	}
}
void WIAC82_HandleGPS_InitializationALMANAC_Msg29R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_GPS_InitializationALMANAC_Message *ALMAC;

	ALMAC = (sWIAC82_1553_MLAN_GPS_InitializationALMANAC_Message *)RxData;
	// Word 1 Almanac GPS
	WIAC82_SS.ALMAC.Init.Identifier						= ALMAC->Init.Identifier;						// default 2
	// Word 2
	WIAC82_SS.ALMAC.Init.UsefulLength					= ALMAC->Init.UsefulLength;						// 38
	// Word 3
	WIAC82_SS.ALMAC.Init.Validity						= ALMAC->Init.Validity;
	// Word 4
	WIAC82_SS.ALMAC.Init.PseudoRandomNoise				= ALMAC->Init.PseudoRandomNoise;
	// Word 5
	WIAC82_SS.ALMAC.Init.WeekNumber						= ALMAC->Init.WeekNumber;
	// Word 6
	WIAC82_SS.ALMAC.Init.ReferenceTime					= ALMAC->Init.ReferenceTime;
	// Word 7
	WIAC82_SS.ALMAC.Init.SpaceVehicleHealth				= ALMAC->Init.SpaceVehicleHealth;
	// Word 8
	WIAC82_SS.ALMAC.Init.InclinationCorrection			= ALMAC->Init.InclinationCorrection;
	// Word 9
	WIAC82_SS.ALMAC.Init.EccentricAnomaly				= ALMAC->Init.EccentricAnomaly;
	// Word 10..11
	WIAC82_SS.ALMAC.Init.SQRTSemiMajorAxis1				= ALMAC->Init.SQRTSemiMajorAxis1;
	WIAC82_SS.ALMAC.Init.SQRTSemiMajorAxis2				= ALMAC->Init.SQRTSemiMajorAxis2;
	// Word 12..13
	WIAC82_SS.ALMAC.Init.MeanAnomaly1					= ALMAC->Init.MeanAnomaly1;
	WIAC82_SS.ALMAC.Init.MeanAnomaly2					= ALMAC->Init.MeanAnomaly2;
	// Word 14
	WIAC82_SS.ALMAC.Init.RateRightAscension				= ALMAC->Init.RateRightAscension;
	// Word 15..16
	WIAC82_SS.ALMAC.Init.LongitudeAscendingNode1		= ALMAC->Init.LongitudeAscendingNode1;
	WIAC82_SS.ALMAC.Init.LongitudeAscendingNode2		= ALMAC->Init.LongitudeAscendingNode2;
	// Word 17..18
	WIAC82_SS.ALMAC.Init.ArgumentPerigee1				= ALMAC->Init.ArgumentPerigee1;
	WIAC82_SS.ALMAC.Init.ArgumentPerigee2				= ALMAC->Init.ArgumentPerigee2;
	// Word 19..20
	WIAC82_SS.ALMAC.Init.AlmanacParametersSubframeA		= ALMAC->Init.AlmanacParametersSubframeA;
	WIAC82_SS.ALMAC.Init.AlmanacParametersSubframeB		= ALMAC->Init.AlmanacParametersSubframeB;
	// Word 21..22
	WIAC82_SS.ALMAC.Init.Almanac1ParametersSubframeA	= ALMAC->Init.Almanac1ParametersSubframeA;
	WIAC82_SS.ALMAC.Init.Almanac1ParametersSubframeB	= ALMAC->Init.Almanac1ParametersSubframeB;

	if (AUXCTRL_SS.WIAC82Controls.RxAuxALMAC||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (29R) GPS ALMAC INITIALIZATION_MESSAGE RX - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx29RMsgCnt);
		sprintf(WIAC82_SS.AuxStr,"%s [Size=%d",WIAC82_SS.AuxStr,ALMAC->MLANHeader.DataLength);
		if (ALMAC->MLANHeader.DataLength == (sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)))
			sprintf(WIAC82_SS.AuxStr,"%s OK]\n",WIAC82_SS.AuxStr);
		else
			sprintf(WIAC82_SS.AuxStr,"%s NOK(%d)]\n",WIAC82_SS.AuxStr,(sizeof(sWIAC82_1553_MLAN_Message) - sizeof(sWIAC82_MLAN_Header)));

		WIAC82_PrnMLANHeader(&ALMAC->MLANHeader,0);
		WIAC82_Prn1553Header(&ALMAC->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier                   = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.Identifier);						// default 2
			sprintf(WIAC82_SS.AuxStr,"%s  UsefulLength                 = 0x%4.4x %d\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.UsefulLength, WIAC82_SS.ALMAC.Init.UsefulLength);						// 38
			sprintf(WIAC82_SS.AuxStr,"%s  Validity                     = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.Validity);
			sprintf(WIAC82_SS.AuxStr,"%s  PseudoRandomNoise            = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.PseudoRandomNoise);
			sprintf(WIAC82_SS.AuxStr,"%s  WeekNumber                   = 0x%4.4x %d\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.WeekNumber, WIAC82_SS.ALMAC.Init.WeekNumber);
			sprintf(WIAC82_SS.AuxStr,"%s  ReferenceTime                = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.ReferenceTime);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get1WordFloatU(WIAC82_SS.ALMAC.Init.ReferenceTime,FLOAT_2POWER_12)));

			sprintf(WIAC82_SS.AuxStr,"%s  SpaceVehicleHealth           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.SpaceVehicleHealth);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationCorrection        = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.InclinationCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  EccentricAnomaly             = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.EccentricAnomaly);
			sprintf(WIAC82_SS.AuxStr,"%s  SQRTSemiMajorAxis1           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.SQRTSemiMajorAxis1);
			sprintf(WIAC82_SS.AuxStr,"%s  SQRTSemiMajorAxis2           = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.SQRTSemiMajorAxis2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.SQRTSemiMajorAxis1,FLOAT_2POWER_11)));

			sprintf(WIAC82_SS.AuxStr,"%s  MeanAnomaly1                 = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.MeanAnomaly1);
			sprintf(WIAC82_SS.AuxStr,"%s  MeanAnomaly2                 = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.MeanAnomaly2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.MeanAnomaly1,FLOAT_2POWER_23)));

			sprintf(WIAC82_SS.AuxStr,"%s  RateRightAscension           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.RateRightAscension);
			sprintf(WIAC82_SS.AuxStr,"%s  LongitudeAscendingNode1      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.LongitudeAscendingNode1);
			sprintf(WIAC82_SS.AuxStr,"%s  LongitudeAscendingNode2      = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.LongitudeAscendingNode2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.LongitudeAscendingNode1,FLOAT_2POWER_23)));

			sprintf(WIAC82_SS.AuxStr,"%s  ArgumentPerigee1             = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.ArgumentPerigee1);
			sprintf(WIAC82_SS.AuxStr,"%s  ArgumentPerigee2             = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.ArgumentPerigee2);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.ArgumentPerigee1,FLOAT_2POWER_23)));

			sprintf(WIAC82_SS.AuxStr,"%s  AlmanacParametersSubframeA   = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.AlmanacParametersSubframeA);
			sprintf(WIAC82_SS.AuxStr,"%s  AlmanacParametersSubframeB   = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.AlmanacParametersSubframeB);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s [s]\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.AlmanacParametersSubframeA,FLOAT_2POWER_20)));

			sprintf(WIAC82_SS.AuxStr,"%s  Almanac1ParametersSubframeA  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.Almanac1ParametersSubframeA);
			sprintf(WIAC82_SS.AuxStr,"%s  Almanac1ParametersSubframeB  = 0x%4.4x",WIAC82_SS.AuxStr, WIAC82_SS.ALMAC.Init.Almanac1ParametersSubframeB);
			sprintf(WIAC82_SS.AuxStr,"%s  %8s\n",WIAC82_SS.AuxStr,AUXsprintfFloat(Get2WordFloatS(&WIAC82_SS.ALMAC.Init.Almanac1ParametersSubframeA,FLOAT_2POWER_38)));
		}
	}
}
void WIAC82_HandleGPS_InitializationEPHEMERIDES_Msg29R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message *EPHEMERIDES;

	EPHEMERIDES = (sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message *)RxData;
	// Word 1 Ephemeris GPS
	WIAC82_SS.EPHEMERIDES.Init.Identifier						= EPHEMERIDES->Init.Identifier;						// default 3
	// Word 2
	WIAC82_SS.EPHEMERIDES.Init.UsefulLength						= EPHEMERIDES->Init.UsefulLength;						// 56
	// Word 3
	WIAC82_SS.EPHEMERIDES.Init.Validity							= EPHEMERIDES->Init.Validity;
	// Word 4
	WIAC82_SS.EPHEMERIDES.Init.SatelliteID						= EPHEMERIDES->Init.SatelliteID;
	// Word 5
	WIAC82_SS.EPHEMERIDES.Init.WeekNumber						= EPHEMERIDES->Init.WeekNumber;
	// Word 6
	WIAC82_SS.EPHEMERIDES.Init.ReferenceTime					= EPHEMERIDES->Init.ReferenceTime;
	// Word 7
	WIAC82_SS.EPHEMERIDES.Init.SpaceVehicleHealth				= EPHEMERIDES->Init.SpaceVehicleHealth;
	// Word 8..9
	WIAC82_SS.EPHEMERIDES.Init.Eccentricity1					= EPHEMERIDES->Init.Eccentricity1;
	WIAC82_SS.EPHEMERIDES.Init.Eccentricity2					= EPHEMERIDES->Init.Eccentricity2;
	// Word 10..11
	WIAC82_SS.EPHEMERIDES.Init.SQRTSemiMajorAxis1				= EPHEMERIDES->Init.SQRTSemiMajorAxis1;
	WIAC82_SS.EPHEMERIDES.Init.SQRTSemiMajorAxis2				= EPHEMERIDES->Init.SQRTSemiMajorAxis2;
	// Word 12..13
	WIAC82_SS.EPHEMERIDES.Init.MeanAnomaly1						= EPHEMERIDES->Init.MeanAnomaly1;
	WIAC82_SS.EPHEMERIDES.Init.MeanAnomaly2						= EPHEMERIDES->Init.MeanAnomaly2;
	// Word 14..15
	WIAC82_SS.EPHEMERIDES.Init.RateRightAscension1				= EPHEMERIDES->Init.RateRightAscension1;
	WIAC82_SS.EPHEMERIDES.Init.RateRightAscension2				= EPHEMERIDES->Init.RateRightAscension2;
	// Word 16..17
	WIAC82_SS.EPHEMERIDES.Init.LongitudeAscendingNode1			= EPHEMERIDES->Init.LongitudeAscendingNode1;
	WIAC82_SS.EPHEMERIDES.Init.LongitudeAscendingNode2			= EPHEMERIDES->Init.LongitudeAscendingNode2;
	// Word 18..19
	WIAC82_SS.EPHEMERIDES.Init.ArgumentPerigee1					= EPHEMERIDES->Init.ArgumentPerigee1;
	WIAC82_SS.EPHEMERIDES.Init.ArgumentPerigee2					= EPHEMERIDES->Init.ArgumentPerigee2;
	// Word 20..21
	WIAC82_SS.EPHEMERIDES.Init.InclinationAngleReferenceTime1	= EPHEMERIDES->Init.InclinationAngleReferenceTime1;
	WIAC82_SS.EPHEMERIDES.Init.InclinationAngleReferenceTime2	= EPHEMERIDES->Init.InclinationAngleReferenceTime2;
	// Word 22
	WIAC82_SS.EPHEMERIDES.Init.DataIssue						= EPHEMERIDES->Init.DataIssue;
	// Word 23
	WIAC82_SS.EPHEMERIDES.Init.InclinationAngleRate				= EPHEMERIDES->Init.InclinationAngleRate;
	// Word 24
	WIAC82_SS.EPHEMERIDES.Init.MeanMotionDiff					= EPHEMERIDES->Init.MeanMotionDiff;
	// Word 25
	WIAC82_SS.EPHEMERIDES.Init.LatCosHarmonicCorrection			= EPHEMERIDES->Init.LatCosHarmonicCorrection;
	// Word 26
	WIAC82_SS.EPHEMERIDES.Init.LatSinHarmonicCorrection			= EPHEMERIDES->Init.LatSinHarmonicCorrection;
	// Word 27
	WIAC82_SS.EPHEMERIDES.Init.RadiusCosHarmonicCorrection		= EPHEMERIDES->Init.RadiusCosHarmonicCorrection;
	// Word 28
	WIAC82_SS.EPHEMERIDES.Init.RadiusSinHarmonicCorrection		= EPHEMERIDES->Init.RadiusSinHarmonicCorrection;
	// Word 29
	WIAC82_SS.EPHEMERIDES.Init.InclinationCosHarmonicCorrection	= EPHEMERIDES->Init.InclinationCosHarmonicCorrection;
	// Word 30
	WIAC82_SS.EPHEMERIDES.Init.InclinationSinsHarmonicCorrection= EPHEMERIDES->Init.InclinationSinsHarmonicCorrection;

	if (AUXCTRL_SS.WIAC82Controls.RxAuxEPHEMERIDES||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (29R) GPS EPHEMERIDES INITIALIZATION_MESSAGE received - %u\n",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx29RMsgCnt);
		WIAC82_PrnMLANHeader(&EPHEMERIDES->MLANHeader,0);
		WIAC82_Prn1553Header(&EPHEMERIDES->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Identifier                         = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.Identifier);			// default 3
			sprintf(WIAC82_SS.AuxStr,"%s  UsefulLength                       = 0x%4.4x %d\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.UsefulLength, WIAC82_SS.EPHEMERIDES.Init.UsefulLength);			// 56
			sprintf(WIAC82_SS.AuxStr,"%s  Validity                           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.Validity);
			sprintf(WIAC82_SS.AuxStr,"%s  SatelliteID                        = 0x%4.4x %d\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.SatelliteID, WIAC82_SS.EPHEMERIDES.Init.SatelliteID);
			sprintf(WIAC82_SS.AuxStr,"%s  WeekNumber                         = 0x%4.4x %d\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.WeekNumber, WIAC82_SS.EPHEMERIDES.Init.WeekNumber);
			sprintf(WIAC82_SS.AuxStr,"%s  ReferenceTime                      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.ReferenceTime);
			sprintf(WIAC82_SS.AuxStr,"%s  SpaceVehicleHealth                 = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.SpaceVehicleHealth);
			sprintf(WIAC82_SS.AuxStr,"%s  Eccentricity1                      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.Eccentricity1);
			sprintf(WIAC82_SS.AuxStr,"%s  Eccentricity2                      = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.Eccentricity2);
			sprintf(WIAC82_SS.AuxStr,"%s  SQRTSemiMajorAxis1                 = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.SQRTSemiMajorAxis1);
			sprintf(WIAC82_SS.AuxStr,"%s  SQRTSemiMajorAxis2                 = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.SQRTSemiMajorAxis2);
			sprintf(WIAC82_SS.AuxStr,"%s  MeanAnomaly1                       = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.MeanAnomaly1);
			sprintf(WIAC82_SS.AuxStr,"%s  MeanAnomaly2                       = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.MeanAnomaly2);
			sprintf(WIAC82_SS.AuxStr,"%s  RateRightAscension1                = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.RateRightAscension1);
			sprintf(WIAC82_SS.AuxStr,"%s  RateRightAscension2                = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.RateRightAscension2);
			sprintf(WIAC82_SS.AuxStr,"%s  LongitudeAscendingNode1            = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.LongitudeAscendingNode1);
			sprintf(WIAC82_SS.AuxStr,"%s  LongitudeAscendingNode2            = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.LongitudeAscendingNode2);
			sprintf(WIAC82_SS.AuxStr,"%s  ArgumentPerigee1                   = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.ArgumentPerigee1);
			sprintf(WIAC82_SS.AuxStr,"%s  ArgumentPerigee2                   = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.ArgumentPerigee2);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationAngleReferenceTime1     = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.InclinationAngleReferenceTime1);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationAngleReferenceTime2     = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.InclinationAngleReferenceTime2);
			sprintf(WIAC82_SS.AuxStr,"%s  DataIssue                          = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.DataIssue);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationAngleRate               = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.InclinationAngleRate);
			sprintf(WIAC82_SS.AuxStr,"%s  MeanMotionDiff                     = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.MeanMotionDiff);
			sprintf(WIAC82_SS.AuxStr,"%s  LatCosHarmonicCorrection           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.LatCosHarmonicCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  LatSinHarmonicCorrection           = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.LatSinHarmonicCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  RadiusCosHarmonicCorrection        = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.RadiusCosHarmonicCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  RadiusSinHarmonicCorrection        = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.RadiusSinHarmonicCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationCosHarmonicCorrection   = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.InclinationCosHarmonicCorrection);
			sprintf(WIAC82_SS.AuxStr,"%s  InclinationSinsHarmonicCorrection  = 0x%4.4x\n",WIAC82_SS.AuxStr, WIAC82_SS.EPHEMERIDES.Init.InclinationSinsHarmonicCorrection);
		}
	}
}
void WIAC82_HandleGPS_Initialization_Msg(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message *EPHEMERIDES;

	EPHEMERIDES = (sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message *)RxData;
	if (EPHEMERIDES->Init.Identifier==2){
		WIAC82_HandleGPS_InitializationALMANAC_Msg29R(RxData,Timestamp);
	}
	else if (EPHEMERIDES->Init.Identifier==3){
		WIAC82_HandleGPS_InitializationEPHEMERIDES_Msg29R(RxData,Timestamp);
	}
	else if (AUXCTRL_SS.WIAC82Controls.ErrDetectOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 UNKNOWN 1553 GPS Initialization Message Received ID =%d\n",WIAC82_SS.AuxStr,GetTimeUs(),EPHEMERIDES->Init.Identifier);
	}
}
void WIAC82_HandleEquipmentDialogueMsg30R(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_EquipmentDiaglogueTest_Message *BC_TEST;

	BC_TEST = (sWIAC82_1553_MLAN_EquipmentDiaglogueTest_Message *)RxData;
	WIAC82_SS.BC_TEST.Test.Word						= BC_TEST->Test.Word;
	if (AUXCTRL_SS.WIAC82Controls.RxAuxBC_TEST||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 RX (30R) EQUIPMENT_DIALOGUE_TEST_MESSAGE received - %u",WIAC82_SS.AuxStr,GetTimeUs(),WIAC82_SS.RxCounters.Rx30RMsgCnt);
		WIAC82_PrnMLANHeader(&BC_TEST->MLANHeader,0);
		WIAC82_Prn1553Header(&BC_TEST->MIL1553Header);
		if (AUXCTRL_SS.WIAC82Controls.RxDetail1AuxOn) {
			sprintf(WIAC82_SS.AuxStr,"%s  Word  = 0x%x\n",WIAC82_SS.AuxStr, WIAC82_SS.BC_TEST.Test.Word); 	//Cycling of the three words each time a message is sent
		}																														//0xA55A
	}
}
void WIAC82_HandleBC_WPNmsg(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Message *Message;

	Message = (sWIAC82_1553_MLAN_Message *)RxData;
	if (AUXCTRL_SS.WIAC82Controls.RxAuxBC_WPN) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 BC to WPN Message RX SubAddr=%dR Len=%d\n",WIAC82_SS.AuxStr,GetTimeUs(),Message->MIL1553Header.SubAddr,Message->MLANHeader.DataLength);
		WIAC82_PrnMLANHeader(&Message->MLANHeader,1);
		WIAC82_Prn1553Header(&Message->MIL1553Header);
	}
	switch (Message->MIL1553Header.SubAddr) {
    	case WIAC82_AIRCRAFT_DESCIPTION_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx1RMsgCnt++;
    		WIAC82_HandleAC_DescriptionMsg1R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_SYSTEM_TIME_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx2RMsgCnt++;
    		WIAC82_HandleAC_SystemTimeMsg2R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_GENERAL_REFUSAL_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx3RMsgCnt++;
    		WIAC82_HandleGeneralRefusalMsg3R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_WEAPON_CONFIGURATION_MESSAGE_8R :
    	case WIAC82_WEAPON_CONFIGURATION_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx10RMsgCnt++;
    		WIAC82_HandleWeaponConfigMsg10R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_STORE_CONTROL_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx11RMsgCnt++;
    		WIAC82_HandleWeaponStoreControlMsg11R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_STATE_VECTOR_50HZ : {
    		WIAC82_SS.RxCounters.Rx14RMsgCnt++;
    		WIAC82_HandleStateVector50HzMsg14R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_STATE_VECTOR_6P25HZ_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx16RMsgCnt++;
    		WIAC82_HandleStateVector6p25HzMsg16R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_FUNCTIONAL_EXCHANGE_1D_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx21RMsgCnt++;
    		WIAC82_HandleFunctionalExchange1DMsg21R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_FUNCTIONAL_EXCHANGE_3D_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx22RMsgCnt++;
    		WIAC82_HandleFunctionalExchange3DMsg22R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_SATELLITE_USE_PARAMETERS : {
    		WIAC82_SS.RxCounters.Rx26RMsgCnt++;
    		WIAC82_HandleSatteliteUseMsg26R(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_GPS_INITIALIZATION_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx29RMsgCnt++;
    		WIAC82_HandleGPS_Initialization_Msg(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_EQUIPMENT_DIALOGUE_TEST_MESSAGE : {
    		WIAC82_SS.RxCounters.Rx30RMsgCnt++;
    		WIAC82_HandleEquipmentDialogueMsg30R(RxData,Timestamp);
    		break;
    	}
    	default : {
    		if (AUXCTRL_SS.WIAC82Controls.ErrDetectOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received UNKNOWN 1553 Message SubAddress =[%d 0x%x] Length=%d\n",WIAC82_SS.AuxStr,GetTimeUs(),Message->MIL1553Header.SubAddr,Message->MIL1553Header.SubAddr,Message->MLANHeader.DataLength);
    		}

    		break;
    	}
	}
}
void WIAC82_HandleBCDataReqmsg(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Message *Message;

	Message = (sWIAC82_1553_MLAN_Message *)RxData;
	if (AUXCTRL_SS.WIAC82Controls.RxAuxBCDataReq||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 BC Data Request Message RX SubAddr=%dR Len=%d\n",WIAC82_SS.AuxStr,GetTimeUs(),Message->MIL1553Header.SubAddr,Message->MLANHeader.DataLength);
		WIAC82_PrnMLANHeader(&Message->MLANHeader,1);
		WIAC82_Prn1553Header(&Message->MIL1553Header);
	}
}
void WIAC82_HandleBC_WPN_Cmdmsg(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_1553_MLAN_Message *Message;

	Message = (sWIAC82_1553_MLAN_Message *)RxData;
	if (AUXCTRL_SS.WIAC82Controls.RxAuxBCWPNcmd||AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
		sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 BC BC WPN Command Message RX SubAddr=%dR Len=%d\n",WIAC82_SS.AuxStr,GetTimeUs(),Message->MIL1553Header.SubAddr,Message->MLANHeader.DataLength);
		WIAC82_PrnMLANHeader(&Message->MLANHeader,1);
		WIAC82_Prn1553Header(&Message->MIL1553Header);
	}
}

void WIAC82HandleRxData(unsigned char *RxData, unsigned int Timestamp) {
	sWIAC82_MLAN_Header *MLANHeader;

	WIAC82_SS.RxTimestamp = Weapon.SystemTime;
	WIAC82_SS.RxCnt++;

	MLANHeader = (sWIAC82_MLAN_Header *)RxData;
	switch (MLANHeader->Opcode) {
    	case WIAC82_RX_ID_SBIT : {
    		if (AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received %d:0x%2.2x SBIT Message from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		WIAC82_SS.WIAC82WaitForRxMsg = FALSE;
    		WIAC82_HandleSBITmsg(RxData, Timestamp);

    		break;
    	}
    	case WIAC82_RX_ID_STATUS : {
    		if (AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received %d:0x%2.2x STATUS Message from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		WIAC82_SS.WIAC82WaitForRxMsg = FALSE;
    		WIAC82_HandleSTATUSmsg(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_RX_MLAN_1553_BC_WPN_MESSAGE : {
    		if (AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received %d:0x%2.2x BC to WPN Message from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		WIAC82_HandleBC_WPNmsg(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_RX_MLAN_1553_BC_WPN_DATA_REQ : {
    		if (AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received %d:0x%2.2x BC to WPN Data Request Message from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		WIAC82_HandleBCDataReqmsg(RxData,Timestamp);
    		break;
    	}
    	case WIAC82_RX_MLAN_1553_BC_WPN_CMD : {
    		if (AUXCTRL_SS.WIAC82Controls.RxAuxOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 Received %d:0x%2.2x BC to WPN Command Message from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		WIAC82_HandleBC_WPN_Cmdmsg(RxData,Timestamp);
    		break;
    	}
    	default : {
    		if (AUXCTRL_SS.WIAC82Controls.ErrDetectOn) {
    			sprintf(WIAC82_SS.AuxStr,"%s%u:WIAC82 MLAN UNKNOWN Message OpCode =%d 0x%2.2x from WIAC82\n",WIAC82_SS.AuxStr,GetTimeUs(),MLANHeader->Opcode,MLANHeader->Opcode);
    		}
    		break;
    	}
	}
}
