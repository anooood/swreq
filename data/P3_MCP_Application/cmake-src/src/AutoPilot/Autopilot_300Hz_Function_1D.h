/*
 * File: Autopilot_300Hz_Function_1D.h
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

#ifndef RTW_HEADER_Autopilot_300Hz_Function_1D_h_
#define RTW_HEADER_Autopilot_300Hz_Function_1D_h_
#include "rtwtypes.h"
#include <math.h>
#include <stddef.h>
#ifndef Autopilot_300Hz_Function_1D_COMMON_INCLUDES_
# define Autopilot_300Hz_Function_1D_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                        /* Autopilot_300Hz_Function_1D_COMMON_INCLUDES_ */

#include "rt_nonfinite.h"
#include "rtGetInf.h"

#include "Autopilot_300Hz_1D.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_T_GUID_Autop_Gains_
#define DEFINED_TYPEDEF_FOR_T_GUID_Autop_Gains_

typedef struct {
  real_T t_delt;
  real_T Kp_phi;
  real_T Ki_phi;
  real_T Kp_wbx;
  real_T NCProll;
  real_T RPlagZ;
  real_T RPlagP;
  real_T NCWroll;
  real_T RRleadZ;
  real_T RRleadP;
  real_T RRlagZ;
  real_T RRlagP;
  real_T Kp_gbz;
  real_T Ki_wby;
  real_T Kp_wby;
  real_T NCApitch;
  real_T LZAlowWN;
  real_T LZAleadZ;
  real_T LZAleadP;
  real_T LZAlagZ;
  real_T LZAlagP;
  real_T NCWpitch;
  real_T LZWleadZ;
  real_T LZWleadP;
  real_T LZWlagZ;
  real_T LZWlagP;
  real_T Kp_gby;
  real_T Ki_wbz;
  real_T Kp_wbz;
  real_T NCAyaw;
  real_T LYAlowWN;
  real_T LYAleadZ;
  real_T LYAleadP;
  real_T LYAlagZ;
  real_T LYAlagP;
  real_T NCWyaw;
  real_T LYWleadZ;
  real_T LYWleadP;
  real_T LYWlagZ;
  real_T LYWlagP;
} T_GUID_Autop_Gains;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_tIeUbrNDQog5Bi5u9ack2C_
#define DEFINED_TYPEDEF_FOR_struct_tIeUbrNDQog5Bi5u9ack2C_

