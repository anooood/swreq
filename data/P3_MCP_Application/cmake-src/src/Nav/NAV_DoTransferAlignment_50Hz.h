/*
 * File: NAV_DoTransferAlignment_50Hz.h
 *
 * Code generated for Simulink model 'NAV_DoTransferAlignment_50Hz'.
 *
 * Model version                  : 1.67
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue Sep 13 10:47:39 2022
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

#ifndef RTW_HEADER_NAV_DoTransferAlignment_50Hz_h_
#define RTW_HEADER_NAV_DoTransferAlignment_50Hz_h_
#include "rtwtypes.h"
#include <math.h>
#include <stddef.h>
#ifndef NAV_DoTransferAlignment_50Hz_COMMON_INCLUDES_
# define NAV_DoTransferAlignment_50Hz_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                       /* NAV_DoTransferAlignment_50Hz_COMMON_INCLUDES_ */

#include "rtGetInf.h"
#include "rt_nonfinite.h"

#include "Nav_50Hz.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;


/* Custom Type definition for MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
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
  T_TA_TransferAlignState TransferAlignState_private;
                               /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  T_TA_PylonState nominal_orientation_private;
                               /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  real_T number_imu_dvelocity_average_private;
                               /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  real_T imu_dvelocity_x;      /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  real_T imu_dvelocity_y;      /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  real_T imu_dvelocity_z;      /* '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
} DW;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Model entry point functions */
extern void NAV_DoTransferAlignment_50Hz_initialize(RT_MODEL *const
  NAV_DoTransferAlignment_50Hz_M);

/* Exported entry point function */
extern void NAV_DoNavigation_50Hz(RT_MODEL *const NAV_DoTransferAlignment_50Hz_M,
  T_RTAC_TransferAlignment *rtU_RTAC_Data, real_T rtU_Time,
  T_NAV_GenericInertialNavigationSensor *rtU_GenericINSensor100Hz,
  T_TA_TransferAlignState *rtY_TransferAlignState);

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
 * '<Root>' : 'NAV_DoTransferAlignment_50Hz'
 * '<S1>'   : 'NAV_DoTransferAlignment_50Hz/NAV_Simulink_DoTransferAlignment'
 * '<S2>'   : 'NAV_DoTransferAlignment_50Hz/NAV_Simulink_DoTransferAlignment/NAV_Embedded_DoTransferAlignment50Hz'
 */
#endif                          /* RTW_HEADER_NAV_DoTransferAlignment_50Hz_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
