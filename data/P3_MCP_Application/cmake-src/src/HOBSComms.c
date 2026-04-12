/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <HOBSComms.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication between the HoBs and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - HOBS_SetMode                 : Sets the current mode of the HOBS system.
 *  - HOBS_SetConfiguration        : Configures HOBS with the specified parameters.
 *  - HOBS_Plot_Track_Request      : Sends a request to plot a track on HOBS display.
 *  - HOBS_Status_Request          : Requests current status information from HOBS.
 *  - HOBS_FW_Version_Request      : Requests the firmware version running on HOBS.
 *  - HOBS_Logistic_Data_Request   : Requests logistic data from the HOBS system.
 *  - HOBS_HandleModeChangeResp    : Handles the response to a mode change command.
 *  - HOBS_HandleMissionConfigResp : Processes the mission configuration response.
 *  - HOBS_HandleSystemStatusResp  : Handles system status response messages.
 *  - HOBS_HandleGeneralNACKResp   : Processes general NACK response.
 *  - HOBS_HandlePlotTrackResp     : Handles the response to a plot track request.
 *  - HOBS_HandleFWVersionResp     : Processes the firmware version response.
 *  - HOBS_HandleLogisticDataResp  : Handles logistic data response messages.
 *  - HOBS_HandleRxData            : Handles incoming data received by HOBS.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <stdio.h>
#include "Timer.h"
#include "MLAN.h"
#include "FIFO.h"
#include "AuxControls.h"
#include "HOBSComms.h"
#include "HOBS.h"
#include "CRC.h"
#include "WiFi.h"

void HOBS_SetMode(uint8_t newMode)
{
	sMCP_HOBS_MODE_CMD *ModeCmd;
	sMLAN_Header *MLAN_Header;
	int32_t Cnt;
	int32_t Len;

	MLAN_Header = (sMLAN_Header *)SS_HOBS.CommsControls.TxBuffer;
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_MODE_CMD);

	SS_HOBS.Counters.TxModeCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Set Mode Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.TxModeCmdCnt);
	}
	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=%d\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	ModeCmd = (sMCP_HOBS_MODE_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	ModeCmd->Header.SyncByte = HOBS_MSG_SYNC_BYTE;
	ModeCmd->Header.SubSystemAddress = SOFT_PROCESSOR;
	ModeCmd->Header.MessageID = MODE_CHANGE_CMD;
	ModeCmd->Header.FragmentOffset = 0;
	ModeCmd->Header.FragmentOffset2 = 0;
	ModeCmd->Header.PayloadLength =	MODE_CHANGE_CMD_PAYLOAD_LENGTH;
	ModeCmd->Mode = newMode;

	ModeCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(HOBS_MSG_HEADER_SIZE + MODE_CHANGE_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s Mode=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Mode);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,ModeCmd->Checksum);
	}

	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		Len=MLAN_HEADER_SIZE+MLAN_Header->DataLength;
		sprintf(SS_HOBS.AuxStr,"%sARCNET Length=%d\n",SS_HOBS.AuxStr,(int)Len);
		for (Cnt=0;Cnt<Len;Cnt++) {
			sprintf(SS_HOBS.AuxStr,"%s %2.2x",SS_HOBS.AuxStr,SS_HOBS.CommsControls.TxBuffer[Cnt]);
			if ((Cnt & HOBS_SET_MODE)  == HOBS_SET_MODE) {
				sprintf(SS_HOBS.AuxStr,"%s\n",SS_HOBS.AuxStr);
			}
		}
		sprintf(SS_HOBS.AuxStr,"%s\n",SS_HOBS.AuxStr);
	}
}

