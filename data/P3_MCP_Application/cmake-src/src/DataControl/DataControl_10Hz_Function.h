/*
 * File: DataControl_10Hz_Function.h
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
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

#ifndef RTW_HEADER_DataControl_10Hz_Function_h_
#define RTW_HEADER_DataControl_10Hz_Function_h_
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef DataControl_10Hz_Function_COMMON_INCLUDES_
# define DataControl_10Hz_Function_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                          /* DataControl_10Hz_Function_COMMON_INCLUDES_ */

#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"
#include "rt_defines.h"

#include "DataControl_10Hz.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

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

/* Custom Type definition for MATLAB Function: '<S1>/Data Controller' */
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

#ifndef struct_tag_stE9DGVPdbGPhyouEV4GctG
#define struct_tag_stE9DGVPdbGPhyouEV4GctG

struct tag_stE9DGVPdbGPhyouEV4GctG
{
  real_T inside;
  real_T tg;
  real_T te;
  real_T ti;
  real_T ts;
};

#endif                                 /*struct_tag_stE9DGVPdbGPhyouEV4GctG*/

#ifndef typedef_stE9DGVPdbGPhyouEV4GctG
#define typedef_stE9DGVPdbGPhyouEV4GctG

typedef struct tag_stE9DGVPdbGPhyouEV4GctG stE9DGVPdbGPhyouEV4GctG;

#endif                                 /*typedef_stE9DGVPdbGPhyouEV4GctG*/

#ifndef struct_tag_snFb1SHWmbC64DSgfqhsaYG
#define struct_tag_snFb1SHWmbC64DSgfqhsaYG

struct tag_snFb1SHWmbC64DSgfqhsaYG
{
  real_T N[11];
  real_T E[11];
  real_T diveangle;
  real_T attackhead;
  real_T impspeed;
  real_T groundrange;
};

#endif                                 /*struct_tag_snFb1SHWmbC64DSgfqhsaYG*/

#ifndef typedef_snFb1SHWmbC64DSgfqhsaYG
#define typedef_snFb1SHWmbC64DSgfqhsaYG

typedef struct tag_snFb1SHWmbC64DSgfqhsaYG snFb1SHWmbC64DSgfqhsaYG;

#endif                                 /*typedef_snFb1SHWmbC64DSgfqhsaYG*/

#ifndef struct_tag_s0qQI8FPZFwxQDcpW3UrxsD
#define struct_tag_s0qQI8FPZFwxQDcpW3UrxsD

struct tag_s0qQI8FPZFwxQDcpW3UrxsD
{
  real_T radius;
  real_T exclrad;
  real_T RelX;
  real_T RelY;
  real_T Psi2LAR;
  real_T Psi2Tgt;
  real_T Rge2LAR;
  real_T Rge2Tgt;
  real_T Time2Imp;
  real_T Time2Lase;
  real_T Time2Go;
  uint32_T LARFlags;
  real_T radius_Test;
  real_T Psi2LAR_Test;
  real_T Rge2LAR_Test;
};

#endif                                 /*struct_tag_s0qQI8FPZFwxQDcpW3UrxsD*/

#ifndef typedef_s0qQI8FPZFwxQDcpW3UrxsD
#define typedef_s0qQI8FPZFwxQDcpW3UrxsD

typedef struct tag_s0qQI8FPZFwxQDcpW3UrxsD s0qQI8FPZFwxQDcpW3UrxsD;

#endif                                 /*typedef_s0qQI8FPZFwxQDcpW3UrxsD*/

#ifndef struct_emxArray_real32_T_3x6x2
#define struct_emxArray_real32_T_3x6x2

struct emxArray_real32_T_3x6x2
{
  real32_T data[36];
  int32_T size[3];
};

#endif                                 /*struct_emxArray_real32_T_3x6x2*/

#ifndef typedef_emxArray_real32_T_3x6x2
#define typedef_emxArray_real32_T_3x6x2

typedef struct emxArray_real32_T_3x6x2 emxArray_real32_T_3x6x2;

#endif                                 /*typedef_emxArray_real32_T_3x6x2*/

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

#ifndef struct_tag_sW6z4ntaPZV6RXLllkgQXpF
#define struct_tag_sW6z4ntaPZV6RXLllkgQXpF

struct tag_sW6z4ntaPZV6RXLllkgQXpF
{
  real_T excl_radius;
  real_T GndDistTgt;
  real_T RelBearTgt;
  real_T radius;
  uint8_T radius_valid;
  real_T GndDistLAR;
  real_T RelBearLAR;
  real_T RelDistX;
  real_T RelDistY;
  uint8_T excl_valid;
  real_T ctr_lat;
  real_T ctr_lon;
  real_T trajlat[32];
  real_T trajlon[32];
  real_T est_GroundRange;
  real_T est_ImpactSpd;
  real_T est_ImpactTrack;
  real_T est_ImpactDive;
  real_T inside;
  real_T ToGo;
  real_T ToExit;
  real_T ToImpact;
  real_T ToIllum;
};

