/*
 * File: interp2_OIomwLVf.c
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
 */

#include "rtwtypes.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "interp2_OIomwLVf.h"

/* Function for MATLAB Function: '<S1>/Data Controller' */
real_T interp2_OIomwLVf(const real_T varargin_1[3], const real_T varargin_2[6],
  const real_T varargin_3[18], real_T varargin_4, real_T varargin_5)
{
  real_T Vq;
  int32_T low_i;
  int32_T low_ip1;
  int32_T high_i;
  int32_T b_high_i;
  int32_T mid_i;
  real_T qx1;
  real_T rx;
  if (varargin_4 >= varargin_1[0]) {
    if (varargin_4 <= varargin_1[2]) {
      if (varargin_5 >= varargin_2[0]) {
        if (varargin_5 <= varargin_2[5]) {
          low_i = 0;
          if (varargin_4 >= varargin_1[1]) {
            low_i = 1;
          }

          low_ip1 = 1;
          high_i = 2;
          b_high_i = 6;
          while (b_high_i > high_i) {
            mid_i = ((low_ip1 + b_high_i) >> ((uint64_T)1));
            if (varargin_5 >= varargin_2[mid_i - 1]) {
              low_ip1 = mid_i;
              high_i = mid_i + 1;
            } else {
              b_high_i = mid_i;
            }
          }

          if (varargin_4 == varargin_1[low_i]) {
            low_i = (6 * low_i) + low_ip1;
            qx1 = varargin_3[low_i - 1];
            Vq = varargin_3[low_i];
          } else {
            rx = varargin_1[low_i + 1];
            if (rx == varargin_4) {
              low_i = ((low_i + 1) * 6) + low_ip1;
              qx1 = varargin_3[low_i - 1];
              Vq = varargin_3[low_i];
            } else {
              rx = (varargin_4 - varargin_1[low_i]) / (rx - varargin_1[low_i]);
              if (varargin_3[(((low_i + 1) * 6) + low_ip1) - 1] == varargin_3
                  [((6 * low_i) + low_ip1) - 1]) {
                qx1 = varargin_3[((6 * low_i) + low_ip1) - 1];
              } else {
                qx1 = (varargin_3[(((low_i + 1) * 6) + low_ip1) - 1] * rx) +
                  (varargin_3[((6 * low_i) + low_ip1) - 1] * (1.0 - rx));
              }

              if (varargin_3[((low_i + 1) * 6) + low_ip1] == varargin_3[(6 *
                   low_i) + low_ip1]) {
                Vq = varargin_3[(6 * low_i) + low_ip1];
              } else {
                Vq = (varargin_3[((low_i + 1) * 6) + low_ip1] * rx) +
                  (varargin_3[(6 * low_i) + low_ip1] * (1.0 - rx));
              }
            }
          }

          rx = varargin_2[low_ip1 - 1];
          if (rx == varargin_5) {
            Vq = qx1;
          } else if (qx1 == Vq) {
            Vq = qx1;
          } else {
            if (!(varargin_5 == varargin_2[low_ip1])) {
              rx = (varargin_5 - rx) / (varargin_2[low_ip1] - rx);
              Vq = ((1.0 - rx) * qx1) + (rx * Vq);
            }
          }
        } else {
          Vq = (rtNaN);
        }
      } else {
        Vq = (rtNaN);
      }
    } else {
      Vq = (rtNaN);
    }
  } else {
    Vq = (rtNaN);
  }

  return Vq;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
