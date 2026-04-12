/*
 * File: cross_6Ajhxtbg.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1498
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 08:25:17 2023
 */

#include "rtwtypes.h"
#include "cross_6Ajhxtbg.h"

/* Function for MATLAB Function: '<S1>/Guidance Controller' */
void cross_6Ajhxtbg(const real_T a[3], const real_T b[3], real_T c[3])
{
  c[0] = (a[1] * b[2]) - (a[2] * b[1]);
  c[1] = (a[2] * b[0]) - (a[0] * b[2]);
  c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
