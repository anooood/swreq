/*
 * File: Guidance_100Hz_Function_1D.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1501
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 15:34:58 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA C:2012 guidelines
 *    4. Safety precaution
 *    5. Debugging
 * Validation result: Not run
 */

#include "Guidance_100Hz_Function_1D.h"
#include "LIB_getreflvec_9sSePAMS.h"
#include "LIB_getrefposarc_0TuejMUx.h"
#include "LIB_roll2gvec_MgzTYQaE.h"
#include "LIB_unitvector_gdXEDyOJ.h"
#include "LIB_unitvector_jvWMvfXP.h"
#include "cross_6Ajhxtbg.h"
#include "dot_lFwBkxue.h"
#include "dot_tXnwwRPM.h"
#include "interp1_EKJBPReX.h"
#include "interp1_FTXwGFAn.h"
#include "interp1_Gy1LATh1.h"
#include "interp1_Ln1e4hwu.h"
#include "interp1_UImAjR6u.h"
#include "interp1_WUCGY3Hg.h"
#include "interp1_zsuvPUhx.h"
#include "interp2_uQdDEBQV.h"
#include "norm_MfVYifVG.h"
#include "norm_lpbhAiWU.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"
#include "rt_roundd_snf.h"

/* Forward declaration for local functions */
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9]);
static void TransformationsNED(real_T TTKFin_tgtrex, real_T TTKFin_tgtrey,
  real_T TTKFin_tgtrez, real_T TTKFin_tgtrex_uncond, real_T TTKFin_tgtrey_uncond,
  real_T TTKFin_tgtrez_uncond, real_T NAVin_pos_g_lat, real_T NAVin_pos_g_lon,
  real_T NAVin_pos_e_x, real_T NAVin_pos_e_y, real_T NAVin_pos_e_z, real_T
  NAVin_vel_e_x, real_T NAVin_vel_e_y, real_T NAVin_vel_e_z, const T_Euler
  NAVin_att_bn, const real_T REFin[3], real_T RE0_x, real_T RE0_y, real_T RE0_z,
  T_Cartesian *TPos, T_Cartesian *TPos_uncond, T_Cartesian *MPos, T_Cartesian
  *MVel, T_Cartesian *MRef, sPLFBkBfmjj013qOjLWm7GC *DCM);
static void GetAirdataParams(sVUEBEgeCpQlxFVEgYjRFMH *ENV, real_T
  NAVin_pos_g_alt, const T_Cartesian Vned, const sPLFBkBfmjj013qOjLWm7GC *DCM,
  real_T FCin_Temp, real_T FCin_Psta, real_T FCin_Pdyn, real_T ValidGNSSNav,
  real_T IMUin_gbym, real_T IMUin_gbzm, real_T IMUin_wbym, real_T IMUin_wbzm,
  const T_Weapon_MCP_DATA_Guid *fromDATA, real_T ftime, real_T delt, const
  struct_6qvaB88DLLAFHx6ej5JcWG *b_cnsts);
static void SetGuidanceMode(uint8_T *GUIDMODE, real_T TRACK_reset3cnt, real_T
  TRACK_mpos0_x, real_T TRACK_mpos0_y, real_T TRACK_mpos0_z, real_T TRACK_Rwe_x,
  real_T TRACK_Rwe_y, real_T TRACK_Rwe_z, sETPRuSGTG6V05GSmeYzNjH *guidcmds,
  uint8_T AFmode, uint8_T opportunity_mode, real32_T MP_in_DiveAngle, uint16_T
  TTKF_in_confidence, real_T TTKF_in_tgtrge, const T_Cartesian NAVin_pos_e,
  const T_Euler NAVin_att_bn, real_T IMUin_wbxm, uint8_T WeaCfg_in_WingsPresent,
  real_T mach, real_T ftime);
static void GetOptimalFPath(sSZzX5nI8YD60UjueDawhgE *OPT_FP, uint8_T GUIDMODE,
  real_T ENV_rho, real_T ENV_Vsound, real_T ENV_FPthem, real_T ENV_mach, real_T
  ENV_alpha, real_T ENV_Vairspeed, real_T ENV_Q, real_T ENV_atmbeta, const
  T_Weapon_MCP_DATA_Guid *fromDATA, real_T TTKFin_tgtrge, real_T b_cnsts_r2d);
static void GetDoubleArcVectors(real_T ftime, uint8_T opportunity_mode, int8_T
  *solution, real_T *FPazi, real_T tgt0_x, real_T tgt0_y, real_T tgt0_z, real_T
  Ut0[3], const real_T Uv0[3], real_T Kglide, real_T Mglide0, real_T Mach_diff,
  uint8_T latch[2], real_T Rlim_lo, real_T Rarc0, real_T psi_Engage, uint8_T
  Weapon_Type, uint8_T AFmode, uint8_T log_FP, sFMpPTQ95PcZEskZd3p4FcD
  *telstruct, uint16_T *iW, T_Cartesian *V_Rto, T_Cartesian *V_Rte, T_Cartesian *
  V_Rwo, T_Cartesian *V_Rwe, T_Cartesian *V_Rmwo, T_Cartesian *V_Rmto,
  T_Cartesian *V_Rm, real_T *V_RarcT, T_Cartesian *V_OarcT, real_T *V_RarcW,
  T_Cartesian *V_OarcW, real_T Uw[3], real_T *Lfpath2bunt, real_T *Lfpath2tgt,
  real_T Ut[3], real_T *ratio_gcapability, real_T *Mglide);
static void DefineFlightPath(sFYrLWvx1kVFonG2Nu0GNkH *TRACK, real_T OPT_FP_Mopt,
  real_T OPT_FP_Mglide_opt, uint8_T MP_in_WeaponType, const T_Cartesian TPosNED,
  const T_Cartesian MPosNED, const T_Cartesian MVelNED, const T_Cartesian
  MRefNED, const sPLFBkBfmjj013qOjLWm7GC *DCM, real_T ENV_mach, uint8_T
  opportunity_mode, uint8_T AFmode, real_T guid_settings_psiR_Thr, real_T
  guid_settings_psi_Engage, real_T Rarc_T, real_T ftime, sFMpPTQ95PcZEskZd3p4FcD
  *tel_struct, sqfeYN8Hrouv8AbOaDO2afG *telem);
static void GetCLOSParams(uint8_T guidmode, uint8_T guidcmds_gmode_sw, uint8_T
  guidcmds_PurePropNav, real_T guidcmds_GndRge, real_T ftime,
  s28FIl9ilY9eNQKo2dEzV1C *gains, ssOdl51LB8ZQlaB7SirUWkE *clos,
  sFYrLWvx1kVFonG2Nu0GNkH *TRACK, real_T ENV_FPthem, real_T ENV_FPpsim, real_T
  ENV_Vairspeed, real_T ENV_Vnav, real_T ENV_Q, real_T OPT_FP_Qopt, real_T
  OPT_FP_Mglide_opt, const sPLFBkBfmjj013qOjLWm7GC *DCM, const
  T_GUID_TTKF_Out_IF1002 *TTKF_in, const sb6O280JGWZ46sxhPtS13aC *NAVin, const
  T_Cartesian tgt0, uint8_T AFmode, real_T RrefL[3], real_T RrefC[3]);
static void ExeGuidanceCommands(uint8_T opportunity_mode, uint8_T guidmode,
  real_T delt, sETPRuSGTG6V05GSmeYzNjH *guidcmds, uint8_T AFmode, uint8_T reset,
  uint8_T bunt_arc, real_T gains_Kp, const real_T TRACK_Ut0[3], const real_T
  TRACK_Ut[3], const ssOdl51LB8ZQlaB7SirUWkE *clos, const
  sVUEBEgeCpQlxFVEgYjRFMH *ENV, const sSZzX5nI8YD60UjueDawhgE *OPT_FP, const
  sPLFBkBfmjj013qOjLWm7GC *DCM, const T_GUID_TTKF_Out_IF1002 *TTKF_in, const
  sb6O280JGWZ46sxhPtS13aC *NAVin, const shkoXMvGDT09CSMPYwHF4WB IMUin, const
  T_GUID_MissionPlanning *MPin, T_GUID_Guid_Out_IF1003 *guid2AP, const
  T_Weapon_MCP_DATA_Guid *fromDATA, real_T psiLOSb, const
  struct_Nfy90zz6zGRJDmnb4iAwuF *b_def, real_T *phic, real_T *gbyc, real_T *gbzc,
  real_T *temp_gbyc);
static void ExeP1TestCommands(uint8_T GC_Mode, uint8_T guidmode, real_T ftime,
  sETPRuSGTG6V05GSmeYzNjH *guidcmds, real_T b_def_AP_PROG1, real_T
  b_def_AP_PROG2, real_T ENV_Vnav, const real_T ENV_GRAVbody[3], real_T
  OPT_FP_GLim, const sPLFBkBfmjj013qOjLWm7GC *DCM, real_T TTKF_in_tgtrge, real_T
  TTKF_in_dthe_b, real_T TTKF_in_dpsi_b, T_GUID_Guid_Out_IF1003 *guid2AP, const
  sbS2o27mNjUInq39Qf98jAF *guid_settings, real_T *phic, real_T *gbyc, real_T
  *gbzc);
static void ExeDeflInspTest(real_T ftime, T_GUID_Guid_Out_IF1003 *guid2AP, const
  real_T guid_settings_matDEFL[266]);

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function dcm = LIB_e2d(eg)
 */
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9])
{
  real_T dcm_tmp;
  real_T dcm_tmp_0;
  real_T dcm_tmp_1;
  real_T dcm_tmp_2;
  real_T dcm_tmp_3;
  real_T dcm_tmp_4;
  real_T dcm_tmp_5;
  real_T dcm_tmp_6;

  /* 'LIB_e2d:26' dcm = zeros(3,3); */
  /* 'LIB_e2d:28' phi   = eg.phi; */
  /* 'LIB_e2d:29' theta = eg.the; */
  /* 'LIB_e2d:30' psi   = eg.psi; */
  /* 'LIB_e2d:32' dcm(1,1) = cos(theta)*cos(psi); */
  dcm_tmp = cos(eg_psi);
  dcm_tmp_4 = cos(eg_the);
  dcm[0] = dcm_tmp_4 * dcm_tmp;

  /* 'LIB_e2d:33' dcm(1,2) = (sin(phi)*sin(theta)*cos(psi)) - (cos(phi)*sin(psi)); */
  dcm_tmp_0 = cos(eg_phi);
  dcm_tmp_1 = sin(eg_the);
  dcm_tmp_2 = sin(eg_phi);
  dcm_tmp_3 = sin(eg_psi);
  dcm_tmp_5 = dcm_tmp_2 * dcm_tmp_1;
  dcm[3] = (dcm_tmp_5 * dcm_tmp) - (dcm_tmp_0 * dcm_tmp_3);

  /* 'LIB_e2d:34' dcm(1,3) = (cos(phi)*sin(theta)*cos(psi)) + (sin(phi)*sin(psi)); */
  dcm_tmp_6 = dcm_tmp_0 * dcm_tmp_1;
  dcm[6] = (dcm_tmp_6 * dcm_tmp) + (dcm_tmp_2 * dcm_tmp_3);

  /* 'LIB_e2d:35' dcm(2,1) = cos(theta)*sin(psi); */
  dcm[1] = dcm_tmp_4 * dcm_tmp_3;

  /* 'LIB_e2d:36' dcm(2,2) = (sin(phi)*sin(theta)*sin(psi)) + (cos(phi)*cos(psi)); */
  dcm[4] = (dcm_tmp_5 * dcm_tmp_3) + (dcm_tmp_0 * dcm_tmp);

  /* 'LIB_e2d:37' dcm(2,3) = (cos(phi)*sin(theta)*sin(psi)) - (sin(phi)*cos(psi)); */
  dcm[7] = (dcm_tmp_6 * dcm_tmp_3) - (dcm_tmp_2 * dcm_tmp);

  /* 'LIB_e2d:38' dcm(3,1) = -sin(theta); */
  dcm[2] = -dcm_tmp_1;

  /* 'LIB_e2d:39' dcm(3,2) =  sin(phi)*cos(theta); */
  dcm[5] = dcm_tmp_2 * dcm_tmp_4;

  /* 'LIB_e2d:40' dcm(3,3) =  cos(phi)*cos(theta); */
  dcm[8] = dcm_tmp_0 * dcm_tmp_4;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [TPos,TPos_uncond,MPos,MVel,MRef,DCM] = TransformationsNED(TTKFin,NAVin,REFin,RE0)
 */
static void TransformationsNED(real_T TTKFin_tgtrex, real_T TTKFin_tgtrey,
  real_T TTKFin_tgtrez, real_T TTKFin_tgtrex_uncond, real_T TTKFin_tgtrey_uncond,
  real_T TTKFin_tgtrez_uncond, real_T NAVin_pos_g_lat, real_T NAVin_pos_g_lon,
  real_T NAVin_pos_e_x, real_T NAVin_pos_e_y, real_T NAVin_pos_e_z, real_T
  NAVin_vel_e_x, real_T NAVin_vel_e_y, real_T NAVin_vel_e_z, const T_Euler
  NAVin_att_bn, const real_T REFin[3], real_T RE0_x, real_T RE0_y, real_T RE0_z,
  T_Cartesian *TPos, T_Cartesian *TPos_uncond, T_Cartesian *MPos, T_Cartesian
  *MVel, T_Cartesian *MRef, sPLFBkBfmjj013qOjLWm7GC *DCM)
{
  real_T TGTinrx;
  real_T TGTinry;
  real_T TGTinrz;
  real_T TGTinrx_uncond;
  real_T TGTinry_uncond;
  real_T TGTinrz_uncond;
  real_T MISinrx;
  real_T MISinry;
  real_T MISinrz;
  real_T REFinrx;
  real_T REFinry;
  real_T REFinrz;
  real_T tempdcm_tmp[9];
  real_T tmp[9];
  int32_T i;
  real_T tmp_0;
  real_T tmp_1;
  real_T tmp_2;
  real_T tempdcm_tmp_tmp;

  /* 'GUID_Embedded_Guid_Control_1D:1974' eg.phi = NAVin.att_bn.phi; */
  /* 'GUID_Embedded_Guid_Control_1D:1975' eg.the = NAVin.att_bn.the; */
  /* 'GUID_Embedded_Guid_Control_1D:1976' eg.psi = NAVin.att_bn.psi; */
  /* 'GUID_Embedded_Guid_Control_1D:1977' DCM.NED2BODY = (LIB_e2d(eg))'; */
  LIB_e2d(NAVin_att_bn.phi, NAVin_att_bn.the, NAVin_att_bn.psi, tmp);
  for (i = 0; i < 3; i++) {
    DCM->NED2BODY[3 * i] = tmp[i];
    DCM->NED2BODY[1 + (3 * i)] = tmp[i + 3];
    DCM->NED2BODY[2 + (3 * i)] = tmp[i + 6];
  }

  /* 'GUID_Embedded_Guid_Control_1D:1985' templla.lat = NAVin.pos_g.lat; */
  /* 'GUID_Embedded_Guid_Control_1D:1986' templla.lon = NAVin.pos_g.lon; */
  /* 'GUID_Embedded_Guid_Control_1D:1987' templla.alt = NAVin.pos_g.alt; */
  /* 'GUID_Embedded_Guid_Control_1D:1988' tempdcm = LIB_dcm_ecef2ned(templla); */
  /* 'LIB_dcm_ecef2ned:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
  /* 'LIB_dcm_ecef2ned:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
  /* 'LIB_dcm_ecef2ned:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
  tmp_2 = cos(NAVin_pos_g_lat);
  tmp_1 = cos(NAVin_pos_g_lon);
  tmp_0 = sin(NAVin_pos_g_lat);
  tempdcm_tmp[0] = (-tmp_0) * tmp_1;
  tempdcm_tmp_tmp = sin(NAVin_pos_g_lon);
  tempdcm_tmp[3] = (-sin(NAVin_pos_g_lat)) * tempdcm_tmp_tmp;
  tempdcm_tmp[6] = tmp_2;
  tempdcm_tmp[1] = -tempdcm_tmp_tmp;
  tempdcm_tmp[4] = tmp_1;
  tempdcm_tmp[7] = 0.0;
  tempdcm_tmp[2] = (-tmp_2) * tmp_1;
  tempdcm_tmp[5] = (-cos(NAVin_pos_g_lat)) * tempdcm_tmp_tmp;
  tempdcm_tmp[8] = -sin(NAVin_pos_g_lat);

  /* 'GUID_Embedded_Guid_Control_1D:1989' DCM.ECEF2NED = tempdcm; */
  memcpy(&DCM->ECEF2NED[0], &tempdcm_tmp[0], 9U * (sizeof(real_T)));

  /* 'GUID_Embedded_Guid_Control_1D:1996' TGTinrx = TTKFin.tgtrex-RE0.x; */
  TGTinrx = TTKFin_tgtrex - RE0_x;

  /* 'GUID_Embedded_Guid_Control_1D:1997' TGTinry = TTKFin.tgtrey-RE0.y; */
  TGTinry = TTKFin_tgtrey - RE0_y;

  /* 'GUID_Embedded_Guid_Control_1D:1998' TGTinrz = TTKFin.tgtrez-RE0.z; */
  TGTinrz = TTKFin_tgtrez - RE0_z;

  /* 'GUID_Embedded_Guid_Control_1D:1999' TGTinrx_uncond = TTKFin.tgtrex_uncond-RE0.x; */
  TGTinrx_uncond = TTKFin_tgtrex_uncond - RE0_x;

  /* 'GUID_Embedded_Guid_Control_1D:2000' TGTinry_uncond = TTKFin.tgtrey_uncond-RE0.y; */
  TGTinry_uncond = TTKFin_tgtrey_uncond - RE0_y;

  /* 'GUID_Embedded_Guid_Control_1D:2001' TGTinrz_uncond = TTKFin.tgtrez_uncond-RE0.z; */
  TGTinrz_uncond = TTKFin_tgtrez_uncond - RE0_z;

  /* 'GUID_Embedded_Guid_Control_1D:2002' MISinrx = NAVin.pos_e.x-RE0.x; */
  MISinrx = NAVin_pos_e_x - RE0_x;

  /* 'GUID_Embedded_Guid_Control_1D:2003' MISinry = NAVin.pos_e.y-RE0.y; */
  MISinry = NAVin_pos_e_y - RE0_y;

  /* 'GUID_Embedded_Guid_Control_1D:2004' MISinrz = NAVin.pos_e.z-RE0.z; */
  MISinrz = NAVin_pos_e_z - RE0_z;

  /* 'GUID_Embedded_Guid_Control_1D:2005' REFinrx = REFin(1)-RE0.x; */
  REFinrx = REFin[0] - RE0_x;

  /* 'GUID_Embedded_Guid_Control_1D:2006' REFinry = REFin(2)-RE0.y; */
  REFinry = REFin[1] - RE0_y;

  /* 'GUID_Embedded_Guid_Control_1D:2007' REFinrz = REFin(3)-RE0.z; */
  REFinrz = REFin[2] - RE0_z;

  /* 'GUID_Embedded_Guid_Control_1D:2008' MISinvx = NAVin.vel_e.x; */
  /* 'GUID_Embedded_Guid_Control_1D:2009' MISinvy = NAVin.vel_e.y; */
  /* 'GUID_Embedded_Guid_Control_1D:2010' MISinvz = NAVin.vel_e.z; */
  /* 'GUID_Embedded_Guid_Control_1D:2012' TPos.x = DCM.ECEF2NED(1,:)*[TGTinrx TGTinry TGTinrz]'; */
  TPos->x = ((tempdcm_tmp[0] * TGTinrx) + (tempdcm_tmp[3] * TGTinry)) + (tmp_2 *
    TGTinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2013' TPos.y = DCM.ECEF2NED(2,:)*[TGTinrx TGTinry TGTinrz]'; */
  TPos->y = (((-tempdcm_tmp_tmp) * TGTinrx) + (tmp_1 * TGTinry)) + (0.0 *
    TGTinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2014' TPos.z = DCM.ECEF2NED(3,:)*[TGTinrx TGTinry TGTinrz]'; */
  TPos->z = ((tempdcm_tmp[2] * TGTinrx) + (tempdcm_tmp[5] * TGTinry)) + ((-tmp_0)
    * TGTinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2015' TPos_uncond.x = DCM.ECEF2NED(1,:)*[TGTinrx_uncond TGTinry_uncond TGTinrz_uncond]'; */
  TPos_uncond->x = ((tempdcm_tmp[0] * TGTinrx_uncond) + (tempdcm_tmp[3] *
    TGTinry_uncond)) + (tmp_2 * TGTinrz_uncond);

  /* 'GUID_Embedded_Guid_Control_1D:2016' TPos_uncond.y = DCM.ECEF2NED(2,:)*[TGTinrx_uncond TGTinry_uncond TGTinrz_uncond]'; */
  TPos_uncond->y = (((-tempdcm_tmp_tmp) * TGTinrx_uncond) + (tmp_1 *
    TGTinry_uncond)) + (0.0 * TGTinrz_uncond);

  /* 'GUID_Embedded_Guid_Control_1D:2017' TPos_uncond.z = DCM.ECEF2NED(3,:)*[TGTinrx_uncond TGTinry_uncond TGTinrz_uncond]'; */
  TPos_uncond->z = ((tempdcm_tmp[2] * TGTinrx_uncond) + (tempdcm_tmp[5] *
    TGTinry_uncond)) + ((-tmp_0) * TGTinrz_uncond);

  /* 'GUID_Embedded_Guid_Control_1D:2018' MPos.x = DCM.ECEF2NED(1,:)*[MISinrx MISinry MISinrz]'; */
  MPos->x = ((tempdcm_tmp[0] * MISinrx) + (tempdcm_tmp[3] * MISinry)) + (tmp_2 *
    MISinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2019' MPos.y = DCM.ECEF2NED(2,:)*[MISinrx MISinry MISinrz]'; */
  MPos->y = (((-tempdcm_tmp_tmp) * MISinrx) + (tmp_1 * MISinry)) + (0.0 *
    MISinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2020' MPos.z = DCM.ECEF2NED(3,:)*[MISinrx MISinry MISinrz]'; */
  MPos->z = ((tempdcm_tmp[2] * MISinrx) + (tempdcm_tmp[5] * MISinry)) + ((-tmp_0)
    * MISinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2021' MRef.x = DCM.ECEF2NED(1,:)*[REFinrx REFinry REFinrz]'; */
  MRef->x = ((tempdcm_tmp[0] * REFinrx) + (tempdcm_tmp[3] * REFinry)) + (tmp_2 *
    REFinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2022' MRef.y = DCM.ECEF2NED(2,:)*[REFinrx REFinry REFinrz]'; */
  MRef->y = (((-tempdcm_tmp_tmp) * REFinrx) + (tmp_1 * REFinry)) + (0.0 *
    REFinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2023' MRef.z = DCM.ECEF2NED(3,:)*[REFinrx REFinry REFinrz]'; */
  MRef->z = ((tempdcm_tmp[2] * REFinrx) + (tempdcm_tmp[5] * REFinry)) + ((-tmp_0)
    * REFinrz);

  /* 'GUID_Embedded_Guid_Control_1D:2024' MVel.x = DCM.ECEF2NED(1,:)*[MISinvx MISinvy MISinvz]'; */
  MVel->x = ((tempdcm_tmp[0] * NAVin_vel_e_x) + (tempdcm_tmp[3] * NAVin_vel_e_y))
    + (tmp_2 * NAVin_vel_e_z);

  /* 'GUID_Embedded_Guid_Control_1D:2025' MVel.y = DCM.ECEF2NED(2,:)*[MISinvx MISinvy MISinvz]'; */
  MVel->y = (((-tempdcm_tmp_tmp) * NAVin_vel_e_x) + (tmp_1 * NAVin_vel_e_y)) +
    (0.0 * NAVin_vel_e_z);

  /* 'GUID_Embedded_Guid_Control_1D:2026' MVel.z = DCM.ECEF2NED(3,:)*[MISinvx MISinvy MISinvz]'; */
  MVel->z = ((tempdcm_tmp[2] * NAVin_vel_e_x) + (tempdcm_tmp[5] * NAVin_vel_e_y))
    + ((-tmp_0) * NAVin_vel_e_z);
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function ENV = GetAirdataParams(ENV,NAVin,Vned,DCM,FCin,ValidGNSSNav,IMUin,WVN,WVE,fromDATA,ftime,delt,cnsts)
 */
static void GetAirdataParams(sVUEBEgeCpQlxFVEgYjRFMH *ENV, real_T
  NAVin_pos_g_alt, const T_Cartesian Vned, const sPLFBkBfmjj013qOjLWm7GC *DCM,
  real_T FCin_Temp, real_T FCin_Psta, real_T FCin_Pdyn, real_T ValidGNSSNav,
  real_T IMUin_gbym, real_T IMUin_gbzm, real_T IMUin_wbym, real_T IMUin_wbzm,
  const T_Weapon_MCP_DATA_Guid *fromDATA, real_T ftime, real_T delt, const
  struct_6qvaB88DLLAFHx6ej5JcWG *b_cnsts)
{
  real_T alphavec[10];
  real_T Ta;
  real_T b_pow;
  real_T tmp;
  real_T Pa;
  real_T U_Vned[3];
  real_T Vnedn;
  real_T Vnede;
  real_T Vnedd;
  real_T CLtrim_M[10];
  real_T beta_G;
  real_T b;
  real_T den;
  real_T fromDATA_0[7];
  real_T fromDATA_1[70];
  real_T Vned_0[3];
  int32_T i;
  real_T Vnedd_tmp;

  /* 'GUID_Embedded_Guid_Control_1D:2034' mass     = double(fromDATA.AirframeProp.mass); */
  /* 'GUID_Embedded_Guid_Control_1D:2035' Sref     = double(fromDATA.AirframeProp.Sref); */
  /* 'GUID_Embedded_Guid_Control_1D:2036' machvec  = double(fromDATA.AirframeProp.machvec); */
  /* 'GUID_Embedded_Guid_Control_1D:2037' alphavec = double(fromDATA.AirframeProp.alphavec); */
  for (i = 0; i < 10; i++) {
    alphavec[i] = (real_T)fromDATA->AirframeProp.alphavec[i];
  }

  /* 'GUID_Embedded_Guid_Control_1D:2038' CLtrim   = 1.00*double(fromDATA.AirframeProp.CLtrim); */
  /* 'GUID_Embedded_Guid_Control_1D:2040' ENV.GRAVbody = DCM.NED2BODY*[0 0 9.81]'; */
  for (i = 0; i < 3; i++) {
    ENV->GRAVbody[i] = 0.0;
    ENV->GRAVbody[i] += DCM->NED2BODY[i] * 0.0;
    ENV->GRAVbody[i] += DCM->NED2BODY[i + 3] * 0.0;
    ENV->GRAVbody[i] += DCM->NED2BODY[i + 6] * 9.81;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2043' if (ftime < delt) */
  if (ftime < delt) {
    /* 'GUID_Embedded_Guid_Control_1D:2044' ENV.temp  = FCin.Temp - 40.0; */
    /* 'GUID_Embedded_Guid_Control_1D:2045' ENV.temp0 = LIB_limit(20.0,40.0,ENV.temp+cnsts.L*NAVin.pos_g.alt); */
    Pa = (FCin_Temp - 40.0) + (b_cnsts->L * NAVin_pos_g_alt);

    /* 'LIB_limit:27' if (in > max) */
    if (Pa > 40.0) {
      /* 'LIB_limit:28' out = max; */
      ENV->temp0 = 40.0;
    } else if (Pa < 20.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      ENV->temp0 = 20.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      ENV->temp0 = Pa;
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2047' ENV.temp  = ENV.temp0-cnsts.L*NAVin.pos_g.alt; */
  Pa = b_cnsts->L * NAVin_pos_g_alt;
  ENV->temp = ENV->temp0 - Pa;

  /* 'GUID_Embedded_Guid_Control_1D:2048' Ta = LIB_limit(150.0,330.0,ENV.temp + cnsts.ZK); */
  Ta = ENV->temp + b_cnsts->ZK;

  /* 'LIB_limit:27' if (in > max) */
  if (Ta > 330.0) {
    /* 'LIB_limit:28' out = max; */
    Ta = 330.0;
  } else if (Ta < 150.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Ta = 150.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2051' pow = cnsts.grav/(cnsts.R*cnsts.L); */
  b_pow = b_cnsts->grav / (b_cnsts->R * b_cnsts->L);

  /* 'GUID_Embedded_Guid_Control_1D:2052' if (NAVin.pos_g.alt > 1.0) */
  if (NAVin_pos_g_alt > 1.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2053' tmp = 1-(cnsts.L*NAVin.pos_g.alt)/(Ta+cnsts.L*NAVin.pos_g.alt); */
    tmp = 1.0 - (Pa / (Pa + Ta));
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2054' else */
    /* 'GUID_Embedded_Guid_Control_1D:2055' tmp = 1-(cnsts.L*1.0)/(Ta+cnsts.L*1.0); */
    tmp = 1.0 - (b_cnsts->L / (Ta + b_cnsts->L));
  }

  /* 'GUID_Embedded_Guid_Control_1D:2057' Pa = LIB_limit(10000.0,110000.0,cnsts.P0*(tmp^pow)); */
  Pa = b_cnsts->P0 * rt_powd_snf(tmp, b_pow);

  /* 'LIB_limit:27' if (in > max) */
  if (Pa > 110000.0) {
    /* 'LIB_limit:28' out = max; */
    Pa = 110000.0;
  } else if (Pa < 10000.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Pa = 10000.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2063' rho = LIB_limit(0.200,1.350,cnsts.rho*(tmp^(pow-1.0))); */
  b_pow = rt_powd_snf(tmp, b_pow - 1.0) * b_cnsts->rho;

  /* 'LIB_limit:27' if (in > max) */
  if (b_pow > 1.35) {
    /* 'LIB_limit:28' out = max; */
    b_pow = 1.35;
  } else if (b_pow < 0.2) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    b_pow = 0.2;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2065' Vsound = LIB_limit(290.0,360.0,sqrt(Ta*cnsts.gamma*cnsts.R)); */
  Ta = sqrt((Ta * b_cnsts->gamma) * b_cnsts->R);

  /* 'LIB_limit:27' if (in > max) */
  if (Ta > 360.0) {
    /* 'LIB_limit:28' out = max; */
    Ta = 360.0;
  } else if (Ta < 290.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Ta = 290.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2067' U_Vned = LIB_unitvector([Vned.x Vned.y Vned.z]); */
  U_Vned[0] = Vned.x;
  U_Vned[1] = Vned.y;
  U_Vned[2] = Vned.z;

  /* 'LIB_unitvector:4' den = norm(in); */
  den = norm_MfVYifVG(U_Vned);

  /* 'LIB_unitvector:5' if (den > 1.0e-12) */
  if (den > 1.0E-12) {
    /* 'LIB_unitvector:6' out = in/den; */
    U_Vned[0] = Vned.x / den;
    U_Vned[1] = Vned.y / den;
    U_Vned[2] = Vned.z / den;
  } else {
    /* 'LIB_unitvector:7' else */
    /* 'LIB_unitvector:8' out = zeros(size(in)); */
    U_Vned[1] = 0.0;
    U_Vned[2] = 0.0;

    /* 'LIB_unitvector:9' out(1) = 1.0; */
    U_Vned[0] = 1.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2068' VELned = LIB_limit(0.001,500.0,norm([Vned.x Vned.y Vned.z])); */
  Vned_0[0] = Vned.x;
  Vned_0[1] = Vned.y;
  Vned_0[2] = Vned.z;
  den = norm_MfVYifVG(Vned_0);

  /* 'LIB_limit:27' if (in > max) */
  if (den > 500.0) {
    /* 'LIB_limit:28' out = max; */
    den = 500.0;
  } else if (den < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    den = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2069' Vnedn = VELned*U_Vned(1); */
  Vnedn = den * U_Vned[0];

  /* 'GUID_Embedded_Guid_Control_1D:2070' Vnede = VELned*U_Vned(2); */
  Vnede = den * U_Vned[1];

  /* 'GUID_Embedded_Guid_Control_1D:2071' Vnedd = VELned*U_Vned(3); */
  Vnedd = den * U_Vned[2];

  /* 'GUID_Embedded_Guid_Control_1D:2073' DCMtemp = DCM.NED2BODY; */
  /* 'GUID_Embedded_Guid_Control_1D:2074' Vbody = DCMtemp*[Vnedn+WVN Vnede+WVE Vnedd]'; */
  for (i = 0; i < 3; i++) {
    U_Vned[i] = (DCM->NED2BODY[i + 6] * Vnedd) + ((DCM->NED2BODY[i + 3] * Vnede)
      + (DCM->NED2BODY[i] * Vnedn));
  }

  /* 'GUID_Embedded_Guid_Control_1D:2077' Vbody_mag = norm(Vbody); */
  tmp = norm_MfVYifVG(U_Vned);

  /* 'GUID_Embedded_Guid_Control_1D:2078' if (Vbody_mag > 0.0) */
  if (tmp > 0.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2079' ENV.FPthem = asin(-Vnedd/Vbody_mag); */
    ENV->FPthem = asin((-Vnedd) / tmp);

    /* 'GUID_Embedded_Guid_Control_1D:2080' ENV.FPpsim = atan2(Vnede,Vnedn); */
    ENV->FPpsim = rt_atan2d_snf(Vnede, Vnedn);
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2081' else */
    /* 'GUID_Embedded_Guid_Control_1D:2082' ENV.FPthem = -0.001; */
    ENV->FPthem = -0.001;

    /* 'GUID_Embedded_Guid_Control_1D:2083' ENV.FPpsim =  0.000; */
    ENV->FPpsim = 0.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2088' if (FCin.Pdyn == 0.0 || FCin.Psta == 0.0) */
  if ((FCin_Pdyn == 0.0) || (FCin_Psta == 0.0)) {
    /* 'GUID_Embedded_Guid_Control_1D:2089' mach = Vbody_mag/Vsound; */
    Vnedn = tmp / Ta;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2090' else */
    /* 'GUID_Embedded_Guid_Control_1D:2091' Pdyn = LIB_limit(0.001,1000000.0,(FCin.Pdyn+FCin.Psta)-Pa); */
    Vnedn = (FCin_Pdyn + FCin_Psta) - Pa;

    /* 'LIB_limit:27' if (in > max) */
    if (Vnedn > 1.0E+6) {
      /* 'LIB_limit:28' out = max; */
      Vnedn = 1.0E+6;
    } else if (Vnedn < 0.001) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Vnedn = 0.001;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:2092' if (Pa <= 0.0) */
    /* 'GUID_Embedded_Guid_Control_1D:2094' else */
    /* 'GUID_Embedded_Guid_Control_1D:2096' mach = sqrt(5*((Pdyn/Pa+1)^0.2857-1)); */
    Vnedn = sqrt((rt_powd_snf((Vnedn / Pa) + 1.0, 0.2857) - 1.0) * 5.0);
  }

  /* 'GUID_Embedded_Guid_Control_1D:2104' if (ValidGNSSNav) */
  if (ValidGNSSNav != 0.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2105' if (Vbody_mag > VELned) */
    if (tmp > den) {
      /* 'GUID_Embedded_Guid_Control_1D:2106' Vq = LIB_limit(0.95*VELned,Vbody_mag+0.05*VELned,mach*Vsound); */
      Vnede = 0.95 * den;
      Vnedd = (0.05 * den) + tmp;
      Vnedn *= Ta;

      /* 'LIB_limit:27' if (in > max) */
      if (Vnedn > Vnedd) {
        /* 'LIB_limit:28' out = max; */
        Vnedn = Vnedd;
      } else if (Vnedn < Vnede) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Vnedn = Vnede;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2107' else */
      /* 'GUID_Embedded_Guid_Control_1D:2108' Vq = LIB_limit(Vbody_mag-0.05*VELned,1.05*VELned,mach*Vsound); */
      Vnede = tmp - (0.05 * den);
      Vnedd = 1.05 * den;
      Vnedn *= Ta;

      /* 'LIB_limit:27' if (in > max) */
      if (Vnedn > Vnedd) {
        /* 'LIB_limit:28' out = max; */
        Vnedn = Vnedd;
      } else if (Vnedn < Vnede) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Vnedn = Vnede;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    }
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2110' else */
    /* 'GUID_Embedded_Guid_Control_1D:2111' Vq = mach*Vsound; */
    Vnedn *= Ta;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2118' Q = LIB_limit(0.001,300000.0,0.5*rho*Vq*Vq); */
  Vnede = ((0.5 * b_pow) * Vnedn) * Vnedn;

  /* 'LIB_limit:27' if (in > max) */
  if (Vnede > 300000.0) {
    /* 'LIB_limit:28' out = max; */
    Vnede = 300000.0;
  } else if (Vnede < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Vnede = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2120' Vq0 = (Vq/Vsound)*341.0; */
  Vnedd_tmp = Vnedn / Ta;
  Vnedd = Vnedd_tmp * 341.0;

  /* 'GUID_Embedded_Guid_Control_1D:2121' Q0  = LIB_limit(0.001,300000.0,0.5*cnsts.rho*Vq0*Vq0); */
  Vnedd *= (0.5 * b_cnsts->rho) * Vnedd;

  /* 'LIB_limit:27' if (in > max) */
  if (Vnedd > 300000.0) {
    /* 'LIB_limit:28' out = max; */
    Vnedd = 300000.0;
  } else if (Vnedd < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Vnedd = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2129' if (Vbody_mag > 10.0) */
  if (tmp > 10.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2130' ENV.beta  = LIB_limit(-0.2617,0.2617,atan2(-Vbody(2),Vbody(1))); */
    tmp = rt_atan2d_snf(-U_Vned[1], U_Vned[0]);

    /* 'LIB_limit:27' if (in > max) */
    if (tmp > 0.2617) {
      /* 'LIB_limit:28' out = max; */
      ENV->beta = 0.2617;
    } else if (tmp < -0.2617) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      ENV->beta = -0.2617;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      ENV->beta = tmp;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2131' ENV.alpha = LIB_limit(-0.2617,0.2617,atan(Vbody(3)/sqrt(Vbody(1)*Vbody(1)+Vbody(2)*Vbody(2)))); */
    tmp = atan(U_Vned[2] / sqrt((U_Vned[0] * U_Vned[0]) + (U_Vned[1] * U_Vned[1])));

    /* 'LIB_limit:27' if (in > max) */
    if (tmp > 0.2617) {
      /* 'LIB_limit:28' out = max; */
      ENV->alpha = 0.2617;
    } else if (tmp < -0.2617) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      ENV->alpha = -0.2617;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      ENV->alpha = tmp;
    }
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2132' else */
    /* 'GUID_Embedded_Guid_Control_1D:2133' ENV.beta  = 0.0; */
    ENV->beta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2134' ENV.alpha = 0.0; */
    ENV->alpha = 0.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2136' if (ftime < delt) */
  if (ftime < delt) {
    /* 'GUID_Embedded_Guid_Control_1D:2137' ENV.BIAS_beta  = 0.0; */
    ENV->BIAS_beta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2138' ENV.BIAS_alpha = 0.0; */
    ENV->BIAS_alpha = 0.0;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2139' else */
    /* 'GUID_Embedded_Guid_Control_1D:2140' CNYmeas = abs(IMUin.gbym*mass)/(Q*Sref); */
    /* 'GUID_Embedded_Guid_Control_1D:2141' CNZmeas = abs(IMUin.gbzm*mass)/(Q*Sref); */
    /* 'GUID_Embedded_Guid_Control_1D:2142' CLtrim_M = interp1(machvec,CLtrim,Vq/Vsound,'linear','extrap'); */
    for (i = 0; i < 7; i++) {
      fromDATA_0[i] = (real_T)fromDATA->AirframeProp.machvec[i];
    }

    for (i = 0; i < 70; i++) {
      fromDATA_1[i] = (real_T)fromDATA->AirframeProp.CLtrim[i];
    }

    interp1_EKJBPReX(fromDATA_0, fromDATA_1, Vnedd_tmp, CLtrim_M);

    /* 'GUID_Embedded_Guid_Control_1D:2143' beta_G  = sign(IMUin.gbym)*LIB_limit(0.0,alphavec(end),interp1(CLtrim_M,alphavec,CNYmeas,'linear','extrap'))*cnsts.d2r; */
    b = Vnede * ((real_T)fromDATA->AirframeProp.Sref);
    tmp = interp1_UImAjR6u(CLtrim_M, alphavec, fabs(IMUin_gbym * ((real_T)
      fromDATA->AirframeProp.mass)) / b);

    /* 'LIB_limit:27' if (in > max) */
    if (tmp > alphavec[9]) {
      /* 'LIB_limit:28' out = max; */
      tmp = alphavec[9];
    } else if (tmp < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      tmp = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    if (IMUin_gbym < 0.0) {
      beta_G = -1.0;
    } else if (IMUin_gbym > 0.0) {
      beta_G = 1.0;
    } else if (IMUin_gbym == 0.0) {
      beta_G = 0.0;
    } else {
      beta_G = (rtNaN);
    }

    beta_G = (beta_G * tmp) * b_cnsts->d2r;

    /* 'GUID_Embedded_Guid_Control_1D:2144' alpha_G =-sign(IMUin.gbzm)*LIB_limit(0.0,alphavec(end),interp1(CLtrim_M,alphavec,CNZmeas,'linear','extrap'))*cnsts.d2r; */
    tmp = interp1_UImAjR6u(CLtrim_M, alphavec, fabs(IMUin_gbzm * ((real_T)
      fromDATA->AirframeProp.mass)) / b);

    /* 'LIB_limit:27' if (in > max) */
    if (tmp > alphavec[9]) {
      /* 'LIB_limit:28' out = max; */
      tmp = alphavec[9];
    } else if (tmp < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      tmp = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    if (IMUin_gbzm < 0.0) {
      b = -1.0;
    } else if (IMUin_gbzm > 0.0) {
      b = 1.0;
    } else if (IMUin_gbzm == 0.0) {
      b = 0.0;
    } else {
      b = (rtNaN);
    }

    tmp = ((-b) * tmp) * b_cnsts->d2r;

    /* 'GUID_Embedded_Guid_Control_1D:2148' a = 0.0309; */
    /* 'GUID_Embedded_Guid_Control_1D:2149' b = LIB_limit(0.0,1.0,0.2*ftime); */
    b = 0.2 * ftime;

    /* 'LIB_limit:27' if (in > max) */
    if (b > 1.0) {
      /* 'LIB_limit:28' out = max; */
      b = 1.0;
    } else if (b < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      b = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:2150' if (abs(IMUin.wbym) < 0.0873) */
    if (fabs(IMUin_wbym) < 0.0873) {
      /* 'GUID_Embedded_Guid_Control_1D:2151' tmp = b*(a*(LIB_limit(-0.2617,0.2617,alpha_G)-ENV.alpha) + (1.0-a)*ENV.BIAS_alpha); */
      /* 'LIB_limit:27' if (in > max) */
      if (tmp > 0.2617) {
        /* 'LIB_limit:28' out = max; */
        tmp = 0.2617;
      } else if (tmp < -0.2617) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        tmp = -0.2617;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      tmp = (((tmp - ENV->alpha) * 0.0309) + (0.9691 * ENV->BIAS_alpha)) * b;

      /* 'GUID_Embedded_Guid_Control_1D:2152' if (~isnan(tmp)) */
      if (!rtIsNaN(tmp)) {
        /* 'GUID_Embedded_Guid_Control_1D:2153' ENV.BIAS_alpha = LIB_limit(-0.0873,0.0873,tmp); */
        /* 'LIB_limit:27' if (in > max) */
        if (tmp > 0.0873) {
          /* 'LIB_limit:28' out = max; */
          ENV->BIAS_alpha = 0.0873;
        } else if (tmp < -0.0873) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          ENV->BIAS_alpha = -0.0873;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          ENV->BIAS_alpha = tmp;
        }
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:2156' if (abs(IMUin.wbzm) < 0.0873) */
    if (fabs(IMUin_wbzm) < 0.0873) {
      /* 'GUID_Embedded_Guid_Control_1D:2157' tmp = b*(a*(LIB_limit(-0.2617,0.2617,beta_G)-ENV.beta) + (1.0-a)*ENV.BIAS_beta); */
      /* 'LIB_limit:27' if (in > max) */
      if (beta_G > 0.2617) {
        /* 'LIB_limit:28' out = max; */
        beta_G = 0.2617;
      } else if (beta_G < -0.2617) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        beta_G = -0.2617;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      tmp = (((beta_G - ENV->beta) * 0.0309) + (0.9691 * ENV->BIAS_beta)) * b;

      /* 'GUID_Embedded_Guid_Control_1D:2158' if (~isnan(tmp)) */
      if (!rtIsNaN(tmp)) {
        /* 'GUID_Embedded_Guid_Control_1D:2159' ENV.BIAS_beta = LIB_limit(-0.0873,0.0873,tmp); */
        /* 'LIB_limit:27' if (in > max) */
        if (tmp > 0.0873) {
          /* 'LIB_limit:28' out = max; */
          ENV->BIAS_beta = 0.0873;
        } else if (tmp < -0.0873) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          ENV->BIAS_beta = -0.0873;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          ENV->BIAS_beta = tmp;
        }
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:2162' ENV.beta  = ENV.beta + ENV.BIAS_beta; */
    ENV->beta += ENV->BIAS_beta;

    /* 'GUID_Embedded_Guid_Control_1D:2163' ENV.alpha = ENV.alpha + ENV.BIAS_alpha; */
    ENV->alpha += ENV->BIAS_alpha;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2173' if (NAVin.pos_g.alt < 6000.0) */
  if (NAVin_pos_g_alt < 6000.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2174' ENV.atmbeta = -0.1024e-03; */
    ENV->atmbeta = -0.0001024;
  } else if (NAVin_pos_g_alt < 11000.0) {
    /* 'GUID_Embedded_Guid_Control_1D:2176' elseif (NAVin.pos_g.alt < 11000.0) */
    /* 'GUID_Embedded_Guid_Control_1D:2177' ENV.atmbeta = -0.1207e-03; */
    ENV->atmbeta = -0.0001207;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2179' else */
    /* 'GUID_Embedded_Guid_Control_1D:2180' ENV.atmbeta = -0.1572e-03; */
    ENV->atmbeta = -0.0001572;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2184' ENV.rho = rho; */
  ENV->rho = b_pow;

  /* 'GUID_Embedded_Guid_Control_1D:2185' ENV.pressure = Pa; */
  ENV->pressure = Pa;

  /* 'GUID_Embedded_Guid_Control_1D:2186' ENV.Vairspeed = Vq; */
  ENV->Vairspeed = Vnedn;

  /* 'GUID_Embedded_Guid_Control_1D:2187' ENV.Q = Q; */
  ENV->Q = Vnede;

  /* 'GUID_Embedded_Guid_Control_1D:2189' ENV.Qratio = LIB_limit(1.0,3.0,Q0/Q); */
  Pa = Vnedd / Vnede;

  /* 'LIB_limit:27' if (in > max) */
  if (Pa > 3.0) {
    /* 'LIB_limit:28' out = max; */
    ENV->Qratio = 3.0;
  } else if (Pa < 1.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    ENV->Qratio = 1.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    ENV->Qratio = Pa;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2190' ENV.Vsound = Vsound; */
  ENV->Vsound = Ta;

  /* 'GUID_Embedded_Guid_Control_1D:2191' ENV.mach = Vq/Vsound; */
  ENV->mach = Vnedd_tmp;

  /* 'GUID_Embedded_Guid_Control_1D:2192' ENV.Vnav = VELned; */
  ENV->Vnav = den;

  /* 'GUID_Embedded_Guid_Control_1D:2193' ENV.Vbody = Vbody; */
  ENV->Vbody[0] = U_Vned[0];
  ENV->Vbody[1] = U_Vned[1];
  ENV->Vbody[2] = U_Vned[2];
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [GUIDMODE,guidcmds] = SetGuidanceMode(GUIDMODE,TRACK,guidcmds,AFmode,opportunity_mode,MP_in,TTKF_in,NAVin,IMUin,WeaCfg_in,Rarc,psiLOSb,mach,ftime,newmeas_cnt,guid_settings)
 */
static void SetGuidanceMode(uint8_T *GUIDMODE, real_T TRACK_reset3cnt, real_T
  TRACK_mpos0_x, real_T TRACK_mpos0_y, real_T TRACK_mpos0_z, real_T TRACK_Rwe_x,
  real_T TRACK_Rwe_y, real_T TRACK_Rwe_z, sETPRuSGTG6V05GSmeYzNjH *guidcmds,
  uint8_T AFmode, uint8_T opportunity_mode, real32_T MP_in_DiveAngle, uint16_T
  TTKF_in_confidence, real_T TTKF_in_tgtrge, const T_Cartesian NAVin_pos_e,
  const T_Euler NAVin_att_bn, real_T IMUin_wbxm, uint8_T WeaCfg_in_WingsPresent,
  real_T mach, real_T ftime)
{
  real_T RGEswitch;
  real_T NAVin_pos_e_0[3];
  real_T TRACK_Rwe_x_0[3];

  /* 'GUID_Embedded_Guid_Control_1D:2720' RGEwea   = norm([NAVin.pos_e.x NAVin.pos_e.y NAVin.pos_e.z] - [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]); */
  /* 'GUID_Embedded_Guid_Control_1D:2721' RGEbuntE = norm([TRACK.Rwe.x TRACK.Rwe.y TRACK.Rwe.z] - [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]); */
  /* 'GUID_Embedded_Guid_Control_1D:2724' guidcmds.gmode_sw = uint8(0); */
  guidcmds->gmode_sw = 0U;

  /* 'GUID_Embedded_Guid_Control_1D:2725' if (GUIDMODE == 0 && ftime > 0.000001) */
  if ((((int32_T)(*GUIDMODE)) == 0) && (ftime > 1.0E-6)) {
    /* 'GUID_Embedded_Guid_Control_1D:2729' GUIDMODE = uint8(1); */
    *GUIDMODE = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:2730' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;
  } else if ((((int32_T)(*GUIDMODE)) == 1) && (ftime > 1.5)) {
    /* 'GUID_Embedded_Guid_Control_1D:2731' elseif (GUIDMODE == 1 && ftime > 1.5) */
    /* 'GUID_Embedded_Guid_Control_1D:2736' GUIDMODE = uint8(2); */
    *GUIDMODE = 2U;

    /* 'GUID_Embedded_Guid_Control_1D:2737' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;
  } else if ((((int32_T)(*GUIDMODE)) == 2) && (ftime > 1.5)) {
    /* 'GUID_Embedded_Guid_Control_1D:2738' elseif (GUIDMODE == 2 && ftime > 1.5) */
    /* 'GUID_Embedded_Guid_Control_1D:2744' GUIDMODE = uint8(3); */
    *GUIDMODE = 3U;

    /* 'GUID_Embedded_Guid_Control_1D:2745' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;
  } else if ((((((((int32_T)WeaCfg_in_WingsPresent) != 0) && (((int32_T)
      (*GUIDMODE)) == 3)) && (ftime > 2.8)) && (fabs(NAVin_att_bn.phi) < 0.1745))
              && (fabs(IMUin_wbxm) < 0.8727)) || (((((int32_T)
      WeaCfg_in_WingsPresent) == 0) && (((int32_T)(*GUIDMODE)) == 3)) && (ftime >
    2.5))) {
    /* 'GUID_Embedded_Guid_Control_1D:2746' elseif ((WeaCfg_in.WingsPresent && GUIDMODE == 3 && ftime > 2.8 && abs(NAVin.att_bn.phi) < 0.1745 && abs(IMUin.wbxm) < 0.8727) || ... */
    /* 'GUID_Embedded_Guid_Control_1D:2747'           (~WeaCfg_in.WingsPresent && GUIDMODE == 3 && ftime > 2.5)) */
    /* 'GUID_Embedded_Guid_Control_1D:2753' GUIDMODE = uint8(4); */
    *GUIDMODE = 4U;

    /* 'GUID_Embedded_Guid_Control_1D:2754' guidcmds.AZy = 1.0; */
    guidcmds->AZy = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2754' guidcmds.AZz = 1.0; */
    guidcmds->AZz = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2755' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;
  } else if ((ftime > 2.5) && (((((((int32_T)WeaCfg_in_WingsPresent) != 0) &&
      (((int32_T)(*GUIDMODE)) == 4)) && (mach < 0.7)) || (((int32_T)
      guidcmds->forcegmode5) != 0)) || (((((int32_T)WeaCfg_in_WingsPresent) == 0)
    && (((int32_T)(*GUIDMODE)) == 4)) && (((int32_T)opportunity_mode) == 0)))) {
    /* 'GUID_Embedded_Guid_Control_1D:2761' elseif (ftime > 2.5 && ... */
    /* 'GUID_Embedded_Guid_Control_1D:2762'          ((WeaCfg_in.WingsPresent && (GUIDMODE == 4 && mach < 0.7) || guidcmds.forcegmode5) || ... % change AirframeType == 1 to 2 after LR check for extra range */
    /* 'GUID_Embedded_Guid_Control_1D:2763'          ((~WeaCfg_in.WingsPresent && GUIDMODE == 4) && ~opportunity_mode))) */
    /* 'GUID_Embedded_Guid_Control_1D:2767' guidcmds.forcegmode5 = uint8(0); */
    guidcmds->forcegmode5 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:2768' GUIDMODE = uint8(5); */
    *GUIDMODE = 5U;

    /* 'GUID_Embedded_Guid_Control_1D:2769' guidcmds.AZy = 1.0; */
    guidcmds->AZy = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2769' guidcmds.AZz = 1.0; */
    guidcmds->AZz = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2770' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;
  } else if (((((int32_T)(*GUIDMODE)) == 5) && (((int32_T)AFmode) != 1)) ||
             (((((int32_T)(*GUIDMODE)) == 4) && (((int32_T)TTKF_in_confidence) >
    10)) && (((int32_T)opportunity_mode) != 0))) {
    /* 'GUID_Embedded_Guid_Control_1D:2772' elseif ( GUIDMODE == 5 && AFmode ~= 1 || ... */
    /* 'GUID_Embedded_Guid_Control_1D:2773' 		   (GUIDMODE == 4 && TTKF_in.confidence > 10.0 && opportunity_mode)) */
    /* 'GUID_Embedded_Guid_Control_1D:2777' GUIDMODE = uint8(6); */
    *GUIDMODE = 6U;

    /* 'GUID_Embedded_Guid_Control_1D:2778' guidcmds.AZy = 1.0; */
    guidcmds->AZy = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2778' guidcmds.AZz = 1.0; */
    guidcmds->AZz = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2779' guidcmds.gmode_sw = uint8(1); */
    guidcmds->gmode_sw = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:2780' guidcmds.PurePropNav = uint8(0); */
    guidcmds->PurePropNav = 0U;
  } else {
    if ((((int32_T)(*GUIDMODE)) == 6) || (((int32_T)(*GUIDMODE)) == 7)) {
      NAVin_pos_e_0[0] = NAVin_pos_e.x - TRACK_mpos0_x;
      NAVin_pos_e_0[1] = NAVin_pos_e.y - TRACK_mpos0_y;
      NAVin_pos_e_0[2] = NAVin_pos_e.z - TRACK_mpos0_z;
      TRACK_Rwe_x_0[0] = TRACK_Rwe_x - TRACK_mpos0_x;
      TRACK_Rwe_x_0[1] = TRACK_Rwe_y - TRACK_mpos0_y;
      TRACK_Rwe_x_0[2] = TRACK_Rwe_z - TRACK_mpos0_z;
      if (norm_MfVYifVG(NAVin_pos_e_0) > norm_MfVYifVG(TRACK_Rwe_x_0)) {
        /* 'GUID_Embedded_Guid_Control_1D:2787' elseif ((GUIDMODE == 6 || GUIDMODE == 7) && RGEwea > RGEbuntE) */
        /* 'GUID_Embedded_Guid_Control_1D:2790' GUIDMODE = uint8(8); */
        *GUIDMODE = 8U;

        /* 'GUID_Embedded_Guid_Control_1D:2791' guidcmds.AZy = 1.0; */
        guidcmds->AZy = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:2791' guidcmds.AZz = 1.0; */
        guidcmds->AZz = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:2792' guidcmds.gmode_sw = uint8(1); */
        guidcmds->gmode_sw = 1U;

        /* 'GUID_Embedded_Guid_Control_1D:2793' guidcmds.PurePropNav = uint8(0); */
        guidcmds->PurePropNav = 0U;
      }
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2796' if (GUIDMODE == 8 && guidcmds.gmode_sw == 0 && double(MP_in.DiveAngle < 1.396)) */
  if (((((int32_T)(*GUIDMODE)) == 8) && (((int32_T)guidcmds->gmode_sw) == 0)) &&
      (((real_T)MP_in_DiveAngle) < 1.396)) {
    /* 'GUID_Embedded_Guid_Control_1D:2798' RGEswitch = 2000.0 + TRACK.reset3cnt*500.0; */
    RGEswitch = (TRACK_reset3cnt * 500.0) + 2000.0;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2799' else */
    /* 'GUID_Embedded_Guid_Control_1D:2800' RGEswitch = 2000.0; */
    RGEswitch = 2000.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2804' if (GUIDMODE ~= 9) */
  if (((int32_T)(*GUIDMODE)) != 9) {
    /* 'GUID_Embedded_Guid_Control_1D:2808' if (GUIDMODE >= 4 && TTKF_in.tgtrge < RGEswitch) */
    if ((((int32_T)(*GUIDMODE)) >= 4) && (TTKF_in_tgtrge < RGEswitch)) {
      /* 'GUID_Embedded_Guid_Control_1D:2809' GUIDMODE = uint8(9); */
      *GUIDMODE = 9U;

      /* 'GUID_Embedded_Guid_Control_1D:2810' guidcmds.AZy = 1.0; */
      guidcmds->AZy = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:2810' guidcmds.AZz = 1.0; */
      guidcmds->AZz = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:2811' guidcmds.gmode_sw = uint8(1); */
      guidcmds->gmode_sw = 1U;
    } else {
      if ((((((int32_T)(*GUIDMODE)) >= 3) && (((int32_T)TTKF_in_confidence) >= 1))
           && (((int32_T)TTKF_in_confidence) <= 10)) && (((int32_T)
            opportunity_mode) != 0)) {
        /* 'GUID_Embedded_Guid_Control_1D:2812' elseif (GUIDMODE >= 3 && TTKF_in.confidence >= 1.0 && TTKF_in.confidence <= 10.0 && opportunity_mode) */
        /* 'GUID_Embedded_Guid_Control_1D:2813' GUIDMODE = uint8(9); */
        *GUIDMODE = 9U;

        /* 'GUID_Embedded_Guid_Control_1D:2814' guidcmds.AZy = 1.0; */
        guidcmds->AZy = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:2814' guidcmds.AZz = 1.0; */
        guidcmds->AZz = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:2815' guidcmds.gmode_sw = uint8(1); */
        guidcmds->gmode_sw = 1U;

        /* 'GUID_Embedded_Guid_Control_1D:2816' guidcmds.PurePropNav = uint8(1); */
        guidcmds->PurePropNav = 1U;
      }
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2820' if (0) */
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [OPT_FP,t_debug] = GetOptimalFPath(OPT_FP,GUIDMODE,ENV,gbyc_unl,gbzc_unl,gbym,gbzm,fromDATA,TTKFin,ftime,debug,t_debug,cnsts)
 */
static void GetOptimalFPath(sSZzX5nI8YD60UjueDawhgE *OPT_FP, uint8_T GUIDMODE,
  real_T ENV_rho, real_T ENV_Vsound, real_T ENV_FPthem, real_T ENV_mach, real_T
  ENV_alpha, real_T ENV_Vairspeed, real_T ENV_Q, real_T ENV_atmbeta, const
  T_Weapon_MCP_DATA_Guid *fromDATA, real_T TTKFin_tgtrge, real_T b_cnsts_r2d)
{
  real_T Sref;
  real_T machvec[7];
  real_T alphavec[10];
  real_T CLCDtrim[70];
  real_T CLtrim[70];
  real_T CDtrim[70];
  real_T Fgrav;
  real_T Mbeg;
  real_T Mend;
  real_T machana[20];
  real_T machinc;
  real_T ClCd_MA[20];
  real_T CLtrimA[10];
  real_T CLCDtrimA[10];
  real_T M0;
  real_T ClCd_M0;
  real_T den;
  int32_T j;
  int32_T i;
  real_T tmp[10];
  real_T tmp_0[10];
  boolean_T exitg1;
  boolean_T guard1 = false;

  /* 'GUID_Embedded_Guid_Control_1D:2202' mass     = double(fromDATA.AirframeProp.mass); */
  /* 'GUID_Embedded_Guid_Control_1D:2203' Sref     = double(fromDATA.AirframeProp.Sref); */
  Sref = (real_T)fromDATA->AirframeProp.Sref;

  /* 'GUID_Embedded_Guid_Control_1D:2204' machvec  = double(fromDATA.AirframeProp.machvec); */
  for (i = 0; i < 7; i++) {
    machvec[i] = (real_T)fromDATA->AirframeProp.machvec[i];
  }

  /* 'GUID_Embedded_Guid_Control_1D:2205' alphavec = double(fromDATA.AirframeProp.alphavec); */
  for (i = 0; i < 10; i++) {
    alphavec[i] = (real_T)fromDATA->AirframeProp.alphavec[i];
  }

  /* 'GUID_Embedded_Guid_Control_1D:2206' CLCDtrim = double(fromDATA.AirframeProp.CLCDtrim); */
  /* 'GUID_Embedded_Guid_Control_1D:2207' CLtrim   = double(fromDATA.AirframeProp.CLtrim); */
  /* 'GUID_Embedded_Guid_Control_1D:2208' CDtrim   = double(fromDATA.AirframeProp.CDtrim); */
  for (i = 0; i < 70; i++) {
    CLCDtrim[i] = (real_T)fromDATA->AirframeProp.CLCDtrim[i];
    CLtrim[i] = (real_T)fromDATA->AirframeProp.CLtrim[i];
    CDtrim[i] = (real_T)fromDATA->AirframeProp.CDtrim[i];
  }

  /* 'GUID_Embedded_Guid_Control_1D:2211' Fgrav = 9.81*mass; */
  Fgrav = 9.81 * ((real_T)fromDATA->AirframeProp.mass);

  /* 'GUID_Embedded_Guid_Control_1D:2230' if (ftime > 10.0) */
  /* 'GUID_Embedded_Guid_Control_1D:2233' if (debug) */
  /* 'GUID_Embedded_Guid_Control_1D:2242' Mbeg = 0.5*machvec(2); */
  Mbeg = 0.5 * machvec[1];

  /* 'GUID_Embedded_Guid_Control_1D:2243' Mend = machvec(end-1); */
  Mend = machvec[5];

  /* 'GUID_Embedded_Guid_Control_1D:2245' CNgrav = zeros(length(machvec),1); */
  /* 'GUID_Embedded_Guid_Control_1D:2246' alpha_M = zeros(length(machvec),1); */
  /* 'GUID_Embedded_Guid_Control_1D:2247' ClCd_M = zeros(length(machvec),1); */
  /* 'GUID_Embedded_Guid_Control_1D:2248' ClCd_MA_min = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:2252' for i=1:length(machvec) */
  /* 'GUID_Embedded_Guid_Control_1D:2278' N = 20; */
  /* 'GUID_Embedded_Guid_Control_1D:2279' machana = ones(N,1)*Mbeg; */
  for (i = 0; i < 20; i++) {
    machana[i] = Mbeg;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2280' ind_min = 1; */
  /* 'GUID_Embedded_Guid_Control_1D:2282' ClCd_M0A = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:2284' machinc = (Mend-Mbeg)/(N-1); */
  machinc = (machvec[5] - Mbeg) / 19.0;

  /* 'GUID_Embedded_Guid_Control_1D:2285' machana(1,1) = Mbeg; */
  machana[0] = Mbeg;

  /* 'GUID_Embedded_Guid_Control_1D:2286' for i = 2:N */
  for (i = 0; i < 19; i++) {
    /* 'GUID_Embedded_Guid_Control_1D:2287' machana(i,1) = machana(i-1,1) + machinc; */
    machana[i + 1] = machana[i] + machinc;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2289' CLgravA = zeros(N,1); */
  /* 'GUID_Embedded_Guid_Control_1D:2290' alpha_MA = zeros(N,1); */
  /* 'GUID_Embedded_Guid_Control_1D:2291' ClCd_MA = zeros(N,1); */
  /* 'GUID_Embedded_Guid_Control_1D:2292' CLtrimA = zeros(10,1); */
  /* 'GUID_Embedded_Guid_Control_1D:2293' CLCDtrimA = zeros(10,1); */
  /* 'GUID_Embedded_Guid_Control_1D:2294' for i=1:N */
  for (i = 0; i < 20; i++) {
    /* 'GUID_Embedded_Guid_Control_1D:2295' CLgravA(i,1) = Fgrav/(Sref*0.5*ENV.rho*(machana(i,1)*ENV.Vsound)^2); */
    M0 = machana[i] * ENV_Vsound;

    /* 'GUID_Embedded_Guid_Control_1D:2296' for j=1:length(alphavec) */
    for (j = 0; j < 10; j++) {
      /* 'GUID_Embedded_Guid_Control_1D:2297' CLtrimA(j)   = interp1(machvec,CLtrim(:,j),machana(i,1),'linear','extrap'); */
      CLtrimA[j] = interp1_Gy1LATh1(machvec, &CLtrim[7 * j], machana[i]);

      /* 'GUID_Embedded_Guid_Control_1D:2298' CLCDtrimA(j) = interp1(machvec,CLCDtrim(:,j),machana(i,1),'linear','extrap'); */
      CLCDtrimA[j] = interp1_Gy1LATh1(machvec, &CLCDtrim[7 * j], machana[i]);
    }

    /* 'GUID_Embedded_Guid_Control_1D:2300' if (debug) */
    /* 'GUID_Embedded_Guid_Control_1D:2305' alpha_MA(i,1) = interp1(CLtrimA,alphavec,CLgravA(i,1),'linear','extrap'); */
    /* 'GUID_Embedded_Guid_Control_1D:2306' ClCd_MA(i,1) = interp1(alphavec,CLCDtrimA,alpha_MA(i,1),'linear','extrap'); */
    ClCd_MA[i] = interp1_UImAjR6u(alphavec, CLCDtrimA, interp1_UImAjR6u(CLtrimA,
      alphavec, Fgrav / (((Sref * 0.5) * ENV_rho) * (M0 * M0))));

    /* 'GUID_Embedded_Guid_Control_1D:2307' if (ClCd_MA(i) < ClCd_M0A) */
  }

  /* 'GUID_Embedded_Guid_Control_1D:2311' [ClCd_MA_min,ind_min] = min(ClCd_MA); */
  if (!rtIsNaN(ClCd_MA[0])) {
    i = 1;
  } else {
    i = 0;
    j = 2;
    exitg1 = false;
    while ((!exitg1) && (j < 21)) {
      if (!rtIsNaN(ClCd_MA[j - 1])) {
        i = j;
        exitg1 = true;
      } else {
        j++;
      }
    }
  }

  if (i == 0) {
    i = 1;
  } else {
    M0 = ClCd_MA[i - 1];
    for (j = i; (j + 1) < 21; j++) {
      if (M0 > ClCd_MA[j]) {
        M0 = ClCd_MA[j];
        i = j + 1;
      }
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2312' if (ind_min > 1) */
  if (i > 1) {
    /* 'GUID_Embedded_Guid_Control_1D:2313' M0 = machana(ind_min-1); */
    M0 = machana[i - 2];

    /* 'GUID_Embedded_Guid_Control_1D:2314' ClCd_M0 = ClCd_MA(ind_min-1); */
    ClCd_M0 = ClCd_MA[i - 2];

    /* 'GUID_Embedded_Guid_Control_1D:2315' if (ind_min == N) */
    if (i == 20) {
      /* 'GUID_Embedded_Guid_Control_1D:2316' OPT_FP.M = M0 - 0.5*machinc; */
      OPT_FP->M = machana[18] - (0.5 * machinc);

      /* 'GUID_Embedded_Guid_Control_1D:2317' pol = -1; */
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2318' else */
      /* 'GUID_Embedded_Guid_Control_1D:2319' OPT_FP.M = M0 + 0.5*machinc; */
      OPT_FP->M = M0 + (0.5 * machinc);
    }
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2321' else */
    /* 'GUID_Embedded_Guid_Control_1D:2322' M0 = Mbeg; */
    M0 = Mbeg;

    /* 'GUID_Embedded_Guid_Control_1D:2323' ClCd_M0 = 0.0; */
    ClCd_M0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2324' OPT_FP.M = M0; */
    OPT_FP->M = Mbeg;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2328' if (debug && ftime > t_debug) */
  /* 'GUID_Embedded_Guid_Control_1D:2357' FP0 = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:2358' FPopt = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:2360' pol = 1; */
  machinc = 1.0;

  /* 'GUID_Embedded_Guid_Control_1D:2361' for i=1:15 */
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 15)) {
    /* 'GUID_Embedded_Guid_Control_1D:2363' den = Sref*0.5*ENV.rho*(OPT_FP.M*ENV.Vsound)^2; */
    den = OPT_FP->M * ENV_Vsound;
    den = ((Sref * 0.5) * ENV_rho) * (den * den);

    /* 'GUID_Embedded_Guid_Control_1D:2364' if (den < 0.000001) */
    if (den < 1.0E-6) {
      /* 'GUID_Embedded_Guid_Control_1D:2365' OPT_FP.CNgrav   = 0.0; */
      OPT_FP->CNgrav = 0.0;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2366' else */
      /* 'GUID_Embedded_Guid_Control_1D:2367' OPT_FP.CNgrav   = Fgrav/den; */
      OPT_FP->CNgrav = Fgrav / den;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2372' CLtrim_M = interp1(machvec,CLtrim,OPT_FP.M,'linear','extrap'); */
    /* 'GUID_Embedded_Guid_Control_1D:2373' OPT_FP.alpha_M  = LIB_limit(0.0,alphavec(end),interp1(CLtrim_M,alphavec,OPT_FP.CNgrav,'linear','extrap')); */
    interp1_EKJBPReX(machvec, CLtrim, OPT_FP->M, tmp);
    den = interp1_UImAjR6u(tmp, alphavec, OPT_FP->CNgrav);

    /* 'LIB_limit:27' if (in > max) */
    if (den > alphavec[9]) {
      /* 'LIB_limit:28' out = max; */
      den = alphavec[9];
    } else if (den < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    OPT_FP->alpha_M = den;

    /* 'GUID_Embedded_Guid_Control_1D:2377' OPT_FP.ClCd_M   = interp2(alphavec,machvec,CLCDtrim,OPT_FP.alpha_M,OPT_FP.M); */
    OPT_FP->ClCd_M = interp2_uQdDEBQV(alphavec, machvec, CLCDtrim, den,
      OPT_FP->M);

    /* 'GUID_Embedded_Guid_Control_1D:2379' if (debug && ftime > t_debug) */
    /* 'GUID_Embedded_Guid_Control_1D:2444' if (OPT_FP.ClCd_M < ClCd_M0) */
    guard1 = false;
    if (OPT_FP->ClCd_M < ClCd_M0) {
      /* 'GUID_Embedded_Guid_Control_1D:2445' if (OPT_FP.M == M0) */
      if (OPT_FP->M == M0) {
        /* 'GUID_Embedded_Guid_Control_1D:2446' m = 1.0; */
        den = 1.0;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:2447' else */
        /* 'GUID_Embedded_Guid_Control_1D:2448' m = (OPT_FP.ClCd_M-ClCd_M0)/(OPT_FP.M-M0); */
        den = (OPT_FP->ClCd_M - ClCd_M0) / (OPT_FP->M - M0);

        /* 'GUID_Embedded_Guid_Control_1D:2449' M0 = OPT_FP.M; */
        M0 = OPT_FP->M;
      }

      /* 'GUID_Embedded_Guid_Control_1D:2452' OPT_FP.M = OPT_FP.M + pol*LIB_limit(0.0001,0.1,0.8*abs(ClCd_M0-OPT_FP.ClCd_M)/abs(m)); */
      ClCd_M0 = (fabs(ClCd_M0 - OPT_FP->ClCd_M) * 0.8) / fabs(den);

      /* 'LIB_limit:27' if (in > max) */
      if (ClCd_M0 > 0.1) {
        /* 'LIB_limit:28' out = max; */
        ClCd_M0 = 0.1;
      } else if (ClCd_M0 < 0.0001) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        ClCd_M0 = 0.0001;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      OPT_FP->M += machinc * ClCd_M0;

      /* 'GUID_Embedded_Guid_Control_1D:2453' ClCd_M0 = OPT_FP.ClCd_M; */
      ClCd_M0 = OPT_FP->ClCd_M;
      guard1 = true;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2454' else */
      /* 'GUID_Embedded_Guid_Control_1D:2455' if (i==1 && M0 == Mbeg) */
      if (((i + 1) == 1) && (M0 == Mbeg)) {
        /* 'GUID_Embedded_Guid_Control_1D:2456' OPT_FP.M = Mbeg; */
        OPT_FP->M = Mbeg;

        /* 'GUID_Embedded_Guid_Control_1D:2457' OPT_FP.ClCd_M = ClCd_M0; */
        OPT_FP->ClCd_M = ClCd_M0;
        exitg1 = true;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:2459' else */
        /* 'GUID_Embedded_Guid_Control_1D:2460' pol = -pol; */
        machinc = -machinc;

        /* 'GUID_Embedded_Guid_Control_1D:2461' M1 = OPT_FP.M; */
        den = OPT_FP->M;

        /* 'GUID_Embedded_Guid_Control_1D:2462' ClCd_M0 = OPT_FP.ClCd_M; */
        ClCd_M0 = OPT_FP->ClCd_M;

        /* 'GUID_Embedded_Guid_Control_1D:2463' OPT_FP.M = 0.5*OPT_FP.M + 0.5*M0; */
        OPT_FP->M = (0.5 * OPT_FP->M) + (0.5 * M0);

        /* 'GUID_Embedded_Guid_Control_1D:2464' M0 = M1; */
        M0 = den;
        guard1 = true;
      }
    }

    if (guard1) {
      /* 'GUID_Embedded_Guid_Control_1D:2475' OPT_FP.M = LIB_limit(Mbeg,Mend,OPT_FP.M); */
      /* 'LIB_limit:27' if (in > max) */
      if (OPT_FP->M > Mend) {
        /* 'LIB_limit:28' out = max; */
        den = Mend;
      } else if (OPT_FP->M < Mbeg) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        den = Mbeg;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        den = OPT_FP->M;
      }

      OPT_FP->M = den;

      /* 'GUID_Embedded_Guid_Control_1D:2476' M0 = LIB_limit(Mbeg,Mend,M0); */
      /* 'LIB_limit:27' if (in > max) */
      if (M0 > Mend) {
        /* 'LIB_limit:28' out = max; */
        M0 = Mend;
      } else if (M0 < Mbeg) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        M0 = Mbeg;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'GUID_Embedded_Guid_Control_1D:2477' FP0 = FPopt; */
      i++;
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2479' if (debug && ftime > t_debug) */
  /* 'GUID_Embedded_Guid_Control_1D:2487' if (fromDATA.AirframeMode == 2) */
  if (((int32_T)fromDATA->AirframeMode) == 2) {
    /* 'GUID_Embedded_Guid_Control_1D:2490' OPT_FP.M = LIB_limit(0.40,0.70,OPT_FP.M); */
    /* 'LIB_limit:27' if (in > max) */
    if (OPT_FP->M > 0.7) {
      /* 'LIB_limit:28' out = max; */
      Sref = 0.7;
    } else if (OPT_FP->M < 0.4) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Sref = 0.4;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Sref = OPT_FP->M;
    }

    OPT_FP->M = Sref;

    /* 'GUID_Embedded_Guid_Control_1D:2491' Gmax = 20.0; */
    i = 20;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2492' else */
    /* 'GUID_Embedded_Guid_Control_1D:2493' Gmax = 50.0; */
    i = 50;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2496' if (GUIDMODE < 4) */
  if (((int32_T)GUIDMODE) < 4) {
    /* 'GUID_Embedded_Guid_Control_1D:2497' OPT_FP.Mopt   = ENV.mach; */
    OPT_FP->Mopt = ENV_mach;

    /* 'GUID_Embedded_Guid_Control_1D:2498' OPT_FP.FPthec = ENV.FPthem; */
    OPT_FP->FPthec = ENV_FPthem;

    /* 'GUID_Embedded_Guid_Control_1D:2499' OPT_FP.FPcmdD = 0.0; */
    OPT_FP->FPcmdD = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2500' OPT_FP.FPerrD = 0.0; */
    OPT_FP->FPerrD = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2501' OPT_FP.FPint  = ENV.FPthem; */
    OPT_FP->FPint = ENV_FPthem;

    /* 'GUID_Embedded_Guid_Control_1D:2502' OPT_FP.Vopt = ENV.Vairspeed; */
    OPT_FP->Vopt = ENV_Vairspeed;

    /* 'GUID_Embedded_Guid_Control_1D:2503' OPT_FP.Qopt = ENV.Q; */
    OPT_FP->Qopt = ENV_Q;

    /* 'GUID_Embedded_Guid_Control_1D:2504' alpha = LIB_limit(0.0,15.0,ENV.alpha*cnsts.r2d); */
    Fgrav = ENV_alpha * b_cnsts_r2d;

    /* 'LIB_limit:27' if (in > max) */
    if (Fgrav > 15.0) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = 15.0;
    } else if (Fgrav < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:2505' OPT_FP.Cd0 = -interp2(alphavec,machvec,CDtrim,alpha,ENV.mach); */
    OPT_FP->Cd0 = -interp2_uQdDEBQV(alphavec, machvec, CDtrim, Fgrav, ENV_mach);
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2506' else */
    /* 'GUID_Embedded_Guid_Control_1D:2508' OPT_FP.Mopt = OPT_FP.M; */
    OPT_FP->Mopt = OPT_FP->M;

    /* 'GUID_Embedded_Guid_Control_1D:2509' VELerr = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:2510' OPT_FP.FPthec = OPT_FP.FPthec - 0.5*10.0e-06*VELerr; */
    /* 'GUID_Embedded_Guid_Control_1D:2511' OPT_FP.FPthec = LIB_limit(ENV.FPthem-0.1745,ENV.FPthem+0.1745,OPT_FP.FPthec); */
    /* 'LIB_limit:27' if (in > max) */
    if (OPT_FP->FPthec > (ENV_FPthem + 0.1745)) {
      /* 'LIB_limit:28' out = max; */
      Sref = ENV_FPthem + 0.1745;
    } else if (OPT_FP->FPthec < (ENV_FPthem - 0.1745)) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Sref = ENV_FPthem - 0.1745;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Sref = OPT_FP->FPthec;
    }

    OPT_FP->FPthec = Sref;

    /* 'GUID_Embedded_Guid_Control_1D:2513' OPT_FP.Vopt = OPT_FP.Mopt*ENV.Vsound; */
    OPT_FP->Vopt = OPT_FP->Mopt * ENV_Vsound;

    /* 'GUID_Embedded_Guid_Control_1D:2514' OPT_FP.Qopt = 0.5*ENV.rho*OPT_FP.Vopt*OPT_FP.Vopt; */
    OPT_FP->Qopt = ((0.5 * ENV_rho) * OPT_FP->Vopt) * OPT_FP->Vopt;

    /* 'GUID_Embedded_Guid_Control_1D:2516' if (OPT_FP.FPCmd0 == 0.0) */
    if (OPT_FP->FPCmd0 == 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:2517' OPT_FP.FPint = 0.0; */
      OPT_FP->FPint = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:2518' OPT_FP.FPcmdD = LIB_limit(-1.0,0.1745,OPT_FP.FPopt - 0.0001*(OPT_FP.Qopt-ENV.Q)); */
      Sref = OPT_FP->FPopt - ((OPT_FP->Qopt - ENV_Q) * 0.0001);

      /* 'LIB_limit:27' if (in > max) */
      if (Sref > 0.1745) {
        /* 'LIB_limit:28' out = max; */
        OPT_FP->FPcmdD = 0.1745;
      } else if (Sref < -1.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        OPT_FP->FPcmdD = -1.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        OPT_FP->FPcmdD = Sref;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2519' else */
      /* 'GUID_Embedded_Guid_Control_1D:2521' OPT_FP.FPint = LIB_limit(-0.0873,0.0873,OPT_FP.FPint - 0.00000010*(OPT_FP.Qopt-ENV.Q)); */
      Sref = OPT_FP->FPint - ((OPT_FP->Qopt - ENV_Q) * 1.0E-7);

      /* 'LIB_limit:27' if (in > max) */
      if (Sref > 0.0873) {
        /* 'LIB_limit:28' out = max; */
        Sref = 0.0873;
      } else if (Sref < -0.0873) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Sref = -0.0873;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      OPT_FP->FPint = Sref;

      /* 'GUID_Embedded_Guid_Control_1D:2524' OPT_FP.FPcmdD = OPT_FP.FPopt + OPT_FP.FPint - 0.00005*(OPT_FP.Qopt-ENV.Q); */
      OPT_FP->FPcmdD = (OPT_FP->FPopt + Sref) - ((OPT_FP->Qopt - ENV_Q) * 5.0E-5);
    }

    /* 'GUID_Embedded_Guid_Control_1D:2527' OPT_FP.FPerrD = OPT_FP.FPcmdD-ENV.FPthem; */
    OPT_FP->FPerrD = OPT_FP->FPcmdD - ENV_FPthem;

    /* 'GUID_Embedded_Guid_Control_1D:2528' OPT_FP.FPCmd0 = LIB_limit(-1.0,0.1745,OPT_FP.FPcmdD); */
    /* 'LIB_limit:27' if (in > max) */
    if (OPT_FP->FPcmdD > 0.1745) {
      /* 'LIB_limit:28' out = max; */
      Sref = 0.1745;
    } else if (OPT_FP->FPcmdD < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Sref = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Sref = OPT_FP->FPcmdD;
    }

    OPT_FP->FPCmd0 = Sref;

    /* 'GUID_Embedded_Guid_Control_1D:2530' den = Sref*0.5*ENV.rho*(ENV.mach*ENV.Vsound)^2; */
    Sref = ENV_mach * ENV_Vsound;
    den = ((((real_T)fromDATA->AirframeProp.Sref) * 0.5) * ENV_rho) * (Sref *
      Sref);

    /* 'GUID_Embedded_Guid_Control_1D:2531' if (den < 1.0) */
    if (den < 1.0) {
      /* 'GUID_Embedded_Guid_Control_1D:2532' den = 1.0; */
      den = 1.0;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2534' CNgrav = Fgrav/den; */
    /* 'GUID_Embedded_Guid_Control_1D:2535' CL0    = interp1(machvec,CLtrim,ENV.mach,'linear','extrap'); */
    /* 'GUID_Embedded_Guid_Control_1D:2536' if (debug) */
    /* 'GUID_Embedded_Guid_Control_1D:2541' alpha  = LIB_limit(0.0,alphavec(end),interp1(CL0,alphavec,CNgrav,'linear','extrap')); */
    interp1_EKJBPReX(machvec, CLtrim, ENV_mach, tmp_0);
    Fgrav = interp1_UImAjR6u(tmp_0, alphavec, Fgrav / den);

    /* 'LIB_limit:27' if (in > max) */
    if (Fgrav > alphavec[9]) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = alphavec[9];
    } else if (Fgrav < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:2542' Cd     = -interp2(alphavec,machvec,CDtrim,alpha,ENV.mach); */
    Fgrav = -interp2_uQdDEBQV(alphavec, machvec, CDtrim, Fgrav, ENV_mach);

    /* 'GUID_Embedded_Guid_Control_1D:2544' OPT_FP.Cd0 = Cd; */
    OPT_FP->Cd0 = Fgrav;

    /* 'GUID_Embedded_Guid_Control_1D:2545' den = mass*(ENV.Q*ENV.atmbeta-9.81*ENV.rho); */
    den = ((ENV_Q * ENV_atmbeta) - (9.81 * ENV_rho)) * ((real_T)
      fromDATA->AirframeProp.mass);

    /* 'GUID_Embedded_Guid_Control_1D:2546' if (abs(den) > 0.001) */
    Sref = fabs(den);
    if (Sref > 0.001) {
      /* 'GUID_Embedded_Guid_Control_1D:2547' tmp = (Cd*ENV.Q*Sref*ENV.rho)/den; */
      Fgrav = (((Fgrav * ENV_Q) * ((real_T)fromDATA->AirframeProp.Sref)) *
               ENV_rho) / den;

      /* 'GUID_Embedded_Guid_Control_1D:2548' if (tmp < -1.0) */
      if (Fgrav < -1.0) {
        /* 'GUID_Embedded_Guid_Control_1D:2549' tmp = -1.0; */
        Fgrav = -1.0;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2551' else */
      /* 'GUID_Embedded_Guid_Control_1D:2552' tmp = -0.001; */
      Fgrav = -0.001;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2555' OPT_FP.FPopt = asin(tmp); */
    OPT_FP->FPopt = asin(Fgrav);

    /* 'GUID_Embedded_Guid_Control_1D:2556' OPT_FP.FPopt0 = LIB_limit(-1.0,0.1745,OPT_FP.FPopt); */
    /* 'LIB_limit:27' if (in > max) */
    if (OPT_FP->FPopt > 0.1745) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = 0.1745;
    } else if (OPT_FP->FPopt < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Fgrav = OPT_FP->FPopt;
    }

    OPT_FP->FPopt0 = Fgrav;

    /* 'GUID_Embedded_Guid_Control_1D:2558' Cd = -interp2(alphavec,machvec,CDtrim,OPT_FP.alpha_M,ENV.mach); */
    /* 'GUID_Embedded_Guid_Control_1D:2559' if (abs(den) > 0.001) */
    if (Sref > 0.001) {
      /* 'GUID_Embedded_Guid_Control_1D:2560' tmp = (Cd*ENV.Q*Sref*ENV.rho)/den; */
      Fgrav = ((((-interp2_uQdDEBQV(alphavec, machvec, CDtrim, OPT_FP->alpha_M,
        ENV_mach)) * ENV_Q) * ((real_T)fromDATA->AirframeProp.Sref)) * ENV_rho) /
        den;

      /* 'GUID_Embedded_Guid_Control_1D:2561' if (tmp < -1.0) */
      if (Fgrav < -1.0) {
        /* 'GUID_Embedded_Guid_Control_1D:2562' tmp = -1.0; */
        Fgrav = -1.0;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2564' else */
      /* 'GUID_Embedded_Guid_Control_1D:2565' tmp = -0.001; */
      Fgrav = -0.001;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2567' FPopt0 = asin(tmp); */
    /* 'GUID_Embedded_Guid_Control_1D:2568' OPT_FP.Mglide_opt = tan(FPopt0); */
    OPT_FP->Mglide_opt = tan(asin(Fgrav));
  }

  /* 'GUID_Embedded_Guid_Control_1D:2570' if (isnan(OPT_FP.Mglide_opt)) */
  switch (fromDATA->AirframeMode) {
   case 1:
    /* 'GUID_Embedded_Guid_Control_1D:2599' OPT_FP.Cl =  interp2(alphavec,machvec,CLtrim,LIB_limit(0.0,20.0,alphavec(end)),ENV.mach); */
    /* 'LIB_limit:27' if (in > max) */
    if (alphavec[9] > 20.0) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = 20.0;
    } else if (alphavec[9] < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Fgrav = alphavec[9];
    }

    OPT_FP->Cl = interp2_uQdDEBQV(alphavec, machvec, CLtrim, Fgrav, ENV_mach);
    break;

   case 2:
    /* 'GUID_Embedded_Guid_Control_1D:2600' elseif (fromDATA.AirframeMode == 2) */
    /* 'GUID_Embedded_Guid_Control_1D:2601' OPT_FP.Cl =  interp2(alphavec,machvec,CLtrim,LIB_limit(0.0,10.0,alphavec(end)),ENV.mach); */
    /* 'LIB_limit:27' if (in > max) */
    if (alphavec[9] > 10.0) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = 10.0;
    } else if (alphavec[9] < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Fgrav = alphavec[9];
    }

    OPT_FP->Cl = interp2_uQdDEBQV(alphavec, machvec, CLtrim, Fgrav, ENV_mach);
    break;

   default:
    /* 'GUID_Embedded_Guid_Control_1D:2602' else */
    /* 'GUID_Embedded_Guid_Control_1D:2606' OPT_FP.Cl =  interp2(alphavec,machvec,CLtrim,LIB_limit(0.0,18.0,alphavec(end)),ENV.mach); */
    /* 'LIB_limit:27' if (in > max) */
    if (alphavec[9] > 18.0) {
      /* 'LIB_limit:28' out = max; */
      Fgrav = 18.0;
    } else if (alphavec[9] < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Fgrav = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Fgrav = alphavec[9];
    }

    OPT_FP->Cl = interp2_uQdDEBQV(alphavec, machvec, CLtrim, Fgrav, ENV_mach);
    break;
  }

  /* 'GUID_Embedded_Guid_Control_1D:2609' if (mass > 0.0) */
  if (fromDATA->AirframeProp.mass > 0.0F) {
    /* 'GUID_Embedded_Guid_Control_1D:2610' if (fromDATA.AirframeType < 3) */
    if (((int32_T)fromDATA->AirframeType) < 3) {
      /* 'GUID_Embedded_Guid_Control_1D:2611' Gfac = LIB_limit(1.0,1.5,TTKFin.tgtrge/3000.0); */
      Fgrav = TTKFin_tgtrge / 3000.0;

      /* 'LIB_limit:27' if (in > max) */
      if (Fgrav > 1.5) {
        /* 'LIB_limit:28' out = max; */
        Fgrav = 1.5;
      } else if (Fgrav < 1.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Fgrav = 1.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2612' else */
      /* 'GUID_Embedded_Guid_Control_1D:2613' Gfac = 1.0; */
      Fgrav = 1.0;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2615' OPT_FP.GLim = Gfac*LIB_limit(0.0,Gmax,1.0*(ENV.Q*Sref*OPT_FP.Cl)/mass); */
    Sref = ((ENV_Q * ((real_T)fromDATA->AirframeProp.Sref)) * OPT_FP->Cl) /
      ((real_T)fromDATA->AirframeProp.mass);

    /* 'LIB_limit:27' if (in > max) */
    if (Sref > ((real_T)i)) {
      /* 'LIB_limit:28' out = max; */
      Sref = (real_T)i;
    } else if (Sref < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Sref = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    OPT_FP->GLim = Fgrav * Sref;

    /* 'GUID_Embedded_Guid_Control_1D:2618' the_off = TTKFin.the_b - double(fromDATA.seeker.Bele); */
    /* 'GUID_Embedded_Guid_Control_1D:2619' psi_off = TTKFin.psi_b; */
    /* 'GUID_Embedded_Guid_Control_1D:2620' if (0) */
    /* 'GUID_Embedded_Guid_Control_1D:2690' else */
    /* 'GUID_Embedded_Guid_Control_1D:2691' OPT_FP.GLimZup =  OPT_FP.GLim; */
    OPT_FP->GLimZup = OPT_FP->GLim;

    /* 'GUID_Embedded_Guid_Control_1D:2692' OPT_FP.GLimZlo = -OPT_FP.GLim; */
    OPT_FP->GLimZlo = -OPT_FP->GLim;

    /* 'GUID_Embedded_Guid_Control_1D:2693' OPT_FP.GLimYup =  OPT_FP.GLim; */
    OPT_FP->GLimYup = OPT_FP->GLim;

    /* 'GUID_Embedded_Guid_Control_1D:2694' OPT_FP.GLimYlo = -OPT_FP.GLim; */
    OPT_FP->GLimYlo = -OPT_FP->GLim;

    /* 'GUID_Embedded_Guid_Control_1D:2698' OPT_FP.GLimZup_prev = OPT_FP.GLimZup; */
    OPT_FP->GLimZup_prev = OPT_FP->GLimZup;

    /* 'GUID_Embedded_Guid_Control_1D:2700' OPT_FP.GLimZlo_prev = OPT_FP.GLimZlo; */
    OPT_FP->GLimZlo_prev = OPT_FP->GLimZlo;

    /* 'GUID_Embedded_Guid_Control_1D:2703' OPT_FP.GLimYup_prev = OPT_FP.GLimYup; */
    OPT_FP->GLimYup_prev = OPT_FP->GLimYup;

    /* 'GUID_Embedded_Guid_Control_1D:2705' OPT_FP.GLimYlo_prev = OPT_FP.GLimYlo; */
    OPT_FP->GLimYlo_prev = OPT_FP->GLimYlo;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2707' else */
    /* 'GUID_Embedded_Guid_Control_1D:2708' OPT_FP.GLimZup = Gmax; */
    OPT_FP->GLimZup = (real_T)i;

    /* 'GUID_Embedded_Guid_Control_1D:2709' OPT_FP.GLimZlo = Gmax; */
    OPT_FP->GLimZlo = (real_T)i;

    /* 'GUID_Embedded_Guid_Control_1D:2710' OPT_FP.GLimYup = Gmax; */
    OPT_FP->GLimYup = (real_T)i;

    /* 'GUID_Embedded_Guid_Control_1D:2711' OPT_FP.GLimYlo = Gmax; */
    OPT_FP->GLimYlo = (real_T)i;
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [V_Rto,V_Rte,V_Rwo,V_Rwe,V_Rmwo,V_Rmto,V_Rm,V_RarcT,V_OarcT,V_RarcW,V_OarcW, ...
 *           Uw,solution,FPazi,Lfpath2bunt,Lfpath2tgt,Ut,latch,ratio_gcapability,Mglide,telstruct,iW] = ...
 *   GetDoubleArcVectors(ftime,opportunity_mode,once,solution,FPazi,tgt0,Ut0,Uv0,Kglide,Mglide0,Mach_diff,latch, ...
 *                       Rterm0,Rlim_lo,Rarc0,psi_Engage,Weapon_Type,AFmode,log_FP,telstruct,iW,do_plot)
 */
static void GetDoubleArcVectors(real_T ftime, uint8_T opportunity_mode, int8_T
  *solution, real_T *FPazi, real_T tgt0_x, real_T tgt0_y, real_T tgt0_z, real_T
  Ut0[3], const real_T Uv0[3], real_T Kglide, real_T Mglide0, real_T Mach_diff,
  uint8_T latch[2], real_T Rlim_lo, real_T Rarc0, real_T psi_Engage, uint8_T
  Weapon_Type, uint8_T AFmode, uint8_T log_FP, sFMpPTQ95PcZEskZd3p4FcD
  *telstruct, uint16_T *iW, T_Cartesian *V_Rto, T_Cartesian *V_Rte, T_Cartesian *
  V_Rwo, T_Cartesian *V_Rwe, T_Cartesian *V_Rmwo, T_Cartesian *V_Rmto,
  T_Cartesian *V_Rm, real_T *V_RarcT, T_Cartesian *V_OarcT, real_T *V_RarcW,
  T_Cartesian *V_OarcW, real_T Uw[3], real_T *Lfpath2bunt, real_T *Lfpath2tgt,
  real_T Ut[3], real_T *ratio_gcapability, real_T *Mglide)
{
  real_T Rarc0Lim;
  real_T Earc;
  real_T Rto[3];
  real_T tgt0rge;
  real_T FP0;
  real_T FPlim;
  real_T tgtrgehor;
  real_T LOSazi;
  real_T LOSrelV;
  real_T FP_prev;
  real_T delRE_prev;
  real_T Rterm;
  real_T RtermW;
  real_T RtermT;
  int32_T latch_R4;
  real_T Mfpath;
  real_T den;
  int32_T pol;
  real_T ratio_prev;
  int32_T count;
  real_T RarcW_prev;
  real_T Lw;
  real_T Lt;
  real_T Km;
  real_T Rte_0[3];
  real_T a1[3];
  real_T La1;
  real_T Rwo[3];
  real_T Um[3];
  real_T U_a1[3];
  real_T mRwe;
  real_T theT;
  real_T OarcT[3];
  real_T ImpAng;
  real_T AttHeading;
  real_T psiUw;
  real_T psiUt0;
  real_T Larc;
  real_T Oarc[3];
  real_T vecV[3];
  real_T Ut_V[3];
  real_T Ut_tmp[3];
  real_T Utxy[2];
  real_T Uv0xy[2];
  real_T vecT[3];
  real_T Uw_plane[3];
  real_T K[9];
  int32_T i;
  real_T e_min;
  real_T d_max;
  static const int8_T b_I[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real_T Rto_tmp[3];
  boolean_T tmp;
  real_T tmp_0[3];
  real_T Uw_0[3];
  real_T tmp_1[9];
  real_T Rto_tmp_0[3];
  real_T vecT_0[2];
  real_T Rm[2];
  real_T Uv0_0[2];
  boolean_T tmp_2;
  real_T tmp_3[3];
  boolean_T tmp_4;
  real_T tmp_5[3];
  boolean_T tmp_6;
  real_T tmp_7[3];
  boolean_T tmp_8;
  real_T tmp_9[3];
  real_T b_I_0[3];
  int32_T i_0;
  int32_T i_1;
  real_T U_a1_0;
  real_T Rm_idx_0;
  real_T Rm_idx_1;
  real_T Rm_idx_2;
  real_T Rm_idx_0_0;
  real_T Rm_idx_1_0;
  real_T Rm_idx_2_0;
  real_T den_tmp;
  real_T den_tmp_0;
  real_T den_tmp_1;
  real_T La1_tmp;
  real_T tmp_tmp;
  uint32_T tmp_a;
  int32_T b_I_tmp;
  real_T tmp_b[3];
  real_T tmp_c[3];
  real_T tmp_d[3];
  int32_T exitg1;
  boolean_T exitg2;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T guard3 = false;
  boolean_T guard4 = false;
  tmp_8 = false;
  tmp_6 = false;
  tmp_4 = false;
  tmp_2 = false;
  tmp = false;
  *solution = 0;

  /* 'DefineFlightPath:176' if (do_plot) */
  /* 'DefineFlightPath:227' if (ftime > 4.5) */
  /* 'DefineFlightPath:231' no_iter = 30; */
  /* 'DefineFlightPath:232' ratio_gcapability = 1.0; */
  *ratio_gcapability = 1.0;

  /* 'DefineFlightPath:233' Rarc = Rarc0; */
  /* 'DefineFlightPath:234' Rarc0Lim = Rarc0; */
  Rarc0Lim = Rarc0;

  /* 'DefineFlightPath:235' Earc = 0.0; */
  Earc = 0.0;

  /* 'DefineFlightPath:236' Rto = [tgt0.x tgt0.y tgt0.z]; */
  Rto_tmp[0] = tgt0_x;
  Rto_tmp[1] = tgt0_y;
  Rto_tmp[2] = tgt0_z;
  Rto[0] = tgt0_x;
  Rto[1] = tgt0_y;
  Rto[2] = tgt0_z;

  /* 'DefineFlightPath:237' tgt0rge = LIB_limit(0.001,1000000.0,norm(Rto)); */
  tgt0rge = norm_MfVYifVG(Rto_tmp);

  /* 'LIB_limit:27' if (in > max) */
  if (tgt0rge > 1.0E+6) {
    /* 'LIB_limit:28' out = max; */
    tgt0rge = 1.0E+6;
  } else if (tgt0rge < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    tgt0rge = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'DefineFlightPath:238' FP0 = atan2(Uv0(2),Uv0(1)); */
  FP0 = rt_atan2d_snf(Uv0[1], Uv0[0]);

  /* 'DefineFlightPath:239' FPlim = (5500.0/tgt0rge)^2; */
  FPlim = 5500.0 / tgt0rge;
  FPlim *= FPlim;

  /* 'DefineFlightPath:240' tgtrgehor = sqrt(tgt0.x^2 + tgt0.y^2); */
  tgtrgehor = sqrt((tgt0_x * tgt0_x) + (tgt0_y * tgt0_y));

  /* 'DefineFlightPath:241' LOSazi = atan2(tgt0.y,tgt0.x); */
  LOSazi = rt_atan2d_snf(tgt0_y, tgt0_x);

  /* 'DefineFlightPath:242' LOSrelV = FP0 - LOSazi; */
  LOSrelV = FP0 - LOSazi;

  /* 'DefineFlightPath:243' FP_prev = 0.0; */
  FP_prev = 0.0;

  /* 'DefineFlightPath:244' delRE_prev = -1.0e06; */
  delRE_prev = -1.0E+6;

  /* 'DefineFlightPath:245' Rterm  = Rterm0; */
  Rterm = 2000.0;

  /* 'DefineFlightPath:246' RtermW = Rterm0; */
  RtermW = 2000.0;

  /* 'DefineFlightPath:247' RtermT = Rterm0; */
  RtermT = 2000.0;

  /* 'DefineFlightPath:249' latch_R4 = 0; */
  latch_R4 = 0;

  /* 'DefineFlightPath:252' Ut = Ut0; */
  /* 'DefineFlightPath:254' a0 = -Rto; */
  /* 'DefineFlightPath:255' [Rwe,RarcW,OarcW] = LIB_getreflvec(a0,RtermT*Ut',Rto); */
  Ut[0] = Ut0[0];
  Rto_tmp_0[0] = -tgt0_x;
  b_I_0[0] = 2000.0 * Ut0[0];
  Ut[1] = Ut0[1];
  Rto_tmp_0[1] = -tgt0_y;
  b_I_0[1] = 2000.0 * Ut0[1];
  Ut[2] = Ut0[2];
  Rto_tmp_0[2] = -tgt0_z;
  b_I_0[2] = 2000.0 * Ut0[2];
  LIB_getreflvec_9sSePAMS(Rto_tmp_0, b_I_0, Rto_tmp, vecT, V_RarcW, Oarc);

  /* 'DefineFlightPath:257' Mfpath = -Uv0(3)/sqrt(Uv0(1)^2+Uv0(2)^2); */
  Mfpath = (-Uv0[2]) / sqrt((Uv0[0] * Uv0[0]) + (Uv0[1] * Uv0[1]));

  /* 'DefineFlightPath:259' if (Mfpath > Mglide0) */
  if (Mfpath > Mglide0) {
    /* 'DefineFlightPath:260' Mglide = Mglide0; */
    *Mglide = Mglide0;
  } else {
    /* 'DefineFlightPath:261' else */
    /* 'DefineFlightPath:262' den = sqrt(Rwe(1)^2+Rwe(2)^2); */
    den = sqrt((vecT[0] * vecT[0]) + (vecT[1] * vecT[1]));

    /* 'DefineFlightPath:263' if (den > 0.001) */
    if (den > 0.001) {
      /* 'DefineFlightPath:264' Mglide = LIB_limit(Mfpath,Mglide0,-Rwe(3)/den); */
      *Mglide = (-vecT[2]) / den;

      /* 'LIB_limit:27' if (in > max) */
      if ((*Mglide) > Mglide0) {
        /* 'LIB_limit:28' out = max; */
        *Mglide = Mglide0;
      } else if ((*Mglide) < Mfpath) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        *Mglide = Mfpath;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'DefineFlightPath:265' else */
      /* 'DefineFlightPath:266' Mglide = -0.001; */
      *Mglide = -0.001;
    }
  }

  /* 'DefineFlightPath:270' eg.phi=0; */
  /* 'DefineFlightPath:270' eg.the = atan(Mglide); */
  /* 'DefineFlightPath:270' eg.psi = FPazi; */
  /* 'DefineFlightPath:271' DCMfpath = LIB_e2d(eg); */
  /* 'DefineFlightPath:272' fpath_vec = DCMfpath*[RtermW 0 0]'; */
  /* 'DefineFlightPath:274' Uw = LIB_unitvector(fpath_vec); */
  LIB_e2d(0.0, atan(*Mglide), *FPazi, tmp_1);
  for (i_0 = 0; i_0 < 3; i_0++) {
    b_I_0[i_0] = (tmp_1[i_0 + 6] * 0.0) + ((tmp_1[i_0 + 3] * 0.0) + (tmp_1[i_0] *
      2000.0));
  }

  LIB_unitvector_gdXEDyOJ(b_I_0, Uw);

  /* 'DefineFlightPath:276' Ctmp = cross([Uw(1) Uw(2) 0],[Ut(1) Ut(2) 0]); */
  /* 'DefineFlightPath:277' if (Ctmp(3) < 0.0) */
  Uw_0[0] = Uw[0];
  Uw_0[1] = Uw[1];
  Uw_0[2] = 0.0;
  Uw_plane[0] = Ut0[0];
  Uw_plane[1] = Ut0[1];
  Uw_plane[2] = 0.0;
  cross_6Ajhxtbg(Uw_0, Uw_plane, b_I_0);
  if (b_I_0[2] < 0.0) {
    /* 'DefineFlightPath:278' pol = -1; */
    pol = -1;
  } else {
    /* 'DefineFlightPath:279' else */
    /* 'DefineFlightPath:280' pol = +1; */
    pol = 1;
  }

  /* 'DefineFlightPath:283' ratio_prev = 1.0; */
  ratio_prev = 1.0;

  /* 'DefineFlightPath:285' Kele = 1.0; */
  /* 'DefineFlightPath:286' count = 0; */
  count = 0;

  /* 'DefineFlightPath:287' RarcW_prev = 0.0; */
  RarcW_prev = 0.0;

  /* 'DefineFlightPath:288' for i = 1:no_iter */
  i = 1;
  exitg2 = false;
  while ((!exitg2) && ((i - 1) < 30)) {
    /* 'DefineFlightPath:290' den = (Uw(1)^2*Ut(2)^2 + Uw(1)^2*Ut(3)^2 - 2*Uw(1)*Uw(2)*Ut(1)*Ut(2) - 2*Uw(1)*Uw(3)*Ut(1)*Ut(3) + Uw(2)^2*Ut(1)^2 + Uw(2)^2*Ut(3)^2 - 2*Uw(2)*Uw(3)*Ut(2)*Ut(3) + Uw(3)^2*Ut(1)^2 + Uw(3)^2*Ut(2)^2); */
    Km = Uw[0] * Uw[0];
    La1 = Uw[1] * Uw[1];
    den_tmp = Ut[2] * Ut[2];
    den_tmp_0 = Ut[0] * Ut[0];
    Lt = Uw[2] * Uw[2];
    den_tmp_1 = Ut[1] * Ut[1];
    den = ((((((((Km * den_tmp_1) + (Km * den_tmp)) - ((((2.0 * Uw[0]) * Uw[1]) *
      Ut[0]) * Ut[1])) - ((((2.0 * Uw[0]) * Uw[2]) * Ut[0]) * Ut[2])) + (La1 *
               den_tmp_0)) + (La1 * den_tmp)) - ((((2.0 * Uw[1]) * Uw[2]) * Ut[1])
             * Ut[2])) + (Lt * den_tmp_0)) + (Lt * den_tmp_1);

    /* 'DefineFlightPath:291' if (abs(den) > 0.001) */
    if (fabs(den) > 0.001) {
      /* 'DefineFlightPath:292' Lw = (tgt0.x*Uw(1)*Ut(2)^2 + tgt0.x*Uw(1)*Ut(3)^2 + tgt0.y*Uw(2)*Ut(1)^2 + tgt0.y*Uw(2)*Ut(3)^2 + tgt0.z*Uw(3)*Ut(1)^2 + tgt0.z*Uw(3)*Ut(2)^2 - tgt0.x*Uw(2)*Ut(1)*Ut(2) - tgt0.y*Uw(1)*Ut(1)*Ut(2) - tgt0.x*Uw(3)*Ut(1)*Ut(3) - tgt0.z*Uw(1)*Ut(1)*Ut(3) - tgt0.y*Uw(3)*Ut(2)*Ut(3) - tgt0.z*Uw(2)*Ut(2)*Ut(3))/den; */
      Lw = tgt0_x * Uw[0];
      theT = tgt0_y * Uw[1];
      Rm_idx_0 = tgt0_z * Uw[2];
      Lw = ((((((((((((Lw * den_tmp_1) + (Lw * den_tmp)) + (theT * den_tmp_0)) +
                    (theT * den_tmp)) + (Rm_idx_0 * den_tmp_0)) + (Rm_idx_0 *
        den_tmp_1)) - (((tgt0_x * Uw[1]) * Ut[0]) * Ut[1])) - (((tgt0_y * Uw[0])
                  * Ut[0]) * Ut[1])) - (((tgt0_x * Uw[2]) * Ut[0]) * Ut[2])) -
              (((tgt0_z * Uw[0]) * Ut[0]) * Ut[2])) - (((tgt0_y * Uw[2]) * Ut[1])
              * Ut[2])) - (((tgt0_z * Uw[1]) * Ut[1]) * Ut[2])) / den;

      /* 'DefineFlightPath:298' Lt = -(tgt0.x*Uw(2)^2*Ut(1) + tgt0.x*Uw(3)^2*Ut(1) + tgt0.y*Uw(1)^2*Ut(2) + tgt0.y*Uw(3)^2*Ut(2) + tgt0.z*Uw(1)^2*Ut(3) + tgt0.z*Uw(2)^2*Ut(3) - tgt0.x*Uw(1)*Uw(2)*Ut(2) - tgt0.y*Uw(1)*Uw(2)*Ut(1) - tgt0.x*Uw(1)*Uw(3)*Ut(3) - tgt0.z*Uw(1)*Uw(3)*Ut(1) - tgt0.y*Uw(2)*Uw(3)*Ut(3) - tgt0.z*Uw(2)*Uw(3)*Ut(2))/den; */
      Lt = (-(((((((((((((La1 * tgt0_x) * Ut[0]) + ((Lt * tgt0_x) * Ut[0])) +
                       ((Km * tgt0_y) * Ut[1])) + ((Lt * tgt0_y) * Ut[1])) +
                     ((Km * tgt0_z) * Ut[2])) + ((La1 * tgt0_z) * Ut[2])) -
                   (((tgt0_x * Uw[0]) * Uw[1]) * Ut[1])) - (((tgt0_y * Uw[0]) *
        Uw[1]) * Ut[0])) - (((tgt0_x * Uw[0]) * Uw[2]) * Ut[2])) - (((tgt0_z *
                  Uw[0]) * Uw[2]) * Ut[0])) - (((tgt0_y * Uw[1]) * Uw[2]) * Ut[2]))
              - (((tgt0_z * Uw[1]) * Uw[2]) * Ut[1]))) / den;
    } else {
      /* 'DefineFlightPath:293' else */
      /* 'DefineFlightPath:294' Lw = tgt0rge; */
      Lw = tgt0rge;

      /* 'DefineFlightPath:299' else */
      /* 'DefineFlightPath:300' Lt = tgt0rge; */
      Lt = tgt0rge;
    }

    /* 'DefineFlightPath:296' den = (Uw(1)^2*Ut(2)^2 + Uw(1)^2*Ut(3)^2 - 2*Uw(1)*Uw(2)*Ut(1)*Ut(2) - 2*Uw(1)*Uw(3)*Ut(1)*Ut(3) + Uw(2)^2*Ut(1)^2 + Uw(2)^2*Ut(3)^2 - 2*Uw(2)*Uw(3)*Ut(2)*Ut(3) + Uw(3)^2*Ut(1)^2 + Uw(3)^2*Ut(2)^2); */
    /* 'DefineFlightPath:297' if (abs(den) > 0.001) */
    /* 'DefineFlightPath:303' Km = Lw - Lt; */
    Km = Lw - Lt;

    /* 'DefineFlightPath:305' Rte = RtermT*Ut' + [tgt0.x tgt0.y tgt0.z]; */
    /* 'DefineFlightPath:306' Rwe = (Km+RtermW)*Uw'; */
    La1 = Km + RtermW;

    /* 'DefineFlightPath:307' a1 = Rte-Rwe; */
    /* 'DefineFlightPath:308' La1  = norm(a1); */
    Oarc[0] = ((RtermT * Ut[0]) + tgt0_x) - (La1 * Uw[0]);
    Oarc[1] = ((RtermT * Ut[1]) + tgt0_y) - (La1 * Uw[1]);
    Oarc[2] = ((RtermT * Ut[2]) + tgt0_z) - (La1 * Uw[2]);
    La1 = norm_MfVYifVG(Oarc);

    /* 'DefineFlightPath:310' if (i==1 && La1 < 2*Rterm) */
    if ((i == 1) && (La1 < (2.0 * Rterm))) {
      /* 'DefineFlightPath:311' Rterm = 0.5*La1; */
      Rterm = 0.5 * La1;

      /* 'DefineFlightPath:312' RtermW = Rterm; */
      RtermW = Rterm;

      /* 'DefineFlightPath:313' RtermT = Rterm; */
      RtermT = Rterm;
    }

    /* 'DefineFlightPath:316' if (ratio_gcapability < 0.01) */
    if ((*ratio_gcapability) < 0.01) {
      /* 'DefineFlightPath:317' Rterm = LIB_limit(Rlim_lo,1000000.0,Rterm - 0.2*LIB_limit(-500.0,500.0,Earc)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Earc > 500.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 500.0;
      } else if (Earc < -500.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -500.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      Rterm -= 0.2 * Earc;

      /* 'LIB_limit:27' if (in > max) */
      if (Rterm > 1.0E+6) {
        /* 'LIB_limit:28' out = max; */
        Rterm = 1.0E+6;
      } else if (Rterm < Rlim_lo) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rterm = Rlim_lo;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'DefineFlightPath:318' RtermW = Rterm; */
      RtermW = Rterm;

      /* 'DefineFlightPath:319' RtermT = Rterm; */
      RtermT = Rterm;
    }

    /* 'DefineFlightPath:321' if (Km < 0.0) */
    if (Km < 0.0) {
      /* 'DefineFlightPath:324' RtermW = LIB_limit(Rlim_lo,1000000.0,Rterm + Km); */
      RtermW = Rterm + Km;

      /* 'LIB_limit:27' if (in > max) */
      if (RtermW > 1.0E+6) {
        /* 'LIB_limit:28' out = max; */
        RtermW = 1.0E+6;
      } else if (RtermW < Rlim_lo) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RtermW = Rlim_lo;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'DefineFlightPath:325' RtermT = LIB_limit(0.0,1.0*tgt0rge,Lt - (Lw-RtermW)); */
      RtermT = Lt - (Lw - RtermW);

      /* 'LIB_limit:27' if (in > max) */
      if (RtermT > tgt0rge) {
        /* 'LIB_limit:28' out = max; */
        RtermT = tgt0rge;
      } else if (RtermT < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RtermT = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'DefineFlightPath:326' Km = 0.0; */
      Km = 0.0;
    }

    /* 'DefineFlightPath:330' intsctW.pos = Lw*Uw'; */
    /* 'DefineFlightPath:331' intsctT.pos = Lt*Ut' + [tgt0.x tgt0.y tgt0.z]; */
    /* 'DefineFlightPath:332' mid_vecE.pos = 0.5*(intsctW.pos + intsctT.pos); */
    /* 'DefineFlightPath:334' Rte = RtermT*Ut' + [tgt0.x tgt0.y tgt0.z]; */
    /* 'DefineFlightPath:335' Rto = (RtermT-RtermW)*Ut' + [tgt0.x tgt0.y tgt0.z]; */
    La1 = RtermT - RtermW;
    Rto_tmp_0[0] = Lw * Uw[0];
    den_tmp = (Lt * Ut[0]) + tgt0_x;
    Larc = (RtermT * Ut[0]) + tgt0_x;
    Rte_0[0] = Larc;
    Rto[0] = (La1 * Ut[0]) + tgt0_x;
    Rto_tmp_0[1] = Lw * Uw[1];
    den_tmp_0 = (Lt * Ut[1]) + tgt0_y;
    Rm_idx_0_0 = (RtermT * Ut[1]) + tgt0_y;
    Rte_0[1] = Rm_idx_0_0;
    Rto[1] = (La1 * Ut[1]) + tgt0_y;
    Rto_tmp_0[2] = Lw * Uw[2];
    den_tmp_1 = (Lt * Ut[2]) + tgt0_z;
    ImpAng = (RtermT * Ut[2]) + tgt0_z;
    Rte_0[2] = ImpAng;
    Rto[2] = (La1 * Ut[2]) + tgt0_z;

    /* 'DefineFlightPath:337' Rwe = (Km+RtermW)*Uw'; */
    La1_tmp = Km + RtermW;

    /* 'DefineFlightPath:338' Rwo =  Km*Uw'; */
    /* 'DefineFlightPath:339' mid_vec0.pos = 0.5*(Rto + Rwo); */
    /* 'DefineFlightPath:342' Um = LIB_unitvector(mid_vecE.pos-mid_vec0.pos); */
    vecT[0] = La1_tmp * Uw[0];
    psiUt0 = Km * Uw[0];
    Earc = (Rto[0] + psiUt0) * 0.5;
    b_I_0[0] = ((Rto_tmp_0[0] + den_tmp) * 0.5) - Earc;
    Rwo[0] = psiUt0;
    Oarc[0] = Earc;
    vecT[1] = La1_tmp * Uw[1];
    psiUt0 = Km * Uw[1];
    Earc = (Rto[1] + psiUt0) * 0.5;
    b_I_0[1] = ((Rto_tmp_0[1] + den_tmp_0) * 0.5) - Earc;
    Rwo[1] = psiUt0;
    Oarc[1] = Earc;
    vecT[2] = La1_tmp * Uw[2];
    psiUt0 = Km * Uw[2];
    Earc = (Rto[2] + psiUt0) * 0.5;
    b_I_0[2] = ((Rto_tmp_0[2] + den_tmp_1) * 0.5) - Earc;
    Rwo[2] = psiUt0;
    LIB_unitvector_gdXEDyOJ(b_I_0, Um);

    /* 'DefineFlightPath:344' den = (Uw(1)*Um(1) + Uw(2)*Um(2) + Uw(3)*Um(3)); */
    den = ((Uw[0] * Um[0]) + (Uw[1] * Um[1])) + (Uw[2] * Um[2]);

    /* 'DefineFlightPath:345' if (den > 0.001) */
    if (den > 0.001) {
      /* 'DefineFlightPath:346' K_Owe = (Uw(1)*(Rwe(1) - mid_vec0.pos(1)) + Uw(2)*(Rwe(2) - mid_vec0.pos(2)) + Uw(3)*(Rwe(3) - mid_vec0.pos(3)))/den; */
      den = ((((vecT[0] - Oarc[0]) * Uw[0]) + ((vecT[1] - Oarc[1]) * Uw[1])) +
             ((vecT[2] - Earc) * Uw[2])) / den;
    } else {
      /* 'DefineFlightPath:347' else */
      /* 'DefineFlightPath:348' K_Owe = 1000.0; */
      den = 1000.0;
    }

    /* 'DefineFlightPath:351' Owe.x = K_Owe*Um(1) + mid_vec0.pos(1); */
    La1 = (den * Um[0]) + Oarc[0];

    /* 'DefineFlightPath:352' Owe.y = K_Owe*Um(2) + mid_vec0.pos(2); */
    theT = (den * Um[1]) + Oarc[1];

    /* 'DefineFlightPath:353' Owe.z = K_Owe*Um(3) + mid_vec0.pos(3); */
    den = (den * Um[2]) + Earc;

    /* 'DefineFlightPath:354' R_arcW = norm([Owe.x Owe.y Owe.z]-Rwe); */
    Oarc[0] = La1 - vecT[0];
    Oarc[1] = theT - vecT[1];
    Oarc[2] = den - vecT[2];
    Earc = norm_MfVYifVG(Oarc);

    /* 'DefineFlightPath:355' Rm = [Owe.x Owe.y Owe.z] + R_arcW*Um; */
    /* 'DefineFlightPath:357' a1 = Rm - Rwe; */
    Rm_idx_2 = (Earc * Um[0]) + La1;
    a1[0] = Rm_idx_2 - vecT[0];
    Rm_idx_0 = Rm_idx_2;
    Rm_idx_2 = (Earc * Um[1]) + theT;
    a1[1] = Rm_idx_2 - vecT[1];
    Rm_idx_1 = Rm_idx_2;
    Rm_idx_2 = (Earc * Um[2]) + den;
    a1[2] = Rm_idx_2 - vecT[2];

    /* 'DefineFlightPath:359' U_a1 = LIB_unitvector(a1); */
    LIB_unitvector_gdXEDyOJ(a1, U_a1);

    /* 'DefineFlightPath:360' tmp = LIB_limit(-1.0,1.0,dot(Uw,U_a1)); */
    den = dot_lFwBkxue(Uw, U_a1);

    /* 'LIB_limit:27' if (in > max) */
    if (den > 1.0) {
      /* 'LIB_limit:28' out = max; */
      den = 1.0;
    } else if (den < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'DefineFlightPath:361' theW = acos(tmp); */
    La1 = acos(den);

    /* 'DefineFlightPath:362' mag_a0 = norm(a1) + 2*RtermW*cos(theW); */
    den = ((2.0 * RtermW) * cos(La1)) + norm_MfVYifVG(a1);

    /* 'DefineFlightPath:363' a0 = mag_a0*U_a1; */
    /* 'DefineFlightPath:364' [aprW,RarcW,OarcW] = LIB_getreflvec(a0,RtermW*Uw',Rwo); */
    U_a1_0 = den * U_a1[0];
    Uw_0[0] = RtermW * Uw[0];
    Um[0] = Rwo[0] + U_a1_0;
    U_a1[0] = U_a1_0;
    U_a1_0 = den * U_a1[1];
    Uw_0[1] = RtermW * Uw[1];
    Um[1] = Rwo[1] + U_a1_0;
    U_a1[1] = U_a1_0;
    U_a1_0 = den * U_a1[2];
    Uw_0[2] = RtermW * Uw[2];
    Um[2] = psiUt0 + U_a1_0;
    U_a1[2] = U_a1_0;
    LIB_getreflvec_9sSePAMS(U_a1, Uw_0, Rwo, vecV, V_RarcW, Oarc);

    /* 'DefineFlightPath:365' a0W = Rwo + a0; */
    /* 'DefineFlightPath:366' den = sqrt((a0W(1)-Rm(1))^2+(a0W(2)-Rm(2))^2); */
    theT = Um[0] - Rm_idx_0;
    den = Um[1] - Rm_idx_1;
    den = sqrt((theT * theT) + (den * den));

    /* 'DefineFlightPath:367' if (den > 0.001) */
    if (den > 0.001) {
      /* 'DefineFlightPath:368' mRwe = -(a0W(3)-Rm(3))/den; */
      mRwe = (-(Um[2] - Rm_idx_2)) / den;
    } else {
      /* 'DefineFlightPath:369' else */
      /* 'DefineFlightPath:370' mRwe = -1000.0; */
      mRwe = -1000.0;
    }

    /* 'DefineFlightPath:373' a1 = Rm - Rte; */
    a1[0] = Rm_idx_0 - Larc;
    a1[1] = Rm_idx_1 - Rm_idx_0_0;
    a1[2] = Rm_idx_2 - ImpAng;

    /* 'DefineFlightPath:375' U_a1 = LIB_unitvector(a1); */
    LIB_unitvector_gdXEDyOJ(a1, U_a1);

    /* 'DefineFlightPath:376' tmp = LIB_limit(-1.0,1.0,dot(Ut,U_a1)); */
    den = dot_lFwBkxue(Ut, U_a1);

    /* 'LIB_limit:27' if (in > max) */
    if (den > 1.0) {
      /* 'LIB_limit:28' out = max; */
      den = 1.0;
    } else if (den < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'DefineFlightPath:377' theT = acos(tmp); */
    theT = acos(den);

    /* 'DefineFlightPath:378' mag_a0 = norm(a1) + 2*RtermW*cos(theT); */
    den = ((2.0 * RtermW) * cos(theT)) + norm_MfVYifVG(a1);

    /* 'DefineFlightPath:379' a0 = mag_a0*U_a1; */
    /* 'DefineFlightPath:380' [aprT,RarcT,OarcT] = LIB_getreflvec(a0,RtermW*Ut',Rto); */
    Uw_0[0] = RtermW * Ut[0];
    U_a1[0] *= den;
    Uw_0[1] = RtermW * Ut[1];
    U_a1[1] *= den;
    den *= U_a1[2];
    Uw_0[2] = RtermW * Ut[2];
    U_a1[2] = den;
    LIB_getreflvec_9sSePAMS(U_a1, Uw_0, Rto, vecV, V_RarcT, OarcT);

    /* 'DefineFlightPath:381' a0T = Rto + a0; */
    U_a1[0] += Rto[0];
    U_a1[1] += Rto[1];
    U_a1_0 = Rto[2] + den;
    U_a1[2] = U_a1_0;

    /* 'DefineFlightPath:383' if (ftime > 4.5) */
    /* 'DefineFlightPath:387' ImpAng = atan(-Ut(3)/sqrt(Ut(1)^2+Ut(2)^2)); */
    /* 'DefineFlightPath:388' AttHeading = atan2(Ut(2),Ut(1)); */
    AttHeading = rt_atan2d_snf(Ut[1], Ut[0]);

    /* 'DefineFlightPath:389' if (AttHeading < 0.0) */
    if (AttHeading < 0.0) {
      /* 'DefineFlightPath:390' AttHeading = AttHeading + 2*pi; */
      AttHeading += 6.2831853071795862;
    }

    /* 'DefineFlightPath:399' psiUw  = atan2(Uw(2),Uw(1)); */
    Earc = rt_atan2d_snf(Uw[1], Uw[0]);

    /* 'DefineFlightPath:400' psiUt0 = atan2(-Ut0(2),-Ut0(1)); */
    psiUw = rt_atan2d_snf(-Ut0[1], -Ut0[0]);

    /* 'DefineFlightPath:401' psifac = LIB_limit(0.0,1.0,tgtrgehor/7000.0); */
    den = tgtrgehor / 7000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (den > 1.0) {
      /* 'LIB_limit:28' out = max; */
      den = 1.0;
    } else if (den < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'DefineFlightPath:402' X = (1.0-psifac)*cos(psiUw) + psifac*cos(psiUt0); */
    /* 'DefineFlightPath:403' Y = (1.0-psifac)*sin(psiUw) + psifac*sin(psiUt0); */
    /* 'DefineFlightPath:404' psiApproach = atan2(Y,X); */
    psiUw = rt_atan2d_snf(((1.0 - den) * sin(Earc)) + (den * sin(psiUw)), ((1.0
      - den) * cos(Earc)) + (den * cos(psiUw)));

    /* 'DefineFlightPath:405' UtN = [0 0 0]; */
    /* 'DefineFlightPath:406' UtN(1) = Ut(1)*cos(psiApproach) + Ut(2)*sin(psiApproach); */
    /* 'DefineFlightPath:407' UtN(2) =-Ut(1)*sin(psiApproach) + Ut(2)*cos(psiApproach); */
    /* 'DefineFlightPath:408' UtN(3) = Ut(3); */
    /* 'DefineFlightPath:409' fac90 = LIB_limit(0.0,1.0,atan(UtN(1)/-UtN(3))+0.5236); */
    den = atan(((Ut[0] * cos(psiUw)) + (Ut[1] * sin(psiUw))) / (-Ut[2]));

    /* 'LIB_limit:27' if (in > max) */
    if ((den + 0.5236) > 1.0) {
      /* 'LIB_limit:28' out = max; */
      psiUw = 1.0;
    } else if ((den + 0.5236) < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      psiUw = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      psiUw = den + 0.5236;
    }

    /* 'DefineFlightPath:411' Rarc0 = LIB_limit(0.8*Rarc0Lim,Rarc0Lim,Rarc0*LIB_limit(0.98,1.00,1.0-0.100*fac90)); */
    den = 1.0 - (0.1 * psiUw);

    /* 'LIB_limit:27' if (in > max) */
    if (den > 1.0) {
      /* 'LIB_limit:28' out = max; */
      den = 1.0;
    } else if (den < 0.98) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = 0.98;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    Earc = 0.8 * Rarc0Lim;
    Rarc0 *= den;

    /* 'LIB_limit:27' if (in > max) */
    if (Rarc0 > Rarc0Lim) {
      /* 'LIB_limit:28' out = max; */
      Rarc0 = Rarc0Lim;
    } else if (Rarc0 < Earc) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Rarc0 = Earc;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'DefineFlightPath:417' psi = 2.0*(theW+theT); */
    /* 'DefineFlightPath:418' Larc = RarcW*psi; */
    /* 'DefineFlightPath:419' Lfpath2bunt = Km+RtermW+Larc; */
    La1_tmp += ((La1 + theT) * 2.0) * (*V_RarcW);
    *Lfpath2bunt = La1_tmp;

    /* 'DefineFlightPath:420' Lfpath2tgt = Km+RtermW+Larc+RtermT; */
    *Lfpath2tgt = La1_tmp + RtermT;

    /* 'DefineFlightPath:424' if (abs(Mglide0) > 0.000001 && RarcW > 0.001) */
    if (fabs(Mglide0) > 1.0E-6) {
      if ((*V_RarcW) > 0.001) {
        /* 'DefineFlightPath:426' R2buntAVL = -Rte(3)/Mglide0 - 250.0e-06*(Lfpath2bunt^2)*abs(FPazi-FP0) - LIB_limit(0.0,5000.0,20.0e06/RarcW) - 30.0e03*LIB_limit(0.0,0.0873,Mglide0-Mfpath); */
        La1 = 2.0E+7 / (*V_RarcW);

        /* 'LIB_limit:27' if (in > max) */
        if (La1 > 5000.0) {
          /* 'LIB_limit:28' out = max; */
          La1 = 5000.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        theT = Mglide0 - Mfpath;

        /* 'LIB_limit:27' if (in > max) */
        if (theT > 0.0873) {
          /* 'LIB_limit:28' out = max; */
          theT = 0.0873;
        } else if (theT < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          theT = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        theT = ((((-ImpAng) / Mglide0) - (((La1_tmp * La1_tmp) * 0.00025) * fabs
                  ((*FPazi) - FP0))) - La1) - (30000.0 * theT);
      } else {
        /* 'DefineFlightPath:427' else */
        /* 'DefineFlightPath:428' R2buntAVL = 1000000.0; */
        theT = 1.0E+6;
      }
    } else {
      /* 'DefineFlightPath:427' else */
      /* 'DefineFlightPath:428' R2buntAVL = 1000000.0; */
      theT = 1.0E+6;
    }

    /* 'DefineFlightPath:430' R2buntREQ = Lfpath2bunt; */
    /* 'DefineFlightPath:431' delRE = R2buntAVL - R2buntREQ; */
    La1 = theT - La1_tmp;

    /* 'DefineFlightPath:433' if (do_plot) */
    /* 'DefineFlightPath:473' if (log_FP) */
    if (((int32_T)log_FP) != 0) {
      /* 'DefineFlightPath:474' iW = iW + 1; */
      tmp_a = ((uint32_T)(*iW)) + 1U;
      if (tmp_a > 65535U) {
        tmp_a = 65535U;
      }

      *iW = (uint16_T)tmp_a;

      /* 'DefineFlightPath:475' if (iW > 1000) */
      if (((int32_T)(*iW)) > 1000) {
        /* 'DefineFlightPath:476' iW = uint16(1000); */
        *iW = 1000U;
      }

      /* 'DefineFlightPath:478' telstruct.GUI_iW(iW)         = uint16(LIB_limit(0,65535,double(iW))); */
      /* 'LIB_limit:27' if (in > max) */
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      i_0 = ((int32_T)(*iW)) - 1;
      telstruct->GUI_iW[i_0] = *iW;

      /* 'DefineFlightPath:479' telstruct.GUI_solution(iW)   = int8(LIB_limit(-128,127,double(solution))); */
      /* 'LIB_limit:27' if (in > max) */
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      telstruct->GUI_solution[i_0] = *solution;

      /* 'DefineFlightPath:480' telstruct.GUI_inccnt(iW)     = uint8(i); */
      telstruct->GUI_inccnt[i_0] = (uint8_T)i;

      /* 'DefineFlightPath:481' telstruct.GUI_ftime(iW)      = single(ftime); */
      telstruct->GUI_ftime[i_0] = (real32_T)ftime;

      /* 'DefineFlightPath:482' telstruct.GUI_Rarc(iW)       = uint16(LIB_limit(0,65535,Rarc0)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rarc0 > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (Rarc0 < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rarc0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_Rarc[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_Rarc[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:483' telstruct.GUI_RarcW(iW)      = uint16(LIB_limit(0,65535,RarcW)); */
      /* 'LIB_limit:27' if (in > max) */
      if ((*V_RarcW) > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if ((*V_RarcW) < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = *V_RarcW;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_RarcW[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_RarcW[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:484' telstruct.GUI_Rterm(iW)      = uint16(LIB_limit(0,65535,Rterm)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rterm > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (Rterm < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rterm;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_Rterm[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_Rterm[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:485' telstruct.GUI_Km(iW)         = int16(LIB_limit(-32768,32767,Km)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Km > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Km;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Km[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Km[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:486' telstruct.GUI_RtermT(iW)     = uint16(LIB_limit(0,65535,RtermT)); */
      /* 'LIB_limit:27' if (in > max) */
      if (RtermT > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (RtermT < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = RtermT;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_RtermT[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_RtermT[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:487' telstruct.GUI_RtermW(iW)     = uint16(LIB_limit(0,65535,RtermW)); */
      /* 'LIB_limit:27' if (in > max) */
      if (RtermW > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (RtermW < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = RtermW;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_RtermW[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_RtermW[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:488' telstruct.GUI_LT(iW)         = int16(LIB_limit(-32768,32767,Lt)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Lt > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Lt < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Lt;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_LT[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_LT[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:489' telstruct.GUI_LW(iW)         = int16(LIB_limit(-32768,32767,Lw)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Lw > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Lw < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Lw;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_LW[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_LW[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:490' telstruct.GUI_Mglide(iW)     = single(-Mglide); */
      telstruct->GUI_Mglide[i_0] = (real32_T)((real_T)((real_T)(-(*Mglide))));

      /* 'DefineFlightPath:491' telstruct.GUI_FPazi(iW)      = single(FPazi*57.296); */
      telstruct->GUI_FPazi[i_0] = (real32_T)((real_T)((real_T)((*FPazi) * 57.296)));

      /* 'DefineFlightPath:492' telstruct.GUI_ratio(iW)      = single(ratio_gcapability); */
      telstruct->GUI_ratio[i_0] = (real32_T)(*ratio_gcapability);

      /* 'DefineFlightPath:493' telstruct.GUI_ImpAng(iW)     = int16(LIB_limit(-32768,32767,ImpAng*57.296)); */
      Earc = atan((-Ut[2]) / sqrt((Ut[0] * Ut[0]) + (Ut[1] * Ut[1]))) * 57.296;

      /* 'LIB_limit:27' if (in > max) */
      if (Earc > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Earc < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_ImpAng[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_ImpAng[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:494' telstruct.GUI_AttHeading(iW) = int16(LIB_limit(-32768,32767,AttHeading*57.296)); */
      Earc = AttHeading * 57.296;

      /* 'LIB_limit:27' if (in > max) */
      if (Earc > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Earc < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_AttHeading[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_AttHeading[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:496' telstruct.GUI_intsctTx(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (den_tmp > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (den_tmp < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = den_tmp;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctTx[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctTx[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:497' telstruct.GUI_intsctTy(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (den_tmp_0 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (den_tmp_0 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = den_tmp_0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctTy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctTy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:498' telstruct.GUI_intsctTz(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (den_tmp_1 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (den_tmp_1 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = den_tmp_1;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctTz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctTz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:499' telstruct.GUI_intsctWx(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto_tmp_0[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto_tmp_0[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto_tmp_0[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctWx[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctWx[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:500' telstruct.GUI_intsctWy(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto_tmp_0[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto_tmp_0[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto_tmp_0[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctWy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctWy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:501' telstruct.GUI_intsctWz(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto_tmp_0[2] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto_tmp_0[2] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto_tmp_0[2];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_intsctWz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_intsctWz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:502' telstruct.GUI_Rtox(iW)       = int16(LIB_limit(-32768,32767,Rto(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtox[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtox[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:503' telstruct.GUI_Rtoy(iW)       = int16(LIB_limit(-32768,32767,Rto(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtoy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtoy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:504' telstruct.GUI_Rtoz(iW)       = int16(LIB_limit(-32768,32767,Rto(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rto[2] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rto[2] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rto[2];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtoz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtoz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:505' telstruct.GUI_Rtex(iW)       = int16(LIB_limit(-32768,32767,Rte(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Larc > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Larc < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Larc;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtex[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtex[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:506' telstruct.GUI_Rtey(iW)       = int16(LIB_limit(-32768,32767,Rte(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rm_idx_0_0 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rm_idx_0_0 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rm_idx_0_0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtey[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtey[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:507' telstruct.GUI_Rtez(iW)       = int16(LIB_limit(-32768,32767,Rte(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (ImpAng > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (ImpAng < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = ImpAng;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rtez[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rtez[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:508' telstruct.GUI_Rwox(iW)       = int16(LIB_limit(-32768,32767,Rwo(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rwo[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rwo[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rwo[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwox[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwox[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:509' telstruct.GUI_Rwoy(iW)       = int16(LIB_limit(-32768,32767,Rwo(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rwo[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rwo[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rwo[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwoy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwoy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:510' telstruct.GUI_Rwoz(iW)       = int16(LIB_limit(-32768,32767,Rwo(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (psiUt0 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (psiUt0 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = psiUt0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwoz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwoz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:511' telstruct.GUI_Rwex(iW)       = int16(LIB_limit(-32768,32767,Rwe(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (vecT[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (vecT[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = vecT[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwex[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwex[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:512' telstruct.GUI_Rwey(iW)       = int16(LIB_limit(-32768,32767,Rwe(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (vecT[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (vecT[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = vecT[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwey[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwey[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:513' telstruct.GUI_Rwez(iW)       = int16(LIB_limit(-32768,32767,Rwe(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (vecT[2] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (vecT[2] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = vecT[2];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rwez[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rwez[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:514' telstruct.GUI_Rmx(iW)        = int16(LIB_limit(-32768,32767,Rm(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rm_idx_0 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rm_idx_0 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rm_idx_0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rmx[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rmx[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:515' telstruct.GUI_Rmy(iW)        = int16(LIB_limit(-32768,32767,Rm(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rm_idx_1 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rm_idx_1 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rm_idx_1;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rmy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rmy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:516' telstruct.GUI_Rmz(iW)        = int16(LIB_limit(-32768,32767,Rm(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rm_idx_2 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Rm_idx_2 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Rm_idx_2;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_Rmz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_Rmz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:517' telstruct.GUI_a0Wx(iW)       = int16(LIB_limit(-32768,32767,a0W(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Um[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Um[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Um[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Wx[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Wx[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:518' telstruct.GUI_a0Wy(iW)       = int16(LIB_limit(-32768,32767,a0W(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Um[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Um[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Um[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Wy[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Wy[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:519' telstruct.GUI_a0Wz(iW)       = int16(LIB_limit(-32768,32767,a0W(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (Um[2] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (Um[2] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = Um[2];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Wz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Wz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:520' telstruct.GUI_a0Tx(iW)       = int16(LIB_limit(-32768,32767,a0T(1))); */
      /* 'LIB_limit:27' if (in > max) */
      if (U_a1[0] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (U_a1[0] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = U_a1[0];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Tx[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Tx[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:521' telstruct.GUI_a0Ty(iW)       = int16(LIB_limit(-32768,32767,a0T(2))); */
      /* 'LIB_limit:27' if (in > max) */
      if (U_a1[1] > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (U_a1[1] < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = U_a1[1];
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Ty[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Ty[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:522' telstruct.GUI_a0Tz(iW)       = int16(LIB_limit(-32768,32767,a0T(3))); */
      /* 'LIB_limit:27' if (in > max) */
      if (U_a1_0 > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (U_a1_0 < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = U_a1_0;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_a0Tz[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_a0Tz[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:524' telstruct.GUI_spare(iW)      = uint16(0); */
      telstruct->GUI_spare[i_0] = 0U;

      /* 'DefineFlightPath:525' telstruct.GUI_tgt0x(iW)      = int16(LIB_limit(-32768,32767,tgt0.x)); */
      /* 'LIB_limit:27' if (in > max) */
      if (tgt0_x > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (tgt0_x < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = tgt0_x;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_tgt0x[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_tgt0x[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:526' telstruct.GUI_tgt0y(iW)      = int16(LIB_limit(-32768,32767,tgt0.y)); */
      /* 'LIB_limit:27' if (in > max) */
      if (tgt0_y > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (tgt0_y < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = tgt0_y;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_tgt0y[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_tgt0y[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:527' telstruct.GUI_tgt0z(iW)      = int16(LIB_limit(-32768,32767,tgt0.z)); */
      /* 'LIB_limit:27' if (in > max) */
      if (tgt0_z > 32767.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 32767.0;
      } else if (tgt0_z < -32768.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = -32768.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = tgt0_z;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 32768.0) {
        telstruct->GUI_tgt0z[i_0] = (int16_T)Earc;
      } else {
        telstruct->GUI_tgt0z[i_0] = MAX_int16_T;
      }

      /* 'DefineFlightPath:528' telstruct.GUI_altcnt(iW)     = uint16(0); */
      telstruct->GUI_altcnt[i_0] = 0U;

      /* 'DefineFlightPath:529' telstruct.GUI_R2buntREQ(iW)  = uint16(LIB_limit(0,65535,R2buntREQ)); */
      /* 'LIB_limit:27' if (in > max) */
      if (La1_tmp > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (La1_tmp < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = La1_tmp;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_R2buntREQ[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_R2buntREQ[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:530' telstruct.GUI_R2buntAVL(iW)  = uint16(LIB_limit(0,65535,R2buntAVL)); */
      /* 'LIB_limit:27' if (in > max) */
      if (theT > 65535.0) {
        /* 'LIB_limit:28' out = max; */
        Earc = 65535.0;
      } else if (theT < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Earc = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Earc = theT;
      }

      Earc = rt_roundd_snf(Earc);
      if (Earc < 65536.0) {
        telstruct->GUI_R2buntAVL[i_0] = (uint16_T)Earc;
      } else {
        telstruct->GUI_R2buntAVL[i_0] = MAX_uint16_T;
      }

      /* 'DefineFlightPath:531' telstruct.GUI_RarcW_cmp(iW)  = uint16(0); */
      telstruct->GUI_RarcW_cmp[i_0] = 0U;

      /* 'DefineFlightPath:532' telstruct.GUI_theFP3D(iW)    = int16(0); */
      telstruct->GUI_theFP3D[i_0] = 0;

      /* 'DefineFlightPath:533' telstruct.GUI_theFP2D(iW)    = int16(0); */
      telstruct->GUI_theFP2D[i_0] = 0;

      /* 'DefineFlightPath:534' telstruct.GUI_theErr(iW)     = int16(0); */
      telstruct->GUI_theErr[i_0] = 0;
    }

    /* 'DefineFlightPath:538' Earc = Rarc0 - RarcW; */
    Earc = Rarc0 - (*V_RarcW);

    /* 'DefineFlightPath:539' RarcW_up = 1.20*Rarc0; */
    psiUw = 1.2 * Rarc0;

    /* 'DefineFlightPath:542' [vec,Rarc,Oarc] = LIB_getreflvec([tgt0.x tgt0.y tgt0.z],RtermW*Uv0,[0 0 0]); */
    vecV[0] = tgt0_x;
    vecV[1] = tgt0_y;
    vecV[2] = tgt0_z;
    Uw_0[0] = RtermW * Uv0[0];
    Uw_0[1] = RtermW * Uv0[1];
    Uw_0[2] = RtermW * Uv0[2];
    if (!tmp) {
      tmp_0[0] = 0.0;
      tmp_0[1] = 0.0;
      tmp_0[2] = 0.0;
      tmp = true;
    }

    LIB_getreflvec_9sSePAMS(vecV, Uw_0, tmp_0, Ut_V, &AttHeading, Uw_plane);

    /* 'DefineFlightPath:545' Ut_cmp = LIB_unitvector((vec-[tgt0.x tgt0.y tgt0.z])'); */
    /* 'DefineFlightPath:548' Ua1 = LIB_unitvector(Rte-Rwe); */
    /* 'DefineFlightPath:561' if (ftime > 11.3) */
    /* 'DefineFlightPath:564' tmp = LIB_limit(-1.0,1.0,dot(Ut0,Ut)); */
    den = dot_lFwBkxue(Ut0, Ut);

    /* 'LIB_limit:27' if (in > max) */
    if (den > 1.0) {
      /* 'LIB_limit:28' out = max; */
      den = 1.0;
    } else if (den < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      den = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'DefineFlightPath:565' if (~opportunity_mode && ... */
    /* 'DefineFlightPath:566'       (Lt > RtermT && RtermT > Rlim_lo && Kglide > 0.1 && dot(Uw,Ua1) > 0.707 && acos(tmp) < 0.1745 && ... */
    /* 'DefineFlightPath:567'       count == 0 && RarcW ~= RarcW_prev && i < no_iter && latch_R4 == 0)) */
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if ((((((int32_T)opportunity_mode) == 0) && (Lt > RtermT)) && (RtermT >
          Rlim_lo)) && (Kglide > 0.1)) {
      b_I_0[0] = Larc - vecT[0];
      b_I_0[1] = Rm_idx_0_0 - vecT[1];
      b_I_0[2] = ImpAng - vecT[2];
      LIB_unitvector_gdXEDyOJ(b_I_0, Uw_plane);
      if (((dot_lFwBkxue(Uw, Uw_plane) > 0.707) && (acos(den) < 0.1745)) &&
          ((((count == 0) && ((*V_RarcW) != RarcW_prev)) && (i < 30)) &&
           (latch_R4 == 0))) {
        /* 'DefineFlightPath:579' if (RarcW > 1.5*RarcW_up && RtermW <= Rterm-0.001 && RtermT <= Rterm-0.001) */
        if ((((*V_RarcW) > (1.5 * psiUw)) && (RtermW <= (Rterm - 0.001))) &&
            (RtermT <= (Rterm - 0.001))) {
          /* 'DefineFlightPath:580' if (RtermW > RtermT && RtermW + Rlim_lo > Rterm) */
          if ((RtermW > RtermT) && ((RtermW + Rlim_lo) > Rterm)) {
            /* 'DefineFlightPath:581' dRterm = Rterm - RtermW; */
            Lw = Rterm - RtermW;
          } else if ((RtermT > RtermW) && ((RtermT + Rlim_lo) > Rterm)) {
            /* 'DefineFlightPath:582' elseif (RtermT > RtermW && RtermT + Rlim_lo > Rterm) */
            /* 'DefineFlightPath:583' dRterm = Rterm - RtermT; */
            Lw = Rterm - RtermT;
          } else {
            /* 'DefineFlightPath:584' else */
            /* 'DefineFlightPath:585' dRterm = Rlim_lo; */
            Lw = Rlim_lo;
          }

          /* 'DefineFlightPath:587' RtermW = LIB_limit(Rlim_lo,Rterm,RtermW + dRterm); */
          RtermW += Lw;

          /* 'LIB_limit:27' if (in > max) */
          if (RtermW > Rterm) {
            /* 'LIB_limit:28' out = max; */
            RtermW = Rterm;
          } else if (RtermW < Rlim_lo) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            RtermW = Rlim_lo;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:588' RtermT = LIB_limit(Rlim_lo,Rterm,RtermT + dRterm); */
          RtermT += Lw;

          /* 'LIB_limit:27' if (in > max) */
          if (RtermT > Rterm) {
            /* 'LIB_limit:28' out = max; */
            RtermT = Rterm;
          } else if (RtermT < Rlim_lo) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            RtermT = Rlim_lo;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          guard3 = true;
        } else if (((((*V_RarcW) > (0.95 * Rarc0)) && (((*V_RarcW) < psiUw) ||
                      ((La1 < (delRE_prev - 1.0)) && (La1 > 0.0)))) &&
                    (((int32_T)Weapon_Type) != 2)) || ((((*V_RarcW) > (0.8 *
            Rarc0)) && ((*V_RarcW) < (1.2 * Rarc0))) && (((int32_T)Weapon_Type) ==
          2))) {
          /* 'DefineFlightPath:589' elseif ((RarcW > 0.95*Rarc0 && (RarcW < RarcW_up || ((delRE < delRE_prev-1.0 || i==no_iter) && delRE > 0.0)) && Weapon_Type ~= 2) || ... */
          /* 'DefineFlightPath:590'         (RarcW > 0.80*Rarc0 && RarcW < 1.20*Rarc0 && Weapon_Type == 2)) */
          /* 'DefineFlightPath:593' solution = int8(1); */
          *solution = 1;

          /* 'DefineFlightPath:594' if (log_FP) */
          if (((int32_T)log_FP) != 0) {
            /* 'DefineFlightPath:595' telstruct.GUI_solution(iW) = int8(solution); */
            telstruct->GUI_solution[((int32_T)(*iW)) - 1] = 1;
          }

          /* 'DefineFlightPath:597' Ulos = LIB_unitvector([tgt0.x tgt0.y tgt0.z]); */
          /* 'DefineFlightPath:598' Uplane = LIB_unitvector(cross(Ulos,Uv0)); */
          vecV[0] = tgt0_x;
          vecV[1] = tgt0_y;
          vecV[2] = tgt0_z;
          LIB_unitvector_gdXEDyOJ(vecV, b_I_0);
          cross_6Ajhxtbg(b_I_0, Uv0, Uw_plane);
          LIB_unitvector_gdXEDyOJ(Uw_plane, Rto_tmp_0);

          /* 'DefineFlightPath:599' Ut0_plane = LIB_unitvector(Ut0 - dot(Ut0,Uplane')*Uplane'); */
          Lw = dot_lFwBkxue(Ut0, Rto_tmp_0);
          Uw_plane[0] = Ut0[0] - (Lw * Rto_tmp_0[0]);
          Uw_plane[1] = Ut0[1] - (Lw * Rto_tmp_0[1]);
          Uw_plane[2] = Ut0[2] - (Lw * Rto_tmp_0[2]);
          LIB_unitvector_gdXEDyOJ(Uw_plane, vecV);

          /* 'DefineFlightPath:600' Ut_plane = LIB_unitvector(Ut - dot(Ut,Uplane')*Uplane'); */
          Lw = dot_lFwBkxue(Ut, Rto_tmp_0);
          Uw_0[0] = Ut[0] - (Lw * Rto_tmp_0[0]);
          Uw_0[1] = Ut[1] - (Lw * Rto_tmp_0[1]);
          Uw_0[2] = Ut[2] - (Lw * Rto_tmp_0[2]);
          LIB_unitvector_gdXEDyOJ(Uw_0, Uw_plane);

          /* 'DefineFlightPath:604' tmp = LIB_limit(-1.0,1.0,dot(Ut0_plane,Ut_plane)); */
          den = dot_lFwBkxue(vecV, Uw_plane);

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1.0;
          } else if (den < -1.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = -1.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:605' mag = acos(tmp)*57.296; */
          /* 'DefineFlightPath:606' nrm = cross(Ut0_plane,Ut_plane); */
          /* 'DefineFlightPath:607' theAttack_inplane = -mag*sign(dot(Uplane,nrm)); */
          /* 'DefineFlightPath:608' if (theAttack_inplane > 5.0) */
          cross_6Ajhxtbg(vecV, Uw_plane, tmp_d);
          Larc = dot_lFwBkxue(Rto_tmp_0, tmp_d);
          if (Larc < 0.0) {
            Larc = -1.0;
          } else if (Larc > 0.0) {
            Larc = 1.0;
          } else if (Larc == 0.0) {
            Larc = 0.0;
          } else {
            Larc = (rtNaN);
          }

          if (((-(acos(den) * 57.296)) * Larc) > 5.0) {
            /* 'DefineFlightPath:609' latch_R4 = 1; */
            latch_R4 = 1;
          }

          /* 'DefineFlightPath:611' if (latch_R4 == 0) */
          if (latch_R4 == 0) {
            /* 'DefineFlightPath:612' if (do_plot) */
            exitg2 = true;
          } else {
            guard3 = true;
          }
        } else {
          guard3 = true;
        }
      } else {
        guard2 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      /* 'DefineFlightPath:628' tmp = LIB_limit(-1.0,1.0,dot(Uw,Ut)); */
      den = dot_lFwBkxue(Uw, Ut);

      /* 'LIB_limit:27' if (in > max) */
      if (den > 1.0) {
        /* 'LIB_limit:28' out = max; */
        den = 1.0;
      } else if (den < -1.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        den = -1.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'DefineFlightPath:629' Ftmp = LIB_limit(0.0873,3.0543,(pi-acos(tmp)))/20000; */
      Lw = acos(den);

      /* 'LIB_limit:27' if (in > max) */
      if ((3.1415926535897931 - Lw) > 3.0543) {
        /* 'LIB_limit:28' out = max; */
        Lw = 3.0543;
      } else if ((3.1415926535897931 - Lw) < 0.0873) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Lw = 0.0873;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Lw = 3.1415926535897931 - Lw;
      }

      /* 'DefineFlightPath:630' den = 10000.0*dot(Uw,Uv0); */
      den = 10000.0 * dot_lFwBkxue(Uw, Uv0);

      /* 'DefineFlightPath:631' if (den > 0.001) */
      if (den > 0.001) {
        /* 'DefineFlightPath:632' Kele = LIB_limit(0.1,1.0,norm(Rwe)/den-0.5); */
        Lt = (norm_MfVYifVG(vecT) / den) - 0.5;

        /* 'LIB_limit:27' if (in > max) */
        if (Lt > 1.0) {
          /* 'LIB_limit:28' out = max; */
          Lt = 1.0;
        } else if (Lt < 0.1) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Lt = 0.1;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }
      } else {
        /* 'DefineFlightPath:633' else */
        /* 'DefineFlightPath:634' Kele = 1.0; */
        Lt = 1.0;
      }

      /* 'DefineFlightPath:638' if (Ftmp < 1.0e-06 && i == no_iter) */
      /* 'DefineFlightPath:654' Kglide0 = 1.0; */
      /* 'DefineFlightPath:655' if (R2buntAVL > R2buntREQ && mRwe < Kglide0*Mglide0 || RarcW > RarcW_up) */
      if (((theT > La1_tmp) && (mRwe < Mglide0)) || ((*V_RarcW) > psiUw)) {
        /* 'DefineFlightPath:657' FPazi = LIB_limit(FP0-FPlim,FP0+FPlim,FPazi + pol*Ftmp*Kele*LIB_limit(-1000.0,1000.0,Earc)); */
        /* 'LIB_limit:27' if (in > max) */
        if (Earc > 1000.0) {
          /* 'LIB_limit:28' out = max; */
          theT = 1000.0;
        } else if (Earc < -1000.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          theT = -1000.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          theT = Earc;
        }

        Larc = FP0 - FPlim;
        Rm_idx_0_0 = FP0 + FPlim;
        Lw = ((((Lw / 20000.0) * ((real_T)pol)) * Lt) * theT) + (*FPazi);

        /* 'LIB_limit:27' if (in > max) */
        if (Lw > Rm_idx_0_0) {
          /* 'LIB_limit:28' out = max; */
          *FPazi = Rm_idx_0_0;
        } else if (Lw < Larc) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          *FPazi = Larc;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          *FPazi = Lw;
        }
      }

      /* 'DefineFlightPath:659' if (mRwe >= Kglide0*Mglide0) */
      if (mRwe >= Mglide0) {
        /* 'DefineFlightPath:660' FPazi = LIB_limit(FP0-FPlim,FP0+FPlim,FPazi + 0.1*(mRwe-Kglide0*Mglide0)*(LOSazi-FPazi)); */
        Lw = FP0 - FPlim;
        RarcW_prev = FP0 + FPlim;
        Km = (((mRwe - Mglide0) * 0.1) * (LOSazi - (*FPazi))) + (*FPazi);

        /* 'LIB_limit:27' if (in > max) */
        if (Km > RarcW_prev) {
          /* 'LIB_limit:28' out = max; */
          *FPazi = RarcW_prev;
        } else if (Km < Lw) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          *FPazi = Lw;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          *FPazi = Km;
        }

        /* 'DefineFlightPath:661' if (do_plot) */
      }

      /* 'DefineFlightPath:665' if (RtermW < Rlim_lo) */
      if (RtermW < Rlim_lo) {
        /* 'DefineFlightPath:667' EUt = 100.0e-06*LIB_limit(-1000.0,1000.0,Rarc0 - RarcW); */
        Lw = Earc;

        /* 'LIB_limit:27' if (in > max) */
        if (Earc > 1000.0) {
          /* 'LIB_limit:28' out = max; */
          Lw = 1000.0;
        } else if (Earc < -1000.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Lw = -1000.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        Lw *= 0.0001;

        /* 'DefineFlightPath:669' Ut = LIB_unitvector(EUt*(Ut0-Ut)+Ut); */
        Rto_tmp_0[0] = ((Ut0[0] - Ut[0]) * Lw) + Ut[0];
        Rto_tmp_0[1] = ((Ut0[1] - Ut[1]) * Lw) + Ut[1];
        Rto_tmp_0[2] = ((Ut0[2] - Ut[2]) * Lw) + Ut[2];
        LIB_unitvector_gdXEDyOJ(Rto_tmp_0, Ut);
      }

      /* 'DefineFlightPath:671' if (RarcW < RarcW_up) */
      if ((*V_RarcW) < psiUw) {
        /* 'DefineFlightPath:672' ratio_gcapability = LIB_limit(ratio_prev-0.10,ratio_prev,LIB_limit(0.0,1.0,ratio_gcapability-10.0*(0.00873-abs(FPazi-FP_prev)))); */
        *ratio_gcapability -= (0.00873 - fabs((*FPazi) - FP_prev)) * 10.0;

        /* 'LIB_limit:27' if (in > max) */
        if ((*ratio_gcapability) > 1.0) {
          /* 'LIB_limit:28' out = max; */
          *ratio_gcapability = 1.0;
        } else if ((*ratio_gcapability) < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          *ratio_gcapability = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        /* 'LIB_limit:27' if (in > max) */
        if ((*ratio_gcapability) > ratio_prev) {
          /* 'LIB_limit:28' out = max; */
          *ratio_gcapability = ratio_prev;
        } else if ((*ratio_gcapability) < (ratio_prev - 0.1)) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          *ratio_gcapability = ratio_prev - 0.1;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        /* 'DefineFlightPath:675' Ut = ratio_gcapability*Ut0 + (1.0-ratio_gcapability)*Ut_cmp; */
        /* 'DefineFlightPath:677' Ut = LIB_unitvector(Ut); */
        Rto_tmp_0[0] = Ut_V[0] - tgt0_x;
        Rto_tmp_0[1] = Ut_V[1] - tgt0_y;
        Rto_tmp_0[2] = Ut_V[2] - tgt0_z;
        LIB_unitvector_gdXEDyOJ(Rto_tmp_0, b_I_0);
        Ut_V[0] = ((1.0 - (*ratio_gcapability)) * b_I_0[0]) +
          ((*ratio_gcapability) * Ut0[0]);
        Ut_V[1] = ((1.0 - (*ratio_gcapability)) * b_I_0[1]) +
          ((*ratio_gcapability) * Ut0[1]);
        Ut_V[2] = ((1.0 - (*ratio_gcapability)) * b_I_0[2]) +
          ((*ratio_gcapability) * Ut0[2]);
        LIB_unitvector_gdXEDyOJ(Ut_V, Ut);
      }

      /* 'DefineFlightPath:679' ratio_prev = ratio_gcapability; */
      ratio_prev = *ratio_gcapability;

      /* 'DefineFlightPath:682' eg.phi=0; */
      /* 'DefineFlightPath:682' eg.the = atan(Mglide); */
      /* 'DefineFlightPath:682' eg.psi = FPazi; */
      /* 'DefineFlightPath:683' DCMfpath = LIB_e2d(eg); */
      /* 'DefineFlightPath:684' fpath_vec = DCMfpath*[RtermW 0 0]'; */
      /* 'DefineFlightPath:686' Uw = LIB_unitvector(fpath_vec); */
      LIB_e2d(0.0, atan(*Mglide), *FPazi, tmp_1);
      for (i_0 = 0; i_0 < 3; i_0++) {
        b_I_0[i_0] = (tmp_1[i_0 + 6] * 0.0) + ((tmp_1[i_0 + 3] * 0.0) +
          (tmp_1[i_0] * RtermW));
      }

      LIB_unitvector_gdXEDyOJ(b_I_0, Uw);

      /* 'DefineFlightPath:688' fac_Uwxy = sqrt(1.0-Uw(3)^2)/sqrt(Uw(1)^2+Uw(2)^2); */
      Lw = sqrt(1.0 - (Uw[2] * Uw[2])) / sqrt((Uw[0] * Uw[0]) + (Uw[1] * Uw[1]));

      /* 'DefineFlightPath:689' Uw(1) = Uw(1)*fac_Uwxy; */
      Uw[0] *= Lw;

      /* 'DefineFlightPath:690' Uw(2) = Uw(2)*fac_Uwxy; */
      Uw[1] *= Lw;

      /* 'DefineFlightPath:692' RarcW_prev = RarcW; */
      RarcW_prev = *V_RarcW;
      guard1 = true;
    }

    if (guard2) {
      /* 'DefineFlightPath:695' else */
      /* 'DefineFlightPath:697' if (do_plot) */
      /* 'DefineFlightPath:701' RtermT = Rterm0; */
      RtermT = 2000.0;

      /* 'DefineFlightPath:702' Ut = Ut0; */
      Ut[0] = Ut0[0];
      Ut[1] = Ut0[1];
      Ut[2] = Ut0[2];

      /* 'DefineFlightPath:703' ratio_gcapability = 1.0; */
      *ratio_gcapability = 1.0;

      /* 'DefineFlightPath:705' RarcW_prev = 0.0; */
      RarcW_prev = 0.0;

      /* 'DefineFlightPath:708' if (opportunity_mode) */
      if (((int32_T)opportunity_mode) != 0) {
        /* 'DefineFlightPath:709' Rto = [tgt0.x tgt0.y tgt0.z]; */
        /* 'DefineFlightPath:710' RtermT = Rterm0; */
        /* 'DefineFlightPath:711' RtermW = Rterm0; */
        RtermW = 2000.0;

        /* 'DefineFlightPath:712' [vecV,RarcV,Oarc] = LIB_getreflvec(Rto,RtermW*Uv0,[0 0 0]); */
        b_I_0[0] = 2000.0 * Uv0[0];
        b_I_0[1] = 2000.0 * Uv0[1];
        b_I_0[2] = 2000.0 * Uv0[2];
        if (!tmp_4) {
          tmp_5[0] = 0.0;
          tmp_5[1] = 0.0;
          tmp_5[2] = 0.0;
          tmp_4 = true;
        }

        LIB_getreflvec_9sSePAMS(Rto_tmp, b_I_0, tmp_5, vecV, &FP_prev, Uw_plane);

        /* 'DefineFlightPath:713' while(1) */
        /* 'DefineFlightPath:714' if (RarcV > 0.0 || RtermW < 150.0 || RtermT < 150.0) */
        while (!(((FP_prev > 0.0) || (RtermW < 150.0)) || (RtermT < 150.0))) {
          /* 'DefineFlightPath:716' else */
          /* 'DefineFlightPath:717' RtermW = RtermW - 100.0; */
          RtermW -= 100.0;

          /* 'DefineFlightPath:718' RtermT = RtermT - 100.0; */
          RtermT -= 100.0;

          /* 'DefineFlightPath:719' [vecV,RarcV,Oarc] = LIB_getreflvec(Rto,RtermW*Uv0,[0 0 0]); */
          Uw_0[0] = RtermW * Uv0[0];
          Uw_0[1] = RtermW * Uv0[1];
          Uw_0[2] = RtermW * Uv0[2];
          if (!tmp_6) {
            tmp_7[0] = 0.0;
            tmp_7[1] = 0.0;
            tmp_7[2] = 0.0;
            tmp_6 = true;
          }

          LIB_getreflvec_9sSePAMS(Rto_tmp, Uw_0, tmp_7, vecV, &FP_prev, Uw_plane);
        }

        /* 'DefineFlightPath:722' Ut_V = LIB_unitvector((vecV-[tgt0.x tgt0.y tgt0.z])'); */
        Ut_V[0] = vecV[0] - tgt0_x;
        Ut_V[1] = vecV[1] - tgt0_y;
        Ut_V[2] = vecV[2] - tgt0_z;
        LIB_unitvector_gdXEDyOJ(Ut_V, Ut);

        /* 'DefineFlightPath:723' if (-asin(Ut_V(3)/norm(Ut_V)) < 0.5236) */
        if ((-asin(Ut[2] / norm_MfVYifVG(Ut))) < 0.5236) {
          /* 'DefineFlightPath:724' Ut_tmp = [1 0 0]'; */
          /* 'DefineFlightPath:725' Utxy_mag = norm([Ut_V(1) Ut_V(2)]); */
          Uv0xy[0] = Ut[0];
          Uv0xy[1] = Ut[1];
          psiUw = norm_lpbhAiWU(Uv0xy);

          /* 'DefineFlightPath:726' Ut_tmp(1,1) = (Ut_V(1)/Utxy_mag)*0.866; */
          Ut_tmp[0] = (Ut[0] / psiUw) * 0.866;

          /* 'DefineFlightPath:727' Ut_tmp(2,1) = (Ut_V(2)/Utxy_mag)*0.866; */
          Ut_tmp[1] = (Ut[1] / psiUw) * 0.866;

          /* 'DefineFlightPath:728' Ut_tmp(3,1) = 0.5*sign(Ut_V(3)); */
          if (Ut[2] < 0.0) {
            FP_prev = -1.0;
          } else if (Ut[2] > 0.0) {
            FP_prev = 1.0;
          } else if (Ut[2] == 0.0) {
            FP_prev = 0.0;
          } else {
            FP_prev = (rtNaN);
          }

          Ut_tmp[2] = 0.5 * FP_prev;

          /* 'DefineFlightPath:729' Ut = LIB_unitvector(Ut_tmp); */
          LIB_unitvector_gdXEDyOJ(Ut_tmp, Ut);
        } else {
          /* 'DefineFlightPath:730' else */
          /* 'DefineFlightPath:731' Ut = Ut_V; */
        }
      }

      /* 'DefineFlightPath:735' while (1) */
      do {
        exitg1 = 0;

        /* 'DefineFlightPath:737' count = count + 1; */
        count++;

        /* 'DefineFlightPath:739' ImpAng = atan(-Ut(3)/sqrt(Ut(1)^2+Ut(2)^2)); */
        ImpAng = atan((-Ut[2]) / sqrt((Ut[0] * Ut[0]) + (Ut[1] * Ut[1])));

        /* 'DefineFlightPath:740' AttHeading = atan2(Ut(2),Ut(1)); */
        AttHeading = rt_atan2d_snf(Ut[1], Ut[0]);

        /* 'DefineFlightPath:741' if (AttHeading < 0.0) */
        if (AttHeading < 0.0) {
          /* 'DefineFlightPath:742' AttHeading = AttHeading + 2*pi; */
          AttHeading += 6.2831853071795862;
        }

        /* 'DefineFlightPath:748' psiUw  = atan2(Uw(2),Uw(1)); */
        Larc = rt_atan2d_snf(Uw[1], Uw[0]);

        /* 'DefineFlightPath:749' psiUt0 = atan2(-Ut0(2),-Ut0(1)); */
        Rm_idx_0_0 = rt_atan2d_snf(-Ut0[1], -Ut0[0]);

        /* 'DefineFlightPath:750' psifac = LIB_limit(0.0,1.0,tgtrgehor/7000.0); */
        den = tgtrgehor / 7000.0;

        /* 'LIB_limit:27' if (in > max) */
        if (den > 1.0) {
          /* 'LIB_limit:28' out = max; */
          den = 1.0;
        } else if (den < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          den = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        /* 'DefineFlightPath:751' X = (1.0-psifac)*cos(psiUw) + psifac*cos(psiUt0); */
        /* 'DefineFlightPath:752' Y = (1.0-psifac)*sin(psiUw) + psifac*sin(psiUt0); */
        /* 'DefineFlightPath:753' psiApproach = atan2(Y,X); */
        psiUw = rt_atan2d_snf(((1.0 - den) * sin(Larc)) + (den * sin(Rm_idx_0_0)),
                              ((1.0 - den) * cos(Larc)) + (den * cos(Rm_idx_0_0)));

        /* 'DefineFlightPath:754' UtN = [0 0 0]; */
        /* 'DefineFlightPath:755' UtN(1) = Ut(1)*cos(psiApproach) + Ut(2)*sin(psiApproach); */
        /* 'DefineFlightPath:756' UtN(2) =-Ut(1)*sin(psiApproach) + Ut(2)*cos(psiApproach); */
        /* 'DefineFlightPath:757' UtN(3) = Ut(3); */
        /* 'DefineFlightPath:758' fac90 = LIB_limit(0.0,1.0,atan(UtN(1)/-UtN(3))+0.5236); */
        psiUw = atan(((Ut[0] * cos(psiUw)) + (Ut[1] * sin(psiUw))) / (-Ut[2]));

        /* 'LIB_limit:27' if (in > max) */
        if ((psiUw + 0.5236) > 1.0) {
          /* 'LIB_limit:28' out = max; */
          psiUw = 1.0;
        } else if ((psiUw + 0.5236) < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          psiUw = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          psiUw += 0.5236;
        }

        /* 'DefineFlightPath:759' Rarc0_prev = Rarc0; */
        FP_prev = Rarc0;

        /* 'DefineFlightPath:760' Rarc0 = LIB_limit(0.6*Rarc0Lim,Rarc0Lim,Rarc0*LIB_limit(0.98,1.00,1.0-0.100*fac90)); */
        mRwe = 1.0 - (0.1 * psiUw);

        /* 'LIB_limit:27' if (in > max) */
        if (mRwe > 1.0) {
          /* 'LIB_limit:28' out = max; */
          mRwe = 1.0;
        } else if (mRwe < 0.98) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          mRwe = 0.98;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        delRE_prev = 0.6 * Rarc0Lim;
        Rarc0 *= mRwe;

        /* 'LIB_limit:27' if (in > max) */
        if (Rarc0 > Rarc0Lim) {
          /* 'LIB_limit:28' out = max; */
          Rarc0 = Rarc0Lim;
        } else if (Rarc0 < delRE_prev) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Rarc0 = delRE_prev;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        /* 'DefineFlightPath:764' del_Rarc0 = Rarc0 - Rarc0_prev; */
        psiUt0 = Rarc0 - FP_prev;

        /* 'DefineFlightPath:766' Rto = (RtermT-RtermW)*Ut' + [tgt0.x tgt0.y tgt0.z]; */
        FP_prev = RtermT - RtermW;

        /* 'DefineFlightPath:767' [vecV,RarcV,Oarc] = LIB_getreflvec(Rto,RtermW*Uv0,[0 0 0]); */
        Rto[0] = (FP_prev * Ut[0]) + tgt0_x;
        Uw_0[0] = RtermW * Uv0[0];
        Rto[1] = (FP_prev * Ut[1]) + tgt0_y;
        Uw_0[1] = RtermW * Uv0[1];
        Rto[2] = (FP_prev * Ut[2]) + tgt0_z;
        Uw_0[2] = RtermW * Uv0[2];
        if (!tmp_2) {
          tmp_3[0] = 0.0;
          tmp_3[1] = 0.0;
          tmp_3[2] = 0.0;
          tmp_2 = true;
        }

        LIB_getreflvec_9sSePAMS(Rto, Uw_0, tmp_3, vecV, &FP_prev, Uw_plane);

        /* 'DefineFlightPath:768' while(1) */
        /* 'DefineFlightPath:769' if (RarcV > 0.0 || RtermW < 150.0 || RtermT < 150.0) */
        while (!(((FP_prev > 0.0) || (RtermW < 150.0)) || (RtermT < 150.0))) {
          /* 'DefineFlightPath:774' else */
          /* 'DefineFlightPath:775' RtermW = RtermW - 100.0; */
          RtermW -= 100.0;

          /* 'DefineFlightPath:776' RtermT = RtermT - 100.0; */
          RtermT -= 100.0;

          /* 'DefineFlightPath:777' [vecV,RarcV,Oarc] = LIB_getreflvec(Rto,RtermW*Uv0,[0 0 0]); */
          Uw_0[0] = RtermW * Uv0[0];
          Uw_0[1] = RtermW * Uv0[1];
          Uw_0[2] = RtermW * Uv0[2];
          if (!tmp_8) {
            tmp_9[0] = 0.0;
            tmp_9[1] = 0.0;
            tmp_9[2] = 0.0;
            tmp_8 = true;
          }

          LIB_getreflvec_9sSePAMS(Rto, Uw_0, tmp_9, vecV, &FP_prev, Uw_plane);
        }

        /* 'DefineFlightPath:770' if (RarcV < 1.0) */
        if (FP_prev < 1.0) {
          /* 'DefineFlightPath:771' RarcV = 1.0; */
          FP_prev = 1.0;
        }

        /* 'DefineFlightPath:782' Ut_V = LIB_unitvector((vecV-[tgt0.x tgt0.y tgt0.z])'); */
        Ut_V[0] = vecV[0] - tgt0_x;
        Ut_V[1] = vecV[1] - tgt0_y;
        Ut_V[2] = vecV[2] - tgt0_z;
        LIB_unitvector_gdXEDyOJ(Ut_V, b_I_0);

        /* 'DefineFlightPath:791' if (abs(LOSrelV) > psi_Engage && latch(1) == 0) */
        Larc = fabs(LOSrelV);
        guard4 = false;
        if ((Larc > psi_Engage) && (((int32_T)latch[0]) == 0)) {
          /* 'DefineFlightPath:793' if (abs(LOSrelV) < psi_Engage) */
          if (Larc < psi_Engage) {
            /* 'DefineFlightPath:794' latch(1) = uint8(1); */
            latch[0] = 1U;
          }

          /* 'DefineFlightPath:800' RarcW_cmp = Rarc0; */
          FP_prev = Rarc0;

          /* 'DefineFlightPath:804' Utxy  = LIB_unitvector([Ut(1) Ut(2)]); */
          Uv0xy[0] = Ut[0];
          Uv0xy[1] = Ut[1];
          LIB_unitvector_jvWMvfXP(Uv0xy, Utxy);

          /* 'DefineFlightPath:805' Uv0xy = LIB_unitvector([Uv0(1) Uv0(2)]); */
          Uv0_0[0] = Uv0[0];
          Uv0_0[1] = Uv0[1];
          LIB_unitvector_jvWMvfXP(Uv0_0, Uv0xy);

          /* 'DefineFlightPath:806' c = cross([Utxy(1) Utxy(2) 0.0],[Uv0xy(1) Uv0xy(2) 0.0]); */
          b_I_0[0] = Utxy[0];
          b_I_0[1] = Utxy[1];
          b_I_0[2] = 0.0;
          Ut_V[0] = Uv0xy[0];
          Ut_V[1] = Uv0xy[1];
          Ut_V[2] = 0.0;
          cross_6Ajhxtbg(b_I_0, Ut_V, vecV);

          /* 'DefineFlightPath:808' ma = -1.0/LIB_limit(-1.0e09,1.0e09,(Utxy(2)+Uv0xy(2))/(Utxy(1)+Uv0xy(1))); */
          AttHeading = (Utxy[1] + Uv0xy[1]) / (Utxy[0] + Uv0xy[0]);

          /* 'LIB_limit:27' if (in > max) */
          if (AttHeading > 1.0E+9) {
            /* 'LIB_limit:28' out = max; */
            AttHeading = 1.0E+9;
          } else if (AttHeading < -1.0E+9) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            AttHeading = -1.0E+9;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:809' ca = 0.0; */
          /* 'DefineFlightPath:810' mt = LIB_limit(-1.0e09,1.0e09,Utxy(2)/Utxy(1)); */
          delRE_prev = Utxy[1] / Utxy[0];

          /* 'LIB_limit:27' if (in > max) */
          if (delRE_prev > 1.0E+9) {
            /* 'LIB_limit:28' out = max; */
            delRE_prev = 1.0E+9;
          } else if (delRE_prev < -1.0E+9) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            delRE_prev = -1.0E+9;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:811' ct = tgt0.y+(mt*-tgt0.x); */
          /* 'DefineFlightPath:812' mw = LIB_limit(-1.0e09,1.0e09,Uv0(2)/Uv0(1)); */
          mRwe = Uv0[1] / Uv0[0];

          /* 'LIB_limit:27' if (in > max) */
          if (mRwe > 1.0E+9) {
            /* 'LIB_limit:28' out = max; */
            mRwe = 1.0E+9;
          } else if (mRwe < -1.0E+9) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            mRwe = -1.0E+9;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:815' x = (ct-ca)/(ma-mt); */
          AttHeading = ((delRE_prev * (-tgt0_x)) + tgt0_y) / ((-1.0 / AttHeading)
            - delRE_prev);

          /* 'DefineFlightPath:816' Ln = (x-tgt0.x)/Ut(1); */
          psiUt0 = (AttHeading - tgt0_x) / Ut[0];

          /* 'DefineFlightPath:818' if ((x > Rto(1) && c(3) > 0.0) || (x < Rto(1) && c(3) < 0.0) || latch(2)) */
          if ((((AttHeading > Rto[0]) && (vecV[2] > 0.0)) || ((AttHeading < Rto
                 [0]) && (vecV[2] < 0.0))) || (((int32_T)latch[1]) != 0)) {
            /* 'DefineFlightPath:820' Rto = [tgt0.x tgt0.y tgt0.z]+ Ut'*Ln; */
            Rto[0] = (Ut[0] * psiUt0) + tgt0_x;
            Rto[1] = (Ut[1] * psiUt0) + tgt0_y;
            Rto[2] = (Ut[2] * psiUt0) + tgt0_z;

            /* 'DefineFlightPath:821' latch(2) = 1; */
            latch[1] = 1U;
          } else {
            /* 'DefineFlightPath:822' else */
            /* 'DefineFlightPath:827' Rwo = -Uw'*Ln; */
            Rwo[0] = (-Uw[0]) * psiUt0;
            Rwo[1] = (-Uw[1]) * psiUt0;
            Rwo[2] = (-Uw[2]) * psiUt0;
          }

          /* 'DefineFlightPath:831' Uw = LIB_unitvector([Uv0xy(1) Uv0xy(2) -Mglide]'); */
          Ut_V[0] = Uv0xy[0];
          Ut_V[1] = Uv0xy[1];
          Ut_V[2] = -(*Mglide);
          LIB_unitvector_gdXEDyOJ(Ut_V, Uw);

          /* 'DefineFlightPath:832' Rwe = Rwo + Uw'*RtermW; */
          /* 'DefineFlightPath:833' Rte = Rto + Ut'*RtermW; */
          /* 'DefineFlightPath:835' mto = -1.0/mt; */
          delRE_prev = -1.0 / delRE_prev;

          /* 'DefineFlightPath:836' cto = Rte(2) - mto*Rte(1); */
          /* 'DefineFlightPath:837' mwo = -1.0/mw; */
          AttHeading = -1.0 / mRwe;

          /* 'DefineFlightPath:838' cwo = Rwe(2) - mwo*Rwe(1); */
          vecT[0] = (Uw[0] * RtermW) + Rwo[0];
          Rte_0[0] = (Ut[0] * RtermW) + Rto[0];
          vecT[1] = (Uw[1] * RtermW) + Rwo[1];
          Rte_0[1] = (Ut[1] * RtermW) + Rto[1];
          vecT[2] = (Uw[2] * RtermW) + Rwo[2];
          Rte_0[2] = (Ut[2] * RtermW) + Rto[2];
          psiUt0 = vecT[1] - (AttHeading * vecT[0]);

          /* 'DefineFlightPath:839' xo = (cto-cwo)/(mwo-mto); */
          mRwe = ((Rte_0[1] - (delRE_prev * Rte_0[0])) - psiUt0) / (AttHeading -
            delRE_prev);

          /* 'DefineFlightPath:840' yo = mwo*xo + cwo; */
          delRE_prev = (AttHeading * mRwe) + psiUt0;

          /* 'DefineFlightPath:842' Rarc = norm([xo-Rwe(1) yo-Rwe(2)]); */
          Utxy[0] = mRwe - vecT[0];
          Utxy[1] = delRE_prev - vecT[1];
          *V_RarcW = norm_lpbhAiWU(Utxy);

          /* 'DefineFlightPath:844' Um = 0.5*(Uw'+Ut'); */
          /* 'DefineFlightPath:846' Umxy = LIB_unitvector([Um(1) Um(2)]); */
          Uv0_0[0] = (Uw[0] + Ut[0]) * 0.5;
          Uv0_0[1] = (Uw[1] + Ut[1]) * 0.5;
          LIB_unitvector_jvWMvfXP(Uv0_0, Utxy);

          /* 'DefineFlightPath:847' Rm(1) = xo + Umxy(1)*Rarc; */
          Rm_idx_0 = (Utxy[0] * (*V_RarcW)) + mRwe;

          /* 'DefineFlightPath:848' Rm(2) = yo + Umxy(2)*Rarc; */
          Rm_idx_1 = (Utxy[1] * (*V_RarcW)) + delRE_prev;

          /* 'DefineFlightPath:849' tmp = LIB_limit(-1.0,1.0,dot([Rwe(1) Rwe(2)],[Rm(1) Rm(2)])/(norm([Rwe(1) Rwe(2)])*norm([Rm(1) Rm(2)]))); */
          Utxy[0] = vecT[0];
          Utxy[1] = vecT[1];
          Uv0_0[0] = Rm_idx_0;
          Uv0_0[1] = Rm_idx_1;
          vecT_0[0] = vecT[0];
          vecT_0[1] = vecT[1];
          Rm[0] = Rm_idx_0;
          Rm[1] = Rm_idx_1;
          den = dot_tXnwwRPM(Utxy, Uv0_0) / (norm_lpbhAiWU(vecT_0) *
            norm_lpbhAiWU(Rm));

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1.0;
          } else if (den < -1.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = -1.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:850' the = acos(tmp); */
          /* 'DefineFlightPath:851' Rm(3) = Rwe(3) - Mglide*the*Rarc; */
          Rm_idx_2 = vecT[2] - (((*Mglide) * acos(den)) * (*V_RarcW));

          /* 'DefineFlightPath:853' VECw = Rwe - Rwo; */
          /* 'DefineFlightPath:854' VECt = Rte - Rto; */
          /* 'DefineFlightPath:855' Um(1) =  (VECw(2)+VECt(2)); */
          Um[0] = (vecT[1] - Rwo[1]) + (Rte_0[1] - Rto[1]);

          /* 'DefineFlightPath:856' Um(2) = -(VECw(1)+VECt(1)); */
          Um[1] = -((vecT[0] - Rwo[0]) + (Rte_0[0] - Rto[0]));

          /* 'DefineFlightPath:857' Um(3) = -Mglide*norm([Um(1) Um(2)]); */
          Uv0_0[0] = Um[0];
          Uv0_0[1] = Um[1];

          /* 'DefineFlightPath:859' Um = LIB_unitvector(Um); */
          b_I_0[0] = Um[0];
          b_I_0[1] = Um[1];
          b_I_0[2] = (-(*Mglide)) * norm_lpbhAiWU(Uv0_0);
          LIB_unitvector_gdXEDyOJ(b_I_0, Um);

          /* 'DefineFlightPath:860' a0T = Rm - Um*RtermW; */
          /* 'DefineFlightPath:861' a0W = Rm + Um*RtermW; */
          delRE_prev = Um[0] * RtermW;
          U_a1[0] = Rm_idx_0 - delRE_prev;
          Um[0] = Rm_idx_0 + delRE_prev;
          delRE_prev = Um[1] * RtermW;
          U_a1[1] = Rm_idx_1 - delRE_prev;
          Um[1] = Rm_idx_1 + delRE_prev;
          delRE_prev = Um[2] * RtermW;
          U_a1[2] = Rm_idx_2 - delRE_prev;
          Um[2] = Rm_idx_2 + delRE_prev;

          /* 'DefineFlightPath:863' RarcW = Rarc; */
          /* 'DefineFlightPath:864' RarcT = Rarc; */
          *V_RarcT = *V_RarcW;

          /* 'DefineFlightPath:865' FPazi = atan2(Uw(2),Uw(1)); */
          *FPazi = rt_atan2d_snf(Uw[1], Uw[0]);

          /* 'DefineFlightPath:867' Lfpath2bunt = tgt0rge; */
          *Lfpath2bunt = tgt0rge;

          /* 'DefineFlightPath:868' Lfpath2tgt = tgt0rge; */
          *Lfpath2tgt = tgt0rge;

          /* 'DefineFlightPath:872' if (do_plot) */
          /* 'DefineFlightPath:896' N = LIB_limit(0.8,2.0,1.5+3.5*Mach_diff); */
          mRwe = (3.5 * Mach_diff) + 1.5;

          /* 'LIB_limit:27' if (in > max) */
          if (mRwe > 2.0) {
            /* 'LIB_limit:28' out = max; */
            mRwe = 2.0;
          } else if (mRwe < 0.8) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            mRwe = 0.8;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:897' if (do_plot) */
          /* 'DefineFlightPath:919' if (RarcW > N*Rarc0 && R2buntAVL < R2buntREQ && count < 20) */
          if ((((*V_RarcW) > (mRwe * Rarc0)) && (theT < La1_tmp)) && (count < 20))
          {
            /* 'DefineFlightPath:921' a = Rto - Rwo; */
            /* 'DefineFlightPath:922' c = cross([Uv0xy(1) Uv0xy(2) 0.0],[a(1) a(2) 0.0]); */
            /* 'DefineFlightPath:923' dpsi = 0.0873*sign(c(3)); */
            Ut_V[0] = Uv0xy[0];
            Ut_V[1] = Uv0xy[1];
            Ut_V[2] = 0.0;
            vecV[0] = Rto[0] - Rwo[0];
            vecV[1] = Rto[1] - Rwo[1];
            vecV[2] = 0.0;
            cross_6Ajhxtbg(Ut_V, vecV, b_I_0);
            if (b_I_0[2] < 0.0) {
              Larc = -1.0;
            } else if (b_I_0[2] > 0.0) {
              Larc = 1.0;
            } else if (b_I_0[2] == 0.0) {
              Larc = 0.0;
            } else {
              Larc = (rtNaN);
            }

            RarcW_prev = 0.0873 * Larc;

            /* 'DefineFlightPath:924' Utx = Ut(1); */
            mRwe = Ut[0];

            /* 'DefineFlightPath:924' Uty = Ut(2); */
            /* 'DefineFlightPath:925' Ut(1) =  Utx*cos(dpsi) + Uty*sin(dpsi); */
            delRE_prev = sin(RarcW_prev);
            RarcW_prev = cos(RarcW_prev);
            Ut[0] = (Ut[0] * RarcW_prev) + (Ut[1] * delRE_prev);

            /* 'DefineFlightPath:926' Ut(2) = -Utx*sin(dpsi) + Uty*cos(dpsi); */
            Ut[1] = ((-mRwe) * delRE_prev) + (Ut[1] * RarcW_prev);

            /* 'DefineFlightPath:938' sol = 2; */
            guard4 = true;
          } else {
            /* 'DefineFlightPath:927' else */
            /* 'DefineFlightPath:928' Rrel = Rto - a0T; */
            /* 'DefineFlightPath:929' Vref = Rm - a0T; */
            /* 'DefineFlightPath:930' [Rte,RarcT,OarcT] = LIB_getreflvec(Rrel,Vref,a0T); */
            Rto_tmp_0[0] = Rto[0] - U_a1[0];
            b_I_0[0] = Rm_idx_0 - U_a1[0];
            Rto_tmp_0[1] = Rto[1] - U_a1[1];
            b_I_0[1] = Rm_idx_1 - U_a1[1];
            Rto_tmp_0[2] = Rto[2] - U_a1[2];
            b_I_0[2] = Rm_idx_2 - U_a1[2];
            LIB_getreflvec_9sSePAMS(Rto_tmp_0, b_I_0, U_a1, Rte_0, V_RarcT,
              OarcT);

            /* 'DefineFlightPath:931' solution = int8(2); */
            *solution = 2;

            /* 'DefineFlightPath:932' if (log_FP) */
            if (((int32_T)log_FP) != 0) {
              /* 'DefineFlightPath:933' telstruct.GUI_solution(iW) = int8(solution); */
              telstruct->GUI_solution[((int32_T)(*iW)) - 1] = 2;
            }

            /* 'DefineFlightPath:935' count = 20; */
            count = 20;
            exitg1 = 1;
          }
        } else {
          /* 'DefineFlightPath:942' else */
          /* 'DefineFlightPath:951' latch(1) = uint8(1); */
          latch[0] = 1U;

          /* 'DefineFlightPath:954' a0 = [tgt0.x tgt0.y tgt0.z]+(RtermT-RtermW)*Ut'; */
          mRwe = RtermT - RtermW;

          /* 'DefineFlightPath:955' [vecT,Rarc,Oarc] = LIB_getreflvec(-a0,RtermW*Ut',a0); */
          U_a1_0 = (mRwe * Ut[0]) + tgt0_x;
          Um[0] = -U_a1_0;
          Uw_0[0] = RtermW * Ut[0];
          U_a1[0] = U_a1_0;
          U_a1_0 = (mRwe * Ut[1]) + tgt0_y;
          Um[1] = -U_a1_0;
          Uw_0[1] = RtermW * Ut[1];
          U_a1[1] = U_a1_0;
          U_a1_0 = (mRwe * Ut[2]) + tgt0_z;
          Um[2] = -U_a1_0;
          Uw_0[2] = RtermW * Ut[2];
          U_a1[2] = U_a1_0;
          LIB_getreflvec_9sSePAMS(Um, Uw_0, U_a1, vecT, V_RarcW, Oarc);

          /* 'DefineFlightPath:956' while(1) */
          /* 'DefineFlightPath:957' if (Rarc > 0.0 || RtermW < 150.0 || RtermT < 150.0) */
          while (!((((*V_RarcW) > 0.0) || (RtermW < 150.0)) || (RtermT < 150.0)))
          {
            /* 'DefineFlightPath:962' else */
            /* 'DefineFlightPath:963' RtermW = RtermW - 100.0; */
            RtermW -= 100.0;

            /* 'DefineFlightPath:964' RtermT = RtermT - 100.0; */
            RtermT -= 100.0;

            /* 'DefineFlightPath:965' [vecT,Rarc,Oarc] = LIB_getreflvec(-a0,RtermW*Ut',a0); */
            Um[0] = -U_a1[0];
            Uw_0[0] = RtermW * Ut[0];
            Um[1] = -U_a1[1];
            Uw_0[1] = RtermW * Ut[1];
            Um[2] = -U_a1_0;
            Uw_0[2] = RtermW * Ut[2];
            LIB_getreflvec_9sSePAMS(Um, Uw_0, U_a1, vecT, V_RarcW, Oarc);
          }

          /* 'DefineFlightPath:958' if (Rarc < 1.0) */
          if ((*V_RarcW) < 1.0) {
            /* 'DefineFlightPath:959' Rarc = 1.0; */
            *V_RarcW = 1.0;
          }

          /* 'DefineFlightPath:980' Rte = RtermT*Ut' + [tgt0.x tgt0.y tgt0.z]; */
          /* 'DefineFlightPath:981' Rwo = [0 0 0]; */
          /* 'DefineFlightPath:982' Rwe = vecT; */
          /* 'DefineFlightPath:983' a1 = Rwe-Rte; */
          /* 'DefineFlightPath:984' Rom = 0.5*(Rte-Rwe)+Rwe - Oarc; */
          /* 'DefineFlightPath:986' Uom = LIB_unitvector(Rom); */
          /* 'DefineFlightPath:987' Rm  = Oarc + Uom*Rarc; */
          U_a1_0 = (RtermT * Ut[0]) + tgt0_x;
          Rwo[0] = 0.0;
          a1[0] = vecT[0] - U_a1_0;
          Uw_plane[0] = (((U_a1_0 - vecT[0]) * 0.5) + vecT[0]) - Oarc[0];
          Rte_0[0] = U_a1_0;
          U_a1_0 = (RtermT * Ut[1]) + tgt0_y;
          Rwo[1] = 0.0;
          a1[1] = vecT[1] - U_a1_0;
          Uw_plane[1] = (((U_a1_0 - vecT[1]) * 0.5) + vecT[1]) - Oarc[1];
          Rte_0[1] = U_a1_0;
          U_a1_0 = (RtermT * Ut[2]) + tgt0_z;
          Rwo[2] = 0.0;
          a1[2] = vecT[2] - U_a1_0;
          Uw_plane[2] = (((U_a1_0 - vecT[2]) * 0.5) + vecT[2]) - Oarc[2];
          Rte_0[2] = U_a1_0;
          LIB_unitvector_gdXEDyOJ(Uw_plane, vecV);
          Rm_idx_0 = (vecV[0] * (*V_RarcW)) + Oarc[0];
          Rm_idx_1 = (vecV[1] * (*V_RarcW)) + Oarc[1];
          Rm_idx_2 = (vecV[2] * (*V_RarcW)) + Oarc[2];

          /* 'DefineFlightPath:989' Uw = LIB_unitvector(vecT'); */
          LIB_unitvector_gdXEDyOJ(vecT, Uw);

          /* 'DefineFlightPath:992' FPazi = LIB_limit(FP0-FPlim,FP0+FPlim,atan2(Uw(2),Uw(1))); */
          mRwe = rt_atan2d_snf(Uw[1], Uw[0]);
          Larc = FP0 - FPlim;
          Rm_idx_0_0 = FP0 + FPlim;

          /* 'LIB_limit:27' if (in > max) */
          if (mRwe > Rm_idx_0_0) {
            /* 'LIB_limit:28' out = max; */
            *FPazi = Rm_idx_0_0;
          } else if (mRwe < Larc) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            *FPazi = Larc;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            *FPazi = mRwe;
          }

          /* 'DefineFlightPath:994' Um = 0.5*(Uw-Ut); */
          /* 'DefineFlightPath:996' Um = LIB_unitvector(Um); */
          Uw_plane[0] = (Uw[0] - Ut[0]) * 0.5;
          Uw_plane[1] = (Uw[1] - Ut[1]) * 0.5;
          Uw_plane[2] = (Uw[2] - Ut[2]) * 0.5;
          LIB_unitvector_gdXEDyOJ(Uw_plane, Um);

          /* 'DefineFlightPath:999' a0T = Rm - Um'*RtermW; */
          /* 'DefineFlightPath:1000' a0W = Rm + Um'*RtermW; */
          den = Um[0] * RtermW;
          U_a1[0] = Rm_idx_0 - den;
          Rm_idx_0_0 = den + Rm_idx_0;
          den = Um[1] * RtermW;
          U_a1[1] = Rm_idx_1 - den;
          Rm_idx_1_0 = den + Rm_idx_1;
          den = Um[2] * RtermW;
          U_a1[2] = Rm_idx_2 - den;
          Rm_idx_2_0 = den + Rm_idx_2;

          /* 'DefineFlightPath:1003' RarcW = Rarc; */
          /* 'DefineFlightPath:1004' RarcT = Rarc; */
          *V_RarcT = *V_RarcW;

          /* 'DefineFlightPath:1005' OarcW = Oarc; */
          /* 'DefineFlightPath:1006' OarcT = Oarc; */
          /* 'DefineFlightPath:1017' if (do_plot) */
          /* 'DefineFlightPath:1042' Ulos = LIB_unitvector([tgt0.x tgt0.y tgt0.z]); */
          /* 'DefineFlightPath:1044' Uplane = LIB_unitvector(cross(Ulos,Uv0)); */
          vecV[0] = tgt0_x;
          vecV[1] = tgt0_y;
          vecV[2] = tgt0_z;
          LIB_unitvector_gdXEDyOJ(vecV, Uw_plane);
          cross_6Ajhxtbg(Uw_plane, Uv0, vecV);
          LIB_unitvector_gdXEDyOJ(vecV, Ut_V);

          /* 'DefineFlightPath:1046' Uv0_plane = LIB_unitvector(Uv0' - dot(Uv0,Uplane')*Uplane'); */
          mRwe = dot_lFwBkxue(Uv0, Ut_V);
          Um[0] = Rm_idx_0_0;
          OarcT[0] = Oarc[0];
          Uw_plane[0] = Uv0[0] - (mRwe * Ut_V[0]);
          Um[1] = Rm_idx_1_0;
          OarcT[1] = Oarc[1];
          Uw_plane[1] = Uv0[1] - (mRwe * Ut_V[1]);
          Um[2] = (Um[2] * RtermW) + Rm_idx_2;
          OarcT[2] = Oarc[2];
          Uw_plane[2] = Uv0[2] - (mRwe * Ut_V[2]);
          LIB_unitvector_gdXEDyOJ(Uw_plane, vecV);

          /* 'DefineFlightPath:1048' Uw_plane = LIB_unitvector(Uw - dot(Uw,Uplane')*Uplane'); */
          mRwe = dot_lFwBkxue(Uw, Ut_V);
          Uw_0[0] = Uw[0] - (mRwe * Ut_V[0]);
          Uw_0[1] = Uw[1] - (mRwe * Ut_V[1]);
          Uw_0[2] = Uw[2] - (mRwe * Ut_V[2]);
          LIB_unitvector_gdXEDyOJ(Uw_0, Uw_plane);

          /* 'DefineFlightPath:1049' ANGv0 = atan2(-Uv0_plane(3),norm([Uv0_plane(1) Uv0_plane(2)]))*57.296; */
          /* 'DefineFlightPath:1050' ANGw  = atan2(-Uw_plane(3),norm([Uw_plane(1) Uw_plane(2)]))*57.296; */
          /* 'DefineFlightPath:1051' theFP_inplane = ANGw - ANGv0; */
          Uv0xy[0] = Uw_plane[0];
          Uv0xy[1] = Uw_plane[1];
          Utxy[0] = vecV[0];
          Utxy[1] = vecV[1];
          mRwe = (rt_atan2d_snf(-Uw_plane[2], norm_lpbhAiWU(Uv0xy)) * 57.296) -
            (rt_atan2d_snf(-vecV[2], norm_lpbhAiWU(Utxy)) * 57.296);

          /* 'DefineFlightPath:1052' tmp = LIB_limit(-1.0,1.0,dot(Uv0,Uw)); */
          den = dot_lFwBkxue(Uv0, Uw);

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1.0;
          } else if (den < -1.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = -1.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:1053' theFP = acos(tmp)*57.296; */
          delRE_prev = acos(den) * 57.296;

          /* 'DefineFlightPath:1058' RarcW_cmp = LIB_limit(0.0,2.0*RarcV,RarcW)*LIB_limit(0.1,1.0,(norm(a1)/Rarc0Lim)*sqrt(10.0/abs(theFP_inplane))); */
          FP_prev *= 2.0;

          /* 'LIB_limit:27' if (in > max) */
          if (!((*V_RarcW) > FP_prev)) {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            FP_prev = *V_RarcW;
          } else {
            /* 'LIB_limit:28' out = max; */
          }

          tmp_tmp = norm_MfVYifVG(a1);
          den = (tmp_tmp / Rarc0Lim) * sqrt(10.0 / fabs(mRwe));

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1.0;
          } else if (den < 0.1) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = 0.1;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          FP_prev *= den;

          /* 'DefineFlightPath:1065' if (ftime > 20.0) */
          /* 'DefineFlightPath:1073' Ut0_plane = LIB_unitvector(Ut0 - dot(Ut0,Uplane')*Uplane'); */
          den = dot_lFwBkxue(Ut0, Ut_V);
          Uw_plane[0] = Ut0[0] - (den * Ut_V[0]);
          Uw_plane[1] = Ut0[1] - (den * Ut_V[1]);
          Uw_plane[2] = Ut0[2] - (den * Ut_V[2]);
          LIB_unitvector_gdXEDyOJ(Uw_plane, vecV);

          /* 'DefineFlightPath:1074' Ut_plane = LIB_unitvector(Ut - dot(Ut,Uplane')*Uplane'); */
          den = dot_lFwBkxue(Ut, Ut_V);
          Uw_0[0] = Ut[0] - (den * Ut_V[0]);
          Uw_0[1] = Ut[1] - (den * Ut_V[1]);
          Uw_0[2] = Ut[2] - (den * Ut_V[2]);
          LIB_unitvector_gdXEDyOJ(Uw_0, Uw_plane);

          /* 'DefineFlightPath:1075' tmp = LIB_limit(-1.0,1.0,dot(Ut0_plane,Ut_plane)); */
          den = dot_lFwBkxue(vecV, Uw_plane);

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1.0;
          } else if (den < -1.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = -1.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:1076' mag = acos(tmp)*57.296; */
          /* 'DefineFlightPath:1077' nrm = cross(Ut0_plane,Ut_plane); */
          /* 'DefineFlightPath:1078' theAttack_inplane = -mag*sign(dot(Uplane,nrm)); */
          /* 'DefineFlightPath:1079' if (theAttack_inplane > 5.0) */
          cross_6Ajhxtbg(vecV, Uw_plane, tmp_b);
          Larc = dot_lFwBkxue(Ut_V, tmp_b);
          if (Larc < 0.0) {
            Larc = -1.0;
          } else if (Larc > 0.0) {
            Larc = 1.0;
          } else if (Larc == 0.0) {
            Larc = 0.0;
          } else {
            Larc = (rtNaN);
          }

          if (((-(acos(den) * 57.296)) * Larc) > 5.0) {
            /* 'DefineFlightPath:1080' latch_R4 = 1; */
            latch_R4 = 1;
          }

          /* 'DefineFlightPath:1083' psi = 2.0*acos(dot(-a1,Uv0)/norm(a1)); */
          Ut_V[0] = -a1[0];
          Ut_V[1] = -a1[1];
          Ut_V[2] = -a1[2];
          den = acos(dot_lFwBkxue(Ut_V, Uv0) / tmp_tmp) * 2.0;

          /* 'DefineFlightPath:1084' Larc = RarcW*psi; */
          Larc = (*V_RarcW) * den;

          /* 'DefineFlightPath:1085' psifac = LIB_limit(1.0,1000.0,sqrt(0.5*psi/0.3491)); */
          den = sqrt((0.5 * den) / 0.3491);

          /* 'LIB_limit:27' if (in > max) */
          if (den > 1000.0) {
            /* 'LIB_limit:28' out = max; */
            den = 1000.0;
          } else if (den < 1.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            den = 1.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:1086' if (tgt0rge > RtermT) */
          if (tgt0rge > RtermT) {
            /* 'DefineFlightPath:1087' Lfpath2bunt = LIB_limit(tgt0rge-RtermT,tgt0rge*psifac-RtermT,RtermW+Larc); */
            e_min = tgt0rge - RtermT;
            den *= tgt0rge;
            d_max = den - RtermT;
            Larc += RtermW;
            *Lfpath2bunt = Larc;

            /* 'LIB_limit:27' if (in > max) */
            if (Larc > d_max) {
              /* 'LIB_limit:28' out = max; */
              *Lfpath2bunt = d_max;
            } else if (Larc < e_min) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              *Lfpath2bunt = e_min;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }

            /* 'DefineFlightPath:1088' Lfpath2tgt = LIB_limit(tgt0rge,tgt0rge*psifac,RtermW+Larc+RtermT); */
            *Lfpath2tgt = Larc + RtermT;

            /* 'LIB_limit:27' if (in > max) */
            if ((*Lfpath2tgt) > den) {
              /* 'LIB_limit:28' out = max; */
              *Lfpath2tgt = den;
            } else if ((*Lfpath2tgt) < tgt0rge) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              *Lfpath2tgt = tgt0rge;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }
          } else {
            /* 'DefineFlightPath:1089' else */
            /* 'DefineFlightPath:1090' Lfpath2bunt = 0.0; */
            *Lfpath2bunt = 0.0;

            /* 'DefineFlightPath:1091' Lfpath2tgt = tgt0rge; */
            *Lfpath2tgt = tgt0rge;
          }

          /* 'DefineFlightPath:1094' if ((theFP_inplane < 0.0 || Kglide < 0.999) && RarcW_cmp ~= RarcW_prev && latch_R4 == 0 || AFmode == 2) */
          if (((((mRwe < 0.0) || (Kglide < 0.999)) && (FP_prev != RarcW_prev)) &&
               (latch_R4 == 0)) || (((int32_T)AFmode) == 2)) {
            /* 'DefineFlightPath:1099' if (do_plot) */
            /* 'DefineFlightPath:1116' if (log_FP) */
            if (((int32_T)log_FP) != 0) {
              /* 'DefineFlightPath:1117' iW = iW + 1; */
              tmp_a = ((uint32_T)(*iW)) + 1U;
              if (tmp_a > 65535U) {
                tmp_a = 65535U;
              }

              *iW = (uint16_T)tmp_a;

              /* 'DefineFlightPath:1118' if (iW > 1000) */
              if (((int32_T)(*iW)) > 1000) {
                /* 'DefineFlightPath:1119' iW = uint16(1000); */
                *iW = 1000U;
              }

              /* 'DefineFlightPath:1121' telstruct.GUI_iW(iW)         = uint16(LIB_limit(0,65535,double(iW))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_iW[((int32_T)(*iW)) - 1] = *iW;

              /* 'DefineFlightPath:1122' telstruct.GUI_solution(iW)   = int8(LIB_limit(-128,127,double(solution))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_solution[((int32_T)(*iW)) - 1] = *solution;

              /* 'DefineFlightPath:1123' telstruct.GUI_inccnt(iW)     = uint8(i); */
              telstruct->GUI_inccnt[((int32_T)(*iW)) - 1] = (uint8_T)i;

              /* 'DefineFlightPath:1124' telstruct.GUI_ftime(iW)      = single(ftime); */
              telstruct->GUI_ftime[((int32_T)(*iW)) - 1] = (real32_T)ftime;

              /* 'DefineFlightPath:1125' telstruct.GUI_Rarc(iW)       = uint16(LIB_limit(0,65535,Rarc0)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rarc0 > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (Rarc0 < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Rarc0;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_Rarc[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_Rarc[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1126' telstruct.GUI_RarcW(iW)      = uint16(LIB_limit(0,65535,RarcW)); */
              /* 'LIB_limit:27' if (in > max) */
              if ((*V_RarcW) > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = *V_RarcW;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RarcW[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RarcW[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1127' telstruct.GUI_Rterm(iW)      = uint16(LIB_limit(0,65535,Rterm)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rterm > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (Rterm < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Rterm;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_Rterm[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_Rterm[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1128' telstruct.GUI_Km(iW)         = int16(LIB_limit(-32768,32767,Km)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Km > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Km;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_Km[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Km[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1129' telstruct.GUI_RtermT(iW)     = uint16(LIB_limit(0,65535,RtermT)); */
              /* 'LIB_limit:27' if (in > max) */
              if (RtermT > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (RtermT < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = RtermT;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RtermT[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RtermT[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1130' telstruct.GUI_RtermW(iW)     = uint16(LIB_limit(0,65535,RtermW)); */
              /* 'LIB_limit:27' if (in > max) */
              if (RtermW > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (RtermW < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = RtermW;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RtermW[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RtermW[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1131' telstruct.GUI_LT(iW)         = int16(LIB_limit(-32768,32767,Lt)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Lt > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else if (Lt < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Lt;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_LT[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_LT[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1132' telstruct.GUI_LW(iW)         = int16(LIB_limit(-32768,32767,Lw)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Lw > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else if (Lw < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Lw;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_LW[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_LW[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1133' telstruct.GUI_Mglide(iW)     = single(-Mglide); */
              telstruct->GUI_Mglide[((int32_T)(*iW)) - 1] = (real32_T)((real_T)
                ((real_T)(-(*Mglide))));

              /* 'DefineFlightPath:1134' telstruct.GUI_FPazi(iW)      = single(FPazi*57.296); */
              telstruct->GUI_FPazi[((int32_T)(*iW)) - 1] = (real32_T)((real_T)
                ((real_T)((*FPazi) * 57.296)));

              /* 'DefineFlightPath:1135' telstruct.GUI_ratio(iW)      = single(ratio_gcapability); */
              telstruct->GUI_ratio[((int32_T)(*iW)) - 1] = (real32_T)
                (*ratio_gcapability);

              /* 'DefineFlightPath:1136' telstruct.GUI_ImpAng(iW)     = int16(LIB_limit(-32768,32767,ImpAng*57.296)); */
              ImpAng *= 57.296;

              /* 'LIB_limit:27' if (in > max) */
              if (ImpAng > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                ImpAng = 32767.0;
              } else if (ImpAng < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                ImpAng = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(ImpAng);
              if (Larc < 32768.0) {
                telstruct->GUI_ImpAng[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_ImpAng[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1137' telstruct.GUI_AttHeading(iW) = int16(LIB_limit(-32768,32767,AttHeading*57.296)); */
              AttHeading *= 57.296;

              /* 'LIB_limit:27' if (in > max) */
              if (AttHeading > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (AttHeading < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_AttHeading[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_AttHeading[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1139' telstruct.GUI_intsctTx(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1140' telstruct.GUI_intsctTy(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1141' telstruct.GUI_intsctTz(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp_1 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp_1 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp_1;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1142' telstruct.GUI_intsctWx(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1143' telstruct.GUI_intsctWy(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1144' telstruct.GUI_intsctWz(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1145' telstruct.GUI_Rtox(iW)       = int16(LIB_limit(-32768,32767,Rto(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtox[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtox[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1146' telstruct.GUI_Rtoy(iW)       = int16(LIB_limit(-32768,32767,Rto(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtoy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtoy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1147' telstruct.GUI_Rtoz(iW)       = int16(LIB_limit(-32768,32767,Rto(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtoz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtoz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1148' telstruct.GUI_Rtex(iW)       = int16(LIB_limit(-32768,32767,Rte(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rte_0[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rte_0[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rte_0[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtex[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtex[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1149' telstruct.GUI_Rtey(iW)       = int16(LIB_limit(-32768,32767,Rte(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rte_0[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rte_0[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rte_0[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtey[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtey[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1150' telstruct.GUI_Rtez(iW)       = int16(LIB_limit(-32768,32767,Rte(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtez[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtez[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1151' telstruct.GUI_Rwox(iW)       = int16(LIB_limit(-32768,32767,Rwo(1))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwox[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1152' telstruct.GUI_Rwoy(iW)       = int16(LIB_limit(-32768,32767,Rwo(2))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwoy[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1153' telstruct.GUI_Rwoz(iW)       = int16(LIB_limit(-32768,32767,Rwo(3))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwoz[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1154' telstruct.GUI_Rwex(iW)       = int16(LIB_limit(-32768,32767,Rwe(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwex[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwex[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1155' telstruct.GUI_Rwey(iW)       = int16(LIB_limit(-32768,32767,Rwe(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwey[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwey[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1156' telstruct.GUI_Rwez(iW)       = int16(LIB_limit(-32768,32767,Rwe(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwez[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwez[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1157' telstruct.GUI_Rmx(iW)        = int16(LIB_limit(-32768,32767,Rm(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1158' telstruct.GUI_Rmy(iW)        = int16(LIB_limit(-32768,32767,Rm(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_1 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_1 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_1;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1159' telstruct.GUI_Rmz(iW)        = int16(LIB_limit(-32768,32767,Rm(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_2 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_2 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_2;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1160' telstruct.GUI_a0Wx(iW)       = int16(LIB_limit(-32768,32767,a0W(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_0_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_0_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_0_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1161' telstruct.GUI_a0Wy(iW)       = int16(LIB_limit(-32768,32767,a0W(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_1_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_1_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_1_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1162' telstruct.GUI_a0Wz(iW)       = int16(LIB_limit(-32768,32767,a0W(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_2_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_2_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_2_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1163' telstruct.GUI_a0Tx(iW)       = int16(LIB_limit(-32768,32767,a0T(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Tx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Tx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1164' telstruct.GUI_a0Ty(iW)       = int16(LIB_limit(-32768,32767,a0T(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Ty[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Ty[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1165' telstruct.GUI_a0Tz(iW)       = int16(LIB_limit(-32768,32767,a0T(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Tz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Tz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1167' telstruct.GUI_spare(iW)      = uint16(0); */
              telstruct->GUI_spare[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1168' telstruct.GUI_tgt0x(iW)      = int16(LIB_limit(-32768,32767,tgt0.x)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_x > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_x < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_x;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0x[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0x[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1169' telstruct.GUI_tgt0y(iW)      = int16(LIB_limit(-32768,32767,tgt0.y)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_y > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_y < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_y;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0y[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0y[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1170' telstruct.GUI_tgt0z(iW)      = int16(LIB_limit(-32768,32767,tgt0.z)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_z > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_z < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_z;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0z[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0z[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1171' telstruct.GUI_altcnt(iW)     = uint16(LIB_limit(0,65535,count)); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_altcnt[((int32_T)(*iW)) - 1] = (uint16_T)count;

              /* 'DefineFlightPath:1172' telstruct.GUI_R2buntREQ(iW)  = uint16(0); */
              telstruct->GUI_R2buntREQ[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1173' telstruct.GUI_R2buntAVL(iW)  = uint16(0); */
              telstruct->GUI_R2buntAVL[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1174' telstruct.GUI_RarcW_cmp(iW)  = uint16(LIB_limit(0,65535,RarcW_cmp)); */
              /* 'LIB_limit:27' if (in > max) */
              if (FP_prev > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 65535.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = FP_prev;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 65536.0) {
                telstruct->GUI_RarcW_cmp[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RarcW_cmp[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1175' telstruct.GUI_theFP3D(iW)    = int16(LIB_limit(-32768,32767,theFP*100.0)); */
              AttHeading = delRE_prev * 100.0;

              /* 'LIB_limit:27' if (in > max) */
              if (AttHeading > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (AttHeading < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_theFP3D[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_theFP3D[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1176' telstruct.GUI_theFP2D(iW)    = int16(LIB_limit(-32768,32767,theFP_inplane*100.0)); */
              mRwe *= 100.0;

              /* 'LIB_limit:27' if (in > max) */
              if (mRwe > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                mRwe = 32767.0;
              } else if (mRwe < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                mRwe = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(mRwe);
              if (Larc < 32768.0) {
                telstruct->GUI_theFP2D[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_theFP2D[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1177' telstruct.GUI_theErr(iW)     = int16(0); */
              telstruct->GUI_theErr[((int32_T)(*iW)) - 1] = 0;
            }

            /* 'DefineFlightPath:1182' if ((theFP > 10.0 || RarcW_cmp < 0.95*Rarc0) && norm(a1) > 2.0*RtermW) */
            if (((delRE_prev > 10.0) || (FP_prev < (0.95 * Rarc0))) && (tmp_tmp >
                 (2.0 * RtermW))) {
              /* 'DefineFlightPath:1186' if (del_Rarc0 > -10.0 || RtermT == Rlim_lo || RtermW == Rlim_lo) */
              if (((psiUt0 > -10.0) || (RtermT == Rlim_lo)) || (RtermW ==
                   Rlim_lo)) {
                /* 'DefineFlightPath:1187' ratio_gcapability = LIB_limit(0.1,1.0,ratio_gcapability-0.1); */
                /* 'LIB_limit:27' if (in > max) */
                if (((*ratio_gcapability) - 0.1) > 1.0) {
                  /* 'LIB_limit:28' out = max; */
                  *ratio_gcapability = 1.0;
                } else if (((*ratio_gcapability) - 0.1) < 0.1) {
                  /* 'LIB_limit:29' elseif (in < min) */
                  /* 'LIB_limit:30' out = min; */
                  *ratio_gcapability = 0.1;
                } else {
                  /* 'LIB_limit:31' else */
                  /* 'LIB_limit:32' out = in; */
                  *ratio_gcapability -= 0.1;
                }
              }

              /* 'DefineFlightPath:1191' RtermT = ratio_gcapability*RtermT + (1.0-ratio_gcapability)*RtermW; */
              RtermT = ((1.0 - (*ratio_gcapability)) * RtermW) +
                ((*ratio_gcapability) * RtermT);

              /* 'DefineFlightPath:1193' Ut = ratio_gcapability*Ut0 + (1.0-ratio_gcapability)*Ut_V; */
              /* 'DefineFlightPath:1195' Ut = LIB_unitvector(Ut); */
              Ut_V[0] = ((1.0 - (*ratio_gcapability)) * b_I_0[0]) +
                ((*ratio_gcapability) * Ut0[0]);
              Ut_V[1] = ((1.0 - (*ratio_gcapability)) * b_I_0[1]) +
                ((*ratio_gcapability) * Ut0[1]);
              Ut_V[2] = ((1.0 - (*ratio_gcapability)) * b_I_0[2]) +
                ((*ratio_gcapability) * Ut0[2]);
              LIB_unitvector_gdXEDyOJ(Ut_V, Ut);
              guard4 = true;
            } else if (((FP_prev > (0.95 * Rarc0)) && (psiUt0 < -10.0)) &&
                       (count < 20)) {
              /* 'DefineFlightPath:1198' elseif (RarcW_cmp > 0.95*Rarc0 && del_Rarc0 < -10.0 && count < 20) */
              /* 'DefineFlightPath:1199' ratio_gcapability = LIB_limit(0.1,1.0,ratio_gcapability+0.05); */
              /* 'LIB_limit:27' if (in > max) */
              if (((*ratio_gcapability) + 0.05) > 1.0) {
                /* 'LIB_limit:28' out = max; */
                *ratio_gcapability = 1.0;
              } else if (((*ratio_gcapability) + 0.05) < 0.1) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                *ratio_gcapability = 0.1;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                *ratio_gcapability += 0.05;
              }

              /* 'DefineFlightPath:1200' RtermT = ratio_gcapability*RtermT + (1.0-ratio_gcapability)*RtermW; */
              RtermT = ((1.0 - (*ratio_gcapability)) * RtermW) +
                ((*ratio_gcapability) * RtermT);

              /* 'DefineFlightPath:1202' Ut = ratio_gcapability*Ut0 + (1.0-ratio_gcapability)*Ut_V; */
              /* 'DefineFlightPath:1204' Ut = LIB_unitvector(Ut); */
              Ut_V[0] = ((1.0 - (*ratio_gcapability)) * b_I_0[0]) +
                ((*ratio_gcapability) * Ut0[0]);
              Ut_V[1] = ((1.0 - (*ratio_gcapability)) * b_I_0[1]) +
                ((*ratio_gcapability) * Ut0[1]);
              Ut_V[2] = ((1.0 - (*ratio_gcapability)) * b_I_0[2]) +
                ((*ratio_gcapability) * Ut0[2]);
              LIB_unitvector_gdXEDyOJ(Ut_V, Ut);
              guard4 = true;
            } else if ((((RtermW < (Rlim_lo + 0.1)) && ((*ratio_gcapability) <
                          0.101)) || ((FP_prev > (0.95 * Rarc0)) && (psiUt0 >
                          -10.0))) || (count == 20)) {
              /* 'DefineFlightPath:1205' elseif ((RtermW < Rlim_lo+0.1 && ratio_gcapability < 0.101) || (RarcW_cmp > 0.95*Rarc0 && del_Rarc0 > -10.0) || count == 20) */
              /* 'DefineFlightPath:1207' solution = int8(-1); */
              *solution = -1;

              /* 'DefineFlightPath:1208' if (log_FP) */
              if (((int32_T)log_FP) != 0) {
                /* 'DefineFlightPath:1209' telstruct.GUI_solution(iW) = int8(solution); */
                telstruct->GUI_solution[((int32_T)(*iW)) - 1] = -1;
              }

              /* 'DefineFlightPath:1211' count = 20; */
              count = 20;
              exitg1 = 1;
            } else {
              guard4 = true;
            }
          } else {
            /* 'DefineFlightPath:1217' else */
            /* 'DefineFlightPath:1221' latch_R4 = 1; */
            latch_R4 = 1;

            /* 'DefineFlightPath:1223' a1 = Rwe-Rte; */
            /* 'DefineFlightPath:1224' v = cross(Ut,a1'); */
            /* 'DefineFlightPath:1225' c = dot(Ut,a1'); */
            Larc = dot_lFwBkxue(Ut, a1);

            /* 'DefineFlightPath:1227' k = LIB_unitvector(v); */
            cross_6Ajhxtbg(Ut, a1, tmp_c);
            LIB_unitvector_gdXEDyOJ(tmp_c, vecV);

            /* 'DefineFlightPath:1228' I = [1 0 0;0 1 0;0 0 1]; */
            /* 'DefineFlightPath:1229' K =[0 -k(3) k(2);k(3) 0 -k(1);-k(2) k(1) 0]; */
            K[0] = 0.0;
            K[3] = -vecV[2];
            K[6] = vecV[1];
            K[1] = vecV[2];
            K[4] = 0.0;
            K[7] = -vecV[0];
            K[2] = -vecV[1];
            K[5] = vecV[0];
            K[8] = 0.0;

            /* 'DefineFlightPath:1230' tmp = LIB_limit(-1.0,1.0,c/norm(a1)); */
            den = Larc / tmp_tmp;

            /* 'LIB_limit:27' if (in > max) */
            if (den > 1.0) {
              /* 'LIB_limit:28' out = max; */
              den = 1.0;
            } else if (den < -1.0) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              den = -1.0;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }

            /* 'DefineFlightPath:1231' theErr = acos(tmp); */
            psiUt0 = acos(den);

            /* 'DefineFlightPath:1232' dthe = 0.1*theErr*sign(c); */
            if (Larc < 0.0) {
              Larc = -1.0;
            } else if (Larc > 0.0) {
              Larc = 1.0;
            } else if (Larc == 0.0) {
              Larc = 0.0;
            } else {
              Larc = (rtNaN);
            }

            Larc *= 0.1 * psiUt0;

            /* 'DefineFlightPath:1233' R = I + sin(dthe)*K + (1-cos(dthe))*K*K; */
            den = sin(Larc);
            Larc = cos(Larc);

            /* 'DefineFlightPath:1234' Utmp = Ut; */
            /* 'DefineFlightPath:1235' Ut = R*Utmp; */
            for (i_0 = 0; i_0 < 3; i_0++) {
              b_I_0[i_0] = 0.0;
              for (i_1 = 0; i_1 < 3; i_1++) {
                b_I_tmp = (3 * i_1) + i_0;
                b_I_0[i_0] += ((((((1.0 - Larc) * K[i_0]) * K[3 * i_1]) + (((1.0
                  - Larc) * K[i_0 + 3]) * K[(3 * i_1) + 1])) + (((1.0 - Larc) *
                  K[i_0 + 6]) * K[(3 * i_1) + 2])) + ((K[b_I_tmp] * den) +
                  ((real_T)b_I[b_I_tmp]))) * Ut[i_1];
              }
            }

            /* 'DefineFlightPath:1236' Ut0 = Ut; */
            Ut0[0] = b_I_0[0];
            Ut[0] = b_I_0[0];
            Ut0[1] = b_I_0[1];
            Ut[1] = b_I_0[1];
            Ut0[2] = b_I_0[2];
            Ut[2] = b_I_0[2];

            /* 'DefineFlightPath:1238' if (do_plot) */
            /* 'DefineFlightPath:1256' if (log_FP) */
            if (((int32_T)log_FP) != 0) {
              /* 'DefineFlightPath:1257' iW = iW + 1; */
              tmp_a = ((uint32_T)(*iW)) + 1U;
              if (tmp_a > 65535U) {
                tmp_a = 65535U;
              }

              *iW = (uint16_T)tmp_a;

              /* 'DefineFlightPath:1258' if (iW > 1000) */
              if (((int32_T)(*iW)) > 1000) {
                /* 'DefineFlightPath:1259' iW = uint16(1000); */
                *iW = 1000U;
              }

              /* 'DefineFlightPath:1261' telstruct.GUI_iW(iW)         = uint16(LIB_limit(0,65535,double(iW))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_iW[((int32_T)(*iW)) - 1] = *iW;

              /* 'DefineFlightPath:1262' telstruct.GUI_solution(iW)   = int8(LIB_limit(-128,127,double(solution))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_solution[((int32_T)(*iW)) - 1] = *solution;

              /* 'DefineFlightPath:1263' telstruct.GUI_inccnt(iW)     = uint8(i); */
              telstruct->GUI_inccnt[((int32_T)(*iW)) - 1] = (uint8_T)i;

              /* 'DefineFlightPath:1264' telstruct.GUI_ftime(iW)      = single(ftime); */
              telstruct->GUI_ftime[((int32_T)(*iW)) - 1] = (real32_T)ftime;

              /* 'DefineFlightPath:1265' telstruct.GUI_Rarc(iW)       = uint16(LIB_limit(0,65535,Rarc0)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rarc0 > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (Rarc0 < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Rarc0;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_Rarc[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_Rarc[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1266' telstruct.GUI_RarcW(iW)      = uint16(LIB_limit(0,65535,RarcW)); */
              /* 'LIB_limit:27' if (in > max) */
              if ((*V_RarcW) > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = *V_RarcW;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RarcW[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RarcW[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1267' telstruct.GUI_Rterm(iW)      = uint16(LIB_limit(0,65535,Rterm)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rterm > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (Rterm < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Rterm;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_Rterm[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_Rterm[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1268' telstruct.GUI_Km(iW)         = int16(LIB_limit(-32768,32767,Km)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Km > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Km;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_Km[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Km[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1269' telstruct.GUI_RtermT(iW)     = uint16(LIB_limit(0,65535,RtermT)); */
              /* 'LIB_limit:27' if (in > max) */
              if (RtermT > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (RtermT < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = RtermT;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RtermT[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RtermT[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1270' telstruct.GUI_RtermW(iW)     = uint16(LIB_limit(0,65535,RtermW)); */
              /* 'LIB_limit:27' if (in > max) */
              if (RtermW > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                den = 65535.0;
              } else if (RtermW < 0.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = 0.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = RtermW;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 65536.0) {
                telstruct->GUI_RtermW[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RtermW[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1271' telstruct.GUI_LT(iW)         = int16(LIB_limit(-32768,32767,Lt)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Lt > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else if (Lt < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Lt;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_LT[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_LT[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1272' telstruct.GUI_LW(iW)         = int16(LIB_limit(-32768,32767,Lw)); */
              /* 'LIB_limit:27' if (in > max) */
              if (Lw > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                den = 32767.0;
              } else if (Lw < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                den = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                den = Lw;
              }

              Larc = rt_roundd_snf(den);
              if (Larc < 32768.0) {
                telstruct->GUI_LW[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_LW[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1273' telstruct.GUI_Mglide(iW)     = single(-Mglide); */
              telstruct->GUI_Mglide[((int32_T)(*iW)) - 1] = (real32_T)((real_T)
                ((real_T)(-(*Mglide))));

              /* 'DefineFlightPath:1274' telstruct.GUI_FPazi(iW)      = single(FPazi*57.296); */
              telstruct->GUI_FPazi[((int32_T)(*iW)) - 1] = (real32_T)((real_T)
                ((real_T)((*FPazi) * 57.296)));

              /* 'DefineFlightPath:1275' telstruct.GUI_ratio(iW)      = single(ratio_gcapability); */
              telstruct->GUI_ratio[((int32_T)(*iW)) - 1] = (real32_T)
                (*ratio_gcapability);

              /* 'DefineFlightPath:1276' telstruct.GUI_ImpAng(iW)     = int16(LIB_limit(-32768,32767,ImpAng*57.296)); */
              ImpAng *= 57.296;

              /* 'LIB_limit:27' if (in > max) */
              if (ImpAng > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                ImpAng = 32767.0;
              } else if (ImpAng < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                ImpAng = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(ImpAng);
              if (Larc < 32768.0) {
                telstruct->GUI_ImpAng[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_ImpAng[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1277' telstruct.GUI_AttHeading(iW) = int16(LIB_limit(-32768,32767,AttHeading*57.296)); */
              AttHeading *= 57.296;

              /* 'LIB_limit:27' if (in > max) */
              if (AttHeading > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (AttHeading < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_AttHeading[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_AttHeading[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1279' telstruct.GUI_intsctTx(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1280' telstruct.GUI_intsctTy(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1281' telstruct.GUI_intsctTz(iW)   = int16(LIB_limit(-32768,32767,intsctT.pos(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (den_tmp_1 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (den_tmp_1 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = den_tmp_1;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctTz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctTz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1282' telstruct.GUI_intsctWx(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1283' telstruct.GUI_intsctWy(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1284' telstruct.GUI_intsctWz(iW)   = int16(LIB_limit(-32768,32767,intsctW.pos(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto_tmp_0[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto_tmp_0[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto_tmp_0[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_intsctWz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_intsctWz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1285' telstruct.GUI_Rtox(iW)       = int16(LIB_limit(-32768,32767,Rto(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtox[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtox[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1286' telstruct.GUI_Rtoy(iW)       = int16(LIB_limit(-32768,32767,Rto(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtoy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtoy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1287' telstruct.GUI_Rtoz(iW)       = int16(LIB_limit(-32768,32767,Rto(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rto[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rto[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rto[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtoz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtoz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1288' telstruct.GUI_Rtex(iW)       = int16(LIB_limit(-32768,32767,Rte(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rte_0[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rte_0[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rte_0[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtex[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtex[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1289' telstruct.GUI_Rtey(iW)       = int16(LIB_limit(-32768,32767,Rte(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rte_0[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rte_0[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rte_0[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtey[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtey[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1290' telstruct.GUI_Rtez(iW)       = int16(LIB_limit(-32768,32767,Rte(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rtez[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rtez[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1291' telstruct.GUI_Rwox(iW)       = int16(LIB_limit(-32768,32767,Rwo(1))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwox[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1292' telstruct.GUI_Rwoy(iW)       = int16(LIB_limit(-32768,32767,Rwo(2))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwoy[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1293' telstruct.GUI_Rwoz(iW)       = int16(LIB_limit(-32768,32767,Rwo(3))); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_Rwoz[((int32_T)(*iW)) - 1] = 0;

              /* 'DefineFlightPath:1294' telstruct.GUI_Rwex(iW)       = int16(LIB_limit(-32768,32767,Rwe(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwex[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwex[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1295' telstruct.GUI_Rwey(iW)       = int16(LIB_limit(-32768,32767,Rwe(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwey[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwey[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1296' telstruct.GUI_Rwez(iW)       = int16(LIB_limit(-32768,32767,Rwe(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (vecT[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (vecT[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = vecT[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rwez[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rwez[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1297' telstruct.GUI_Rmx(iW)        = int16(LIB_limit(-32768,32767,Rm(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1298' telstruct.GUI_Rmy(iW)        = int16(LIB_limit(-32768,32767,Rm(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_1 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_1 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_1;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1299' telstruct.GUI_Rmz(iW)        = int16(LIB_limit(-32768,32767,Rm(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_2 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_2 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_2;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_Rmz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_Rmz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1300' telstruct.GUI_a0Wx(iW)       = int16(LIB_limit(-32768,32767,a0W(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_0_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_0_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_0_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1301' telstruct.GUI_a0Wy(iW)       = int16(LIB_limit(-32768,32767,a0W(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_1_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_1_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_1_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wy[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wy[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1302' telstruct.GUI_a0Wz(iW)       = int16(LIB_limit(-32768,32767,a0W(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (Rm_idx_2_0 > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (Rm_idx_2_0 < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = Rm_idx_2_0;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Wz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Wz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1303' telstruct.GUI_a0Tx(iW)       = int16(LIB_limit(-32768,32767,a0T(1))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[0] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[0] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[0];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Tx[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Tx[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1304' telstruct.GUI_a0Ty(iW)       = int16(LIB_limit(-32768,32767,a0T(2))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[1] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[1] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[1];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Ty[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Ty[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1305' telstruct.GUI_a0Tz(iW)       = int16(LIB_limit(-32768,32767,a0T(3))); */
              /* 'LIB_limit:27' if (in > max) */
              if (U_a1[2] > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (U_a1[2] < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = U_a1[2];
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_a0Tz[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_a0Tz[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1307' telstruct.GUI_spare(iW)      = uint16(0); */
              telstruct->GUI_spare[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1308' telstruct.GUI_tgt0x(iW)      = int16(LIB_limit(-32768,32767,tgt0.x)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_x > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_x < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_x;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0x[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0x[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1309' telstruct.GUI_tgt0y(iW)      = int16(LIB_limit(-32768,32767,tgt0.y)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_y > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_y < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_y;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0y[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0y[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1310' telstruct.GUI_tgt0z(iW)      = int16(LIB_limit(-32768,32767,tgt0.z)); */
              /* 'LIB_limit:27' if (in > max) */
              if (tgt0_z > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 32767.0;
              } else if (tgt0_z < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                AttHeading = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = tgt0_z;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 32768.0) {
                telstruct->GUI_tgt0z[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_tgt0z[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1311' telstruct.GUI_altcnt(iW)     = uint16(LIB_limit(0,65535,count)); */
              /* 'LIB_limit:27' if (in > max) */
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
              telstruct->GUI_altcnt[((int32_T)(*iW)) - 1] = (uint16_T)count;

              /* 'DefineFlightPath:1312' telstruct.GUI_R2buntREQ(iW)  = uint16(0); */
              telstruct->GUI_R2buntREQ[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1313' telstruct.GUI_R2buntAVL(iW)  = uint16(0); */
              telstruct->GUI_R2buntAVL[((int32_T)(*iW)) - 1] = 0U;

              /* 'DefineFlightPath:1314' telstruct.GUI_RarcW_cmp(iW)  = uint16(LIB_limit(0,65535,RarcW_cmp)); */
              /* 'LIB_limit:27' if (in > max) */
              if (FP_prev > 65535.0) {
                /* 'LIB_limit:28' out = max; */
                AttHeading = 65535.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
                AttHeading = FP_prev;
              }

              Larc = rt_roundd_snf(AttHeading);
              if (Larc < 65536.0) {
                telstruct->GUI_RarcW_cmp[((int32_T)(*iW)) - 1] = (uint16_T)Larc;
              } else {
                telstruct->GUI_RarcW_cmp[((int32_T)(*iW)) - 1] = MAX_uint16_T;
              }

              /* 'DefineFlightPath:1315' telstruct.GUI_theFP3D(iW)    = int16(LIB_limit(-32768,32767,theFP*100.0)); */
              delRE_prev *= 100.0;

              /* 'LIB_limit:27' if (in > max) */
              if (delRE_prev > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                delRE_prev = 32767.0;
              } else if (delRE_prev < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                delRE_prev = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(delRE_prev);
              if (Larc < 32768.0) {
                telstruct->GUI_theFP3D[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_theFP3D[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1316' telstruct.GUI_theFP2D(iW)    = int16(LIB_limit(-32768,32767,theFP_inplane*100.0)); */
              mRwe *= 100.0;

              /* 'LIB_limit:27' if (in > max) */
              if (mRwe > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                mRwe = 32767.0;
              } else if (mRwe < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                mRwe = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(mRwe);
              if (Larc < 32768.0) {
                telstruct->GUI_theFP2D[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_theFP2D[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }

              /* 'DefineFlightPath:1317' telstruct.GUI_theErr(iW)     = int16(LIB_limit(-32768,32767,theErr*5729.6)); */
              mRwe = psiUt0 * 5729.6;

              /* 'LIB_limit:27' if (in > max) */
              if (mRwe > 32767.0) {
                /* 'LIB_limit:28' out = max; */
                mRwe = 32767.0;
              } else if (mRwe < -32768.0) {
                /* 'LIB_limit:29' elseif (in < min) */
                /* 'LIB_limit:30' out = min; */
                mRwe = -32768.0;
              } else {
                /* 'LIB_limit:31' else */
                /* 'LIB_limit:32' out = in; */
              }

              Larc = rt_roundd_snf(mRwe);
              if (Larc < 32768.0) {
                telstruct->GUI_theErr[((int32_T)(*iW)) - 1] = (int16_T)Larc;
              } else {
                telstruct->GUI_theErr[((int32_T)(*iW)) - 1] = MAX_int16_T;
              }
            }

            /* 'DefineFlightPath:1321' if (abs(theErr) < 0.01745 || RarcW_cmp > 0.95*Rarc0) */
            if ((fabs(psiUt0) < 0.01745) || (FP_prev > (0.95 * Rarc0))) {
              /* 'DefineFlightPath:1322' solution = int8(-2); */
              *solution = -2;

              /* 'DefineFlightPath:1323' if (log_FP) */
              if (((int32_T)log_FP) != 0) {
                /* 'DefineFlightPath:1324' telstruct.GUI_solution(iW) = int8(solution); */
                telstruct->GUI_solution[((int32_T)(*iW)) - 1] = -2;
              }

              exitg1 = 1;
            } else {
              guard4 = true;
            }
          }
        }

        if (guard4) {
          /* 'DefineFlightPath:1334' RarcW_prev = RarcW; */
          RarcW_prev = *V_RarcW;

          /* 'DefineFlightPath:1335' if (norm(a1) < 2.0*RtermW) */
          if (norm_MfVYifVG(a1) < (2.0 * RtermW)) {
            /* 'DefineFlightPath:1336' lim_lo = Rlim_lo; */
            mRwe = Rlim_lo;
          } else {
            /* 'DefineFlightPath:1337' else */
            /* 'DefineFlightPath:1338' lim_lo = 0.0; */
            mRwe = 0.0;
          }

          /* 'DefineFlightPath:1340' RtermT = LIB_limit(Rlim_lo,1000000.0,RtermT - LIB_limit(lim_lo,(1.0-fac90)*500.0,Rarc0-RarcW_cmp)); */
          delRE_prev = (1.0 - psiUw) * 500.0;
          FP_prev = Rarc0 - FP_prev;
          AttHeading = FP_prev;

          /* 'LIB_limit:27' if (in > max) */
          if (FP_prev > delRE_prev) {
            /* 'LIB_limit:28' out = max; */
            AttHeading = delRE_prev;
          } else if (FP_prev < mRwe) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            AttHeading = mRwe;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          RtermT -= AttHeading;

          /* 'LIB_limit:27' if (in > max) */
          if (RtermT > 1.0E+6) {
            /* 'LIB_limit:28' out = max; */
            RtermT = 1.0E+6;
          } else if (RtermT < Rlim_lo) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            RtermT = Rlim_lo;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:1341' RtermW = LIB_limit(Rlim_lo,1000000.0,RtermW - LIB_limit(lim_lo,(1.0-fac90)*500.0,Rarc0-RarcW_cmp)); */
          /* 'LIB_limit:27' if (in > max) */
          if (FP_prev > delRE_prev) {
            /* 'LIB_limit:28' out = max; */
            FP_prev = delRE_prev;
          } else if (FP_prev < mRwe) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            FP_prev = mRwe;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          RtermW -= FP_prev;

          /* 'LIB_limit:27' if (in > max) */
          if (RtermW > 1.0E+6) {
            /* 'LIB_limit:28' out = max; */
            RtermW = 1.0E+6;
          } else if (RtermW < Rlim_lo) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            RtermW = Rlim_lo;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'DefineFlightPath:1344' if (count >= 20) */
          if (count >= 20) {
            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);

      guard1 = true;
    }

    if (guard1) {
      /* 'DefineFlightPath:1353' if (do_plot) */
      /* 'DefineFlightPath:1381' FP_prev = FPazi; */
      FP_prev = *FPazi;

      /* 'DefineFlightPath:1382' delRE_prev = delRE; */
      delRE_prev = La1;

      /* 'DefineFlightPath:1392' if (solution < 0) */
      if (((*solution) < 0) || (count >= 20)) {
        /* 'DefineFlightPath:1393' if (do_plot) */
        exitg2 = true;
      } else {
        /* 'DefineFlightPath:1398' if (count >= 20) */
        i++;
      }
    }
  }

  /* 'DefineFlightPath:1404' Mglide = -Uw(3)/sqrt(Uw(1)*Uw(1)+Uw(2)*Uw(2)); */
  *Mglide = (-Uw[2]) / sqrt((Uw[0] * Uw[0]) + (Uw[1] * Uw[1]));

  /* 'DefineFlightPath:1405' Mtgt = -Rm(3)/sqrt(Rm(1)^2+Rm(2)^2); */
  Rarc0Lim = (-Rm_idx_2) / sqrt((Rm_idx_0 * Rm_idx_0) + (Rm_idx_1 * Rm_idx_1));

  /* 'DefineFlightPath:1406' if (Mtgt < Mglide && solution) */
  if ((Rarc0Lim < (*Mglide)) && ((*solution) != 0)) {
    /* 'DefineFlightPath:1407' Mglide = Mtgt; */
    *Mglide = Rarc0Lim;
  }

  /* 'DefineFlightPath:1410' if (do_plot) */
  /* 'DefineFlightPath:1416' V_Rto.x  =  Rto(1); */
  V_Rto->x = Rto[0];

  /* 'DefineFlightPath:1417' V_Rto.y  =  Rto(2); */
  V_Rto->y = Rto[1];

  /* 'DefineFlightPath:1418' V_Rto.z  =  Rto(3); */
  V_Rto->z = Rto[2];

  /* 'DefineFlightPath:1419' V_Rte.x  =  Rte(1); */
  V_Rte->x = Rte_0[0];

  /* 'DefineFlightPath:1420' V_Rte.y  =  Rte(2); */
  V_Rte->y = Rte_0[1];

  /* 'DefineFlightPath:1421' V_Rte.z  =  Rte(3); */
  V_Rte->z = Rte_0[2];

  /* 'DefineFlightPath:1422' V_Rwo.x  =  Rwo(1); */
  V_Rwo->x = Rwo[0];

  /* 'DefineFlightPath:1423' V_Rwo.y  =  Rwo(2); */
  V_Rwo->y = Rwo[1];

  /* 'DefineFlightPath:1424' V_Rwo.z  =  Rwo(3); */
  V_Rwo->z = Rwo[2];

  /* 'DefineFlightPath:1425' V_Rwe.x  =  Rwe(1); */
  V_Rwe->x = vecT[0];

  /* 'DefineFlightPath:1426' V_Rwe.y  =  Rwe(2); */
  V_Rwe->y = vecT[1];

  /* 'DefineFlightPath:1427' V_Rwe.z  =  Rwe(3); */
  V_Rwe->z = vecT[2];

  /* 'DefineFlightPath:1428' V_Rmwo.x =  a0W(1); */
  V_Rmwo->x = Um[0];

  /* 'DefineFlightPath:1429' V_Rmwo.y =  a0W(2); */
  V_Rmwo->y = Um[1];

  /* 'DefineFlightPath:1430' V_Rmwo.z =  a0W(3); */
  V_Rmwo->z = Um[2];

  /* 'DefineFlightPath:1431' V_Rmto.x =  a0T(1); */
  V_Rmto->x = U_a1[0];

  /* 'DefineFlightPath:1432' V_Rmto.y =  a0T(2); */
  V_Rmto->y = U_a1[1];

  /* 'DefineFlightPath:1433' V_Rmto.z =  a0T(3); */
  V_Rmto->z = U_a1[2];

  /* 'DefineFlightPath:1434' V_Rm.x   =  Rm(1); */
  V_Rm->x = Rm_idx_0;

  /* 'DefineFlightPath:1435' V_Rm.y   =  Rm(2); */
  V_Rm->y = Rm_idx_1;

  /* 'DefineFlightPath:1436' V_Rm.z   =  Rm(3); */
  V_Rm->z = Rm_idx_2;

  /* 'DefineFlightPath:1437' V_RarcT  =  RarcT; */
  /* 'DefineFlightPath:1438' V_OarcT.x=  OarcT(1); */
  V_OarcT->x = OarcT[0];

  /* 'DefineFlightPath:1439' V_OarcT.y=  OarcT(2); */
  V_OarcT->y = OarcT[1];

  /* 'DefineFlightPath:1440' V_OarcT.z=  OarcT(3); */
  V_OarcT->z = OarcT[2];

  /* 'DefineFlightPath:1441' V_RarcW  =  RarcW; */
  /* 'DefineFlightPath:1442' V_OarcW.x=  OarcW(1); */
  V_OarcW->x = Oarc[0];

  /* 'DefineFlightPath:1443' V_OarcW.y=  OarcW(2); */
  V_OarcW->y = Oarc[1];

  /* 'DefineFlightPath:1444' V_OarcW.z=  OarcW(3); */
  V_OarcW->z = Oarc[2];
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [TRACK, tel_struct, telem] = DefineFlightPath(TRACK, OPT_FP, MP_in, TPosNED, MPosNED, MVelNED, MRefNED, DCM, ENV, ...
 *                                                        guidmode, opportunity_mode, AFmode, guid_settings, Rarc_T, cnsts, ftime, tel_struct, telem)
 */
static void DefineFlightPath(sFYrLWvx1kVFonG2Nu0GNkH *TRACK, real_T OPT_FP_Mopt,
  real_T OPT_FP_Mglide_opt, uint8_T MP_in_WeaponType, const T_Cartesian TPosNED,
  const T_Cartesian MPosNED, const T_Cartesian MVelNED, const T_Cartesian
  MRefNED, const sPLFBkBfmjj013qOjLWm7GC *DCM, real_T ENV_mach, uint8_T
  opportunity_mode, uint8_T AFmode, real_T guid_settings_psiR_Thr, real_T
  guid_settings_psi_Engage, real_T Rarc_T, real_T ftime, sFMpPTQ95PcZEskZd3p4FcD
  *tel_struct, sqfeYN8Hrouv8AbOaDO2afG *telem)
{
  real_T Roff;
  real_T Sfac;
  real_T ref0_y;
  real_T tgt0_y;
  real_T tgt0_z;
  real_T Uv[3];
  real_T Ut[3];
  real_T psi;
  real_T Ut0[3];
  real_T temp[3];
  real_T den;
  real_T V_RarcT;
  real_T V_RarcW;
  real_T Uw[3];
  int8_T sol;
  real_T FPazi;
  real_T Lfpath2bunt;
  real_T Lfpath2tgt;
  uint8_T latch[2];
  real_T Mglide;
  T_Cartesian V_Rto;
  T_Cartesian V_Rte;
  T_Cartesian V_Rwo;
  T_Cartesian V_Rwe;
  T_Cartesian V_Rmwo;
  T_Cartesian V_Rmto;
  T_Cartesian V_Rm;
  T_Cartesian V_OarcT;
  T_Cartesian V_OarcW;
  real_T Uv_0[2];
  real_T Uw_0[2];
  real_T MVelNED_0[3];

  /* 'DefineFlightPath:5' Roff = norm([MRefNED.x MRefNED.y MRefNED.z]-[MPosNED.x MPosNED.y MPosNED.z]); */
  Uv[0] = MRefNED.x - MPosNED.x;
  Uv[1] = MRefNED.y - MPosNED.y;
  Uv[2] = MRefNED.z - MPosNED.z;
  Roff = norm_MfVYifVG(Uv);

  /* 'DefineFlightPath:6' if (Roff > 0.001) */
  if (Roff > 0.001) {
    /* 'DefineFlightPath:7' Sfac = LIB_limit(0.0,200.0/Roff,0.5); */
    Sfac = 200.0 / Roff;

    /* 'LIB_limit:27' if (in > max) */
    if (!(0.5 > Sfac)) {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Sfac = 0.5;
    } else {
      /* 'LIB_limit:28' out = max; */
    }

    /* 'DefineFlightPath:8' TRACK.Roff_intg = (1.0-Sfac)*TRACK.Roff_intg; */
    TRACK->Roff_intg *= 1.0 - Sfac;
  } else {
    /* 'DefineFlightPath:9' else */
    /* 'DefineFlightPath:10' Sfac = 1.0; */
    Sfac = 1.0;
  }

  /* 'DefineFlightPath:12' ref0.x = Sfac*MRefNED.x + (1.0-Sfac)*MPosNED.x; */
  Roff = ((1.0 - Sfac) * MPosNED.x) + (Sfac * MRefNED.x);

  /* 'DefineFlightPath:13' ref0.y = Sfac*MRefNED.y + (1.0-Sfac)*MPosNED.y; */
  ref0_y = ((1.0 - Sfac) * MPosNED.y) + (Sfac * MRefNED.y);

  /* 'DefineFlightPath:14' ref0.z = Sfac*MRefNED.z + (1.0-Sfac)*MPosNED.z; */
  Sfac = ((1.0 - Sfac) * MPosNED.z) + (Sfac * MRefNED.z);

  /* 'DefineFlightPath:21' tgt0.x = TPosNED.x - ref0.x; */
  psi = TPosNED.x - Roff;

  /* 'DefineFlightPath:22' tgt0.y = TPosNED.y - ref0.y; */
  tgt0_y = TPosNED.y - ref0_y;

  /* 'DefineFlightPath:23' if (ref0.z > TPosNED.z) */
  if (Sfac > TPosNED.z) {
    /* 'DefineFlightPath:24' tgt0.z = 0.001; */
    tgt0_z = 0.001;
  } else {
    /* 'DefineFlightPath:25' else */
    /* 'DefineFlightPath:26' tgt0.z = TPosNED.z - ref0.z; */
    tgt0_z = TPosNED.z - Sfac;
  }

  /* 'DefineFlightPath:29' if (ftime > 0.0 && abs(tgt0.x) < 32767 && abs(tgt0.y) < 32767 && abs(tgt0.z) < 32767) */
  if ((((ftime > 0.0) && (fabs(psi) < 32767.0)) && (fabs(tgt0_y) < 32767.0)) &&
      (fabs(tgt0_z) < 32767.0)) {
    /* 'DefineFlightPath:30' TRACK.log_FP = uint8(1); */
    TRACK->log_FP = 1U;
  }

  /* 'DefineFlightPath:33' if (ftime > 64.0) */
  /* 'DefineFlightPath:38' iW = telem.IW; */
  /* 'DefineFlightPath:41' Uv = LIB_unitvector([MVelNED.x MVelNED.y MVelNED.z]); */
  MVelNED_0[0] = MVelNED.x;
  MVelNED_0[1] = MVelNED.y;
  MVelNED_0[2] = MVelNED.z;
  LIB_unitvector_gdXEDyOJ(MVelNED_0, Uv);

  /* 'DefineFlightPath:43' FPazi = atan2(MVelNED.y,MVelNED.x); */
  /* 'DefineFlightPath:44' Mach_diff = OPT_FP.Mopt-ENV.mach; */
  /* 'DefineFlightPath:45' latch = TRACK.latch; */
  /* 'DefineFlightPath:46' TRACK.solution = int8(0); */
  /* 'DefineFlightPath:50' [V_Rto,V_Rte,V_Rwo,V_Rwe,V_Rmwo,V_Rmto,V_Rm,V_RarcT,V_OarcT,V_RarcW,V_OarcW,Uw,sol,FPazi,Lfpath2bunt,Lfpath2tgt,Ut,latch,ratio,Mglide,tel_struct,iW] = ... */
  /* 'DefineFlightPath:51'   GetDoubleArcVectors(ftime, ... */
  /* 'DefineFlightPath:52'   opportunity_mode, ... */
  /* 'DefineFlightPath:53'   TRACK.once, ... */
  /* 'DefineFlightPath:54'   TRACK.solution, ... */
  /* 'DefineFlightPath:55'   FPazi, ... */
  /* 'DefineFlightPath:56'   tgt0, ... */
  /* 'DefineFlightPath:57'   TRACK.Ut, ... */
  /* 'DefineFlightPath:58'   Uv, ... */
  /* 'DefineFlightPath:59'   TRACK.Kglide, ... */
  /* 'DefineFlightPath:60'   OPT_FP.Mglide_opt, ... */
  /* 'DefineFlightPath:61'   Mach_diff, ... */
  /* 'DefineFlightPath:62'   latch, ... */
  /* 'DefineFlightPath:63'   guid_settings.Rterm, ... */
  /* 'DefineFlightPath:64'   TRACK.Rlim_lo, ... */
  /* 'DefineFlightPath:65'   Rarc_T, ... */
  /* 'DefineFlightPath:66'   guid_settings.psi_Engage, ... */
  /* 'DefineFlightPath:67'   MP_in.WeaponType, ... */
  /* 'DefineFlightPath:68'   AFmode, ... */
  /* 'DefineFlightPath:69'   TRACK.log_FP, ... */
  /* 'DefineFlightPath:70'   tel_struct, ... */
  /* 'DefineFlightPath:71'   iW, ... */
  /* 'DefineFlightPath:72'   guid_settings.do_plot); */
  sol = 0;
  FPazi = rt_atan2d_snf(MVelNED.y, MVelNED.x);
  latch[0] = TRACK->latch[0];
  latch[1] = TRACK->latch[1];
  MVelNED_0[0] = TRACK->Ut[0];
  MVelNED_0[1] = TRACK->Ut[1];
  MVelNED_0[2] = TRACK->Ut[2];
  GetDoubleArcVectors(ftime, opportunity_mode, &sol, &FPazi, psi, tgt0_y, tgt0_z,
                      MVelNED_0, Uv, TRACK->Kglide, OPT_FP_Mglide_opt,
                      OPT_FP_Mopt - ENV_mach, latch, TRACK->Rlim_lo, Rarc_T,
                      guid_settings_psi_Engage, MP_in_WeaponType, AFmode,
                      TRACK->log_FP, tel_struct, &telem->IW, &V_Rto, &V_Rte,
                      &V_Rwo, &V_Rwe, &V_Rmwo, &V_Rmto, &V_Rm, &V_RarcT,
                      &V_OarcT, &V_RarcW, &V_OarcW, Uw, &Lfpath2bunt,
                      &Lfpath2tgt, temp, &den, &Mglide);

  /* 'DefineFlightPath:75' telem.IW = iW; */
  /* 'DefineFlightPath:77' TRACK.solution = int8(sol); */
  TRACK->solution = sol;

  /* 'DefineFlightPath:78' TRACK.FPazi = FPazi; */
  TRACK->FPazi = FPazi;

  /* 'DefineFlightPath:79' TRACK.Lfpath2bunt = Lfpath2bunt; */
  TRACK->Lfpath2bunt = Lfpath2bunt;

  /* 'DefineFlightPath:80' TRACK.Lfpath2tgt = Lfpath2tgt; */
  TRACK->Lfpath2tgt = Lfpath2tgt;

  /* 'DefineFlightPath:81' TRACK.Ut = Ut; */
  TRACK->Ut[0] = temp[0];
  TRACK->Ut[1] = temp[1];
  TRACK->Ut[2] = temp[2];

  /* 'DefineFlightPath:83' psi = atan2(TPosNED.y-MPosNED.y,TPosNED.x-MPosNED.x); */
  psi = rt_atan2d_snf(TPosNED.y - MPosNED.y, TPosNED.x - MPosNED.x);

  /* 'DefineFlightPath:84' Ut0 = zeros(3,1); */
  /* 'DefineFlightPath:85' Ut0(1) =  TRACK.Ut0(1)*cos(psi) + TRACK.Ut0(2)*sin(psi); */
  tgt0_y = sin(psi);
  psi = cos(psi);
  Ut0[0] = (TRACK->Ut0[0] * psi) + (TRACK->Ut0[1] * tgt0_y);

  /* 'DefineFlightPath:86' Ut0(2) = -TRACK.Ut0(1)*sin(psi) + TRACK.Ut0(2)*cos(psi); */
  Ut0[1] = ((-TRACK->Ut0[0]) * tgt0_y) + (TRACK->Ut0[1] * psi);

  /* 'DefineFlightPath:87' Ut0(3) =  TRACK.Ut0(3); */
  Ut0[2] = TRACK->Ut0[2];

  /* 'DefineFlightPath:88' Ut(1)  =  TRACK.Ut(1)*cos(psi) + TRACK.Ut(2)*sin(psi); */
  Ut[0] = (temp[0] * psi) + (temp[1] * tgt0_y);

  /* 'DefineFlightPath:89' Ut(2)  = -TRACK.Ut(1)*sin(psi) + TRACK.Ut(2)*cos(psi); */
  Ut[1] = ((-temp[0]) * tgt0_y) + (temp[1] * psi);

  /* 'DefineFlightPath:90' Ut(3)  =  TRACK.Ut(3); */
  Ut[2] = temp[2];

  /* 'DefineFlightPath:98' temp = cross(Ut,Ut0); */
  cross_6Ajhxtbg(Ut, Ut0, temp);

  /* 'DefineFlightPath:99' temp(2) = LIB_limit(0.0,0.5,temp(2)); */
  /* 'LIB_limit:27' if (in > max) */
  if (temp[1] > 0.5) {
    /* 'LIB_limit:28' out = max; */
    psi = 0.5;
  } else if (temp[1] < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    psi = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    psi = temp[1];
  }

  /* 'DefineFlightPath:100' TRACK.AprAngErr = asin(temp(2)); */
  TRACK->AprAngErr = asin(psi);

  /* 'DefineFlightPath:102' TRACK.latch = latch; */
  TRACK->latch[0] = latch[0];
  TRACK->latch[1] = latch[1];

  /* 'DefineFlightPath:103' TRACK.ratio = ratio; */
  TRACK->ratio = den;

  /* 'DefineFlightPath:105' TRACK.Mglide = Mglide; */
  TRACK->Mglide = Mglide;

  /* 'DefineFlightPath:107' den = norm([Uv(1) Uv(2)])*norm([Uw(1) Uw(2)]); */
  Uv_0[0] = Uv[0];
  Uv_0[1] = Uv[1];
  Uw_0[0] = Uw[0];
  Uw_0[1] = Uw[1];
  den = norm_lpbhAiWU(Uv_0) * norm_lpbhAiWU(Uw_0);

  /* 'DefineFlightPath:108' if (den > 0.001) */
  if (den > 0.001) {
    /* 'DefineFlightPath:109' temp = dot([Uv(1) Uv(2)],[Uw(1) Uw(2)])/den; */
    Uv_0[0] = Uv[0];
    Uv_0[1] = Uv[1];
    Uw_0[0] = Uw[0];
    Uw_0[1] = Uw[1];
    den = dot_tXnwwRPM(Uv_0, Uw_0) / den;

    /* 'DefineFlightPath:110' if (temp > 0.999999) */
    if (den > 0.999999) {
      /* 'DefineFlightPath:111' temp = 0.999999; */
      den = 0.999999;
    }
  } else {
    /* 'DefineFlightPath:113' else */
    /* 'DefineFlightPath:114' temp = 0.999999; */
    den = 0.999999;
  }

  /* 'DefineFlightPath:116' psiR = acos(temp); */
  den = acos(den);

  /* 'DefineFlightPath:117' TRACK.psiR = psiR; */
  TRACK->psiR = den;

  /* 'DefineFlightPath:118' TRACK.rge2bunt = norm([V_Rwe.x V_Rwe.y V_Rwe.z]); */
  Ut0[0] = V_Rwe.x;
  Ut0[1] = V_Rwe.y;
  Ut0[2] = V_Rwe.z;
  TRACK->rge2bunt = norm_MfVYifVG(Ut0);

  /* 'DefineFlightPath:119' if (abs(psiR) > guid_settings.psiR_Thr || TRACK.once == 0 || TRACK.reset >= 1) */
  if ((fabs(den) > guid_settings_psiR_Thr) || ((((int32_T)TRACK->once) == 0) ||
       (((int32_T)TRACK->reset) >= 1))) {
    /* 'DefineFlightPath:120' if (TRACK.reset >= 1) */
    if (((int32_T)TRACK->reset) >= 1) {
      /* 'DefineFlightPath:121' TRACK.Rmis0   = [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]; */
      TRACK->Rmis0[0] = TRACK->mpos0.x;
      TRACK->Rmis0[1] = TRACK->mpos0.y;
      TRACK->Rmis0[2] = TRACK->mpos0.z;

      /* 'DefineFlightPath:122' TRACK.Umis0   = TRACK.Umis; */
      TRACK->Umis0[0] = TRACK->Umis[0];
      TRACK->Umis0[1] = TRACK->Umis[1];
      TRACK->Umis0[2] = TRACK->Umis[2];
    }

    /* 'DefineFlightPath:124' TRACK.mpos0.x = DCM.ECEF2NED(:,1)'*[ref0.x ref0.y ref0.z]' + TRACK.re0.x; */
    TRACK->mpos0.x = (((DCM->ECEF2NED[0] * Roff) + (DCM->ECEF2NED[1] * ref0_y))
                      + (DCM->ECEF2NED[2] * Sfac)) + TRACK->re0.x;

    /* 'DefineFlightPath:125' TRACK.mpos0.y = DCM.ECEF2NED(:,2)'*[ref0.x ref0.y ref0.z]' + TRACK.re0.y; */
    TRACK->mpos0.y = (((DCM->ECEF2NED[3] * Roff) + (DCM->ECEF2NED[4] * ref0_y))
                      + (DCM->ECEF2NED[5] * Sfac)) + TRACK->re0.y;

    /* 'DefineFlightPath:126' TRACK.mpos0.z = DCM.ECEF2NED(:,3)'*[ref0.x ref0.y ref0.z]' + TRACK.re0.z; */
    TRACK->mpos0.z = (((DCM->ECEF2NED[6] * Roff) + (DCM->ECEF2NED[7] * ref0_y))
                      + (DCM->ECEF2NED[8] * Sfac)) + TRACK->re0.z;

    /* 'DefineFlightPath:127' TRACK.Rto.x   = DCM.ECEF2NED(:,1)'*[V_Rto.x+ref0.x V_Rto.y+ref0.y V_Rto.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rto.x + Roff;
    Mglide = V_Rto.y + ref0_y;
    psi = V_Rto.z + Sfac;
    TRACK->Rto.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                    (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:128' TRACK.Rto.y   = DCM.ECEF2NED(:,2)'*[V_Rto.x+ref0.x V_Rto.y+ref0.y V_Rto.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rto.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                    (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:129' TRACK.Rto.z   = DCM.ECEF2NED(:,3)'*[V_Rto.x+ref0.x V_Rto.y+ref0.y V_Rto.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rto.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                    (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:130' TRACK.Rte.x   = DCM.ECEF2NED(:,1)'*[V_Rte.x+ref0.x V_Rte.y+ref0.y V_Rte.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rte.x + Roff;
    Mglide = V_Rte.y + ref0_y;
    psi = V_Rte.z + Sfac;
    TRACK->Rte.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                    (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:131' TRACK.Rte.y   = DCM.ECEF2NED(:,2)'*[V_Rte.x+ref0.x V_Rte.y+ref0.y V_Rte.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rte.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                    (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:132' TRACK.Rte.z   = DCM.ECEF2NED(:,3)'*[V_Rte.x+ref0.x V_Rte.y+ref0.y V_Rte.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rte.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                    (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:133' TRACK.Rwo.x   = DCM.ECEF2NED(:,1)'*[V_Rwo.x+ref0.x V_Rwo.y+ref0.y V_Rwo.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rwo.x + Roff;
    Mglide = V_Rwo.y + ref0_y;
    psi = V_Rwo.z + Sfac;
    TRACK->Rwo.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                    (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:134' TRACK.Rwo.y   = DCM.ECEF2NED(:,2)'*[V_Rwo.x+ref0.x V_Rwo.y+ref0.y V_Rwo.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rwo.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                    (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:135' TRACK.Rwo.z   = DCM.ECEF2NED(:,3)'*[V_Rwo.x+ref0.x V_Rwo.y+ref0.y V_Rwo.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rwo.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                    (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:136' TRACK.Rwe.x   = DCM.ECEF2NED(:,1)'*[V_Rwe.x+ref0.x V_Rwe.y+ref0.y V_Rwe.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rwe.x + Roff;
    Mglide = V_Rwe.y + ref0_y;
    psi = V_Rwe.z + Sfac;
    TRACK->Rwe.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                    (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:137' TRACK.Rwe.y   = DCM.ECEF2NED(:,2)'*[V_Rwe.x+ref0.x V_Rwe.y+ref0.y V_Rwe.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rwe.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                    (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:138' TRACK.Rwe.z   = DCM.ECEF2NED(:,3)'*[V_Rwe.x+ref0.x V_Rwe.y+ref0.y V_Rwe.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rwe.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                    (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:139' TRACK.Rmwo.x  = DCM.ECEF2NED(:,1)'*[V_Rmwo.x+ref0.x V_Rmwo.y+ref0.y V_Rmwo.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rmwo.x + Roff;
    Mglide = V_Rmwo.y + ref0_y;
    psi = V_Rmwo.z + Sfac;
    TRACK->Rmwo.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                     (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:140' TRACK.Rmwo.y  = DCM.ECEF2NED(:,2)'*[V_Rmwo.x+ref0.x V_Rmwo.y+ref0.y V_Rmwo.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rmwo.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                     (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:141' TRACK.Rmwo.z  = DCM.ECEF2NED(:,3)'*[V_Rmwo.x+ref0.x V_Rmwo.y+ref0.y V_Rmwo.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rmwo.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                     (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:142' TRACK.Rmto.x  = DCM.ECEF2NED(:,1)'*[V_Rmto.x+ref0.x V_Rmto.y+ref0.y V_Rmto.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rmto.x + Roff;
    Mglide = V_Rmto.y + ref0_y;
    psi = V_Rmto.z + Sfac;
    TRACK->Rmto.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                     (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:143' TRACK.Rmto.y  = DCM.ECEF2NED(:,2)'*[V_Rmto.x+ref0.x V_Rmto.y+ref0.y V_Rmto.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rmto.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                     (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:144' TRACK.Rmto.z  = DCM.ECEF2NED(:,3)'*[V_Rmto.x+ref0.x V_Rmto.y+ref0.y V_Rmto.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rmto.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                     (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:145' TRACK.Rm.x    = DCM.ECEF2NED(:,1)'*[V_Rm.x+ref0.x V_Rm.y+ref0.y V_Rm.z+ref0.z]' + TRACK.re0.x; */
    den = V_Rm.x + Roff;
    Mglide = V_Rm.y + ref0_y;
    psi = V_Rm.z + Sfac;
    TRACK->Rm.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                   (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:146' TRACK.Rm.y    = DCM.ECEF2NED(:,2)'*[V_Rm.x+ref0.x V_Rm.y+ref0.y V_Rm.z+ref0.z]' + TRACK.re0.y; */
    TRACK->Rm.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                   (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:147' TRACK.Rm.z    = DCM.ECEF2NED(:,3)'*[V_Rm.x+ref0.x V_Rm.y+ref0.y V_Rm.z+ref0.z]' + TRACK.re0.z; */
    TRACK->Rm.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                   (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:148' TRACK.RarcT   = V_RarcT; */
    TRACK->RarcT = V_RarcT;

    /* 'DefineFlightPath:149' TRACK.OarcT.x = DCM.ECEF2NED(:,1)'*[V_OarcT.x+ref0.x V_OarcT.y+ref0.y V_OarcT.z+ref0.z]' + TRACK.re0.x; */
    den = V_OarcT.x + Roff;
    Mglide = V_OarcT.y + ref0_y;
    psi = V_OarcT.z + Sfac;
    TRACK->OarcT.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                      (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:150' TRACK.OarcT.y = DCM.ECEF2NED(:,2)'*[V_OarcT.x+ref0.x V_OarcT.y+ref0.y V_OarcT.z+ref0.z]' + TRACK.re0.y; */
    TRACK->OarcT.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                      (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:151' TRACK.OarcT.z = DCM.ECEF2NED(:,3)'*[V_OarcT.x+ref0.x V_OarcT.y+ref0.y V_OarcT.z+ref0.z]' + TRACK.re0.z; */
    TRACK->OarcT.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                      (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:152' TRACK.RarcW   = V_RarcW; */
    TRACK->RarcW = V_RarcW;

    /* 'DefineFlightPath:153' TRACK.OarcW.x = DCM.ECEF2NED(:,1)'*[V_OarcW.x+ref0.x V_OarcW.y+ref0.y V_OarcW.z+ref0.z]' + TRACK.re0.x; */
    den = V_OarcW.x + Roff;
    Mglide = V_OarcW.y + ref0_y;
    psi = V_OarcW.z + Sfac;
    TRACK->OarcW.x = (((den * DCM->ECEF2NED[0]) + (Mglide * DCM->ECEF2NED[1])) +
                      (psi * DCM->ECEF2NED[2])) + TRACK->re0.x;

    /* 'DefineFlightPath:154' TRACK.OarcW.y = DCM.ECEF2NED(:,2)'*[V_OarcW.x+ref0.x V_OarcW.y+ref0.y V_OarcW.z+ref0.z]' + TRACK.re0.y; */
    TRACK->OarcW.y = (((den * DCM->ECEF2NED[3]) + (Mglide * DCM->ECEF2NED[4])) +
                      (psi * DCM->ECEF2NED[5])) + TRACK->re0.y;

    /* 'DefineFlightPath:155' TRACK.OarcW.z = DCM.ECEF2NED(:,3)'*[V_OarcW.x+ref0.x V_OarcW.y+ref0.y V_OarcW.z+ref0.z]' + TRACK.re0.z; */
    TRACK->OarcW.z = (((den * DCM->ECEF2NED[6]) + (Mglide * DCM->ECEF2NED[7])) +
                      (psi * DCM->ECEF2NED[8])) + TRACK->re0.z;

    /* 'DefineFlightPath:157' TRACK.Umis    = LIB_unitvector([TRACK.Rwe.x TRACK.Rwe.y TRACK.Rwe.z]-[TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]); */
    MVelNED_0[0] = TRACK->Rwe.x - TRACK->mpos0.x;
    MVelNED_0[1] = TRACK->Rwe.y - TRACK->mpos0.y;
    MVelNED_0[2] = TRACK->Rwe.z - TRACK->mpos0.z;
    LIB_unitvector_gdXEDyOJ(MVelNED_0, TRACK->Umis);

    /* 'DefineFlightPath:158' if (TRACK.once == 0) */
    if (((int32_T)TRACK->once) == 0) {
      /* 'DefineFlightPath:159' TRACK.Rmis0 = [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]; */
      TRACK->Rmis0[0] = TRACK->mpos0.x;
      TRACK->Rmis0[1] = TRACK->mpos0.y;
      TRACK->Rmis0[2] = TRACK->mpos0.z;

      /* 'DefineFlightPath:160' TRACK.Umis0 = Uv; */
      TRACK->Umis0[0] = Uv[0];
      TRACK->Umis0[1] = Uv[1];
      TRACK->Umis0[2] = Uv[2];
    }

    /* 'DefineFlightPath:163' TRACK.once = uint8(1); */
    TRACK->once = 1U;
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [TRACK,clos,gains,RrefL,RrefC] = GetCLOSParams(guidmode,guidcmds,ftime,gains,clos,TRACK,ENV,OPT_FP,DCM,TTKF_in,NAVin,tgt0,AFmode,AFtype,guid_settings)
 */
static void GetCLOSParams(uint8_T guidmode, uint8_T guidcmds_gmode_sw, uint8_T
  guidcmds_PurePropNav, real_T guidcmds_GndRge, real_T ftime,
  s28FIl9ilY9eNQKo2dEzV1C *gains, ssOdl51LB8ZQlaB7SirUWkE *clos,
  sFYrLWvx1kVFonG2Nu0GNkH *TRACK, real_T ENV_FPthem, real_T ENV_FPpsim, real_T
  ENV_Vairspeed, real_T ENV_Vnav, real_T ENV_Q, real_T OPT_FP_Qopt, real_T
  OPT_FP_Mglide_opt, const sPLFBkBfmjj013qOjLWm7GC *DCM, const
  T_GUID_TTKF_Out_IF1002 *TTKF_in, const sb6O280JGWZ46sxhPtS13aC *NAVin, const
  T_Cartesian tgt0, uint8_T AFmode, real_T RrefL[3], real_T RrefC[3])
{
  real_T Rmis0[3];
  real_T Rwea[3];
  real_T RGEarcE;
  real_T Ufpath[3];
  real_T RGEwea;
  real_T KQ;
  real_T Rclos_ecef[3];
  real_T Acori_b_data[9];
  int32_T transition;
  real_T Rref[3];
  real_T UrefL[3];
  real_T UrefC[3];
  real_T corvec[3];
  real_T DCM0fpath[9];
  real_T sumRef_b0[3];
  real_T corvec_tmp[9];
  real_T tmp[9];
  real_T corvec_0[2];
  real_T sumRef_b0_0[2];
  real_T clos_0[3];
  real_T NAVin_0[3];
  real_T Rmis0_0;
  real_T Rclos_b_idx_1;
  real_T Rclos_b_idx_2;
  real_T Vclos_b_idx_1;
  real_T Vclos_b_idx_2;
  real_T coracc_ecef_idx_0;
  real_T coracc_ecef_idx_1;
  real_T coracc_ecef_idx_2;
  real_T ENV_Vnav_idx_2;
  real_T Rwea_tmp;

  /* 'GUID_Embedded_Guid_Control_1D:2840' Rmis0 = [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]; */
  /* 'GUID_Embedded_Guid_Control_1D:2841' Rwea  = [NAVin.pos_e.x NAVin.pos_e.y NAVin.pos_e.z]; */
  Rwea[0] = NAVin->pos_e.x;
  Rwea[1] = NAVin->pos_e.y;
  Rwea[2] = NAVin->pos_e.z;

  /* 'GUID_Embedded_Guid_Control_1D:2842' RrefL = Rwea; */
  /* 'GUID_Embedded_Guid_Control_1D:2843' RrefC = Rwea; */
  /* 'GUID_Embedded_Guid_Control_1D:2846' RGEarcE = norm([clos.ArcState.Rapr_T(1) clos.ArcState.Rapr_T(2) clos.ArcState.Rapr_T(3)] - [TRACK.mpos0.x TRACK.mpos0.y TRACK.mpos0.z]); */
  RrefL[0] = NAVin->pos_e.x;
  RrefC[0] = NAVin->pos_e.x;
  sumRef_b0[0] = clos->ArcState.Rapr_T[0] - TRACK->mpos0.x;
  RrefL[1] = NAVin->pos_e.y;
  RrefC[1] = NAVin->pos_e.y;
  sumRef_b0[1] = clos->ArcState.Rapr_T[1] - TRACK->mpos0.y;
  RrefL[2] = NAVin->pos_e.z;
  RrefC[2] = NAVin->pos_e.z;
  sumRef_b0[2] = clos->ArcState.Rapr_T[2] - TRACK->mpos0.z;

  /* 'GUID_Embedded_Guid_Control_1D:2847' Ufpath  = LIB_unitvector([NAVin.vel_e.x NAVin.vel_e.y NAVin.vel_e.z]); */
  NAVin_0[0] = NAVin->vel_e.x;
  NAVin_0[1] = NAVin->vel_e.y;
  NAVin_0[2] = NAVin->vel_e.z;
  LIB_unitvector_gdXEDyOJ(NAVin_0, Ufpath);

  /* 'GUID_Embedded_Guid_Control_1D:2848' RGEwea  = norm(Rwea - Rmis0); */
  NAVin_0[0] = NAVin->pos_e.x - TRACK->mpos0.x;
  NAVin_0[1] = NAVin->pos_e.y - TRACK->mpos0.y;
  NAVin_0[2] = NAVin->pos_e.z - TRACK->mpos0.z;
  RGEwea = norm_MfVYifVG(NAVin_0);

  /* 'GUID_Embedded_Guid_Control_1D:2849' RrefL0  = Rmis0 + RGEwea*TRACK.Umis; */
  Rmis0[0] = (RGEwea * TRACK->Umis[0]) + TRACK->mpos0.x;
  Rmis0[1] = (RGEwea * TRACK->Umis[1]) + TRACK->mpos0.y;
  Rmis0_0 = (RGEwea * TRACK->Umis[2]) + TRACK->mpos0.z;
  Rmis0[2] = Rmis0_0;

  /* 'GUID_Embedded_Guid_Control_1D:2854' if (AFmode == 2 || TTKF_in.tgtrge <= 0.0) */
  if ((((int32_T)AFmode) == 2) || (TTKF_in->tgtrge <= 0.0)) {
    /* 'GUID_Embedded_Guid_Control_1D:2857' KQ = 1.0; */
    KQ = 1.0;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2858' else */
    /* 'GUID_Embedded_Guid_Control_1D:2861' KQ = LIB_limit(1.0,1.667,5000.0/TTKF_in.tgtrge); */
    KQ = 5000.0 / TTKF_in->tgtrge;

    /* 'LIB_limit:27' if (in > max) */
    if (KQ > 1.667) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.667;
    } else if (KQ < 1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:2867' gains.Kp = KQ*0.30; */
 // gains->Kp = KQ * 0.3;
   gains->Kp = KQ * 0.14;

  /* 'GUID_Embedded_Guid_Control_1D:2868' gains.Kv = 1.20; */
  gains->Kv = 1.2;
   

  /* 'GUID_Embedded_Guid_Control_1D:2869' Rclos_ecef = [0 0 0]; */
  /* 'GUID_Embedded_Guid_Control_1D:2870' Rclos_b = [0 0 0]'; */
  /* 'GUID_Embedded_Guid_Control_1D:2871' Vclos_b = [0 0 0]; */
  /* 'GUID_Embedded_Guid_Control_1D:2872' Acori_b = [0 0 0]; */
  Rclos_ecef[0] = 0.0;
  Rclos_ecef[1] = 0.0;
  Rclos_b_idx_1 = 0.0;
  Vclos_b_idx_1 = 0.0;
  Acori_b_data[1] = 0.0;
  Rclos_ecef[2] = 0.0;
  Rclos_b_idx_2 = 0.0;
  Vclos_b_idx_2 = 0.0;
  Acori_b_data[2] = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:2881' if (ftime > 43.00) */
  /* 'GUID_Embedded_Guid_Control_1D:2885' transition = 0; */
  transition = 0;

  /* 'GUID_Embedded_Guid_Control_1D:2887' if (guidmode <= 3) */
  if (((int32_T)guidmode) <= 3) {
    /* 'GUID_Embedded_Guid_Control_1D:2891' Rref = Rwea; */
    /* 'GUID_Embedded_Guid_Control_1D:2892' Uref = Ufpath; */
    /* 'GUID_Embedded_Guid_Control_1D:2893' UrefL = Ufpath; */
    /* 'GUID_Embedded_Guid_Control_1D:2894' clos.AZcori = 0.0; */
    clos->AZcori = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2896' coracc_ecef = [0.0 0.0 0.0]; */
    Rref[0] = NAVin->pos_e.x;
    UrefC[0] = Ufpath[0];
    UrefL[0] = Ufpath[0];
    coracc_ecef_idx_0 = 0.0;
    Rref[1] = NAVin->pos_e.y;
    UrefC[1] = Ufpath[1];
    UrefL[1] = Ufpath[1];
    coracc_ecef_idx_1 = 0.0;
    Rref[2] = NAVin->pos_e.z;
    UrefC[2] = Ufpath[2];
    UrefL[2] = Ufpath[2];
    coracc_ecef_idx_2 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:2898' TRACK.AZrefL = 1.0; */
    TRACK->AZrefL = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2899' TRACK.AZref = 1.0; */
    TRACK->AZref = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:2901' clos.ArcState.Rapr_T(1) = TTKF_in.tgtrex; */
    clos->ArcState.Rapr_T[0] = TTKF_in->tgtrex;

    /* 'GUID_Embedded_Guid_Control_1D:2902' clos.ArcState.Rapr_T(2) = TTKF_in.tgtrey; */
    clos->ArcState.Rapr_T[1] = TTKF_in->tgtrey;

    /* 'GUID_Embedded_Guid_Control_1D:2903' clos.ArcState.Rapr_T(3) = TTKF_in.tgtrez; */
    clos->ArcState.Rapr_T[2] = TTKF_in->tgtrez;

    /* 'GUID_Embedded_Guid_Control_1D:2904' clos.RWE = [TRACK.Rwe.x TRACK.Rwe.y TRACK.Rwe.z]; */
    clos->RWE[0] = TRACK->Rwe.x;
    clos->RWE[1] = TRACK->Rwe.y;
    clos->RWE[2] = TRACK->Rwe.z;

    /* 'GUID_Embedded_Guid_Control_1D:2905' clos.RMW0 = [TRACK.Rmwo.x TRACK.Rmwo.y TRACK.Rmwo.z]; */
    clos->RMW0[0] = TRACK->Rmwo.x;
    clos->RMW0[1] = TRACK->Rmwo.y;
    clos->RMW0[2] = TRACK->Rmwo.z;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:2908' else */
    /* 'GUID_Embedded_Guid_Control_1D:2909' if (guidmode == 8) */
    if (((int32_T)guidmode) == 8) {
      /* 'GUID_Embedded_Guid_Control_1D:2910' clos.fadeFac = 0.0; */
      clos->fadeFac = 0.0;
    }

    /* 'GUID_Embedded_Guid_Control_1D:2913' if (TRACK.reset && TRACK.AZrefL >= 0.0) */
    if ((((int32_T)TRACK->reset) != 0) && (TRACK->AZrefL >= 0.0)) {
      /* 'GUID_Embedded_Guid_Control_1D:2915' AZrefLim = 0.1*(10.0 - (ftime - TRACK.Time8)); */
      KQ = (10.0 - (ftime - TRACK->Time8)) * 0.1;

      /* 'GUID_Embedded_Guid_Control_1D:2916' TRACK.AZrefL = LIB_limit(0.001,1.0,AZrefLim); */
      /* 'LIB_limit:27' if (in > max) */
      if (KQ > 1.0) {
        /* 'LIB_limit:28' out = max; */
        TRACK->AZrefL = 1.0;
      } else if (KQ < 0.001) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TRACK->AZrefL = 0.001;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TRACK->AZrefL = KQ;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2917' else */
      /* 'GUID_Embedded_Guid_Control_1D:2918' AZrefL = TRACK.AZrefL; */
      /* 'GUID_Embedded_Guid_Control_1D:2919' TRACK.AZrefL = LIB_limit(0.001,1.0,AZrefL-0.0025-clos.fadeFac*0.0025); */
      KQ = (TRACK->AZrefL - 0.0025) - (clos->fadeFac * 0.0025);

      /* 'LIB_limit:27' if (in > max) */
      if (KQ > 1.0) {
        /* 'LIB_limit:28' out = max; */
        TRACK->AZrefL = 1.0;
      } else if (KQ < 0.001) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TRACK->AZrefL = 0.001;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TRACK->AZrefL = KQ;
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:2922' if (guidmode < 9 && guidcmds.gmode_sw) */
    if ((((int32_T)guidmode) < 9) && (((int32_T)guidcmds_gmode_sw) != 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:2923' transition = 1; */
      transition = 1;

      /* 'GUID_Embedded_Guid_Control_1D:2924' if (guidmode == 8) */
      if (((int32_T)guidmode) == 8) {
        /* 'GUID_Embedded_Guid_Control_1D:2926' TRACK.AZrefL = 0.0; */
        TRACK->AZrefL = 0.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:2928' if (TRACK.reset == 3) */
      if (((int32_T)TRACK->reset) == 3) {
        /* 'GUID_Embedded_Guid_Control_1D:2929' TRACK.AZref  = 1.0; */
        TRACK->AZref = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:2930' TRACK.AZrefL = 0.0; */
        TRACK->AZrefL = 0.0;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:2931' else */
        /* 'GUID_Embedded_Guid_Control_1D:2932' TRACK.AZref  = 1.0; */
        TRACK->AZref = 1.0;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:2934' else */
      /* 'GUID_Embedded_Guid_Control_1D:2935' AZref = TRACK.AZref; */
      /* 'GUID_Embedded_Guid_Control_1D:2936' TRACK.AZref = LIB_limit(0.0,1.0,AZref-0.0025-clos.fadeFac*0.0075); */
      KQ = (TRACK->AZref - 0.0025) - (clos->fadeFac * 0.0075);

      /* 'LIB_limit:27' if (in > max) */
      if (KQ > 1.0) {
        /* 'LIB_limit:28' out = max; */
        TRACK->AZref = 1.0;
      } else if (KQ < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TRACK->AZref = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TRACK->AZref = KQ;
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:2940' RGEwea0  = norm(Rwea - TRACK.Rmis0); */
    NAVin_0[0] = NAVin->pos_e.x - TRACK->Rmis0[0];
    NAVin_0[1] = NAVin->pos_e.y - TRACK->Rmis0[1];
    NAVin_0[2] = NAVin->pos_e.z - TRACK->Rmis0[2];
    KQ = norm_MfVYifVG(NAVin_0);

    /* 'GUID_Embedded_Guid_Control_1D:2941' RrefL = TRACK.AZrefL*(TRACK.Rmis0 + RGEwea0*TRACK.Umis0) + (1.0-TRACK.AZrefL)*RrefL0; */
    /* 'GUID_Embedded_Guid_Control_1D:2944' UrefL = TRACK.AZrefL*TRACK.Umis0 + (1.0-TRACK.AZrefL)*TRACK.Umis; */
    RrefL[0] = (((KQ * TRACK->Umis0[0]) + TRACK->Rmis0[0]) * TRACK->AZrefL) +
      ((1.0 - TRACK->AZrefL) * Rmis0[0]);
    UrefL[0] = ((1.0 - TRACK->AZrefL) * TRACK->Umis[0]) + (TRACK->AZrefL *
      TRACK->Umis0[0]);
    RrefL[1] = (((KQ * TRACK->Umis0[1]) + TRACK->Rmis0[1]) * TRACK->AZrefL) +
      ((1.0 - TRACK->AZrefL) * Rmis0[1]);
    UrefL[1] = ((1.0 - TRACK->AZrefL) * TRACK->Umis[1]) + (TRACK->AZrefL *
      TRACK->Umis0[1]);
    RrefL[2] = (((KQ * TRACK->Umis0[2]) + TRACK->Rmis0[2]) * TRACK->AZrefL) +
      ((1.0 - TRACK->AZrefL) * Rmis0_0);
    UrefL[2] = ((1.0 - TRACK->AZrefL) * TRACK->Umis[2]) + (TRACK->AZrefL *
      TRACK->Umis0[2]);

    /* 'GUID_Embedded_Guid_Control_1D:2947' if (guidmode == 8 && (guidcmds.gmode_sw || ~clos.once)) */
    if ((((int32_T)guidmode) == 8) && ((((int32_T)guidcmds_gmode_sw) != 0) ||
         (((int32_T)clos->once) == 0))) {
      /* 'GUID_Embedded_Guid_Control_1D:2949' clos.once = uint8(1); */
      clos->once = 1U;

      /* 'GUID_Embedded_Guid_Control_1D:2951' clos.RM   = [TRACK.Rm.x TRACK.Rm.y TRACK.Rm.z]; */
      clos->RM[0] = TRACK->Rm.x;
      clos->RM[1] = TRACK->Rm.y;
      clos->RM[2] = TRACK->Rm.z;

      /* 'GUID_Embedded_Guid_Control_1D:2952' clos.RWE  = [TRACK.Rwe.x TRACK.Rwe.y TRACK.Rwe.z]; */
      clos->RWE[0] = TRACK->Rwe.x;
      clos->RWE[1] = TRACK->Rwe.y;
      clos->RWE[2] = TRACK->Rwe.z;

      /* 'GUID_Embedded_Guid_Control_1D:2953' clos.RMW0 = [TRACK.Rmwo.x TRACK.Rmwo.y TRACK.Rmwo.z]; */
      clos->RMW0[0] = TRACK->Rmwo.x;
      clos->RMW0[1] = TRACK->Rmwo.y;
      clos->RMW0[2] = TRACK->Rmwo.z;

      /* 'GUID_Embedded_Guid_Control_1D:2956' RT0 = [TRACK.Rto.x TRACK.Rto.y TRACK.Rto.z]; */
      /* 'GUID_Embedded_Guid_Control_1D:2957' RW0 = [TRACK.Rwo.x TRACK.Rwo.y TRACK.Rwo.z]; */
      /* 'GUID_Embedded_Guid_Control_1D:2963' clos.ArcState.Rapr_T(1) = TRACK.Rte.x; */
      clos->ArcState.Rapr_T[0] = TRACK->Rte.x;

      /* 'GUID_Embedded_Guid_Control_1D:2964' clos.ArcState.Rapr_T(2) = TRACK.Rte.y; */
      clos->ArcState.Rapr_T[1] = TRACK->Rte.y;

      /* 'GUID_Embedded_Guid_Control_1D:2965' clos.ArcState.Rapr_T(3) = TRACK.Rte.z; */
      clos->ArcState.Rapr_T[2] = TRACK->Rte.z;

      /* 'GUID_Embedded_Guid_Control_1D:2966' clos.ArcState.Rarc_T    = TRACK.RarcT; */
      clos->ArcState.Rarc_T = TRACK->RarcT;

      /* 'GUID_Embedded_Guid_Control_1D:2967' clos.ArcState.Oarc_T(1) = TRACK.OarcT.x; */
      clos->ArcState.Oarc_T[0] = TRACK->OarcT.x;

      /* 'GUID_Embedded_Guid_Control_1D:2968' clos.ArcState.Oarc_T(2) = TRACK.OarcT.y; */
      clos->ArcState.Oarc_T[1] = TRACK->OarcT.y;

      /* 'GUID_Embedded_Guid_Control_1D:2969' clos.ArcState.Oarc_T(3) = TRACK.OarcT.z; */
      clos->ArcState.Oarc_T[2] = TRACK->OarcT.z;

      /* 'GUID_Embedded_Guid_Control_1D:2970' if (clos.ArcState.Rarc_T < 1.0) */
      if (TRACK->RarcT < 1.0) {
        /* 'GUID_Embedded_Guid_Control_1D:2971' clos.ArcState.Rarc_T = 1.0; */
        clos->ArcState.Rarc_T = 1.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:2977' clos.ArcState.Rapr_Rm(1) = TRACK.Rm.x; */
      clos->ArcState.Rapr_Rm[0] = TRACK->Rm.x;

      /* 'GUID_Embedded_Guid_Control_1D:2978' clos.ArcState.Rapr_Rm(2) = TRACK.Rm.y; */
      clos->ArcState.Rapr_Rm[1] = TRACK->Rm.y;

      /* 'GUID_Embedded_Guid_Control_1D:2979' clos.ArcState.Rapr_Rm(3) = TRACK.Rm.z; */
      clos->ArcState.Rapr_Rm[2] = TRACK->Rm.z;

      /* 'GUID_Embedded_Guid_Control_1D:2980' clos.ArcState.Rarc_W     = TRACK.RarcW; */
      clos->ArcState.Rarc_W = TRACK->RarcW;

      /* 'GUID_Embedded_Guid_Control_1D:2981' clos.ArcState.Oarc_W(1)  = TRACK.OarcW.x; */
      clos->ArcState.Oarc_W[0] = TRACK->OarcW.x;

      /* 'GUID_Embedded_Guid_Control_1D:2982' clos.ArcState.Oarc_W(2)  = TRACK.OarcW.y; */
      clos->ArcState.Oarc_W[1] = TRACK->OarcW.y;

      /* 'GUID_Embedded_Guid_Control_1D:2983' clos.ArcState.Oarc_W(3)  = TRACK.OarcW.z; */
      clos->ArcState.Oarc_W[2] = TRACK->OarcW.z;

      /* 'GUID_Embedded_Guid_Control_1D:2984' if (clos.ArcState.Rarc_W < 1.0) */
      if (TRACK->RarcW < 1.0) {
        /* 'GUID_Embedded_Guid_Control_1D:2985' clos.ArcState.Rarc_W = 1.0; */
        clos->ArcState.Rarc_W = 1.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:2989' TRACK.Time8 = ftime; */
      TRACK->Time8 = ftime;
    } else {
      if (((int32_T)guidmode) != 8) {
        /* 'GUID_Embedded_Guid_Control_1D:2992' elseif (guidmode ~= 8) */
        /* 'GUID_Embedded_Guid_Control_1D:2994' TRACK.Time8 = ftime; */
        TRACK->Time8 = ftime;
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:2999' coracc_ecef = [0.0 0.0 0.0]; */
    coracc_ecef_idx_0 = 0.0;
    coracc_ecef_idx_1 = 0.0;
    coracc_ecef_idx_2 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3000' if (clos.once) */
    if (((int32_T)clos->once) != 0) {
      /* 'GUID_Embedded_Guid_Control_1D:3001' if (dot(clos.RM-clos.RMW0,Rwea-clos.RM) >= 0.0) */
      coracc_ecef_idx_0 = clos->RM[0] - clos->RMW0[0];
      clos_0[0] = coracc_ecef_idx_0;
      Rwea_tmp = NAVin->pos_e.x - clos->RM[0];
      NAVin_0[0] = Rwea_tmp;
      coracc_ecef_idx_1 = clos->RM[1] - clos->RMW0[1];
      clos_0[1] = coracc_ecef_idx_1;
      coracc_ecef_idx_2 = NAVin->pos_e.y - clos->RM[1];
      NAVin_0[1] = coracc_ecef_idx_2;
      RGEarcE = clos->RM[2] - clos->RMW0[2];
      clos_0[2] = RGEarcE;
      KQ = NAVin->pos_e.z - clos->RM[2];
      NAVin_0[2] = KQ;
      if (dot_lFwBkxue(clos_0, NAVin_0) >= 0.0) {
        /* 'GUID_Embedded_Guid_Control_1D:3003' [RrefC,UrefC] = LIB_getrefposarc(Rwea,clos.RWE,clos.ArcState.Rapr_Rm,clos.ArcState.Oarc_W); */
        NAVin_0[0] = NAVin->pos_e.x;
        sumRef_b0[0] = clos->RWE[0];
        clos_0[0] = clos->ArcState.Rapr_Rm[0];
        NAVin_0[1] = NAVin->pos_e.y;
        sumRef_b0[1] = clos->RWE[1];
        clos_0[1] = clos->ArcState.Rapr_Rm[1];
        NAVin_0[2] = NAVin->pos_e.z;
        sumRef_b0[2] = clos->RWE[2];
        clos_0[2] = clos->ArcState.Rapr_Rm[2];
        LIB_getrefposarc_0TuejMUx(NAVin_0, sumRef_b0, clos_0,
          clos->ArcState.Oarc_W, RrefC, UrefC);

        /* 'GUID_Embedded_Guid_Control_1D:3004' if (clos.ArcState.Rarc_W > 0.0) */
        if (clos->ArcState.Rarc_W > 0.0) {
          /* 'GUID_Embedded_Guid_Control_1D:3005' cormag = (ENV.Vairspeed*ENV.Vairspeed)/clos.ArcState.Rarc_W; */
          RGEarcE = (ENV_Vairspeed * ENV_Vairspeed) / clos->ArcState.Rarc_W;
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3006' else */
          /* 'GUID_Embedded_Guid_Control_1D:3007' cormag = 0.0; */
          RGEarcE = 0.0;
        }

        /* 'GUID_Embedded_Guid_Control_1D:3009' corvec = RrefC - clos.ArcState.Oarc_W; */
        corvec[0] = RrefC[0] - clos->ArcState.Oarc_W[0];
        corvec[1] = RrefC[1] - clos->ArcState.Oarc_W[1];
        corvec[2] = RrefC[2] - clos->ArcState.Oarc_W[2];
      } else {
        clos_0[0] = coracc_ecef_idx_0;
        NAVin_0[0] = Rwea_tmp;
        clos_0[1] = coracc_ecef_idx_1;
        NAVin_0[1] = coracc_ecef_idx_2;
        clos_0[2] = RGEarcE;
        NAVin_0[2] = KQ;
        if ((dot_lFwBkxue(clos_0, NAVin_0) < 0.0) && (RGEwea < norm_MfVYifVG
             (sumRef_b0))) {
          /* 'GUID_Embedded_Guid_Control_1D:3010' elseif (dot(clos.RM-clos.RMW0,Rwea-clos.RM) < 0.0 && RGEwea < RGEarcE) */
          /* 'GUID_Embedded_Guid_Control_1D:3012' [RrefC,UrefC] = LIB_getrefposarc(Rwea,clos.RM,clos.ArcState.Rapr_T,clos.ArcState.Oarc_T); */
          NAVin_0[0] = NAVin->pos_e.x;
          sumRef_b0[0] = clos->RM[0];
          clos_0[0] = clos->ArcState.Rapr_T[0];
          NAVin_0[1] = NAVin->pos_e.y;
          sumRef_b0[1] = clos->RM[1];
          clos_0[1] = clos->ArcState.Rapr_T[1];
          NAVin_0[2] = NAVin->pos_e.z;
          sumRef_b0[2] = clos->RM[2];
          clos_0[2] = clos->ArcState.Rapr_T[2];
          LIB_getrefposarc_0TuejMUx(NAVin_0, sumRef_b0, clos_0,
            clos->ArcState.Oarc_T, RrefC, UrefC);

          /* 'GUID_Embedded_Guid_Control_1D:3013' if (clos.ArcState.Rarc_T > 0.0) */
          if (clos->ArcState.Rarc_T > 0.0) {
            /* 'GUID_Embedded_Guid_Control_1D:3014' cormag = (ENV.Vairspeed*ENV.Vairspeed)/clos.ArcState.Rarc_T; */
            RGEarcE = (ENV_Vairspeed * ENV_Vairspeed) / clos->ArcState.Rarc_T;
          } else {
            /* 'GUID_Embedded_Guid_Control_1D:3015' else */
            /* 'GUID_Embedded_Guid_Control_1D:3016' cormag = 0.0; */
            RGEarcE = 0.0;
          }

          /* 'GUID_Embedded_Guid_Control_1D:3018' corvec = RrefC - clos.ArcState.Oarc_T; */
          corvec[0] = RrefC[0] - clos->ArcState.Oarc_T[0];
          corvec[1] = RrefC[1] - clos->ArcState.Oarc_T[1];
          corvec[2] = RrefC[2] - clos->ArcState.Oarc_T[2];
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3019' else */
          /* 'GUID_Embedded_Guid_Control_1D:3021' RGE2Tgt = norm([tgt0.x tgt0.y tgt0.z]); */
          UrefC[0] = tgt0.x;
          UrefC[1] = tgt0.y;
          UrefC[2] = tgt0.z;
          RGEarcE = norm_MfVYifVG(UrefC);

          /* 'GUID_Embedded_Guid_Control_1D:3022' Rtgt2apr = clos.ArcState.Rapr_T-[TTKF_in.tgtrex TTKF_in.tgtrey TTKF_in.tgtrez]; */
          /* 'GUID_Embedded_Guid_Control_1D:3023' RrefC = [TTKF_in.tgtrex TTKF_in.tgtrey TTKF_in.tgtrez] + RGE2Tgt*LIB_unitvector(Rtgt2apr); */
          sumRef_b0[0] = clos->ArcState.Rapr_T[0] - TTKF_in->tgtrex;
          sumRef_b0[1] = clos->ArcState.Rapr_T[1] - TTKF_in->tgtrey;
          sumRef_b0[2] = clos->ArcState.Rapr_T[2] - TTKF_in->tgtrez;
          LIB_unitvector_gdXEDyOJ(sumRef_b0, NAVin_0);
          RrefC[0] = (RGEarcE * NAVin_0[0]) + TTKF_in->tgtrex;
          RrefC[1] = (RGEarcE * NAVin_0[1]) + TTKF_in->tgtrey;
          RrefC[2] = (RGEarcE * NAVin_0[2]) + TTKF_in->tgtrez;

          /* 'GUID_Embedded_Guid_Control_1D:3024' UrefC = LIB_unitvector([TTKF_in.tgtrex TTKF_in.tgtrey TTKF_in.tgtrez]-RrefC); */
          sumRef_b0[0] = TTKF_in->tgtrex - RrefC[0];
          corvec[0] = 0.0;
          sumRef_b0[1] = TTKF_in->tgtrey - RrefC[1];
          corvec[1] = 0.0;
          sumRef_b0[2] = TTKF_in->tgtrez - RrefC[2];
          corvec[2] = 0.0;
          LIB_unitvector_gdXEDyOJ(sumRef_b0, UrefC);

          /* 'GUID_Embedded_Guid_Control_1D:3025' cormag = 0.0; */
          RGEarcE = 0.0;

          /* 'GUID_Embedded_Guid_Control_1D:3026' corvec = [0.0 0.0 0.0]; */
        }
      }

      /* 'GUID_Embedded_Guid_Control_1D:3028' if (abs(corvec(1)) < 1.0) */
      if (fabs(corvec[0]) < 1.0) {
        /* 'GUID_Embedded_Guid_Control_1D:3029' corvec(1) = 1.0; */
        corvec[0] = 1.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:3032' Ucor   = LIB_unitvector(corvec); */
      /* 'GUID_Embedded_Guid_Control_1D:3033' coracc_ecef = (1.0-TRACK.AZref)*cormag*Ucor; */
      RGEarcE *= 1.0 - TRACK->AZref;
      LIB_unitvector_gdXEDyOJ(corvec, NAVin_0);
      coracc_ecef_idx_0 = RGEarcE * NAVin_0[0];
      coracc_ecef_idx_1 = RGEarcE * NAVin_0[1];
      coracc_ecef_idx_2 = RGEarcE * NAVin_0[2];
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3035' else */
      /* 'GUID_Embedded_Guid_Control_1D:3036' RrefC = RrefL; */
      /* 'GUID_Embedded_Guid_Control_1D:3037' UrefC = UrefL; */
      RrefC[0] = RrefL[0];
      UrefC[0] = UrefL[0];
      RrefC[1] = RrefL[1];
      UrefC[1] = UrefL[1];
      RrefC[2] = RrefL[2];
      UrefC[2] = UrefL[2];
    }

    /* 'GUID_Embedded_Guid_Control_1D:3040' if (guidmode >= 8) */
    if (((int32_T)guidmode) >= 8) {
      /* 'GUID_Embedded_Guid_Control_1D:3042' if (transition) */
      if (transition != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:3043' clos.Rrel = clos.Rref - RrefL; */
        clos->Rrel[0] = clos->Rref[0] - RrefL[0];
        clos->Rrel[1] = clos->Rref[1] - RrefL[1];
        clos->Rrel[2] = clos->Rref[2] - RrefL[2];
      }

      /* 'GUID_Embedded_Guid_Control_1D:3045' clos.AZcori = LIB_limit(0.0,1.0,clos.AZcori+0.020); */
      /* 'LIB_limit:27' if (in > max) */
      if ((clos->AZcori + 0.02) > 1.0) {
        /* 'LIB_limit:28' out = max; */
        RGEarcE = 1.0;
      } else if ((clos->AZcori + 0.02) < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RGEarcE = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        RGEarcE = clos->AZcori + 0.02;
      }

      clos->AZcori = RGEarcE;

      /* 'GUID_Embedded_Guid_Control_1D:3047' Rref = (1.0-TRACK.AZref)*RrefC + TRACK.AZref*(RrefL+clos.Rrel); */
      Rref[0] = ((1.0 - TRACK->AZref) * RrefC[0]) + ((RrefL[0] + clos->Rrel[0]) *
        TRACK->AZref);
      Rref[1] = ((1.0 - TRACK->AZref) * RrefC[1]) + ((RrefL[1] + clos->Rrel[1]) *
        TRACK->AZref);
      Rref[2] = ((1.0 - TRACK->AZref) * RrefC[2]) + ((RrefL[2] + clos->Rrel[2]) *
        TRACK->AZref);

      /* 'GUID_Embedded_Guid_Control_1D:3048' if (guidmode == 9) */
      if (((int32_T)guidmode) != 9) {
        /* 'GUID_Embedded_Guid_Control_1D:3050' else */
        /* 'GUID_Embedded_Guid_Control_1D:3051' Uref = (1.0-TRACK.AZref)*UrefC + TRACK.AZref*UrefL; */
        UrefC[0] = ((1.0 - TRACK->AZref) * UrefC[0]) + (TRACK->AZref * UrefL[0]);
        UrefC[1] = ((1.0 - TRACK->AZref) * UrefC[1]) + (TRACK->AZref * UrefL[1]);
        UrefC[2] = ((1.0 - TRACK->AZref) * UrefC[2]) + (TRACK->AZref * UrefL[2]);
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3049' Uref = UrefC; */
      }

      /* 'GUID_Embedded_Guid_Control_1D:3053' if (guidcmds.PurePropNav) */
      if (((int32_T)guidcmds_PurePropNav) != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:3054' Rref = Rwea; */
        Rref[0] = NAVin->pos_e.x;
        Rref[1] = NAVin->pos_e.y;
        Rref[2] = NAVin->pos_e.z;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3058' else */
      /* 'GUID_Embedded_Guid_Control_1D:3060' if (transition) */
      if (transition != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:3061' clos.Rrel = clos.Rref - RrefC; */
        clos->Rrel[0] = clos->Rref[0] - RrefC[0];
        clos->Rrel[1] = clos->Rref[1] - RrefC[1];
        clos->Rrel[2] = clos->Rref[2] - RrefC[2];
      }

      /* 'GUID_Embedded_Guid_Control_1D:3063' clos.AZcori = LIB_limit(0.0,1.0,clos.AZcori-0.005); */
      /* 'LIB_limit:27' if (in > max) */
      if ((clos->AZcori - 0.005) > 1.0) {
        /* 'LIB_limit:28' out = max; */
        RGEarcE = 1.0;
      } else if ((clos->AZcori - 0.005) < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RGEarcE = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        RGEarcE = clos->AZcori - 0.005;
      }

      clos->AZcori = RGEarcE;

      /* 'GUID_Embedded_Guid_Control_1D:3065' Rref = (1.0-TRACK.AZref)*RrefL + TRACK.AZref*(RrefC+clos.Rrel); */
      /* 'GUID_Embedded_Guid_Control_1D:3066' Uref = (1.0-TRACK.AZref)*UrefL + TRACK.AZref*UrefC; */
      Rref[0] = ((1.0 - TRACK->AZref) * RrefL[0]) + ((RrefC[0] + clos->Rrel[0]) *
        TRACK->AZref);
      UrefC[0] = ((1.0 - TRACK->AZref) * UrefL[0]) + (TRACK->AZref * UrefC[0]);
      Rref[1] = ((1.0 - TRACK->AZref) * RrefL[1]) + ((RrefC[1] + clos->Rrel[1]) *
        TRACK->AZref);
      UrefC[1] = ((1.0 - TRACK->AZref) * UrefL[1]) + (TRACK->AZref * UrefC[1]);
      Rref[2] = ((1.0 - TRACK->AZref) * RrefL[2]) + ((RrefC[2] + clos->Rrel[2]) *
        TRACK->AZref);
      UrefC[2] = ((1.0 - TRACK->AZref) * UrefL[2]) + (TRACK->AZref * UrefC[2]);
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:3073' Mdiff = sqrt(LIB_limit(1.0,2.0,guidcmds.GndRge/(tgt0.z+2000.0)))*OPT_FP.Mglide_opt - TRACK.Mglide; */
  RGEarcE = guidcmds_GndRge / (tgt0.z + 2000.0);

  /* 'LIB_limit:27' if (in > max) */
  if (RGEarcE > 2.0) {
    /* 'LIB_limit:28' out = max; */
    RGEarcE = 2.0;
  } else if (RGEarcE < 1.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    RGEarcE = 1.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  RGEarcE = (sqrt(RGEarcE) * OPT_FP_Mglide_opt) - TRACK->Mglide;

  /* 'GUID_Embedded_Guid_Control_1D:3074' if (guidmode < 5) */
  if (((int32_T)guidmode) < 5) {
    /* 'GUID_Embedded_Guid_Control_1D:3087' Rclos_ned = [0.0 0.0 0.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:3088' Rclos_b0  = [0.0 0.0 0.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:3089' Vclos_b0  = [0.0 0.0 0.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:3090' Rclos_FP0 = [0.0 0.0 0.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:3091' Vclos_FP0 = [0.0 0.0 0.0]'; */
    Rwea[0] = 0.0;
    Rwea[1] = 0.0;
    UrefL[1] = 0.0;
    Rmis0[1] = 0.0;
    Rwea[2] = 0.0;
    UrefL[2] = 0.0;
    Rmis0[2] = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3100' clos.Kglide_prev = LIB_limit(0.001, ... */
    /* 'GUID_Embedded_Guid_Control_1D:3101' 	                                 0.999, ... */
    /* 'GUID_Embedded_Guid_Control_1D:3102' 							         LIB_limit(0.0,10.0,10.0*Mdiff) + ... */
    /* 'GUID_Embedded_Guid_Control_1D:3103' 							         clos.Qfac*LIB_limit(-1.0e06,0.0,ENV.Q-OPT_FP.Qopt) + ... */
    /* 'GUID_Embedded_Guid_Control_1D:3104' 							         LIB_limit(0.0,1.0,1.0-(guidcmds.GndRge-1000)/6000.0)); */
    RGEarcE *= 10.0;

    /* 'LIB_limit:27' if (in > max) */
    if (RGEarcE > 10.0) {
      /* 'LIB_limit:28' out = max; */
      RGEarcE = 10.0;
    } else if (RGEarcE < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      RGEarcE = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    RGEwea = ENV_Q - OPT_FP_Qopt;

    /* 'LIB_limit:27' if (in > max) */
    if (RGEwea > 0.0) {
      /* 'LIB_limit:28' out = max; */
      RGEwea = 0.0;
    } else if (RGEwea < -1.0E+6) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      RGEwea = -1.0E+6;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    KQ = 1.0 - ((guidcmds_GndRge - 1000.0) / 6000.0);

    /* 'LIB_limit:27' if (in > max) */
    if (KQ > 1.0) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.0;
    } else if (KQ < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    RGEarcE = ((clos->Qfac * RGEwea) + RGEarcE) + KQ;

    /* 'LIB_limit:27' if (in > max) */
    if (RGEarcE > 0.999) {
      /* 'LIB_limit:28' out = max; */
      clos->Kglide_prev = 0.999;
    } else if (RGEarcE < 0.001) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      clos->Kglide_prev = 0.001;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      clos->Kglide_prev = RGEarcE;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3107' Dclos = sqrt(Rclos_b(2)^2+Rclos_b(3)^2); */
    RGEwea = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3111' if (guidmode == 4) */
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3115' else */
    /* 'GUID_Embedded_Guid_Control_1D:3118' EG0.phi = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3119' EG0.the = ENV.FPthem; */
    /* 'GUID_Embedded_Guid_Control_1D:3120' EG0.psi = ENV.FPpsim; */
    /* 'GUID_Embedded_Guid_Control_1D:3121' DCM0fpath = LIB_e2d(EG0); */
    LIB_e2d(0.0, ENV_FPthem, ENV_FPpsim, DCM0fpath);

    /* 'GUID_Embedded_Guid_Control_1D:3122' if (guidmode == 6) */
    if (((int32_T)guidmode) == 6) {
      /* 'GUID_Embedded_Guid_Control_1D:3123' RrelWea_ecef = Rwea - RrefL0; */
      /* 'GUID_Embedded_Guid_Control_1D:3124' RrelRef_ecef = Rref - RrefL0; */
      /* 'GUID_Embedded_Guid_Control_1D:3125' RrelWea_ned = DCM.ECEF2NED*RrelWea_ecef'; */
      /* 'GUID_Embedded_Guid_Control_1D:3126' RrelRef_ned = DCM.ECEF2NED*RrelRef_ecef'; */
      /* 'GUID_Embedded_Guid_Control_1D:3127' RrelWea_b0 = DCM0fpath'*RrelWea_ned; */
      /* 'GUID_Embedded_Guid_Control_1D:3128' RrelRef_b0 = DCM0fpath'*RrelRef_ned; */
      /* 'GUID_Embedded_Guid_Control_1D:3129' VrelWea_ecef = ENV.Vnav*(Ufpath - UrefL); */
      /* 'GUID_Embedded_Guid_Control_1D:3130' VrelRef_ecef = ENV.Vnav*(Uref   - UrefL); */
      /* 'GUID_Embedded_Guid_Control_1D:3131' VrelWea_ned = DCM.ECEF2NED*VrelWea_ecef'; */
      /* 'GUID_Embedded_Guid_Control_1D:3132' VrelRef_ned = DCM.ECEF2NED*VrelRef_ecef'; */
      /* 'GUID_Embedded_Guid_Control_1D:3133' VrelWea_b0 = DCM0fpath'*VrelWea_ned; */
      /* 'GUID_Embedded_Guid_Control_1D:3134' VrelRef_b0 = DCM0fpath'*VrelRef_ned; */
      /* 'GUID_Embedded_Guid_Control_1D:3136' sumWea_b0 = RrelWea_b0 + 1.0*VrelWea_b0; */
      for (transition = 0; transition < 3; transition++) {
        corvec_tmp[3 * transition] = DCM0fpath[transition];
        corvec_tmp[1 + (3 * transition)] = DCM0fpath[transition + 3];
        corvec_tmp[2 + (3 * transition)] = DCM0fpath[transition + 6];
        NAVin_0[transition] = Rwea[transition] - Rmis0[transition];
      }

      for (transition = 0; transition < 3; transition++) {
        Rclos_ecef[transition] = (Ufpath[transition] - UrefL[transition]) *
          ENV_Vnav;
        sumRef_b0[transition] = (DCM->ECEF2NED[transition + 6] * NAVin_0[2]) +
          ((DCM->ECEF2NED[transition + 3] * NAVin_0[1]) + (DCM->
            ECEF2NED[transition] * NAVin_0[0]));
      }

      for (transition = 0; transition < 3; transition++) {
        clos_0[transition] = (DCM->ECEF2NED[transition + 6] * Rclos_ecef[2]) +
          ((DCM->ECEF2NED[transition + 3] * Rclos_ecef[1]) + (DCM->
            ECEF2NED[transition] * Rclos_ecef[0]));
        NAVin_0[transition] = (corvec_tmp[transition + 6] * sumRef_b0[2]) +
          ((corvec_tmp[transition + 3] * sumRef_b0[1]) + (corvec_tmp[transition]
            * sumRef_b0[0]));
      }

      for (transition = 0; transition < 3; transition++) {
        corvec[transition] = NAVin_0[transition] + ((corvec_tmp[transition + 6] *
          clos_0[2]) + ((corvec_tmp[transition + 3] * clos_0[1]) +
                        (corvec_tmp[transition] * clos_0[0])));
      }

      /* 'GUID_Embedded_Guid_Control_1D:3137' sumRef_b0 = RrelRef_b0 + 1.0*VrelRef_b0; */
      RGEwea = Rref[0] - Rmis0[0];
      Rclos_b_idx_1 = Rref[1] - Rmis0[1];
      Rclos_b_idx_2 = Rref[2] - Rmis0_0;
      for (transition = 0; transition < 3; transition++) {
        Rclos_ecef[transition] = (UrefC[transition] - UrefL[transition]) *
          ENV_Vnav;
        sumRef_b0[transition] = (DCM->ECEF2NED[transition + 6] * Rclos_b_idx_2)
          + ((DCM->ECEF2NED[transition + 3] * Rclos_b_idx_1) + (DCM->
              ECEF2NED[transition] * RGEwea));
      }

      for (transition = 0; transition < 3; transition++) {
        clos_0[transition] = (DCM->ECEF2NED[transition + 6] * Rclos_ecef[2]) +
          ((DCM->ECEF2NED[transition + 3] * Rclos_ecef[1]) + (DCM->
            ECEF2NED[transition] * Rclos_ecef[0]));
        NAVin_0[transition] = (corvec_tmp[transition + 6] * sumRef_b0[2]) +
          ((corvec_tmp[transition + 3] * sumRef_b0[1]) + (corvec_tmp[transition]
            * sumRef_b0[0]));
      }

      for (transition = 0; transition < 3; transition++) {
        sumRef_b0[transition] = NAVin_0[transition] + ((corvec_tmp[transition +
          6] * clos_0[2]) + ((corvec_tmp[transition + 3] * clos_0[1]) +
                             (corvec_tmp[transition] * clos_0[0])));
      }

      /* 'GUID_Embedded_Guid_Control_1D:3138' clos.fadeFac = LIB_limit(0.0,1.0,-0.001*dot([sumWea_b0(2) sumWea_b0(3)],[sumRef_b0(2) sumRef_b0(3)])); */
      corvec_0[0] = corvec[1];
      corvec_0[1] = corvec[2];
      sumRef_b0_0[0] = sumRef_b0[1];
      sumRef_b0_0[1] = sumRef_b0[2];
      RGEwea = -0.001 * dot_tXnwwRPM(corvec_0, sumRef_b0_0);

      /* 'LIB_limit:27' if (in > max) */
      if (RGEwea > 1.0) {
        /* 'LIB_limit:28' out = max; */
        clos->fadeFac = 1.0;
      } else if (RGEwea < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        clos->fadeFac = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        clos->fadeFac = RGEwea;
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:3147' if (AFmode == 2) */
    switch (AFmode) {
     case 2:
      /* 'GUID_Embedded_Guid_Control_1D:3148' clos.Kglide = LIB_limit(clos.Kglide_prev-0.0002, ... */
      /* 'GUID_Embedded_Guid_Control_1D:3149' 	                          clos.Kglide_prev+0.001, ... */
      /* 'GUID_Embedded_Guid_Control_1D:3150' 							  LIB_limit(0.0,1.0,Mdiff) + ... */
      /* 'GUID_Embedded_Guid_Control_1D:3151' 							  clos.Qfac*LIB_limit(-0.1e06,0.0,0.1*(ENV.Q-OPT_FP.Qopt))); */
      /* 'LIB_limit:27' if (in > max) */
      if (RGEarcE > 1.0) {
        /* 'LIB_limit:28' out = max; */
        RGEarcE = 1.0;
      } else if (RGEarcE < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RGEarcE = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RGEwea = (ENV_Q - OPT_FP_Qopt) * 0.1;

      /* 'LIB_limit:27' if (in > max) */
      if (RGEwea > 0.0) {
        /* 'LIB_limit:28' out = max; */
        RGEwea = 0.0;
      } else if (RGEwea < -100000.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RGEwea = -100000.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RGEarcE += clos->Qfac * RGEwea;

      /* 'LIB_limit:27' if (in > max) */
      if (RGEarcE > (clos->Kglide_prev + 0.001)) {
        /* 'LIB_limit:28' out = max; */
        RGEarcE = clos->Kglide_prev + 0.001;
      } else if (RGEarcE < (clos->Kglide_prev - 0.0002)) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RGEarcE = clos->Kglide_prev - 0.0002;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      clos->Kglide = RGEarcE;
      break;

     case 3:
      /* 'GUID_Embedded_Guid_Control_1D:3152' elseif (AFmode == 3) */
      /* 'GUID_Embedded_Guid_Control_1D:3153' clos.Kglide = 1.0; */
      clos->Kglide = 1.0;
      break;

     default:
      if (clos->Kglide < 0.999) {
        /* 'GUID_Embedded_Guid_Control_1D:3154' elseif (clos.Kglide < 0.999) */
        /* 'GUID_Embedded_Guid_Control_1D:3163' clos.Kglide = LIB_limit(clos.Kglide_prev-0.0002, ... */
        /* 'GUID_Embedded_Guid_Control_1D:3164' 	                            clos.Kglide_prev+0.002, ... */
        /* 'GUID_Embedded_Guid_Control_1D:3165' 			  				    LIB_limit(0.0,10.0,10.0*Mdiff) + ... */
        /* 'GUID_Embedded_Guid_Control_1D:3166' 				  			    clos.Qfac*LIB_limit(-1.0e06,0.0,ENV.Q-OPT_FP.Qopt) + ... */
        /* 'GUID_Embedded_Guid_Control_1D:3167' 							    LIB_limit(0.0,1.0,1.0-(guidcmds.GndRge-1000)/6000.0)); */
        RGEarcE *= 10.0;

        /* 'LIB_limit:27' if (in > max) */
        if (RGEarcE > 10.0) {
          /* 'LIB_limit:28' out = max; */
          RGEarcE = 10.0;
        } else if (RGEarcE < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          RGEarcE = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        RGEwea = ENV_Q - OPT_FP_Qopt;

        /* 'LIB_limit:27' if (in > max) */
        if (RGEwea > 0.0) {
          /* 'LIB_limit:28' out = max; */
          RGEwea = 0.0;
        } else if (RGEwea < -1.0E+6) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          RGEwea = -1.0E+6;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        KQ = 1.0 - ((guidcmds_GndRge - 1000.0) / 6000.0);

        /* 'LIB_limit:27' if (in > max) */
        if (KQ > 1.0) {
          /* 'LIB_limit:28' out = max; */
          KQ = 1.0;
        } else if (KQ < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          KQ = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        RGEarcE = ((clos->Qfac * RGEwea) + RGEarcE) + KQ;

        /* 'LIB_limit:27' if (in > max) */
        if (RGEarcE > (clos->Kglide_prev + 0.002)) {
          /* 'LIB_limit:28' out = max; */
          RGEarcE = clos->Kglide_prev + 0.002;
        } else if (RGEarcE < (clos->Kglide_prev - 0.0002)) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          RGEarcE = clos->Kglide_prev - 0.0002;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        clos->Kglide = RGEarcE;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3170' else */
        /* 'GUID_Embedded_Guid_Control_1D:3171' clos.Kglide = 1.0; */
        clos->Kglide = 1.0;
      }
      break;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3176' PHI = NAVin.att_bn.phi; */
    /* 'GUID_Embedded_Guid_Control_1D:3177' EG0.phi = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3180' EG0.the = NAVin.att_bn.the; */
    /* 'GUID_Embedded_Guid_Control_1D:3181' EG0.psi = NAVin.att_bn.psi; */
    /* 'GUID_Embedded_Guid_Control_1D:3182' DCM0body = LIB_e2d(EG0); */
    /* 'GUID_Embedded_Guid_Control_1D:3184' clos.Kglide_prev = LIB_limit(0.001,0.999,clos.Kglide); */
    /* 'LIB_limit:27' if (in > max) */
    if (clos->Kglide > 0.999) {
      /* 'LIB_limit:28' out = max; */
      RGEarcE = 0.999;
    } else if (clos->Kglide < 0.001) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      RGEarcE = 0.001;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      RGEarcE = clos->Kglide;
    }

    clos->Kglide_prev = RGEarcE;

    /* 'GUID_Embedded_Guid_Control_1D:3187' Rref_ned = DCM.ECEF2NED*Rref'; */
    /* 'GUID_Embedded_Guid_Control_1D:3188' Rwea_ned = DCM.ECEF2NED*Rwea'; */
    /* 'GUID_Embedded_Guid_Control_1D:3189' Rref_ned(3) = clos.Kglide*Rref_ned(3) + (1.0-clos.Kglide)*Rwea_ned(3); */
    for (transition = 0; transition < 3; transition++) {
      corvec_tmp[3 * transition] = DCM->ECEF2NED[transition];
      RGEwea = DCM->ECEF2NED[transition + 3];
      corvec_tmp[1 + (3 * transition)] = RGEwea;
      Rclos_b_idx_1 = DCM->ECEF2NED[transition + 6];
      corvec_tmp[2 + (3 * transition)] = Rclos_b_idx_1;
      corvec[transition] = (Rclos_b_idx_1 * Rref[2]) + ((RGEwea * Rref[1]) +
        (DCM->ECEF2NED[transition] * Rref[0]));
      sumRef_b0[transition] = (Rclos_b_idx_1 * Rwea[2]) + ((RGEwea * Rwea[1]) +
        (DCM->ECEF2NED[transition] * Rwea[0]));
    }

    corvec[2] = ((1.0 - clos->Kglide) * sumRef_b0[2]) + (clos->Kglide * corvec[2]);

    /* 'GUID_Embedded_Guid_Control_1D:3190' Rref = (DCM.ECEF2NED'*Rref_ned)'; */
    /* 'GUID_Embedded_Guid_Control_1D:3191' Rclos_ecef = Rwea - Rref; */
    for (transition = 0; transition < 3; transition++) {
      RGEwea = corvec_tmp[transition + 3];
      Rclos_b_idx_1 = corvec_tmp[transition + 6] * corvec[2];
      Rclos_ecef[transition] = Rwea[transition] - (Rclos_b_idx_1 + ((RGEwea *
        corvec[1]) + (corvec_tmp[transition] * corvec[0])));
      Rref[transition] = Rclos_b_idx_1 + ((RGEwea * corvec[1]) +
        (corvec_tmp[transition] * corvec[0]));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3192' Vclos_ecef = ENV.Vnav*(Ufpath - Uref); */
    /* 'GUID_Embedded_Guid_Control_1D:3194' Rclos_ned = DCM.ECEF2NED*Rclos_ecef'; */
    /* 'GUID_Embedded_Guid_Control_1D:3195' Vclos_ned = DCM.ECEF2NED*Vclos_ecef'; */
    /* 'GUID_Embedded_Guid_Control_1D:3196' Acori_ned = DCM.ECEF2NED*coracc_ecef'; */
    /* 'GUID_Embedded_Guid_Control_1D:3198' Acori_ned = clos.Kglide*clos.Kglide*Acori_ned; */
    RGEarcE = clos->Kglide * clos->Kglide;

    /* 'GUID_Embedded_Guid_Control_1D:3201' Rclos_FP0 = DCM0fpath'*Rclos_ned; */
    for (transition = 0; transition < 3; transition++) {
      sumRef_b0[transition] = (DCM->ECEF2NED[transition + 6] * Rclos_ecef[2]) +
        ((DCM->ECEF2NED[transition + 3] * Rclos_ecef[1]) + (DCM->
          ECEF2NED[transition] * Rclos_ecef[0]));
    }

    for (transition = 0; transition < 3; transition++) {
      UrefL[transition] = (DCM0fpath[(3 * transition) + 2] * sumRef_b0[2]) +
        ((DCM0fpath[(3 * transition) + 1] * sumRef_b0[1]) + (DCM0fpath[3 *
          transition] * sumRef_b0[0]));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3202' Rclos_FP0(1) = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3203' Rclos_FP0(2) = LIB_limit(clos.Rby0_prev-clos.dRlim,clos.Rby0_prev+clos.dRlim,Rclos_FP0(2)); */
    RGEwea = clos->Rby0_prev - clos->dRlim;
    KQ = clos->Rby0_prev + clos->dRlim;

    /* 'LIB_limit:27' if (in > max) */
    if (UrefL[1] > KQ) {
      /* 'LIB_limit:28' out = max; */
      RGEwea = KQ;
    } else if (!(UrefL[1] < RGEwea)) {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      RGEwea = UrefL[1];
    } else {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
    }

    UrefL[1] = RGEwea;

    /* 'GUID_Embedded_Guid_Control_1D:3204' Rclos_FP0(3) = LIB_limit(clos.Rbz0_prev-clos.dRlim,clos.Rbz0_prev+clos.dRlim,Rclos_FP0(3)); */
    RGEwea = clos->Rbz0_prev - clos->dRlim;
    KQ = clos->Rbz0_prev + clos->dRlim;

    /* 'LIB_limit:27' if (in > max) */
    if (UrefL[2] > KQ) {
      /* 'LIB_limit:28' out = max; */
      RGEwea = KQ;
    } else if (!(UrefL[2] < RGEwea)) {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      RGEwea = UrefL[2];
    } else {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
    }

    UrefL[2] = RGEwea;

    /* 'GUID_Embedded_Guid_Control_1D:3205' Rclos_ned = DCM0fpath*Rclos_FP0; */
    for (transition = 0; transition < 3; transition++) {
      Rwea[transition] = (DCM0fpath[transition + 6] * RGEwea) +
        ((DCM0fpath[transition + 3] * UrefL[1]) + (DCM0fpath[transition] * 0.0));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3216' clos.Qfac = LIB_limit(0.00002,100.0,0.00002-0.000002*(10.0-Rclos_ned(3))); */
    RGEwea = 2.0E-5 - ((10.0 - Rwea[2]) * 2.0E-6);

    /* 'LIB_limit:27' if (in > max) */
    if (RGEwea > 100.0) {
      /* 'LIB_limit:28' out = max; */
      clos->Qfac = 100.0;
    } else if (RGEwea < 2.0E-5) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      clos->Qfac = 2.0E-5;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      clos->Qfac = RGEwea;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3217' Rclos_b0 = DCM0body'*Rclos_ned; */
    /* 'GUID_Embedded_Guid_Control_1D:3218' Dclos = norm(Rclos_b0); */
    LIB_e2d(0.0, NAVin->att_bn.the, NAVin->att_bn.psi, tmp);
    for (transition = 0; transition < 3; transition++) {
      corvec_tmp[3 * transition] = tmp[transition];
      corvec_tmp[1 + (3 * transition)] = tmp[transition + 3];
      corvec_tmp[2 + (3 * transition)] = tmp[transition + 6];
    }

    for (transition = 0; transition < 3; transition++) {
      NAVin_0[transition] = (corvec_tmp[transition + 6] * Rwea[2]) +
        ((corvec_tmp[transition + 3] * Rwea[1]) + (corvec_tmp[transition] *
          Rwea[0]));
    }

    RGEwea = norm_MfVYifVG(NAVin_0);

    /* 'GUID_Embedded_Guid_Control_1D:3219' Rclos_ned(1) = LIB_limit(0.12,1.0,clos.Kglide)*Rclos_ned(1); */
    /* 'LIB_limit:27' if (in > max) */
    if (clos->Kglide > 1.0) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.0;
    } else if (clos->Kglide < 0.12) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 0.12;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = clos->Kglide;
    }

    Rwea[0] *= KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3220' Rclos_ned(2) = LIB_limit(0.12,1.0,clos.Kglide)*Rclos_ned(2); */
    /* 'LIB_limit:27' if (in > max) */
    if (clos->Kglide > 1.0) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.0;
    } else if (clos->Kglide < 0.12) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 0.12;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = clos->Kglide;
    }

    Rwea[1] *= KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3221' Rclos_ned(3) = (clos.Kglide^1.5)*Rclos_ned(3); */
    Rwea_tmp = rt_powd_snf(clos->Kglide, 1.5);
    Rwea[2] *= Rwea_tmp;

    /* 'GUID_Embedded_Guid_Control_1D:3222' Rclos_b0 = DCM0body'*Rclos_ned; */
    for (transition = 0; transition < 3; transition++) {
      corvec[transition] = (corvec_tmp[transition + 6] * Rwea[2]) +
        ((corvec_tmp[transition + 3] * Rwea[1]) + (corvec_tmp[transition] *
          Rwea[0]));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3226' Rclos_b(1) = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3227' Rclos_b(2) =  Rclos_b0(2)*cos(PHI)+Rclos_b0(3)*sin(PHI); */
    Rmis0_0 = sin(NAVin->att_bn.phi);
    Vclos_b_idx_2 = cos(NAVin->att_bn.phi);
    Rclos_b_idx_1 = (corvec[1] * Vclos_b_idx_2) + (corvec[2] * Rmis0_0);

    /* 'GUID_Embedded_Guid_Control_1D:3228' Rclos_b(3) = -Rclos_b0(2)*sin(PHI)+Rclos_b0(3)*cos(PHI); */
    Rclos_b_idx_2 = ((-corvec[1]) * Rmis0_0) + (corvec[2] * Vclos_b_idx_2);

    /* 'GUID_Embedded_Guid_Control_1D:3231' Vclos_FP0 = DCM0fpath'*Vclos_ned; */
    Vclos_b_idx_1 = (Ufpath[0] - UrefC[0]) * ENV_Vnav;
    KQ = (Ufpath[1] - UrefC[1]) * ENV_Vnav;
    ENV_Vnav_idx_2 = (Ufpath[2] - UrefC[2]) * ENV_Vnav;
    for (transition = 0; transition < 3; transition++) {
      sumRef_b0[transition] = (DCM->ECEF2NED[transition + 6] * ENV_Vnav_idx_2) +
        ((DCM->ECEF2NED[transition + 3] * KQ) + (DCM->ECEF2NED[transition] *
          Vclos_b_idx_1));
    }

    for (transition = 0; transition < 3; transition++) {
      Rmis0[transition] = (DCM0fpath[(3 * transition) + 2] * sumRef_b0[2]) +
        ((DCM0fpath[(3 * transition) + 1] * sumRef_b0[1]) + (DCM0fpath[3 *
          transition] * sumRef_b0[0]));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3232' Vclos_FP0(1) = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3233' Vclos_FP0(2) = LIB_limit(clos.Vby0_prev-clos.dVlim,clos.Vby0_prev+clos.dVlim,Vclos_FP0(2)); */
    KQ = clos->Vby0_prev - clos->dVlim;
    Vclos_b_idx_1 = clos->Vby0_prev + clos->dVlim;

    /* 'LIB_limit:27' if (in > max) */
    if (Rmis0[1] > Vclos_b_idx_1) {
      /* 'LIB_limit:28' out = max; */
      KQ = Vclos_b_idx_1;
    } else if (!(Rmis0[1] < KQ)) {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = Rmis0[1];
    } else {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
    }

    Rmis0[1] = KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3234' Vclos_FP0(3) = LIB_limit(clos.Vbz0_prev-clos.dVlim,clos.Vbz0_prev+clos.dVlim,Vclos_FP0(3)); */
    KQ = clos->Vbz0_prev - clos->dVlim;
    Vclos_b_idx_1 = clos->Vbz0_prev + clos->dVlim;

    /* 'LIB_limit:27' if (in > max) */
    if (Rmis0[2] > Vclos_b_idx_1) {
      /* 'LIB_limit:28' out = max; */
      KQ = Vclos_b_idx_1;
    } else if (!(Rmis0[2] < KQ)) {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = Rmis0[2];
    } else {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
    }

    Rmis0[2] = KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3235' Vclos_ned = DCM0fpath*Vclos_FP0; */
    for (transition = 0; transition < 3; transition++) {
      corvec[transition] = (DCM0fpath[transition + 6] * KQ) +
        ((DCM0fpath[transition + 3] * Rmis0[1]) + (DCM0fpath[transition] * 0.0));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3246' Vclos_ned(1) = LIB_limit(0.1,1.0,clos.Kglide)*Vclos_ned(1); */
    /* 'LIB_limit:27' if (in > max) */
    if (clos->Kglide > 1.0) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.0;
    } else if (clos->Kglide < 0.1) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 0.1;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = clos->Kglide;
    }

    corvec[0] *= KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3247' Vclos_ned(2) = LIB_limit(0.1,1.0,clos.Kglide)*Vclos_ned(2); */
    /* 'LIB_limit:27' if (in > max) */
    if (clos->Kglide > 1.0) {
      /* 'LIB_limit:28' out = max; */
      KQ = 1.0;
    } else if (clos->Kglide < 0.1) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      KQ = 0.1;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      KQ = clos->Kglide;
    }

    corvec[1] *= KQ;

    /* 'GUID_Embedded_Guid_Control_1D:3248' Vclos_ned(3) = (clos.Kglide^1.5)*Vclos_ned(3); */
    corvec[2] *= Rwea_tmp;

    /* 'GUID_Embedded_Guid_Control_1D:3249' Vclos_b0 = DCM0body'*Vclos_ned; */
    for (transition = 0; transition < 3; transition++) {
      sumRef_b0[transition] = (corvec_tmp[transition + 6] * corvec[2]) +
        ((corvec_tmp[transition + 3] * corvec[1]) + (corvec_tmp[transition] *
          corvec[0]));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3252' Vclos_b(1) = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:3253' Vclos_b(2) =  Vclos_b0(2)*cos(PHI)+Vclos_b0(3)*sin(PHI); */
    Vclos_b_idx_1 = (sumRef_b0[1] * Vclos_b_idx_2) + (sumRef_b0[2] * Rmis0_0);

    /* 'GUID_Embedded_Guid_Control_1D:3254' Vclos_b(3) = -Vclos_b0(2)*sin(PHI)+Vclos_b0(3)*cos(PHI); */
    Vclos_b_idx_2 = ((-sumRef_b0[1]) * Rmis0_0) + (sumRef_b0[2] * Vclos_b_idx_2);

    /* 'GUID_Embedded_Guid_Control_1D:3256' Acori_b = DCM.NED2BODY*Acori_ned; */
    for (transition = 0; transition < 3; transition++) {
      Ufpath[transition] = RGEarcE * ((DCM->ECEF2NED[transition + 6] *
        coracc_ecef_idx_2) + ((DCM->ECEF2NED[transition + 3] * coracc_ecef_idx_1)
        + (DCM->ECEF2NED[transition] * coracc_ecef_idx_0)));
    }

    for (transition = 0; transition < 3; transition++) {
      Acori_b_data[transition] = (DCM->NED2BODY[transition + 6] * Ufpath[2]) +
        ((DCM->NED2BODY[transition + 3] * Ufpath[1]) + (DCM->NED2BODY[transition]
          * Ufpath[0]));
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:3261' clos.Rref = Rref; */
  /* 'GUID_Embedded_Guid_Control_1D:3262' clos.Recef = Rclos_ecef; */
  /* 'GUID_Embedded_Guid_Control_1D:3263' clos.Rned = Rclos_ned'; */
  /* 'GUID_Embedded_Guid_Control_1D:3264' clos.Rfpath = Rclos_FP0'; */
  clos->Rref[0] = Rref[0];
  clos->Recef[0] = Rclos_ecef[0];

  clos->Rned[0] = Rwea[0];
  clos->Vned[0] = corvec[0];
  clos->Aned[0] = Ufpath[0];
  clos->Rned[1] = Rwea[1];
  clos->Vned[1] = corvec[1];
  clos->Aned[1] = Ufpath[1];
  clos->Rned[2] = Rwea[2];
  clos->Vned[2] = corvec[2];
  clos->Aned[2] = Ufpath[2];

  clos->Rfpath[0] = 0.0;
  clos->Rref[1] = Rref[1];
  clos->Recef[1] = Rclos_ecef[1];
  clos->Rned[1] = Rwea[1];
  clos->Rfpath[1] = UrefL[1];
  clos->Rref[2] = Rref[2];
  clos->Recef[2] = Rclos_ecef[2];
  clos->Rned[2] = Rwea[2];
  clos->Rfpath[2] = UrefL[2];

  /* 'GUID_Embedded_Guid_Control_1D:3265' clos.Roffset = Dclos; */
  clos->Roffset = RGEwea;

  /* 'GUID_Embedded_Guid_Control_1D:3270' clos.Rby = Rclos_b(2); */
  clos->Rby = Rclos_b_idx_1;

  /* 'GUID_Embedded_Guid_Control_1D:3271' clos.Rbz = Rclos_b(3); */
  clos->Rbz = Rclos_b_idx_2;

  /* 'GUID_Embedded_Guid_Control_1D:3272' clos.Vby = Vclos_b(2); */
  clos->Vby = Vclos_b_idx_1;

  /* 'GUID_Embedded_Guid_Control_1D:3273' clos.Vbz = Vclos_b(3); */
  clos->Vbz = Vclos_b_idx_2;

  /* 'GUID_Embedded_Guid_Control_1D:3274' clos.Aby = clos.AZcori*Acori_b(2); */
  clos->Aby = clos->AZcori * Acori_b_data[1];

  /* 'GUID_Embedded_Guid_Control_1D:3275' clos.Abz = clos.AZcori*Acori_b(3); */
  clos->Abz = clos->AZcori * Acori_b_data[2];

  /* 'GUID_Embedded_Guid_Control_1D:3285' clos.Rby0_prev = LIB_limit(-100.0,100.0,Rclos_FP0(2)); */
  /* 'LIB_limit:27' if (in > max) */
  if (UrefL[1] > 100.0) {
    /* 'LIB_limit:28' out = max; */
    clos->Rby0_prev = 100.0;
  } else if (UrefL[1] < -100.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    clos->Rby0_prev = -100.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    clos->Rby0_prev = UrefL[1];
  }

  /* 'GUID_Embedded_Guid_Control_1D:3286' clos.Rbz0_prev = LIB_limit(-100.0,100.0,Rclos_FP0(3)); */
  /* 'LIB_limit:27' if (in > max) */
  if (UrefL[2] > 100.0) {
    /* 'LIB_limit:28' out = max; */
    clos->Rbz0_prev = 100.0;
  } else if (UrefL[2] < -100.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    clos->Rbz0_prev = -100.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    clos->Rbz0_prev = UrefL[2];
  }

  /* 'GUID_Embedded_Guid_Control_1D:3287' clos.Vby0_prev = LIB_limit(-100.0,100.0,Vclos_FP0(2)); */
  /* 'LIB_limit:27' if (in > max) */
  if (Rmis0[1] > 100.0) {
    /* 'LIB_limit:28' out = max; */
    clos->Vby0_prev = 100.0;
  } else if (Rmis0[1] < -100.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    clos->Vby0_prev = -100.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    clos->Vby0_prev = Rmis0[1];
  }

  /* 'GUID_Embedded_Guid_Control_1D:3288' clos.Vbz0_prev = LIB_limit(-100.0,100.0,Vclos_FP0(3)); */
  /* 'LIB_limit:27' if (in > max) */
  if (Rmis0[2] > 100.0) {
    /* 'LIB_limit:28' out = max; */
    clos->Vbz0_prev = 100.0;
  } else if (Rmis0[2] < -100.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    clos->Vbz0_prev = -100.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    clos->Vbz0_prev = Rmis0[2];
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [phic,gbyc,gbzc,guidcmds,temp_gbyc] = ExeGuidanceCommands(opportunity_mode,guidmode,delt,ftime,guidcmds,AFmode,reset,bunt_arc,gains,TRACK,clos,ENV,OPT_FP,DCM,TTKF_in,NAVin,IMUin,MPin,guid2AP,fromDATA,guid_settings,psiFPi,psiLOSb,def)
 */
static void ExeGuidanceCommands(uint8_T opportunity_mode, uint8_T guidmode,
  real_T delt, sETPRuSGTG6V05GSmeYzNjH *guidcmds, uint8_T AFmode, uint8_T reset,
  uint8_T bunt_arc, real_T gains_Kp, const real_T TRACK_Ut0[3], const real_T
  TRACK_Ut[3], const ssOdl51LB8ZQlaB7SirUWkE *clos, const
  sVUEBEgeCpQlxFVEgYjRFMH *ENV, const sSZzX5nI8YD60UjueDawhgE *OPT_FP, const
  sPLFBkBfmjj013qOjLWm7GC *DCM, const T_GUID_TTKF_Out_IF1002 *TTKF_in, const
  sb6O280JGWZ46sxhPtS13aC *NAVin, const shkoXMvGDT09CSMPYwHF4WB IMUin, const
  T_GUID_MissionPlanning *MPin, T_GUID_Guid_Out_IF1003 *guid2AP, const
  T_Weapon_MCP_DATA_Guid *fromDATA, real_T psiLOSb, const
  struct_Nfy90zz6zGRJDmnb4iAwuF *b_def, real_T *phic, real_T *gbyc, real_T *gbzc,
  real_T *temp_gbyc)
{
  real_T Uup[3];
  real_T Glim_glide;
  real_T the_off;
  real_T fac_FOV;
  real_T wbx_neg;
  real_T wbx_pos;
  real_T dT;
  real_T phi_neg;
  real_T phi_pos;
  int32_T b_dT;

  real_T Gned_clos[3];
  real_T Gbody_clos[3];
  int32_T count;


  static const real_T c[7] = { 0.0, 500.0, 1000.0, 2000.0, 3000.0, 5000.0,
    500000.0 };

  static const real_T d[7] = { 0.0, 0.2, 1.0, 1.0, 0.5, 0.0, 0.0 };

  static const real_T e[7] = { 0.0, 500.0, 1000.0, 1500.0, 2000.0, 3000.0,
    500000.0 };

  real_T fromDATA_0[7];
  real_T fromDATA_1[7];
  real_T phi_neg_0;
  boolean_T exitg1;

  /* 'GUID_Embedded_Guid_Control_1D:3296' if (ftime > 19.5) */
  /* 'GUID_Embedded_Guid_Control_1D:3299' if (ftime > 24.0) */
  /* 'GUID_Embedded_Guid_Control_1D:3303' d2r = pi/180.0; */
  /* 'GUID_Embedded_Guid_Control_1D:3304' machvec = double(fromDATA.AirframeProp.machvec); */
  /* 'GUID_Embedded_Guid_Control_1D:3305' CMXdefl = double(fromDATA.AirframeProp.CMXdefl); */
  /* 'GUID_Embedded_Guid_Control_1D:3306' Jxx     = double(fromDATA.AirframeProp.Jxx); */
  /* 'GUID_Embedded_Guid_Control_1D:3307' Sref    = double(fromDATA.AirframeProp.Sref); */
  /* 'GUID_Embedded_Guid_Control_1D:3308' Xref    = double(fromDATA.AirframeProp.Xref); */
  /* 'GUID_Embedded_Guid_Control_1D:3310' if (guidcmds.verticaldive) */
  if (((int32_T)guidcmds->verticaldive) != 0) {
    /* 'GUID_Embedded_Guid_Control_1D:3311' Uup = DCM.NED2BODY'*[0 0 -1]'; */
    for (b_dT = 0; b_dT < 3; b_dT++) {
      Uup[b_dT] = (-DCM->NED2BODY[(3 * b_dT) + 2]) + ((DCM->NED2BODY[(3 * b_dT)
        + 1] * 0.0) + (DCM->NED2BODY[3 * b_dT] * 0.0));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3312' guidcmds.phim = atan2(Uup(2),Uup(1)); */
    guidcmds->phim = rt_atan2d_snf(Uup[1], Uup[0]);
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3313' else */
    /* 'GUID_Embedded_Guid_Control_1D:3314' guidcmds.phim = NAVin.att_bn.phi; */
    guidcmds->phim = NAVin->att_bn.phi;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3318' guidcmds.inty = guidcmds.inty + gains.Ki*clos.Rby; */
  guidcmds->inty += 0.0 * clos->Rby;

  /* 'GUID_Embedded_Guid_Control_1D:3319' temp_gbyc = 0.0; */
  *temp_gbyc = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:3323' Glim_glide = -12.0 - 10.0*LIB_limit(0.0,1.0,1.0-TRACK.Ut(3)/TRACK.Ut0(3)); */
  Glim_glide = 1.0 - (TRACK_Ut[2] / TRACK_Ut0[2]);

  /* 'LIB_limit:27' if (in > max) */
  if (Glim_glide > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Glim_glide = 1.0;
  } else if (Glim_glide < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Glim_glide = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Glim_glide = -12.0 - (10.0 * Glim_glide);

  /* 'GUID_Embedded_Guid_Control_1D:3324' if (AFmode == 2) */
  if (((int32_T)AFmode) == 2) {
    /* 'GUID_Embedded_Guid_Control_1D:3325' Glim_glide = -22.0; */
    Glim_glide = -22.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3328' if (MPin.MissionType == def.SAL || MPin.MissionType == def.IIR) */
  if ((((real_T)MPin->MissionType) == b_def->SAL) || (((real_T)MPin->MissionType)
       == b_def->IIR)) {
    /* 'GUID_Embedded_Guid_Control_1D:3329' the_off = TTKF_in.the_b - double(fromDATA.seeker.Bele); */
    the_off = TTKF_in->the_b - ((real_T)fromDATA->seeker.Bele);

    /* 'GUID_Embedded_Guid_Control_1D:3330' psi_off = TTKF_in.psi_b; */
    /* 'GUID_Embedded_Guid_Control_1D:3332' if (MPin.MissionType == def.SAL) */
    if (((real_T)MPin->MissionType) == b_def->SAL) {
      /* 'GUID_Embedded_Guid_Control_1D:3333' fac_FOV  = LIB_limit(0.0,1.0,interp1([   0.0  500.0 1000.0 2000.0 3000.0 5000.0 500000.0],... */
      /* 'GUID_Embedded_Guid_Control_1D:3334'                                            [   0.0    0.2    1.0    1.0    0.5    0.0      0.0],... */
      /* 'GUID_Embedded_Guid_Control_1D:3335'                                            TTKF_in.tgtrge)); */
      fac_FOV = interp1_WUCGY3Hg(c, d, TTKF_in->tgtrge);

      /* 'LIB_limit:27' if (in > max) */
      if (fac_FOV > 1.0) {
        /* 'LIB_limit:28' out = max; */
        fac_FOV = 1.0;
      } else if (fac_FOV < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        fac_FOV = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3336' else */
      /* 'GUID_Embedded_Guid_Control_1D:3337' fac_FOV  = LIB_limit(0.0,1.0,interp1([   0.0  500.0 1000.0 1500.0 2000.0 3000.0 500000.0],... */
      /* 'GUID_Embedded_Guid_Control_1D:3338'                                            [   0.0    0.2    1.0    1.0    0.5    0.0      0.0],... */
      /* 'GUID_Embedded_Guid_Control_1D:3339'                                            TTKF_in.tgtrge)); */
      fac_FOV = interp1_WUCGY3Hg(e, d, TTKF_in->tgtrge);

      /* 'LIB_limit:27' if (in > max) */
      if (fac_FOV > 1.0) {
        /* 'LIB_limit:28' out = max; */
        fac_FOV = 1.0;
      } else if (fac_FOV < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        fac_FOV = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:3341' thresholdFOV = double(fromDATA.seeker.Alin) - 0.0698; */
    /* 'GUID_Embedded_Guid_Control_1D:3342' if (the_off >= 0.0) */
    if (the_off >= 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:3343' guidcmds.gbzc_FOV = 0.5*fac_FOV*abs(OPT_FP.GLimZup)*LIB_limit(-0.5, 0.0,-7.0*(the_off - thresholdFOV)); */
      the_off = (the_off - (((real_T)fromDATA->seeker.Alin) - 0.0698)) * -7.0;

      /* 'LIB_limit:27' if (in > max) */
      if (the_off > 0.0) {
        /* 'LIB_limit:28' out = max; */
        the_off = 0.0;
      } else if (the_off < -0.5) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        the_off = -0.5;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      guidcmds->gbzc_FOV = ((0.5 * fac_FOV) * fabs(OPT_FP->GLimZup)) * the_off;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3344' else */
      /* 'GUID_Embedded_Guid_Control_1D:3345' guidcmds.gbzc_FOV = 0.5*fac_FOV*abs(OPT_FP.GLimZlo)*LIB_limit( 0.0, 0.5,-7.0*(the_off + thresholdFOV)); */
      the_off = ((((real_T)fromDATA->seeker.Alin) - 0.0698) + the_off) * -7.0;

      /* 'LIB_limit:27' if (in > max) */
      if (the_off > 0.5) {
        /* 'LIB_limit:28' out = max; */
        the_off = 0.5;
      } else if (the_off < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        the_off = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      guidcmds->gbzc_FOV = ((0.5 * fac_FOV) * fabs(OPT_FP->GLimZlo)) * the_off;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3347' intz_FOV_lim = 0.120*(OPT_FP.GLimZup-OPT_FP.GLimZlo); */
    the_off = (OPT_FP->GLimZup - OPT_FP->GLimZlo) * 0.12;

    /* 'GUID_Embedded_Guid_Control_1D:3348' guidcmds.intz_FOV = LIB_limit(-intz_FOV_lim,intz_FOV_lim,sqrt(fac_FOV)*0.995*guidcmds.intz_FOV + guidcmds.gbzc_FOV*delt); */
    wbx_pos = sqrt(fac_FOV) * 0.995;
    wbx_neg = (wbx_pos * guidcmds->intz_FOV) + (guidcmds->gbzc_FOV * delt);

    /* 'LIB_limit:27' if (in > max) */
    if (wbx_neg > the_off) {
      /* 'LIB_limit:28' out = max; */
      guidcmds->intz_FOV = the_off;
    } else if (wbx_neg < (-the_off)) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      guidcmds->intz_FOV = -the_off;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      guidcmds->intz_FOV = wbx_neg;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3349' if (psi_off >= 0.0) */
    if (TTKF_in->psi_b >= 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:3350' guidcmds.gbyc_FOV = 0.5*fac_FOV*abs(OPT_FP.GLimYup)*LIB_limit( 0.0, 0.5,7.0*(psi_off - thresholdFOV)); */
      the_off = (TTKF_in->psi_b - (((real_T)fromDATA->seeker.Alin) - 0.0698)) *
        7.0;

      /* 'LIB_limit:27' if (in > max) */
      if (the_off > 0.5) {
        /* 'LIB_limit:28' out = max; */
        the_off = 0.5;
      } else if (the_off < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        the_off = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      guidcmds->gbyc_FOV = ((0.5 * fac_FOV) * fabs(OPT_FP->GLimYup)) * the_off;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3351' else */
      /* 'GUID_Embedded_Guid_Control_1D:3352' guidcmds.gbyc_FOV = 0.5*fac_FOV*abs(OPT_FP.GLimYlo)*LIB_limit(-0.5, 0.0,7.0*(psi_off + thresholdFOV)); */
      the_off = ((((real_T)fromDATA->seeker.Alin) - 0.0698) + TTKF_in->psi_b) *
        7.0;

      /* 'LIB_limit:27' if (in > max) */
      if (the_off > 0.0) {
        /* 'LIB_limit:28' out = max; */
        the_off = 0.0;
      } else if (the_off < -0.5) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        the_off = -0.5;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      guidcmds->gbyc_FOV = ((0.5 * fac_FOV) * fabs(OPT_FP->GLimYlo)) * the_off;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3354' inty_FOV_lim = 0.120*(OPT_FP.GLimYup-OPT_FP.GLimYlo); */
    the_off = (OPT_FP->GLimYup - OPT_FP->GLimYlo) * 0.12;

    /* 'GUID_Embedded_Guid_Control_1D:3355' guidcmds.inty_FOV = LIB_limit(-inty_FOV_lim,inty_FOV_lim,sqrt(fac_FOV)*0.995*guidcmds.inty_FOV + guidcmds.gbyc_FOV*delt); */
    fac_FOV = (wbx_pos * guidcmds->inty_FOV) + (guidcmds->gbyc_FOV * delt);

    /* 'LIB_limit:27' if (in > max) */
    if (fac_FOV > the_off) {
      /* 'LIB_limit:28' out = max; */
      guidcmds->inty_FOV = the_off;
    } else if (fac_FOV < (-the_off)) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      guidcmds->inty_FOV = -the_off;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      guidcmds->inty_FOV = fac_FOV;
    }
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3356' else */
    /* 'GUID_Embedded_Guid_Control_1D:3357' guidcmds.gbzc_FOV = 0.0; */
    guidcmds->gbzc_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3358' guidcmds.gbyc_FOV = 0.0; */
    guidcmds->gbyc_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3359' guidcmds.intz_FOV = 0.0; */
    guidcmds->intz_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3360' guidcmds.inty_FOV = 0.0; */
    guidcmds->inty_FOV = 0.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3363' if (guidmode <= 3) */
  if (((int32_T)guidmode) <= 3) {
    /* 'GUID_Embedded_Guid_Control_1D:3364' gbzc_unl = 0.0; */
    *gbzc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3365' gbyc_unl = 0.0; */
    *gbyc = 0.0;
  } else {
    switch (guidmode) {
     case 4:
      /* 'GUID_Embedded_Guid_Control_1D:3366' elseif (guidmode == 4) */
      /* 'GUID_Embedded_Guid_Control_1D:3368' guidcmds.AZy = guidcmds.AZy*0.99; */
      guidcmds->AZy *= 0.99;

      /* 'GUID_Embedded_Guid_Control_1D:3369' guidcmds.AZz = guidcmds.AZz*0.99; */
      guidcmds->AZz *= 0.99;

      /* 'GUID_Embedded_Guid_Control_1D:3370' gbzc_tmp = -guidcmds.intz - gains.Kp*clos.Rbz - gains.Kv*clos.Vbz - clos.Abz; */
      /* 'GUID_Embedded_Guid_Control_1D:3371' gbyc_tmp = -guidcmds.inty - gains.Kp*clos.Rby - gains.Kv*clos.Vby - clos.Aby; */
      /* 'GUID_Embedded_Guid_Control_1D:3373' if (opportunity_mode) */
      if (((int32_T)opportunity_mode) != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:3374' gbzc_unl = guidcmds.AZz*guid2AP.gbzc; */
        *gbzc = guidcmds->AZz * guid2AP->gbzc;

        /* 'GUID_Embedded_Guid_Control_1D:3375' gbyc_unl = guidcmds.AZy*guid2AP.gbyc; */
        *gbyc = guidcmds->AZy * guid2AP->gbyc;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3376' else */
        /* 'GUID_Embedded_Guid_Control_1D:3377' gbzc_unl = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*LIB_limit(-20.0,0.0,LIB_limit(0.0,1.0,10.0*(ENV.mach-0.60))*-15.0+0.1*gbzc_tmp); */
        Glim_glide = (ENV->mach - 0.6) * 10.0;

        /* 'LIB_limit:27' if (in > max) */
        if (Glim_glide > 1.0) {
          /* 'LIB_limit:28' out = max; */
          Glim_glide = 1.0;
        } else if (Glim_glide < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Glim_glide = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        Glim_glide = (((((-guidcmds->intz) - (gains_Kp * clos->Rbz)) - (1.2 *
          clos->Vbz)) - clos->Abz) * 0.1) + (Glim_glide * -15.0);

        /* 'LIB_limit:27' if (in > max) */
        if (Glim_glide > 0.0) {
          /* 'LIB_limit:28' out = max; */
          Glim_glide = 0.0;
        } else if (Glim_glide < -20.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Glim_glide = -20.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        *gbzc = ((1.0 - guidcmds->AZz) * Glim_glide) + (guidcmds->AZz *
          guid2AP->gbzc);

        /* 'GUID_Embedded_Guid_Control_1D:3378' gbyc_unl = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*0.1*gbyc_tmp; */
        *gbyc = (((((-guidcmds->inty) - (gains_Kp * clos->Rby)) - (1.2 *
                    clos->Vby)) - clos->Aby) * ((1.0 - guidcmds->AZy) * 0.1)) +
          (guidcmds->AZy * guid2AP->gbyc);
      }

      /* 'GUID_Embedded_Guid_Control_1D:3380' guidcmds.gbycD0 = 30.0*sign(psiLOSb); */
      if (psiLOSb < 0.0) {
        Glim_glide = -1.0;
      } else if (psiLOSb > 0.0) {
        Glim_glide = 1.0;
      } else if (psiLOSb == 0.0) {
        Glim_glide = 0.0;
      } else {
        Glim_glide = (rtNaN);
      }

      guidcmds->gbycD0 = 30.0 * Glim_glide;
      break;

     case 5:
      /* 'GUID_Embedded_Guid_Control_1D:3385' elseif (guidmode == 5) */
      /* 'GUID_Embedded_Guid_Control_1D:3386' if (reset) */
      if (((int32_T)reset) != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:3387' guidcmds.AZy = 1.0; */
        guidcmds->AZy = 1.0;

        /* 'GUID_Embedded_Guid_Control_1D:3387' guidcmds.AZz = 1.0; */
        guidcmds->AZz = 1.0;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3388' else */
        /* 'GUID_Embedded_Guid_Control_1D:3389' guidcmds.AZy = guidcmds.AZy*0.999; */
        guidcmds->AZy *= 0.999;

        /* 'GUID_Embedded_Guid_Control_1D:3390' guidcmds.AZz = guidcmds.AZz*0.999; */
        guidcmds->AZz *= 0.999;
      }

      /* 'GUID_Embedded_Guid_Control_1D:3392' guidcmds.gbzcD = LIB_limit(Glim_glide,10.0,-(1.0-clos.Kglide)*gains.Kfp*OPT_FP.FPerrD); */
      //the_off = ((-(1.0 - clos->Kglide)) * 150.0) * OPT_FP->FPerrD;
      the_off = ((-(1.0 - clos->Kglide)) * 50.0) * OPT_FP->FPerrD;

      /* 'LIB_limit:27' if (in > max) */
      if (the_off > 10.0) {
        /* 'LIB_limit:28' out = max; */
        guidcmds->gbzcD = 10.0;
      } else if (the_off < Glim_glide) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        guidcmds->gbzcD = Glim_glide;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        guidcmds->gbzcD = the_off;
      }

      /* 'GUID_Embedded_Guid_Control_1D:3397' gbzc_unl = guidcmds.AZz*guid2AP.gbzc; */
      *gbzc = guidcmds->AZz * guid2AP->gbzc;

      /* 'GUID_Embedded_Guid_Control_1D:3398' gbyc_unl = guidcmds.AZy*guid2AP.gbyc; */
      *gbyc = guidcmds->AZy * guid2AP->gbyc;
      break;

     default:
      if (((int32_T)guidmode) < 9) {
        /* 'GUID_Embedded_Guid_Control_1D:3402' elseif (guidmode < 9) */
        /* 'GUID_Embedded_Guid_Control_1D:3403' if (guidmode == 6) */
        if (((int32_T)guidmode) == 6) {
          /* 'GUID_Embedded_Guid_Control_1D:3406' if (guidcmds.gmode_sw == 1) */
          if (((int32_T)guidcmds->gmode_sw) == 1) {
            /* 'GUID_Embedded_Guid_Control_1D:3411' guid2AP.gbzc = IMUin.gbzm; */
            guid2AP->gbzc = IMUin.gbzm;
          }

          /* 'GUID_Embedded_Guid_Control_1D:3413' if (reset) */
          if (((int32_T)reset) != 0) {
            /* 'GUID_Embedded_Guid_Control_1D:3414' guidcmds.AZy = 1.0; */
            guidcmds->AZy = 1.0;

            /* 'GUID_Embedded_Guid_Control_1D:3414' guidcmds.AZz = 1.0; */
            guidcmds->AZz = 1.0;
          } else {
            /* 'GUID_Embedded_Guid_Control_1D:3415' else */
            /* 'GUID_Embedded_Guid_Control_1D:3416' guidcmds.AZy = guidcmds.AZy*0.999; */
            guidcmds->AZy *= 0.999;

            /* 'GUID_Embedded_Guid_Control_1D:3417' guidcmds.AZz = guidcmds.AZz*0.999; */
            guidcmds->AZz *= 0.999;
          }
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3419' else */
          /* 'GUID_Embedded_Guid_Control_1D:3420' if (reset) */
          if (((int32_T)reset) != 0) {
            /* 'GUID_Embedded_Guid_Control_1D:3421' guidcmds.AZy = 1.0; */
            guidcmds->AZy = 1.0;

            /* 'GUID_Embedded_Guid_Control_1D:3421' guidcmds.AZz = 1.0; */
            guidcmds->AZz = 1.0;
          } else {
            /* 'GUID_Embedded_Guid_Control_1D:3422' else */
            /* 'GUID_Embedded_Guid_Control_1D:3423' guidcmds.AZy = guidcmds.AZy*0.998; */
            guidcmds->AZy *= 0.998;

            /* 'GUID_Embedded_Guid_Control_1D:3424' guidcmds.AZz = guidcmds.AZz*0.998; */
            guidcmds->AZz *= 0.998;
          }
        }

        /* 'GUID_Embedded_Guid_Control_1D:3427' guidcmds.intz = guidcmds.intz + gains.Ki*clos.Rbz; */
        guidcmds->intz += 0.0 * clos->Rbz;

        /* 'GUID_Embedded_Guid_Control_1D:3428' guidcmds.gbzcD = LIB_limit(Glim_glide,10.0,-(1.0-clos.Kglide)*gains.Kfp*OPT_FP.FPerrD - 9.81); */
        the_off = ((-(1.0 - clos->Kglide)) * 50.0) * OPT_FP->FPerrD;
        fac_FOV = (((-(1.0 - clos->Kglide)) * 50.0) * OPT_FP->FPerrD) - 9.81;

        /* 'LIB_limit:27' if (in > max) */
        if ((the_off - 9.81) > 10.0) {
          /* 'LIB_limit:28' out = max; */
          fac_FOV = 10.0;
        } else if ((the_off - 9.81) < Glim_glide) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          fac_FOV = Glim_glide;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }

        guidcmds->gbzcD = fac_FOV;

        /* 'GUID_Embedded_Guid_Control_1D:3429' gbc = DCM.NED2BODY(:,3)*guidcmds.gbzcD; */
        /* 'GUID_Embedded_Guid_Control_1D:3430' gbzc_tmp = gbc(3) - guidcmds.intz - gains.Kp*clos.Rbz - gains.Kv*clos.Vbz - clos.Abz; */
        /* 'GUID_Embedded_Guid_Control_1D:3431' gbyc_tmp = gbc(2) - guidcmds.inty - gains.Kp*clos.Rby - gains.Kv*clos.Vby - clos.Aby; */
        /* 'GUID_Embedded_Guid_Control_1D:3432' gbzc_unl = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*gbzc_tmp; */
        Gned_clos[0] = -gains_Kp * clos->Rned[0] - 1.2 * clos->Vned[0]- clos->Aned[0];
        Gned_clos[1] = -gains_Kp * clos->Rned[1] - 1.2 * clos->Vned[1]- clos->Aned[1];
        Gned_clos[2] = -gains_Kp * clos->Rned[2] - 1.2 * clos->Vned[2]- clos->Aned[2];

        for (count = 0; count < 3; count++) {
            Gbody_clos[count] = (DCM->NED2BODY[count + 6] * Gned_clos[2]) + ((DCM->NED2BODY[count + 3] * Gned_clos[1]) + (DCM->NED2BODY[count]  * Gned_clos[0]));            
        }

           //*gbzc = ((((((DCM->NED2BODY[8] * fac_FOV) - guidcmds->intz) - (gains_Kp * clos->Rbz)) 
           //           - (1.2 * clos->Vbz)) - clos->Abz) * (1.0 - guidcmds->AZz))  + (guidcmds->AZz * guid2AP->gbzc);

           *gbzc = ((((DCM->NED2BODY[8] * fac_FOV) - guidcmds->intz) + Gbody_clos[2]) * (1.0 - guidcmds->AZz)) + (guidcmds->AZz * guid2AP->gbzc);

          /* 'GUID_Embedded_Guid_Control_1D:3433' gbyc_unl = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*gbyc_tmp; */
            //Glim_glide = ((((DCM->NED2BODY[7] * fac_FOV) - guidcmds->inty) -
            //               (gains_Kp * clos->Rby)) - (1.2 * clos->Vby)) - clos->Aby;
           
           Glim_glide = (((DCM->NED2BODY[7] * fac_FOV) - guidcmds->inty) + Gbody_clos[1]);
            
           *gbyc = (Glim_glide * (1.0 - guidcmds->AZy)) + (guidcmds->AZy * guid2AP->gbyc);
            

        /* 'GUID_Embedded_Guid_Control_1D:3434' if (guidmode == 6) */
        if (((int32_T)guidmode) == 6) {
          /* 'GUID_Embedded_Guid_Control_1D:3435' temp_gbyc = gbc(2) - guidcmds.inty - gains.Kp*clos.Rby - gains.Kv*clos.Vby - clos.Aby; */
          *temp_gbyc = Glim_glide;
        }
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3438' else */
        /* 'GUID_Embedded_Guid_Control_1D:3439' if (guidcmds.PurePropNav) */
        if (((int32_T)guidcmds->PurePropNav) != 0) {
          /* 'GUID_Embedded_Guid_Control_1D:3440' guidcmds.gb_ratio = LIB_limit(guidcmds.gb_ratio_prev-0.005,guidcmds.gb_ratio_prev+0.005,1.0); */
          /* 'LIB_limit:27' if (in > max) */
          if (1.0 > (guidcmds->gb_ratio_prev + 0.005)) {
            /* 'LIB_limit:28' out = max; */
            Glim_glide = guidcmds->gb_ratio_prev + 0.005;
          } else if (1.0 < (guidcmds->gb_ratio_prev - 0.005)) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            Glim_glide = guidcmds->gb_ratio_prev - 0.005;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            Glim_glide = 1.0;
          }

          guidcmds->gb_ratio = Glim_glide;
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3441' else */
          /* 'GUID_Embedded_Guid_Control_1D:3443' guidcmds.gb_ratio = LIB_limit(guidcmds.gb_ratio_prev-0.0025,guidcmds.gb_ratio_prev+0.0025,... */
          /* 'GUID_Embedded_Guid_Control_1D:3444'                                      1.0*LIB_limit(0.0,1.0,-guidcmds.GndRge/(5000.0-guid_settings.Rterm) + 1.667)); */
          Glim_glide = ((-guidcmds->GndRge) / 3000.0) + 1.667;

          /* 'LIB_limit:27' if (in > max) */
          if (Glim_glide > 1.0) {
            /* 'LIB_limit:28' out = max; */
            Glim_glide = 1.0;
          } else if (Glim_glide < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            Glim_glide = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'LIB_limit:27' if (in > max) */
          if (Glim_glide > (guidcmds->gb_ratio_prev + 0.0025)) {
            /* 'LIB_limit:28' out = max; */
            Glim_glide = guidcmds->gb_ratio_prev + 0.0025;
          } else if (Glim_glide < (guidcmds->gb_ratio_prev - 0.0025)) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            Glim_glide = guidcmds->gb_ratio_prev - 0.0025;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          guidcmds->gb_ratio = Glim_glide;
        }

        /* 'GUID_Embedded_Guid_Control_1D:3446' guidcmds.gb_ratio_prev = guidcmds.gb_ratio; */
        guidcmds->gb_ratio_prev = guidcmds->gb_ratio;

        /* 'GUID_Embedded_Guid_Control_1D:3447' guidcmds.AZy = guidcmds.AZy*0.99; */
        guidcmds->AZy *= 0.99;

        /* 'GUID_Embedded_Guid_Control_1D:3448' guidcmds.AZz = guidcmds.AZz*0.99; */
        guidcmds->AZz *= 0.99;

        /* 'GUID_Embedded_Guid_Control_1D:3449' gbzc_clos = -gains.Kv*clos.Vbz - clos.Abz - ENV.GRAVbody(3); */
        /* 'GUID_Embedded_Guid_Control_1D:3450' gbyc_clos = -gains.Kv*clos.Vby - clos.Aby - ENV.GRAVbody(2); */
        /* 'GUID_Embedded_Guid_Control_1D:3453' gbzc_pnav = -TTKF_in.dthe_b*3.5*ENV.Vnav - ENV.GRAVbody(3); */
        /* 'GUID_Embedded_Guid_Control_1D:3454' gbyc_pnav =  TTKF_in.dpsi_b*3.5*ENV.Vnav - ENV.GRAVbody(2); */
        /* 'GUID_Embedded_Guid_Control_1D:3456' gbzc_unl = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*((1.0-guidcmds.gb_ratio)*gbzc_clos + guidcmds.gb_ratio*gbzc_pnav); */
        *gbzc = ((((((-1.2 * clos->Vbz) - clos->Abz) - ENV->GRAVbody[2]) * (1.0
                    - guidcmds->gb_ratio)) + (((((-TTKF_in->dthe_b) * 3.5) *
          ENV->Vnav) - ENV->GRAVbody[2]) * guidcmds->gb_ratio)) * (1.0 -
                  guidcmds->AZz)) + (guidcmds->AZz * guid2AP->gbzc);

        /* 'GUID_Embedded_Guid_Control_1D:3457' gbyc_unl = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*((1.0-guidcmds.gb_ratio)*gbyc_clos + guidcmds.gb_ratio*gbyc_pnav); */
        *gbyc = ((((((-1.2 * clos->Vby) - clos->Aby) - ENV->GRAVbody[1]) * (1.0
                    - guidcmds->gb_ratio)) + ((((TTKF_in->dpsi_b * 3.5) *
          ENV->Vnav) - ENV->GRAVbody[1]) * guidcmds->gb_ratio)) * (1.0 -
                  guidcmds->AZy)) + (guidcmds->AZy * guid2AP->gbyc);
      }
      break;
    }
  }

  /* 'GUID_Embedded_Guid_Control_1D:3473' guidcmds.gbyc_unl =  gbyc_unl; */
  guidcmds->gbyc_unl = *gbyc;

  /* 'GUID_Embedded_Guid_Control_1D:3474' guidcmds.gbzc_unl =  gbzc_unl; */
  guidcmds->gbzc_unl = *gbzc;

  /* 'GUID_Embedded_Guid_Control_1D:3475' guidcmds.gizc     =  gbzc_unl*cos(guidcmds.phim) + gbyc_unl*sin(guidcmds.phim); */
  guidcmds->gizc = ((*gbzc) * cos(guidcmds->phim)) + ((*gbyc) * sin
    (guidcmds->phim));

  /* 'GUID_Embedded_Guid_Control_1D:3476' guidcmds.giyc     = -gbzc_unl*sin(guidcmds.phim) + gbyc_unl*cos(guidcmds.phim); */
  guidcmds->giyc = ((-(*gbzc)) * sin(guidcmds->phim)) + ((*gbyc) * cos
    (guidcmds->phim));

  /* 'GUID_Embedded_Guid_Control_1D:3482' epsmedian = TTKF_in.themedian - TTKF_in.the_b; */
  Glim_glide = TTKF_in->themedian - TTKF_in->the_b;

  /* 'GUID_Embedded_Guid_Control_1D:3483' etamedian = TTKF_in.psimedian - TTKF_in.psi_b; */
  fac_FOV = TTKF_in->psimedian - TTKF_in->psi_b;

  /* 'GUID_Embedded_Guid_Control_1D:3484' eta_prop = LIB_limit(0.0,0.262,abs(etamedian) - 0.08726)*sign(etamedian); */
  the_off = fabs(fac_FOV);

  /* 'LIB_limit:27' if (in > max) */
  if ((the_off - 0.08726) > 0.262) {
    /* 'LIB_limit:28' out = max; */
    the_off = 0.262;
  } else if ((the_off - 0.08726) < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    the_off = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    the_off -= 0.08726;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3485' eps_prop = LIB_limit(0.0,0.262,abs(epsmedian) - 0.08726)*sign(epsmedian); */
  wbx_neg = fabs(Glim_glide);

  /* 'LIB_limit:27' if (in > max) */
  if ((wbx_neg - 0.08726) > 0.262) {
    /* 'LIB_limit:28' out = max; */
    wbx_neg = 0.262;
  } else if ((wbx_neg - 0.08726) < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    wbx_neg = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    wbx_neg -= 0.08726;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3486' guidcmds.eta_int  = LIB_limit(0.0,0.262,guidcmds.eta_int + eta_prop*delt); */
  if (fac_FOV < 0.0) {
    fac_FOV = -1.0;
  } else if (fac_FOV > 0.0) {
    fac_FOV = 1.0;
  } else if (fac_FOV == 0.0) {
    fac_FOV = 0.0;
  } else {
    fac_FOV = (rtNaN);
  }

  fac_FOV = ((the_off * fac_FOV) * delt) + guidcmds->eta_int;

  /* 'LIB_limit:27' if (in > max) */
  if (fac_FOV > 0.262) {
    /* 'LIB_limit:28' out = max; */
    guidcmds->eta_int = 0.262;
  } else if (fac_FOV < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    guidcmds->eta_int = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    guidcmds->eta_int = fac_FOV;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3487' guidcmds.eps_int  = LIB_limit(0.0,0.262,guidcmds.eps_int + eps_prop*delt); */
  if (Glim_glide < 0.0) {
    Glim_glide = -1.0;
  } else if (Glim_glide > 0.0) {
    Glim_glide = 1.0;
  } else if (Glim_glide == 0.0) {
    Glim_glide = 0.0;
  } else {
    Glim_glide = (rtNaN);
  }

  Glim_glide = ((wbx_neg * Glim_glide) * delt) + guidcmds->eps_int;

  /* 'LIB_limit:27' if (in > max) */
  if (Glim_glide > 0.262) {
    /* 'LIB_limit:28' out = max; */
    guidcmds->eps_int = 0.262;
  } else if (Glim_glide < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    guidcmds->eps_int = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    guidcmds->eps_int = Glim_glide;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3490' if (AFmode == 1) */
  switch (AFmode) {
   case 1:
    /* 'GUID_Embedded_Guid_Control_1D:3491' gbyc = LIB_limit( -0.1, 0.1,gbyc_unl); */
    /* 'LIB_limit:27' if (in > max) */
    if ((*gbyc) > 0.1) {
      /* 'LIB_limit:28' out = max; */
      *gbyc = 0.1;
    } else if ((*gbyc) < -0.1) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      *gbyc = -0.1;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:3492' gbzc = LIB_limit(-20.0, 0.0,gbzc_unl); */
    /* 'LIB_limit:27' if (in > max) */
    if ((*gbzc) > 0.0) {
      /* 'LIB_limit:28' out = max; */
      *gbzc = 0.0;
    } else if ((*gbzc) < -20.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      *gbzc = -20.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
    break;

   case 2:
    /* 'GUID_Embedded_Guid_Control_1D:3493' elseif (AFmode == 2) */
    /* 'GUID_Embedded_Guid_Control_1D:3494' gbyc = 0.0; */
    *gbyc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3495' gbzc = LIB_limit(-20.0,10.0,gbzc_unl); */
    /* 'LIB_limit:27' if (in > max) */
    if ((*gbzc) > 10.0) {
      /* 'LIB_limit:28' out = max; */
      *gbzc = 10.0;
    } else if ((*gbzc) < -20.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      *gbzc = -20.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
    break;

   default:
    /* 'GUID_Embedded_Guid_Control_1D:3496' else */
    /* 'GUID_Embedded_Guid_Control_1D:3497' gbyc = LIB_limit(OPT_FP.GLimYlo,OPT_FP.GLimYup,gbyc_unl) + guidcmds.gbyc_FOV + 3.0*guidcmds.inty_FOV; */
    /* 'LIB_limit:27' if (in > max) */
    if ((*gbyc) > OPT_FP->GLimYup) {
      /* 'LIB_limit:28' out = max; */
      *gbyc = OPT_FP->GLimYup;
    } else if ((*gbyc) < OPT_FP->GLimYlo) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      *gbyc = OPT_FP->GLimYlo;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    *gbyc = ((*gbyc) + guidcmds->gbyc_FOV) + (3.0 * guidcmds->inty_FOV);

    /* 'GUID_Embedded_Guid_Control_1D:3498' gbzc = LIB_limit(OPT_FP.GLimZlo,OPT_FP.GLimZup,gbzc_unl) + guidcmds.gbzc_FOV + 3.0*guidcmds.intz_FOV; */
    /* 'LIB_limit:27' if (in > max) */
    if ((*gbzc) > OPT_FP->GLimZup) {
      /* 'LIB_limit:28' out = max; */
      *gbzc = OPT_FP->GLimZup;
    } else if ((*gbzc) < OPT_FP->GLimZlo) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      *gbzc = OPT_FP->GLimZlo;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    *gbzc = ((*gbzc) + guidcmds->gbzc_FOV) + (3.0 * guidcmds->intz_FOV);
    break;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3500' gbc = sqrt(gbyc^2+gbzc^2); */
  /* 'GUID_Embedded_Guid_Control_1D:3508' if (opportunity_mode) */
  if (((int32_T)opportunity_mode) != 0) {
    /* 'GUID_Embedded_Guid_Control_1D:3509' philim = 180.0*d2r; */
    Glim_glide = 3.1415926535897931;
  } else if ((((int32_T)bunt_arc) != 0) || (((int32_T)AFmode) == 2)) {
    /* 'GUID_Embedded_Guid_Control_1D:3510' elseif (bunt_arc || AFmode == 2) */
    /* 'GUID_Embedded_Guid_Control_1D:3511' philim = 70.0*d2r; */
    Glim_glide = 1.2217304763960306;
  } else if (((int32_T)AFmode) == 1) {
    /* 'GUID_Embedded_Guid_Control_1D:3512' elseif (AFmode == 1) */
    /* 'GUID_Embedded_Guid_Control_1D:3513' philim = 20.0*d2r; */
    Glim_glide = 0.3490658503988659;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3514' else */
    /* 'GUID_Embedded_Guid_Control_1D:3515' philim = 45.0*d2r; */
    Glim_glide = 0.78539816339744828;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3517' SRlim  = 100.0*d2r*delt; */
  fac_FOV = 1.7453292519943295 * delt;

  /* 'GUID_Embedded_Guid_Control_1D:3520' if (AFmode == 0 || AFmode == 1 || AFmode == 3) */
  if (((((int32_T)AFmode) == 0) || (((int32_T)AFmode) == 1)) || (((int32_T)
        AFmode) == 3)) {
    /* 'GUID_Embedded_Guid_Control_1D:3522' if (guidcmds.verticaldive && MPin.AttackHeadingValid) */
    if ((((int32_T)guidcmds->verticaldive) != 0) && (((int32_T)
          MPin->AttackHeadingValid) != 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:3523' phic = guidcmds.phi_attack; */
      *phic = guidcmds->phi_attack;

      /* 'GUID_Embedded_Guid_Control_1D:3524' guidcmds.phic_hold = phic; */
      guidcmds->phic_hold = guidcmds->phi_attack;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3525' else */
      /* 'GUID_Embedded_Guid_Control_1D:3526' if (guidmode <= 1) */
      if (((int32_T)guidmode) <= 1) {
        /* 'GUID_Embedded_Guid_Control_1D:3531' phic = guidcmds.phim; */
        *phic = guidcmds->phim;

        /* 'GUID_Embedded_Guid_Control_1D:3532' guidcmds.phic_hold = phic; */
        guidcmds->phic_hold = guidcmds->phim;

        /* 'GUID_Embedded_Guid_Control_1D:3533' guidcmds.phic_prev = phic; */
        guidcmds->phic_prev = guidcmds->phim;
      } else if ((((int32_T)guidmode) <= 3) || (((int32_T)guidmode) == 5)) {
        /* 'GUID_Embedded_Guid_Control_1D:3534' elseif (guidmode <= 3 || guidmode == 5) */
        /* 'GUID_Embedded_Guid_Control_1D:3535' if (guidmode == 2) */
        if (((int32_T)guidmode) == 2) {
          /* 'GUID_Embedded_Guid_Control_1D:3537' CMX = 0.70*interp1(machvec,CMXdefl,ENV.mach,'linear','extrap'); */
          /* 'GUID_Embedded_Guid_Control_1D:3538' if (Jxx > 0.001) */
          if (((real_T)fromDATA->AirframeProp.Jxx) > 0.001) {
            /* 'GUID_Embedded_Guid_Control_1D:3539' wdot_max = (CMX*ENV.Q*Sref*Xref)/Jxx; */
            for (b_dT = 0; b_dT < 7; b_dT++) {
              fromDATA_0[b_dT] = (real_T)fromDATA->AirframeProp.machvec[b_dT];
              fromDATA_1[b_dT] = (real_T)fromDATA->AirframeProp.CMXdefl[b_dT];
            }

            Glim_glide = ((((0.7 * interp1_Gy1LATh1(fromDATA_0, fromDATA_1,
              ENV->mach)) * ENV->Q) * ((real_T)fromDATA->AirframeProp.Sref)) *
                          ((real_T)fromDATA->AirframeProp.Xref)) / ((real_T)
              fromDATA->AirframeProp.Jxx);
          } else {
            /* 'GUID_Embedded_Guid_Control_1D:3540' else */
            /* 'GUID_Embedded_Guid_Control_1D:3541' wdot_max = 100.0; */
            Glim_glide = 100.0;
          }

          /* 'GUID_Embedded_Guid_Control_1D:3543' phi_neg_prev = guidcmds.phim; */
          fac_FOV = guidcmds->phim;

          /* 'GUID_Embedded_Guid_Control_1D:3544' phi_pos_prev = guidcmds.phim; */
          the_off = guidcmds->phim;

          /* 'GUID_Embedded_Guid_Control_1D:3545' if (delt > 0.001) */
          if (delt > 0.001) {
            /* 'GUID_Embedded_Guid_Control_1D:3546' wbx_neg = IMUin.wbxm; */
            wbx_neg = IMUin.wbxm;

            /* 'GUID_Embedded_Guid_Control_1D:3547' wbx_pos = IMUin.wbxm; */
            wbx_pos = IMUin.wbxm;
          } else {
            /* 'GUID_Embedded_Guid_Control_1D:3548' else */
            /* 'GUID_Embedded_Guid_Control_1D:3549' wbx_neg = 0.0; */
            wbx_neg = 0.0;

            /* 'GUID_Embedded_Guid_Control_1D:3550' wbx_pos = 0.0; */
            wbx_pos = 0.0;
          }

          /* 'GUID_Embedded_Guid_Control_1D:3552' guidcmds.dT = 0.0; */
          guidcmds->dT = 0.0;

          /* 'GUID_Embedded_Guid_Control_1D:3554' for dT = 0:0.05:2.0 */
          b_dT = 0;
          exitg1 = false;
          while ((!exitg1) && (b_dT < 41)) {
            dT = ((real_T)b_dT) * 0.05;

            /* 'GUID_Embedded_Guid_Control_1D:3555' wbx_neg = LIB_limit(-6.981,6.981,wbx_neg - wdot_max*0.05); */
            wbx_neg -= Glim_glide * 0.05;

            /* 'LIB_limit:27' if (in > max) */
            if (wbx_neg > 6.981) {
              /* 'LIB_limit:28' out = max; */
              wbx_neg = 6.981;
            } else if (wbx_neg < -6.981) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              wbx_neg = -6.981;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }

            /* 'GUID_Embedded_Guid_Control_1D:3556' wbx_pos = LIB_limit(-6.981,6.981,wbx_pos + wdot_max*0.05); */
            wbx_pos += Glim_glide * 0.05;

            /* 'LIB_limit:27' if (in > max) */
            if (wbx_pos > 6.981) {
              /* 'LIB_limit:28' out = max; */
              wbx_pos = 6.981;
            } else if (wbx_pos < -6.981) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              wbx_pos = -6.981;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }

            /* 'GUID_Embedded_Guid_Control_1D:3557' phi_neg = phi_neg_prev + wbx_neg*0.05; */
            phi_neg = (wbx_neg * 0.05) + fac_FOV;

            /* 'GUID_Embedded_Guid_Control_1D:3558' phi_pos = phi_pos_prev + wbx_pos*0.05; */
            phi_pos = (wbx_pos * 0.05) + the_off;

            /* 'GUID_Embedded_Guid_Control_1D:3560' if (sign(phi_neg) ~= sign(phi_neg_prev) || sign(phi_pos) ~= sign(phi_pos_prev)) */
            if (phi_neg < 0.0) {
              phi_neg_0 = -1.0;
            } else if (phi_neg > 0.0) {
              phi_neg_0 = 1.0;
            } else if (phi_neg == 0.0) {
              phi_neg_0 = 0.0;
            } else {
              phi_neg_0 = (rtNaN);
            }

            if (fac_FOV < 0.0) {
              fac_FOV = -1.0;
            } else if (fac_FOV > 0.0) {
              fac_FOV = 1.0;
            } else if (fac_FOV == 0.0) {
              fac_FOV = 0.0;
            } else {
              fac_FOV = (rtNaN);
            }

            if (phi_neg_0 != fac_FOV) {
              /* 'GUID_Embedded_Guid_Control_1D:3561' guidcmds.dT = 0.0; */
              guidcmds->dT = 0.0;
              exitg1 = true;
            } else {
              if (phi_pos < 0.0) {
                fac_FOV = -1.0;
              } else if (phi_pos > 0.0) {
                fac_FOV = 1.0;
              } else if (phi_pos == 0.0) {
                fac_FOV = 0.0;
              } else {
                fac_FOV = (rtNaN);
              }

              if (the_off < 0.0) {
                the_off = -1.0;
              } else if (the_off > 0.0) {
                the_off = 1.0;
              } else if (the_off == 0.0) {
                the_off = 0.0;
              } else {
                the_off = (rtNaN);
              }

              if (fac_FOV != the_off) {
                /* 'GUID_Embedded_Guid_Control_1D:3561' guidcmds.dT = 0.0; */
                guidcmds->dT = 0.0;
                exitg1 = true;
              } else {
                /* 'GUID_Embedded_Guid_Control_1D:3564' if (phi_neg < -6.283) */
                if (phi_neg < -6.283) {
                  /* 'GUID_Embedded_Guid_Control_1D:3565' guidcmds.dT = -dT; */
                  guidcmds->dT = -dT;
                  exitg1 = true;
                } else {
                  /* 'GUID_Embedded_Guid_Control_1D:3568' if (phi_pos >  6.283) */
                  if (phi_pos > 6.283) {
                    /* 'GUID_Embedded_Guid_Control_1D:3569' guidcmds.dT =  dT; */
                    guidcmds->dT = dT;
                    exitg1 = true;
                  } else {
                    /* 'GUID_Embedded_Guid_Control_1D:3572' phi_neg_prev = phi_neg; */
                    fac_FOV = phi_neg;

                    /* 'GUID_Embedded_Guid_Control_1D:3573' phi_pos_prev = phi_pos; */
                    the_off = phi_pos;
                    b_dT++;
                  }
                }
              }
            }
          }
        }

        /* 'GUID_Embedded_Guid_Control_1D:3576' if (guidmode == 5) */
        if (((int32_T)guidmode) == 5) {
          /* 'GUID_Embedded_Guid_Control_1D:3579' phic = guidcmds.phic_hold; */
          *phic = guidcmds->phic_hold;

          /* 'GUID_Embedded_Guid_Control_1D:3580' guidcmds.phic_prev = guidcmds.phic_hold; */
          guidcmds->phic_prev = guidcmds->phic_hold;
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3581' else */
          /* 'GUID_Embedded_Guid_Control_1D:3583' phic = 0.0 + sign(guidcmds.dT)*6.283185; */
          if (guidcmds->dT < 0.0) {
            Glim_glide = -1.0;
          } else if (guidcmds->dT > 0.0) {
            Glim_glide = 1.0;
          } else if (guidcmds->dT == 0.0) {
            Glim_glide = 0.0;
          } else {
            Glim_glide = (rtNaN);
          }

          *phic = Glim_glide * 6.283185;

          /* 'GUID_Embedded_Guid_Control_1D:3584' if ((abs(guidcmds.phim) > 1.571) && ... */
          /* 'GUID_Embedded_Guid_Control_1D:3585'               (sign(guidcmds.phim) ~= sign(guidcmds.phic_hold))) */
          if (fabs(guidcmds->phim) > 1.571) {
            if (guidcmds->phim < 0.0) {
              Glim_glide = -1.0;
            } else if (guidcmds->phim > 0.0) {
              Glim_glide = 1.0;
            } else if (guidcmds->phim == 0.0) {
              Glim_glide = 0.0;
            } else {
              Glim_glide = (rtNaN);
            }

            if (guidcmds->phic_hold < 0.0) {
              the_off = -1.0;
            } else if (guidcmds->phic_hold > 0.0) {
              the_off = 1.0;
            } else if (guidcmds->phic_hold == 0.0) {
              the_off = 0.0;
            } else {
              the_off = (rtNaN);
            }

            if (Glim_glide != the_off) {
              /* 'GUID_Embedded_Guid_Control_1D:3586' phic = 0.0; */
              *phic = 0.0;

              /* 'GUID_Embedded_Guid_Control_1D:3587' guidcmds.dT = 0.0; */
              guidcmds->dT = 0.0;
            }
          }

          /* 'GUID_Embedded_Guid_Control_1D:3589' guidcmds.phic_hold = guidcmds.phim; */
          guidcmds->phic_hold = guidcmds->phim;

          /* 'GUID_Embedded_Guid_Control_1D:3590' guidcmds.phic_prev = guidcmds.phic_hold; */
          guidcmds->phic_prev = guidcmds->phic_hold;
        }
      } else if ((((int32_T)guidmode) == 4) || ((((int32_T)guidmode) >= 6) &&
                  (((int32_T)guidmode) < 9))) {
        /* 'GUID_Embedded_Guid_Control_1D:3592' elseif (guidmode == 4 || (guidmode >= 6 && guidmode < 9)) */
        /* 'GUID_Embedded_Guid_Control_1D:3593' if (guidmode == 6 || opportunity_mode) */
        if ((((int32_T)guidmode) == 6) || (((int32_T)opportunity_mode) != 0)) {
          /* 'GUID_Embedded_Guid_Control_1D:3595' guidcmds.phic_hold = guidcmds.phic_hold*0.995; */
          guidcmds->phic_hold *= 0.995;
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3596' elseif (guidmode < 9) */
          /* 'GUID_Embedded_Guid_Control_1D:3598' guidcmds.phic_hold = LIB_limit(-philim,philim,guidcmds.phim); */
          /* 'LIB_limit:27' if (in > max) */
          if (guidcmds->phim > Glim_glide) {
            /* 'LIB_limit:28' out = max; */
            the_off = Glim_glide;
          } else if (guidcmds->phim < (-Glim_glide)) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            the_off = -Glim_glide;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            the_off = guidcmds->phim;
          }

          guidcmds->phic_hold = the_off;
        }

        /* 'GUID_Embedded_Guid_Control_1D:3603' if (OPT_FP.GLim-gbc < 0.1*OPT_FP.GLim) */
        if ((OPT_FP->GLim - sqrt(((*gbyc) * (*gbyc)) + ((*gbzc) * (*gbzc)))) <
            (0.1 * OPT_FP->GLim)) {
          /* 'GUID_Embedded_Guid_Control_1D:3604' guidcmds.AZphi = LIB_limit(0.0,1.0,guidcmds.AZphi + 0.005); */
          /* 'LIB_limit:27' if (in > max) */
          if ((guidcmds->AZphi + 0.005) > 1.0) {
            /* 'LIB_limit:28' out = max; */
            the_off = 1.0;
          } else if ((guidcmds->AZphi + 0.005) < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            the_off = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            the_off = guidcmds->AZphi + 0.005;
          }

          guidcmds->AZphi = the_off;
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:3605' else */
          /* 'GUID_Embedded_Guid_Control_1D:3606' guidcmds.AZphi = LIB_limit(0.0,1.0,guidcmds.AZphi - 0.002); */
          /* 'LIB_limit:27' if (in > max) */
          if ((guidcmds->AZphi - 0.002) > 1.0) {
            /* 'LIB_limit:28' out = max; */
            the_off = 1.0;
          } else if ((guidcmds->AZphi - 0.002) < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            the_off = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            the_off = guidcmds->AZphi - 0.002;
          }

          guidcmds->AZphi = the_off;
        }

        /* 'GUID_Embedded_Guid_Control_1D:3609' if (TTKF_in.tgtrge < 500.0) */
        if (TTKF_in->tgtrge < 500.0) {
          /* 'GUID_Embedded_Guid_Control_1D:3610' guidcmds.AZphi = 0.0; */
          guidcmds->AZphi = 0.0;
        }

        /* 'GUID_Embedded_Guid_Control_1D:3612' guidcmds.phic_hold = (1.0-guidcmds.AZphi)*guidcmds.phic_hold + guidcmds.AZphi*guidcmds.phim; */
        guidcmds->phic_hold = ((1.0 - guidcmds->AZphi) * guidcmds->phic_hold) +
          (guidcmds->AZphi * guidcmds->phim);

        /* 'GUID_Embedded_Guid_Control_1D:3614' [phic,phi_gic,phi_int] = LIB_roll2gvec(guidcmds.phic_hold,guidcmds.phic_prev,guidcmds.phim,guidcmds.giyc,guidcmds.gizc, ... */
        /* 'GUID_Embedded_Guid_Control_1D:3615' 	                                           guidcmds.phi_int,guidcmds.AZphi,philim,SRlim,OPT_FP.GLim); */
        LIB_roll2gvec_MgzTYQaE(guidcmds->phic_hold, guidcmds->phic_prev,
          guidcmds->phim, guidcmds->giyc, guidcmds->gizc, &guidcmds->phi_int,
          guidcmds->AZphi, Glim_glide, fac_FOV, OPT_FP->GLim, phic,
          &guidcmds->phi_gic);

        /* 'GUID_Embedded_Guid_Control_1D:3617' guidcmds.phi_gic   = phi_gic; */
        /* 'GUID_Embedded_Guid_Control_1D:3618' guidcmds.phi_int   = phi_int; */
        /* 'GUID_Embedded_Guid_Control_1D:3619' guidcmds.phic_prev = phic; */
        // Force phic to be zero (Skid-to-turn always)
        *phic = 0.0 ;
        guidcmds->phic_hold = *phic;
        guidcmds->phic_prev = *phic;

      } else {
        /* 'GUID_Embedded_Guid_Control_1D:3620' else */
        /* 'GUID_Embedded_Guid_Control_1D:3622' guidcmds.phic_hold = guidcmds.phic_hold*0.99; */
        guidcmds->phic_hold *= 0.99;

        /* 'GUID_Embedded_Guid_Control_1D:3623' phic = guidcmds.phic_hold; */
        *phic = guidcmds->phic_hold;

        /* 'GUID_Embedded_Guid_Control_1D:3624' guidcmds.phic_prev = phic; */
        guidcmds->phic_prev = guidcmds->phic_hold;
      }
    }
  } else if (((int32_T)AFmode) == 2) {
    /* 'GUID_Embedded_Guid_Control_1D:3629' elseif (AFmode == 2) */
    /* 'GUID_Embedded_Guid_Control_1D:3631' guidcmds.phic_hold = LIB_limit(-philim,philim,guidcmds.phim); */
    /* 'LIB_limit:27' if (in > max) */
    if (guidcmds->phim > Glim_glide) {
      /* 'LIB_limit:28' out = max; */
      the_off = Glim_glide;
    } else if (guidcmds->phim < (-Glim_glide)) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      the_off = -Glim_glide;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      the_off = guidcmds->phim;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3633' [phic,phi_gic,phi_int] = LIB_roll2gvec(guidcmds.phic_hold,guidcmds.phic_prev,guidcmds.phim,guidcmds.giyc,guidcmds.gizc, ... */
    /* 'GUID_Embedded_Guid_Control_1D:3634' 	                                      guidcmds.phi_int,guidcmds.AZphi,philim,SRlim,OPT_FP.GLim); */
    LIB_roll2gvec_MgzTYQaE(the_off, guidcmds->phic_prev, guidcmds->phim,
      guidcmds->giyc, guidcmds->gizc, &guidcmds->phi_int, guidcmds->AZphi,
      Glim_glide, fac_FOV, OPT_FP->GLim, phic, &guidcmds->phi_gic);

    /* 'GUID_Embedded_Guid_Control_1D:3636' guidcmds.phi_gic   = phi_gic; */
    /* 'GUID_Embedded_Guid_Control_1D:3637' guidcmds.phi_int    = phi_int; */
    /* 'GUID_Embedded_Guid_Control_1D:3638' guidcmds.phic_hold = phic; */
    guidcmds->phic_hold = *phic;

    /* 'GUID_Embedded_Guid_Control_1D:3639' guidcmds.phic_prev = phic; */
    guidcmds->phic_prev = *phic;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3642' else */
    /* 'GUID_Embedded_Guid_Control_1D:3646' phic = 0.0; */
    *phic = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3647' guidcmds.phic_hold = phic; */
    guidcmds->phic_hold = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3648' guidcmds.phic_prev = phic; */
    guidcmds->phic_prev = 0.0;
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [phic,gbyc,gbzc,guidcmds,guid2AP] = ExeP1TestCommands(GC_Mode,guidmode,ftime,guidcmds,def,ENV,OPT_FP,DCM,TTKF_in,NAVin,guid2AP,fromDATA,guid_settings)
 */
static void ExeP1TestCommands(uint8_T GC_Mode, uint8_T guidmode, real_T ftime,
  sETPRuSGTG6V05GSmeYzNjH *guidcmds, real_T b_def_AP_PROG1, real_T
  b_def_AP_PROG2, real_T ENV_Vnav, const real_T ENV_GRAVbody[3], real_T
  OPT_FP_GLim, const sPLFBkBfmjj013qOjLWm7GC *DCM, real_T TTKF_in_tgtrge, real_T
  TTKF_in_dthe_b, real_T TTKF_in_dpsi_b, T_GUID_Guid_Out_IF1003 *guid2AP, const
  sbS2o27mNjUInq39Qf98jAF *guid_settings, real_T *phic, real_T *gbyc, real_T
  *gbzc)
{
  real_T t_end_prog;
  real_T gbc[3];
  real_T phi_prog;
  int32_T i;

  /* 'GUID_Embedded_Guid_Control_1D:3669' t_beg_prog = 3.0; */
  /* 'GUID_Embedded_Guid_Control_1D:3670' if (GC_Mode == def.AP_PROG1) */
  if (((real_T)GC_Mode) == b_def_AP_PROG1) {
    /* 'GUID_Embedded_Guid_Control_1D:3671' t_end_prog = guid_settings.matPROG1(end-2,1); */
    t_end_prog = guid_settings->matPROG1[35];
  } else if (((real_T)GC_Mode) == b_def_AP_PROG2) {
    /* 'GUID_Embedded_Guid_Control_1D:3672' elseif (GC_Mode == def.AP_PROG2) */
    /* 'GUID_Embedded_Guid_Control_1D:3673' t_end_prog = guid_settings.matPROG2(end-2,1); */
    t_end_prog = guid_settings->matPROG2[128];
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3674' else */
    /* 'GUID_Embedded_Guid_Control_1D:3675' t_end_prog = guid_settings.matDEFAULT(end,1); */
    t_end_prog = guid_settings->matDEFAULT[1];
  }

  /* 'GUID_Embedded_Guid_Control_1D:3677' guid2AP.dFin1 = 0.0; */
  guid2AP->dFin1 = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:3678' guid2AP.dFin2 = 0.0; */
  guid2AP->dFin2 = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:3679' guid2AP.dFin3 = 0.0; */
  guid2AP->dFin3 = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:3680' guid2AP.dFin4 = 0.0; */
  guid2AP->dFin4 = 0.0;

  /* 'GUID_Embedded_Guid_Control_1D:3681' if (guidmode < 5) */
  if (((int32_T)guidmode) < 5) {
    /* 'GUID_Embedded_Guid_Control_1D:3682' gbyc = 0.0; */
    *gbyc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3683' gbzc = 0.0; */
    *gbzc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:3684' guidcmds.gbycD0 = 0.0; */
    guidcmds->gbycD0 = 0.0;
  } else if (ftime < 3.0) {
    /* 'GUID_Embedded_Guid_Control_1D:3685' elseif (ftime < t_beg_prog) */
    /* 'GUID_Embedded_Guid_Control_1D:3686' OPT_FP.GLim = 10.0; */
    OPT_FP_GLim = 10.0;

    /* 'GUID_Embedded_Guid_Control_1D:3687' guidcmds.AZy = guidcmds.AZy*0.99; */
    guidcmds->AZy *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3688' guidcmds.AZz = guidcmds.AZz*0.999; */
    guidcmds->AZz *= 0.999;

    /* 'GUID_Embedded_Guid_Control_1D:3689' guidcmds.gbzcD = -5.0; */
    guidcmds->gbzcD = -5.0;

    /* 'GUID_Embedded_Guid_Control_1D:3691' gbc = DCM.NED2BODY*[0.0 0.0 guidcmds.gbzcD]'; */
    for (i = 0; i < 3; i++) {
      gbc[i] = (DCM->NED2BODY[i + 6] * -5.0) + ((DCM->NED2BODY[i + 3] * 0.0) +
        (DCM->NED2BODY[i] * 0.0));
    }

    /* 'GUID_Embedded_Guid_Control_1D:3692' gbyc = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*gbc(2); */
    *gbyc = ((1.0 - guidcmds->AZy) * gbc[1]) + (guidcmds->AZy * guid2AP->gbyc);

    /* 'GUID_Embedded_Guid_Control_1D:3693' gbzc = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*gbc(3); */
    *gbzc = ((1.0 - guidcmds->AZz) * gbc[2]) + (guidcmds->AZz * guid2AP->gbzc);
  } else if (ftime < t_end_prog) {
    /* 'GUID_Embedded_Guid_Control_1D:3696' elseif (ftime < t_end_prog) */
    /* 'GUID_Embedded_Guid_Control_1D:3697' guidcmds.AZy = guidcmds.AZy*0.99; */
    guidcmds->AZy *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3698' guidcmds.AZz = guidcmds.AZz*0.99; */
    guidcmds->AZz *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3699' if (GC_Mode == def.AP_PROG1) */
    if (((real_T)GC_Mode) == b_def_AP_PROG1) {
      /* 'GUID_Embedded_Guid_Control_1D:3700' gbc_prog = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,3),ftime); */
      t_end_prog = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[76], ftime);

      /* 'GUID_Embedded_Guid_Control_1D:3701' phi_prog = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,2),ftime)*(pi/180.0); */
      phi_prog = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[38], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3702' guid2AP.dFin1 = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,4),ftime)*(pi/180.0); */
      guid2AP->dFin1 = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[114], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3703' guid2AP.dFin2 = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,5),ftime)*(pi/180.0); */
      guid2AP->dFin2 = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[152], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3704' guid2AP.dFin3 = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,6),ftime)*(pi/180.0); */
      guid2AP->dFin3 = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[190], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3705' guid2AP.dFin4 = interp1(guid_settings.matPROG1(:,1),guid_settings.matPROG1(:,7),ftime)*(pi/180.0); */
      guid2AP->dFin4 = interp1_FTXwGFAn(&guid_settings->matPROG1[0],
        &guid_settings->matPROG1[228], ftime) * 0.017453292519943295;
    } else if (((real_T)GC_Mode) == b_def_AP_PROG2) {
      /* 'GUID_Embedded_Guid_Control_1D:3706' elseif (GC_Mode == def.AP_PROG2) */
      /* 'GUID_Embedded_Guid_Control_1D:3707' gbc_prog = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,3),ftime); */
      t_end_prog = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[262], ftime);

      /* 'GUID_Embedded_Guid_Control_1D:3708' phi_prog = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,2),ftime)*(pi/180.0); */
      phi_prog = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[131], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3709' guid2AP.dFin1 = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,4),ftime)*(pi/180.0); */
      guid2AP->dFin1 = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[393], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3710' guid2AP.dFin2 = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,5),ftime)*(pi/180.0); */
      guid2AP->dFin2 = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[524], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3711' guid2AP.dFin3 = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,6),ftime)*(pi/180.0); */
      guid2AP->dFin3 = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[655], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3712' guid2AP.dFin4 = interp1(guid_settings.matPROG2(:,1),guid_settings.matPROG2(:,7),ftime)*(pi/180.0); */
      guid2AP->dFin4 = interp1_Ln1e4hwu(&guid_settings->matPROG2[0],
        &guid_settings->matPROG2[786], ftime) * 0.017453292519943295;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:3713' else */
      /* 'GUID_Embedded_Guid_Control_1D:3714' gbc_prog = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,3),ftime); */
      t_end_prog = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[4], ftime);

      /* 'GUID_Embedded_Guid_Control_1D:3715' phi_prog = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,2),ftime)*(pi/180.0); */
      phi_prog = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[2], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3716' guid2AP.dFin1 = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,4),ftime)*(pi/180.0); */
      guid2AP->dFin1 = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[6], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3717' guid2AP.dFin2 = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,5),ftime)*(pi/180.0); */
      guid2AP->dFin2 = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[8], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3718' guid2AP.dFin3 = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,6),ftime)*(pi/180.0); */
      guid2AP->dFin3 = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[10], ftime) * 0.017453292519943295;

      /* 'GUID_Embedded_Guid_Control_1D:3719' guid2AP.dFin4 = interp1(guid_settings.matDEFAULT(:,1),guid_settings.matDEFAULT(:,7),ftime)*(pi/180.0); */
      guid2AP->dFin4 = interp1_zsuvPUhx(&guid_settings->matDEFAULT[0],
        &guid_settings->matDEFAULT[12], ftime) * 0.017453292519943295;
    }

    /* 'GUID_Embedded_Guid_Control_1D:3721' gbyc_prog =  gbc_prog*sin(phi_prog); */
    /* 'GUID_Embedded_Guid_Control_1D:3722' gbzc_prog = -gbc_prog*cos(phi_prog); */
    /* 'GUID_Embedded_Guid_Control_1D:3723' gbyc = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*gbyc_prog; */
    *gbyc = ((1.0 - guidcmds->AZy) * (t_end_prog * sin(phi_prog))) +
      (guidcmds->AZy * guid2AP->gbyc);

    /* 'GUID_Embedded_Guid_Control_1D:3724' gbzc = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*gbzc_prog; */
    *gbzc = ((1.0 - guidcmds->AZz) * ((-t_end_prog) * cos(phi_prog))) +
      (guidcmds->AZz * guid2AP->gbzc);
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3725' else */
    /* 'GUID_Embedded_Guid_Control_1D:3726' guidcmds.AZy = guidcmds.AZy*0.99; */
    guidcmds->AZy *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3727' guidcmds.AZz = guidcmds.AZz*0.99; */
    guidcmds->AZz *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3728' gbyc_pnav =  TTKF_in.dpsi_b*4.0*ENV.Vnav - ENV.GRAVbody(2); */
    /* 'GUID_Embedded_Guid_Control_1D:3729' gbzc_pnav = -TTKF_in.dthe_b*4.0*ENV.Vnav - ENV.GRAVbody(3); */
    /* 'GUID_Embedded_Guid_Control_1D:3730' gbyc = guidcmds.AZy*guid2AP.gbyc + (1.0-guidcmds.AZy)*gbyc_pnav; */
    *gbyc = ((((TTKF_in_dpsi_b * 4.0) * ENV_Vnav) - ENV_GRAVbody[1]) * (1.0 -
              guidcmds->AZy)) + (guidcmds->AZy * guid2AP->gbyc);

    /* 'GUID_Embedded_Guid_Control_1D:3731' gbzc = guidcmds.AZz*guid2AP.gbzc + (1.0-guidcmds.AZz)*gbzc_pnav; */
    *gbzc = (((((-TTKF_in_dthe_b) * 4.0) * ENV_Vnav) - ENV_GRAVbody[2]) * (1.0 -
              guidcmds->AZz)) + (guidcmds->AZz * guid2AP->gbzc);
  }

  /* 'GUID_Embedded_Guid_Control_1D:3734' gbc = sqrt(gbyc^2+gbzc^2); */
  t_end_prog = sqrt(((*gbyc) * (*gbyc)) + ((*gbzc) * (*gbzc)));

  /* 'GUID_Embedded_Guid_Control_1D:3735' if (gbc > OPT_FP.GLim) */
  if (t_end_prog > OPT_FP_GLim) {
    /* 'GUID_Embedded_Guid_Control_1D:3736' gbyc = gbyc*(OPT_FP.GLim/gbc); */
    t_end_prog = OPT_FP_GLim / t_end_prog;
    *gbyc *= t_end_prog;

    /* 'GUID_Embedded_Guid_Control_1D:3737' gbzc = gbzc*(OPT_FP.GLim/gbc); */
    *gbzc *= t_end_prog;
  }

  /* 'GUID_Embedded_Guid_Control_1D:3745' if (guidmode < 5) */
  if (((int32_T)guidmode) < 5) {
    /* 'GUID_Embedded_Guid_Control_1D:3746' phic = 0.0; */
    *phic = 0.0;
  } else if (TTKF_in_tgtrge > 3000.0) {
    /* 'GUID_Embedded_Guid_Control_1D:3750' elseif (TTKF_in.tgtrge > 3000.0) */
    /* 'GUID_Embedded_Guid_Control_1D:3751' guidcmds.phic_hold = guidcmds.phic_hold*0.99; */
    guidcmds->phic_hold *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3752' phic = 0.0; */
    *phic = 0.0;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:3753' else */
    /* 'GUID_Embedded_Guid_Control_1D:3754' guidcmds.phic_hold = guidcmds.phic_hold*0.99; */
    guidcmds->phic_hold *= 0.99;

    /* 'GUID_Embedded_Guid_Control_1D:3755' phic = guidcmds.phic_hold; */
    *phic = guidcmds->phic_hold;
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [phic,gbyc,gbzc,guid2AP] = ExeDeflInspTest(ftime,guid2AP,guid_settings)
 */
static void ExeDeflInspTest(real_T ftime, T_GUID_Guid_Out_IF1003 *guid2AP, const
  real_T guid_settings_matDEFL[266])
{
  /* 'GUID_Embedded_Guid_Control_1D:3764' phic = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:3765' gbyc = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:3766' gbzc = 0.0; */
  /* 'GUID_Embedded_Guid_Control_1D:3767' guid2AP.dFin1 = interp1(guid_settings.matDEFL(:,1),guid_settings.matDEFL(:,4),ftime)*(pi/180.0); */
  guid2AP->dFin1 = interp1_FTXwGFAn(&guid_settings_matDEFL[0],
    &guid_settings_matDEFL[114], ftime) * 0.017453292519943295;

  /* 'GUID_Embedded_Guid_Control_1D:3768' guid2AP.dFin2 = interp1(guid_settings.matDEFL(:,1),guid_settings.matDEFL(:,5),ftime)*(pi/180.0); */
  guid2AP->dFin2 = interp1_FTXwGFAn(&guid_settings_matDEFL[0],
    &guid_settings_matDEFL[152], ftime) * 0.017453292519943295;

  /* 'GUID_Embedded_Guid_Control_1D:3769' guid2AP.dFin3 = interp1(guid_settings.matDEFL(:,1),guid_settings.matDEFL(:,6),ftime)*(pi/180.0); */
  guid2AP->dFin3 = interp1_FTXwGFAn(&guid_settings_matDEFL[0],
    &guid_settings_matDEFL[190], ftime) * 0.017453292519943295;

  /* 'GUID_Embedded_Guid_Control_1D:3770' guid2AP.dFin4 = interp1(guid_settings.matDEFL(:,1),guid_settings.matDEFL(:,7),ftime)*(pi/180.0); */
  guid2AP->dFin4 = interp1_FTXwGFAn(&guid_settings_matDEFL[0],
    &guid_settings_matDEFL[228], ftime) * 0.017453292519943295;
}

/* Model step function */
void Guidance_100Hz_Function_main(RT_MODEL *const Guidance_100Hz_Function_1D_M,
  real_T rtU_SysTimein, T_GUID_Discretes *rtU_Discretesin,
  T_GUID_TTKF_Out_IF1002 *rtU_ttkfin, uint8_T rtU_navmodein,
  T_NAV_NavigationState *rtU_navstatein, T_NAV_GenericInertialNavigationSensor
  *rtU_IMUin, T_GUID_MissionPlanning *rtU_MPin, T_GUID_WindParams *rtU_Windin,
  T_GUID_FlightConditions *rtU_Pitotin, T_Weapon_MCP_DATA_Guid *rtU_DATAguid,
  T_GUID_WeaponConfig *rtU_WeaCfgin, T_GUID_TestConfig *rtU_TestCfgin,
  T_GUID_Guid_Out_IF1003 *rtY_guid2AP, T_GUID_GuidCtrlFlags *rtY_guidCtrlFlags,
  T_GUID_Guid2DATA *rtY_guid2DATA, T_GUID_Guid_Tel100Hz *rtY_guidtel_100Hz,
  T_GUID_Guid_Tel20HzA *rtY_guidtel_20HzA, T_GUID_Guid_Tel20HzB
  *rtY_guidtel_20HzB)
{
  real_T RrefC[3];
  T_Cartesian TPosNED_uncond;
  T_Cartesian MPosNED;
  T_Cartesian MVelNED;
  T_Cartesian MRefNED;
  sPLFBkBfmjj013qOjLWm7GC DCM;
  int32_T ValidGNSSNav;
  real_T psiLOSi;
  real_T psiLOSb;
  real_T term_vec[3];
  real_T H_release;
  uint8_T keep_arc_8;
  real_T dNavPos;
  real_T dNavVel;
  sFYrLWvx1kVFonG2Nu0GNkH TRACKnow;
  real_T ang_threshold;
  real_T phic;
  real_T gbyc;
  real_T gbzc;
  real_T n_a;
  real_T ftime;
  static const real_T b[266] = { 0.0, 0.001, 4.0, 4.001, 5.0, 10.0, 10.001, 12.0,
    12.001, 14.0, 14.001, 15.0, 15.001, 17.0, 17.001, 19.0, 19.001, 21.0, 21.001,
    23.0, 23.001, 24.0, 24.001, 26.0, 26.001, 28.0, 28.001, 30.0, 30.001, 31.0,
    31.001, 33.0, 33.001, 35.0, 35.001, 37.0, 37.001, 100.0, 0.0, 0.0, 0.0,
    180.0, 180.0, 360.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 45.0, 45.0, 45.0, 45.0,
    45.0, 45.0, 45.0, 45.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0,
    345.0, 345.0, 345.0, 345.0, 345.0, 345.0, 345.0, 345.0, 0.0, 0.0, 0.0, 5.0,
    5.0, 10.0, 10.0, 10.0, 20.0, 20.0, 10.0, 10.0, 0.1, 0.1, 0.1, 0.1, 20.0,
    20.0, 10.0, 10.0, 0.1, 0.1, 0.1, 0.1, 20.0, 20.0, 10.0, 10.0, 0.1, 0.1, 0.1,
    0.1, 28.0, 28.0, 14.0, 14.0, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  static const real_T f[917] = { 0.0, 0.001, 6.0, 6.001, 6.05, 6.051, 7.0, 7.001,
    7.1, 7.101, 8.0, 9.0, 9.001, 10.0, 10.001, 10.05, 10.051, 11.0, 11.001, 11.1,
    11.101, 13.0, 13.001, 14.0, 14.001, 14.05, 14.051, 15.0, 15.001, 15.1,
    15.101, 16.0, 16.001, 17.0, 17.001, 17.05, 17.051, 18.0, 18.001, 18.1,
    18.101, 19.0, 19.001, 20.0, 20.001, 22.0, 22.001, 22.05, 22.051, 23.0,
    23.001, 23.1, 23.101, 24.0, 25.0, 25.001, 27.0, 27.001, 27.05, 27.051, 29.0,
    29.001, 29.1, 29.101, 31.0, 31.001, 33.0, 33.001, 33.05, 33.051, 36.0,
    36.001, 36.1, 36.101, 38.0, 38.001, 39.0, 39.001, 39.05, 39.051, 40.0,
    40.001, 40.1, 40.101, 42.0, 42.001, 43.0, 43.001, 45.0, 45.001, 45.05,
    45.051, 46.0, 46.001, 46.1, 46.101, 47.0, 47.001, 49.0, 49.001, 49.05,
    49.051, 50.0, 50.001, 50.1, 50.101, 52.0, 52.001, 53.0, 53.001, 53.05,
    53.051, 54.0, 54.001, 54.1, 54.101, 55.0, 55.001, 56.0, 56.001, 56.05,
    56.051, 57.0, 57.001, 57.1, 57.101, 58.0, 58.001, 59.0, 59.001, 100.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 15.0, 15.0,
    15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 330.0, 330.0, 330.0, 330.0, 330.0,
    330.0, 330.0, 330.0, 330.0, 330.0, 45.0, 45.0, 45.0, 45.0, 45.0, 45.0, 45.0,
    45.0, 45.0, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0,
    30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 15.0, 15.0, 15.0,
    15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 345.0, 345.0, 345.0, 345.0, 345.0, 345.0, 345.0,
    345.0, 345.0, 345.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0,
    30.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0, 315.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0,
    8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0,
    8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0,
    8.0, 8.0, 8.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
    13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
    13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
    13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
    4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
    4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
    4.0, 4.0, 4.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0,
    5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 2.9, 2.9, 0.0, 0.0, 0.0,
    0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 2.0,
    2.0, 0.0, 0.0, 1.3, 1.3, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 2.9, 2.9,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0,
    0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0,
    1.3, 1.3, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0,
    0.0, 1.3, 1.3, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0,
    5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0,
    0.0, 2.9, 2.9, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
    0.0, 0.0, -2.9, -2.9, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0,
    0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0,
    0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -1.3, -1.3, 0.0,
    0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -2.9, -2.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0,
    -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -1.3, -1.3, 0.0, 0.0, 0.0, 0.0,
    2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0,
    -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -1.3, -1.3, 0.0, 0.0,
    0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0,
    2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0,
    2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0,
    0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -2.9, -2.9, 0.0,
    0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  static const real_T h[266] = { 0.0, 10.0, 10.001, 15.0, 15.001, 20.0, 20.001,
    25.0, 25.001, 30.0, 30.001, 35.0, 35.001, 40.0, 40.001, 45.0, 45.001, 50.0,
    50.001, 55.0, 55.001, 60.0, 60.001, 65.0, 65.001, 70.0, 70.001, 100.0,
    100.001, 105.0, 105.001, 135.0, 135.001, 140.0, 140.001, 170.0, 170.001,
    1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, -15.0, -15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0,
    15.0, 0.0, 0.0, 15.0, 15.0, 0.0, 0.0, -15.0, -15.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, -15.0, -15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 15.0, 0.0, 0.0, 15.0,
    15.0, 0.0, 0.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -15.0, -15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 15.0, 15.0, 0.0, 0.0, -15.0, -15.0, 0.0, 0.0, 15.0, 15.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -15.0, -15.0, 15.0, 15.0, 0.0, 0.0, 15.0,
    15.0, 0.0, 0.0, -15.0, -15.0, 0.0, 0.0, -15.0, -15.0, 0.0, 0.0 };

  static const int16_T j[14] = { 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  T_Cartesian TPosNED;
  T_Cartesian expl_temp;
  T_GUID_Guid_Out_IF1003 tmp;
  real_T tmp_0[9];
  uint32_T qY;
  int32_T guidtel20HzB_tmp;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_guid_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Guidance'
   */
  /* MATLAB Function: '<S1>/Guidance Controller' incorporates:
   *  Inport: '<Root>/DATAguid'
   *  Inport: '<Root>/Discretesin'
   *  Inport: '<Root>/IMUin'
   *  Inport: '<Root>/MPin'
   *  Inport: '<Root>/Pitotin'
   *  Inport: '<Root>/SystemTime'
   *  Inport: '<Root>/TestCfgin'
   *  Inport: '<Root>/WINDin'
   *  Inport: '<Root>/WeaCfgin'
   *  Inport: '<Root>/navmodein'
   *  Inport: '<Root>/navstatein'
   *  Inport: '<Root>/ttkfin'
   */
  /* :  [Guid2AP,GuidCtrlFlags,Guid2DATACtrl,Guid_100Hz_tel,Guid_20HzA_tel,Guid_20HzB_tel,Guid_ana] = ... */
  /* :      GUID_Embedded_Guid_Control_1D(SysTime_in, Discretesin, WeaCfgin, TestCfgin, TTKF_in, MP_in, WIND_in, NavMode_in, NavSolution_in, IMU_in, FlightCond_in, fromDATA, guid_prv, def, cnsts); */
  /* 'GUID_Embedded_Guid_Control_1D:66' if (isempty(delt)) */
  if (!Guidance_100Hz_Function_1D_M->dwork.delt_not_empty) {
    /* 'GUID_Embedded_Guid_Control_1D:69' delt = guid_prv.delt; */
    Guidance_100Hz_Function_1D_M->dwork.delt = 0.01;
    Guidance_100Hz_Function_1D_M->dwork.delt_not_empty = true;

    /* 'GUID_Embedded_Guid_Control_1D:72' ftime = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:73' ftime0 = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:74' nav_time = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:75' debug = uint8(0); */
    /* 'GUID_Embedded_Guid_Control_1D:76' t_debug = 30.0+1000.0; */
    /* 'GUID_Embedded_Guid_Control_1D:79' t_noGNSS = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:82' wingsopen_timeout = 1000.0; */
    /* 'GUID_Embedded_Guid_Control_1D:84' guid_settings.Rarc_R     = 5000.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.Rarc_R = 5000.0;

    /* 'GUID_Embedded_Guid_Control_1D:85' guid_settings.Rterm      = 2000.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.Rterm = 2000.0;

    /* 'GUID_Embedded_Guid_Control_1D:86' guid_settings.machglide  = 0.6; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.machglide = 0.6;

    /* 'GUID_Embedded_Guid_Control_1D:87' guid_settings.psiR_Thr   = 2.5*cnsts.d2r; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.psiR_Thr =
      0.043633231299858237;

    /* 'GUID_Embedded_Guid_Control_1D:88' guid_settings.psi_Engage = 90.0*cnsts.d2r; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.psi_Engage =
      1.5707963267948966;

    /* 'GUID_Embedded_Guid_Control_1D:89' guid_settings.do_plot    = 0; */
    Guidance_100Hz_Function_1D_M->dwork.guid_settings.do_plot = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:141' guid_settings.matPROG1 = [ */
    /* 'GUID_Embedded_Guid_Control_1D:142'     % time   phi    gbc   fin1   fin2   fin3   fin4 */
    /* 'GUID_Embedded_Guid_Control_1D:143'      0.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:144'      0.001     0      5    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:145'      4.000     0      5    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:146'      4.001   180     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:147'      5.000   180     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:148'     10.000   360     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:149'     10.001     0     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:150'     12.000     0     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:151'     12.001     0     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:152'     14.000     0     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:153'     14.001     0    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:154'     15.000     0    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:155'     15.001    45    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:156'     17.000    45    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:157'     17.001    45     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:158'     19.000    45     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:159'     19.001    45     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:160'     21.000    45     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:161'     21.001    45    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:162'     23.000    45    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:163'     23.001    30    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:164'     24.000    30    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:165'     24.001    30     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:166'     26.000    30     20    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:167'     26.001    30     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:168'     28.000    30     10    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:169'     28.001    30    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:170'     30.000    30    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:171'     30.001   345    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:172'     31.000   345    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:173'     31.001   345     28    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:174'     33.000   345     28    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:175'     33.001   345     14    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:176'     35.000   345     14    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:177'     35.001   345    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:178'     37.000   345    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:179'     37.001     0    0.1    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:180'     100.00     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:181'   ]; */
    memcpy(&Guidance_100Hz_Function_1D_M->dwork.guid_settings.matPROG1[0], &b[0],
           266U * (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_Control_1D:183' gb = [8 13 4]; */
    /* 'GUID_Embedded_Guid_Control_1D:184' guid_settings.matPROG2 = [ */
    /* 'GUID_Embedded_Guid_Control_1D:185'     % time   phi    gbc   fin1   fin2   fin3   fin4 */
    /* 'GUID_Embedded_Guid_Control_1D:186'      0.000     0      0      0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:187'      0.001     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:188'      6.000     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:189'      6.001     0   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:190'      6.050     0   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:191'      6.051     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:192'      7.000     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:193'      7.001     0   gb(1)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:194'      7.100     0   gb(1)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:195'      7.101     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:196'      8.000     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:197'      9.000     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:198'      9.001    15   gb(1)     0      0      0      0% 9.001   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:199'     10.000    15   gb(1)     0      0      0      0%10.000   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:200'     10.001    15   gb(1)    +2     +2     +2     +2%10.001   345   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:201'     10.050    15   gb(1)    +2     +2     +2     +2%10.050   345   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:202'     10.051    15   gb(1)     0      0      0      0%10.051   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:203'     11.000    15   gb(1)     0      0      0      0%11.000   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:204'     11.001    15   gb(1)  +2.9     +5   -2.9     -5%11.001   345   gb(1)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:205'     11.100    15   gb(1)  +2.9     +5   -2.9     -5%11.100   345   gb(1)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:206'     11.101    15   gb(1)     0      0      0      0%11.101   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:207'     13.000    15   gb(1)     0      0      0      0%13.000   345   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:208'     13.001   330   gb(1)     0      0      0      0%13.001    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:209'     14.000   330   gb(1)     0      0      0      0%14.000    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:210'     14.001   330   gb(1)    +2     +2     +2     +2%14.001    30   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:211'     14.050   330   gb(1)    +2     +2     +2     +2%14.050    30   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:212'     14.051   330   gb(1)     0      0      0      0%14.051    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:213'     15.000   330   gb(1)     0      0      0      0%15.000    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:214'     15.001   330   gb(1)    +5   +1.3     -5   -1.3%15.001    30   gb(1)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:215'     15.100   330   gb(1)    +5   +1.3     -5   -1.3%15.100    30   gb(1)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:216'     15.101   330   gb(1)     0      0      0      0%15.101    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:217'     16.000   330   gb(1)     0      0      0      0%16.000    30   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:218'     16.001    45   gb(1)     0      0      0      0%16.001   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:219'     17.000    45   gb(1)     0      0      0      0%17.000   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:220'     17.001    45   gb(1)    +2     +2     +2     +2%17.001   315   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:221'     17.050    45   gb(1)    +2     +2     +2     +2%17.050   315   gb(1)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:222'     17.051    45   gb(1)     0      0      0      0%17.051   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:223'     18.000    45   gb(1)     0      0      0      0%18.000   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:224'     18.001    45   gb(1)     0     +5      0     -5%18.001   315   gb(1)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:225'     18.100    45   gb(1)     0     +5      0     -5%18.100   315   gb(1)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:226'     18.101    45   gb(1)     0      0      0      0%18.101   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:227'     19.000    45   gb(1)     0      0      0      0%19.000   315   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:228'     19.001     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:229'     20.000     0   gb(1)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:230'     20.001     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:231'     22.000     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:232'     22.001     0   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:233'     22.050     0   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:234'     22.051     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:235'     23.000     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:236'     23.001     0   gb(2)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:237'     23.100     0   gb(2)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:238'     23.101     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:239'     24.000     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:240'     25.000     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:241'     25.001   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:242'     27.000   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:243'     27.001   315   gb(2)    +2     +2     +2     +2%345   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:244'     27.050   315   gb(2)    +2     +2     +2     +2%345   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:245'     27.051   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:246'     29.000   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:247'     29.001   315   gb(2)    +5      0     -5      0%345   gb(2)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:248'     29.100   315   gb(2)    +5      0     -5      0%345   gb(2)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:249'     29.101   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:250'     31.000   315   gb(2)     0      0      0      0%345   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:251'     31.001    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:252'     33.000    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:253'     33.001    30   gb(2)    +2     +2     +2     +2% 30   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:254'     33.050    30   gb(2)    +2     +2     +2     +2% 30   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:255'     33.051    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:256'     36.000    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:257'     36.001    30   gb(2)  +1.3     +5   -1.3     -5% 30   gb(2)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:258'     36.100    30   gb(2)  +1.3     +5   -1.3     -5% 30   gb(2)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:259'     36.101    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:260'     38.000    30   gb(2)     0      0      0      0% 30   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:261'     38.001    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:262'     39.000    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:263'     39.001    15   gb(2)    +2     +2     +2     +2%315   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:264'     39.050    15   gb(2)    +2     +2     +2     +2%315   gb(2)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:265'     39.051    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:266'     40.000    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:267'     40.001    15   gb(2)  +2.9     +5   -2.9     -5%315   gb(2)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:268'     40.100    15   gb(2)  +2.9     +5   -2.9     -5%315   gb(2)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:269'     40.101    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:270'     42.000    15   gb(2)     0      0      0      0%315   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:271'     42.001     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:272'     43.000     0   gb(2)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:273'     43.001     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:274'     45.000     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:275'     45.001     0   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:276'     45.050     0   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:277'     45.051     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:278'     46.000     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:279'     46.001     0   gb(3)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:280'     46.100     0   gb(3)    +5     +5     -5     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:281'     46.101     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:282'     47.000     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:283'     47.001   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:284'     49.000   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:285'     49.001   345   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:286'     49.050   345   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:287'     49.051   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:288'     50.000   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:289'     50.001   345   gb(3)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:290'     50.100   345   gb(3)    +5   +2.9     -5   -2.9 */
    /* 'GUID_Embedded_Guid_Control_1D:291'     50.101   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:292'     52.000   345   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:293'     52.001    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:294'     53.000    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:295'     53.001    30   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:296'     53.050    30   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:297'     53.051    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:298'     54.000    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:299'     54.001    30   gb(3)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:300'     54.100    30   gb(3)  +1.3     +5   -1.3     -5 */
    /* 'GUID_Embedded_Guid_Control_1D:301'     54.101    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:302'     55.000    30   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:303'     55.001   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:304'     56.000   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:305'     56.001   315   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:306'     56.050   315   gb(3)    +2     +2     +2     +2 */
    /* 'GUID_Embedded_Guid_Control_1D:307'     56.051   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:308'     57.000   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:309'     57.001   315   gb(3)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:310'     57.100   315   gb(3)    +5      0     -5      0 */
    /* 'GUID_Embedded_Guid_Control_1D:311'     57.101   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:312'     58.000   315   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:313'     58.001     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:314'     59.000     0   gb(3)     0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:315'     59.001     0      0      0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:316'     100.00     0      0      0      0      0      0 */
    /* 'GUID_Embedded_Guid_Control_1D:317'   ]; */
    memcpy(&Guidance_100Hz_Function_1D_M->dwork.guid_settings.matPROG2[0], &f[0],
           917U * (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_Control_1D:319' guid_settings.matDEFL = [ */
    /* 'GUID_Embedded_Guid_Control_1D:320'     % time   phi    gbc   fin1   fin2   fin3   fin4 */
    /* 'GUID_Embedded_Guid_Control_1D:321'      0.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:322'     10.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:323'     10.001     0      0  -15.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:324'     15.000     0      0  -15.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:325'     15.001     0      0   15.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:326'     20.000     0      0   15.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:327'     20.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:328'     25.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:329'     25.001     0      0    0.0  -15.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:330'     30.000     0      0    0.0  -15.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:331'     30.001     0      0    0.0   15.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:332'     35.000     0      0    0.0   15.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:333'     35.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:334'     40.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:335'     40.001     0      0    0.0    0.0  -15.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:336'     45.000     0      0    0.0    0.0  -15.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:337'     45.001     0      0    0.0    0.0   15.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:338'     50.000     0      0    0.0    0.0   15.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:339'     50.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:340'     55.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:341'     55.001     0      0    0.0    0.0    0.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:342'     60.000     0      0    0.0    0.0    0.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:343'     60.001     0      0    0.0    0.0    0.0   15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:344'     65.000     0      0    0.0    0.0    0.0   15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:345'     65.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:346'     70.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:347'     70.001     0      0   15.0   15.0   15.0   15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:348'    100.000     0      0   15.0   15.0   15.0   15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:349'    100.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:350'    105.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:351'    105.001     0      0   15.0   15.0  -15.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:352'    135.000     0      0   15.0   15.0  -15.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:353'    135.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:354'    140.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:355'    140.001     0      0  -15.0   15.0   15.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:356'    170.000     0      0  -15.0   15.0   15.0  -15.0 */
    /* 'GUID_Embedded_Guid_Control_1D:357'    170.001     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:358'   1000.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:359'   ]; */
    memcpy(&Guidance_100Hz_Function_1D_M->dwork.guid_settings.matDEFL[0], &h[0],
           266U * (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_Control_1D:362' guid_settings.matDEFAULT = [ */
    /* 'GUID_Embedded_Guid_Control_1D:363'     % time   phi    gbc   fin1   fin2   fin3   fin4 */
    /* 'GUID_Embedded_Guid_Control_1D:364'      0.000     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:365'     1000.0     0      0    0.0    0.0    0.0    0.0 */
    /* 'GUID_Embedded_Guid_Control_1D:366'   ]; */
    for (guidtel20HzB_tmp = 0; guidtel20HzB_tmp < 14; guidtel20HzB_tmp++) {
      Guidance_100Hz_Function_1D_M->
        dwork.guid_settings.matDEFAULT[guidtel20HzB_tmp] = (real_T)
        j[guidtel20HzB_tmp];
    }

    /* 'GUID_Embedded_Guid_Control_1D:370' if (guid_settings.do_plot) */
    /* 'GUID_Embedded_Guid_Control_1D:374' TPosNED_uncond_prev.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:375' TPosNED_uncond_prev.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:376' TPosNED_uncond_prev.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:377' MPosNED_prev.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:378' MPosNED_prev.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:379' MPosNED_prev.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:380' MVelNED_prev.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:381' MVelNED_prev.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:382' MVelNED_prev.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:384' opportunity_mode = uint8(0); */
  }

  /* 'GUID_Embedded_Guid_Control_1D:391' if ((NavMode_in >= 30 && NavMode_in ~= 39 && NavMode_in ~= 69 && NavMode_in ~= 99) || ~Discretes_in.umbilical) */
  if ((((int32_T)rtU_navmodein) >= 30) || (((int32_T)rtU_Discretesin->umbilical) == 0)) {
    /* 'GUID_Embedded_Guid_Control_1D:392' nav_time = nav_time + delt; */
    Guidance_100Hz_Function_1D_M->dwork.nav_time +=
      Guidance_100Hz_Function_1D_M->dwork.delt;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:393' else */
    /* 'GUID_Embedded_Guid_Control_1D:394' nav_time = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.nav_time = 0.0;
  }

  /* 'GUID_Embedded_Guid_Control_1D:397' if (nav_time > 25.0) */
  /* 'GUID_Embedded_Guid_Control_1D:401' if (nav_time < 0.1 || isempty(once)) */
  if ((Guidance_100Hz_Function_1D_M->dwork.nav_time < 0.1) ||
      (!Guidance_100Hz_Function_1D_M->dwork.once_not_empty)) {
    /* 'GUID_Embedded_Guid_Control_1D:403' once = 1; */
    Guidance_100Hz_Function_1D_M->dwork.once_not_empty = true;

    /* 'GUID_Embedded_Guid_Control_1D:406' guidmode = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidmode = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:407' opportunity_mode = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.opportunity_mode = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:410' newmeas_cnt = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.newmeas_cnt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:413' WindTab_Alt = zeros(39,1); */
    /* 'GUID_Embedded_Guid_Control_1D:414' WindTab_VN  = zeros(39,1); */
    /* 'GUID_Embedded_Guid_Control_1D:415' WindTab_VE  = zeros(39,1); */
    memset(&Guidance_100Hz_Function_1D_M->dwork.WindTab_Alt[0], 0, 39U * (sizeof
            (real_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.WindTab_VN[0], 0, 39U * (sizeof
            (real_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.WindTab_VE[0], 0, 39U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_Control_1D:418' NAV_in.pos_g.lat  = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lat = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:419' NAV_in.pos_g.lon  = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lon = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:420' NAV_in.pos_g.alt  = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.alt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:421' NAV_in.pos_e.x    = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.x = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:422' NAV_in.pos_e.y    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:423' NAV_in.pos_e.z    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:424' NAV_in.vel_e.x    = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.x = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:425' NAV_in.vel_e.y    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:426' NAV_in.vel_e.z    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:427' NAV_in.att_bn.phi = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.phi = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:428' NAV_in.att_bn.the = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.the = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:429' NAV_in.att_bn.psi = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.psi = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:430' NAV_in.fib_e.z    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.NAV_in.fib_e.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:433' IMU_in.gbxm =  0.0; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbxm = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:434' IMU_in.gbym =  0.0; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbym = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:435' IMU_in.gbzm = -9.8; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbzm = -9.8;

    /* 'GUID_Embedded_Guid_Control_1D:436' IMU_in.wbxm =  0.0; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbxm = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:437' IMU_in.wbym =  0.0; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbym = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:438' IMU_in.wbzm =  0.0; */
    Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbzm = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:441' FC_in.Temp = 25.0; */
    Guidance_100Hz_Function_1D_M->dwork.FC_in.Temp = 25.0;

    /* 'GUID_Embedded_Guid_Control_1D:442' FC_in.Psta = 101300.0; */
    Guidance_100Hz_Function_1D_M->dwork.FC_in.Psta = 101300.0;

    /* 'GUID_Embedded_Guid_Control_1D:443' FC_in.Pdyn = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.FC_in.Pdyn = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:446' ENV.rho = 1.225; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.rho = 1.225;

    /* 'GUID_Embedded_Guid_Control_1D:447' ENV.pressure = 101300; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.pressure = 101300.0;

    /* 'GUID_Embedded_Guid_Control_1D:448' ENV.temp0 = 15.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.temp0 = 15.0;

    /* 'GUID_Embedded_Guid_Control_1D:449' ENV.temp = 15.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.temp = 15.0;

    /* 'GUID_Embedded_Guid_Control_1D:450' ENV.Vsound = 341.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vsound = 341.0;

    /* 'GUID_Embedded_Guid_Control_1D:451' ENV.FPthem = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.FPthem = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:452' ENV.FPpsim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.FPpsim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:453' ENV.mach = 0.90; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.mach = 0.9;

    /* 'GUID_Embedded_Guid_Control_1D:454' ENV.alpha = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.alpha = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:455' ENV.beta = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.beta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:456' ENV.Vairspeed = 306.9; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vairspeed = 306.9;

    /* 'GUID_Embedded_Guid_Control_1D:457' ENV.Vnav = 306.9; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vnav = 306.9;

    /* 'GUID_Embedded_Guid_Control_1D:458' ENV.Vbody = [306.9 0.0 0.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:459' ENV.Q = 57689.9; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Q = 57689.9;

    /* 'GUID_Embedded_Guid_Control_1D:460' ENV.Qratio = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Qratio = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:461' ENV.GRAVbody = [0.0 0.0 9.81]'; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vbody[0] = 306.9;
    Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vbody[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.ENV.Vbody[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[2] = 9.81;

    /* 'GUID_Embedded_Guid_Control_1D:462' ENV.atmbeta = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.atmbeta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:463' ENV.BIAS_alpha = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.BIAS_alpha = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:464' ENV.BIAS_beta = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.ENV.BIAS_beta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:467' OPT_FP.ClCd_M = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.ClCd_M = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:468' OPT_FP.alpha_M = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.alpha_M = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:469' OPT_FP.CNgrav = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.CNgrav = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:470' OPT_FP.M = 0.6; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.M = 0.6;

    /* 'GUID_Embedded_Guid_Control_1D:471' OPT_FP.Mopt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mopt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:472' OPT_FP.Vopt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Vopt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:473' OPT_FP.Qopt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Qopt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:474' OPT_FP.Mglide_opt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mglide_opt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:475' OPT_FP.FPopt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPopt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:476' OPT_FP.FPopt0 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPopt0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:477' OPT_FP.Cd0 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Cd0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:478' OPT_FP.FPCmd0 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPCmd0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:479' OPT_FP.FPcmdD = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPcmdD = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:480' OPT_FP.FPerrD = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPerrD = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:481' OPT_FP.FPint  = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPint = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:482' OPT_FP.FPthec = double(-MP_in.DiveAngle); */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPthec = (real_T)((real32_T)
      (-rtU_MPin->DiveAngle));

    /* 'GUID_Embedded_Guid_Control_1D:483' OPT_FP.Cl = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Cl = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:484' OPT_FP.GLim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:485' OPT_FP.GLimZlo = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimZlo = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:486' OPT_FP.GLimZup = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimZup = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:487' OPT_FP.GLimYlo = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimYlo = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:488' OPT_FP.GLimYup = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimYup = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:489' OPT_FP.GLimZup_prev = 100.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimZup_prev = 100.0;

    /* 'GUID_Embedded_Guid_Control_1D:490' OPT_FP.GLimZlo_prev =-100.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimZlo_prev = -100.0;

    /* 'GUID_Embedded_Guid_Control_1D:491' OPT_FP.GLimYup_prev = 100.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimYup_prev = 100.0;

    /* 'GUID_Embedded_Guid_Control_1D:492' OPT_FP.GLimYlo_prev =-100.0; */
    Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLimYlo_prev = -100.0;

    /* 'GUID_Embedded_Guid_Control_1D:495' TRACK.log_FP  = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.log_FP = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:496' TRACK.once    = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.once = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:497' TRACK.reset   = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:498' TRACK.bunt_arc = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.bunt_arc = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:499' TRACK.reset3cnt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.reset3cnt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:500' TRACK.Rlim_lo = 100.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 100.0;

    /* 'GUID_Embedded_Guid_Control_1D:501' TRACK.Roff_intg = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Roff_intg = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:502' TRACK.ftime_step = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.ftime_step = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:503' TRACK.mintime = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.mintime = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:504' TRACK.FPbunt_cnt = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.FPbunt_cnt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:505' TRACK.solution= int8(0); */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.solution = 0;

    /* 'GUID_Embedded_Guid_Control_1D:506' TRACK.Mtgt0 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Mtgt0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:507' TRACK.Mglide  = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Mglide = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:508' TRACK.psiR    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.psiR = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:509' TRACK.FPazi   = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.FPazi = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:510' TRACK.Lfpath2bunt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2bunt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:511' TRACK.Lfpath2tgt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2tgt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:512' TRACK.ratio   = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.ratio = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:513' TRACK.rge2bunt= 10000.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.rge2bunt = 10000.0;

    /* 'GUID_Embedded_Guid_Control_1D:514' TRACK.PosRst  = 100.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.PosRst = 100.0;

    /* 'GUID_Embedded_Guid_Control_1D:515' TRACK.latch  = [uint8(0) uint8(0)]; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.latch[0] = 0U;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.latch[1] = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:516' TRACK.Ut0    = [0.0 0.0 -1.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:517' TRACK.Ut8    = [0.0 0.0 -1.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:518' TRACK.Ut     = [0.0 0.0 -1.0]'; */
    /* 'GUID_Embedded_Guid_Control_1D:519' TRACK.Kglide = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Kglide = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:520' TRACK.AprAngErr = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.AprAngErr = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:521' TRACK.re0.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:522' TRACK.re0.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:523' TRACK.re0.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:524' TRACK.mpos0.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:525' TRACK.mpos0.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:526' TRACK.mpos0.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:527' TRACK.Rto.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:528' TRACK.Rto.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:529' TRACK.Rto.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:530' TRACK.Rte.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:531' TRACK.Rte.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:532' TRACK.Rte.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:533' TRACK.Rwo.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:534' TRACK.Rwo.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:535' TRACK.Rwo.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:536' TRACK.Rwe.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:537' TRACK.Rwe.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:538' TRACK.Rwe.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:539' TRACK.Rmwo.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:540' TRACK.Rmwo.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:541' TRACK.Rmwo.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:542' TRACK.Rmto.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:543' TRACK.Rmto.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:544' TRACK.Rmto.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:545' TRACK.Rm.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:546' TRACK.Rm.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:547' TRACK.Rm.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:548' TRACK.RarcT = 1000.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.RarcT = 1000.0;

    /* 'GUID_Embedded_Guid_Control_1D:549' TRACK.OarcT.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:550' TRACK.OarcT.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:551' TRACK.OarcT.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:552' TRACK.RarcW = 1000.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.RarcW = 1000.0;

    /* 'GUID_Embedded_Guid_Control_1D:553' TRACK.OarcW.x = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.x = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:554' TRACK.OarcW.y = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.y = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:555' TRACK.OarcW.z = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.z = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:556' TRACK.AZrefL = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.AZrefL = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:557' TRACK.AZref = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.AZref = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:558' TRACK.Time8 = 1000.0; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Time8 = 1000.0;

    /* 'GUID_Embedded_Guid_Control_1D:559' TRACK.Umis = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:560' TRACK.Rmis0 = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:561' TRACK.Umis0 = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:564' clos.once = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.clos.once = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:565' clos.Rref = [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:566' clos.Rrel = [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:567' clos.Recef = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:568' clos.Rned = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:569' clos.Rfpath = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:570' clos.Roffset = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Roffset = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:571' clos.ArcState.Rarc_T = 100000.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_T = 100000.0;

    /* 'GUID_Embedded_Guid_Control_1D:572' clos.ArcState.Rapr_T = [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:573' clos.ArcState.Oarc_T = [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:574' clos.ArcState.Rarc_W = 100000.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_W = 100000.0;

    /* 'GUID_Embedded_Guid_Control_1D:575' clos.ArcState.Rapr_Rm= [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:576' clos.ArcState.Oarc_W = [0 0 0]; */
    /* 'GUID_Embedded_Guid_Control_1D:577' clos.Kglide = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Kglide = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:578' clos.Kglide_prev = 0.002; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Kglide_prev = 0.002;

    /* 'GUID_Embedded_Guid_Control_1D:579' clos.Qfac = 0.00002; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Qfac = 2.0E-5;

    /* 'GUID_Embedded_Guid_Control_1D:580' clos.Rby = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Rby = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:581' clos.Rbz = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Rbz = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:582' clos.Vby = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Vby = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:583' clos.Vbz = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Vbz = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:584' clos.Aby = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Aby = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:585' clos.Abz = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Abz = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:586' clos.AZcori = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.AZcori = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:587' clos.dRlim = 20.0*delt; */
    Guidance_100Hz_Function_1D_M->dwork.clos.dRlim = 20.0 *
      Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'GUID_Embedded_Guid_Control_1D:588' clos.dVlim = 40.0*delt; */
    Guidance_100Hz_Function_1D_M->dwork.clos.dVlim = 40.0 *
      Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'GUID_Embedded_Guid_Control_1D:589' clos.Rby0_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Rby0_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:590' clos.Rbz0_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Rbz0_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:591' clos.Vby0_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Vby0_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:592' clos.Vbz0_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.Vbz0_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:593' clos.RM   = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:594' clos.RWE  = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:595' clos.RMW0 = [0.0 0.0 0.0]; */
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rref[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rrel[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Recef[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rned[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rfpath[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RM[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RWE[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RMW0[0] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rref[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rrel[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Recef[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rned[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rfpath[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RM[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RWE[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RMW0[1] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[2] = -1.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[2] = -1.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[2] = -1.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rref[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rrel[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Recef[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rned[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.Rfpath[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RM[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RWE[2] = 0.0;
    Guidance_100Hz_Function_1D_M->dwork.clos.RMW0[2] = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:596' clos.fadeFac = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.clos.fadeFac = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:599' gains.Kfp = 150.0; */
    Guidance_100Hz_Function_1D_M->dwork.gains.Kfp = 50.0; // 150.0;

    /* 'GUID_Embedded_Guid_Control_1D:600' gains.Ki = 0.000; */
    Guidance_100Hz_Function_1D_M->dwork.gains.Ki = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:601' gains.Kp = 0.000; */
    Guidance_100Hz_Function_1D_M->dwork.gains.Kp = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:602' gains.Kv = 0.000; */
    Guidance_100Hz_Function_1D_M->dwork.gains.Kv = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:605' guidcmds.AZy = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZy = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:606' guidcmds.AZz = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZz = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:607' guidcmds.gbycD0 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbycD0 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:608' guidcmds.gbzcD = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzcD = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:609' guidcmds.inty = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.inty = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:610' guidcmds.intz = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.intz = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:611' guidcmds.gbzc_unl = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzc_unl = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:612' guidcmds.gbyc_unl = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbyc_unl = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:613' guidcmds.gbzc_FOV = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzc_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:614' guidcmds.gbyc_FOV = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbyc_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:615' guidcmds.intz_FOV = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.intz_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:616' guidcmds.inty_FOV = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.inty_FOV = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:617' guidcmds.gizc = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gizc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:618' guidcmds.giyc = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:619' guidcmds.eta_int = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.eta_int = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:620' guidcmds.eps_int = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.eps_int = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:621' guidcmds.phic_hold = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phic_hold = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:622' guidcmds.phi_int = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_int = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:623' guidcmds.phi_gic = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_gic = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:624' guidcmds.phic_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phic_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:625' guidcmds.AZphi = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZphi = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:626' guidcmds.gmode_sw = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:627' guidcmds.PurePropNav = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.PurePropNav = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:628' guidcmds.phi_attack = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_attack = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:629' guidcmds.phim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:630' guidcmds.verticaldive = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.verticaldive = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:631' guidcmds.dT = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.dT = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:632' guidcmds.GndRge = 1000000.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge = 1.0E+6;

    /* 'GUID_Embedded_Guid_Control_1D:633' guidcmds.forcegmode5 = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.forcegmode5 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:634' guidcmds.gb_ratio = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gb_ratio = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:635' guidcmds.gb_ratio_prev = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.gb_ratio_prev = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:636' guidcmds.giyc_int = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:637' guidcmds.navctrlreset = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.navctrlreset = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:642' guid2AP.AP_Loop = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:650' guid2AP.verticaldive = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.verticaldive = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:651' guid2AP.spare01 = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.spare01 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:652' guid2AP.spare02 = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.spare02 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:653' guid2AP.spare03 = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.spare03 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:654' guid2AP.ftime = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.ftime = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:655' guid2AP.phic = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.phic = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:656' guid2AP.phim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.phim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:657' guid2AP.gbyc = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbyc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:658' guid2AP.gbzc = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbzc = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:659' guid2AP.gbylim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbylim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:660' guid2AP.gbzlim = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbzlim = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:661' guid2AP.tgtrge = 10000.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.tgtrge = 10000.0;

    /* 'GUID_Embedded_Guid_Control_1D:662' guid2AP.mach = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.mach = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:663' guid2AP.alpha = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.alpha = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:664' guid2AP.beta = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.beta = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:665' guid2AP.Qratio = 1.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.Qratio = 1.0;

    /* 'GUID_Embedded_Guid_Control_1D:666' guid2AP.FPthec = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.FPthec = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:667' guid2AP.FPthem = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.FPthem = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:668' guid2AP.dFin1 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.dFin1 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:669' guid2AP.dFin2 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.dFin2 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:670' guid2AP.dFin3 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.dFin3 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:671' guid2AP.dFin4 = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.dFin4 = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:674' guidFLAGS.WingsOpen     = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsOpen = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:675' guidFLAGS.WingsRelease  = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsRelease = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:676' guidFLAGS.SADFuze       = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SADFuze = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:677' guidFLAGS.SwitchforBunt = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SwitchforBunt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:678' guidFLAGS.NavCtrlFlag   = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:679' guidFLAGS.Umbilical     = uint8(1); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.Umbilical = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:680' guidFLAGS.BatteryOn     = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.BatteryOn = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:681' guidFLAGS.PstaticValid  = uint8(1); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:684' guid2DATA.umbilical            = uint8(1); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.umbilical = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:685' guid2DATA.NavActive            = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.NavActive = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:686' guid2DATA.OpportunityMode      = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.OpportunityMode = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:687' guid2DATA.Spare01              = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.Spare01 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:688' guid2DATA.Spare02              = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.Spare02 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:689' guid2DATA.tgt_data.valid       = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.valid = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:690' guid2DATA.tgt_data.Spare       = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.Spare = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:691' guid2DATA.tgt_data.rgx         = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgx = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:692' guid2DATA.tgt_data.rgy         = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgy = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:693' guid2DATA.tgt_data.rgz         = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgz = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:694' guid2DATA.tgt_data.rex         = 6378137.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rex = 6.378137E+6;

    /* 'GUID_Embedded_Guid_Control_1D:695' guid2DATA.tgt_data.rey         = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rey = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:696' guid2DATA.tgt_data.rez         = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rez = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:697' guid2DATA.tgt_data.alt         = 100.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.alt = 100.0;

    /* 'GUID_Embedded_Guid_Control_1D:698' guid2DATA.tgt_data.dive        = 30.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.dive = 30.0;

    /* 'GUID_Embedded_Guid_Control_1D:699' guid2DATA.tgt_data.arrive      = 180.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.arrive = 180.0;

    /* 'GUID_Embedded_Guid_Control_1D:700' guid2DATA.tgt_data.speed       = 250.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.speed = 250.0;

    /* 'GUID_Embedded_Guid_Control_1D:701' guid2DATA.launch_data.lat      = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.lat = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:702' guid2DATA.launch_data.lon      = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.lon = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:703' guid2DATA.launch_data.alt      = 3048.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.alt = 3048.0;

    /* 'GUID_Embedded_Guid_Control_1D:704' guid2DATA.launch_data.rex      = 6381185; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rex = 6.381185E+6;

    /* 'GUID_Embedded_Guid_Control_1D:705' guid2DATA.launch_data.rey      = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rey = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:706' guid2DATA.launch_data.rez      = -20000.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rez = -20000.0;

    /* 'GUID_Embedded_Guid_Control_1D:707' guid2DATA.launch_data.Vgnd     = 250.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.Vgnd = 250.0;

    /* 'GUID_Embedded_Guid_Control_1D:708' guid2DATA.launch_data.mach     = 0.8; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.mach = 0.8;

    /* 'GUID_Embedded_Guid_Control_1D:709' guid2DATA.launch_data.track    = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.track = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:710' guid2DATA.launch_data.wind_spd = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.wind_spd = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:711' guid2DATA.launch_data.wind_dir = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.wind_dir = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:715' telem.time20Hz = 0.050; */
    Guidance_100Hz_Function_1D_M->dwork.telem.time20Hz = 0.05;

    /* 'GUID_Embedded_Guid_Control_1D:716' telem.IW      = uint16(1); */
    Guidance_100Hz_Function_1D_M->dwork.telem.IW = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:717' telem.IW_last = uint16(1); */
    Guidance_100Hz_Function_1D_M->dwork.telem.IW_last = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:720' guidtel100Hz.GUI_mode        = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_mode = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:721' guidtel100Hz.GUI_FPbunt_cnt  = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPbunt_cnt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:722' guidtel100Hz.GUI_Status      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Status = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:723' guidtel100Hz.GUI_GuidTime    = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_GuidTime = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:724' guidtel100Hz.GUI_PhiCmd      = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_PhiCmd = 0;

    /* 'GUID_Embedded_Guid_Control_1D:725' guidtel100Hz.GUI_FPTheCmd    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPTheCmd = 0;

    /* 'GUID_Embedded_Guid_Control_1D:726' guidtel100Hz.GUI_FPGBYCmd    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBYCmd = 0;

    /* 'GUID_Embedded_Guid_Control_1D:727' guidtel100Hz.GUI_FPGBZCmd    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBZCmd = 0;

    /* 'GUID_Embedded_Guid_Control_1D:728' guidtel100Hz.GUI_NavCtrlFlag = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_NavCtrlFlag = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:729' guidtel100Hz.GUI_ENVtemp     = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_ENVtemp = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:730' guidtel100Hz.GUI_Qratio      = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Qratio = 0;

    /* 'GUID_Embedded_Guid_Control_1D:731' guidtel100Hz.GUI_BiasAlpha   = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_BiasAlpha = 0;

    /* 'GUID_Embedded_Guid_Control_1D:732' guidtel100Hz.GUI_Rarc_T      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Rarc_T = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:733' guidtel100Hz.GUI_CASErr      = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_CASErr = 0;

    /* 'GUID_Embedded_Guid_Control_1D:734' guidtel100Hz.GUI_AccLim      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_AccLim = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:735' guidtel100Hz.GUI_WingOpenCmd = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_WingOpenCmd = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:736' guidtel100Hz.GUI_WingRelCmd  = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_WingRelCmd = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:737' guidtel100Hz.GUI_SADFuze     = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_SADFuze = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:738' guidtel100Hz.GUI_SwitchforBunt = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_SwitchforBunt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:741' guidtel20HzA.GUI_rho         = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_rho = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:742' guidtel20HzA.GUI_Vsound      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vsound = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:743' guidtel20HzA.GUI_FPthem      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPthem = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:744' guidtel20HzA.GUI_Mach        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mach = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:745' guidtel20HzA.GUI_Alpha       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Alpha = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:746' guidtel20HzA.GUI_Beta        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Beta = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:747' guidtel20HzA.GUI_Vdyn        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vdyn = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:748' guidtel20HzA.GUI_Qdyn        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Qdyn = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:749' guidtel20HzA.GUI_Gcompx      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompx = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:750' guidtel20HzA.GUI_Gcompy      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompy = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:751' guidtel20HzA.GUI_Gcompz      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompz = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:752' guidtel20HzA.GUI_gbycD0      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_gbycD0 = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:753' guidtel20HzA.GUI_Clest       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Clest = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:754' guidtel20HzA.GUI_ClCd_M      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_ClCd_M = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:755' guidtel20HzA.GUI_CNgrav      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_CNgrav = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:756' guidtel20HzA.GUI_FPCmd0      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPCmd0 = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:757' guidtel20HzA.GUI_Qopt        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Qopt = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:758' guidtel20HzA.GUI_Vopt        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vopt = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:759' guidtel20HzA.GUI_Mopt        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mopt = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:760' guidtel20HzA.GUI_FPopt       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPopt = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:761' guidtel20HzA.GUI_FPerrD      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPerrD = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:762' guidtel20HzA.GUI_alpha_M     = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_alpha_M = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:763' guidtel20HzA.GUI_Mglideopt   = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mglideopt = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:764' guidtel20HzA.GUI_Kglide      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kglide = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:765' guidtel20HzA.GUI_Rapr_Tx     = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Tx = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:766' guidtel20HzA.GUI_Rapr_Ty     = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Ty = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:767' guidtel20HzA.GUI_Rapr_Tz     = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Tz = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:768' guidtel20HzA.GUI_Rref0x      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0x = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:769' guidtel20HzA.GUI_Rref0y      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0y = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:770' guidtel20HzA.GUI_Rref0z      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0z = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:771' guidtel20HzA.GUI_RrefWx      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWx = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:772' guidtel20HzA.GUI_RrefWy      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWy = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:773' guidtel20HzA.GUI_RrefWz      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWz = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:774' guidtel20HzA.GUI_Mpos0x      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0x = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:775' guidtel20HzA.GUI_Mpos0y      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0y = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:776' guidtel20HzA.GUI_Mpos0z      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0z = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:777' guidtel20HzA.GUI_RweaEx      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEx = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:778' guidtel20HzA.GUI_RweaEy      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEy = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:779' guidtel20HzA.GUI_RweaEz      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEz = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:780' guidtel20HzA.GUI_Ki          = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Ki = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:781' guidtel20HzA.GUI_Kp          = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kp = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:782' guidtel20HzA.GUI_Kv          = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kv = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:783' guidtel20HzA.GUI_Kfp         = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kfp = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:784' guidtel20HzA.GUI_XTPosY      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTPosY = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:785' guidtel20HzA.GUI_XTPosZ      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTPosZ = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:786' guidtel20HzA.GUI_XTVelY      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTVelY = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:787' guidtel20HzA.GUI_XTVelZ      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTVelZ = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:788' guidtel20HzA.GUI_XTAccY      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTAccY = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:789' guidtel20HzA.GUI_XTAccZ      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTAccZ = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:791' guidtel20HzB.GUI_iW          = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_iW = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:792' guidtel20HzB.GUI_solution    = int8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_solution = 0;

    /* 'GUID_Embedded_Guid_Control_1D:793' guidtel20HzB.GUI_inccnt      = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_inccnt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:794' guidtel20HzB.GUI_ftime       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ftime = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:795' guidtel20HzB.GUI_Rarc        = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rarc = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:796' guidtel20HzB.GUI_RarcW       = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:797' guidtel20HzB.GUI_Rterm       = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rterm = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:798' guidtel20HzB.GUI_Km          = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Km = 0;

    /* 'GUID_Embedded_Guid_Control_1D:799' guidtel20HzB.GUI_RtermT      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermT = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:800' guidtel20HzB.GUI_RtermW      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermW = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:801' guidtel20HzB.GUI_LT          = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LT = 0;

    /* 'GUID_Embedded_Guid_Control_1D:802' guidtel20HzB.GUI_LW          = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LW = 0;

    /* 'GUID_Embedded_Guid_Control_1D:803' guidtel20HzB.GUI_Mglide      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Mglide = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:804' guidtel20HzB.GUI_FPazi       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_FPazi = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:805' guidtel20HzB.GUI_ratio       = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ratio = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:806' guidtel20HzB.GUI_ImpAng      = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ImpAng = 0;

    /* 'GUID_Embedded_Guid_Control_1D:807' guidtel20HzB.GUI_AttHeading  = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_AttHeading = 0;

    /* 'GUID_Embedded_Guid_Control_1D:808' guidtel20HzB.GUI_intsctTx    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTx = 0;

    /* 'GUID_Embedded_Guid_Control_1D:809' guidtel20HzB.GUI_intsctTy    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:810' guidtel20HzB.GUI_intsctTz    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:811' guidtel20HzB.GUI_intsctWx    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWx = 0;

    /* 'GUID_Embedded_Guid_Control_1D:812' guidtel20HzB.GUI_intsctWy    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:813' guidtel20HzB.GUI_intsctWz    = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:814' guidtel20HzB.GUI_Rtox        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtox = 0;

    /* 'GUID_Embedded_Guid_Control_1D:815' guidtel20HzB.GUI_Rtoy        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:816' guidtel20HzB.GUI_Rtoz        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:817' guidtel20HzB.GUI_Rtex        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtex = 0;

    /* 'GUID_Embedded_Guid_Control_1D:818' guidtel20HzB.GUI_Rtey        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtey = 0;

    /* 'GUID_Embedded_Guid_Control_1D:819' guidtel20HzB.GUI_Rtez        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtez = 0;

    /* 'GUID_Embedded_Guid_Control_1D:820' guidtel20HzB.GUI_Rwox        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwox = 0;

    /* 'GUID_Embedded_Guid_Control_1D:821' guidtel20HzB.GUI_Rwoy        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:822' guidtel20HzB.GUI_Rwoz        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:823' guidtel20HzB.GUI_Rwex        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwex = 0;

    /* 'GUID_Embedded_Guid_Control_1D:824' guidtel20HzB.GUI_Rwey        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwey = 0;

    /* 'GUID_Embedded_Guid_Control_1D:825' guidtel20HzB.GUI_Rwez        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwez = 0;

    /* 'GUID_Embedded_Guid_Control_1D:826' guidtel20HzB.GUI_Rmx         = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmx = 0;

    /* 'GUID_Embedded_Guid_Control_1D:827' guidtel20HzB.GUI_Rmy         = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:828' guidtel20HzB.GUI_Rmz         = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:829' guidtel20HzB.GUI_a0Wx        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wx = 0;

    /* 'GUID_Embedded_Guid_Control_1D:830' guidtel20HzB.GUI_a0Wy        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wy = 0;

    /* 'GUID_Embedded_Guid_Control_1D:831' guidtel20HzB.GUI_a0Wz        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:832' guidtel20HzB.GUI_a0Tx        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tx = 0;

    /* 'GUID_Embedded_Guid_Control_1D:833' guidtel20HzB.GUI_a0Ty        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Ty = 0;

    /* 'GUID_Embedded_Guid_Control_1D:834' guidtel20HzB.GUI_a0Tz        = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tz = 0;

    /* 'GUID_Embedded_Guid_Control_1D:835' guidtel20HzB.GUI_spare       = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_spare = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:836' guidtel20HzB.GUI_tgt0x       = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0x = 0;

    /* 'GUID_Embedded_Guid_Control_1D:837' guidtel20HzB.GUI_tgt0y       = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0y = 0;

    /* 'GUID_Embedded_Guid_Control_1D:838' guidtel20HzB.GUI_tgt0z       = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0z = 0;

    /* 'GUID_Embedded_Guid_Control_1D:839' guidtel20HzB.GUI_altcnt      = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_altcnt = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:840' guidtel20HzB.GUI_R2buntREQ   = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntREQ = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:841' guidtel20HzB.GUI_R2buntAVL   = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntAVL = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:842' guidtel20HzB.GUI_RarcW_cmp   = uint16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW_cmp = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:843' guidtel20HzB.GUI_theFP3D     = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP3D = 0;

    /* 'GUID_Embedded_Guid_Control_1D:844' guidtel20HzB.GUI_theFP2D     = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP2D = 0;

    /* 'GUID_Embedded_Guid_Control_1D:845' guidtel20HzB.GUI_theErr      = int16(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theErr = 0;

    /* 'GUID_Embedded_Guid_Control_1D:846' guidtel20HzB.GUI_ang8        = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ang8 = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:847' guidtel20HzB.GUI_angnow      = single(0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_angnow = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:849' N = 1000; */
    /* 'GUID_Embedded_Guid_Control_1D:850' guidtel_fpath.GUI_iW          = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:851' guidtel_fpath.GUI_solution    = int8(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:852' guidtel_fpath.GUI_inccnt      = uint8(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:853' guidtel_fpath.GUI_ftime       = single(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:854' guidtel_fpath.GUI_Rarc        = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:855' guidtel_fpath.GUI_RarcW       = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:856' guidtel_fpath.GUI_Rterm       = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:857' guidtel_fpath.GUI_Km          = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:858' guidtel_fpath.GUI_RtermT      = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:859' guidtel_fpath.GUI_RtermW      = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:860' guidtel_fpath.GUI_LT          = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:861' guidtel_fpath.GUI_LW          = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:862' guidtel_fpath.GUI_Mglide      = single(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:863' guidtel_fpath.GUI_FPazi       = single(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:864' guidtel_fpath.GUI_ratio       = single(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:865' guidtel_fpath.GUI_ImpAng      = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:866' guidtel_fpath.GUI_AttHeading  = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:867' guidtel_fpath.GUI_intsctTx    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:868' guidtel_fpath.GUI_intsctTy    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:869' guidtel_fpath.GUI_intsctTz    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:870' guidtel_fpath.GUI_intsctWx    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:871' guidtel_fpath.GUI_intsctWy    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:872' guidtel_fpath.GUI_intsctWz    = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:873' guidtel_fpath.GUI_Rtox        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:874' guidtel_fpath.GUI_Rtoy        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:875' guidtel_fpath.GUI_Rtoz        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:876' guidtel_fpath.GUI_Rtex        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:877' guidtel_fpath.GUI_Rtey        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:878' guidtel_fpath.GUI_Rtez        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:879' guidtel_fpath.GUI_Rwox        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:880' guidtel_fpath.GUI_Rwoy        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:881' guidtel_fpath.GUI_Rwoz        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:882' guidtel_fpath.GUI_Rwex        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:883' guidtel_fpath.GUI_Rwey        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:884' guidtel_fpath.GUI_Rwez        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:885' guidtel_fpath.GUI_Rmx         = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:886' guidtel_fpath.GUI_Rmy         = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:887' guidtel_fpath.GUI_Rmz         = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:888' guidtel_fpath.GUI_a0Wx        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:889' guidtel_fpath.GUI_a0Wy        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:890' guidtel_fpath.GUI_a0Wz        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:891' guidtel_fpath.GUI_a0Tx        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:892' guidtel_fpath.GUI_a0Ty        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:893' guidtel_fpath.GUI_a0Tz        = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:894' guidtel_fpath.GUI_spare       = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:895' guidtel_fpath.GUI_tgt0x       = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:896' guidtel_fpath.GUI_tgt0y       = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:897' guidtel_fpath.GUI_tgt0z       = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:898' guidtel_fpath.GUI_altcnt      = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:899' guidtel_fpath.GUI_R2buntREQ   = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:900' guidtel_fpath.GUI_R2buntAVL   = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:901' guidtel_fpath.GUI_RarcW_cmp   = uint16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:902' guidtel_fpath.GUI_theFP3D     = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:903' guidtel_fpath.GUI_theFP2D     = int16(zeros(N,1)); */
    /* 'GUID_Embedded_Guid_Control_1D:904' guidtel_fpath.GUI_theErr      = int16(zeros(N,1)); */
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_iW[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_solution[0], 0,
           1000U * (sizeof(int8_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_inccnt[0], 0,
           1000U * (sizeof(uint8_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_ftime[0], 0,
           1000U * (sizeof(real32_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rarc[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_RarcW[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rterm[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Km[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_RtermT[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_RtermW[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_LT[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_LW[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Mglide[0], 0,
           1000U * (sizeof(real32_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_FPazi[0], 0,
           1000U * (sizeof(real32_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_ratio[0], 0,
           1000U * (sizeof(real32_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_ImpAng[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_AttHeading[0],
           0, 1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctTx[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctTy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctTz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctWx[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctWy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_intsctWz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtox[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtoy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtoz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtex[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtey[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rtez[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwox[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwoy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwoz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwex[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwey[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rwez[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rmx[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rmy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_Rmz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Wx[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Wy[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Wz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Tx[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Ty[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_a0Tz[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_spare[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_tgt0x[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_tgt0y[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_tgt0z[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_altcnt[0], 0,
           1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_R2buntREQ[0],
           0, 1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_R2buntAVL[0],
           0, 1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_RarcW_cmp[0],
           0, 1000U * (sizeof(uint16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_theFP3D[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_theFP2D[0], 0,
           1000U * (sizeof(int16_T)));
    memset(&Guidance_100Hz_Function_1D_M->dwork.guidtel_fpath.GUI_theErr[0], 0,
           1000U * (sizeof(int16_T)));

    /* 'GUID_Embedded_Guid_Control_1D:907' RrefL = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:908' RrefC = [0.0 0.0 0.0]; */
    /* 'GUID_Embedded_Guid_Control_1D:909' t_threshold = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:910' angdot = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:912' GndRge2Tgt = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:913' GndPosN = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.GndPosN = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:914' GndPosE = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.GndPosE = 0.0;
  } else {
    /* 'GUID_Embedded_Guid_Control_1D:916' else */
    /* 'GUID_Embedded_Guid_Control_1D:918' NAV_in.pos_g.lat  = LIB_checkNAN(NavSolution_in.pos_g.lat ,NAV_in.pos_g.lat ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.lat)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lat =
        rtU_navstatein->pos_g.lat;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:919' NAV_in.pos_g.lon  = LIB_checkNAN(NavSolution_in.pos_g.lon ,NAV_in.pos_g.lon ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.lon)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lon =
        rtU_navstatein->pos_g.lon;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:920' NAV_in.pos_g.alt  = LIB_checkNAN(NavSolution_in.pos_g.alt ,NAV_in.pos_g.alt ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.alt)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.alt =
        rtU_navstatein->pos_g.alt;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:921' NAV_in.pos_e.x    = LIB_checkNAN(NavSolution_in.pos_e.x   ,NAV_in.pos_e.x   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.x)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.x =
        rtU_navstatein->pos_e.x;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:922' NAV_in.pos_e.y    = LIB_checkNAN(NavSolution_in.pos_e.y   ,NAV_in.pos_e.y   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.y)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.y =
        rtU_navstatein->pos_e.y;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:923' NAV_in.pos_e.z    = LIB_checkNAN(NavSolution_in.pos_e.z   ,NAV_in.pos_e.z   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.z)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.z =
        rtU_navstatein->pos_e.z;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:924' NAV_in.vel_e.x    = LIB_checkNAN(NavSolution_in.vel_e.x   ,NAV_in.vel_e.x   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.x)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.x =
        rtU_navstatein->vel_e.x;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:925' NAV_in.vel_e.y    = LIB_checkNAN(NavSolution_in.vel_e.y   ,NAV_in.vel_e.y   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.y)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.y =
        rtU_navstatein->vel_e.y;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:926' NAV_in.vel_e.z    = LIB_checkNAN(NavSolution_in.vel_e.z   ,NAV_in.vel_e.z   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.z)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.vel_e.z =
        rtU_navstatein->vel_e.z;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:927' NAV_in.att_bn.phi = LIB_checkNAN(NavSolution_in.att_bn.phi,NAV_in.att_bn.phi,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.phi)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.phi =
        rtU_navstatein->att_bn.phi;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:928' NAV_in.att_bn.the = LIB_checkNAN(NavSolution_in.att_bn.the,NAV_in.att_bn.the,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.the)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.the =
        rtU_navstatein->att_bn.the;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:929' NAV_in.att_bn.psi = LIB_checkNAN(NavSolution_in.att_bn.psi,NAV_in.att_bn.psi,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.psi)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.att_bn.psi =
        rtU_navstatein->att_bn.psi;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:930' NAV_in.fib_e.z    = LIB_checkNAN(NavSolution_in.fib_e.z   ,NAV_in.fib_e.z   ,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->fib_e.z)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.fib_e.z =
        rtU_navstatein->fib_e.z;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:932' IMU_in.gbxm = LIB_checkNAN(NavIMU_in.dvelocity.x/delt,IMU_in.gbxm,1,0); */
    ftime = rtU_IMUin->dvelocity.x / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbxm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:933' IMU_in.gbym = LIB_checkNAN(NavIMU_in.dvelocity.y/delt,IMU_in.gbym,1,0); */
    ftime = rtU_IMUin->dvelocity.y / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbym = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:934' IMU_in.gbzm = LIB_checkNAN(NavIMU_in.dvelocity.z/delt,IMU_in.gbzm,1,0); */
    ftime = rtU_IMUin->dvelocity.z / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbzm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:935' IMU_in.wbxm = LIB_checkNAN(NavIMU_in.dangle.x/delt   ,IMU_in.wbxm,1,0); */
    ftime = rtU_IMUin->dangle.x / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbxm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:936' IMU_in.wbym = LIB_checkNAN(NavIMU_in.dangle.y/delt   ,IMU_in.wbym,1,0); */
    ftime = rtU_IMUin->dangle.y / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbym = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:937' IMU_in.wbzm = LIB_checkNAN(NavIMU_in.dangle.z/delt   ,IMU_in.wbzm,1,0); */
    ftime = rtU_IMUin->dangle.z / Guidance_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.IMU_in.wbzm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:939' FC_in.Temp = LIB_checkNAN(FlightCond_in.Temp,FC_in.Temp,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_Pitotin->Temp)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.FC_in.Temp = rtU_Pitotin->Temp;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:940' FC_in.Psta = LIB_checkNAN(FlightCond_in.Psta,FC_in.Psta,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_Pitotin->Psta)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.FC_in.Psta = rtU_Pitotin->Psta;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:941' FC_in.Pdyn = LIB_checkNAN(FlightCond_in.Pdyn,FC_in.Pdyn,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_Pitotin->Pdyn)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      Guidance_100Hz_Function_1D_M->dwork.FC_in.Pdyn = rtU_Pitotin->Pdyn;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_Control_1D:943' if (Discretes_in.umbilical) */
    if (((int32_T)rtU_Discretesin->umbilical) != 0) {
      /* 'GUID_Embedded_Guid_Control_1D:945' ftime = 0.0; */
      ftime = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:946' ftime0 = LIB_checkNAN(SysTime_in,ftime0,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(rtU_SysTimein)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        Guidance_100Hz_Function_1D_M->dwork.ftime0 = rtU_SysTimein;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_Control_1D:947' guidmode = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidmode = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:948' clos.once = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.clos.once = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:949' clos.ArcState.Rarc_T = 100000.0; */
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_T = 100000.0;

      /* 'GUID_Embedded_Guid_Control_1D:950' clos.ArcState.Rapr_T = [0 0 0]; */
      /* 'GUID_Embedded_Guid_Control_1D:951' clos.ArcState.Oarc_T = [0 0 0]; */
      /* 'GUID_Embedded_Guid_Control_1D:952' clos.ArcState.Rarc_W = 100000.0; */
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_W = 100000.0;

      /* 'GUID_Embedded_Guid_Control_1D:953' clos.ArcState.Rapr_Rm= [0 0 0]; */
      /* 'GUID_Embedded_Guid_Control_1D:954' clos.ArcState.Oarc_W = [0 0 0]; */
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[2] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_T[2] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_Rm[2] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Oarc_W[2] = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:955' newmeas_cnt = uint32(0); */
      Guidance_100Hz_Function_1D_M->dwork.newmeas_cnt = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:956' TRACK.log_FP  = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.log_FP = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:957' TRACK.once    = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.once = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:958' TRACK.reset   = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:959' TRACK.bunt_arc = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.bunt_arc = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:960' TRACK.reset3cnt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.reset3cnt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:961' TRACK.Rlim_lo = 100.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 100.0;

      /* 'GUID_Embedded_Guid_Control_1D:962' TRACK.Roff_intg = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Roff_intg = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:963' TRACK.ftime_step = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.ftime_step = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:964' TRACK.mintime = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mintime = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:965' TRACK.FPbunt_cnt = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.FPbunt_cnt = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:966' TRACK.solution= int8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.solution = 0;

      /* 'GUID_Embedded_Guid_Control_1D:967' TRACK.Mtgt0 = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Mtgt0 = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:968' TRACK.Mglide  = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Mglide = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:969' TRACK.psiR    = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.psiR = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:970' TRACK.FPazi   = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.FPazi = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:971' TRACK.Lfpath2bunt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2bunt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:972' TRACK.Lfpath2tgt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2tgt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:973' TRACK.ratio   = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.ratio = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:974' TRACK.rge2bunt= 10000.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.rge2bunt = 10000.0;

      /* 'GUID_Embedded_Guid_Control_1D:975' TRACK.PosRst  = 100.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.PosRst = 100.0;

      /* 'GUID_Embedded_Guid_Control_1D:976' TRACK.latch  = [uint8(0) uint8(0)]; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.latch[0] = 0U;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.latch[1] = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:977' TRACK.Ut0    = [0.0 0.0 -1.0]'; */
      /* 'GUID_Embedded_Guid_Control_1D:978' TRACK.Ut8    = [0.0 0.0 -1.0]'; */
      /* 'GUID_Embedded_Guid_Control_1D:979' TRACK.Ut     = [0.0 0.0 -1.0]'; */
      /* 'GUID_Embedded_Guid_Control_1D:980' TRACK.Kglide = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Kglide = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:981' TRACK.AprAngErr = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.AprAngErr = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:982' TRACK.re0.x = NAV_in.pos_e.x; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.x =
        Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.x;

      /* 'GUID_Embedded_Guid_Control_1D:983' TRACK.re0.y = NAV_in.pos_e.y; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.y =
        Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.y;

      /* 'GUID_Embedded_Guid_Control_1D:984' TRACK.re0.z = NAV_in.pos_e.z; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.re0.z =
        Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.z;

      /* 'GUID_Embedded_Guid_Control_1D:985' TRACK.mpos0.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:986' TRACK.mpos0.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:987' TRACK.mpos0.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:988' TRACK.Rto.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:989' TRACK.Rto.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:990' TRACK.Rto.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rto.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:991' TRACK.Rte.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:992' TRACK.Rte.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:993' TRACK.Rte.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rte.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:994' TRACK.Rwo.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:995' TRACK.Rwo.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:996' TRACK.Rwo.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwo.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:997' TRACK.Rwe.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:998' TRACK.Rwe.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:999' TRACK.Rwe.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1000' TRACK.Rmwo.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1001' TRACK.Rmwo.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1002' TRACK.Rmwo.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmwo.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1003' TRACK.Rmto.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1004' TRACK.Rmto.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1005' TRACK.Rmto.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmto.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1006' TRACK.Rm.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1007' TRACK.Rm.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1008' TRACK.Rm.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rm.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1009' TRACK.RarcT = 1000.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.RarcT = 1000.0;

      /* 'GUID_Embedded_Guid_Control_1D:1010' TRACK.OarcT.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1011' TRACK.OarcT.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1012' TRACK.OarcT.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcT.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1013' TRACK.RarcW = 1000.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.RarcW = 1000.0;

      /* 'GUID_Embedded_Guid_Control_1D:1014' TRACK.OarcW.x = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.x = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1015' TRACK.OarcW.y = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.y = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1016' TRACK.OarcW.z = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.OarcW.z = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1017' TRACK.AZrefL = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.AZrefL = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:1018' TRACK.AZref = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.AZref = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:1019' TRACK.Time8 = 1000.0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Time8 = 1000.0;

      /* 'GUID_Embedded_Guid_Control_1D:1020' TRACK.Umis = [0.0 0.0 0.0]; */
      /* 'GUID_Embedded_Guid_Control_1D:1021' TRACK.Rmis0 = [0.0 0.0 0.0]; */
      /* 'GUID_Embedded_Guid_Control_1D:1022' TRACK.Umis0 = [0.0 0.0 0.0]; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[0] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[1] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[2] = -1.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[2] = -1.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[2] = -1.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis[2] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rmis0[2] = 0.0;
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Umis0[2] = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1023' OPT_FP.M = 0.6; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.M = 0.6;

      /* 'GUID_Embedded_Guid_Control_1D:1024' OPT_FP.Mopt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mopt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1025' OPT_FP.Mglide_opt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mglide_opt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1026' OPT_FP.FPopt = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPopt = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1027' OPT_FP.FPopt0 = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPopt0 = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1028' OPT_FP.Cd0 = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Cd0 = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1029' OPT_FP.FPCmd0 = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPCmd0 = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1030' OPT_FP.FPthec = double(-MP_in.DiveAngle); */
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPthec = (real_T)((real32_T)
        (-rtU_MPin->DiveAngle));

      /* 'GUID_Embedded_Guid_Control_1D:1031' telem.time20Hz = 0.050; */
      Guidance_100Hz_Function_1D_M->dwork.telem.time20Hz = 0.05;

      /* 'GUID_Embedded_Guid_Control_1D:1032' for i = 1:39 */
      for (ValidGNSSNav = 0; ValidGNSSNav < 39; ValidGNSSNav++) {
        /* 'GUID_Embedded_Guid_Control_1D:1033' if (i <= WIND_in.WindEntryCnt) */
        if ((1 + ValidGNSSNav) <= ((int32_T)rtU_Windin->WindEntryCnt)) {
          /* 'GUID_Embedded_Guid_Control_1D:1034' WindTab_Alt(i,1) = double(WIND_in.WindEntries(i).Altitude); */
          Guidance_100Hz_Function_1D_M->dwork.WindTab_Alt[ValidGNSSNav] =
            (real_T)rtU_Windin->WindEntries[ValidGNSSNav].Altitude;

          /* 'GUID_Embedded_Guid_Control_1D:1035' WindTab_VN(i,1)  = double(WIND_in.WindEntries(i).Speed)*cos(double(WIND_in.WindEntries(i).Direction)*cnsts.d2r); */
          gbzc = ((real_T)rtU_Windin->WindEntries[ValidGNSSNav].Direction) *
            0.017453292519943295;
          Guidance_100Hz_Function_1D_M->dwork.WindTab_VN[ValidGNSSNav] = cos
            (gbzc) * ((real_T)rtU_Windin->WindEntries[ValidGNSSNav].Speed);

          /* 'GUID_Embedded_Guid_Control_1D:1036' WindTab_VE(i,1)  = double(WIND_in.WindEntries(i).Speed)*sin(double(WIND_in.WindEntries(i).Direction)*cnsts.d2r); */
          Guidance_100Hz_Function_1D_M->dwork.WindTab_VE[ValidGNSSNav] = sin
            (gbzc) * ((real_T)rtU_Windin->WindEntries[ValidGNSSNav].Speed);
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:1037' else */
          /* 'GUID_Embedded_Guid_Control_1D:1038' WindTab_Alt(i,1) = double(WindTab_Alt(i-1,1))+1.0; */
          Guidance_100Hz_Function_1D_M->dwork.WindTab_Alt[ValidGNSSNav] =
            Guidance_100Hz_Function_1D_M->dwork.WindTab_Alt[ValidGNSSNav - 1] +
            1.0;

          /* 'GUID_Embedded_Guid_Control_1D:1039' WindTab_VN(i,1)  = double(WindTab_VN(i-1,1)); */
          Guidance_100Hz_Function_1D_M->dwork.WindTab_VN[ValidGNSSNav] =
            Guidance_100Hz_Function_1D_M->dwork.WindTab_VN[ValidGNSSNav - 1];

          /* 'GUID_Embedded_Guid_Control_1D:1040' WindTab_VE(i,1)  = double(WindTab_VE(i-1,1)); */
          Guidance_100Hz_Function_1D_M->dwork.WindTab_VE[ValidGNSSNav] =
            Guidance_100Hz_Function_1D_M->dwork.WindTab_VE[ValidGNSSNav - 1];
        }
      }

      /* 'GUID_Embedded_Guid_Control_1D:1043' guidcmds.AZy = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZy = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:1044' guidcmds.AZz = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZz = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:1045' guidcmds.gbycD0 = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbycD0 = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1046' guidcmds.gbzcD = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzcD = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1047' guidcmds.inty = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.inty = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1048' guidcmds.intz = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.intz = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1049' guidcmds.gbzc_unl = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzc_unl = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1050' guidcmds.gbyc_unl = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbyc_unl = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1051' guidcmds.gbzc_FOV = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzc_FOV = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1052' guidcmds.gbyc_FOV = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbyc_FOV = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1053' guidcmds.intz_FOV = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.intz_FOV = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1054' guidcmds.inty_FOV = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.inty_FOV = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1055' guidcmds.gizc = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gizc = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1056' guidcmds.giyc = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1057' guidcmds.eta_int = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.eta_int = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1058' guidcmds.eps_int = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.eps_int = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1059' guidcmds.phic_hold = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.phic_hold = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1060' guidcmds.phi_int = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_int = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1061' guidcmds.phi_gic = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_gic = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1062' guidcmds.phic_prev = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.phic_prev = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1063' guidcmds.verticaldive = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.verticaldive = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1064' guidcmds.gmode_sw = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1065' guidcmds.PurePropNav = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.PurePropNav = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1066' guidcmds.forcegmode5 = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.forcegmode5 = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1067' guidcmds.gb_ratio = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gb_ratio = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1068' guidcmds.gb_ratio_prev = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gb_ratio_prev = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1069' guidcmds.giyc_int = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int = 0.0;

      /* 'GUID_Embedded_Guid_Control_1D:1070' guidcmds.navctrlreset = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.navctrlreset = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1072' [TPosNED,TPosNED_uncond,MPosNED,MVelNED,MRefNED,DCM] = TransformationsNED(TTKF_in,NAV_in,clos.Rref,TRACK.re0); */
      TransformationsNED(rtU_ttkfin->tgtrex, rtU_ttkfin->tgtrey,
                         rtU_ttkfin->tgtrez, rtU_ttkfin->tgtrex_uncond,
                         rtU_ttkfin->tgtrey_uncond, rtU_ttkfin->tgtrez_uncond,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_g.lat,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_g.lon,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.z,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.z,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.att_bn,
                         (Guidance_100Hz_Function_1D_M)->dwork.clos.Rref,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.z,
                         &TPosNED, &TPosNED_uncond, &MPosNED, &MVelNED, &MRefNED,
                         &DCM);
      gbzc = TPosNED.x;
      gbyc = TPosNED.y;
      psiLOSb = TPosNED.z;

      /* 'GUID_Embedded_Guid_Control_1D:1073' TPosNED_uncond_prev.x = TPosNED_uncond.x; */
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.x =
        TPosNED_uncond.x;

      /* 'GUID_Embedded_Guid_Control_1D:1074' TPosNED_uncond_prev.y = TPosNED_uncond.y; */
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.y =
        TPosNED_uncond.y;

      /* 'GUID_Embedded_Guid_Control_1D:1075' TPosNED_uncond_prev.z = TPosNED_uncond.z; */
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.z =
        TPosNED_uncond.z;

      /* 'GUID_Embedded_Guid_Control_1D:1076' if (MP_in.PriTgtLatitude == 0.0 || MP_in.PriTgtLongitude == 0.0) */
      if ((rtU_MPin->PriTgtLatitude == 0.0) || (rtU_MPin->PriTgtLongitude == 0.0))
      {
        /* 'GUID_Embedded_Guid_Control_1D:1077' opportunity_mode = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.opportunity_mode = 1U;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1078' else */
        /* 'GUID_Embedded_Guid_Control_1D:1079' opportunity_mode = uint8(0); */
        Guidance_100Hz_Function_1D_M->dwork.opportunity_mode = 0U;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1082' GndRge2Tgt = sqrt((TPosNED.x-MPosNED.x)^2+(TPosNED.y-MPosNED.y)^2); */
      phic = TPosNED.x - MPosNED.x;
      psiLOSi = TPosNED.y - MPosNED.y;
      Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = (phic * phic) + (psiLOSi *
        psiLOSi);
      Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = sqrt
        (Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt);

      /* 'GUID_Embedded_Guid_Control_1D:1083' GndPosN = MPosNED.x; */
      Guidance_100Hz_Function_1D_M->dwork.GndPosN = MPosNED.x;

      /* 'GUID_Embedded_Guid_Control_1D:1084' GndPosE = MPosNED.y; */
      Guidance_100Hz_Function_1D_M->dwork.GndPosE = MPosNED.y;

      /* 'GUID_Embedded_Guid_Control_1D:1086' wingsopen_timeout = 1000.0; */
      Guidance_100Hz_Function_1D_M->dwork.wingsopen_timeout = 1000.0;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1088' else */
      /* 'GUID_Embedded_Guid_Control_1D:1091' ftime = SysTime_in - ftime0; */
      ftime = rtU_SysTimein - Guidance_100Hz_Function_1D_M->dwork.ftime0;

      /* 'GUID_Embedded_Guid_Control_1D:1094' [TPosNED,TPosNED_uncond,MPosNED,MVelNED,MRefNED,DCM] = TransformationsNED(TTKF_in,NAV_in,clos.Rref,TRACK.re0); */
      TransformationsNED(rtU_ttkfin->tgtrex, rtU_ttkfin->tgtrey,
                         rtU_ttkfin->tgtrez, rtU_ttkfin->tgtrex_uncond,
                         rtU_ttkfin->tgtrey_uncond, rtU_ttkfin->tgtrez_uncond,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_g.lat,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_g.lon,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e.z,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.vel_e.z,
                         (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.att_bn,
                         (Guidance_100Hz_Function_1D_M)->dwork.clos.Rref,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.x,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.y,
                         (Guidance_100Hz_Function_1D_M)->dwork.TRACK.re0.z,
                         &TPosNED, &TPosNED_uncond, &MPosNED, &MVelNED, &MRefNED,
                         &DCM);
      gbzc = TPosNED.x;
      gbyc = TPosNED.y;
      psiLOSb = TPosNED.z;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1098' if (ftime > 7.00) */
    /* 'GUID_Embedded_Guid_Control_1D:1101' if (ftime > 40.00) */
    /* 'GUID_Embedded_Guid_Control_1D:1104' if (ftime > 51.83) */
    /* 'GUID_Embedded_Guid_Control_1D:1107' if (ftime > 165.00) */
    /* 'GUID_Embedded_Guid_Control_1D:1113' WVN = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:1114' WVE = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:1116' if (NavMode_in >= 30 && (rem(NavMode_in,10)==0 || rem(NavMode_in,10)==1)) */
    if (((int32_T)rtU_navmodein) >= 30) {
      switch ((uint8_T)((int32_T)(((int32_T)rtU_navmodein) % 10))) {
       case 0:
        /* 'GUID_Embedded_Guid_Control_1D:1117' ValidGNSSNav = 1; */
        ValidGNSSNav = 1;
        break;

       case 1:
        /* 'GUID_Embedded_Guid_Control_1D:1117' ValidGNSSNav = 1; */
        ValidGNSSNav = 1;
        break;

       default:
        /* 'GUID_Embedded_Guid_Control_1D:1118' else */
        /* 'GUID_Embedded_Guid_Control_1D:1119' ValidGNSSNav = 0; */
        ValidGNSSNav = 0;
        break;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1118' else */
      /* 'GUID_Embedded_Guid_Control_1D:1119' ValidGNSSNav = 0; */
      ValidGNSSNav = 0;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1124' ENV = GetAirdataParams(ENV,NAV_in,MVelNED,DCM,FC_in,ValidGNSSNav,IMU_in,WVN,WVE,fromDATA,ftime,delt,cnsts); */
    GetAirdataParams(&(Guidance_100Hz_Function_1D_M)->dwork.ENV,
                     (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_g.alt,
                     MVelNED, &DCM, (Guidance_100Hz_Function_1D_M)
                     ->dwork.FC_in.Temp, (Guidance_100Hz_Function_1D_M)
                     ->dwork.FC_in.Psta, (Guidance_100Hz_Function_1D_M)
                     ->dwork.FC_in.Pdyn, (real_T)ValidGNSSNav,
                     (Guidance_100Hz_Function_1D_M)->dwork.IMU_in.gbym,
                     (Guidance_100Hz_Function_1D_M)->dwork.IMU_in.gbzm,
                     (Guidance_100Hz_Function_1D_M)->dwork.IMU_in.wbym,
                     (Guidance_100Hz_Function_1D_M)->dwork.IMU_in.wbzm,
                     rtU_DATAguid, ftime, (Guidance_100Hz_Function_1D_M)
                     ->dwork.delt, &rtGUIDConstP.GuidanceController_cnsts);

    /* 'GUID_Embedded_Guid_Control_1D:1126' guid2DATA.umbilical            = uint8(Discretes_in.umbilical); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.umbilical =
      rtU_Discretesin->umbilical;

    /* 'GUID_Embedded_Guid_Control_1D:1127' guid2DATA.NavActive            = uint8(1); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.NavActive = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:1128' guid2DATA.OpportunityMode      = uint8(opportunity_mode); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.OpportunityMode =
      Guidance_100Hz_Function_1D_M->dwork.opportunity_mode;

    /* 'GUID_Embedded_Guid_Control_1D:1129' guid2DATA.Spare01              = uint8(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.Spare01 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:1130' guid2DATA.Spare02              = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.Spare02 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:1131' guid2DATA.tgt_data.valid       = uint32(0); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.valid = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:1132' guid2DATA.tgt_data.valid       = bitset(guid2DATA.tgt_data.valid,1,bitget(TTKF_in.status,2)); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.valid = (uint32_T)
      (((((uint32_T)rtU_ttkfin->status) & 2U) != 0U) ? 1 : 0);

    /* 'GUID_Embedded_Guid_Control_1D:1133' guid2DATA.tgt_data.valid       = bitset(guid2DATA.tgt_data.valid,2,MP_in.AttackHeadingValid); */
    if (((int32_T)rtU_MPin->AttackHeadingValid) != 0) {
      Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.valid |= 2U;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.valid &=
        4294967293U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1134' guid2DATA.tgt_data.rgx         = TPosNED_uncond.x; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgx =
      TPosNED_uncond.x;

    /* 'GUID_Embedded_Guid_Control_1D:1135' guid2DATA.tgt_data.rgy         = TPosNED_uncond.y; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgy =
      TPosNED_uncond.y;

    /* 'GUID_Embedded_Guid_Control_1D:1136' guid2DATA.tgt_data.rgz         = TPosNED_uncond.z; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rgz =
      TPosNED_uncond.z;

    /* 'GUID_Embedded_Guid_Control_1D:1137' guid2DATA.tgt_data.rex         = TTKF_in.tgtrex_uncond; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rex =
      rtU_ttkfin->tgtrex_uncond;

    /* 'GUID_Embedded_Guid_Control_1D:1138' guid2DATA.tgt_data.rey         = TTKF_in.tgtrey_uncond; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rey =
      rtU_ttkfin->tgtrey_uncond;

    /* 'GUID_Embedded_Guid_Control_1D:1139' guid2DATA.tgt_data.rez         = TTKF_in.tgtrez_uncond; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.rez =
      rtU_ttkfin->tgtrez_uncond;

    /* 'GUID_Embedded_Guid_Control_1D:1143' guid2DATA.tgt_data.alt         = double(MP_in.PriTgtAltitude); */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.alt = (real_T)
      rtU_MPin->PriTgtAltitude;

    /* 'GUID_Embedded_Guid_Control_1D:1144' guid2DATA.tgt_data.dive        = double(MP_in.DiveAngle)*cnsts.r2d; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.dive = ((real_T)
      rtU_MPin->DiveAngle) * 57.295779513082323;

    /* 'GUID_Embedded_Guid_Control_1D:1145' guid2DATA.tgt_data.arrive      = double(MP_in.AttackHeading)*cnsts.r2d; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.arrive = ((real_T)
      rtU_MPin->AttackHeading) * 57.295779513082323;

    /* 'GUID_Embedded_Guid_Control_1D:1146' guid2DATA.tgt_data.speed       = 250.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.tgt_data.speed = 250.0;

    /* 'GUID_Embedded_Guid_Control_1D:1147' guid2DATA.launch_data.lat      = NAV_in.pos_g.lat; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.lat =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lat;

    /* 'GUID_Embedded_Guid_Control_1D:1148' guid2DATA.launch_data.lon      = NAV_in.pos_g.lon; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.lon =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.lon;

    /* 'GUID_Embedded_Guid_Control_1D:1149' guid2DATA.launch_data.alt      = NAV_in.pos_g.alt; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.alt =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_g.alt;

    /* 'GUID_Embedded_Guid_Control_1D:1150' guid2DATA.launch_data.rex      = NAV_in.pos_e.x; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rex =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.x;

    /* 'GUID_Embedded_Guid_Control_1D:1151' guid2DATA.launch_data.rey      = NAV_in.pos_e.y; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rey =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.y;

    /* 'GUID_Embedded_Guid_Control_1D:1152' guid2DATA.launch_data.rez      = NAV_in.pos_e.z; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.rez =
      Guidance_100Hz_Function_1D_M->dwork.NAV_in.pos_e.z;

    /* 'GUID_Embedded_Guid_Control_1D:1153' guid2DATA.launch_data.Vgnd     = ENV.Vnav; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.Vgnd =
      Guidance_100Hz_Function_1D_M->dwork.ENV.Vnav;

    /* 'GUID_Embedded_Guid_Control_1D:1154' guid2DATA.launch_data.mach     = ENV.mach; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.mach =
      Guidance_100Hz_Function_1D_M->dwork.ENV.mach;

    /* 'GUID_Embedded_Guid_Control_1D:1155' guid2DATA.launch_data.track    = ENV.FPpsim; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.track =
      Guidance_100Hz_Function_1D_M->dwork.ENV.FPpsim;

    /* 'GUID_Embedded_Guid_Control_1D:1156' guid2DATA.launch_data.wind_spd = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.wind_spd = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:1157' guid2DATA.launch_data.wind_dir = 0.0; */
    Guidance_100Hz_Function_1D_M->dwork.guid2DATA.launch_data.wind_dir = 0.0;

    /* 'GUID_Embedded_Guid_Control_1D:1160' Uv = LIB_unitvector([MVelNED.x MVelNED.y MVelNED.z]); */
    /* 'GUID_Embedded_Guid_Control_1D:1163' tgt0.x = TPosNED.x - MPosNED.x; */
    gbzc -= MPosNED.x;

    /* 'GUID_Embedded_Guid_Control_1D:1164' tgt0.y = TPosNED.y - MPosNED.y; */
    gbyc -= MPosNED.y;

    /* 'GUID_Embedded_Guid_Control_1D:1165' tgt0.z = TPosNED.z - MPosNED.z; */
    phic = psiLOSb - MPosNED.z;

    /* 'GUID_Embedded_Guid_Control_1D:1201' if (TRACK.reset) */
    if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.reset) != 0) {
      /* 'GUID_Embedded_Guid_Control_1D:1202' if (TRACK.Lfpath2tgt > abs(tgt0.z)) */
      if (Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2tgt > fabs(phic)) {
        /* 'GUID_Embedded_Guid_Control_1D:1203' GndRge2Tgt = sqrt(TRACK.Lfpath2tgt^2 - tgt0.z^2); */
        Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt =
          (Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2tgt *
           Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2tgt) - (phic * phic);
        Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = sqrt
          (Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt);
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1204' else */
        /* 'GUID_Embedded_Guid_Control_1D:1205' GndRge2Tgt = sqrt(tgt0.x^2 + tgt0.y^2); */
        Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = (gbzc * gbzc) + (gbyc *
          gbyc);
        Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt = sqrt
          (Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt);
      }

      /* 'GUID_Embedded_Guid_Control_1D:1207' GndPosN  = MPosNED.x; */
      Guidance_100Hz_Function_1D_M->dwork.GndPosN = MPosNED.x;

      /* 'GUID_Embedded_Guid_Control_1D:1208' GndPosE  = MPosNED.y; */
      Guidance_100Hz_Function_1D_M->dwork.GndPosE = MPosNED.y;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1210' if (guidmode < 9) */
    if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) < 9) {
      /* 'GUID_Embedded_Guid_Control_1D:1212' guidcmds.GndRge = GndRge2Tgt - sqrt((GndPosN-MPosNED.x)^2 + (GndPosE-MPosNED.y)^2); */
      psiLOSb = Guidance_100Hz_Function_1D_M->dwork.GndPosN - MPosNED.x;
      psiLOSi = Guidance_100Hz_Function_1D_M->dwork.GndPosE - MPosNED.y;
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge =
        Guidance_100Hz_Function_1D_M->dwork.GndRge2Tgt - sqrt((psiLOSb * psiLOSb)
        + (psiLOSi * psiLOSi));
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1215' else */
      /* 'GUID_Embedded_Guid_Control_1D:1216' guidcmds.GndRge = sqrt(tgt0.x^2 + tgt0.y^2); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge = sqrt((gbzc * gbzc) +
        (gbyc * gbyc));
    }

    /* 'GUID_Embedded_Guid_Control_1D:1220' psiLOSi = atan2(tgt0.y,tgt0.x); */
    psiLOSi = rt_atan2d_snf(gbyc, gbzc);

    /* 'GUID_Embedded_Guid_Control_1D:1221' psiFPi  = atan2(MVelNED.y,MVelNED.x); */
    /* 'GUID_Embedded_Guid_Control_1D:1222' psiLOSb = psiLOSi - psiFPi; */
    psiLOSb = psiLOSi - rt_atan2d_snf(MVelNED.y, MVelNED.x);

    /* 'GUID_Embedded_Guid_Control_1D:1223' if (psiLOSb == 0.0) */
    if (psiLOSb == 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:1224' psiLOSb = 0.001; */
      psiLOSb = 0.001;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1228' eg0t.phi = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:1229' eg0t.the = double(MP_in.DiveAngle); */
    /* 'GUID_Embedded_Guid_Control_1D:1230' if (MP_in.AttackHeadingValid && ~opportunity_mode) */
    if ((((int32_T)rtU_MPin->AttackHeadingValid) != 0) && (((int32_T)
          Guidance_100Hz_Function_1D_M->dwork.opportunity_mode) == 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:1231' attackdirection = double(MP_in.AttackHeading) - pi; */
      psiLOSi = ((real_T)rtU_MPin->AttackHeading) - 3.1415926535897931;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1232' else */
      /* 'GUID_Embedded_Guid_Control_1D:1233' attackdirection = psiLOSi - pi; */
      psiLOSi -= 3.1415926535897931;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1235' if (attackdirection < 0.0) */
    if (psiLOSi < 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:1236' attackdirection = attackdirection + 2.0*pi; */
      psiLOSi += 6.2831853071795862;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1238' eg0t.psi = double(attackdirection); */
    /* 'GUID_Embedded_Guid_Control_1D:1239' DCMterm = LIB_e2d(eg0t); */
    /* 'GUID_Embedded_Guid_Control_1D:1243' term_vec = DCMterm*[guid_settings.Rterm 0 0]'; */
    LIB_e2d(0.0, (real_T)rtU_MPin->DiveAngle, psiLOSi, tmp_0);
    for (guidtel20HzB_tmp = 0; guidtel20HzB_tmp < 3; guidtel20HzB_tmp++) {
      term_vec[guidtel20HzB_tmp] = (tmp_0[guidtel20HzB_tmp + 6] * 0.0) +
        ((tmp_0[guidtel20HzB_tmp + 3] * 0.0) + (tmp_0[guidtel20HzB_tmp] * 2000.0));
    }

    /* 'GUID_Embedded_Guid_Control_1D:1244' TRACK.Ut0 = LIB_unitvector(term_vec); */
    LIB_unitvector_gdXEDyOJ(term_vec, (Guidance_100Hz_Function_1D_M)
      ->dwork.TRACK.Ut0);

    /* 'GUID_Embedded_Guid_Control_1D:1247' psiFP0 = atan2(tgt0.y+term_vec(2),tgt0.x+term_vec(1)); */
    /* 'GUID_Embedded_Guid_Control_1D:1248' if (guidmode < 4) */
    if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) < 4) {
      /* 'GUID_Embedded_Guid_Control_1D:1249' TRACK.FPazi = psiFP0; */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.FPazi = rt_atan2d_snf(gbyc +
        term_vec[1], gbzc + term_vec[0]);
    }

    /* 'GUID_Embedded_Guid_Control_1D:1255' [vec,Rarc,Oarc] = LIB_getreflvec([tgt0.x tgt0.y tgt0.z],guid_settings.Rterm*Uv,[0 0 0]); */
    /* 'GUID_Embedded_Guid_Control_1D:1258' if (TTKF_in.newmeas) */
    if (((int32_T)rtU_ttkfin->newmeas) != 0) {
      /* 'GUID_Embedded_Guid_Control_1D:1259' newmeas_cnt = newmeas_cnt + uint32(1); */
      qY = Guidance_100Hz_Function_1D_M->dwork.newmeas_cnt + /*MW:OvSatOk*/ 1U;
      if (qY < Guidance_100Hz_Function_1D_M->dwork.newmeas_cnt) {
        qY = MAX_uint32_T;
      }

      Guidance_100Hz_Function_1D_M->dwork.newmeas_cnt = qY;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1262' [guidmode,guidcmds] = SetGuidanceMode(guidmode,TRACK,guidcmds,fromDATA.AirframeMode,opportunity_mode,MP_in,TTKF_in,NAV_in,IMU_in,WeaCfg_in,Rarc,psiLOSb,ENV.mach,ftime,newmeas_cnt,guid_settings); */
    SetGuidanceMode(&(Guidance_100Hz_Function_1D_M)->dwork.guidmode,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.reset3cnt,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.mpos0.x,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.mpos0.y,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.mpos0.z,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Rwe.x,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Rwe.y,
                    (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Rwe.z,
                    &(Guidance_100Hz_Function_1D_M)->dwork.guidcmds,
                    rtU_DATAguid->AirframeMode, (Guidance_100Hz_Function_1D_M)
                    ->dwork.opportunity_mode, rtU_MPin->DiveAngle,
                    rtU_ttkfin->confidence, rtU_ttkfin->tgtrge,
                    (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.pos_e,
                    (Guidance_100Hz_Function_1D_M)->dwork.NAV_in.att_bn,
                    (Guidance_100Hz_Function_1D_M)->dwork.IMU_in.wbxm,
                    rtU_WeaCfgin->WingsPresent, (Guidance_100Hz_Function_1D_M)
                    ->dwork.ENV.mach, ftime);

    /* 'GUID_Embedded_Guid_Control_1D:1266' [OPT_FP,t_debug] = GetOptimalFPath(OPT_FP,guidmode,ENV,guidcmds.gbyc_unl,guidcmds.gbyc_unl,IMU_in.gbym,IMU_in.gbzm,fromDATA,TTKF_in,ftime,debug,t_debug,cnsts); */
    GetOptimalFPath(&(Guidance_100Hz_Function_1D_M)->dwork.OPT_FP,
                    (Guidance_100Hz_Function_1D_M)->dwork.guidmode,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.rho,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.Vsound,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.FPthem,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.mach,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.alpha,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.Vairspeed,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.Q,
                    (Guidance_100Hz_Function_1D_M)->dwork.ENV.atmbeta,
                    rtU_DATAguid, rtU_ttkfin->tgtrge, 57.295779513082323);

    /* 'GUID_Embedded_Guid_Control_1D:1273' if (~Discretes_in.WingsOpenRcvd && ... */
    /* 'GUID_Embedded_Guid_Control_1D:1274'       (guidmode == 5 && IMU_in.gbzm > -2.0 && WeaCfg_in.WingsPresent)) */
    if ((((((int32_T)rtU_Discretesin->WingsOpenRcvd) == 0) && (((int32_T)
            Guidance_100Hz_Function_1D_M->dwork.guidmode) == 5)) &&
         (Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbzm > -2.0)) && (((int32_T)
          rtU_WeaCfgin->WingsPresent) != 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:1275' guidFLAGS.WingsOpen = uint8(1); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsOpen = 1U;

      /* 'GUID_Embedded_Guid_Control_1D:1276' wingsopen_timeout = ftime + 0.500; */
      Guidance_100Hz_Function_1D_M->dwork.wingsopen_timeout = ftime + 0.5;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1281' if (guidcmds.GndRge*sin(double(MP_in.DiveAngle)) > double(fromDATA.AirframeProp.Rarc)) */
    dNavPos = sin((real_T)rtU_MPin->DiveAngle);
    H_release = Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge * dNavPos;
    if (H_release > ((real_T)rtU_DATAguid->AirframeProp.Rarc)) {
      /* 'GUID_Embedded_Guid_Control_1D:1282' GndTmp = guidcmds.GndRge-double(fromDATA.AirframeProp.Rarc)/sin(double(MP_in.DiveAngle)); */
      /* 'GUID_Embedded_Guid_Control_1D:1283' H_release = GndTmp*LIB_limit(0.0,10.0,tan(double(MP_in.DiveAngle)-0.001)) + double(fromDATA.AirframeProp.Rarc); */
      H_release = tan(((real_T)rtU_MPin->DiveAngle) - 0.001);

      /* 'LIB_limit:27' if (in > max) */
      if (H_release > 10.0) {
        /* 'LIB_limit:28' out = max; */
        H_release = 10.0;
      } else if (H_release < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        H_release = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      H_release = ((Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge -
                    (((real_T)rtU_DATAguid->AirframeProp.Rarc) / dNavPos)) *
                   H_release) + ((real_T)rtU_DATAguid->AirframeProp.Rarc);
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1284' else */
      /* 'GUID_Embedded_Guid_Control_1D:1285' H_release = guidcmds.GndRge*sin(double(MP_in.DiveAngle)); */
    }

    /* 'GUID_Embedded_Guid_Control_1D:1288' if ((NAV_in.fib_e.z < 0.0 && ... */
    /* 'GUID_Embedded_Guid_Control_1D:1289'        (guidmode == 9) || ... */
    /* 'GUID_Embedded_Guid_Control_1D:1290'        (guidmode >= 4 && guidcmds.GndRge < 0.8*double(fromDATA.AirframeProp.Rarc)*sin(double(MP_in.DiveAngle))) || ... */
    /* 'GUID_Embedded_Guid_Control_1D:1291'        (guidmode >= 6 && tgt0.z > H_release)) || ... */
    /* 'GUID_Embedded_Guid_Control_1D:1292' 	  (TTKF_in.tgtrge < 2000.0)) */
    if ((((Guidance_100Hz_Function_1D_M->dwork.NAV_in.fib_e.z < 0.0) &&
          (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 9)) ||
         ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 4) &&
          (Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge < ((0.8 *
             ((real_T)rtU_DATAguid->AirframeProp.Rarc)) * dNavPos)))) ||
        (((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 6) &&
          (phic > H_release)) || (rtU_ttkfin->tgtrge < 2000.0))) {
      /* 'GUID_Embedded_Guid_Control_1D:1294' if (guidmode < 5 && ~opportunity_mode) */
      if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) < 5) &&
          (((int32_T)Guidance_100Hz_Function_1D_M->dwork.opportunity_mode) == 0))
      {
        /* 'GUID_Embedded_Guid_Control_1D:1295' guidcmds.forcegmode5 = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidcmds.forcegmode5 = 1U;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1297' if (ftime > wingsopen_timeout && Discretes_in.WingsOpenRcvd) */
      if ((ftime > Guidance_100Hz_Function_1D_M->dwork.wingsopen_timeout) &&
          (((int32_T)rtU_Discretesin->WingsOpenRcvd) != 0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1298' guidFLAGS.WingsRelease = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsRelease = 1U;
      } else {
        if ((((((int32_T)rtU_Discretesin->WingsOpenRcvd) == 0) && (((int32_T)
                Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 5)) &&
             (Guidance_100Hz_Function_1D_M->dwork.IMU_in.gbzm > -2.0)) &&
            (((int32_T)rtU_WeaCfgin->WingsPresent) != 0)) {
          /* 'GUID_Embedded_Guid_Control_1D:1299' elseif (~Discretes_in.WingsOpenRcvd && guidmode >= 5 && IMU_in.gbzm > -2.0 && WeaCfg_in.WingsPresent) */
          /* 'GUID_Embedded_Guid_Control_1D:1300' guidFLAGS.WingsOpen = uint8(1); */
          Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsOpen = 1U;

          /* 'GUID_Embedded_Guid_Control_1D:1301' wingsopen_timeout = ftime + 0.500; */
          Guidance_100Hz_Function_1D_M->dwork.wingsopen_timeout = ftime + 0.5;
        }
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:1310' if (ENV.FPthem < -0.7854 && TTKF_in.confidence < 90 && guidmode >= 8) */
    if (((Guidance_100Hz_Function_1D_M->dwork.ENV.FPthem < -0.7854) &&
         (((int32_T)rtU_ttkfin->confidence) < 90)) && (((int32_T)
          Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 8)) {
      /* 'GUID_Embedded_Guid_Control_1D:1311' guidFLAGS.SwitchforBunt = uint8(1); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SwitchforBunt = 1U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1313' if (ValidGNSSNav) */
    if (ValidGNSSNav != 0) {
      /* 'GUID_Embedded_Guid_Control_1D:1314' t_noGNSS = 0.0; */
      Guidance_100Hz_Function_1D_M->dwork.t_noGNSS = 0.0;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1315' else */
      /* 'GUID_Embedded_Guid_Control_1D:1316' t_noGNSS = t_noGNSS + delt; */
      Guidance_100Hz_Function_1D_M->dwork.t_noGNSS +=
        Guidance_100Hz_Function_1D_M->dwork.delt;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1318' if (TTKF_in.tgtrge > 5000.0 || TTKF_in.tgtrge <= 0.0 || TTKF_in.confidence < 20 || guidmode < 3) */
    if ((((rtU_ttkfin->tgtrge > 5000.0) || (rtU_ttkfin->tgtrge <= 0.0)) ||
         (((int32_T)rtU_ttkfin->confidence) < 20)) || (((int32_T)
          Guidance_100Hz_Function_1D_M->dwork.guidmode) < 3)) {
      /* 'GUID_Embedded_Guid_Control_1D:1319' t_threshold = 0.0; */
      /* 'GUID_Embedded_Guid_Control_1D:1324' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,1,0); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag = (uint8_T)
        (((uint16_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag) &
         ((uint16_T)254));

      /* 'GUID_Embedded_Guid_Control_1D:1325' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,2,0); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag = (uint8_T)
        (((uint16_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag) &
         ((uint16_T)253));
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1326' else */
      /* 'GUID_Embedded_Guid_Control_1D:1330' if (TTKF_in.tgtrge > 0.0) */
      if (rtU_ttkfin->tgtrge > 0.0) {
        /* 'GUID_Embedded_Guid_Control_1D:1331' t_threshold = LIB_limit(0.0,10.0,11.0 - (5000.0/TTKF_in.tgtrge)^2); */
        H_release = 5000.0 / rtU_ttkfin->tgtrge;
        H_release *= H_release;

        /* 'LIB_limit:27' if (in > max) */
        if ((11.0 - H_release) > 10.0) {
          /* 'LIB_limit:28' out = max; */
          H_release = 10.0;
        } else if ((11.0 - H_release) < 0.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          H_release = 0.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          H_release = 11.0 - H_release;
        }
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1332' else */
        /* 'GUID_Embedded_Guid_Control_1D:1333' t_threshold = 10.0; */
        H_release = 10.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1335' if (t_noGNSS > t_threshold && bitget(guidFLAGS.NavCtrlFlag,2) == 0) */
      if ((Guidance_100Hz_Function_1D_M->dwork.t_noGNSS > H_release) &&
          ((((uint32_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag)
            & 2U) == 0U)) {
        /* 'GUID_Embedded_Guid_Control_1D:1338' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,1,1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)1;

        /* 'GUID_Embedded_Guid_Control_1D:1339' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,2,1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)2;
      } else {
        if ((((int32_T)rtU_ttkfin->confidence) > 90) && ((((uint32_T)
               Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag) & 1U) ==
             0U)) {
          /* 'GUID_Embedded_Guid_Control_1D:1340' elseif (TTKF_in.confidence > 90 && bitget(guidFLAGS.NavCtrlFlag,1) == 0) */
          /* 'GUID_Embedded_Guid_Control_1D:1343' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,1,1); */
          Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)
            1;
        }
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:1348' guidcmds.giyc_int = LIB_limit(0.0,20.0,0.998*guidcmds.giyc_int + LIB_limit(0.0,20.0,abs(guidcmds.giyc)-2.0)*delt); */
    H_release = fabs(Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc);

    /* 'LIB_limit:27' if (in > max) */
    if ((H_release - 2.0) > 20.0) {
      /* 'LIB_limit:28' out = max; */
      H_release = 20.0;
    } else if ((H_release - 2.0) < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      H_release = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      H_release -= 2.0;
    }

    H_release = (0.998 * Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int)
      + (H_release * Guidance_100Hz_Function_1D_M->dwork.delt);

    /* 'LIB_limit:27' if (in > max) */
    if (H_release > 20.0) {
      /* 'LIB_limit:28' out = max; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int = 20.0;
    } else if (H_release < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int = H_release;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1353' if ((~Discretes_in.umbilical && guidmode <= 3) || guidFLAGS.SwitchforBunt) */
    if (((((int32_T)rtU_Discretesin->umbilical) == 0) && (((int32_T)
           Guidance_100Hz_Function_1D_M->dwork.guidmode) <= 3)) || (((int32_T)
          Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SwitchforBunt) != 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:1354' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,3,1); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)4;
    } else {
      qY = ((uint32_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag)
        & 4U;
      if (((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) > 3) && (qY
            != 0U)) && (((int32_T)
                         Guidance_100Hz_Function_1D_M->dwork.guidcmds.navctrlreset)
                        == 0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1355' elseif (guidmode > 3 && bitget(guidFLAGS.NavCtrlFlag,3) == 1 && ~guidcmds.navctrlreset) */
        /* 'GUID_Embedded_Guid_Control_1D:1356' guidcmds.navctrlreset = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidcmds.navctrlreset = 1U;

        /* 'GUID_Embedded_Guid_Control_1D:1357' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,3,0); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag = (uint8_T)
          (((uint16_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag)
           & ((uint16_T)251));
      } else if (((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) > 3) &&
                  (Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge < 6000.0))
                 && (qY == 0U)) {
        /* 'GUID_Embedded_Guid_Control_1D:1358' elseif (guidmode > 3 && guidcmds.GndRge < 6000.0 && bitget(guidFLAGS.NavCtrlFlag,3) == 0) */
        /* 'GUID_Embedded_Guid_Control_1D:1360' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,3,1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)4;
      } else if ((fabs(Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int) <
                  3.0) && (Guidance_100Hz_Function_1D_M->dwork.guidcmds.GndRge >
                           6000.0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1361' elseif (abs(guidcmds.giyc_int) < 3.0 && guidcmds.GndRge > 6000.0) */
        /* 'GUID_Embedded_Guid_Control_1D:1362' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,3,0); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag = (uint8_T)
          (((uint16_T)Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag)
           & ((uint16_T)251));
      } else {
        if (fabs(Guidance_100Hz_Function_1D_M->dwork.guidcmds.giyc_int) > 6.0) {
          /* 'GUID_Embedded_Guid_Control_1D:1363' elseif (abs(guidcmds.giyc_int) > 6.0) */
          /* 'GUID_Embedded_Guid_Control_1D:1364' guidFLAGS.NavCtrlFlag = bitset(guidFLAGS.NavCtrlFlag,3,1); */
          Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag |= (uint8_T)
            4;
        }
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:1376' if (MP_in.PriTgtLatitude == 0.0) */
    if (rtU_MPin->PriTgtLatitude == 0.0) {
      /* 'GUID_Embedded_Guid_Control_1D:1377' if (ftime > 1.5) */
      if (ftime > 1.5) {
        /* 'GUID_Embedded_Guid_Control_1D:1379' guidFLAGS.SADFuze = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SADFuze = 1U;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1381' else */
      /* 'GUID_Embedded_Guid_Control_1D:1383' if (TTKF_in.tgtrge <= 10000.0 && ftime > 14.0) */
      if ((rtU_ttkfin->tgtrge <= 10000.0) && (ftime > 14.0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1384' guidFLAGS.SADFuze = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SADFuze = 1U;
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:1387' guidFLAGS.Umbilical = uint8(Discretes_in.umbilical); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.Umbilical =
      rtU_Discretesin->umbilical;

    /* 'GUID_Embedded_Guid_Control_1D:1388' guidFLAGS.BatteryOn = uint8(Discretes_in.BatteryOn); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.BatteryOn =
      rtU_Discretesin->BatteryOn;

    /* 'GUID_Embedded_Guid_Control_1D:1394' if (abs(ENV.alpha) < 0.0873 && abs(ENV.beta) < 0.0873) */
    dNavPos = fabs(Guidance_100Hz_Function_1D_M->dwork.ENV.alpha);
    if ((dNavPos < 0.0873) && (fabs(Guidance_100Hz_Function_1D_M->dwork.ENV.beta)
         < 0.0873)) {
      /* 'GUID_Embedded_Guid_Control_1D:1395' guidFLAGS.PstaticValid = uint8(10); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 10U;
    } else if (dNavPos < 0.1745) {
      if (fabs(Guidance_100Hz_Function_1D_M->dwork.ENV.beta) < 0.1745) {
        /* 'GUID_Embedded_Guid_Control_1D:1396' elseif (abs(ENV.alpha) < 0.1745 && abs(ENV.beta) < 0.1745) */
        /* 'GUID_Embedded_Guid_Control_1D:1397' guidFLAGS.PstaticValid = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 1U;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1398' else */
        /* 'GUID_Embedded_Guid_Control_1D:1399' guidFLAGS.PstaticValid = uint8(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 0U;
      }
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1398' else */
      /* 'GUID_Embedded_Guid_Control_1D:1399' guidFLAGS.PstaticValid = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 0U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1401' guidFLAGS.PstaticValid = uint8(1); */
    Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.PstaticValid = 1U;

    /* 'GUID_Embedded_Guid_Control_1D:1404' angdot = 0.999999; */
    /* 'GUID_Embedded_Guid_Control_1D:1407' keep_arc_8 = uint8(0); */
    keep_arc_8 = 0U;

    /* 'GUID_Embedded_Guid_Control_1D:1408' dTTKFPos = sqrt((TPosNED_uncond.x-TPosNED_uncond_prev.x)^2 + ... */
    /* 'GUID_Embedded_Guid_Control_1D:1409'                   (TPosNED_uncond.y-TPosNED_uncond_prev.y)^2 + ... */
    /* 'GUID_Embedded_Guid_Control_1D:1410'                   (TPosNED_uncond.z-TPosNED_uncond_prev.z)^2); */
    H_release = TPosNED_uncond.x -
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.x;
    dNavPos = TPosNED_uncond.y -
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.y;
    dNavVel = TPosNED_uncond.z -
      Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.z;
    H_release = sqrt(((H_release * H_release) + (dNavPos * dNavPos)) + (dNavVel *
      dNavVel));

    /* 'GUID_Embedded_Guid_Control_1D:1411' dNavPos  = sqrt((MPosNED.x-MPosNED_prev.x)^2 + (MPosNED.y-MPosNED_prev.y)^2 + (MPosNED.z-MPosNED_prev.z)^2); */
    dNavPos = MPosNED.x - Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.x;
    dNavVel = MPosNED.y - Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.y;
    ang_threshold = MPosNED.z -
      Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.z;
    dNavPos = sqrt(((dNavPos * dNavPos) + (dNavVel * dNavVel)) + (ang_threshold *
      ang_threshold));

    /* 'GUID_Embedded_Guid_Control_1D:1412' dNavVel  = sqrt((MVelNED.x-MVelNED_prev.x)^2 + (MVelNED.y-MVelNED_prev.y)^2 + (MVelNED.z-MVelNED_prev.z)^2); */
    dNavVel = MVelNED.x - Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.x;
    ang_threshold = MVelNED.y -
      Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.y;
    n_a = MVelNED.z - Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.z;
    dNavVel = sqrt(((dNavVel * dNavVel) + (ang_threshold * ang_threshold)) +
                   (n_a * n_a));

    /* 'GUID_Embedded_Guid_Control_1D:1413' if (guidmode >= 4 && guidmode < 9) */
    if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 4) &&
        (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) < 9)) {
      /* 'GUID_Embedded_Guid_Control_1D:1415' if (TRACK.Lfpath2bunt > 0.0) */
      if (Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2bunt > 0.0) {
        /* 'GUID_Embedded_Guid_Control_1D:1416' TRACK.Mtgt0 = -(tgt0.z+guid_settings.Rterm*TRACK.Ut0(3))/TRACK.Lfpath2bunt; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Mtgt0 = (-((2000.0 *
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[2]) + phic)) /
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Lfpath2bunt;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1417' else */
        /* 'GUID_Embedded_Guid_Control_1D:1418' TRACK.Mtgt0 = -0.001; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Mtgt0 = -0.001;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1421' TRACK.reset = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 0U;

      /* 'GUID_Embedded_Guid_Control_1D:1422' if (guidcmds.gmode_sw && (guidmode == 4 || guidmode == 5 || guidmode == 6 || guidmode == 7)) */
      if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw) != 0)
          && ((((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 4) ||
                (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 5)) ||
               (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 6)) ||
              (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 7))) {
        /* 'GUID_Embedded_Guid_Control_1D:1423' TRACK.reset = uint8(1); */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 1U;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1426' if (ftime > TRACK.mintime || dNavPos > 200.0 || dNavVel > 20.0) */
      if (((ftime > Guidance_100Hz_Function_1D_M->dwork.TRACK.mintime) ||
           (dNavPos > 200.0)) || (dNavVel > 20.0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1429' if (dTTKFPos > 100.0 || dNavPos > 200.0 || dNavVel > 20.0) */
        if (((H_release > 100.0) || (dNavPos > 200.0)) || (dNavVel > 20.0)) {
          /* 'GUID_Embedded_Guid_Control_1D:1431' TRACK.reset = uint8(3); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 3U;

          /* 'GUID_Embedded_Guid_Control_1D:1432' TRACK.reset3cnt = TRACK.reset3cnt + 1; */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.reset3cnt++;

          /* 'GUID_Embedded_Guid_Control_1D:1433' TPosNED_uncond_prev.x = TPosNED_uncond.x; */
          Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.x =
            TPosNED_uncond.x;

          /* 'GUID_Embedded_Guid_Control_1D:1434' TPosNED_uncond_prev.y = TPosNED_uncond.y; */
          Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.y =
            TPosNED_uncond.y;

          /* 'GUID_Embedded_Guid_Control_1D:1435' TPosNED_uncond_prev.z = TPosNED_uncond.z; */
          Guidance_100Hz_Function_1D_M->dwork.TPosNED_uncond_prev.z =
            TPosNED_uncond.z;
        } else if (ftime > Guidance_100Hz_Function_1D_M->dwork.TRACK.ftime_step)
        {
          /* 'GUID_Embedded_Guid_Control_1D:1436' elseif (ftime > TRACK.ftime_step) */
          /* 'GUID_Embedded_Guid_Control_1D:1439' TRACK.reset = uint8(1); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 1U;
        } else {
          if (((Guidance_100Hz_Function_1D_M->dwork.TRACK.Roff_intg *
                Guidance_100Hz_Function_1D_M->dwork.clos.Kglide) *
               Guidance_100Hz_Function_1D_M->dwork.clos.Kglide) > 500.0) {
            /* 'GUID_Embedded_Guid_Control_1D:1440' elseif (TRACK.Roff_intg*clos.Kglide*clos.Kglide > 500) */
            /* 'GUID_Embedded_Guid_Control_1D:1441' TRACK.reset = uint8(2); */
            Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 2U;
          }
        }
      }

      /* 'GUID_Embedded_Guid_Control_1D:1457' TRACK.Roff_intg = TRACK.Roff_intg*0.999 + LIB_limit(0.0,1000.0,clos.Roffset-10.0)*delt; */
      /* 'LIB_limit:27' if (in > max) */
      if ((Guidance_100Hz_Function_1D_M->dwork.clos.Roffset - 10.0) > 1000.0) {
        /* 'LIB_limit:28' out = max; */
        dNavPos = 1000.0;
      } else if ((Guidance_100Hz_Function_1D_M->dwork.clos.Roffset - 10.0) < 0.0)
      {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        dNavPos = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        dNavPos = Guidance_100Hz_Function_1D_M->dwork.clos.Roffset - 10.0;
      }

      Guidance_100Hz_Function_1D_M->dwork.TRACK.Roff_intg =
        (Guidance_100Hz_Function_1D_M->dwork.TRACK.Roff_intg * 0.999) + (dNavPos
        * Guidance_100Hz_Function_1D_M->dwork.delt);

      /* 'GUID_Embedded_Guid_Control_1D:1460' if (guidmode == 8 && guidcmds.gmode_sw) */
      /* 'GUID_Embedded_Guid_Control_1D:1464' if (MP_in.MissionType == def.IIR && WeaCfg_in.WingsPresent) */
      if ((((int32_T)rtU_MPin->MissionType) == 2) && (((int32_T)
            rtU_WeaCfgin->WingsPresent) != 0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1465' TRACK.Rlim_lo = 1000.0; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 1000.0;
      } else if ((((int32_T)rtU_MPin->MissionType) == 2) && (((int32_T)
                   rtU_WeaCfgin->WingsPresent) == 0)) {
        /* 'GUID_Embedded_Guid_Control_1D:1466' elseif (MP_in.MissionType == def.IIR && ~WeaCfg_in.WingsPresent) */
        /* 'GUID_Embedded_Guid_Control_1D:1467' TRACK.Rlim_lo = 500.0; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 500.0;
      } else if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.reset) ==
                  3) && (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode)
                         == 8)) {
        /* 'GUID_Embedded_Guid_Control_1D:1468' elseif (TRACK.reset == 3 && guidmode == 8) */
        /* 'GUID_Embedded_Guid_Control_1D:1469' TRACK.Rlim_lo = 10.0; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 10.0;
      } else if (((real_T)rtU_MPin->DiveAngle) > 1.308) {
        /* 'GUID_Embedded_Guid_Control_1D:1470' elseif (MP_in.DiveAngle > 1.308) */
        /* 'GUID_Embedded_Guid_Control_1D:1471' TRACK.Rlim_lo = 500.0; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 500.0;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1472' else */
        /* 'GUID_Embedded_Guid_Control_1D:1473' TRACK.Rlim_lo = 100.0; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Rlim_lo = 100.0;
      }

      /* 'GUID_Embedded_Guid_Control_1D:1476' if (TRACK.reset) */
      if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.reset) != 0) {
        /* 'GUID_Embedded_Guid_Control_1D:1477' TRACKnow = TRACK; */
        TRACKnow = Guidance_100Hz_Function_1D_M->dwork.TRACK;

        /* 'GUID_Embedded_Guid_Control_1D:1478' TRACKnow.Ut = TRACK.Ut0; */
        TRACKnow.Ut[0] = Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[0];
        TRACKnow.Ut[1] = Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[1];
        TRACKnow.Ut[2] = Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[2];

        /* 'GUID_Embedded_Guid_Control_1D:1479' TRACKnow.Kglide = clos.Kglide; */
        TRACKnow.Kglide = Guidance_100Hz_Function_1D_M->dwork.clos.Kglide;

        /* 'GUID_Embedded_Guid_Control_1D:1480' if (guid_settings.do_plot) */
        /* 'GUID_Embedded_Guid_Control_1D:1484' [TRACKnow, guidtel_fpath, telem] = ... */
        /* 'GUID_Embedded_Guid_Control_1D:1485'         DefineFlightPath(TRACKnow, OPT_FP, MP_in, TPosNED_uncond, MPosNED, MVelNED, MRefNED, DCM, ENV, guidmode, opportunity_mode, fromDATA.AirframeMode, guid_settings, double(fromDATA.AirframeProp.Rarc), cnsts, ftime, guidtel_fpath, telem); */
        DefineFlightPath(&TRACKnow, (Guidance_100Hz_Function_1D_M)
                         ->dwork.OPT_FP.Mopt, (Guidance_100Hz_Function_1D_M)
                         ->dwork.OPT_FP.Mglide_opt, rtU_MPin->WeaponType,
                         TPosNED_uncond, MPosNED, MVelNED, MRefNED, &DCM,
                         (Guidance_100Hz_Function_1D_M)->dwork.ENV.mach,
                         (Guidance_100Hz_Function_1D_M)->dwork.opportunity_mode,
                         rtU_DATAguid->AirframeMode,
                         (Guidance_100Hz_Function_1D_M)
                         ->dwork.guid_settings.psiR_Thr,
                         (Guidance_100Hz_Function_1D_M)
                         ->dwork.guid_settings.psi_Engage, (real_T)
                         rtU_DATAguid->AirframeProp.Rarc, ftime,
                         &(Guidance_100Hz_Function_1D_M)->dwork.guidtel_fpath,
                         &(Guidance_100Hz_Function_1D_M)->dwork.telem);

        /* 'GUID_Embedded_Guid_Control_1D:1486' temp = dot(TRACK.Ut0,TRACK.Ut8); */
        dNavPos = dot_lFwBkxue((Guidance_100Hz_Function_1D_M)->dwork.TRACK.Ut0,
          (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Ut8);

        /* 'GUID_Embedded_Guid_Control_1D:1487' if (temp > 0.999999) */
        if (dNavPos > 0.999999) {
          /* 'GUID_Embedded_Guid_Control_1D:1488' temp = 0.999999; */
          dNavPos = 0.999999;
        } else {
          if (dNavPos < -0.999999) {
            /* 'GUID_Embedded_Guid_Control_1D:1489' elseif (temp < -0.999999) */
            /* 'GUID_Embedded_Guid_Control_1D:1490' temp = -0.999999; */
            dNavPos = -0.999999;
          }
        }

        /* 'GUID_Embedded_Guid_Control_1D:1492' ang8 = acos(temp); */
        dNavVel = acos(dNavPos);

        /* 'GUID_Embedded_Guid_Control_1D:1493' temp = dot(TRACK.Ut0,TRACKnow.Ut); */
        dNavPos = dot_lFwBkxue((Guidance_100Hz_Function_1D_M)->dwork.TRACK.Ut0,
          TRACKnow.Ut);

        /* 'GUID_Embedded_Guid_Control_1D:1494' if (temp > 0.999999) */
        if (dNavPos > 0.999999) {
          /* 'GUID_Embedded_Guid_Control_1D:1495' temp = 0.999999; */
          dNavPos = 0.999999;
        } else {
          if (dNavPos < -0.999999) {
            /* 'GUID_Embedded_Guid_Control_1D:1496' elseif (temp < -0.999999) */
            /* 'GUID_Embedded_Guid_Control_1D:1497' temp = -0.999999; */
            dNavPos = -0.999999;
          }
        }

        /* 'GUID_Embedded_Guid_Control_1D:1500' angnow = acos(temp); */
        /* 'GUID_Embedded_Guid_Control_1D:1501' ang_threshold = 2.5*cnsts.d2r; */
        /* 'GUID_Embedded_Guid_Control_1D:1502' if (ang8 < ang_threshold || ang8-angnow < ang_threshold) */
        if (dNavVel < 0.043633231299858237) {
          /* 'GUID_Embedded_Guid_Control_1D:1503' keep_arc_8 = uint8(1); */
          keep_arc_8 = 1U;
        } else {
          if ((dNavVel - acos(dNavPos)) < 0.043633231299858237) {
            /* 'GUID_Embedded_Guid_Control_1D:1503' keep_arc_8 = uint8(1); */
            keep_arc_8 = 1U;
          }
        }

        /* 'GUID_Embedded_Guid_Control_1D:1505' guidtel_20HzB.GUI_ang8   = single(ang8); */
        /* 'GUID_Embedded_Guid_Control_1D:1506' guidtel_20HzB.GUI_angnow = single(angnow); */
        /* 'GUID_Embedded_Guid_Control_1D:1507' if (guidmode == 8 && clos.Kglide > 0.99 && keep_arc_8 == 1 && dTTKFPos < 100.0 && TRACK.reset < 2) */
        if (((((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 8) &&
               (Guidance_100Hz_Function_1D_M->dwork.clos.Kglide > 0.99)) &&
              (((int32_T)keep_arc_8) == 1)) && (H_release < 100.0)) &&
            (((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.reset) < 2)) {
          /* 'GUID_Embedded_Guid_Control_1D:1508' TRACK.bunt_arc = uint8(1); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.bunt_arc = 1U;

          /* 'GUID_Embedded_Guid_Control_1D:1509' TRACK.ftime_step = 1000.0; */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.ftime_step = 1000.0;

          /* 'GUID_Embedded_Guid_Control_1D:1510' TRACK.reset = uint8(0); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.reset = 0U;

          /* 'GUID_Embedded_Guid_Control_1D:1511' if (guid_settings.do_plot) */
        } else {
          /* 'GUID_Embedded_Guid_Control_1D:1514' else */
          /* 'GUID_Embedded_Guid_Control_1D:1515' TRACK.bunt_arc = uint8(0); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.bunt_arc = 0U;

          /* 'GUID_Embedded_Guid_Control_1D:1516' TRACKnow.mintime = ftime + 1.0; */
          TRACKnow.mintime = ftime + 1.0;

          /* 'GUID_Embedded_Guid_Control_1D:1518' TRACKnow.ftime_step = ftime + LIB_limit(2.0,1000.0,2.0 + 10.0*TRACKnow.ratio*TRACKnow.Kglide); */
          H_release = ((10.0 * TRACKnow.ratio) * TRACKnow.Kglide) + 2.0;

          /* 'LIB_limit:27' if (in > max) */
          if (H_release > 1000.0) {
            /* 'LIB_limit:28' out = max; */
            H_release = 1000.0;
          } else if (H_release < 2.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            H_release = 2.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          TRACKnow.ftime_step = ftime + H_release;

          /* 'GUID_Embedded_Guid_Control_1D:1519' TRACKnow.FPbunt_cnt = TRACKnow.FPbunt_cnt + uint8(1); */
          guidtel20HzB_tmp = (int32_T)((uint32_T)(((uint32_T)TRACKnow.FPbunt_cnt)
            + 1U));
          if (((uint32_T)guidtel20HzB_tmp) > 255U) {
            guidtel20HzB_tmp = 255;
          }

          TRACKnow.FPbunt_cnt = (uint8_T)guidtel20HzB_tmp;

          /* 'GUID_Embedded_Guid_Control_1D:1520' if (TRACKnow.FPbunt_cnt > 254) */
          if (((int32_T)((uint8_T)guidtel20HzB_tmp)) > 254) {
            /* 'GUID_Embedded_Guid_Control_1D:1521' TRACKnow.FPbunt_cnt = uint8(1); */
            TRACKnow.FPbunt_cnt = 1U;
          }

          /* 'GUID_Embedded_Guid_Control_1D:1523' TRACK = TRACKnow; */
          Guidance_100Hz_Function_1D_M->dwork.TRACK = TRACKnow;
        }
      } else {
        if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 5) &&
            (((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.FPbunt_cnt) ==
             0)) {
          /* 'GUID_Embedded_Guid_Control_1D:1527' elseif (guidmode == 5 && TRACK.FPbunt_cnt == 0) */
          /* 'GUID_Embedded_Guid_Control_1D:1528' if (guid_settings.do_plot) */
          /* 'GUID_Embedded_Guid_Control_1D:1532' TRACK.Ut = TRACK.Ut0; */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[0] =
            Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[0];
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[1] =
            Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[1];
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[2] =
            Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut0[2];

          /* 'GUID_Embedded_Guid_Control_1D:1533' [TRACK, guidtel_fpath, telem] = ... */
          /* 'GUID_Embedded_Guid_Control_1D:1534'         DefineFlightPath(TRACK, OPT_FP, MP_in, TPosNED_uncond, MPosNED, MVelNED, MRefNED, DCM, ENV, guidmode, opportunity_mode, fromDATA.AirframeMode, guid_settings, double(fromDATA.AirframeProp.Rarc), cnsts, ftime, guidtel_fpath, telem); */
          DefineFlightPath(&(Guidance_100Hz_Function_1D_M)->dwork.TRACK,
                           (Guidance_100Hz_Function_1D_M)->dwork.OPT_FP.Mopt,
                           (Guidance_100Hz_Function_1D_M)
                           ->dwork.OPT_FP.Mglide_opt, rtU_MPin->WeaponType,
                           TPosNED_uncond, MPosNED, MVelNED, MRefNED, &DCM,
                           (Guidance_100Hz_Function_1D_M)->dwork.ENV.mach,
                           (Guidance_100Hz_Function_1D_M)
                           ->dwork.opportunity_mode, rtU_DATAguid->AirframeMode,
                           (Guidance_100Hz_Function_1D_M)
                           ->dwork.guid_settings.psiR_Thr,
                           (Guidance_100Hz_Function_1D_M)
                           ->dwork.guid_settings.psi_Engage, (real_T)
                           rtU_DATAguid->AirframeProp.Rarc, ftime,
                           &(Guidance_100Hz_Function_1D_M)->dwork.guidtel_fpath,
                           &(Guidance_100Hz_Function_1D_M)->dwork.telem);

          /* 'GUID_Embedded_Guid_Control_1D:1535' TRACK.FPbunt_cnt = uint8(1); */
          Guidance_100Hz_Function_1D_M->dwork.TRACK.FPbunt_cnt = 1U;
        }
      }

      /* 'GUID_Embedded_Guid_Control_1D:1537' if (guidmode == 8 && guidcmds.gmode_sw) */
      if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 8) &&
          (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw) != 0))
      {
        /* 'GUID_Embedded_Guid_Control_1D:1538' TRACK.Ut8 = TRACK.Ut; */
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[0] =
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[0];
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[1] =
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[1];
        Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut8[2] =
          Guidance_100Hz_Function_1D_M->dwork.TRACK.Ut[2];
      }
    }

    /* 'GUID_Embedded_Guid_Control_1D:1542' MPosNED_prev.x = MPosNED.x; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.x = MPosNED.x;

    /* 'GUID_Embedded_Guid_Control_1D:1543' MPosNED_prev.y = MPosNED.y; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.y = MPosNED.y;

    /* 'GUID_Embedded_Guid_Control_1D:1544' MPosNED_prev.z = MPosNED.z; */
    Guidance_100Hz_Function_1D_M->dwork.MPosNED_prev.z = MPosNED.z;

    /* 'GUID_Embedded_Guid_Control_1D:1545' MVelNED_prev.x = MVelNED.x; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.x = MVelNED.x;

    /* 'GUID_Embedded_Guid_Control_1D:1546' MVelNED_prev.y = MVelNED.y; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.y = MVelNED.y;

    /* 'GUID_Embedded_Guid_Control_1D:1547' MVelNED_prev.z = MVelNED.z; */
    Guidance_100Hz_Function_1D_M->dwork.MVelNED_prev.z = MVelNED.z;

    /* 'GUID_Embedded_Guid_Control_1D:1549' if (guidmode == 8 && TRACK.reset) */
    if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) == 8) &&
        (((int32_T)Guidance_100Hz_Function_1D_M->dwork.TRACK.reset) != 0)) {
      /* 'GUID_Embedded_Guid_Control_1D:1550' guidmode = uint8(6); */
      Guidance_100Hz_Function_1D_M->dwork.guidmode = 6U;

      /* 'GUID_Embedded_Guid_Control_1D:1551' guidcmds.gmode_sw = uint8(2); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw = 2U;

      /* 'GUID_Embedded_Guid_Control_1D:1552' guidcmds.AZy = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZy = 1.0;

      /* 'GUID_Embedded_Guid_Control_1D:1553' guidcmds.AZz = 1.0; */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.AZz = 1.0;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1556' [TRACK,clos,gains,RrefL,RrefC] = GetCLOSParams(guidmode,guidcmds,ftime,gains,clos,TRACK,ENV,OPT_FP,DCM,TTKF_in,NAV_in,tgt0,fromDATA.AirframeMode,fromDATA.AirframeType,guid_settings); */
    expl_temp.z = phic;
    expl_temp.y = gbyc;
    expl_temp.x = gbzc;
    GetCLOSParams((Guidance_100Hz_Function_1D_M)->dwork.guidmode,
                  (Guidance_100Hz_Function_1D_M)->dwork.guidcmds.gmode_sw,
                  (Guidance_100Hz_Function_1D_M)->dwork.guidcmds.PurePropNav,
                  (Guidance_100Hz_Function_1D_M)->dwork.guidcmds.GndRge, ftime,
                  &(Guidance_100Hz_Function_1D_M)->dwork.gains,
                  &(Guidance_100Hz_Function_1D_M)->dwork.clos,
                  &(Guidance_100Hz_Function_1D_M)->dwork.TRACK,
                  (Guidance_100Hz_Function_1D_M)->dwork.ENV.FPthem,
                  (Guidance_100Hz_Function_1D_M)->dwork.ENV.FPpsim,
                  (Guidance_100Hz_Function_1D_M)->dwork.ENV.Vairspeed,
                  (Guidance_100Hz_Function_1D_M)->dwork.ENV.Vnav,
                  (Guidance_100Hz_Function_1D_M)->dwork.ENV.Q,
                  (Guidance_100Hz_Function_1D_M)->dwork.OPT_FP.Qopt,
                  (Guidance_100Hz_Function_1D_M)->dwork.OPT_FP.Mglide_opt, &DCM,
                  rtU_ttkfin, &(Guidance_100Hz_Function_1D_M)->dwork.NAV_in,
                  expl_temp, rtU_DATAguid->AirframeMode, term_vec, RrefC);

    /* 'GUID_Embedded_Guid_Control_1D:1560' temp_gbyc = 0.0; */
    /* 'GUID_Embedded_Guid_Control_1D:1561' guidcmds.phi_attack = attackdirection - pi; */
    Guidance_100Hz_Function_1D_M->dwork.guidcmds.phi_attack = psiLOSi -
      3.1415926535897931;

    /* 'GUID_Embedded_Guid_Control_1D:1562' if (ENV.FPthem < -1.222 && guidmode >= 6) */
    if ((Guidance_100Hz_Function_1D_M->dwork.ENV.FPthem < -1.222) && (((int32_T)
          Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 6)) {
      /* 'GUID_Embedded_Guid_Control_1D:1563' guidcmds.verticaldive = uint8(1); */
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.verticaldive = 1U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1565' if (TestCfg_in.GC_Mode == def.AP_DEFL1) */
    if (((int32_T)rtU_TestCfgin->GC_Mode) == 71) {
      /* 'GUID_Embedded_Guid_Control_1D:1566' [phic,gbyc,gbzc,guid2AP] = ExeDeflInspTest(ftime,guid2AP,guid_settings); */
      phic = 0.0;
      gbyc = 0.0;
      gbzc = 0.0;
      ExeDeflInspTest(ftime, &(Guidance_100Hz_Function_1D_M)->dwork.guid2AP,
                      (Guidance_100Hz_Function_1D_M)
                      ->dwork.guid_settings.matDEFL);

      /* 'GUID_Embedded_Guid_Control_1D:1567' guidmode = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guidmode = 0U;
    } else if ((((int32_T)rtU_TestCfgin->GC_Mode) == 31) || (((int32_T)
                 rtU_TestCfgin->GC_Mode) == 32)) {
      /* 'GUID_Embedded_Guid_Control_1D:1568' elseif (TestCfg_in.GC_Mode == def.AP_PROG1 || TestCfg_in.GC_Mode == def.AP_PROG2) */
      /* 'GUID_Embedded_Guid_Control_1D:1570' [phic,gbyc,gbzc,guidcmds,guid2AP] = ExeP1TestCommands(TestCfg_in.GC_Mode,guidmode,ftime,guidcmds,def,ENV,OPT_FP,DCM,TTKF_in,NAV_in,guid2AP,fromDATA,guid_settings); */
      ExeP1TestCommands(rtU_TestCfgin->GC_Mode, (Guidance_100Hz_Function_1D_M)
                        ->dwork.guidmode, ftime, &(Guidance_100Hz_Function_1D_M
                        )->dwork.guidcmds, 31.0, 32.0,
                        (Guidance_100Hz_Function_1D_M)->dwork.ENV.Vnav,
                        (Guidance_100Hz_Function_1D_M)->dwork.ENV.GRAVbody,
                        (Guidance_100Hz_Function_1D_M)->dwork.OPT_FP.GLim, &DCM,
                        rtU_ttkfin->tgtrge, rtU_ttkfin->dthe_b,
                        rtU_ttkfin->dpsi_b, &(Guidance_100Hz_Function_1D_M)
                        ->dwork.guid2AP, &(Guidance_100Hz_Function_1D_M)
                        ->dwork.guid_settings, &phic, &gbyc, &gbzc);
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1571' else */
      /* 'GUID_Embedded_Guid_Control_1D:1573' [phic,gbyc,gbzc,guidcmds,temp_gbyc] = ExeGuidanceCommands(opportunity_mode,guidmode,delt,ftime,guidcmds,fromDATA.AirframeMode,TRACK.reset,TRACK.bunt_arc,gains,TRACK,clos,ENV,OPT_FP,DCM,TTKF_in,NAV_in,IMU_in,MP_in,guid2AP,fromDATA,guid_settings,psiFPi,psiLOSb,def); */
      tmp = Guidance_100Hz_Function_1D_M->dwork.guid2AP;
      ExeGuidanceCommands((Guidance_100Hz_Function_1D_M)->dwork.opportunity_mode,
                          (Guidance_100Hz_Function_1D_M)->dwork.guidmode,
                          (Guidance_100Hz_Function_1D_M)->dwork.delt,
                          &(Guidance_100Hz_Function_1D_M)->dwork.guidcmds,
                          rtU_DATAguid->AirframeMode,
                          (Guidance_100Hz_Function_1D_M)->dwork.TRACK.reset,
                          (Guidance_100Hz_Function_1D_M)->dwork.TRACK.bunt_arc,
                          (Guidance_100Hz_Function_1D_M)->dwork.gains.Kp,
                          (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Ut0,
                          (Guidance_100Hz_Function_1D_M)->dwork.TRACK.Ut,
                          &(Guidance_100Hz_Function_1D_M)->dwork.clos,
                          &(Guidance_100Hz_Function_1D_M)->dwork.ENV,
                          &(Guidance_100Hz_Function_1D_M)->dwork.OPT_FP, &DCM,
                          rtU_ttkfin, &(Guidance_100Hz_Function_1D_M)
                          ->dwork.NAV_in, (Guidance_100Hz_Function_1D_M)
                          ->dwork.IMU_in, rtU_MPin, &tmp, rtU_DATAguid, psiLOSb,
                          &rtGUIDConstP.GuidanceController_def, &phic, &gbyc, &gbzc,
                          &psiLOSi);
    }

    /* 'GUID_Embedded_Guid_Control_1D:1613' if (nav_time > 5.0 && ~Discretes_in.BatteryOn && Discretes_in.umbilical==1) */
    if (((Guidance_100Hz_Function_1D_M->dwork.nav_time > 5.0) && (((int32_T)
           rtU_Discretesin->BatteryOn) == 0)) && (((int32_T)
          rtU_Discretesin->umbilical) == 1)) {
      /* 'GUID_Embedded_Guid_Control_1D:1615' guid2AP.AP_Loop = uint8(127); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 127U;
    } else if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 4) ||
               ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 3) &&
                (((int32_T)rtU_WeaCfgin->WingsPresent) != 0))) {
      /* 'GUID_Embedded_Guid_Control_1D:1617' elseif (guidmode >= 4 || (guidmode >= 3 && WeaCfg_in.WingsPresent)) */
      /* 'GUID_Embedded_Guid_Control_1D:1619' guid2AP.AP_Loop = uint8(255); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = MAX_uint8_T;
    } else if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 3) {
      /* 'GUID_Embedded_Guid_Control_1D:1620' elseif (guidmode >= 3) */
      /* 'GUID_Embedded_Guid_Control_1D:1622' guid2AP.AP_Loop = uint8(195); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 195U;
    } else if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 1) {
      /* 'GUID_Embedded_Guid_Control_1D:1625' elseif (guidmode >= 1) */
      /* 'GUID_Embedded_Guid_Control_1D:1627' guid2AP.AP_Loop = uint8(192); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 192U;
    } else {
      /* 'GUID_Embedded_Guid_Control_1D:1628' else */
      /* 'GUID_Embedded_Guid_Control_1D:1629' guid2AP.AP_Loop = uint8(0); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 0U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1632' if (guidmode >= 9 && bitget(TTKF_in.status,4)) */
    if ((((int32_T)Guidance_100Hz_Function_1D_M->dwork.guidmode) >= 9) &&
        ((((uint32_T)rtU_ttkfin->status) & 8U) != 0U)) {
      /* 'GUID_Embedded_Guid_Control_1D:1634' guid2AP.AP_Loop = uint8(127); */
      Guidance_100Hz_Function_1D_M->dwork.guid2AP.AP_Loop = 127U;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1636' guid2AP.ftime = ftime; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.ftime = ftime;

    /* 'GUID_Embedded_Guid_Control_1D:1637' guid2AP.verticaldive = guidcmds.verticaldive; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.verticaldive =
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.verticaldive;

    /* 'GUID_Embedded_Guid_Control_1D:1639' guid2AP.phic = phic; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.phic = phic;

    /* 'GUID_Embedded_Guid_Control_1D:1640' guid2AP.gbyc = gbyc; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbyc = gbyc;

    /* 'GUID_Embedded_Guid_Control_1D:1641' guid2AP.gbzc = gbzc; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.gbzc = gbzc;

    /* 'GUID_Embedded_Guid_Control_1D:1643' guid2AP.phim  = guidcmds.phim; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.phim =
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.phim;

    /* 'GUID_Embedded_Guid_Control_1D:1644' guid2AP.FPthec= OPT_FP.FPcmdD; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.FPthec =
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPcmdD;

    /* 'GUID_Embedded_Guid_Control_1D:1645' guid2AP.FPthem= ENV.FPthem; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.FPthem =
      Guidance_100Hz_Function_1D_M->dwork.ENV.FPthem;

    /* 'GUID_Embedded_Guid_Control_1D:1660' guid2AP.tgtrge = TTKF_in.tgtrge; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.tgtrge = rtU_ttkfin->tgtrge;

    /* 'GUID_Embedded_Guid_Control_1D:1662' guid2AP.mach   = ENV.mach; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.mach =
      Guidance_100Hz_Function_1D_M->dwork.ENV.mach;

    /* 'GUID_Embedded_Guid_Control_1D:1663' guid2AP.alpha  = ENV.alpha; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.alpha =
      Guidance_100Hz_Function_1D_M->dwork.ENV.alpha;

    /* 'GUID_Embedded_Guid_Control_1D:1664' guid2AP.beta   = ENV.beta; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.beta =
      Guidance_100Hz_Function_1D_M->dwork.ENV.beta;

    /* 'GUID_Embedded_Guid_Control_1D:1665' guid2AP.Qratio = ENV.Qratio; */
    Guidance_100Hz_Function_1D_M->dwork.guid2AP.Qratio =
      Guidance_100Hz_Function_1D_M->dwork.ENV.Qratio;

    /* 'GUID_Embedded_Guid_Control_1D:1669' guid_status =                  1.0*double(TRACK.reset); */
    /* 'GUID_Embedded_Guid_Control_1D:1670' guid_status = guid_status +    4.0*double(TRACK.solution+5); */
    /* 'GUID_Embedded_Guid_Control_1D:1671' guid_status = guid_status +   32.0*double(guidcmds.gmode_sw); */
    /* 'GUID_Embedded_Guid_Control_1D:1672' guid_status = guid_status +   64.0*double(guidFLAGS.PstaticValid); */
    /* 'GUID_Embedded_Guid_Control_1D:1673' guid_status = guid_status + 1024.0*double(guidFLAGS.Umbilical); */
    /* 'GUID_Embedded_Guid_Control_1D:1674' guid_status = guid_status + 2048.0*double(TRACK.bunt_arc); */
    /* 'GUID_Embedded_Guid_Control_1D:1675' guid_status = guid_status + 4096.0*double(keep_arc_8); */
    /* 'GUID_Embedded_Guid_Control_1D:1676' guid_status = guid_status + 8192.0*double(fromDATA.AirframeMode); */
    guidtel20HzB_tmp = ((int32_T)
                        Guidance_100Hz_Function_1D_M->dwork.TRACK.solution) + 5;
    if (guidtel20HzB_tmp > 127) {
      guidtel20HzB_tmp = 127;
    }

    ValidGNSSNav = (((((((guidtel20HzB_tmp << ((uint64_T)2)) + ((int32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.reset)) + ((int32_T)((uint32_T)
      (((uint32_T)Guidance_100Hz_Function_1D_M->dwork.guidcmds.gmode_sw) <<
       ((uint64_T)5))))) + ((int32_T)((uint32_T)(((uint32_T)
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.Umbilical) << ((uint64_T)10)))))
                      + ((int32_T)((uint32_T)(((uint32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.bunt_arc) << ((uint64_T)11)))))
                     + ((int32_T)((uint32_T)(((uint32_T)keep_arc_8) <<
      ((uint64_T)12))))) + ((int32_T)((uint32_T)(((uint32_T)
      rtU_DATAguid->AirframeMode) << ((uint64_T)13))))) + 64;

    /* 'GUID_Embedded_Guid_Control_1D:1680' guidtel100Hz.GUI_mode        = guidmode; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_mode =
      Guidance_100Hz_Function_1D_M->dwork.guidmode;

    /* 'GUID_Embedded_Guid_Control_1D:1681' guidtel100Hz.GUI_FPbunt_cnt  = TRACK.FPbunt_cnt; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPbunt_cnt =
      Guidance_100Hz_Function_1D_M->dwork.TRACK.FPbunt_cnt;

    /* 'GUID_Embedded_Guid_Control_1D:1682' guidtel100Hz.GUI_Status      = uint16(LIB_limit(0,65535,guid_status)); */
    /* 'LIB_limit:27' if (in > max) */
    if (ValidGNSSNav > 65535) {
      /* 'LIB_limit:28' out = max; */
      ValidGNSSNav = 65535;
    } else if (ValidGNSSNav < 0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      ValidGNSSNav = 0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Status = (uint16_T)
      ValidGNSSNav;

    /* 'GUID_Embedded_Guid_Control_1D:1683' guidtel100Hz.GUI_GuidTime    = single(ftime); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_GuidTime = (real32_T)
      ftime;

    /* 'GUID_Embedded_Guid_Control_1D:1684' guidtel100Hz.GUI_PhiCmd      = int16(LIB_limit(-32768,32767,phic*10000)); */
    psiLOSb = phic * 10000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (psiLOSb > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      psiLOSb = 32767.0;
    } else if (psiLOSb < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      psiLOSb = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(psiLOSb);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_PhiCmd = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_PhiCmd = MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1686' guidtel100Hz.GUI_FPTheCmd    = int16(LIB_limit(-32768,32767,OPT_FP.FPcmdD*10000)); */
    psiLOSb = Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPcmdD * 10000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (psiLOSb > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      psiLOSb = 32767.0;
    } else if (psiLOSb < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      psiLOSb = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(psiLOSb);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPTheCmd = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPTheCmd =
        MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1687' guidtel100Hz.GUI_FPGBYCmd    = int16(LIB_limit(-32768,32767,gbyc*500)); */
    psiLOSb = gbyc * 500.0;

    /* 'LIB_limit:27' if (in > max) */
    if (psiLOSb > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      psiLOSb = 32767.0;
    } else if (psiLOSb < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      psiLOSb = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(psiLOSb);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBYCmd = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBYCmd =
        MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1688' guidtel100Hz.GUI_FPGBZCmd    = int16(LIB_limit(-32768,32767,gbzc*500)); */
    gbzc *= 500.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 32767.0;
    } else if (gbzc < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBZCmd = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_FPGBZCmd =
        MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1689' guidtel100Hz.GUI_NavCtrlFlag = guidFLAGS.NavCtrlFlag; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_NavCtrlFlag =
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.NavCtrlFlag;

    /* 'GUID_Embedded_Guid_Control_1D:1690' guidtel100Hz.GUI_ENVtemp     = uint8(LIB_limit(0,255,ENV.temp*3 + 100)); */
    gbzc = (Guidance_100Hz_Function_1D_M->dwork.ENV.temp * 3.0) + 100.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 255.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 255.0;
    } else if (gbzc < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 256.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_ENVtemp = (uint8_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_ENVtemp = MAX_uint8_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1691' guidtel100Hz.GUI_Qratio      = int16(LIB_limit(-32768,32767,ENV.Qratio*1000)); */
    gbzc = Guidance_100Hz_Function_1D_M->dwork.ENV.Qratio * 1000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 32767.0;
    } else if (gbzc < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Qratio = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Qratio = MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1692' guidtel100Hz.GUI_BiasAlpha   = int16(LIB_limit(-32768,32767,ENV.BIAS_alpha*100000)); */
    gbzc = Guidance_100Hz_Function_1D_M->dwork.ENV.BIAS_alpha * 100000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 32767.0;
    } else if (gbzc < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_BiasAlpha = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_BiasAlpha =
        MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1693' guidtel100Hz.GUI_Rarc_T      = uint16(LIB_limit(0,65535,clos.ArcState.Rarc_T)); */
    /* 'LIB_limit:27' if (in > max) */
    if (Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_T > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 65535.0;
    } else if (Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_T < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      gbzc = Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rarc_T;
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 65536.0) {
      if (dNavPos >= 0.0) {
        Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Rarc_T = (uint16_T)
          dNavPos;
      } else {
        Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Rarc_T = 0U;
      }
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_Rarc_T = MAX_uint16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1694' guidtel100Hz.GUI_CASErr      = int16(LIB_limit(-32768,32767,TRACK.Mglide*1000)); */
    gbzc = Guidance_100Hz_Function_1D_M->dwork.TRACK.Mglide * 1000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 32767.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 32767.0;
    } else if (gbzc < -32768.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = -32768.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 32768.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_CASErr = (int16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_CASErr = MAX_int16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1695' guidtel100Hz.GUI_AccLim      = uint16(LIB_limit(0,65535,OPT_FP.GLim*500)); */
    gbzc = Guidance_100Hz_Function_1D_M->dwork.OPT_FP.GLim * 500.0;

    /* 'LIB_limit:27' if (in > max) */
    if (gbzc > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      gbzc = 65535.0;
    } else if (gbzc < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      gbzc = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    dNavPos = rt_roundd_snf(gbzc);
    if (dNavPos < 65536.0) {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_AccLim = (uint16_T)
        dNavPos;
    } else {
      Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_AccLim = MAX_uint16_T;
    }

    /* 'GUID_Embedded_Guid_Control_1D:1696' guidtel100Hz.GUI_WingOpenCmd = guidFLAGS.WingsOpen; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_WingOpenCmd =
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsOpen;

    /* 'GUID_Embedded_Guid_Control_1D:1697' guidtel100Hz.GUI_WingRelCmd  = guidFLAGS.WingsRelease; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_WingRelCmd =
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.WingsRelease;

    /* 'GUID_Embedded_Guid_Control_1D:1698' guidtel100Hz.GUI_SADFuze     = guidFLAGS.SADFuze; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_SADFuze =
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SADFuze;

    /* 'GUID_Embedded_Guid_Control_1D:1699' guidtel100Hz.GUI_SwitchforBunt = guidFLAGS.SwitchforBunt; */
    Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz.GUI_SwitchforBunt =
      Guidance_100Hz_Function_1D_M->dwork.guidFLAGS.SwitchforBunt;

    /* 'GUID_Embedded_Guid_Control_1D:1703' guidtel20HzA.GUI_rho         = single(ENV.rho); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_rho = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.rho;

    /* 'GUID_Embedded_Guid_Control_1D:1704' guidtel20HzA.GUI_Vsound      = single(ENV.Vsound); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vsound = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.Vsound;

    /* 'GUID_Embedded_Guid_Control_1D:1705' guidtel20HzA.GUI_FPthem      = single(ENV.FPthem); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPthem = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.FPthem;

    /* 'GUID_Embedded_Guid_Control_1D:1706' guidtel20HzA.GUI_Mach        = single(ENV.mach); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mach = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.mach;

    /* 'GUID_Embedded_Guid_Control_1D:1707' guidtel20HzA.GUI_Alpha       = single(ENV.alpha); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Alpha = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.alpha;

    /* 'GUID_Embedded_Guid_Control_1D:1708' guidtel20HzA.GUI_Beta        = single(ENV.beta); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Beta = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.beta;

    /* 'GUID_Embedded_Guid_Control_1D:1709' guidtel20HzA.GUI_Vdyn        = single(ENV.Vairspeed); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vdyn = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.Vairspeed;

    /* 'GUID_Embedded_Guid_Control_1D:1710' guidtel20HzA.GUI_Qdyn        = single(ENV.Q); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Qdyn = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.Q;

    /* 'GUID_Embedded_Guid_Control_1D:1711' guidtel20HzA.GUI_Gcompx      = single(ENV.GRAVbody(1)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompx = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[0];

    /* 'GUID_Embedded_Guid_Control_1D:1712' guidtel20HzA.GUI_Gcompy      = single(ENV.GRAVbody(2)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompy = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[1];

    /* 'GUID_Embedded_Guid_Control_1D:1713' guidtel20HzA.GUI_Gcompz      = single(ENV.GRAVbody(3)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Gcompz = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.ENV.GRAVbody[2];

    /* 'GUID_Embedded_Guid_Control_1D:1716' guidtel20HzA.GUI_gbycD0      = single(guidcmds.gbzcD); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_gbycD0 = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.guidcmds.gbzcD;

    /* 'GUID_Embedded_Guid_Control_1D:1717' guidtel20HzA.GUI_Clest       = single(OPT_FP.Cl); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Clest = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Cl;

    /* 'GUID_Embedded_Guid_Control_1D:1718' guidtel20HzA.GUI_ClCd_M      = single(OPT_FP.ClCd_M); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_ClCd_M = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.ClCd_M;

    /* 'GUID_Embedded_Guid_Control_1D:1719' guidtel20HzA.GUI_CNgrav      = single(OPT_FP.CNgrav); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_CNgrav = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.CNgrav;

    /* 'GUID_Embedded_Guid_Control_1D:1721' guidtel20HzA.GUI_FPCmd0      = single(OPT_FP.Cd0); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPCmd0 = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Cd0;

    /* 'GUID_Embedded_Guid_Control_1D:1722' guidtel20HzA.GUI_Qopt        = single(OPT_FP.Qopt); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Qopt = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Qopt;

    /* 'GUID_Embedded_Guid_Control_1D:1723' guidtel20HzA.GUI_Vopt        = single(OPT_FP.Vopt); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Vopt = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Vopt;

    /* 'GUID_Embedded_Guid_Control_1D:1724' guidtel20HzA.GUI_Mopt        = single(OPT_FP.Mopt); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mopt = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mopt;

    /* 'GUID_Embedded_Guid_Control_1D:1725' guidtel20HzA.GUI_FPopt       = single(OPT_FP.FPopt); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPopt = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPopt;

    /* 'GUID_Embedded_Guid_Control_1D:1726' guidtel20HzA.GUI_FPerrD      = single(OPT_FP.FPerrD); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_FPerrD = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.FPerrD;

    /* 'GUID_Embedded_Guid_Control_1D:1727' guidtel20HzA.GUI_alpha_M     = single(OPT_FP.alpha_M); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_alpha_M = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.alpha_M;

    /* 'GUID_Embedded_Guid_Control_1D:1728' guidtel20HzA.GUI_Mglideopt   = single(OPT_FP.Mglide_opt); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mglideopt = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.OPT_FP.Mglide_opt;

    /* 'GUID_Embedded_Guid_Control_1D:1729' guidtel20HzA.GUI_Kglide      = single(clos.Kglide); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kglide = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Kglide;

    /* 'GUID_Embedded_Guid_Control_1D:1733' guidtel20HzA.GUI_Rapr_Tx     = single(clos.ArcState.Rapr_T(1)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Tx = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[0];

    /* 'GUID_Embedded_Guid_Control_1D:1734' guidtel20HzA.GUI_Rapr_Ty     = single(clos.ArcState.Rapr_T(2)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Ty = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[1];

    /* 'GUID_Embedded_Guid_Control_1D:1735' guidtel20HzA.GUI_Rapr_Tz     = single(clos.ArcState.Rapr_T(3)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rapr_Tz = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.ArcState.Rapr_T[2];

    /* 'GUID_Embedded_Guid_Control_1D:1736' guidtel20HzA.GUI_Rref0x      = single(clos.Rref(1)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0x = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Rref[0];

    /* 'GUID_Embedded_Guid_Control_1D:1737' guidtel20HzA.GUI_Rref0y      = single(clos.Rref(2)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0y = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Rref[1];

    /* 'GUID_Embedded_Guid_Control_1D:1738' guidtel20HzA.GUI_Rref0z      = single(clos.Rref(3)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Rref0z = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Rref[2];

    /* 'GUID_Embedded_Guid_Control_1D:1739' guidtel20HzA.GUI_RrefWx      = single(clos.Rref(1)+clos.Recef(1)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWx = (real32_T)
      ((real_T)((real_T)(Guidance_100Hz_Function_1D_M->dwork.clos.Rref[0] +
                         Guidance_100Hz_Function_1D_M->dwork.clos.Recef[0])));

    /* 'GUID_Embedded_Guid_Control_1D:1740' guidtel20HzA.GUI_RrefWy      = single(clos.Rref(2)+clos.Recef(2)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWy = (real32_T)
      ((real_T)((real_T)(Guidance_100Hz_Function_1D_M->dwork.clos.Rref[1] +
                         Guidance_100Hz_Function_1D_M->dwork.clos.Recef[1])));

    /* 'GUID_Embedded_Guid_Control_1D:1741' guidtel20HzA.GUI_RrefWz      = single(clos.Rref(3)+clos.Recef(3)); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RrefWz = (real32_T)
      ((real_T)((real_T)(Guidance_100Hz_Function_1D_M->dwork.clos.Rref[2] +
                         Guidance_100Hz_Function_1D_M->dwork.clos.Recef[2])));

    /* 'GUID_Embedded_Guid_Control_1D:1742' guidtel20HzA.GUI_Mpos0x      = single(TRACK.mpos0.x); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0x = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.x;

    /* 'GUID_Embedded_Guid_Control_1D:1743' guidtel20HzA.GUI_Mpos0y      = single(TRACK.mpos0.y); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0y = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.y;

    /* 'GUID_Embedded_Guid_Control_1D:1744' guidtel20HzA.GUI_Mpos0z      = single(TRACK.mpos0.z); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Mpos0z = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.mpos0.z;

    /* 'GUID_Embedded_Guid_Control_1D:1745' guidtel20HzA.GUI_RweaEx      = single(TRACK.Rwe.x); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEx = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.x;

    /* 'GUID_Embedded_Guid_Control_1D:1746' guidtel20HzA.GUI_RweaEy      = single(TRACK.Rwe.y); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEy = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.y;

    /* 'GUID_Embedded_Guid_Control_1D:1747' guidtel20HzA.GUI_RweaEz      = single(TRACK.Rwe.z); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_RweaEz = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.TRACK.Rwe.z;

    /* 'GUID_Embedded_Guid_Control_1D:1750' guidtel20HzA.GUI_Ki          = single(gains.Ki); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Ki = 0.0F;

    /* 'GUID_Embedded_Guid_Control_1D:1751' guidtel20HzA.GUI_Kp          = single(gains.Kp); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kp = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.gains.Kp;

    /* 'GUID_Embedded_Guid_Control_1D:1752' guidtel20HzA.GUI_Kv          = single(gains.Kv); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kv = 1.2F;

    /* 'GUID_Embedded_Guid_Control_1D:1753' guidtel20HzA.GUI_Kfp         = single(gains.Kfp); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_Kfp = 50.0F; // 150.0F;

    /* 'GUID_Embedded_Guid_Control_1D:1756' guidtel20HzA.GUI_XTPosY      = single(clos.Rby); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTPosY = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Rby;

    /* 'GUID_Embedded_Guid_Control_1D:1757' guidtel20HzA.GUI_XTPosZ      = single(clos.Rbz); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTPosZ = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Rbz;

    /* 'GUID_Embedded_Guid_Control_1D:1758' guidtel20HzA.GUI_XTVelY      = single(clos.Vby); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTVelY = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Vby;

    /* 'GUID_Embedded_Guid_Control_1D:1759' guidtel20HzA.GUI_XTVelZ      = single(clos.Vbz); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTVelZ = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Vbz;

    /* 'GUID_Embedded_Guid_Control_1D:1760' guidtel20HzA.GUI_XTAccY      = single(clos.Aby); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTAccY = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Aby;

    /* 'GUID_Embedded_Guid_Control_1D:1761' guidtel20HzA.GUI_XTAccZ      = single(clos.Abz); */
    Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA.GUI_XTAccZ = (real32_T)
      Guidance_100Hz_Function_1D_M->dwork.clos.Abz;

    /* 'GUID_Embedded_Guid_Control_1D:1765' if (ftime >= telem.time20Hz) */
    if (ftime >= Guidance_100Hz_Function_1D_M->dwork.telem.time20Hz) {
      /* 'GUID_Embedded_Guid_Control_1D:1766' if (telem.IW > telem.IW_last) */
      if (Guidance_100Hz_Function_1D_M->dwork.telem.IW >
          Guidance_100Hz_Function_1D_M->dwork.telem.IW_last) {
        /* 'GUID_Embedded_Guid_Control_1D:1767' telem.IW_last = telem.IW_last + 1; */
        qY = ((uint32_T)Guidance_100Hz_Function_1D_M->dwork.telem.IW_last) + 1U;
        if (qY > 65535U) {
          qY = 65535U;
        }

        Guidance_100Hz_Function_1D_M->dwork.telem.IW_last = (uint16_T)qY;

        /* 'GUID_Embedded_Guid_Control_1D:1768' if (telem.IW_last > 1000) */
        if (((int32_T)Guidance_100Hz_Function_1D_M->dwork.telem.IW_last) > 1000)
        {
          /* 'GUID_Embedded_Guid_Control_1D:1769' telem.IW_last = uint16(1000); */
          Guidance_100Hz_Function_1D_M->dwork.telem.IW_last = 1000U;
        }

        /* 'GUID_Embedded_Guid_Control_1D:1775' guidtel20HzB.GUI_iW          = guidtel_fpath.GUI_iW(telem.IW_last); */
        guidtel20HzB_tmp = ((int32_T)
                            Guidance_100Hz_Function_1D_M->dwork.telem.IW_last) -
          1;
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_iW =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_iW[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1776' guidtel20HzB.GUI_solution    = guidtel_fpath.GUI_solution(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_solution =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_solution[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1777' guidtel20HzB.GUI_inccnt      = guidtel_fpath.GUI_inccnt(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_inccnt =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_inccnt[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1778' guidtel20HzB.GUI_ftime       = guidtel_fpath.GUI_ftime(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ftime =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_ftime[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1779' guidtel20HzB.GUI_Rarc        = guidtel_fpath.GUI_Rarc(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rarc =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rarc[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1780' guidtel20HzB.GUI_RarcW       = guidtel_fpath.GUI_RarcW(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_RarcW[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1781' guidtel20HzB.GUI_Rterm       = guidtel_fpath.GUI_Rterm(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rterm =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rterm[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1782' guidtel20HzB.GUI_Km          = guidtel_fpath.GUI_Km(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Km =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Km[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1783' guidtel20HzB.GUI_RtermT      = guidtel_fpath.GUI_RtermT(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermT =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_RtermT[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1784' guidtel20HzB.GUI_RtermW      = guidtel_fpath.GUI_RtermW(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermW =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_RtermW[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1785' guidtel20HzB.GUI_LT          = guidtel_fpath.GUI_LT(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LT =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_LT[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1786' guidtel20HzB.GUI_LW          = guidtel_fpath.GUI_LW(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LW =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_LW[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1787' guidtel20HzB.GUI_Mglide      = guidtel_fpath.GUI_Mglide(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Mglide =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Mglide[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1788' guidtel20HzB.GUI_FPazi       = guidtel_fpath.GUI_FPazi(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_FPazi =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_FPazi[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1789' guidtel20HzB.GUI_ratio       = guidtel_fpath.GUI_ratio(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ratio =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_ratio[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1790' guidtel20HzB.GUI_ImpAng      = guidtel_fpath.GUI_ImpAng(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ImpAng =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_ImpAng[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1791' guidtel20HzB.GUI_AttHeading  = guidtel_fpath.GUI_AttHeading(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_AttHeading =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_AttHeading[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1792' guidtel20HzB.GUI_intsctTx    = guidtel_fpath.GUI_intsctTx(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTx =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctTx[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1793' guidtel20HzB.GUI_intsctTy    = guidtel_fpath.GUI_intsctTy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctTy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1794' guidtel20HzB.GUI_intsctTz    = guidtel_fpath.GUI_intsctTz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctTz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1795' guidtel20HzB.GUI_intsctWx    = guidtel_fpath.GUI_intsctWx(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWx =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctWx[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1796' guidtel20HzB.GUI_intsctWy    = guidtel_fpath.GUI_intsctWy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctWy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1797' guidtel20HzB.GUI_intsctWz    = guidtel_fpath.GUI_intsctWz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_intsctWz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1798' guidtel20HzB.GUI_Rtox        = guidtel_fpath.GUI_Rtox(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtox =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtox[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1799' guidtel20HzB.GUI_Rtoy        = guidtel_fpath.GUI_Rtoy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtoy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1800' guidtel20HzB.GUI_Rtoz        = guidtel_fpath.GUI_Rtoz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtoz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1801' guidtel20HzB.GUI_Rtex        = guidtel_fpath.GUI_Rtex(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtex =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtex[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1802' guidtel20HzB.GUI_Rtey        = guidtel_fpath.GUI_Rtey(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtey =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtey[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1803' guidtel20HzB.GUI_Rtez        = guidtel_fpath.GUI_Rtez(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtez =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rtez[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1804' guidtel20HzB.GUI_Rwox        = guidtel_fpath.GUI_Rwox(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwox =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwox[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1805' guidtel20HzB.GUI_Rwoy        = guidtel_fpath.GUI_Rwoy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwoy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1806' guidtel20HzB.GUI_Rwoz        = guidtel_fpath.GUI_Rwoz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwoz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1807' guidtel20HzB.GUI_Rwex        = guidtel_fpath.GUI_Rwex(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwex =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwex[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1808' guidtel20HzB.GUI_Rwey        = guidtel_fpath.GUI_Rwey(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwey =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwey[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1809' guidtel20HzB.GUI_Rwez        = guidtel_fpath.GUI_Rwez(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwez =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rwez[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1810' guidtel20HzB.GUI_Rmx         = guidtel_fpath.GUI_Rmx(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmx =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rmx[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1811' guidtel20HzB.GUI_Rmy         = guidtel_fpath.GUI_Rmy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rmy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1812' guidtel20HzB.GUI_Rmz         = guidtel_fpath.GUI_Rmz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_Rmz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1813' guidtel20HzB.GUI_a0Wx        = guidtel_fpath.GUI_a0Wx(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wx =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Wx[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1814' guidtel20HzB.GUI_a0Wy        = guidtel_fpath.GUI_a0Wy(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wy =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Wy[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1815' guidtel20HzB.GUI_a0Wz        = guidtel_fpath.GUI_a0Wz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Wz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1816' guidtel20HzB.GUI_a0Tx        = guidtel_fpath.GUI_a0Tx(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tx =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Tx[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1817' guidtel20HzB.GUI_a0Ty        = guidtel_fpath.GUI_a0Ty(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Ty =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Ty[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1818' guidtel20HzB.GUI_a0Tz        = guidtel_fpath.GUI_a0Tz(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tz =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_a0Tz[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1819' guidtel20HzB.GUI_spare       = guidtel_fpath.GUI_spare(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_spare = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1820' guidtel20HzB.GUI_tgt0x       = guidtel_fpath.GUI_tgt0x(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0x =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_tgt0x[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1821' guidtel20HzB.GUI_tgt0y       = guidtel_fpath.GUI_tgt0y(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0y =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_tgt0y[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1822' guidtel20HzB.GUI_tgt0z       = guidtel_fpath.GUI_tgt0z(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0z =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_tgt0z[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1823' guidtel20HzB.GUI_altcnt      = guidtel_fpath.GUI_altcnt(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_altcnt =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_altcnt[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1824' guidtel20HzB.GUI_R2buntREQ   = guidtel_fpath.GUI_R2buntREQ(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntREQ =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_R2buntREQ[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1825' guidtel20HzB.GUI_R2buntAVL   = guidtel_fpath.GUI_R2buntAVL(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntAVL =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_R2buntAVL[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1826' guidtel20HzB.GUI_RarcW_cmp   = guidtel_fpath.GUI_RarcW_cmp(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW_cmp =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_RarcW_cmp[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1827' guidtel20HzB.GUI_theFP3D     = guidtel_fpath.GUI_theFP3D(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP3D =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_theFP3D[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1828' guidtel20HzB.GUI_theFP2D     = guidtel_fpath.GUI_theFP2D(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP2D =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_theFP2D[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1829' guidtel20HzB.GUI_theErr      = guidtel_fpath.GUI_theErr(telem.IW_last); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theErr =
          Guidance_100Hz_Function_1D_M->
          dwork.guidtel_fpath.GUI_theErr[guidtel20HzB_tmp];

        /* 'GUID_Embedded_Guid_Control_1D:1830' telem.time20Hz = ftime + 0.050; */
        Guidance_100Hz_Function_1D_M->dwork.telem.time20Hz = ftime + 0.05;
      } else {
        /* 'GUID_Embedded_Guid_Control_1D:1831' else */
        /* 'GUID_Embedded_Guid_Control_1D:1833' guidtel20HzB.GUI_solution    = int8(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_solution = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1834' guidtel20HzB.GUI_inccnt      = uint8(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_inccnt = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1835' guidtel20HzB.GUI_ftime       = single(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ftime = 0.0F;

        /* 'GUID_Embedded_Guid_Control_1D:1836' guidtel20HzB.GUI_Rarc        = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rarc = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1837' guidtel20HzB.GUI_RarcW       = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1838' guidtel20HzB.GUI_Rterm       = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rterm = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1839' guidtel20HzB.GUI_Km          = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Km = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1840' guidtel20HzB.GUI_RtermT      = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermT = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1841' guidtel20HzB.GUI_RtermW      = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RtermW = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1842' guidtel20HzB.GUI_LT          = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LT = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1843' guidtel20HzB.GUI_LW          = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_LW = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1844' guidtel20HzB.GUI_Mglide      = single(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Mglide = 0.0F;

        /* 'GUID_Embedded_Guid_Control_1D:1845' guidtel20HzB.GUI_FPazi       = single(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_FPazi = 0.0F;

        /* 'GUID_Embedded_Guid_Control_1D:1846' guidtel20HzB.GUI_ratio       = single(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ratio = 0.0F;

        /* 'GUID_Embedded_Guid_Control_1D:1847' guidtel20HzB.GUI_ImpAng      = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_ImpAng = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1848' guidtel20HzB.GUI_AttHeading  = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_AttHeading = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1849' guidtel20HzB.GUI_intsctTx    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTx = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1850' guidtel20HzB.GUI_intsctTy    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1851' guidtel20HzB.GUI_intsctTz    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctTz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1852' guidtel20HzB.GUI_intsctWx    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWx = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1853' guidtel20HzB.GUI_intsctWy    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1854' guidtel20HzB.GUI_intsctWz    = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_intsctWz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1855' guidtel20HzB.GUI_Rtox        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtox = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1856' guidtel20HzB.GUI_Rtoy        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1857' guidtel20HzB.GUI_Rtoz        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtoz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1858' guidtel20HzB.GUI_Rtex        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtex = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1859' guidtel20HzB.GUI_Rtey        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtey = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1860' guidtel20HzB.GUI_Rtez        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rtez = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1861' guidtel20HzB.GUI_Rwox        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwox = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1862' guidtel20HzB.GUI_Rwoy        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1863' guidtel20HzB.GUI_Rwoz        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwoz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1864' guidtel20HzB.GUI_Rwex        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwex = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1865' guidtel20HzB.GUI_Rwey        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwey = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1866' guidtel20HzB.GUI_Rwez        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rwez = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1867' guidtel20HzB.GUI_Rmx         = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmx = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1868' guidtel20HzB.GUI_Rmy         = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1869' guidtel20HzB.GUI_Rmz         = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_Rmz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1870' guidtel20HzB.GUI_a0Wx        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wx = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1871' guidtel20HzB.GUI_a0Wy        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wy = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1872' guidtel20HzB.GUI_a0Wz        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Wz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1873' guidtel20HzB.GUI_a0Tx        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tx = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1874' guidtel20HzB.GUI_a0Ty        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Ty = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1875' guidtel20HzB.GUI_a0Tz        = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_a0Tz = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1876' guidtel20HzB.GUI_spare       = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_spare = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1877' guidtel20HzB.GUI_tgt0x       = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0x = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1878' guidtel20HzB.GUI_tgt0y       = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0y = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1879' guidtel20HzB.GUI_tgt0z       = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_tgt0z = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1880' guidtel20HzB.GUI_altcnt      = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_altcnt = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1881' guidtel20HzB.GUI_R2buntREQ   = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntREQ = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1882' guidtel20HzB.GUI_R2buntAVL   = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_R2buntAVL = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1883' guidtel20HzB.GUI_RarcW_cmp   = uint16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_RarcW_cmp = 0U;

        /* 'GUID_Embedded_Guid_Control_1D:1884' guidtel20HzB.GUI_theFP3D     = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP3D = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1885' guidtel20HzB.GUI_theFP2D     = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theFP2D = 0;

        /* 'GUID_Embedded_Guid_Control_1D:1886' guidtel20HzB.GUI_theErr      = int16(0); */
        Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB.GUI_theErr = 0;
      }
    }
  }

  /* Outport: '<Root>/guid2AP' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  /* 'GUID_Embedded_Guid_Control_1D:1895' Guid2AP       = guid2AP; */
  /* 'GUID_Embedded_Guid_Control_1D:1897' GuidCtrlFlags = guidFLAGS; */
  /* 'GUID_Embedded_Guid_Control_1D:1899' Guid2DATACtrl = guid2DATA; */
  /* 'GUID_Embedded_Guid_Control_1D:1901' Guid_100Hz_tel = guidtel100Hz; */
  /* 'GUID_Embedded_Guid_Control_1D:1902' Guid_20HzA_tel = guidtel20HzA; */
  /* 'GUID_Embedded_Guid_Control_1D:1903' Guid_20HzB_tel = guidtel20HzB; */
  /* 'GUID_Embedded_Guid_Control_1D:1905' Guid_ana.p001 = double(TRACK.reset); */
  /* 'GUID_Embedded_Guid_Control_1D:1906' Guid_ana.p002 = TRACK.AZrefL; */
  /* 'GUID_Embedded_Guid_Control_1D:1907' Guid_ana.p003 = TRACK.AZref; */
  /* 'GUID_Embedded_Guid_Control_1D:1908' Guid_ana.p004 = OPT_FP.GLimZup_prev; */
  /* 'GUID_Embedded_Guid_Control_1D:1909' Guid_ana.p005 = ENV.pressure; */
  /* 'GUID_Embedded_Guid_Control_1D:1910' Guid_ana.p006 = OPT_FP.GLimZup; */
  /* 'GUID_Embedded_Guid_Control_1D:1911' Guid_ana.p007 = OPT_FP.GLimZlo; */
  /* 'GUID_Embedded_Guid_Control_1D:1912' Guid_ana.p008 = OPT_FP.GLimYup; */
  /* 'GUID_Embedded_Guid_Control_1D:1913' Guid_ana.p009 = OPT_FP.GLimYlo; */
  /* 'GUID_Embedded_Guid_Control_1D:1914' Guid_ana.p010 = guidcmds.gbyc_FOV; */
  /* 'GUID_Embedded_Guid_Control_1D:1915' Guid_ana.p011 = guidcmds.gbzc_FOV; */
  /* 'GUID_Embedded_Guid_Control_1D:1916' Guid_ana.p012 = 3.0*guidcmds.inty_FOV; */
  /* 'GUID_Embedded_Guid_Control_1D:1917' Guid_ana.p013 = 3.0*guidcmds.intz_FOV; */
  /* 'GUID_Embedded_Guid_Control_1D:1918' Guid_ana.p014 = RrefL(1); */
  /* 'GUID_Embedded_Guid_Control_1D:1919' Guid_ana.p015 = RrefL(2); */
  /* 'GUID_Embedded_Guid_Control_1D:1920' Guid_ana.p016 = RrefL(3); */
  /* 'GUID_Embedded_Guid_Control_1D:1921' Guid_ana.p017 = RrefC(1); */
  /* 'GUID_Embedded_Guid_Control_1D:1922' Guid_ana.p018 = RrefC(2); */
  /* 'GUID_Embedded_Guid_Control_1D:1923' Guid_ana.p019 = RrefC(3); */
  /* 'GUID_Embedded_Guid_Control_1D:1924' Guid_ana.p020 = clos.ArcState.Rarc_T; */
  /* 'GUID_Embedded_Guid_Control_1D:1946' Guid_ana.p021 = guidcmds.AZphi; */
  /* 'GUID_Embedded_Guid_Control_1D:1947' Guid_ana.p022 = t_threshold; */
  /* 'GUID_Embedded_Guid_Control_1D:1948' Guid_ana.p023 = t_noGNSS; */
  /* 'GUID_Embedded_Guid_Control_1D:1949' Guid_ana.p024 = guidcmds.phic_hold; */
  /* 'GUID_Embedded_Guid_Control_1D:1950' Guid_ana.p025 = guidcmds.phi_int; */
  /* 'GUID_Embedded_Guid_Control_1D:1951' Guid_ana.p026 = guidcmds.phi_gic; */
  /* 'GUID_Embedded_Guid_Control_1D:1952' Guid_ana.p027 = NavSolution_in.att_bn.phi; */
  /* 'GUID_Embedded_Guid_Control_1D:1953' Guid_ana.p028 = guidcmds.phic_prev; */
  /* 'GUID_Embedded_Guid_Control_1D:1955' Guid_ana.p029 = guidcmds.phim; */
  /* 'GUID_Embedded_Guid_Control_1D:1956' Guid_ana.p030 = double(fromDATA.AirframeMode); */
  /* 'GUID_Embedded_Guid_Control_1D:1957' Guid_ana.p031 = guidcmds.GndRge; */
  /* 'GUID_Embedded_Guid_Control_1D:1958' Guid_ana.p032 = TRACK.Lfpath2tgt; */
  /* 'GUID_Embedded_Guid_Control_1D:1959' Guid_ana.p033 = TRACK.Roff_intg; */
  /* 'GUID_Embedded_Guid_Control_1D:1960' Guid_ana.p034 = clos.Roffset; */
  /* 'GUID_Embedded_Guid_Control_1D:1961' Guid_ana.p035 = guidcmds.giyc; */
  /* 'GUID_Embedded_Guid_Control_1D:1962' Guid_ana.p036 = guidcmds.gizc; */
  /* 'GUID_Embedded_Guid_Control_1D:1963' Guid_ana.p037 = guidcmds.gb_ratio; */
  /* 'GUID_Embedded_Guid_Control_1D:1964' Guid_ana.p038 = guidcmds.giyc_int; */
  /* 'GUID_Embedded_Guid_Control_1D:1965' Guid_ana.p039 = TRACK.Ut0(3); */
  /* 'GUID_Embedded_Guid_Control_1D:1966' Guid_ana.p040 = TRACK.Ut(3); */
  *rtY_guid2AP = Guidance_100Hz_Function_1D_M->dwork.guid2AP;

  /* Outport: '<Root>/guidCtrlFlags' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  *rtY_guidCtrlFlags = Guidance_100Hz_Function_1D_M->dwork.guidFLAGS;

  /* Outport: '<Root>/guid2DATA' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  *rtY_guid2DATA = Guidance_100Hz_Function_1D_M->dwork.guid2DATA;

  /* Outport: '<Root>/guidtel_100Hz' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  *rtY_guidtel_100Hz = Guidance_100Hz_Function_1D_M->dwork.guidtel100Hz;

  /* Outport: '<Root>/guidtel_20HzA' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  *rtY_guidtel_20HzA = Guidance_100Hz_Function_1D_M->dwork.guidtel20HzA;

  /* Outport: '<Root>/guidtel_20HzB' incorporates:
   *  MATLAB Function: '<S1>/Guidance Controller'
   */
  *rtY_guidtel_20HzB = Guidance_100Hz_Function_1D_M->dwork.guidtel20HzB;

  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_guid_at_outport_1' */
}

/* Model initialize function */
void Guidance_100Hz_Function_1D_initialize(RT_MODEL *const
  Guidance_100Hz_Function_1D_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_guid_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Guidance'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/Guidance Controller' */
  Guidance_100Hz_Function_1D_M->dwork.t_debug = 1030.0;
  Guidance_100Hz_Function_1D_M->dwork.wingsopen_timeout = 1000.0;

  /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_guid_at_outport_1' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
