/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <TelemetryDef.h>
 *
 *  Description:
 *  ------------
 *  Header file for the TelemetryDef module.
 *
 ******************************************************************************/

#ifndef TELEMETRYDEF_H
#define TELEMETRYDEF_H

#include "Nav100Hz_Output.h"
#include "Nav2Hz_Output.h"
#include "AircraftInterface.h"
#include "AUTOP_Output.h"
#include "GUID_Output.h"
#include "TTKF_Output.h"
#include "DataControl_Output.h"
#include "HOBSComms.h"
#include "MPSAL_Comms.h"
#include "FIFO.h"
#include "QSAL.h"
#include "Mission.h"


#define TEL_VERSION       103


#ifndef _WIN32

#include "Timer.h"
#include "Weapon.h"
#include "Mission.h"
#include "BIT.h"
#include "WiFi.h"

#else

typedef unsigned long long TIME;

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
  double T_meas_GPS;
  double T_meas_GLO;
  double T_meas_Sys;
  double Pos_X;
  double Pos_Y;
  double Pos_Z;
  float Vel_X;
  float Vel_Y;
  float Vel_Z;
  float Clk_Bias_s;
  float Clk_Drift_ss;
  float Clk_tau_GLO_s;
  unsigned char Valid;
  unsigned char Reserved0;
  unsigned char SVsInFix_CntGPS;
  unsigned char SVsInFix_CntGLO;
  unsigned int SVsInFix_GPS;
  unsigned int SVsInFix_GLO;
  unsigned char SV_UserCnt;
  unsigned char NavSV_Cnt;
  unsigned short Reserved1;
} sGNSS_TelResult;

typedef struct
{
  TIME Timestamp;
  unsigned char IMU_Cycle;
  unsigned char BIT_Index;
  unsigned short Spare;
} sBIT_Event;

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
  unsigned short Spare3                ;       // 66
  int            TgtLatitude           ;       // 68
  int            TgtLongitude          ;       // 72
  int            TgtAltitude           ;       // 76
  unsigned int   SAL_LaserPeriod       ;       // 80
  unsigned short EstimatedDiveAngle    ;       // 84
  unsigned short EstimatedAttackHeading;       // 86
  unsigned short EstimatedImpactSpeed  ;       // 88
  unsigned char  EFUZE_Data[12]        ;       // 90
  unsigned short Spare4[11]            ;       // 102
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
  unsigned int   Spare3[4]         ;           // 188
} sWeaponTelemetry;

typedef struct
{
  unsigned char WeaponId;
  unsigned char PageId;
  unsigned char ButtonCounter;
  unsigned char EventType;
  unsigned short ScreenPosX;
  unsigned short ScreenPosY;
  unsigned int Spare;
} sWiFiUserLogEntry;

#endif


#pragma pack(1)


typedef union
{                                                   // 600 Hz 1   600 Hz 2
  unsigned char Bytes[16];                          // 8 - 23     24 - 39
  struct {
    short IMU_AngularRateX;                         // 8          24
    short IMU_AngularRateY;                         // 10         26
    short IMU_AngularRateZ;                         // 12         28
    short IMU_AccelerationX;                        // 14         30
    short IMU_AccelerationY;                        // 16         32
    short IMU_AccelerationZ;                        // 18         34
    unsigned short IMU_RxCnt;                       // 20         36
    unsigned short IMU_ErrorCnt;                    // 22         38
  };
} uTel600Hz;

