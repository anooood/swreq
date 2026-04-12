/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Weapon.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Weapon module.
 *
 ******************************************************************************/

#ifndef WEAPON_H
#define WEAPON_H


#include "Timer.h"
#include "Mission.h"
#include <stdint.h>

#define WEAPON_SERVO_TEST_ID                      0xA34DA668

#define WEAPON_MAX_LAR_TIME                       1200        // [s]


typedef enum
{
  wsOFF,                            // 0
  wsSTANDBY,                        // 1
  wsWAIT_FOR_BATTERY_ON,            // 2
  wsBATTERY_ERROR,                  // 3
  wsBATTERY_ON,                     // 4
  wsLAUNCHED,                       // 5
  wsSERVO_TEST,                     // 6
  wsHILS,                           // 7
  wsBATTERY_ARMED		            // 8
} eWeaponState;

#pragma pack(1)

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short WeaponActivation      : 1;  // 0
    unsigned short LaunchLinkPresent     : 1;  // 1
    unsigned short LanyardPresent        : 1;  // 2
    unsigned short LanyardPulled         : 1;  // 3
    unsigned short BatterySwitchedOn     : 1;  // 4
    unsigned short GNSS_AntennaSwitch    : 1;  // 5
    unsigned short HILS_Mode             : 1;  // 6
    unsigned short DownloadCablePresent  : 1;  // 7
    unsigned short ServosEnabled         : 1;  // 8
    unsigned short TailFinUnlocked       : 1;  // 9
    unsigned short Reserved              : 6;  // 10 - 15
  } Bits;
} uWeaponDiscretes;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short WeaponActivation      : 1;  // 0
    unsigned short SwitchOnBattery       : 1;  // 1
    unsigned short BatterySwitchedOn     : 1;  // 2
    unsigned short LaunchLinkPresent     : 1;  // 3
    unsigned short LanyardPresent        : 1;  // 4
    unsigned short LanyardPulled         : 1;  // 5
    unsigned short ServosEnabled         : 1;  // 6
    unsigned short TailFinUnlocked       : 1;  // 7
    unsigned short GNSS_AntennaSwitch    : 1;  // 8
    unsigned short WiFiAntennaSelected   : 1;  // 9
    unsigned short ActivateSAD           : 1;  // 10
    unsigned short GCA_Mode              : 2;  // 11-12
    unsigned short BatteryError          : 1;  // 13
    unsigned short WaitForLaunchSTC      : 1;  // 14
    unsigned short Spare                 : 1;  // 15
  } Bits;
} uWeaponDiscretesDummy;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short GoNoGo                   : 1;  // Bit 0
    unsigned short Ready                    : 1;  // Bit 1
    unsigned short MissionMode              : 1;  // Bit 2
    unsigned short MissionOk                : 1;  // Bit 3
    uint16_t WindOk                   : 1;  // Bit 4
    unsigned short WeaponArmed             	: 1;  // Bit 5
    unsigned short GNSS_Lock                : 1;  // Bit 6
    unsigned short ActivateSAD              : 1;  // Bit 7
    unsigned short SwitchOnBattery          : 1;  // Bit 8
    unsigned short BatteryError             : 1;  // Bit 9
    unsigned short WeaponCDSS               : 1;  // Bit 10
    unsigned short WeaponBunt               : 1;  // Bit 11
    unsigned short Reserved3                : 4;  // Bits 12 - 15
  } Bits;
} uWeaponFlags;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int STC1_State              : 3;  // 0 - 2
    unsigned int STC2_State              : 3;  // 3 - 5
    unsigned int WeaponLaunched          : 1;  // 6
    unsigned int NAV_Aligned             : 1;  // 7
    unsigned int State                   : 4;  // 8 - 11  (eWeaponState)
    unsigned int Reserved2               : 4;  // 12 - 15
    unsigned int NAV_Mode                : 8;  // 16 - 23
    unsigned int Reserved3               : 16; // 24 - 31
  } Bits;
} uWeaponStatus;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short WeaponLaunched          : 4;  // 0 - 3
    unsigned short NAV_Mode                : 4;  // 4 - 7
    unsigned short STC1_State              : 4;  // 8 - 11
    unsigned short STC2_State              : 4;  // 12 - 15
  } Bits;
} uWeaponStatusDummy;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int GCA_Fail                : 1;  // Bit 0   1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int IMU_Fail                : 1;  // Bit 1   1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int GNSS_Fail               : 1;  // Bit 2   1 - Critical Weapon Failure (Rafale - Degraded)
    unsigned int Reserved3               : 1;  // Bit 3   0
    unsigned int HOBS_Fail               : 1;  // Bit 4   1 - Critical Weapon Failure
    unsigned int Reserved5               : 1;  // Bit 5   0
    unsigned int ServoFail               : 1;  // Bit 6   1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int WirelessLinkFail        : 1;  // Bit 7   0
    unsigned int SeekerFail              : 1;  // Bit 8   0                           (Rafale - Degraded)
    unsigned int BatteryFail             : 1;  // Bit 9   1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int DTAC_Fail               : 1;  // Bit 10  1 - Critical Weapon Failure
    unsigned int FinLockFail             : 1;  // Bit 11  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int IMU_CommsFail           : 1;  // Bit 12  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int GNSS_CommsFail          : 1;  // Bit 13  1 - Critical Weapon Failure (Rafale - Degraded)
    unsigned int Reserved14              : 1;  // Bit 14  0
    unsigned int HOBS_CommsFail          : 1;  // Bit 15  1 - Critical Weapon Failure
    unsigned int Reserved16              : 1;  // Bit 16  0
    unsigned int ServoCommsFail          : 1;  // Bit 17  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int WirelessLinkCommsFail   : 1;  // Bit 18  0
    unsigned int SeekerCommsFail         : 1;  // Bit 19  0                           (Rafale - Degraded)
    unsigned int ArcnetReconFail         : 1;  // Bit 20  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int DTAC_CommsFail          : 1;  // Bit 21  1 - Critical Weapon Failure
    unsigned int BatteryCommsFail        : 1;  // Bit 22  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int TailFinFail             : 1;  // Bit 23  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int Reserved24              : 1;  // BIT 24  0
    unsigned int Reserved25              : 1;  // BIT 25  0
    unsigned int Reserved26              : 1;  // BIT 26  0
    unsigned int DegradedFail            : 1;  // BIT 27  0
    unsigned int IF_CommsFail            : 1;  // BIT 28  1 - Critical Weapon Failure
    unsigned int NavFail                 : 1;  // BIT 29  1 - Critical Weapon Failure
    unsigned int ConfigFail              : 1;  // BIT 30  1 - Critical Weapon Failure (Rafale - Critical)
    unsigned int CriticalFail            : 1;  // Bit 31  0
  } Bits;
} uWeaponBIT;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int GCA_Fail                : 1;  // Bit 0
    unsigned int IMU_Fail                : 1;  // Bit 1
    unsigned int GNSS_Fail               : 1;  // Bit 2
    unsigned int FinLockFail             : 1;  // Bit 3
    unsigned int ServoFail               : 1;  // Bit 4
    unsigned int WirelessLinkFail        : 1;  // Bit 5
    unsigned int SeekerFail              : 1;  // Bit 6
    unsigned int BatteryFail             : 1;  // Bit 7
    unsigned int TailFinFail             : 1;  // Bit 8
    unsigned int NavFail                 : 1;  // Bit 9
    unsigned int SW_ExecOverrun          : 1;  // Bit 10
    unsigned int Reserved1               : 1;  // Bit 11
    unsigned int IMU_CommsFail           : 1;  // Bit 12
    unsigned int GNSS_CommsFail          : 1;  // Bit 13
    unsigned int BatteryCommsFail        : 1;  // Bit 14
    unsigned int IF_CommsFail            : 1;  // Bit 15
    unsigned int ServoCommsFail          : 1;  // Bit 16
    unsigned int WirelessLinkCommsFail   : 1;  // Bit 17
    unsigned int SeekerCommsFail         : 1;  // Bit 18
    unsigned int ArcnetReconFail         : 1;  // Bit 19
    unsigned int Reserved2               : 1;  // Bit 20
    unsigned int Reserved3               : 1;  // Bit 21
    unsigned int Reserved4               : 1;  // Bit 22
    unsigned int Reserved5               : 1;  // BIT 23
    unsigned int Reserved6               : 1;  // BIT 24
    unsigned int Reserved7               : 1;  // BIT 25
    unsigned int Reserved8               : 1;  // BIT 26
    unsigned int Reserved9               : 1;  // BIT 27
    unsigned int Reserved10              : 1;  // BIT 28
    unsigned int Reserved11              : 1;  // BIT 29
    unsigned int Reserved12              : 1;  // Bit 30
    unsigned int CriticalFail            : 1;  // Bit 31
  } Bits;
} uWeaponBITDummy;

