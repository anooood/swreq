/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MPSAL.c>
 *
 *  Description:
 *  ------------
 *  Interface module for control data between the MPSAL and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - MPSAL_SeekerInit               : Initializes the MPSAL seeker communication and internal state.
 *  - MPSAL_SeekerSend               : Sends a command or message to the seeker module.
 *  - MPSAL_HandleACK_Msg            : Processes an acknowledgment (ACK) message from the seeker.
 *  - MPSAL_HandleNACK_Msg           : Processes a negative acknowledgment (NACK) message from the seeker.
 *  - MPSAL_HandleSBIT_Msg           : Handles Seeker Built-In Test (SBIT) results.
 *  - MPSAL_HandlePreFlightBIT_Msg   : Processes pre-flight BIT messages from the seeker.
 *  - MPSAL_HandleDetectionMsg       : Handles detection event messages (e.g., target acquired).
 *  - MPSAL_HandleTelemetryMsg       : Processes incoming telemetry data from the seeker.
 *  - MPSAL_HandleSmallImageMsg      : Handles a small image transfer message from the seeker.
 *  - MPSAL_HandleLargeImageMsg      : Handles a large image transfer message from the seeker.
 *  - MPSAL_RxService                : Services and routes received MPSAL messages to appropriate handlers.
 *  - MPSAL_SeekerSetNewParms        : Updates seeker parameters with new configuration values.
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
#include "FPGA.h"
#include "Interrupts.h"
#include "Timer.h"
#include "UART.h"
#include "FIFO.h"
#include "UART_PR.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Wind.h"
#include "Fuze.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "SAL_Seeker.h"
#include "MLAN.h"
#include "MCP.h"
#include "WIAC82_Comms.h"
#include "AircraftInterface.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "BIT.h"

#pragma pack(1)

sSAL_StateSpace SAL_SS;
static sUART MPSALPackUART;

//detection message est. time. 1 start 1 stop per byte and 32 bytes/message
#define DETECTION_MESSAGE_TIME_SEC ((double)(10*32)/3125000.0)

#pragma pack()
void mpsal_Isrcb(unsigned int i)
{
	/*get timestamp
	 *copy msg from uart to fifo
	 *append ts
	 * */
	TIME msgts=GetSystemTime();
	unsigned char* pb=(unsigned char*)&msgts;
	sCharFIFO* pfifo=&SAL_SS.CommsControls.RxFIFO;
	UART_PR_Read(&MPSALPackUART, &SAL_SS.CommsControls.RxFIFO,500,SAL_SS.CommsControls.PacketMode);
	PUT_FIFO_Ptr(pfifo,pb[0]);
	PUT_FIFO_Ptr(pfifo,pb[1]);
	PUT_FIFO_Ptr(pfifo,pb[2]);
	PUT_FIFO_Ptr(pfifo,pb[3]);
	PUT_FIFO_Ptr(pfifo,pb[4]);
	PUT_FIFO_Ptr(pfifo,pb[5]);
	PUT_FIFO_Ptr(pfifo,pb[6]);
	PUT_FIFO_Ptr(pfifo,pb[7]);


}

unsigned int MPSAL_SeekerInit(char *AuxStr, unsigned int FW_Version)
{

//	memset(&SAL_SS, 0, sizeof(sSAL_StateSpace));
	SAL_SS.StateMachine.SetParms_Sent 		= FALSE;
	SAL_SS.StateMachine.SetParms_Complete 	= FALSE;
	SAL_SS.StateMachine.SBIT_Sent 			= FALSE;
	SAL_SS.StateMachine.SBIT_Complete 		= FALSE;
	SAL_SS.StateMachine.DoPreFlightBIT 		= 0;
	SAL_SS.StateMachine.DoPreFlightBIT_Complete = 0;
	SAL_SS.ServiceControl.Cnt10ms 			= 0;
	SAL_SS.Parameters.DesignatorType 		= SAL_PULSED_DESIGNATOR;
	SAL_SS.Parameters.DesignatorCode 		= SAL_DEFAULT_DESIGNATOR_CODE;
	SAL_SS.Parameters.PRFaccuracy 			= SAL_DEFAULT_LASER_PRF_ACCURACY;
	SAL_SS.Parameters.LaserJitter 			= SAL_DEFAULT_LASER_JITTER;
	SAL_SS.Parameters.SeekerYaw 			= 0;//SAL_DEFAULT_SEEKER_YAW;
	SAL_SS.Parameters.SeekerPitch 			= 0;//SAL_DEFAULT_SEEKER_PITCH;
	SAL_SS.Parameters.SeekerRoll 			= 0;//SAL_DEFAULT_SEEKER_ROLL;
	SAL_SS.TelemetryControlParameter 		= TELEMETRY_PARAMETER_DEFAULT;

	SAL_SS.CommsControls.PacketMode				= 1;
	SAL_SS.CommsControls.RxInteruptEnable		= 1;
	SAL_SS.CommsControls.TxMsgEnabled			= 1;
	SAL_SS.CommsControls.TxTestMsgCnt			= 0;

	SAL_SS.ServiceControl.NonTxPollCycleCnt		= 0;
	SAL_SS.ServiceControl.TxPollCycleCnt		= 0;

	if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.MPSALControls.AuxOn 					= 1;
		AUXCTRL_SS.MPSALControls.SchedulerAuxOn			= 0;
		AUXCTRL_SS.MPSALControls.RxAuxOn				= 0;
		AUXCTRL_SS.MPSALControls.RxDetailAuxOn			= 0;
		AUXCTRL_SS.MPSALControls.TxAuxOn				= 0;
		AUXCTRL_SS.MPSALControls.TxDetailAuxOn 			= 0;
		AUXCTRL_SS.MPSALControls.PerformLoopBackTest	= 0;
		AUXCTRL_SS.MPSALControls.SetInternalLoopBack 	= 0;
		AUXCTRL_SS.MPSALControls.ExtRxAuxOn				= 0;
		AUXCTRL_SS.MPSALControls.ErrDetectOn			= 1;
		AUXCTRL_SS.MPSALControls.WarningOn				= 0;
	}

	INIT_FIFO(SAL_SS.CommsControls.RxFIFO);

	SAL_SS.AuxStr[0] = '\0'; //clear buffer

	if (AUXCTRL_SS.MPSALControls.AuxOn) {
		sprintf(AuxStr,"%s%u:MPSAL Parameters Initialized ",AuxStr,GetTimeUs());
		sprintf(AuxStr,"%s UART BASE ADDRESS = 0x%x\n",AuxStr,FPGA_UART_MPSAL_ADDR);
	}

	volatile sMCP_StatusRegister mcp_status_register;

	mcp_status_register = *((volatile sMCP_StatusRegister *) FPGA_STATUS_REG_ADDR);

	if(mcp_status_register.HILS_Mode)
		UART_PR_Init((sUART *) &MPSALPackUART, FPGA_UART_MPSAL_ADDR, br921600, parNONE, 1, SAL_SS.CommsControls.PacketMode, AUXCTRL_SS.MPSALControls.PerformLoopBackTest, AUXCTRL_SS.MPSALControls.SetInternalLoopBack, SAL_SS.CommsControls.RxInteruptEnable);
	else
		UART_PR_Init((sUART *) &MPSALPackUART, FPGA_UART_MPSAL_ADDR, br3125000, parNONE, 1, SAL_SS.CommsControls.PacketMode, AUXCTRL_SS.MPSALControls.PerformLoopBackTest, AUXCTRL_SS.MPSALControls.SetInternalLoopBack, SAL_SS.CommsControls.RxInteruptEnable);

	*((long*)0x43001800)|=(1<<27);//set bit 27 to enable rx while tx active otherwise msgs lost now and then.
	*((long*)0x43001810) =0xABCD1234;   //pkt_sync;
	*((long*)0x43001814) =4;            //pkt_len_offset;
	*((long*)0x43001818) =4;            //pkt_payload_offset
	EnableHardwareInterrupt(mpsal_Isrcb, 0, intUART_4, TRUE);

	return(UART_CheckFW_Version((sUART *) &MPSALPackUART, FW_Version));

}

