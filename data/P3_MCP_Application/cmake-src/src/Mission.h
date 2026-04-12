/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Mission.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Mission module.
 *
 ******************************************************************************/

#ifndef MISSION_H_
#define MISSION_H_

#include "MCP.h"
#include "AircraftInterface.h"
#include "WIAC82_Comms.h"
#include "Fuze.h"


#pragma pack(1)

typedef enum
{
  msWIRELESS_CONTROLLER,
  msDTAC,
  msRTAC,
  msETE,
  msEEPROM
} eMissionSource;

typedef enum
{
  mmOPPORTUNITY,
  mmPROGRAMMED
} eMissionMode;


typedef struct
{
  int            PriTgtLatitude;              // 12 - 15
  int            PriTgtLongitude;             // 16 - 19
  int            PriTgtAltitude;              // 20 - 23
  unsigned int   AttackHeading;               // 24 - 27
  unsigned char  DiveAngle;                   // 28
  unsigned char  Spare1;                      // 29
  unsigned short ImpactSpeed;                 // 30 - 31
  uGNSS_Usage    GNSS_Usage;                  // 32          Bit 0: 0 - use GPS, 1 - don't use GPS, Bit 1: 0 - use GLONASS, 1 - don't use GLONASS
  unsigned char  Reserved1;                   // 33
  unsigned char  AttackHeadingValid;          // 34
  unsigned char  SecondarySwitch;             // 35
  int            SecTgtLatitude;              // 36 - 39
  int            SecTgtLongitude;             // 40 - 43
  int            SecTgtAltitude;              // 44 - 47
  unsigned int   Reserved2[5];                // 48 - 67      RTAC-BC #1 filling
  unsigned int   SAL_Code;                    // 68 - 71
  unsigned int   SAL_Activation;              // 72 - 75
  unsigned short SAL_Accuracy;                // 76 - 77
  unsigned short SAL_Jitter;                  // 78 - 79
  unsigned short IIR_ImageNo;                 // 80 - 81
  unsigned short IIR_ImageOffsetX;            // 82 - 83
  unsigned short IIR_ImageOffsetY;            // 84 - 85
  unsigned short IIR_ImageCRC;                // 86 - 87
  unsigned int   Reserved3[3];                // 88 - 99      Used for IIR seeker and EFUZE
  unsigned int   Reserved4[5];                // 100 - 119    RTAC-BC #2 filling
  unsigned char  WeaponType;                  // 120          0 = Al-Tariq, 1 = P3
  unsigned char  MissionType;                 // 121          0 = GNSS, 1 = SAL, 2 = IIR
  unsigned short CRC;                         // 122 - 123
} sRawMission;

typedef union
{
  unsigned short Word;
  struct {
    unsigned short PriTgtLatitude     : 1;
    unsigned short PriTgtLongitude    : 1;
    unsigned short PriTgtAltitude     : 1;
    unsigned short DiveAngle          : 1;
    unsigned short AttackHeading      : 1;
    unsigned short ImpactSpeed        : 1;
    unsigned short SAL_Code           : 1;
    unsigned short IIR_ImageNo        : 1;
    unsigned short FuzeSettings       : 1;
    unsigned short FuzeDelay          : 1;
    unsigned short FuzeHeight         : 1;
    unsigned short LAR_Track          : 1;
    unsigned short LAR_Mach           : 1;
    unsigned short LAR_Altitude       : 1;
    unsigned short LAR_WindSpeed      : 1;
    unsigned short LAR_WindDirection  : 1;
  } Bits;
} uMissionValidityFlags;

typedef struct
{
  double PriTgtLatitude;                      // [rad]
  double PriTgtLongitude;                     // [rad]
  float PriTgtAltitude;                       // [m]
  float AttackHeading;                        // [rad]
  float DiveAngle;                            // [rad]
  float ImpactSpeed;                          // [m/s]
  uGNSS_Usage GNSS_Usage;
  unsigned char MissionValid;
  unsigned char TargetValid;
  unsigned char AttackHeadingValid;
  unsigned char LAR_LaunchConditionsValid;
  unsigned char LAR_WindValid;
  sAircraftMissionValidityFlags ValidityFlags;
  unsigned char MissionSetResult;
  unsigned char IIR_ImageNo;
  unsigned short IIR_ImageCRC;
  unsigned int SAL_Code;
  float SAL_Frequency;                        // [Hz]
  unsigned int SAL_Activation;
  unsigned short SAL_Accuracy;                // [ns]
  unsigned short SAL_Jitter;                  // [ns]
  unsigned char WeaponType;
  unsigned char MissionType;
  unsigned char MissionSource;
  unsigned short CRC;
  float LAR_Track;                            // [rad]
  float LAR_Mach;
  float LAR_Altitude;                         // [m]
  float LAR_WindSpeed;                        // [m/s]
  float LAR_WindDirection;                    // [rad]
  eFuzeMode FuzeMode;
  unsigned char FuzeDelay;                    // [ms]
  unsigned char UpdatingFuze;
  unsigned char FuzingDistance;
} sMission;

#pragma pack()

typedef struct
{
	char		SetFuzeDelay;
	char		SetHOBS_Height;
	char 		WeaponTypeOK;
	char 		MissionTypeOK;
	char 		TargetPositionOK;
	char 		MovingTargetOK;
	char 		DiveAngleOK;
	char 		SALCodeOK;
	char 		MissionOK;
} sMISSION_Controls;

typedef struct
{
	sMission 					WIAC82_Mission;
	sMISSION_Controls	 		Control;
} sMISSION_StateSpace;

extern sMISSION_StateSpace MISS_SS;


void InitMission(char *AuxStr);
void ClearMission(eMissionSource Source);
unsigned char MissionSet(sRawMission *RawMissionData, eMissionSource MissionSource);
unsigned char MissionSetFromAircraft(sAircraftRawMission *RawMissionData);
unsigned char SetMissionFromWIAC82(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData, unsigned short CRC_OK);
char *DecodeMissionGNSS_Usage(uGNSS_Usage GNSS_Use);
void CheckValidityOfMission(sWIAC82_1553_MLAN_FunctionalExchange3D_Message *WIAC82MissionData);
void CopyWIAC82DatatoTelemetry(void);
uint16_t EncodeDiveAngle(float DiveAngle);
void UpdateGNSS_Usage(void);
#endif /* MISSION_H_ */


