#ifndef DataControl_OUTPUT_H
#define DataControl_OUTPUT_H

#include "rtwtypes.h"
#include "MCP_Interface.h"

//---------------------------------------------------------------------------
typedef struct {
  uint32_T radius;
  uint16_T Psi2LAR;
  uint16_T Psi2Tgt;
  uint32_T Rge2LAR;
  uint32_T Rge2Tgt;
  uint32_T exclrad;
  uint16_T Time2Imp;
  uint16_T Time2Lase;
  uint16_T Time2Go;
  uint16_T Psi2LAR_Test;
  uint32_T radius_Test;
  uint32_T Rge2LAR_Test;
} T_Weapon_MCP_LAR;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_nav_weaponprop WeaponProp;
} T_Weapon_MCP_DATA_Nav;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T AirframeMode;
  uint8_T AirframeType;
  uint16_T Spare02;
  T_Weapon_MCP_GlobalMemory_guid_AFprop AirframeProp;
  T_Weapon_MCP_GlobalMemory_ttkf_seeker seeker;
} T_Weapon_MCP_DATA_Guid;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_ttkf_seeker seeker;
  T_Weapon_MCP_GlobalMemory_ttkf_setup setup;
  real32_T BallisticRange;
} T_Weapon_MCP_DATA_TTKF;
//---------------------------------------------------------------------------
typedef struct {
  real32_T MachVec[6];
  real32_T AlphaVec[5];
  real32_T Kp_phi[30];
  real32_T Ki_phi[30];
  real32_T Kp_wbx[30];
  real32_T SIGMAfilt;
  real32_T NCP[30];
  real32_T RPlagZ[30];
  real32_T RPlagP[30];
  real32_T NCW[30];
  real32_T RRleadZ[30];
  real32_T RRleadP[30];
  real32_T RRlagZ[30];
  real32_T RRlagP[30];
} T_Weapon_MCP_autop_roll_gains;
//---------------------------------------------------------------------------
typedef struct {
  real32_T MachVec[6];
  real32_T AlphaVec[5];
  real32_T Kpa[30];
  real32_T Kiw[30];
  real32_T Kpw[30];
  real32_T ALPHAfilt;
  real32_T NCA[30];
  real32_T LAlowpass[30];
  real32_T LAleadZ[30];
  real32_T LAleadP[30];
  real32_T LAlagZ[30];
  real32_T LAlagP[30];
  real32_T NCW[30];
  real32_T LWleadZ[30];
  real32_T LWleadP[30];
  real32_T LWlagZ[30];
  real32_T LWlagP[30];
} T_Weapon_MCP_autop_lat_gains;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T AirframeID;
  uint8_T Spare01;
  uint16_T Spare02;
  real32_T defl_limit;
  real32_T t_delt;
  T_Weapon_MCP_autop_roll_gains gainsR;
  T_Weapon_MCP_autop_lat_gains gainsP;
  T_Weapon_MCP_autop_lat_gains gainsY;
} T_Weapon_MCP_DATA_Autop;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_DATA_Nav toNav;
  T_Weapon_MCP_DATA_Guid toGuid;
  T_Weapon_MCP_DATA_TTKF toTTKF;
  T_Weapon_MCP_DATA_Autop toAutop;
} T_Weapon_MCP_DATA;
//---------------------------------------------------------------------------
#endif