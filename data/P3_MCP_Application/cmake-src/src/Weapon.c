/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Weapon.c>
 *
 *  Description:
 *  ------------
 *  The Weapon module manages the interface, control logic, and operational status of the weapon system.
 *  It provides high-level control functions for arming, firing, safety mechanisms, and status monitoring.
 *
 *  Function(s):
 *  ------------
 *  - WeaponInit                               : Initializes weapon control
 *  - WeaponSyncInterruptHandler               : Timer interrupt handler
 *  - WeaponUpdate                             : Update weapon states (called at a rate of 600 Hz)
 *  - WeaponManagementStandby                  : Weapon standby state management
 *  - WeaponManagementWaitForOn                : Weapon wait for on state management
 *  - WeaponManagementArmed                    : Weapon armed state management
 *  - WeaponManagementBatError                 : Weapon battery error state management
 *  - WeaponManagementBatOn                    : Weapon battery on state management
 *  - WeaponManagementLaunched                 : Weapon launched state management
 *  - WeaponManagementTestServos               : Weapon test servos state management
 *  - WeaponGetReadyState                      : Weapon Ready state management
 *  - WeaponGetReadyStateWIAC82                : Weapon Ready WIAC82 state management
 *  - WeaponCalculateP32_LAR                   : Weapon LAR Calculation state management
 *  - WeaponInsideLAR                          : Weapon whether is inside the LAR or not
 *  - WeaponGetHealthAndStatus                 : Build and returns weapon health & status
 *  - WeaponGetTelemetry                       : Build and returns weapon telemetry
 *  - WeaponGetDummyHealthStatusAndTelemetry   : Build and returns weapon health & status
 *  - WeaponInfoToCsv                          : Weapon data csv format to AUX PORT
 *  ...
 *
 *
 ******************************************************************************/


#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "TypeDefines.h"
#include "FPGA.h"
#include "Util.h"
#include "Timer.h"
#include "Interrupts.h"
#include "FIFO.h"
#include "ScheduleControl.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "MCP.h"
#include "SAL_Seeker.h"
#include "Servos.h"
#include "WiFi.h"
#include "IMU.h"
#include "BIT.h"
#include "Guidance.h"
#include "UBLOX.h"
#include "Nav.h"
#include "FIFO.h"
#include "UART.h"
#include "MLAN.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "TaskManager.h"
#include "BatteryPack.h"
#include "P32LarApproximation.h"
#include "AircraftInterface.h"
#include "PressureSensor.h"
#include "MLAN.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"

volatile sWeaponInfo Weapon;

static unsigned char PrevActivateWeaponCommand;
static TIME WeaponActivationChangeTimestamp;
static TIME WeaponNotReadyTimestamp;
static TIME WeaponCDSSTimestamp;

static void WeaponSyncInterruptHandler(unsigned int Input);
static unsigned char WeaponGetReadyState(void);
static unsigned char WeaponGetReadyStateWIAC82(void);
static unsigned char WeaponInsideLAR(void);
static void WeaponManagementStandby(void);
static void WeaponManagementWaitForOn(void);
static void WeaponManagementArmed(void);
static void WeaponManagementBatError(void);
static void WeaponManagementBatOn(void);
static void WeaponManagementLaunched(void);
static void WeaponManagementTestServos(void);
static void (*WeaponStateManagement)(void);

void WeaponInit(void)
{
  PrevActivateWeaponCommand = FALSE;
  WeaponActivationChangeTimestamp = 0;

  WeaponStateManagement = WeaponManagementStandby;

  memset((void *) &Weapon, 0, sizeof(sWeaponInfo));

  Weapon.Status.Bits.State = wsSTANDBY;

  Weapon.Flags.Bits.SwitchOnBattery = FALSE;
  Weapon.Flags.Bits.BatteryError = FALSE;
  Weapon.Flags.Bits.GNSS_Lock = FALSE;
  Weapon.Flags.Bits.GoNoGo = FALSE;
  Weapon.Flags.Bits.Ready = FALSE;
  Weapon.Flags.Bits.ActivateSAD = FALSE;
  Weapon.Flags.Bits.MissionOk = FALSE;
  Weapon.Flags.Bits.WindOk = FALSE;
  Weapon.Flags.Bits.WeaponBunt = FALSE;
  Weapon.Flags.Bits.MissionMode = mmOPPORTUNITY;

  Weapon.ConfigureUBLOX_Receiver = FALSE;

  Weapon.AuxToCsv = 0;

  // Enable seeker sync timer interrupt to interrupt 0 us after rising edge
  *((volatile unsigned int *) FPGA_SYNC_SEEKER_INT_TIME_0_ADDR) = 0;
  *((volatile unsigned int *) FPGA_SYNC_ENABLE_INTERRUPT_ADDR) = 0x01;   // Enable Seeker sync interrupt 0 (set bit 0)
  EnableHardwareInterrupt(WeaponSyncInterruptHandler, 0, intSYNC, TRUE);
}

void WeaponSyncInterruptHandler(unsigned int Input)
{
  unsigned int mask;


  mask = *((volatile unsigned int *) FPGA_SYNC_ACTIVE_INTERRUPT_ADDR) & 0x7FF;

  // Seeker sync interrupt 0
  if (mask & 0x01) {
    Weapon.SeekerSyncEventTime = GetSystemTime() - *((volatile unsigned int *) FPGA_SYNC_SEEKER_TIMER_VALUE_ADDR);
  }

  // Clear all sync interrupts
  *((volatile unsigned int *) FPGA_SYNC_CLEAR_INTERRUPT_ADDR) = mask;
}

