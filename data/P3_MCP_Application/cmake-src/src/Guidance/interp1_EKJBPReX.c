/*
 * File: interp1_EKJBPReX.c
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
#include "interp1_EKJBPReX.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
void interp1_EKJBPReX(const real_T varargin_1[7], const real_T varargin_2[70],
                      real_T varargin_3, real_T Vq[10])
{
  real_T y[70];
  real_T x[7];
  real_T xtmp;
  int32_T offset;
  real_T y2;
  int32_T low_i;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
  memcpy(&y[0], &varargin_2[0], 70U * (sizeof(real_T)));
  for (high_i = 0; high_i < 7; high_i++) {
    x[high_i] = varargin_1[high_i];
  }

  memset(&Vq[0], 0, 10U * (sizeof(real_T)));
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
        for (low_i = 0; low_i < 10; low_i++) {
          offset = low_i * 7;
          xtmp = y[offset];
          y[offset] = y[offset + 6];
          y[offset + 6] = xtmp;
          xtmp = y[offset + 1];
          y[offset + 1] = y[offset + 5];
          y[offset + 5] = xtmp;
          xtmp = y[offset + 2];
          y[offset + 2] = y[offset + 4];
          y[offset + 4] = xtmp;
        }
      }

      if (rtIsNaN(varargin_3)) {
        for (low_i = 0; low_i < 10; low_i++) {
          Vq[low_i] = (rtNaN);
        }
      } else if (varargin_3 > x[6]) {
        xtmp = (varargin_3 - x[6]) / (x[6] - x[5]);
        for (low_i = 0; low_i < 10; low_i++) {
          y2 = y[(low_i * 7) + 6];
          Vq[low_i] = ((y2 - y[(low_i * 7) + 5]) * xtmp) + y2;
        }
      } else if (varargin_3 < x[0]) {
        xtmp = (varargin_3 - x[0]) / (x[1] - x[0]);
        for (low_i = 0; low_i < 10; low_i++) {
          y2 = y[low_i * 7];
          Vq[low_i] = ((y[(low_i * 7) + 1] - y2) * xtmp) + y2;
        }
      } else {
        low_i = 1;
        offset = 2;
        high_i = 7;
        while (high_i > offset) {
          mid_i = ((low_i + high_i) >> ((uint64_T)1));
          if (varargin_3 >= x[mid_i - 1]) {
            low_i = mid_i;
            offset = mid_i + 1;
          } else {
            high_i = mid_i;
          }
        }

        xtmp = x[low_i - 1];
        xtmp = (varargin_3 - xtmp) / (x[low_i] - xtmp);
        if (xtmp == 0.0) {
          for (offset = 0; offset < 10; offset++) {
            Vq[offset] = y[((offset * 7) + low_i) - 1];
          }
        } else if (xtmp == 1.0) {
          for (offset = 0; offset < 10; offset++) {
            Vq[offset] = y[(offset * 7) + low_i];
          }
        } else {
          for (offset = 0; offset < 10; offset++) {
            high_i = ((offset * 7) + low_i) - 1;
            y2 = y[(offset * 7) + low_i];
            if (y[high_i] == y2) {
              Vq[offset] = y[high_i];
            } else {
              Vq[offset] = ((1.0 - xtmp) * y[high_i]) + (xtmp * y2);
            }
          }
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