/*
typedef union
{
  unsigned char Bytes[120];                         // 40 - 159
  struct
  {
    TIME WeaponTime;                                // 40
    unsigned char IMU_Cycle;                        // 48
    unsigned char Task600HzOverrunCnt;              // 49
    unsigned char Task100HzOverrunCnt;              // 50
    unsigned char TaskzHzOverrunCnt;                // 51
    unsigned int Task600HzExecutionTime;            // 52
    unsigned int Task100HzExecutionTime;            // 56
    unsigned int Task2HzExecutionTime;              // 60
    unsigned int Task600HzCount;                    // 64
    unsigned short Task100HzCount;                  // 68
    unsigned short Task2HzCount;                    // 70
    unsigned int FrameCnt;                          // 72
    unsigned int WeaponStatus;                      // 76
    unsigned short WeaponDiscretes;                 // 80
    unsigned short WeaponFlags;                     // 82
    unsigned int TimeSinceLaunch;                   // 84
    short SV_DeflCmd1;                              // 88
    short SV_DeflCmd2;                              // 90
    short SV_DeflCmd3;                              // 92
    short SV_DeflCmd4;                              // 94
    short SV_Feedback1;                             // 96
    short SV_Feedback2;                             // 98
    short SV_Feedback3;                             // 100
    short SV_Feedback4;                             // 102
    signed char SV_Current1;                        // 104
    signed char SV_Current2;                        // 105
    signed char SV_Current3;                        // 106
    signed char SV_Current4;                        // 107
    unsigned char SV_MotorVoltage;                  // 108
    unsigned char SV_SupplyVoltage;                 // 109
    unsigned char SV_Discretes;                     // 110
    unsigned char SV_CBIT;                          // 111
    unsigned short SV_FinLockPos;                   // 112
    signed char SV_Temperature;                     // 114
    signed char SV_FinLocked;                       // 115
    signed char SV_Spare1;                          // 116
    signed char SV_Spare2;                          // 117
    unsigned short SV_Spare3;                       // 118
    unsigned short AC_To_WPN_CommandMsgCnt;         // 120
    unsigned short AC_To_WPN_TA_MsgCnt;             // 122
    unsigned short AC_To_WPN_StatusMsgCnt;          // 124
    unsigned short AC_To_WPN_MissionMsgCnt;         // 126
    unsigned short AC_To_WPN_WindMsgCnt;            // 128
    unsigned short AC_To_WPN_ConfigReqMsgCnt;       // 130
    unsigned short AC_To_WPN_SBIT_MsgCnt;           // 132
    unsigned short WPN_To_AC_SBIT_ReqMsgCnt;        // 134
    unsigned short WPN_To_AC_ConfigMsgCnt;          // 136
    unsigned short WPN_To_AC_StatusMsgCnt;          // 138
    unsigned int Task50HzExecutionTime;             // 140
    unsigned int Task50HzCount;                     // 144
    unsigned char Task50HzOverrunCnt;               // 148
    unsigned char Spare1;                           // 149
    unsigned short Spare2;                          // 150
    unsigned short Spare3[4];                       // 152 - 159
  };
} uTel300Hz;
*/
typedef union
{
  unsigned char Bytes[120];                         // 40 - 159
  struct
  {
    TIME WeaponTime;                                // 40
    unsigned char IMU_Cycle;                        // 48
    unsigned char Task600HzOverrunCnt;              // 49
    unsigned char Task100HzOverrunCnt;              // 50
    unsigned char Task50HzOverrunCnt;               // 51
    unsigned short Task600HzExecutionTime;          // 52
    unsigned short Task100HzExecutionTime;          // 54
    unsigned int Task600HzCount;                    // 56
    unsigned short Task100HzCount;                  // 60
    unsigned short Task50HzCount;                   // 62
    unsigned short Task2HzCount;                    // 64
    unsigned short ServoFrameCnt;                   // 66
    unsigned short ITUC_FrameCnt;                   // 68
    unsigned short PitotFrameCnt;                   // 70
    unsigned int FrameCnt;                          // 72
    unsigned int WeaponStatus;                      // 76
    unsigned short WeaponDiscretes;                 // 80
    unsigned short WeaponFlags;                     // 82
    unsigned int TimeSinceLaunch;                   // 84
    short SV_DeflCmd1;                              // 88
    short SV_DeflCmd2;                              // 90
    short SV_DeflCmd3;                              // 92
    short SV_DeflCmd4;                              // 94
    short SV_Feedback1;                             // 96
    short SV_Feedback2;                             // 98
    short SV_Feedback3;                             // 100
    short SV_Feedback4;                             // 102
    signed char SV_Current1;                        // 104
    signed char SV_Current2;                        // 105
    signed char SV_Current3;                        // 106
    signed char SV_Current4;                        // 107
    unsigned char SV_MotorVoltage;                  // 108
    unsigned char SV_SupplyVoltage;                 // 109
    unsigned char SV_Discretes;                     // 110
    unsigned char SV_CBIT;                          // 111
    unsigned short SV_FinLockPos;                   // 112
    signed char SV_Temperature;                     // 114
    signed char SV_FinLocked;                       // 115
    signed char SV_Spare1;                          // 116
    signed char SV_Spare2;                          // 117
    unsigned short SV_Spare3;                       // 118
    T_GUID_Autop_Tel300Hz AutoPilot300HzTel;        // 120 - 159
  };
} uTel300Hz;