unsigned int MPSAL_SeekerSend(void)
{
	unsigned int DataToSend;
	unsigned int Cnt;

	DataToSend = 0;
	if (WeaponSettings.WeaponConfig.SeekerType != seekerSAL) {
		if (AUXCTRL_SS.MPSALControls.ErrDetectOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL INVALID Weapon Type%d \n",SAL_SS.AuxStr, GetTimeUs(), WeaponSettings.WeaponConfig.SeekerType);
  		return(SAL_SKR_WEAPON_CONFIG_ERR);
  	}
	if (!(SAL_SS.CommsControls.TxMsgEnabled)) {
		if (AUXCTRL_SS.MPSALControls.WarningOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Tx Disable%d \n",SAL_SS.AuxStr, GetTimeUs(), SAL_SS.CommsControls.TxMsgEnabled);
		return(SAL_SKR_NO_ERROR);
	}
	// To prevent false error counters (Rx) during start-up
	if (Weapon.SystemTime > 2000000) {
		// Check if poll cycle at 250 ms
		SAL_SS.ServiceControl.Cnt10ms++;

		SAL_SS.ServiceControl.PollCycle = SAL_SS.ServiceControl.Cnt10ms % 10;
		if (!(SAL_SS.ServiceControl.PollCycle == 0)) {
			SAL_SS.ServiceControl.NonTxPollCycleCnt++;
			if (AUXCTRL_SS.MPSALControls.SchedulerAuxOn)
				sprintf(SAL_SS.AuxStr,"%s%u:MP-SAL Non Tx Cycle= %d\n",SAL_SS.AuxStr, GetTimeUs(), SAL_SS.ServiceControl.NonTxPollCycleCnt);
			return(SAL_SKR_NO_ERROR);
		}

		SAL_SS.ServiceControl.TxPollCycleCnt++;
		if (AUXCTRL_SS.MPSALControls.SchedulerAuxOn)
			sprintf(SAL_SS.AuxStr,"%s%u:MPSAL - Tx Cycle =%d\n",SAL_SS.AuxStr, GetTimeUs(),SAL_SS.ServiceControl.TxPollCycleCnt);

		// Check if Set Parameters complete
		if ((!SAL_SS.StateMachine.SetParms_Sent) || (!SAL_SS.StateMachine.SetParms_Complete)) {
			DataToSend = 1;
			MPSAL_Pack(SAL_SEEKER_ID_SET_PARAMETERS, &SAL_SS);
			SAL_SS.StateMachine.SetParms_Sent= (unsigned short)(TRUE);
			if (AUXCTRL_SS.MPSALControls.TxAuxOn) {
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL - Tx Set Parameters Sent\n",SAL_SS.AuxStr, GetTimeUs());
			}
		}
		// Check if Stop Telemetry
		else if ((!SAL_SS.StateMachine.Stop_Telem_Sent) || (!SAL_SS.StateMachine.Stop_Telem_Complete)) {
			DataToSend = 1;
			MPSAL_Pack(SAL_SEEKER_ID_STOP_TELEMETRY, &SAL_SS);
			SAL_SS.StateMachine.Stop_Telem_Sent=(unsigned short)(TRUE);
			if (AUXCTRL_SS.MPSALControls.TxAuxOn) {
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL - Tx Stop Telemetry Request Sent\n",SAL_SS.AuxStr, GetTimeUs());
			}
		}
		// Check if SBIT Results received
		else if ((!SAL_SS.StateMachine.SBIT_Sent) || (!SAL_SS.StateMachine.SBIT_Complete)) {
			DataToSend = 1;
			MPSAL_Pack(SAL_SEEKER_ID_SBIT_REQ_ID, &SAL_SS);
			SAL_SS.StateMachine.SBIT_Sent=(unsigned short)(TRUE);
			if (AUXCTRL_SS.MPSALControls.TxAuxOn) {
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL - Tx SBIT Request Sent\n",SAL_SS.AuxStr, GetTimeUs());
			}
		}
		// Check if Get PreFlight BIT control set
		else if ((SAL_SS.StateMachine.DoPreFlightBIT) && (!SAL_SS.StateMachine.DoPreFlightBIT_Complete)) {
			DataToSend = 1;
			MPSAL_Pack(SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS, &SAL_SS);
			if (AUXCTRL_SS.MPSALControls.TxAuxOn) {
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL - Tx PreFlight BIT Sent\n",SAL_SS.AuxStr, GetTimeUs());
			}
		}
		if (DataToSend) {
			UART_PR_Write(&MPSALPackUART, SAL_SS.CommsControls.TxBuffer, SAL_SS.CommsControls.Length);
			if (AUXCTRL_SS.MPSALControls.TxDetailAuxOn) {
				sprintf(SAL_SS.AuxStr,"%s    Length of message=%d\n",SAL_SS.AuxStr, SAL_SS.CommsControls.Length);
				for (Cnt=0;Cnt<SAL_SS.CommsControls.Length;Cnt++) {
					sprintf(SAL_SS.AuxStr,"%s %2.2x",SAL_SS.AuxStr, SAL_SS.CommsControls.TxBuffer[Cnt]);
					if ((Cnt & 0xf)  == 0) {
						sprintf(SAL_SS.AuxStr,"%s \n",SAL_SS.AuxStr);
					}
				}
				sprintf(SAL_SS.AuxStr,"%s\n",SAL_SS.AuxStr);
			}
		}
	}

	return(SAL_SKR_NO_ERROR);
}

void MPSAL_HandleACK_Msg(void)
{

	SAL_SS.RxPtr.ACK_Msg = (sSAL_MCP_ACK *)(SAL_SS.CommsControls.RxBuffer);


	switch (SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF) {
		case SAL_SEEKER_ID_SBIT_REQ_ID: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=SBIT_REQ \n",SAL_SS.AuxStr,GetTimeUs());

		} break;
		case SAL_SEEKER_ID_EXECUTE_PRE_FLIGHT_BIT: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=EXECUTE_PRE_FLIGHT_BIT \n",SAL_SS.AuxStr,GetTimeUs());
			SAL_SS.StateMachine.DoPreFlightBIT_Complete = 1;
		} break;
		case SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=GET_PRE_FLIGHT_BIT_RESULTS \n",SAL_SS.AuxStr,GetTimeUs());

		} break;
		case SAL_SEEKER_ID_SET_PARAMETERS: {
			SAL_SS.StateMachine.SetParms_Complete = 1;
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=SET_PARAMETERS \n",SAL_SS.AuxStr,GetTimeUs());
		} break;
		case SAL_SEEKER_ID_START_TELEMETRY: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=START_TELEMETRY \n",SAL_SS.AuxStr,GetTimeUs());

		} break;
		case SAL_SEEKER_ID_STOP_TELEMETRY: {
			SAL_SS.StateMachine.Stop_Telem_Complete = 1;
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL ACK=STOP_TELEMETRY \n",SAL_SS.AuxStr,GetTimeUs());

		} break;
		default : {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Unknown ACK=0x%x \n",	SAL_SS.AuxStr,GetTimeUs(),(SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF));
		} break;
	}

}

