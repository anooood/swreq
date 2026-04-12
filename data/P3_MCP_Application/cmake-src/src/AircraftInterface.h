/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <AircraftInterface.h>  - DTAC
 *
 *  Description:
 *  ------------
 *  Header file for Aircraft communication interface - DTAC.
 *
 ******************************************************************************/

#ifndef AIRCRAFTINTERFACE_H_
#define AIRCRAFTINTERFACE_H_


#include "Timer.h"
#include "MCP.h"
#include "Wind.h"

#pragma pack(1)

typedef struct
{
  unsigned short SW_Version;                  // 12
  unsigned short SW_CRC;                      // 14
  unsigned short FW_Version;                  // 16
  unsigned short FW_CRC;                      // 18
  unsigned short LoaderVersion;               // 20
  unsigned short LoaderCRC;                   // 22
  char           AircraftId[20];              // 24
  unsigned char  ConsentActive;               // 44
  unsigned char  Spare;                       // 45
  unsigned short SBIT;                        // 46
} sAircraftSBIT;                              // Size = 36 (without MLAN header)

typedef struct
{
  unsigned char  ReleaseConsent;              // 12
  unsigned char  Mode;                        // 13
  unsigned char  Spare1;                      // 14
  unsigned char  WeaponConfigReqCnt;          // 15
  unsigned int   Timer1;                      // 16
  unsigned int   Timer2;                      // 20
  float          CPU_Usage;                   // 24
  unsigned int   ArcnetRxCnt;                 // 28
  unsigned int   ArcnetTxCnt;                 // 32
  unsigned int   AircraftRxCnt;               // 36
  unsigned int   EventFlags;                  // 40
  unsigned short CtrlRegister;                // 44
  unsigned short OppStatusRegister;           // 46
  unsigned short CmdRegister;                 // 48
  unsigned short IntMaskRegister;             // 50
  unsigned short PendingIntregister;          // 52
  unsigned short BIT_Register;                // 54
  unsigned short StatusRegister;              // 56
  unsigned char  RT_Address;                  // 58
  unsigned char  Spare2;                      // 59
  unsigned char  Spare3;                      // 60
  unsigned char  Spare4;                      // 61
  unsigned short CBIT;                        // 62
} sAircraftStatus;                            // Size = 52 (without MLAN header)

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int Roll                 : 1;    // 0
    unsigned int Pitch                : 1;    // 1
    unsigned int Yaw                  : 1;    // 2
    unsigned int RollRate             : 1;    // 3
    unsigned int PitchRate            : 1;    // 4
    unsigned int YawRate              : 1;    // 5
    unsigned int VelocityNorth        : 1;    // 6      * Required for TA convergence (position valid)
    unsigned int VelocityEast         : 1;    // 7      * Required for TA convergence (position valid)
    unsigned int VelocityDown         : 1;    // 8      * Required for TA convergence (position valid)
    unsigned int Latitude             : 1;    // 9      * Required for TA convergence (position valid)
    unsigned int Longitude            : 1;    // 10     * Required for TA convergence (position valid)
    unsigned int Altitude             : 1;    // 11     * Required for TA convergence (position valid)
    unsigned int LeverArmX            : 1;    // 12
    unsigned int LeverArmY            : 1;    // 13
    unsigned int LeverArmZ            : 1;    // 14
    unsigned int Latency1             : 1;    // 15
    unsigned int Latency2             : 1;    // 16
    unsigned int Latency3             : 1;    // 17
    unsigned int Latency4             : 1;    // 18
    unsigned int TrueAirspeed         : 1;    // 19
    unsigned int CalibratedAirspeed   : 1;    // 20
    unsigned int StaticPressure       : 1;    // 21
    unsigned int AngleOfAttack        : 1;    // 22
    unsigned int AngleOfSideslip      : 1;    // 23
    unsigned int Reserved             : 8;    // 24 - 31
  };
} uTransferAlignmentValidity;