void WeaponUpdate(void)
{
  float battery_capacity;
  volatile sMCP_StatusRegister mcp_status_register;
  volatile sMCP_ControlRegister mcp_ctrl_register;


  mcp_status_register = *((volatile sMCP_StatusRegister *) FPGA_STATUS_REG_ADDR);
  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);

  Weapon.Discretes.Bits.WeaponActivation       = mcp_status_register.ActivateWeapon;
  Weapon.Discretes.Bits.BatterySwitchedOn      = mcp_status_register.BatterySwitchedOn;
  Weapon.Discretes.Bits.LaunchLinkPresent      = mcp_status_register.LaunchLinkPresent;
  Weapon.Discretes.Bits.LanyardPresent         = mcp_status_register.LanyardPresent;
  Weapon.Discretes.Bits.LanyardPulled          = mcp_status_register.LanyardPulled;
  Weapon.Discretes.Bits.ServosEnabled          = Servo.Discretes.Bits.MotorsEnabled;
  Weapon.Discretes.Bits.TailFinUnlocked        = Servo.Discretes.Bits.TailFinUnlock;
  Weapon.Discretes.Bits.HILS_Mode              = mcp_status_register.HILS_Mode;
  Weapon.Discretes.Bits.DownloadCablePresent   = mcp_status_register.DownloadCablePresent;
  Weapon.Discretes.Bits.GNSS_AntennaSwitch     = mcp_ctrl_register.GNSS_Antenna;
  Weapon.Discretes.Bits.Reserved               = 0;

  if ((mcp_status_register.STC1_State > 0) && (mcp_status_register.STC2_State > 1)) {
    Weapon.Status.Bits.STC1_State = mcp_status_register.STC1_State - 1;
    Weapon.Status.Bits.STC2_State = mcp_status_register.STC2_State - 2;
  }
  else {
    Weapon.Status.Bits.STC1_State = 0;
    Weapon.Status.Bits.STC2_State = 0;
  }

  // Determine if the weapon should be activated
  if (WeaponSettings.WeaponConfig.BatteryActivation == baAIRCRAFT) {
    Weapon.Flags.Bits.SwitchOnBattery = Weapon.Discretes.Bits.WeaponActivation;
  }
  else if (WeaponSettings.WeaponConfig.BatteryActivation == baWIRELESS_CONTROLLER) {
    Weapon.Flags.Bits.SwitchOnBattery = WiFi.ActivateWeapon;
  }
  else {
    Weapon.Flags.Bits.SwitchOnBattery = Weapon.Discretes.Bits.WeaponActivation | WiFi.ActivateWeapon;
  }

  // Check whether Weapon Activation has changed
  if (PrevActivateWeaponCommand != Weapon.Flags.Bits.SwitchOnBattery) {
    PrevActivateWeaponCommand = Weapon.Flags.Bits.SwitchOnBattery;
    WeaponActivationChangeTimestamp = Weapon.SystemTime;

    // Change battery temperature control
    if (Weapon.Discretes.Bits.BatterySwitchedOn) {
      // Battery on, temperature control off
      if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
    	  AUXsprintf("%u:Weapon - TEMPERATURE CONTROL (Heaters) ---- OFF ----\n",GetTimeUs());
      BatteryPackSetCommand(batSET_CONTROL_MODE, btcOFF);
    }
    else {
      // Battery off, temperature control to auto
      if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
    	  AUXsprintf("%u:Weapon - TEMPERATURE CONTROL (Heaters) ---- AUTO ----\n",GetTimeUs());
      BatteryPackSetCommand(batSET_CONTROL_MODE, btcAUTO);
    }
  }

  // Rafale has a different logic for READY state (ICD_WPN_33 Logical ICD)
  if (AUXCTRL_SS.MAINControls.WIAC82_On)
	  Weapon.Flags.Bits.Ready = WeaponGetReadyStateWIAC82();
  else
	  Weapon.Flags.Bits.Ready = WeaponGetReadyState();

  if (Weapon.Flags.Bits.Ready && Weapon.Flags.Bits.MissionOk && Weapon.Discretes.Bits.BatterySwitchedOn) {
    if (Weapon.UseAircraftLAR) {
      Weapon.Flags.Bits.GoNoGo = Weapon.InsideAircraftLAR;
    }
    else if (Weapon.Flags.Bits.MissionMode == mmPROGRAMMED) {
      Weapon.Flags.Bits.GoNoGo = WeaponInsideLAR();
    }
    else {
      // Opportunity mode - GO/NO GO is true if the following requirements below are met (see common ICD):
      //    1. Weapon seeker type is SAL seeker
      //    2. Weapon pitch >= 0 deg OR
      //       Weapon pitch is between -45 deg and -20 deg AND seeker track is valid AND seeker angles are between -10 deg and +10 deg
      //       AND the height above the target is between 3000 ft (914 m) and 15000 ft (5000 m)

      if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
        if (Weapon.Pitch >= 0.0) {
          Weapon.Flags.Bits.GoNoGo = TRUE;
        }
        else if ((Weapon.Pitch >= -0.785398) && (Weapon.Pitch <= -0.349066)) {
        	if (AUXCTRL_SS.MAINControls.DSAL_On) {
        		Weapon.Flags.Bits.GoNoGo = ((SAL_Seeker.Status.Bits.TrackStatus == TRUE) && (SAL_Seeker.TargetYaw >= -0.174533) &&
        				(SAL_Seeker.TargetYaw <= 0.174533) && (SAL_Seeker.TargetPitch >= -0.174533) && (SAL_Seeker.TargetPitch <= 0.174533) &&
						((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) >= 914) && ((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) <= 4572)) ? TRUE : FALSE;
        	} else if (AUXCTRL_SS.MAINControls.MPSAL_On){
        		Weapon.Flags.Bits.GoNoGo = ((SAL_SS.Detection.DetectionStatus == TRUE) && (SAL_SS.GuidanceData.TargetYaw >= -0.174533) &&
        		        				(SAL_SS.GuidanceData.TargetYaw <= 0.174533) && (SAL_SS.GuidanceData.TargetPitch >= -0.174533) && (SAL_SS.GuidanceData.TargetPitch <= 0.174533) &&
        								((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) >= 914) && ((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) <= 4572)) ? TRUE : FALSE;
        	} else if (AUXCTRL_SS.MAINControls.QSAL_On){
        		Weapon.Flags.Bits.GoNoGo = ((QSAL_SS.SharedData.Status.Bits.Tracking == TRUE) && (QSAL_SS.SharedData.TargetYaw >= -0.174533) &&
        		        				(QSAL_SS.SharedData.TargetYaw <= 0.174533) && (QSAL_SS.SharedData.TargetPitch >= -0.174533) && (QSAL_SS.SharedData.TargetPitch <= 0.174533) &&
        								((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) >= 914) && ((Weapon.Altitude - Weapon.Mission.PriTgtAltitude) <= 4572)) ? TRUE : FALSE;
        	} else {
        		Weapon.Flags.Bits.GoNoGo = FALSE;
        	}
        }
        else {
          Weapon.Flags.Bits.GoNoGo = FALSE;
        }
      }
      else {
        Weapon.Flags.Bits.GoNoGo = FALSE;
      }
    }
  }
  else {
    Weapon.Flags.Bits.GoNoGo = FALSE;
  }

  if (BatteryPack.Capacity >= BatteryPack.CumulativeUsage) {
    battery_capacity = (BatteryPack.Capacity - BatteryPack.CumulativeUsage) / BatteryPack.Capacity;
    Weapon.BatteryCapacity = (unsigned char) (battery_capacity * 100.0 + 0.5);
    Weapon.BatteryTimeRemaining = BATTERY_MAX_ON_TIME * battery_capacity;
  }
  else {
    Weapon.BatteryCapacity = 0;
    Weapon.BatteryTimeRemaining = 0;
  }

  // Force 2Hz interrupt if UBLOX fail
  if (BIT.SBIT_Complete && Weapon.CBIT.Bits.GNSS_CommsFail) {
    if (((Weapon.SystemTime & 0xFFFFFFFF) - Weapon.Weapon2HzTimestamp) >= 500000) {
      TaskManagerAdd(task2HZ);
    }
  }

  WeaponStateManagement();

  // Switch battery on or off depending the weapon state
  switch (Weapon.Status.Bits.State) {
    case wsWAIT_FOR_BATTERY_ON :
    case wsBATTERY_ON :
    case wsLAUNCHED :
    case wsSERVO_TEST : {
      if (!mcp_ctrl_register.SwitchOnBattery) {
        // Switch on the battery
        MCP_WeaponBatteryCtrl(ON);
      }
    } break;
    case wsBATTERY_ARMED : {
    } break;
    default : {
      if (mcp_ctrl_register.SwitchOnBattery) {
        // Switch off the battery
        MCP_WeaponBatteryCtrl(OFF);
      }
    } break;
  }

  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);

  // Set Wait For Launch when weapon battery is switched on
  if ((Weapon.Status.Bits.State >= wsBATTERY_ON) && (Weapon.Status.Bits.State != wsBATTERY_ARMED)) {
    mcp_ctrl_register.STC1_WaitForLaunch = TRUE;
    mcp_ctrl_register.STC2_WaitForLaunch = TRUE;
    Weapon.DiscretesDummy.Bits.WaitForLaunchSTC = TRUE;
  }
  else {
    mcp_ctrl_register.STC1_WaitForLaunch = FALSE;
    mcp_ctrl_register.STC2_WaitForLaunch = FALSE;
    Weapon.DiscretesDummy.Bits.WaitForLaunchSTC = FALSE;
  }

  if (Weapon.Status.Bits.WeaponLaunched) {
    // Activate servo's 1.6s after launch one at a time 20ms apart to minimize current spikes
    if (Weapon.ActiveServos < 4) {
      if (Weapon.SystemTime > 1600000) {
        Weapon.ActiveServos = (Weapon.SystemTime - 1600000) / 20000;
      }
    }

    // Activate SAD 2s after launch
    if (!Weapon.Flags.Bits.ActivateSAD) {
      if (Weapon.TimeSinceLaunch > 2000000) {
        Weapon.Flags.Bits.ActivateSAD = TRUE;
        mcp_ctrl_register.STC1_SAD_Active = 1;
        mcp_ctrl_register.STC2_SAD_Active = 1;
      }
    }
  }
  else {
    Weapon.Flags.Bits.ActivateSAD = FALSE;
  }

  if (Weapon.AuxToCsv)
	  WeaponInfoToCsv();

  *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR) = mcp_ctrl_register;
}

void WeaponManagementStandby(void)
{
  Weapon.Status.Bits.State = wsSTANDBY;
  Weapon.Flags.Bits.BatteryError     = FALSE;
  Weapon.Flags.Bits.WeaponArmed      = FALSE; // Weapon Armed
  Weapon.Flags.Bits.WeaponCDSS       = FALSE; // Weapon Committed to Store Separate
  Weapon.BatteryFirstActivation      = FALSE; // First Battery Activation before Weapon Armed

  if (AUXCTRL_SS.MAINControls.WIAC82_On){
	  // Execute Arming from the Aircraft & Release Consent is NOT sent
	  if (WIAC82_SS.WIAC82ExecuteArming && !WIAC82_SS.WIAC82ReleaseConsent) {

		  if (Weapon.Flags.Bits.SwitchOnBattery && Weapon.Discretes.Bits.LaunchLinkPresent && Weapon.Flags.Bits.Ready) {
			  // Switch ON battery flag, Launch Link Present and weapon must be ready to activate the battery
			  WeaponStateManagement = WeaponManagementWaitForOn;
		  }
	  }
  } else {
	  if (Weapon.Flags.Bits.SwitchOnBattery && Weapon.Discretes.Bits.LaunchLinkPresent) {
		// MCP_WeaponBatteryCtrl(ON);
		WeaponStateManagement = WeaponManagementWaitForOn;
	  }
  }
}

