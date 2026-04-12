/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <BIT.c>
 *
 *  Description:
 *  ------------
 *  P3 Weapon BIT information and control.
 *
 *  Function(s):
 *  ------------
 *  - BIT_Init:                    Initializes BIT Information
 *  - BIT_Update:                  Update MCP and weapon BIT
 *  - BIT_DoSBIT:                  Do MCP and Weapon SBIT
 *  - BIT_ClearCBIT:               Clear MCP and weapon CBIT
 *  - BIT_ClearSBIT:               Clear MCP and weapon SBIT
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
#include "Timer.h"
#include "Wind.h"
#include "Arcnet.h"
#include "MCP.h"
#include "ScheduleControl.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "TaskManager.h"
#include "MCP.h"
#include "WiFi.h"
#include "FIFO.h"
#include "UART.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "MLAN.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"
#include "Fuze.h"
#include "IF.h"
#include "Servos.h"
#include "Telemetry.h"
#include "UBLOX.h"
#include "IMU.h"
#include "BatteryPack.h"
#include "AircraftInterface.h"
#include "Stack.h"
#include "HOBS.h"
#include "HOBSComms.h"
#include "BIT.h"
#include "AuxControls.h"
#include "MPSAL.h"
#include "MPSAL_Comms.h"
#include "AuxiliaryPort.h"

#define BIT_FAIL            1
#define BIT_PASS            0


sBIT BIT;

static unsigned int BIT_PSU_5V_FailCnt;
static unsigned int BIT_PSU_1V_FailCnt;
static unsigned int BIT_PSU_1V35_FailCnt;
static unsigned int BIT_PSU_1V8_FailCnt;
static unsigned int BIT_PSU_3V3_FailCnt;
static unsigned int BIT_PSU_28V_FailCnt;
static unsigned int BIT_PSU_IMU_5V_FailCnt;
static unsigned short BIT_PrevMCP_CBIT;
static unsigned int BIT_PrevWeaponCBIT;
static unsigned int BIT_PrevMyReconCnt;
static unsigned int BIT_PrevReconCnt;
static unsigned int BIT_Seconds;
static unsigned int BIT_MyReconHist[60];
static unsigned int BIT_ReconHist[60];
static TIME BIT_StartTime;
static TIME SBIT_Timestamp;
static TIME BIT_MinuteTimer;

void BIT_DoSBIT(void);

void BIT_Init(void)
{
  memset(&BIT, 0, sizeof(sBIT));
  memset(&BIT_MyReconHist, 0, sizeof(BIT_MyReconHist));
  memset(&BIT_ReconHist, 0, sizeof(BIT_ReconHist));

  BIT_PSU_5V_FailCnt = 0;
  BIT_PSU_1V_FailCnt = 0;
  BIT_PSU_1V35_FailCnt = 0;
  BIT_PSU_1V8_FailCnt = 0;
  BIT_PSU_3V3_FailCnt = 0;
  BIT_PSU_28V_FailCnt = 0;
  BIT_PSU_IMU_5V_FailCnt = 0;
  BIT_PrevMCP_CBIT = 0;
  BIT_PrevWeaponCBIT = 0;
  BIT_PrevMyReconCnt = 0;
  BIT_PrevReconCnt = 0;
  BIT_Seconds = 0;

  BIT_StartTime = 0;
  SBIT_Timestamp = 0;
  BIT_MinuteTimer = 0;
}

