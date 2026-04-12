/*
 * File: f_ee_aoa441JQ.c
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
 */

#include "rtwtypes.h"
#include "f_ee_aoa441JQ.h"

/*
 * Function for MATLAB Function: '<S1>/Data Controller'
 * function [ent, exd, inside] = f_ee(b1,b2)
 */
void f_ee_aoa441JQ(real_T b1, real_T b2, real_T *ent, real_T *exd, real_T
                   *inside)
{
  /* 'LARTimings:64' if b1 > 0 */
  if (b1 > 0.0) {
    /* 'LARTimings:65' ent = b1; */
    *ent = b1;

    /* 'LARTimings:65' exd = b2; */
    *exd = b2;

    /* 'LARTimings:65' inside = 0; */
    *inside = 0.0;
  } else if (b2 > 0.0) {
    /* 'LARTimings:66' elseif b2 > 0 */
    /* 'LARTimings:67' ent = 0; */
    *ent = 0.0;

    /* 'LARTimings:67' exd = b2; */
    *exd = b2;

    /* 'LARTimings:67' inside = 1; */
    *inside = 1.0;
  } else {
    /* 'LARTimings:68' else */
    /* 'LARTimings:69' ent = 0; */
    *ent = 0.0;

    /* 'LARTimings:69' exd = 0; */
    *exd = 0.0;

    /* 'LARTimings:69' inside = 0; */
    *inside = 0.0;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
