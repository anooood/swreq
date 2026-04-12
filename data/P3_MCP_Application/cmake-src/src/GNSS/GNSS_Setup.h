#ifndef GNSS_SETUP_H
#define GNSS_SETUP_H
//-------------------------------------------------------------------------------
#ifndef tmwtypes_h
#include "rtwtypes.h"
#endif
//---------------------------------------------------------------------------
//  SystemsToUse: Bitfield indicating which systems to use
//                Bit[0]: GPS
//                Bit[1]: GLONASS
//  MinElev     : [rad]  SVs below this limit will not be used in fix
//  MinSigStr   : [dBHz] SVs below this limit will not be used in fix
//  MinLockTime : [s]    SVs below this limit will not be used in fix
//---------------------------------------------------------------------------
typedef struct {
  uint32_T SystemsToUse;
  real32_T MinElev;
  real32_T MinSigStr;
  real32_T MinLockTime;
  real32_T MinGDOP;
} T_GNSS_Setup;
//---------------------------------------------------------------------------
#endif
