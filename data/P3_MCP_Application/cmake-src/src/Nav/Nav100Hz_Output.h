#ifndef NAV100HZ_OUTPUT_H
#define NAV100HZ_OUTPUT_H
//---------------------------------------------------------------------------
#include "commontypes.h"
//---------------------------------------------------------------------------
typedef struct {
  real_T storesize;
  real_T storeplace;
  real_T store[2040];
} T_NAV_Store;
//---------------------------------------------------------------------------
typedef struct {
  real_T systemTime;
  T_Cartesian wib_b;
  T_Cartesian fib_e;
  T_Cartesian grav_e;
  T_Cartesian pos_e;
  T_GeographicPos pos_g;
  real_T baro_alt;
  T_Cartesian vel_e;
  T_Euler att_bn;
  real_T dcm_be[9];
  T_Quaternion q_be;
  real_T deltaT;
  real_T WE;
  T_NAV_Store store;
  T_GUID_GuidCtrlFlags GuidCtrlFlags;
} T_NAV_NavigationState;
//---------------------------------------------------------------------------
typedef struct {
  real_T range;
  real_T rrate;
  real_T ta_position;
  real_T ta_velocity;
  real_T ta_attitude;
  real_T mag_attitude;
  real_T baroG;
  real_T baro_P_N;
} T_NAV_MeasurementNoise;
//---------------------------------------------------------------------------
typedef struct {
  real_T states;
  real_T FeedbackWeight;
  real_T timeupdateINS;
  real_T timeupdateAttitude;
  uint8_T updateINS;
  uint8_T updateAttitude;
  uint16_T Reserved;
  real_T X[16];
  real_T A[256];
  real_T P[256];
  real_T Q[256];
  T_NAV_MeasurementNoise R;
  real_T B_gyro[3];
  real_T B_accel[3];
  uint8_T align_valid;
} T_NAV_KalmanFilterState;
//---------------------------------------------------------------------------
typedef struct {
  real_T SystemTime;
  real_T alt;
  real_T pressure;
  uint8_T valid;
  uint8_T Reserved01;
  uint16_T Reserved02;
  real_T comp_alt;
  real_T KalmanFilter_delta_baro;
} T_NAV_BaroState;
//---------------------------------------------------------------------------
typedef struct {
  uint16_T versionNumberNAV;
  uint16_T NavigationModeOut;
  T_NAV_NavigationState NAV_NavigationState;
  T_NAV_KalmanFilterState NAV_KalmanFilterState;
  T_NAV_BaroState NAV_BaroState;
} T_NAV_State;
//---------------------------------------------------------------------------
typedef struct
{
  uint32_T NAV_SystemTime       ;
  uint32_T NAV_baro_SystemTime  ;
  real32_T NAV_baro_alt         ;
  real32_T NAV_baro_pressure    ;
  uint32_T NAV_Reserved1        ;
  uint32_T NAV_Reserved2        ;
  uint32_T NAV_Reserved3        ;
  uint32_T NAV_100HzTimestamp   ;
  uint32_T NAV_100HzCount       ;
  real32_T NAV_AngPsi           ;
  real32_T NAV_AngTheta         ;
  real32_T NAV_AngPhi           ;
  int32_T  NAV_PosXecef         ;
  int32_T  NAV_PosYecef         ;
  int32_T  NAV_PosZecef         ;
  real32_T NAV_VelXecef         ;
  real32_T NAV_VelYecef         ;
  real32_T NAV_VelZecef         ;
  real32_T NAV_LatLLA           ;
  real32_T NAV_LongLLA          ;
  real32_T NAV_AltLLA           ;
  real32_T NAV_PressAlt         ;
  uint16_T NAV_Mode             ;
  uint16_T NAV_Version          ;
  uint32_T NAV_Align_Timestamp  ;
  uint32_T NAV_Align_CallCount  ;
  real_T   NAV_Align_pos_x      ;
  real_T   NAV_Align_pos_y      ;
  real_T   NAV_Align_pos_z      ;
  real32_T NAV_Align_vel_x      ;
  real32_T NAV_Align_vel_y      ;
  real32_T NAV_Align_vel_z      ;
  real32_T NAV_Align_phi        ;
  real32_T NAV_Align_the        ;
  real32_T NAV_Align_psi        ;
  uint8_T  NAV_Align_valid_pos  ;
  uint8_T  NAV_Align_valid_vel  ;
  uint8_T  NAV_Align_valid_euler;
  uint8_T  NAV_Align_valid      ;
  uint32_T NAV_Align_Reserved1  ;
  uint32_T NAV_Align_Reserved2  ;
  real32_T NAV_CovMatrix[16]    ;
  real32_T NAV_xhatneg_K[16]    ;
  real32_T NAV_BiasGyro  [3]    ;
  real32_T NAV_BiasAccel [3]    ;
  real32_T NAV_BiasBaroAlt      ;
}T_NAV_Tel100Hz_B1;
//---------------------------------------------------------------------------
typedef struct
{
  // Additional NAV 100Hz Output Data
  uint32_T NAV_100HzCountAux;
  real32_T NAV_GeVecXecef   ;
  real32_T NAV_GeVecYecef   ;
  real32_T NAV_GeVecZecef   ;
  real32_T NAV_FeVecXecef   ;
  real32_T NAV_FeVecYecef   ;
  real32_T NAV_FeVecZecef   ;
  real32_T NAV_DCMbe11      ;
  real32_T NAV_DCMbe12      ;
  real32_T NAV_DCMbe13      ;
  real32_T NAV_DCMbe21      ;
  real32_T NAV_DCMbe22      ;
  real32_T NAV_DCMbe23      ;
  real32_T NAV_DCMbe31      ;
  real32_T NAV_DCMbe32      ;
  real32_T NAV_DCMbe33      ;
  real_T   NAV_InitXpos     ;
  real_T   NAV_InitYpos     ;
  real_T   NAV_InitZpos     ;
  real32_T NAV_InitXvel     ;
  real32_T NAV_InitYvel     ;
  real32_T NAV_InitZvel     ;
  real32_T NAV_InitPhi      ;
  real32_T NAV_InitTheta    ;
  real32_T NAV_InitPsi      ;
}T_NAV_Tel100at2Hz;
//---------------------------------------------------------------------------
typedef struct {
  T_NAV_Tel100Hz_B1 T100B1;  // 100Hz Data
  T_NAV_Tel100at2Hz T2Bx  ;  // Additional NAV 100Hz Output Data in 2Hz tel
}T_NAV_Tel100Hz;
//---------------------------------------------------------------------------
#endif
