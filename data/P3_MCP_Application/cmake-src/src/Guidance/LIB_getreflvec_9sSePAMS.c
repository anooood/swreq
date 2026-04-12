/*
 * File: LIB_getreflvec_9sSePAMS.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1498
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 08:25:17 2023
 */

#include "rtwtypes.h"
#include <math.h>
#include "norm_MfVYifVG.h"
#include "rt_hypotd_snf.h"
#include "xnrm2_31kUuzXk.h"
#include "LIB_getreflvec_9sSePAMS.h"

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [r1arc,Rarc,Oarc]=LIB_getreflvec(a,b,r0arc)
 */
void LIB_getreflvec_9sSePAMS(const real_T a[3], const real_T b[3], const real_T
  r0arc[3], real_T r1arc[3], real_T *Rarc, real_T Oarc[3])
{
  real_T xprd[3];
  real_T dprd;
  real_T xrev[3];
  real_T dotprd;
  boolean_T y;
  real_T xnorm;
  int32_T knt;
  int32_T f_k;
  real_T arev_idx_2;
  real_T c_idx_1;
  real_T c_idx_2;
  real_T c_idx_0;
  boolean_T exitg1;

  /* 'LIB_getreflvec:41' xprd = cross(b,a); */
  xprd[0] = (b[1] * a[2]) - (b[2] * a[1]);
  xprd[1] = (b[2] * a[0]) - (b[0] * a[2]);
  xprd[2] = (b[0] * a[1]) - (b[1] * a[0]);

  /* 'LIB_getreflvec:42' dprd = dot(b,a); */
  /* 'LIB_getreflvec:43' xrev = -xprd; */
  /* 'LIB_getreflvec:44' arev = -a; */
  /* 'LIB_getreflvec:46' dotprd = dot(arev,arev); */
  dprd = ((b[0] * a[0]) + (b[1] * a[1])) + (b[2] * a[2]);
  dotprd = (((-a[0]) * (-a[0])) + ((-a[1]) * (-a[1]))) + ((-a[2]) * (-a[2]));

  /* 'LIB_getreflvec:47' if (abs(dotprd) > 0.001) */
  if (fabs(dotprd) > 0.001) {
    /* 'LIB_getreflvec:48' c = (cross(arev,xrev) + dprd*arev)/dotprd; */
    c_idx_0 = ((((-a[1]) * (-xprd[2])) - ((-a[2]) * (-xprd[1]))) + (dprd * (-a[0])))
      / dotprd;
    c_idx_1 = ((((-a[2]) * (-xprd[0])) - ((-a[0]) * (-xprd[2]))) + (dprd * (-a[1])))
      / dotprd;
    c_idx_2 = ((((-a[0]) * (-xprd[1])) - ((-a[1]) * (-xprd[0]))) + (dprd * (-a[2])))
      / dotprd;
  } else {
    /* 'LIB_getreflvec:49' else */
    /* 'LIB_getreflvec:50' c = (cross(arev,xrev) + dprd*arev)/0.001; */
    c_idx_0 = ((((-a[1]) * (-xprd[2])) - ((-a[2]) * (-xprd[1]))) + (dprd * (-a[0])))
      / 0.001;
    c_idx_1 = ((((-a[2]) * (-xprd[0])) - ((-a[0]) * (-xprd[2]))) + (dprd * (-a[1])))
      / 0.001;
    c_idx_2 = ((((-a[0]) * (-xprd[1])) - ((-a[1]) * (-xprd[0]))) + (dprd * (-a[2])))
      / 0.001;
  }

  /* 'LIB_getreflvec:52' r1arc = r0arc + a + c; */
  r1arc[0] = (r0arc[0] + a[0]) + c_idx_0;
  r1arc[1] = (r0arc[1] + a[1]) + c_idx_1;
  r1arc[2] = (r0arc[2] + a[2]) + c_idx_2;

  /* 'LIB_getreflvec:54' d0 = cross(xprd,b); */
  xrev[0] = (xprd[1] * b[2]) - (xprd[2] * b[1]);
  xrev[1] = (xprd[2] * b[0]) - (xprd[0] * b[2]);
  xrev[2] = (xprd[0] * b[1]) - (xprd[1] * b[0]);

  /* 'LIB_getreflvec:55' if (abs(d0(1)) < 0.0011) */
  if (fabs(xrev[0]) < 0.0011) {
    /* 'LIB_getreflvec:56' d0(1) = 0.0011; */
    xrev[0] = 0.0011;
  }

  /* 'LIB_getreflvec:58' d0n = d0/norm(d0); */
  dotprd = norm_MfVYifVG(xrev);

  /* 'LIB_getreflvec:60' d1 = cross(cross(c,-a),c); */
  Oarc[0] = xrev[0] / dotprd;
  Oarc[1] = xrev[1] / dotprd;
  Oarc[2] = xrev[2] / dotprd;
  dprd = (c_idx_1 * (-a[2])) - (c_idx_2 * (-a[1]));
  dotprd = (c_idx_2 * (-a[0])) - (c_idx_0 * (-a[2]));
  arev_idx_2 = (c_idx_0 * (-a[1])) - (c_idx_1 * (-a[0]));
  xprd[0] = (dotprd * c_idx_2) - (arev_idx_2 * c_idx_1);
  xprd[1] = (arev_idx_2 * c_idx_0) - (dprd * c_idx_2);
  xprd[2] = (dprd * c_idx_1) - (dotprd * c_idx_0);

  /* 'LIB_getreflvec:61' if (abs(d1(1)) < 0.001) */
  if (fabs(xprd[0]) < 0.001) {
    /* 'LIB_getreflvec:62' d1(1) = 0.001; */
    xprd[0] = 0.001;
  }

  /* 'LIB_getreflvec:64' d1n = d1/norm(d1); */
  dotprd = norm_MfVYifVG(xprd);
  xrev[0] = xprd[0] / dotprd;
  xrev[1] = xprd[1] / dotprd;
  xrev[2] = xprd[2] / dotprd;

  /* 'LIB_getreflvec:66' a0 = b; */
  /* 'LIB_getreflvec:67' a1 = a + c; */
  /* 'LIB_getreflvec:69' if (d0n==d1n) */
  y = true;
  knt = 0;
  exitg1 = false;
  while ((!exitg1) && (knt < 3)) {
    if (!(Oarc[knt] == xrev[knt])) {
      y = false;
      exitg1 = true;
    } else {
      knt++;
    }
  }

  if (y) {
    /* 'LIB_getreflvec:70' Rarc = 1.0e12; */
    *Rarc = 1.0E+12;
  } else {
    /* 'LIB_getreflvec:71' else */
    /* 'LIB_getreflvec:72' Rarc = (a1-a0)/(d0n-d1n); */
    xrev[0] = Oarc[0] - xrev[0];
    c_idx_0 = (a[0] + c_idx_0) - b[0];
    xrev[1] = Oarc[1] - xrev[1];
    c_idx_1 = (a[1] + c_idx_1) - b[1];
    arev_idx_2 = Oarc[2] - xrev[2];
    xrev[2] = arev_idx_2;
    dotprd = xrev[0];
    dprd = 0.0;
    xnorm = xnrm2_31kUuzXk(xrev);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(xrev[0], xnorm);
      if (xrev[0] >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = -1;
        do {
          knt++;
          xrev[1] *= 9.9792015476736E+291;
          xrev[2] *= 9.9792015476736E+291;
          xnorm *= 9.9792015476736E+291;
          dotprd *= 9.9792015476736E+291;
        } while (!(fabs(xnorm) >= 1.0020841800044864E-292));

        xnorm = rt_hypotd_snf(dotprd, xnrm2_31kUuzXk(xrev));
        if (dotprd >= 0.0) {
          xnorm = -xnorm;
        }

        dprd = (xnorm - dotprd) / xnorm;
        dotprd = 1.0 / (dotprd - xnorm);
        xrev[1] *= dotprd;
        xrev[2] *= dotprd;
        for (f_k = 0; f_k <= knt; f_k++) {
          xnorm *= 1.0020841800044864E-292;
        }

        dotprd = xnorm;
      } else {
        dprd = (xnorm - xrev[0]) / xnorm;
        dotprd = 1.0 / (xrev[0] - xnorm);
        xrev[1] *= dotprd;
        xrev[2] = dotprd * arev_idx_2;
        dotprd = xnorm;
      }
    }

    xrev[0] = dotprd;
    knt = 0;
    dotprd = fabs(dotprd);
    if (!(dotprd <= (6.6613381477509392E-15 * dotprd))) {
      knt = 1;
    }

    *Rarc = 0.0;
    if (dprd != 0.0) {
      dotprd = ((((a[2] + c_idx_2) - b[2]) * xrev[2]) + ((xrev[1] * c_idx_1) +
                 c_idx_0)) * dprd;
      if (dotprd != 0.0) {
        c_idx_0 -= dotprd;
      }
    }

    f_k = 0;
    while (f_k <= (knt - 1)) {
      *Rarc = c_idx_0;
      f_k = 1;
    }

    while (knt > 0) {
      *Rarc /= xrev[0];
      knt = 0;
    }
  }

  /* 'LIB_getreflvec:76' Oarc = r0arc + b + Rarc*d0n; */
  Oarc[0] = (r0arc[0] + b[0]) + ((*Rarc) * Oarc[0]);
  Oarc[1] = (r0arc[1] + b[1]) + ((*Rarc) * Oarc[1]);
  Oarc[2] = (r0arc[2] + b[2]) + ((*Rarc) * Oarc[2]);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
