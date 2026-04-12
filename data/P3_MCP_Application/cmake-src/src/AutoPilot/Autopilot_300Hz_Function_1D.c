/*
 * File: Autopilot_300Hz_Function_1D.c
 *
 * Code generated for Simulink model 'Autopilot_300Hz_Function_1D'.
 *
 * Model version                  : 1.2104
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:35:53 2023
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

#include "Autopilot_300Hz_Function_1D.h"
#include "interpn_s1jYV2bZ.h"
#include "rt_roundd_snf.h"

/* Model step function */
void Autopilot_300Hz_Function_main(RT_MODEL *const Autopilot_300Hz_Function_1D_M,
  T_NAV_InertialMeasurementUnitAutopilot *rtU_IMU300Hzin, T_NAV_NavigationState *
  rtU_navstatein, T_GUID_Guid_Out_IF1003 *rtU_guidin, T_Weapon_MCP_DATA_Autop
  *rtU_DATAautop, T_Weapon_MCP_Out_IF103 *rtY_deflcmd, T_GUID_Autop_Tel300Hz
  *rtY_autoptel_300Hz, T_GUID_Autop_Tel20Hz *rtY_autoptel_20Hz)
{
  uint8_T AF_ID;
  real_T diff[3];
  real_T numNAN1;
  real_T numNAN2;
  real_T numNAN3;
  real_T dummy;
  real_T y[3];
  int32_T k;
  int32_T b_k;
  boolean_T b_b[9];
  boolean_T c_b[6];
  real_T MachVecR[6];
  real_T AlphaVecR[5];
  real_T MachVecP[6];
  real_T AlphaVecP[5];
  real_T MachVecY[6];
  real_T AlphaVecY[5];
  real_T machR;
  real_T machP;
  real_T SigmaP;
  real_T AlphaP;
  real_T BetaP;
  real_T den;
  real_T P2d_sigma;
  real_T Z2d_sigma;
  real_T P2d_alpha;
  real_T Z2d_alpha;
  real_T sigma_in;
  real_T alpha_in;
  real_T P2d_lead;
  real_T Z2d_lead;
  real_T Dcmd2;
  real_T Dcmd3;
  real_T dFin1;
  real_T dFin2;
  real_T dFin3;
  real_T dFin4;
  uint8_T rtb_reset;
  uint32_T rtb_FixPtSum1;
  real_T rtb_sigma;
  real_T tmp[30];
  real_T AF_ID_0[9];
  uint16_T tmp_0;
  boolean_T exitg1;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_release_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Autopilot'
   */
  /* MATLAB Function: '<S1>/Autopilot Input Conditioning' incorporates:
   *  Inport: '<Root>/DATAautop'
   *  Inport: '<Root>/guidin'
   *  Inport: '<Root>/nav100Hzin'
   *  Inport: '<Root>/nav300Hzin'
   */
  /* :  if (isempty(countNaNin)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.countNaNin_not_empty) {
    /* :  countNaNin = uint16(0); */
    Autopilot_300Hz_Function_1D_M->dwork.countNaNin_not_empty = true;

    /* :  LOOP       = 0.0; */
    /* :  MACH       = 0.8; */
    /* :  QRATIO     = 1.0; */
    /* :  ALPHA      = 0.0; */
    /* :  BETA       = 0.0; */
    /* :  PHIC       = 0.0; */
    /* :  GBZC       = 0.0; */
    /* :  GBYC       = 0.0; */
    /* :  PHIM       = 0.0; */
    /* :  GBZM       = 0.0; */
    /* :  GBYM       = 0.0; */
    /* :  WBXM       = 0.0; */
    /* :  WBYM       = 0.0; */
    /* :  WBZM       = 0.0; */
    /* :  AZfac      = exp(-autop_prv.delt/2.0); */
    Autopilot_300Hz_Function_1D_M->dwork.AZfac = -0.0016666666666666668;
    Autopilot_300Hz_Function_1D_M->dwork.AZfac = exp
      (Autopilot_300Hz_Function_1D_M->dwork.AZfac);
  }

  /* :  if (fromDATA.AirframeID < 1) */
  if (((int32_T)rtU_DATAautop->AirframeID) < 1) {
    /* :  AF_ID = uint8(1); */
    AF_ID = 1U;
  } else {
    /* :  else */
    /* :  AF_ID = uint8(fromDATA.AirframeID); */
    AF_ID = rtU_DATAautop->AirframeID;
  }

  /* :  reset = uint8(0); */
  rtb_reset = 0U;

  /* :  if (guid_in.AP_Loop == 0) */
  if (((int32_T)rtU_guidin->AP_Loop) == 0) {
    /* :  reset = uint8(1); */
    rtb_reset = 1U;
  }

  /* :  LOOP   = LIB_checkNAN(double(guid_in.AP_Loop),LOOP,1,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  /* 'LIB_checkNAN:13' else */
  /* 'LIB_checkNAN:14' state = in; */
  /* 'LIB_checkNAN:15' out = state; */
  /* :  MACH   = LIB_checkNAN(double(guid_in.mach),MACH,1,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (!rtIsNaN(rtU_guidin->mach)) {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.MACH = rtU_guidin->mach;
  } else {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:10' else */
    /* 'LIB_checkNAN:11' out = state; */
  }

  /* :  QRATIO = LIB_checkNAN(double(guid_in.Qratio),QRATIO,1,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (!rtIsNaN(rtU_guidin->Qratio)) {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.QRATIO = rtU_guidin->Qratio;
  } else {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:10' else */
    /* 'LIB_checkNAN:11' out = state; */
  }

  /* :  ALPHA  = LIB_checkNAN(double(guid_in.alpha)*cnsts.r2d,ALPHA,2,AZfac); */
  P2d_lead = rtU_guidin->alpha * 57.295779513082323;

  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(P2d_lead)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.ALPHA *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.ALPHA = P2d_lead;
  }

  /* :  BETA   = LIB_checkNAN(double(guid_in.beta)*cnsts.r2d,BETA,2,AZfac); */
  Z2d_lead = rtU_guidin->beta * 57.295779513082323;

  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(Z2d_lead)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.BETA *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.BETA = Z2d_lead;
  }

  /* :  sigma  = sqrt(ALPHA^2+BETA^2); */
  /* :  PHIC   = LIB_checkNAN(double(guid_in.phic),PHIC,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->phic)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.PHIC *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.PHIC = rtU_guidin->phic;
  }

  /* :  GBZC   = LIB_checkNAN(double(guid_in.gbzc),GBZC,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->gbzc)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBZC *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBZC = rtU_guidin->gbzc;
  }

  /* :  GBYC   = LIB_checkNAN(double(guid_in.gbyc),GBYC,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->gbyc)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBYC *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBYC = rtU_guidin->gbyc;
  }

  /* :  PHIM   = LIB_checkNAN(double(guid_in.phim),PHIM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->phim)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.PHIM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.PHIM = rtU_guidin->phim;
  }

  /* :  diff = zeros(3,1); */
  /* :  diff(1) = PHIC-2*pi-PHIM; */
  diff[0] = (Autopilot_300Hz_Function_1D_M->dwork.PHIC - 6.2831853071795862) -
    Autopilot_300Hz_Function_1D_M->dwork.PHIM;

  /* :  diff(2) = PHIC     -PHIM; */
  diff[1] = Autopilot_300Hz_Function_1D_M->dwork.PHIC -
    Autopilot_300Hz_Function_1D_M->dwork.PHIM;

  /* :  diff(3) = PHIC+2*pi-PHIM; */
  diff[2] = (Autopilot_300Hz_Function_1D_M->dwork.PHIC + 6.2831853071795862) -
    Autopilot_300Hz_Function_1D_M->dwork.PHIM;

  /* :  [val,ind] = min(abs(diff)); */
  y[0] = fabs(diff[0]);
  y[1] = fabs(diff[1]);
  y[2] = fabs(diff[2]);
  if (!rtIsNaN(y[0])) {
    k = 0;
  } else {
    k = -1;
    b_k = 2;
    exitg1 = false;
    while ((!exitg1) && (b_k < 4)) {
      if (!rtIsNaN(y[b_k - 1])) {
        k = b_k - 1;
        exitg1 = true;
      } else {
        b_k++;
      }
    }
  }

  if ((k + 1) == 0) {
    k = 0;
  } else {
    numNAN1 = y[k];
    for (b_k = k + 1; (b_k + 1) < 4; b_k++) {
      if (numNAN1 > y[b_k]) {
        numNAN1 = y[b_k];
        k = b_k;
      }
    }
  }

  /* :  PHIC = PHIM + diff(ind); */
  Autopilot_300Hz_Function_1D_M->dwork.PHIC =
    Autopilot_300Hz_Function_1D_M->dwork.PHIM + diff[k];

  /* :  GBZM   = LIB_checkNAN(nav300Hz_in.ab.z,GBZM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_IMU300Hzin->ab.z)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBZM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBZM = rtU_IMU300Hzin->ab.z;
  }

  /* :  GBYM   = LIB_checkNAN(nav300Hz_in.ab.y,GBYM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_IMU300Hzin->ab.y)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBYM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.GBYM = rtU_IMU300Hzin->ab.y;
  }

  /* :  WBXM   = LIB_checkNAN(nav300Hz_in.wb.x,WBXM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_IMU300Hzin->wb.x)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBXM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBXM = rtU_IMU300Hzin->wb.x;
  }

  /* :  WBYM   = LIB_checkNAN(nav300Hz_in.wb.y,WBYM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_IMU300Hzin->wb.y)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBYM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBYM = rtU_IMU300Hzin->wb.y;
  }

  /* :  WBZM   = LIB_checkNAN(nav300Hz_in.wb.z,WBZM,2,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_IMU300Hzin->wb.z)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:7' elseif (typehold == 2) */
    /* 'LIB_checkNAN:8' state = state*AZfac; */
    /* 'LIB_checkNAN:9' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBZM *=
      Autopilot_300Hz_Function_1D_M->dwork.AZfac;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    Autopilot_300Hz_Function_1D_M->dwork.WBZM = rtU_IMU300Hzin->wb.z;
  }

  /* :  numNAN1 = sum(isnan([guid_in.dFin1 guid_in.dFin2 guid_in.dFin3 guid_in.dFin4])); */
  /* :  numNAN2 = sum(isnan([double(AF_ID) double(guid_in.AP_Loop) guid_in.mach guid_in.Qratio guid_in.alpha guid_in.beta guid_in.phic guid_in.gbzc guid_in.gbyc])); */
  AF_ID_0[0] = (real_T)AF_ID;
  AF_ID_0[1] = (real_T)rtU_guidin->AP_Loop;
  AF_ID_0[2] = rtU_guidin->mach;
  AF_ID_0[3] = rtU_guidin->Qratio;
  AF_ID_0[4] = rtU_guidin->alpha;
  AF_ID_0[5] = rtU_guidin->beta;
  AF_ID_0[6] = rtU_guidin->phic;
  AF_ID_0[7] = rtU_guidin->gbzc;
  AF_ID_0[8] = rtU_guidin->gbyc;
  for (k = 0; k < 9; k++) {
    b_b[k] = rtIsNaN(AF_ID_0[k]);
  }

  numNAN2 = 0.0;
  for (k = 0; k < 8; k++) {
    numNAN2 += (real_T)(b_b[k + 1] ? 1.0 : 0.0);
  }

  /* :  numNAN3 = sum(isnan([nav100Hz_in.att_bn.phi nav300Hz_in.ab.z nav300Hz_in.ab.y nav300Hz_in.wb.x nav300Hz_in.wb.y nav300Hz_in.wb.z])); */
  MachVecR[0] = rtU_navstatein->att_bn.phi;
  MachVecR[1] = rtU_IMU300Hzin->ab.z;
  MachVecR[2] = rtU_IMU300Hzin->ab.y;
  MachVecR[3] = rtU_IMU300Hzin->wb.x;
  MachVecR[4] = rtU_IMU300Hzin->wb.y;
  MachVecR[5] = rtU_IMU300Hzin->wb.z;
  for (k = 0; k < 6; k++) {
    c_b[k] = rtIsNaN(MachVecR[k]);
  }

  numNAN3 = c_b[0] ? 1.0 : 0.0;
  for (k = 0; k < 5; k++) {
    numNAN3 += (real_T)(c_b[k + 1] ? 1.0 : 0.0);
  }

  /* :  countNaNin = countNaNin + uint16(numNAN1+numNAN2+numNAN3); */
  P2d_lead = ((((((real_T)(rtIsNaN(rtU_guidin->dFin1) ? 1.0 : 0.0)) + ((real_T)
    (rtIsNaN(rtU_guidin->dFin2) ? 1.0 : 0.0))) + ((real_T)(rtIsNaN
    (rtU_guidin->dFin3) ? 1.0 : 0.0))) + ((real_T)(rtIsNaN(rtU_guidin->dFin4) ?
    1.0 : 0.0))) + numNAN2) + numNAN3;
  if (P2d_lead < 65536.0) {
    tmp_0 = (uint16_T)P2d_lead;
  } else {
    tmp_0 = MAX_uint16_T;
  }

  rtb_FixPtSum1 = ((uint32_T)Autopilot_300Hz_Function_1D_M->dwork.countNaNin) +
    ((uint32_T)tmp_0);
  if (rtb_FixPtSum1 > 65535U) {
    rtb_FixPtSum1 = 65535U;
  }

  Autopilot_300Hz_Function_1D_M->dwork.countNaNin = (uint16_T)rtb_FixPtSum1;

  /* :  if (countNaNin > 65534) */
  if (((int32_T)Autopilot_300Hz_Function_1D_M->dwork.countNaNin) > 65534) {
    /* :  countNaNin = uint16(0); */
    Autopilot_300Hz_Function_1D_M->dwork.countNaNin = 0U;
  }

  /* :  APloop    = uint8(LOOP); */
  /* :  mach      = MACH; */
  /* :  Qratio    = QRATIO; */
  /* :  alpha     = ALPHA; */
  /* :  beta      = BETA; */
  /* :  phic      = PHIC; */
  /* :  gbzc      = GBZC; */
  /* :  gbyc      = GBYC; */
  /* :  phim      = PHIM; */
  /* :  gbzm      = GBZM; */
  /* :  gbym      = GBYM; */
  /* :  wbxm      = WBXM; */
  /* :  wbym      = WBYM; */
  /* :  wbzm      = WBZM; */
  /* :  cntNANin  = countNaNin; */
  rtb_sigma = sqrt((Autopilot_300Hz_Function_1D_M->dwork.ALPHA *
                    Autopilot_300Hz_Function_1D_M->dwork.ALPHA) +
                   (Autopilot_300Hz_Function_1D_M->dwork.BETA *
                    Autopilot_300Hz_Function_1D_M->dwork.BETA));

  /* MATLAB Function: '<S1>/Activate Fins' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  /* :  out = double(bitget(uint8(in),8)); */
  numNAN1 = (real_T)(((((uint32_T)rtU_guidin->AP_Loop) & 128U) != 0U) ? 1 : 0);

  /* MATLAB Function: '<S1>/Autopilot Gains' incorporates:
   *  Inport: '<Root>/DATAautop'
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Memory: '<S1>/Memory'
   *  Outport: '<Root>/autoptel_20Hz'
   */
  /* :  if (isempty(cnt)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.cnt_not_empty) {
    /* :  cnt = uint16(0); */
    Autopilot_300Hz_Function_1D_M->dwork.cnt_not_empty = true;

    /* :  K = double(2.0/fromDATA.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_i = (real_T)((real32_T)(2.0F /
      rtU_DATAautop->t_delt));

    /* :  sigma_prev = 0.0; */
    /* :  alpha_prev = 0.0; */
    /* :  beta_prev = 0.0; */
    /* :  sigma_filt_prev = 0.0; */
    /* :  alpha_filt_prev = 0.0; */
    /* :  beta_filt_prev = 0.0; */
    /* :  AP.t_delt  = double(fromDATA.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt = (real_T)
      rtU_DATAautop->t_delt;

    /* :  AP.Kp_phi  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_phi = 0.0;

    /* :  AP.Ki_phi  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_phi = 0.0;

    /* :  AP.Kp_wbx  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbx = 0.0;

    /* :  AP.NCProll = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll = 0.0;

    /* :  AP.RPlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ = 0.0;

    /* :  AP.RPlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP = 0.0;

    /* :  AP.NCWroll = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll = 0.0;

    /* :  AP.RRleadZ = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ = 0.0;

    /* :  AP.RRleadP = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP = 0.0;

    /* :  AP.RRlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ = 0.0;

    /* :  AP.RRlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP = 0.0;

    /* :  AP.Kp_gbz   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gbz = 0.0;

    /* :  AP.Ki_wby   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wby = 0.0;

    /* :  AP.Kp_wby   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby = 0.0;

    /* :  AP.NCApitch = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch = 0.0;

    /* :  AP.LZAlowWN = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN = 0.0;

    /* :  AP.LZAleadZ = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ = 0.0;

    /* :  AP.LZAleadP = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP = 0.0;

    /* :  AP.LZAlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ = 0.0;

    /* :  AP.LZAlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP = 0.0;

    /* :  AP.NCWpitch = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch = 0.0;

    /* :  AP.LZWleadZ = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ = 0.0;

    /* :  AP.LZWleadP = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP = 0.0;

    /* :  AP.LZWlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ = 0.0;

    /* :  AP.LZWlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP = 0.0;

    /* :  AP.Kp_gby   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gby = 0.0;

    /* :  AP.Ki_wbz   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wbz = 0.0;

    /* :  AP.Kp_wbz   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz = 0.0;

    /* :  AP.NCAyaw   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw = 0.0;

    /* :  AP.LYAlowWN = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN = 0.0;

    /* :  AP.LYAleadZ = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ = 0.0;

    /* :  AP.LYAleadP = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP = 0.0;

    /* :  AP.LYAlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ = 0.0;

    /* :  AP.LYAlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP = 0.0;

    /* :  AP.NCWyaw   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw = 0.0;

    /* :  AP.LYWleadZ = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ = 0.0;

    /* :  AP.LYWleadP = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP = 0.0;

    /* :  AP.LYWlagZ  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ = 0.0;

    /* :  AP.LYWlagP  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP = 0.0;
  }

  /* :  cnt = cnt  + 1; */
  rtb_FixPtSum1 = ((uint32_T)Autopilot_300Hz_Function_1D_M->dwork.cnt) + 1U;
  if (rtb_FixPtSum1 > 65535U) {
    rtb_FixPtSum1 = 65535U;
  }

  Autopilot_300Hz_Function_1D_M->dwork.cnt = (uint16_T)rtb_FixPtSum1;

  /* :  if (cnt > 65534) */
  if (((int32_T)Autopilot_300Hz_Function_1D_M->dwork.cnt) > 65534) {
    /* :  cnt = uint16(0); */
    Autopilot_300Hz_Function_1D_M->dwork.cnt = 0U;
  }

  /* :  if (Airframe_ID < 1) */
  if (((int32_T)AF_ID) < 1) {
    /* :  Airframe_ID = uint8(1); */
    AF_ID = 1U;
  }

  /* :  if (Airframe_ID > 30) */
  if (((int32_T)AF_ID) > 30) {
    /* :  Airframe_ID = uint8(30); */
    AF_ID = 30U;
  }

  /* :  if (mach > 0.4 && tgtrge < 13000.0) */
  /* :  MachVecR  = double(fromDATA.gainsR.MachVec); */
  for (k = 0; k < 6; k++) {
    MachVecR[k] = (real_T)rtU_DATAautop->gainsR.MachVec[k];
  }

  /* :  AlphaVecR = double(fromDATA.gainsR.AlphaVec); */
  for (k = 0; k < 5; k++) {
    AlphaVecR[k] = (real_T)rtU_DATAautop->gainsR.AlphaVec[k];
  }

  /* :  MachVecP  = double(fromDATA.gainsP.MachVec); */
  for (k = 0; k < 6; k++) {
    MachVecP[k] = (real_T)rtU_DATAautop->gainsP.MachVec[k];
  }

  /* :  AlphaVecP = double(fromDATA.gainsP.AlphaVec); */
  for (k = 0; k < 5; k++) {
    AlphaVecP[k] = (real_T)rtU_DATAautop->gainsP.AlphaVec[k];
  }

  /* :  MachVecY  = double(fromDATA.gainsY.MachVec); */
  for (k = 0; k < 6; k++) {
    MachVecY[k] = (real_T)rtU_DATAautop->gainsY.MachVec[k];
  }

  /* :  AlphaVecY = double(fromDATA.gainsY.AlphaVec); */
  for (k = 0; k < 5; k++) {
    AlphaVecY[k] = (real_T)rtU_DATAautop->gainsY.AlphaVec[k];
  }

  /* :  len = length(MachVecR); */
  /* :  if (mach > MachVecR(len)) */
  if (Autopilot_300Hz_Function_1D_M->dwork.MACH > MachVecR[5]) {
    /* :  machR = MachVecR(len); */
    machR = MachVecR[5];
  } else {
    /* :  else */
    /* :  machR = mach; */
    machR = Autopilot_300Hz_Function_1D_M->dwork.MACH;
  }

  /* :  len = length(MachVecP); */
  /* :  if (mach > MachVecP(len)) */
  if (Autopilot_300Hz_Function_1D_M->dwork.MACH > MachVecP[5]) {
    /* :  machP = MachVecP(len); */
    machP = MachVecP[5];
  } else {
    /* :  else */
    /* :  machP = mach; */
    machP = Autopilot_300Hz_Function_1D_M->dwork.MACH;
  }

  /* :  len = length(MachVecY); */
  /* :  if (mach > MachVecY(len)) */
  if (Autopilot_300Hz_Function_1D_M->dwork.MACH > MachVecY[5]) {
    /* :  machY = MachVecY(len); */
    numNAN3 = MachVecY[5];
  } else {
    /* :  else */
    /* :  machY = mach; */
    numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.MACH;
  }

  /* :  SigmaP  = LIB_limit(0.001,1000.0,double(fromDATA.gainsR.SIGMAfilt)); */
  /* 'LIB_limit:27' if (in > max) */
  if (rtU_DATAautop->gainsR.SIGMAfilt > 1000.0F) {
    /* 'LIB_limit:28' out = max; */
    SigmaP = 1000.0;
  } else if (((real_T)rtU_DATAautop->gainsR.SIGMAfilt) < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    SigmaP = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    SigmaP = (real_T)rtU_DATAautop->gainsR.SIGMAfilt;
  }

  /* :  AlphaP  = LIB_limit(0.001,1000.0,double(fromDATA.gainsP.ALPHAfilt)); */
  /* 'LIB_limit:27' if (in > max) */
  if (rtU_DATAautop->gainsP.ALPHAfilt > 1000.0F) {
    /* 'LIB_limit:28' out = max; */
    AlphaP = 1000.0;
  } else if (((real_T)rtU_DATAautop->gainsP.ALPHAfilt) < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    AlphaP = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    AlphaP = (real_T)rtU_DATAautop->gainsP.ALPHAfilt;
  }

  /* :  BetaP   = LIB_limit(0.001,1000.0,double(fromDATA.gainsY.ALPHAfilt)); */
  /* 'LIB_limit:27' if (in > max) */
  if (rtU_DATAautop->gainsY.ALPHAfilt > 1000.0F) {
    /* 'LIB_limit:28' out = max; */
    BetaP = 1000.0;
  } else if (((real_T)rtU_DATAautop->gainsY.ALPHAfilt) < 0.001) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    BetaP = 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    BetaP = (real_T)rtU_DATAautop->gainsY.ALPHAfilt;
  }

  /* :  den = K + SigmaP; */
  den = Autopilot_300Hz_Function_1D_M->dwork.K_i + SigmaP;

  /* :  P1d_sigma = SigmaP/den; */
  numNAN2 = SigmaP / den;

  /* :  P2d_sigma = SigmaP/den; */
  /* :  Z2d_sigma = (-K + SigmaP)/den; */
  Z2d_sigma = ((-Autopilot_300Hz_Function_1D_M->dwork.K_i) + SigmaP) / den;

  /* :  den = K + AlphaP; */
  den = Autopilot_300Hz_Function_1D_M->dwork.K_i + AlphaP;

  /* :  P1d_alpha = AlphaP/den; */
  SigmaP = AlphaP / den;

  /* :  P2d_alpha = AlphaP/den; */
  /* :  Z2d_alpha = (-K + AlphaP)/den; */
  Z2d_alpha = ((-Autopilot_300Hz_Function_1D_M->dwork.K_i) + AlphaP) / den;

  /* :  den = K + BetaP; */
  den = Autopilot_300Hz_Function_1D_M->dwork.K_i + BetaP;

  /* :  P1d_beta = BetaP/den; */
  /* :  P2d_beta = BetaP/den; */
  /* :  Z2d_beta = (-K + BetaP)/den; */
  /* :  if (bitget(uint8(APloop),7)) */
  if ((((uint32_T)rtU_guidin->AP_Loop) & 64U) != 0U) {
    /* :  sigma_in = sigma; */
    sigma_in = rtb_sigma;

    /* :  alpha_in = alpha; */
    alpha_in = Autopilot_300Hz_Function_1D_M->dwork.ALPHA;

    /* :  beta_in  = beta; */
    AlphaP = Autopilot_300Hz_Function_1D_M->dwork.BETA;
  } else {
    /* :  else */
    /* :  sigma_in = 0.0; */
    sigma_in = 0.0;

    /* :  alpha_in = 0.0; */
    alpha_in = 0.0;

    /* :  beta_in  = 0.0; */
    AlphaP = 0.0;
  }

  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  sigma_filt      = 0.0; */
    numNAN2 = 0.0;

    /* :  sigma_filt_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.sigma_filt_prev = 0.0;

    /* :  sigma_prev      = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.sigma_prev = 0.0;

    /* :  alpha_filt      = 0.0; */
    P2d_sigma = 0.0;

    /* :  alpha_filt_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.alpha_filt_prev = 0.0;

    /* :  alpha_prev      = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.alpha_prev = 0.0;

    /* :  beta_filt       = 0.0; */
    BetaP = 0.0;

    /* :  beta_filt_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.beta_filt_prev = 0.0;

    /* :  beta_prev       = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.beta_prev = 0.0;
  } else {
    /* :  else */
    /* :  sigma_filt = P1d_sigma*sigma_in + P2d_sigma*sigma_prev - Z2d_sigma*sigma_filt_prev; */
    numNAN2 = ((numNAN2 * sigma_in) + (numNAN2 *
                Autopilot_300Hz_Function_1D_M->dwork.sigma_prev)) - (Z2d_sigma *
      Autopilot_300Hz_Function_1D_M->dwork.sigma_filt_prev);

    /* :  sigma_filt_prev = sigma_filt; */
    Autopilot_300Hz_Function_1D_M->dwork.sigma_filt_prev = numNAN2;

    /* :  sigma_prev      = sigma_in; */
    Autopilot_300Hz_Function_1D_M->dwork.sigma_prev = sigma_in;

    /* :  alpha_filt = P1d_alpha*alpha_in + P2d_alpha*alpha_prev - Z2d_alpha*alpha_filt_prev; */
    P2d_sigma = ((SigmaP * alpha_in) + (SigmaP *
      Autopilot_300Hz_Function_1D_M->dwork.alpha_prev)) - (Z2d_alpha *
      Autopilot_300Hz_Function_1D_M->dwork.alpha_filt_prev);

    /* :  alpha_filt_prev = alpha_filt; */
    Autopilot_300Hz_Function_1D_M->dwork.alpha_filt_prev = P2d_sigma;

    /* :  alpha_prev      = alpha_in; */
    Autopilot_300Hz_Function_1D_M->dwork.alpha_prev = alpha_in;

    /* :  beta_filt  = P1d_beta*beta_in + P2d_beta*beta_prev - Z2d_beta*beta_filt_prev; */
    Z2d_alpha = BetaP / den;
    BetaP = ((Z2d_alpha * AlphaP) + (Z2d_alpha *
              Autopilot_300Hz_Function_1D_M->dwork.beta_prev)) -
      ((((-Autopilot_300Hz_Function_1D_M->dwork.K_i) + BetaP) / den) *
       Autopilot_300Hz_Function_1D_M->dwork.beta_filt_prev);

    /* :  beta_filt_prev  = beta_filt; */
    Autopilot_300Hz_Function_1D_M->dwork.beta_filt_prev = BetaP;

    /* :  beta_prev       = beta_in; */
    Autopilot_300Hz_Function_1D_M->dwork.beta_prev = AlphaP;
  }

  /* :  Kterm = LIB_limit(1.0,1.5,(2500-tgtrge)/1000); */
  AlphaP = (2500.0 - rtU_guidin->tgtrge) / 1000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (AlphaP > 1.5) {
    /* 'LIB_limit:28' out = max; */
    AlphaP = 1.5;
  } else if (AlphaP < 1.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    AlphaP = 1.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* :  sigma      = LIB_limit(AlphaVecR(1),AlphaVecR(end),sigma); */
  /* 'LIB_limit:27' if (in > max) */
  if (rtb_sigma > AlphaVecR[4]) {
    /* 'LIB_limit:28' out = max; */
    rtb_sigma = AlphaVecR[4];
  } else if (rtb_sigma < AlphaVecR[0]) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    rtb_sigma = AlphaVecR[0];
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* :  sigma_filt = LIB_limit(AlphaVecR(1),AlphaVecR(end),sigma_filt); */
  /* 'LIB_limit:27' if (in > max) */
  if (numNAN2 > AlphaVecR[4]) {
    /* 'LIB_limit:28' out = max; */
    numNAN2 = AlphaVecR[4];
  } else if (numNAN2 < AlphaVecR[0]) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN2 = AlphaVecR[0];
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* :  if (AlphaVecP(1) < 0.0) */
  if (AlphaVecP[0] < 0.0) {
    /* :  alpha      = LIB_limit(AlphaVecP(1),AlphaVecP(end),alpha); */
    /* 'LIB_limit:27' if (in > max) */
    if (Autopilot_300Hz_Function_1D_M->dwork.ALPHA > AlphaVecP[4]) {
      /* 'LIB_limit:28' out = max; */
      alpha_in = AlphaVecP[4];
    } else if (Autopilot_300Hz_Function_1D_M->dwork.ALPHA < AlphaVecP[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      alpha_in = AlphaVecP[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      alpha_in = Autopilot_300Hz_Function_1D_M->dwork.ALPHA;
    }

    /* :  alpha_filt = LIB_limit(AlphaVecP(1),AlphaVecP(end),alpha_filt); */
    /* 'LIB_limit:27' if (in > max) */
    if (P2d_sigma > AlphaVecP[4]) {
      /* 'LIB_limit:28' out = max; */
      P2d_sigma = AlphaVecP[4];
    } else if (P2d_sigma < AlphaVecP[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      P2d_sigma = AlphaVecP[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  } else {
    /* :  else */
    /* :  alpha      = LIB_limit(AlphaVecP(1),AlphaVecP(end),abs(alpha)); */
    alpha_in = fabs(Autopilot_300Hz_Function_1D_M->dwork.ALPHA);

    /* 'LIB_limit:27' if (in > max) */
    if (alpha_in > AlphaVecP[4]) {
      /* 'LIB_limit:28' out = max; */
      alpha_in = AlphaVecP[4];
    } else if (alpha_in < AlphaVecP[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      alpha_in = AlphaVecP[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* :  alpha_filt = LIB_limit(AlphaVecP(1),AlphaVecP(end),abs(alpha_filt)); */
    P2d_sigma = fabs(P2d_sigma);

    /* 'LIB_limit:27' if (in > max) */
    if (P2d_sigma > AlphaVecP[4]) {
      /* 'LIB_limit:28' out = max; */
      P2d_sigma = AlphaVecP[4];
    } else if (P2d_sigma < AlphaVecP[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      P2d_sigma = AlphaVecP[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  }

  /* :  if (AlphaVecY(1) < 0.0) */
  if (AlphaVecY[0] < 0.0) {
    /* :  beta       = LIB_limit(AlphaVecY(1),AlphaVecY(end),beta); */
    /* 'LIB_limit:27' if (in > max) */
    if (Autopilot_300Hz_Function_1D_M->dwork.BETA > AlphaVecY[4]) {
      /* 'LIB_limit:28' out = max; */
      SigmaP = AlphaVecY[4];
    } else if (Autopilot_300Hz_Function_1D_M->dwork.BETA < AlphaVecY[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      SigmaP = AlphaVecY[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      SigmaP = Autopilot_300Hz_Function_1D_M->dwork.BETA;
    }

    /* :  beta_filt  = LIB_limit(AlphaVecY(1),AlphaVecY(end),beta_filt); */
    /* 'LIB_limit:27' if (in > max) */
    if (BetaP > AlphaVecY[4]) {
      /* 'LIB_limit:28' out = max; */
      BetaP = AlphaVecY[4];
    } else if (BetaP < AlphaVecY[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      BetaP = AlphaVecY[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  } else {
    /* :  else */
    /* :  beta       = LIB_limit(AlphaVecY(1),AlphaVecY(end),abs(beta)); */
    SigmaP = fabs(Autopilot_300Hz_Function_1D_M->dwork.BETA);

    /* 'LIB_limit:27' if (in > max) */
    if (SigmaP > AlphaVecY[4]) {
      /* 'LIB_limit:28' out = max; */
      SigmaP = AlphaVecY[4];
    } else if (SigmaP < AlphaVecY[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      SigmaP = AlphaVecY[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* :  beta_filt  = LIB_limit(AlphaVecY(1),AlphaVecY(end),abs(beta_filt)); */
    BetaP = fabs(BetaP);

    /* 'LIB_limit:27' if (in > max) */
    if (BetaP > AlphaVecY[4]) {
      /* 'LIB_limit:28' out = max; */
      BetaP = AlphaVecY[4];
    } else if (BetaP < AlphaVecY[0]) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      BetaP = AlphaVecY[0];
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  }

  /* :  AP.Kp_phi  = interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.Kp_phi),machR,sigma); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.Kp_phi[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_phi = interpn_s1jYV2bZ(MachVecR,
    AlphaVecR, tmp, machR, rtb_sigma);

  /* :  AP.Ki_phi  = interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.Ki_phi),machR,sigma); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.Ki_phi[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_phi = interpn_s1jYV2bZ(MachVecR,
    AlphaVecR, tmp, machR, rtb_sigma);

  /* :  AP.Kp_wbx  = interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.Kp_wbx),machR,sigma); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.Kp_wbx[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbx = interpn_s1jYV2bZ(MachVecR,
    AlphaVecR, tmp, machR, rtb_sigma);

  /* :  AP.NCProll = LIB_limit(0.0,1.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.NCP),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.NCP[k];
  }

  Z2d_lead = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll = 1.0;
  } else if (Z2d_lead < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll = Z2d_lead;
  }

  /* :  AP.RPlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RPlagZ),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RPlagZ[k];
  }

  Z2d_lead = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ = 160.0;
  } else if (Z2d_lead < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ = Z2d_lead;
  }

  /* :  AP.RPlagP  = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RPlagP),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RPlagP[k];
  }

  Z2d_lead = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP = 160.0;
  } else if (Z2d_lead < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP = Z2d_lead;
  }

  /* :  AP.NCWroll = LIB_limit(0.0,1.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.NCW),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.NCW[k];
  }

  P2d_alpha = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll = 1.0;
  } else if (P2d_alpha < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll = P2d_alpha;
  }

  /* :  AP.RRleadZ = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RRleadZ),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RRleadZ[k];
  }

  P2d_alpha = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ = 160.0;
  } else if (P2d_alpha < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ = P2d_alpha;
  }

  /* :  AP.RRleadP = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RRleadP),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RRleadP[k];
  }

  P2d_alpha = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP = 160.0;
  } else if (P2d_alpha < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP = P2d_alpha;
  }

  /* :  AP.RRlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RRlagZ),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RRlagZ[k];
  }

  P2d_alpha = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ = 160.0;
  } else if (P2d_alpha < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ = P2d_alpha;
  }

  /* :  AP.RRlagP  = LIB_limit(0.000160,160.0,interpn(MachVecR,AlphaVecR,double(fromDATA.gainsR.RRlagP),machR,sigma_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsR.RRlagP[k];
  }

  P2d_alpha = interpn_s1jYV2bZ(MachVecR, AlphaVecR, tmp,
      machR, numNAN2);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP = 160.0;
  } else if (P2d_alpha < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP = P2d_alpha;
  }

  /* :  AP.Kp_gbz   = Kterm*interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.Kpa),machP,alpha); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.Kpa[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gbz = AlphaP * interpn_s1jYV2bZ
    (MachVecP, AlphaVecP, tmp, machP, alpha_in);

  /* :  AP.Ki_wby   = interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.Kiw),machP,alpha); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.Kiw[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wby = interpn_s1jYV2bZ(MachVecP,
    AlphaVecP, tmp, machP, alpha_in);

  /* :  AP.Kp_wby   = interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.Kpw),machP,alpha); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.Kpw[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby = interpn_s1jYV2bZ(MachVecP,
    AlphaVecP, tmp, machP, alpha_in);

  /* :  AP.NCApitch = LIB_limit(0.0,1.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.NCA),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.NCA[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch = 1.0;
  } else if (dummy < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch = dummy;
  }

  /* :  AP.LZAlowWN = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LAlowpass),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LAlowpass[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN = dummy;
  }

  /* :  AP.LZAleadZ = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LAleadZ),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LAleadZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ = dummy;
  }

  /* :  AP.LZAleadP = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LAleadP),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LAleadP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP = dummy;
  }

  /* :  AP.LZAlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LAlagZ),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LAlagZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ = dummy;
  }

  /* :  AP.LZAlagP  = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LAlagP),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LAlagP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP = dummy;
  }

  /* :  AP.NCWpitch = LIB_limit(0.0,1.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.NCA),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.NCA[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch = 1.0;
  } else if (dummy < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch = dummy;
  }

  /* :  AP.LZWleadZ = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LWleadZ),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LWleadZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ = dummy;
  }

  /* :  AP.LZWleadP = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LWleadP),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LWleadP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP = dummy;
  }

  /* :  AP.LZWlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LWlagZ),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LWlagZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ = dummy;
  }

  /* :  AP.LZWlagP  = LIB_limit(0.000160,160.0,interpn(MachVecP,AlphaVecP,double(fromDATA.gainsP.LWlagP),machP,alpha_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsP.LWlagP[k];
  }

  P2d_sigma = interpn_s1jYV2bZ(MachVecP, AlphaVecP, tmp,
      machP, P2d_sigma);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_sigma > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP = 160.0;
  } else if (P2d_sigma < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP = P2d_sigma;
  }

  /* :  AP.Kp_gby   = Kterm*interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.Kpa),machY,beta); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.Kpa[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gby = AlphaP * interpn_s1jYV2bZ
    (MachVecY, AlphaVecY, tmp, numNAN3, SigmaP);

  /* :  AP.Ki_wbz   = interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.Kiw),machY,beta); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.Kiw[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wbz = interpn_s1jYV2bZ(MachVecY,
    AlphaVecY, tmp, numNAN3, SigmaP);

  /* :  AP.Kp_wbz   = interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.Kpw),machY,beta); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.Kpw[k];
  }

  Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz = interpn_s1jYV2bZ(MachVecY,
    AlphaVecY, tmp, numNAN3, SigmaP);

  /* :  AP.NCAyaw   = LIB_limit(0.0,1.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.NCA),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.NCA[k];
  }

  P2d_sigma = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_sigma > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw = 1.0;
  } else if (P2d_sigma < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw = P2d_sigma;
  }

  /* :  AP.LYAlowWN = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LAlowpass),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LAlowpass[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN = dummy;
  }

  /* :  AP.LYAleadZ = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LAleadZ),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LAleadZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ = dummy;
  }

  /* :  AP.LYAleadP = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LAleadP),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LAleadP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP = dummy;
  }

  /* :  AP.LYAlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LAlagZ),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LAlagZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ = dummy;
  }

  /* :  AP.LYAlagP  = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LAlagP),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LAlagP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP = dummy;
  }

  /* :  AP.NCWyaw   = LIB_limit(0.0,1.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.NCA),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.NCA[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 1.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw = 1.0;
  } else if (dummy < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw = dummy;
  }

  /* :  AP.LYWleadZ = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LWleadZ),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LWleadZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ = dummy;
  }

  /* :  AP.LYWleadP = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LWleadP),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LWleadP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP = dummy;
  }

  /* :  AP.LYWlagZ  = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LWlagZ),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LWlagZ[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ = dummy;
  }

  /* :  AP.LYWlagP  = LIB_limit(0.000160,160.0,interpn(MachVecY,AlphaVecY,double(fromDATA.gainsY.LWlagP),machY,beta_filt)); */
  for (k = 0; k < 30; k++) {
    tmp[k] = (real_T)rtU_DATAautop->gainsY.LWlagP[k];
  }

  dummy = interpn_s1jYV2bZ(MachVecY, AlphaVecY, tmp,
      numNAN3, BetaP);

  /* 'LIB_limit:27' if (in > max) */
  if (dummy > 160.0) {
    /* 'LIB_limit:28' out = max; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP = 160.0;
  } else if (dummy < 0.00016) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP = 0.00016;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP = dummy;
  }

  /* :  APout = AP; */
  /* :  Tel20Hz.ATP_count  = uint16(cnt); */
  rtY_autoptel_20Hz->ATP_count = Autopilot_300Hz_Function_1D_M->dwork.cnt;

  /* :  Tel20Hz.ATP_mode20 = uint8(APloop); */
  rtY_autoptel_20Hz->ATP_mode20 = rtU_guidin->AP_Loop;

  /* :  Tel20Hz.ATP_AFtype = uint8(Airframe_ID); */
  rtY_autoptel_20Hz->ATP_AFtype = AF_ID;

  /* :  Tel20Hz.ATP_Kp_phi = uint8(LIB_limit(0,255,AP.Kp_phi*20.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_phi * 20.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_phi = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_phi = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Kp_wbx = uint8(LIB_limit(0,255,AP.Kp_wbx*1000.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbx * 1000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_wbx = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_wbx = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Kp_gbz = uint8(LIB_limit(0,255,AP.Kp_gbz*2000.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gbz * 2000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_gbz = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_gbz = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Ki_wby = uint8(LIB_limit(0,255,AP.Ki_wby*20.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wby * 20.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Ki_wby = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Ki_wby = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Kp_wby = uint8(LIB_limit(0,255,AP.Kp_wby*250.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby * 250.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_wby = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_wby = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Kp_gby = uint8(LIB_limit(0,255,AP.Kp_gby*2000.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gby * 2000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_gby = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_gby = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Ki_wbz = uint8(LIB_limit(0,255,AP.Ki_wbz*20.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wbz * 20.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Ki_wbz = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Ki_wbz = MAX_uint8_T;
  }

  /* :  Tel20Hz.ATP_Kp_wbz = uint8(LIB_limit(0,255,AP.Kp_wbz*250.0)); */
  numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz * 250.0;

  /* 'LIB_limit:27' if (in > max) */
  if (numNAN3 > 255.0) {
    /* 'LIB_limit:28' out = max; */
    numNAN3 = 255.0;
  } else if (numNAN3 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    numNAN3 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(numNAN3);
  if (P2d_lead < 256.0) {
    rtY_autoptel_20Hz->ATP_Kp_wbz = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_20Hz->ATP_Kp_wbz = MAX_uint8_T;
  }

  /* :  if (APloop == 127) */
  if (((int32_T)rtU_guidin->AP_Loop) == 127) {
    /* :  Tel20Hz.ATP_RPlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.RPlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_RPlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_RPlagCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_RRleadCRC = uint16(LIB_limit(0,65535,100.0/(AP.RRleadZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_RRleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_RRleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_RRlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.RRlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_RRlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_RRlagCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_AlowCRC   = uint16(LIB_limit(0,65535,100.0*(AP.LZAlowWN/(2.0*pi)))); */
    numNAN3 = (Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN /
               6.2831853071795862) * 100.0;

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_AlowCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_AlowCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LAleadCRC = uint16(LIB_limit(0,65535,100.0/(AP.LZAleadZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LAleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LAleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LAlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.LZAlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LAlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LAlagCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LWleadCRC = uint16(LIB_limit(0,65535,100.0/(AP.LYWleadZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LWleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LWleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LWlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.LYWlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LWlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LWlagCRC = MAX_uint16_T;
    }
  } else {
    /* :  else */
    /* :  Tel20Hz.ATP_RPlagCRC  = cnt; */
    rtY_autoptel_20Hz->ATP_RPlagCRC = Autopilot_300Hz_Function_1D_M->dwork.cnt;

    /* :  Tel20Hz.ATP_RRleadCRC = uint16(LIB_limit(0,65535,DXL*100000.0)); */
    numNAN3 = Autopilot_300Hz_Function_1D_M->dwork.Memory_PreviousInput *
      100000.0;

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_RRleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_RRleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_RRlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.RRlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_RRlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_RRlagCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_AlowCRC   = uint16(LIB_limit(0,65535,100.0*(AP.LZAlowWN/(2.0*pi)))); */
    numNAN3 = (Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN /
               6.2831853071795862) * 100.0;

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_AlowCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_AlowCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LAleadCRC = uint16(LIB_limit(0,65535,100.0/(AP.LZAleadZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LAleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LAleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LAlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.LZAlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LAlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LAlagCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LWleadCRC = uint16(LIB_limit(0,65535,100.0/(AP.LYWleadZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LWleadCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LWleadCRC = MAX_uint16_T;
    }

    /* :  Tel20Hz.ATP_LWlagCRC  = uint16(LIB_limit(0,65535,100.0/(AP.LYWlagZ*2.0*pi))); */
    numNAN3 = 100.0 / ((Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ * 2.0) *
                       3.1415926535897931);

    /* 'LIB_limit:27' if (in > max) */
    if (numNAN3 > 65535.0) {
      /* 'LIB_limit:28' out = max; */
      numNAN3 = 65535.0;
    } else if (numNAN3 < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      numNAN3 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    P2d_lead = rt_roundd_snf(numNAN3);
    if (P2d_lead < 65536.0) {
      rtY_autoptel_20Hz->ATP_LWlagCRC = (uint16_T)P2d_lead;
    } else {
      rtY_autoptel_20Hz->ATP_LWlagCRC = MAX_uint16_T;
    }
  }

  /* MATLAB Function: '<S1>/Activate wby loop' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  /* :  out = double(bitget(uint8(in),4)); */
  SigmaP = (real_T)(((((uint32_T)rtU_guidin->AP_Loop) & 8U) != 0U) ? 1 : 0);

  /* MATLAB Function: '<S1>/GBZ_Filter' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Activate gbz loop'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Product: '<S1>/Product2'
   *  Sum: '<S1>/Sum'
   */
  /* :  out = double(bitget(uint8(in),3)); */
  numNAN3 = -(((Autopilot_300Hz_Function_1D_M->dwork.GBZC -
                Autopilot_300Hz_Function_1D_M->dwork.GBZM) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gbz) * ((real_T)
    (((((uint32_T)rtU_guidin->AP_Loop) & 4U) != 0U) ? 1 : 0)));

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_dk) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_ke = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_dk = true;

    /* :  xlow2_prev1 = 0.0; */
    /* :  ylow2_prev1 = 0.0; */
    /* :  xlow2_prev2 = 0.0; */
    /* :  ylow2_prev2 = 0.0; */
    /* :  ylead_prev  = 0.0; */
    /* :  xlead_prev  = 0.0; */
    /* :  ylag_prev   = 0.0; */
    /* :  xlag_prev   = 0.0; */
  }

  /* :  den = K*K + K*2.0*AP.LZAlowWN*0.7 + AP.LZAlowWN*AP.LZAlowWN; */
  AlphaP = Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN *
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN;
  rtb_sigma = Autopilot_300Hz_Function_1D_M->dwork.K_ke *
    Autopilot_300Hz_Function_1D_M->dwork.K_ke;
  machR = ((Autopilot_300Hz_Function_1D_M->dwork.K_ke * 2.0) *
           Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN) * 0.7;
  den = (machR + rtb_sigma) + AlphaP;

  /* :  P1d_low2 = (AP.LZAlowWN*AP.LZAlowWN)/den; */
  machP = AlphaP / den;

  /* :  P2d_low2 = (2.0*AP.LZAlowWN*AP.LZAlowWN)/den; */
  /* :  P3d_low2 = P1d_low2; */
  /* :  Z2d_low2 = (2.0*AP.LZAlowWN*AP.LZAlowWN - 2.0*K*K)/den; */
  /* :  Z3d_low2 = (K*K - K*2.0*AP.LZAlowWN*0.7 + AP.LZAlowWN*AP.LZAlowWN)/den; */
  /* :  P1d_lead = ( K*AP.LZAleadZ+1.0)/(K*AP.LZAleadP+1.0); */
  /* :  P2d_lead = (-K*AP.LZAleadZ+1.0)/(K*AP.LZAleadP+1.0); */
  /* :  Z2d_lead = (-K*AP.LZAleadP+1.0)/(K*AP.LZAleadP+1.0); */
  /* :  P1d_lag  = ( K*AP.LZAlagZ+1.0)/(K*AP.LZAlagP+1.0); */
  /* :  P2d_lag  = (-K*AP.LZAlagZ+1.0)/(K*AP.LZAlagP+1.0); */
  /* :  Z2d_lag  = (-K*AP.LZAlagP+1.0)/(K*AP.LZAlagP+1.0); */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  GBZin       = 0.0; */
    numNAN3 = 0.0;

    /* :  xlow2_prev1 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1 = 0.0;

    /* :  ylow2_prev1 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1 = 0.0;

    /* :  xlow2_prev2 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2 = 0.0;

    /* :  ylow2_prev2 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2 = 0.0;

    /* :  xlead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_k = 0.0;

    /* :  ylead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_f = 0.0;

    /* :  xlag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_a = 0.0;

    /* :  ylag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_p = 0.0;
  }

  /* :  GBZlowpass = P1d_low2*GBZin + P2d_low2*xlow2_prev1 + P3d_low2*xlow2_prev2 ... */
  /* :                              - Z2d_low2*ylow2_prev1 - Z3d_low2*ylow2_prev2; */
  Z2d_alpha = (2.0 * Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlowWN;
  P2d_sigma = (((((Z2d_alpha / den) *
                  Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1) + (machP *
    numNAN3)) + (machP * Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2)) -
               (((Z2d_alpha - ((2.0 * Autopilot_300Hz_Function_1D_M->dwork.K_ke)
    * Autopilot_300Hz_Function_1D_M->dwork.K_ke)) / den) *
                Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1)) -
    ((((rtb_sigma - machR) + AlphaP) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2);

  /* :  xlow2_prev2 = xlow2_prev1; */
  Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2 =
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1;

  /* :  ylow2_prev2 = ylow2_prev1; */
  Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2 =
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1;

  /* :  xlow2_prev1 = GBZin; */
  Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1 = numNAN3;

  /* :  ylow2_prev1 = GBZlowpass; */
  Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1 = P2d_sigma;

  /* :  GBZlead = P1d_lead*GBZlowpass + P2d_lead*xlead_prev - Z2d_lead*ylead_prev; */
  Z2d_alpha = (Autopilot_300Hz_Function_1D_M->dwork.K_ke *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP) + 1.0;
  BetaP = (((((Autopilot_300Hz_Function_1D_M->dwork.K_ke *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ) + 1.0) /
             Z2d_alpha) * P2d_sigma) +
           (((((-Autopilot_300Hz_Function_1D_M->dwork.K_ke) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadZ) + 1.0) /
             Z2d_alpha) * Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_k)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_ke) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.LZAleadP) + 1.0) / Z2d_alpha) *
     Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_f);

  /* :  xlead_prev = GBZlowpass; */
  Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_k = P2d_sigma;

  /* :  ylead_prev = GBZlead; */
  Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_f = BetaP;

  /* :  GBZlag  = P1d_lag*GBZlead + P2d_lag*xlag_prev - Z2d_lag*ylag_prev; */
  Z2d_alpha = (Autopilot_300Hz_Function_1D_M->dwork.K_ke *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP) + 1.0;
  P2d_sigma = (((((Autopilot_300Hz_Function_1D_M->dwork.K_ke *
                   Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ) + 1.0) /
                 Z2d_alpha) * BetaP) +
               (((((-Autopilot_300Hz_Function_1D_M->dwork.K_ke) *
                   Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagZ) + 1.0) /
                 Z2d_alpha) * Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_a))
    - (((((-Autopilot_300Hz_Function_1D_M->dwork.K_ke) *
          Autopilot_300Hz_Function_1D_M->dwork.AP.LZAlagP) + 1.0) / Z2d_alpha) *
       Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_p);

  /* :  xlag_prev  = GBZlead; */
  Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_a = BetaP;

  /* :  ylag_prev  = GBZlag; */
  Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_p = P2d_sigma;

  /* :  GBZout = GBZlag*AP.NCApitch + GBZin*(1.0-AP.NCApitch); */
  P2d_sigma = ((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch) *
               numNAN3) + (P2d_sigma *
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCApitch);

  /* End of MATLAB Function: '<S1>/GBZ_Filter' */

  /* Sum: '<S1>/Sum13' incorporates:
   *  Gain: '<S1>/Gain6'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Memory: '<S1>/Memory1'
   *  Product: '<S1>/Product3'
   *  Sum: '<S1>/Sum1'
   */
  /* :  GBZfil = GBZlag; */
  BetaP = ((((P2d_sigma - Autopilot_300Hz_Function_1D_M->dwork.WBYM) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wby) * SigmaP) *
           0.0033333333333333335) +
    Autopilot_300Hz_Function_1D_M->dwork.Memory1_PreviousInput;

  /* MATLAB Function: '<S1>/Activate wbz loop' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  /* :  out = double(bitget(uint8(in),6)); */
  Z2d_alpha = (real_T)(((((uint32_T)rtU_guidin->AP_Loop) & 32U) != 0U) ? 1 : 0);

  /* MATLAB Function: '<S1>/GBY_Filter' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Activate gby loop'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Product: '<S1>/Product5'
   *  Sum: '<S1>/Sum5'
   */
  /* :  out = double(bitget(uint8(in),5)); */
  numNAN3 = ((Autopilot_300Hz_Function_1D_M->dwork.GBYC -
              Autopilot_300Hz_Function_1D_M->dwork.GBYM) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_gby) * ((real_T)
    (((((uint32_T)rtU_guidin->AP_Loop) & 16U) != 0U) ? 1 : 0));

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_e) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_m = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_e = true;

    /* :  xlow2_prev1 = 0.0; */
    /* :  ylow2_prev1 = 0.0; */
    /* :  xlow2_prev2 = 0.0; */
    /* :  ylow2_prev2 = 0.0; */
    /* :  ylead_prev  = 0.0; */
    /* :  xlead_prev  = 0.0; */
    /* :  ylag_prev   = 0.0; */
    /* :  xlag_prev   = 0.0; */
  }

  /* :  den = K*K + K*2.0*AP.LYAlowWN*0.7 + AP.LYAlowWN*AP.LYAlowWN; */
  AlphaP = Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN *
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN;
  rtb_sigma = Autopilot_300Hz_Function_1D_M->dwork.K_m *
    Autopilot_300Hz_Function_1D_M->dwork.K_m;
  machR = ((Autopilot_300Hz_Function_1D_M->dwork.K_m * 2.0) *
           Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN) * 0.7;
  den = (machR + rtb_sigma) + AlphaP;

  /* :  P1d_low2 = (AP.LYAlowWN*AP.LYAlowWN)/den; */
  machP = AlphaP / den;

  /* :  P2d_low2 = (2.0*AP.LYAlowWN*AP.LYAlowWN)/den; */
  /* :  P3d_low2 = P1d_low2; */
  /* :  Z2d_low2 = (2.0*AP.LYAlowWN*AP.LYAlowWN - 2.0*K*K)/den; */
  /* :  Z3d_low2 = (K*K - K*2.0*AP.LYAlowWN*0.7 + AP.LYAlowWN*AP.LYAlowWN)/den; */
  /* :  P1d_lead = ( K*AP.LYAleadZ+1.0)/(K*AP.LYAleadP+1.0); */
  /* :  P2d_lead = (-K*AP.LYAleadZ+1.0)/(K*AP.LYAleadP+1.0); */
  /* :  Z2d_lead = (-K*AP.LYAleadP+1.0)/(K*AP.LYAleadP+1.0); */
  /* :  P1d_lag  = ( K*AP.LYAlagZ+1.0)/(K*AP.LYAlagP+1.0); */
  /* :  P2d_lag  = (-K*AP.LYAlagZ+1.0)/(K*AP.LYAlagP+1.0); */
  /* :  Z2d_lag  = (-K*AP.LYAlagP+1.0)/(K*AP.LYAlagP+1.0); */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  GBYin       = 0.0; */
    numNAN3 = 0.0;

    /* :  xlow2_prev1 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1_p = 0.0;

    /* :  ylow2_prev1 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1_d = 0.0;

    /* :  xlow2_prev2 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2_a = 0.0;

    /* :  ylow2_prev2 = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2_i = 0.0;

    /* :  xlead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_f = 0.0;

    /* :  ylead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_d = 0.0;

    /* :  xlag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_h = 0.0;

    /* :  ylag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_i = 0.0;
  }

  /* :  GBYlowpass = P1d_low2*GBYin + P2d_low2*xlow2_prev1 + P3d_low2*xlow2_prev2 ... */
  /* :                              - Z2d_low2*ylow2_prev1 - Z3d_low2*ylow2_prev2; */
  Z2d_sigma = (2.0 * Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlowWN;
  AlphaP = (((((Z2d_sigma / den) *
               Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1_p) + (machP *
    numNAN3)) + (machP * Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2_a)) -
            (((Z2d_sigma - ((2.0 * Autopilot_300Hz_Function_1D_M->dwork.K_m) *
    Autopilot_300Hz_Function_1D_M->dwork.K_m)) / den) *
             Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1_d)) -
    ((((rtb_sigma - machR) + AlphaP) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2_i);

  /* :  xlow2_prev2 = xlow2_prev1; */
  Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev2_a =
    Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1_p;

  /* :  ylow2_prev2 = ylow2_prev1; */
  Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev2_i =
    Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1_d;

  /* :  xlow2_prev1 = GBYin; */
  Autopilot_300Hz_Function_1D_M->dwork.xlow2_prev1_p = numNAN3;

  /* :  ylow2_prev1 = GBYlowpass; */
  Autopilot_300Hz_Function_1D_M->dwork.ylow2_prev1_d = AlphaP;

  /* :  GBYlead = P1d_lead*GBYlowpass + P2d_lead*xlead_prev - Z2d_lead*ylead_prev; */
  machP = (Autopilot_300Hz_Function_1D_M->dwork.K_m *
           Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP) + 1.0;
  machP = (((((Autopilot_300Hz_Function_1D_M->dwork.K_m *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ) + 1.0) / machP)
            * AlphaP) + (((((-Autopilot_300Hz_Function_1D_M->dwork.K_m) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadZ) + 1.0) / machP) *
            Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_f)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_m) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.LYAleadP) + 1.0) / machP) *
     Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_d);

  /* :  xlead_prev = GBYlowpass; */
  Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_f = AlphaP;

  /* :  ylead_prev = GBYlead; */
  Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_d = machP;

  /* :  GBYlag  = P1d_lag*GBYlead + P2d_lag*xlag_prev - Z2d_lag*ylag_prev; */
  Z2d_sigma = (Autopilot_300Hz_Function_1D_M->dwork.K_m *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP) + 1.0;
  AlphaP = (((((Autopilot_300Hz_Function_1D_M->dwork.K_m *
                Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ) + 1.0) /
              Z2d_sigma) * machP) +
            (((((-Autopilot_300Hz_Function_1D_M->dwork.K_m) *
                Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagZ) + 1.0) /
              Z2d_sigma) * Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_h)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_m) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.LYAlagP) + 1.0) / Z2d_sigma) *
     Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_i);

  /* :  xlag_prev  = GBYlead; */
  Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_h = machP;

  /* :  ylag_prev  = GBYlag; */
  Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_i = AlphaP;

  /* :  GBYout = GBYlag*AP.NCAyaw + GBYin*(1.0-AP.NCAyaw); */
  AlphaP = ((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw) * numNAN3) +
    (AlphaP * Autopilot_300Hz_Function_1D_M->dwork.AP.NCAyaw);

  /* End of MATLAB Function: '<S1>/GBY_Filter' */

  /* Sum: '<S1>/Sum14' incorporates:
   *  Gain: '<S1>/Gain7'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Memory: '<S1>/Memory2'
   *  Product: '<S1>/Product6'
   *  Sum: '<S1>/Sum6'
   */
  /* :  GBYfil = GBYlag; */
  machP = ((((AlphaP - Autopilot_300Hz_Function_1D_M->dwork.WBZM) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_wbz) * Z2d_alpha) *
           0.0033333333333333335) +
    Autopilot_300Hz_Function_1D_M->dwork.Memory2_PreviousInput;

  /* MATLAB Function: '<S1>/Activate phi loop' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  /* :  out = double(bitget(uint8(in),2)); */
  /* :  out = double(bitget(uint8(in),1)); */
  numNAN3 = (real_T)(((((uint32_T)rtU_guidin->AP_Loop) & 1U) != 0U) ? 1 : 0);

  /* Sum: '<S1>/Sum3' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  alpha_in = Autopilot_300Hz_Function_1D_M->dwork.PHIC -
    Autopilot_300Hz_Function_1D_M->dwork.PHIM;

  /* MATLAB Function: '<S1>/RP_Filter' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  Product: '<S1>/Product'
   */
  rtb_sigma = (Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_phi * alpha_in) *
    numNAN3;

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_d) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_b = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_d = true;

    /* :  y_prev = 0.0; */
    /* :  x_prev = 0.0; */
  }

  /* :  den = K*AP.RPlagP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K_b *
         Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP) + 1.0;

  /* :  P1d = ( K*AP.RPlagZ+1.0)/den; */
  /* :  P2d = (-K*AP.RPlagZ+1.0)/den; */
  /* :  Z2d = (-K*AP.RPlagP+1.0)/den; */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  RPFin  = 0.0; */
    rtb_sigma = 0.0;

    /* :  x_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.x_prev = 0.0;

    /* :  y_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.y_prev = 0.0;
  }

  /* :  RPF = P1d*RPFin + P2d*x_prev - Z2d*y_prev; */
  machR = (((((Autopilot_300Hz_Function_1D_M->dwork.K_b *
               Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ) + 1.0) / den) *
            rtb_sigma) + (((((-Autopilot_300Hz_Function_1D_M->dwork.K_b) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagZ) + 1.0) / den) *
            Autopilot_300Hz_Function_1D_M->dwork.x_prev)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_b) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.RPlagP) + 1.0) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.y_prev);

  /* :  x_prev  = RPFin; */
  Autopilot_300Hz_Function_1D_M->dwork.x_prev = rtb_sigma;

  /* :  y_prev  = RPF; */
  Autopilot_300Hz_Function_1D_M->dwork.y_prev = machR;

  /* Product: '<S1>/Product34' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   */
  /* :  RPFout = RPF*AP.NCProll + RPFin*(1.0-AP.NCProll); */
  /* :  RPFfil = RPF; */
  alpha_in = (Autopilot_300Hz_Function_1D_M->dwork.AP.Ki_phi * alpha_in) *
    numNAN3;

  /* Sum: '<S1>/Sum17' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/RP_Filter'
   */
  P2d_alpha = (((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll) *
                rtb_sigma) + (machR *
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCProll)) + alpha_in;

  /* Saturate: '<S1>/Saturation7' */
  if (P2d_alpha > 6.9813170079773181) {
    numNAN3 = 6.9813170079773181;
  } else if (P2d_alpha < -6.9813170079773181) {
    numNAN3 = -6.9813170079773181;
  } else {
    numNAN3 = P2d_alpha;
  }

  /* End of Saturate: '<S1>/Saturation7' */

  /* MATLAB Function: '<S1>/RR_Filter' incorporates:
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Activate wbx loop'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Product: '<S1>/Product1'
   *  Sum: '<S1>/Sum4'
   */
  numNAN3 = ((numNAN3 - Autopilot_300Hz_Function_1D_M->dwork.WBXM) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbx) * ((real_T)
    (((((uint32_T)rtU_guidin->AP_Loop) & 2U) != 0U) ? 1 : 0));

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty = true;

    /* :  ylead_prev = 0.0; */
    /* :  xlead_prev = 0.0; */
    /* :  ylag_prev  = 0.0; */
    /* :  xlag_prev  = 0.0; */
  }

  /* :  den = K*AP.RRleadP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K *
         Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP) + 1.0;

  /* :  P1d_lead = ( K*AP.RRleadZ+1.0)/den; */
  sigma_in = ((Autopilot_300Hz_Function_1D_M->dwork.K *
               Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ) + 1.0) / den;

  /* :  P2d_lead = (-K*AP.RRleadZ+1.0)/den; */
  P2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadZ) + 1.0) / den;

  /* :  Z2d_lead = (-K*AP.RRleadP+1.0)/den; */
  Z2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.RRleadP) + 1.0) / den;

  /* :  den = K*AP.RRlagP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K *
         Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP) + 1.0;

  /* :  P1d_lag  = ( K*AP.RRlagZ+1.0)/den; */
  /* :  P2d_lag  = (-K*AP.RRlagZ+1.0)/den; */
  /* :  Z2d_lag  = (-K*AP.RRlagP+1.0)/den; */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  RWFin      = 0.0; */
    numNAN3 = 0.0;

    /* :  xlead_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev = 0.0;

    /* :  ylead_prev = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev = 0.0;

    /* :  xlag_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev = 0.0;

    /* :  ylag_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylag_prev = 0.0;
  }

  /* :  RWFlead = P1d_lead*RWFin + P2d_lead*xlead_prev - Z2d_lead*ylead_prev; */
  machR = ((sigma_in * numNAN3) + (P2d_lead *
            Autopilot_300Hz_Function_1D_M->dwork.xlead_prev)) - (Z2d_lead *
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev);

  /* :  xlead_prev = RWFin; */
  Autopilot_300Hz_Function_1D_M->dwork.xlead_prev = numNAN3;

  /* :  ylead_prev = RWFlead; */
  Autopilot_300Hz_Function_1D_M->dwork.ylead_prev = machR;

  /* :  RWFlag  = P1d_lag*RWFlead + P2d_lag*xlag_prev - Z2d_lag*ylag_prev; */
  rtb_sigma = (((((Autopilot_300Hz_Function_1D_M->dwork.K *
                   Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ) + 1.0) / den)
                * machR) + (((((-Autopilot_300Hz_Function_1D_M->dwork.K) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagZ) + 1.0) / den) *
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.RRlagP) + 1.0) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.ylag_prev);

  /* :  xlag_prev  = RWFlead; */
  Autopilot_300Hz_Function_1D_M->dwork.xlag_prev = machR;

  /* :  ylag_prev  = RWFlag; */
  Autopilot_300Hz_Function_1D_M->dwork.ylag_prev = rtb_sigma;

  /* Product: '<S1>/Product23' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/RR_Filter'
   */
  /* :  RWFout = RWFlag*AP.NCWroll + RWFin*(1.0-AP.NCWroll); */
  /* :  DXCfil = RWFlag; */
  machR = (((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll) * numNAN3) +
           (rtb_sigma * Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll)) *
    Autopilot_300Hz_Function_1D_M->dwork.QRATIO;

  /* Saturate: '<S1>/Saturation1' */
  if (machR > 0.087266462599716474) {
    machR = 0.087266462599716474;
  } else {
    if (machR < -0.087266462599716474) {
      machR = -0.087266462599716474;
    }
  }

  /* End of Saturate: '<S1>/Saturation1' */

  /* Switch: '<S22>/Switch2' incorporates:
   *  Gain: '<S1>/Gain10'
   *  Memory: '<S1>/Memory4'
   *  RelationalOperator: '<S22>/LowerRelop1'
   *  RelationalOperator: '<S22>/UpperRelop'
   *  Switch: '<S22>/Switch'
   */
  if (BetaP > Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput) {
    alpha_in = Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput;
  } else if (BetaP <
             (-Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput)) {
    /* Switch: '<S22>/Switch' incorporates:
     *  Gain: '<S1>/Gain10'
     */
    alpha_in = -Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput;
  } else {
    alpha_in = BetaP;
  }

  /* End of Switch: '<S22>/Switch2' */

  /* Product: '<S1>/Product25' incorporates:
   *  Constant: '<S1>/Constant19'
   *  Sum: '<S1>/Sum2'
   */
  numNAN3 = (1.0 - ((real_T)rtb_reset)) * alpha_in;

  /* Product: '<S1>/Product4' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   */
  alpha_in = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby * numNAN3;

  /* Switch: '<S20>/Switch2' incorporates:
   *  Gain: '<S1>/Gain3'
   *  Memory: '<S1>/Memory6'
   *  RelationalOperator: '<S20>/LowerRelop1'
   *  RelationalOperator: '<S20>/UpperRelop'
   *  Switch: '<S20>/Switch'
   */
  if (alpha_in > Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput) {
    alpha_in = Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput;
  } else {
    if (alpha_in < (-Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput))
    {
      /* Switch: '<S20>/Switch' incorporates:
       *  Gain: '<S1>/Gain3'
       */
      alpha_in = -Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput;
    }
  }

  /* End of Switch: '<S20>/Switch2' */

  /* Sum: '<S1>/Sum12' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Product: '<S1>/Product11'
   *  Product: '<S1>/Product8'
   */
  alpha_in -= (SigmaP * Autopilot_300Hz_Function_1D_M->dwork.WBYM) *
    Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby;

  /* MATLAB Function: '<S1>/LRY_Filter' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   */
  SigmaP = alpha_in;

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_n) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_k = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_n = true;

    /* :  ylead_prev  = 0.0; */
    /* :  xlead_prev  = 0.0; */
    /* :  ylag_prev   = 0.0; */
    /* :  xlag_prev   = 0.0; */
  }

  /* :  den = K*AP.LZWleadP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K_k *
         Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP) + 1.0;

  /* :  P1d_lead = ( K*AP.LZWleadZ+1.0)/den; */
  sigma_in = ((Autopilot_300Hz_Function_1D_M->dwork.K_k *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ) + 1.0) / den;

  /* :  P2d_lead = (-K*AP.LZWleadZ+1.0)/den; */
  P2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K_k) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadZ) + 1.0) / den;

  /* :  Z2d_lead = (-K*AP.LZWleadP+1.0)/den; */
  Z2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K_k) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LZWleadP) + 1.0) / den;

  /* :  den = K*AP.LZWlagP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K_k *
         Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP) + 1.0;

  /* :  P1d_lag  = ( K*AP.LZWlagZ+1.0)/den; */
  /* :  P2d_lag  = (-K*AP.LZWlagZ+1.0)/den; */
  /* :  Z2d_lag  = (-K*AP.LZWlagP+1.0)/den; */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  DZCin       = 0.0; */
    SigmaP = 0.0;

    /* :  xlead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_l = 0.0;

    /* :  ylead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_b = 0.0;

    /* :  xlag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_i = 0.0;

    /* :  ylag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_m = 0.0;
  }

  /* :  DZClead = P1d_lead*DZCin + P2d_lead*xlead_prev - Z2d_lead*ylead_prev; */
  Z2d_sigma = ((sigma_in * SigmaP) + (P2d_lead *
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_l)) - (Z2d_lead *
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_b);

  /* :  xlead_prev = DZCin; */
  Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_l = SigmaP;

  /* :  ylead_prev = DZClead; */
  Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_b = Z2d_sigma;

  /* :  DZClag  = P1d_lag*DZClead + P2d_lag*xlag_prev - Z2d_lag*ylag_prev; */
  den = (((((Autopilot_300Hz_Function_1D_M->dwork.K_k *
             Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ) + 1.0) / den) *
          Z2d_sigma) + (((((-Autopilot_300Hz_Function_1D_M->dwork.K_k) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagZ) + 1.0) / den) *
                        Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_i)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_k) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.LZWlagP) + 1.0) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_m);

  /* :  xlag_prev  = DZClead; */
  Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_i = Z2d_sigma;

  /* :  ylag_prev  = DZClag; */
  Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_m = den;

  /* Product: '<S1>/Product21' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/LRY_Filter'
   */
  /* :  DZCout = DZClag*AP.NCWpitch + DZCin*(1.0-AP.NCWpitch); */
  /* :  DZCfil = DZClag; */
  Z2d_sigma = (((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch) *
                SigmaP) + (den *
    Autopilot_300Hz_Function_1D_M->dwork.AP.NCWpitch)) *
    Autopilot_300Hz_Function_1D_M->dwork.QRATIO;

  /* Switch: '<S21>/Switch2' incorporates:
   *  Gain: '<S1>/Gain9'
   *  Memory: '<S1>/Memory3'
   *  RelationalOperator: '<S21>/LowerRelop1'
   *  RelationalOperator: '<S21>/UpperRelop'
   *  Switch: '<S21>/Switch'
   */
  if (machP > Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput) {
    den = Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput;
  } else if (machP <
             (-Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput)) {
    /* Switch: '<S21>/Switch' incorporates:
     *  Gain: '<S1>/Gain9'
     */
    den = -Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput;
  } else {
    den = machP;
  }

  /* End of Switch: '<S21>/Switch2' */

  /* Product: '<S1>/Product24' incorporates:
   *  Constant: '<S1>/Constant20'
   *  Sum: '<S1>/Sum7'
   */
  SigmaP = (1.0 - ((real_T)rtb_reset)) * den;

  /* Product: '<S1>/Product7' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   */
  den = Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz * SigmaP;

  /* Switch: '<S23>/Switch2' incorporates:
   *  Gain: '<S1>/Gain2'
   *  Memory: '<S1>/Memory5'
   *  RelationalOperator: '<S23>/LowerRelop1'
   *  RelationalOperator: '<S23>/UpperRelop'
   *  Switch: '<S23>/Switch'
   */
  if (den > Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput) {
    den = Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput;
  } else {
    if (den < (-Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput)) {
      /* Switch: '<S23>/Switch' incorporates:
       *  Gain: '<S1>/Gain2'
       */
      den = -Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput;
    }
  }

  /* End of Switch: '<S23>/Switch2' */

  /* Sum: '<S1>/Sum15' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  Product: '<S1>/Product10'
   *  Product: '<S1>/Product9'
   */
  Z2d_alpha = den - ((Z2d_alpha * Autopilot_300Hz_Function_1D_M->dwork.WBZM) *
                     Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz);

  /* MATLAB Function: '<S1>/LRZ_Filter' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   */
  dFin1 = Z2d_alpha;

  /* :  if (isempty(K)) */
  if (!Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_a) {
    /* :  K = double(2.0/AP.t_delt); */
    Autopilot_300Hz_Function_1D_M->dwork.K_a = 2.0 /
      Autopilot_300Hz_Function_1D_M->dwork.AP.t_delt;
    Autopilot_300Hz_Function_1D_M->dwork.K_not_empty_a = true;

    /* :  ylead_prev  = 0.0; */
    /* :  xlead_prev  = 0.0; */
    /* :  ylag_prev   = 0.0; */
    /* :  xlag_prev   = 0.0; */
  }

  /* :  den = K*AP.LYWleadP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K_a *
         Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP) + 1.0;

  /* :  P1d_lead = ( K*AP.LYWleadZ+1.0)/den; */
  sigma_in = ((Autopilot_300Hz_Function_1D_M->dwork.K_a *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ) + 1.0) / den;

  /* :  P2d_lead = (-K*AP.LYWleadZ+1.0)/den; */
  P2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K_a) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadZ) + 1.0) / den;

  /* :  Z2d_lead = (-K*AP.LYWleadP+1.0)/den; */
  Z2d_lead = (((-Autopilot_300Hz_Function_1D_M->dwork.K_a) *
               Autopilot_300Hz_Function_1D_M->dwork.AP.LYWleadP) + 1.0) / den;

  /* :  den = K*AP.LYWlagP+1.0; */
  den = (Autopilot_300Hz_Function_1D_M->dwork.K_a *
         Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP) + 1.0;

  /* :  P1d_lag  = ( K*AP.LYWlagZ+1.0)/den; */
  /* :  P2d_lag  = (-K*AP.LYWlagZ+1.0)/den; */
  /* :  Z2d_lag  = (-K*AP.LYWlagP+1.0)/den; */
  /* :  if (reset) */
  if (((int32_T)rtb_reset) != 0) {
    /* :  DYCin       = 0.0; */
    dFin1 = 0.0;

    /* :  xlead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_i = 0.0;

    /* :  ylead_prev  = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_i = 0.0;

    /* :  xlag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_m = 0.0;

    /* :  ylag_prev   = 0.0; */
    Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_b = 0.0;
  }

  /* :  DYClead = P1d_lead*DYCin + P2d_lead*xlead_prev - Z2d_lead*ylead_prev; */
  sigma_in = ((sigma_in * dFin1) + (P2d_lead *
    Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_i)) - (Z2d_lead *
    Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_i);

  /* :  xlead_prev = DYCin; */
  Autopilot_300Hz_Function_1D_M->dwork.xlead_prev_i = dFin1;

  /* :  ylead_prev = DYClead; */
  Autopilot_300Hz_Function_1D_M->dwork.ylead_prev_i = sigma_in;

  /* :  DYClag  = P1d_lag*DYClead + P2d_lag*xlag_prev - Z2d_lag*ylag_prev; */
  den = (((((Autopilot_300Hz_Function_1D_M->dwork.K_a *
             Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ) + 1.0) / den) *
          sigma_in) + (((((-Autopilot_300Hz_Function_1D_M->dwork.K_a) *
             Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagZ) + 1.0) / den) *
                       Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_m)) -
    (((((-Autopilot_300Hz_Function_1D_M->dwork.K_a) *
        Autopilot_300Hz_Function_1D_M->dwork.AP.LYWlagP) + 1.0) / den) *
     Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_b);

  /* :  xlag_prev  = DYClead; */
  Autopilot_300Hz_Function_1D_M->dwork.xlag_prev_m = sigma_in;

  /* :  ylag_prev  = DYClag; */
  Autopilot_300Hz_Function_1D_M->dwork.ylag_prev_b = den;

  /* Product: '<S1>/Product22' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/LRZ_Filter'
   */
  /* :  DYCout = DYClag*AP.NCWyaw + DYCin*(1.0-AP.NCWyaw); */
  /* :  DYCfil = DYClag; */
  sigma_in = (((1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw) * dFin1) +
              (den * Autopilot_300Hz_Function_1D_M->dwork.AP.NCWyaw)) *
    Autopilot_300Hz_Function_1D_M->dwork.QRATIO;

  /* MATLAB Function: '<S1>/Dynamic Fin Allocation' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  Inport: '<Root>/DATAautop'
   *  Inport: '<Root>/guidin'
   */
  /* :  if (isempty(Dcmd)) */
  /* :  dFin1 = LIB_checkNAN(guid_in.dFin1,dFin1,0,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->dFin1)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:6' out = 0.0; */
    dFin1 = 0.0;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    dFin1 = rtU_guidin->dFin1;
  }

  /* :  dFin2 = LIB_checkNAN(guid_in.dFin2,dFin2,0,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->dFin2)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:6' out = 0.0; */
    dFin2 = 0.0;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    dFin2 = rtU_guidin->dFin2;
  }

  /* :  dFin3 = LIB_checkNAN(guid_in.dFin3,dFin3,0,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->dFin3)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:6' out = 0.0; */
    dFin3 = 0.0;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    dFin3 = rtU_guidin->dFin3;
  }

  /* :  dFin4 = LIB_checkNAN(guid_in.dFin4,dFin4,0,AZfac); */
  /* 'LIB_checkNAN:4' if (isnan(in)) */
  if (rtIsNaN(rtU_guidin->dFin4)) {
    /* 'LIB_checkNAN:5' if (typehold == 0) */
    /* 'LIB_checkNAN:6' out = 0.0; */
    dFin4 = 0.0;
  } else {
    /* 'LIB_checkNAN:13' else */
    /* 'LIB_checkNAN:14' state = in; */
    /* 'LIB_checkNAN:15' out = state; */
    dFin4 = rtU_guidin->dFin4;
  }

  /* :  if (abs(DXC) > DXLim) */
  P2d_lead = fabs(machR);
  if (P2d_lead > Autopilot_300Hz_Function_1D_M->dwork.DXLim) {
    /* :  DXLim = abs(DXC); */
    Autopilot_300Hz_Function_1D_M->dwork.DXLim = P2d_lead;
  } else {
    /* :  else */
    /* :  DXLim = DXLim - 0.0001; */
    Autopilot_300Hz_Function_1D_M->dwork.DXLim -= 0.0001;

    /* :  if (DXLim < 0.0349) */
    if (Autopilot_300Hz_Function_1D_M->dwork.DXLim < 0.0349) {
      /* :  DXLim = 0.0349; */
      Autopilot_300Hz_Function_1D_M->dwork.DXLim = 0.0349;
    }
  }

  /* :  DXL = DXLim; */
  /* :  KpathZ = APgains.Kp_wby*Qratio; */
  /* :  KpathY = APgains.Kp_wbz*Qratio; */
  /* :  DYZCLim = defl_lim - DXLim; */
  den = ((real_T)rtU_DATAautop->defl_limit) -
    Autopilot_300Hz_Function_1D_M->dwork.DXLim;

  /* :  DYCL = DYZCLim; */
  /* :  DZCL = DYZCLim; */
  /* :  intYlim = 0.8*DYCL/KpathY; */
  /* :  intZlim = 0.8*DZCL/KpathZ; */
  /* :  dyclim = 0.67*DYCL/Qratio; */
  /* :  dzclim = 0.67*DZCL/Qratio; */
  /* :  Dcmd1 = DXC + LIB_limit(-DYZCLim,DYZCLim,-DYC + DZC); */
  P2d_lead = (-sigma_in) + Z2d_sigma;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_lead > den) {
    /* 'LIB_limit:28' out = max; */
    P2d_lead = den;
  } else if (P2d_lead < (-den)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_lead = -den;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead += machR;

  /* :  Dcmd2 = DXC + LIB_limit(-DYZCLim,DYZCLim, DYC + DZC); */
  Z2d_lead = sigma_in + Z2d_sigma;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > den) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = den;
  } else if (Z2d_lead < (-den)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = -den;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Dcmd2 = machR + Z2d_lead;

  /* :  Dcmd3 = DXC + LIB_limit(-DYZCLim,DYZCLim, DYC - DZC); */
  Z2d_lead = sigma_in - Z2d_sigma;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > den) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = den;
  } else if (Z2d_lead < (-den)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = -den;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Dcmd3 = machR + Z2d_lead;

  /* :  Dcmd4 = DXC + LIB_limit(-DYZCLim,DYZCLim,-DYC - DZC); */
  Z2d_lead = (-sigma_in) - Z2d_sigma;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > den) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = den;
  } else if (Z2d_lead < (-den)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = -den;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Z2d_lead += machR;

  /* :  if (abs(dFin1) > 0.001) */
  if (fabs(dFin1) > 0.001) {
    /* :  fin1 = Dcmd(1) + dFin1; */
    P2d_lead = Autopilot_300Hz_Function_1D_M->dwork.Dcmd[0] + dFin1;
  } else {
    /* :  else */
    /* :  fin1 = Dcmd1; */
    /* :  Dcmd(1) = Dcmd1; */
    Autopilot_300Hz_Function_1D_M->dwork.Dcmd[0] = P2d_lead;
  }

  /* :  if (abs(dFin2) > 0.001) */
  if (fabs(dFin2) > 0.001) {
    /* :  fin2 = Dcmd(2) + dFin2; */
    Dcmd2 = Autopilot_300Hz_Function_1D_M->dwork.Dcmd[1] + dFin2;
  } else {
    /* :  else */
    /* :  fin2 = Dcmd2; */
    /* :  Dcmd(2) = Dcmd2; */
    Autopilot_300Hz_Function_1D_M->dwork.Dcmd[1] = Dcmd2;
  }

  /* :  if (abs(dFin3) > 0.001) */
  if (fabs(dFin3) > 0.001) {
    /* :  fin3 = Dcmd(3) + dFin3; */
    Dcmd3 = Autopilot_300Hz_Function_1D_M->dwork.Dcmd[2] + dFin3;
  } else {
    /* :  else */
    /* :  fin3 = Dcmd3; */
    /* :  Dcmd(3) = Dcmd3; */
    Autopilot_300Hz_Function_1D_M->dwork.Dcmd[2] = Dcmd3;
  }

  /* :  if (abs(dFin4) > 0.001) */
  if (fabs(dFin4) > 0.001) {
    /* :  fin4 = Dcmd(4) + dFin4; */
    Z2d_lead = Autopilot_300Hz_Function_1D_M->dwork.Dcmd[3] + dFin4;
  } else {
    /* :  else */
    /* :  fin4 = Dcmd4; */
    /* :  Dcmd(4) = Dcmd4; */
    Autopilot_300Hz_Function_1D_M->dwork.Dcmd[3] = Z2d_lead;
  }

  /* :  numNAN = sum(isnan([DXC DYC DZC])); */
  /* :  countNaNout = countNaNout + uint16(numNAN); */
  rtb_FixPtSum1 = ((uint32_T)((uint16_T)((int32_T)((real_T)((((real_T)(rtIsNaN
    (machR) ? 1.0 : 0.0)) + ((real_T)(rtIsNaN(sigma_in) ? 1.0 : 0.0))) +
    ((real_T)(rtIsNaN(Z2d_sigma) ? 1.0 : 0.0))))))) + ((uint32_T)
    Autopilot_300Hz_Function_1D_M->dwork.countNaNout);
  if (rtb_FixPtSum1 > 65535U) {
    rtb_FixPtSum1 = 65535U;
  }

  Autopilot_300Hz_Function_1D_M->dwork.countNaNout = (uint16_T)rtb_FixPtSum1;

  /* :  if (countNaNout > 65534) */
  if (((int32_T)Autopilot_300Hz_Function_1D_M->dwork.countNaNout) > 65534) {
    /* :  countNaNout = uint16(0); */
    Autopilot_300Hz_Function_1D_M->dwork.countNaNout = 0U;
  }

  /* Product: '<S1>/Product17' */
  /* :  cntNANout  = countNaNout; */
  dFin2 = Dcmd2 * numNAN1;

  /* Switch: '<S25>/Switch2' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  Gain: '<S1>/Gain4'
   *  Inport: '<Root>/DATAautop'
   *  RelationalOperator: '<S25>/LowerRelop1'
   *  RelationalOperator: '<S25>/UpperRelop'
   *  Switch: '<S25>/Switch'
   */
  if (dFin2 > ((real_T)rtU_DATAautop->defl_limit)) {
    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  Outport: '<Root>/deflcmd'
     */
    rtY_deflcmd->fin1 = (real_T)rtU_DATAautop->defl_limit;
  } else if (dFin2 < (-((real_T)rtU_DATAautop->defl_limit))) {
    /* Switch: '<S25>/Switch' incorporates:
     *  BusCreator: '<S1>/Bus Creator'
     *  Gain: '<S1>/Gain4'
     *  Outport: '<Root>/deflcmd'
     */
    rtY_deflcmd->fin1 = -((real_T)rtU_DATAautop->defl_limit);
  } else {
    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  Outport: '<Root>/deflcmd'
     *  Switch: '<S25>/Switch'
     */
    rtY_deflcmd->fin1 = dFin2;
  }

  /* End of Switch: '<S25>/Switch2' */

  /* Product: '<S1>/Product18' */
  dFin2 = Dcmd3 * numNAN1;

  /* Switch: '<S26>/Switch2' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  Gain: '<S1>/Gain4'
   *  Inport: '<Root>/DATAautop'
   *  RelationalOperator: '<S26>/LowerRelop1'
   *  RelationalOperator: '<S26>/UpperRelop'
   *  Switch: '<S26>/Switch'
   */
  if (dFin2 > ((real_T)rtU_DATAautop->defl_limit)) {
    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  Outport: '<Root>/deflcmd'
     */
    rtY_deflcmd->fin2 = (real_T)rtU_DATAautop->defl_limit;
  } else if (dFin2 < (-((real_T)rtU_DATAautop->defl_limit))) {
    /* Switch: '<S26>/Switch' incorporates:
     *  BusCreator: '<S1>/Bus Creator'
     *  Gain: '<S1>/Gain4'
     *  Outport: '<Root>/deflcmd'
     */
    rtY_deflcmd->fin2 = -((real_T)rtU_DATAautop->defl_limit);
  } else {
    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  Outport: '<Root>/deflcmd'
     *  Switch: '<S26>/Switch'
     */
    rtY_deflcmd->fin2 = dFin2;
  }

  /* End of Switch: '<S26>/Switch2' */

  /* Product: '<S1>/Product19' */
  Z2d_lead *= numNAN1;

  /* Switch: '<S27>/Switch2' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  Gain: '<S1>/Gain4'
   *  Inport: '<Root>/DATAautop'
   *  RelationalOperator: '<S27>/LowerRelop1'
   *  RelationalOperator: '<S27>/UpperRelop'
   *  Switch: '<S27>/Switch'
   */
  if (Z2d_lead > ((real_T)rtU_DATAautop->defl_limit)) {
    rtY_deflcmd->fin3 = (real_T)rtU_DATAautop->defl_limit;
  } else if (Z2d_lead < (-((real_T)rtU_DATAautop->defl_limit))) {
    /* Switch: '<S27>/Switch' incorporates:
     *  Gain: '<S1>/Gain4'
     */
    rtY_deflcmd->fin3 = -((real_T)rtU_DATAautop->defl_limit);
  } else {
    rtY_deflcmd->fin3 = Z2d_lead;
  }

  /* End of Switch: '<S27>/Switch2' */

  /* Product: '<S1>/Product16' */
  numNAN1 *= P2d_lead;

  /* Switch: '<S24>/Switch2' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  Gain: '<S1>/Gain4'
   *  Inport: '<Root>/DATAautop'
   *  RelationalOperator: '<S24>/LowerRelop1'
   *  RelationalOperator: '<S24>/UpperRelop'
   *  Switch: '<S24>/Switch'
   */
  if (numNAN1 > ((real_T)rtU_DATAautop->defl_limit)) {
    rtY_deflcmd->fin4 = (real_T)rtU_DATAautop->defl_limit;
  } else if (numNAN1 < (-((real_T)rtU_DATAautop->defl_limit))) {
    /* Switch: '<S24>/Switch' incorporates:
     *  Gain: '<S1>/Gain4'
     */
    rtY_deflcmd->fin4 = -((real_T)rtU_DATAautop->defl_limit);
  } else {
    rtY_deflcmd->fin4 = numNAN1;
  }

  /* End of Switch: '<S24>/Switch2' */

  /* BusCreator: '<S1>/Bus Creator' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Outport: '<Root>/deflcmd'
   *  UnitDelay: '<S11>/Output'
   */
  rtY_deflcmd->msg_count = Autopilot_300Hz_Function_1D_M->dwork.Output_DSTATE;
  rtY_deflcmd->Spare = 0U;

  /* Sum: '<S28>/FixPt Sum1' incorporates:
   *  Constant: '<S28>/FixPt Constant'
   *  UnitDelay: '<S11>/Output'
   */
  rtb_FixPtSum1 = Autopilot_300Hz_Function_1D_M->dwork.Output_DSTATE + 1U;

  /* MATLAB Function: '<S1>/Modes and Telemetry' incorporates:
   *  Constant: '<S1>/Constant2'
   *  Inport: '<Root>/guidin'
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/Dynamic Fin Allocation'
   *  MATLAB Function: '<S1>/RR_Filter'
   *  Memory: '<S1>/Memory3'
   *  Memory: '<S1>/Memory4'
   *  Memory: '<S1>/Memory5'
   *  Memory: '<S1>/Memory6'
   *  Outport: '<Root>/autoptel_300Hz'
   *  Sum: '<S1>/Sum8'
   */
  /* :  if (isempty(cnt)) */
  /* :  cnt = cnt  + 1; */
  k = (int32_T)((uint32_T)(((uint32_T)Autopilot_300Hz_Function_1D_M->dwork.cnt_j)
    + 1U));
  if (((uint32_T)k) > 255U) {
    k = 255;
  }

  Autopilot_300Hz_Function_1D_M->dwork.cnt_j = (uint8_T)k;

  /* :  if (cnt > 254) */
  if (((int32_T)Autopilot_300Hz_Function_1D_M->dwork.cnt_j) > 254) {
    /* :  cnt = uint8(0); */
    Autopilot_300Hz_Function_1D_M->dwork.cnt_j = 0U;
  }

  /* :  ATP_Flags = APloop; */
  /* :  ATP_Flags = bitset(ATP_Flags,4,reset); */
  /* :  autop_tel.ATP_CycleCntr = cnt; */
  rtY_autoptel_300Hz->ATP_CycleCntr = Autopilot_300Hz_Function_1D_M->dwork.cnt_j;

  /* :  autop_tel.ATP_Flags     = uint8(ATP_Flags); */
  if (((int32_T)rtb_reset) != 0) {
    rtY_autoptel_300Hz->ATP_Flags = (uint8_T)(rtU_guidin->AP_Loop | ((uint8_T)8));
  } else {
    rtY_autoptel_300Hz->ATP_Flags = (uint8_T)(((uint16_T)rtU_guidin->AP_Loop) &
      ((uint16_T)247));
  }

  /* :  autop_tel.ATP_SigmaF    = uint8(LIB_limit(0,255,SigmaF*750)); */
  P2d_lead = numNAN2 * 750.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_lead > 255.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_lead = 255.0;
  } else if (P2d_lead < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_lead = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_lead);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_SigmaF = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_SigmaF = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_Mach      = uint8(LIB_limit(0,255,mach*128)); */
  Z2d_lead = Autopilot_300Hz_Function_1D_M->dwork.MACH * 128.0;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 255.0) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = 255.0;
  } else if (Z2d_lead < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(Z2d_lead);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_Mach = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_Mach = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_Alpha     = int16(LIB_limit(-32768,32767,alpha*350)); */
  Z2d_lead = Autopilot_300Hz_Function_1D_M->dwork.ALPHA * 350.0;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = 32767.0;
  } else if (Z2d_lead < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(Z2d_lead);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_Alpha = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_Alpha = MAX_int16_T;
  }

  /* :  autop_tel.ATP_Beta      = int16(LIB_limit(-32768,32767,beta*350)); */
  Z2d_lead = Autopilot_300Hz_Function_1D_M->dwork.BETA * 350.0;

  /* 'LIB_limit:27' if (in > max) */
  if (Z2d_lead > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    Z2d_lead = 32767.0;
  } else if (Z2d_lead < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Z2d_lead = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(Z2d_lead);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_Beta = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_Beta = MAX_int16_T;
  }

  /* :  autop_tel.ATP_NANCntIn  = cntNANin; */
  rtY_autoptel_300Hz->ATP_NANCntIn =
    Autopilot_300Hz_Function_1D_M->dwork.countNaNin;

  /* :  autop_tel.ATP_NANCntOut = cntNANout; */
  rtY_autoptel_300Hz->ATP_NANCntOut =
    Autopilot_300Hz_Function_1D_M->dwork.countNaNout;

  /* :  autop_tel.ATP_wbxcmd    = int16(LIB_limit(-32768,32767,wbxcmd*5000)); */
  P2d_alpha *= 5000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_alpha = 32767.0;
  } else if (P2d_alpha < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_alpha = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_alpha);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_wbxcmd = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_wbxcmd = MAX_int16_T;
  }

  /* :  autop_tel.ATP_wbyicmd   = int16(LIB_limit(-32768,32767,wbyicmd*10000)); */
  P2d_alpha = P2d_sigma * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_alpha = 32767.0;
  } else if (P2d_alpha < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_alpha = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_alpha);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_wbyicmd = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_wbyicmd = MAX_int16_T;
  }

  /* :  autop_tel.ATP_wbypcmd   = int16(LIB_limit(-32768,32767,wbypcmd*10000)); */
  P2d_alpha = BetaP * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_alpha = 32767.0;
  } else if (P2d_alpha < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_alpha = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_alpha);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_wbypcmd = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_wbypcmd = MAX_int16_T;
  }

  /* :  autop_tel.ATP_wbzicmd   = int16(LIB_limit(-32768,32767,wbzicmd*10000)); */
  P2d_alpha = AlphaP * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_alpha = 32767.0;
  } else if (P2d_alpha < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_alpha = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_alpha);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_wbzicmd = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_wbzicmd = MAX_int16_T;
  }

  /* :  autop_tel.ATP_wbzpcmd   = int16(LIB_limit(-32768,32767,wbzpcmd*10000)); */
  P2d_alpha = machP * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_alpha > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_alpha = 32767.0;
  } else if (P2d_alpha < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_alpha = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_alpha);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_wbzpcmd = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_wbzpcmd = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DXCfil    = int16(LIB_limit(-32768,32767,DXCfilter*10000)); */
  machP = rtb_sigma * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 32767.0;
  } else if (machP < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DXCfil = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DXCfil = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DXCout    = int16(LIB_limit(-32768,32767,DXCout*10000)); */
  /* 'LIB_limit:27' if (in > max) */
  /* 'LIB_limit:31' else */
  /* 'LIB_limit:32' out = in; */
  P2d_lead = rt_roundd_snf(machR * 10000.0);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DXCout = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DXCout = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DZCfil    = int16(LIB_limit(-32768,32767,DZCfilter*10000)); */
  machP = alpha_in * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 32767.0;
  } else if (machP < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DZCfil = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DZCfil = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DZCout    = int16(LIB_limit(-32768,32767,DZCout*10000)); */
  machP = Z2d_sigma * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 32767.0;
  } else if (machP < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DZCout = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DZCout = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DYCfil    = int16(LIB_limit(-32768,32767,DYCfilter*10000)); */
  machP = Z2d_alpha * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 32767.0;
  } else if (machP < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DYCfil = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DYCfil = MAX_int16_T;
  }

  /* :  autop_tel.ATP_DYCout    = int16(LIB_limit(-32768,32767,DYCout*10000)); */
  machP = sigma_in * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 32767.0;
  } else if (machP < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 32768.0) {
    rtY_autoptel_300Hz->ATP_DYCout = (int16_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DYCout = MAX_int16_T;
  }

  /* :  autop_tel.ATP_intYlim   = uint8(LIB_limit(0,255,intYlim*100)); */
  machP = Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput * 100.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 255.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 255.0;
  } else if (machP < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_intYlim = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_intYlim = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_intZlim   = uint8(LIB_limit(0,255,intZlim*100)); */
  machP = Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput * 100.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 255.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 255.0;
  } else if (machP < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_intZlim = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_intZlim = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_DYClim    = uint8(LIB_limit(0,255,dyclim*750)); */
  machP = Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput * 750.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 255.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 255.0;
  } else if (machP < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_DYClim = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DYClim = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_DZClim    = uint8(LIB_limit(0,255,dzclim*750)); */
  machP = Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput * 750.0;

  /* 'LIB_limit:27' if (in > max) */
  if (machP > 255.0) {
    /* 'LIB_limit:28' out = max; */
    machP = 255.0;
  } else if (machP < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machP = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(machP);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_DZClim = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_DZClim = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_NpropX    = uint8(LIB_limit(0,255,NpropX*255.0)); */
  P2d_sigma = (1.0 - Autopilot_300Hz_Function_1D_M->dwork.AP.NCWroll) * 255.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_sigma > 255.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_sigma = 255.0;
  } else if (P2d_sigma < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_sigma = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_sigma);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_NpropX = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_NpropX = MAX_uint8_T;
  }

  /* :  autop_tel.ATP_Qratio    = uint8(LIB_limit(0,255,Qratio*50.0)); */
  P2d_sigma = Autopilot_300Hz_Function_1D_M->dwork.QRATIO * 50.0;

  /* 'LIB_limit:27' if (in > max) */
  if (P2d_sigma > 255.0) {
    /* 'LIB_limit:28' out = max; */
    P2d_sigma = 255.0;
  } else if (P2d_sigma < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    P2d_sigma = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  P2d_lead = rt_roundd_snf(P2d_sigma);
  if (P2d_lead < 256.0) {
    rtY_autoptel_300Hz->ATP_Qratio = (uint8_T)P2d_lead;
  } else {
    rtY_autoptel_300Hz->ATP_Qratio = MAX_uint8_T;
  }

  /* End of MATLAB Function: '<S1>/Modes and Telemetry' */

  /* Update for Memory: '<S1>/Memory' incorporates:
   *  MATLAB Function: '<S1>/Dynamic Fin Allocation'
   */
  Autopilot_300Hz_Function_1D_M->dwork.Memory_PreviousInput =
    Autopilot_300Hz_Function_1D_M->dwork.DXLim;

  /* Update for Memory: '<S1>/Memory1' */
  Autopilot_300Hz_Function_1D_M->dwork.Memory1_PreviousInput = numNAN3;

  /* Update for Memory: '<S1>/Memory2' */
  Autopilot_300Hz_Function_1D_M->dwork.Memory2_PreviousInput = SigmaP;

  /* MATLAB Function: '<S1>/Dynamic Fin Allocation' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   */
  numNAN1 = (0.67 * den) / Autopilot_300Hz_Function_1D_M->dwork.QRATIO;

  /* Update for Memory: '<S1>/Memory6' incorporates:
   *  MATLAB Function: '<S1>/Dynamic Fin Allocation'
   */
  Autopilot_300Hz_Function_1D_M->dwork.Memory6_PreviousInput = numNAN1;

  /* Update for Memory: '<S1>/Memory4' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/Dynamic Fin Allocation'
   */
  Autopilot_300Hz_Function_1D_M->dwork.Memory4_PreviousInput = (0.8 * den) /
    (Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wby *
     Autopilot_300Hz_Function_1D_M->dwork.QRATIO);

  /* Update for Memory: '<S1>/Memory5' */
  Autopilot_300Hz_Function_1D_M->dwork.Memory5_PreviousInput = numNAN1;

  /* Update for Memory: '<S1>/Memory3' incorporates:
   *  MATLAB Function: '<S1>/Autopilot Gains'
   *  MATLAB Function: '<S1>/Autopilot Input Conditioning'
   *  MATLAB Function: '<S1>/Dynamic Fin Allocation'
   */
  Autopilot_300Hz_Function_1D_M->dwork.Memory3_PreviousInput = (0.8 * den) /
    (Autopilot_300Hz_Function_1D_M->dwork.AP.Kp_wbz *
     Autopilot_300Hz_Function_1D_M->dwork.QRATIO);

  /* Switch: '<S29>/FixPt Switch' */
  if (rtb_FixPtSum1 > 10000000U) {
    /* Update for UnitDelay: '<S11>/Output' incorporates:
     *  Constant: '<S29>/Constant'
     */
    Autopilot_300Hz_Function_1D_M->dwork.Output_DSTATE = 0U;
  } else {
    /* Update for UnitDelay: '<S11>/Output' */
    Autopilot_300Hz_Function_1D_M->dwork.Output_DSTATE = rtb_FixPtSum1;
  }

  /* End of Switch: '<S29>/FixPt Switch' */
  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_release_at_outport_1' */
}

/* Model initialize function */
void Autopilot_300Hz_Function_1D_initialize(RT_MODEL *const
  Autopilot_300Hz_Function_1D_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_release_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Autopilot'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/Autopilot Input Conditioning' */
  Autopilot_300Hz_Function_1D_M->dwork.MACH = 0.8;
  Autopilot_300Hz_Function_1D_M->dwork.QRATIO = 1.0;

  /* SystemInitialize for MATLAB Function: '<S1>/Dynamic Fin Allocation' */
  /* :  Dcmd = [0 0 0 0]; */
  /* :  DXLim = 0.0873; */
  Autopilot_300Hz_Function_1D_M->dwork.DXLim = 0.0873;

  /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_release_at_outport_1' */
  /* :  countNaNout  = uint16(0); */
  /* :  dFin1 = 0.0; */
  /* :  dFin2 = 0.0; */
  /* :  dFin3 = 0.0; */
  /* :  dFin4 = 0.0; */
  /* :  AZfac = 0.99; */
  /* :  cnt = uint8(0); */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
