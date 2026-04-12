/*
 * File: xrot_R8eANFiG.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include "xrot_R8eANFiG.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void xrot_R8eANFiG(real_T x[9], int32_T ix0, int32_T iy0, real_T c, real_T s)
{
  int32_T ix;
  int32_T iy;
  real_T temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  temp = (c * x[ix]) + (s * x[iy]);
  x[iy] = (c * x[iy]) - (s * x[ix]);
  x[ix] = temp;
  iy++;
  ix++;
  temp = (c * x[ix]) + (s * x[iy]);
  x[iy] = (c * x[iy]) - (s * x[ix]);
  x[ix] = temp;
  iy++;
  ix++;
  temp = (c * x[ix]) + (s * x[iy]);
  x[iy] = (c * x[iy]) - (s * x[ix]);
  x[ix] = temp;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