// 100 Hz telemetry blocks

typedef union
{
  unsigned char Bytes[120];                         // 160 - 279
  struct
  {
    unsigned short CRC;                             // 160
  };
} sTel100Hz;

typedef union
{
  unsigned char Bytes[120];                         // 160 - 279
  struct
  {
    unsigned short CRC;                             // 160
    unsigned short Weapon100HzTimestamp;            // 162
    T_GUID_TTKF_Tel100Hz TTKF_Telemetry;            // 164 - 275
    unsigned int IIR_ImageTxTime;                   // 276
  };
} uTel100HzBlock1;


typedef union
{
  unsigned char Bytes[120];                         // 160 - 279
  struct
  {
    unsigned short CRC;                             // 160
    unsigned short Weapon100HzTimestamp;            // 162
    int IMU_DeltaAngleX;                            // 164
    int IMU_DeltaAngleY;                            // 168
    int IMU_DeltaAngleZ;                            // 172
    int IMU_DeltaVelocityX;                         // 176
    int IMU_DeltaVelocityY;                         // 180
    int IMU_DeltaVelocityZ;                         // 184
    unsigned int NAV_SystemTime;                    // 188
    unsigned int NAV_baro_SystemTime;               // 192
    float NAV_baro_alt;                             // 196
    float NAV_baro_pressure;                        // 200
    unsigned int NAV_Reserved1;                     // 204
    unsigned int NAV_Reserved2;                     // 208
    unsigned int NAV_Reserved3;                     // 212
    unsigned int NAV_100HzTimestamp;                // 216
    unsigned int NAV_100HzCount;                    // 220
    float NAV_AngPsi;                               // 224
    float NAV_AngTheta;                             // 228
    float NAV_AngPhi;                               // 232
    int NAV_PosXecef;                               // 236
    int NAV_PosYecef;                               // 240
    int NAV_PosZecef;                               // 244
    float NAV_VelXecef;                             // 248
    float NAV_VelYecef;                             // 252
    float NAV_VelZecef;                             // 256
    float NAV_LatLLA;                               // 260
    float NAV_LongLLA;                              // 264
    float NAV_AltLLA;                               // 268
    float NAV_PressAlt;                             // 272
    unsigned short NAV_Mode;                        // 276
    unsigned short NAV_Version;                     // 278
  };
} uTel100HzBlock2;

typedef union
{
  unsigned char Bytes[120];                         // 160 - 279
  struct
  {
    unsigned short CRC;                             // 160
    unsigned short Weapon100HzTimestamp;            // 162
    unsigned int NAV_Align_Timestamp;               // 164
    unsigned int NAV_Align_CallCount;               // 168
    double NAV_Align_pos_x;                         // 172
    double NAV_Align_pos_y;                         // 180
    double NAV_Align_pos_z;                         // 188
    float NAV_Align_vel_x;                          // 196
    float NAV_Align_vel_y;                          // 200
    float NAV_Align_vel_z;                          // 204
    float NAV_Align_phi;                            // 208
    float NAV_Align_the;                            // 212
    float NAV_Align_psi;                            // 216
    unsigned char NAV_Align_valid_pos;              // 220
    unsigned char NAV_Align_valid_vel;              // 221
    unsigned char NAV_Align_valid_euler;            // 222
    unsigned char NAV_Align_valid;                  // 223
    unsigned int NAV_Reserved1;                     // 224
    unsigned int NAV_Reserved2;                     // 228
    unsigned int NAV_Reserved3;                     // 232
    unsigned int NAV_Reserved4;                     // 236
    unsigned int NAV_Reserved5;                     // 240
    T_GUID_Guid_Tel100Hz GuidTel100Hz;              // 244 - 275
    unsigned int Spare;                             // 276 - 279
  };
} uTel100HzBlock3;