void WeaponManagementWaitForOn(void)
{
  Weapon.Status.Bits.State = wsWAIT_FOR_BATTERY_ON;

  if (Weapon.Discretes.Bits.BatterySwitchedOn) {
	  if (AUXCTRL_SS.MAINControls.WIAC82_On) {
		  if (!Weapon.BatteryFirstActivation) {
			  // Checking Battery Voltage higher than 19V (2s)
			  if (MCP.PSU_28V > MIN_BATT_VOLT_LEVEL && GetElapsedTime(WeaponActivationChangeTimestamp) > 2000000) {
				  Weapon.BatteryFirstActivation = TRUE;
				  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					  AUXsprintf("%u:Weapon ---- FIRST BATTERY ACTIVATION ----\n",GetTimeUs());
				  WeaponStateManagement = WeaponManagementBatOn;
			  } else {
			  // Battery Voltage is not higher than 19V (After 10s)
				  if (GetElapsedTime(WeaponActivationChangeTimestamp) > 10000000) {
					  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
						  AUXsprintf("%u:Weapon ---- BATTERY Error Voltage less than 19V (After 10) ----\n",GetTimeUs());
					  MCP_WeaponBatteryCtrl(OFF);
					  Weapon.Flags.Bits.BatteryError = TRUE;
					  WeaponStateManagement = WeaponManagementBatError;
				  }
			  }
		  } else {
			  WeaponStateManagement = WeaponManagementBatOn;
		  }
	  }
	  else {
		// Battery successfully switched on
		WeaponStateManagement = WeaponManagementBatOn;
		//MCP_SetWaitForLaunch(TRUE);
	  }

  }
  else if (GetElapsedTime(WeaponActivationChangeTimestamp) > 7000000) {
    // Battery on timeout reached (battery did not switch on within 7s)
    MCP_WeaponBatteryCtrl(OFF);
    Weapon.Flags.Bits.BatteryError = TRUE;
    WeaponStateManagement = WeaponManagementBatError;
  }
  else if (!Weapon.Flags.Bits.SwitchOnBattery) {
    // Switch off battery command
    // MCP_WeaponBatteryCtrl(OFF);
    WeaponStateManagement = WeaponManagementStandby;
  }
}

void WeaponManagementArmed(void)
{
  Weapon.Status.Bits.State = wsBATTERY_ARMED;

  // Weapon must be Ready to be Released
  if (WIAC82_SS.WIAC82ReleaseConsent) {
	  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
		  AUXsprintf("%u:Weapon ---- RELEASE CONSENT & BATTERY POWER SWITCHING ON ----\n",GetTimeUs());

	  if (Weapon.Flags.Bits.SwitchOnBattery && Weapon.Discretes.Bits.LaunchLinkPresent) {
		  WeaponActivationChangeTimestamp = Weapon.SystemTime; // Second activation (New TimeStamp)
		  WeaponStateManagement = WeaponManagementWaitForOn;
	  }
  }

  if (Weapon.Flags.Bits.Ready){
	  Weapon.ArmedNotReady = FALSE;
  } else {
	  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
		  AUXsprintf("%u:Weapon ---- NOT READY BUT LESS 1SEC ----\n",GetTimeUs());
	  if (!Weapon.ArmedNotReady){
		  WeaponNotReadyTimestamp = Weapon.SystemTime;
		  Weapon.ArmedNotReady    = TRUE;
	  }
	  // To prevent sporadic errors, weapon must be NOT READY for at least on second
	  if (GetElapsedTime(WeaponNotReadyTimestamp) > 1000000){
		  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
			  AUXsprintf("%u:Weapon ---- NOT READY AFTER BEING ARMED ----\n",GetTimeUs());
		  WIAC82_SendBatteryDeactivation();
		  WIAC82_SS.PreviousCriticalMonitorArmed = FALSE;
		  Weapon.ArmedNotReady    				 = FALSE;
		  WeaponStateManagement = WeaponManagementStandby;
	  }
  }

  if (!WIAC82_SS.WIAC82ExecuteArming)
	  WeaponStateManagement = WeaponManagementStandby;

}

void WeaponManagementBatError(void)
{
  Weapon.Status.Bits.State = wsBATTERY_ERROR;

  if (!Weapon.Flags.Bits.SwitchOnBattery) {
    WeaponStateManagement = WeaponManagementStandby;
  }
}

void WeaponManagementBatOn(void)
{
  Weapon.Status.Bits.State = wsBATTERY_ON;

  if (AUXCTRL_SS.MAINControls.WIAC82_On) {
	  // Weapon has been previously ARMED
	  if (Weapon.Flags.Bits.WeaponArmed) {
		  if ((WIAC82_SS.WIAC82ReleaseConsent && WIAC82_SS.WIAC82CommitToSeparate) || Weapon.Flags.Bits.WeaponCDSS) {

			  if (! Weapon.Flags.Bits.WeaponCDSS) {
				  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
					  AUXsprintf("%u:Weapon ---- COMMITTED TO STORE SEPARATED ----\n",GetTimeUs());
				  Weapon.Flags.Bits.WeaponCDSS = TRUE;
				  WeaponCDSSTimestamp          = Weapon.SystemTime;
			  }

			  if (!Weapon.Discretes.Bits.LaunchLinkPresent && Weapon.Discretes.Bits.LanyardPulled) {
				  // Weapon launched
				  Weapon.TimeAtLaunch = GetSystemTime();
				  Weapon.TimeSinceLaunch = 0;

				  Weapon.Status.Bits.WeaponLaunched = TRUE;

				  if (Weapon.ServoTest == WEAPON_SERVO_TEST_ID) {
					  WeaponStateManagement = WeaponManagementTestServos;
				  } else {
					  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
						  AUXsprintf("%u:Weapon ---- LAUNCHED ----\n",GetTimeUs());
					  WeaponStateManagement = WeaponManagementLaunched;
				  }
			  // Weapon CSDD was sent to Rafale and after 5s the LaunchLink is still present (Switch Off Battery)
			  } else if (Weapon.Discretes.Bits.LaunchLinkPresent && GetElapsedTime(WeaponCDSSTimestamp) > 5000000) {
					  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
						  AUXsprintf("%u:Weapon ---- Time after CDSS is greater than 5s (NOT RELEASED) ----\n",GetTimeUs());
					  WIAC82_MisfiredWeapon();
					  MCP_WeaponBatteryCtrl(OFF); // Battery OFF
					  WeaponStateManagement = WeaponManagementStandby;
			  }
		  } else {
			  if (GetElapsedTime(WeaponActivationChangeTimestamp) > 1000000) {
			  	  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
			  		  AUXsprintf("%u:Weapon ---- RELEASE SEQUENCE WITHOUT COMMIT TO SEPARATE (After 1 sec)----\n",GetTimeUs());
				  if (Weapon.Discretes.Bits.LaunchLinkPresent) {
					  WIAC82_SendBatteryDeactivation();
					  MCP_WeaponBatteryCtrl(OFF); // Battery OFF
					  WeaponStateManagement = WeaponManagementArmed;
				  }
			  }
		  }
	  }else{
		  // Weapon has NOT been previously ARMED
		  // After 10s Battery ON, Weapon is ARMED.
		  if (GetElapsedTime(WeaponActivationChangeTimestamp) > 10000000)
		  {
			  Weapon.Flags.Bits.WeaponArmed = TRUE;
			  WIAC82_SendBatteryDeactivation();
			  MCP_WeaponBatteryCtrl(OFF); // Aircraft Power
			  if (AUXCTRL_SS.WIAC82Controls.ReleaseSequence)
				  AUXsprintf("%u:Weapon ---- ARMED ----\n",GetTimeUs());
			  WeaponStateManagement = WeaponManagementArmed;
		  }
  	  }

	  if (!Weapon.Flags.Bits.SwitchOnBattery) {
		  if (Weapon.Discretes.Bits.LaunchLinkPresent && GetElapsedTime(WeaponActivationChangeTimestamp) > 500000) {
			  WeaponStateManagement = WeaponManagementStandby;
		  }
	  }

  } else {
	  if (!Weapon.Discretes.Bits.LaunchLinkPresent && Weapon.Discretes.Bits.LanyardPulled) {

		  // Weapon launched
		  Weapon.TimeAtLaunch = GetSystemTime();
		  Weapon.TimeSinceLaunch = 0;

		  Weapon.Status.Bits.WeaponLaunched = TRUE;

		  if (Weapon.ServoTest == WEAPON_SERVO_TEST_ID) {
			  WeaponStateManagement = WeaponManagementTestServos;
		  } else {
			  WeaponStateManagement = WeaponManagementLaunched;
		  }
	  } else if (!Weapon.Flags.Bits.SwitchOnBattery) {
		  if (Weapon.Discretes.Bits.LaunchLinkPresent && GetElapsedTime(WeaponActivationChangeTimestamp) > 500000) {
			  // MCP_WeaponBatteryCtrl(OFF);
			  WeaponStateManagement = WeaponManagementStandby;
			  //MCP_SetWaitForLaunch(FALSE);
		  }
	  }
  }
}