typedef struct
{
  unsigned char Valid;
  unsigned char Spare[3];
  unsigned int LAR_Rad;
  int DistX;
  int DistY;
  double Latitude;
  double Longitude;
  unsigned short RelBrngToLAR;
  unsigned short RelBrngToTGT;
  unsigned int DistToLAR;
  unsigned int DistToTGT;
  unsigned int LAR_Rad_Excl;
  unsigned short TimeToImpact;
  unsigned short TimeToIllum;
  unsigned short TimeToGo;
  unsigned short TimeToExit;
} sWeaponLAR;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int MCP_Fail                : 1;  // Bit 0
    unsigned int IMU_Fail                : 1;  // Bit 1
    unsigned int UBLOX_Fail              : 1;  // Bit 2
    unsigned int PitotFail               : 1;  // Bit 3
    unsigned int Reserved1               : 1;  // Bit 4
    unsigned int ITU_Fail                : 1;  // Bit 5
    unsigned int ServosFail              : 1;  // Bit 6
    unsigned int WiFiFail                : 1;  // Bit 7
    unsigned int SeekerFail              : 1;  // Bit 8
    unsigned int BatteryPackFail         : 1;  // Bit 9
    unsigned int Reserved2               : 2;  // Bit 10 - 11
    unsigned int IMU_CommsFail           : 1;  // Bit 12
    unsigned int UBLOX_CommsFail         : 1;  // Bit 13
    unsigned int PitotCommsFail          : 1;  // Bit 14
    unsigned int Reserved3               : 1;  // Bit 15
    unsigned int ITU_CommsFail           : 1;  // Bit 16
    unsigned int ServosCommsFail         : 1;  // Bit 17
    unsigned int WiFiCommsFail           : 1;  // Bit 18
    unsigned int SeekerCommsFail         : 1;  // Bit 19
    unsigned int ArcnetReconFail         : 1;  // Bit 20
    unsigned int Reserved4               : 10; // Bits 21 - 30
    unsigned int CriticalFail            : 1;  // Bit 31
  } Bits;
} uWeaponHealthBIT;

