/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Guidance.c>
 *
 *  Description:
 *  ------------
 *  Guidance of the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - GuidanceInit           : Initializes the guidance system and related resources.
 *  - GuidanceSetMission     : Configures the guidance system with mission parameters.
 *  - RunAutoPilot           : Executes the autopilot control loop.
 *  - Guidance100Hz          : Performs guidance calculations and updates at 100 Hz.
 *  - Guidance10Hz           : Performs guidance updates and checks at 10 Hz.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include "rt_atan2d_snf.h"
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "TypeDefines.h"
#include "CRC.h"
#include "Flash.h"
#include "Timer.h"
#include "Wind.h"
#include "Nav.h"
#include "IMU.h"
#include "Servos.h"
#include "MCP.h"
#include "WIAC82_Comms.h"
#include "FIFO.h"
#include "Weapon.h"
#include "UART.h"
#include "MPSAL_Comms.h"
#include "Fuze.h"
#include "WeaponSettings.h"
#include "DataControl_10Hz.h"
#include "DataControl_10Hz_Function.h"
//#include "HOBS.h"
#include "Fuze.h"
#include "AircraftInterface.h"
#include "Mission.h"
#include "Guidance.h"
#include "AuxControls.h"
#include "SAL_Seeker.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"

unsigned int Guidance10HzCnt;
T_GUID_Autop_Tel300Hz AutoPilot300HzTelemetry;
T_GUID_Autop_Tel20Hz AutoPilot20HzTelemetry;
T_Weapon_MCP_LAR LAR;
T_GUID_GuidCtrlFlags GuidCtrl;
T_GUID_Guid_Out_IF1003 GuidanceOut;
T_GUID_Guid_Tel100Hz GuidTel100Hz;
T_GUID_Guid_Tel20HzA GuidTel20HzA;
T_GUID_Guid_Tel20HzB GuidTel20HzB;
T_GUID_TTKF_Tel100Hz TTKF_Telemetry;
T_Weapon_MCP_Out_IF103 ServoCmd;


static unsigned char RunUutPilotCountdown;
static unsigned char GuidanceCountdown;
static unsigned char Guidane10HzBusy;
static unsigned char GuidancePrevWingsOpen;
static unsigned char GuidancePrevWingsRelease;
static unsigned int GuidancePrevious100HzCnt;
static T_NAV_InertialMeasurementUnitAutopilot GuidanceIMU_Input;
static T_GUID_Guid2DATA GuidanceToDataCtrl;
static T_GUID_MissionPlanning MissionData;
static T_GUID_TTKF_Out_IF1002 TTKF_Out;
static T_Weapon_SALSeeker_IF102 TTKF_SeekerData;
static T_GUID_WeaponConfig GuidanceWeaponConfig;
static T_GUID_TestConfig GuidanceTestConfig;
static T_Weapon_MCP_DATA MCP_Data;
static T_Weapon_MCP_DATA Guidane10HzMCP_Data;
static T_GUID_Discretes GuidanceDiscretes;
static T_Weapon_MCP_LAR Guidane10Hz_LAR;


