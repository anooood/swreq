#ifndef UBX_H
#define UBX_H
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
  uint32_T iTOW ;
  int32_T  ecefX;
  int32_T  ecefY;
  int32_T  ecefZ;
  uint32_T pAcc ;
} T_UBX_NAV_POSECEF;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW  ;
  int32_T  ecefVX;
  int32_T  ecefVY;
  int32_T  ecefVZ;
  uint32_T sAcc  ;
} T_UBX_NAV_VELECEF;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW;
  int32_T  clkB;
  int32_T  clkD;
  uint32_T tAcc;
  uint32_T fAcc;
} T_UBX_NAV_CLOCK;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW     ;
  int32_T  fTOW     ;
  int16_T  week     ;
  uint8_T  gpsFix   ;
  uint8_T  flags    ;
  int32_T  ecefX    ;
  int32_T  ecefY    ;
  int32_T  ecefZ    ;
  uint32_T pAcc     ;
  int32_T  ecefVX   ;
  int32_T  ecefVY   ;
  int32_T  ecefVZ   ;
  uint32_T sAcc     ;
  uint16_T pDOP     ;
  uint8_T  reserved1;
  uint8_T  numSV    ;
  uint32_T reserved2;
} T_UBX_NAV_SOL;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW;
  int32_T fTOW;
  int16_T week;
  int8_T leapS;
  uint8_T valid;
  uint32_T tAcc;
} T_UBX_NAV_TIMEGPS;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW;
  uint32_T TOD;
  int32_T fTOD;
  uint16_T Nt;
  uint8_T N4;
  uint8_T valid;
  uint32_T tAcc;
} T_UBX_NAV_TIMEGLO;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint8_T gnssId;
  uint8_T svId;
  uint8_T cn0;
  int8_T elev;
  int16_T azim;
  int16_T prRes;
  uint32_T flags;
} T_UBX_NAV_SAT_Entry;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T iTOW;
  uint8_T version;
  uint8_T numSvs;
  uint16_T reserved1;
  T_UBX_NAV_SAT_Entry Entries[28];
} T_UBX_NAV_SAT;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint8_T gnssId;
  uint8_T svId;
  uint8_T reserved1;
  uint8_T freqId;
  uint8_T numWords;
  uint8_T reserved2;
  uint8_T version;
  uint8_T reserved3;
  uint32_T dwrd[10];
} T_UBX_RXM_SFRBX;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  real_T prMes;
  real_T cpMes;
  real32_T doMes;
  uint8_T gnssId;
  uint8_T svId;
  uint8_T reserved2;
  uint8_T freqId;
  uint16_T locktime;
  uint8_T cn0;
  uint8_T prStdev;
  uint8_T cpStdev;
  uint8_T doStdev;
  uint8_T trkStat;
  uint8_T reserved3;
} T_UBX_RXM_RAWX_Entry;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  real_T rcvTow;
  uint16_T week;
  int8_T leapS;
  uint8_T numMeas;
  uint8_T recStat;
  uint8_T reserved1[3];
  T_UBX_RXM_RAWX_Entry Entries[28];
} T_UBX_RXM_RAWX;
//---------------------------------------------------------------------------
typedef SIMULINK_BUS_ALIGN struct {
  uint32_T towMS;
  uint32_T towSubMS;
  int32_T qErr;
  uint16_T week;
  uint8_T flags;
  uint8_T refInfo;
} T_UBX_TIM_TP;
//---------------------------------------------------------------------------
#endif
