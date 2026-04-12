/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MPSAL_Comms.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication between the MPSAL and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - utCrc16Skr                 : Calculate MP_SAL Seeker CRC. Formula customized by Shilat.
 *  - MPSAL_Pack       			 : Manages MP_SAL Packages.
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include "Timer.h"
#include "FIFO.h"
#include "UART.h"
#include "crc.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"

#pragma pack(1)

/* --------------------------------------------------------------------------------------
    Purpose      : calculate SAL Seeker CRC. This is a CRC formula customised by Shilat
    Requirements : N/A
    Parameters   : see below
    Returns      : see below
    -------------------------------------------------------------------------------------- */
// CRC16 seed
#define CRC16_SEED 0x0000

unsigned short           // checksum
utCrc16Skr
(
    char* ptr,  // pointer to start of block
	unsigned int size  // size of block in bytes
)
{
    uint16_t crc = CRC16_SEED;
    uint16_t aux;
    uint16_t i   = 0;

    while (size--)
    {
    	aux = ((crc >> 8) ^ ptr[i++]) & 0x00FF;
    	aux ^= aux >> 4;
        crc = (crc << 8) ^ (aux << 12) ^ (aux << 5) ^ aux;
    }

    return crc;
}


unsigned short MPSAL_Pack(unsigned short Opcode, sSAL_StateSpace *SAL_SS) { // returns length of message

	memset(SAL_SS->CommsControls.TxBuffer,0,AUX_FIFO_SIZE);
	if (Opcode==SAL_SEEKER_ID_SBIT_REQ_ID) {
			SAL_SS->TxPtr.GetPBITResultsCmd = (sMCP_SAL_GetPBITResultsCmd *)(SAL_SS->CommsControls.TxBuffer);
//			SAL_SS->TxPtr.GetPBITResultsCmd = (sMCP_SAL_GetPBITResultsCmd *)(&SAL_SS->CommsControls.TxBuffer[0]);
			SAL_SS->TxPtr.GetPBITResultsCmd->MsgStart.MsgHeader1 			= MPSAL_MESSAGE_HEADER_1;
			SAL_SS->TxPtr.GetPBITResultsCmd->MsgStart.MsgHeader2 			= MPSAL_MESSAGE_HEADER_2;
			SAL_SS->TxPtr.GetPBITResultsCmd->MsgStart.DataLength 			= SAL_SEEKER_LENGTH_SBIT_REQ_ID;
			SAL_SS->TxPtr.GetPBITResultsCmd->MsgStart.Opcode 				= SAL_SEEKER_ID_SBIT_REQ_ID;
			SAL_SS->TxPtr.GetPBITResultsCmd->Checksum 						= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_GetPBITResultsCmd)-6));
			SAL_SS->CommsControls.Length 									= sizeof(sMCP_SAL_GetPBITResultsCmd);
			SAL_SS->TxCounters.TxSBITMessageCmdCnt++;
			return(SAL_SS->CommsControls.Length);
	  }
	  else if (Opcode==SAL_SEEKER_ID_EXECUTE_PRE_FLIGHT_BIT) {
		  	SAL_SS->TxPtr.ExececutePreFlightBITCmd = (sMCP_SAL_ExececutePreFlightBITCmd *)(SAL_SS->CommsControls.TxBuffer);
		  	SAL_SS->TxPtr.ExececutePreFlightBITCmd->MsgStart.MsgHeader1 	= MPSAL_MESSAGE_HEADER_1;
		  	SAL_SS->TxPtr.ExececutePreFlightBITCmd->MsgStart.MsgHeader2 	= MPSAL_MESSAGE_HEADER_2;
		  	SAL_SS->TxPtr.ExececutePreFlightBITCmd->MsgStart.DataLength 	= SAL_SEEKER_LENGTH_EXECUTE_PRE_FLIGHT_BIT;
		  	SAL_SS->TxPtr.ExececutePreFlightBITCmd->MsgStart.Opcode 		= SAL_SEEKER_ID_EXECUTE_PRE_FLIGHT_BIT;
			SAL_SS->TxPtr.ExececutePreFlightBITCmd->Checksum 				= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_ExececutePreFlightBITCmd)-6));
		  	SAL_SS->CommsControls.Length 									= sizeof(sMCP_SAL_ExececutePreFlightBITCmd);
			SAL_SS->TxCounters.TxPFBITMessageCmdCnt++;
			return(SAL_SS->CommsControls.Length);
	  }
	  else if (Opcode==SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS) {
		  	SAL_SS->TxPtr.GetPreFlightBITResultsCmd = (sMCP_SAL_GetPreFlightBITResultsCmd *)(SAL_SS->CommsControls.TxBuffer);
		  	SAL_SS->TxPtr.GetPreFlightBITResultsCmd->MsgStart.MsgHeader1 	= MPSAL_MESSAGE_HEADER_1;
		  	SAL_SS->TxPtr.GetPreFlightBITResultsCmd->MsgStart.MsgHeader2 	= MPSAL_MESSAGE_HEADER_2;
		  	SAL_SS->TxPtr.GetPreFlightBITResultsCmd->MsgStart.DataLength 	= SAL_SEEKER_LENGTH_GET_PRE_FLIGHT_BIT_RESULTS;
		  	SAL_SS->TxPtr.GetPreFlightBITResultsCmd->MsgStart.Opcode 		= SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS;
			SAL_SS->TxPtr.GetPBITResultsCmd->Checksum 						= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_GetPreFlightBITResultsCmd)-6));
		  	SAL_SS->CommsControls.Length 									= sizeof(sMCP_SAL_GetPreFlightBITResultsCmd);
			return(SAL_SS->CommsControls.Length);
	  }
	  else if (Opcode==SAL_SEEKER_ID_SET_PARAMETERS) {
		  	SAL_SS->TxPtr.SetParametersCmd = (sMCP_SAL_SetParametersCmd *)(SAL_SS->CommsControls.TxBuffer);
			SAL_SS->TxPtr.SetParametersCmd->MsgStart.MsgHeader1 			= MPSAL_MESSAGE_HEADER_1;
			SAL_SS->TxPtr.SetParametersCmd->MsgStart.MsgHeader2 			= MPSAL_MESSAGE_HEADER_2;
			SAL_SS->TxPtr.SetParametersCmd->MsgStart.DataLength 			= SAL_SEEKER_LENGTH_SET_PARAMETERS;
			SAL_SS->TxPtr.SetParametersCmd->MsgStart.Opcode 				= SAL_SEEKER_ID_SET_PARAMETERS;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.DesignatorType 		= SAL_SS->Parameters.DesignatorType;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.DesignatorCode 		= SAL_SS->Parameters.DesignatorCode;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.PRFaccuracy 			= SAL_SS->Parameters.PRFaccuracy;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.LaserJitter 			= SAL_SS->Parameters.LaserJitter;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.SeekerYaw 			= SAL_SS->Parameters.SeekerYaw;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.SeekerPitch 			= SAL_SS->Parameters.SeekerPitch;
			SAL_SS->TxPtr.SetParametersCmd->Parameters.SeekerRoll 			= SAL_SS->Parameters.SeekerRoll;
			SAL_SS->TxPtr.SetParametersCmd->Checksum 						= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_SetParametersCmd)-6));
		  	SAL_SS->CommsControls.Length								 	= sizeof(sMCP_SAL_SetParametersCmd);
			SAL_SS->TxCounters.TxSetParmsMessageCmdCnt++;
			return(SAL_SS->CommsControls.Length);
	  }
	  else if (Opcode==SAL_SEEKER_ID_START_TELEMETRY) {
		  	SAL_SS->TxPtr.StartTelemetryCmd = (sMCP_SAL_StartTelemetryCmd *)(SAL_SS->CommsControls.TxBuffer);
		  	SAL_SS->TxPtr.StartTelemetryCmd->MsgStart.MsgHeader1 			= MPSAL_MESSAGE_HEADER_1;
		  	SAL_SS->TxPtr.StartTelemetryCmd->MsgStart.MsgHeader2 			= MPSAL_MESSAGE_HEADER_2;
		  	SAL_SS->TxPtr.StartTelemetryCmd->MsgStart.DataLength 			= SAL_SEEKER_LENGTH_START_TELEMETRY;
		  	SAL_SS->TxPtr.StartTelemetryCmd->MsgStart.Opcode 				= SAL_SEEKER_ID_START_TELEMETRY;
		  	SAL_SS->TxPtr.StartTelemetryCmd->TelemetryContrtolParameter 	= SAL_SS->TelemetryControlParameter;
			SAL_SS->TxPtr.StartTelemetryCmd->Checksum 						= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_StartTelemetryCmd)-6));
		  	SAL_SS->CommsControls.Length 									= sizeof(sMCP_SAL_StartTelemetryCmd);
			return(SAL_SS->CommsControls.Length);
	  }
	  else if (Opcode==SAL_SEEKER_ID_STOP_TELEMETRY) {
		  	SAL_SS->TxPtr.StopTelemetryCmd = (sMCP_SAL_StopTelemetryCmd *)(SAL_SS->CommsControls.TxBuffer);
		  	SAL_SS->TxPtr.StopTelemetryCmd->MsgStart.MsgHeader1 			= MPSAL_MESSAGE_HEADER_1;
		  	SAL_SS->TxPtr.StopTelemetryCmd->MsgStart.MsgHeader2 			= MPSAL_MESSAGE_HEADER_2;
		  	SAL_SS->TxPtr.StopTelemetryCmd->MsgStart.DataLength 			= SAL_SEEKER_LENGTH_STOP_TELEMETRY;
		  	SAL_SS->TxPtr.StopTelemetryCmd->MsgStart.Opcode 				= SAL_SEEKER_ID_STOP_TELEMETRY;
			SAL_SS->TxPtr.StopTelemetryCmd->Checksum 						= utCrc16Skr(&SAL_SS->CommsControls.TxBuffer[4],(sizeof(sMCP_SAL_StopTelemetryCmd)-6));
		  	SAL_SS->CommsControls.Length 									= sizeof(sMCP_SAL_StopTelemetryCmd);
			return(SAL_SS->CommsControls.Length);
	  }
      return(0);
}

#pragma pack()
