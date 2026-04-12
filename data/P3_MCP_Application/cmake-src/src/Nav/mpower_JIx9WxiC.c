/*
 * File: mpower_JIx9WxiC.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_100Hz'.
 *
 * Model version                  : 1.605
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:08:05 2022
 */

#include "rtwtypes.h"
#include "mpower_JIx9WxiC.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
void mpower_JIx9WxiC(const real_T a[256], real_T c[256])
{
  real_T a_0[256];
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T a_tmp;
  int32_T a_tmp_0;
  for (i_0 = 0; i_0 < 16; i_0++) {
    for (i = 0; i < 16; i++) {
      a_tmp = (i_0 << ((uint64_T)4));
      a_tmp_0 = i + a_tmp;
      a_0[a_tmp_0] = 0.0;
      for (i_1 = 0; i_1 < 16; i_1++) {
        a_0[a_tmp_0] += a[(i_1 << ((uint64_T)4)) + i] * a[a_tmp + i_1];
      }
    }
  }

  for (i_0 = 0; i_0 < 16; i_0++) {
    for (i = 0; i < 16; i++) {
      a_tmp = (i_0 << ((uint64_T)4));
      a_tmp_0 = i + a_tmp;
      c[a_tmp_0] = 0.0;
      for (i_1 = 0; i_1 < 16; i_1++) {
        c[a_tmp_0] += a[(i_1 << ((uint64_T)4)) + i] * a_0[a_tmp + i_1];
      }
    }
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