typedef struct
{
  uTransferAlignmentValidity Validity;        // 12
  short Roll;                                 // 16   [rad] LSB = PI / 2 ^ 15
  short Pitch;                                // 18   [rad] LSB = PI / 2 ^ 16
  short Yaw;                                  // 20   [rad] LSB = PI / 2 ^ 15
  short RollRate;                             // 22   [rad/s] LSB = PI / 2 ^ 13
  short PitchRate;                            // 24   [rad/s] LSB = PI / 2 ^ 14
  short YawRate;                              // 26   [rad/s] LSB = PI / 2 ^ 14
  short VelocityNorth;                        // 28   [m/s] LSB = 0.01
  short VelocityEast;                         // 30   [m/s] LSB = 0.01
  short VelocityDown;                         // 32   [m/s] LSB = 0.01
  unsigned short Reserved1;                   // 34
  int Latitude;                               // 36   [rad] LSB = PI / 2 ^ 32
  int Longitude;                              // 40   [rad] LSB = PI / 2 ^ 31
  short Altitude;                             // 44   [m] LSB = 1;
  short LeverArmX;                            // 46   [m] LSB = 0.01
  short LeverArmY;                            // 48   [m] LSB = 0.01
  short LeverArmZ;                            // 50   [m] LSB = 0.01
  unsigned short Latency1;                    // 52   [ms] LSB = 0.1
  unsigned short Latency2;                    // 54   [ms] LSB = 0.1
  unsigned short Latency3;                    // 56   [ms] LSB = 0.1
  unsigned short Latency4;                    // 58   [ms] LSB = 0.1
  unsigned short TrueAirspeed;                // 60   [m/s] LSB = 0.01
  unsigned short CalibratedAirspeed;          // 62   [m/s] LSB = 0.01
  unsigned short StaticPressure;              // 64   [Pa] LSB = 10
  short AngleOfAttack;                        // 66   [rad] LSB = PI / 2 ^ 16
  short AngleOfSideslip;                      // 68   [rad] LSB = PI / 2 ^ 16
  unsigned short Reserved2;                   // 70
} sAircraftTransferAlignment;                 // Size = 60 (without MLAN header)

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short LAR_Control          : 1;  // 0 = use weapon LAR, 1 = use platform LAR
    unsigned short InsideLAR            : 1;
    unsigned short ForseOppertunityMode : 1;
    unsigned short Spare                : 13;
  } Bits;
} uAircraftControl;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int Reserved1            : 1;    // 0
    unsigned int EnArmDelay           : 1;    // 1
    unsigned int Reserved2            : 1;    // 2
    unsigned int FunctHeight          : 1;    // 3
    unsigned int Reserved3            : 6;    // 4
    unsigned int FunctProximity       : 1;    // 10
    unsigned int Reserved4            : 1;    // 11
    unsigned int FunctAltitude        : 1;    // 12
    unsigned int FunctTimeAfterImpact : 1;    // 13
    unsigned int Reserved5            : 1;    // 14
    unsigned int FuncImpact           : 1;    // 15
    unsigned int Reserved6            : 3;    // 16
    unsigned int EnBeforeLaunch       : 1;    // 19
    unsigned int EnAfterLaunch        : 1;    // 20
    unsigned int Reserved7            : 5;    // 21
    unsigned int Init                 : 2;    // 26
    unsigned int Reserved8            : 4;    // 28
  };
} uAircraftFuzeMode;

typedef struct
{
  unsigned short ClearCBIT;                   // 12
  uAircraftControl Control;                   // 14
} sAircraftCommand;                           // Size = 4 (without MLAN header)

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short GoNoGo             : 1;    // 0
    unsigned short WeaponReady        : 1;    // 1
    unsigned short TgtProgrammed      : 1;    // 2
    unsigned short MissionValid       : 1;    // 3
    unsigned short WindValid          : 1;    // 4
    unsigned short BatteryActive      : 1;    // 5
    unsigned short GNSS_Locked        : 1;    // 6
    unsigned short IIR_TemplateValid  : 1;    // 7
    unsigned short WingsPresent       : 1;    // 8
    unsigned short BombType           : 2;    // 9 - 10
    unsigned short SeekerType         : 2;    // 11 - 12
    unsigned short MissionCtrlGeom    : 2;    // 13 - 14
    unsigned short AlignmentStatus    : 1;    // 15
  };
} uAircraftWeaponFlags;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short Mode               : 1;    // 0
    unsigned short Tracking           : 1;    // 1
    unsigned short DetectionValid     : 1;    // 2
    unsigned short AnglesValid        : 1;    // 3
    unsigned short WindowSize         : 1;    // 4
    unsigned short TemperatureState   : 1;    // 5
    unsigned short AnglesLinear       : 1;    // 6
    unsigned short Reserved           : 1;    // 7
    unsigned short GainStage          : 4;    // 8 - 11
    unsigned short HeaterA_On         : 1;    // 12
    unsigned short HeaterB_On         : 1;    // 12
    unsigned short HeaterC_On         : 1;    // 14
    unsigned short HeaterD_On         : 1;    // 15
  };
} uAircraftSAL_StatusFlags;

