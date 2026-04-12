/*
 * File: interp1_zsuvPUhx.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1498
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 08:25:17 2023
 */

#include "rtwtypes.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "interp1_zsuvPUhx.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
real_T interp1_zsuvPUhx(const real_T varargin_1[2], const real_T varargin_2[2],
  real_T varargin_3)
{
  real_T Vq;
  int32_T k;
  real_T r;
  real_T x_idx_1;
  real_T y_idx_0;
  real_T y_idx_1;
  int32_T exitg1;
  y_idx_0 = varargin_2[0];
  r = varargin_1[0];
  y_idx_1 = varargin_2[1];
  x_idx_1 = varargin_1[1];
  Vq = (rtNaN);
  k = 0;
  do {
    exitg1 = 0;
    if (k < 2) {
      if (rtIsNaN(varargin_1[k])) {
        exitg1 = 1;
      } else {
        k++;
      }
    } else {
      if (varargin_1[1] < varargin_1[0]) {
        r = varargin_1[1];
        x_idx_1 = varargin_1[0];
        y_idx_0 = varargin_2[1];
        y_idx_1 = varargin_2[0];
      }

      if (((!rtIsNaN(varargin_3)) && (!(varargin_3 > x_idx_1))) && (!(varargin_3
            < r))) {
        r = (varargin_3 - r) / (x_idx_1 - r);
        if (r == 0.0) {
          Vq = y_idx_0;
        } else if (r == 1.0) {
          Vq = y_idx_1;
        } else if (y_idx_0 == y_idx_1) {
          Vq = y_idx_0;
        } else {
          Vq = ((1.0 - r) * y_idx_0) + (r * y_idx_1);
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return Vq;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
