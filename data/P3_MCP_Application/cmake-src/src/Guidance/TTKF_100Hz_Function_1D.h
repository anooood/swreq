/*
 * File: TTKF_100Hz_Function_1D.h
 *
 * Code generated for Simulink model 'TTKF_100Hz_Function_1D'.
 *
 * Model version                  : 1.1467
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:33:35 2023
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

#ifndef RTW_HEADER_TTKF_100Hz_Function_1D_h_
#define RTW_HEADER_TTKF_100Hz_Function_1D_h_
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef TTKF_100Hz_Function_1D_COMMON_INCLUDES_
# define TTKF_100Hz_Function_1D_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                             /* TTKF_100Hz_Function_1D_COMMON_INCLUDES_ */

#include "rt_nonfinite.h"
#include "rtGetInf.h"

#include "TTKF_100Hz_1D.h"

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

#ifndef DEFINED_TYPEDEF_FOR_struct_CBcWDs8VhbfWjt6mqMD31D_
#define DEFINED_TYPEDEF_FOR_struct_CBcWDs8VhbfWjt6mqMD31D_

typedef struct {
  real_T delt;
  real_T Alin_skr;
  real_T Bele_skr;
} struct_CBcWDs8VhbfWjt6mqMD31D;

#endif

/* Custom Type definition for MATLAB Function: '<S1>/Guidance Estimator' */
#ifndef struct_tag_spGKsvEVm7uA89hv31XX4LH
#define struct_tag_spGKsvEVm7uA89hv31XX4LH

struct tag_spGKsvEVm7uA89hv31XX4LH
{
  uint32_T MissingPlacement;
  uint32_T ComparisonMethod;
};

#endif                                 /*struct_tag_spGKsvEVm7uA89hv31XX4LH*/

#ifndef typedef_spGKsvEVm7uA89hv31XX4LH
#define typedef_spGKsvEVm7uA89hv31XX4LH

typedef struct tag_spGKsvEVm7uA89hv31XX4LH spGKsvEVm7uA89hv31XX4LH;

#endif                                 /*typedef_spGKsvEVm7uA89hv31XX4LH*/

#ifndef struct_tag_se4ARCDdZxEuFIKZpTZ14jE
#define struct_tag_se4ARCDdZxEuFIKZpTZ14jE

struct tag_se4ARCDdZxEuFIKZpTZ14jE
{
  real_T hP0;
  real_T vP0;
  real_T hV0;
  real_T vV0;
  real_T hPN;
  real_T vPN;
  real_T hVN;
  real_T vVN;
  real_T Q[36];
};

#endif                                 /*struct_tag_se4ARCDdZxEuFIKZpTZ14jE*/

#ifndef typedef_se4ARCDdZxEuFIKZpTZ14jE
#define typedef_se4ARCDdZxEuFIKZpTZ14jE

typedef struct tag_se4ARCDdZxEuFIKZpTZ14jE se4ARCDdZxEuFIKZpTZ14jE;

#endif                                 /*typedef_se4ARCDdZxEuFIKZpTZ14jE*/

#ifndef struct_tag_sjaNUYpKpc2EHQMOiEPkrKE
#define struct_tag_sjaNUYpKpc2EHQMOiEPkrKE

struct tag_sjaNUYpKpc2EHQMOiEPkrKE
{
  real_T mode;
  real_T freq;
  real_T talt0;
  real_T trex;
  real_T trey;
  real_T trez;
  real_T tvex;
  real_T tvey;
  real_T tvez;
  real_T malt;
  real_T mrex;
  real_T mrey;
  real_T mrez;
  real_T mvex;
  real_T mvey;
  real_T mvez;
  real_T mphi;
  real_T mthe;
  real_T mpsi;
  real_T dcm_eb[9];
  real_T dcm_es[9];
  real_T epsm;
  real_T etam;
  real_T rgem;
  real_T wbxm;
  real_T wbym;
  real_T wbzm;
  uint8_T ValidGNSSNav;
};

#endif                                 /*struct_tag_sjaNUYpKpc2EHQMOiEPkrKE*/

#ifndef typedef_sjaNUYpKpc2EHQMOiEPkrKE
#define typedef_sjaNUYpKpc2EHQMOiEPkrKE

typedef struct tag_sjaNUYpKpc2EHQMOiEPkrKE sjaNUYpKpc2EHQMOiEPkrKE;

#endif                                 /*typedef_sjaNUYpKpc2EHQMOiEPkrKE*/

#ifndef struct_tag_sfl7P65dOznAw2EcCMFPbAE
#define struct_tag_sfl7P65dOznAw2EcCMFPbAE

struct tag_sfl7P65dOznAw2EcCMFPbAE
{
  real_T A[36];
  real_T X[6];
  real_T H[18];
  real_T P[36];
  real_T R[9];
  real_T K[18];
  real_T rex;
  real_T rey;
  real_T rez;
  real_T vex;
  real_T vey;
  real_T vez;
  real_T rge;
  real_T the_b;
  real_T psi_b;
  real_T the_g;
  real_T psi_g;
  real_T them_g;
  real_T psim_g;
  real_T eps;
  real_T eta;
  real_T dthe_b;
  real_T dpsi_b;
  real_T rge_clamp;
  real_T conf;
  real_T measweight;
  real_T nserge;
  real_T AZ;
  real_T eps_median;
  real_T the_median;
  real_T the_lin_up;
  real_T the_lin_lo;
  real_T eta_median;
  real_T psi_median;
  real_T psi_lin_up;
  real_T psi_lin_lo;
  real_T delpsi;
  real_T delthe;
  uint8_T no_samples;
  uint8_T i_025;
  uint8_T i_075;
  uint8_T k;
  real_T eps_diff;
  real_T eta_diff;
  real_T eps_sample[20];
  real_T eta_sample[20];
  real_T t_fillsample;
  real_T i;
  real_T time_vec[10];
  real_T tposn_vec[10];
  real_T tpose_vec[10];
  real_T Wfac[10];
};