void WeaponManagementLaunched(void)
{
  Weapon.Status.Bits.State = wsLAUNCHED;
  Weapon.TimeSinceLaunch = Weapon.SystemTime - Weapon.TimeAtLaunch;

  if (Weapon.Flags.Bits.WeaponBunt && (Weapon.Discretes.Bits.GNSS_AntennaSwitch != antREAR)) {
    MCP_SelectGNSS_Antenna(antREAR);
  }

  if (Weapon.Discretes.Bits.HILS_Mode && (Weapon.ServoTest == WEAPON_SERVO_TEST_ID)) {
    WeaponStateManagement = WeaponManagementTestServos;
  }
}

void WeaponManagementTestServos(void)
{
  Weapon.Status.Bits.State = wsSERVO_TEST;
  Weapon.Flags.Bits.Ready = FALSE;
}

static unsigned char WeaponGetReadyState(void)
{
  unsigned char battery_logic;


  if (Weapon.Flags.Bits.SwitchOnBattery) {
    if (Weapon.Discretes.Bits.BatterySwitchedOn) {
      // Weapon activated and battery is on
      battery_logic = TRUE;
    }
    else if (Weapon.Flags.Bits.BatteryError) {
      // Battery error due to timeout
      battery_logic = FALSE;
    }
    else {
      // Toggle Ready while battery is being switched on (Weapon Activate is switched on but battery is off)
      battery_logic = ((GetElapsedTime(WeaponActivationChangeTimestamp) % 2000000) < 1000000) ? TRUE : FALSE;
    }
  }
  else if (!Weapon.Discretes.Bits.BatterySwitchedOn) {
    // Weapon Activation switch is off and the battery is off
    battery_logic = TRUE;
  }
  else {
    // For 500 ms after Weapon Activation is switched off but the battery is still on
    battery_logic = FALSE;
  }

  return (Weapon.Flags.Bits.GNSS_Lock && Weapon.Flags.Bits.MissionOk && !Weapon.CBIT.Bits.CriticalFail && battery_logic);
}

static unsigned char WeaponGetReadyStateWIAC82(void)
{
  unsigned char battery_logic;

  if (Weapon.Flags.Bits.SwitchOnBattery) {
    if (Weapon.Discretes.Bits.BatterySwitchedOn) {
      // Weapon activated and battery is on
      battery_logic = TRUE;
    }
    else if (Weapon.Flags.Bits.BatteryError) {
      // Battery error due to timeout
      battery_logic = FALSE;
    }
    else {
      if(Weapon.Status.Bits.State != wsBATTERY_ARMED){
    	  // Toggle Ready while battery is being switched on (Weapon Activate is switched on but battery is off)
    	  battery_logic = ((GetElapsedTime(WeaponActivationChangeTimestamp) % 2000000) < 1000000) ? TRUE : FALSE;
      } else {
    	  // Armed Mode (First Battery Activation OK)
    	  battery_logic = TRUE;
      }
    }
  }
  else if (!Weapon.Discretes.Bits.BatterySwitchedOn) {
    // Weapon Activation switch is off and the battery is off
    battery_logic = TRUE;
  }
  else {
    // For 500 ms after Weapon Activation is switched off but the battery is still on
    battery_logic = FALSE;
  }

  // GNSS Locked status is not mandatory before release.
  // A weapon with GNSS not locked can be fired. (DATA ITEME NUMBER 52)
  // Weapon.Flags.Bits.GNSS_Lock GNSS only?? with Sal Seeker??

  // Weapon is READY for Rafale WIAC 82 when is ARMED (Battery Activated), Aligned, Mission Valid, NO Critical Fails.
  return (Weapon.Status.Bits.NAV_Aligned && Weapon.Flags.Bits.MissionOk && !Weapon.CBIT.Bits.CriticalFail && battery_logic);
}

void WeaponCalculateP32_LAR(sWeaponLAR *LAR, unsigned char InRange)
{
  unsigned char error;
  sLaunchParams_t lar_input;
  sTgtParams_t lar_tgt;
  sBallisticParams_t ballistic_params;
  sLar_t lar;


  memset((void *) LAR, 0, sizeof(sWeaponLAR));

  error = FALSE;

  // Calculate LAR if the weapon position as well as the mission parameters are valid
  if ((Weapon.Status.Bits.NAV_Mode >= 30) && Weapon.Flags.Bits.MissionOk && Weapon.Mission.LAR_LaunchConditionsValid) {
    lar_input.dLat_rad = Aircraft.Latitude;       //use aircraft position until weapon position is fixed
    lar_input.dLong_rad = Aircraft.Longitude;     //use aircraft position until weapon position is fixed
    lar_input.dAlt_m = (int) (Weapon.Mission.LAR_Altitude * 1000.0 + 0.5) / 1000.0;
    lar_input.dMach = (int) (Weapon.Mission.LAR_Mach * 1000.0 + 0.5) / 1000.0;
    lar_input.dTrack_rad = Weapon.Mission.LAR_Track;

    if (Weapon.Mission.LAR_WindValid) {
      lar_input.dWindSpeed_m_s = Weapon.Mission.LAR_WindSpeed;
      lar_input.dWindDir_rad = Weapon.Mission.LAR_WindDirection;
      lar_input.uiWindValidFlag = Weapon.Mission.LAR_WindValid;
    }
    else {
      lar_input.dWindSpeed_m_s = 0;
      lar_input.dWindDir_rad = 0;
      lar_input.uiWindValidFlag = FALSE;
    }

    if (!WITHIN_LIMITS(lar_input.dLat_rad, -PI / 2.0, PI / 2.0) ||
        !WITHIN_LIMITS(lar_input.dLong_rad, -PI, PI)            ||
        !WITHIN_LIMITS(lar_input.dTrack_rad, 0.0, TWO_PI)       ||
        !WITHIN_LIMITS(lar_input.dWindSpeed_m_s, 0, 80.0)       ||
        !WITHIN_LIMITS(lar_input.dWindDir_rad, 0.0, TWO_PI)     ||
        !WITHIN_LIMITS(lar_input.dMach, 0.7, 0.9)               ||
        !WITHIN_LIMITS(lar_input.dAlt_m, 3048.0, 12802.0)         ) {
      error = TRUE;
    }
  }
  else {
    MCP.DebugInt[0] = 8;
    error = TRUE;
  }

  // Do not calculate LAR if input launch conditions are invalid
  if (!error) {
    if (Weapon.Flags.Bits.MissionMode == mmPROGRAMMED) {
      lar_tgt.dLat_rad = Weapon.Mission.PriTgtLatitude;
      lar_tgt.dLong_rad = Weapon.Mission.PriTgtLongitude;
      lar_tgt.dAlt_m = Weapon.Mission.PriTgtAltitude;
      lar_tgt.dDiveAng_rad = (InRange == FALSE) ? Weapon.Mission.DiveAngle : DEG2RAD(30.0);

      if (calculateP32Lar(lar_input, lar_tgt, &lar) == 0) {
        LAR->LAR_Rad = lar.dLarRadius_m + 0.5;
        LAR->DistX = (lar.dRelX_DistToLar < 0) ? (lar.dRelX_DistToLar - 0.5) : (lar.dRelX_DistToLar + 0.5);
        LAR->DistY = (lar.dRelY_DistToLar < 0) ? (lar.dRelY_DistToLar - 0.5) : (lar.dRelY_DistToLar + 0.5);
        LAR->Latitude = RAD2DEG(lar.dLarCtrLat_rad);
        LAR->Longitude = RAD2DEG(lar.dLarCtrLong_rad);
        LAR->RelBrngToLAR = ConvertRadToDegPos(lar.dRelBearToLAR);
        LAR->RelBrngToTGT = ConvertRadToDegPos(lar.dRelBearToTgt_rad);
        LAR->DistToLAR = lar.dGndDistToLAR_m + 0.5;
        LAR->DistToTGT = lar.dGndDistToTgt_m + 0.5;
        LAR->LAR_Rad_Excl = lar.dExclRadius_m + 0.5;
        LAR->TimeToImpact = (lar.dTimeToImpact_s < WEAPON_MAX_LAR_TIME) ? (lar.dTimeToImpact_s + 0.5) : WEAPON_MAX_LAR_TIME;
        LAR->TimeToIllum = lar.dTimeToIllum_s + 0.5;
        LAR->TimeToGo = lar.dTimeToGo_s + 0.5;
        LAR->TimeToExit = lar.dTimeToExit_s + 0.5;

        if ((lar.uwInsideLar == FALSE) && (lar.dTimeToGo_s < 0.0001)) {
          error = TRUE;
        }

        if (lar.dLarRadius_m == 0) {
          error = TRUE;
        }
      }
      else {
        error = TRUE;
      }
    }
    else if (calculateBallisticParams(Weapon.Mission.PriTgtAltitude, lar_input.dAlt_m, lar_input.dMach, &ballistic_params)) {
      LAR->LAR_Rad = 0;
      LAR->DistX = 0;
      LAR->DistY = 0;
      LAR->Latitude = 0;
      LAR->Longitude = 0;
      LAR->RelBrngToLAR = 0;
      LAR->RelBrngToTGT = 0;
      LAR->DistToLAR = 0;
      LAR->DistToTGT = ballistic_params.dBallisticRange_m;
      LAR->LAR_Rad_Excl = 0;
      LAR->TimeToImpact = ballistic_params.dBallisticFlightTime_s;
      LAR->TimeToIllum = WEAPON_MAX_LAR_TIME;
      LAR->TimeToGo = WEAPON_MAX_LAR_TIME;
      LAR->TimeToExit = 0;
      LAR->Valid = TRUE;
    }
    else {
      error = TRUE;
    }
  }

  if (!error) {
    LAR->Valid = TRUE;
  }
  else {
    LAR->Valid = FALSE;
    LAR->TimeToImpact = WEAPON_MAX_LAR_TIME;
    LAR->TimeToIllum = WEAPON_MAX_LAR_TIME;
    LAR->TimeToGo = WEAPON_MAX_LAR_TIME;
    LAR->TimeToExit = 0;
  }
}