typedef union
{
  unsigned char Byte;
  struct {
    unsigned char WingsPresent           : 1;
    unsigned char BombType               : 2;
    unsigned char SeekerType             : 2;
    unsigned char MissionGeom            : 2;
    unsigned char Spare                  : 1;
  } Bits;
} uAP_Config;
/*
typedef struct
{
  unsigned int   LAR_Rad               ;       // 12
  int            DistX                 ;       // 16
  int            DistY                 ;       // 20
  unsigned short RelBrngToLAR          ;       // 24
  unsigned short RelBrngToTGT          ;       // 26
  unsigned int   DistToLAR             ;       // 28
  unsigned int   DistToTGT             ;       // 32
  unsigned int   LAR_Rad_Excl          ;       // 36
  unsigned short ImpactTime            ;       // 40
  unsigned short IllumTime             ;       // 42
  unsigned short TimeToGo              ;       // 44
  uGNSS_Usage    GNSS_Usage            ;       // 46
  unsigned char  HealthStatusVersion   ;       // 47
  unsigned char  WeaponFlags           ;       // 48
  unsigned char  AP_Config             ;       // 49
  unsigned short SAL_Status            ;       // 50
  unsigned int   WeaponBIT             ;       // 52
  unsigned char  BatteryCapacity       ;       // 56
  unsigned char  WirelessLinkStatus    ;       // 57
  unsigned short MissionParamsCRC      ;       // 58
  unsigned short WindParamsCRC         ;       // 60
  unsigned short IIR_ImageCRC          ;       // 62
  unsigned short IIR_TemplateCnt       ;       // 64
  unsigned short Spare1                ;       // 66
  int            TgtLatitude           ;       // 68
  int            TgtLongitude          ;       // 72
  int            TgtAltitude           ;       // 76
  unsigned int   SAL_LaserPeriod       ;       // 80
  unsigned short EstimatedDiveAngle    ;       // 84
  unsigned short EstimatedAttackHeading;       // 86
  unsigned short EstimatedImpactSpeed  ;       // 88
  unsigned char  EFUZE_Data[12]        ;       // 90
  unsigned short Spare2[11]            ;       // 102
} sWeaponHealthAndStatus;

typedef struct
{
  float          LaserFreq         ;           // 124
  float          GPS_TimeOfWeek    ;           // 128
  float          WeaponTime        ;           // 132
  unsigned char  ServoInputVoltage ;           // 136
  signed char    PitotTubeTemp     ;           // 137
  signed char    PressureSensTemp  ;           // 138
  unsigned char  WiFi_RSSI         ;           // 139
  unsigned short TotalPressure     ;           // 140
  unsigned short StaticPressure    ;           // 142
  float          WeaponLatitude    ;           // 144
  float          WeaponLongitude   ;           // 148
  float          WeaponAltitude    ;           // 152
  unsigned short CAS               ;           // 156
  short          LLLN_Heading      ;           // 158
  short          LLLN_Pitch        ;           // 160
  short          LLLN_Roll         ;           // 162
  unsigned char  Discretes         ;           // 164
  unsigned char  NavStatus         ;           // 165
  unsigned char  MCP_State         ;           // 166
  unsigned char  LAN_ReconCnt      ;           // 167
  unsigned int   PitotCBIT         ;           // 168
  unsigned int   SeekerCBIT        ;           // 172
  unsigned short ITUC_CBIT         ;           // 176
  unsigned short MCP_CBIT          ;           // 178
  unsigned short WLinkCBIT         ;           // 180
  unsigned char  ServoA_Status     ;           // 182
  unsigned char  ServoB_Status     ;           // 183
  unsigned char  ServoC_Status     ;           // 184
  unsigned char  ServoD_Status     ;           // 185
  unsigned char  GNSS_GPS_UsedCnt  ;           // 186
  unsigned char  GNSS_GLO_UsedCnt  ;           // 187
  unsigned char  EnableUserLog     ;           // 188
  unsigned char  Spare3[3]         ;           // 189
  unsigned int   Spare4[3]         ;           // 192 - 203
} sWeaponTelemetry;*/

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char GoNoGo                   : 1;  // Bit 0
    unsigned char Ready                    : 1;  // Bit 1
    unsigned char MissionMode              : 1;  // Bit 2
    unsigned char MissionOk                : 1;  // Bit 3
    unsigned char WindOk                   : 1;  // Bit 4
    unsigned char BatterySwitchedOn        : 1;  // Bit 5
    unsigned char GNSS_Lock                : 1;  // Bit 6
    unsigned char IIR_TemplateValid        : 1;  // Bit 7
  } Bits;
} uWeaponWC_Flags;

