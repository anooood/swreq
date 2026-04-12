/*
 * File: Guidance_100Hz_Function_1D.h
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

#ifndef RTW_HEADER_Guidance_100Hz_Function_1D_h_
#define RTW_HEADER_Guidance_100Hz_Function_1D_h_
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef Guidance_100Hz_Function_1D_COMMON_INCLUDES_
# define Guidance_100Hz_Function_1D_COMMON_INCLUDES_
#include <stdio.h>
#include "rtwtypes.h"
#endif                         /* Guidance_100Hz_Function_1D_COMMON_INCLUDES_ */

#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

#include "Guidance_100Hz_1D.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_T_ana40_bus_
#define DEFINED_TYPEDEF_FOR_T_ana40_bus_

typedef struct {
  real_T p001;
  real_T p002;
  real_T p003;
  real_T p004;
  real_T p005;
  real_T p006;
  real_T p007;
  real_T p008;
  real_T p009;
  real_T p010;
  real_T p011;
  real_T p012;
  real_T p013;
  real_T p014;
  real_T p015;
  real_T p016;
  real_T p017;
  real_T p018;
  real_T p019;
  real_T p020;
  real_T p021;
  real_T p022;
  real_T p023;
  real_T p024;
  real_T p025;
  real_T p026;
  real_T p027;
  real_T p028;
  real_T p029;
  real_T p030;
  real_T p031;
  real_T p032;
  real_T p033;
  real_T p034;
  real_T p035;
  real_T p036;
  real_T p037;
  real_T p038;
  real_T p039;
  real_T p040;
} T_ana40_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_6qvaB88DLLAFHx6ej5JcWG_
#define DEFINED_TYPEDEF_FOR_struct_6qvaB88DLLAFHx6ej5JcWG_

typedef struct {
  real_T pi;
  real_T grav;
  real_T w_Earth;
  real_T R_eq;
  real_T Flat;
  real_T gamma;
  real_T R;
  real_T L;
  real_T Htrph;
  real_T Htrpa;
  real_T rho;
  real_T Vsnd;
  real_T P0;
  real_T ZK;
  real_T d2r;
  real_T r2d;
  real_T Hz2rad;
  real_T kmph2mps;
  real_T mps2kmph;
  real_T ft2m;
} struct_6qvaB88DLLAFHx6ej5JcWG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_Nfy90zz6zGRJDmnb4iAwuF_
#define DEFINED_TYPEDEF_FOR_struct_Nfy90zz6zGRJDmnb4iAwuF_

typedef struct {
  real_T simdelt;
  real_T hilsdelt;
  real_T AC_Mirage2000;
  real_T AC_F16;
  real_T AC_UAV;
  real_T W_1D;
  real_T W_2A;
  real_T NOM;
  real_T RND;
  real_T FXD;
  real_T MP_OPPORTUNITY;
  real_T MP_MISSIONLOAD;
  real_T MC_OPER;
  real_T GD_PROG;
  real_T MC_CFO;
  real_T AP_PROG1;
  real_T AP_PROG2;
  real_T GD_PROF1;
  real_T GD_PROF2;
  real_T GD_PROF3;
  real_T QAT2S_PROF1;
  real_T QAT2S_PROF2;
  real_T QAT2S_PROF3;
  real_T QAT2S_PROF5;
  real_T QAT2S_PROF6;
  real_T QAT3S_PROF4;
  real_T QAT3S_PROF5;
  real_T PAT2S_PROF1;
  real_T PAT2S_PROF2;
  real_T AP_DEFL1;
  real_T Undef;
  real_T Mk81;
  real_T Mk82;
  real_T GPS;
  real_T SAL;
  real_T IIR;
  real_T fail_DeadWeapon;
  real_T fail_BrokenAirframe;
  real_T fail_MCPhold;
  real_T fail_MCPlimits;
  real_T fail_MPdata_error;
  real_T fail_GuidHold;
  real_T fail_GuidErrors;
  real_T fail_SeekHold;
  real_T fail_SeekErrors;
  real_T fail_IMUHold;
  real_T fail_IMUErrors;
  real_T fail_IMUDrift;
  real_T fail_ServoLocked;
  real_T fail_ServoFree;
  real_T fail_ServoLimits;
  real_T fail_ServoHallSens;
  real_T fail_AeroResponsiveness;
  real_T fail_GNSS;
} struct_Nfy90zz6zGRJDmnb4iAwuF;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_LoNCOuZQnczdeEXA6Cufb_
#define DEFINED_TYPEDEF_FOR_struct_LoNCOuZQnczdeEXA6Cufb_