typedef struct {
  real_T delt;
} struct_tIeUbrNDQog5Bi5u9ack2C;

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

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  T_GUID_Autop_Gains AP;               /* '<S1>/Autopilot Gains' */
  real_T Dcmd[4];                      /* '<S1>/Dynamic Fin Allocation' */
  real_T Memory_PreviousInput;         /* '<S1>/Memory' */
  real_T Memory1_PreviousInput;        /* '<S1>/Memory1' */
  real_T Memory2_PreviousInput;        /* '<S1>/Memory2' */
  real_T Memory6_PreviousInput;        /* '<S1>/Memory6' */
  real_T Memory4_PreviousInput;        /* '<S1>/Memory4' */
  real_T Memory5_PreviousInput;        /* '<S1>/Memory5' */
  real_T Memory3_PreviousInput;        /* '<S1>/Memory3' */
  real_T K;                            /* '<S1>/RR_Filter' */
  real_T ylead_prev;                   /* '<S1>/RR_Filter' */
  real_T xlead_prev;                   /* '<S1>/RR_Filter' */
  real_T ylag_prev;                    /* '<S1>/RR_Filter' */
  real_T xlag_prev;                    /* '<S1>/RR_Filter' */
  real_T K_b;                          /* '<S1>/RP_Filter' */
  real_T y_prev;                       /* '<S1>/RP_Filter' */
  real_T x_prev;                       /* '<S1>/RP_Filter' */
  real_T K_a;                          /* '<S1>/LRZ_Filter' */
  real_T ylead_prev_i;                 /* '<S1>/LRZ_Filter' */
  real_T xlead_prev_i;                 /* '<S1>/LRZ_Filter' */
  real_T ylag_prev_b;                  /* '<S1>/LRZ_Filter' */
  real_T xlag_prev_m;                  /* '<S1>/LRZ_Filter' */
  real_T K_k;                          /* '<S1>/LRY_Filter' */
  real_T ylead_prev_b;                 /* '<S1>/LRY_Filter' */
  real_T xlead_prev_l;                 /* '<S1>/LRY_Filter' */
  real_T ylag_prev_m;                  /* '<S1>/LRY_Filter' */
  real_T xlag_prev_i;                  /* '<S1>/LRY_Filter' */
  real_T K_ke;                         /* '<S1>/GBZ_Filter' */
  real_T xlow2_prev1;                  /* '<S1>/GBZ_Filter' */
  real_T ylow2_prev1;                  /* '<S1>/GBZ_Filter' */
  real_T xlow2_prev2;                  /* '<S1>/GBZ_Filter' */
  real_T ylow2_prev2;                  /* '<S1>/GBZ_Filter' */
  real_T ylead_prev_f;                 /* '<S1>/GBZ_Filter' */
  real_T xlead_prev_k;                 /* '<S1>/GBZ_Filter' */
  real_T ylag_prev_p;                  /* '<S1>/GBZ_Filter' */
  real_T xlag_prev_a;                  /* '<S1>/GBZ_Filter' */
  real_T K_m;                          /* '<S1>/GBY_Filter' */
  real_T xlow2_prev1_p;                /* '<S1>/GBY_Filter' */
  real_T ylow2_prev1_d;                /* '<S1>/GBY_Filter' */
  real_T xlow2_prev2_a;                /* '<S1>/GBY_Filter' */
  real_T ylow2_prev2_i;                /* '<S1>/GBY_Filter' */
  real_T ylead_prev_d;                 /* '<S1>/GBY_Filter' */
  real_T xlead_prev_f;                 /* '<S1>/GBY_Filter' */
  real_T ylag_prev_i;                  /* '<S1>/GBY_Filter' */
  real_T xlag_prev_h;                  /* '<S1>/GBY_Filter' */
  real_T DXLim;                        /* '<S1>/Dynamic Fin Allocation' */
  real_T MACH;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T QRATIO;                       /* '<S1>/Autopilot Input Conditioning' */
  real_T ALPHA;                        /* '<S1>/Autopilot Input Conditioning' */
  real_T BETA;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T PHIC;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T GBZC;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T GBYC;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T PHIM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T GBZM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T GBYM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T WBXM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T WBYM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T WBZM;                         /* '<S1>/Autopilot Input Conditioning' */
  real_T AZfac;                        /* '<S1>/Autopilot Input Conditioning' */
  real_T K_i;                          /* '<S1>/Autopilot Gains' */
  real_T sigma_prev;                   /* '<S1>/Autopilot Gains' */
  real_T alpha_prev;                   /* '<S1>/Autopilot Gains' */
  real_T beta_prev;                    /* '<S1>/Autopilot Gains' */
  real_T sigma_filt_prev;              /* '<S1>/Autopilot Gains' */
  real_T alpha_filt_prev;              /* '<S1>/Autopilot Gains' */
  real_T beta_filt_prev;               /* '<S1>/Autopilot Gains' */
  uint32_T Output_DSTATE;              /* '<S11>/Output' */
  uint16_T countNaNout;                /* '<S1>/Dynamic Fin Allocation' */
  uint16_T countNaNin;                 /* '<S1>/Autopilot Input Conditioning' */
  uint16_T cnt;                        /* '<S1>/Autopilot Gains' */
  uint8_T cnt_j;                       /* '<S1>/Modes and Telemetry' */
  boolean_T K_not_empty;               /* '<S1>/RR_Filter' */
  boolean_T K_not_empty_d;             /* '<S1>/RP_Filter' */
  boolean_T K_not_empty_a;             /* '<S1>/LRZ_Filter' */
  boolean_T K_not_empty_n;             /* '<S1>/LRY_Filter' */
  boolean_T K_not_empty_dk;            /* '<S1>/GBZ_Filter' */
  boolean_T K_not_empty_e;             /* '<S1>/GBY_Filter' */
  boolean_T countNaNin_not_empty;      /* '<S1>/Autopilot Input Conditioning' */
  boolean_T cnt_not_empty;             /* '<S1>/Autopilot Gains' */
} DW;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Model entry point functions */
extern void Autopilot_300Hz_Function_1D_initialize(RT_MODEL *const
  Autopilot_300Hz_Function_1D_M);

