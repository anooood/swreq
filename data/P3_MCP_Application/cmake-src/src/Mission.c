/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Mission.c>
 *
 *  Description:
 *  ------------
 *  Mission Module interprets mission plans, interacts with navigation and guidance systems,
 *  and ensures that operational objectives are executed according to defined parameters.
 *
 *  Function(s):
 *  ------------
 *  - InitMission                      : Initializes mission data structures and state.
 *  - ClearMission                     : Resets mission parameters to default or safe values.
 *  - MissionSet                       : Sets mission parameters from internal data sources.
 *  - MissionSetFromAircraft           : Loads mission parameters received from the aircraft interface.
 *  - SetGNSS_Usage                    : Configures whether GNSS is used in mission execution.
 *  - DecodeMissionSource              : Decodes the source of the mission data.
 *  - DecodeMissionGNSS_Usage          : Extracts GNSS usage information from the mission data.
 *  - DecodeMissionFuzeMode            : Parses and applies the fuze mode from the mission data.
 *  - DecodeMissionWeaponType          : Interprets the weapon type assigned to the mission.
 *  - DecodeMissionType                : Determines the mission type.
 *  - SetMissionType                   : Sets the mission type flag internally.
 *  - SetValidityFlags                 : Updates internal flags representing mission data validity.
 *  - SetMissionFromWIAC82             : Applies mission settings based on WIAC82 protocol data.
 *  - VerifyWIAC82WeaponType           : Validates the weapon type field from WIAC82 data.
 *  - VerifyWIAC82MissionType          : Validates the mission type field from WIAC82 data.
 *  - VerifyWIAC82TargetPosition       : Checks the correctness of the target coordinates.
 *  - DiveAngleWIAC82P32Values         : Dive angle parameters from P32 values.
 *  - VerifyWIAC82DiveAngle            : Verifies dive angle constraints for safety and validity.
 *  - EncodeDiveAngle                  : Encodes dive angle into mission-specific data format.
 *  - VerifyWIAC82MovingTarget         : Validates moving target parameters for WIAC82 missions.
 *  - VerifyWIAC82_SALCode             : Verifies the SAL (semi-active laser) code in the mission.
 *  - DetermineWIAC82_FuzeMode         : Derives the fuze mode based on WIAC82 mission data.
 *  - FuzeSettingDelayPossibilities    : Determines valid fuze delay configurations.
 *  - FuzeSettingHeightPossibilities   : Determines valid fuze height configurations.
 *  - DetermimeWIAC82_FuzeSetting      : Determines fuze settings.
 *  - DetermineWIAC82_HOBSSetting      : Determines HOBS settings.
 *  - CopyWIAC82DatatoTelemetry        : Copies WIAC82 mission data into telemetry format.
 *  - CheckValidityOfMission           : Performs a full mission validity check across all parameters.
 *  - UpdateGNSS_Usage                 : Updates GNSS usage flag based on latest mission context.
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
#include "ScaleFactors.h"
#include "ScheduleControl.h"
#include "M1553Convert.h"
#include "Timer.h"
#include "FIFO.h"
#include "UART.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "Mission.h"
#include "CRC.h"
#include "SAL_Seeker.h"
#include "QSAL.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "Telemetry.h"
#include "Guidance.h"
#include "EGM96.h"
#include "HOBS.h"
#include "MLAN.h"


#define MISSION_ANGLE_SF        (PI / 2147483648.0)
#define MISSION_FREQ_SF         0.000001
#define DELAYROWSIZE            13
#define HEIGHTROWSIZE           19

sMISSION_StateSpace MISS_SS;

void InitMission(char *AuxStr){

	if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.MissionControls.AuxOn 				= 1;
		AUXCTRL_SS.MissionControls.MissionAuxOn			= 0;
		AUXCTRL_SS.MissionControls.MissionDetail1AuxOn	= 0;
		AUXCTRL_SS.MissionControls.MissionDetail2AuxOn	= 0;
	}

	if (AUXCTRL_SS.MissionControls.AuxOn) {
		sprintf(AuxStr,"%s%d:MISSION Parameters Initialized\n",AuxStr,GetTimeUs());
	}

}

void ClearMission(eMissionSource Source){
	  sRawMission mission;


	  memset(&mission, 0, sizeof(sRawMission));
	  MissionSet(&mission, Source);

}

unsigned char MissionSet(sRawMission *RawMissionData, eMissionSource MissionSource)
{
  unsigned short crc;
  sMission new_mission;


  crc = CalculateMIL_STD_1760D_Crc((unsigned short *) RawMissionData, sizeof(sRawMission) / 2 - 1);

  if (crc == RawMissionData->CRC) {
    new_mission.MissionValid = TRUE;
    new_mission.MissionSetResult = 0;
    new_mission.MissionSource = MissionSource;
    new_mission.PriTgtLatitude = (double) RawMissionData->PriTgtLatitude * MISSION_ANGLE_SF;
    new_mission.PriTgtLongitude = (double) RawMissionData->PriTgtLongitude * MISSION_ANGLE_SF;
    new_mission.PriTgtAltitude  = RawMissionData->PriTgtAltitude;
    new_mission.AttackHeading = (double) RawMissionData->AttackHeading * MISSION_ANGLE_SF;
    new_mission.DiveAngle = DEG2RAD(RawMissionData->DiveAngle);
    new_mission.ImpactSpeed = RawMissionData->ImpactSpeed;
    new_mission.GNSS_Usage = RawMissionData->GNSS_Usage;
    new_mission.AttackHeadingValid = RawMissionData->AttackHeadingValid;
    new_mission.SAL_Frequency = 10.0;
    new_mission.SAL_Activation = RawMissionData->SAL_Activation;
    new_mission.SAL_Accuracy = RawMissionData->SAL_Accuracy;
    new_mission.SAL_Jitter = RawMissionData->SAL_Jitter;
    new_mission.IIR_ImageCRC = RawMissionData->IIR_ImageCRC;
    new_mission.IIR_ImageNo = 0;   // Will be set after the mission type is verified
    new_mission.FuzeMode = fmNON_PROGRAMMABLE;
    new_mission.FuzeDelay = 0;
    new_mission.FuzingDistance = 0;
    new_mission.LAR_Altitude = 0;
    new_mission.LAR_Track = 0;
    new_mission.LAR_Mach = 0;
    new_mission.ValidityFlags.LAR_Altitude = FALSE;
    new_mission.ValidityFlags.LAR_Track = FALSE;
    new_mission.ValidityFlags.LAR_Mach = FALSE;
    new_mission.ValidityFlags.LAR_WindSpeed = FALSE;
    new_mission.ValidityFlags.LAR_WindDirection = FALSE;
    new_mission.LAR_WindSpeed = 0;
    new_mission.LAR_WindDirection = 0;
    new_mission.WeaponType = RawMissionData->WeaponType;
    new_mission.MissionType = RawMissionData->MissionType;
    new_mission.CRC = RawMissionData->CRC;

    // Do validity checks...
    // Verify weapon type
    if (new_mission.WeaponType != weaponP3) {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 2;
    }

    // Verify mission type
    if (new_mission.MissionType == seekerSAL) {
      if (WeaponSettings.WeaponConfig.SeekerType != seekerSAL) {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 3;
      }
    }
    else if (new_mission.MissionType == seekerIIR) {
      if (WeaponSettings.WeaponConfig.SeekerType != seekerIIR) {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 4;
      }
    }

    // Verify target position
    if (WITHIN_LIMITS(RAD2DEG(new_mission.PriTgtLatitude), -90.0, 90.0)    &&
        WITHIN_LIMITS(RAD2DEG(new_mission.PriTgtLongitude), -180.0, 180.0) &&
        WITHIN_LIMITS(METER2FT(new_mission.PriTgtAltitude), -25000.0, 25000.0)) {
      if ((new_mission.PriTgtLatitude == 0.0) && (new_mission.PriTgtLongitude == 0.0)) {
        // Opportunity mode
        new_mission.TargetValid = FALSE;

        // Opportunity mode only valid if it is a SAL weapon
        if (new_mission.MissionType != seekerSAL) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 7;
        }
      }
      else {
        // Programmed mode
        new_mission.TargetValid = TRUE;
      }
    }
    else {
      new_mission.TargetValid = FALSE;
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 6;
    }

    // Verify dive angle and attack heading only when in programmed mode (target valid)
    if (new_mission.TargetValid) {
      // Verify dive angle
      if (new_mission.MissionType == seekerIIR) {
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.DiveAngle), 74.95, 90.05)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 11;
        }
      }
      else if (new_mission.MissionType == seekerSAL) {
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.DiveAngle), 29.95, 90.05)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 14;
        }
      }
      else {  // MissionType = GNSS
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.DiveAngle), 29.95, 90.05)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 15;
        }
      }

      // Verify attack heading
      if (new_mission.AttackHeadingValid) {
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.AttackHeading), 0.0, 360.0)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 16;
        }
      }
    }

    // Verify SAL code
    if (new_mission.MissionType == seekerSAL) {
    	// SAL Code is either a NATO code, a frequency or a period, depending on the range
    	if (RawMissionData->SAL_Code < 2000) {
    		// SAL Code is a NATO code
    		if (SAL_GetFrequency(&new_mission.SAL_Frequency, RawMissionData->SAL_Code) == FALSE) {
    			new_mission.MissionValid = FALSE;
    			new_mission.MissionSetResult = 21;
    		}
    	}
    	else if ((RawMissionData->SAL_Code >= 8000000) && (RawMissionData->SAL_Code  <= 20000000)) {
    		// SAL Code is a frequency
    		new_mission.SAL_Frequency = RawMissionData->SAL_Code * MISSION_FREQ_SF;
    	}
    	else if ((RawMissionData->SAL_Code >= 50000) && (RawMissionData->SAL_Code <= 125000)) {
    		// SAL Code is a period
    		new_mission.SAL_Frequency = 1.0 / (RawMissionData->SAL_Code * MISSION_FREQ_SF);
    	}
    	else {
    		// Invalid SAL Code
    		new_mission.MissionValid = FALSE;
    		new_mission.MissionSetResult = 22;
    	}

    	if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	{
    		MPSAL_SeekerSetNewParms(new_mission.SAL_Frequency,new_mission.SAL_Accuracy,new_mission.SAL_Jitter);
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On)
    	{
    		QSAL_SeekerSetNewParms(new_mission.SAL_Frequency,new_mission.SAL_Accuracy,new_mission.SAL_Jitter);
    	}
    	else if (AUXCTRL_SS.MAINControls.DSAL_On)
    	{
    		SAL_SeekerSetPRI(new_mission.SAL_Frequency);
    		SAL_SeekerSetAccuracy(new_mission.SAL_Accuracy);
    		SAL_SeekerSetJitter(new_mission.SAL_Jitter);
    	}
    }

    // Verify IIR image no
    if (new_mission.MissionType == seekerIIR) {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 24;
    }

    // Verify LAR launch conditions
    if (new_mission.ValidityFlags.LAR_Altitude &&
        new_mission.ValidityFlags.LAR_Track    &&
        new_mission.ValidityFlags.LAR_Mach       ) {
      new_mission.LAR_LaunchConditionsValid = TRUE;
    }
    else {
      new_mission.LAR_LaunchConditionsValid = FALSE;
    }

    // Verify LAR wind
    if (new_mission.ValidityFlags.LAR_WindDirection &&
        new_mission.ValidityFlags.LAR_WindSpeed        ) {
      new_mission.LAR_WindValid = TRUE;
    }
    else {
      new_mission.LAR_WindValid = FALSE;
    }

    // If mission is invalid, set target valid, LAR launch conditions and LAR wind to false
    if (!new_mission.MissionValid) {
      new_mission.TargetValid = FALSE;
      new_mission.LAR_LaunchConditionsValid = FALSE;
      new_mission.LAR_WindValid = FALSE;
    }
  }
  else {
    // CRC failed. Zero all mission parameters
    memset(&new_mission, 0, sizeof(sMission));
    memset(RawMissionData, 0, sizeof(sRawMission));

    new_mission.MissionValid = FALSE;
    new_mission.TargetValid  = FALSE;
    new_mission.LAR_LaunchConditionsValid = FALSE;
    new_mission.LAR_WindValid = FALSE;
    new_mission.MissionSetResult = 1;
  }

  Weapon.Mission = new_mission;
  Weapon.RawMission = *RawMissionData;
  Weapon.Flags.Bits.MissionOk = Weapon.Mission.MissionValid;
  Weapon.Flags.Bits.MissionMode = (Weapon.Mission.TargetValid == TRUE) ? mmPROGRAMMED : mmOPPORTUNITY;

  if (Weapon.Mission.MissionValid) {
    // Set the UBLOX receiver as required
    if (Weapon.GNSS_Usage.Byte != Weapon.Mission.GNSS_Usage.Byte) {
      Weapon.GNSS_Usage = new_mission.GNSS_Usage;
      Weapon.ConfigureUBLOX_ReceiverCnt = 0;
      Weapon.ConfigureUBLOX_Receiver = TRUE;
    }
  }

  GuidanceSetMission((sMission *) &Weapon.Mission);

  TelemetryBuild2HzBlock2();

  return new_mission.MissionValid;
}