typedef struct {
  real_T delt;
  real_T MODE;
  real_T glidespeed;
  real_T LOSRflag;
  real_T plotpath;
  real_T LARalt;
  real_T LARradius;
  real_T LARtimeimp;
  real_T LARdownrange;
  real_T LARcrossrange;
} struct_LoNCOuZQnczdeEXA6Cufb;

#endif

/* Custom Type definition for MATLAB Function: '<S1>/Guidance Controller' */
#ifndef struct_tag_szkrz2cY4hHc7jLma1AukxC
#define struct_tag_szkrz2cY4hHc7jLma1AukxC

struct tag_szkrz2cY4hHc7jLma1AukxC
{
  real_T z;
};

#endif                                 /*struct_tag_szkrz2cY4hHc7jLma1AukxC*/

#ifndef typedef_szkrz2cY4hHc7jLma1AukxC
#define typedef_szkrz2cY4hHc7jLma1AukxC

typedef struct tag_szkrz2cY4hHc7jLma1AukxC szkrz2cY4hHc7jLma1AukxC;

#endif                                 /*typedef_szkrz2cY4hHc7jLma1AukxC*/

#ifndef struct_tag_sb6O280JGWZ46sxhPtS13aC
#define struct_tag_sb6O280JGWZ46sxhPtS13aC

struct tag_sb6O280JGWZ46sxhPtS13aC
{
  T_GeographicPos pos_g;
  T_Cartesian pos_e;
  T_Cartesian vel_e;
  T_Euler att_bn;
  szkrz2cY4hHc7jLma1AukxC fib_e;
};

#endif                                 /*struct_tag_sb6O280JGWZ46sxhPtS13aC*/

#ifndef typedef_sb6O280JGWZ46sxhPtS13aC
#define typedef_sb6O280JGWZ46sxhPtS13aC

typedef struct tag_sb6O280JGWZ46sxhPtS13aC sb6O280JGWZ46sxhPtS13aC;

#endif                                 /*typedef_sb6O280JGWZ46sxhPtS13aC*/

#ifndef struct_tag_shkoXMvGDT09CSMPYwHF4WB
#define struct_tag_shkoXMvGDT09CSMPYwHF4WB

struct tag_shkoXMvGDT09CSMPYwHF4WB
{
  real_T gbxm;
  real_T gbym;
  real_T gbzm;
  real_T wbxm;
  real_T wbym;
  real_T wbzm;
};

#endif                                 /*struct_tag_shkoXMvGDT09CSMPYwHF4WB*/

#ifndef typedef_shkoXMvGDT09CSMPYwHF4WB
#define typedef_shkoXMvGDT09CSMPYwHF4WB

typedef struct tag_shkoXMvGDT09CSMPYwHF4WB shkoXMvGDT09CSMPYwHF4WB;

#endif                                 /*typedef_shkoXMvGDT09CSMPYwHF4WB*/

#ifndef struct_tag_sVUEBEgeCpQlxFVEgYjRFMH
#define struct_tag_sVUEBEgeCpQlxFVEgYjRFMH

struct tag_sVUEBEgeCpQlxFVEgYjRFMH
{
  real_T rho;
  real_T pressure;
  real_T temp0;
  real_T temp;
  real_T Vsound;
  real_T FPthem;
  real_T FPpsim;
  real_T mach;
  real_T alpha;
  real_T beta;
  real_T Vairspeed;
  real_T Vnav;
  real_T Vbody[3];
  real_T Q;
  real_T Qratio;
  real_T GRAVbody[3];
  real_T atmbeta;
  real_T BIAS_alpha;
  real_T BIAS_beta;
};

#endif                                 /*struct_tag_sVUEBEgeCpQlxFVEgYjRFMH*/

#ifndef typedef_sVUEBEgeCpQlxFVEgYjRFMH
#define typedef_sVUEBEgeCpQlxFVEgYjRFMH

