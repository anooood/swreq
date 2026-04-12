/*
 * File: interp1_Ln1e4hwu.c
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
#include "interp1_Ln1e4hwu.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
real_T interp1_Ln1e4hwu(const real_T varargin_1[131], const real_T varargin_2
  [131], real_T varargin_3)
{
  real_T Vq;
  real_T y[131];
  real_T x[131];
  real_T xtmp;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
  memcpy(&y[0], &varargin_2[0], 131U * (sizeof(real_T)));
  memcpy(&x[0], &varargin_1[0], 131U * (sizeof(real_T)));
  Vq = (rtNaN);
  low_i = 0;
  do {
    exitg1 = 0;
    if (low_i < 131) {
      if (rtIsNaN(varargin_1[low_i])) {
        exitg1 = 1;
      } else {
        low_i++;
      }
    } else {
      if (varargin_1[1] < varargin_1[0]) {
        for (low_i = 0; low_i < 65; low_i++) {
          xtmp = x[low_i];
          x[low_i] = x[130 - low_i];
          x[130 - low_i] = xtmp;
        }

        for (low_i = 0; low_i < 65; low_i++) {
          xtmp = y[low_i];
          y[low_i] = y[130 - low_i];
          y[130 - low_i] = xtmp;
        }
      }

      if (((!rtIsNaN(varargin_3)) && (!(varargin_3 > x[130]))) && (!(varargin_3 <
            x[0]))) {
        low_i = 1;
        low_ip1 = 2;
        high_i = 131;
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
