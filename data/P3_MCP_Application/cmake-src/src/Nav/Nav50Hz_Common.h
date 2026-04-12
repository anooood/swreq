#ifndef NAV50HZ_COMMON_H
#define NAV50HZ_COMMON_H
//---------------------------------------------------------------------------
#ifndef tmwtypes_h
#include "rtwtypes.h"
#endif
//---------------------------------------------------------------------------
typedef struct {
  uint32_T ValidityWord;
  real_T Roll;
  real_T Pitch;
  real_T Heading;
  real_T RollRate;
  real_T PitchRate;
  real_T YawRate;
  real_T VelocityNorth;
  real_T VelocityEast;
  real_T VelocityDown;
  real_T Spare1;
  real_T Latitude;
  real_T Longitude;
  real_T Altitude;
  real_T LeverArmX;
  real_T LeverArmY;
  real_T LeverArmZ;
  real_T Latency1;
  real_T Latency2;
  real_T Latency3;
  real_T Latency4;
  real_T TrueAirspeed;
  real_T CalibratedAirspeed;
  real_T StaticPressure;
  real_T AngleOfAttack;
  real_T AngleOfSideslip;
  real_T Spare2;
} T_RTAC_TransferAlignment;
//---------------------------------------------------------------------------
#endif