unsigned char WeaponInsideLAR(void)
{
  if (Weapon.LAR.Valid) {
    if ((Weapon.LAR.DistToLAR < Weapon.LAR.LAR_Rad) && (Weapon.LAR.DistToTGT > Weapon.LAR.LAR_Rad_Excl)) {
      return TRUE;
    }
  }

  return FALSE;
}

void WeaponGetHealthAndStatus(sWeaponHealthAndStatus *HealthAndStatus)
{
  uWeaponHealthBIT cbit;
  uAP_Config ap_config;
  uWeaponWC_Flags flags;


  if (!WeaponSettings.WeaponConfig.DTAC_Present && !Weapon.Status.Bits.WeaponLaunched) {
    Weapon.Mission.LAR_LaunchConditionsValid = TRUE;
    Weapon.Mission.LAR_Altitude = NavTel100Hz.T100B1.NAV_AltLLA;
    Weapon.Mission.LAR_Mach = GuidanceOut.mach;
    Weapon.Mission.LAR_Track = DEG2RAD(ConvertRadToDegPos(NavTel100Hz.T100B1.NAV_AngPsi));

    Aircraft.Latitude = NavTel100Hz.T100B1.NAV_LatLLA;
    Aircraft.Longitude = NavTel100Hz.T100B1.NAV_LongLLA;
    WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR, FALSE);
  }

  cbit.DWord                              = 0;
  cbit.Bits.MCP_Fail                      = Weapon.CBIT.Bits.GCA_Fail;
  cbit.Bits.IMU_Fail                      = Weapon.CBIT.Bits.IMU_Fail;
  cbit.Bits.UBLOX_Fail                    = Weapon.CBIT.Bits.GNSS_Fail;
  cbit.Bits.ServosFail                    = Weapon.CBIT.Bits.ServoFail;
  cbit.Bits.WiFiFail                      = Weapon.CBIT.Bits.WirelessLinkFail;
  cbit.Bits.SeekerFail                    = Weapon.CBIT.Bits.SeekerFail;
  cbit.Bits.BatteryPackFail               = Weapon.CBIT.Bits.BatteryFail;
  cbit.Bits.IMU_CommsFail                 = Weapon.CBIT.Bits.IMU_CommsFail;
  cbit.Bits.UBLOX_CommsFail               = Weapon.CBIT.Bits.GNSS_CommsFail;
  cbit.Bits.ServosCommsFail               = Weapon.CBIT.Bits.ServoCommsFail;
  cbit.Bits.WiFiCommsFail                 = Weapon.CBIT.Bits.WirelessLinkCommsFail;
  cbit.Bits.SeekerCommsFail               = Weapon.CBIT.Bits.SeekerCommsFail;
  cbit.Bits.ArcnetReconFail               = Weapon.CBIT.Bits.ArcnetReconFail;
  cbit.Bits.CriticalFail                  = Weapon.CBIT.Bits.CriticalFail;

  ap_config.Bits.BombType                 = WeaponSettings.WeaponConfig.BombType + 1;  // The +1 is to convert from new to the old ICD (0 = Unknown1, 1 = MK81
  ap_config.Bits.SeekerType               = WeaponSettings.WeaponConfig.SeekerType;
  ap_config.Bits.WingsPresent             = 0;
  ap_config.Bits.MissionGeom              = 1;
  ap_config.Bits.Spare                    = 0;

  flags.Bits.GoNoGo                       = Weapon.Flags.Bits.GoNoGo;
  flags.Bits.Ready                        = Weapon.Flags.Bits.Ready;
  flags.Bits.MissionMode                  = Weapon.Flags.Bits.MissionMode;
  flags.Bits.MissionOk                    = Weapon.Flags.Bits.MissionOk;
  flags.Bits.WindOk                       = Weapon.Flags.Bits.WindOk;
  flags.Bits.BatterySwitchedOn            = Weapon.Discretes.Bits.BatterySwitchedOn;
  flags.Bits.GNSS_Lock                    = Weapon.Flags.Bits.GNSS_Lock;
  flags.Bits.IIR_TemplateValid            = FALSE;

  HealthAndStatus->LAR_Rad                = Weapon.LAR.LAR_Rad;//LAR.radius;
  HealthAndStatus->DistX                  = 0;
  HealthAndStatus->DistY                  = 0;
  HealthAndStatus->RelBrngToLAR           = Weapon.LAR.RelBrngToLAR;//LAR.Psi2LAR;
  HealthAndStatus->RelBrngToTGT           = Weapon.LAR.RelBrngToTGT;//LAR.Psi2Tgt;
  HealthAndStatus->DistToLAR              = Weapon.LAR.DistToLAR;//LAR.Rge2LAR;
  HealthAndStatus->DistToTGT              = Weapon.LAR.DistToTGT;//LAR.Rge2Tgt;
  HealthAndStatus->LAR_Rad_Excl           = Weapon.LAR.LAR_Rad_Excl;//LAR.exclrad;
  HealthAndStatus->ImpactTime             = Weapon.LAR.TimeToImpact;//LAR.Time2Imp;
  HealthAndStatus->IllumTime              = Weapon.LAR.TimeToIllum;//LAR.Time2Lase;
  HealthAndStatus->TimeToGo               = Weapon.LAR.TimeToGo;//LAR.Time2Go;
  HealthAndStatus->GNSS_Usage             = Weapon.GNSS_Usage;
  HealthAndStatus->HealthStatusVersion    = 101;
  HealthAndStatus->WeaponFlags            = flags.Byte;
  HealthAndStatus->AP_Config              = ap_config.Byte;
  HealthAndStatus->SAL_Status             = SAL_Seeker.Status.Word;
  HealthAndStatus->WeaponBIT              = cbit.DWord;
  HealthAndStatus->BatteryCapacity        = Weapon.BatteryCapacity;
  HealthAndStatus->WirelessLinkStatus     = WiFi.Settings.Byte;
  HealthAndStatus->MissionParamsCRC       = Weapon.RawMission.CRC;
  HealthAndStatus->WindParamsCRC          = Weapon.Wind.CRC;
  HealthAndStatus->IIR_ImageCRC           = 0;
  HealthAndStatus->IIR_TemplateCnt        = 0;
  HealthAndStatus->TgtLatitude            = Weapon.Mission.PriTgtLatitude * 683565275.5764316;    // SF = (2147483648.0 / PI);
  HealthAndStatus->TgtLongitude           = Weapon.Mission.PriTgtLongitude * 683565275.5764316;   // SF = (2147483648.0 / PI);
  HealthAndStatus->TgtAltitude            = Weapon.Mission.PriTgtAltitude;
  HealthAndStatus->SAL_LaserPeriod        = 1000000.0 / Weapon.Mission.SAL_Frequency;
  HealthAndStatus->EstimatedDiveAngle     = RAD2DEG(Weapon.Mission.DiveAngle);
  HealthAndStatus->EstimatedAttackHeading = Weapon.Mission.AttackHeading * 10430.3783505;         // SF = (32768 / PI)
  HealthAndStatus->EstimatedImpactSpeed   = Weapon.Mission.ImpactSpeed;
}