typedef struct tag_sVUEBEgeCpQlxFVEgYjRFMH sVUEBEgeCpQlxFVEgYjRFMH;

#endif                                 /*typedef_sVUEBEgeCpQlxFVEgYjRFMH*/

#ifndef struct_tag_sETPRuSGTG6V05GSmeYzNjH
#define struct_tag_sETPRuSGTG6V05GSmeYzNjH

struct tag_sETPRuSGTG6V05GSmeYzNjH
{
  real_T AZy;
  real_T AZz;
  real_T gbycD0;
  real_T gbzcD;
  real_T inty;
  real_T intz;
  real_T gbzc_unl;
  real_T gbyc_unl;
  real_T gbzc_FOV;
  real_T gbyc_FOV;
  real_T intz_FOV;
  real_T inty_FOV;
  real_T gizc;
  real_T giyc;
  real_T eta_int;
  real_T eps_int;
  real_T phic_hold;
  real_T phi_int;
  real_T phi_gic;
  real_T phic_prev;
  real_T AZphi;
  uint8_T gmode_sw;
  uint8_T PurePropNav;
  real_T phi_attack;
  real_T phim;
  uint8_T verticaldive;
  real_T dT;
  real_T GndRge;
  uint8_T forcegmode5;
  real_T gb_ratio;
  real_T gb_ratio_prev;
  real_T giyc_int;
  uint8_T navctrlreset;
};

#endif                                 /*struct_tag_sETPRuSGTG6V05GSmeYzNjH*/

#ifndef typedef_sETPRuSGTG6V05GSmeYzNjH
#define typedef_sETPRuSGTG6V05GSmeYzNjH

typedef struct tag_sETPRuSGTG6V05GSmeYzNjH sETPRuSGTG6V05GSmeYzNjH;

#endif                                 /*typedef_sETPRuSGTG6V05GSmeYzNjH*/

#ifndef struct_tag_sSZzX5nI8YD60UjueDawhgE
#define struct_tag_sSZzX5nI8YD60UjueDawhgE

struct tag_sSZzX5nI8YD60UjueDawhgE
{
  real_T ClCd_M;
  real_T alpha_M;
  real_T CNgrav;
  real_T M;
  real_T Mopt;
  real_T Vopt;
  real_T Qopt;
  real_T Mglide_opt;
  real_T FPopt;
  real_T FPopt0;
  real_T Cd0;
  real_T FPCmd0;
  real_T FPcmdD;
  real_T FPerrD;
  real_T FPint;
  real_T FPthec;
  real_T Cl;
  real_T GLim;
  real_T GLimZlo;
  real_T GLimZup;
  real_T GLimYlo;
  real_T GLimYup;
  real_T GLimZup_prev;
  real_T GLimZlo_prev;
  real_T GLimYup_prev;
  real_T GLimYlo_prev;
};

#endif                                 /*struct_tag_sSZzX5nI8YD60UjueDawhgE*/

#ifndef typedef_sSZzX5nI8YD60UjueDawhgE
#define typedef_sSZzX5nI8YD60UjueDawhgE

typedef struct tag_sSZzX5nI8YD60UjueDawhgE sSZzX5nI8YD60UjueDawhgE;

#endif                                 /*typedef_sSZzX5nI8YD60UjueDawhgE*/

#ifndef struct_tag_sL6LJlPlxhdTxZzXh5NTaQC
#define struct_tag_sL6LJlPlxhdTxZzXh5NTaQC

struct tag_sL6LJlPlxhdTxZzXh5NTaQC
{
  int32_T intNumBits;
};

#endif                                 /*struct_tag_sL6LJlPlxhdTxZzXh5NTaQC*/

#ifndef typedef_sL6LJlPlxhdTxZzXh5NTaQC
#define typedef_sL6LJlPlxhdTxZzXh5NTaQC

typedef struct tag_sL6LJlPlxhdTxZzXh5NTaQC sL6LJlPlxhdTxZzXh5NTaQC;

#endif                                 /*typedef_sL6LJlPlxhdTxZzXh5NTaQC*/

#ifndef struct_tag_sFMpPTQ95PcZEskZd3p4FcD
#define struct_tag_sFMpPTQ95PcZEskZd3p4FcD