unsigned char MissionSetFromAircraft(sAircraftRawMission *RawMissionData)
{
  unsigned char set_fuze_delay = FALSE;
  unsigned char set_hobs_height = FALSE;
  unsigned short crc;
  float altitude;
  sMission new_mission;


  crc = CalculateMIL_STD_1760D_Crc((unsigned short *) RawMissionData, (sizeof(sAircraftRawMission) - 2) / 2);

  if (crc == RawMissionData->CRC) {
    new_mission.MissionValid = TRUE;
    new_mission.MissionSetResult = 0;
    new_mission.MissionSource = msDTAC;
    new_mission.PriTgtLatitude = DEG2RAD((double) RawMissionData->PriTgtLatitude * 360.0 / 4294967296);
    new_mission.PriTgtLongitude = DEG2RAD((double) RawMissionData->PriTgtLongitude * 360.0 / 4294967296);
    new_mission.PriTgtAltitude  = FT2METER((double) RawMissionData->PriTgtAltitude * (50000.0 / 4294967296));
    new_mission.AttackHeading = DEG2RAD((double) RawMissionData->AttackHeading);
    new_mission.DiveAngle = DEG2RAD((double) RawMissionData->DiveAngle);
    new_mission.ImpactSpeed = RawMissionData->ImpactSpeed;
    new_mission.GNSS_Usage = RawMissionData->GNSS_Usage;
    new_mission.AttackHeadingValid = RawMissionData->AttackHeadingValid;
    new_mission.SAL_Frequency = 10.0;
    new_mission.SAL_Activation = RawMissionData->SAL_Activation;
    new_mission.SAL_Accuracy = RawMissionData->SAL_Accuracy;
    new_mission.SAL_Jitter = RawMissionData->SAL_Jitter;
    new_mission.IIR_ImageNo = 0;
    new_mission.IIR_ImageCRC = RawMissionData->IIR_ImageCRC;
    new_mission.LAR_Track = DEG2RAD((double) RawMissionData->LAR_Track * 0.0054932479);
    new_mission.LAR_Mach = (double) RawMissionData->LAR_Mach * 0.0001249961;
    new_mission.LAR_Altitude = FT2METER((double) RawMissionData->LAR_Altitude * 0.00004656612);
    new_mission.LAR_WindSpeed = RawMissionData->LAR_WindSpeed * 0.031250954;
    new_mission.LAR_WindDirection = DEG2RAD(RawMissionData->LAR_WindDirection * 0.0054932479);
    new_mission.WeaponType = RawMissionData->WeaponType;
    new_mission.MissionType = RawMissionData->MissionType & 0x03;
    new_mission.ValidityFlags = RawMissionData->ValidityFlags;
    new_mission.CRC = RawMissionData->CRC;

    // Update mission fuze mode
    if (RawMissionData->FuzingMode.FunctTimeAfterImpact && RawMissionData->FuzingMode.FunctHeight) {
      new_mission.FuzeMode = fmHEIGHT_AND_DELAY;
    }
    else if (RawMissionData->FuzingMode.FunctTimeAfterImpact) {
      new_mission.FuzeMode = fmDELAY;
    }
    else if (RawMissionData->FuzingMode.FunctHeight) {
      new_mission.FuzeMode = fmHEIGHT;
    }
    else {
      new_mission.FuzeMode = fmNON_PROGRAMMABLE;
    }

    // Do validity checks...

    // Verify weapon type
    if (new_mission.WeaponType != weaponP3) {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 2;
    }

    // Verify mission type
    if (new_mission.MissionType == seekerSAL) {
      if (WeaponSettings.WeaponConfig.SeekerType != seekerSAL) {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 3;
      }
    }
    else if (new_mission.MissionType == seekerIIR) {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 4;
    }

    // Verify target position
    if (new_mission.ValidityFlags.PriTgtLatitude  &&
        new_mission.ValidityFlags.PriTgtLongitude &&
        new_mission.ValidityFlags.PriTgtAltitude    ) {
      if (WITHIN_LIMITS(RAD2DEG(new_mission.PriTgtLatitude), -90.0, 90.0)    &&
          WITHIN_LIMITS(RAD2DEG(new_mission.PriTgtLongitude), -180.0, 180.0) &&
          WITHIN_LIMITS(METER2FT(new_mission.PriTgtAltitude), -25000.0, 25000.0)) {
        // Convert the Altitude in EGM-96 to WGS84
        if (GetEGM96GeoidHeight(&altitude, new_mission.PriTgtLatitude, new_mission.PriTgtLongitude)) {
          new_mission.PriTgtAltitude = new_mission.PriTgtAltitude + altitude;
        }

        new_mission.TargetValid = TRUE;
      }
      else {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 6;
      }
    }
    else {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 5;
    }

    // Verify dive angle
    if (RawMissionData->ValidityFlags.DiveAngle) {
      if (new_mission.MissionType == seekerSAL) {
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.DiveAngle), 29.95, 60.05)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 13;
        }
      }
      else {  // MissionType = GNSS
        if (!WITHIN_LIMITS(RAD2DEG(new_mission.DiveAngle), 29.95, 90.05)) {
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 15;
        }
      }
    }
    else {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 10;
    }

    // Verify attack heading
    if (new_mission.AttackHeadingValid) {
      if (!WITHIN_LIMITS(RAD2DEG(new_mission.AttackHeading), 0.0, 360.0)) {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 16;
      }
    }

    // Verify SAL code.
    if (new_mission.MissionType == seekerSAL) {
      if (RawMissionData->ValidityFlags.SAL_Code == TRUE) {
        // SAL Code is either a NATO code, a frequency or a period, depending on the range
        if (RawMissionData->SAL_Code < 2000) {
          // SAL Code is a NATO code
          if (SAL_GetFrequency(&new_mission.SAL_Frequency, RawMissionData->SAL_Code) == FALSE) {
            new_mission.MissionValid = FALSE;
            new_mission.MissionSetResult = 21;
          }
        }
        else if ((RawMissionData->SAL_Code >= 8000000) && (RawMissionData->SAL_Code  <= 20000000)) {
          // SAL Code is a frequency
          new_mission.SAL_Frequency = RawMissionData->SAL_Code * MISSION_FREQ_SF;
        }
        else if ((RawMissionData->SAL_Code >= 50000) && (RawMissionData->SAL_Code <= 125000)) {
          // SAL Code is a period
          new_mission.SAL_Frequency = 1.0 / (RawMissionData->SAL_Code * MISSION_FREQ_SF);
        }
        else {
          // Invalid SAL Code
          new_mission.MissionValid = FALSE;
          new_mission.MissionSetResult = 22;
        }
      }
      else {
        new_mission.MissionValid = FALSE;
        new_mission.MissionSetResult = 20;
      }

      SAL_SeekerSetPRI(new_mission.SAL_Frequency);
      SAL_SeekerSetAccuracy(5000);
      SAL_SeekerSetJitter(500);
    }

    // Verify IIR image no
    if (new_mission.MissionType == seekerIIR) {
      new_mission.MissionValid = FALSE;
      new_mission.MissionSetResult = 24;
    }

    // Verify fuze mode
    if (new_mission.ValidityFlags.FuzeSettings == TRUE) {
      if (RawMissionData->FuzingMode.FunctTimeAfterImpact) {
        set_fuze_delay = TRUE;
      }

      if (RawMissionData->FuzingMode.FunctHeight) {
        set_hobs_height = TRUE;
      }
    }

    // Verify fuze mode
    if (new_mission.ValidityFlags.FuzeSettings == TRUE) {
      if (RawMissionData->FuzingMode.FunctTimeAfterImpact) {
        set_fuze_delay = TRUE;
      }

      if (RawMissionData->FuzingMode.FunctHeight) {
        set_hobs_height = TRUE;
      }
    }

    // Set fuze delay
    if (set_fuze_delay) {
      if ((new_mission.ValidityFlags.FuzeDelay == TRUE) && ((SS_FUZE.Fuze.Mode == fmDELAY) || (SS_FUZE.Fuze.Mode == fmHEIGHT_AND_DELAY))) {
        if (SS_FUZE.Fuze.FuzeTimeDelay != new_mission.FuzeDelay) {
          // Update fuze delay
          new_mission.UpdatingFuze = TRUE;
          Fuze_SetDelay(new_mission.FuzeDelay, MLAN_ITU_NODE_ID);
        }
      }
    }
    else if (SS_FUZE.Fuze.FuzeTimeDelay != 0) {
    	Fuze_SetDelay(0, MLAN_ITU_NODE_ID);
    }

    // Set HOBS height
    if (set_hobs_height) {
      if (WeaponSettings.WeaponConfig.HOBS_Present && RawMissionData->ValidityFlags.FuzeHeight) {
//        HOBS_SetSetHeight(new_mission.FuzingDistance);
      }
    }
    else {
//      HOBS_SetSetHeight(0);
    }

    // Verify LAR launch conditions
    if (new_mission.ValidityFlags.LAR_Altitude &&
        new_mission.ValidityFlags.LAR_Track    &&
        new_mission.ValidityFlags.LAR_Mach       ) {
      // Convert the Altitude in EGM-96 to WGS84
      if (GetEGM96GeoidHeight(&altitude, Weapon.Latitude, Weapon.Longitude)) {
        new_mission.LAR_Altitude = new_mission.LAR_Altitude + altitude;
      }

      new_mission.LAR_LaunchConditionsValid = TRUE;
    }
    else {
      new_mission.LAR_LaunchConditionsValid = FALSE;
    }

    // Verify LAR wind
    if (new_mission.ValidityFlags.LAR_WindDirection &&
        new_mission.ValidityFlags.LAR_WindSpeed        ) {
      new_mission.LAR_WindValid = TRUE;
    }
    else {
      new_mission.LAR_WindValid = FALSE;
    }

    // If mission is invalid, set target valid, LAR launch conditions and LAR wind to false
    if (!new_mission.MissionValid) {
      new_mission.TargetValid = FALSE;
      new_mission.LAR_LaunchConditionsValid = FALSE;
      new_mission.LAR_WindValid = FALSE;
    }
  }
  else {
    // CRC failed. Zero all mission parameters
    memset(&new_mission, 0, sizeof(sMission));
    memset(RawMissionData, 0, sizeof(sAircraftRawMission));

    new_mission.MissionValid = FALSE;
    new_mission.TargetValid  = FALSE;
    new_mission.LAR_LaunchConditionsValid = FALSE;
    new_mission.LAR_WindValid = FALSE;
    new_mission.MissionSetResult = 1;
  }

  Weapon.Mission = new_mission;
  Weapon.AircraftRawMission = *RawMissionData;
  Weapon.Flags.Bits.MissionOk = new_mission.MissionValid;
  Weapon.Flags.Bits.MissionMode = (new_mission.TargetValid == TRUE) ? mmPROGRAMMED : mmOPPORTUNITY;


  // Copy to RawMission for telemetry purposes
  Weapon.RawMission.PriTgtLatitude      = Weapon.AircraftRawMission.PriTgtLatitude;
  Weapon.RawMission.PriTgtLongitude     = Weapon.AircraftRawMission.PriTgtLongitude;
  Weapon.RawMission.PriTgtAltitude      = Weapon.AircraftRawMission.PriTgtAltitude;
  Weapon.RawMission.AttackHeading       = Weapon.AircraftRawMission.AttackHeading;
  Weapon.RawMission.DiveAngle           = Weapon.AircraftRawMission.DiveAngle;
  Weapon.RawMission.ImpactSpeed         = Weapon.AircraftRawMission.ImpactSpeed;
  Weapon.RawMission.GNSS_Usage          = Weapon.AircraftRawMission.GNSS_Usage;
  Weapon.RawMission.AttackHeadingValid  = Weapon.AircraftRawMission.AttackHeadingValid;
  Weapon.RawMission.SecondarySwitch     = Weapon.AircraftRawMission.SecondarySwitch;
  Weapon.RawMission.SecTgtLatitude      = Weapon.AircraftRawMission.SecTgtLatitude;
  Weapon.RawMission.SecTgtLongitude     = Weapon.AircraftRawMission.SecTgtLongitude;
  Weapon.RawMission.SecTgtAltitude      = Weapon.AircraftRawMission.SecTgtAltitude;
  Weapon.RawMission.SAL_Code            = Weapon.AircraftRawMission.SAL_Code;
  Weapon.RawMission.SAL_Activation      = Weapon.AircraftRawMission.SAL_Activation;
  Weapon.RawMission.SAL_Accuracy        = Weapon.AircraftRawMission.SAL_Accuracy;
  Weapon.RawMission.SAL_Jitter          = Weapon.AircraftRawMission.SAL_Jitter;
  Weapon.RawMission.IIR_ImageNo         = Weapon.AircraftRawMission.IIR_ImageNo;
  Weapon.RawMission.IIR_ImageOffsetX    = Weapon.AircraftRawMission.IIR_ImageOffsetX;
  Weapon.RawMission.IIR_ImageOffsetY    = Weapon.AircraftRawMission.IIR_ImageOffsetY;
  Weapon.RawMission.IIR_ImageCRC        = Weapon.AircraftRawMission.IIR_ImageCRC;
  Weapon.RawMission.WeaponType          = Weapon.AircraftRawMission.WeaponType;
  Weapon.RawMission.MissionType         = Weapon.AircraftRawMission.MissionType;
  Weapon.RawMission.CRC                 = Weapon.AircraftRawMission.CRC;

  if (Weapon.Mission.MissionValid) {
    // Update GNSS usage
    if (Weapon.GNSS_Usage.Byte != Weapon.Mission.GNSS_Usage.Byte) {
      Weapon.GNSS_Usage = Weapon.Mission.GNSS_Usage;
      Weapon.ConfigureUBLOX_ReceiverCnt = 0;
      Weapon.ConfigureUBLOX_Receiver = TRUE;
    }
  }

  GuidanceSetMission((sMission *) &Weapon.Mission);

  TelemetryBuild2HzBlock2();

  return new_mission.MissionValid;
}

