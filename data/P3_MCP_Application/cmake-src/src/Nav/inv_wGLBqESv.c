/*
 * File: inv_wGLBqESv.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include "xgetrf_Kr78gVIr.h"
#include "inv_wGLBqESv.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void inv_wGLBqESv(const real_T x[361], real_T y[361])
{
  int8_T p[19];
  real_T b_x[361];
  int32_T ipiv[19];
  int32_T j;
  int32_T pipk;
  int32_T b_k;
  int32_T kAcol;
  int32_T b_i;
  int32_T i;
  int32_T y_tmp;
  for (i = 0; i < 361; i++) {
    y[i] = 0.0;
    b_x[i] = x[i];
  }

  xgetrf_Kr78gVIr(b_x, ipiv);
  for (i = 0; i < 19; i++) {
    p[i] = (int8_T)(1 + i);
  }

  for (b_k = 0; b_k < 18; b_k++) {
    if (ipiv[b_k] > (1 + b_k)) {
      pipk = (int32_T)p[ipiv[b_k] - 1];
      p[ipiv[b_k] - 1] = p[b_k];
      p[b_k] = (int8_T)pipk;
    }
  }

  for (b_k = 0; b_k < 19; b_k++) {
    pipk = ((int32_T)p[b_k]) - 1;
    y[b_k + (19 * pipk)] = 1.0;
    for (j = b_k; (j + 1) < 20; j++) {
      i = (19 * pipk) + j;
      if (y[i] != 0.0) {
        for (kAcol = j + 1; (kAcol + 1) < 20; kAcol++) {
          y_tmp = (19 * pipk) + kAcol;
          y[y_tmp] -= y[i] * b_x[(19 * j) + kAcol];
        }
      }
    }
  }

  for (b_k = 0; b_k < 19; b_k++) {
    pipk = 19 * b_k;
    for (j = 18; j >= 0; j--) {
      kAcol = 19 * j;
      i = j + pipk;
      if (y[i] != 0.0) {
        y[i] /= b_x[j + kAcol];
        for (b_i = 0; b_i < j; b_i++) {
          y_tmp = b_i + pipk;
          y[y_tmp] -= y[i] * b_x[b_i + kAcol];
        }
      }
    }
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
