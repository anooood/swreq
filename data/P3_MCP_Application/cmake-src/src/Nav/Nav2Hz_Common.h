#ifndef NAV2HZ_COMMON_H
#define NAV2HZ_COMMON_H
//---------------------------------------------------------------------------
#include "commontypes.h"
//---------------------------------------------------------------------------
typedef struct {
  uint8_T Valid;
  real_T SystemTime;
  T_Cartesian Field;
} T_NAV_MagnetometerMeasurement;
//---------------------------------------------------------------------------
#endif
