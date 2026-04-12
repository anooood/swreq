/*
 * File: interp1_WUCGY3Hg.c
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
#include "interp1_WUCGY3Hg.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
real_T interp1_WUCGY3Hg(const real_T varargin_1[7], const real_T varargin_2[7],
  real_T varargin_3)
{
  real_T Vq;
  real_T r;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
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
      if ((!rtIsNaN(varargin_3)) && ((!(varargin_3 > 500000.0)) && (!(varargin_3
             < 0.0)))) {
        low_i = 1;
        low_ip1 = 2;
        high_i = 7;
        while (high_i > low_ip1) {
          mid_i = ((low_i + high_i) >> ((uint64_T)1));
          if (varargin_3 >= varargin_1[mid_i - 1]) {
            low_i = mid_i;
            low_ip1 = mid_i + 1;
          } else {
            high_i = mid_i;
          }
        }

        r = varargin_1[low_i - 1];
        r = (varargin_3 - r) / (varargin_1[low_i] - r);
        if (r == 0.0) {
          Vq = varargin_2[low_i - 1];
        } else if (r == 1.0) {
          Vq = varargin_2[low_i];
        } else if (varargin_2[low_i - 1] == varargin_2[low_i]) {
          Vq = varargin_2[low_i - 1];
        } else {
          Vq = ((1.0 - r) * varargin_2[low_i - 1]) + (r * varargin_2[low_i]);
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
