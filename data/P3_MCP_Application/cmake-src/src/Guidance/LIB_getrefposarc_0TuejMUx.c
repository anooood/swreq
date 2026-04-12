/*
 * File: LIB_getrefposarc_0TuejMUx.c
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
#include "LIB_getrefposarc_0TuejMUx.h"

/*
 * Function for MATLAB Function: '<S1>/Guidance Controller'
 * function [Rref,Uref]=LIB_getrefposarc(pos,a0,a1,Oarc)
 */
void LIB_getrefposarc_0TuejMUx(real_T pos[3], real_T a0[3], real_T a1[3], const
  real_T Oarc[3], real_T Rref[3], real_T Uref[3])
{
  real_T temp;
  real_T angarc;
  real_T sarc;
  real_T Rref_del[3];
  real_T b_a;
  real_T a0_0;
  real_T a1_0;
  real_T temp_tmp;

  /* 'LIB_getrefposarc:43' a0  = a0  - Oarc; */
  /* 'LIB_getrefposarc:44' a1  = a1  - Oarc; */
  /* 'LIB_getrefposarc:45' pos = pos - Oarc; */
  a0[0] -= Oarc[0];
  a1[0] -= Oarc[0];
  pos[0] -= Oarc[0];
  a0[1] -= Oarc[1];
  a1[1] -= Oarc[1];
  pos[1] -= Oarc[1];
  a0_0 = a0[2] - Oarc[2];
  a1_0 = a1[2] - Oarc[2];
  sarc = pos[2] - Oarc[2];
  a0[2] = a0_0;
  a1[2] = a1_0;
  pos[2] = sarc;

  /* 'LIB_getrefposarc:46' if (abs(a0(1)) < 0.001) */
  if (fabs(a0[0]) < 0.001) {
    /* 'LIB_getrefposarc:47' a0(1) = 0.001; */
    a0[0] = 0.001;
  }

  /* 'LIB_getrefposarc:49' if (abs(a1(1)) < 0.001) */
  if (fabs(a1[0]) < 0.001) {
    /* 'LIB_getrefposarc:50' a1(1) = 0.001; */
    a1[0] = 0.001;
  }

  /* 'LIB_getrefposarc:52' if (abs(pos(1)) < 0.001) */
  if (fabs(pos[0]) < 0.001) {
    /* 'LIB_getrefposarc:53' pos(1) = 0.001; */
    pos[0] = 0.001;
  }

  /* 'LIB_getrefposarc:55' temp = dot(a0,a1)/(norm(a0)*norm(a1)); */
  temp_tmp = norm_MfVYifVG(a0);
  temp = (((a0[0] * a1[0]) + (a0[1] * a1[1])) + (a0_0 * a1_0)) / (temp_tmp *
    norm_MfVYifVG(a1));

  /* 'LIB_getrefposarc:56' if (temp > 0.999999) */
  if (temp > 0.999999) {
    /* 'LIB_getrefposarc:57' temp = 0.999999; */
    temp = 0.999999;
  }

  /* 'LIB_getrefposarc:59' angarc = acos(temp); */
  angarc = acos(temp);

  /* 'LIB_getrefposarc:60' temp = dot(a0,pos)/(norm(a0)*norm(pos)); */
  temp = (((a0[0] * pos[0]) + (a0[1] * pos[1])) + (a0_0 * sarc)) / (temp_tmp *
    norm_MfVYifVG(pos));

  /* 'LIB_getrefposarc:61' if (temp > 0.999999) */
  if (temp > 0.999999) {
    /* 'LIB_getrefposarc:62' temp = 0.999999; */
    temp = 0.999999;
  }

  /* 'LIB_getrefposarc:64' angpos = acos(temp); */
  /* 'LIB_getrefposarc:65' sarc = sin(angarc); */
  sarc = sin(angarc);

  /* 'LIB_getrefposarc:67' t = angpos/angarc; */
  temp = acos(temp) / angarc;

  /* 'LIB_getrefposarc:68' if (t > 1.0) */
  if (temp > 1.0) {
    /* 'LIB_getrefposarc:69' t = 1.0; */
    temp = 1.0;
  }

  /* 'LIB_getrefposarc:72' Rref = (sin((1.0-t)*angarc)/sarc)*a0 + (sin(t*angarc)/sarc)*a1 + Oarc; */
  temp_tmp = sin((1.0 - temp) * angarc) / sarc;
  b_a = sin(temp * angarc) / sarc;
  Rref[0] = ((temp_tmp * a0[0]) + (b_a * a1[0])) + Oarc[0];
  Rref[1] = ((temp_tmp * a0[1]) + (b_a * a1[1])) + Oarc[1];
  Rref[2] = ((temp_tmp * a0_0) + (b_a * a1_0)) + Oarc[2];

  /* 'LIB_getrefposarc:74' if (t > 0.001) */
  if (temp > 0.001) {
    /* 'LIB_getrefposarc:75' Rref_acc = (sin((1.0-(t-0.001))*angarc)/sarc)*a0 + (sin((t-0.001)*angarc)/sarc)*a1 + Oarc; */
    temp_tmp = sin((1.0 - (temp - 0.001)) * angarc) / sarc;
    angarc = sin((temp - 0.001) * angarc) / sarc;

    /* 'LIB_getrefposarc:76' Rref_del = Rref - Rref_acc; */
    Rref_del[0] = Rref[0] - (((temp_tmp * a0[0]) + (angarc * a1[0])) + Oarc[0]);
    Rref_del[1] = Rref[1] - (((temp_tmp * a0[1]) + (angarc * a1[1])) + Oarc[1]);
    Rref_del[2] = Rref[2] - (((temp_tmp * a0_0) + (angarc * a1_0)) + Oarc[2]);
  } else {
    /* 'LIB_getrefposarc:77' else */
    /* 'LIB_getrefposarc:78' Rref_acc = (sin((1.0-(t+0.001))*angarc)/sarc)*a0 + (sin((t+0.001)*angarc)/sarc)*a1 + Oarc; */
    temp_tmp = sin((1.0 - (temp + 0.001)) * angarc) / sarc;
    angarc = sin((temp + 0.001) * angarc) / sarc;

    /* 'LIB_getrefposarc:79' Rref_del = Rref_acc - Rref; */
    Rref_del[0] = (((temp_tmp * a0[0]) + (angarc * a1[0])) + Oarc[0]) - Rref[0];
    Rref_del[1] = (((temp_tmp * a0[1]) + (angarc * a1[1])) + Oarc[1]) - Rref[1];
    Rref_del[2] = (((temp_tmp * a0_0) + (angarc * a1_0)) + Oarc[2]) - Rref[2];
  }

  /* 'LIB_getrefposarc:81' if (abs(Rref_del(1)) < 0.001) */
  if (fabs(Rref_del[0]) < 0.001) {
    /* 'LIB_getrefposarc:82' Rref_del(1) = 0.001; */
    Rref_del[0] = 0.001;
  }

  /* 'LIB_getrefposarc:84' Uref = Rref_del/norm(Rref_del); */
  a0_0 = norm_MfVYifVG(Rref_del);
  Uref[0] = Rref_del[0] / a0_0;
  Uref[1] = Rref_del[1] / a0_0;
  Uref[2] = Rref_del[2] / a0_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
