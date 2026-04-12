/*
 * File: det_U5whr0XH.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include "det_U5whr0XH.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
real_T det_U5whr0XH(const real_T x[9])
{
  real_T y;
  boolean_T isodd;
  real_T A[9];
  int8_T ipiv[3];
  int32_T j;
  int32_T c;
  int32_T ix;
  real_T smax;
  real_T s;
  int32_T c_k;
  int32_T iy;
  int32_T c_ix;
  int32_T d;
  int32_T ijA;
  memcpy(&A[0], &x[0], 9U * (sizeof(real_T)));
  ipiv[0] = 1;
  ipiv[1] = 2;
  for (j = 0; j < 2; j++) {
    c = (j << ((uint64_T)2));
    iy = 0;
    ix = c;
    smax = fabs(A[c]);
    for (c_k = 2; c_k <= (3 - j); c_k++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        iy = c_k - 1;
        smax = s;
      }
    }

    if (A[c + iy] != 0.0) {
      if (iy != 0) {
        iy += j;
        ipiv[j] = (int8_T)(iy + 1);
        smax = A[j];
        A[j] = A[iy];
        A[iy] = smax;
        ix = j + 3;
        iy += 3;
        smax = A[ix];
        A[ix] = A[iy];
        A[iy] = smax;
        ix += 3;
        iy += 3;
        smax = A[ix];
        A[ix] = A[iy];
        A[iy] = smax;
      }

      iy = (c - j) + 3;
      for (ix = c + 1; ix < iy; ix++) {
        A[ix] /= A[c];
      }
    }

    iy = c + 4;
    ix = c + 3;
    for (c_k = 0; c_k <= (1 - j); c_k++) {
      smax = A[ix];
      if (A[ix] != 0.0) {
        c_ix = c + 1;
        d = (iy - j) + 2;
        for (ijA = iy; ijA < d; ijA++) {
          A[ijA] += A[c_ix] * (-smax);
          c_ix++;
        }
      }

      ix += 3;
      iy += 3;
    }
  }

  isodd = false;
  if (ipiv[0] > 1) {
    isodd = true;
  }

  y = (A[0] * A[4]) * A[8];
  if (ipiv[1] > 2) {
    isodd = !isodd;
  }

  if (isodd) {
    y = -y;
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