void GuidanceInit(void)
{
  T_Weapon_MCP_LAR lar;


  Guidane10HzBusy = FALSE;
  GuidancePrevWingsOpen = FALSE;
  GuidancePrevWingsRelease = FALSE;
  RunUutPilotCountdown = 10;
  GuidanceCountdown = 10;
  GuidancePrevious100HzCnt = 0;
  Guidance10HzCnt = 0;

  memset((void *) &GuidanceIMU_Input, 0, sizeof(T_NAV_InertialMeasurementUnitAutopilot));
  memset(&LAR, 0, sizeof(T_Weapon_MCP_LAR));
  memset(&Guidane10Hz_LAR, 0, sizeof(T_Weapon_MCP_LAR));
  memset(&GuidCtrl, 0, sizeof(T_GUID_GuidCtrlFlags));
  memset(&GuidanceToDataCtrl, 0, sizeof(T_GUID_Guid2DATA));

  GuidCtrl.PstaticValid = 1;
  GuidCtrl.Umbilical = 1;

  GuidanceWeaponConfig.BombType = WeaponSettings.WeaponConfig.BombType;
  GuidanceWeaponConfig.SeekerType = WeaponSettings.WeaponConfig.SeekerType;
  GuidanceWeaponConfig.WeaponType = WeaponSettings.WeaponConfig.WeaponType;
  GuidanceWeaponConfig.WingsPresent = FALSE;
  GuidanceWeaponConfig.BatActMethod = WeaponSettings.WeaponConfig.BatteryActivation;
  GuidanceWeaponConfig.DTACPresent = WeaponSettings.WeaponConfig.DTAC_Present;
  GuidanceWeaponConfig.RTACPresent = FALSE;
  GuidanceWeaponConfig.HOBSPresent = WeaponSettings.WeaponConfig.HOBS_Present;

  GuidanceTestConfig.GC_Mode = WeaponSettings.WeaponConfig.GuidanceAndControlMode;
  GuidanceTestConfig.LARradius = 0;
  GuidanceTestConfig.LARX = 0;
  GuidanceTestConfig.LARY = 0;
  GuidanceTestConfig.LARtimeimp = 0;
  GuidanceTestConfig.LARalt = 0;
  GuidanceTestConfig.WC_Log = 0;

  GuidanceDiscretes.BatteryOn = Weapon.Discretes.Bits.BatterySwitchedOn;
  GuidanceDiscretes.WingsOpenRcvd = FALSE;
  GuidanceDiscretes.WingsRelRcvd = FALSE;
  GuidanceDiscretes.umbilical = Weapon.Discretes.Bits.LaunchLinkPresent;

  DataControl_10Hz_initialize();
  Autopilot_300Hz_initialize();
  Guidance_100Hz_initialize();
  TTKF_100Hz_initialize();

  DataControl_10Hz_main(&GuidanceToDataCtrl, &GuidanceWeaponConfig, &GuidanceTestConfig, &GuidanceDiscretes, FALSE, &MCP_Data, &lar);

  Guidane10HzMCP_Data = MCP_Data;

  NavWeaponLeverArm.X = MCP_Data.toNav.WeaponProp.leverarmX;
  NavWeaponLeverArm.Y = MCP_Data.toNav.WeaponProp.leverarmY;
  NavWeaponLeverArm.Z = MCP_Data.toNav.WeaponProp.leverarmZ;
}


void GuidanceSetMission(sMission *WeaponMission)
{
  MissionData.PriTgtLatitude = WeaponMission->PriTgtLatitude;
  MissionData.PriTgtLongitude = WeaponMission->PriTgtLongitude;
  MissionData.PriTgtAltitude = WeaponMission->PriTgtAltitude;
  MissionData.DiveAngle = WeaponMission->DiveAngle;
  MissionData.AttackHeading = WeaponMission->AttackHeading;
  MissionData.AttackHeadingValid = WeaponMission->AttackHeadingValid;
  MissionData.GNSS_Usage = WeaponMission->GNSS_Usage.Byte;
  MissionData.ImpactSpeed = WeaponMission->ImpactSpeed;
  MissionData.MissionType = WeaponMission->MissionType;
  MissionData.WeaponType = weaponP3; //WeaponMission->WeaponType;
  MissionData.SAL_Frequency = WeaponMission->SAL_Frequency;
}


void RunAutoPilot(void)
{
  if (!Guidane10HzBusy) {
    MCP_Data = Guidane10HzMCP_Data;
    LAR = Guidane10Hz_LAR;
  }

  //if (Weapon.Status.Bits.NAV_Mode > 10) {
  if (RunUutPilotCountdown == 0) {
    if (IMU.DataValid) {
      GuidanceIMU_Input.ab.x = IMU.InertialData.Acceleration[0];
      GuidanceIMU_Input.ab.y = IMU.InertialData.Acceleration[1];
      GuidanceIMU_Input.ab.z = IMU.InertialData.Acceleration[2];
      GuidanceIMU_Input.wb.x = IMU.InertialData.AngularRate[0];
      GuidanceIMU_Input.wb.y = IMU.InertialData.AngularRate[1];
      GuidanceIMU_Input.wb.z = IMU.InertialData.AngularRate[2];
    }

    Autopilot_300Hz_main((T_NAV_InertialMeasurementUnitAutopilot *) &GuidanceIMU_Input, (T_NAV_NavigationState *) &Nav100HzOutput.NAV_NavigationState,
          (T_GUID_Guid_Out_IF1003 *) &GuidanceOut, &MCP_Data.toAutop, &ServoCmd, &AutoPilot300HzTelemetry, &AutoPilot20HzTelemetry);

    // Command servos if ok to do so
    ServoCmd.fin1 = (Weapon.ActiveServos > 0) ? ServoCmd.fin1 : 0;
    ServoCmd.fin2 = (Weapon.ActiveServos > 1) ? ServoCmd.fin2 : 0;
    ServoCmd.fin3 = (Weapon.ActiveServos > 2) ? ServoCmd.fin3 : 0;
    ServoCmd.fin4 = (Weapon.ActiveServos > 3) ? ServoCmd.fin4 : 0;

    ServosUpdatePositions(ServoCmd.fin1, ServoCmd.fin2, ServoCmd.fin3, ServoCmd.fin4);
  }
  //else if (Servo.DoManualCtrlId == SERVO_MANUAL_CTRL_ID) {
  //  ServosUpdatePositions(ServoCmd.fin1, ServoCmd.fin2, ServoCmd.fin3, ServoCmd.fin4);
  //}
}


