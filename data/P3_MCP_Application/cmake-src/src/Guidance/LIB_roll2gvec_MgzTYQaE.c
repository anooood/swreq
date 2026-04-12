/*
 * File: LIB_roll2gvec_MgzTYQaE.c
 *
 * Code generated for Simulink model 'Guidance_100Hz_Function_1D'.
 *
 * Model version                  : 1.1498
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue May  9 08:25:17 2023
 */

#include "rtwtypes.h"
#include <math.h>
#include "norm_lpbhAiWU.h"
#include "rt_atan2d_snf.h"
#include "LIB_roll2gvec_MgzTYQaE.h"

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [phic_out,phi_gic,phiint] = LIB_roll2gvec(phic_in,phic_prev,phim,giyc,gizc,phiint,AZphi,philim,SRlim,Glim)
 */
void LIB_roll2gvec_MgzTYQaE(real_T phic_in, real_T phic_prev, real_T phim,
  real_T giyc, real_T gizc, real_T *phiint, real_T AZphi, real_T philim, real_T
  SRlim, real_T Glim, real_T *phic_out, real_T *phi_gic)
{
  real_T fac;
  real_T gic_mag;
  real_T giyc_0[2];

  /* 'LIB_roll2gvec:37' if (phim > 1.571) */
  if (phim > 1.571) {
    /* 'LIB_roll2gvec:38' phim =  pi-phim; */
    phim = 3.1415926535897931 - phim;
  } else {
    if (phim < -1.571) {
      /* 'LIB_roll2gvec:39' elseif (phim < -1.571) */
      /* 'LIB_roll2gvec:40' phim = -pi-phim; */
      phim = -3.1415926535897931 - phim;
    }
  }

  /* 'LIB_roll2gvec:44' gic_phi = atan2(giyc,-gizc); */
  *phi_gic = rt_atan2d_snf(giyc, -gizc);

  /* 'LIB_roll2gvec:46' if (gic_phi > 1.571) */
  if ((*phi_gic) > 1.571) {
    /* 'LIB_roll2gvec:47' gic_phi =  pi-gic_phi; */
    *phi_gic = 3.1415926535897931 - (*phi_gic);
  } else {
    if ((*phi_gic) < -1.571) {
      /* 'LIB_roll2gvec:48' elseif (gic_phi < -1.571) */
      /* 'LIB_roll2gvec:49' gic_phi = -pi-gic_phi; */
      *phi_gic = -3.1415926535897931 - (*phi_gic);
    }
  }

  /* 'LIB_roll2gvec:51' fac = LIB_limit(0.0,1.0,5.0*(norm([giyc gizc])/Glim-0.20)); */
  giyc_0[0] = giyc;
  giyc_0[1] = gizc;
  fac = ((norm_lpbhAiWU(giyc_0) / Glim) - 0.2) * 5.0;

  /* 'LIB_limit:27' if (in > max) */
  if (fac > 1.0) {
    /* 'LIB_limit:28' out = max; */
    fac = 1.0;
  } else if (fac < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    fac = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'LIB_roll2gvec:52' gic_phi = fac*gic_phi; */
  *phi_gic *= fac;

  /* 'LIB_roll2gvec:54' gic_phi = LIB_limit(-philim,philim,gic_phi); */
  /* 'LIB_limit:27' if (in > max) */
  if ((*phi_gic) > philim) {
    /* 'LIB_limit:28' out = max; */
    *phi_gic = philim;
  } else if ((*phi_gic) < (-philim)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    *phi_gic = -philim;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'LIB_roll2gvec:56' gic_mag = sqrt(giyc*giyc+gizc*gizc); */
  gic_mag = sqrt((giyc * giyc) + (gizc * gizc));

  /* 'LIB_roll2gvec:57' phi_del = cos(abs(gic_phi-phim)-0.5236); */
  fac = cos(fabs((*phi_gic) - phim) - 0.5236);

  /* 'LIB_roll2gvec:59' if (gic_mag > 5.0 && phi_del > 0.0) */
  if ((gic_mag > 5.0) && (fac > 0.0)) {
    /* 'LIB_roll2gvec:60' phiint = abs(LIB_limit(-1.0,1.0,phiint + 0.002*LIB_limit(0.0,50.0,gic_mag-5.0)*LIB_limit(-1.0,1.0,phi_del))); */
    /* 'LIB_limit:27' if (in > max) */
    if ((gic_mag - 5.0) > 50.0) {
      /* 'LIB_limit:28' out = max; */
      gic_mag = 50.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      gic_mag -= 5.0;
    }

    /* 'LIB_limit:27' if (in > max) */
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    fac = ((0.002 * gic_mag) * fac) + (*phiint);

    /* 'LIB_limit:27' if (in > max) */
    if (fac > 1.0) {
      /* 'LIB_limit:28' out = max; */
      fac = 1.0;
    } else if (fac < -1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      fac = -1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    *phiint = fabs(fac);

    /* 'LIB_roll2gvec:61' phic_in = gic_phi*phiint; */
    phic_in = (*phi_gic) * (*phiint);
  } else {
    /* 'LIB_roll2gvec:62' else */
    /* 'LIB_roll2gvec:63' phiint = phiint*0.95; */
    *phiint *= 0.95;
  }

  /* 'LIB_roll2gvec:66' if (philim > 1.571) */
  if (philim > 1.571) {
    /* 'LIB_roll2gvec:67' AZtmp = 0.0; */
    fac = 0.0;
  } else {
    /* 'LIB_roll2gvec:68' else */
    /* 'LIB_roll2gvec:69' AZtmp = phiint*LIB_limit(0.5,1.0,1.0-AZphi); */
    /* 'LIB_limit:27' if (in > max) */
    if ((1.0 - AZphi) > 1.0) {
      /* 'LIB_limit:28' out = max; */
      fac = 1.0;
    } else if ((1.0 - AZphi) < 0.5) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      fac = 0.5;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      fac = 1.0 - AZphi;
    }

    fac *= *phiint;
  }

  /* 'LIB_roll2gvec:71' phitmp = (1.0-AZtmp)*phic_in + gic_phi*AZtmp; */
  *phic_out = ((1.0 - fac) * phic_in) + ((*phi_gic) * fac);

  /* 'LIB_roll2gvec:72' phic_out = LIB_limit(phic_prev-SRlim,phic_prev+SRlim,phitmp); */
  fac = phic_prev - SRlim;
  gic_mag = phic_prev + SRlim;

  /* 'LIB_limit:27' if (in > max) */
  if ((*phic_out) > gic_mag) {
    /* 'LIB_limit:28' out = max; */
    *phic_out = gic_mag;
  } else if ((*phic_out) < fac) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    *phic_out = fac;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'LIB_roll2gvec:73' phi_gic = gic_phi; */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
