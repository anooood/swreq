/*
 * File: sum_sVbQhjnY.c
 *
 * Code generated for Simulink model 'TTKF_100Hz_Function_1D'.
 *
 * Model version                  : 1.1467
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:33:35 2023
 */

#include "rtwtypes.h"
#include "sum_sVbQhjnY.h"

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
real_T sum_sVbQhjnY(const real_T x[9])
{
  real_T y;
  int32_T k;
  y = x[0];
  for (k = 0; k < 8; k++) {
    y += x[k + 1];
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