void HOBS_XConfigurationSet(const sHOBS_MISSION_CONFIG *newConfiguration, uint8_t newBurstHeight)
{
	sMCP_HOBS_MISSION_CONFIG_CMD *ConfigCmd;
	sMLAN_Header *MLAN_Header;

	MLAN_Header = (sMLAN_Header *)(SS_HOBS.CommsControls.TxBuffer);
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_MISSION_CONFIG_CMD);

	SS_HOBS.Counters.TxConfigCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Mission Configuration Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.TxConfigCmdCnt);
	}
	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	ConfigCmd = (sMCP_HOBS_MISSION_CONFIG_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	ConfigCmd->Header.SyncByte              = HOBS_MSG_SYNC_BYTE;
	ConfigCmd->Header.SubSystemAddress      = SOFT_PROCESSOR;
	ConfigCmd->Header.MessageID             = MISSION_CONFIG_CMD;
	ConfigCmd->Header.FragmentOffset        = 0;
	ConfigCmd->Header.FragmentOffset2       = 0;
	ConfigCmd->Header.PayloadLength         = MISSION_CFG_CMD_PAYLOAD_LENGTH;
	ConfigCmd->Config.BurstHeight 			= newBurstHeight;
	ConfigCmd->Config.ColocationID			= newConfiguration->ColocationID;
	ConfigCmd->Config.EstImpactAngle		= newConfiguration->EstImpactAngle;
	ConfigCmd->Config.EstWeaponSpeed		= newConfiguration->EstWeaponSpeed;
	ConfigCmd->Config.WarnSignalWaveform	= newConfiguration->WarnSignalWaveform;

	ConfigCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(HOBS_MSG_HEADER_SIZE + MISSION_CFG_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s BurstHeight=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Config.BurstHeight);
		sprintf(SS_HOBS.AuxStr,"%s ColocationID=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Config.ColocationID);
		sprintf(SS_HOBS.AuxStr,"%s EstImpactAngle=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Config.EstImpactAngle);
		sprintf(SS_HOBS.AuxStr,"%s EstWeaponSpeed=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Config.EstWeaponSpeed);
		sprintf(SS_HOBS.AuxStr,"%s WarnSignalWaveform=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Config.WarnSignalWaveform);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,ConfigCmd->Checksum);
	}

	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);
}
void HOBS_UPlot_Track_Request(void) {
	sMCP_HOBS_PLOT_AND_TRACK_CMD *TrackCmd;
	sMLAN_Header *MLAN_Header;

	MLAN_Header = (sMLAN_Header *)(SS_HOBS.CommsControls.TxBuffer);
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_PLOT_AND_TRACK_CMD);

	SS_HOBS.Counters.TxTrackCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Plot Track Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(), (unsigned int)SS_HOBS.Counters.TxTrackCmdCnt);
	}
	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	TrackCmd = (sMCP_HOBS_PLOT_AND_TRACK_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	TrackCmd->Header.SyncByte = HOBS_MSG_SYNC_BYTE;
	TrackCmd->Header.SubSystemAddress = SOFT_PROCESSOR;
	TrackCmd->Header.MessageID = PLOT_AND_TRACK_CMD;
	TrackCmd->Header.FragmentOffset = 0;
	TrackCmd->Header.PayloadLength =	PLOTTRACK_CMD_PAYLOAD_LENGTH;
	TrackCmd->Spare 						= 0;
	TrackCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + PLOTTRACK_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s Spare=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Spare);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,TrackCmd->Checksum);
	}
	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);
}

void HOBSStatus_Request(void) {
	sMCP_HOBS_SYSTEM_STATUS_CMD *StatusCmd;
	sMLAN_Header *MLAN_Header;

	MLAN_Header = (sMLAN_Header *)(SS_HOBS.CommsControls.TxBuffer);
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_SYSTEM_STATUS_CMD);

	SS_HOBS.Counters.TxStatusCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS System Status Request Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.TxStatusCmdCnt);
	}
	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	StatusCmd = (sMCP_HOBS_SYSTEM_STATUS_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	StatusCmd->Header.SyncByte = HOBS_MSG_SYNC_BYTE;
	StatusCmd->Header.SubSystemAddress = SOFT_PROCESSOR;
	StatusCmd->Header.MessageID = SYSTEM_STATUS_CMD;
	StatusCmd->Header.FragmentOffset = 0;
	StatusCmd->Header.PayloadLength =	SYSTEM_STAT_CMD_PAYLOAD_LENGTH;
	StatusCmd->Spare 			= 0;

	StatusCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + SYSTEM_STAT_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s Spare=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Spare);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,StatusCmd->Checksum);
	}

	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);
}

