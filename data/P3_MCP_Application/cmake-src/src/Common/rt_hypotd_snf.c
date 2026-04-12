/*
 * File: rt_hypotd_snf.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function'.
 *
 * Model version                  : 1.1430
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Fri Oct  7 08:02:01 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "rt_hypotd_snf.h"

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt((a * a) + 1.0);
  } else if (a > y) {
    y /= a;
    y = sqrt((y * y) + 1.0) * a;
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