typedef union
{
  unsigned short Byte;
  struct
  {
    unsigned short GoNoGo                   : 1;  // Bit 0
    unsigned short Ready                    : 1;  // Bit 1
    unsigned short MissionMode              : 1;  // Bit 2
    unsigned short MissionOk                : 1;  // Bit 3
    unsigned short WindOk                   : 1;  // Bit 4
    unsigned short BatterySwitchedOn        : 1;  // Bit 5
    unsigned short GNSS_Lock                : 1;  // Bit 6
    unsigned short IIR_TemplateValid        : 1;  // Bit 7
    unsigned short WingsPresent             : 1;  // Bit 8
    unsigned short BombType                 : 2;  // Bit 10
    unsigned short SeekerType               : 2;  // Bit 12
    unsigned short MissionControlGeometry   : 2;  // Bit 14
    unsigned short Spare                    : 1;  // Bit 15
  } Bits;
} uWeaponWC_FlagsDummy;

typedef struct
{
  unsigned int   LAR_Rad               ;       // 12
  int            DistX                 ;       // 16
  int            DistY                 ;       // 20
  unsigned short RelBrngToLAR          ;       // 24
  unsigned short RelBrngToTGT          ;       // 26
  unsigned int   DistToLAR             ;       // 28
  unsigned int   DistToTGT             ;       // 32
  unsigned int   LAR_Rad_Excl          ;       // 36
  unsigned short ImpactTime            ;       // 40
  unsigned short IllumTime             ;       // 42
  unsigned short TimeToGo              ;       // 44
  uGNSS_Usage    GNSS_Usage            ;       // 46
  unsigned char  HealthStatusVersion   ;       // 47
  unsigned char  WeaponFlags           ;       // 48
  unsigned char  AP_Config             ;       // 49
  unsigned short SAL_Status            ;       // 50
  unsigned int   WeaponBIT             ;       // 52
  unsigned char  BatteryCapacity       ;       // 56
  unsigned char  WirelessLinkStatus    ;       // 57
  unsigned short MissionParamsCRC      ;       // 58
  unsigned short WindParamsCRC         ;       // 60
  unsigned short IIR_ImageCRC          ;       // 62
  unsigned short IIR_TemplateCnt       ;       // 64
  unsigned char  MissionSetResult      ;       // 66
  unsigned char  Spare                 ;       // 67
  int            TgtLatitude           ;       // 68
  int            TgtLongitude          ;       // 72
  int            TgtAltitude           ;       // 76
  unsigned int   SAL_LaserPeriod       ;       // 80
  unsigned short EstimatedDiveAngle    ;       // 84
  unsigned short EstimatedAttackHeading;       // 86
  unsigned short EstimatedImpactSpeed  ;       // 88
  unsigned char  EFUZE_Data[12]        ;       // 90
  unsigned short Spare2[11]            ;       // 102
} sWeaponHealthAndStatus;