void HOBSFW_Version_Request(void)
{
	sMCP_HOBS_FW_VERSION_CMD *VersionCmd;
	sMLAN_Header *MLAN_Header;

	MLAN_Header = (sMLAN_Header *)(SS_HOBS.CommsControls.TxBuffer);
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_FW_VERSION_CMD);

	SS_HOBS.Counters.TxVersionCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS FW Version Request Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.TxVersionCmdCnt);
	}

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	VersionCmd = (sMCP_HOBS_FW_VERSION_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	VersionCmd->Header.SyncByte = HOBS_MSG_SYNC_BYTE;
	VersionCmd->Header.SubSystemAddress = SOFT_PROCESSOR;
	VersionCmd->Header.MessageID = FW_VERSION_CMD;
	VersionCmd->Header.FragmentOffset = 0;
	VersionCmd->Header.FragmentOffset2 = 0;
	VersionCmd->Header.PayloadLength =	FW_VERSION_CMD_PAYLOAD_LENGTH;
	VersionCmd->Spare 			= 0;

	VersionCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + FW_VERSION_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset2=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.FragmentOffset2);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s Spare=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Spare);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,VersionCmd->Checksum);
	}

	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);
}

void HOBSLogistic_Data_Request(void)
{
	sMCP_HOBS_LOG_DATA_READ_CMD *LogisticCmd;
	sMLAN_Header *MLAN_Header;

	MLAN_Header = (sMLAN_Header *)(SS_HOBS.CommsControls.TxBuffer);
	MLAN_Header->SourceId = MLAN_MCP_NODE_ID;
	MLAN_Header->DestinationId = MLAN_WIFI_NODE_ID;
	MLAN_Header->Opcode = WIFI_ID_HOBS_MESSAGES;
	MLAN_Header->DataLength = sizeof(sMCP_HOBS_LOG_DATA_READ_CMD);

	SS_HOBS.Counters.TxLogisticCmdCnt++;

	if (AUXCTRL_SS.HOBSControls.TxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Logistic Data Request Command Sent %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.TxLogisticCmdCnt);
	}

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sLAN HEADER:\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SourceId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->SourceId);
		sprintf(SS_HOBS.AuxStr,"%s DestinationId=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DestinationId);
		sprintf(SS_HOBS.AuxStr,"%s Opcode=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->Opcode);
		sprintf(SS_HOBS.AuxStr,"%s DataLength=0x%2.2x\n",SS_HOBS.AuxStr,MLAN_Header->DataLength);
	}

	LogisticCmd = (sMCP_HOBS_LOG_DATA_READ_CMD *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE];		/* Offset to payload */
	LogisticCmd->Header.SyncByte = HOBS_MSG_SYNC_BYTE;
	LogisticCmd->Header.SubSystemAddress = FLASH_LOADER;
	LogisticCmd->Header.MessageID = LOGISTIC_DATA_READ_CMD;
	LogisticCmd->Header.FragmentOffset = 0;
	LogisticCmd->Header.FragmentOffset2 = 0;
	LogisticCmd->Header.PayloadLength =	LOGDAT_READ_CMD_PAYLOAD_LENGTH;
	LogisticCmd->Spare 			= 0;

	LogisticCmd->Checksum = CalculateXOR_Checksum((uint8_t *)&SS_HOBS.CommsControls.TxBuffer[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + LOGDAT_READ_CMD_PAYLOAD_LENGTH));

	if (AUXCTRL_SS.HOBSControls.TxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%sHOBS Message Section\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%sRS485 Header\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s SyncByte=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Header.SyncByte);
		sprintf(SS_HOBS.AuxStr,"%s SubSystemAddress=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Header.SubSystemAddress);
		sprintf(SS_HOBS.AuxStr,"%s MessageID=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Header.MessageID);
		sprintf(SS_HOBS.AuxStr,"%s FragmentOffset=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Header.FragmentOffset);
		sprintf(SS_HOBS.AuxStr,"%s PayloadLength=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Header.PayloadLength);
		sprintf(SS_HOBS.AuxStr,"%s Spare=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Spare);
		sprintf(SS_HOBS.AuxStr,"%s Checksum=0x%2.2x\n",SS_HOBS.AuxStr,LogisticCmd->Checksum);
	}

	MLAN_Send(MLAN_WIFI_NODE_ID,(uint8_t *)SS_HOBS.CommsControls.TxBuffer,MLAN_HEADER_SIZE+MLAN_Header->DataLength);
}

void HOBS_ChangeModeRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_MODE_RESP *ModeResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxModeRespCnt++;
	ModeResp = (sHOBS_MCP_MODE_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + MODE_CHANGE_RESP_PAYLOAD_LENGTH));
	if (localChecksum == ModeResp->Checksum)
	{
		SS_HOBS.Counters.RxModeRespChecksumOKCnt++;
	}
	else
	{
		SS_HOBS.Counters.RxModeRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Mode Change Response Received %u\n",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxModeRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxModeRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxModeRespChecksumErrCnt);
	}
}

void HOBS_MissionConfigRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_MISSION_CONFIG_RESP *ConfigResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxConfigRespCnt++;
	ConfigResp = (sHOBS_MCP_MISSION_CONFIG_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + MISSION_CFG_RESP_PAYLOAD_LENGTH));
	if (localChecksum == ConfigResp->Checksum)
	{
		SS_HOBS.Counters.RxConfigRespChecksumOKCnt++;
	}
	else
	{
		SS_HOBS.Counters.RxConfigRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Mission Configuration Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxConfigRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxConfigRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxConfigRespChecksumErrCnt);
	}
}