void MPSAL_HandleNACK_Msg(void)
{

	SAL_SS.RxPtr.NACK_Msg = (sSAL_MCP_NACK *)(SAL_SS.CommsControls.RxBuffer);

	switch (SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF) {
		case SAL_SEEKER_ID_SBIT_REQ_ID: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=SBIT_REQ \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		case SAL_SEEKER_ID_EXECUTE_PRE_FLIGHT_BIT: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=EXECUTE_PRE_FLIGHT_BIT \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		case SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=GET_PRE_FLIGHT_BIT_RESULTS \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		case SAL_SEEKER_ID_SET_PARAMETERS: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=SET_PARAMETERS \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		case SAL_SEEKER_ID_START_TELEMETRY: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=START_TELEMETRY \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		case SAL_SEEKER_ID_STOP_TELEMETRY: {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL NACK=STOP_TELEMETRY \n",SAL_SS.AuxStr, GetTimeUs());
		} break;
		default : {
			if (AUXCTRL_SS.MPSALControls.RxAuxOn) sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Unknown NACK=0x%x \n",SAL_SS.AuxStr, GetTimeUs(),(SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF));
		} break;
	}
}

void MPSAL_HandleSBIT_Msg(void)
{

	SAL_SS.RxPtr.SBITRx = (sSAL_MCP_SBITResults *)(SAL_SS.CommsControls.RxBuffer);

	SAL_SS.RxCounters.RxSBITMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_SBITResults)];
	memcpy(&SAL_SS.TimeStamps.RxSBITTimestamp,pb,8);

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL SBIT Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SBITRx->MsgStart.Opcode);

	SAL_SS.SBIT.PBITStatus 								= SAL_SS.RxPtr.SBITRx->SBIT.PBITStatus;
	SAL_SS.SBIT.PBITResults.SensorCommunication 		= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.SensorCommunication;
	SAL_SS.SBIT.PBITResults.SensorVideo 				= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.SensorVideo;
	SAL_SS.SBIT.PBITResults.SensorNoise 				= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.SensorNoise;
	SAL_SS.SBIT.PBITResults.SensorTemperature 			= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.SensorTemperature;
	SAL_SS.SBIT.PBITResults.FPGATemperature 			= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.FPGATemperature;
	SAL_SS.SBIT.PBITResults.DRAM 						= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.DRAM;
	SAL_SS.SBIT.PBITResults.FLASH 						= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.FLASH;
	SAL_SS.SBIT.PBITResults.Spare 						= SAL_SS.RxPtr.SBITRx->SBIT.PBITResults.Spare;

	if (AUXCTRL_SS.MPSALControls.RxDetailAuxOn) {
		sprintf(SAL_SS.AuxStr,"%s    PBITStatus=0x%x\n",			SAL_SS.AuxStr, SAL_SS.SBIT.PBITStatus);
		sprintf(SAL_SS.AuxStr,"%s    SBIT SensorComms=0x%x\n",		SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.SensorCommunication);
		sprintf(SAL_SS.AuxStr,"%s    SBIT SensorVideo=%x\n",		SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.SensorVideo);
		sprintf(SAL_SS.AuxStr,"%s    SBIT SensorNoise=0x%x\n",		SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.SensorNoise );
		sprintf(SAL_SS.AuxStr,"%s    SBIT SensorTemperature=0x%x\n",SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.SensorTemperature);
		sprintf(SAL_SS.AuxStr,"%s    SBIT FPGATemperature=0x%x\n",	SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.FPGATemperature);
		sprintf(SAL_SS.AuxStr,"%s    SBIT DRAM=0x%x\n",				SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.DRAM);
		sprintf(SAL_SS.AuxStr,"%s    SBIT FLASH-0x%x\n",			SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.FLASH);
		sprintf(SAL_SS.AuxStr,"%s    SBIT Spare=0x%x\n",			SAL_SS.AuxStr, SAL_SS.SBIT.PBITResults.Spare);
	}

	SAL_SS.StateMachine.DoPreFlightBIT=1;

	SAL_SS.StateMachine.SBIT_Complete = (unsigned short)(TRUE);
}

