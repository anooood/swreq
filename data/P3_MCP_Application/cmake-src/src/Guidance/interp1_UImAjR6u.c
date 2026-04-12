/*
 * File: interp1_UImAjR6u.c
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
#include <string.h>
#include "interp1_UImAjR6u.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
real_T interp1_UImAjR6u(const real_T varargin_1[10], const real_T varargin_2[10],
  real_T varargin_3)
{
  real_T Vq;
  real_T y[10];
  real_T x[10];
  real_T xtmp;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
  memcpy(&y[0], &varargin_2[0], 10U * (sizeof(real_T)));
  memcpy(&x[0], &varargin_1[0], 10U * (sizeof(real_T)));
  Vq = 0.0;
  low_i = 0;
  do {
    exitg1 = 0;
    if (low_i < 10) {
      if (rtIsNaN(varargin_1[low_i])) {
        exitg1 = 1;
      } else {
        low_i++;
      }
    } else {
      if (varargin_1[1] < varargin_1[0]) {
        for (low_i = 0; low_i < 5; low_i++) {
          xtmp = x[low_i];
          x[low_i] = x[9 - low_i];
          x[9 - low_i] = xtmp;
        }

        for (low_i = 0; low_i < 5; low_i++) {
          xtmp = y[low_i];
          y[low_i] = y[9 - low_i];
          y[9 - low_i] = xtmp;
        }
      }

      if (rtIsNaN(varargin_3)) {
        Vq = (rtNaN);
      } else if (varargin_3 > x[9]) {
        Vq = (((varargin_3 - x[9]) / (x[9] - x[8])) * (y[9] - y[8])) + y[9];
      } else if (varargin_3 < x[0]) {
        Vq = (((varargin_3 - x[0]) / (x[1] - x[0])) * (y[1] - y[0])) + y[0];
      } else {
        low_i = 1;
        low_ip1 = 2;
        high_i = 10;
        while (high_i > low_ip1) {
          mid_i = ((low_i + high_i) >> ((uint64_T)1));
          if (varargin_3 >= x[mid_i - 1]) {
            low_i = mid_i;
            low_ip1 = mid_i + 1;
          } else {
            high_i = mid_i;
          }
        }

        xtmp = x[low_i - 1];
        xtmp = (varargin_3 - xtmp) / (x[low_i] - xtmp);
        if (xtmp == 0.0) {
          Vq = y[low_i - 1];
        } else if (xtmp == 1.0) {
          Vq = y[low_i];
        } else if (y[low_i - 1] == y[low_i]) {
          Vq = y[low_i - 1];
        } else {
          Vq = ((1.0 - xtmp) * y[low_i - 1]) + (xtmp * y[low_i]);
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