void WeaponGetTelemetry(sWeaponTelemetry *Telemetry)
{
  Telemetry->LaserFreq          = Weapon.Mission.SAL_Frequency;
  Telemetry->GPS_TimeOfWeek     = UBLOX.GNSS_Result.T_meas.GPS;
  Telemetry->WeaponTime         = (float) Weapon.SystemTime / 1000000.0;
  Telemetry->ServoInputVoltage  = Servo.ScaledSupplyVoltage * 0.8;
  Telemetry->PitotTubeTemp      = 0;
  Telemetry->PressureSensTemp   = 0;
  Telemetry->WiFi_RSSI          = -WiFi.RSSI;
  Telemetry->TotalPressure      = 0;
  Telemetry->StaticPressure     = 0;
  Telemetry->WeaponLatitude     = RAD2DEG(Weapon.Latitude);
  Telemetry->WeaponLongitude    = RAD2DEG(Weapon.Longitude);
  Telemetry->WeaponAltitude     = Weapon.Altitude;
  Telemetry->CAS                = 0;
  Telemetry->LLLN_Heading       = RAD2DEG(NavTel100Hz.T100B1.NAV_AngPsi) * 100;
  Telemetry->LLLN_Pitch         = RAD2DEG(NavTel100Hz.T100B1.NAV_AngTheta) * 100;
  Telemetry->LLLN_Roll          = RAD2DEG(NavTel100Hz.T100B1.NAV_AngPhi) * 100;
  Telemetry->Discretes          = Weapon.Discretes.Word & 0xFF;   // Only the first 8 bits are used for old H&S messages
  Telemetry->NavStatus          = Weapon.Status.Bits.NAV_Mode;
  Telemetry->MCP_State          = Weapon.Status.Bits.State;
  Telemetry->LAN_ReconCnt       = MLAN.ReconCnt;
  Telemetry->PitotCBIT          = 0;
  Telemetry->SeekerCBIT         = SAL_Seeker.CBIT.DWord;
  Telemetry->MCP_CBIT           = MCP.CBIT.Word;
  Telemetry->WLinkCBIT          = WiFi.CBIT.Word;
  Telemetry->ServoA_Status      = 0;
  Telemetry->ServoB_Status      = 0;
  Telemetry->ServoC_Status      = 0;
  Telemetry->ServoD_Status      = 0;
  Telemetry->GNSS_GPS_UsedCnt   = UBLOX.GPS_SatUsedCnt;
  Telemetry->GNSS_GLO_UsedCnt   = UBLOX.GLO_SatUsedCnt;
  Telemetry->EnableUserLog      = 0;
}

