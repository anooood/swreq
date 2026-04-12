/*
 * File: sqrt_pNDOZDUg.c
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
 */

#include "rtwtypes.h"
#include <math.h>
#include "sqrt_pNDOZDUg.h"

/* Function for MATLAB Function: '<S1>/Data Controller' */
void sqrt_pNDOZDUg(real_T x[10])
{
  int32_T k;
  for (k = 0; k < 10; k++) {
    x[k] = sqrt(x[k]);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