void Guidance100Hz(void)
{
  double weapon_time;
  T_GUID_FlightConditions flight_conditions;
  T_GUID_WindParams wind;

  double DCM_s2b[9];
  double SalMountingAngle;
  double Vec_seeker[3];
  double Vec_body[3];
  double SAL_TargetYaw;
  double SAL_TargetPitch;
  double SpotAzimuth;
  double SpotElevation;
  double PI_value;

  PI_value = 3.14159265358979323846;
  weapon_time = ((double) GetSystemTime())* 1E-6;//(double) Weapon.SystemTime * 1E-6;

  // if (Weapon.Status.Bits.NAV_Mode >= 30) {
  if (Weapon.Status.Bits.NAV_Mode >= 20) {
    if (GuidanceCountdown > 0) {
      GuidanceCountdown--;
    }
  }
//#define _SAL_MOUNT_ANGLE_TRANSFORM_TEST
#ifdef _SAL_MOUNT_ANGLE_TRANSFORM_TEST
  GuidanceCountdown=0;
#endif
  if (GuidanceCountdown == 0) {
    flight_conditions.Temp = MCP.Temperature;
    flight_conditions.Pdyn = 0;
    flight_conditions.Psta = 0;

    GuidanceWeaponConfig.BombType = WeaponSettings.WeaponConfig.BombType;
    GuidanceWeaponConfig.SeekerType = WeaponSettings.WeaponConfig.SeekerType;
    GuidanceWeaponConfig.WeaponType = WeaponSettings.WeaponConfig.WeaponType;
    GuidanceWeaponConfig.WingsPresent = FALSE;
    GuidanceWeaponConfig.BatActMethod = 0;
    GuidanceWeaponConfig.DTACPresent = WeaponSettings.WeaponConfig.DTAC_Present;
    GuidanceWeaponConfig.RTACPresent = FALSE;
    GuidanceWeaponConfig.HOBSPresent = WeaponSettings.WeaponConfig.HOBS_Present;

    wind.WindEntryCnt = Weapon.Wind.WindEntryCnt;
    memcpy(wind.WindEntries, (sWindEntry *) Weapon.Wind.WindEntries, sizeof(sWindEntry) * Weapon.Wind.WindEntryCnt);

    GuidanceDiscretes.BatteryOn = Weapon.Discretes.Bits.BatterySwitchedOn;
    GuidanceDiscretes.WingsOpenRcvd = FALSE;
    GuidanceDiscretes.WingsRelRcvd = FALSE;
    GuidanceDiscretes.umbilical = Weapon.Discretes.Bits.LaunchLinkPresent;

    if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
    {
    	if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	{


            // SalMountingAngle convertion to rad
            SalMountingAngle = -7.0 * (PI_value /180.0);

#ifdef _SAL_MOUNT_ANGLE_TRANSFORM_TEST
            //    						  INPUT	OUTPUT
            //    	SpotElevation [rad]	-0.2793	-0.3895
            //    	SpotAzimuth [rad]	0.4363	-0.4546
            //
            //    	SpotElevation [rad]	0.1047	-0.0115
            //    	SpotAzimuth [rad]	-0.3142	0.3124
            //
            //    	SpotElevation [rad]	-0.5236	-0.6424
            //    	SpotAzimuth [rad]	-0.2269	0.2458
            //
            //    	SpotElevation [rad]	0.0873	-0.0207
            //    	SpotAzimuth [rad]	0.4887	-0.4868
            static int idx;
            static double SpotElevationTestInput[]={-0.2793, 0.1047,-0.5236,0.0873};
            static double SpotAzimuthTestInput[]={   0.4363,-0.3142,-0.2269,0.4887};
            static double SpotElevationTestOutputExpected[]={-0.3895,-0.0115,-0.6424,-0.0207};
            static double SpotAzimuthTestOutputExpected[]={  -0.4546, 0.3124, 0.2458,-0.4868};
            static double SpotElevationTestOutput[4];
            static double SpotAzimuthTestOutput[4];

            SpotAzimuth   = -1.0 * SpotAzimuthTestInput[idx];
            SpotElevation = SpotElevationTestInput[idx];

#else
            // Capuring the SAL seeker measurement in radians (corrected for sign convention)
             SpotAzimuth   = -1.0 * SAL_SS.GuidanceData.TargetYaw;
             SpotElevation = SAL_SS.GuidanceData.TargetPitch;
#endif

            // Loading the DCM_seeker_to_body  matrix
             DCM_s2b[0] = cos(SalMountingAngle);
             DCM_s2b[1] = 0.0;
             DCM_s2b[2] = -sin(SalMountingAngle);
             DCM_s2b[3] = 0.0;
             DCM_s2b[4] = 1.0;
             DCM_s2b[5] = 0.0;
             DCM_s2b[6] = sin(SalMountingAngle);
             DCM_s2b[7] = 0.0;
             DCM_s2b[8] = cos(SalMountingAngle);


            // LOS vector in seeker axis
             Vec_seeker[0] = cos(SpotElevation) * cos(SpotAzimuth);
             Vec_seeker[1] = cos(SpotElevation) * sin(SpotAzimuth);
             Vec_seeker[2] = -sin(SpotElevation);

            // LOS vector in body axis
             Vec_body[0] = DCM_s2b[0] * Vec_seeker[0] + DCM_s2b[3] * Vec_seeker[1] + DCM_s2b[6] * Vec_seeker[2];
             Vec_body[1] = DCM_s2b[1] * Vec_seeker[0] + DCM_s2b[4] * Vec_seeker[1] + DCM_s2b[7] * Vec_seeker[2];
             Vec_body[2] = DCM_s2b[2] * Vec_seeker[0] + DCM_s2b[5] * Vec_seeker[1] + DCM_s2b[8] * Vec_seeker[2];

            // Seeker Measurement in body axis
            // SAL_TargetYaw = atan2(Vec_body[1], Vec_body[0]);        // function to compute Arctan from 'math.h'
            SAL_TargetYaw =  rt_atan2d_snf(Vec_body[1], Vec_body[0]);  // Better function to compute Arctan, has more protection checks
            SAL_TargetPitch = asin(-Vec_body[2]);
#ifdef _SAL_MOUNT_ANGLE_TRANSFORM_TEST

            SpotElevationTestOutput[idx]=SAL_TargetPitch;
            SpotAzimuthTestOutput[idx++]=SAL_TargetYaw;

            if(idx==4)
            {
            	int test=0;
            }
#endif

    		TTKF_SeekerData.TrackStatus = (real_T)(SAL_SS.Detection.DetectionStatus);
    		TTKF_SeekerData.AnglesValid = (real_T)(SAL_SS.Detection.DetectionStatus);
    		TTKF_SeekerData.Detection = (real_T)(SAL_SS.Detection.DetectionStatus);
    		TTKF_SeekerData.TargetYaw = (real_T)SAL_TargetYaw;
    		TTKF_SeekerData.TargetPitch = (real_T)SAL_TargetPitch;
    		TTKF_SeekerData.MeasureTime = (real_T)(SAL_SS.GuidanceData.DetectionTime);
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On)
    	{
    		TTKF_SeekerData.TrackStatus = (real_T)(QSAL_SS.SharedData.Status.Bits.Tracking);
    		TTKF_SeekerData.AnglesValid = (real_T)(QSAL_SS.SharedData.Status.Bits.AnglesValid);
    		TTKF_SeekerData.Detection = (real_T)(QSAL_SS.SharedData.Status.Bits.TargetPulse);
    		TTKF_SeekerData.TargetYaw = (real_T)(QSAL_SS.SharedData.TargetYaw);
    		TTKF_SeekerData.TargetPitch = (real_T)(QSAL_SS.SharedData.TargetPitch);
    		TTKF_SeekerData.MeasureTime = (real_T)(QSAL_SS.SharedData.DetectionTime);
    	}
    	else if (AUXCTRL_SS.MAINControls.DSAL_On)
		{
    	    TTKF_SeekerData.TrackStatus = SAL_Seeker.Status.Bits.TrackStatus;
    	    TTKF_SeekerData.AnglesValid = SAL_Seeker.Status.Bits.AnglesValid;
    	    TTKF_SeekerData.Detection = SAL_Seeker.Status.Bits.Detection;
    	    TTKF_SeekerData.TargetYaw = SAL_Seeker.TargetYaw;
    	    TTKF_SeekerData.TargetPitch = SAL_Seeker.TargetPitch;
    	    TTKF_SeekerData.MeasureTime = SAL_Seeker.DetectionTime;
		}
    	else
    	{
    		TTKF_SeekerData.AnglesValid = FALSE;
    		TTKF_SeekerData.Detection = FALSE;
    	}
    }
    TTKF_100Hz_main(weapon_time, &GuidanceDiscretes, &TTKF_SeekerData, &NavINS_Data, Weapon.Status.Bits.NAV_Mode,
          &Nav100HzOutput.NAV_NavigationState, &MissionData, &MCP_Data.toTTKF, &TTKF_Out, &TTKF_Telemetry);

    Guidance_100Hz_main(weapon_time, &GuidanceDiscretes, &TTKF_Out, Weapon.Status.Bits.NAV_Mode, &Nav100HzOutput.NAV_NavigationState,
          &NavINS_Data, &MissionData, &wind, &flight_conditions, &MCP_Data.toGuid, &GuidanceWeaponConfig, &GuidanceTestConfig,
          &GuidanceOut, &GuidCtrl, &GuidanceToDataCtrl, &GuidTel100Hz, &GuidTel20HzA, &GuidTel20HzB);

    Weapon.Flags.Bits.WeaponBunt = GuidCtrl.SwitchforBunt;

    // Activate HOBS if required
    if (WeaponSettings.WeaponConfig.HOBS_Present && GuidCtrl.SADFuze) {
//    		HOBS_Activate();
    }

    if (RunUutPilotCountdown > 0) {
      RunUutPilotCountdown--;
    }
  }
}


