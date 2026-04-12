/*
 * File: invNxN_W1KzGHzo.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include "invNxN_W1KzGHzo.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void invNxN_W1KzGHzo(const real_T x[49], real_T y[49])
{
  int8_T p[7];
  real_T A[49];
  int8_T ipiv[7];
  int32_T b_j;
  int32_T ix;
  real_T smax;
  real_T s;
  int32_T b_k;
  int32_T iy;
  int32_T c_ix;
  int32_T d;
  int32_T ijA;
  int32_T pipk;
  for (b_k = 0; b_k < 49; b_k++) {
    y[b_k] = 0.0;
    A[b_k] = x[b_k];
  }

  for (b_k = 0; b_k < 7; b_k++) {
    ipiv[b_k] = (int8_T)(1 + b_k);
  }

  for (b_j = 0; b_j < 6; b_j++) {
    pipk = (b_j << ((uint64_T)3));
    iy = 0;
    ix = pipk;
    smax = fabs(A[pipk]);
    for (b_k = 2; b_k <= (7 - b_j); b_k++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        iy = b_k - 1;
        smax = s;
      }
    }

    if (A[pipk + iy] != 0.0) {
      if (iy != 0) {
        iy += b_j;
        ipiv[b_j] = (int8_T)(iy + 1);
        ix = b_j;
        for (b_k = 0; b_k < 7; b_k++) {
          smax = A[ix];
          A[ix] = A[iy];
          A[iy] = smax;
          ix += 7;
          iy += 7;
        }
      }

      iy = (pipk - b_j) + 7;
      for (ix = pipk + 1; ix < iy; ix++) {
        A[ix] /= A[pipk];
      }
    }

    iy = pipk + 8;
    ix = pipk + 7;
    for (b_k = 0; b_k <= (5 - b_j); b_k++) {
      smax = A[ix];
      if (A[ix] != 0.0) {
        c_ix = pipk + 1;
        d = (iy - b_j) + 6;
        for (ijA = iy; ijA < d; ijA++) {
          A[ijA] += A[c_ix] * (-smax);
          c_ix++;
        }
      }

      ix += 7;
      iy += 7;
    }
  }

  for (b_k = 0; b_k < 7; b_k++) {
    p[b_k] = (int8_T)(1 + b_k);
  }

  for (b_j = 0; b_j < 6; b_j++) {
    if (((int32_T)ipiv[b_j]) > (1 + b_j)) {
      d = ((int32_T)ipiv[b_j]) - 1;
      pipk = (int32_T)p[d];
      p[d] = p[b_j];
      p[b_j] = (int8_T)pipk;
    }
  }

  for (b_j = 0; b_j < 7; b_j++) {
    d = ((int32_T)p[b_j]) - 1;
    y[b_j + (7 * d)] = 1.0;
    for (iy = b_j; (iy + 1) < 8; iy++) {
      b_k = (7 * d) + iy;
      if (y[b_k] != 0.0) {
        for (ix = iy + 1; (ix + 1) < 8; ix++) {
          c_ix = (7 * d) + ix;
          y[c_ix] -= y[b_k] * A[(7 * iy) + ix];
        }
      }
    }
  }

  for (b_j = 0; b_j < 7; b_j++) {
    pipk = 7 * b_j;
    for (iy = 6; iy >= 0; iy--) {
      ix = 7 * iy;
      b_k = iy + pipk;
      if (y[b_k] != 0.0) {
        y[b_k] /= A[iy + ix];
        for (b_k = 0; b_k < iy; b_k++) {
          c_ix = b_k + pipk;
          y[c_ix] -= y[iy + pipk] * A[b_k + ix];
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