void HOBS_SystemStatusRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_SYSTEM_STATUS_RESP *StatusResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxStatusRespCnt++;
	StatusResp = (sHOBS_MCP_SYSTEM_STATUS_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + SYSTEM_STAT_RESP_PAYLOAD_LENGTH));
	if (localChecksum == StatusResp->Checksum) {
		SS_HOBS.Counters.RxStatusRespChecksumOKCnt++;
		SS_HOBS.Status.Mode = StatusResp->SystemStatus.Mode;
		SS_HOBS.StateMachine.Mode = SS_HOBS.Status.Mode;
		SS_HOBS.Status.SBIT = StatusResp->SystemStatus.SBIT;
		SS_HOBS.Status.CBIT = StatusResp->SystemStatus.CBIT;
		SS_HOBS.Status.TestSettings = StatusResp->SystemStatus.TestSettings;
		SS_HOBS.Status.BurstHeight = StatusResp->SystemStatus.BurstHeight;
		SS_HOBS.Status.EstImpactAngle = StatusResp->SystemStatus.EstImpactAngle;
		SS_HOBS.Status.WeaponSpeed = StatusResp->SystemStatus.WeaponSpeed;
		SS_HOBS.Status.WarningSignalWaveform =StatusResp->SystemStatus.WarningSignalWaveform;
		SS_HOBS.Status.CoLocationID = StatusResp->SystemStatus.CoLocationID;
		SS_HOBS.Status.PSU_1V8 = StatusResp->SystemStatus.PSU_1V8;
		SS_HOBS.Status.PSU_4V1 = StatusResp->SystemStatus.PSU_4V1;
		SS_HOBS.Status.PSU_5V = StatusResp->SystemStatus.PSU_5V;
		SS_HOBS.Status.PSU_5V5 = StatusResp->SystemStatus.PSU_5V5;
		SS_HOBS.Status.PSU_10V = StatusResp->SystemStatus.PSU_10V;
		SS_HOBS.Status.PSU_10V5 = StatusResp->SystemStatus.PSU_10V5;
		SS_HOBS.Status.PSU_InputVoltage = StatusResp->SystemStatus.PSU_InputVoltage;
		SS_HOBS.Status.OutputDetectorLevel = StatusResp->SystemStatus.OutputDetectorLevel;
		SS_HOBS.Status.PSU_3V3 = StatusResp->SystemStatus.PSU_3V3;
		SS_HOBS.Status.PSU_PCB__Temp = StatusResp->SystemStatus.PSU_PCB__Temp;
	}
	else
	{
		SS_HOBS.Counters.RxStatusRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS System Status Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxStatusRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxStatusRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxStatusRespChecksumErrCnt);
	}

	if (AUXCTRL_SS.HOBSControls.RxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s Mode=%d\n",						SS_HOBS.AuxStr,SS_HOBS.Status.Mode);
		sprintf(SS_HOBS.AuxStr,"%s SBIT=0x%x\n",					SS_HOBS.AuxStr,SS_HOBS.Status.SBIT.DWord);
		sprintf(SS_HOBS.AuxStr,"%s CBIT=0x%x\n",					SS_HOBS.AuxStr,SS_HOBS.Status.CBIT.DWord);
		sprintf(SS_HOBS.AuxStr,"%s Test Settings=0x%x\n",			SS_HOBS.AuxStr,SS_HOBS.Status.TestSettings.DWord);
		sprintf(SS_HOBS.AuxStr,"%s Burst Height=%d meters\n",		SS_HOBS.AuxStr,SS_HOBS.Status.BurstHeight);
		sprintf(SS_HOBS.AuxStr,"%s Est Impact Angle=%d degrees\n",	SS_HOBS.AuxStr,SS_HOBS.Status.EstImpactAngle);
		sprintf(SS_HOBS.AuxStr,"%s Weapon Speed=%d\n",				SS_HOBS.AuxStr,SS_HOBS.Status.WeaponSpeed);
		sprintf(SS_HOBS.AuxStr,"%s Warning Signal Waveform=%d\n",	SS_HOBS.AuxStr,SS_HOBS.Status.WarningSignalWaveform);
		sprintf(SS_HOBS.AuxStr,"%s Co-Location ID=%d\n",			SS_HOBS.AuxStr,SS_HOBS.Status.CoLocationID);
		sprintf(SS_HOBS.AuxStr,"%s PSU 1V8=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_1V8);
		sprintf(SS_HOBS.AuxStr,"%s PSU 4V1=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_4V1);
		sprintf(SS_HOBS.AuxStr,"%s PSU 5V=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_5V);
		sprintf(SS_HOBS.AuxStr,"%s PSU 5V5=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_5V5);
		sprintf(SS_HOBS.AuxStr,"%s PSU 10V=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_10V);
		sprintf(SS_HOBS.AuxStr,"%s PSU 10V5=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_10V5);
		sprintf(SS_HOBS.AuxStr,"%s PSU Input Voltage=%d\n",			SS_HOBS.AuxStr,SS_HOBS.Status.PSU_InputVoltage);
		sprintf(SS_HOBS.AuxStr,"%s Output Detector Level=%d\n",		SS_HOBS.AuxStr,SS_HOBS.Status.OutputDetectorLevel);
		sprintf(SS_HOBS.AuxStr,"%s PSU 3V3=%d\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_3V3);
		sprintf(SS_HOBS.AuxStr,"%s PCB Temp=%d x 0.0625 (degree C)\n",					SS_HOBS.AuxStr,SS_HOBS.Status.PSU_PCB__Temp);
		sprintf(SS_HOBS.AuxStr,"%s\n",					SS_HOBS.AuxStr);
	}
}

void HOBS_GeneralNACKRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_NACK_RESP *NACKResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxNACKRespCnt++;
	NACKResp = (sHOBS_MCP_NACK_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + NACK_RESP_PAYLOAD_LENGTH));
	SS_HOBS.StateMachine.NACKResponse = NACKResp->NAK_Response;
	if (localChecksum == NACKResp->Checksum)
	{
		SS_HOBS.Counters.RxNACKRespChecksumOKCnt++;
	}
	else
	{
		SS_HOBS.Counters.RxNACKRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS NACK Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxNACKRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxNACKRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxNACKRespChecksumErrCnt);
	}
}

void HOBS_PlotTrackRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_PLOT_AND_TRACK_RESP *TrackResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxTrackRespCnt++;
	TrackResp = (sHOBS_MCP_PLOT_AND_TRACK_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + PLOTTRACK_RESP_PAYLOAD_LENGTH));
	if (localChecksum == TrackResp->Checksum) {
		SS_HOBS.Counters.RxTrackRespChecksumOKCnt++;
		SS_HOBS.Track.TimeStamp = TrackResp->Track.TimeStamp;
		SS_HOBS.Track.CBIT = TrackResp->Track.CBIT;
		SS_HOBS.Track.TestSettings = TrackResp->Track.TestSettings;
		SS_HOBS.Track.DetectorHeight = TrackResp->Track.DetectorHeight;
		SS_HOBS.Track.TrackHeight = TrackResp->Track.TrackHeight;
		SS_HOBS.Track.TrackSpeed = TrackResp->Track.TrackSpeed;
		SS_HOBS.Track.GPW_HeightSetting = TrackResp->Track.GPW_HeightSetting;
	}
	else
	{
		SS_HOBS.Counters.RxTrackRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Plot & Track Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxTrackRespCnt);
		sprintf(SS_HOBS.AuxStr,"%sCRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxTrackRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxTrackRespChecksumErrCnt);
	}
}

void HOBS_FWVersionRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_FW_VERSION_RESP *VersionResp;
	uint8_t localChecksum;
	uint8_t majorVersion = 0;
	uint8_t minorVersion = 0;

	SS_HOBS.Counters.RxVersionRespCnt++;
	VersionResp = (sHOBS_MCP_FW_VERSION_RESP *)&RxData[MLAN_HEADER_SIZE];

	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + FW_VERSION_RESP_PAYLOAD_LENGTH));

	if (localChecksum == VersionResp->Checksum) {
		SS_HOBS.Counters.RxVersionRespChecksumOKCnt++;
		SS_HOBS.Version.Version = VersionResp->FirmwareVersion.Version;
		SS_HOBS.Version.CRC = VersionResp->FirmwareVersion.CRC;

		minorVersion = SS_HOBS.Version.Version & 0x0FU;
		majorVersion = (SS_HOBS.Version.Version >> HOBS_MAJOR_CONST) & 0x0FU;

		uint16_t fw_version = ((uint16_t)majorVersion * 100U) + ((uint16_t)minorVersion * 10U);

		SS_HOBS.FW_HOBS_Version = fw_version;
	}
	else
	{
		SS_HOBS.Counters.RxVersionRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS FW Version Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxVersionRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxVersionRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxVersionRespChecksumErrCnt);
	}

	if (AUXCTRL_SS.HOBSControls.RxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s FW CRC=%08X\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Version.CRC);
		sprintf(SS_HOBS.AuxStr,"%s FW Version=%d.%d \n",SS_HOBS.AuxStr,majorVersion,minorVersion);
	}
}