void Guidance10Hz(void)
{
  unsigned char do_guidance_10hz = FALSE;
  unsigned char calculate_lar = FALSE;
  T_GUID_Guid2DATA guidance_to_data;
  T_GUID_WeaponConfig weapon_config;
  T_GUID_TestConfig test_config;
  T_GUID_Discretes discretes;


  if (!Weapon.Status.Bits.WeaponLaunched) {
    // Always execute the guidance 10Hz before launch (for LAR)
    if ((Weapon.Weapon100HzCnt - GuidancePrevious100HzCnt) >= 10) {
      GuidancePrevious100HzCnt = Weapon.Weapon100HzCnt;
      calculate_lar = FALSE; //!WeaponSettings.WeaponConfig.DTAC_Present;
      do_guidance_10hz = TRUE;
    }
  }

  if (do_guidance_10hz) {
    Guidane10HzBusy = TRUE;

    Guidance10HzCnt++;

    guidance_to_data = GuidanceToDataCtrl;
    weapon_config = GuidanceWeaponConfig;
    test_config = GuidanceTestConfig;
    discretes = GuidanceDiscretes;

    DataControl_10Hz_main(&guidance_to_data, &weapon_config, &test_config, &discretes, calculate_lar, &Guidane10HzMCP_Data, &Guidane10Hz_LAR);

    if (calculate_lar) {
      LAR = Guidane10Hz_LAR;

      Weapon.LAR.Valid = TRUE;
      Weapon.LAR.LAR_Rad = Guidane10Hz_LAR.radius;
      Weapon.LAR.LAR_Rad_Excl = Guidane10Hz_LAR.exclrad;
      Weapon.LAR.DistToLAR = Guidane10Hz_LAR.Rge2LAR;
      Weapon.LAR.DistToTGT = Guidane10Hz_LAR.Rge2Tgt;
    }

    Guidane10HzBusy = FALSE;
  }
}
