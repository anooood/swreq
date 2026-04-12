#ifndef NAV100HZ_COMMON_H
#define NAV100HZ_COMMON_H
//---------------------------------------------------------------------------
#include "commontypes.h"
//---------------------------------------------------------------------------
typedef struct {
  real_T SystemTime;
  T_Cartesian dangle;
  T_Cartesian dvelocity;
  real_T baro_SystemTime;
  real_T baro_alt;
  real_T baro_pressure;
  T_GUID_GuidCtrlFlags GuidCtrlFlags;
} T_NAV_GenericInertialNavigationSensor;
//---------------------------------------------------------------------------
typedef struct {
  real_T attitude;
  real_T rate;
  real_T velocity;
  real_T position;
} T_TA_TimeLatency;
//---------------------------------------------------------------------------
typedef struct {
  T_TA_TimeLatency time;
  T_Cartesian rate;
  T_Euler attitude;
  T_Euler ac_attitude;
  T_GeographicPos pos_g;
  T_Cartesian pos_e;
  T_Cartesian vel_e;
  uint8_T valid;
} T_TA_TransferAlignState;
//---------------------------------------------------------------------------
#endif