struct tag_sFMpPTQ95PcZEskZd3p4FcD
{
  uint16_T GUI_iW[1000];
  int8_T GUI_solution[1000];
  uint8_T GUI_inccnt[1000];
  real32_T GUI_ftime[1000];
  uint16_T GUI_Rarc[1000];
  uint16_T GUI_RarcW[1000];
  uint16_T GUI_Rterm[1000];
  int16_T GUI_Km[1000];
  uint16_T GUI_RtermT[1000];
  uint16_T GUI_RtermW[1000];
  int16_T GUI_LT[1000];
  int16_T GUI_LW[1000];
  real32_T GUI_Mglide[1000];
  real32_T GUI_FPazi[1000];
  real32_T GUI_ratio[1000];
  int16_T GUI_ImpAng[1000];
  int16_T GUI_AttHeading[1000];
  int16_T GUI_intsctTx[1000];
  int16_T GUI_intsctTy[1000];
  int16_T GUI_intsctTz[1000];
  int16_T GUI_intsctWx[1000];
  int16_T GUI_intsctWy[1000];
  int16_T GUI_intsctWz[1000];
  int16_T GUI_Rtox[1000];
  int16_T GUI_Rtoy[1000];
  int16_T GUI_Rtoz[1000];
  int16_T GUI_Rtex[1000];
  int16_T GUI_Rtey[1000];
  int16_T GUI_Rtez[1000];
  int16_T GUI_Rwox[1000];
  int16_T GUI_Rwoy[1000];
  int16_T GUI_Rwoz[1000];
  int16_T GUI_Rwex[1000];
  int16_T GUI_Rwey[1000];
  int16_T GUI_Rwez[1000];
  int16_T GUI_Rmx[1000];
  int16_T GUI_Rmy[1000];
  int16_T GUI_Rmz[1000];
  int16_T GUI_a0Wx[1000];
  int16_T GUI_a0Wy[1000];
  int16_T GUI_a0Wz[1000];
  int16_T GUI_a0Tx[1000];
  int16_T GUI_a0Ty[1000];
  int16_T GUI_a0Tz[1000];
  uint16_T GUI_spare[1000];
  int16_T GUI_tgt0x[1000];
  int16_T GUI_tgt0y[1000];
  int16_T GUI_tgt0z[1000];
  uint16_T GUI_altcnt[1000];
  uint16_T GUI_R2buntREQ[1000];
  uint16_T GUI_R2buntAVL[1000];
  uint16_T GUI_RarcW_cmp[1000];
  int16_T GUI_theFP3D[1000];
  int16_T GUI_theFP2D[1000];
  int16_T GUI_theErr[1000];
};

#endif                                 /*struct_tag_sFMpPTQ95PcZEskZd3p4FcD*/

#ifndef typedef_sFMpPTQ95PcZEskZd3p4FcD
#define typedef_sFMpPTQ95PcZEskZd3p4FcD

typedef struct tag_sFMpPTQ95PcZEskZd3p4FcD sFMpPTQ95PcZEskZd3p4FcD;

#endif                                 /*typedef_sFMpPTQ95PcZEskZd3p4FcD*/

#ifndef struct_tag_sqfeYN8Hrouv8AbOaDO2afG
#define struct_tag_sqfeYN8Hrouv8AbOaDO2afG

struct tag_sqfeYN8Hrouv8AbOaDO2afG
{
  real_T time20Hz;
  uint16_T IW;
  uint16_T IW_last;
};

#endif                                 /*struct_tag_sqfeYN8Hrouv8AbOaDO2afG*/

#ifndef typedef_sqfeYN8Hrouv8AbOaDO2afG
#define typedef_sqfeYN8Hrouv8AbOaDO2afG

typedef struct tag_sqfeYN8Hrouv8AbOaDO2afG sqfeYN8Hrouv8AbOaDO2afG;

#endif                                 /*typedef_sqfeYN8Hrouv8AbOaDO2afG*/

#ifndef struct_tag_s28FIl9ilY9eNQKo2dEzV1C
#define struct_tag_s28FIl9ilY9eNQKo2dEzV1C

struct tag_s28FIl9ilY9eNQKo2dEzV1C
{
  real_T Kfp;
  real_T Ki;
  real_T Kp;
  real_T Kv;
};

#endif                                 /*struct_tag_s28FIl9ilY9eNQKo2dEzV1C*/

