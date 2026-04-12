/*
 * File: NAV_DoNavigationMain_100Hz.h
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_100Hz'.
 *
 * Model version                  : 1.644
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Wed Jan 18 11:12:26 2023
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

#ifndef RTW_HEADER_NAV_DoNavigationMain_100Hz_h_
#define RTW_HEADER_NAV_DoNavigationMain_100Hz_h_
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef NAV_DoNavigationMain_100Hz_COMMON_INCLUDES_
# define NAV_DoNavigationMain_100Hz_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                         /* NAV_DoNavigationMain_100Hz_COMMON_INCLUDES_ */

#include "rtGetInf.h"
#include "rt_nonfinite.h"

#include "nav_100Hz.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_struct_nFYOE7wW0LEic5IIo1QyqE_
#define DEFINED_TYPEDEF_FOR_struct_nFYOE7wW0LEic5IIo1QyqE_

typedef struct {
  uint8_T firstrun;
  real_T deltaT;
  real_T deltaTMCP;
  real_T WE;
  real_T Req;
  real_T flattening;
  real_T Reb;
} struct_nFYOE7wW0LEic5IIo1QyqE;

#endif

/* Custom Type definition for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
#ifndef struct_tag_s35kaHDnJgzMD9fGCDpCc7E
#define struct_tag_s35kaHDnJgzMD9fGCDpCc7E

struct tag_s35kaHDnJgzMD9fGCDpCc7E
{
  uint32_T trade;
};

#endif                                 /*struct_tag_s35kaHDnJgzMD9fGCDpCc7E*/

#ifndef typedef_s35kaHDnJgzMD9fGCDpCc7E
#define typedef_s35kaHDnJgzMD9fGCDpCc7E

typedef struct tag_s35kaHDnJgzMD9fGCDpCc7E s35kaHDnJgzMD9fGCDpCc7E;

#endif                                 /*typedef_s35kaHDnJgzMD9fGCDpCc7E*/

#ifndef struct_tag_sL5wYVhGKgEV44lEuJklE6E
#define struct_tag_sL5wYVhGKgEV44lEuJklE6E

struct tag_sL5wYVhGKgEV44lEuJklE6E
{
  real_T SystemTime;
  real_T alt;
  real_T pressure;
  uint8_T valid;
  uint8_T Reserved01;
  uint16_T Reserved02;
  real_T comp_alt;
  real_T KalmanFilter_delta_baro;
};

#endif                                 /*struct_tag_sL5wYVhGKgEV44lEuJklE6E*/

#ifndef typedef_T_NAV_BAROState
#define typedef_T_NAV_BAROState

typedef struct tag_sL5wYVhGKgEV44lEuJklE6E T_NAV_BAROState;

#endif                                 /*typedef_T_NAV_BAROState*/

#ifndef struct_tag_slWWUqPWyaBdNdmELcs1dW
#define struct_tag_slWWUqPWyaBdNdmELcs1dW

struct tag_slWWUqPWyaBdNdmELcs1dW
{
  T_Euler attitude;
  real_T accel_limit_low;
  real_T accel_limit_high;
  uint8_T valid;
  uint8_T initial_attitude_set;
};

#endif                                 /*struct_tag_slWWUqPWyaBdNdmELcs1dW*/

#ifndef typedef_T_TA_PylonState
#define typedef_T_TA_PylonState

typedef struct tag_slWWUqPWyaBdNdmELcs1dW T_TA_PylonState;

#endif                                 /*typedef_T_TA_PylonState*/

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  T_NAV_NavigationState navigationstateINS_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  T_NAV_KalmanFilterState KalmanFilterState;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  T_NAV_KalmanFilterState KalmanFilterstate_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  T_NAV_BaroState BaroState;     /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  T_NAV_BAROState BaroState_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  T_TA_PylonState nominal_orientation_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  s35kaHDnJgzMD9fGCDpCc7E KalmanFilterDeltaState_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T BaroValidityTimeOut;    /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T BaroValidityTimeOutCounter;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T navigationModeINS_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T number_imu_dvelocity_average_private;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T imu_dvelocity_x;        /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T imu_dvelocity_y;        /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T imu_dvelocity_z;        /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T BaroScaleFactor;        /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  real_T LocalBaroBias;          /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  uint8_T PreviousUmbilicalState;/* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  boolean_T navigationstateINS_private_not_empty;
                                 /* '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
} DW;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Model entry point functions */
extern void NAV_DoNavigationMain_100Hz_initialize(RT_MODEL *const
  NAV_DoNavigationMain_100Hz_M);

/* Exported entry point function */
extern void NAV_DoNavigation_100Hz(RT_MODEL *const NAV_DoNavigationMain_100Hz_M,
  T_NAV_GenericInertialNavigationSensor *rtU_GenericINSensor100Hz,
  T_TA_TransferAlignState *rtU_TransferAlignState,
  T_NAV_KFilterErrorEstimateState *rtU_KalmanFilterErrorEstimates, T_GNSS_Result
  *rtU_GNSS_Result, T_NAV_State *rtY_Nav_State);

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
 * '<Root>' : 'NAV_DoNavigationMain_100Hz'
 * '<S1>'   : 'NAV_DoNavigationMain_100Hz/Subsystem'
 * '<S2>'   : 'NAV_DoNavigationMain_100Hz/Subsystem/NAV_Embedded_DoNavigationMain100Hz'
 */
#endif                            /* RTW_HEADER_NAV_DoNavigationMain_100Hz_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