void HOBS_LogisticDataRespHandler(uint8_t *RxData)
{
	sHOBS_MCP_LOG_DATA_READ_RESP *LogisticResp;
	uint8_t localChecksum;

	SS_HOBS.Counters.RxLogisticRespCnt++;
	LogisticResp = (sHOBS_MCP_LOG_DATA_READ_RESP *)&RxData[MLAN_HEADER_SIZE];
	localChecksum = CalculateXOR_Checksum(&RxData[MLAN_HEADER_SIZE],(uint32_t)(sizeof(sHOBS_Msg_Header) + LOGDAT_READ_RESP_PAYLOAD_LENGTH));
	if (localChecksum == LogisticResp->Checksum) {
		SS_HOBS.Counters.RxLogisticRespChecksumOKCnt++;
		for (uint8_t i_P = 0; i_P < HOBS_PART_NUMBER_LENGTH; i_P++)
		{
			SS_HOBS.Logistic.PartNumber[i_P] = LogisticResp->RRS_Data.PartNumber[i_P];
		}

		for (uint8_t i_S = 0; i_S < HOBS_SERIAL_NUMBER_LENGTH; i_S++)
		{
			SS_HOBS.Logistic.SerialNumber[i_S] = LogisticResp->RRS_Data.SerialNumber[i_S];
		}
	}
	else
	{
		SS_HOBS.Counters.RxLogisticRespChecksumErrCnt++;
	}

	if (AUXCTRL_SS.HOBSControls.RxAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Logistic Data Response Received %u",SS_HOBS.AuxStr,GetTimeUs(),(unsigned int)SS_HOBS.Counters.RxLogisticRespCnt);
		sprintf(SS_HOBS.AuxStr,"%s CRC OK %u NOK %u\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.Counters.RxLogisticRespChecksumOKCnt,(unsigned int)SS_HOBS.Counters.RxLogisticRespChecksumErrCnt);
	}

	if (AUXCTRL_SS.HOBSControls.RxDetailAuxOn != 0U) {

		sprintf(SS_HOBS.AuxStr,"%s HOBS Part Number: ",SS_HOBS.AuxStr);
		for (uint8_t i_P2 = 0; i_P2 < HOBS_PART_NUMBER_LENGTH; i_P2++) {

	        if (SS_HOBS.Logistic.PartNumber[i_P2] != '\0') {
	            sprintf(SS_HOBS.AuxStr,"%s%c",SS_HOBS.AuxStr,(char)SS_HOBS.Logistic.PartNumber[i_P2]);
	        } else {
	            break;
	        }

		}
		sprintf(SS_HOBS.AuxStr,"%s\n",SS_HOBS.AuxStr);
		sprintf(SS_HOBS.AuxStr,"%s HOBS Serial Number: ",SS_HOBS.AuxStr);
		for (uint8_t i_S2 = 0; i_S2 < HOBS_SERIAL_NUMBER_LENGTH; i_S2++) {

	        if (SS_HOBS.Logistic.SerialNumber[i_S2] != '\0') {
	        	sprintf(SS_HOBS.AuxStr,"%s%c",SS_HOBS.AuxStr,(char)SS_HOBS.Logistic.SerialNumber[i_S2]);
	        } else {
	            break;
	        }

		}
		sprintf(SS_HOBS.AuxStr,"%s\n",SS_HOBS.AuxStr);
	}
}

void HOBS_HandleRxData(uint8_t *RxData)
{
	sHOBS_Msg_Header *HOBS_Header;

	SS_HOBS.Counters.RxMessageCnt++;
	SS_HOBS.RxTimestamp = GetSystemTime();
	HOBS_Header = (sHOBS_Msg_Header *)&RxData[MLAN_HEADER_SIZE];

	if (AUXCTRL_SS.HOBSControls.RxDetailAuxOn != 0U) {
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS SubSystem: 0x%x MessageID: 0x%x ",SS_HOBS.AuxStr,GetTimeUs(),(unsigned char)HOBS_Header->SubSystemAddress, (unsigned char)HOBS_Header->MessageID);
		sprintf(SS_HOBS.AuxStr,"%s HOBS Rx Message Received - %d Errors: %d Unknown %d\n",SS_HOBS.AuxStr,(unsigned int)SS_HOBS.HOBSWaitForRxMsg,(unsigned int)SS_HOBS.Counters.ErrCnt,(unsigned int)SS_HOBS.Counters.UnknownMsgCnt);
	}

	if (HOBS_Header->SubSystemAddress == SOFT_PROCESSOR) {
		if (HOBS_Header->MessageID == MODE_CHANGE_RESP) 				{HOBS_ChangeModeRespHandler(RxData);}
		else if (HOBS_Header->MessageID == MISSION_CONFIG_RESP) 		{HOBS_MissionConfigRespHandler(RxData);}
		else if (HOBS_Header->MessageID == SYSTEM_STATUS_RESP) 			{HOBS_SystemStatusRespHandler(RxData);}
		else if (HOBS_Header->MessageID == FW_VERSION_RESP) 			{HOBS_FWVersionRespHandler(RxData);}
		else {SS_HOBS.Counters.RxUnknownSoftProcMsgCnt++;}
	}
	else if (HOBS_Header->SubSystemAddress == SIGNAL_PROCESSOR) {
		if (HOBS_Header->MessageID == PLOT_AND_TRACK_RESP) 				{HOBS_PlotTrackRespHandler(RxData);}
		else {SS_HOBS.Counters.RxUnknownSignalProcMsgCnt++;}
	}
	else if (HOBS_Header->SubSystemAddress == FLASH_LOADER) {
		if (HOBS_Header->MessageID == LOGISTIC_DATA_READ_RESP) 			{HOBS_LogisticDataRespHandler(RxData);}
		else {SS_HOBS.Counters.RxUnknownFlashLoaderProcMsgCnt++;}
	}
	else if (HOBS_Header->SubSystemAddress == GENERAL_NACK) {
		if (HOBS_Header->MessageID == NACK_RESP) 						{HOBS_GeneralNACKRespHandler(RxData);}
		else {SS_HOBS.Counters.RxUnknownNACKMsgCnt++;}
	}
	else {SS_HOBS.Counters.UnknownMsgCnt++;}

}