#ifndef typedef_s28FIl9ilY9eNQKo2dEzV1C
#define typedef_s28FIl9ilY9eNQKo2dEzV1C

typedef struct tag_s28FIl9ilY9eNQKo2dEzV1C s28FIl9ilY9eNQKo2dEzV1C;

#endif                                 /*typedef_s28FIl9ilY9eNQKo2dEzV1C*/

#ifndef struct_tag_sbS2o27mNjUInq39Qf98jAF
#define struct_tag_sbS2o27mNjUInq39Qf98jAF

struct tag_sbS2o27mNjUInq39Qf98jAF
{
  real_T Rarc_R;
  real_T Rterm;
  real_T machglide;
  real_T psiR_Thr;
  real_T psi_Engage;
  real_T do_plot;
  real_T matPROG1[266];
  real_T matPROG2[917];
  real_T matDEFL[266];
  real_T matDEFAULT[14];
};

#endif                                 /*struct_tag_sbS2o27mNjUInq39Qf98jAF*/

#ifndef typedef_sbS2o27mNjUInq39Qf98jAF
#define typedef_sbS2o27mNjUInq39Qf98jAF

typedef struct tag_sbS2o27mNjUInq39Qf98jAF sbS2o27mNjUInq39Qf98jAF;

#endif                                 /*typedef_sbS2o27mNjUInq39Qf98jAF*/

#ifndef struct_tag_s7MzcK0J2kNr812eOGEV7PE
#define struct_tag_s7MzcK0J2kNr812eOGEV7PE

struct tag_s7MzcK0J2kNr812eOGEV7PE
{
  real_T Rarc_T;
  real_T Rapr_T[3];
  real_T Oarc_T[3];
  real_T Rarc_W;
  real_T Rapr_Rm[3];
  real_T Oarc_W[3];
};

#endif                                 /*struct_tag_s7MzcK0J2kNr812eOGEV7PE*/

#ifndef typedef_s7MzcK0J2kNr812eOGEV7PE
#define typedef_s7MzcK0J2kNr812eOGEV7PE

typedef struct tag_s7MzcK0J2kNr812eOGEV7PE s7MzcK0J2kNr812eOGEV7PE;

#endif                                 /*typedef_s7MzcK0J2kNr812eOGEV7PE*/

#ifndef struct_tag_ssOdl51LB8ZQlaB7SirUWkE
#define struct_tag_ssOdl51LB8ZQlaB7SirUWkE

struct tag_ssOdl51LB8ZQlaB7SirUWkE
{
  uint8_T once;
  real_T Rref[3];
  real_T Rrel[3];
  real_T Recef[3];
  real_T Rned[3];
  real_T Vned[3];
  real_T Aned[3];
  real_T Rfpath[3];
  real_T Roffset;
  s7MzcK0J2kNr812eOGEV7PE ArcState;
  real_T Kglide;
  real_T Kglide_prev;
  real_T Qfac;
  real_T Rby;
  real_T Rbz;
  real_T Vby;
  real_T Vbz;
  real_T Aby;
  real_T Abz;
  real_T AZcori;
  real_T dRlim;
  real_T dVlim;
  real_T Rby0_prev;
  real_T Rbz0_prev;
  real_T Vby0_prev;
  real_T Vbz0_prev;
  real_T RM[3];
  real_T RWE[3];
  real_T RMW0[3];
  real_T fadeFac;
};

#endif                                 /*struct_tag_ssOdl51LB8ZQlaB7SirUWkE*/

#ifndef typedef_ssOdl51LB8ZQlaB7SirUWkE
#define typedef_ssOdl51LB8ZQlaB7SirUWkE

typedef struct tag_ssOdl51LB8ZQlaB7SirUWkE ssOdl51LB8ZQlaB7SirUWkE;

#endif                                 /*typedef_ssOdl51LB8ZQlaB7SirUWkE*/

#ifndef struct_tag_sFYrLWvx1kVFonG2Nu0GNkH
#define struct_tag_sFYrLWvx1kVFonG2Nu0GNkH

