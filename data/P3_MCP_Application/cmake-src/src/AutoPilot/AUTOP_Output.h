#ifndef AUTOP_OUTPUT_H
#define AUTOP_OUTPUT_H

#include "rtwtypes.h"

//---------------------------------------------------------------------------
typedef struct {
  uint32_T msg_count;
  uint32_T Spare;
  real_T fin1;
  real_T fin2;
  real_T fin3;
  real_T fin4;
} T_Weapon_MCP_Out_IF103;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T ATP_CycleCntr;
  uint8_T ATP_Flags;
  uint8_T ATP_SigmaF;
  uint8_T ATP_Mach;
  int16_T ATP_Alpha;
  int16_T ATP_Beta;
  uint16_T ATP_NANCntIn;
  uint16_T ATP_NANCntOut;
  int16_T ATP_wbxcmd;
  int16_T ATP_wbyicmd;
  int16_T ATP_wbypcmd;
  int16_T ATP_wbzicmd;
  int16_T ATP_wbzpcmd;
  int16_T ATP_DXCfil;
  int16_T ATP_DXCout;
  int16_T ATP_DZCfil;
  int16_T ATP_DZCout;
  int16_T ATP_DYCfil;
  int16_T ATP_DYCout;
  uint8_T ATP_intYlim;
  uint8_T ATP_intZlim;
  uint8_T ATP_DYClim;
  uint8_T ATP_DZClim;
  uint8_T ATP_NpropX;
  uint8_T ATP_Qratio;
} T_GUID_Autop_Tel300Hz;
//---------------------------------------------------------------------------
typedef struct {
  uint32_T ATP_K_count;
  uint8_T ATP_K_APloop;
  uint8_T ATP_K_AF_ID;
  uint16_T ATP_K_Spare01;
  uint16_T ATP_K_Kp_phi;
  uint16_T ATP_K_Kp_wbx;
  uint16_T ATP_K_Kp_gbz;
  uint16_T ATP_K_Ki_wby;
  uint16_T ATP_K_Kp_wby;
  uint16_T ATP_K_Kp_gby;
  uint16_T ATP_K_Ki_wbz;
  uint16_T ATP_K_Kp_wbz;
  uint16_T ATP_K_RPlagCRC_P;
  uint16_T ATP_K_RRleadCRC_P;
  uint16_T ATP_K_RRlagCRC_P;
  uint16_T ATP_K_AlowCRC_P;
  uint16_T ATP_K_LAleadCRC_P;
  uint16_T ATP_K_LAlagCRC_P;
  uint16_T ATP_K_LWleadCRC_P;
  uint16_T ATP_K_LWlagCRC_P;
  uint16_T ATP_K_RPlagCRC_Y;
  uint16_T ATP_K_RRleadCRC_Y;
  uint16_T ATP_K_RRlagCRC_Y;
  uint16_T ATP_K_AlowCRC_Y;
  uint16_T ATP_K_LAleadCRC_Y;
  uint16_T ATP_K_LAlagCRC_Y;
  uint16_T ATP_K_LWleadCRC_Y;
  uint16_T ATP_K_LWlagCRC_Y;
} T_GUID_Autop_Tel20HzB3;
//---------------------------------------------------------------------------
// Autopilot 20Hz Telem Structure for 1D
typedef struct {
  uint16_T ATP_count;
  uint8_T ATP_mode20;
  uint8_T ATP_AFtype;
  uint8_T ATP_Kp_phi;
  uint8_T ATP_Kp_wbx;
  uint8_T ATP_Kp_gbz;
  uint8_T ATP_Ki_wby;
  uint8_T ATP_Kp_wby;
  uint8_T ATP_Kp_gby;
  uint8_T ATP_Ki_wbz;
  uint8_T ATP_Kp_wbz;
  uint16_T ATP_RPlagCRC;
  uint16_T ATP_RRleadCRC;
  uint16_T ATP_RRlagCRC;
  uint16_T ATP_AlowCRC;
  uint16_T ATP_LAleadCRC;
  uint16_T ATP_LAlagCRC;
  uint16_T ATP_LWleadCRC;
  uint16_T ATP_LWlagCRC;
} T_GUID_Autop_Tel20Hz;
//---------------------------------------------------------------------------
#endif