#endif                                 /*struct_tag_sfl7P65dOznAw2EcCMFPbAE*/

#ifndef typedef_sfl7P65dOznAw2EcCMFPbAE
#define typedef_sfl7P65dOznAw2EcCMFPbAE

typedef struct tag_sfl7P65dOznAw2EcCMFPbAE sfl7P65dOznAw2EcCMFPbAE;

#endif                                 /*typedef_sfl7P65dOznAw2EcCMFPbAE*/

#ifndef struct_tag_sJCxfmxS8gBOONUZjbjUd9E
#define struct_tag_sJCxfmxS8gBOONUZjbjUd9E

struct tag_sJCxfmxS8gBOONUZjbjUd9E
{
  boolean_T CaseSensitivity;
  boolean_T StructExpand;
  char_T PartialMatching[6];
  boolean_T IgnoreNulls;
};

#endif                                 /*struct_tag_sJCxfmxS8gBOONUZjbjUd9E*/

#ifndef typedef_sJCxfmxS8gBOONUZjbjUd9E
#define typedef_sJCxfmxS8gBOONUZjbjUd9E

typedef struct tag_sJCxfmxS8gBOONUZjbjUd9E sJCxfmxS8gBOONUZjbjUd9E;

#endif                                 /*typedef_sJCxfmxS8gBOONUZjbjUd9E*/

#ifndef struct_emxArray_int32_T_20
#define struct_emxArray_int32_T_20

struct emxArray_int32_T_20
{
  int32_T data[20];
  int32_T size;
};

#endif                                 /*struct_emxArray_int32_T_20*/

#ifndef typedef_emxArray_int32_T_20
#define typedef_emxArray_int32_T_20

typedef struct emxArray_int32_T_20 emxArray_int32_T_20;

#endif                                 /*typedef_emxArray_int32_T_20*/

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  sfl7P65dOznAw2EcCMFPbAE KF_internals;/* '<S1>/Guidance Estimator' */
  se4ARCDdZxEuFIKZpTZ14jE setup;       /* '<S1>/Guidance Estimator' */
  sjaNUYpKpc2EHQMOiEPkrKE input;       /* '<S1>/Guidance Estimator' */
  T_GUID_TTKF_Out_IF1002 ttkfout;      /* '<S1>/Guidance Estimator' */
  T_GUID_TTKF_Tel100Hz ttkftel;        /* '<S1>/Guidance Estimator' */
  T_Quaternion temp_q;                 /* '<S1>/Guidance Estimator' */
  T_Cartesian mre0;                    /* '<S1>/Guidance Estimator' */
  T_GeographicPos temp_g;              /* '<S1>/Guidance Estimator' */
  real_T delX[18];                     /* '<S1>/Guidance Estimator' */
  real_T delt;                         /* '<S1>/Guidance Estimator' */
  real_T ftime0;                       /* '<S1>/Guidance Estimator' */
  real_T time;                         /* '<S1>/Guidance Estimator' */
  real_T time_prev;                    /* '<S1>/Guidance Estimator' */
  real_T nav_time;                     /* '<S1>/Guidance Estimator' */
  real_T epsm_prev;                    /* '<S1>/Guidance Estimator' */
  real_T etam_prev;                    /* '<S1>/Guidance Estimator' */
  real_T NavAlt0;                      /* '<S1>/Guidance Estimator' */
  real_T t_no_lock;                    /* '<S1>/Guidance Estimator' */
  real_T t_no_input;                   /* '<S1>/Guidance Estimator' */
  real_T t_clamp;                      /* '<S1>/Guidance Estimator' */
  real_T lat_prev;                     /* '<S1>/Guidance Estimator' */
  real_T relx_prev;                    /* '<S1>/Guidance Estimator' */
  real_T rely_prev;                    /* '<S1>/Guidance Estimator' */
  uint32_T state[625];                 /* '<S1>/Guidance Estimator' */
  boolean_T once_not_empty;            /* '<S1>/Guidance Estimator' */
  boolean_T delt_not_empty;            /* '<S1>/Guidance Estimator' */
} DW;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Model entry point functions */
extern void TTKF_100Hz_Function_1D_initialize(RT_MODEL *const
  TTKF_100Hz_Function_1D_M);

/* Exported entry point function */
extern void TTKF_100Hz_Function_main(RT_MODEL *const TTKF_100Hz_Function_1D_M,
  real_T rtU_SysTimein, T_GUID_Discretes *rtU_Discretesin,
  T_Weapon_SALSeeker_IF102 *rtU_seekerin, T_NAV_GenericInertialNavigationSensor *
  rtU_IMU100Hzin, uint8_T rtU_navmodein, T_NAV_NavigationState *rtU_navstatein,
  T_GUID_MissionPlanning *rtU_MPin, T_Weapon_MCP_DATA_TTKF *rtU_DATAttkf,
  T_GUID_TTKF_Out_IF1002 *rtY_ttkfout, T_GUID_TTKF_Tel100Hz *rtY_ttkftel_100Hz);

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
 * '<Root>' : 'TTKF_100Hz_Function_1D'
 * '<S1>'   : 'TTKF_100Hz_Function_1D/TTKF'
 * '<S2>'   : 'TTKF_100Hz_Function_1D/TTKF/Guidance Estimator'
 */
#endif                                /* RTW_HEADER_TTKF_100Hz_Function_1D_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