typedef struct
{
  float          LaserFreq         ;           // 124
  float          GPS_TimeOfWeek    ;           // 128
  float          WeaponTime        ;           // 132
  unsigned char  ServoInputVoltage ;           // 136
  signed char    PitotTubeTemp     ;           // 137
  signed char    PressureSensTemp  ;           // 138
  unsigned char  WiFi_RSSI         ;           // 139
  unsigned short TotalPressure     ;           // 140
  unsigned short StaticPressure    ;           // 142
  float          WeaponLatitude    ;           // 144
  float          WeaponLongitude   ;           // 148
  float          WeaponAltitude    ;           // 152
  unsigned short CAS               ;           // 156
  short          LLLN_Heading      ;           // 158
  short          LLLN_Pitch        ;           // 160
  short          LLLN_Roll         ;           // 162
  unsigned char  Discretes         ;           // 164
  unsigned char  NavStatus         ;           // 165
  unsigned char  MCP_State         ;           // 166
  unsigned char  LAN_ReconCnt      ;           // 167
  unsigned int   PitotCBIT         ;           // 168
  unsigned int   SeekerCBIT        ;           // 172
  unsigned short ITUC_CBIT         ;           // 176
  unsigned short MCP_CBIT          ;           // 178
  unsigned short WLinkCBIT         ;           // 180
  unsigned char  ServoA_Status     ;           // 182
  unsigned char  ServoB_Status     ;           // 183
  unsigned char  ServoC_Status     ;           // 184
  unsigned char  ServoD_Status     ;           // 185
  unsigned char  GNSS_GPS_UsedCnt  ;           // 186
  unsigned char  GNSS_GLO_UsedCnt  ;           // 187
  unsigned char  EnableUserLog     ;           // 188
  unsigned char  Spare3            ;           // 189
  unsigned char  Spare4            ;           // 190
  unsigned char  Spare5            ;           // 191
  unsigned int   ActualWeaponCBIT  ;           // 192
  unsigned int   Spare6[2]         ;           // 196 - 203
} sWeaponTelemetry;