// 20 Hz telemetry blocks

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
  };
} sTel20Hz;

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned short Spare;                           // 282
    unsigned int   NAV_2HzTimestamp  ;              // 284
    unsigned int   NAV_2HzCount      ;              // 288
    float          NAV_CovMatrix[16] ;              // 292
    float          NAV_xhatneg_K[16] ;              // 356
    float          NAV_BiasGyro[3]   ;              // 420
    float          NAV_BiasAccel[3]  ;              // 432
    float          NAV_BiasBaroAlt   ;              // 444
    unsigned int   NAV_StoreTimeReq  ;              // 448
    unsigned char  NAV_StoreCase     ;              // 452
    unsigned char  NAV_StoreIdxLow   ;              // 453
    unsigned char  NAV_StoreIdxHi    ;              // 454
    unsigned char  NAV_StoreReserved1;              // 455
    unsigned int   NAV_StoreTimeLow  ;              // 456
    unsigned int   NAV_StoreTimeHi   ;              // 460
    int            NAV_StorePos[3]   ;              // 464
    unsigned int   NAV_StoreReserved2;              // 476
  };
} uTel20HzBlock1;

typedef union
{
  unsigned char Data[200];                          // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned short Spare;                           // 282
    T_GUID_Guid_Tel20HzA GuidTel20HzA;              // 284 - 479
  };
} uTel20HzBlock2;

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned short Spare;                           // 282
    T_GUID_Guid_Tel20HzB GuidTel20HzB;              // 284 - 407
    unsigned int Spare854;                          // 408
    unsigned int Guidance10HzCnt;                   // 412
    T_Weapon_MCP_LAR GuidLAR;                       // 416 - 451
    T_GUID_Autop_Tel20Hz GUID_Autop_Tel20Hz;        // 452 - 479
  };
} uTel20HzBlock3;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned short Spare;                           // 282
    unsigned int AC_TA_Timestamp;                   // 284
    sAircraftTransferAlignment AC_TA;               // 288 - 347
    unsigned int AC_MissionTimestamp;               // 348
    unsigned short AC_MissionCRC;                   // 352
    unsigned short AC_MissionSpare1;                // 354
    sAircraftRawMission AC_Mission;                 // 356 - 443
    unsigned char HOBS_Mode;                        // 444
    unsigned char HOBS_BurstHeight;                 // 445
    uHOBS_SBIT HOBS_SBIT;                           // 446
    uHOBS_CBIT HOBS_CBIT;                           // 447
    unsigned char HOBS_EstImpactAngle;              // 448
    unsigned char HOBS_WarningSignalWaveform;       // 449
    unsigned short HOBS_WeaponSpeed;                // 450
    short HOBS_OutputDetectorLevel;                 // 452
    unsigned short HOBS_TrackDetectorHeight;        // 454
    short HOBS_PSU_PCB__Temp;                       // 456
    unsigned char HOBS_TrackGPW_HeightSetting;      // 458
    unsigned char HOBS_CoLocationID;                // 459
    short HOBS_PSU_InputVoltage;                    // 460
    int HOBS_TrackHeight;                           // 462
    uHOBS_TestSettings HOBS_Status_TestSettings;    // 466
    uHOBS_TestSettings HOBS_Track_TestSettings;     // 467
    unsigned int HOBS_TrackTimestamp;               // 468
    int HOBS_TrackSpeed;                            // 472
    unsigned short HOBS_PSU_10V;                    // 476
    unsigned short HOBS_PSU_5V;                     // 478
  };
} uTel20HzBlock4;
typedef union
{
	unsigned char Bytes[28];
	struct
	{
		unsigned short MPSAL_DesignatorType;         // 348
		unsigned short MPSAL_PRFaccuracy;            // 350
		bMPSAL_BITResults MPSAL_SBIT;                // 352
		bMPSAL_BITResults MPSAL_CBIT;                // 354
		unsigned short MPSAL_DetectionStatus;        // 356
		unsigned short MPSAL_SensorTempValue;        // 358
		short 		   MPSAL_SeekerRoll;             // 360
		short          MPSAL_SeekerYaw;              // 362
		short          MPSAL_SeekerPitch;            // 364
		unsigned short MPSAL_LaserJitter;            // 366
		unsigned int   MPSAL_DesignatorCode;         // 368
		unsigned short MPSAL_TimeStamp;              // 372
		unsigned short MPSAL_FrameCount;             // 374

	};
	struct
	{
		unsigned short 		 QSAL_RangeCmd;			 // 348
		unsigned short 		 QSAL_PRIaccuracy_us;	 // 350
		sMCP_QSAL_CBITStatus QSAL_CBIT;              // 352
		unsigned short       QSAL_DetectionStatus;   // 356
		unsigned short       QSAL_SensorTempValue;   // 358
		short                Unused1;				 // 360
		short                QSAL_TargetYaw;         // 362
		short                QSAL_TargetPitch;       // 364
		unsigned short       QSAL_PRIJitter_ns;      // 366
		unsigned int         QSAL_PRI;               // 368
		unsigned short       QSAL_RxTimeStamp;       // 372
		unsigned short       QSAL_RxCount;           // 374
	};

}uSALSeeker_TelInfo;
typedef union                                       // 280 - 479
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned short Spare;                           // 282
    unsigned int   WeaponCBIT;                      // 284
    unsigned int   Spare1003;                       // 288
    unsigned short Spare1004;                       // 292
    unsigned short WeaponNodesOnNet;                // 294
    unsigned short WeaponReconCnt;                  // 296
    unsigned short ServoCommsErrCnt;                // 298
    unsigned short WL_CommsErrCnt;                  // 300
    unsigned short SeekerCommsErrCnt;               // 302
    unsigned short UBLOX_ErrCnt;                    // 304
    unsigned short Spare1011;                       // 306
    unsigned short BatteryPackCommsErrCnt;          // 308
    unsigned short HOBS_CommsErrCnt;                // 310
    unsigned short IF_SW_Version;                   // 312
    unsigned short IF_FuzeStatus;                   // 314
    unsigned short MCP_CBIT;                        // 316
    unsigned short Spare1017;                       // 318
    short          PSU_5V;                          // 320
    short          PSU_1V;                          // 322
    short          PSU_1V35;                        // 324
    short          PSU_1V8;                         // 326
    short          PSU_3V3;                         // 328
    signed char    MCP_Temperature;                 // 330
    unsigned char  IF_StatusRxCnt;                  // 331
    unsigned char  IF_ACK_RxCnt;                    // 332
    unsigned char  IF_NACK_RxCnt;                   // 333
    unsigned char  IF_SetDelayTxCnt;                // 334
    unsigned char  IF_RequestStatusTxCnt;           // 335
    unsigned char  IF_FuzeType;                     // 336
    unsigned char  IF_FuzeMode;                     // 337
    unsigned char  IF_FuzeDelay;                    // 338
    unsigned char  IF_UpdatingFuze;                 // 339
    unsigned short GNSS_GPS_AveSatStrength;         // 340
    unsigned short GNSS_GLO_AveSatStrength;         // 342
    unsigned char  GNSS_GPS_Used;                   // 344
    unsigned char  GNSS_GLO_Used;                   // 345
    unsigned char  GNSS_GPS_SatUsedCnt;             // 346
    unsigned char  GNSS_GLO_SatUsedCnt;             // 347
    uSALSeeker_TelInfo  SALSeeker_TelInfo;          // 348-375
    unsigned short WL_ArcnetErrorCnt;               // 376
    unsigned short WL_TabletErrorCnt;               // 378
    unsigned short WL_CBIT;                         // 380
    unsigned char  WL_StatusFlags;                  // 382
    unsigned char  WL_MyReconCnt;                   // 383
    unsigned char  WL_RSSI;                         // 384
    signed char    WL_Temperature;                  // 385
    unsigned char  WL_State;                        // 386
    unsigned char  WL_WeaponId;                     // 387
    unsigned short WL_PIC_3V3;                      // 388
    unsigned short WL_Main3V3;                      // 390
    int            PressSensTemperature1;           // 392
    unsigned char  BAT_HeaterCtrl;                  // 396
    signed char    BAT_TempSetPoint;                // 397
    unsigned short BAT_Capacity;                    // 398
    unsigned short BAT_CapacityUsed;                // 400
    unsigned char  BAT_Activations;                 // 402
    signed char    BAT_Temp1;                       // 403
    signed char    BAT_Temp2;                       // 404
    signed char    BAT_Temp3;                       // 405
    signed char    BAT_Temp4;                       // 406
    signed char    BAT_Temp5;                       // 407
    signed char    BAT_Temp6;                       // 408
    signed char    BAT_Temp7;                       // 409
    signed char    BAT_Temp8;                       // 410
    unsigned char  BAT_Current1;                    // 411
    unsigned char  BAT_Current2;                    // 412
    unsigned char  BAT_Current3;                    // 413
    unsigned char  BAT_Current4;                    // 414
    unsigned char  BAT_Current5;                    // 415
    unsigned int   PressSensorUsed;                 // 416
    unsigned int   PressSensTimestamp;              // 420
    float          PressSensPressure;               // 424
    float          PressSensTemperature;            // 428
    unsigned int   DebugInt1;                       // 432
    int	           PressSensPressure1;              // 436
    int	           PressSensPressure2;              // 440
    unsigned int   ServoRxCnt;                      // 444
    unsigned int   WL_RxCnt;                        // 448
    unsigned int   SeekerRxCnt;                     // 452
    unsigned int   ITU_RxCnt;                       // 456
    unsigned int   PitotRxCnt;                      // 460
    unsigned int   BatteryPackRxCnt;                // 464
    unsigned int   HOBS_RxCnt;                      // 468
    unsigned int   UBLOX_RxCnt;                     // 472
    int            PressSensTemperature2;           // 476
  };
} uTel20HzBlock5;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
  };
} uTel20HzBlock6;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
  };
} uTel20HzBlock7;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
  };
} uTel20HzBlock8;

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    unsigned char Spare1;                           // 284
    unsigned char Spare2;                           // 285
    unsigned short PowerCycleCnt;                   // 286
    unsigned int MCP_ETI;                           // 288
    unsigned int MCP_PLD_StatusReg;                 // 292
    unsigned int WeaponSBIT;                        // 296
    unsigned short MCP_SBIT;                        // 300
    unsigned short WL_SBIT;                         // 302
    unsigned short ITUC_SBIT;                       // 304
    unsigned short HOBS_SBIT;                       // 306
    unsigned int SeekerSBIT;                        // 308
    unsigned int ServoSBIT;                         // 312
    unsigned int PitotSBIT;                         // 316
    unsigned short BatteryPackSBIT;                 // 320
    unsigned short Spare4;                          // 322
    unsigned int Spare5;                            // 324
    unsigned int Spare6;                            // 328
    unsigned char WeaponType;                       // 332    0 = AL TARIQ, 1 = P32
    unsigned char BombType;                         // 333    0 = MK81, 1 = MK82, 2 = MK84
    unsigned char SeekerType;                       // 334    0 = GNSS, 1 = SAL, 2 = IIR
    unsigned char HOBS_Present;                     // 335
    unsigned char FuzeType;                         // 336
    unsigned char GuidanceAndControlMode;           // 337
    unsigned char DTAC_Present;                     // 338
    unsigned char FuzeMode;                         // 339
    unsigned char FuzeDelay;                        // 340
    unsigned char Spare7;                           // 341
    unsigned short FuzeStatus;                      // 342
    //unsigned int Spare8[35];                        // 340 - 479
  };
} uTel2HzBlock1;

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    sRawMission RawMission;                         // 284 - 395
    unsigned short WindEntryCnt;                    // 396
    unsigned short WindCRC;                         // 398
    sWindEntry WindEntries[8];                      // 400
    float MissionLatitude;                          // 448
    float MissionLongitude;                         // 452
    float MissionAltitude;                          // 456
    unsigned char MissionSource;                    // 460
    unsigned char MissionSetResult;                 // 461
    unsigned char Spare2;                           // 462
    unsigned char Spare3;                           // 463
    unsigned short Spare4[4];                       // 464 - 479
  };
} uTel2HzBlock2;

