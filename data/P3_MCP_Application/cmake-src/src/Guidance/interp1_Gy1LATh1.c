/*
 * File: interp1_Gy1LATh1.c
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
#include "interp1_Gy1LATh1.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
real_T interp1_Gy1LATh1(const real_T varargin_1[7], const real_T varargin_2[7],
  real_T varargin_3)
{
  real_T Vq;
  real_T y[7];
  real_T x[7];
  real_T xtmp;
  int32_T low_ip1;
  int32_T high_i;
  int32_T mid_i;
  int32_T i;
  int32_T exitg1;
  for (i = 0; i < 7; i++) {
    y[i] = varargin_2[i];
    x[i] = varargin_1[i];
  }

  Vq = 0.0;
  i = 0;
  do {
    exitg1 = 0;
    if (i < 7) {
      if (rtIsNaN(varargin_1[i])) {
        exitg1 = 1;
      } else {
        i++;
      }
    } else {
      if (varargin_1[1] < varargin_1[0]) {
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

      if (rtIsNaN(varargin_3)) {
        Vq = (rtNaN);
      } else if (varargin_3 > x[6]) {
        Vq = (((varargin_3 - x[6]) / (x[6] - x[5])) * (y[6] - y[5])) + y[6];
      } else if (varargin_3 < x[0]) {
        Vq = (((varargin_3 - x[0]) / (x[1] - x[0])) * (y[1] - y[0])) + y[0];
      } else {
        i = 1;
        low_ip1 = 2;
        high_i = 7;
        while (high_i > low_ip1) {
          mid_i = ((i + high_i) >> ((uint64_T)1));
          if (varargin_3 >= x[mid_i - 1]) {
            i = mid_i;
            low_ip1 = mid_i + 1;
          } else {
            high_i = mid_i;
          }
        }

        xtmp = x[i - 1];
        xtmp = (varargin_3 - xtmp) / (x[i] - xtmp);
        if (xtmp == 0.0) {
          Vq = y[i - 1];
        } else if (xtmp == 1.0) {
          Vq = y[i];
        } else {
          Vq = y[i - 1];
          if (!(Vq == y[i])) {
            Vq = ((1.0 - xtmp) * Vq) + (xtmp * y[i]);
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