typedef struct
{
  unsigned int   LAR_Radius          ;       // 12 (LAR Parameters)
  int            CrossDistance       ;       // 16 (LAR Parameters)
  int            DownDistance        ;       // 20 (LAR Parameters)
  unsigned short RelBearingLAR       ;       // 24 (LAR Parameters)
  unsigned short RelBearingTarget    ;       // 26 (LAR Parameters)
  unsigned int   GroundDistLAR       ;       // 28 (LAR Parameters)
  unsigned int   GroundDistTarget    ;       // 32 (LAR Parameters)
  unsigned int   RadiusExclTarget    ;       // 36 (LAR Parameters)
  unsigned short ExpectedImpactTime  ;       // 40 (LAR Parameters)
  unsigned short TimeToIluminate     ;       // 42 (LAR Parameters)
  unsigned short TimeToGo            ;       // 44 (LAR Parameters)
  unsigned short Spare1              ;       // 46 (LAR Parameters)
  unsigned short WeaponFlags         ;       // 48 (Weapon States)
  unsigned short SeekerStatus        ;       // 50 (Weapon States)
  unsigned int   WeaponCBIT          ;       // 52 (Weapon States)
  unsigned char  SpareBattCapacity   ;       // 56 (Weapon States)
  unsigned char  Spare2              ;       // 57 (Weapon States)
  unsigned short MissionParChecksum  ;       // 58 (Weapon States)
  unsigned short WindParChecksum     ;       // 60 (Weapon States)
  unsigned short IIRImageChecksum    ;       // 62 (Weapon States)
  unsigned short IIRNumTemplates     ;       // 64 (Weapon States)
  unsigned char  SWSuiteMajorVerNum  ;       // 66 (Weapon States)
  unsigned char  Spare3              ;       // 67 (Weapon States)
  unsigned short EstAttackDiveAngle  ;       // 68 (Weapon States)
  unsigned short EstAttackHeading    ;       // 70 (Weapon States)
  unsigned short EstImpactSpeed      ;       // 72 (Weapon States)
  int            TargetLatitude      ;       // 74 (Weapon States)
  int            TargetLongitude     ;       // 78 (Weapon States)
  int            TargetAltitude      ;       // 82 (Weapon States)
  unsigned int   SAL_LaserPeriod     ;       // 86 (Weapon States)
  unsigned short EFUZEParameters[6]  ;       // 90 (Weapon States)
  unsigned short Spare[11]           ;       // 102 (Weapon States)
} sWeaponHealthAndStatusDummy;