typedef union
{
  unsigned char Bytes[200];                         // 280 - 479
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    unsigned int TelListVersion;                    // 284
    sLogInfo MCP_LogInfo;                           // 288
    sLogInfo ServoLogInfo;                          // 320
    sLogInfo WiFiLogInfo;                           // 352
    sLogInfo ITU_LogInfo;                           // 384
    sLogInfo SeekerLogInfo;                         // 416
    sLogInfo BatteryPackLogInfo;                    // 448
  };
} uTel2HzBlock3;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    unsigned int GuidanceSVN_Revision;              // 284
    sLogInfo PitotLogInfo;                          // 288
    sLogInfo HOBS_LogInfo;                          // 320
    unsigned short DataFileCRC;                     // 352
    unsigned short DataFileVersion;                 // 354
    unsigned char DataFileStructId;                 // 356
    unsigned char DataFileLoaded;                   // 357
    unsigned char DataFileValid;                    // 358
    unsigned char WeaponDefIndex;                   // 359
    unsigned int WeaponDefCRC;                      // 360
    unsigned short WeaponDefVersion;                // 364
    unsigned char WeaponDefBombType;                // 366
    unsigned char WeaponDefWeaponType;              // 367
    unsigned int WeaponDefCalculatedCRC;            // 368
    unsigned int WeaponDefSize;                     // 372
    unsigned int WeaponDefSizeInDataFileHeader;     // 376
    unsigned int WeaponDefStartID;                  // 380
    unsigned int Spare3[24];                        // 384 - 479
  };
} uTel2HzBlock4;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    T_NAV_Tel100at2Hz NAV_Tel100at2Hz;              // 284
  };
} uTel2HzBlock5;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    unsigned int Spare[49];                         // 284 - 479
  };
} uTel2HzBlock6;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    sBIT_Event MCP_CBIT_Events[8];                  // 284
    sBIT_Event WeaponCBIT_Events[8];                // 280
    unsigned short MCP_CBIT_EventCnt;               // 476
    unsigned short WeaponCBIT_EventCnt;             // 478
  };
} uTel2HzBlock7;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    sWiFiUserLogEntry WC_UserLogEntries[8];         // 284 - 379
    unsigned char Spare[100];                       // 380 - 479
  };
} uTel2HzBlock8;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
  };
} uTel2HzBlock9;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;                             // 280
    unsigned char ConstID;                          // 282
    unsigned char Tel2HzIndex;                      // 283
    sWeaponHealthAndStatus WPN_HS;                  // 284 - 395
    sWeaponTelemetry WPN_HS_Telemetry;              // 396 - 475
    unsigned int WPN_HS_Spare10;                    // 476
  };
} uTel2HzBlock10;