void BIT_Update(void)
{
  int index;
  int total_my_recon_cnt;
  int total_recon_cnt;
  unsigned int tmp;


  BIT_StartTime = GetSystemTime();

  if (!BIT.SBIT_Complete) {
    BIT_DoSBIT();

    if ((BIT_StartTime - SBIT_Timestamp) > 5999999) {
      BIT.SBIT_Complete = TRUE;

      // Indicate that SBIT is completed
      MCP.SBIT.Bits.CriticalFail = BIT_PASS;
      Weapon.SBIT.Bits.CriticalFail = BIT_PASS;

      BIT_ClearCBIT();
    }

    BIT_PSU_5V_FailCnt = 0;
    BIT_PSU_1V_FailCnt = 0;
    BIT_PSU_1V35_FailCnt = 0;
    BIT_PSU_1V8_FailCnt = 0;
    BIT_PSU_3V3_FailCnt = 0;
    BIT_PSU_28V_FailCnt = 0;
    BIT_PSU_IMU_5V_FailCnt = 0;
    BIT_PrevMCP_CBIT = 0;
    BIT_PrevWeaponCBIT = 0;

    TelemetryBuild2HzBlock1();
    TelemetryBuild2HzBlock2();
    TelemetryBuild2HzBlock3();
    TelemetryBuild2HzBlock4();
  }
  else {
    // Do CBIT
    // MCP CBIT
    if (MCP.I2C_Updated) {
      if (!WITHIN_LIMITS(MCP.PSU_5V, 5.50f, 6.50f)) {
        BIT_PSU_5V_FailCnt++;
      }
      else {
        BIT_PSU_5V_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_1V, 0.90f, 1.10f)) {
        BIT_PSU_1V_FailCnt++;
      }
      else {
        BIT_PSU_1V_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_1V35, 1.25f, 1.45f)) {
        BIT_PSU_1V35_FailCnt++;
      }
      else {
        BIT_PSU_1V35_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_1V8, 1.65f, 1.95f)) {
        BIT_PSU_1V8_FailCnt++;
      }
      else {
        BIT_PSU_1V8_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_3V3, 3.10f, 3.50f)) {
        BIT_PSU_3V3_FailCnt++;
      }
      else {
        BIT_PSU_3V3_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_28V, 20.00f, 36.00f)) {
        BIT_PSU_28V_FailCnt++;
      }
      else {
        BIT_PSU_28V_FailCnt = 0;
      }

      if (!WITHIN_LIMITS(MCP.PSU_IMU_5V, 4.70f, 5.30f)) {
        BIT_PSU_IMU_5V_FailCnt++;
      }
      else {
        BIT_PSU_IMU_5V_FailCnt = 0;
      }

      MCP.CBIT.Bits.PSU_5V_Fail = (BIT_PSU_5V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_1V_Fail = (BIT_PSU_1V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_1V35_Fail = (BIT_PSU_1V35_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_1V8_Fail = (BIT_PSU_1V8_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_3V3_Fail = (BIT_PSU_3V3_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_28V_Fail = (BIT_PSU_28V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
      MCP.CBIT.Bits.PSU_IMU_5V_Fail = (BIT_PSU_IMU_5V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
    }

    MCP.CBIT.Bits.TemperatureFail = !WITHIN_LIMITS(MCP.Temperature, -45.0f, 70.0f);
    MCP.CBIT.Bits.StackOverrun = Stack.Overrun;

    if ((TaskMananger.Tasks[task600HZ].OverrunCnt > 2) || (TaskMananger.Tasks[task100HZ].OverrunCnt > 2) ||
        (TaskMananger.Tasks[task2HZ].OverrunCnt > 2)                                                       ) {
      // Latch error
      MCP.CBIT.Bits.ExecutionOverrunFail = BIT_FAIL;
    }

    // Once a second
    if ((Weapon.Weapon600HzCnt % 600) == 0) {
      BIT_MyReconHist[BIT_Seconds] = MLAN.MyReconCnt - BIT_PrevMyReconCnt;
      BIT_ReconHist[BIT_Seconds] = MLAN.ReconCnt - BIT_PrevReconCnt;

      BIT_PrevMyReconCnt = MLAN.MyReconCnt;
      BIT_PrevReconCnt = MLAN.ReconCnt;

      total_my_recon_cnt = 0;
      total_recon_cnt = 0;

      // Add the total number of recons that occurred during the last minute
      for (index = 0; index < 60; index++) {
        total_my_recon_cnt += BIT_MyReconHist[index];
        total_recon_cnt += BIT_ReconHist[index];
      }

      // Indicate fail if more than 10 recons per minute
      MCP.CBIT.Bits.ArcnetMyReconFail = (total_my_recon_cnt > 10) ? BIT_FAIL : BIT_PASS;
      Weapon.CBIT.Bits.ArcnetReconFail = (total_recon_cnt > 10) ? BIT_FAIL : BIT_PASS;

      BIT_Seconds++;

      if (BIT_Seconds >= 60) {
        BIT_Seconds = 0;
      }
    }

    // Determine if there is a critical fail
    MCP.CBIT.Bits.CriticalFail = ((MCP.CBIT.Word & 0x0E7F) > 0) ? BIT_FAIL : BIT_PASS;

    // Weapon BIT
    Weapon.CBIT.Bits.GCA_Fail = MCP.CBIT.Bits.CriticalFail;
    Weapon.CBIT.Bits.IMU_Fail = Weapon.SBIT.Bits.IMU_Fail;
    Weapon.CBIT.Bits.GNSS_Fail = Weapon.SBIT.Bits.GNSS_Fail;
    Weapon.CBIT.Bits.HOBS_Fail = (WeaponSettings.WeaponConfig.HOBS_Present && (SS_HOBS.Status.CBIT.DWord > 0)) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.ServoFail = (Servo.CBIT.Bits.CriticalFail) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.WirelessLinkFail = (WiFi.CBIT.Bits.CriticalFail) ?  BIT_FAIL : BIT_PASS;
    if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
    	if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	{
    		Weapon.CBIT.Bits.SeekerFail = SAL_SS.Detection.CBITStatus?(char)1:(char)0;
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On) {

    		Weapon.CBIT.Bits.SeekerFail = QSAL_SS.SharedData.CBIT.DWord?(char)1:(char)0;
    	}
    	else {
    		Weapon.CBIT.Bits.SeekerFail = BIT_PASS;
    	}
    }

    Weapon.CBIT.Bits.BatteryFail = ((Weapon.BatteryCapacity < 1) || Weapon.Flags.Bits.BatteryError ||
    		(Weapon.Discretes.Bits.BatterySwitchedOn && (MCP.PSU_28V < MIN_BATT_VOLT_LEVEL))) ? BIT_FAIL : BIT_PASS;

    if (WeaponSettings.WeaponConfig.DTAC_Present){
    	if (AUXCTRL_SS.MAINControls.WIAC82_On){
    		Weapon.CBIT.Bits.DTAC_Fail = BIT_PASS; // No CBIT Status in WIAC82
    	} else {
    		Weapon.CBIT.Bits.DTAC_Fail = ((Aircraft.Status.CBIT & 0x8000) == 0x8000) ? BIT_FAIL : BIT_PASS;
    	}
    } else {
    	Weapon.CBIT.Bits.DTAC_Fail = BIT_PASS;
    }
    Weapon.CBIT.Bits.FinLockFail = (!Weapon.Status.Bits.WeaponLaunched && !Servo.FinLocked) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.IMU_CommsFail = ((BIT_StartTime - IMU.RxTimestamp) > 5000);
    Weapon.CBIT.Bits.GNSS_CommsFail = ((BIT_StartTime - UBLOX.RxTimestamp) > 5000000);
    Weapon.CBIT.Bits.HOBS_CommsFail = (WeaponSettings.WeaponConfig.HOBS_Present && ((BIT_StartTime - SS_HOBS.RxTimestamp) > 2000000) && (BIT_StartTime > 10000000)) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.ServoCommsFail = ((BIT_StartTime - Servo.RxTimestamp) > 100000) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.WirelessLinkCommsFail = ((BIT_StartTime - WiFi.RxTimestamp) > 1000000);

    if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
    	if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	{
    		Weapon.CBIT.Bits.SeekerCommsFail = (((BIT_StartTime - SAL_SS.TimeStamps.RxTimestamp)  > 1000000) && (BIT_StartTime > 10000000)) ? BIT_FAIL : BIT_PASS;
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On) {

    		Weapon.CBIT.Bits.SeekerCommsFail = (((BIT_StartTime - QSAL_SS.SharedData.RxTimestamp) > 1000000) && (BIT_StartTime > 10000000)) ? BIT_FAIL : BIT_PASS;
    	}
    	else {
    		Weapon.CBIT.Bits.SeekerCommsFail = BIT_PASS;
    	}
    }

    if (WeaponSettings.WeaponConfig.DTAC_Present){
    	if (AUXCTRL_SS.MAINControls.WIAC82_On){
    		Weapon.CBIT.Bits.DTAC_CommsFail = ((BIT_StartTime - WIAC82_SS.RxTimestamp) > 1000000);
    	} else {
    		Weapon.CBIT.Bits.DTAC_CommsFail = ((BIT_StartTime - Aircraft.RxTimestamp) > 1000000);
    	}
    } else {
    	Weapon.CBIT.Bits.DTAC_CommsFail = BIT_PASS;
    }
    Weapon.CBIT.Bits.BatteryCommsFail = ((BIT_StartTime - BatteryPack.RxTimestamp) > 1000000);
    Weapon.CBIT.Bits.TailFinFail = (Servo.TailFinsError == TRUE) ? BIT_FAIL : BIT_PASS;
    Weapon.CBIT.Bits.IF_CommsFail = (WeaponSettings.WeaponConfig.DTAC_Present && ((BIT_StartTime - IF.RxTimstamp) > 2000000));
    Weapon.CBIT.Bits.NavFail = BIT_PASS;
    Weapon.CBIT.Bits.ConfigFail = (WeaponSettings.WeaponConfigValid == FALSE) ? BIT_FAIL : BIT_PASS;

    // CBIT's that are Al-Tariq specific as well as CBIT's that are not critical should be ignored
    //Weapon.CBIT.Bits.CriticalFail = ((Weapon.CBIT.DWord & 0x7FF23E47) == 0) ? BIT_PASS : BIT_FAIL;
    if (AUXCTRL_SS.MAINControls.WIAC82_On) {
    	Weapon.CBIT.Bits.CriticalFail = ((Weapon.CBIT.DWord & 0x40D21A43) == 0) ? BIT_PASS : BIT_FAIL; // Critical Failure Rafale
    	if (!Weapon.CBIT.Bits.CriticalFail){
    		Weapon.CBIT.Bits.DegradedFail = ((Weapon.CBIT.DWord & 0x00082104) == 0) ? BIT_PASS : BIT_FAIL; // Degraded Failure Rafale
    	}
    } else {
    	Weapon.CBIT.Bits.CriticalFail = ((Weapon.CBIT.DWord & 0x70F2BE57) == 0) ? BIT_PASS : BIT_FAIL;
    }

    // Log all MCP and weapon CBIT errors after SBIT is completed
    if (BIT.SBIT_Complete) {
      if (BIT_PrevMCP_CBIT != MCP.CBIT.Word) {
        // Check which errors occurred. Ignore the critical error bit (bit 15)
        tmp = (BIT_PrevMCP_CBIT ^ MCP.CBIT.Word) & 0x7FFF;

        index = 0;

        // Add each CBIT error to the event log.
        do {
          if ((tmp & 0x01) == 0x01) {
            if (BIT.MCP_CBIT_EventCnt < BIT_CBIT_EVENT_LOG_MAX_ENTRIES) {
              BIT.MCP_CBIT_Events[BIT.MCP_CBIT_EventCnt].Timestamp = BIT_StartTime;
              BIT.MCP_CBIT_Events[BIT.MCP_CBIT_EventCnt].BIT_Index = index;
              BIT.MCP_CBIT_Events[BIT.MCP_CBIT_EventCnt].IMU_Cycle = IMU.Cycle;
            }

            BIT.MCP_CBIT_EventCnt++;
          }

          index++;
          tmp >>= 1;
        } while (tmp > 0);

        BIT_PrevMCP_CBIT = MCP.CBIT.Word;

        TelemetryBuild2HzBlock7();
      }

      if (BIT_PrevWeaponCBIT != Weapon.CBIT.DWord) {
        // Check which errors occurred. Ignore the critical error bit (bit 31)
        tmp = (BIT_PrevWeaponCBIT ^ Weapon.CBIT.DWord) & 0x7FFFFFFF;

        index = 0;

        // Add each CBIT error to the event log.
        do {
          if ((tmp & 0x01) == 0x01) {
            if (BIT.WeaponCBIT_EventCnt < BIT_CBIT_EVENT_LOG_MAX_ENTRIES) {
              BIT.WeaponCBIT_Events[BIT.WeaponCBIT_EventCnt].Timestamp = BIT_StartTime;
              BIT.WeaponCBIT_Events[BIT.WeaponCBIT_EventCnt].BIT_Index = index;
              BIT.WeaponCBIT_Events[BIT.WeaponCBIT_EventCnt].IMU_Cycle = IMU.Cycle;
            }

            BIT.WeaponCBIT_EventCnt++;
          }

          index++;
          tmp >>= 1;
        } while (tmp > 0);

        BIT_PrevWeaponCBIT = Weapon.CBIT.DWord;

        TelemetryBuild2HzBlock7();
      }
    }
  }
}

void BIT_DoSBIT(void)
{
  // Zero all comms error counters during SBIT
  IMU.ErrorCnt = 0;
  WiFi.ErrCnt = 0;
  SAL_SS.RxCounters.RxTimeoutErrCnt = 0;
  QSAL_SS.RxCounters.RxTimeoutErrCnt = 0;

  // No CBIT Errors
  MCP.CBIT.Word = 0;
  Weapon.CBIT.DWord = 0;

  // Ensure that the ARCNET recon BIT will not fail when CBIT start after SBIT is completed
  BIT_PrevMyReconCnt = MLAN.MyReconCnt;
  BIT_PrevReconCnt = MLAN.ReconCnt;

  // MCP BIT
  if (MCP.I2C_Updated) {
    if (!WITHIN_LIMITS(MCP.PSU_5V, 4.50f, 5.50f)) {
      BIT_PSU_5V_FailCnt++;
    }
    else {
      BIT_PSU_5V_FailCnt = 0;
    }

    if (!WITHIN_LIMITS(MCP.PSU_1V, 0.90f, 1.10f)) {
      BIT_PSU_1V_FailCnt++;
    }
    else {
      BIT_PSU_1V_FailCnt = 0;
    }

    if (!WITHIN_LIMITS(MCP.PSU_1V35, 1.25f, 1.45f)) {
      BIT_PSU_1V35_FailCnt++;
    }
    else {
      BIT_PSU_1V35_FailCnt = 0;
    }

    if (!WITHIN_LIMITS(MCP.PSU_1V8, 1.65f, 1.95f)) {
      BIT_PSU_1V8_FailCnt++;
    }
    else {
      BIT_PSU_1V8_FailCnt = 0;
    }

    if (!WITHIN_LIMITS(MCP.PSU_3V3, 3.10f, 3.50f)) {
      BIT_PSU_3V3_FailCnt++;
    }
    else {
      BIT_PSU_3V3_FailCnt = 0;
    }

    MCP.SBIT.Bits.PSU_5V_Fail = (BIT_PSU_5V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
    MCP.SBIT.Bits.PSU_1V_Fail = (BIT_PSU_1V_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
    MCP.SBIT.Bits.PSU_1V35_Fail = (BIT_PSU_1V35_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
    MCP.SBIT.Bits.PSU_1V8_Fail = (BIT_PSU_1V8_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
    MCP.SBIT.Bits.PSU_3V3_Fail = (BIT_PSU_3V3_FailCnt > 60) ? BIT_FAIL : BIT_PASS;
  }

  if ((TaskMananger.Tasks[task600HZ].OverrunCnt > 1) || (TaskMananger.Tasks[task100HZ].OverrunCnt > 1) ||
      (TaskMananger.Tasks[task2HZ].OverrunCnt > 1)                                                       ) {
    MCP.SBIT.Bits.ExecutionOverrunFail = BIT_FAIL;
  }

  MCP.SBIT.Bits.ArcnetMyReconFail = BIT_PASS;  // Do not check for ARCNET recons during SBIT
  MCP.SBIT.Bits.CriticalFail = BIT_FAIL;       // Use CriticalFail to indicate SBIT busy

  Weapon.SBIT.Bits.GCA_Fail = ((MCP.SBIT.Word & 0x7FFF) > 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.IMU_Fail = (IMU.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.GNSS_Fail = (UBLOX.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.HOBS_Fail = (WeaponSettings.WeaponConfig.HOBS_Present && (SS_HOBS.Status.SBIT.DWord > 0)) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.ServoFail = (Servo.SBIT > 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.WirelessLinkFail = (WiFi.SBIT.Word > 0) ? BIT_FAIL : BIT_PASS;
  if((WeaponSettings.WeaponConfig.SeekerType == seekerSAL) )
  {
	  if ((AUXCTRL_SS.MAINControls.MPSAL_On)&& (SAL_SS.SBIT.PBITStatus > 0))
	  {
		  Weapon.SBIT.Bits.SeekerFail =BIT_FAIL;
	  }
	  else if((AUXCTRL_SS.MAINControls.QSAL_On) && (QSAL_SS.SharedData.SBIT.DWord > 0))
	  {
		  Weapon.SBIT.Bits.SeekerFail =BIT_FAIL;
	  }
	  else
	  {
		  Weapon.SBIT.Bits.SeekerFail = BIT_PASS;
	  }
  }

  Weapon.SBIT.Bits.BatteryFail = (BatteryPack.SBIT.Word > 0) ? BIT_FAIL : BIT_PASS;
  if (WeaponSettings.WeaponConfig.DTAC_Present){
  	if (AUXCTRL_SS.MAINControls.WIAC82_On){
  		Weapon.SBIT.Bits.DTAC_Fail = (WIAC82_SS.SBIT_Message.SBIT.Byte > 0) ? BIT_FAIL : BIT_PASS;
  	}else{
  		Weapon.SBIT.Bits.DTAC_Fail = BIT_PASS;
  	}
  } else {
	  Weapon.SBIT.Bits.DTAC_Fail = BIT_PASS;
  }
  Weapon.SBIT.Bits.FinLockFail = (!Servo.FinLocked) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.IMU_CommsFail = (IMU.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.GNSS_CommsFail = (UBLOX.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.HOBS_CommsFail = (WeaponSettings.WeaponConfig.HOBS_Present && (SS_HOBS.Counters.RxMessageCnt == 0)) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.ServoCommsFail = (Servo.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.WirelessLinkCommsFail = (WiFi.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  if((WeaponSettings.WeaponConfig.SeekerType == seekerSAL) )
  {
	  if ((AUXCTRL_SS.MAINControls.MPSAL_On) && (SAL_SS.RxCounters.RxMessageCnt == 0))
	  {
		  Weapon.SBIT.Bits.SeekerCommsFail = BIT_FAIL;
	  }
	  else if((AUXCTRL_SS.MAINControls.QSAL_On) && ((QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt+QSAL_SS.RxCounters.RxSBITReportMsgCnt)==0))
	  {
		  Weapon.SBIT.Bits.SeekerCommsFail = BIT_FAIL;
	  }
	  else
	  {
		  Weapon.SBIT.Bits.SeekerCommsFail = BIT_PASS;
	  }
  }

  if (WeaponSettings.WeaponConfig.DTAC_Present){
  	if (AUXCTRL_SS.MAINControls.WIAC82_On){
  		Weapon.SBIT.Bits.DTAC_CommsFail = (WIAC82_SS.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  	}else{
  		Weapon.SBIT.Bits.DTAC_CommsFail = (Aircraft.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  	}
  } else {
	  Weapon.SBIT.Bits.DTAC_CommsFail = BIT_PASS;
  }
  Weapon.SBIT.Bits.BatteryCommsFail = (BatteryPack.RxCnt == 0) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.TailFinFail = (Servo.TailFinsError == TRUE) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.IF_CommsFail = (WeaponSettings.WeaponConfig.DTAC_Present && (IF.RxCnt == 0)) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.NavFail = BIT_PASS;
  Weapon.SBIT.Bits.ConfigFail = (WeaponSettings.WeaponConfigValid == FALSE) ? BIT_FAIL : BIT_PASS;
  Weapon.SBIT.Bits.CriticalFail = BIT_FAIL;   // Use CriticalFail to indicate SBIT busy
}

void BIT_ClearCBIT(void)
{
  TaskMananger.Tasks[task600HZ].OverrunCnt = 0;
  TaskMananger.Tasks[task100HZ].OverrunCnt = 0;
  TaskMananger.Tasks[task2HZ].OverrunCnt = 0;

  MCP.CBIT.Word = 0;
  Weapon.CBIT.DWord = 0;

  WiFiSendClearCBIT();

  if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
	// This command is not valid for MP-SAL
//    SAL_SeekerSendClearCBIT();
  }
}

void BIT_ClearSBIT(void)
{
  MCP.SBIT.Word = 0;
  Weapon.SBIT.DWord = 0;
}