typedef struct
{
  float          SALFrequency        ;       // 124 (Telemetry Data)
  float          GPSTimeWeek         ;       // 128 (Telemetry Data)
  float          WeaponTime          ;       // 132 (Telemetry Data)
  unsigned char  ServoMotorInputVolt ;       // 136 (Telemetry Data)
  unsigned char  ServoSupplyVoltage  ;       // 137 (Telemetry Data)
  unsigned char  WiFiSupplyVoltage   ;       // 138 (Telemetry Data)
  unsigned char  RSSIByWeapon        ;       // 139 (Telemetry Data)
  unsigned char  RSSIByTablet        ;       // 140 (Telemetry Data)
  unsigned char  RemainingBattCap    ;       // 141 (Telemetry Data)
  unsigned char  AverageBattCurrent  ;       // 142 (Telemetry Data)
  unsigned char  ServoCBIT           ;       // 143 (Telemetry Data)
  unsigned short FinLockCamPosition  ;       // 144 (Telemetry Data)
  short          TailFin1Position    ;       // 146 (Telemetry Data)
  short          TailFin2Position    ;       // 148 (Telemetry Data)
  short          TailFin3Position    ;       // 150 (Telemetry Data)
  short          TailFin4Position    ;       // 152 (Telemetry Data)
  char           Servo1MotorCurrent  ;       // 154 (Telemetry Data)
  char           Servo2MotorCurrent  ;       // 155 (Telemetry Data)
  char           Servo3MotorCurrent  ;       // 156 (Telemetry Data)
  char           Servo4MotorCurrent  ;       // 157 (Telemetry Data)
  unsigned short GCAPressure1        ;       // 158 (Telemetry Data)
  unsigned short GCAPressure2        ;       // 160 (Telemetry Data)
  int            WeaponLatitude      ;       // 162 (Telemetry Data)
  int            WeaponLongitude     ;       // 166 (Telemetry Data)
  int            WeaponAltitude      ;       // 170 (Telemetry Data)
  short          WeaponHeading       ;       // 174 (Telemetry Data)
  short          WeaponPitch         ;       // 176 (Telemetry Data)
  short          WeaponRoll          ;       // 178 (Telemetry Data)
  unsigned short DiscreteStates      ;       // 180 (Telemetry Data)
  unsigned short WeaponStates        ;       // 182 (Telemetry Data)
  unsigned char  UBloxSatellites     ;       // 184 (Telemetry Data)
  unsigned char  ArcnetReconCount    ;       // 185 (Telemetry Data)
  unsigned int   SeekerCBIT          ;       // 186 (Telemetry Data)
  unsigned short GCACBIT             ;       // 190 (Telemetry Data)
  unsigned short WiFiCBIT            ;       // 192 (Telemetry Data)
  unsigned short ServoSBIT           ;       // 194 (Telemetry Data)
  unsigned short WiFiSBIT            ;       // 196 (Telemetry Data)
  unsigned int   SeekerSBIT          ;       // 198 (Telemetry Data)
  short          SALTargetYaw        ;       // 202 (Telemetry Data)
  short          SALTargetPitch      ;       // 204 (Telemetry Data)
  short          IMUAngularRateX     ;       // 206 (Telemetry Data)
  short          IMUAngularRateY     ;       // 208 (Telemetry Data)
  short          IMUAngularRateZ     ;       // 210 (Telemetry Data)
  short          IMUAccelerationX    ;       // 212 (Telemetry Data)
  short          IMUAccelerationY    ;       // 214 (Telemetry Data)
  short          IMUAccelerationZ    ;       // 216 (Telemetry Data)
  unsigned char  AvgGPSSatellites    ;       // 218 (Telemetry Data)
  unsigned char  AvgGLOSatellites    ;       // 219 (Telemetry Data)
  unsigned char  WPN28VLevel         ;       // 220 (Telemetry Data)
  unsigned char  MissionMode         ;       // 221 (Telemetry Data)
  unsigned short WindAltitudeGuid    ;       // 222 (Telemetry Data)
  unsigned char  WindDirectionGuid   ;       // 224 (Telemetry Data)
  unsigned char  WindSpeedGuid       ;       // 225 (Telemetry Data)
  unsigned short ServoSwCRC          ;       // 226 (Telemetry Data)
  unsigned short MCPSwCRC            ;       // 228 (Telemetry Data)
  unsigned short IFPCBSwCRC          ;       // 230 (Telemetry Data)
  unsigned short WirelessLinkCRC     ;       // 232 (Telemetry Data)
  unsigned short SeekerCRC           ;       // 234 (Telemetry Data)
  unsigned char  Spare[16]           ;       // 236 (Telemetry Data)
} sWeaponTelemetryDummy;

