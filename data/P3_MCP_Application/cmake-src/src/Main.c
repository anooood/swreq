/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Main.c>
 *
 *  Description:
 *  ------------
 *  Main function of P3
 *
 *  Function(s):
 *  ------------
 *  - InitMain                 : Performs system-wide initialization at startup.
 *  - CheckArcnetNodes         : Checks the status and availability of ARCNET nodes on the network.
 *  - main                     : Entry point of the application; manages overall program execution.
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include "TypeDefines.h"
#include "ScheduleControl.h"
#include "CRC.h"
#include "BIT.h"
#include "Interrupts.h"
#include "Timer.h"
#include "I2C_Controller.h"
#include "MCP.h"
#include "Flash.h"
#include "WIAC82_Comms.h"
#include "UART.h"
#include "FIFO.h"
#include "Weapon.h"
#include "Servos.h"
#include "WiFi.h"
#include "SAL_Seeker.h"
#include "FIFO.h"
#include "UART.h"
#include "MLAN.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "BatteryPack.h"
#include "IMU.h"
#include "UBLOX.h"
#include "HOBSComms.h"
#include "HOBS.h"
#include "TaskManager.h"
#include "Telemetry.h"
#include "Nav.h"
#include "Guidance.h"
#include "WeaponSettings.h"
#include "ETE.h"
#include "Flash.h"
#include "Stack.h"
#include "AircraftInterface.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "IF.h"
#include "UART.h"
#include "FIFO.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"
#include "Main.h"
#include "GCA_Config.h"

sMAIN_StateSpace MAIN_SS;

void InitMain(void) {

	AUXCTRL_SS.MAINControls.AuxOn 				= 1;
	AUXCTRL_SS.MAINControls.NodeCheckOn			= 0;
	AUXCTRL_SS.MAINControls.X_Config_On     	= 0;
	AUXCTRL_SS.MAINControls.DBAS_On				= 0;
	AUXCTRL_SS.MAINControls.WIAC82_On			= 1;
	AUXCTRL_SS.MAINControls.P3LIGHT_On			= 0;
	AUXCTRL_SS.MAINControls.ShallowOn			= 0;
	AUXCTRL_SS.MAINControls.MPSAL_On			= 1;
	AUXCTRL_SS.MAINControls.QSAL_On				= 0;
	AUXCTRL_SS.MAINControls.DSAL_On				= 0;
	AUXCTRL_SS.MAINControls.IMU20_On			= 1;
	AUXCTRL_SS.MAINControls.IMU15_On			= 0;
	AUXCTRL_SS.MAINControls.GNSS_On				= 1;
	AUXCTRL_SS.MAINControls.FuzeCardFittedOn 	= 1;

	MAIN_SS.ServiceControls.OldSystemTime=0;
	MAIN_SS.ServiceControls.SystemTime=GetTimeUs();
	MAIN_SS.AuxStr[0]='\0'; // clear string

}

void CheckArcnetNodes(char *AuxStr) {
	unsigned int NodeID;
	for (NodeID = 240; NodeID <= 255; NodeID++) {
		if (MLAN_IsNodeOnNet(NodeID)==TRUE) {
			AUXsprintf("NodeID=%d 0x%2.2x is on Net\n",NodeID,NodeID);
		}
		else {
			AUXsprintf("NodeID=%d 0x%2.2x is NOT on Net\n",NodeID,NodeID);
		}
	}
}

int main(void)
{
	StackInit();
	MAIN_SS.AuxStr[0] = '\0'; //clear buffer
	InitMain();
	InitCRC();
	FlashInit();
	BIT_Init();
	InitInterrups();
	I2C_ControllerInit();
	WeaponInit();
	WeaponSettingsInit();

	LoadWeaponData(MAIN_SS.AuxStr);

	MCP_Init(MAIN_SS.AuxStr);
	AUXInit(MAIN_SS.AuxStr);

	if (AUXCTRL_SS.MAINControls.FuzeCardFittedOn)
		IF_Init(MAIN_SS.AuxStr);

	MLAN_Init(MAIN_SS.AuxStr);
	TelemetryInit();
	TestEquipmentInit();
	InitMission(MAIN_SS.AuxStr);

	if (AUXCTRL_SS.MAINControls.FuzeCardFittedOn)
		FuzeInit(MAIN_SS.AuxStr);

	ServosInit(MAIN_SS.AuxStr);
	InitGCA_GNSS(MAIN_SS.AuxStr, AUXCTRL_SS.MAINControls.AuxOn);
	InitGCA_IMU(MAIN_SS.AuxStr, AUXCTRL_SS.MAINControls.AuxOn);
	WiFiInit(MAIN_SS.AuxStr);

	InitGCA_AircraftIF(MAIN_SS.AuxStr, AUXCTRL_SS.MAINControls.AuxOn);

	if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
		InitGCA_SAL(MAIN_SS.AuxStr, AUXCTRL_SS.MAINControls.AuxOn);

	BatteryPackInit(MAIN_SS.AuxStr);

	if (WeaponSettings.WeaponConfig.HOBS_Present)
		HOBS_Init(MAIN_SS.AuxStr);

	GuidanceInit();
	NavInit();
	TaskManagerInit();
	BIT_ClearCBIT();
	EnableInterrupts();

	/**/
	AUXsprintf("\nGCA INITIALIZATION\n%s%u:GCA INITIALIZATION COMPLETE \n\nMCP>",MAIN_SS.AuxStr,GetTimeUs());
	MAIN_SS.AuxStr[0]='\0'; // clear string
	/**/

	PrintGCA_Config(AUXCTRL_SS.MAINControls.AuxOn);

	while (1) {
		ServiceAUXTx(&HILS_ADS_RxUART);
/**/
		IMU_CheckComms();
		Guidance10Hz();
/**/
		if (WeaponSettings.WeaponConfig.DTAC_Present) {

			if (AUXCTRL_SS.MAINControls.DBAS_On) {
				AircraftSend();
			}
			if (AUXCTRL_SS.MAINControls.WIAC82_On) {
				WIAC82_Send();
				if (strlen(WIAC82_SS.AuxStr)>0) {
					AUXsprintf("%s",WIAC82_SS.AuxStr);
					WIAC82_SS.AuxStr[0]='\0'; // clear string
				}
			}
		}
/**/
	};

	return 0;
}


