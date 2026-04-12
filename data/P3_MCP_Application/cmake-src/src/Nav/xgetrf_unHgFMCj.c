/*
 * File: xgetrf_unHgFMCj.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include "xgetrf_unHgFMCj.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void xgetrf_unHgFMCj(real_T A[169], int32_T ipiv[13])
{
  int32_T j;
  int32_T c;
  int32_T ix;
  real_T smax;
  real_T s;
  int32_T k;
  int32_T iy;
  int32_T c_ix;
  int32_T d;
  int32_T ijA;
  for (j = 0; j < 13; j++) {
    ipiv[j] = 1 + j;
  }

  for (j = 0; j < 12; j++) {
    c = j * 14;
    iy = 0;
    ix = c;
    smax = fabs(A[c]);
    for (k = 2; k <= (13 - j); k++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        iy = k - 1;
        smax = s;
      }
    }

    if (A[c + iy] != 0.0) {
      if (iy != 0) {
        iy += j;
        ipiv[j] = iy + 1;
        ix = j;
        for (k = 0; k < 13; k++) {
          smax = A[ix];
          A[ix] = A[iy];
          A[iy] = smax;
          ix += 13;
          iy += 13;
        }
      }

      iy = (c - j) + 13;
      for (ix = c + 1; ix < iy; ix++) {
        A[ix] /= A[c];
      }
    }

    iy = c + 14;
    ix = c + 13;
    for (k = 0; k <= (11 - j); k++) {
      smax = A[ix];
      if (A[ix] != 0.0) {
        c_ix = c + 1;
        d = (iy - j) + 12;
        for (ijA = iy; ijA < d; ijA++) {
          A[ijA] += A[c_ix] * (-smax);
          c_ix++;
        }
      }

      ix += 13;
      iy += 13;
    }
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