void WeaponGetDummyHealthStatusAndTelemetry(sWeaponHealthAndStatusDummy *HealthAndStatusDummy, sWeaponTelemetryDummy *TelemetryDummy)
{
	uWeaponWC_FlagsDummy flagsDummy;
	uWeaponDiscretesDummy discretesDummy;
	uWeaponBITDummy weaponBitDummy;
	uWeaponStatusDummy weaponStatusDummy;

    HealthAndStatusDummy->LAR_Radius         = Weapon.LAR.LAR_Rad;
    HealthAndStatusDummy->CrossDistance      = Weapon.LAR.DistX;
    HealthAndStatusDummy->DownDistance       = Weapon.LAR.DistY;
    HealthAndStatusDummy->RelBearingLAR      = Weapon.LAR.RelBrngToLAR;//LAR.Psi2LAR;
    HealthAndStatusDummy->RelBearingTarget   = Weapon.LAR.RelBrngToTGT;//LAR.Psi2Tgt;
    HealthAndStatusDummy->GroundDistLAR      = Weapon.LAR.DistToLAR;//LAR.Rge2LAR;
    HealthAndStatusDummy->GroundDistTarget   = Weapon.LAR.DistToTGT;//LAR.Rge2Tgt;
    HealthAndStatusDummy->RadiusExclTarget   = Weapon.LAR.LAR_Rad_Excl;//LAR.exclrad;
    HealthAndStatusDummy->ExpectedImpactTime = Weapon.LAR.TimeToImpact;//LAR.Time2Imp;
    HealthAndStatusDummy->TimeToIluminate    = Weapon.LAR.TimeToIllum;//LAR.Time2Lase;
    HealthAndStatusDummy->TimeToGo           = Weapon.LAR.TimeToGo;//LAR.Time2Go;
    HealthAndStatusDummy->Spare1             = 0;

    flagsDummy.Bits.GoNoGo                                              = Weapon.Flags.Bits.GoNoGo;
    flagsDummy.Bits.Ready                                               = Weapon.Flags.Bits.Ready;
    flagsDummy.Bits.MissionMode                                         = Weapon.Flags.Bits.MissionMode;
    flagsDummy.Bits.MissionOk                                           = Weapon.Flags.Bits.MissionOk;
    flagsDummy.Bits.WindOk                                              = Weapon.Flags.Bits.WindOk;
    flagsDummy.Bits.BatterySwitchedOn                                   = Weapon.Discretes.Bits.BatterySwitchedOn;
    flagsDummy.Bits.GNSS_Lock                                           = Weapon.Flags.Bits.GNSS_Lock;
    flagsDummy.Bits.IIR_TemplateValid                                   = FALSE;
    flagsDummy.Bits.WingsPresent                                        = 0;
    flagsDummy.Bits.BombType                                            = WeaponSettings.WeaponConfig.BombType + 1;
    flagsDummy.Bits.SeekerType                                          = WeaponSettings.WeaponConfig.SeekerType;
    flagsDummy.Bits.MissionControlGeometry                              = 1;
    flagsDummy.Bits.Spare                                               = 0;

    HealthAndStatusDummy->WeaponFlags        = flagsDummy.Byte;
    HealthAndStatusDummy->SeekerStatus       = 0; // Not compatible with new MPSAL

    weaponBitDummy.Bits.GCA_Fail             = Weapon.CBIT.Bits.GCA_Fail;
    weaponBitDummy.Bits.IMU_Fail             = Weapon.CBIT.Bits.IMU_Fail;
    weaponBitDummy.Bits.GNSS_Fail            = Weapon.CBIT.Bits.GNSS_Fail;
    weaponBitDummy.Bits.FinLockFail          = Weapon.CBIT.Bits.FinLockFail;
    weaponBitDummy.Bits.ServoFail            = Weapon.CBIT.Bits.ServoFail;
    weaponBitDummy.Bits.WirelessLinkFail     = Weapon.CBIT.Bits.WirelessLinkFail;
    weaponBitDummy.Bits.SeekerFail           = Weapon.CBIT.Bits.SeekerFail;
    weaponBitDummy.Bits.BatteryFail          = Weapon.CBIT.Bits.BatteryFail;
    weaponBitDummy.Bits.TailFinFail          = Weapon.CBIT.Bits.TailFinFail;
    weaponBitDummy.Bits.NavFail              = Weapon.CBIT.Bits.NavFail;
    weaponBitDummy.Bits.SW_ExecOverrun       = MCP.CBIT.Bits.ExecutionOverrunFail;
    weaponBitDummy.Bits.Reserved1            = 0;
    weaponBitDummy.Bits.IMU_CommsFail        = Weapon.CBIT.Bits.IMU_CommsFail;
    weaponBitDummy.Bits.GNSS_CommsFail       = Weapon.CBIT.Bits.GNSS_CommsFail;
    weaponBitDummy.Bits.BatteryCommsFail     = Weapon.CBIT.Bits.BatteryCommsFail;
    weaponBitDummy.Bits.IF_CommsFail         = Weapon.CBIT.Bits.IF_CommsFail;
    weaponBitDummy.Bits.ServoCommsFail       = Weapon.CBIT.Bits.ServoCommsFail;
    weaponBitDummy.Bits.WirelessLinkCommsFail= Weapon.CBIT.Bits.WirelessLinkCommsFail;
    weaponBitDummy.Bits.SeekerCommsFail      = Weapon.CBIT.Bits.SeekerCommsFail;
    weaponBitDummy.Bits.ArcnetReconFail      = Weapon.CBIT.Bits.ArcnetReconFail;
    weaponBitDummy.Bits.Reserved2            = 0;
    weaponBitDummy.Bits.Reserved3            = 0;
    weaponBitDummy.Bits.Reserved4            = 0;
    weaponBitDummy.Bits.Reserved5            = 0;
    weaponBitDummy.Bits.Reserved6            = 0;
    weaponBitDummy.Bits.Reserved7            = 0;
    weaponBitDummy.Bits.Reserved8            = 0;
    weaponBitDummy.Bits.Reserved9            = 0;
    weaponBitDummy.Bits.Reserved10           = 0;
    weaponBitDummy.Bits.Reserved11           = 0;
    weaponBitDummy.Bits.Reserved12           = 0;
    weaponBitDummy.Bits.CriticalFail         = Weapon.CBIT.Bits.CriticalFail;

    HealthAndStatusDummy->WeaponCBIT         = weaponBitDummy.DWord;
    HealthAndStatusDummy->SpareBattCapacity  = Weapon.BatteryCapacity; // Temporary (ICD check)
    HealthAndStatusDummy->Spare2             = 0;
    HealthAndStatusDummy->MissionParChecksum = Weapon.RawMission.CRC;
    HealthAndStatusDummy->WindParChecksum    = Weapon.Wind.CRC;
    HealthAndStatusDummy->IIRImageChecksum   = 0;
    HealthAndStatusDummy->IIRNumTemplates    = 0;
    HealthAndStatusDummy->SWSuiteMajorVerNum = 0;
    HealthAndStatusDummy->Spare3             = 0;
    HealthAndStatusDummy->EstAttackDiveAngle = RAD2DEG(Weapon.Mission.DiveAngle);
    HealthAndStatusDummy->EstAttackHeading   = Weapon.Mission.AttackHeading * 10430.3783505;
    HealthAndStatusDummy->EstImpactSpeed     = Weapon.Mission.ImpactSpeed;
    HealthAndStatusDummy->TargetLatitude     = Weapon.Mission.PriTgtLatitude * 683565275.5764316;
    HealthAndStatusDummy->TargetLongitude    = Weapon.Mission.PriTgtLongitude * 683565275.5764316;
    HealthAndStatusDummy->TargetAltitude     = Weapon.Mission.PriTgtAltitude;
    HealthAndStatusDummy->SAL_LaserPeriod    = 1000000.0 / Weapon.Mission.SAL_Frequency;

    TelemetryDummy->SALFrequency             = Weapon.Mission.SAL_Frequency;
    TelemetryDummy->GPSTimeWeek              = UBLOX.GNSS_Result.T_meas.GPS;
    TelemetryDummy->WeaponTime               = (float) Weapon.SystemTime / 1000000.0;
    TelemetryDummy->ServoMotorInputVolt      = Servo.ScaledMotorVoltage * 0.8;
    TelemetryDummy->ServoSupplyVoltage       = Servo.ScaledSupplyVoltage * 0.8;
    TelemetryDummy->WiFiSupplyVoltage        = 0;
    TelemetryDummy->RSSIByWeapon             = -WiFi.RSSI;
    TelemetryDummy->RSSIByTablet             = 0;
    TelemetryDummy->RemainingBattCap         = BatteryPack.RawCapacity;
    TelemetryDummy->AverageBattCurrent       = BatteryPack.RawAveCurrent_0_10ms; // Not 100ms only 10ms
    TelemetryDummy->ServoCBIT                = Servo.CBIT.Byte;
    TelemetryDummy->FinLockCamPosition       = Servo.ScaledFinLockPosition * (65536 / 360);
    TelemetryDummy->TailFin1Position         = Servo.ScaledFeedbacks[0];
    TelemetryDummy->TailFin2Position         = Servo.ScaledFeedbacks[1];
    TelemetryDummy->TailFin3Position         = Servo.ScaledFeedbacks[2];
    TelemetryDummy->TailFin4Position         = Servo.ScaledFeedbacks[3];
    TelemetryDummy->Servo1MotorCurrent       = Servo.Currents[0] * 10;
    TelemetryDummy->Servo2MotorCurrent       = Servo.Currents[1] * 10;
    TelemetryDummy->Servo3MotorCurrent       = Servo.Currents[2] * 10;
    TelemetryDummy->Servo4MotorCurrent       = Servo.Currents[3] * 10;
    TelemetryDummy->GCAPressure1             = (unsigned short) PressureSensor1.Pressure;
    TelemetryDummy->GCAPressure2             = (unsigned short) PressureSensor2.Pressure;
    TelemetryDummy->WeaponLatitude           = (int) RAD2DEG(Weapon.Latitude);
    TelemetryDummy->WeaponLongitude          = (int) RAD2DEG(Weapon.Longitude);
    TelemetryDummy->WeaponAltitude           = (int) Weapon.Altitude;
    TelemetryDummy->WeaponHeading            = RAD2DEG(NavTel100Hz.T100B1.NAV_AngPsi) * 100;
    TelemetryDummy->WeaponPitch              = RAD2DEG(NavTel100Hz.T100B1.NAV_AngTheta) * 100;
    TelemetryDummy->WeaponRoll               = RAD2DEG(NavTel100Hz.T100B1.NAV_AngPhi) * 100;

    discretesDummy.Bits.WeaponActivation                                = Weapon.Discretes.Bits.WeaponActivation;
    discretesDummy.Bits.SwitchOnBattery                                 = Weapon.Flags.Bits.SwitchOnBattery;
    discretesDummy.Bits.BatterySwitchedOn                               = Weapon.Discretes.Bits.BatterySwitchedOn;
    discretesDummy.Bits.LaunchLinkPresent    							= Weapon.Discretes.Bits.LaunchLinkPresent;
    discretesDummy.Bits.LanyardPresent       							= Weapon.Discretes.Bits.LanyardPresent;
    discretesDummy.Bits.LanyardPulled                                   = Weapon.Discretes.Bits.LanyardPulled;
    discretesDummy.Bits.ServosEnabled                                   = Weapon.Discretes.Bits.ServosEnabled;
    discretesDummy.Bits.TailFinUnlocked                                 = Weapon.Discretes.Bits.TailFinUnlocked;
    discretesDummy.Bits.GNSS_AntennaSwitch                              = Weapon.Discretes.Bits.GNSS_AntennaSwitch;
    discretesDummy.Bits.WiFiAntennaSelected                             = (short) WiFi.Settings.Bits.WiFiAntenna;
    discretesDummy.Bits.ActivateSAD                                     = Weapon.Flags.Bits.ActivateSAD;
    discretesDummy.Bits.GCA_Mode                                        = 0;
    discretesDummy.Bits.BatteryError                                    = Weapon.Flags.Bits.BatteryError;
    discretesDummy.Bits.WaitForLaunchSTC                                = Weapon.DiscretesDummy.Bits.WaitForLaunchSTC;
    discretesDummy.Bits.Spare                                           = 0;

    TelemetryDummy->DiscreteStates           = discretesDummy.Word;

    weaponStatusDummy.Bits.WeaponLaunched                               = (short) Weapon.Status.Bits.State;
    weaponStatusDummy.Bits.NAV_Mode                                     = (short) (Weapon.Status.Bits.NAV_Mode - 30); //Short only allows 0-15
    weaponStatusDummy.Bits.STC1_State                                   = (short) Weapon.Status.Bits.STC1_State;
    weaponStatusDummy.Bits.STC2_State                                   = (short) Weapon.Status.Bits.STC2_State;

    TelemetryDummy->WeaponStates             = weaponStatusDummy.Word;
    TelemetryDummy->UBloxSatellites          = (UBLOX.GPS_SatUsedCnt & 0x0F) | ((UBLOX.GLO_SatUsedCnt & 0x0F) << 4);
    TelemetryDummy->ArcnetReconCount         = MLAN.ReconCnt;
    TelemetryDummy->GCACBIT                  = MCP.CBIT.Word;
    TelemetryDummy->WiFiCBIT                 = WiFi.CBIT.Word;
    TelemetryDummy->ServoSBIT                = Servo.SBIT;

	if (AUXCTRL_SS.MAINControls.MPSAL_On)
	{
	    TelemetryDummy->SeekerCBIT               = (unsigned int) SAL_SS.PFBIT.PreFlightBITStatus;
		TelemetryDummy->SeekerSBIT               = (unsigned int) SAL_SS.SBIT.PBITStatus;
		TelemetryDummy->SALTargetYaw             = (short) RAD2DEG(SAL_SS.GuidanceData.TargetYaw);
	    TelemetryDummy->SALTargetPitch           = (short) RAD2DEG(SAL_SS.GuidanceData.TargetPitch);
	}
	else if (AUXCTRL_SS.MAINControls.QSAL_On)
	{
	    TelemetryDummy->SeekerCBIT               = QSAL_SS.SharedData.CBIT.DWord;
		TelemetryDummy->SeekerSBIT               = QSAL_SS.SharedData.SBIT.DWord;
	    TelemetryDummy->SALTargetYaw             = (short) RAD2DEG(QSAL_SS.SharedData.TargetYaw);
	    TelemetryDummy->SALTargetPitch           = (short) RAD2DEG(QSAL_SS.SharedData.TargetPitch);
	}
	else if (AUXCTRL_SS.MAINControls.DSAL_On)
	{
		TelemetryDummy->SeekerCBIT               = SAL_Seeker.CBIT.DWord;
	    TelemetryDummy->SeekerSBIT               = SAL_Seeker.SBIT;
	    TelemetryDummy->SALTargetYaw             = (short) RAD2DEG(SAL_Seeker.TargetYaw);
	    TelemetryDummy->SALTargetPitch           = (short) RAD2DEG(SAL_Seeker.TargetPitch);
	} else {
	    TelemetryDummy->SeekerCBIT               = 0;
		TelemetryDummy->SeekerSBIT               = 0;
	    TelemetryDummy->SALTargetYaw             = 0;
	    TelemetryDummy->SALTargetPitch           = 0;
	}

    //These values are calculated to align the WifiTelemetry data with the P3 Communication Utility tool.
    TelemetryDummy->IMUAngularRateX		  	 = (short) DEG2RAD(IMU.RawData.AngularRateX) / (0.00457763671875 * 100);
	TelemetryDummy->IMUAngularRateY 		 = (short) DEG2RAD(IMU.RawData.AngularRateY) / (0.0011444091796875 * 100);
	TelemetryDummy->IMUAngularRateZ          = (short) DEG2RAD(IMU.RawData.AngularRateY) / (0.0011444091796875 * 100);
    TelemetryDummy->IMUAccelerationX         = (short) (IMU.RawData.AccelerationX * 9.81) / ((0.011162109375 * 0.3048) * 1000);
	TelemetryDummy->IMUAccelerationY         = (short) (IMU.RawData.AccelerationY * 9.81) / ((0.011162109375 * 0.3048) * 1000);
	TelemetryDummy->IMUAccelerationZ         = (short) (IMU.RawData.AccelerationZ * 9.81) / ((0.02232421875 * 0.3048) * 1000);
	TelemetryDummy->AvgGLOSatellites         = UBLOX.GLO_SatUsedCnt;
	TelemetryDummy->AvgGPSSatellites         = UBLOX.GPS_SatUsedCnt;
	TelemetryDummy->WPN28VLevel              = 0;
	TelemetryDummy->MissionMode              = 0;
	TelemetryDummy->WindAltitudeGuid         = 0;
	TelemetryDummy->WindDirectionGuid        = 0;
	TelemetryDummy->WindSpeedGuid            = 0;
	TelemetryDummy->ServoSwCRC               = Servo.SwCRC;
	TelemetryDummy->MCPSwCRC                 = MCP.SW_CRC;
	TelemetryDummy->IFPCBSwCRC               = 0;
	TelemetryDummy->WirelessLinkCRC          = WiFi.SW_CRC;
	TelemetryDummy->SeekerCRC                = 0;

}