#endif                                 /*struct_tag_sW6z4ntaPZV6RXLllkgQXpF*/

#ifndef typedef_sW6z4ntaPZV6RXLllkgQXpF
#define typedef_sW6z4ntaPZV6RXLllkgQXpF

typedef struct tag_sW6z4ntaPZV6RXLllkgQXpF sW6z4ntaPZV6RXLllkgQXpF;

#endif                                 /*typedef_sW6z4ntaPZV6RXLllkgQXpF*/

#ifndef struct_sL2evrqj8mNanCUwypz38l_tag
#define struct_sL2evrqj8mNanCUwypz38l_tag

struct sL2evrqj8mNanCUwypz38l_tag
{
  real32_T psilim;
  real32_T psivec[8];
  real32_T altmin;
  real32_T altmax;
  real32_T altvec[6];
  real32_T alttgt[3];
  real32_T CX070[144];
  real32_T CX080[144];
  real32_T CX090[144];
  real32_T CY070[144];
  real32_T CY080[144];
  real32_T CY090[144];
  real32_T EXCL070[144];
  real32_T EXCL080[144];
  real32_T EXCL090[144];
  real32_T RAD070[144];
  real32_T RAD080[144];
  real32_T RAD090[144];
  emxArray_real32_T_3x6x2 RBAL070;
  emxArray_real32_T_3x6x2 RBAL080;
  emxArray_real32_T_3x6x2 RBAL090;
};

#endif                                 /*struct_sL2evrqj8mNanCUwypz38l_tag*/

#ifndef typedef_b_sYbzGJxaGNvEV1Jh0D0xgEF
#define typedef_b_sYbzGJxaGNvEV1Jh0D0xgEF

typedef struct sL2evrqj8mNanCUwypz38l_tag b_sYbzGJxaGNvEV1Jh0D0xgEF;

#endif                                 /*typedef_b_sYbzGJxaGNvEV1Jh0D0xgEF*/

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  T_Weapon_MCP_GlobalMemory_LARtraj LAR_traj;/* '<S1>/Data Controller' */
  b_sYbzGJxaGNvEV1Jh0D0xgEF LAR_data;  /* '<S1>/Data Controller' */
  T_Weapon_MCP_LAR LAR_out;            /* '<S1>/Data Controller' */
  int32_T dTgt2[8000];
  int32_T ii_data[8000];
  int32_T dataMAT1_data[8000];
  int32_T tmp_data[8000];
  int32_T z1_data[8000];
  int32_T vwork_data[8000];
  int32_T iidx_data[8000];
  int32_T iwork_data[8000];
  int32_T xwork_data[8000];
  int32_T c_x_data[8000];
  uint32_T cnt;                        /* '<S1>/Data Controller' */
  int16_T ind1_data[8000];
  int16_T dataMAT1[192000];
  int16_T dataMAT2[192000];
  int16_T ind2_data[8000];
  uint8_T AFmode;                      /* '<S1>/Data Controller' */
  uint8_T WingsRelRcvd_prev;           /* '<S1>/Data Controller' */
  boolean_T d_x[8000];
  boolean_T once_not_empty;            /* '<S1>/Data Controller' */
} DW;

/* Parameters (default storage) */
struct P_ {
  T_Weapon_MCP_GlobalMemory MCP_Global;/* Variable: MCP_Global
                                        * Referenced by: '<S1>/Data Controller'
                                        */
};

/* Parameters (default storage) */
typedef struct P_ P;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Block parameters (default storage) */
extern P *rtP;
extern unsigned char weapon_config_data[];

/* Model entry point functions */
extern void DataControl_10Hz_Function_initialize(RT_MODEL *const
  DataControl_10Hz_Function_M);

/* Exported entry point function */
extern void DataControl_10Hz_Function_main(RT_MODEL *const
  DataControl_10Hz_Function_M, T_GUID_Guid2DATA *rtU_Guidin, T_GUID_WeaponConfig
  *rtU_WeaCfgin, T_GUID_TestConfig *rtU_TestCfgin, T_GUID_Discretes
  *rtU_Discretesin, uint8_T rtU_calcLAR, T_Weapon_MCP_DATA *rtY_MCPdata,
  T_Weapon_MCP_LAR *rtY_LAR);

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
 * '<Root>' : 'DataControl_10Hz_Function'
 * '<S1>'   : 'DataControl_10Hz_Function/DataControl'
 * '<S2>'   : 'DataControl_10Hz_Function/DataControl/Data Controller'
 */
#endif                             /* RTW_HEADER_DataControl_10Hz_Function_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
