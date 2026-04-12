/*
 * File: NAV_DoNavigationMain_2Hz.h
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.619
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Wed Jan 18 11:09:50 2023
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

#ifndef RTW_HEADER_NAV_DoNavigationMain_2Hz_h_
#define RTW_HEADER_NAV_DoNavigationMain_2Hz_h_
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef NAV_DoNavigationMain_2Hz_COMMON_INCLUDES_
# define NAV_DoNavigationMain_2Hz_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                           /* NAV_DoNavigationMain_2Hz_COMMON_INCLUDES_ */

#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

#include "Nav_2Hz.h"


/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;


/* Custom Type definition for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
#ifndef struct_tag_s0NUee4xPz8iFsXXE4IOTWF
#define struct_tag_s0NUee4xPz8iFsXXE4IOTWF

struct tag_s0NUee4xPz8iFsXXE4IOTWF
{
  uint32_T Flags;
  real_T FieldWMM[3];
  real_T b[3];
};

#endif                                 /*struct_tag_s0NUee4xPz8iFsXXE4IOTWF*/

#ifndef typedef_T_MAG_State
#define typedef_T_MAG_State

typedef struct tag_s0NUee4xPz8iFsXXE4IOTWF T_MAG_State;

#endif                                 /*typedef_T_MAG_State*/

#ifndef struct_tag_s6UIRkbadDjHFgznoao6ruC
#define struct_tag_s6UIRkbadDjHFgznoao6ruC

struct tag_s6UIRkbadDjHFgznoao6ruC
{
  real_T TimeStamp;
  uint32_T Flags;
  real_T FieldMeas[3];
  real_T FieldWMM[3];
  real_T Attitude[3];
};

#endif                                 /*struct_tag_s6UIRkbadDjHFgznoao6ruC*/

#ifndef typedef_T_MAG_Input
#define typedef_T_MAG_Input

typedef struct tag_s6UIRkbadDjHFgznoao6ruC T_MAG_Input;

#endif                                 /*typedef_T_MAG_Input*/

#ifndef struct_tag_s09vSIKzLhbKmxN5V5lp06B
#define struct_tag_s09vSIKzLhbKmxN5V5lp06B

struct tag_s09vSIKzLhbKmxN5V5lp06B
{
  uint32_T Valid;
  real_T Attitude[3];
  real_T P[9];
};

#endif                                 /*struct_tag_s09vSIKzLhbKmxN5V5lp06B*/

#ifndef typedef_T_MAG_Output
#define typedef_T_MAG_Output

typedef struct tag_s09vSIKzLhbKmxN5V5lp06B T_MAG_Output;

#endif                                 /*typedef_T_MAG_Output*/

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  T_NAV_KFilterErrorEstimateState delta_private;
                                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  T_MAG_State MAG_State;           /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T c[169];                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T dc[169];                  /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T fm[13];                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T fn[13];                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T k[169];                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T KalmanFilterState_private[16];
                                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T KalmanFilterCovariance_private[256];
                                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  real_T time_updateINS;           /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  uint8_T DoMagnetometerBiasCalibration;
                                   /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  uint8_T firstTimeMeasurement;    /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  uint8_T gnss_valid_private;      /* '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
} DW;

/* Self model data, for system '<Root>' */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW dwork;
};

/* Model entry point functions */
extern void NAV_DoNavigationMain_2Hz_initialize(RT_MODEL *const
  NAV_DoNavigationMain_2Hz_M);

/* Exported entry point function */
extern void NAV_DoNavigation_2Hz(RT_MODEL *const NAV_DoNavigationMain_2Hz_M,
  T_GNSS_Result *rtU_GNSS_Result, T_NAV_State *rtU_NavigationState,
  T_TA_TransferAlignState *rtU_TransferAlignState, T_NAV_MagnetometerMeasurement
  *rtU_MagnetometerMeasurement, T_NAV_KFilterErrorEstimateState
  *rtY_KalmanFilterErrorEstimatesOut, T_NAV_StoreInfo *rtY_StoreInfo, T_MAG_Tel *
  rtY_MagnetometerState);

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
 * '<Root>' : 'NAV_DoNavigationMain_2Hz'
 * '<S1>'   : 'NAV_DoNavigationMain_2Hz/Subsystem'
 * '<S2>'   : 'NAV_DoNavigationMain_2Hz/Subsystem/NAV_Embedded_DoNavigationMain2Hz'
 */
#endif                              /* RTW_HEADER_NAV_DoNavigationMain_2Hz_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