void MPSAL_HandlePreFlightBIT_Msg(void)
{
	SAL_SS.RxPtr.PFBITRx = (sSAL_MCP_PreFlightBITResults *)(SAL_SS.CommsControls.RxBuffer);

	SAL_SS.RxCounters.RxFPBITMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_PreFlightBITResults)];
	memcpy(&SAL_SS.TimeStamps.RxPFBITTimestamp,pb,8);

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Pre-flight BIT Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SeekerMsgStart->Opcode);

	SAL_SS.PFBIT.PreFlightBITStatus 					= SAL_SS.RxPtr.PFBITRx->PFBIT.PreFlightBITStatus;
	SAL_SS.PFBIT.PFBITResults.SensorCommunication 		= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.SensorCommunication;
	SAL_SS.PFBIT.PFBITResults.SensorVideo 				= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.SensorVideo;
	SAL_SS.PFBIT.PFBITResults.SensorNoise 				= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.SensorNoise;
	SAL_SS.PFBIT.PFBITResults.SensorTemperature 		= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.SensorTemperature;
	SAL_SS.PFBIT.PFBITResults.FPGATemperature 			= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.FPGATemperature;
	SAL_SS.PFBIT.PFBITResults.DRAM 						= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.DRAM;
	SAL_SS.PFBIT.PFBITResults.FLASH 					= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.FLASH;
	SAL_SS.PFBIT.PFBITResults.Spare 					= SAL_SS.RxPtr.PFBITRx->PFBIT.PFBITResults.Spare;
	SAL_SS.PFBIT.SensorCommsTestResults 				= SAL_SS.RxPtr.PFBITRx->PFBIT.SensorCommsTestResults;
	SAL_SS.PFBIT.SensorVideoTestResults 				= SAL_SS.RxPtr.PFBITRx->PFBIT.SensorVideoTestResults;
	SAL_SS.PFBIT.SensorNoiseTestResults 				= SAL_SS.RxPtr.PFBITRx->PFBIT.SensorNoiseTestResults;
	SAL_SS.PFBIT.SensorTempValue 						= SAL_SS.RxPtr.PFBITRx->PFBIT.SensorTempValue;
	SAL_SS.PFBIT.FPGATempValue 							= SAL_SS.RxPtr.PFBITRx->PFBIT.FPGATempValue;
	SAL_SS.PFBIT.DRAMTestResults 						= SAL_SS.RxPtr.PFBITRx->PFBIT.DRAMTestResults;
	SAL_SS.PFBIT.FLASHTestResults 						= SAL_SS.RxPtr.PFBITRx->PFBIT.FLASHTestResults;
	SAL_SS.PFBIT.Spare1 								= SAL_SS.RxPtr.PFBITRx->PFBIT.Spare1;
	SAL_SS.PFBIT.Spare2 								= SAL_SS.RxPtr.PFBITRx->PFBIT.Spare2;
	SAL_SS.PFBIT.Spare3 								= SAL_SS.RxPtr.PFBITRx->PFBIT.Spare3;
	SAL_SS.PFBIT.SerialNo 								= SAL_SS.RxPtr.PFBITRx->PFBIT.SerialNo;
	SAL_SS.PFBIT.FirmwareVersionNo 						= SAL_SS.RxPtr.PFBITRx->PFBIT.FirmwareVersionNo;
	SAL_SS.PFBIT.FirmwareReleaseDay 					= SAL_SS.RxPtr.PFBITRx->PFBIT.FirmwareReleaseDay;
	SAL_SS.PFBIT.FirmwareReleaseMonth 					= SAL_SS.RxPtr.PFBITRx->PFBIT.FirmwareReleaseMonth;
	SAL_SS.PFBIT.FirmwareReleaseYear 					= SAL_SS.RxPtr.PFBITRx->PFBIT.FirmwareReleaseYear;
	SAL_SS.PFBIT.DesignatorType 						= SAL_SS.RxPtr.PFBITRx->PFBIT.DesignatorType;
	SAL_SS.PFBIT.DesignatorCode 						= SAL_SS.RxPtr.PFBITRx->PFBIT.DesignatorCode;
	SAL_SS.PFBIT.PRFAccuracy 							= SAL_SS.RxPtr.PFBITRx->PFBIT.PRFAccuracy;
	SAL_SS.PFBIT.LaserHitter 							= SAL_SS.RxPtr.PFBITRx->PFBIT.LaserHitter;
	SAL_SS.PFBIT.Yaw 									= SAL_SS.RxPtr.PFBITRx->PFBIT.Yaw;
	SAL_SS.PFBIT.Pitch 									= SAL_SS.RxPtr.PFBITRx->PFBIT.Pitch;
	SAL_SS.PFBIT.Roll 									= SAL_SS.RxPtr.PFBITRx->PFBIT.Roll;

	if (AUXCTRL_SS.MPSALControls.RxDetailAuxOn) {
		sprintf(SAL_SS.AuxStr,"%s    PreFlightBITStatus: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PreFlightBITStatus);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.SensorCommunication: PreFlightBITStatus: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.SensorCommunication);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.SensorVideo: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.SensorVideo);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.SensorNoise: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.SensorNoise);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.SensorTemperature: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.SensorTemperature);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.FPGATemperature: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.FPGATemperature);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.DRAM: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.DRAM);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.FLASH: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.FLASH);
		sprintf(SAL_SS.AuxStr,"%s    PFBITResults.Spare: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.PFBITResults.Spare);
		sprintf(SAL_SS.AuxStr,"%s    MP_SAL SerialNo: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.SerialNo);
		sprintf(SAL_SS.AuxStr,"%s    MP_SAL Firmware Version: %d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.FirmwareVersionNo);
		sprintf(SAL_SS.AuxStr,"%s    MP_SAL Firmware Release Date: %d-%d-%d\n",SAL_SS.AuxStr,SAL_SS.PFBIT.FirmwareReleaseDay,SAL_SS.PFBIT.FirmwareReleaseMonth,SAL_SS.PFBIT.FirmwareReleaseYear);
	}

	SAL_SS.StateMachine.DoPreFlightBIT_Complete = 1;
}

void MPSAL_HandleDetectionMsg(void)
{
	SAL_SS.RxPtr.DetectionData = (sSAL_MCP_DetectionData *)(SAL_SS.CommsControls.RxBuffer);
	SAL_SS.RxCounters.RxDetectionDataMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_DetectionData)];
	memcpy((unsigned char *)(&SAL_SS.TimeStamps.RxDetectionDataTimestamp),pb,8);
	double TsInSeconds=(double)SAL_SS.TimeStamps.RxDetectionDataTimestamp/1000000.0;
	double TotalMsgLatency = DETECTION_MESSAGE_TIME_SEC;

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Detection Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SeekerMsgStart->Opcode);

	SAL_SS.Detection.FrameCount 						= SAL_SS.RxPtr.DetectionData->Detection.FrameCount;
	SAL_SS.Detection.CBITStatus 						= SAL_SS.RxPtr.DetectionData->Detection.CBITStatus;
	SAL_SS.Detection.CBITResults.SensorCommunication 	= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.SensorCommunication;
	SAL_SS.Detection.CBITResults.SensorVideo 			= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.SensorVideo;
	SAL_SS.Detection.CBITResults.SensorNoise 			= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.SensorNoise;
	SAL_SS.Detection.CBITResults.SensorTemperature 		= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.SensorTemperature;
	SAL_SS.Detection.CBITResults.FPGATemperature 		= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.FPGATemperature;
	SAL_SS.Detection.CBITResults.DRAM 					= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.DRAM;
	SAL_SS.Detection.CBITResults.FLASH 					= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.FLASH;
	SAL_SS.Detection.CBITResults.Spare 					= SAL_SS.RxPtr.DetectionData->Detection.CBITResults.Spare;
	SAL_SS.Detection.SpotAzimuth 						= SAL_SS.RxPtr.DetectionData->Detection.SpotAzimuth;
	SAL_SS.Detection.SpotElevation 						= SAL_SS.RxPtr.DetectionData->Detection.SpotElevation;
	SAL_SS.Detection.DetectionStatus 					= SAL_SS.RxPtr.DetectionData->Detection.DetectionStatus;
	SAL_SS.Detection.SpotEnergy 						= SAL_SS.RxPtr.DetectionData->Detection.SpotEnergy;
	SAL_SS.Detection.SpotSize 							= SAL_SS.RxPtr.DetectionData->Detection.SpotSize;
	SAL_SS.Detection.UsedDesignatorCode 				= SAL_SS.RxPtr.DetectionData->Detection.UsedDesignatorCode;
	SAL_SS.Detection.TimeStamp 							= SAL_SS.RxPtr.DetectionData->Detection.TimeStamp;
	SAL_SS.GuidanceData.TargetPitch					    = MPSAL_TO_RAD((double)SAL_SS.Detection.SpotElevation);
	SAL_SS.GuidanceData.TargetYaw  					    = MPSAL_TO_RAD((double)SAL_SS.Detection.SpotAzimuth);
	SAL_SS.GuidanceData.DetectionTime					= TsInSeconds-TotalMsgLatency;

	if (AUXCTRL_SS.MPSALControls.RxDetailAuxOn) {
		sprintf(SAL_SS.AuxStr,"%s    FrameCount = 0x%x \n", 		SAL_SS.AuxStr, SAL_SS.Detection.FrameCount);
		sprintf(SAL_SS.AuxStr,"%s    CBIT Status = 0x%x \n", 		SAL_SS.AuxStr, SAL_SS.Detection.CBITStatus);
		sprintf(SAL_SS.AuxStr,"%s    CBIT Sensor Comms = %d \n", 	SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.SensorCommunication);
		sprintf(SAL_SS.AuxStr,"%s    CBIT SensorVideo = %d \n", 	SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.SensorVideo);
		sprintf(SAL_SS.AuxStr,"%s    CBIT SensorNoise = %d \n", 	SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.SensorNoise);
		sprintf(SAL_SS.AuxStr,"%s    CBIT SensorTemp = %d \n", 		SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.SensorTemperature);
		sprintf(SAL_SS.AuxStr,"%s    CBIT FPGATemp = %d \n", 		SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.FPGATemperature);
		sprintf(SAL_SS.AuxStr,"%s    CBIT DRAM = %d \n", 			SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.DRAM);
		sprintf(SAL_SS.AuxStr,"%s    CBIT FLASH = %d \n", 			SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.FLASH);
		sprintf(SAL_SS.AuxStr,"%s    Spare = %d \n", 				SAL_SS.AuxStr, SAL_SS.Detection.CBITResults.Spare);
		sprintf(SAL_SS.AuxStr,"%s    SpotAzimuth = %d \n", 			SAL_SS.AuxStr, SAL_SS.Detection.SpotAzimuth);
		sprintf(SAL_SS.AuxStr,"%s    SpotElevation = %d \n", 		SAL_SS.AuxStr, SAL_SS.Detection.SpotElevation);
		sprintf(SAL_SS.AuxStr,"%s    DetectionStatus = %d \n", 		SAL_SS.AuxStr, SAL_SS.Detection.DetectionStatus);
		sprintf(SAL_SS.AuxStr,"%s    SpotEnergy = %d \n", 			SAL_SS.AuxStr, SAL_SS.Detection.SpotEnergy);
		sprintf(SAL_SS.AuxStr,"%s    SpotSize = %d \n", 			SAL_SS.AuxStr, SAL_SS.Detection.SpotSize);
		sprintf(SAL_SS.AuxStr,"%s    UsedDesignatorCode = 0x%x \n", SAL_SS.AuxStr, SAL_SS.Detection.UsedDesignatorCode);
		sprintf(SAL_SS.AuxStr,"%s    TimeStamp = 0x%x\n", 			SAL_SS.AuxStr, SAL_SS.Detection.TimeStamp);
	}
}

