/*
 * File: interpn_s1jYV2bZ.c
 *
 * Code generated for Simulink model 'Autopilot_300Hz_Function_1D'.
 *
 * Model version                  : 1.2104
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:35:53 2023
 */

#include "rtwtypes.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "interpn_s1jYV2bZ.h"

/* Function for MATLAB Function: '<S1>/Autopilot Gains' */
real_T interpn_s1jYV2bZ(const real_T varargin_1[6], const real_T varargin_2[5],
  const real_T varargin_3[30], real_T varargin_4, real_T varargin_5)
{
  real_T Vq;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T b_high_i;
  int32_T b_mid_i;
  real_T v[4];
  real_T xbox_idx_0;
  real_T xbox_idx_1;
  real_T xbox_idx_2;
  if (((!(varargin_4 >= varargin_1[0])) || (!(varargin_4 <= varargin_1[5]))) ||
      ((!(varargin_5 >= varargin_2[0])) || (!(varargin_5 <= varargin_2[4])))) {
    Vq = (rtNaN);
  } else {
    low_i = 1;
    low_ip1 = 2;
    high_i = 6;
    while (high_i > low_ip1) {
      b_high_i = ((low_i + high_i) >> ((uint64_T)1));
      if (varargin_4 >= varargin_1[b_high_i - 1]) {
        low_i = b_high_i;
        low_ip1 = b_high_i + 1;
      } else {
        high_i = b_high_i;
      }
    }

    xbox_idx_0 = varargin_1[low_i - 1];
    xbox_idx_1 = varargin_1[low_i];
    low_ip1 = 1;
    high_i = 2;
    b_high_i = 5;
    while (b_high_i > high_i) {
      b_mid_i = ((low_ip1 + b_high_i) >> ((uint64_T)1));
      if (varargin_5 >= varargin_2[b_mid_i - 1]) {
        low_ip1 = b_mid_i;
        high_i = b_mid_i + 1;
      } else {
        b_high_i = b_mid_i;
      }
    }

    low_i += (low_ip1 - 1) * 6;
    xbox_idx_2 = varargin_2[low_ip1 - 1];
    v[0] = varargin_3[low_i - 1];
    v[1] = varargin_3[low_i];
    v[2] = varargin_3[low_i + 5];
    v[3] = varargin_3[low_i + 6];
    if (varargin_4 == xbox_idx_0) {
      for (high_i = 0; high_i < 2; high_i++) {
        v[high_i] = v[((high_i + 1) << ((uint64_T)1)) - 2];
      }
    } else if (varargin_4 == xbox_idx_1) {
      for (high_i = 0; high_i < 2; high_i++) {
        v[high_i] = v[((high_i + 1) << ((uint64_T)1)) - 1];
      }
    } else {
      xbox_idx_0 = (varargin_4 - xbox_idx_0) / (xbox_idx_1 - xbox_idx_0);
      for (high_i = 1; (high_i - 1) < 2; high_i++) {
        low_i = (high_i << ((uint64_T)1));
        v[high_i - 1] = (v[low_i - 2] * (1.0 - xbox_idx_0)) + (v[low_i - 1] *
          xbox_idx_0);
      }
    }

    if (!(varargin_5 == xbox_idx_2)) {
      if (varargin_5 == varargin_2[low_ip1]) {
        v[0] = v[1];
      } else {
        xbox_idx_0 = (varargin_5 - xbox_idx_2) / (varargin_2[low_ip1] -
          xbox_idx_2);
        for (high_i = 1; (high_i - 1) < 1; high_i++) {
          low_i = (high_i << ((uint64_T)1));
          v[high_i - 1] = (v[low_i - 2] * (1.0 - xbox_idx_0)) + (v[low_i - 1] *
            xbox_idx_0);
        }
      }
    }

    Vq = v[0];
  }

  return Vq;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
