/*
 * File: inv_gKrb5zHJ.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include "xgetrf_4kjt1P2q.h"
#include "inv_gKrb5zHJ.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void inv_gKrb5zHJ(const real_T x[225], real_T y[225])
{
  int8_T p[15];
  real_T b_x[225];
  int32_T ipiv[15];
  int32_T j;
  int32_T pipk;
  int32_T b_k;
  int32_T kAcol;
  int32_T b_i;
  int32_T i;
  int32_T y_tmp;
  for (i = 0; i < 225; i++) {
    y[i] = 0.0;
    b_x[i] = x[i];
  }

  xgetrf_4kjt1P2q(b_x, ipiv);
  for (i = 0; i < 15; i++) {
    p[i] = (int8_T)(1 + i);
  }

  for (b_k = 0; b_k < 14; b_k++) {
    if (ipiv[b_k] > (1 + b_k)) {
      pipk = (int32_T)p[ipiv[b_k] - 1];
      p[ipiv[b_k] - 1] = p[b_k];
      p[b_k] = (int8_T)pipk;
    }
  }

  for (b_k = 0; b_k < 15; b_k++) {
    pipk = ((int32_T)p[b_k]) - 1;
    y[b_k + (15 * pipk)] = 1.0;
    for (j = b_k; (j + 1) < 16; j++) {
      i = (15 * pipk) + j;
      if (y[i] != 0.0) {
        for (kAcol = j + 1; (kAcol + 1) < 16; kAcol++) {
          y_tmp = (15 * pipk) + kAcol;
          y[y_tmp] -= y[i] * b_x[(15 * j) + kAcol];
        }
      }
    }
  }

  for (b_k = 0; b_k < 15; b_k++) {
    pipk = 15 * b_k;
    for (j = 14; j >= 0; j--) {
      kAcol = 15 * j;
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