void MPSAL_HandleTelemetryMsg(void)
{
	SAL_SS.RxPtr.Telemetry = (sSAL_MCP_TelemetryData *)(SAL_SS.CommsControls.RxBuffer);

	SAL_SS.RxCounters.RxTelemetryDataMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_TelemetryData)];
	memcpy(&SAL_SS.TimeStamps.RxTelemetryDataTimestamp,pb,8);

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Telemetry Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SeekerMsgStart->Opcode);

	SAL_SS.TelemetryData.FrameCount 					= SAL_SS.RxPtr.Telemetry->TelemetryData.FrameCount;
	SAL_SS.TelemetryData.TelemetryCount 				= SAL_SS.RxPtr.Telemetry->TelemetryData.TelemetryCount;

	SAL_SS.TelemetryData.Spot1.SpotX 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot1.SpotX;
	SAL_SS.TelemetryData.Spot1.SpotY 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot1.SpotY;
	SAL_SS.TelemetryData.Spot1.SpotSize 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot1.SpotSize;
	SAL_SS.TelemetryData.Spot1.SpotEnergy 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot1.SpotEnergy;

	SAL_SS.TelemetryData.Spot2.SpotX 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot2.SpotX;
	SAL_SS.TelemetryData.Spot2.SpotY 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot2.SpotY;
	SAL_SS.TelemetryData.Spot2.SpotSize 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot2.SpotSize;
	SAL_SS.TelemetryData.Spot2.SpotEnergy 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot2.SpotEnergy;

	SAL_SS.TelemetryData.Spot3.SpotX 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot3.SpotX;
	SAL_SS.TelemetryData.Spot3.SpotY 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot3.SpotY;
	SAL_SS.TelemetryData.Spot3.SpotSize 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot3.SpotSize;
	SAL_SS.TelemetryData.Spot3.SpotEnergy 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot3.SpotEnergy;

	SAL_SS.TelemetryData.Spot4.SpotX 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot4.SpotX;
	SAL_SS.TelemetryData.Spot4.SpotY 					= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot4.SpotY;
	SAL_SS.TelemetryData.Spot4.SpotSize 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot4.SpotSize;
	SAL_SS.TelemetryData.Spot4.SpotEnergy 				= SAL_SS.RxPtr.Telemetry->TelemetryData.Spot4.SpotEnergy;

	if (AUXCTRL_SS.MPSALControls.RxDetailAuxOn) {
		sprintf(SAL_SS.AuxStr,"%s    FrameCount=0x%x \n", 		SAL_SS.AuxStr, SAL_SS.TelemetryData.FrameCount);
		sprintf(SAL_SS.AuxStr,"%s    TelemetryCount=0x%x \n", 	SAL_SS.AuxStr, SAL_SS.TelemetryData.TelemetryCount);
		sprintf(SAL_SS.AuxStr,"%s    Spot1 - X=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot1.SpotX);
		sprintf(SAL_SS.AuxStr,"%s    Spot1 - Y=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot1.SpotY);
		sprintf(SAL_SS.AuxStr,"%s    Spot1 - Size=%d \n", 		SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot1.SpotSize);
		sprintf(SAL_SS.AuxStr,"%s    Spot1 - Energy=%d \n", 	SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot1.SpotEnergy);
		sprintf(SAL_SS.AuxStr,"%s    Spot2 - X=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot2.SpotX);
		sprintf(SAL_SS.AuxStr,"%s    Spot2 - Y=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot2.SpotY);
		sprintf(SAL_SS.AuxStr,"%s    Spot2 - Size=%d \n", 		SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot2.SpotSize);
		sprintf(SAL_SS.AuxStr,"%s    Spot2 - Energy=%d \n", 	SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot2.SpotEnergy);
		sprintf(SAL_SS.AuxStr,"%s    Spot3 - X=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot3.SpotX);
		sprintf(SAL_SS.AuxStr,"%s    Spot3 - Y=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot3.SpotY);
		sprintf(SAL_SS.AuxStr,"%s    Spot3 - Size=%d \n", 		SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot3.SpotSize);
		sprintf(SAL_SS.AuxStr,"%s    Spot3 - Energy=%d \n", 	SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot3.SpotEnergy);
		sprintf(SAL_SS.AuxStr,"%s    Spot4 - X=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot4.SpotX);
		sprintf(SAL_SS.AuxStr,"%s    Spot4 - Y=%d \n", 		    SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot4.SpotY);
		sprintf(SAL_SS.AuxStr,"%s    Spot4 - Size=%d \n", 		SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot4.SpotSize);
		sprintf(SAL_SS.AuxStr,"%s    Spot4 - Energy=%d \n", 	SAL_SS.AuxStr, SAL_SS.TelemetryData.Spot4.SpotEnergy);
	}
}

void MPSAL_HandleSmallImageMsg(void)
{
	SAL_SS.RxPtr.SmallImage = (sSAL_MCP_TelemetrySmallImage *)(SAL_SS.CommsControls.RxBuffer);
	SAL_SS.RxCounters.RxTelemetrySmallImageMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_TelemetrySmallImage)];
	memcpy(&SAL_SS.TimeStamps.RxTelemetrySmallImageTimestamp,pb,8);

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Small Image Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SeekerMsgStart->Opcode);

	SAL_SS.SmallImage.FrameCount 				= SAL_SS.RxPtr.SmallImage->SmallImage.FrameCount;
	SAL_SS.SmallImage.TelemetryCount 			= SAL_SS.RxPtr.SmallImage->SmallImage.TelemetryCount;
	memcpy(SAL_SS.SmallImage.SmallImageData, SAL_SS.RxPtr.SmallImage->SmallImage.SmallImageData, SAL_SEEKER_TEL_SMALL_IMAGE_SIZE);
}

void MPSAL_HandleLargeImageMsg(void)
{
	SAL_SS.RxPtr.LargeImage = (sSAL_MCP_TelemetryLargeImage *)(SAL_SS.CommsControls.RxBuffer);
	SAL_SS.RxCounters.RxTelemetryLargeImageMessageCnt++;
	unsigned char *pb=(unsigned char *)&SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MCP_TelemetryLargeImage)];
	memcpy(&SAL_SS.TimeStamps.RxTelemetryLargeImageTimestamp,pb,8);

	if (AUXCTRL_SS.MPSALControls.RxAuxOn)
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Large Image Message Received=0x%x \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxPtr.SeekerMsgStart->Opcode);

	SAL_SS.LargeImage.FrameCount 				= SAL_SS.RxPtr.LargeImage->LargeImage.FrameCount;
	SAL_SS.LargeImage.TelemetryCount 			= SAL_SS.RxPtr.LargeImage->LargeImage.TelemetryCount;
	memcpy(SAL_SS.LargeImage.LargeImageData, SAL_SS.RxPtr.LargeImage->LargeImage.LargeImageData, SAL_SEEKER_TEL_LARGE_IMAGE_SIZE);

}

