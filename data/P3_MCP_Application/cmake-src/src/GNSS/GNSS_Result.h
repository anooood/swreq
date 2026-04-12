#ifndef GNSS_RESULT_H
#define GNSS_RESULT_H
//---------------------------------------------------------------------------
#ifndef tmwtypes_h
#include "rtwtypes.h"
#endif
//---------------------------------------------------------------------------
#ifdef COMP_6DOF
#define SIMULINK_BUS_ALIGN __declspec(align(8))
#else
#define SIMULINK_BUS_ALIGN
#endif
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  real_T GPS;
  real_T GLO;
  real_T Sys;
} T_GNSS_TimeOfMeas;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint8_T           Valid     ;
  uint8_T           Reserved0 ;
  uint16_T          NoSVsInFix;
  uint32_T          Reserved1 ;
  T_GNSS_TimeOfMeas T_meas    ;
  real_T            Pos[3]    ;
  real32_T          Vel[3]    ;
} T_GNSS_Result;
//---------------------------------------------------------------------------
#endif
