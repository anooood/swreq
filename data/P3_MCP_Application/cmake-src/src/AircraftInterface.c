/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <AircraftInterface.c>  - DTAC
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control between the Dassault Mirage aircraft system and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - AircraftInit:                Initialization
 *  - AircraftSendSBIT_Request:    SBIT Request to the Aircraft
 *  - AircraftSendHealthAndStatus: Health and Status of the P3 Weapon to the Aircraft
 *  - AircraftSend:                Send messages to DTAC
 *  - AircraftOnSBIT:              Handles received aircraft SBIT Data
 *  - AircraftOnStatus:            Handles received aircraft status data
 *  - AircraftOnMission:           Handles received aircraft Mission data
 *  - AircraftOnWind:              Handles received aircraft wind data
 *  - AircraftOnTransferAlignment: Handles received aircraft transfer alignment data
 *  - AircraftOnConfigRequest:     Handles received aircraft config request
 *  - AircraftOnCommand:           Handles received aircraft command data
 *  - AircraftHandleRxData:        Handles received aircraft data
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
#include "Timer.h"
#include "CRC.h"
#include "Wind.h"
#include "EGM96.h"
#include "Util.h"
#include "BIT.h"
#include "WIAC82_Comms.h"
#include "FIFO.h"
#include "UART.h"
#include "MLAN.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WeaponSettings.h"
#include "Nav.h"
#include "IF.h"
#include "Fuze.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "Weapon.h"
#include "MCP.h"
#include "AircraftInterface.h"

#define AIRCRAFT_MSG_ID_SBIT_REQ           0x00
#define AIRCRAFT_MSG_ID_SBIT               0x01
#define AIRCRAFT_MSG_ID_CLEAR_CBIT         0x04
#define AIRCRAFT_MSG_ID_WPN_CONFIQ_REQ     0x12
#define AIRCRAFT_MSG_ID_COMMAND            0x13
#define AIRCRAFT_MSG_ID_HEALTH_DATA        0x80
#define AIRCRAFT_MSG_ID_STATUS             0x81
#define AIRCRAFT_MSG_ID_MISSION            0x82
#define AIRCRAFT_MSG_ID_WIND               0x83
#define AIRCRAFT_MSG_ID_TRANSFER_ALIGNMENT 0x84
#define AIRCRAFT_MSG_ID_WPN_CONFIG         0x88

#define AIRCRAFT_MAX_WIND_ENTRIES          8

#define AIRCRAFT_CLEAR_CBIT_ID             0xFFFF


typedef struct
{
  sMLAN_Header Header;
  sAircraftHealthStatus Status;
} sAircraftHealthStatusMsg;

typedef struct
{
  sMLAN_Header Header;
  sAircraftWeaponConfig WeaponConfig;
} sAircraftWeaponConfigMsg;

typedef union
{
  unsigned short Words[17];
  struct {
    unsigned short ValidityFlags;
    unsigned short TgtLatitudeHi;
    unsigned short TgtLatitudeLo;
    unsigned short TgtLongitudeHi;
    unsigned short TgtLongitudeLo;
    unsigned short TgtAltitudeHi;
    unsigned short TgtAltitudeLo;
    unsigned short DiveAngle;
    unsigned short ImpactHeading;
    unsigned short ImpactSpeed;
    unsigned short SAL_PeriodHi;
    unsigned short SAL_PeriodLo;
    unsigned short IIR_ImageNo;
    unsigned short IIR_ImageCRC;
    unsigned short FuzeSetting;
    unsigned short FuzeHeight;
    unsigned short WeaponConfig;
  };
} sAircraftRawWords;

typedef struct
{
  sMLAN_Header Header;
  union
  {
    sAircraftSBIT SBIT;
    sAircraftStatus Status;
    sAircraftTransferAlignment TransferAlignment;
    sAircraftRawMission Mission;
    sWind Wind;
    sAircraftCommand Command;
  } Payload;
} sAircraftRxMsg;

typedef struct
{
  unsigned int Cnt;
  unsigned short MissionCRC;
  unsigned short AircraftCRC;
} sHealthAndStatusLog;


sAircraft Aircraft;
sDBAS_StateSpace DBAS_SS;

static unsigned char AircraftConnected;
static unsigned char AircraftSBIT_Completed;
static sMLAN_Header AircraftSBIT_RequestMsg;
static sAircraftHealthStatusMsg AircraftHealthStatusMsg;
static sAircraftWeaponConfigMsg AircraftWeaponConfig;
static sAircraftRxMsg AircraftRxMsg;
static sAircraftRawWords AircraftRawWords;
static TIME AircraftTA_RxTimestamp;
static TIME AircraftTxTimestamp;

void AircraftInit(char *AuxStr)
{
  AircraftConnected = FALSE;
  AircraftSBIT_Completed = FALSE;
  AircraftTA_RxTimestamp = 0;
  AircraftTxTimestamp = 0;

  DBAS_SS.DBASWaitForRxMsg = FALSE;

  memset(&Aircraft, 0, sizeof(sAircraft));
  memset(&AircraftRawWords, 0, sizeof(sAircraftRawWords));

  AircraftSBIT_RequestMsg.SourceId = 0xFF;
  AircraftSBIT_RequestMsg.DestinationId = 0xF6;
  AircraftSBIT_RequestMsg.Opcode = AIRCRAFT_MSG_ID_SBIT_REQ;
  AircraftSBIT_RequestMsg.DataLength = 0;
  AircraftSBIT_RequestMsg.SequenceNo = 1;
  AircraftSBIT_RequestMsg.SequenceCnt = 1;
  AircraftSBIT_RequestMsg.FrameCnt = 0;

  AircraftHealthStatusMsg.Header.SourceId = 0xFF;
  AircraftHealthStatusMsg.Header.DestinationId = 0xF6;
  AircraftHealthStatusMsg.Header.Opcode = AIRCRAFT_MSG_ID_HEALTH_DATA;
  AircraftHealthStatusMsg.Header.DataLength = sizeof(sAircraftHealthStatusMsg) - MLAN_HEADER_SIZE;
  AircraftHealthStatusMsg.Header.SequenceNo = 1;
  AircraftHealthStatusMsg.Header.SequenceCnt = 1;
  AircraftHealthStatusMsg.Header.FrameCnt = 0;

  AircraftWeaponConfig.Header.SourceId = 0xFF;
  AircraftWeaponConfig.Header.DestinationId = 0xF6;
  AircraftWeaponConfig.Header.Opcode = AIRCRAFT_MSG_ID_WPN_CONFIG;
  AircraftWeaponConfig.Header.DataLength = sizeof(sAircraftWeaponConfigMsg) - MLAN_HEADER_SIZE;
  AircraftWeaponConfig.Header.SequenceNo = 1;
  AircraftWeaponConfig.Header.SequenceCnt = 1;
  AircraftWeaponConfig.Header.FrameCnt = 0;

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.DBASControls.AuxOn				     = 1;
	  AUXCTRL_SS.DBASControls.SchedulerAuxOn     	 = 0;
	  AUXCTRL_SS.DBASControls.RxAuxOn			     = 0;
	  AUXCTRL_SS.DBASControls.RxDetailAuxOn		     = 0;
	  AUXCTRL_SS.DBASControls.RxDetailStatusAux      = 0;
	  AUXCTRL_SS.DBASControls.RxDetailMissionAux     = 0;
	  AUXCTRL_SS.DBASControls.RxDetailWindAux        = 0;
	  AUXCTRL_SS.DBASControls.RxDetailtTransAlignAux = 0;
	  AUXCTRL_SS.DBASControls.TxAuxOn			     = 0;
	  AUXCTRL_SS.DBASControls.TxDetailAuxOn		     = 0;
	  AUXCTRL_SS.DBASControls.TxDetailLARAux         = 0;
	  AUXCTRL_SS.DBASControls.TxDetailFlagsAux       = 0;
	  AUXCTRL_SS.DBASControls.TxDetailMissionAux     = 0;
	  AUXCTRL_SS.DBASControls.TxDetailLARInRangeAux  = 0;
	  AUXCTRL_SS.DBASControls.TxDetailWeaponCfgAux   = 0;
	  AUXCTRL_SS.DBASControls.ErrDetectOn		     = 1;
  }

  if (AUXCTRL_SS.DBASControls.AuxOn) {
	  sprintf(AuxStr,"%s%u: AIRCRAFT DBAS Interface Initialized\n", AuxStr,GetTimeUs());
  }
}