/* Exported entry point function */
extern void Autopilot_300Hz_Function_main(RT_MODEL *const
  Autopilot_300Hz_Function_1D_M, T_NAV_InertialMeasurementUnitAutopilot
  *rtU_IMU300Hzin, T_NAV_NavigationState *rtU_navstatein, T_GUID_Guid_Out_IF1003
  *rtU_guidin, T_Weapon_MCP_DATA_Autop *rtU_DATAautop, T_Weapon_MCP_Out_IF103
  *rtY_deflcmd, T_GUID_Autop_Tel300Hz *rtY_autoptel_300Hz, T_GUID_Autop_Tel20Hz *
  rtY_autoptel_20Hz);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S11>/Data Type Propagation' : Unused code path elimination
 * Block '<S28>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S29>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S20>/Data Type Duplicate' : Unused code path elimination
 * Block '<S20>/Data Type Propagation' : Unused code path elimination
 * Block '<S21>/Data Type Duplicate' : Unused code path elimination
 * Block '<S21>/Data Type Propagation' : Unused code path elimination
 * Block '<S22>/Data Type Duplicate' : Unused code path elimination
 * Block '<S22>/Data Type Propagation' : Unused code path elimination
 * Block '<S23>/Data Type Duplicate' : Unused code path elimination
 * Block '<S23>/Data Type Propagation' : Unused code path elimination
 * Block '<S24>/Data Type Duplicate' : Unused code path elimination
 * Block '<S24>/Data Type Propagation' : Unused code path elimination
 * Block '<S25>/Data Type Duplicate' : Unused code path elimination
 * Block '<S25>/Data Type Propagation' : Unused code path elimination
 * Block '<S26>/Data Type Duplicate' : Unused code path elimination
 * Block '<S26>/Data Type Propagation' : Unused code path elimination
 * Block '<S27>/Data Type Duplicate' : Unused code path elimination
 * Block '<S27>/Data Type Propagation' : Unused code path elimination
 * Block '<S1>/Data Type Conversion1' : Eliminate redundant data type conversion
 */

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
 * '<Root>' : 'Autopilot_300Hz_Function_1D'
 * '<S1>'   : 'Autopilot_300Hz_Function_1D/Autopilot'
 * '<S2>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate Fins'
 * '<S3>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate gby loop'
 * '<S4>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate gbz loop'
 * '<S5>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate phi loop'
 * '<S6>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate wbx loop'
 * '<S7>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate wby loop'
 * '<S8>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Activate wbz loop'
 * '<S9>'   : 'Autopilot_300Hz_Function_1D/Autopilot/Autopilot Gains'
 * '<S10>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Autopilot Input Conditioning'
 * '<S11>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Counter Limited'
 * '<S12>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Dynamic Fin Allocation'
 * '<S13>'  : 'Autopilot_300Hz_Function_1D/Autopilot/GBY_Filter'
 * '<S14>'  : 'Autopilot_300Hz_Function_1D/Autopilot/GBZ_Filter'
 * '<S15>'  : 'Autopilot_300Hz_Function_1D/Autopilot/LRY_Filter'
 * '<S16>'  : 'Autopilot_300Hz_Function_1D/Autopilot/LRZ_Filter'
 * '<S17>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Modes and Telemetry'
 * '<S18>'  : 'Autopilot_300Hz_Function_1D/Autopilot/RP_Filter'
 * '<S19>'  : 'Autopilot_300Hz_Function_1D/Autopilot/RR_Filter'
 * '<S20>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic'
 * '<S21>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic1'
 * '<S22>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic2'
 * '<S23>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic3'
 * '<S24>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic4'
 * '<S25>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic5'
 * '<S26>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic6'
 * '<S27>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Saturation Dynamic7'
 * '<S28>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Counter Limited/Increment Real World'
 * '<S29>'  : 'Autopilot_300Hz_Function_1D/Autopilot/Counter Limited/Wrap To Zero'
 */
#endif                           /* RTW_HEADER_Autopilot_300Hz_Function_1D_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