struct tag_sFYrLWvx1kVFonG2Nu0GNkH
{
  uint8_T log_FP;
  uint8_T once;
  uint8_T reset;
  uint8_T bunt_arc;
  real_T reset3cnt;
  real_T Rlim_lo;
  real_T Roff_intg;
  real_T ftime_step;
  real_T mintime;
  uint8_T FPbunt_cnt;
  int8_T solution;
  real_T Mtgt0;
  real_T Mglide;
  real_T psiR;
  real_T FPazi;
  real_T Lfpath2bunt;
  real_T Lfpath2tgt;
  real_T ratio;
  real_T rge2bunt;
  real_T PosRst;
  uint8_T latch[2];
  real_T Ut0[3];
  real_T Ut8[3];
  real_T Ut[3];
  real_T Kglide;
  real_T AprAngErr;
  T_Cartesian re0;
  T_Cartesian mpos0;
  T_Cartesian Rto;
  T_Cartesian Rte;
  T_Cartesian Rwo;
  T_Cartesian Rwe;
  T_Cartesian Rmwo;
  T_Cartesian Rmto;
  T_Cartesian Rm;
  real_T RarcT;
  T_Cartesian OarcT;
  real_T RarcW;
  T_Cartesian OarcW;
  real_T AZrefL;
  real_T AZref;
  real_T Time8;
  real_T Umis[3];
  real_T Rmis0[3];
  real_T Umis0[3];
};

#endif                                 /*struct_tag_sFYrLWvx1kVFonG2Nu0GNkH*/

#ifndef typedef_sFYrLWvx1kVFonG2Nu0GNkH
#define typedef_sFYrLWvx1kVFonG2Nu0GNkH

typedef struct tag_sFYrLWvx1kVFonG2Nu0GNkH sFYrLWvx1kVFonG2Nu0GNkH;

#endif                                 /*typedef_sFYrLWvx1kVFonG2Nu0GNkH*/

#ifndef struct_tag_sPLFBkBfmjj013qOjLWm7GC
#define struct_tag_sPLFBkBfmjj013qOjLWm7GC

struct tag_sPLFBkBfmjj013qOjLWm7GC
{
  real_T NED2BODY[9];
  real_T ECEF2NED[9];
};

#endif                                 /*struct_tag_sPLFBkBfmjj013qOjLWm7GC*/

#ifndef typedef_sPLFBkBfmjj013qOjLWm7GC
#define typedef_sPLFBkBfmjj013qOjLWm7GC

typedef struct tag_sPLFBkBfmjj013qOjLWm7GC sPLFBkBfmjj013qOjLWm7GC;

#endif                                 /*typedef_sPLFBkBfmjj013qOjLWm7GC*/

#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  char_T Value[5];
} struct_T;