void SetGNSS_Usage(uint16_t *WeaponSettings,  uGNSS_Usage *GNSS_Usage){
	bFunctionalExchange2MissionSettings *WIAC82MissionSettings;

	WIAC82MissionSettings = (bFunctionalExchange2MissionSettings *)WeaponSettings;

	if (WIAC82MissionSettings->GNSSMode == WIAC82_GNSS_MODE_ALL_OFF) {
		GNSS_Usage->GPS_NotUsed = 1;
		GNSS_Usage->GlonassNotUsed = 1;
		GNSS_Usage->Reserved = 0;
	}
	else if (WIAC82MissionSettings->GNSSMode == WIAC82_GNSS_MODE_GLONASS_OFF_GPS_ON) {
		GNSS_Usage->GPS_NotUsed = 0;
		GNSS_Usage->GlonassNotUsed = 1;
		GNSS_Usage->Reserved = 0;
	}
	else if (WIAC82MissionSettings->GNSSMode == WIAC82_GNSS_MODE_GLONASS_ON_GPS_OFF) {
		GNSS_Usage->GPS_NotUsed = 1;
		GNSS_Usage->GlonassNotUsed = 0;
		GNSS_Usage->Reserved = 0;
	}
	else if (WIAC82MissionSettings->GNSSMode == WIAC82_GNSS_MODE_GLONASS_ON_GPS_ON) {
		GNSS_Usage->GPS_NotUsed = 0;
		GNSS_Usage->GlonassNotUsed = 0;
		GNSS_Usage->Reserved = 0;
	}
	else {
		GNSS_Usage->GPS_NotUsed = 1;
		GNSS_Usage->GlonassNotUsed = 1;
		GNSS_Usage->Reserved = 0;
	}
}

