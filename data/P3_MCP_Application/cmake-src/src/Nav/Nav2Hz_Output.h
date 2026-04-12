#ifndef NAV2HZ_OUTPUT_H
#define NAV2HZ_OUTPUT_H
//---------------------------------------------------------------------------
#ifndef MAX_int8_T
#include "rtwtypes.h"
#endif
//---------------------------------------------------------------------------
typedef struct {
  real_T state[16];
  real_T covariance[256];
} T_NAV_KalmanFilterStateError;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T gnss;
  uint8_T baro;
  uint8_T ta;
  uint32_T mag;
  uint8_T feedback_nav;
  uint8_T feedback_att;
  uint8_T reset_navigation;
} T_NAV_KFilterFeedbackValidity;
//---------------------------------------------------------------------------
typedef struct {
  real_T nav[16];
  T_NAV_KalmanFilterStateError kalmanfilter;
  uint16_T numbersatellites;
  uint16_T Reserved;
  T_NAV_KFilterFeedbackValidity valid;
  uint32_T trade;
} T_NAV_KFilterErrorEstimateState;
//---------------------------------------------------------------------------
typedef struct {
  uint32_T TimeReq;
  uint8_T Case;
  uint8_T IdxLow;
  uint8_T IdxHi;
  uint8_T Reserved1;
  uint32_T TimeLow;
  uint32_T TimeHi;
  int32_T Pos[3];
  uint32_T Reserved2;
} T_NAV_StoreInfo;
//---------------------------------------------------------------------------
typedef struct {
  real_T MAG_Input_TimeStamp;
  uint32_T MAG_Input_Flags;
  real32_T MAG_Input_FieldMeas[3];
  real32_T MAG_Input_FieldWMM[3];
  real32_T MAG_Input_Attitude[3];
  uint32_T MAG_State_Flags;
  real32_T MAG_State_FieldWMM[3];
  real32_T MAG_State_b[3];
  uint32_T MAG_Output_Valid;
  real32_T MAG_Output_Attitude[3];
  real32_T MAG_Output_P[9];
} T_MAG_Tel;
//---------------------------------------------------------------------------
typedef struct
{
  uint32_T NAV_2HzTimestamp  ;
  uint32_T NAV_2HzCount      ;
  uint32_T NAV_StoreTimeReq  ;
  uint8_T  NAV_StoreCase     ;
  uint8_T  NAV_StoreIdxLow   ;
  uint8_T  NAV_StoreIdxHi    ;
  uint8_T  NAV_StoreReserved1;
  uint32_T NAV_StoreTimeLow  ;
  uint32_T NAV_StoreTimeHi   ;
  int32_T  NAV_StorePos   [3];
  uint32_T NAV_StoreReserved2;
}T_NAV_Tel2Hz;
//---------------------------------------------------------------------------
#endif