void MPSAL_RxService(unsigned char Cycle)
{
	static int CommsTimeoutCheckCounter;
	int Cnt,BytesInFIFO;
	unsigned char rxByte;
	NUMBER_BYTES_IN_FIFO(SAL_SS.CommsControls.RxFIFO, BytesInFIFO);
	sSAL_MsgStart *phdr = (sSAL_MsgStart*)SAL_SS.CommsControls.RxBuffer;

	if (BytesInFIFO>0)
	{
		if(BytesInFIFO<sizeof(sSAL_MsgStart))
		{
			SAL_SS.RxCounters.RxMsgSizeErrCnt++;
			return;
		}
		for(Cnt=0;Cnt<sizeof(sSAL_MsgStart);Cnt++)
		{
			GET_FIFO_Ptr(((sCharFIFO*)&SAL_SS.CommsControls.RxFIFO), rxByte);
			SAL_SS.CommsControls.RxBuffer[Cnt]=rxByte;
		}
		if(phdr->MsgHeader1!=0x1234)
		{
			SAL_SS.RxCounters.RxCrcErrCnt++;
			INIT_FIFO(SAL_SS.CommsControls.RxFIFO);
			return;
		}
		if(BytesInFIFO<(phdr->DataLength+sizeof(TIME)-4))
		{
			SAL_SS.RxCounters.RxMsgSizeErrCnt++;
			return;

		}
		for(Cnt=0;Cnt<(phdr->DataLength+sizeof(TIME)-4);Cnt++)
		{
			GET_FIFO_Ptr(((sCharFIFO*)&SAL_SS.CommsControls.RxFIFO), rxByte);
			SAL_SS.CommsControls.RxBuffer[sizeof(sSAL_MsgStart)+Cnt]=rxByte;
		}
		SAL_SS.RxPtr.SeekerMsgStart = (sSAL_MsgStart *)(SAL_SS.CommsControls.RxBuffer);

		unsigned short *pCRC=(unsigned short*)&SAL_SS.CommsControls.RxBuffer[phdr->DataLength+2];
		if (utCrc16Skr((char*)&phdr->DataLength, phdr->DataLength-2) == *pCRC)
		{
			SAL_SS.RxCounters.RxMessageCnt++;
			SAL_SS.TimeStamps.RxTimestamp = Weapon.SystemTime;
			if 	    (SAL_SS.RxPtr.SeekerMsgStart->Opcode    		== 	SAL_SEEKER_ID_SBIT)  					MPSAL_HandleSBIT_Msg		();
			else if (SAL_SS.RxPtr.SeekerMsgStart->Opcode			== 	SAL_SEEKER_ID_PRE_FLIGHT_BIT_RESULTS)	MPSAL_HandlePreFlightBIT_Msg();
			else if (SAL_SS.RxPtr.SeekerMsgStart->Opcode			== 	SAL_SEEKER_ID_DATA) 					MPSAL_HandleDetectionMsg    ();
			else if (SAL_SS.RxPtr.SeekerMsgStart->Opcode			== 	SAL_SEEKER_ID_TEL) 						MPSAL_HandleTelemetryMsg    ();
			else if (SAL_SS.RxPtr.SeekerMsgStart->Opcode			==	SAL_SEEKER_ID_TEL_SMALL_IMAGE)			MPSAL_HandleSmallImageMsg   ();
			else if (SAL_SS.RxPtr.SeekerMsgStart->Opcode			==	SAL_SEEKER_ID_TEL_LARGE_IMAGE) 			MPSAL_HandleLargeImageMsg   ();
			else if ((SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF00) == 	SAL_SEEKER_ID_ACK) 						MPSAL_HandleACK_Msg         ();
			else if ((SAL_SS.RxPtr.SeekerMsgStart->Opcode & 0xFF00) == 	SAL_SEEKER_ID_NACK) 					MPSAL_HandleNACK_Msg        ();
			else
			{
				SAL_SS.RxCounters.RxMsgIdErrCnt++;
				return;

			}
		}
		else
		{
			SAL_SS.RxCounters.RxCrcErrCnt++;
			if(AUXCTRL_SS.MPSALControls.ErrDetectOn)
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Rx CRC ERROR - %d OpCode: 0x%x\n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxCounters.RxCrcErrCnt,SAL_SS.RxPtr.SeekerMsgStart->Opcode);
		}
	}

	if(CommsTimeoutCheckCounter++%300==0) //Checked at @2hz = 500000us
		if ((Weapon.SystemTime - SAL_SS.TimeStamps.RxTimestamp) >= 400000 && BIT.SBIT_Complete)
		{
			SAL_SS.RxCounters.RxTimeoutErrCnt++;
			if(AUXCTRL_SS.MPSALControls.ErrDetectOn)
				sprintf(SAL_SS.AuxStr,"%s%u:MPSAL Rx Timeout ERROR - %d \n",SAL_SS.AuxStr,GetTimeUs(),SAL_SS.RxCounters.RxTimeoutErrCnt);
		}
}