typedef struct
{
  unsigned int LAR_Rad;                       //  12 [m]      LSB = 1
  int DistX;                                  //  16 [m]      LSB = 1
  int DistY;                                  //  20 [m]      LSB = 1
  int LAR_Latitude;                           //  24 [deg]    LSB = 360 / 2 ^ 32
  int LAR_Longitude;                          //  28 [deg]    LSB = 360 / 2 ^ 32
  unsigned short RelBrngToLAR;                //  32 [deg]    LSB = 1
  unsigned short RelBrngToTGT;                //  34 [deg]    LSB = 1
  unsigned int DistToLAR;                     //  36 [m]      LSB = 1
  unsigned int DistToTGT;                     //  40 [m]      LSB = 1
  unsigned int LAR_Rad_Excl;                  //  44 [m]      LSB = 1
  unsigned short TimeToImpact;                //  48 [s]      LSB = 1
  unsigned short TimeToIllum;                 //  50 [s]      LSB = 1
  unsigned short TimeToGo;                    //  52 [s]      LSB = 1
  unsigned short TimeToExit;                  //  54 [s]      LSB = 1
  uAircraftWeaponFlags WeaponFlags;           //  56
  uAircraftSAL_StatusFlags SAL_Status;        //  58
  unsigned int WeaponBIT;                     //  60
  unsigned char IIR_TransferBusy;             //  64
  unsigned char BatteryCapacity;              //  65 [%]      LSB = 1
  unsigned short BatteryTimeRemaining;        //  66 [s]      LSB = 1
  unsigned short MissionParamsCRC;            //  68
  unsigned short WindParamsCRC;               //  70
  unsigned short IIR_ImageCRC;                //  72
  unsigned char IIR_TemplateCnt;              //  74
  unsigned char SW_MajorVersion;              //  75
  unsigned short Reserved1;                   //  76
  unsigned short EstimatedDiveAngle;          //  78 [deg]    LSB = 1
  unsigned short EstimatedAttackHeading;      //  80 [deg]    LSB = 1
  unsigned short EstimatedImpactSpeed;        //  82 [m/s]    LSB = 1
  int TgtLatitude;                            //  84 [deg]    LSB = 360 / 2 ^ 32    - WGS84
  int TgtLongitude;                           //  88 [deg]    LSB = 360 / 2 ^ 32    - WGS84
  int TgtAltitude;                            //  92 [ft]     LSB = 50000 / 2 ^ 32  - EGM96
  unsigned int SAL_LaserPeriod;               //  96 [us]     LSB = 1
  uAircraftFuzeMode FuzingMode;               // 100
  unsigned short ArmingDelay;                 // 104 [ms]     LSB = 1
  unsigned short ImpactDelay;                 // 106 [ms]     LSB = 1
  unsigned short FuzingDistance;              // 108 [ft]     LSB = 1
  unsigned short FuzeTime1;                   // 110 [ms]     LSB = 2
  unsigned char IIR_ImageNo;                  // 112
  unsigned char Reserved2;                    // 113
  unsigned short AircraftMissionCRC;          // 114
  unsigned int LAR_RadInRange;                // 116 [m]      LSB = 1
  int DistXInRange;                           // 120 [m]      LSB = 1
  int DistYInRange;                           // 124 [m]      LSB = 1
  int LAR_LatitudeInRange;                    // 128 [deg]    LSB = 360 / 2 ^ 32
  int LAR_LongitudeInRange;                   // 132 [deg]    LSB = 360 / 2 ^ 32
  unsigned short RelBrngToLAR_InRange;        // 136 [deg]    LSB = 1
  unsigned short Reserved5;                   // 138
  unsigned int DistToLAR_InRange;             // 140 [m]      LSB = 1
  unsigned int ExclusionRadiusInRange;        // 144 [m]      LSB = 1
  unsigned short TimeToImpactInRange;         // 148 [s]      LSB = 1
  unsigned short TimeToIllumInRange;          // 150 [s]      LSB = 1
  unsigned short TimeToGoInRange;             // 152 [s]      LSB = 1
  unsigned short TimeToExitInRange;           // 154 [s]      LSB = 1
} sAircraftHealthStatus;                      // Size = 144 (without MLAN header)

typedef struct
{
  unsigned char SeekerType;
  unsigned char BombType;
  unsigned char WeaponType;
  unsigned char WingsPresent;
  unsigned char BatteryActivationMethod;
  unsigned char FuzeType;
  unsigned short MaxAirburstHeight;
  unsigned char SW_ICD_Version;
  unsigned char Reserved[3];
} sAircraftWeaponConfig;

typedef struct
{
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
} sAircraftMissionValidityFlags;