void AircraftSendSBIT_Request(void)
{
  Aircraft.SBIT_ReqTxCnt++;
  Aircraft.SBIT_ReqTxTimestamp = GetSystemTime();

  DBAS_SS.DBASWaitForRxMsg = TRUE;

  MLAN_Send(MLAN_DTAC_NODE_ID, (unsigned char *) &AircraftSBIT_RequestMsg, sizeof(sMLAN_Header));

  if (AUXCTRL_SS.DBASControls.TxAuxOn)
	  AUXsprintf("%u:DBAS Tx SBIT Request Message (%d) sent to DBAS\n",GetTimeUs(),Aircraft.SBIT_ReqTxCnt);
}


void AircraftSendHealthAndStatus(void)
{
  AircraftTxTimestamp = Weapon.SystemTime;

  Aircraft.HealthStatusTxCnt++;
  Aircraft.HealthStatusTxTimestamp = GetSystemTime();

  DBAS_SS.DBASWaitForRxMsg = TRUE;

  //WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR, FALSE);
  //WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR_InRange, TRUE);

  AircraftHealthStatusMsg.Status.LAR_Rad                        = Weapon.LAR.LAR_Rad;
  AircraftHealthStatusMsg.Status.DistX                          = Weapon.LAR.DistX;
  AircraftHealthStatusMsg.Status.DistY                          = Weapon.LAR.DistY;
  AircraftHealthStatusMsg.Status.LAR_Latitude                   = Weapon.LAR.Latitude * 11930464.711111; // = 4294967296.0 / 360.0;
  AircraftHealthStatusMsg.Status.LAR_Longitude                  = Weapon.LAR.Longitude * 11930464.711111; // = 4294967296.0 / 360.0;
  AircraftHealthStatusMsg.Status.RelBrngToLAR                   = Weapon.LAR.RelBrngToLAR;
  AircraftHealthStatusMsg.Status.RelBrngToTGT                   = Weapon.LAR.RelBrngToTGT;
  AircraftHealthStatusMsg.Status.DistToLAR                      = Weapon.LAR.DistToLAR;
  AircraftHealthStatusMsg.Status.DistToTGT                      = Weapon.LAR.DistToTGT;
  AircraftHealthStatusMsg.Status.LAR_Rad_Excl                   = Weapon.LAR.LAR_Rad_Excl;
  AircraftHealthStatusMsg.Status.TimeToImpact                   = Weapon.LAR.TimeToImpact;
  AircraftHealthStatusMsg.Status.TimeToIllum                    = Weapon.LAR.TimeToIllum;
  AircraftHealthStatusMsg.Status.TimeToGo                       = Weapon.LAR.TimeToGo;
  AircraftHealthStatusMsg.Status.TimeToExit                     = Weapon.LAR.TimeToExit;
  AircraftHealthStatusMsg.Status.WeaponFlags.GoNoGo             = Weapon.Flags.Bits.GoNoGo;
  AircraftHealthStatusMsg.Status.WeaponFlags.WeaponReady        = Weapon.Flags.Bits.Ready;
  AircraftHealthStatusMsg.Status.WeaponFlags.TgtProgrammed      = Weapon.Flags.Bits.MissionMode;
  AircraftHealthStatusMsg.Status.WeaponFlags.MissionValid       = Weapon.Flags.Bits.MissionOk;
  AircraftHealthStatusMsg.Status.WeaponFlags.WindValid          = Weapon.Flags.Bits.WindOk;
  AircraftHealthStatusMsg.Status.WeaponFlags.BatteryActive      = Weapon.Discretes.Bits.BatterySwitchedOn;
  AircraftHealthStatusMsg.Status.WeaponFlags.GNSS_Locked        = Weapon.Flags.Bits.GNSS_Lock;
  AircraftHealthStatusMsg.Status.WeaponFlags.IIR_TemplateValid  = 0;
  AircraftHealthStatusMsg.Status.WeaponFlags.WingsPresent       = FALSE;
  AircraftHealthStatusMsg.Status.WeaponFlags.BombType           = WeaponSettings.WeaponConfig.BombType;
  AircraftHealthStatusMsg.Status.WeaponFlags.SeekerType         = WeaponSettings.WeaponConfig.SeekerType;
  AircraftHealthStatusMsg.Status.WeaponFlags.MissionCtrlGeom    = 0;
  AircraftHealthStatusMsg.Status.WeaponFlags.AlignmentStatus    = Weapon.Status.Bits.NAV_Aligned; //((Weapon.Status.Bits.NAV_Mode == 30) || (Weapon.Status.Bits.NAV_Mode == 32) || (Weapon.Status.Bits.NAV_Mode == 34) || (Weapon.Status.Bits.NAV_Mode == 36)) ? TRUE : FALSE; //Weapon.WeaponPositionValid;
  AircraftHealthStatusMsg.Status.SAL_Status.Word                = SEEKER_DETECTION_CBIT_RESULTS;
  AircraftHealthStatusMsg.Status.WeaponBIT                      = Weapon.CBIT.DWord;
  AircraftHealthStatusMsg.Status.IIR_TransferBusy               = FALSE;
  AircraftHealthStatusMsg.Status.BatteryCapacity                = Weapon.BatteryCapacity;
  AircraftHealthStatusMsg.Status.BatteryTimeRemaining           = Weapon.BatteryTimeRemaining;
  AircraftHealthStatusMsg.Status.MissionParamsCRC               = Weapon.Mission.CRC;
  AircraftHealthStatusMsg.Status.WindParamsCRC                  = Weapon.Wind.CRC;
  AircraftHealthStatusMsg.Status.IIR_ImageCRC                   = Weapon.Mission.IIR_ImageCRC;
  AircraftHealthStatusMsg.Status.IIR_TemplateCnt                = 0;
  AircraftHealthStatusMsg.Status.SW_MajorVersion                = 2;
  AircraftHealthStatusMsg.Status.EstimatedDiveAngle             = RAD2DEG(Weapon.Mission.DiveAngle);
  AircraftHealthStatusMsg.Status.EstimatedAttackHeading         = (Weapon.Mission.AttackHeadingValid) ? RAD2DEG(Weapon.Mission.AttackHeading) : 0;
  AircraftHealthStatusMsg.Status.EstimatedImpactSpeed           = Weapon.Mission.ImpactSpeed;
  AircraftHealthStatusMsg.Status.TgtLatitude                    = Weapon.RawMission.PriTgtLatitude;
  AircraftHealthStatusMsg.Status.TgtLongitude                   = Weapon.RawMission.PriTgtLongitude;
  AircraftHealthStatusMsg.Status.TgtAltitude                    = Weapon.RawMission.PriTgtAltitude;
  AircraftHealthStatusMsg.Status.SAL_LaserPeriod                = (Weapon.Mission.SAL_Frequency > 0) ? (1.0 / Weapon.Mission.SAL_Frequency) * 1000000.0 : 0;
  AircraftHealthStatusMsg.Status.FuzingMode                     = Weapon.AircraftRawMission.FuzingMode;
  AircraftHealthStatusMsg.Status.ArmingDelay                    = Weapon.AircraftRawMission.ArmingDelay;
  AircraftHealthStatusMsg.Status.ImpactDelay                    = SS_FUZE.Fuze.FuzeTimeDelay;
//  AircraftHealthStatusMsg.Status.FuzingDistance                 = HOBS_SS.Configuration.BurstHeight * 3.28084; // Convert to [ft]
  AircraftHealthStatusMsg.Status.FuzeTime1                      = Weapon.AircraftRawMission.FuzeTime;
  AircraftHealthStatusMsg.Status.IIR_ImageNo                    = 0;
  AircraftHealthStatusMsg.Status.AircraftMissionCRC             = Aircraft.AircraftMissionCRC;
  AircraftHealthStatusMsg.Status.LAR_RadInRange                 = Weapon.LAR_InRange.LAR_Rad;
  AircraftHealthStatusMsg.Status.DistXInRange                   = Weapon.LAR_InRange.DistX;
  AircraftHealthStatusMsg.Status.DistYInRange                   = Weapon.LAR_InRange.DistY;
  AircraftHealthStatusMsg.Status.LAR_LatitudeInRange            = Weapon.LAR_InRange.Latitude * 11930464.711111; // = 4294967296.0 / 360.0;
  AircraftHealthStatusMsg.Status.LAR_LongitudeInRange           = Weapon.LAR_InRange.Longitude * 11930464.711111; // = 4294967296.0 / 360.0;
  AircraftHealthStatusMsg.Status.RelBrngToLAR_InRange           = Weapon.LAR_InRange.RelBrngToLAR;
  AircraftHealthStatusMsg.Status.Reserved5                      = 0;
  AircraftHealthStatusMsg.Status.DistToLAR_InRange              = Weapon.LAR_InRange.DistToLAR;
  AircraftHealthStatusMsg.Status.ExclusionRadiusInRange         = Weapon.LAR_InRange.LAR_Rad_Excl;
  AircraftHealthStatusMsg.Status.TimeToImpactInRange            = Weapon.LAR_InRange.TimeToImpact;
  AircraftHealthStatusMsg.Status.TimeToIllumInRange             = Weapon.LAR_InRange.TimeToIllum;
  AircraftHealthStatusMsg.Status.TimeToGoInRange                = Weapon.LAR_InRange.TimeToGo;
  AircraftHealthStatusMsg.Status.TimeToExitInRange              = Weapon.LAR_InRange.TimeToExit;

  AircraftHealthStatusMsg.Header.FrameCnt++;

  Aircraft.HealthStatus = AircraftHealthStatusMsg.Status;

  if (AUXCTRL_SS.DBASControls.TxAuxOn){
	  AUXsprintf("%u:DBAS Tx Health Status Message (%d) sent to DBAS\n",GetTimeUs(),Aircraft.HealthStatusTxCnt);

	  if (AUXCTRL_SS.DBASControls.TxDetailLARAux){
		  AUXsprintf(" LAR_Rad = %d \n",Aircraft.HealthStatus.LAR_Rad);
		  AUXsprintf(" DistX = %d \n",Aircraft.HealthStatus.DistX);
		  AUXsprintf(" DistY = %d \n",Aircraft.HealthStatus.DistY);
		  AUXsprintf(" LAR_Latitude = %d \n",Aircraft.HealthStatus.LAR_Latitude);
		  AUXsprintf(" LAR_Longitude = %d \n",Aircraft.HealthStatus.LAR_Longitude);
		  AUXsprintf(" RelBrngToLAR = %d \n",Aircraft.HealthStatus.RelBrngToLAR);
		  AUXsprintf(" RelBrngToTGT = %d \n",Aircraft.HealthStatus.RelBrngToTGT);
		  AUXsprintf(" DistToLAR = %d \n",Aircraft.HealthStatus.DistToLAR);
		  AUXsprintf(" DistToTGT = %d \n",Aircraft.HealthStatus.DistToTGT);
		  AUXsprintf(" LAR_Rad_Excl = %d \n",Aircraft.HealthStatus.LAR_Rad_Excl);
		  AUXsprintf(" TimeToImpact = %d \n",Aircraft.HealthStatus.TimeToImpact);
		  AUXsprintf(" TimeToIllum = %d \n",Aircraft.HealthStatus.TimeToIllum);
		  AUXsprintf(" TimeToGo = %d \n",Aircraft.HealthStatus.TimeToGo);
		  AUXsprintf(" TimeToExit = %d \n",Aircraft.HealthStatus.TimeToExit);
	  }

	  if (AUXCTRL_SS.DBASControls.TxDetailFlagsAux){
		  AUXsprintf(" Flag GoNoGo = %d \n",Aircraft.HealthStatus.WeaponFlags.GoNoGo);
		  AUXsprintf(" Flag WeaponReady = %d \n",Aircraft.HealthStatus.WeaponFlags.WeaponReady);
		  AUXsprintf(" Flag TgtProgrammed = %d \n",Aircraft.HealthStatus.WeaponFlags.TgtProgrammed);
		  AUXsprintf(" Flag MissionValid = %d \n",Aircraft.HealthStatus.WeaponFlags.MissionValid);
		  AUXsprintf(" Flag WindValid = %d \n",Aircraft.HealthStatus.WeaponFlags.WindValid);
		  AUXsprintf(" Flag BatteryActive = %d \n",Aircraft.HealthStatus.WeaponFlags.BatteryActive);
		  AUXsprintf(" Flag GNSS_Locked = %d \n",Aircraft.HealthStatus.WeaponFlags.GNSS_Locked);
		  AUXsprintf(" Flag IIR_TemplateValid = %d \n",Aircraft.HealthStatus.WeaponFlags.IIR_TemplateValid);
		  AUXsprintf(" Flag WingsPresent = %d \n",Aircraft.HealthStatus.WeaponFlags.WingsPresent);
		  AUXsprintf(" Flag BombType = %d \n",Aircraft.HealthStatus.WeaponFlags.BombType);
		  AUXsprintf(" Flag SeekerType = %d \n",Aircraft.HealthStatus.WeaponFlags.SeekerType);
		  AUXsprintf(" Flag MissionCtrlGeom = %d \n",Aircraft.HealthStatus.WeaponFlags.MissionCtrlGeom);
		  AUXsprintf(" Flag AlignmentStatus = %d \n",Aircraft.HealthStatus.WeaponFlags.AlignmentStatus);
	  }

	  if (AUXCTRL_SS.DBASControls.TxDetailMissionAux){
		  AUXsprintf(" SAL_Status = 0x%x \n",Aircraft.HealthStatus.SAL_Status.Word);
		  AUXsprintf(" WeaponBIT = 0x%x \n",Aircraft.HealthStatus.WeaponBIT);
		  AUXsprintf(" IIR_TransferBusy = %d \n",Aircraft.HealthStatus.IIR_TransferBusy);
		  AUXsprintf(" BatteryCapacity = %d \n",Aircraft.HealthStatus.BatteryCapacity);
		  AUXsprintf(" BatteryTimeRemaining = %d \n",Aircraft.HealthStatus.BatteryTimeRemaining);
		  AUXsprintf(" MissionParamsCRC = %d \n",Aircraft.HealthStatus.MissionParamsCRC);
		  AUXsprintf(" WindParamsCRC = 0x%x \n",Aircraft.HealthStatus.WindParamsCRC);
		  AUXsprintf(" IIR_ImageCRC = 0x%x \n",Aircraft.HealthStatus.IIR_ImageCRC);
		  AUXsprintf(" IIR_TemplateCnt = %d \n",Aircraft.HealthStatus.IIR_TemplateCnt);
		  AUXsprintf(" SW_MajorVersion = %d \n",Aircraft.HealthStatus.SW_MajorVersion);
		  AUXsprintf(" EstimatedDiveAngle = %d \n",Aircraft.HealthStatus.EstimatedDiveAngle);
		  AUXsprintf(" EstimatedAttackHeading = %d \n",Aircraft.HealthStatus.EstimatedAttackHeading);
		  AUXsprintf(" EstimatedImpactSpeed = %d \n",Aircraft.HealthStatus.EstimatedImpactSpeed);
		  AUXsprintf(" TgtLatitude = %d \n",Aircraft.HealthStatus.TgtLatitude);
		  AUXsprintf(" TgtLongitude = %d \n",Aircraft.HealthStatus.TgtLongitude);
		  AUXsprintf(" TgtAltitude = %d \n",Aircraft.HealthStatus.TgtAltitude);
		  AUXsprintf(" SAL_LaserPeriod = %d \n",Aircraft.HealthStatus.SAL_LaserPeriod);
		  AUXsprintf(" FuzingMode = 0x%x  \n",Aircraft.HealthStatus.FuzingMode);
		  AUXsprintf(" ArmingDelay = %d \n",Aircraft.HealthStatus.ArmingDelay);
		  AUXsprintf(" ImpactDelay = %d \n",Aircraft.HealthStatus.ImpactDelay);
		  //AUXsprintf(" FuzingDistance = %d \n",Aircraft.HealthStatus.FuzingDistance);
		  AUXsprintf(" FuzeTime1 = %d \n",Aircraft.HealthStatus.FuzeTime1);
		  AUXsprintf(" IIR_ImageNo = %d \n",Aircraft.HealthStatus.IIR_ImageNo);
		  AUXsprintf(" AircraftMissionCRC = 0x%x \n",Aircraft.HealthStatus.AircraftMissionCRC);
	  }

	  if (AUXCTRL_SS.DBASControls.TxDetailLARInRangeAux){
		  AUXsprintf(" LAR_RadInRange = %d \n",Aircraft.HealthStatus.LAR_RadInRange);
		  AUXsprintf(" DistXInRange = %d \n",Aircraft.HealthStatus.DistXInRange);
		  AUXsprintf(" DistYInRange = %d \n",Aircraft.HealthStatus.DistYInRange);
		  AUXsprintf(" LAR_LatitudeInRange = %d \n",Aircraft.HealthStatus.LAR_LatitudeInRange);
		  AUXsprintf(" LAR_LongitudeInRange = %d \n",Aircraft.HealthStatus.LAR_LongitudeInRange);
		  AUXsprintf(" RelBrngToLAR_InRange = %d \n",Aircraft.HealthStatus.RelBrngToLAR_InRange);
		  AUXsprintf(" Reserved5 = %d \n",Aircraft.HealthStatus.Reserved5);
		  AUXsprintf(" DistToLAR_InRange = %d \n",Aircraft.HealthStatus.DistToLAR_InRange);
		  AUXsprintf(" ExclusionRadiusInRange = %d \n",Aircraft.HealthStatus.ExclusionRadiusInRange);
		  AUXsprintf(" TimeToImpactInRange = %d \n",Aircraft.HealthStatus.TimeToImpactInRange);
		  AUXsprintf(" TimeToIllumInRange = %d \n",Aircraft.HealthStatus.TimeToIllumInRange);
		  AUXsprintf(" TimeToGoInRange = %d \n",Aircraft.HealthStatus.TimeToGoInRange);
		  AUXsprintf(" TimeToExitInRange = %d \n",Aircraft.HealthStatus.TimeToExitInRange);
	  }
  }

  MLAN_Send(MLAN_DTAC_NODE_ID, (unsigned char *) &AircraftHealthStatusMsg, sizeof(sAircraftHealthStatusMsg));
}

