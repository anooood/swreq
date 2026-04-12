/*
 * File: xrotg_j0TJQ8yO.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.567
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Thu Aug  4 15:06:58 2022
 */

#include "rtwtypes.h"
#include <math.h>
#include "xrotg_j0TJQ8yO.h"

/* Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
void xrotg_j0TJQ8yO(real_T *a, real_T *b, real_T *c, real_T *s)
{
  real_T roe;
  real_T absa;
  real_T absb;
  real_T scale;
  real_T ads;
  real_T bds;
  roe = *b;
  absa = fabs(*a);
  absb = fabs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    *b = 0.0;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    scale *= sqrt((ads * ads) + (bds * bds));
    if (roe < 0.0) {
      scale = -scale;
    }

    *c = (*a) / scale;
    *s = (*b) / scale;
    if (absa > absb) {
      *b = *s;
    } else if ((*c) != 0.0) {
      *b = 1.0 / (*c);
    } else {
      *b = 1.0;
    }
  }

  *a = scale;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
