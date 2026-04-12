#ifndef COMMONTYPES_OUTPUT_H
#define COMMONTYPES_OUTPUT_H
//---------------------------------------------------------------------------
#ifndef MAX_int8_T
#include "rtwtypes.h"
#endif
//---------------------------------------------------------------------------
typedef struct {
  real_T x;
  real_T y;
  real_T z;
} T_Cartesian;
//---------------------------------------------------------------------------
typedef struct {
  real_T phi;
  real_T the;
  real_T psi;
} T_Euler;
//---------------------------------------------------------------------------
typedef struct {
  real_T lat;
  real_T lon;
  real_T alt;
} T_GeographicPos;
//---------------------------------------------------------------------------
typedef struct {
  real_T q0;
  real_T q1;
  real_T q2;
  real_T q3;
} T_Quaternion;
//---------------------------------------------------------------------------
typedef struct {
  real32_T num[2];
  real32_T den[2];
} T_TransferFunction1;
//---------------------------------------------------------------------------
typedef struct {
  real32_T num[3];
  real32_T den[3];
} T_TransferFunction2;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T WingsOpen;
  uint8_T WingsRelease;
  uint8_T SADFuze;
  uint8_T SwitchforBunt;
  uint8_T NavCtrlFlag;
  uint8_T Umbilical;
  uint8_T BatteryOn;
  uint8_T PstaticValid;
} T_GUID_GuidCtrlFlags;
//---------------------------------------------------------------------------
#endif
