/*
 * File: inv_SN4v6bUz.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include "xgetrf_kWRqFoTV.h"
#include "inv_SN4v6bUz.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void inv_SN4v6bUz(const real_T x[256], real_T y[256])
{
  int8_T p[16];
  real_T b_x[256];
  int32_T ipiv[16];
  int32_T j;
  int32_T pipk;
  int32_T b_k;
  int32_T kAcol;
  int32_T i;
  int32_T kAcol_tmp;
  int32_T y_tmp;
  for (i = 0; i < 256; i++) {
    y[i] = 0.0;
    b_x[i] = x[i];
  }

  xgetrf_kWRqFoTV(b_x, ipiv);
  for (i = 0; i < 16; i++) {
    p[i] = (int8_T)(1 + i);
  }

  for (b_k = 0; b_k < 15; b_k++) {
    if (ipiv[b_k] > (1 + b_k)) {
      pipk = (int32_T)p[ipiv[b_k] - 1];
      p[ipiv[b_k] - 1] = p[b_k];
      p[b_k] = (int8_T)pipk;
    }
  }

  for (b_k = 0; b_k < 16; b_k++) {
    pipk = ((int32_T)p[b_k]) - 1;
    y[b_k + (pipk << ((uint64_T)4))] = 1.0;
    for (j = b_k; (j + 1) < 17; j++) {
      i = (pipk << ((uint64_T)4));
      kAcol_tmp = i + j;
      if (y[kAcol_tmp] != 0.0) {
        for (kAcol = j + 1; (kAcol + 1) < 17; kAcol++) {
          y_tmp = i + kAcol;
          y[y_tmp] -= y[kAcol_tmp] * b_x[(j << ((uint64_T)4)) + kAcol];
        }
      }
    }
  }

  for (b_k = 0; b_k < 16; b_k++) {
    pipk = (b_k << ((uint64_T)4));
    for (j = 15; j >= 0; j--) {
      kAcol_tmp = (j << ((uint64_T)4));
      i = j + pipk;
      if (y[i] != 0.0) {
        y[i] /= b_x[j + kAcol_tmp];
        for (kAcol = 0; kAcol < j; kAcol++) {
          y_tmp = kAcol + pipk;
          y[y_tmp] -= y[i] * b_x[kAcol + kAcol_tmp];
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