typedef struct
{
  unsigned short SW_Version;      // Version * 100
  unsigned short SW_CRC;
  unsigned short FW_Version;      // Version * 100
  unsigned short FW_CRC;
  char PartNo[11];                // Strings include 0 terminator
  char IssueNo[4];
  char SerialNo[9];
} sLogInfo;

typedef struct
{
  unsigned short Version;
  unsigned short CRC;
  unsigned int Size;
  unsigned char Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Minute;
  unsigned char Spare[3];
} sLoaderMetaData;

typedef struct
{
  TIME SystemTime;                          // [us]
  TIME SeekerSyncEventTime;                 // [us]
  TIME TimeAtLaunch;                        // [us]
  //sMission Mission;
  //sWind Wind;
  unsigned int Weapon600HzCnt;
  unsigned int Weapon100HzTimestamp;        // [us]
  unsigned int Weapon100HzCnt;
  unsigned int Weapon2HzTimestamp;          // [us]
  unsigned int Weapon2HzCnt;
  unsigned int Weapon10HzCnt;
  unsigned int TimeSinceLaunch;             // [us]
  unsigned int ServoTest;
  uGNSS_Usage GNSS_Usage;
  unsigned char BatteryCapacity;            // [%]
  unsigned short BatteryTimeRemaining;      // [s]
  unsigned char ActivateBattery;
  unsigned char ConfigureUBLOX_Receiver;
  unsigned char ConfigureUBLOX_ReceiverCnt;
  unsigned char UseAircraftLAR;
  unsigned char InsideAircraftLAR;
  unsigned char ActiveServos;
  unsigned short Spare2;
  float Pitch;
  float Yaw;
  float Roll;
  float Latitude;
  float Longitude;
  float Altitude;
  uWeaponDiscretes Discretes;
  uWeaponDiscretesDummy DiscretesDummy;
  uWeaponFlags Flags;
  uWeaponStatus Status;
  uWeaponBIT SBIT;
  uWeaponBIT CBIT;
  sWeaponLAR LAR;
  sWeaponLAR LAR_InRange;
  sAircraftRawMission AircraftRawMission;
  sRawMission RawMission;
  sMission Mission;
  sWind Wind;
  sWeaponHealthAndStatus HealthAndStatus;
  sWeaponTelemetry Telemetry;
  sWeaponHealthAndStatusDummy HealthAndStatusDummy;
  sWeaponTelemetryDummy TelemetryDummy;
  char AuxToCsv;
  unsigned char BatteryFirstActivation;
  unsigned char ArmedNotReady;
} sWeaponInfo;



#pragma pack()


extern volatile sWeaponInfo Weapon;

void WeaponInit(void);
void WeaponUpdate(void);
void WeaponCalculateP32_LAR(sWeaponLAR *LAR, unsigned char InRange);
void WeaponGetHealthAndStatus(sWeaponHealthAndStatus *HealthAndStatus);
void WeaponGetTelemetry(sWeaponTelemetry *Telemetry);
void WeaponGetDummyHealthStatusAndTelemetry(sWeaponHealthAndStatusDummy *HealthAndStatusDummy, sWeaponTelemetryDummy *TelemetryDummy);
void WeaponInfoToCsv(void);

#endif
