/*
 * File: interp1_bFwkvuIc.c
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
 */

#include "rtwtypes.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "interp1_bFwkvuIc.h"

/* Function for MATLAB Function: '<S1>/Data Controller' */
real_T interp1_bFwkvuIc(const real_T varargin_1[7], const real_T varargin_2[7],
  real_T varargin_3)
{
  real_T Vq;
  real_T y[7];
  real_T x[7];
  real_T xtmp;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
  for (low_i = 0; low_i < 7; low_i++) {
    y[low_i] = varargin_2[low_i];
    x[low_i] = varargin_1[low_i];
  }

  Vq = (rtNaN);
  low_i = 0;
  do {
    exitg1 = 0;
    if (low_i < 7) {
      if (rtIsNaN(varargin_1[low_i])) {
        exitg1 = 1;
      } else {
        low_i++;
      }
    } else {
      if (varargin_1[1] < 0.0) {
        xtmp = x[0];
        x[0] = x[6];
        x[6] = xtmp;
        xtmp = x[1];
        x[1] = x[5];
        x[5] = xtmp;
        xtmp = x[2];
        x[2] = x[4];
        x[4] = xtmp;
        xtmp = y[0];
        y[0] = y[6];
        y[6] = xtmp;
        xtmp = y[1];
        y[1] = y[5];
        y[5] = xtmp;
        xtmp = y[2];
        y[2] = y[4];
        y[4] = xtmp;
      }

      if (((!rtIsNaN(varargin_3)) && (!(varargin_3 > x[6]))) && (!(varargin_3 <
            x[0]))) {
        low_i = 1;
        low_ip1 = 2;
        high_i = 7;
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
        } else {
          Vq = y[low_i - 1];
          if (!(Vq == y[low_i])) {
            Vq = ((1.0 - xtmp) * Vq) + (xtmp * y[low_i]);
          }
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