void AircraftSend(void)
{
  if (Aircraft.UpdateLAR) {
    Aircraft.UpdateLAR = FALSE;

    WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR, FALSE);
    WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR_InRange, TRUE);

    if (AUXCTRL_SS.DBASControls.TxDetailAuxOn)
  	  AUXsprintf("%u:DBAS LAR has been updated \n",GetTimeUs());
  }

  // Send a message to the aircraft every 100ms
  if ((Weapon.SystemTime - AircraftTxTimestamp) >= 100000) {
    AircraftTxTimestamp = Weapon.SystemTime;

    if (DBAS_SS.DBASWaitForRxMsg){
    	DBAS_SS.ErrCnt++;

		if (AUXCTRL_SS.DBASControls.ErrDetectOn) {
			AUXsprintf("%u:DBAS Receive Message Timeout ERROR - %d \n",GetTimeUs(),DBAS_SS.ErrCnt);
		}
    }

    if (BIT.SBIT_Complete) {
      WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR, FALSE);
      WeaponCalculateP32_LAR((sWeaponLAR *) &Weapon.LAR_InRange, TRUE);

      AircraftSendHealthAndStatus();
    }
    else {
      if (!AircraftSBIT_Completed) {
    	  AircraftSendSBIT_Request();
      }

      if (BIT.SBIT_Complete) {
    	  AircraftSBIT_Completed = TRUE;
      }
    }
  }

