/*
 * File: LIB_unitvector_gdXEDyOJ.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1498
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 08:25:17 2023
 */

#include "rtwtypes.h"
#include "norm_MfVYifVG.h"
#include "LIB_unitvector_gdXEDyOJ.h"

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function out = LIB_unitvector(in)
 */
void LIB_unitvector_gdXEDyOJ(const real_T in[3], real_T out[3])
{
  real_T den;

  /* 'LIB_unitvector:4' den = norm(in); */
  den = norm_MfVYifVG(in);

  /* 'LIB_unitvector:5' if (den > 1.0e-12) */
  if (den > 1.0E-12) {
    /* 'LIB_unitvector:6' out = in/den; */
    out[0] = in[0] / den;
    out[1] = in[1] / den;
    out[2] = in[2] / den;
  } else {
    /* 'LIB_unitvector:7' else */
    /* 'LIB_unitvector:8' out = zeros(size(in)); */
    out[1] = 0.0;
    out[2] = 0.0;

    /* 'LIB_unitvector:9' out(1) = 1.0; */
    out[0] = 1.0;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