void MPSAL_SeekerSetNewParms(double Frequency, unsigned short Accuracy, unsigned short Jitter)
{
	SAL_SS.StateMachine.SetParms_Sent = TRUE;
	SAL_SS.StateMachine.SetParms_Complete = FALSE;

	SAL_SS.Parameters.DesignatorCode = 1.0 / (Frequency * FREQ_CODE_SF);
	SAL_SS.Parameters.PRFaccuracy = NS_TO_US(Accuracy);
	SAL_SS.Parameters.LaserJitter = NS_TO_100NS(Jitter);
	if (AUXCTRL_SS.MPSALControls.ExtRxAuxOn){
		sprintf(SAL_SS.AuxStr,"%s%u:MPSAL  New MP-SAL Parameters received:\n", SAL_SS.AuxStr, GetTimeUs());
		sprintf(SAL_SS.AuxStr,"%s    Frequency = %8s\n", SAL_SS.AuxStr, AUXsprintfDouble(Frequency));
		sprintf(SAL_SS.AuxStr,"%s    DesignatorCode = %d - 0x%x\n", SAL_SS.AuxStr, SAL_SS.Parameters.DesignatorCode, SAL_SS.Parameters.DesignatorCode);
		sprintf(SAL_SS.AuxStr,"%s    PRFaccuracy = %d - 0x%x\n", SAL_SS.AuxStr, SAL_SS.Parameters.PRFaccuracy, SAL_SS.Parameters.PRFaccuracy);
		sprintf(SAL_SS.AuxStr,"%s    LaserJitter = %d - 0x%x\n", SAL_SS.AuxStr, SAL_SS.Parameters.LaserJitter, SAL_SS.Parameters.LaserJitter);
	}
}