typedef struct
{
  int PriTgtLatitude;                         // 12   [deg] LSB = 360 / 2 ^ 32    - GS84
  int PriTgtLongitude;                        // 16   [deg] LSB = 360 / 2 ^ 32    - GS84
  int PriTgtAltitude;                         // 20   [ft]  LSB = 50000 / 2 ^ 32  - EGM96
  unsigned int AttackHeading;                 // 24   [deg] LSB = 1
  unsigned char DiveAngle;                    // 28   [deg] LSB = 1
  unsigned char Reserved1;                    // 29
  unsigned short ImpactSpeed;                 // 30   [m/s] LSB = 1
  uGNSS_Usage GNSS_Usage;                     // 32
  unsigned char Reserved2;                    // 33
  unsigned char AttackHeadingValid;           // 34
  unsigned char SecondarySwitch;              // 35
  int SecTgtLatitude;                         // 36   [deg] LSB = 360 / 2 ^ 32    - GS84
  int SecTgtLongitude;                        // 40   [deg] LSB = 360 / 2 ^ 32    - GS84
  int SecTgtAltitude;                         // 44   [ft]  LSB = 50000 / 2 ^ 32  - EGM96
  unsigned int SAL_Code;                      // 48
  unsigned int SAL_Activation;                // 52
  unsigned short SAL_Accuracy;                // 56   [ns]  LSB = 1
  unsigned short SAL_Jitter;                  // 58   [ns]  LSB = 1
  unsigned short IIR_ImageNo;                 // 60
  unsigned short IIR_ImageOffsetX;            // 62
  unsigned short IIR_ImageOffsetY;            // 64
  unsigned short IIR_ImageCRC;                // 66
  unsigned char WeaponType;                   // 68
  unsigned char MissionType;                  // 69   Bits 0 - 1: Seeker Type, Bits 2 - 4: Bomb Type
  unsigned short Spare;                       // 70
  unsigned short LAR_Track;                   // 72   [deg] LSB = 0.0054932479
  unsigned short LAR_Mach;                    // 74         LSB = 0.0001249961
  int LAR_Altitude;                           // 76   [ft]  LSB = 0.00004656612   - EGM96
  unsigned short LAR_WindSpeed;               // 80   [m/s] LSB = 0.031250954
  unsigned short LAR_WindDirection;           // 82   [deg] LSB = 0.0054932479
  uAircraftFuzeMode FuzingMode;               // 84
  unsigned short ArmingDelay;                 // 88
  unsigned short ImpactDelay;                 // 90   LSB = 10
  unsigned short FuzingDistance;              // 92
  unsigned short FuzeTime;                    // 94   LSB = 2
  sAircraftMissionValidityFlags ValidityFlags;// 96
  unsigned short CRC;                         // 98
} sAircraftRawMission;                        // Size = 88 (without MLAN header)

typedef struct
{
  TIME RxTimestamp;
  unsigned int TransferAlignmentRxTimestamp;
  unsigned int SBIT_RxTimestamp;
  unsigned int StatusRxTimestamp;
  unsigned int ConfigReqRxTimestamp;
  unsigned int CommandRxTimestamp;
  unsigned int MissionRxTimestamp;
  unsigned int WindRxTimestamp;
  unsigned int SBIT_ReqTxTimestamp;
  unsigned int HealthStatusTxTimestamp;
  unsigned int ConfigTxTimestamp;
  unsigned int RxCnt;
  unsigned short SBIT_ReqTxCnt;
  unsigned short SBIT_ResponseRxCnt;
  unsigned int StatusRxCnt;
  unsigned int CommandRxCnt;
  unsigned int TransferAlignmentRxCnt;
  unsigned int MissionRxCnt;
  unsigned int WindRxCnt;
  unsigned short ConfigReqRxCnt;
  unsigned short ConfigTxCnt;
  unsigned int HealthStatusTxCnt;
  unsigned short AircraftMissionCRC;
  unsigned char UpdateLAR;
  unsigned char Spare;
  float Latitude;
  float Longitude;
  float Altitude;
  sAircraftSBIT SBIT;
  sAircraftStatus Status;
  sAircraftCommand Command;
  sAircraftTransferAlignment TransferAlignment;
  sAircraftHealthStatus HealthStatus;
  sAircraftWeaponConfig WeaponConfig;
  sAircraftRawMission RawMission;
  sWind Wind;
} sAircraft;

typedef struct
{
	unsigned short     ErrCnt;
	unsigned char 	   DBASWaitForRxMsg;
} sDBAS_StateSpace;

#pragma pack()

extern sDBAS_StateSpace DBAS_SS;
extern sAircraft Aircraft;

void AircraftInit(char *AuxStr);
void AircraftHandleRxData(unsigned char *RxData, unsigned int Timestamp);
void AircraftSend(void);

#endif /* AIRCRAFTINTERFACE_H_ */
