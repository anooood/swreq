/*
 * File: norm_xgB9aDcJ.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_100Hz'.
 *
 * Model version                  : 1.605
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:08:05 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include "norm_xgB9aDcJ.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
real32_T norm_xgB9aDcJ(const real32_T x[3])
{
  real32_T y;
  real32_T scale;
  real32_T absxk;
  real32_T t;
  scale = 1.29246971E-26F;
  absxk = fabsf(x[0]);
  if (absxk > 1.29246971E-26F) {
    y = 1.0F;
    scale = absxk;
  } else {
    t = absxk / 1.29246971E-26F;
    y = t * t;
  }

  absxk = fabsf(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = ((y * t) * t) + 1.0F;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabsf(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = ((y * t) * t) + 1.0F;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrtf(y);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