typedef union
{
  unsigned char Bytes[200];
  struct
  {
    unsigned short CRC;
    unsigned char ConstID;
    unsigned char Tel2HzIndex;
  };
  uTel2HzBlock1 Tel2HzBlock1;
  uTel2HzBlock2 Tel2HzBlock2;
  uTel2HzBlock3 Tel2HzBlock3;
  uTel2HzBlock4 Tel2HzBlock4;
  uTel2HzBlock5 Tel2HzBlock5;
  uTel2HzBlock6 Tel2HzBlock6;
  uTel2HzBlock7 Tel2HzBlock7;
  uTel2HzBlock8 Tel2HzBlock8;
  uTel2HzBlock9 Tel2HzBlock9;
  uTel2HzBlock10 Tel2HzBlock10;
} uTel20HzBlock9;

typedef struct
{
  unsigned char Tel100HzIndex;                      // 0
  unsigned char Tel20HzIndex;                       // 1
  unsigned short Tel600HzAnd300HzCRC;               // 2
  unsigned int CRC32;                               // 4
  uTel600Hz Tel600Hz_1;                             // 8 - 23
  uTel600Hz Tel600Hz_2;                             // 24 - 39
  uTel300Hz Tel300Hz;                               // 40 - 159
  union
  {
    sTel100Hz Tel100Hz;                             // 160 - 279
    uTel100HzBlock1 Tel100HzBlock1;
    uTel100HzBlock2 Tel100HzBlock2;
    uTel100HzBlock3 Tel100HzBlock3;
  };
  union
  {
    sTel20Hz Tel20Hz;                               // 280 - 479
    uTel20HzBlock1 Tel20HzBlock1;
    uTel20HzBlock2 Tel20HzBlock2;
    uTel20HzBlock3 Tel20HzBlock3;
    uTel20HzBlock4 Tel20HzBlock4;
    uTel20HzBlock5 Tel20HzBlock5;
    uTel20HzBlock6 Tel20HzBlock6;
    uTel20HzBlock7 Tel20HzBlock7;
    uTel20HzBlock8 Tel20HzBlock8;
    uTel20HzBlock9 Tel20HzBlock9;
    sTel20Hz Tel20HzBlock10;
    sTel20Hz Tel20HzBlock11;
    sTel20Hz Tel20HzBlock12;
    sTel20Hz Tel20HzBlock13;
    sTel20Hz Tel20HzBlock14;
    sTel20Hz Tel20HzBlock15;
  };
} sTelemetry;

#pragma pack()

#endif