char *DecodeMissionSource(eMissionSource MissionSource){
	static char CharMissionSource[20];

	if (MissionSource == msWIRELESS_CONTROLLER)
		sprintf(CharMissionSource,"%s","WIRELESS_CONTROLLER");
	else if (MissionSource == msDTAC)
		sprintf(CharMissionSource,"%s","DTAC");
	else if (MissionSource == msRTAC)
		sprintf(CharMissionSource,"%s","RTAC");
	else if (MissionSource == msETE)
		sprintf(CharMissionSource,"%s","sETE");
	else if (MissionSource == msEEPROM)
		sprintf(CharMissionSource,"%s","EEPROM");
	else
		sprintf(CharMissionSource,"%s","UNKNOWN");

	return(CharMissionSource);
}

char *DecodeMissionGNSS_Usage(uGNSS_Usage GNSS_Use){
	static char strGNSS_Usage[50];
	strGNSS_Usage[0]=0;  //clear

	if (GNSS_Use.GPS_NotUsed)
		sprintf(strGNSS_Usage,"%s%s",strGNSS_Usage," NO GPS");
	else
		sprintf(strGNSS_Usage,"%s%s",strGNSS_Usage," GPS USED");
	if (GNSS_Use.GlonassNotUsed)
		sprintf(strGNSS_Usage,"%s%s",strGNSS_Usage," NO GLANASS");
	else
		sprintf(strGNSS_Usage,"%s%s",strGNSS_Usage," GLANASS USED");
	return(strGNSS_Usage);
}

void SetFuzeMode(uint16_t *WeaponSettings,  eFuzeMode *FuzeMode){
	bFunctionalExchange2MissionSettings *WIAC82MissionSettings;

	WIAC82MissionSettings = (bFunctionalExchange2MissionSettings *)WeaponSettings;

	if (WIAC82MissionSettings->FuzeSetting == WIAC82_FUZE_SETTING_AT_IMPACT) {
		*FuzeMode = fmNON_PROGRAMMABLE;
	}
	else if (WIAC82MissionSettings->FuzeSetting == WIAC82_FUZE_SETTING_TIME_AFTER_IMPACT) {
		*FuzeMode = fmDELAY;
	}
	else if (WIAC82MissionSettings->FuzeSetting == WIAC82_FUZE_SETTING_AT_ALTITUDE) {
		*FuzeMode = fmHEIGHT;
	}
	else {
		*FuzeMode = fmHEIGHT_AND_DELAY;
	}
}

char *DecodeMissionFuzeMode(eFuzeMode FuzeMode){
	static char strFuzeMode[50];
	strFuzeMode[0]=0;  //clear

	if (FuzeMode == fmNON_PROGRAMMABLE)
		sprintf(strFuzeMode,"fmNON_PROGRAMMABLE");
	else if (FuzeMode == fmDELAY)
		sprintf(strFuzeMode,"fmDELAY");
	else if (FuzeMode == fmHEIGHT)
		sprintf(strFuzeMode,"fmHEIGHT");
	else if (FuzeMode == fmHEIGHT_AND_DELAY)
		sprintf(strFuzeMode,"fmHEIGHT_AND_DELAY");
	else
		sprintf(strFuzeMode,"UNKNOWN");
	return(strFuzeMode);
}

char *DecodeMissionWeaponType(eWeaponType WeaponType){
	static char strWeaponType[50];
	strWeaponType[0]=0;  //clear

	if (WeaponType == weaponAL_TARIQ)
		sprintf(strWeaponType,"weaponAL_TARIQ");
	else if (WeaponType == weaponP3)
		sprintf(strWeaponType,"weaponP3");
	else
		sprintf(strWeaponType,"UNKNOWN");
	return(strWeaponType);
}

char *DecodeMissionType(eSeekerType MissionType){
	static char strMissionType[50];
	strMissionType[0]=0;  //clear

	if (MissionType == seekerNONE)
		sprintf(strMissionType,"seekerNONE");
	else if (MissionType == seekerSAL)
		sprintf(strMissionType,"seekerSAL");
	else
		sprintf(strMissionType,"UNKNOWN");
	return(strMissionType);
}

void SetMissionType(uint16_t *MissionSettings,  eSeekerType *MissionType){
	bFunctionalExchange2MissionSettings *WIAC82MissionSettings;

	WIAC82MissionSettings = (bFunctionalExchange2MissionSettings *)MissionSettings;

	if (WIAC82MissionSettings->SeekerSetting == WIAC82_SEEKER_SETTING_GNSS_ONLY) {
		*MissionType = seekerNONE;
	}
	else if (WIAC82MissionSettings->SeekerSetting == WIAC82_SEEKER_SETTING_SAL) {
		*MissionType = seekerSAL;
	}
	else {
		*MissionType = seekerIIR;
	}
}

void SetValidityFlags(uint16_t *ValidityWord,  sAircraftMissionValidityFlags *ValidityFlags){
	uint16_t WIAC82ValidityData;
	bFunctionalExchange2ValidityWord *FuncValid;;

	WIAC82ValidityData = (*ValidityWord);
	FuncValid = (bFunctionalExchange2ValidityWord *)&WIAC82ValidityData;
	ValidityFlags->PriTgtLatitude = FuncValid->TargetLongLatAlt;
	ValidityFlags->PriTgtLongitude = FuncValid->TargetLongLatAlt;
	ValidityFlags->PriTgtAltitude = FuncValid->TargetLongLatAlt;
	ValidityFlags->DiveAngle = FuncValid->DiveAngle;
	ValidityFlags->AttackHeading = FuncValid->ImpactHeading;
	ValidityFlags->ImpactSpeed = FuncValid->ImpactSpeed;
	ValidityFlags->SAL_Code =  FuncValid->SALCode;
	ValidityFlags->IIR_ImageNo = FuncValid->ImageIndexCRC;
	//FuncValid->MovingTargetMode; no map
	ValidityFlags->FuzeSettings = FuncValid->FuzeSettingDelayHeight;
	ValidityFlags->FuzeDelay = FuncValid->FuzeSettingDelayHeight;
	ValidityFlags->FuzeHeight = FuncValid->FuzeSettingDelayHeight;
	//FuncValid->GNSS_HybridationActivation; no map
	ValidityFlags->LAR_Track =0;
	ValidityFlags->LAR_Mach  =0;
	ValidityFlags->LAR_Altitude  =0;
	ValidityFlags->LAR_WindSpeed =0;
	ValidityFlags->LAR_WindDirection =0;
}

