/*
 * File: xscal_ujQZn9m2.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include "xscal_ujQZn9m2.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void xscal_ujQZn9m2(real_T a, real_T x[9], int32_T ix0)
{
  int32_T k;
  for (k = ix0; k <= (ix0 + 2); k++) {
    x[k - 1] *= a;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
