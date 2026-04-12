/*
 * File: eye_iBsVrrYi.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include <string.h>
#include "eye_iBsVrrYi.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void eye_iBsVrrYi(real_T b_I[256])
{
  int32_T k;
  memset(&b_I[0], 0, (sizeof(real_T)) << 8U);
  for (k = 0; k < 16; k++) {
    b_I[k + (k << ((uint64_T)4))] = 1.0;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
