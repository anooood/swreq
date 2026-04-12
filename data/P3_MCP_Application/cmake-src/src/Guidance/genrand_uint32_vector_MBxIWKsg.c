/*
 * File: genrand_uint32_vector_MBxIWKsg.c
 *
 * Code generated for Simulink model 'TTKF_100Hz_Function_1D'.
 *
 * Model version                  : 1.1467
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:33:35 2023
 */

#include "rtwtypes.h"
#include "genrand_uint32_vector_MBxIWKsg.h"

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
void genrand_uint32_vector_MBxIWKsg(uint32_T mt[625], uint32_T u[2])
{
  uint32_T mti;
  uint32_T y;
  int32_T j;
  int32_T kk;
  for (j = 0; j < 2; j++) {
    mti = mt[624] + 1U;
    if (mti >= 625U) {
      for (kk = 0; kk < 227; kk++) {
        y = (mt[1 + kk] & 2147483647U) | (mt[kk] & 2147483648U);
        if ((y & 1U) == 0U) {
          y >>= 1U;
        } else {
          y = (y >> 1U) ^ 2567483615U;
        }

        mt[kk] = mt[kk + 397] ^ y;
      }

      for (kk = 0; kk < 396; kk++) {
        y = (mt[kk + 227] & 2147483648U) | (mt[228 + kk] & 2147483647U);
        if ((y & 1U) == 0U) {
          y >>= 1U;
        } else {
          y = (y >> 1U) ^ 2567483615U;
        }

        mt[kk + 227] = mt[kk] ^ y;
      }

      y = (mt[623] & 2147483648U) | (mt[0] & 2147483647U);
      if ((y & 1U) == 0U) {
        y >>= 1U;
      } else {
        y = (y >> 1U) ^ 2567483615U;
      }

      mt[623] = mt[396] ^ y;
      mti = 1U;
    }

    y = mt[((int32_T)mti) - 1];
    mt[624] = mti;
    y ^= (y >> 11U);
    y ^= (y << 7U) & 2636928640U;
    y ^= (y << 15U) & 4022730752U;
    u[j] = (y >> 18U) ^ y;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