#endif                                 /*typedef_struct_T*/

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  sFMpPTQ95PcZEskZd3p4FcD guidtel_fpath;/* '<S1>/Guidance Controller' */
  sbS2o27mNjUInq39Qf98jAF guid_settings;/* '<S1>/Guidance Controller' */
  sFYrLWvx1kVFonG2Nu0GNkH TRACK;       /* '<S1>/Guidance Controller' */
  ssOdl51LB8ZQlaB7SirUWkE clos;        /* '<S1>/Guidance Controller' */
  sETPRuSGTG6V05GSmeYzNjH guidcmds;    /* '<S1>/Guidance Controller' */
  sSZzX5nI8YD60UjueDawhgE OPT_FP;      /* '<S1>/Guidance Controller' */
  T_GUID_Guid_Tel20HzA guidtel20HzA;   /* '<S1>/Guidance Controller' */
  T_GUID_Guid2DATA guid2DATA;          /* '<S1>/Guidance Controller' */
  sVUEBEgeCpQlxFVEgYjRFMH ENV;         /* '<S1>/Guidance Controller' */
  T_GUID_Guid_Out_IF1003 guid2AP;      /* '<S1>/Guidance Controller' */
  T_GUID_Guid_Tel20HzB guidtel20HzB;   /* '<S1>/Guidance Controller' */
  sb6O280JGWZ46sxhPtS13aC NAV_in;      /* '<S1>/Guidance Controller' */
  shkoXMvGDT09CSMPYwHF4WB IMU_in;      /* '<S1>/Guidance Controller' */
  T_GUID_Guid_Tel100Hz guidtel100Hz;   /* '<S1>/Guidance Controller' */
  s28FIl9ilY9eNQKo2dEzV1C gains;       /* '<S1>/Guidance Controller' */
  T_Cartesian TPosNED_uncond_prev;     /* '<S1>/Guidance Controller' */
  T_Cartesian MPosNED_prev;            /* '<S1>/Guidance Controller' */
  T_Cartesian MVelNED_prev;            /* '<S1>/Guidance Controller' */
  T_GUID_FlightConditions FC_in;       /* '<S1>/Guidance Controller' */
  sqfeYN8Hrouv8AbOaDO2afG telem;       /* '<S1>/Guidance Controller' */
  T_GUID_GuidCtrlFlags guidFLAGS;      /* '<S1>/Guidance Controller' */
  real_T WindTab_Alt[39];              /* '<S1>/Guidance Controller' */
  real_T WindTab_VN[39];               /* '<S1>/Guidance Controller' */
  real_T WindTab_VE[39];               /* '<S1>/Guidance Controller' */
  real_T delt;                         /* '<S1>/Guidance Controller' */
  real_T ftime0;                       /* '<S1>/Guidance Controller' */
  real_T nav_time;                     /* '<S1>/Guidance Controller' */
  real_T t_noGNSS;                     /* '<S1>/Guidance Controller' */
  real_T GndRge2Tgt;                   /* '<S1>/Guidance Controller' */
  real_T GndPosN;                      /* '<S1>/Guidance Controller' */
  real_T GndPosE;                      /* '<S1>/Guidance Controller' */
  real_T t_debug;                      /* '<S1>/Guidance Controller' */
  real_T wingsopen_timeout;            /* '<S1>/Guidance Controller' */
  uint32_T newmeas_cnt;                /* '<S1>/Guidance Controller' */
  uint8_T guidmode;                    /* '<S1>/Guidance Controller' */
  uint8_T opportunity_mode;            /* '<S1>/Guidance Controller' */
  boolean_T once_not_empty;            /* '<S1>/Guidance Controller' */
  boolean_T delt_not_empty;            /* '<S1>/Guidance Controller' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: def
   * Referenced by: '<S1>/Guidance Controller'
   */
  struct_Nfy90zz6zGRJDmnb4iAwuF GuidanceController_def;

  /* Expression: cnsts
   * Referenced by: '<S1>/Guidance Controller'
   */
  struct_6qvaB88DLLAFHx6ej5JcWG GuidanceController_cnsts;
} GUIDConstP;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Constant parameters (default storage) */
extern const GUIDConstP rtGUIDConstP;

/* Model entry point functions */
extern void Guidance_100Hz_Function_1D_initialize(RT_MODEL *const
  Guidance_100Hz_Function_1D_M);

/* Exported entry point function */
extern void Guidance_100Hz_Function_main(RT_MODEL *const
  Guidance_100Hz_Function_1D_M, real_T rtU_SysTimein, T_GUID_Discretes
  *rtU_Discretesin, T_GUID_TTKF_Out_IF1002 *rtU_ttkfin, uint8_T rtU_navmodein,
  T_NAV_NavigationState *rtU_navstatein, T_NAV_GenericInertialNavigationSensor
  *rtU_IMUin, T_GUID_MissionPlanning *rtU_MPin, T_GUID_WindParams *rtU_Windin,
  T_GUID_FlightConditions *rtU_Pitotin, T_Weapon_MCP_DATA_Guid *rtU_DATAguid,
  T_GUID_WeaponConfig *rtU_WeaCfgin, T_GUID_TestConfig *rtU_TestCfgin,
  T_GUID_Guid_Out_IF1003 *rtY_guid2AP, T_GUID_GuidCtrlFlags *rtY_guidCtrlFlags,
  T_GUID_Guid2DATA *rtY_guid2DATA, T_GUID_Guid_Tel100Hz *rtY_guidtel_100Hz,
  T_GUID_Guid_Tel20HzA *rtY_guidtel_20HzA, T_GUID_Guid_Tel20HzB
  *rtY_guidtel_20HzB);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Guidance_100Hz_Function_1D'
 * '<S1>'   : 'Guidance_100Hz_Function_1D/Guidance'
 * '<S2>'   : 'Guidance_100Hz_Function_1D/Guidance/Guidance Controller'
 */
#endif                            /* RTW_HEADER_Guidance_100Hz_Function_1D_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