void WeaponInfoToCsv(void)
{
	AUXsprintf("%u,",GetTimeUs());
	AUXsprintf("WEAPON,");
	AUXsprintf("0x%x,",Weapon.SBIT.DWord);
	AUXsprintf("0x%x,",Weapon.CBIT.DWord);
	AUXsprintf("%d,",Weapon.Discretes.Bits.WeaponActivation);
	AUXsprintf("%d,",Weapon.Discretes.Bits.LaunchLinkPresent);
	AUXsprintf("%d,",Weapon.Discretes.Bits.LanyardPresent);
	AUXsprintf("%d,",Weapon.Discretes.Bits.LanyardPulled);
	AUXsprintf("%d,",Weapon.Discretes.Bits.BatterySwitchedOn);
	AUXsprintf("%d,",Weapon.Discretes.Bits.GNSS_AntennaSwitch);
	AUXsprintf("%d,",Weapon.Discretes.Bits.HILS_Mode);
	AUXsprintf("%d,",Weapon.Discretes.Bits.DownloadCablePresent);
	AUXsprintf("%d,",Weapon.Discretes.Bits.ServosEnabled);
	AUXsprintf("%d,,",Weapon.Discretes.Bits.TailFinUnlocked);
	AUXsprintf("%d,",Weapon.Flags.Bits.GoNoGo);
	AUXsprintf("%d,",Weapon.Flags.Bits.Ready);
	AUXsprintf("%d,",Weapon.Flags.Bits.MissionMode);
	AUXsprintf("%d,",Weapon.Flags.Bits.MissionOk);
	AUXsprintf("%d,",Weapon.Flags.Bits.WindOk);
	AUXsprintf("%d,",Weapon.Flags.Bits.GNSS_Lock);
	AUXsprintf("%d,",Weapon.Flags.Bits.ActivateSAD);
	AUXsprintf("%d,",Weapon.Flags.Bits.SwitchOnBattery);
	AUXsprintf("%d,",Weapon.Flags.Bits.BatteryError);
	AUXsprintf("%d,,",Weapon.Flags.Bits.WeaponBunt);
	AUXsprintf("%d,",Weapon.Status.Bits.State);
	AUXsprintf("%d,",Weapon.Status.Bits.STC1_State);
	AUXsprintf("%d,",Weapon.Status.Bits.STC2_State);
	AUXsprintf("%d,",Weapon.Status.Bits.NAV_Aligned);
	AUXsprintf("%d,",Weapon.Status.Bits.NAV_Mode);
	AUXsprintf("%d,,",Weapon.Status.Bits.WeaponLaunched);
	AUXsprintf("%s,",AUXsprintfFloat(Weapon.Pitch));
	AUXsprintf("%s,",AUXsprintfFloat(Weapon.Yaw));
	AUXsprintf("%s,",AUXsprintfFloat(Weapon.Roll));
	AUXsprintf("%s,",AUXsprintfFloat(Weapon.Latitude));
	AUXsprintf("%s,",AUXsprintfFloat(Weapon.Longitude));
	AUXsprintf("%s,,",AUXsprintfFloat(Weapon.Altitude));
	AUXsprintf("%d,",Weapon.BatteryCapacity);
	AUXsprintf("%d,,",Weapon.BatteryTimeRemaining);
	AUXsprintf("%d,",Weapon.Mission.MissionValid);
	AUXsprintf("%d,",Weapon.LAR.Valid);
	AUXsprintf("%d,",Weapon.LAR_InRange.Valid);
	AUXsprintf("%d,",Weapon.UseAircraftLAR);
	AUXsprintf("%d,",Weapon.InsideAircraftLAR);
	AUXsprintf("%d,",Weapon.ActiveServos);
	AUXsprintf("\n");
}