unsigned char SetMissionFromWIAC82(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData, unsigned short CRC_OK)
{
	unsigned char SAL_CodeValid;

	if (CRC_OK) {
		MISS_SS.WIAC82_Mission.MissionValid = TRUE;
		MISS_SS.WIAC82_Mission.MissionSetResult = 0;
		MISS_SS.WIAC82_Mission.MissionSource = msRTAC;
		MISS_SS.WIAC82_Mission.PriTgtLatitude = Get2WordDoubleS(&WIAC82MissionData->FuncExch.TargetLatitude1,DOUBLE_2POWER_32)*TWO_PI;
		MISS_SS.WIAC82_Mission.PriTgtLongitude = Get2WordDoubleS(&WIAC82MissionData->FuncExch.TargetLongitude1,DOUBLE_2POWER_32)*TWO_PI;
		MISS_SS.WIAC82_Mission.PriTgtAltitude  = Get2WordFloatS(&WIAC82MissionData->FuncExch.TargetAltitude1,FLOAT_2POWER_16);
    	MISS_SS.WIAC82_Mission.AttackHeading = 0;
    	MISS_SS.WIAC82_Mission.DiveAngle = Get1WordFloatU(WIAC82MissionData->FuncExch.DiveAngle,FLOAT_2POWER_17)*TWO_PI;
    	MISS_SS.WIAC82_Mission.ImpactSpeed = 0;
    	SetGNSS_Usage(&WIAC82MissionData->FuncExch.MissionSettings,&MISS_SS.WIAC82_Mission.GNSS_Usage);
    	MISS_SS.WIAC82_Mission.AttackHeadingValid = 0;
    	MISS_SS.WIAC82_Mission.SAL_Code = WIAC82MissionData->FuncExch.SALCode;
    	SAL_CodeValid = SAL_GetFrequency(&MISS_SS.WIAC82_Mission.SAL_Frequency,WIAC82MissionData->FuncExch.SALCode);
    	MISS_SS.WIAC82_Mission.SAL_Activation = 0;
    	MISS_SS.WIAC82_Mission.SAL_Accuracy = 5000;
    	MISS_SS.WIAC82_Mission.SAL_Jitter = 500;
    	MISS_SS.WIAC82_Mission.IIR_ImageNo = 0;
    	MISS_SS.WIAC82_Mission.IIR_ImageCRC = 0;
    	SetFuzeMode(&WIAC82MissionData->FuncExch.MissionSettings,&MISS_SS.WIAC82_Mission.FuzeMode);
    	MISS_SS.WIAC82_Mission.FuzeDelay = WIAC82MissionData->FuncExch.FuzeDelay;
    	MISS_SS.WIAC82_Mission.FuzingDistance = WIAC82MissionData->FuncExch.FuzeHeight;
    	MISS_SS.WIAC82_Mission.WeaponType = WeaponSettings.WeaponConfig.WeaponType;
    	SetMissionType(&WIAC82MissionData->FuncExch.MissionSettings,&MISS_SS.WIAC82_Mission.MissionType);
    	SetValidityFlags(&WIAC82MissionData->FuncExch.Validity,&MISS_SS.WIAC82_Mission.ValidityFlags);
    	MISS_SS.WIAC82_Mission.CRC = WIAC82MissionData->FuncExch.CRC;
    	/*
        	MISS_SS.WIAC82_Mission.LAR_Track = DEG2RAD((double) RawMissionData->LAR_Track * 0.0054932479);
        	MISS_SS.WIAC82_Mission.LAR_Mach = (double) RawMissionData->LAR_Mach * 0.0001249961;
        	MISS_SS.WIAC82_Mission.LAR_Altitude = FT2METER((double) RawMissionData->LAR_Altitude * 0.00004656612);
        	MISS_SS.WIAC82_Mission.LAR_WindSpeed = RawMissionData->LAR_WindSpeed * 0.031250954;
        	MISS_SS.WIAC82_Mission.LAR_WindDirection = DEG2RAD(RawMissionData->LAR_WindDirection * 0.0054932479);
    	 */
    	if (AUXCTRL_SS.MissionControls.MissionAuxOn){
    		AUXsprintf("%u:Setting New MISSION from WIAC82 data\n",GetTimeUs());
    		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn){
    			AUXsprintf("  MissionSource      = %s\n",DecodeMissionSource(MISS_SS.WIAC82_Mission.MissionSource));
    			AUXsprintf("  PriTgtLatitude     = %s [rad]\n",AUXsprintfDouble(MISS_SS.WIAC82_Mission.PriTgtLatitude));
    			AUXsprintf("  PriTgtLongitude    = %s [rad]\n",AUXsprintfDouble(MISS_SS.WIAC82_Mission.PriTgtLongitude));
    			AUXsprintf("  PriTgtAltitude     = %s [m]\n",AUXsprintfDouble(MISS_SS.WIAC82_Mission.PriTgtAltitude));
    			AUXsprintf("  AttackHeading      = %s [rad]\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.AttackHeading));
    			AUXsprintf("  DiveAngle          = %s [rad]\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.DiveAngle));
    			AUXsprintf("  ImpactSpeed        = %s [m/s]\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.ImpactSpeed));

    			AUXsprintf("  GNSS_Usage         = 0x%x %s\n",MISS_SS.WIAC82_Mission.GNSS_Usage,DecodeMissionGNSS_Usage(MISS_SS.WIAC82_Mission.GNSS_Usage));
    			AUXsprintf("  AttackHeadingValid = %d\n",MISS_SS.WIAC82_Mission.AttackHeadingValid);
    			AUXsprintf("  SAL_Frequency      = %8s [Hz] Valid = %d\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.SAL_Frequency),SAL_CodeValid);
    			AUXsprintf("  SAL_Activation     = %d\n",MISS_SS.WIAC82_Mission.SAL_Activation);
    			AUXsprintf("  SAL_Accuracy       = %d\n",MISS_SS.WIAC82_Mission.SAL_Accuracy);
    			AUXsprintf("  SAL_Jitter         = %d\n",MISS_SS.WIAC82_Mission.SAL_Jitter);
    			AUXsprintf("  ImageIndex         = %d\n",MISS_SS.WIAC82_Mission.IIR_ImageNo);
    			AUXsprintf("  ImageCRC           = 0x%4.4x\n",MISS_SS.WIAC82_Mission.IIR_ImageCRC);
    			AUXsprintf("  FuzeMode           = %d %s\n",MISS_SS.WIAC82_Mission.FuzeMode,DecodeMissionFuzeMode(MISS_SS.WIAC82_Mission.FuzeMode));
				AUXsprintf("  WeaponType         = %d %s\n",MISS_SS.WIAC82_Mission.WeaponType,DecodeMissionWeaponType(MISS_SS.WIAC82_Mission.WeaponType));
				AUXsprintf("  MissionType        = %d %s\n",MISS_SS.WIAC82_Mission.MissionType,DecodeMissionType(MISS_SS.WIAC82_Mission.MissionType));
				AUXsprintf("  ValidityFlags      = 0x%4.4x\n",MISS_SS.WIAC82_Mission.ValidityFlags);
				if (AUXCTRL_SS.MissionControls.MissionDetail2AuxOn){
					AUXsprintf("   PriTgtLatitude    = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtLatitude);
					AUXsprintf("   PriTgtLongitude   = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtLongitude);
					AUXsprintf("   PriTgtAltitude    = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtAltitude);
					AUXsprintf("   DiveAngle         = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.DiveAngle);
					AUXsprintf("   AttackHeading     = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.AttackHeading);
					AUXsprintf("   ImpactSpeed       = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.ImpactSpeed);
					AUXsprintf("   SAL_Code          = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.SAL_Code);
					AUXsprintf("   IIR_ImageNo       = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.IIR_ImageNo);
					AUXsprintf("   FuzeSettings      = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.FuzeSettings);
					AUXsprintf("   FuzeDelay         = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.FuzeDelay);
					AUXsprintf("   FuzeHeight        = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.FuzeHeight);
					AUXsprintf("   LAR_Track         = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.LAR_Track);
					AUXsprintf("   LAR_Mach          = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.LAR_Mach);
					AUXsprintf("   LAR_Altitude      = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.LAR_Altitude);
					AUXsprintf("   LAR_WindSpeed     = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.LAR_WindSpeed);
					AUXsprintf("   LAR_WindDirection = %d\n",MISS_SS.WIAC82_Mission.ValidityFlags.LAR_WindDirection);
				}
				AUXsprintf("  CRC                = 0x%4.4x\n",MISS_SS.WIAC82_Mission.CRC);
    		}
    	}

		CheckValidityOfMission(WIAC82MissionData);

		Weapon.Mission = MISS_SS.WIAC82_Mission;
		CopyWIAC82DatatoTelemetry(); // RawMission
		Weapon.Flags.Bits.MissionOk = MISS_SS.WIAC82_Mission.MissionValid;
		Weapon.Flags.Bits.MissionMode = (MISS_SS.WIAC82_Mission.TargetValid == TRUE) ? mmPROGRAMMED : mmOPPORTUNITY;

		if (MISS_SS.WIAC82_Mission.MissionValid == TRUE)
	 		UpdateGNSS_Usage();

		GuidanceSetMission((sMission *) &Weapon.Mission);

		TelemetryBuild2HzBlock2();
	}
 	else {
 		// Mission Invalid CRC ERROR
 		MISS_SS.WIAC82_Mission.MissionValid = FALSE;
 		Weapon.Flags.Bits.MissionOk = MISS_SS.WIAC82_Mission.MissionValid;
 	}

 	return MISS_SS.WIAC82_Mission.MissionValid;
}

char VerifyWIAC82WeaponType(void) {
	char WeaponTypeOK;

    // Weapon Mode Error (Not P32)
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying weapon type - ");
	if (MISS_SS.WIAC82_Mission.WeaponType != weaponP3) {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf(" NOT weaponP3 - Mission set INVALID\n");
    	MISS_SS.WIAC82_Mission.MissionSetResult = 2;
    	WeaponTypeOK = 0;
   }
	else{
		WeaponTypeOK = 1;
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf(" weaponP3 - OK\n");
	}
	return(WeaponTypeOK);
}

char VerifyWIAC82MissionType(void) {
	char MissionTypeOK;

	// Verify mission type (Mission configured for SAL but weapon has not SAL configured)
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying mission type -");
	if (MISS_SS.WIAC82_Mission.MissionType == seekerSAL) {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf(" seekerSAL");
		if (WeaponSettings.WeaponConfig.SeekerType != seekerSAL) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf(" Weapon Seeker Type NOT seekerSAL - Mission set INVALID\n");
			MISS_SS.WIAC82_Mission.MissionSetResult = 3;
			MissionTypeOK = 0;
		}
		else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf(" Weapon Mission Type seekerSAL -");
			if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf(" Weapon has seekerSAL - OK\n");
				MissionTypeOK = 1;
			}
			else {
				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf(" Weapon does not have seekerSAL - Mission set INVALID\n");
				MISS_SS.WIAC82_Mission.MissionSetResult = 3;
				MissionTypeOK = 0;
			}
		}
    }
    else if (MISS_SS.WIAC82_Mission.MissionType == seekerIIR) {
    		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    			AUXsprintf(" seekerIIR - Mission set INVALID\n");
    		MISS_SS.WIAC82_Mission.MissionSetResult = 4;
			MissionTypeOK = 0;
  	 }
	else if (MISS_SS.WIAC82_Mission.MissionType == seekerNONE) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf(" seekerNONE is GNSS - OK\n");
			MissionTypeOK = 1;
	}
    else {
    		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    			AUXsprintf(" UNKNOWN - Mission set INVALID\n");
    		MISS_SS.WIAC82_Mission.MissionSetResult = 4;
			MissionTypeOK = 0;
    }
	return(MissionTypeOK);
}

char VerifyWIAC82TargetPosition(void) {
	char TargetPositionOK;
	float altitude;

    // Verify target position (Target Error (Validity of LATITUDE, LONGITUDE AND ALTITUDE))
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying target position ");
    if (MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtLatitude  &&
    	MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtLongitude &&
		MISS_SS.WIAC82_Mission.ValidityFlags.PriTgtAltitude    ) {
    		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    			AUXsprintf("Validity Set -");
    		// 1553 ICD Min and Max Value
    		if (WITHIN_LIMITS(RAD2DEG(MISS_SS.WIAC82_Mission.PriTgtLatitude), -90.0, 90.0)    &&
    			WITHIN_LIMITS(RAD2DEG(MISS_SS.WIAC82_Mission.PriTgtLongitude), -180.0, 180.0) &&
				WITHIN_LIMITS(METER2FT(MISS_SS.WIAC82_Mission.PriTgtAltitude), -1000.0, 20000.0)) {
    	       	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    	       		AUXsprintf(" Value Within Limits -");
    	       	// Convert the Altitude in EGM-96 to WGS84
    			if (GetEGM96GeoidHeight(&altitude, MISS_SS.WIAC82_Mission.PriTgtLatitude, MISS_SS.WIAC82_Mission.PriTgtLongitude)) {
    				MISS_SS.WIAC82_Mission.PriTgtAltitude = MISS_SS.WIAC82_Mission.PriTgtAltitude + altitude;
        	       	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        	       		AUXsprintf(" Altitude Converted EGM-96 to WGS84 - ");
    			}
    	       	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    	       		AUXsprintf(" OK\n");
    			MISS_SS.WIAC82_Mission.TargetValid = TRUE;
    			TargetPositionOK = 1;
    		}
    		else {
            	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
            		AUXsprintf(" Value Out of Limits - Mission set INVALID\n");
    			MISS_SS.WIAC82_Mission.MissionValid = FALSE;
    			MISS_SS.WIAC82_Mission.MissionSetResult = 6;
    			TargetPositionOK = 0;
    		}
    }
    else {
    	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    		AUXsprintf("Validity NOT Set - Mission set INVALID\n");
    	MISS_SS.WIAC82_Mission.MissionValid = FALSE;
    	MISS_SS.WIAC82_Mission.MissionSetResult = 5;
		TargetPositionOK = 0;
    }
    return (TargetPositionOK);
}

float DiveAngleWIAC82P32Values(float diveAngle)
{
    float diveAngleSetOfValues;

    if (WITHIN_LIMITS(RAD2DEG(diveAngle), 29.95, 30.05) ||
            WITHIN_LIMITS(RAD2DEG(diveAngle), 44.95, 45.05) ||
    		WITHIN_LIMITS(RAD2DEG(diveAngle), 59.95, 60.05) ||
    		WITHIN_LIMITS(RAD2DEG(diveAngle), 74.95, 75.05) ||
			WITHIN_LIMITS(RAD2DEG(diveAngle), 89.95, 90.05)) {

        diveAngleSetOfValues = diveAngle;

    } else {
        if (RAD2DEG(diveAngle) <= 37.49) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Dive Angle rounded to - 30 ° \n");
            diveAngleSetOfValues = DEG2RAD(30.01);
        } else if (RAD2DEG(diveAngle) <= 52.49) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Dive Angle rounded to - 45 ° \n");
        	diveAngleSetOfValues = DEG2RAD(45.01);
        } else if (RAD2DEG(diveAngle) <= 67.49) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Dive Angle rounded to - 60 ° \n");
        	diveAngleSetOfValues = DEG2RAD(60.01);
        } else if (RAD2DEG(diveAngle) <= 82.49) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Dive Angle rounded to - 75 ° \n");
        	diveAngleSetOfValues = DEG2RAD(75.01);
        } else {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Dive Angle rounded to - 90 ° \n");
            diveAngleSetOfValues = DEG2RAD(90.01);
        }
    }

    return diveAngleSetOfValues;
}

char VerifyWIAC82DiveAngle(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {
	bFunctionalExchange2MissionSettings *MissionSettings;

	char DiveAngleOK;
    // Verify dive angle
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying dive angle - ");
    if (MISS_SS.WIAC82_Mission.ValidityFlags.DiveAngle) {
    	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    		AUXsprintf("Validity Set -");
    	if (MISS_SS.WIAC82_Mission.MissionType == seekerSAL) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("Type seekerSAL -");
        	// For P32 SAL : (30° to 60° – increment 15°)
    		if (!WITHIN_LIMITS(RAD2DEG(MISS_SS.WIAC82_Mission.DiveAngle), 29.95, 60.05)) {
            	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
            		AUXsprintf(" Value Out of Limits - Mission set INVALID\n");
    			MISS_SS.WIAC82_Mission.MissionValid = FALSE;
    			MISS_SS.WIAC82_Mission.MissionSetResult = 13;
    			DiveAngleOK = 0;
    		}
    		else {
    	       	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    	       		AUXsprintf(" Value Within Limits - OK\n");

    	       	// Added 30°, 45°, 60°, 75° and 90° closest value
    	       	MISS_SS.WIAC82_Mission.DiveAngle = DiveAngleWIAC82P32Values(MISS_SS.WIAC82_Mission.DiveAngle);
    			DiveAngleOK = 1;
    		}
    	}
    	else {
    		// For P32 GNSS : (30° to 90° – increment 15°)
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("Type GNSS -");
    		if (!WITHIN_LIMITS(RAD2DEG(MISS_SS.WIAC82_Mission.DiveAngle), 29.95, 90.05)) {
            	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
            		AUXsprintf(" Value Out of Limits - Mission set INVALID\n");
    			MISS_SS.WIAC82_Mission.MissionValid = FALSE;
    			MISS_SS.WIAC82_Mission.MissionSetResult = 15;
    			DiveAngleOK = 0;
    		}
    		else {
    	       	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    	       		AUXsprintf(" Value Within Limits - OK\n");

    	       	// Added 40°, 60° and 80° closest value
    	       	MISS_SS.WIAC82_Mission.DiveAngle = DiveAngleWIAC82P32Values(MISS_SS.WIAC82_Mission.DiveAngle);
    			DiveAngleOK = 1;
    		}
    	}
    }
    else {
    	MissionSettings = (bFunctionalExchange2MissionSettings *)&WIAC82_SS.Func3D.FuncExch.MissionSettings;
    	// In moving target mode, this parameter is unused and ignored by the weapon
    	if (MissionSettings->MovingTargetActive == 1) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("Moving Target - Dive Angle is IGNORED\n");
			DiveAngleOK = 1;
    	} else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("Validity NOT Set - Mission set INVALID\n");
			MISS_SS.WIAC82_Mission.MissionValid = FALSE;
			MISS_SS.WIAC82_Mission.MissionSetResult = 10;
			DiveAngleOK = 0;
    	}
    }
    return (DiveAngleOK);
}

uint16_t EncodeDiveAngle(float DiveAngle)
{
	uint16_t DiveAngleEncoded;

	DiveAngle = (DiveAngle * FLOAT_2POWER_17)/TWO_PI;

	DiveAngleEncoded = (uint16_t)(DiveAngle);

	return DiveAngleEncoded;
}

char VerifyWIAC82MovingTarget(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {

	bFunctionalExchange2ValidityWord *FuncValid;
	FuncValid = (bFunctionalExchange2ValidityWord *)&WIAC82_SS.Func3D.FuncExch.Validity;

	char MovingTargetOK;
	MovingTargetOK = 1;

 	// Moving target error (weapon is not a SAL weapon)
 	if (WeaponSettings.WeaponConfig.SeekerType != seekerSAL)
 	{
 		if (FuncValid->MovingTargetMode == 1){
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("Moving Target ON in a P32 not config for SAL\n");

 			MISS_SS.WIAC82_Mission.MissionSetResult = 3;
 			MISS_SS.WIAC82_Mission.MissionValid     = FALSE;
 			MovingTargetOK					        = 0;
 		}
 	} else {
 		if (FuncValid->MovingTargetMode == 1){
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("Moving Target ON in a P32 SAL Weapon\n");
 		} else {
 	 		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
 				AUXsprintf("Fixed Target ON in a P32 SAL Weapon\n");
 		}
 	}

	return(MovingTargetOK);
}

char VerifyWIAC82_SALCode(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {
	char SALCodeOK;

    // Verify SAL code
	SALCodeOK = 1;
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying SAL code -");
	if (MISS_SS.WIAC82_Mission.MissionType == seekerSAL) {
    	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    		AUXsprintf("MissionType seekerSAL -");
		/**/
    	if (MISS_SS.WIAC82_Mission.ValidityFlags.SAL_Code == TRUE) {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("Validity Set -");
    		// SAL CODE transmitted in STANAG3733 code
    		if (WIAC82MissionData->FuncExch.SALCode >= 1111 && WIAC82MissionData->FuncExch.SALCode <= 1788) {
    			// SAL Code is a NATO code
            	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
            		AUXsprintf("SAL Code is a NATO code -");
    			if (SAL_GetFrequency(&MISS_SS.WIAC82_Mission.SAL_Frequency, WIAC82MissionData->FuncExch.SALCode) == FALSE) {
                	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
                		AUXsprintf("frequency invalid - default 10Hz / 100ms PRF\n");
                	MISS_SS.WIAC82_Mission.SAL_Frequency = 10.0;
                	MISS_SS.WIAC82_Mission.SAL_Code      = 0x0698;
    			}
    			else {
                	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
                		AUXsprintf("frequency - OK\n");
                	MISS_SS.WIAC82_Mission.SAL_Code      = WIAC82MissionData->FuncExch.SALCode;
    			}
    		} else {
    			// Invalid SAL Code
            	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
            		AUXsprintf("Invalid SAL Code - Mission set INVALID\n");
    			MISS_SS.WIAC82_Mission.MissionValid     = FALSE;
    			MISS_SS.WIAC82_Mission.MissionSetResult = 22;
    			MISS_SS.WIAC82_Mission.SAL_Code         = 0;
    			SALCodeOK = 0;
    		}
    	}
    	else {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("Validity NOT set -Mission set INVALID\n");
    		MISS_SS.WIAC82_Mission.MissionValid     = FALSE;
    		MISS_SS.WIAC82_Mission.MissionSetResult = 20;
    		MISS_SS.WIAC82_Mission.SAL_Code         = 0;
    		SALCodeOK = 0;
   	}
/**/
    	if (AUXCTRL_SS.MAINControls.MPSAL_On){
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn) {
        		AUXsprintf("   Weapon Fitted with MPSAL:\n");
            	if (AUXCTRL_SS.MissionControls.MissionDetail2AuxOn) {
            		AUXsprintf("    Frequency set to %s\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.SAL_Frequency));
            		AUXsprintf("    Accuracy set to %d\n",MISS_SS.WIAC82_Mission.SAL_Accuracy);
            		AUXsprintf("    Jitter set to %d\n",MISS_SS.WIAC82_Mission.SAL_Jitter);
            	}
        	}

        	MPSAL_SeekerSetNewParms(MISS_SS.WIAC82_Mission.SAL_Frequency,MISS_SS.WIAC82_Mission.SAL_Accuracy,MISS_SS.WIAC82_Mission.SAL_Jitter);
    	}
    	else if (AUXCTRL_SS.MAINControls.DSAL_On){
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Weapon Fitted with DSAL\n");

        	SAL_SeekerSetPRI(MISS_SS.WIAC82_Mission.SAL_Frequency);
        	SAL_SeekerSetAccuracy(5000);
        	SAL_SeekerSetJitter(500);
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On){
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn) {
        		AUXsprintf("   Weapon Fitted with QSAL\n");
            	if (AUXCTRL_SS.MissionControls.MissionDetail2AuxOn) {
            		AUXsprintf("    Frequency set to %s\n",AUXsprintfFloat(MISS_SS.WIAC82_Mission.SAL_Frequency));
            		AUXsprintf("    Accuracy set to %d\n",MISS_SS.WIAC82_Mission.SAL_Accuracy);
            		AUXsprintf("    Jitter set to %d\n",MISS_SS.WIAC82_Mission.SAL_Jitter);
            	}
        	}

        	QSAL_SeekerSetPRI(MISS_SS.WIAC82_Mission.SAL_Frequency);
        	QSAL_SeekerSetAccuracy(MISS_SS.WIAC82_Mission.SAL_Accuracy);
        	QSAL_SeekerSetJitter(MISS_SS.WIAC82_Mission.SAL_Jitter);
    	}
    	else {
        	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
        		AUXsprintf("   Weapon Fitted with UNKNOWN SAL\n");
        	MISS_SS.WIAC82_Mission.SAL_Code         = 0;
        	SALCodeOK = 0;
    	}
    }
	else{
		// If the aircraft sends a command to use the seeker in GNSS only,
		// the weapon doesn’t need to receive a valid lasing code.
    	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
    		AUXsprintf("MissionType NOT seekerSAL - OK\n");
	}
	return(SALCodeOK);
}

void DetermineWIAC82_FuzeMode(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {
	bFunctionalExchange2MissionSettings *MissionSettings;

	MISS_SS.Control.SetFuzeDelay = FALSE;
	MISS_SS.Control.SetHOBS_Height = FALSE;

	MissionSettings = (bFunctionalExchange2MissionSettings *)&WIAC82MissionData->FuncExch.MissionSettings;

	// Verify fuze mode
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying Fuze mode -");
	if (MISS_SS.WIAC82_Mission.ValidityFlags.FuzeSettings == TRUE) {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  Validity Flag TRUE -");
		if (MissionSettings->FuzeSetting == WIAC82_FUZE_SETTING_TIME_AFTER_IMPACT) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Time after impact TRUE -");
			MISS_SS.Control.SetFuzeDelay = TRUE;
		}
		else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Time after impact FALSE -");
		}

		if (MissionSettings->FuzeSetting == WIAC82_FUZE_SETTING_AT_ALTITUDE) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Height of Burst TRUE\n");
			MISS_SS.Control.SetHOBS_Height = TRUE;
		}
		else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Height of Burst FALSE\n");
		}
	}
	else {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  Validity Flag FALSE\n");
	}
}

unsigned char FuzeSettingDelayPossibilities(unsigned char crewSettingMs) {

	const unsigned char crewSettingsDelay[DELAYROWSIZE] = {
			100, 60, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0
	};
    const unsigned char fbm21Delays[DELAYROWSIZE]       = {
    		100, 60, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0
    };
    const unsigned char fmu152aDelays[DELAYROWSIZE]     = {
    		90, 60, 60, 45, 45, 35, 35, 25, 25, 15, 15, 5, 0
    };

    for (int i = 0; i < DELAYROWSIZE; i++) {
        if (crewSettingMs == crewSettingsDelay[i]) {
            if (SS_FUZE.Fuze.Type == fuzeFBM_21) {
                return fbm21Delays[i];
            } else {
                return fmu152aDelays[i];
            }
        }
    }

    for (int i = 0; i < DELAYROWSIZE; i++) {
        if (crewSettingMs < crewSettingsDelay[i]) {
        	continue;
        } else {
        	if (i == 0)
        	{
        		return SS_FUZE.Fuze.Type == fuzeFBM_21 ? fbm21Delays[i] : fmu152aDelays[i];

        	} else {
				if (SS_FUZE.Fuze.Type == fuzeFBM_21) {
					if (abs(crewSettingMs - fbm21Delays[i-1]) < abs(crewSettingMs - fbm21Delays[i]))
						return fbm21Delays[i - 1];
					else
						return fbm21Delays[i];
				} else {
					if (abs(crewSettingMs - fmu152aDelays[i-1]) < abs(crewSettingMs - fmu152aDelays[i]))
						return fmu152aDelays[i - 1];
					else
						return fmu152aDelays[i];
				}
        	}
        }
    }

    return 0;
}

unsigned char FuzeSettingHeightPossibilities(unsigned char crewSettingMeters) {

	const unsigned char crewSettingsHeight[HEIGHTROWSIZE] = {
			30, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3
	};
    const unsigned char P32Height[HEIGHTROWSIZE]          = {
    		25, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4
    };

    if (crewSettingMeters < crewSettingsHeight[HEIGHTROWSIZE - 1])
    	return P32Height[HEIGHTROWSIZE - 1];

    for (int i = 0; i < HEIGHTROWSIZE; i++) {
        if (crewSettingMeters == crewSettingsHeight[i]) {
        	return P32Height[i];
        }
    }

    for (int i = 0; i < HEIGHTROWSIZE; i++) {
        if (crewSettingMeters < crewSettingsHeight[i]) {
            continue;
        } else {
        	if (i == 0)
        	{
        		return P32Height[i];

        	} else {
				if (abs(crewSettingMeters - P32Height[i-1]) < abs(crewSettingMeters - P32Height[i]))
					return P32Height[i - 1];
				else
					return P32Height[i];
			}
        	return P32Height[i];
        }
    }

    return 0;

}

void DetermimeWIAC82_FuzeSetting(void) {

	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying Fuze Setting -");

	// SET FUZE DELAT FROM WIAC82
	if (MISS_SS.Control.SetFuzeDelay) {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf(" Fuze to be set with delay -");

		if ((MISS_SS.WIAC82_Mission.ValidityFlags.FuzeDelay == TRUE) &&
				((SS_FUZE.Fuze.Mode == fmDELAY) || (SS_FUZE.Fuze.Mode == fmHEIGHT_AND_DELAY))) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Validity Flag Set AND Mode = fmDELAY OR fmHEIGHT_AND_DELAY - %d [ms]",MISS_SS.WIAC82_Mission.FuzeDelay);

			MISS_SS.WIAC82_Mission.FuzeDelay = FuzeSettingDelayPossibilities(MISS_SS.WIAC82_Mission.FuzeDelay);

			if (SS_FUZE.Fuze.FuzeTimeDelay != MISS_SS.WIAC82_Mission.FuzeDelay) {
				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf("  Current Fuze delay different - UPDATING to P32_Table: %d [ms]\n",MISS_SS.WIAC82_Mission.FuzeDelay);
				// Update fuze delay
				MISS_SS.WIAC82_Mission.UpdatingFuze = TRUE;
				Fuze_SetDelay(MISS_SS.WIAC82_Mission.FuzeDelay, MLAN_ITU_NODE_ID);
			}
			else {
				MISS_SS.WIAC82_Mission.UpdatingFuze = FALSE;
				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf("  Current Fuze delay same - NO UPDATE\n");
			}
		}
		else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Validity Flag NOT Set OR Mode != fmDELAY OR fmHEIGHT_AND_DELAY\n");
		}
	} else if (SS_FUZE.Fuze.Mode == fmNON_PROGRAMMABLE) {

		MISS_SS.WIAC82_Mission.FuzeDelay = SS_FUZE.Fuze.FuzeTimeDelay;
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  Fuze NON Programmable default values\n");

	} else {

		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  does not require delay setting");

		if (AUXCTRL_SS.MAINControls.FuzeCardFittedOn){
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Weapon has Fuze card - set to 0\n");
			MISS_SS.WIAC82_Mission.FuzeDelay = 0;
			Fuze_SetDelay(0, MLAN_ITU_NODE_ID);
		}
	}
}

void DetermineWIAC82_HOBSSetting(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {
	bFunctionalExchange2ValidityWord *MissionValidity;

	MissionValidity = (bFunctionalExchange2ValidityWord *)&WIAC82MissionData->FuncExch.Validity;
	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
		AUXsprintf("  Verifying HOBS Setting -");
	if (WeaponSettings.WeaponConfig.HOBS_Present) {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  Weapon has HOBS - ");
		// Set HOBS height
		if (MISS_SS.Control.SetHOBS_Height) {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  HOBS height to be set - ");
			if (MissionValidity->FuzeSettingDelayHeight) {

				MISS_SS.WIAC82_Mission.FuzingDistance = FuzeSettingHeightPossibilities(WIAC82MissionData->FuncExch.FuzeHeight);

				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf("  Validity flag FuzeSettingDelayHeight SET -set HOBS to: %d [m] P32_Table: %d [m]\n",
							WIAC82MissionData->FuncExch.FuzeHeight,MISS_SS.WIAC82_Mission.FuzingDistance);

				if (SS_HOBS.Status.BurstHeight != MISS_SS.WIAC82_Mission.FuzingDistance) {
					if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
						AUXsprintf("  Burst Height different \n");
					HOBS_SetBurstHeight(MISS_SS.WIAC82_Mission.FuzingDistance);
				}
			}
			else{
				if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
					AUXsprintf("  Validity flag FuzeSettingDelayHeight NOT SET\n");
			}
		}
		else {
			if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
				AUXsprintf("  Height not to be set - HOBS height set to 1\n");
			MISS_SS.WIAC82_Mission.FuzingDistance = 1;
			HOBS_SetBurstHeight(MISS_SS.WIAC82_Mission.FuzingDistance);
		}
	}
	else {
		if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn)
			AUXsprintf("  Weapon does not have HOBS\n");
	}
}

void CopyWIAC82DatatoTelemetry(void) {
	// Copy to RawMission for telemetry purposes

	Weapon.RawMission.PriTgtLatitude      = MISS_SS.WIAC82_Mission.PriTgtLatitude * (2147483648.0 / PI);
	Weapon.RawMission.PriTgtLongitude     = MISS_SS.WIAC82_Mission.PriTgtLongitude * (2147483648.0 / PI);
	Weapon.RawMission.PriTgtAltitude      = MISS_SS.WIAC82_Mission.PriTgtAltitude;
	Weapon.RawMission.AttackHeading       = MISS_SS.WIAC82_Mission.AttackHeading * (2147483648.0 / PI);
	Weapon.RawMission.DiveAngle           = RAD2DEG(MISS_SS.WIAC82_Mission.DiveAngle);
	Weapon.RawMission.ImpactSpeed         = MISS_SS.WIAC82_Mission.ImpactSpeed;
	Weapon.RawMission.GNSS_Usage          = MISS_SS.WIAC82_Mission.GNSS_Usage;
	Weapon.RawMission.AttackHeadingValid  = MISS_SS.WIAC82_Mission.AttackHeadingValid;
	//Weapon.RawMission.SecondarySwitch     = MISS_SS.WIAC82_Mission.SecondarySwitch;
	//Weapon.RawMission.SecTgtLatitude      = MISS_SS.WIAC82_Mission.SecTgtLatitude;
	//Weapon.RawMission.SecTgtLongitude     = MISS_SS.WIAC82_Mission.SecTgtLongitude;
	//Weapon.RawMission.SecTgtAltitude      = MISS_SS.WIAC82_Mission.SecTgtAltitude;
	Weapon.RawMission.SAL_Code            = MISS_SS.WIAC82_Mission.SAL_Code;
	Weapon.RawMission.SAL_Activation      = MISS_SS.WIAC82_Mission.SAL_Activation;
	Weapon.RawMission.SAL_Accuracy        = MISS_SS.WIAC82_Mission.SAL_Accuracy;
	Weapon.RawMission.SAL_Jitter          = MISS_SS.WIAC82_Mission.SAL_Jitter;
	Weapon.RawMission.IIR_ImageNo         = MISS_SS.WIAC82_Mission.IIR_ImageNo;
	//Weapon.RawMission.IIR_ImageOffsetX    = MISS_SS.WIAC82_Mission.IIR_ImageOffsetX;
	//Weapon.RawMission.IIR_ImageOffsetY    = MISS_SS.WIAC82_Mission.IIR_ImageOffsetY;
	Weapon.RawMission.IIR_ImageCRC        = MISS_SS.WIAC82_Mission.IIR_ImageCRC;
	Weapon.RawMission.WeaponType          = MISS_SS.WIAC82_Mission.WeaponType;
	Weapon.RawMission.MissionType         = MISS_SS.WIAC82_Mission.MissionType;
	Weapon.RawMission.CRC                 = MISS_SS.WIAC82_Mission.CRC;
}

void CheckValidityOfMission(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData) {

	MISS_SS.Control.WeaponTypeOK  		= VerifyWIAC82WeaponType();
 	MISS_SS.Control.MissionTypeOK 		= VerifyWIAC82MissionType();
 	MISS_SS.Control.TargetPositionOK 	= VerifyWIAC82TargetPosition();
 	MISS_SS.Control.DiveAngleOK 		= VerifyWIAC82DiveAngle(WIAC82MissionData);
 	MISS_SS.Control.SALCodeOK 			= VerifyWIAC82_SALCode(WIAC82MissionData);
 	MISS_SS.Control.MovingTargetOK      = VerifyWIAC82MovingTarget(WIAC82MissionData);
 	DetermineWIAC82_FuzeMode(WIAC82MissionData);
 	DetermimeWIAC82_FuzeSetting();
 	DetermineWIAC82_HOBSSetting(WIAC82MissionData);

 	if (MISS_SS.Control.WeaponTypeOK && MISS_SS.Control.MissionTypeOK &&
 			MISS_SS.Control.TargetPositionOK && MISS_SS.Control.MovingTargetOK &&
			MISS_SS.Control.SALCodeOK) {
 		MISS_SS.WIAC82_Mission.MissionValid = TRUE;
 	} else {
 		// Dive Angle and Fuze/Hobs are not mandatory for determining Mission Invalid
 		MISS_SS.WIAC82_Mission.MissionValid = FALSE;
 	}

 	if (AUXCTRL_SS.MissionControls.MissionAuxOn || AUXCTRL_SS.WIAC82Controls.MissionData)
 		AUXsprintf("%u:Checking overall validity of MISSION data -",GetTimeUs());

 	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn || AUXCTRL_SS.WIAC82Controls.MissionData) {
 		AUXsprintf("  WeaponTypeOK          = %d\n",MISS_SS.Control.WeaponTypeOK);
 		AUXsprintf("  MissionTypeOK         = %d\n",MISS_SS.Control.MissionTypeOK);
 		AUXsprintf("  TargetPositionOK      = %d\n",MISS_SS.Control.TargetPositionOK);
 		AUXsprintf("  MovingTargetOK        = %d\n",MISS_SS.Control.MovingTargetOK);
 		AUXsprintf("  DiveAngleOK           = %d\n",MISS_SS.Control.DiveAngleOK);
 		AUXsprintf("  SALCodeOK             = %d\n",MISS_SS.Control.SALCodeOK);
 	}
}

void UpdateGNSS_Usage(void){
	// Update GNSS usage
 	if (AUXCTRL_SS.MissionControls.MissionAuxOn)
 		AUXsprintf("%u:Updating GNSS usage 0x%x\n",GetTimeUs(),Weapon.Mission.GNSS_Usage.Byte);

 	if (AUXCTRL_SS.MissionControls.MissionDetail1AuxOn) {
 	    if (Weapon.Mission.GNSS_Usage.GPS_NotUsed)
 	    	AUXsprintf("  GPS OFF\n");
 	    else
 	    	AUXsprintf("  GPS ON\n");
 	    if (Weapon.Mission.GNSS_Usage.GlonassNotUsed)
 	    	AUXsprintf("  GLONASS OFF\n");
 	    else
 	    	AUXsprintf("  GLONASS ON\n");
 	}

	if (Weapon.GNSS_Usage.Byte != Weapon.Mission.GNSS_Usage.Byte) {
		Weapon.GNSS_Usage = Weapon.Mission.GNSS_Usage;
		Weapon.ConfigureUBLOX_ReceiverCnt = 0;
		Weapon.ConfigureUBLOX_Receiver = TRUE;
	}
}

/*

    // Verify LAR launch conditions
    if (new_mission.ValidityFlags.LAR_Altitude &&
        new_mission.ValidityFlags.LAR_Track    &&
        new_mission.ValidityFlags.LAR_Mach       ) {
      // Convert the Altitude in EGM-96 to WGS84
      if (GetEGM96GeoidHeight(&altitude, Weapon.Latitude, Weapon.Longitude)) {
        new_mission.LAR_Altitude = new_mission.LAR_Altitude + altitude;
      }

      new_mission.LAR_LaunchConditionsValid = TRUE;
    }
    else {
      new_mission.LAR_LaunchConditionsValid = FALSE;
    }

    // Verify LAR wind
    if (new_mission.ValidityFlags.LAR_WindDirection &&
        new_mission.ValidityFlags.LAR_WindSpeed        ) {
      new_mission.LAR_WindValid = TRUE;
    }
    else {
      new_mission.LAR_WindValid = FALSE;
    }

    // If mission is invalid, set target valid, LAR launch conditions and LAR wind to false
    if (!new_mission.MissionValid) {
      new_mission.TargetValid = FALSE;
      new_mission.LAR_LaunchConditionsValid = FALSE;
      new_mission.LAR_WindValid = FALSE;
    }
  }
  else {
    // CRC failed. Zero all mission parameters
    memset(&new_mission, 0, sizeof(sMission));
    memset(RawMissionData, 0, sizeof(sAircraftRawMission));

    new_mission.MissionValid = FALSE;
    new_mission.TargetValid  = FALSE;
    new_mission.LAR_LaunchConditionsValid = FALSE;
    new_mission.LAR_WindValid = FALSE;
    new_mission.MissionSetResult = 1;
  }

  Weapon.Mission = new_mission;
  Weapon.AircraftRawMission = *RawMissionData;
  Weapon.Flags.Bits.MissionOk = new_mission.MissionValid;
  Weapon.Flags.Bits.MissionMode = (new_mission.TargetValid == TRUE) ? mmPROGRAMMED : mmOPPORTUNITY;

 */