/*
  if (Aircraft.SendHealthAndStatus) {
    Aircraft.SendHealthAndStatus = FALSE;
    AircraftTxTimestamp = Weapon.SystemTime;

    AircraftSendHealthAndStatus();
  }

  // Send a message to the aircraft every 100ms
  if ((Weapon.SystemTime - AircraftTxTimestamp) >= 100000) {
    AircraftTxTimestamp = Weapon.SystemTime;

    if (BIT.SBIT_Complete) {
      AircraftSendHealthAndStatus();
    }
    else {
      if (!AircraftSBIT_Completed) {
        AircraftSendSBIT_Request();
      }

      if (BIT.SBIT_Complete) {
        AircraftSBIT_Completed = TRUE;
      }
    }
  }*/
}

static void AircraftOnSBIT(sAircraftSBIT SBIT_Data, unsigned int Timestamp)
{
  Aircraft.SBIT_ResponseRxCnt++;
  Aircraft.SBIT_RxTimestamp = Timestamp;
  AircraftSBIT_Completed = TRUE;

  if (AUXCTRL_SS.DBASControls.RxAuxOn)
  	  AUXsprintf("%u:DBAS SBIT Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.SBIT_ResponseRxCnt);
}

static void AircraftOnStatus(sAircraftStatus Status, unsigned int Timestamp)
{
  Aircraft.StatusRxCnt++;
  Aircraft.StatusRxTimestamp = Timestamp;
  Aircraft.Status = Status;

  if (AUXCTRL_SS.DBASControls.RxAuxOn){
   	  AUXsprintf("%u:DBAS Status Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.StatusRxCnt);

   	  if (AUXCTRL_SS.DBASControls.RxDetailStatusAux) {
   		  AUXsprintf(" ReleaseConsent = %d \n",Aircraft.Status.ReleaseConsent);
   		  AUXsprintf(" Mode = %d \n",Aircraft.Status.Mode);
   		  AUXsprintf(" Spare1 = %d \n",Aircraft.Status.Spare1);
   		  AUXsprintf(" WeaponConfigReqCnt = %d \n",Aircraft.Status.WeaponConfigReqCnt);
   		  AUXsprintf(" Timer1 = %d \n",Aircraft.Status.Timer1);
   		  AUXsprintf(" Timer2 = %d \n",Aircraft.Status.Timer2);
   		  AUXsprintf(" CPU_Usage = %s \n",AUXsprintfFloat(Aircraft.Status.CPU_Usage));
   		  AUXsprintf(" ArcnetRxCnt = %d \n",Aircraft.Status.ArcnetRxCnt);
   		  AUXsprintf(" ArcnetTxCnt = %d \n",Aircraft.Status.ArcnetTxCnt);
   		  AUXsprintf(" AircraftRxCnt = %d \n",Aircraft.Status.AircraftRxCnt);
   		  AUXsprintf(" EventFlags = %d \n",Aircraft.Status.EventFlags);
   		  AUXsprintf(" CtrlRegister = %d \n",Aircraft.Status.CtrlRegister);
   		  AUXsprintf(" OppStatusRegister = %d \n",Aircraft.Status.OppStatusRegister);
   		  AUXsprintf(" CmdRegister = %d \n",Aircraft.Status.CmdRegister);
   		  AUXsprintf(" IntMaskRegister = %d \n",Aircraft.Status.IntMaskRegister);
   		  AUXsprintf(" PendingIntregister = %d \n",Aircraft.Status.PendingIntregister);
   		  AUXsprintf(" BIT_Register = %d \n",Aircraft.Status.BIT_Register);
   		  AUXsprintf(" StatusRegister = %d \n",Aircraft.Status.StatusRegister);
   		  AUXsprintf(" RT_Address = %d \n",Aircraft.Status.RT_Address);
   		  AUXsprintf(" Spare2 = %d \n",Aircraft.Status.Spare2);
   		  AUXsprintf(" Spare3 = %d \n",Aircraft.Status.Spare3);
   		  AUXsprintf(" Spare4 = %d \n",Aircraft.Status.Spare4);
   		  AUXsprintf(" CBIT = 0x%x \n",Aircraft.Status.CBIT);
   	  }
  }
}

static void AircraftOnMission(sAircraftRawMission AircraftMissionData, unsigned int Timestamp)
{
  unsigned char fuze_setting;


  Aircraft.MissionRxCnt++;
  Aircraft.MissionRxTimestamp = Timestamp;

  Aircraft.RawMission = AircraftMissionData;

  MissionSetFromAircraft(&AircraftMissionData);

  // Please see the common ICD to understand how the aircraft CRC is calculated
  AircraftMissionData.ValidityFlags.LAR_Altitude = 0;
  AircraftMissionData.ValidityFlags.LAR_Mach = 0;
  AircraftMissionData.ValidityFlags.LAR_Track = 0;
  AircraftMissionData.ValidityFlags.LAR_WindDirection = 0;
  AircraftMissionData.ValidityFlags.LAR_WindSpeed = 0;

  AircraftRawWords.ValidityFlags = MirrorUSHORT(*((unsigned short *) &AircraftMissionData.ValidityFlags));
  AircraftRawWords.TgtLatitudeHi = (AircraftMissionData.PriTgtLatitude >> 16) & 0xFFFF;
  AircraftRawWords.TgtLatitudeLo = AircraftMissionData.PriTgtLatitude & 0xFFFF;
  AircraftRawWords.TgtLongitudeHi = (AircraftMissionData.PriTgtLongitude >> 16) & 0xFFFF;
  AircraftRawWords.TgtLongitudeLo = AircraftMissionData.PriTgtLongitude & 0xFFFF;
  AircraftRawWords.TgtAltitudeHi = (AircraftMissionData.PriTgtAltitude >> 16) & 0xFFFF;
  AircraftRawWords.TgtAltitudeLo = AircraftMissionData.PriTgtAltitude & 0xFFFF;
  AircraftRawWords.DiveAngle = (AircraftMissionData.DiveAngle << 8);
  AircraftRawWords.ImpactHeading = (AircraftMissionData.AttackHeading << 7);
  AircraftRawWords.ImpactSpeed = (AircraftMissionData.ImpactSpeed << 7);
  AircraftRawWords.SAL_PeriodHi = (AircraftMissionData.SAL_Code >> 1);
  AircraftRawWords.SAL_PeriodLo = (AircraftMissionData.SAL_Code & 0x01) << 15;
  AircraftRawWords.IIR_ImageNo = AircraftMissionData.IIR_ImageNo << 11;
  AircraftRawWords.IIR_ImageCRC = AircraftMissionData.IIR_ImageCRC;
  AircraftRawWords.WeaponConfig = (AircraftMissionData.MissionType & 0x03) << 11;

  if (AircraftMissionData.FuzingMode.FunctTimeAfterImpact && AircraftMissionData.FuzingMode.FunctHeight) {
    fuze_setting = 3;
  }
  else if (AircraftMissionData.FuzingMode.FunctTimeAfterImpact) {
    fuze_setting = 1;
  }
  else if (AircraftMissionData.FuzingMode.FunctHeight) {
    fuze_setting = 2;
  }
  else {
    fuze_setting = 0;
  }

  AircraftRawWords.FuzeSetting = (fuze_setting << 14) + ((AircraftMissionData.ImpactDelay & 0xFF) << 4);
  AircraftRawWords.FuzeHeight = (AircraftMissionData.FuzingDistance << 8);

  Aircraft.AircraftMissionCRC = CalculateMIL_STD_1760D_Crc((unsigned short *) &AircraftRawWords, sizeof(sAircraftRawWords) / 2);

  if (AUXCTRL_SS.DBASControls.RxAuxOn){
  	  AUXsprintf("%u:DBAS Mission Data Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.MissionRxCnt);

	  if (AUXCTRL_SS.DBASControls.RxDetailMissionAux) {
			AUXsprintf(" PriTgtLatitude = %d \n",AircraftMissionData.PriTgtLatitude);
			AUXsprintf(" PriTgtLongitude = %d \n",AircraftMissionData.PriTgtLongitude);
			AUXsprintf(" PriTgtAltitude = %d \n",AircraftMissionData.PriTgtAltitude);
			AUXsprintf(" AttackHeading = %d \n",AircraftMissionData.AttackHeading);
			AUXsprintf(" DiveAngle = %d \n",AircraftMissionData.DiveAngle);
			AUXsprintf(" Reserved1 = %d \n",AircraftMissionData.Reserved1);
			AUXsprintf(" ImpactSpeed = %d \n",AircraftMissionData.ImpactSpeed);
			AUXsprintf(" GNSS_Usage = 0x%x \n",AircraftMissionData.GNSS_Usage);
			AUXsprintf(" Reserved2 = %d \n",AircraftMissionData.Reserved2);
			AUXsprintf(" AttackHeadingValid = %d \n",AircraftMissionData.AttackHeadingValid);
			AUXsprintf(" SecondarySwitch = %d \n",AircraftMissionData.SecondarySwitch);
			AUXsprintf(" SecTgtLatitude = %d \n",AircraftMissionData.SecTgtLatitude);
			AUXsprintf(" SecTgtLongitude = %d \n",AircraftMissionData.SecTgtLongitude);
			AUXsprintf(" SecTgtAltitude = %d \n",AircraftMissionData.SecTgtAltitude);
			AUXsprintf(" SAL_Code = %d \n",AircraftMissionData.SAL_Code);
			AUXsprintf(" SAL_Activation = %d \n",AircraftMissionData.SAL_Activation);
			AUXsprintf(" SAL_Accuracy = %d \n",AircraftMissionData.SAL_Accuracy);
			AUXsprintf(" SAL_Jitter = %d \n",AircraftMissionData.SAL_Jitter);
			AUXsprintf(" IIR_ImageNo = %d \n",AircraftMissionData.IIR_ImageNo);
			AUXsprintf(" IIR_ImageOffsetX = %d \n",AircraftMissionData.IIR_ImageOffsetX);
			AUXsprintf(" IIR_ImageOffsetY = %d \n",AircraftMissionData.IIR_ImageOffsetY);
			AUXsprintf(" IIR_ImageCRC = 0x%x \n",AircraftMissionData.IIR_ImageCRC);
			AUXsprintf(" WeaponType = 0x%x \n",AircraftMissionData.WeaponType);
			AUXsprintf(" MissionType = 0x%x \n",AircraftMissionData.MissionType);
			AUXsprintf(" Spare = 0x%x \n",AircraftMissionData.Spare);
			AUXsprintf(" LAR_Track = %d \n",AircraftMissionData.LAR_Track);
			AUXsprintf(" LAR_Mach = %d \n",AircraftMissionData.LAR_Mach);
			AUXsprintf(" LAR_Altitude = %d \n",AircraftMissionData.LAR_Altitude);
			AUXsprintf(" LAR_WindSpeed = %d \n",AircraftMissionData.LAR_WindSpeed);
			AUXsprintf(" LAR_WindDirection = %d \n",AircraftMissionData.LAR_WindDirection);
			AUXsprintf(" FuzingMode = 0x%x \n",AircraftMissionData.FuzingMode);
			AUXsprintf(" ArmingDelay = %d \n",AircraftMissionData.ArmingDelay);
			AUXsprintf(" ImpactDelay = %d \n",AircraftMissionData.ImpactDelay);
			AUXsprintf(" FuzingDistance = %d \n",AircraftMissionData.FuzingDistance);
			AUXsprintf(" FuzeTime = %d \n",AircraftMissionData.FuzeTime);
			AUXsprintf(" ValidityFlags = 0x%x \n",AircraftMissionData.ValidityFlags);
			AUXsprintf(" CRC = 0x%x \n",AircraftMissionData.CRC);
	  }
  }

  if (Aircraft.Command.Control.Bits.ForseOppertunityMode) {
    Weapon.Flags.Bits.MissionMode = mmOPPORTUNITY;
  }

  AircraftSendHealthAndStatus();

  Aircraft.UpdateLAR = TRUE;
}

static void AircraftOnWind(sWind WindData, unsigned int Timestamp)
{
  Aircraft.WindRxCnt++;
  Aircraft.WindRxTimestamp = Timestamp;

  Aircraft.Wind = WindData;

  WindSet(&WindData);

  if (AUXCTRL_SS.DBASControls.RxAuxOn){
 	  AUXsprintf("%u:DBAS Wind Data Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.WindRxCnt);

 	  if (AUXCTRL_SS.DBASControls.RxDetailWindAux){

 		  AUXsprintf("Number of Wind Entries: %d \n",Aircraft.Wind.WindEntryCnt);

 		  for (int i = 0; i < Aircraft.Wind.WindEntryCnt; i++) {
 			  AUXsprintf("Entry %d - Altitude: %d Direction: %d Speed: %d \n",i,Aircraft.Wind.WindEntries[i].Altitude,
   						Aircraft.Wind.WindEntries[i].Direction,Aircraft.Wind.WindEntries[i].Speed);
 		  }

 		  AUXsprintf("CRC: 0x%x \n",Aircraft.Wind.CRC);

 	  }
   }
}

static void AircraftOnTransferAlignment(sAircraftTransferAlignment TransferAlignmentData, unsigned int Timestamp)
{
  float altitude;
  T_RTAC_TransferAlignment ta;


  AircraftTA_RxTimestamp = Weapon.SystemTime;

  Aircraft.TransferAlignmentRxCnt++;
  Aircraft.TransferAlignmentRxTimestamp = Timestamp;

  Aircraft.TransferAlignment = TransferAlignmentData;

  if (AUXCTRL_SS.DBASControls.RxAuxOn) {
	  AUXsprintf("%u:DBAS Transfer Alignment Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.TransferAlignmentRxCnt);

	  if (AUXCTRL_SS.DBASControls.RxDetailtTransAlignAux) {
		  AUXsprintf(" Validity = 0x%x \n",Aircraft.TransferAlignment.Validity.DWord);
		  AUXsprintf(" Roll = %d \n",Aircraft.TransferAlignment.Roll);
		  AUXsprintf(" Pitch = %d \n",Aircraft.TransferAlignment.Pitch);
		  AUXsprintf(" Yaw = %d \n",Aircraft.TransferAlignment.Yaw);
		  AUXsprintf(" RollRate = %d \n",Aircraft.TransferAlignment.RollRate);
		  AUXsprintf(" PitchRate = %d \n",Aircraft.TransferAlignment.PitchRate);
		  AUXsprintf(" YawRate = %d \n",Aircraft.TransferAlignment.YawRate);
		  AUXsprintf(" VelocityNorth = %d \n",Aircraft.TransferAlignment.VelocityNorth);
		  AUXsprintf(" VelocityEast = %d \n",Aircraft.TransferAlignment.VelocityEast);
		  AUXsprintf(" VelocityDown = %d \n",Aircraft.TransferAlignment.VelocityDown);
		  AUXsprintf(" Reserved1 = %d \n",Aircraft.TransferAlignment.Reserved1);
		  AUXsprintf(" Latitude = %d \n",Aircraft.TransferAlignment.Latitude);
		  AUXsprintf(" Longitude = %d \n",Aircraft.TransferAlignment.Longitude);
		  AUXsprintf(" Altitude = %d \n",Aircraft.TransferAlignment.Altitude);
		  AUXsprintf(" LeverArmX = %d \n",Aircraft.TransferAlignment.LeverArmX);
		  AUXsprintf(" LeverArmY = %d \n",Aircraft.TransferAlignment.LeverArmY);
		  AUXsprintf(" LeverArmZ = %d \n",Aircraft.TransferAlignment.LeverArmZ);
		  AUXsprintf(" Latency1 = %d \n",Aircraft.TransferAlignment.Latency1);
		  AUXsprintf(" Latency2 = %d \n",Aircraft.TransferAlignment.Latency2);
		  AUXsprintf(" Latency3 = %d \n",Aircraft.TransferAlignment.Latency3);
		  AUXsprintf(" Latency4 = %d \n",Aircraft.TransferAlignment.Latency4);
		  AUXsprintf(" TrueAirspeed = %d \n",Aircraft.TransferAlignment.TrueAirspeed);
		  AUXsprintf(" CalibratedAirspeed = %d \n",Aircraft.TransferAlignment.CalibratedAirspeed);
		  AUXsprintf(" StaticPressure = %d \n",Aircraft.TransferAlignment.StaticPressure);
		  AUXsprintf(" AngleOfAttack = %d \n",Aircraft.TransferAlignment.AngleOfAttack);
		  AUXsprintf(" AngleOfSideslip = %d \n",Aircraft.TransferAlignment.AngleOfSideslip);
		  AUXsprintf(" Reserved2 = %d \n",Aircraft.TransferAlignment.Reserved2);
	  }
  }

  if (TransferAlignmentData.Validity.Latitude) {
    Aircraft.Latitude = ((double) TransferAlignmentData.Latitude * PI) / 4294967296.0;
  }

  if (TransferAlignmentData.Validity.Longitude) {
    Aircraft.Longitude = ((double) TransferAlignmentData.Longitude * PI) / 2147483648.0;
  }

  ta.ValidityWord = TransferAlignmentData.Validity.DWord;
  ta.Roll = (TransferAlignmentData.Roll * PI) / 32768.0;
  ta.Pitch = (TransferAlignmentData.Pitch * PI) / 65536.0;
  ta.Heading = (TransferAlignmentData.Yaw * PI) / 32768.0;
  ta.RollRate = (TransferAlignmentData.RollRate * PI) / 8192.0;
  ta.PitchRate = (TransferAlignmentData.PitchRate * PI) / 16384.0;
  ta.YawRate = (TransferAlignmentData.YawRate * PI) / 16384.0;
  ta.VelocityNorth = TransferAlignmentData.VelocityNorth * 0.01;
  ta.VelocityEast = TransferAlignmentData.VelocityEast * 0.01;
  ta.VelocityDown = TransferAlignmentData.VelocityDown * 0.01;
  ta.Latitude = ((double) TransferAlignmentData.Latitude * PI) / 4294967296.0;
  ta.Longitude = ((double) TransferAlignmentData.Longitude * PI) / 2147483648.0;
  ta.Altitude = TransferAlignmentData.Altitude;
  ta.LeverArmX = TransferAlignmentData.LeverArmX * 0.01;
  ta.LeverArmY = TransferAlignmentData.LeverArmY * 0.01;
  ta.LeverArmZ = TransferAlignmentData.LeverArmZ * 0.01;
  ta.Latency1 = TransferAlignmentData.Latency1 * 0.0001;
  ta.Latency2 = TransferAlignmentData.Latency2 * 0.0001;
  ta.Latency3 = TransferAlignmentData.Latency3 * 0.0001;
  ta.Latency4 = TransferAlignmentData.Latency4 * 0.0001;
  ta.TrueAirspeed = TransferAlignmentData.TrueAirspeed * 0.01;
  ta.CalibratedAirspeed = TransferAlignmentData.CalibratedAirspeed * 0.01;
  ta.StaticPressure = TransferAlignmentData.StaticPressure * 10;
  ta.AngleOfAttack = (TransferAlignmentData.AngleOfAttack * PI) / 32768.0;
  ta.AngleOfSideslip = (TransferAlignmentData.AngleOfSideslip * PI) / 32768.0;
  ta.Altitude = TransferAlignmentData.Altitude;

  // Convert Altitude in EGM96 to WGS84
  if (GetEGM96GeoidHeight(&altitude, ta.Latitude, ta.Longitude)) {
    ta.Altitude = TransferAlignmentData.Altitude + altitude;
  }

  NavStart50Hz(&ta);
}

void AircraftOnConfigRequest(unsigned int Timestamp)
{
  Aircraft.ConfigReqRxCnt++;
  Aircraft.ConfigReqRxTimestamp = Timestamp;

  if (AUXCTRL_SS.DBASControls.RxAuxOn)
	  AUXsprintf("%u:DBAS Config Request Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.ConfigReqRxCnt);

  // If the fuze initialization is completed, send the config to the DTAC
  if (SS_FUZE.Fuze.InitComplete) {
    AircraftWeaponConfig.WeaponConfig.SeekerType = WeaponSettings.WeaponConfig.SeekerType;
    AircraftWeaponConfig.WeaponConfig.BombType = bombMK82;
    AircraftWeaponConfig.WeaponConfig.WeaponType = weaponP3;
    AircraftWeaponConfig.WeaponConfig.WingsPresent = FALSE;
    AircraftWeaponConfig.WeaponConfig.BatteryActivationMethod = 0;
    AircraftWeaponConfig.WeaponConfig.MaxAirburstHeight = 60;
    AircraftWeaponConfig.WeaponConfig.SW_ICD_Version = 2;
    AircraftWeaponConfig.WeaponConfig.Reserved[0] = 0;
    AircraftWeaponConfig.WeaponConfig.Reserved[1] = 0;
    AircraftWeaponConfig.WeaponConfig.Reserved[2] = 0;

    switch (SS_FUZE.Fuze.Mode) {
      case fmNON_PROGRAMMABLE : AircraftWeaponConfig.WeaponConfig.FuzeType = 0; break;
      case fmDELAY : AircraftWeaponConfig.WeaponConfig.FuzeType = 1; break;
      case fmHEIGHT : AircraftWeaponConfig.WeaponConfig.FuzeType = (WeaponSettings.WeaponConfig.HOBS_Present) ? 3 : 0; break;
      case fmHEIGHT_AND_DELAY : AircraftWeaponConfig.WeaponConfig.FuzeType = (WeaponSettings.WeaponConfig.HOBS_Present) ? 4 : 1; break;
      default : AircraftWeaponConfig.WeaponConfig.FuzeType = 0; break;
    }

    AircraftWeaponConfig.Header.FrameCnt++;

    MLAN_Send(MLAN_DTAC_NODE_ID, (unsigned char *) &AircraftWeaponConfig, sizeof(sAircraftWeaponConfigMsg));

    Aircraft.ConfigTxCnt++;
    Aircraft.ConfigTxTimestamp = GetSystemTime();
    Aircraft.WeaponConfig = AircraftWeaponConfig.WeaponConfig;

    if (AUXCTRL_SS.DBASControls.TxAuxOn){
    	AUXsprintf("%u:DBAS Weapon Config Message (%d) sent to DBAS\n",GetTimeUs(),Aircraft.ConfigTxCnt);

    	if (AUXCTRL_SS.DBASControls.TxDetailWeaponCfgAux){
    		AUXsprintf(" SeekerType = %d \n",Aircraft.WeaponConfig.SeekerType);
    		AUXsprintf(" BombType = %d \n",Aircraft.WeaponConfig.BombType);
    		AUXsprintf(" WeaponType = %d \n",Aircraft.WeaponConfig.WeaponType);
    		AUXsprintf(" WeaponType = %d \n",Aircraft.WeaponConfig.WingsPresent);
    		AUXsprintf(" BatteryActivationMethod = %d \n",Aircraft.WeaponConfig.BatteryActivationMethod);
    		AUXsprintf(" FuzeType = %d \n",Aircraft.WeaponConfig.FuzeType);
    		AUXsprintf(" MaxAirburstHeight = %d \n",Aircraft.WeaponConfig.MaxAirburstHeight);
    		AUXsprintf(" SW_ICD_Version = %d \n",Aircraft.WeaponConfig.SW_ICD_Version);
    	}
    }
  }
}

static void AircraftOnCommand(sAircraftCommand Msg, unsigned int Timestamp)
{
  Aircraft.CommandRxCnt++;
  Aircraft.Command = Msg;

  Weapon.UseAircraftLAR = Aircraft.Command.Control.Bits.LAR_Control;
  Weapon.InsideAircraftLAR = Aircraft.Command.Control.Bits.InsideLAR;

  if (AUXCTRL_SS.DBASControls.RxAuxOn){
	  AUXsprintf("%u:DBAS On Command Message (%d) RECEIVED from DBAS \n",GetTimeUs(),Aircraft.CommandRxCnt);
	  if (AUXCTRL_SS.DBASControls.RxDetailAuxOn){
		  AUXsprintf(" LAR Control = %d \n",Aircraft.Command.Control.Bits.LAR_Control);
		  AUXsprintf(" Inside LAR = %d \n",Aircraft.Command.Control.Bits.InsideLAR);
		  AUXsprintf(" Clear SBIT/CBIT = %d (Yes/No)\n",Aircraft.Command.ClearCBIT);
		  AUXsprintf(" Force Opportunity Mode = %d (Yes/No)\n",Aircraft.Command.Control.Bits.ForseOppertunityMode);
	  }
  }

  if (Aircraft.Command.ClearCBIT) {

	BIT_ClearSBIT();
    BIT_ClearCBIT();

	if (AUXCTRL_SS.DBASControls.RxAuxOn)
		AUXsprintf("%u:DBAS Clear SBIT & CBIT on Command received \n",GetTimeUs());

  }

  if (Aircraft.Command.Control.Bits.ForseOppertunityMode) {
    Weapon.Flags.Bits.MissionMode = mmOPPORTUNITY;
    if (AUXCTRL_SS.DBASControls.RxAuxOn)
    	AUXsprintf("%u:DBAS Force Opportunity Mode on Command received \n",GetTimeUs());
  }
}

void AircraftHandleRxData(unsigned char *RxData, unsigned int Timestamp)
{
  Aircraft.RxTimestamp = Weapon.SystemTime;
  Aircraft.RxCnt++;

  AircraftConnected = TRUE;

  AircraftRxMsg = *(sAircraftRxMsg *) RxData;

  if (AUXCTRL_SS.DBASControls.RxDetailAuxOn){
	  AUXsprintf("%u:DBAS Message Received - OpCode: 0x%x \n",GetTimeUs(),AircraftRxMsg.Header.Opcode);
  }

  if ((AircraftRxMsg.Header.Opcode == AIRCRAFT_MSG_ID_SBIT) || (AircraftRxMsg.Header.Opcode == AIRCRAFT_MSG_ID_STATUS)) {
	  //Check comms when either SBIT or STATUS messages are received
	  DBAS_SS.DBASWaitForRxMsg = FALSE;
  }

  switch (AircraftRxMsg.Header.Opcode) {
    case AIRCRAFT_MSG_ID_SBIT : AircraftOnSBIT(AircraftRxMsg.Payload.SBIT, Timestamp); break;
    case AIRCRAFT_MSG_ID_WPN_CONFIQ_REQ : AircraftOnConfigRequest(Timestamp); break;
    case AIRCRAFT_MSG_ID_COMMAND : AircraftOnCommand(AircraftRxMsg.Payload.Command, Timestamp);  break;
    case AIRCRAFT_MSG_ID_STATUS : AircraftOnStatus(AircraftRxMsg.Payload.Status, Timestamp); break;
    case AIRCRAFT_MSG_ID_MISSION : AircraftOnMission(AircraftRxMsg.Payload.Mission, Timestamp); break;
    case AIRCRAFT_MSG_ID_WIND : AircraftOnWind(AircraftRxMsg.Payload.Wind, Timestamp); break;
    case AIRCRAFT_MSG_ID_TRANSFER_ALIGNMENT : AircraftOnTransferAlignment(AircraftRxMsg.Payload.TransferAlignment, Timestamp); break;
    default : break;
  }
}




