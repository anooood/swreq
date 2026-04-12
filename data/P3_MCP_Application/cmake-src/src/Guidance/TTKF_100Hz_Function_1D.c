/*
 * File: TTKF_100Hz_Function_1D.c
 *
 * Code generated for Simulink model 'TTKF_100Hz_Function_1D'.
 *
 * Model version                  : 1.1467
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:33:35 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA C:2012 guidelines
 *    4. Safety precaution
 *    5. Debugging
 * Validation result: Not run
 */

#include "TTKF_100Hz_Function_1D.h"
#include "genrand_uint32_vector_MBxIWKsg.h"
#include "genrandu_Kv96M2Ap.h"
#include "norm_MfVYifVG.h"
#include "norm_lpbhAiWU.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"
#include "rt_remd_snf.h"
#include "rt_roundd_snf.h"
#include "sum_sVbQhjnY.h"

/* Forward declaration for local functions */
static void LIB_lla2ecef(real_T lla_lat, real_T lla_lon, real_T lla_alt, real_T *
  pos_e_x, real_T *pos_e_y, real_T *pos_e_z);
static void LIB_q2d(real_T q_q0, real_T q_q1, real_T q_q2, real_T q_q3, real_T
                    dcm[9]);
static void initiateKF(real_T set_hP0, real_T set_vP0, real_T set_hV0, real_T
  set_vV0, real_T inp_trex, real_T inp_trey, real_T inp_trez, real_T inp_tvex,
  real_T inp_tvey, real_T inp_tvez, sfl7P65dOznAw2EcCMFPbAE *state);
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9]);
static void modelupdate(real_T inp_mrex, real_T inp_mrey, real_T inp_mrez,
  real_T inp_mvex, real_T inp_mvey, real_T inp_mvez, real_T inp_mphi, real_T
  inp_mthe, real_T inp_mpsi, const real_T inp_dcm_eb[9], const real_T
  inp_dcm_es[9], real_T inp_epsm, real_T inp_etam, sfl7P65dOznAw2EcCMFPbAE
  *state);
static void timeupdate(const real_T set_Q[36], sfl7P65dOznAw2EcCMFPbAE *state);
static void rgemconstruct(sjaNUYpKpc2EHQMOiEPkrKE *inp, const real_T state_X[6],
  real_T state_them_g, real_T b_def_MP_MISSIONLOAD);
static real_T randn(DW *dwork);
static void merge(int32_T idx_data[], real_T x_data[], int32_T offset, int32_T
                  np, int32_T nq, int32_T iwork_data[], real_T xwork_data[]);
static void sortIdx(real_T x_data[], int32_T *x_size, int32_T idx_data[],
                    int32_T *idx_size);
static void sort(real_T x_data[], int32_T *x_size);
static real_T combineVectorElements(const real_T x_data[], const int32_T *x_size);
static void noiseupdate(uint32_T newmeas, real_T time, real_T inp_mode, real_T
  inp_freq, real_T inp_epsm, real_T inp_etam, sfl7P65dOznAw2EcCMFPbAE *state,
  const T_Weapon_MCP_DATA_TTKF *fromDATA, real_T b_def_MP_OPPORTUNITY, real_T
  b_def_MP_MISSIONLOAD, DW *dwork);
static void measupdate(real_T inp_mode, real_T inp_freq, real_T inp_epsm, real_T
  inp_etam, real_T inp_rgem, uint8_T inp_ValidGNSSNav, sfl7P65dOznAw2EcCMFPbAE
  *state, real_T b_def_MP_OPPORTUNITY, real_T b_def_MP_MISSIONLOAD, real_T
  delstate[18]);

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function [pos_e] = LIB_lla2ecef(lla)
 */
static void LIB_lla2ecef(real_T lla_lat, real_T lla_lon, real_T lla_alt, real_T *
  pos_e_x, real_T *pos_e_y, real_T *pos_e_z)
{
  real_T sinLat;
  real_T r_n;
  real_T pos_e_x_tmp;

  /* 'LIB_lla2ecef:28' RE           = 6378137.0; */
  /* 'LIB_lla2ecef:29' E2           = 0.006694379990141316; */
  /* 'LIB_lla2ecef:30' ONE_MIN_E2   = 0.9933056200098587; */
  /* 'LIB_lla2ecef:32' sinLat = sin(lla.lat); */
  sinLat = sin(lla_lat);

  /* 'LIB_lla2ecef:33' cosLat = cos(lla.lat); */
  /* 'LIB_lla2ecef:36' r_n = RE / sqrt(1.0 - E2 * sinLat * sinLat); */
  r_n = 6.378137E+6 / sqrt(1.0 - ((0.0066943799901413156 * sinLat) * sinLat));

  /* 'LIB_lla2ecef:39' pos_e.x = (r_n + lla.alt) * cosLat * cos(lla.lon); */
  pos_e_x_tmp = (r_n + lla_alt) * cos(lla_lat);
  *pos_e_x = pos_e_x_tmp * cos(lla_lon);

  /* 'LIB_lla2ecef:40' pos_e.y = (r_n + lla.alt) * cosLat * sin(lla.lon); */
  *pos_e_y = pos_e_x_tmp * sin(lla_lon);

  /* 'LIB_lla2ecef:41' pos_e.z = (ONE_MIN_E2 * r_n + lla.alt) * sinLat; */
  *pos_e_z = ((0.99330562000985867 * r_n) + lla_alt) * sinLat;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function dcm = LIB_q2d(q)
 */
static void LIB_q2d(real_T q_q0, real_T q_q1, real_T q_q2, real_T q_q3, real_T
                    dcm[9])
{
  real_T dcm_tmp;
  real_T dcm_tmp_0;
  real_T dcm_tmp_1;
  real_T dcm_tmp_2;
  real_T dcm_tmp_3;
  real_T dcm_tmp_4;
  real_T dcm_tmp_5;
  real_T dcm_tmp_6;

  /* 'LIB_q2d:26' q0 = q.q0; */
  /* 'LIB_q2d:27' q1 = q.q1; */
  /* 'LIB_q2d:28' q2 = q.q2; */
  /* 'LIB_q2d:29' q3 = q.q3; */
  /* 'LIB_q2d:31' dcm = zeros(3,3); */
  /* 'LIB_q2d:32' dcm(1,1) = q0^2 + q1^2 - q2^2 - q3^2; */
  dcm_tmp_1 = q_q0 * q_q0;
  dcm_tmp_2 = q_q1 * q_q1;
  dcm_tmp_3 = q_q2 * q_q2;
  dcm_tmp_4 = q_q3 * q_q3;
  dcm[0] = ((dcm_tmp_1 + dcm_tmp_2) - dcm_tmp_3) - dcm_tmp_4;

  /* 'LIB_q2d:33' dcm(1,2) = 2.0*((q1*q2) - (q0*q3)); */
  dcm_tmp = q_q1 * q_q2;
  dcm_tmp_0 = q_q0 * q_q3;
  dcm[3] = (dcm_tmp - dcm_tmp_0) * 2.0;

  /* 'LIB_q2d:34' dcm(1,3) = 2.0*((q1*q3) + (q0*q2)); */
  dcm_tmp_5 = q_q1 * q_q3;
  dcm_tmp_6 = q_q0 * q_q2;
  dcm[6] = (dcm_tmp_5 + dcm_tmp_6) * 2.0;

  /* 'LIB_q2d:35' dcm(2,1) = 2.0*((q1*q2) + (q0*q3)); */
  dcm[1] = (dcm_tmp + dcm_tmp_0) * 2.0;

  /* 'LIB_q2d:36' dcm(2,2) = q0^2 - q1^2 + q2^2 - q3^2; */
  dcm_tmp_1 -= dcm_tmp_2;
  dcm[4] = (dcm_tmp_1 + dcm_tmp_3) - dcm_tmp_4;

  /* 'LIB_q2d:37' dcm(2,3) = 2.0*((q2*q3) - (q0*q1)); */
  dcm_tmp_2 = q_q2 * q_q3;
  dcm_tmp = q_q0 * q_q1;
  dcm[7] = (dcm_tmp_2 - dcm_tmp) * 2.0;

  /* 'LIB_q2d:38' dcm(3,1) = 2.0*((q1*q3) - (q0*q2)); */
  dcm[2] = (dcm_tmp_5 - dcm_tmp_6) * 2.0;

  /* 'LIB_q2d:39' dcm(3,2) = 2.0*((q2*q3) + (q0*q1)); */
  dcm[5] = (dcm_tmp_2 + dcm_tmp) * 2.0;

  /* 'LIB_q2d:40' dcm(3,3) = q0^2 - q1^2 - q2^2 + q3^2; */
  dcm[8] = (dcm_tmp_1 - dcm_tmp_3) + dcm_tmp_4;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function state = initiateKF(set,inp,state)
 */
static void initiateKF(real_T set_hP0, real_T set_vP0, real_T set_hV0, real_T
  set_vV0, real_T inp_trex, real_T inp_trey, real_T inp_trez, real_T inp_tvex,
  real_T inp_tvey, real_T inp_tvez, sfl7P65dOznAw2EcCMFPbAE *state)
{
  static const real_T b[9] = { 0.01, 0.0, 0.0, 0.0, 0.01, 0.0, 0.0, 0.0, 1000.0
  };

  real_T tmp;

  /* 'GUID_Embedded_Guid_TTKF_1D:834' state.X(1) = inp.trex; */
  state->X[0] = inp_trex;

  /* 'GUID_Embedded_Guid_TTKF_1D:835' state.X(2) = inp.trey; */
  state->X[1] = inp_trey;

  /* 'GUID_Embedded_Guid_TTKF_1D:836' state.X(3) = inp.trez; */
  state->X[2] = inp_trez;

  /* 'GUID_Embedded_Guid_TTKF_1D:837' state.X(4) = inp.tvex; */
  state->X[3] = inp_tvex;

  /* 'GUID_Embedded_Guid_TTKF_1D:838' state.X(5) = inp.tvey; */
  state->X[4] = inp_tvey;

  /* 'GUID_Embedded_Guid_TTKF_1D:839' state.X(6) = inp.tvez; */
  state->X[5] = inp_tvez;

  /* 'GUID_Embedded_Guid_TTKF_1D:841' state.R = [0.01   0.00   0.00; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:842'              0.00   0.01   0.00; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:843'              0.00   0.00  1000.0]; */
  memcpy(&state->R[0], &b[0], 9U * (sizeof(real_T)));

  /* 'GUID_Embedded_Guid_TTKF_1D:845' state.P = [set.hP0^2 0.0     0.0    0.0     0.0     0.0; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:846'                 0.0  set.hP0^2 0.0    0.0     0.0     0.0; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:847'                 0.0    0.0  set.vP0^2 0.0     0.0     0.0; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:848'                 0.0    0.0     0.0  set.hV0^2 0.0     0.0; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:849'                 0.0    0.0     0.0    0.0   set.hV0^2 0.0; ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:850'                 0.0    0.0     0.0    0.0     0.0   set.vV0^2]; */
  tmp = set_hP0 * set_hP0;
  state->P[0] = tmp;
  state->P[6] = 0.0;
  state->P[12] = 0.0;
  state->P[18] = 0.0;
  state->P[24] = 0.0;
  state->P[30] = 0.0;
  state->P[1] = 0.0;
  state->P[7] = tmp;
  state->P[13] = 0.0;
  state->P[19] = 0.0;
  state->P[25] = 0.0;
  state->P[31] = 0.0;
  state->P[2] = 0.0;
  state->P[8] = 0.0;
  state->P[14] = set_vP0 * set_vP0;
  state->P[20] = 0.0;
  state->P[26] = 0.0;
  state->P[32] = 0.0;
  state->P[3] = 0.0;
  state->P[9] = 0.0;
  state->P[15] = 0.0;
  tmp = set_hV0 * set_hV0;
  state->P[21] = tmp;
  state->P[27] = 0.0;
  state->P[33] = 0.0;
  state->P[4] = 0.0;
  state->P[10] = 0.0;
  state->P[16] = 0.0;
  state->P[22] = 0.0;
  state->P[28] = tmp;
  state->P[34] = 0.0;
  state->P[5] = 0.0;
  state->P[11] = 0.0;
  state->P[17] = 0.0;
  state->P[23] = 0.0;
  state->P[29] = 0.0;
  state->P[35] = set_vV0 * set_vV0;

  /* 'GUID_Embedded_Guid_TTKF_1D:852' state.rge_clamp = 0; */
  state->rge_clamp = 0.0;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function dcm = LIB_e2d(eg)
 */
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9])
{
  real_T dcm_tmp;
  real_T dcm_tmp_0;
  real_T dcm_tmp_1;
  real_T dcm_tmp_2;
  real_T dcm_tmp_3;
  real_T dcm_tmp_4;
  real_T dcm_tmp_5;
  real_T dcm_tmp_6;

  /* 'LIB_e2d:26' dcm = zeros(3,3); */
  /* 'LIB_e2d:28' phi   = eg.phi; */
  /* 'LIB_e2d:29' theta = eg.the; */
  /* 'LIB_e2d:30' psi   = eg.psi; */
  /* 'LIB_e2d:32' dcm(1,1) = cos(theta)*cos(psi); */
  dcm_tmp = cos(eg_psi);
  dcm_tmp_4 = cos(eg_the);
  dcm[0] = dcm_tmp_4 * dcm_tmp;

  /* 'LIB_e2d:33' dcm(1,2) = (sin(phi)*sin(theta)*cos(psi)) - (cos(phi)*sin(psi)); */
  dcm_tmp_0 = cos(eg_phi);
  dcm_tmp_1 = sin(eg_the);
  dcm_tmp_2 = sin(eg_phi);
  dcm_tmp_3 = sin(eg_psi);
  dcm_tmp_5 = dcm_tmp_2 * dcm_tmp_1;
  dcm[3] = (dcm_tmp_5 * dcm_tmp) - (dcm_tmp_0 * dcm_tmp_3);

  /* 'LIB_e2d:34' dcm(1,3) = (cos(phi)*sin(theta)*cos(psi)) + (sin(phi)*sin(psi)); */
  dcm_tmp_6 = dcm_tmp_0 * dcm_tmp_1;
  dcm[6] = (dcm_tmp_6 * dcm_tmp) + (dcm_tmp_2 * dcm_tmp_3);

  /* 'LIB_e2d:35' dcm(2,1) = cos(theta)*sin(psi); */
  dcm[1] = dcm_tmp_4 * dcm_tmp_3;

  /* 'LIB_e2d:36' dcm(2,2) = (sin(phi)*sin(theta)*sin(psi)) + (cos(phi)*cos(psi)); */
  dcm[4] = (dcm_tmp_5 * dcm_tmp_3) + (dcm_tmp_0 * dcm_tmp);

  /* 'LIB_e2d:37' dcm(2,3) = (cos(phi)*sin(theta)*sin(psi)) - (sin(phi)*cos(psi)); */
  dcm[7] = (dcm_tmp_6 * dcm_tmp_3) - (dcm_tmp_2 * dcm_tmp);

  /* 'LIB_e2d:38' dcm(3,1) = -sin(theta); */
  dcm[2] = -dcm_tmp_1;

  /* 'LIB_e2d:39' dcm(3,2) =  sin(phi)*cos(theta); */
  dcm[5] = dcm_tmp_2 * dcm_tmp_4;

  /* 'LIB_e2d:40' dcm(3,3) =  cos(phi)*cos(theta); */
  dcm[8] = dcm_tmp_0 * dcm_tmp_4;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function state = modelupdate(inp,state)
 */
static void modelupdate(real_T inp_mrex, real_T inp_mrey, real_T inp_mrez,
  real_T inp_mvex, real_T inp_mvey, real_T inp_mvez, real_T inp_mphi, real_T
  inp_mthe, real_T inp_mpsi, const real_T inp_dcm_eb[9], const real_T
  inp_dcm_es[9], real_T inp_epsm, real_T inp_etam, sfl7P65dOznAw2EcCMFPbAE
  *state)
{
  real_T Vt;
  real_T rb[3];
  real_T dcm_bn[9];
  real_T rg[3];
  real_T rsx;
  real_T temp1;
  int32_T i;
  real_T tmp;

  /* 'GUID_Embedded_Guid_TTKF_1D:858' state.rex = state.X(1) - inp.mrex; */
  state->rex = state->X[0] - inp_mrex;

  /* 'GUID_Embedded_Guid_TTKF_1D:859' state.rey = state.X(2) - inp.mrey; */
  state->rey = state->X[1] - inp_mrey;

  /* 'GUID_Embedded_Guid_TTKF_1D:860' state.rez = state.X(3) - inp.mrez; */
  state->rez = state->X[2] - inp_mrez;

  /* 'GUID_Embedded_Guid_TTKF_1D:861' Vt = norm([state.X(4) state.X(5) state.X(6)]); */
  rb[0] = state->X[3];
  rb[1] = state->X[4];
  rb[2] = state->X[5];
  Vt = norm_MfVYifVG(rb);

  /* 'GUID_Embedded_Guid_TTKF_1D:862' Vm = norm([inp.mvex inp.mvey inp.mvez]); */
  /* 'GUID_Embedded_Guid_TTKF_1D:866' if (Vt > 30.0+0.0875*Vm) */
  rb[0] = inp_mvex;
  rb[1] = inp_mvey;
  rb[2] = inp_mvez;
  if (Vt > ((0.0875 * norm_MfVYifVG(rb)) + 30.0)) {
    /* 'GUID_Embedded_Guid_TTKF_1D:867' state.X(4) = (20.0/Vt)*state.X(4); */
    state->X[3] *= 20.0 / Vt;

    /* 'GUID_Embedded_Guid_TTKF_1D:868' state.X(5) = (20.0/Vt)*state.X(5); */
    state->X[4] *= 20.0 / Vt;

    /* 'GUID_Embedded_Guid_TTKF_1D:869' state.X(6) = (20.0/Vt)*state.X(6); */
    state->X[5] *= 20.0 / Vt;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:871' state.vex = state.X(4) - inp.mvex; */
  state->vex = state->X[3] - inp_mvex;

  /* 'GUID_Embedded_Guid_TTKF_1D:872' state.vey = state.X(5) - inp.mvey; */
  state->vey = state->X[4] - inp_mvey;

  /* 'GUID_Embedded_Guid_TTKF_1D:873' state.vez = state.X(6) - inp.mvez; */
  state->vez = state->X[5] - inp_mvez;

  /* 'GUID_Embedded_Guid_TTKF_1D:874' rr = state.rex^2 + state.rey^2 + state.rez^2; */
  Vt = ((state->rex * state->rex) + (state->rey * state->rey)) + (state->rez *
    state->rez);

  /* 'GUID_Embedded_Guid_TTKF_1D:876' if (rr < 1.0) */
  if (Vt < 1.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:877' rr = 1.0; */
    Vt = 1.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:879' state.rge = sqrt(rr); */
  rsx = sqrt(Vt);
  state->rge = rsx;

  /* 'GUID_Embedded_Guid_TTKF_1D:880' if (state.rge < 100.0) */
  if (rsx < 100.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:881' state.rex = (100.0/state.rge)*state.rex; */
    state->rex *= 100.0 / rsx;

    /* 'GUID_Embedded_Guid_TTKF_1D:882' state.rey = (100.0/state.rge)*state.rey; */
    state->rey *= 100.0 / rsx;

    /* 'GUID_Embedded_Guid_TTKF_1D:883' state.rez = (100.0/state.rge)*state.rez; */
    state->rez *= 100.0 / rsx;

    /* 'GUID_Embedded_Guid_TTKF_1D:884' state.rge = 100.0; */
    state->rge = 100.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:885' rr = 10000.0; */
    Vt = 10000.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:886' state.X(1) = state.rex + inp.mrex; */
    state->X[0] = state->rex + inp_mrex;

    /* 'GUID_Embedded_Guid_TTKF_1D:887' state.X(2) = state.rey + inp.mrey; */
    state->X[1] = state->rey + inp_mrey;

    /* 'GUID_Embedded_Guid_TTKF_1D:888' state.X(3) = state.rez + inp.mrez; */
    state->X[2] = state->rez + inp_mrez;

    /* 'GUID_Embedded_Guid_TTKF_1D:889' state.rge_clamp = 1; */
    state->rge_clamp = 1.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:893' rb = inp.dcm_eb*[state.rex state.rey state.rez]'; */
  for (i = 0; i < 3; i++) {
    rb[i] = (inp_dcm_eb[i + 6] * state->rez) + ((inp_dcm_eb[i + 3] * state->rey)
      + (inp_dcm_eb[i] * state->rex));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:894' state.the_b = -asin(LIB_limit(-state.rge+0.001,state.rge-0.001,rb(3))/state.rge); */
  /* 'LIB_limit:27' if (in > max) */
  if (rb[2] > (state->rge - 0.001)) {
    /* 'LIB_limit:28' out = max; */
    rsx = state->rge - 0.001;
  } else if (rb[2] < ((-state->rge) + 0.001)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    rsx = (-state->rge) + 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    rsx = rb[2];
  }

  state->the_b = -asin(rsx / state->rge);

  /* 'GUID_Embedded_Guid_TTKF_1D:895' state.psi_b = atan2(rb(2),rb(1)); */
  state->psi_b = rt_atan2d_snf(rb[1], rb[0]);

  /* 'GUID_Embedded_Guid_TTKF_1D:898' eg.phi  = inp.mphi; */
  /* 'GUID_Embedded_Guid_TTKF_1D:899' eg.the  = inp.mthe; */
  /* 'GUID_Embedded_Guid_TTKF_1D:900' eg.psi  = inp.mpsi; */
  /* 'GUID_Embedded_Guid_TTKF_1D:901' dcm_bn = LIB_e2d(eg); */
  LIB_e2d(inp_mphi, inp_mthe, inp_mpsi, dcm_bn);

  /* 'GUID_Embedded_Guid_TTKF_1D:902' rg = dcm_bn*rb; */
  for (i = 0; i < 3; i++) {
    rg[i] = (dcm_bn[i + 6] * rb[2]) + ((dcm_bn[i + 3] * rb[1]) + (dcm_bn[i] *
      rb[0]));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:903' state.the_g = -asin(LIB_limit(-state.rge+0.001,state.rge-0.001,rg(3))/state.rge); */
  /* 'LIB_limit:27' if (in > max) */
  if (rg[2] > (state->rge - 0.001)) {
    /* 'LIB_limit:28' out = max; */
    rsx = state->rge - 0.001;
  } else if (rg[2] < ((-state->rge) + 0.001)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    rsx = (-state->rge) + 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    rsx = rg[2];
  }

  state->the_g = -asin(rsx / state->rge);

  /* 'GUID_Embedded_Guid_TTKF_1D:904' state.psi_g = atan2(rg(2),rg(1)); */
  state->psi_g = rt_atan2d_snf(rg[1], rg[0]);

  /* 'GUID_Embedded_Guid_TTKF_1D:907' ubm.x = cos(inp.epsm)*cos(inp.etam); */
  /* 'GUID_Embedded_Guid_TTKF_1D:908' ubm.y = cos(inp.epsm)*sin(inp.etam); */
  /* 'GUID_Embedded_Guid_TTKF_1D:909' ubm.z = sin(-inp.epsm); */
  /* 'GUID_Embedded_Guid_TTKF_1D:910' ugm = dcm_bn*[ubm.x ubm.y ubm.z]'; */
  rsx = sin(-inp_epsm);
  tmp = cos(inp_epsm);
  temp1 = tmp * cos(inp_etam);
  tmp *= sin(inp_etam);
  for (i = 0; i < 3; i++) {
    rb[i] = (dcm_bn[i + 6] * rsx) + ((dcm_bn[i + 3] * tmp) + (dcm_bn[i] * temp1));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:911' state.them_g = -asin(LIB_limit(-0.999999,0.999999,ugm(3))/1.0); */
  /* 'LIB_limit:27' if (in > max) */
  if (rb[2] > 0.999999) {
    /* 'LIB_limit:28' out = max; */
    rsx = 0.999999;
  } else if (rb[2] < -0.999999) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    rsx = -0.999999;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    rsx = rb[2];
  }

  state->them_g = -asin(rsx);

  /* 'GUID_Embedded_Guid_TTKF_1D:912' state.psim_g = atan2(ugm(2),ugm(1)); */
  state->psim_g = rt_atan2d_snf(rb[1], rb[0]);

  /* 'GUID_Embedded_Guid_TTKF_1D:915' temp = inp.dcm_es*[state.rex state.rey state.rez]'; */
  for (i = 0; i < 3; i++) {
    rb[i] = (inp_dcm_es[i + 6] * state->rez) + ((inp_dcm_es[i + 3] * state->rey)
      + (inp_dcm_es[i] * state->rex));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:916' rsx = temp(1); */
  rsx = rb[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:917' if (abs(rsx) < 0.001) */
  if (fabs(rb[0]) < 0.001) {
    /* 'GUID_Embedded_Guid_TTKF_1D:918' rsx = 0.001; */
    rsx = 0.001;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:920' rsy = temp(2); */
  /* 'GUID_Embedded_Guid_TTKF_1D:921' rsz = temp(3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:923' temp1 = LIB_limit(-0.999999,0.999999,rsz/state.rge); */
  temp1 = rb[2] / state->rge;

  /* 'LIB_limit:27' if (in > max) */
  if (temp1 > 0.999999) {
    /* 'LIB_limit:28' out = max; */
    temp1 = 0.999999;
  } else if (temp1 < -0.999999) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    temp1 = -0.999999;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:924' temp2 = (sqrt(1-temp1*temp1))*state.rge^3; */
  temp1 = sqrt(1.0 - (temp1 * temp1)) * rt_powd_snf(state->rge, 3.0);

  /* 'GUID_Embedded_Guid_TTKF_1D:926' state.H(1,1) = (rsz*state.rex - rr*inp.dcm_es(3,1))/temp2; */
  state->H[0] = ((rb[2] * state->rex) - (Vt * inp_dcm_es[2])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:927' state.H(1,2) = (rsz*state.rey - rr*inp.dcm_es(3,2))/temp2; */
  state->H[3] = ((rb[2] * state->rey) - (Vt * inp_dcm_es[5])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:928' state.H(1,3) = (rsz*state.rez - rr*inp.dcm_es(3,3))/temp2; */
  state->H[6] = ((rb[2] * state->rez) - (Vt * inp_dcm_es[8])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:930' temp1 = rsy/rsx; */
  temp1 = rb[1] / rsx;

  /* 'GUID_Embedded_Guid_TTKF_1D:931' temp2 = rsx*rsx*(1+temp1*temp1); */
  temp1 = ((temp1 * temp1) + 1.0) * (rsx * rsx);

  /* 'GUID_Embedded_Guid_TTKF_1D:933' state.H(2,1) = (rsx*inp.dcm_es(2,1) - rsy*inp.dcm_es(1,1))/temp2; */
  state->H[1] = ((rsx * inp_dcm_es[1]) - (rb[1] * inp_dcm_es[0])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:934' state.H(2,2) = (rsx*inp.dcm_es(2,2) - rsy*inp.dcm_es(1,2))/temp2; */
  state->H[4] = ((rsx * inp_dcm_es[4]) - (rb[1] * inp_dcm_es[3])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:935' state.H(2,3) = (rsx*inp.dcm_es(2,3) - rsy*inp.dcm_es(1,3))/temp2; */
  state->H[7] = ((rsx * inp_dcm_es[7]) - (rb[1] * inp_dcm_es[6])) / temp1;

  /* 'GUID_Embedded_Guid_TTKF_1D:937' state.H(3,1) = state.rex/state.rge; */
  state->H[2] = state->rex / state->rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:938' state.H(3,2) = state.rey/state.rge; */
  state->H[5] = state->rey / state->rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:939' state.H(3,3) = state.rez/state.rge; */
  state->H[8] = state->rez / state->rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:944' state.eps = -asin(LIB_limit(-state.rge+0.001,state.rge-0.001,rsz)/state.rge); */
  /* 'LIB_limit:27' if (in > max) */
  if (rb[2] > (state->rge - 0.001)) {
    /* 'LIB_limit:28' out = max; */
    temp1 = state->rge - 0.001;
  } else if (rb[2] < ((-state->rge) + 0.001)) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    temp1 = (-state->rge) + 0.001;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    temp1 = rb[2];
  }

  state->eps = -asin(temp1 / state->rge);

  /* 'GUID_Embedded_Guid_TTKF_1D:945' state.eta = atan2(rsy,rsx); */
  state->eta = rt_atan2d_snf(rb[1], rsx);

  /* 'GUID_Embedded_Guid_TTKF_1D:947' losw_e = cross([state.rex state.rey state.rez], ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:948'                  [state.vex state.vey state.vez]) ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:949'                  /rr; */
  /* 'GUID_Embedded_Guid_TTKF_1D:951' losw_b = inp.dcm_eb*losw_e'; */
  rsx = ((state->rey * state->vez) - (state->rez * state->vey)) / Vt;
  temp1 = ((state->rez * state->vex) - (state->rex * state->vez)) / Vt;
  Vt = ((state->rex * state->vey) - (state->rey * state->vex)) / Vt;
  for (i = 0; i < 3; i++) {
    rb[i] = (inp_dcm_eb[i + 6] * Vt) + ((inp_dcm_eb[i + 3] * temp1) +
      (inp_dcm_eb[i] * rsx));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:952' state.dthe_b = losw_b(2); */
  state->dthe_b = rb[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:953' state.dpsi_b = losw_b(3); */
  state->dpsi_b = rb[2];
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function state = timeupdate(set,inp,state)
 */
static void timeupdate(const real_T set_Q[36], sfl7P65dOznAw2EcCMFPbAE *state)
{
  real_T den;
  real_T state_0[6];
  real_T state_1[36];
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T state_tmp;

  /* 'GUID_Embedded_Guid_TTKF_1D:1198' state.X = state.A*state.X; */
  for (i = 0; i < 6; i++) {
    state_0[i] = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      state_0[i] += state->A[(6 * i_0) + i] * state->X[i_0];
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1204' state.P = state.A*state.P*state.A' + set.Q; */
  for (i = 0; i < 6; i++) {
    state->X[i] = state_0[i];
    for (i_0 = 0; i_0 < 6; i_0++) {
      state_tmp = i + (6 * i_0);
      state_1[state_tmp] = 0.0;
      for (i_1 = 0; i_1 < 6; i_1++) {
        state_1[state_tmp] += state->A[(6 * i_1) + i] * state->P[(6 * i_0) + i_1];
      }
    }
  }

  for (i = 0; i < 6; i++) {
    for (i_0 = 0; i_0 < 6; i_0++) {
      den = 0.0;
      for (i_1 = 0; i_1 < 6; i_1++) {
        den += state_1[(6 * i_1) + i] * state->A[(6 * i_1) + i_0];
      }

      i_1 = (6 * i_0) + i;
      state->P[i_1] = set_Q[i_1] + den;
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1213' den = state.P(1,1)^2+state.P(2,2)^2+state.P(3,3)^2; */
  den = ((state->P[0] * state->P[0]) + (state->P[7] * state->P[7])) + (state->P
    [14] * state->P[14]);

  /* 'GUID_Embedded_Guid_TTKF_1D:1214' if (den >= 70000.0) */
  if (den >= 70000.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1215' state.conf = 4.00/(den^0.33); */
    state->conf = 4.0 / rt_powd_snf(den, 0.33);
  } else if (den >= 7.0591176099999977) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1216' elseif (den >= 1.63^4) */
    /* 'GUID_Embedded_Guid_TTKF_1D:1217' state.conf = 1.63/(den^0.25); */
    state->conf = 1.63 / rt_powd_snf(den, 0.25);
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:1218' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:1219' state.conf = 1.0; */
    state->conf = 1.0;
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function inp = rgemconstruct(inp,state,def)
 */
static void rgemconstruct(sjaNUYpKpc2EHQMOiEPkrKE *inp, const real_T state_X[6],
  real_T state_them_g, real_T b_def_MP_MISSIONLOAD)
{
  real_T ratio;
  real_T state_X_0[3];
  real_T tmp;
  real_T tmp_0;
  real_T tmp_1;

  /* 'GUID_Embedded_Guid_TTKF_1D:960' posDdiff0 = inp.malt - inp.talt0; */
  /* 'GUID_Embedded_Guid_TTKF_1D:961' if (state.them_g < -0.001) */
  if (state_them_g < -0.001) {
    /* 'GUID_Embedded_Guid_TTKF_1D:962' inp.rgem = posDdiff0/sin(-state.them_g); */
    inp->rgem = (inp->malt - inp->talt0) / sin(-state_them_g);
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:963' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:964' inp.rgem = posDdiff0/sin(0.001); */
    inp->rgem = (inp->malt - inp->talt0) / 0.00099999983333334168;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:966' if (inp.mode == def.MP_MISSIONLOAD) */
  if (inp->mode == b_def_MP_MISSIONLOAD) {
    /* 'GUID_Embedded_Guid_TTKF_1D:967' Vt = norm([state.X(4) state.X(5) state.X(6)]); */
    /* 'GUID_Embedded_Guid_TTKF_1D:968' ratio = LIB_limit(0.0,1.0,(30.0-Vt)/20.0); */
    state_X_0[0] = state_X[3];
    state_X_0[1] = state_X[4];
    state_X_0[2] = state_X[5];
    ratio = (30.0 - norm_MfVYifVG(state_X_0)) / 20.0;

    /* 'LIB_limit:27' if (in > max) */
    if (ratio > 1.0) {
      /* 'LIB_limit:28' out = max; */
      ratio = 1.0;
    } else if (ratio < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      ratio = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:969' inp.rgem = ratio*sqrt((inp.trex-inp.mrex)*(inp.trex-inp.mrex) + ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:970'                           (inp.trey-inp.mrey)*(inp.trey-inp.mrey) + ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:971'                           (inp.trez-inp.mrez)*(inp.trez-inp.mrez)) + ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:972' 			   (1.0-ratio)*inp.rgem; */
    tmp = inp->trex - inp->mrex;
    tmp_0 = inp->trey - inp->mrey;
    tmp_1 = inp->trez - inp->mrez;
    inp->rgem = (sqrt(((tmp * tmp) + (tmp_0 * tmp_0)) + (tmp_1 * tmp_1)) * ratio)
      + ((1.0 - ratio) * inp->rgem);
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:975' if (inp.rgem > 150000.0) */
  if (inp->rgem > 150000.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:976' inp.rgem = 150000.0; */
    inp->rgem = 150000.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:978' if (inp.rgem < 100.0) */
  if (inp->rgem < 100.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:979' inp.rgem = 100.0; */
    inp->rgem = 100.0;
  }
}

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
static real_T randn(DW *dwork)
{
  real_T r;
  int32_T i;
  real_T x;
  uint32_T u32[2];
  real_T c_u;
  static const real_T b[257] = { 0.0, 0.215241895984875, 0.286174591792068,
    0.335737519214422, 0.375121332878378, 0.408389134611989, 0.43751840220787,
    0.46363433679088, 0.487443966139235, 0.50942332960209, 0.529909720661557,
    0.549151702327164, 0.567338257053817, 0.584616766106378, 0.601104617755991,
    0.61689699000775, 0.63207223638606, 0.646695714894993, 0.660822574244419,
    0.674499822837293, 0.687767892795788, 0.700661841106814, 0.713212285190975,
    0.725446140909999, 0.737387211434295, 0.749056662017815, 0.760473406430107,
    0.771654424224568, 0.782615023307232, 0.793369058840623, 0.80392911698997,
    0.814306670135215, 0.824512208752291, 0.834555354086381, 0.844444954909153,
    0.854189171008163, 0.863795545553308, 0.87327106808886, 0.882622229585165,
    0.891855070732941, 0.900975224461221, 0.909987953496718, 0.91889818364959,
    0.927710533401999, 0.936429340286575, 0.945058684468165, 0.953602409881086,
    0.96206414322304, 0.970447311064224, 0.978755155294224, 0.986990747099062,
    0.99515699963509, 1.00325667954467, 1.01129241744, 1.01926671746548,
    1.02718196603564, 1.03504043983344, 1.04284431314415, 1.05059566459093,
    1.05829648333067, 1.06594867476212, 1.07355406579244, 1.0811144097034,
    1.08863139065398, 1.09610662785202, 1.10354167942464, 1.11093804601357,
    1.11829717411934, 1.12562045921553, 1.13290924865253, 1.14016484436815,
    1.14738850542085, 1.15458145035993, 1.16174485944561, 1.16887987673083,
    1.17598761201545, 1.18306914268269, 1.19012551542669, 1.19715774787944,
    1.20416683014438, 1.2111537262437, 1.21811937548548, 1.22506469375653,
    1.23199057474614, 1.23889789110569, 1.24578749554863, 1.2526602218949,
    1.25951688606371, 1.26635828701823, 1.27318520766536, 1.27999841571382,
    1.28679866449324, 1.29358669373695, 1.30036323033084, 1.30712898903073,
    1.31388467315022, 1.32063097522106, 1.32736857762793, 1.33409815321936,
    1.3408203658964, 1.34753587118059, 1.35424531676263, 1.36094934303328,
    1.36764858359748, 1.37434366577317, 1.38103521107586, 1.38772383568998,
    1.39441015092814, 1.40109476367925, 1.4077782768464, 1.41446128977547,
    1.42114439867531, 1.42782819703026, 1.43451327600589, 1.44120022484872,
    1.44788963128058, 1.45458208188841, 1.46127816251028, 1.46797845861808,
    1.47468355569786, 1.48139403962819, 1.48811049705745, 1.49483351578049,
    1.50156368511546, 1.50830159628131, 1.51504784277671, 1.521803020761,
    1.52856772943771, 1.53534257144151, 1.542128153229, 1.54892508547417,
    1.55573398346918, 1.56255546753104, 1.56939016341512, 1.57623870273591,
    1.58310172339603, 1.58997987002419, 1.59687379442279, 1.60378415602609,
    1.61071162236983, 1.61765686957301, 1.62462058283303, 1.63160345693487,
    1.63860619677555, 1.64562951790478, 1.65267414708306, 1.65974082285818,
    1.66683029616166, 1.67394333092612, 1.68108070472517, 1.68824320943719,
    1.69543165193456, 1.70264685479992, 1.7098896570713, 1.71716091501782,
    1.72446150294804, 1.73179231405296, 1.73915426128591, 1.74654827828172,
    1.75397532031767, 1.76143636531891, 1.76893241491127, 1.77646449552452,
    1.78403365954944, 1.79164098655216, 1.79928758454972, 1.80697459135082,
    1.81470317596628, 1.82247454009388, 1.83028991968276, 1.83815058658281,
    1.84605785028518, 1.8540130597602, 1.86201760539967, 1.87007292107127,
    1.878180486293, 1.88634182853678, 1.8945585256707, 1.90283220855043,
    1.91116456377125, 1.91955733659319, 1.92801233405266, 1.93653142827569,
    1.94511656000868, 1.95376974238465, 1.96249306494436, 1.97128869793366,
    1.98015889690048, 1.98910600761744, 1.99813247135842, 2.00724083056053,
    2.0164337349062, 2.02571394786385, 2.03508435372962, 2.04454796521753,
    2.05410793165065, 2.06376754781173, 2.07353026351874, 2.0833996939983,
    2.09337963113879, 2.10347405571488, 2.11368715068665, 2.12402331568952,
    2.13448718284602, 2.14508363404789, 2.15581781987674, 2.16669518035431,
    2.17772146774029, 2.18890277162636, 2.20024554661128, 2.21175664288416,
    2.22344334009251, 2.23531338492992, 2.24737503294739, 2.25963709517379,
    2.27210899022838, 2.28480080272449, 2.29772334890286, 2.31088825060137,
    2.32430801887113, 2.33799614879653, 2.35196722737914, 2.36623705671729,
    2.38082279517208, 2.39574311978193, 2.41101841390112, 2.42667098493715,
    2.44272531820036, 2.4592083743347, 2.47614993967052, 2.49358304127105,
    2.51154444162669, 2.53007523215985, 2.54922155032478, 2.56903545268184,
    2.58957598670829, 2.61091051848882, 2.63311639363158, 2.65628303757674,
    2.68051464328574, 2.70593365612306, 2.73268535904401, 2.76094400527999,
    2.79092117400193, 2.82287739682644, 2.85713873087322, 2.89412105361341,
    2.93436686720889, 2.97860327988184, 3.02783779176959, 3.08352613200214,
    3.147889289518, 3.2245750520478, 3.32024473383983, 3.44927829856143,
    3.65415288536101, 3.91075795952492 };

  static const real_T c[257] = { 1.0, 0.977101701267673, 0.959879091800108,
    0.9451989534423, 0.932060075959231, 0.919991505039348, 0.908726440052131,
    0.898095921898344, 0.887984660755834, 0.878309655808918, 0.869008688036857,
    0.860033621196332, 0.851346258458678, 0.842915653112205, 0.834716292986884,
    0.826726833946222, 0.818929191603703, 0.811307874312656, 0.803849483170964,
    0.796542330422959, 0.789376143566025, 0.782341832654803, 0.775431304981187,
    0.768637315798486, 0.761953346836795, 0.755373506507096, 0.748892447219157,
    0.742505296340151, 0.736207598126863, 0.729995264561476, 0.72386453346863,
    0.717811932630722, 0.711834248878248, 0.705928501332754, 0.700091918136512,
    0.694321916126117, 0.688616083004672, 0.682972161644995, 0.677388036218774,
    0.671861719897082, 0.66639134390875, 0.660975147776663, 0.655611470579697,
    0.650298743110817, 0.645035480820822, 0.639820277453057, 0.634651799287624,
    0.629528779924837, 0.624450015547027, 0.619414360605834, 0.614420723888914,
    0.609468064925773, 0.604555390697468, 0.599681752619125, 0.594846243767987,
    0.590047996332826, 0.585286179263371, 0.580559996100791, 0.575868682972354,
    0.571211506735253, 0.566587763256165, 0.561996775814525, 0.557437893618766,
    0.552910490425833, 0.548413963255266, 0.543947731190026, 0.539511234256952,
    0.535103932380458, 0.530725304403662, 0.526374847171684, 0.522052074672322,
    0.517756517229756, 0.513487720747327, 0.509245245995748, 0.505028667943468,
    0.500837575126149, 0.49667156905249, 0.492530263643869, 0.488413284705458,
    0.484320269426683, 0.480250865909047, 0.476204732719506, 0.47218153846773,
    0.468180961405694, 0.464202689048174, 0.460246417812843, 0.456311852678716,
    0.452398706861849, 0.448506701507203, 0.444635565395739, 0.440785034665804,
    0.436954852547985, 0.433144769112652, 0.429354541029442, 0.425583931338022,
    0.421832709229496, 0.418100649837848, 0.414387534040891, 0.410693148270188,
    0.407017284329473, 0.403359739221114, 0.399720314980197, 0.396098818515832,
    0.392495061459315, 0.388908860018789, 0.385340034840077, 0.381788410873393,
    0.378253817245619, 0.374736087137891, 0.371235057668239, 0.367750569779032,
    0.364282468129004, 0.360830600989648, 0.357394820145781, 0.353974980800077,
    0.350570941481406, 0.347182563956794, 0.343809713146851, 0.340452257044522,
    0.337110066637006, 0.333783015830718, 0.330470981379163, 0.327173842813601,
    0.323891482376391, 0.320623784956905, 0.317370638029914, 0.314131931596337,
    0.310907558126286, 0.307697412504292, 0.30450139197665, 0.301319396100803,
    0.298151326696685, 0.294997087799962, 0.291856585617095, 0.288729728482183,
    0.285616426815502, 0.282516593083708, 0.279430141761638, 0.276356989295668,
    0.273297054068577, 0.270250256365875, 0.267216518343561, 0.264195763997261,
    0.261187919132721, 0.258192911337619, 0.255210669954662, 0.252241126055942,
    0.249284212418529, 0.246339863501264, 0.24340801542275, 0.240488605940501,
    0.237581574431238, 0.23468686187233, 0.231804410824339, 0.228934165414681,
    0.226076071322381, 0.223230075763918, 0.220396127480152, 0.217574176724331,
    0.214764175251174, 0.211966076307031, 0.209179834621125, 0.206405406397881,
    0.203642749310335, 0.200891822494657, 0.198152586545776, 0.195425003514135,
    0.192709036903589, 0.190004651670465, 0.187311814223801, 0.1846304924268,
    0.181960655599523, 0.179302274522848, 0.176655321443735, 0.174019770081839,
    0.171395595637506, 0.168782774801212, 0.166181285764482, 0.163591108232366,
    0.161012223437511, 0.158444614155925, 0.15588826472448, 0.153343161060263,
    0.150809290681846, 0.148286642732575, 0.145775208005994, 0.143274978973514,
    0.140785949814445, 0.138308116448551, 0.135841476571254, 0.133386029691669,
    0.130941777173644, 0.12850872228, 0.126086870220186, 0.123676228201597,
    0.12127680548479, 0.11888861344291, 0.116511665625611, 0.114145977827839,
    0.111791568163838, 0.109448457146812, 0.107116667774684, 0.104796225622487,
    0.102487158941935, 0.10018949876881, 0.0979032790388625, 0.095628536713009,
    0.093365311912691, 0.0911136480663738, 0.0888735920682759,
    0.0866451944505581, 0.0844285095703535, 0.082223595813203,
    0.0800305158146631, 0.0778493367020961, 0.0756801303589272,
    0.0735229737139814, 0.0713779490588905, 0.0692451443970068,
    0.0671246538277886, 0.065016577971243, 0.0629210244377582, 0.06083810834954,
    0.0587679529209339, 0.0567106901062031, 0.0546664613248891,
    0.0526354182767924, 0.0506177238609479, 0.0486135532158687,
    0.0466230949019305, 0.0446465522512946, 0.0426841449164746,
    0.0407361106559411, 0.0388027074045262, 0.0368842156885674,
    0.0349809414617162, 0.0330932194585786, 0.0312214171919203,
    0.0293659397581334, 0.0275272356696031, 0.0257058040085489,
    0.0239022033057959, 0.0221170627073089, 0.0203510962300445,
    0.0186051212757247, 0.0168800831525432, 0.0151770883079353,
    0.0134974506017399, 0.0118427578579079, 0.0102149714397015,
    0.00861658276939875, 0.00705087547137324, 0.00552240329925101,
    0.00403797259336304, 0.00260907274610216, 0.0012602859304986,
    0.000477467764609386 };

  int32_T exitg1;
  do {
    exitg1 = 0;
    genrand_uint32_vector_MBxIWKsg(dwork->state, u32);
    i = (int32_T)((uint32_T)((u32[1] >> 24U) + 1U));
    r = ((((((real_T)((uint32_T)((uint32_T)(u32[0] >> 3U)))) * 1.6777216E+7) +
           ((real_T)((int32_T)((int32_T)(((int32_T)u32[1]) & 16777215))))) *
          2.2204460492503131E-16) - 1.0) * b[i];
    if (fabs(r) <= b[i - 1]) {
      exitg1 = 1;
    } else if (i < 256) {
      x = genrandu_Kv96M2Ap(dwork->state);
      if ((((c[i - 1] - c[i]) * x) + c[i]) < exp((-0.5 * r) * r)) {
        exitg1 = 1;
      }
    } else {
      do {
        x = genrandu_Kv96M2Ap(dwork->state);
        x = log(x) * 0.273661237329758;
        c_u = genrandu_Kv96M2Ap(dwork->state);
      } while (!((-2.0 * log(c_u)) > (x * x)));

      if (r < 0.0) {
        r = x - 3.65415288536101;
      } else {
        r = 3.65415288536101 - x;
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return r;
}

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
static void merge(int32_T idx_data[], real_T x_data[], int32_T offset, int32_T
                  np, int32_T nq, int32_T iwork_data[], real_T xwork_data[])
{
  int32_T n;
  int32_T q;
  int32_T iout;
  int32_T n_tmp;
  int32_T exitg1;
  if (nq != 0) {
    n_tmp = np + nq;
    for (q = 0; q < n_tmp; q++) {
      iout = offset + q;
      iwork_data[q] = idx_data[iout];
      xwork_data[q] = x_data[iout];
    }

    n = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[n] <= xwork_data[q]) {
        idx_data[iout] = iwork_data[n];
        x_data[iout] = xwork_data[n];
        if ((n + 1) < np) {
          n++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[q];
        x_data[iout] = xwork_data[q];
        if ((q + 1) < n_tmp) {
          q++;
        } else {
          q = iout - n;
          while ((n + 1) <= np) {
            iout = (q + n) + 1;
            idx_data[iout] = iwork_data[n];
            x_data[iout] = xwork_data[n];
            n++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
static void sortIdx(real_T x_data[], int32_T *x_size, int32_T idx_data[],
                    int32_T *idx_size)
{
  real_T c_x_data[20];
  real_T xwork_data[20];
  int32_T iwork_data[20];
  real_T x4[4];
  int8_T idx4[4];
  int32_T ib;
  int32_T wOffset;
  int32_T itmp;
  int8_T perm[4];
  int32_T i3;
  int32_T i4;
  int32_T bLen;
  int32_T nTail;
  int32_T c_x_size;
  int8_T b_x_idx_0;
  int8_T b_idx_0;
  int32_T itmp_tmp;
  real_T tmp;
  real_T tmp_0;
  b_x_idx_0 = (int8_T)(*x_size);
  b_idx_0 = (int8_T)(*x_size);
  *idx_size = (int32_T)b_x_idx_0;
  if (0 <= (b_x_idx_0 - 1)) {
    memset(&idx_data[0], 0, ((uint32_T)b_x_idx_0) * (sizeof(int32_T)));
  }

  if ((*x_size) != 0) {
    c_x_size = *x_size;
    if (0 <= ((*x_size) - 1)) {
      memcpy(&c_x_data[0], &x_data[0], ((uint32_T)(*x_size)) * (sizeof(real_T)));
    }

    *idx_size = (int32_T)b_idx_0;
    if (0 <= (b_idx_0 - 1)) {
      memset(&idx_data[0], 0, ((uint32_T)b_idx_0) * (sizeof(int32_T)));
    }

    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    b_idx_0 = (int8_T)(*x_size);
    if (0 <= (b_idx_0 - 1)) {
      memset(&xwork_data[0], 0, ((uint32_T)b_idx_0) * (sizeof(real_T)));
    }

    bLen = 0;
    ib = -1;
    for (wOffset = 0; wOffset < (*x_size); wOffset++) {
      if (rtIsNaN(c_x_data[wOffset])) {
        i4 = ((*x_size) - bLen) - 1;
        idx_data[i4] = wOffset + 1;
        xwork_data[i4] = c_x_data[wOffset];
        bLen++;
      } else {
        ib++;
        idx4[ib] = (int8_T)(wOffset + 1);
        x4[ib] = c_x_data[wOffset];
        if ((ib + 1) == 4) {
          ib = wOffset - bLen;
          if (x4[0] <= x4[1]) {
            nTail = 1;
            itmp = 2;
          } else {
            nTail = 2;
            itmp = 1;
          }

          if (x4[2] <= x4[3]) {
            i3 = 3;
            i4 = 4;
          } else {
            i3 = 4;
            i4 = 3;
          }

          tmp = x4[nTail - 1];
          tmp_0 = x4[i3 - 1];
          if (tmp <= tmp_0) {
            tmp = x4[itmp - 1];
            if (tmp <= tmp_0) {
              perm[0] = (int8_T)nTail;
              perm[1] = (int8_T)itmp;
              perm[2] = (int8_T)i3;
              perm[3] = (int8_T)i4;
            } else if (tmp <= x4[i4 - 1]) {
              perm[0] = (int8_T)nTail;
              perm[1] = (int8_T)i3;
              perm[2] = (int8_T)itmp;
              perm[3] = (int8_T)i4;
            } else {
              perm[0] = (int8_T)nTail;
              perm[1] = (int8_T)i3;
              perm[2] = (int8_T)i4;
              perm[3] = (int8_T)itmp;
            }
          } else {
            tmp_0 = x4[i4 - 1];
            if (tmp <= tmp_0) {
              if (x4[itmp - 1] <= tmp_0) {
                perm[0] = (int8_T)i3;
                perm[1] = (int8_T)nTail;
                perm[2] = (int8_T)itmp;
                perm[3] = (int8_T)i4;
              } else {
                perm[0] = (int8_T)i3;
                perm[1] = (int8_T)nTail;
                perm[2] = (int8_T)i4;
                perm[3] = (int8_T)itmp;
              }
            } else {
              perm[0] = (int8_T)i3;
              perm[1] = (int8_T)i4;
              perm[2] = (int8_T)nTail;
              perm[3] = (int8_T)itmp;
            }
          }

          i4 = ((int32_T)perm[0]) - 1;
          idx_data[ib - 3] = (int32_T)idx4[i4];
          itmp = ((int32_T)perm[1]) - 1;
          idx_data[ib - 2] = (int32_T)idx4[itmp];
          i3 = ((int32_T)perm[2]) - 1;
          idx_data[ib - 1] = (int32_T)idx4[i3];
          nTail = ((int32_T)perm[3]) - 1;
          idx_data[ib] = (int32_T)idx4[nTail];
          c_x_data[ib - 3] = x4[i4];
          c_x_data[ib - 2] = x4[itmp];
          c_x_data[ib - 1] = x4[i3];
          c_x_data[ib] = x4[nTail];
          ib = -1;
        }
      }
    }

    i3 = (*x_size) - bLen;
    wOffset = i3 - 1;
    if ((ib + 1) > 0) {
      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      switch (ib + 1) {
       case 1:
        perm[0] = 1;
        break;

       case 2:
        if (x4[0] <= x4[1]) {
          perm[0] = 1;
          perm[1] = 2;
        } else {
          perm[0] = 2;
          perm[1] = 1;
        }
        break;

       default:
        if (x4[0] <= x4[1]) {
          if (x4[1] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 2;
            perm[2] = 3;
          } else if (x4[0] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 3;
            perm[2] = 2;
          } else {
            perm[0] = 3;
            perm[1] = 1;
            perm[2] = 2;
          }
        } else if (x4[0] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 1;
          perm[2] = 3;
        } else if (x4[1] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 3;
          perm[2] = 1;
        } else {
          perm[0] = 3;
          perm[1] = 2;
          perm[2] = 1;
        }
        break;
      }

      for (nTail = 0; nTail <= ib; nTail++) {
        i4 = ((int32_T)perm[nTail]) - 1;
        itmp = (wOffset - ib) + nTail;
        idx_data[itmp] = (int32_T)idx4[i4];
        c_x_data[itmp] = x4[i4];
      }
    }

    ib = (bLen >> ((uint64_T)1)) + 1;
    for (nTail = 1; (nTail - 1) <= (ib - 2); nTail++) {
      itmp_tmp = wOffset + nTail;
      itmp = idx_data[itmp_tmp];
      idx_data[itmp_tmp] = idx_data[(*x_size) - nTail];
      i4 = (*x_size) - nTail;
      idx_data[i4] = itmp;
      c_x_data[itmp_tmp] = xwork_data[i4];
      c_x_data[i4] = xwork_data[itmp_tmp];
    }

    if ((((uint32_T)bLen) & 1U) != 0U) {
      bLen = wOffset + ib;
      c_x_data[bLen] = xwork_data[bLen];
    }

    if (i3 > 1) {
      if (0 <= (b_x_idx_0 - 1)) {
        memset(&iwork_data[0], 0, ((uint32_T)b_x_idx_0) * (sizeof(int32_T)));
      }

      wOffset = (i3 >> ((uint64_T)2));
      bLen = 4;
      while (wOffset > 1) {
        if ((((uint32_T)wOffset) & 1U) != 0U) {
          wOffset--;
          ib = bLen * wOffset;
          nTail = i3 - ib;
          if (nTail > bLen) {
            merge(idx_data, c_x_data, ib, bLen, nTail - bLen, iwork_data,
                  xwork_data);
          }
        }

        ib = (bLen << ((uint64_T)1));
        wOffset >>= (uint64_T)1;
        for (nTail = 0; nTail < wOffset; nTail++) {
          merge(idx_data, c_x_data, nTail * ib, bLen, bLen, iwork_data,
                xwork_data);
        }

        bLen = ib;
      }

      if (i3 > bLen) {
        merge(idx_data, c_x_data, 0, bLen, i3 - bLen, iwork_data, xwork_data);
      }
    }

    if (0 <= (c_x_size - 1)) {
      memcpy(&x_data[0], &c_x_data[0], ((uint32_T)c_x_size) * (sizeof(real_T)));
    }
  }
}

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
static void sort(real_T x_data[], int32_T *x_size)
{
  int32_T dim;
  real_T vwork_data[20];
  int32_T vstride;
  int32_T b;
  int32_T c_k;
  int32_T vwork_size;
  emxArray_int32_T_20 vwork_data_0;
  dim = 2;
  if ((*x_size) != 1) {
    dim = 1;
  }

  if (dim <= 1) {
    b = (*x_size) - 1;
  } else {
    b = 0;
  }

  vwork_size = (int32_T)((int8_T)(b + 1));
  vstride = 1;
  c_k = 0;
  while (c_k <= (dim - 2)) {
    vstride *= *x_size;
    c_k = 1;
  }

  for (dim = 0; dim < vstride; dim++) {
    for (c_k = 0; c_k <= b; c_k++) {
      vwork_data[c_k] = x_data[(c_k * vstride) + dim];
    }

    sortIdx(vwork_data, &vwork_size, vwork_data_0.data, &vwork_data_0.size);
    for (c_k = 0; c_k <= b; c_k++) {
      x_data[dim + (c_k * vstride)] = vwork_data[c_k];
    }
  }
}

/* Function for MATLAB Function: '<S1>/Guidance Estimator' */
static real_T combineVectorElements(const real_T x_data[], const int32_T *x_size)
{
  real_T y;
  int32_T k;
  if ((*x_size) == 0) {
    y = 0.0;
  } else {
    y = x_data[0];
    for (k = 2; k <= (*x_size); k++) {
      y += x_data[k - 1];
    }
  }

  return y;
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function state = noiseupdate(newmeas,time,inp,state,fromDATA,def)
 */
static void noiseupdate(uint32_T newmeas, real_T time, real_T inp_mode, real_T
  inp_freq, real_T inp_epsm, real_T inp_etam, sfl7P65dOznAw2EcCMFPbAE *state,
  const T_Weapon_MCP_DATA_TTKF *fromDATA, real_T b_def_MP_OPPORTUNITY, real_T
  b_def_MP_MISSIONLOAD, DW *dwork)
{
  real_T Rele;
  real_T Razi;
  real_T Rrge;
  int32_T no_medsamples;
  real_T eps_sort_data[20];
  real_T eta_sort_data[20];
  real_T medsamples_data[510];
  real_T linmargin;
  real_T eta_median_norm;
  real_T d2mean;
  real_T inp_epsm_norm;
  real_T eps_median_norm;
  int32_T d;
  int32_T s;
  real_T b_x_data[20];
  real_T eps_sort_data_0[20];
  int32_T loop_ub;
  int32_T eps_sort_size;
  int32_T b_x_size;
  uint32_T qY;
  int32_T eps_sort_size_tmp;
  boolean_T tmp;
  boolean_T exitg1;

  /* 'GUID_Embedded_Guid_TTKF_1D:987' if (state.rge < 5000.0) */
  if (state->rge < 5000.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:989' Rele = 0.349e-09*state.rge^2; */
    Rele = (state->rge * state->rge) * 3.49E-10;
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:990' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:993' Rele = 0.01396e-15*state.rge^4; */
    Rele = 1.396E-17 * rt_powd_snf(state->rge, 4.0);
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:996' a = 0.05*0.01745; */
  /* 'GUID_Embedded_Guid_TTKF_1D:998' if (Rele < a) */
  if (Rele < 0.0008725) {
    /* 'GUID_Embedded_Guid_TTKF_1D:999' Rele = a; */
    Rele = 0.0008725;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1001' Razi = Rele; */
  Razi = Rele;

  /* 'GUID_Embedded_Guid_TTKF_1D:1009' if (abs(state.them_g) > 0.00001) */
  Rrge = fabs(state->them_g);
  if (Rrge > 1.0E-5) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1010' if (inp.mode == def.MP_OPPORTUNITY) */
    if (inp_mode == b_def_MP_OPPORTUNITY) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1011' Rrge = LIB_limit(10.0,1000.0,(4000.0*Rele)/sin(abs(state.them_g))); */
      Rrge = (4000.0 * Rele) / sin(Rrge);

      /* 'LIB_limit:27' if (in > max) */
      if (Rrge > 1000.0) {
        /* 'LIB_limit:28' out = max; */
        Rrge = 1000.0;
      } else if (Rrge < 10.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rrge = 10.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:1012' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:1013' Rrge = 1.0/sin(abs(state.them_g)); */
      Rrge = 1.0 / sin(fabs(state->them_g));
    }
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:1015' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:1016' Rrge = 100000.0; */
    Rrge = 100000.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1024' if (newmeas) */
  if (newmeas != 0U) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1025' if (state.k > state.no_samples-1) */
    qY = ((uint32_T)state->no_samples) - /*MW:OvSatOk*/ 1U;
    if (qY > ((uint32_T)state->no_samples)) {
      qY = 0U;
    }

    if (((int32_T)state->k) > ((int32_T)qY)) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1026' state.k = uint8(0); */
      state->k = 0U;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1028' state.k = state.k + uint8(1); */
    d = (int32_T)((uint32_T)(((uint32_T)state->k) + 1U));
    if (((uint32_T)d) > 255U) {
      d = 255;
    }

    state->k = (uint8_T)d;

    /* 'GUID_Embedded_Guid_TTKF_1D:1029' state.eps_sample(state.k) = inp.epsm-state.the_b; */
    state->eps_sample[((int32_T)state->k) - 1] = inp_epsm - state->the_b;

    /* 'GUID_Embedded_Guid_TTKF_1D:1030' state.eta_sample(state.k) = inp.etam-state.psi_b; */
    state->eta_sample[((int32_T)state->k) - 1] = inp_etam - state->psi_b;

    /* 'GUID_Embedded_Guid_TTKF_1D:1032' no_medsamples = double(state.i_075-state.i_025+1); */
    qY = ((uint32_T)state->i_075) - /*MW:OvSatOk*/ ((uint32_T)state->i_025);
    if (qY > ((uint32_T)state->i_075)) {
      qY = 0U;
    }

    no_medsamples = (int32_T)((uint32_T)(qY + 1U));
    if (((uint32_T)no_medsamples) > 255U) {
      no_medsamples = 255;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1034' if (state.rge < 4600.0) */
    /* 'GUID_Embedded_Guid_TTKF_1D:1037' if (state.rge < 4390.0) */
    /* 'GUID_Embedded_Guid_TTKF_1D:1041' eps_mean = mean(state.eps_sample(1:state.no_samples)); */
    if (1 > ((int32_T)state->no_samples)) {
      loop_ub = -1;
    } else {
      loop_ub = ((int32_T)state->no_samples) - 1;
    }

    b_x_size = loop_ub + 1;
    if (0 <= loop_ub) {
      memcpy(&b_x_data[0], &state->eps_sample[0], ((uint32_T)((int32_T)(loop_ub
                + 1))) * (sizeof(real_T)));
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1042' eps_sort = sort(state.eps_sample(1:state.no_samples)); */
    if (1 > ((int32_T)state->no_samples)) {
      loop_ub = -1;
    } else {
      loop_ub = ((int32_T)state->no_samples) - 1;
    }

    eps_sort_size = loop_ub + 1;
    if (0 <= loop_ub) {
      memcpy(&eps_sort_data[0], &state->eps_sample[0], ((uint32_T)((int32_T)
               (loop_ub + 1))) * (sizeof(real_T)));
    }

    sort(eps_sort_data, &eps_sort_size);

    /* 'GUID_Embedded_Guid_TTKF_1D:1043' state.eps_median = sum(eps_sort(state.i_025:state.i_075))/no_medsamples; */
    if (state->i_025 > state->i_075) {
      s = 1;
      d = 0;
    } else {
      s = (int32_T)state->i_025;
      d = (int32_T)state->i_075;
    }

    eps_sort_size_tmp = d - s;
    eps_sort_size = eps_sort_size_tmp + 1;
    for (d = 0; d <= eps_sort_size_tmp; d++) {
      eps_sort_data_0[d] = eps_sort_data[(s + d) - 1];
    }

    state->eps_median = combineVectorElements(eps_sort_data_0, &eps_sort_size) /
      ((real_T)no_medsamples);

    /* 'GUID_Embedded_Guid_TTKF_1D:1045' state.eps_diff = 0.6*state.eps_diff + 0.4*LIB_limit(0.005,0.05,abs(eps_mean-state.eps_median)); */
    linmargin = fabs((combineVectorElements(b_x_data, &b_x_size) / ((real_T)
      b_x_size)) - state->eps_median);

    /* 'LIB_limit:27' if (in > max) */
    if (linmargin > 0.05) {
      /* 'LIB_limit:28' out = max; */
      linmargin = 0.05;
    } else if (linmargin < 0.005) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      linmargin = 0.005;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    state->eps_diff = (0.6 * state->eps_diff) + (0.4 * linmargin);

    /* 'GUID_Embedded_Guid_TTKF_1D:1050' eta_mean = mean(state.eta_sample(1:state.no_samples)); */
    if (1 > ((int32_T)state->no_samples)) {
      loop_ub = -1;
    } else {
      loop_ub = ((int32_T)state->no_samples) - 1;
    }

    b_x_size = loop_ub + 1;
    if (0 <= loop_ub) {
      memcpy(&b_x_data[0], &state->eta_sample[0], ((uint32_T)((int32_T)(loop_ub
                + 1))) * (sizeof(real_T)));
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1051' eta_sort = sort(state.eta_sample(1:state.no_samples)); */
    if (1 > ((int32_T)state->no_samples)) {
      loop_ub = -1;
    } else {
      loop_ub = ((int32_T)state->no_samples) - 1;
    }

    eps_sort_size = loop_ub + 1;
    if (0 <= loop_ub) {
      memcpy(&eta_sort_data[0], &state->eta_sample[0], ((uint32_T)((int32_T)
               (loop_ub + 1))) * (sizeof(real_T)));
    }

    sort(eta_sort_data, &eps_sort_size);

    /* 'GUID_Embedded_Guid_TTKF_1D:1052' state.eta_median = sum(eta_sort(state.i_025:state.i_075))/no_medsamples; */
    if (state->i_025 > state->i_075) {
      s = 1;
      d = 0;
    } else {
      s = (int32_T)state->i_025;
      d = (int32_T)state->i_075;
    }

    eps_sort_size_tmp = d - s;
    loop_ub = eps_sort_size_tmp + 1;
    for (d = 0; d <= eps_sort_size_tmp; d++) {
      eps_sort_data_0[d] = eta_sort_data[(s + d) - 1];
    }

    state->eta_median = combineVectorElements(eps_sort_data_0, &loop_ub) /
      ((real_T)no_medsamples);

    /* 'GUID_Embedded_Guid_TTKF_1D:1054' state.eta_diff = 0.6*state.eta_diff + 0.4*LIB_limit(0.005,0.05,abs(eta_mean-state.eta_median)); */
    linmargin = fabs((combineVectorElements(b_x_data, &b_x_size) / ((real_T)
      b_x_size)) - state->eta_median);

    /* 'LIB_limit:27' if (in > max) */
    if (linmargin > 0.05) {
      /* 'LIB_limit:28' out = max; */
      linmargin = 0.05;
    } else if (linmargin < 0.005) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      linmargin = 0.005;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    state->eta_diff = (0.6 * state->eta_diff) + (0.4 * linmargin);

    /* 'GUID_Embedded_Guid_TTKF_1D:1059' medsamples = zeros(2*no_medsamples,1); */
    eps_sort_size_tmp = (no_medsamples << ((uint64_T)1));
    if (0 <= (eps_sort_size_tmp - 1)) {
      memset(&medsamples_data[0], 0, ((uint32_T)eps_sort_size_tmp) * (sizeof
              (real_T)));
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1060' medsamples(              1:  no_medsamples) = eps_sort(state.i_025:state.i_075); */
    if (state->i_025 > state->i_075) {
      s = 1;
      d = 0;
    } else {
      s = (int32_T)state->i_025;
      d = (int32_T)state->i_075;
    }

    loop_ub = d - s;
    for (d = 0; d <= loop_ub; d++) {
      medsamples_data[d] = eps_sort_data[(s + d) - 1];
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1061' medsamples(no_medsamples+1:2*no_medsamples) = eta_sort(state.i_025:state.i_075); */
    if (state->i_025 > state->i_075) {
      s = 1;
      d = 0;
    } else {
      s = (int32_T)state->i_025;
      d = (int32_T)state->i_075;
    }

    if ((no_medsamples + 1) > eps_sort_size_tmp) {
      no_medsamples = 0;
    }

    loop_ub = d - s;
    for (d = 0; d <= loop_ub; d++) {
      medsamples_data[no_medsamples + d] = eta_sort_data[(s + d) - 1];
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1062' state.nserge = 0.7*state.nserge + 0.3*(max(medsamples) - min(medsamples)); */
    if (eps_sort_size_tmp <= 2) {
      if (medsamples_data[0] < medsamples_data[1]) {
        linmargin = medsamples_data[1];
      } else if (rtIsNaN(medsamples_data[0])) {
        if (!rtIsNaN(medsamples_data[1])) {
          linmargin = medsamples_data[1];
        } else {
          linmargin = medsamples_data[0];
        }
      } else {
        linmargin = medsamples_data[0];
      }

      if (medsamples_data[0] > medsamples_data[1]) {
        d2mean = medsamples_data[1];
      } else if (rtIsNaN(medsamples_data[0])) {
        if (!rtIsNaN(medsamples_data[1])) {
          d2mean = medsamples_data[1];
        } else {
          d2mean = medsamples_data[0];
        }
      } else {
        d2mean = medsamples_data[0];
      }
    } else {
      tmp = rtIsNaN(medsamples_data[0]);
      if (!tmp) {
        no_medsamples = 1;
      } else {
        no_medsamples = 0;
        s = 2;
        exitg1 = false;
        while ((!exitg1) && (s <= eps_sort_size_tmp)) {
          if (!rtIsNaN(medsamples_data[s - 1])) {
            no_medsamples = s;
            exitg1 = true;
          } else {
            s++;
          }
        }
      }

      if (no_medsamples == 0) {
        linmargin = medsamples_data[0];
      } else {
        linmargin = medsamples_data[no_medsamples - 1];
        while ((no_medsamples + 1) <= eps_sort_size_tmp) {
          if (linmargin < medsamples_data[no_medsamples]) {
            linmargin = medsamples_data[no_medsamples];
          }

          no_medsamples++;
        }
      }

      if (!tmp) {
        no_medsamples = 1;
      } else {
        no_medsamples = 0;
        s = 2;
        exitg1 = false;
        while ((!exitg1) && (s <= eps_sort_size_tmp)) {
          if (!rtIsNaN(medsamples_data[s - 1])) {
            no_medsamples = s;
            exitg1 = true;
          } else {
            s++;
          }
        }
      }

      if (no_medsamples == 0) {
        d2mean = medsamples_data[0];
      } else {
        d2mean = medsamples_data[no_medsamples - 1];
        while ((no_medsamples + 1) <= eps_sort_size_tmp) {
          if (d2mean > medsamples_data[no_medsamples]) {
            d2mean = medsamples_data[no_medsamples];
          }

          no_medsamples++;
        }
      }
    }

    state->nserge = ((linmargin - d2mean) * 0.3) + (0.7 * state->nserge);

    /* 'GUID_Embedded_Guid_TTKF_1D:1063' if (inp.mode == def.MP_MISSIONLOAD) */
    if (inp_mode == b_def_MP_MISSIONLOAD) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1064' linmargin = LIB_limit(0.03491,0.0873,0.50*state.nserge); */
      linmargin = 0.5 * state->nserge;

      /* 'LIB_limit:27' if (in > max) */
      if (linmargin > 0.0873) {
        /* 'LIB_limit:28' out = max; */
        linmargin = 0.0873;
      } else if (linmargin < 0.03491) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        linmargin = 0.03491;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:1065' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:1066' linmargin = LIB_limit(0.00000,0.0873,0.50*state.conf*state.nserge); */
      linmargin = (0.5 * state->conf) * state->nserge;

      /* 'LIB_limit:27' if (in > max) */
      if (linmargin > 0.0873) {
        /* 'LIB_limit:28' out = max; */
        linmargin = 0.0873;
      } else if (linmargin < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        linmargin = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1071' eta_median_norm = state.eta_median + state.psi_b; */
    eta_median_norm = state->eta_median + state->psi_b;

    /* 'GUID_Embedded_Guid_TTKF_1D:1072' state.psi_median = eta_median_norm; */
    state->psi_median = eta_median_norm;

    /* 'GUID_Embedded_Guid_TTKF_1D:1073' if (eta_median_norm > 0.0) */
    if (eta_median_norm > 0.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1074' d2mean = (double(fromDATA.seeker.Alin)-linmargin) - eta_median_norm; */
      inp_epsm_norm = ((real_T)fromDATA->seeker.Alin) - linmargin;
      d2mean = inp_epsm_norm - eta_median_norm;

      /* 'GUID_Embedded_Guid_TTKF_1D:1075' if (inp.etam > eta_median_norm) */
      if (inp_etam > eta_median_norm) {
        /* 'GUID_Embedded_Guid_TTKF_1D:1077' delpsi = inp.etam - (double(fromDATA.seeker.Alin)-linmargin); */
        eta_median_norm = inp_etam - inp_epsm_norm;
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:1078' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:1081' delpsi = inp.etam - (double(fromDATA.seeker.Alin)-linmargin-2.0*d2mean); */
        /* 'GUID_Embedded_Guid_TTKF_1D:1082' delpsi = -delpsi; */
        eta_median_norm = -(inp_etam - (inp_epsm_norm - (2.0 * d2mean)));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1084' state.psi_lin_up = double(fromDATA.seeker.Alin)-linmargin; */
      state->psi_lin_up = inp_epsm_norm;

      /* 'GUID_Embedded_Guid_TTKF_1D:1085' state.psi_lin_lo = double(fromDATA.seeker.Alin)-linmargin-2.0*d2mean; */
      state->psi_lin_lo = (((real_T)fromDATA->seeker.Alin) - linmargin) - (2.0 *
        d2mean);
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:1086' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:1087' d2mean = eta_median_norm - -(double(fromDATA.seeker.Alin)-linmargin); */
      d2mean = eta_median_norm - (-(((real_T)fromDATA->seeker.Alin) - linmargin));

      /* 'GUID_Embedded_Guid_TTKF_1D:1088' if (inp.etam < eta_median_norm) */
      if (inp_etam < eta_median_norm) {
        /* 'GUID_Embedded_Guid_TTKF_1D:1090' delpsi = inp.etam - -(double(fromDATA.seeker.Alin)-linmargin); */
        /* 'GUID_Embedded_Guid_TTKF_1D:1091' delpsi = -delpsi; */
        eta_median_norm = -(inp_etam - (-(((real_T)fromDATA->seeker.Alin) -
          linmargin)));
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:1092' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:1094' delpsi = inp.etam - (-(double(fromDATA.seeker.Alin)-linmargin)+2.0*d2mean); */
        eta_median_norm = inp_etam - ((-(((real_T)fromDATA->seeker.Alin) -
          linmargin)) + (2.0 * d2mean));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1096' state.psi_lin_up = -(double(fromDATA.seeker.Alin)-linmargin)+2.0*d2mean; */
      state->psi_lin_up = (-(((real_T)fromDATA->seeker.Alin) - linmargin)) +
        (2.0 * d2mean);

      /* 'GUID_Embedded_Guid_TTKF_1D:1097' state.psi_lin_lo = -(double(fromDATA.seeker.Alin)-linmargin); */
      state->psi_lin_lo = -(((real_T)fromDATA->seeker.Alin) - linmargin);
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1099' state.delpsi = delpsi; */
    state->delpsi = eta_median_norm;

    /* 'GUID_Embedded_Guid_TTKF_1D:1100' if (delpsi < 0.0) */
    if (eta_median_norm < 0.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1101' delpsi = 0.0; */
      eta_median_norm = 0.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1103' inp_epsm_norm = inp.epsm - double(fromDATA.seeker.Bele); */
    inp_epsm_norm = inp_epsm - ((real_T)fromDATA->seeker.Bele);

    /* 'GUID_Embedded_Guid_TTKF_1D:1104' eps_median_norm = state.eps_median + state.the_b - double(fromDATA.seeker.Bele); */
    eps_median_norm = (state->eps_median + state->the_b) - ((real_T)
      fromDATA->seeker.Bele);

    /* 'GUID_Embedded_Guid_TTKF_1D:1105' state.the_median = eps_median_norm + double(fromDATA.seeker.Bele); */
    state->the_median = eps_median_norm + ((real_T)fromDATA->seeker.Bele);

    /* 'GUID_Embedded_Guid_TTKF_1D:1106' if (eps_median_norm > 0.0) */
    if (eps_median_norm > 0.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1107' d2mean = (double(fromDATA.seeker.Alin)-linmargin) - eps_median_norm; */
      d2mean = (((real_T)fromDATA->seeker.Alin) - linmargin) - eps_median_norm;

      /* 'GUID_Embedded_Guid_TTKF_1D:1108' if (inp_epsm_norm > eps_median_norm) */
      if (inp_epsm_norm > eps_median_norm) {
        /* 'GUID_Embedded_Guid_TTKF_1D:1110' delthe = inp_epsm_norm - (double(fromDATA.seeker.Alin)-linmargin); */
        inp_epsm_norm -= ((real_T)fromDATA->seeker.Alin) - linmargin;
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:1111' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:1114' delthe = inp_epsm_norm - (double(fromDATA.seeker.Alin)-linmargin-2.0*d2mean); */
        /* 'GUID_Embedded_Guid_TTKF_1D:1115' delthe = -delthe; */
        inp_epsm_norm = -(inp_epsm_norm - ((((real_T)fromDATA->seeker.Alin) -
          linmargin) - (2.0 * d2mean)));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1117' state.the_lin_up = double(fromDATA.seeker.Alin)-linmargin + double(fromDATA.seeker.Bele); */
      state->the_lin_up = (((real_T)fromDATA->seeker.Alin) - linmargin) +
        ((real_T)fromDATA->seeker.Bele);

      /* 'GUID_Embedded_Guid_TTKF_1D:1118' state.the_lin_lo = double(fromDATA.seeker.Alin)-linmargin-2.0*d2mean + double(fromDATA.seeker.Bele); */
      state->the_lin_lo = ((((real_T)fromDATA->seeker.Alin) - linmargin) - (2.0 *
        d2mean)) + ((real_T)fromDATA->seeker.Bele);
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:1119' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:1120' d2mean = eps_median_norm - -(double(fromDATA.seeker.Alin)-linmargin); */
      d2mean = eps_median_norm - (-(((real_T)fromDATA->seeker.Alin) - linmargin));

      /* 'GUID_Embedded_Guid_TTKF_1D:1121' if (inp_epsm_norm < eps_median_norm) */
      if (inp_epsm_norm < eps_median_norm) {
        /* 'GUID_Embedded_Guid_TTKF_1D:1123' delthe = inp_epsm_norm - -(double(fromDATA.seeker.Alin)-linmargin); */
        /* 'GUID_Embedded_Guid_TTKF_1D:1124' delthe = -delthe; */
        inp_epsm_norm = -(inp_epsm_norm - (-(((real_T)fromDATA->seeker.Alin) -
          linmargin)));
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:1125' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:1127' delthe = inp_epsm_norm - (-(double(fromDATA.seeker.Alin)-linmargin)+2.0*d2mean); */
        inp_epsm_norm -= (-(((real_T)fromDATA->seeker.Alin) - linmargin)) + (2.0
          * d2mean);
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1129' state.the_lin_up = -(double(fromDATA.seeker.Alin)-linmargin)+2.0*d2mean + double(fromDATA.seeker.Bele); */
      state->the_lin_up = ((-(((real_T)fromDATA->seeker.Alin) - linmargin)) +
                           (2.0 * d2mean)) + ((real_T)fromDATA->seeker.Bele);

      /* 'GUID_Embedded_Guid_TTKF_1D:1130' state.the_lin_lo = -(double(fromDATA.seeker.Alin)-linmargin) + double(fromDATA.seeker.Bele); */
      state->the_lin_lo = (-(((real_T)fromDATA->seeker.Alin) - linmargin)) +
        ((real_T)fromDATA->seeker.Bele);
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1132' state.delthe = delthe; */
    state->delthe = inp_epsm_norm;

    /* 'GUID_Embedded_Guid_TTKF_1D:1133' if (delthe < 0.0) */
    if (inp_epsm_norm < 0.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1134' delthe = 0.0; */
      inp_epsm_norm = 0.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1144' Pfac = 1.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:1147' Kele = LIB_limit(1.0,10.0,286*delpsi+143*delthe*Pfac*100*state.eps_diff); */
    linmargin = (((143.0 * inp_epsm_norm) * 100.0) * state->eps_diff) + (286.0 *
      eta_median_norm);

    /* 'LIB_limit:27' if (in > max) */
    if (linmargin > 10.0) {
      /* 'LIB_limit:28' out = max; */
      linmargin = 10.0;
    } else if (linmargin < 1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      linmargin = 1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1148' Kazi = LIB_limit(1.0,10.0,286*delthe+143*delpsi*Pfac*100*state.eta_diff); */
    d2mean = (((143.0 * eta_median_norm) * 100.0) * state->eta_diff) + (286.0 *
      inp_epsm_norm);

    /* 'LIB_limit:27' if (in > max) */
    if (d2mean > 10.0) {
      /* 'LIB_limit:28' out = max; */
      d2mean = 10.0;
    } else if (d2mean < 1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      d2mean = 1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1149' Rele = LIB_limit(a,100.0,Rele*Kele); */
    Rele *= linmargin;

    /* 'LIB_limit:27' if (in > max) */
    if (Rele > 100.0) {
      /* 'LIB_limit:28' out = max; */
      Rele = 100.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1150' Razi = LIB_limit(a,100.0,Razi*Kazi); */
    Razi *= d2mean;

    /* 'LIB_limit:27' if (in > max) */
    if (Razi > 100.0) {
      /* 'LIB_limit:28' out = max; */
      Razi = 100.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1160' state.R(1,1) = Rele^2; */
    state->R[0] = Rele * Rele;

    /* 'GUID_Embedded_Guid_TTKF_1D:1161' state.R(2,2) = Razi^2; */
    state->R[4] = Razi * Razi;

    /* 'GUID_Embedded_Guid_TTKF_1D:1162' if (state.them_g < -0.001) */
    if (state->them_g < -0.001) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1164' state.R(3,3) = Rrge^2; */
      state->R[8] = Rrge * Rrge;
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:1165' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:1166' state.R(3,3) = 10000003333.334; */
      state->R[8] = 1.0000003333334E+10;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1169' state.t_fillsample = time + 5.0/inp.freq; */
    state->t_fillsample = (5.0 / inp_freq) + time;
  } else {
    if (time > state->t_fillsample) {
      /* 'GUID_Embedded_Guid_TTKF_1D:1171' elseif (time > state.t_fillsample) */
      /* 'GUID_Embedded_Guid_TTKF_1D:1172' state.nserge = LIB_limit(0.0,0.1745,state.nserge + 1.0); */
      /* 'LIB_limit:27' if (in > max) */
      if ((state->nserge + 1.0) > 0.1745) {
        /* 'LIB_limit:28' out = max; */
        Razi = 0.1745;
      } else if ((state->nserge + 1.0) < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Razi = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        Razi = state->nserge + 1.0;
      }

      state->nserge = Razi;

      /* 'GUID_Embedded_Guid_TTKF_1D:1173' if (state.k > state.no_samples-1) */
      qY = ((uint32_T)state->no_samples) - /*MW:OvSatOk*/ 1U;
      if (qY > ((uint32_T)state->no_samples)) {
        qY = 0U;
      }

      if (((int32_T)state->k) > ((int32_T)qY)) {
        /* 'GUID_Embedded_Guid_TTKF_1D:1174' state.k = uint8(0); */
        state->k = 0U;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1176' state.k = state.k + uint8(1); */
      d = (int32_T)((uint32_T)(((uint32_T)state->k) + 1U));
      if (((uint32_T)d) > 255U) {
        d = 255;
      }

      state->k = (uint8_T)d;

      /* 'GUID_Embedded_Guid_TTKF_1D:1177' state.eps_sample(state.k) = 50.0*Rele*randn; */
      state->eps_sample[((int32_T)state->k) - 1] = (50.0 * Rele) * randn(dwork);

      /* 'GUID_Embedded_Guid_TTKF_1D:1178' state.eta_sample(state.k) = 50.0*Razi*randn; */
      state->eta_sample[((int32_T)state->k) - 1] = (50.0 * Rele) * randn(dwork);

      /* 'GUID_Embedded_Guid_TTKF_1D:1180' eps_mean = mean(state.eps_sample(1:state.no_samples)); */
      if (1 > ((int32_T)state->no_samples)) {
        loop_ub = -1;
      } else {
        loop_ub = ((int32_T)state->no_samples) - 1;
      }

      b_x_size = loop_ub + 1;
      if (0 <= loop_ub) {
        memcpy(&b_x_data[0], &state->eps_sample[0], ((uint32_T)((int32_T)
                 (loop_ub + 1))) * (sizeof(real_T)));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1181' eps_sort = sort(state.eps_sample(1:state.no_samples)); */
      /* 'GUID_Embedded_Guid_TTKF_1D:1182' state.eps_median = sum(eps_sort(state.i_025:state.i_075))/double(state.i_075-state.i_025+1); */
      if (state->i_025 > state->i_075) {
        no_medsamples = 1;
        s = 0;
      } else {
        no_medsamples = (int32_T)state->i_025;
        s = (int32_T)state->i_075;
      }

      if (1 > ((int32_T)state->no_samples)) {
        loop_ub = -1;
      } else {
        loop_ub = ((int32_T)state->no_samples) - 1;
      }

      eps_sort_size = loop_ub + 1;
      if (0 <= loop_ub) {
        memcpy(&eps_sort_data[0], &state->eps_sample[0], ((uint32_T)((int32_T)
                 (loop_ub + 1))) * (sizeof(real_T)));
      }

      sort(eps_sort_data, &eps_sort_size);
      eps_sort_size_tmp = s - no_medsamples;
      eps_sort_size = eps_sort_size_tmp + 1;
      for (d = 0; d <= eps_sort_size_tmp; d++) {
        eps_sort_data_0[d] = eps_sort_data[(no_medsamples + d) - 1];
      }

      qY = ((uint32_T)state->i_075) - /*MW:OvSatOk*/ ((uint32_T)state->i_025);
      if (qY > ((uint32_T)state->i_075)) {
        qY = 0U;
      }

      d = (int32_T)((uint32_T)(qY + 1U));
      if (((uint32_T)d) > 255U) {
        d = 255;
      }

      state->eps_median = combineVectorElements(eps_sort_data_0, &eps_sort_size)
        / ((real_T)d);

      /* 'GUID_Embedded_Guid_TTKF_1D:1183' state.eps_diff = 0.6*state.eps_diff + 0.4*LIB_limit(0.005,0.05,abs(eps_mean-state.eps_median)); */
      Rele = fabs((combineVectorElements(b_x_data, &b_x_size) / ((real_T)
        b_x_size)) - state->eps_median);

      /* 'LIB_limit:27' if (in > max) */
      if (Rele > 0.05) {
        /* 'LIB_limit:28' out = max; */
        Rele = 0.05;
      } else if (Rele < 0.005) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rele = 0.005;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      state->eps_diff = (0.6 * state->eps_diff) + (0.4 * Rele);

      /* 'GUID_Embedded_Guid_TTKF_1D:1185' eta_mean = mean(state.eta_sample(1:state.no_samples)); */
      if (1 > ((int32_T)state->no_samples)) {
        loop_ub = -1;
      } else {
        loop_ub = ((int32_T)state->no_samples) - 1;
      }

      b_x_size = loop_ub + 1;
      if (0 <= loop_ub) {
        memcpy(&b_x_data[0], &state->eta_sample[0], ((uint32_T)((int32_T)
                 (loop_ub + 1))) * (sizeof(real_T)));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:1186' eta_sort = sort(state.eta_sample(1:state.no_samples)); */
      /* 'GUID_Embedded_Guid_TTKF_1D:1187' state.eta_median = sum(eta_sort(state.i_025:state.i_075))/double(state.i_075-state.i_025+1); */
      if (state->i_025 > state->i_075) {
        no_medsamples = 1;
        s = 0;
      } else {
        no_medsamples = (int32_T)state->i_025;
        s = (int32_T)state->i_075;
      }

      if (1 > ((int32_T)state->no_samples)) {
        loop_ub = -1;
      } else {
        loop_ub = ((int32_T)state->no_samples) - 1;
      }

      eps_sort_size = loop_ub + 1;
      if (0 <= loop_ub) {
        memcpy(&eps_sort_data[0], &state->eta_sample[0], ((uint32_T)((int32_T)
                 (loop_ub + 1))) * (sizeof(real_T)));
      }

      sort(eps_sort_data, &eps_sort_size);
      eps_sort_size_tmp = s - no_medsamples;
      eps_sort_size = eps_sort_size_tmp + 1;
      for (d = 0; d <= eps_sort_size_tmp; d++) {
        eps_sort_data_0[d] = eps_sort_data[(no_medsamples + d) - 1];
      }

      qY = ((uint32_T)state->i_075) - /*MW:OvSatOk*/ ((uint32_T)state->i_025);
      if (qY > ((uint32_T)state->i_075)) {
        qY = 0U;
      }

      d = (int32_T)((uint32_T)(qY + 1U));
      if (((uint32_T)d) > 255U) {
        d = 255;
      }

      state->eta_median = combineVectorElements(eps_sort_data_0, &eps_sort_size)
        / ((real_T)d);

      /* 'GUID_Embedded_Guid_TTKF_1D:1188' state.eta_diff = 0.6*state.eta_diff + 0.4*LIB_limit(0.005,0.05,abs(eta_mean-state.eta_median)); */
      Rele = fabs((combineVectorElements(b_x_data, &b_x_size) / ((real_T)
        b_x_size)) - state->eta_median);

      /* 'LIB_limit:27' if (in > max) */
      if (Rele > 0.05) {
        /* 'LIB_limit:28' out = max; */
        Rele = 0.05;
      } else if (Rele < 0.005) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rele = 0.005;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      state->eta_diff = (0.6 * state->eta_diff) + (0.4 * Rele);

      /* 'GUID_Embedded_Guid_TTKF_1D:1190' state.t_fillsample = time + 5.0/inp.freq; */
      state->t_fillsample = (5.0 / inp_freq) + time;
    }
  }
}

/*
 * Function for MATLAB Function: '<S1>/Guidance Estimator'
 * function [delstate,state] = measupdate(inp,state,def)
 */
static void measupdate(real_T inp_mode, real_T inp_freq, real_T inp_epsm, real_T
  inp_etam, real_T inp_rgem, uint8_T inp_ValidGNSSNav, sfl7P65dOznAw2EcCMFPbAE
  *state, real_T b_def_MP_OPPORTUNITY, real_T b_def_MP_MISSIONLOAD, real_T
  delstate[18])
{
  real_T delR;
  real_T delRlim;
  real_T Vlim;
  real_T delvelY;
  real_T y[9];
  real_T x[9];
  int32_T p1;
  int32_T p2;
  int32_T p3;
  real_T absx11;
  real_T absx21;
  real_T absx31;
  int32_T itmp;
  int8_T b_I[36];
  int8_T c_I[36];
  real_T y_tmp[18];
  real_T absx11_0[3];
  real_T state_0[18];
  real_T b_I_0[36];
  real_T b_I_1[36];
  real_T c_I_0[36];
  real_T state_1[36];
  real_T inov_idx_0;
  real_T inov_idx_1;
  real_T inov_idx_2;
  real_T delstate_tmp;
  real_T delstate_tmp_0;

  /* 'GUID_Embedded_Guid_TTKF_1D:1233' if (state.rge < 9000.0) */
  /* 'GUID_Embedded_Guid_TTKF_1D:1237' inov = zeros(3,1); */
  /* 'GUID_Embedded_Guid_TTKF_1D:1240' inov(3,1) = inp.rgem - state.rge; */
  inov_idx_2 = inp_rgem - state->rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:1241' inov(1,1) = LIB_limit(-0.050,+0.050,inp.epsm - state.eps); */
  absx11 = inp_epsm - state->eps;

  /* 'LIB_limit:27' if (in > max) */
  if (absx11 > 0.05) {
    /* 'LIB_limit:28' out = max; */
    inov_idx_0 = 0.05;
  } else if (absx11 < -0.05) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    inov_idx_0 = -0.05;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    inov_idx_0 = absx11;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1242' inov(2,1) = LIB_limit(-0.050,+0.050,inp.etam - state.eta); */
  absx11 = inp_etam - state->eta;

  /* 'LIB_limit:27' if (in > max) */
  if (absx11 > 0.05) {
    /* 'LIB_limit:28' out = max; */
    inov_idx_1 = 0.05;
  } else if (absx11 < -0.05) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    inov_idx_1 = -0.05;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    inov_idx_1 = absx11;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1245' state.K = state.P*state.H'*(inv(state.H*state.P*state.H' + state.R)); */
  for (p2 = 0; p2 < 3; p2++) {
    for (p3 = 0; p3 < 6; p3++) {
      itmp = (3 * p3) + p2;
      y_tmp[p3 + (6 * p2)] = state->H[itmp];
      state_0[itmp] = 0.0;
      for (p1 = 0; p1 < 6; p1++) {
        state_0[itmp] += state->H[(3 * p1) + p2] * state->P[(6 * p3) + p1];
      }
    }
  }

  for (p2 = 0; p2 < 3; p2++) {
    for (p3 = 0; p3 < 3; p3++) {
      absx11 = 0.0;
      for (p1 = 0; p1 < 6; p1++) {
        absx11 += state_0[(3 * p1) + p3] * y_tmp[(6 * p2) + p1];
      }

      p1 = (3 * p2) + p3;
      y[p1] = state->R[p1] + absx11;
    }
  }

  memcpy(&x[0], &y[0], 9U * (sizeof(real_T)));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(y[0]);
  absx21 = fabs(y[1]);
  absx31 = fabs(y[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    x[0] = y[1];
    x[1] = y[0];
    x[3] = y[4];
    x[4] = y[3];
    x[6] = y[7];
    x[7] = y[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      x[0] = y[2];
      x[2] = y[0];
      x[3] = y[5];
      x[5] = y[3];
      x[6] = y[8];
      x[8] = y[6];
    }
  }

  absx11 = x[1] / x[0];
  x[1] /= x[0];
  absx21 = x[2] / x[0];
  x[2] /= x[0];
  x[4] -= absx11 * x[3];
  x[5] -= absx21 * x[3];
  x[7] -= absx11 * x[6];
  x[8] -= absx21 * x[6];
  if (fabs(x[5]) > fabs(x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    x[1] = absx21;
    x[2] = absx11;
    absx11 = x[4];
    x[4] = x[5];
    x[5] = absx11;
    absx11 = x[7];
    x[7] = x[8];
    x[8] = absx11;
  }

  absx31 = x[5] / x[4];
  x[8] -= absx31 * x[7];
  absx11 = ((absx31 * x[1]) - x[2]) / x[8];
  absx21 = (-((x[7] * absx11) + x[1])) / x[4];
  y[p1] = ((1.0 - (x[3] * absx21)) - (x[6] * absx11)) / x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = (-absx31) / x[8];
  absx21 = (1.0 - (x[7] * absx11)) / x[4];
  y[p2] = (-((x[3] * absx21) + (x[6] * absx11))) / x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / x[8];
  absx21 = ((-x[7]) * absx11) / x[4];
  y[p3] = (-((x[3] * absx21) + (x[6] * absx11))) / x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
  for (p2 = 0; p2 < 6; p2++) {
    for (p3 = 0; p3 < 3; p3++) {
      itmp = p2 + (6 * p3);
      state_0[itmp] = 0.0;
      for (p1 = 0; p1 < 6; p1++) {
        state_0[itmp] += state->P[(6 * p1) + p2] * y_tmp[(6 * p3) + p1];
      }
    }

    for (p3 = 0; p3 < 3; p3++) {
      p1 = p2 + (6 * p3);
      state->K[p1] = 0.0;
      state->K[p1] += y[3 * p3] * state_0[p2];
      state->K[p1] = (y[(3 * p3) + 1] * state_0[p2 + 6]) + state->K[(6 * p3) +
        p2];
      state->K[p1] = (y[(3 * p3) + 2] * state_0[p2 + 12]) + state->K[(6 * p3) +
        p2];
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1247' delstate = zeros(6,3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:1248' delstate(1,1) = state.X(1) + state.K(1,1)*inov(1); */
  absx11 = state->K[0] * inov_idx_0;
  delstate[0] = absx11 + state->X[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:1249' delstate(1,2) = state.X(1) + state.K(1,2)*inov(2); */
  delstate_tmp = state->K[6] * inov_idx_1;
  delstate[6] = delstate_tmp + state->X[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:1250' delstate(1,3) = state.X(1) + state.K(1,3)*inov(3); */
  delstate_tmp_0 = state->K[12] * inov_idx_2;
  delstate[12] = delstate_tmp_0 + state->X[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:1251' delstate(2,1) = state.X(2) + state.K(2,1)*inov(1); */
  absx21 = state->K[1] * inov_idx_0;
  delstate[1] = absx21 + state->X[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:1252' delstate(2,2) = state.X(2) + state.K(2,2)*inov(2); */
  delRlim = state->K[7] * inov_idx_1;
  delstate[7] = delRlim + state->X[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:1253' delstate(2,3) = state.X(2) + state.K(2,3)*inov(3); */
  delvelY = state->K[13] * inov_idx_2;
  delstate[13] = delvelY + state->X[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:1254' delstate(3,1) = state.X(3) + state.K(3,1)*inov(1); */
  absx31 = state->K[2] * inov_idx_0;
  delstate[2] = absx31 + state->X[2];

  /* 'GUID_Embedded_Guid_TTKF_1D:1255' delstate(3,2) = state.X(3) + state.K(3,2)*inov(2); */
  delR = state->K[8] * inov_idx_1;
  delstate[8] = delR + state->X[2];

  /* 'GUID_Embedded_Guid_TTKF_1D:1256' delstate(3,3) = state.X(3) + state.K(3,3)*inov(3); */
  Vlim = state->K[14] * inov_idx_2;
  delstate[14] = Vlim + state->X[2];

  /* 'GUID_Embedded_Guid_TTKF_1D:1257' delstate(4,1) = state.X(4) + state.K(4,1)*inov(1); */
  delstate[3] = (state->K[3] * inov_idx_0) + state->X[3];

  /* 'GUID_Embedded_Guid_TTKF_1D:1258' delstate(4,2) = state.X(4) + state.K(4,2)*inov(2); */
  delstate[9] = (state->K[9] * inov_idx_1) + state->X[3];

  /* 'GUID_Embedded_Guid_TTKF_1D:1259' delstate(4,3) = state.X(4) + state.K(4,3)*inov(3); */
  delstate[15] = (state->K[15] * inov_idx_2) + state->X[3];

  /* 'GUID_Embedded_Guid_TTKF_1D:1260' delstate(5,1) = state.X(5) + state.K(5,1)*inov(1); */
  delstate[4] = (state->K[4] * inov_idx_0) + state->X[4];

  /* 'GUID_Embedded_Guid_TTKF_1D:1261' delstate(5,2) = state.X(5) + state.K(5,2)*inov(2); */
  delstate[10] = (state->K[10] * inov_idx_1) + state->X[4];

  /* 'GUID_Embedded_Guid_TTKF_1D:1262' delstate(5,3) = state.X(5) + state.K(5,3)*inov(3); */
  delstate[16] = (state->K[16] * inov_idx_2) + state->X[4];

  /* 'GUID_Embedded_Guid_TTKF_1D:1263' delstate(6,1) = state.X(6) + state.K(6,1)*inov(1); */
  delstate[5] = (state->K[5] * inov_idx_0) + state->X[5];

  /* 'GUID_Embedded_Guid_TTKF_1D:1264' delstate(6,2) = state.X(6) + state.K(6,2)*inov(2); */
  delstate[11] = (state->K[11] * inov_idx_1) + state->X[5];

  /* 'GUID_Embedded_Guid_TTKF_1D:1265' delstate(6,3) = state.X(6) + state.K(6,3)*inov(3); */
  delstate[17] = (state->K[17] * inov_idx_2) + state->X[5];

  /* 'GUID_Embedded_Guid_TTKF_1D:1268' delposX = state.K(1,1)*inov(1) + state.K(1,2)*inov(2) + state.K(1,3)*inov(3); */
  absx11 = (absx11 + delstate_tmp) + delstate_tmp_0;

  /* 'GUID_Embedded_Guid_TTKF_1D:1269' delposY = state.K(2,1)*inov(1) + state.K(2,2)*inov(2) + state.K(2,3)*inov(3); */
  absx21 = (absx21 + delRlim) + delvelY;

  /* 'GUID_Embedded_Guid_TTKF_1D:1270' delposZ = state.K(3,1)*inov(1) + state.K(3,2)*inov(2) + state.K(3,3)*inov(3); */
  absx31 = (absx31 + delR) + Vlim;

  /* 'GUID_Embedded_Guid_TTKF_1D:1271' delR = norm([delposX delposY delposZ]); */
  absx11_0[0] = absx11;
  absx11_0[1] = absx21;
  absx11_0[2] = absx31;
  delR = norm_MfVYifVG(absx11_0);

  /* 'GUID_Embedded_Guid_TTKF_1D:1272' if (inp.ValidGNSSNav && inp.mode == def.MP_MISSIONLOAD) */
  if ((((int32_T)inp_ValidGNSSNav) != 0) && (inp_mode == b_def_MP_MISSIONLOAD))
  {
    /* 'GUID_Embedded_Guid_TTKF_1D:1273' delRlim = LIB_limit( 20.0/inp.freq,100.0,100000.0/(state.rge*inp.freq)); */
    Vlim = 20.0 / inp_freq;
    delRlim = 100000.0 / (state->rge * inp_freq);

    /* 'LIB_limit:27' if (in > max) */
    if (delRlim > 100.0) {
      /* 'LIB_limit:28' out = max; */
      delRlim = 100.0;
    } else if (delRlim < Vlim) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      delRlim = Vlim;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1274' Vlim = 10.0*LIB_limit(1.0,5.0,2000.0/state.rge); */
    Vlim = 2000.0 / state->rge;

    /* 'LIB_limit:27' if (in > max) */
    if (Vlim > 5.0) {
      /* 'LIB_limit:28' out = max; */
      Vlim = 5.0;
    } else if (Vlim < 1.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      Vlim = 1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    Vlim *= 10.0;
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:1275' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:1276' delRlim = LIB_limit(100.0/inp.freq,100.0,500000.0/(state.rge*inp.freq)); */
    Vlim = 100.0 / inp_freq;
    delRlim = 500000.0 / (state->rge * inp_freq);

    /* 'LIB_limit:27' if (in > max) */
    if (delRlim > 100.0) {
      /* 'LIB_limit:28' out = max; */
      delRlim = 100.0;
    } else if (delRlim < Vlim) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      delRlim = Vlim;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1277' Vlim = 50.0; */
    Vlim = 50.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1279' if (delR > delRlim) */
  if (delR > delRlim) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1280' U_delR = [delposX delposY delposZ]/delR; */
    /* 'GUID_Embedded_Guid_TTKF_1D:1281' delposX = delRlim*U_delR(1); */
    absx11 = (absx11 / delR) * delRlim;

    /* 'GUID_Embedded_Guid_TTKF_1D:1282' delposY = delRlim*U_delR(2); */
    absx21 = (absx21 / delR) * delRlim;

    /* 'GUID_Embedded_Guid_TTKF_1D:1283' delposZ = delRlim*U_delR(3); */
    absx31 = (absx31 / delR) * delRlim;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1285' if (inp.mode == def.MP_OPPORTUNITY) */
  if (inp_mode == b_def_MP_OPPORTUNITY) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1286' conf_fac = LIB_limit(0.0,1.0,(state.conf^2)/0.25); */
    delR = (state->conf * state->conf) / 0.25;

    /* 'LIB_limit:27' if (in > max) */
    if (delR > 1.0) {
      /* 'LIB_limit:28' out = max; */
      delR = 1.0;
    } else if (delR < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      delR = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:1287' inov(1) = inov(1)*conf_fac; */
    inov_idx_0 *= delR;

    /* 'GUID_Embedded_Guid_TTKF_1D:1288' inov(2) = inov(2)*conf_fac; */
    inov_idx_1 *= delR;

    /* 'GUID_Embedded_Guid_TTKF_1D:1289' inov(3) = inov(3)*conf_fac; */
    inov_idx_2 *= delR;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1291' delvelX = state.K(4,1)*inov(1) + state.K(4,2)*inov(2) + state.K(4,3)*inov(3); */
  delR = ((state->K[3] * inov_idx_0) + (state->K[9] * inov_idx_1)) + (state->K
    [15] * inov_idx_2);

  /* 'GUID_Embedded_Guid_TTKF_1D:1292' delvelY = state.K(5,1)*inov(1) + state.K(5,2)*inov(2) + state.K(5,3)*inov(3); */
  delvelY = ((state->K[4] * inov_idx_0) + (state->K[10] * inov_idx_1)) +
    (state->K[16] * inov_idx_2);

  /* 'GUID_Embedded_Guid_TTKF_1D:1293' delvelZ = state.K(6,1)*inov(1) + state.K(6,2)*inov(2) + state.K(6,3)*inov(3); */
  inov_idx_2 = ((state->K[5] * inov_idx_0) + (state->K[11] * inov_idx_1)) +
    (state->K[17] * inov_idx_2);

  /* 'GUID_Embedded_Guid_TTKF_1D:1294' delV = norm([delvelX delvelY delvelZ]); */
  absx11_0[0] = delR;
  absx11_0[1] = delvelY;
  absx11_0[2] = inov_idx_2;
  inov_idx_0 = norm_MfVYifVG(absx11_0);

  /* 'GUID_Embedded_Guid_TTKF_1D:1295' if (delV > 0.5*delRlim) */
  if (inov_idx_0 > (0.5 * delRlim)) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1296' U_delV = [delvelX delvelY delvelZ]/delV; */
    /* 'GUID_Embedded_Guid_TTKF_1D:1297' delvelX = 0.5*delRlim*U_delV(1); */
    delR = (0.5 * delRlim) * (delR / inov_idx_0);

    /* 'GUID_Embedded_Guid_TTKF_1D:1298' delvelY = 0.5*delRlim*U_delV(2); */
    delvelY = (0.5 * delRlim) * (delvelY / inov_idx_0);

    /* 'GUID_Embedded_Guid_TTKF_1D:1299' delvelZ = 0.5*delRlim*U_delV(3); */
    inov_idx_2 = (0.5 * delRlim) * (inov_idx_2 / inov_idx_0);
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1305' state.X(1) = state.X(1) + delposX; */
  state->X[0] += absx11;

  /* 'GUID_Embedded_Guid_TTKF_1D:1306' state.X(2) = state.X(2) + delposY; */
  state->X[1] += absx21;

  /* 'GUID_Embedded_Guid_TTKF_1D:1307' state.X(3) = state.X(3) + delposZ; */
  state->X[2] += absx31;

  /* 'GUID_Embedded_Guid_TTKF_1D:1308' state.X(4) = state.X(4) + delvelX; */
  state->X[3] += delR;

  /* 'GUID_Embedded_Guid_TTKF_1D:1309' state.X(5) = state.X(5) + delvelY; */
  state->X[4] += delvelY;

  /* 'GUID_Embedded_Guid_TTKF_1D:1310' state.X(6) = state.X(6) + delvelZ; */
  state->X[5] += inov_idx_2;

  /* 'GUID_Embedded_Guid_TTKF_1D:1311' V = norm([state.X(4) state.X(5) state.X(6)]); */
  absx11_0[0] = state->X[3];
  absx11_0[1] = state->X[4];
  absx11_0[2] = state->X[5];
  absx11 = norm_MfVYifVG(absx11_0);

  /* 'GUID_Embedded_Guid_TTKF_1D:1312' if (V > Vlim) */
  if (absx11 > Vlim) {
    /* 'GUID_Embedded_Guid_TTKF_1D:1313' U_V = [state.X(4) state.X(5) state.X(6)]/V; */
    inov_idx_1 = state->X[4] / absx11;
    inov_idx_2 = state->X[5] / absx11;

    /* 'GUID_Embedded_Guid_TTKF_1D:1314' state.X(4) = Vlim*U_V(1); */
    state->X[3] = (state->X[3] / absx11) * Vlim;

    /* 'GUID_Embedded_Guid_TTKF_1D:1315' state.X(5) = Vlim*U_V(2); */
    state->X[4] = Vlim * inov_idx_1;

    /* 'GUID_Embedded_Guid_TTKF_1D:1316' state.X(6) = Vlim*U_V(3); */
    state->X[5] = Vlim * inov_idx_2;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:1319' state.P = (eye(6) - state.K*state.H)*state.P*(eye(6) - state.K*state.H)' ... */
  /* 'GUID_Embedded_Guid_TTKF_1D:1320'             + state.K*(state.R)*state.K'; */
  for (p2 = 0; p2 < 36; p2++) {
    b_I[p2] = 0;
  }

  for (p1 = 0; p1 < 6; p1++) {
    b_I[p1 + (6 * p1)] = 1;
  }

  for (p2 = 0; p2 < 36; p2++) {
    c_I[p2] = 0;
  }

  for (p1 = 0; p1 < 6; p1++) {
    c_I[p1 + (6 * p1)] = 1;
    for (p2 = 0; p2 < 6; p2++) {
      p3 = p1 + (6 * p2);
      state_1[p3] = 0.0;
      state_1[p3] += state->H[3 * p2] * state->K[p1];
      state_1[p3] += state->H[(3 * p2) + 1] * state->K[p1 + 6];
      state_1[p3] += state->H[(3 * p2) + 2] * state->K[p1 + 12];
    }
  }

  for (p2 = 0; p2 < 36; p2++) {
    b_I_0[p2] = ((real_T)b_I[p2]) - state_1[p2];
  }

  for (p2 = 0; p2 < 6; p2++) {
    for (p3 = 0; p3 < 6; p3++) {
      itmp = p2 + (6 * p3);
      b_I_1[itmp] = 0.0;
      for (p1 = 0; p1 < 6; p1++) {
        b_I_1[itmp] += b_I_0[(6 * p1) + p2] * state->P[(6 * p3) + p1];
      }

      c_I_0[p3 + (6 * p2)] = ((real_T)c_I[itmp]) - state_1[itmp];
    }

    for (p3 = 0; p3 < 3; p3++) {
      itmp = p2 + (6 * p3);
      state_0[itmp] = 0.0;
      state_0[itmp] += state->R[3 * p3] * state->K[p2];
      state_0[itmp] += state->R[(3 * p3) + 1] * state->K[p2 + 6];
      state_0[itmp] += state->R[(3 * p3) + 2] * state->K[p2 + 12];
    }
  }

  for (p2 = 0; p2 < 6; p2++) {
    for (p3 = 0; p3 < 6; p3++) {
      itmp = p2 + (6 * p3);
      b_I_0[itmp] = 0.0;
      for (p1 = 0; p1 < 6; p1++) {
        b_I_0[itmp] += b_I_1[(6 * p1) + p2] * c_I_0[(6 * p3) + p1];
      }

      state_1[itmp] = 0.0;
      state_1[itmp] += state_0[p2] * state->K[p3];
      state_1[itmp] += state_0[p2 + 6] * state->K[p3 + 6];
      state_1[itmp] += state_0[p2 + 12] * state->K[p3 + 12];
    }
  }

  for (p2 = 0; p2 < 36; p2++) {
    state->P[p2] = b_I_0[p2] + state_1[p2];
  }
}

/* Model step function */
void TTKF_100Hz_Function_main(RT_MODEL *const TTKF_100Hz_Function_1D_M, real_T
  rtU_SysTimein, T_GUID_Discretes *rtU_Discretesin, T_Weapon_SALSeeker_IF102
  *rtU_seekerin, T_NAV_GenericInertialNavigationSensor *rtU_IMU100Hzin, uint8_T
  rtU_navmodein, T_NAV_NavigationState *rtU_navstatein, T_GUID_MissionPlanning
  *rtU_MPin, T_Weapon_MCP_DATA_TTKF *rtU_DATAttkf, T_GUID_TTKF_Out_IF1002
  *rtY_ttkfout, T_GUID_TTKF_Tel100Hz *rtY_ttkftel_100Hz)
{
  int32_T newmeas;
  uint16_T status_seeker;
  uint16_T status_ttkf;
  real_T DCMecef2ned[9];
  real_T temp_dcm_be[9];
  real_T temp_dcm_es[9];
  real_T temp_out[3];
  real_T Tgt_Alt;
  real_T Wfac_vec[9];
  real_T Wfac_den[9];
  real_T k0;
  real_T k1;
  real_T time_diff;
  uint8_T end_of_flight;
  uint8_T lock;
  int32_T seeker_request;
  int32_T seeker_standby;
  int32_T seeker_opertnl;
  real_T posDdiff0;
  real_T rgexy;
  real_T psi_rel;
  real_T ftime;
  static const real_T b[36] = { 0.00010000000000000005, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.00010000000000000005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.00010000000000000005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.5000000000000011E-5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.5000000000000011E-5, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 2.5000000000000011E-5 };

  static const int8_T c[6] = { 0, 0, 0, 1, 0, 0 };

  static const int8_T d[6] = { 0, 0, 0, 0, 1, 0 };

  static const int8_T e[6] = { 0, 0, 0, 0, 0, 1 };

  static const int8_T f[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  real_T Tgt_Alt_0[2];
  real_T temp_dcm_es_0[9];
  real_T Tgt_Alt_tmp;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_ttkf_at_outport_1' incorporates:
   *  SubSystem: '<Root>/TTKF'
   */
  /* MATLAB Function: '<S1>/Guidance Estimator' incorporates:
   *  Inport: '<Root>/DATAttkf'
   *  Inport: '<Root>/Discretesin'
   *  Inport: '<Root>/IMUin'
   *  Inport: '<Root>/MPin'
   *  Inport: '<Root>/NavMode'
   *  Inport: '<Root>/SystemTime'
   *  Inport: '<Root>/navstatein'
   *  Inport: '<Root>/seekerin'
   */
  /* :  [TTKF_out, TTKF_100Hz_tel, TTKF_ana] = GUID_Embedded_Guid_TTKF_1D(SysTime_in, Discretes_in, Skr_in, NavMode_in, NavSolution_in, ... */
  /* :                                                             Nav100Hz_in, MPin, WeaCfgin, fromDATA, ttkf_prv, def, cnsts); */
  /* 'GUID_Embedded_Guid_TTKF_1D:33' if (isempty(delt)) */
  if (!TTKF_100Hz_Function_1D_M->dwork.delt_not_empty) {
    /* 'GUID_Embedded_Guid_TTKF_1D:34' delt = ttkf_prv.delt; */
    TTKF_100Hz_Function_1D_M->dwork.delt = 0.01;
    TTKF_100Hz_Function_1D_M->dwork.delt_not_empty = true;

    /* 'GUID_Embedded_Guid_TTKF_1D:35' ftime = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:36' ftime0 = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:37' time = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:38' time_prev = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:39' lat_prev = -1.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:40' nav_time = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:56' NavAlt0 = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:61' setup.hP0 =  0.5*20.0; */
    TTKF_100Hz_Function_1D_M->dwork.setup.hP0 = 10.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:62' setup.vP0 =  0.5*10.0; */
    TTKF_100Hz_Function_1D_M->dwork.setup.vP0 = 5.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:64' setup.hV0 =  0.1*20.0; */
    TTKF_100Hz_Function_1D_M->dwork.setup.hV0 = 2.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:65' setup.vV0 =  0.1*20.0; */
    TTKF_100Hz_Function_1D_M->dwork.setup.vV0 = 2.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:66' setup.hPN =  0.2*0.05; */
    TTKF_100Hz_Function_1D_M->dwork.setup.hPN = 0.010000000000000002;

    /* 'GUID_Embedded_Guid_TTKF_1D:67' setup.vPN =  0.2*0.05; */
    TTKF_100Hz_Function_1D_M->dwork.setup.vPN = 0.010000000000000002;

    /* 'GUID_Embedded_Guid_TTKF_1D:68' setup.hVN =  0.1*0.05; */
    TTKF_100Hz_Function_1D_M->dwork.setup.hVN = 0.005000000000000001;

    /* 'GUID_Embedded_Guid_TTKF_1D:69' setup.vVN =  0.1*0.05; */
    TTKF_100Hz_Function_1D_M->dwork.setup.vVN = 0.005000000000000001;

    /* 'GUID_Embedded_Guid_TTKF_1D:70' setup.Q   = zeros(6,6); */
    /* 'GUID_Embedded_Guid_TTKF_1D:71' setup.Q = [setup.hPN^2 0.0     0.0     0.0     0.0     0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:72'                 0.0  setup.hPN^2 0.0     0.0     0.0     0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:73'                 0.0      0.0 setup.vPN^2 0.0     0.0     0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:74'                 0.0      0.0     0.0 setup.hVN^2 0.0     0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:75'                 0.0      0.0     0.0     0.0 setup.hVN^2 0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:76'                 0.0      0.0     0.0     0.0     0.0 setup.vVN^2]; */
    for (seeker_request = 0; seeker_request < 36; seeker_request++) {
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[seeker_request] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[seeker_request] = b[seeker_request];
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:78' temp_lla.lat = MP_in.PriTgtLatitude; */
    /* 'GUID_Embedded_Guid_TTKF_1D:79' temp_lla.lon = MP_in.PriTgtLongitude; */
    /* 'GUID_Embedded_Guid_TTKF_1D:80' temp_lla.alt = double(MP_in.PriTgtAltitude); */
    /* 'GUID_Embedded_Guid_TTKF_1D:81' TposECEF = LIB_lla2ecef(temp_lla); */
    LIB_lla2ecef(rtU_MPin->PriTgtLatitude, rtU_MPin->PriTgtLongitude, (real_T)
                 rtU_MPin->PriTgtAltitude, &(TTKF_100Hz_Function_1D_M)
                 ->dwork.input.trex, &(TTKF_100Hz_Function_1D_M)
                 ->dwork.input.trey, &(TTKF_100Hz_Function_1D_M)
                 ->dwork.input.trez);

    /* 'GUID_Embedded_Guid_TTKF_1D:82' input.mode  = def.MP_OPPORTUNITY; */
    TTKF_100Hz_Function_1D_M->dwork.input.mode = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:83' input.freq  = LIB_limit(1.0,100.0,double(MP_in.SAL_Frequency)); */
    /* 'LIB_limit:27' if (in > max) */
    if (rtU_MPin->SAL_Frequency > 100.0F) {
      /* 'LIB_limit:28' out = max; */
      TTKF_100Hz_Function_1D_M->dwork.input.freq = 100.0;
    } else if (rtU_MPin->SAL_Frequency < 1.0F) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      TTKF_100Hz_Function_1D_M->dwork.input.freq = 1.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      TTKF_100Hz_Function_1D_M->dwork.input.freq = (real_T)
        rtU_MPin->SAL_Frequency;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:84' input.talt0 = temp_lla.alt; */
    TTKF_100Hz_Function_1D_M->dwork.input.talt0 = (real_T)
      rtU_MPin->PriTgtAltitude;

    /* 'GUID_Embedded_Guid_TTKF_1D:85' input.trex  = TposECEF.x; */
    /* 'GUID_Embedded_Guid_TTKF_1D:86' input.trey  = TposECEF.y; */
    /* 'GUID_Embedded_Guid_TTKF_1D:87' input.trez  = TposECEF.z; */
    /* 'GUID_Embedded_Guid_TTKF_1D:88' input.tvex  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.tvex = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:89' input.tvey  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.tvey = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:90' input.tvez  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.tvez = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:91' input.malt   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.malt = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:92' input.mrex   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mrex = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:93' input.mrey   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mrey = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:94' input.mrez   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mrez = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:95' input.mvex   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mvex = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:96' input.mvey   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mvey = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:97' input.mvez   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mvez = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:98' input.mphi   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mphi = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:99' input.mthe   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mthe = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:100' input.mpsi   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.mpsi = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:101' input.dcm_eb = zeros(3,3); */
    /* 'GUID_Embedded_Guid_TTKF_1D:102' input.dcm_es = zeros(3,3); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[0], 0, 9U * (sizeof
            (real_T)));
    memset(&TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[0], 0, 9U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:103' input.epsm   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.epsm = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:104' input.etam   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.etam = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:105' input.rgem   = 1000.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.rgem = 1000.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:106' input.wbxm   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.wbxm = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:107' input.wbym   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.wbym = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:108' input.wbzm   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.input.wbzm = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:109' input.ValidGNSSNav = uint8(0); */
    TTKF_100Hz_Function_1D_M->dwork.input.ValidGNSSNav = 0U;

    /* 'GUID_Embedded_Guid_TTKF_1D:111' mre0.x = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.mre0.x = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:112' mre0.y = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.mre0.y = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:113' mre0.z = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.mre0.z = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:115' relx_prev = 0.0; */
    /* 'GUID_Embedded_Guid_TTKF_1D:116' rely_prev = 0.0; */
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:121' if ((NavMode_in >= 30 && NavMode_in ~= 39) || ~Discretes_in.umbilical) */
  if ((((int32_T)rtU_navmodein) >= 30) || (((int32_T)rtU_Discretesin->umbilical) == 0)) {
    /* 'GUID_Embedded_Guid_TTKF_1D:122' nav_time = nav_time + delt; */
    TTKF_100Hz_Function_1D_M->dwork.nav_time +=
      TTKF_100Hz_Function_1D_M->dwork.delt;
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:123' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:124' nav_time = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.nav_time = 0.0;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:126' time = time + delt; */
  TTKF_100Hz_Function_1D_M->dwork.time += TTKF_100Hz_Function_1D_M->dwork.delt;

  /* 'GUID_Embedded_Guid_TTKF_1D:128' if (NavMode_in >= 30 && (rem(NavMode_in,10)==0 || rem(NavMode_in,10)==1)) */
  if (((int32_T)rtU_navmodein) >= 30) {
    switch ((uint8_T)((int32_T)(((int32_T)rtU_navmodein) % 10))) {
     case 0:
      /* 'GUID_Embedded_Guid_TTKF_1D:129' input.ValidGNSSNav = uint8(1); */
      TTKF_100Hz_Function_1D_M->dwork.input.ValidGNSSNav = 1U;
      break;

     case 1:
      /* 'GUID_Embedded_Guid_TTKF_1D:129' input.ValidGNSSNav = uint8(1); */
      TTKF_100Hz_Function_1D_M->dwork.input.ValidGNSSNav = 1U;
      break;

     default:
      /* 'GUID_Embedded_Guid_TTKF_1D:130' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:131' input.ValidGNSSNav = uint8(0); */
      TTKF_100Hz_Function_1D_M->dwork.input.ValidGNSSNav = 0U;
      break;
    }
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:130' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:131' input.ValidGNSSNav = uint8(0); */
    TTKF_100Hz_Function_1D_M->dwork.input.ValidGNSSNav = 0U;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:135' if (nav_time < 0.1 || isempty(once)) */
  if ((TTKF_100Hz_Function_1D_M->dwork.nav_time < 0.1) ||
      (!TTKF_100Hz_Function_1D_M->dwork.once_not_empty)) {
    /* 'GUID_Embedded_Guid_TTKF_1D:137' once = 1; */
    TTKF_100Hz_Function_1D_M->dwork.once_not_empty = true;

    /* 'GUID_Embedded_Guid_TTKF_1D:138' ftime = 0.0; */
    ftime = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:140' epsm_prev = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.epsm_prev = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:141' etam_prev = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.etam_prev = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:143' temp_g.lat = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_g.lat = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:144' temp_g.lon = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_g.lon = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:145' temp_g.alt = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_g.alt = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:147' temp_q.q0 = 1.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_q.q0 = 1.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:148' temp_q.q1 = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_q.q1 = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:149' temp_q.q2 = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_q.q2 = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:150' temp_q.q3 = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.temp_q.q3 = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:163' KF_internals.A = [  1.0    0.0    0.0   delt    0.0    0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:164'                       0.0    1.0    0.0    0.0   delt    0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:165'                       0.0    0.0    1.0    0.0    0.0   delt; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:166'                       0.0    0.0    0.0    1.0    0.0    0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:167'                       0.0    0.0    0.0    0.0    1.0    0.0; ... */
    /* 'GUID_Embedded_Guid_TTKF_1D:168'                       0.0    0.0    0.0    0.0    0.0    1.0]; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[0] = 1.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[6] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[12] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[18] =
      TTKF_100Hz_Function_1D_M->dwork.delt;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[24] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[30] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[1] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[7] = 1.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[13] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[19] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[25] =
      TTKF_100Hz_Function_1D_M->dwork.delt;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[31] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[2] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[8] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[14] = 1.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[20] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[26] = 0.0;
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[32] =
      TTKF_100Hz_Function_1D_M->dwork.delt;

    /* 'GUID_Embedded_Guid_TTKF_1D:170' KF_internals.X = zeros(6,1); */
    for (newmeas = 0; newmeas < 6; newmeas++) {
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[3 + (6 * newmeas)] =
        (real_T)c[newmeas];
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[4 + (6 * newmeas)] =
        (real_T)d[newmeas];
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.A[5 + (6 * newmeas)] =
        (real_T)e[newmeas];
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[newmeas] = 0.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:171' KF_internals.H = zeros(3,6); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.H[0], 0, 18U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:172' KF_internals.P = zeros(6,6); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[0], 0, 36U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:173' KF_internals.R = zeros(3,3); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.R[0], 0, 9U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:174' KF_internals.K = zeros(6,3); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.K[0], 0, 18U * (sizeof
            (real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:176' KF_internals.rex    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rex = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:177' KF_internals.rey    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rey = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:178' KF_internals.rez    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rez = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:179' KF_internals.vex    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.vex = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:180' KF_internals.vey    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.vey = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:181' KF_internals.vez    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.vez = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:182' KF_internals.rge    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:183' KF_internals.the_b  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_b = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:184' KF_internals.psi_b  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_b = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:185' KF_internals.the_g  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_g = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:186' KF_internals.psi_g  = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_g = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:187' KF_internals.them_g = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.them_g = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:188' KF_internals.psim_g = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psim_g = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:189' KF_internals.eps    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:190' KF_internals.eta    = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:191' KF_internals.dthe_b = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dthe_b = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:192' KF_internals.dpsi_b = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dpsi_b = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:193' KF_internals.rge_clamp = 0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge_clamp = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:194' KF_internals.conf   = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.conf = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:195' KF_internals.measweight = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.measweight = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:196' KF_internals.nserge = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.nserge = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:197' KF_internals.AZ     = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:198' KF_internals.eps_median = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_median = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:199' KF_internals.the_median = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_median = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:200' KF_internals.the_lin_up = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_lin_up = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:201' KF_internals.the_lin_lo = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_lin_lo = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:202' KF_internals.eta_median = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_median = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:203' KF_internals.psi_median = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_median = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:204' KF_internals.psi_lin_up = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_lin_up = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:205' KF_internals.psi_lin_lo = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_lin_lo = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:206' KF_internals.delpsi = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.delpsi = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:207' KF_internals.delthe = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.delthe = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:209' no_samples = ceil(LIB_limit(10.0,20.0,double(MP_in.SAL_Frequency))); */
    /* 'LIB_limit:27' if (in > max) */
    if (rtU_MPin->SAL_Frequency > 20.0F) {
      /* 'LIB_limit:28' out = max; */
      k0 = 20.0;
    } else if (rtU_MPin->SAL_Frequency < 10.0F) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      k0 = 10.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      k0 = (real_T)rtU_MPin->SAL_Frequency;
    }

    k0 = ceil(k0);

    /* 'GUID_Embedded_Guid_TTKF_1D:210' KF_internals.no_samples = uint8(no_samples); */
    if (k0 < 256.0) {
      if (k0 >= 0.0) {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.no_samples = (uint8_T)k0;
      } else {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.no_samples = 0U;
      }
    } else {
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.no_samples = MAX_uint8_T;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:211' KF_internals.i_025 = uint8(ceil(0.25*no_samples)+1); */
    Tgt_Alt = ceil(0.25 * k0) + 1.0;
    if (Tgt_Alt < 256.0) {
      if (Tgt_Alt >= 0.0) {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_025 = (uint8_T)Tgt_Alt;
      } else {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_025 = 0U;
      }
    } else {
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_025 = MAX_uint8_T;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:212' KF_internals.i_075 = uint8(floor(0.75*no_samples)); */
    Tgt_Alt = floor(0.75 * k0);
    if (Tgt_Alt < 256.0) {
      if (Tgt_Alt >= 0.0) {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_075 = (uint8_T)Tgt_Alt;
      } else {
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_075 = 0U;
      }
    } else {
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.i_075 = MAX_uint8_T;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:213' KF_internals.k = uint8(1); */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.k = 1U;

    /* 'GUID_Embedded_Guid_TTKF_1D:214' KF_internals.eps_diff = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_diff = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:215' KF_internals.eta_diff = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_diff = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:216' KF_internals.eps_sample = zeros(20,1); */
    /* 'GUID_Embedded_Guid_TTKF_1D:217' KF_internals.eta_sample = zeros(20,1); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_sample[0], 0, 20U *
           (sizeof(real_T)));
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_sample[0], 0, 20U *
           (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:218' KF_internals.t_fillsample = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.t_fillsample = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:220' KF_internals.i = 0; */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.i = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:221' KF_internals.time_vec = zeros(10,1); */
    /* 'GUID_Embedded_Guid_TTKF_1D:222' KF_internals.tposn_vec = zeros(10,1); */
    /* 'GUID_Embedded_Guid_TTKF_1D:223' KF_internals.tpose_vec = zeros(10,1); */
    /* 'GUID_Embedded_Guid_TTKF_1D:224' KF_internals.Wfac = zeros(10,1); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.time_vec[0], 0, 10U *
           (sizeof(real_T)));
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.tposn_vec[0], 0, 10U *
           (sizeof(real_T)));
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.tpose_vec[0], 0, 10U *
           (sizeof(real_T)));
    memset(&TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[0], 0, 10U *
           (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:226' delX = zeros(6,3); */
    memset(&TTKF_100Hz_Function_1D_M->dwork.delX[0], 0, 18U * (sizeof(real_T)));

    /* 'GUID_Embedded_Guid_TTKF_1D:229' t_no_lock = 1000.0; */
    TTKF_100Hz_Function_1D_M->dwork.t_no_lock = 1000.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:230' t_no_input = 1000.0; */
    TTKF_100Hz_Function_1D_M->dwork.t_no_input = 1000.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:231' t_clamp = 0.0; */
    TTKF_100Hz_Function_1D_M->dwork.t_clamp = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:232' newmeas = uint32(0); */
    newmeas = 0;

    /* 'GUID_Embedded_Guid_TTKF_1D:233' status_seeker = uint16(0); */
    status_seeker = 0U;

    /* 'GUID_Embedded_Guid_TTKF_1D:234' status_ttkf = uint16(0); */
    status_ttkf = 0U;

    /* 'GUID_Embedded_Guid_TTKF_1D:235' NavAlt0 = input.malt; */
    TTKF_100Hz_Function_1D_M->dwork.NavAlt0 =
      TTKF_100Hz_Function_1D_M->dwork.input.malt;

    /* 'GUID_Embedded_Guid_TTKF_1D:237' eps_median = 0.0; */
    k0 = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:238' eta_median = 0.0; */
    k1 = 0.0;

    /* 'GUID_Embedded_Guid_TTKF_1D:240' DCMecef2ned = [1.0 0.0 0.0; 0.0 1.0 0.0; 0.0 0.0 1.0]; */
    for (seeker_request = 0; seeker_request < 9; seeker_request++) {
      DCMecef2ned[seeker_request] = (real_T)f[seeker_request];
    }
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:242' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:244' if (MP_in.PriTgtLatitude == 0.0 || MP_in.PriTgtLongitude == 0.0) */
    if ((rtU_MPin->PriTgtLatitude == 0.0) || (rtU_MPin->PriTgtLongitude == 0.0))
    {
      /* 'GUID_Embedded_Guid_TTKF_1D:245' input.mode = def.MP_OPPORTUNITY; */
      TTKF_100Hz_Function_1D_M->dwork.input.mode = 0.0;
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:246' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:247' input.mode = def.MP_MISSIONLOAD; */
      TTKF_100Hz_Function_1D_M->dwork.input.mode = 1.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:249' input.malt = LIB_checkNAN(NavSolution_in.pos_g.alt ,input.malt,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.alt)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.malt = rtU_navstatein->pos_g.alt;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:250' input.mrex = LIB_checkNAN(NavSolution_in.pos_e.x   ,input.mrex,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.x)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mrex = rtU_navstatein->pos_e.x;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:251' input.mrey = LIB_checkNAN(NavSolution_in.pos_e.y   ,input.mrey,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.y)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mrey = rtU_navstatein->pos_e.y;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:252' input.mrez = LIB_checkNAN(NavSolution_in.pos_e.z   ,input.mrez,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_e.z)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mrez = rtU_navstatein->pos_e.z;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:253' input.mvex = LIB_checkNAN(NavSolution_in.vel_e.x   ,input.mvex,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.x)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mvex = rtU_navstatein->vel_e.x;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:254' input.mvey = LIB_checkNAN(NavSolution_in.vel_e.y   ,input.mvey,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.y)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mvey = rtU_navstatein->vel_e.y;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:255' input.mvez = LIB_checkNAN(NavSolution_in.vel_e.z   ,input.mvez,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->vel_e.z)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mvez = rtU_navstatein->vel_e.z;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:256' input.mphi = LIB_checkNAN(NavSolution_in.att_bn.phi,input.mphi,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.phi)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mphi = rtU_navstatein->att_bn.phi;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:257' input.mthe = LIB_checkNAN(NavSolution_in.att_bn.the,input.mthe,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.the)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mthe = rtU_navstatein->att_bn.the;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:258' input.mpsi = LIB_checkNAN(NavSolution_in.att_bn.psi,input.mpsi,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->att_bn.psi)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.mpsi = rtU_navstatein->att_bn.psi;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:259' temp_q.q0  = LIB_checkNAN(NavSolution_in.q_be.q0,temp_q.q0,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->q_be.q0)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_q.q0 = rtU_navstatein->q_be.q0;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:260' temp_q.q1  = LIB_checkNAN(NavSolution_in.q_be.q1,temp_q.q1,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->q_be.q1)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_q.q1 = rtU_navstatein->q_be.q1;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:261' temp_q.q2  = LIB_checkNAN(NavSolution_in.q_be.q2,temp_q.q2,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->q_be.q2)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_q.q2 = rtU_navstatein->q_be.q2;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:262' temp_q.q3  = LIB_checkNAN(NavSolution_in.q_be.q3,temp_q.q3,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->q_be.q3)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_q.q3 = rtU_navstatein->q_be.q3;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:263' temp_dcm_be = LIB_q2d(temp_q); */
    LIB_q2d((TTKF_100Hz_Function_1D_M)->dwork.temp_q.q0,
            (TTKF_100Hz_Function_1D_M)->dwork.temp_q.q1,
            (TTKF_100Hz_Function_1D_M)->dwork.temp_q.q2,
            (TTKF_100Hz_Function_1D_M)->dwork.temp_q.q3, temp_dcm_be);

    /* 'GUID_Embedded_Guid_TTKF_1D:271' temp_dcm_es = temp_dcm_be'; */
    for (seeker_request = 0; seeker_request < 3; seeker_request++) {
      temp_dcm_es[3 * seeker_request] = temp_dcm_be[seeker_request];
      temp_dcm_es[1 + (3 * seeker_request)] = temp_dcm_be[seeker_request + 3];
      temp_dcm_es[2 + (3 * seeker_request)] = temp_dcm_be[seeker_request + 6];
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:272' input.dcm_eb(1,1) = temp_dcm_be(1,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[0] = temp_dcm_be[0];

    /* 'GUID_Embedded_Guid_TTKF_1D:273' input.dcm_eb(1,2) = temp_dcm_be(2,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[3] = temp_dcm_be[1];

    /* 'GUID_Embedded_Guid_TTKF_1D:274' input.dcm_eb(1,3) = temp_dcm_be(3,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[6] = temp_dcm_be[2];

    /* 'GUID_Embedded_Guid_TTKF_1D:275' input.dcm_eb(2,1) = temp_dcm_be(1,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[1] = temp_dcm_be[3];

    /* 'GUID_Embedded_Guid_TTKF_1D:276' input.dcm_eb(2,2) = temp_dcm_be(2,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[4] = temp_dcm_be[4];

    /* 'GUID_Embedded_Guid_TTKF_1D:277' input.dcm_eb(2,3) = temp_dcm_be(3,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[7] = temp_dcm_be[5];

    /* 'GUID_Embedded_Guid_TTKF_1D:278' input.dcm_eb(3,1) = temp_dcm_be(1,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[2] = temp_dcm_be[6];

    /* 'GUID_Embedded_Guid_TTKF_1D:279' input.dcm_eb(3,2) = temp_dcm_be(2,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[5] = temp_dcm_be[7];

    /* 'GUID_Embedded_Guid_TTKF_1D:280' input.dcm_eb(3,3) = temp_dcm_be(3,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_eb[8] = temp_dcm_be[8];

    /* 'GUID_Embedded_Guid_TTKF_1D:281' input.dcm_es(1,1) = temp_dcm_es(1,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[0] = temp_dcm_es[0];

    /* 'GUID_Embedded_Guid_TTKF_1D:282' input.dcm_es(1,2) = temp_dcm_es(1,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[3] = temp_dcm_es[3];

    /* 'GUID_Embedded_Guid_TTKF_1D:283' input.dcm_es(1,3) = temp_dcm_es(1,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[6] = temp_dcm_es[6];

    /* 'GUID_Embedded_Guid_TTKF_1D:284' input.dcm_es(2,1) = temp_dcm_es(2,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[1] = temp_dcm_es[1];

    /* 'GUID_Embedded_Guid_TTKF_1D:285' input.dcm_es(2,2) = temp_dcm_es(2,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[4] = temp_dcm_es[4];

    /* 'GUID_Embedded_Guid_TTKF_1D:286' input.dcm_es(2,3) = temp_dcm_es(2,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[7] = temp_dcm_es[7];

    /* 'GUID_Embedded_Guid_TTKF_1D:287' input.dcm_es(3,1) = temp_dcm_es(3,1); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[2] = temp_dcm_es[2];

    /* 'GUID_Embedded_Guid_TTKF_1D:288' input.dcm_es(3,2) = temp_dcm_es(3,2); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[5] = temp_dcm_es[5];

    /* 'GUID_Embedded_Guid_TTKF_1D:289' input.dcm_es(3,3) = temp_dcm_es(3,3); */
    TTKF_100Hz_Function_1D_M->dwork.input.dcm_es[8] = temp_dcm_es[8];

    /* 'GUID_Embedded_Guid_TTKF_1D:290' input.epsm        = LIB_checkNAN(Skr_in.TargetPitch,input.epsm,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_seekerin->TargetPitch)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.epsm = rtU_seekerin->TargetPitch;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:291' input.etam        = LIB_checkNAN(Skr_in.TargetYaw  ,input.etam,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_seekerin->TargetYaw)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.etam = rtU_seekerin->TargetYaw;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:293' input.wbxm        = LIB_checkNAN(Nav100Hz_in.dangle.x/delt,input.wbxm,1,0); */
    ftime = rtU_IMU100Hzin->dangle.x / TTKF_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.wbxm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:294' input.wbym        = LIB_checkNAN(Nav100Hz_in.dangle.y/delt,input.wbym,1,0); */
    ftime = rtU_IMU100Hzin->dangle.y / TTKF_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.wbym = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:295' input.wbzm        = LIB_checkNAN(Nav100Hz_in.dangle.z/delt,input.wbzm,1,0); */
    ftime = rtU_IMU100Hzin->dangle.z / TTKF_100Hz_Function_1D_M->dwork.delt;

    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(ftime)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.input.wbzm = ftime;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:298' temp_g.lat = LIB_checkNAN(NavSolution_in.pos_g.lat ,temp_g.lat,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.lat)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_g.lat = rtU_navstatein->pos_g.lat;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:299' temp_g.lon = LIB_checkNAN(NavSolution_in.pos_g.lon ,temp_g.lon,1,0); */
    /* 'LIB_checkNAN:4' if (isnan(in)) */
    if (!rtIsNaN(rtU_navstatein->pos_g.lon)) {
      /* 'LIB_checkNAN:13' else */
      /* 'LIB_checkNAN:14' state = in; */
      /* 'LIB_checkNAN:15' out = state; */
      TTKF_100Hz_Function_1D_M->dwork.temp_g.lon = rtU_navstatein->pos_g.lon;
    } else {
      /* 'LIB_checkNAN:5' if (typehold == 0) */
      /* 'LIB_checkNAN:10' else */
      /* 'LIB_checkNAN:11' out = state; */
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:300' temp_g.alt = input.malt; */
    TTKF_100Hz_Function_1D_M->dwork.temp_g.alt =
      TTKF_100Hz_Function_1D_M->dwork.input.malt;

    /* 'GUID_Embedded_Guid_TTKF_1D:301' DCMecef2ned = LIB_dcm_ecef2ned(temp_g); */
    /* 'LIB_dcm_ecef2ned:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
    /* 'LIB_dcm_ecef2ned:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
    /* 'LIB_dcm_ecef2ned:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
    ftime = -sin(TTKF_100Hz_Function_1D_M->dwork.temp_g.lat);
    k0 = cos(TTKF_100Hz_Function_1D_M->dwork.temp_g.lon);
    DCMecef2ned[0] = ftime * k0;
    k1 = sin(TTKF_100Hz_Function_1D_M->dwork.temp_g.lon);
    DCMecef2ned[3] = ftime * k1;
    time_diff = cos(TTKF_100Hz_Function_1D_M->dwork.temp_g.lat);
    DCMecef2ned[6] = time_diff;
    DCMecef2ned[1] = -k1;
    DCMecef2ned[4] = k0;
    DCMecef2ned[7] = 0.0;
    DCMecef2ned[2] = (-time_diff) * k0;
    DCMecef2ned[5] = (-cos(TTKF_100Hz_Function_1D_M->dwork.temp_g.lat)) * k1;
    DCMecef2ned[8] = ftime;

    /* 'GUID_Embedded_Guid_TTKF_1D:303' if (Discretes_in.umbilical) */
    if (((int32_T)rtU_Discretesin->umbilical) != 0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:307' if (input.mode == def.MP_OPPORTUNITY) */
      if (TTKF_100Hz_Function_1D_M->dwork.input.mode == 0.0) {
        /* 'GUID_Embedded_Guid_TTKF_1D:309' setup.hP0 = double(fromDATA.setup.Opp.horPos0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hP0 = (real_T)
          rtU_DATAttkf->setup.Opp.horPos0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:310' if (MP_in.PriTgtAltitude == 0.0) */
        if (rtU_MPin->PriTgtAltitude == 0.0F) {
          /* 'GUID_Embedded_Guid_TTKF_1D:311' setup.vP0 =  double(fromDATA.setup.Opp.verPos0(1)); */
          TTKF_100Hz_Function_1D_M->dwork.setup.vP0 = (real_T)
            rtU_DATAttkf->setup.Opp.verPos0[0];
        } else {
          /* 'GUID_Embedded_Guid_TTKF_1D:312' else */
          /* 'GUID_Embedded_Guid_TTKF_1D:313' setup.vP0 =  double(fromDATA.setup.Opp.verPos0(2)); */
          TTKF_100Hz_Function_1D_M->dwork.setup.vP0 = (real_T)
            rtU_DATAttkf->setup.Opp.verPos0[1];
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:315' setup.hV0 =  double(fromDATA.setup.Opp.horVel0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hV0 = (real_T)
          rtU_DATAttkf->setup.Opp.horVel0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:316' setup.vV0 =  double(fromDATA.setup.Opp.verVel0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vV0 = (real_T)
          rtU_DATAttkf->setup.Opp.verVel0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:317' setup.hPN =  double(fromDATA.setup.Opp.horPosN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN = (real_T)
          rtU_DATAttkf->setup.Opp.horPosN;

        /* 'GUID_Embedded_Guid_TTKF_1D:318' setup.vPN =  double(fromDATA.setup.Opp.verPosN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vPN = (real_T)
          rtU_DATAttkf->setup.Opp.verPosN;

        /* 'GUID_Embedded_Guid_TTKF_1D:319' setup.hVN =  double(fromDATA.setup.Opp.horVelN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN = (real_T)
          rtU_DATAttkf->setup.Opp.horVelN;

        /* 'GUID_Embedded_Guid_TTKF_1D:320' setup.vVN =  double(fromDATA.setup.Opp.verVelN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vVN = (real_T)
          rtU_DATAttkf->setup.Opp.verVelN;
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:321' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:323' setup.hP0 =  double(fromDATA.setup.MP.horPos0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hP0 = (real_T)
          rtU_DATAttkf->setup.MP.horPos0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:324' setup.vP0 =  double(fromDATA.setup.MP.verPos0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vP0 = (real_T)
          rtU_DATAttkf->setup.MP.verPos0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:325' setup.hV0 =  double(fromDATA.setup.MP.horVel0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hV0 = (real_T)
          rtU_DATAttkf->setup.MP.horVel0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:326' setup.vV0 =  double(fromDATA.setup.MP.verVel0(1)); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vV0 = (real_T)
          rtU_DATAttkf->setup.MP.verVel0[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:327' setup.hPN =  double(fromDATA.setup.MP.horPosN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN = (real_T)
          rtU_DATAttkf->setup.MP.horPosN;

        /* 'GUID_Embedded_Guid_TTKF_1D:328' setup.vPN =  double(fromDATA.setup.MP.verPosN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vPN = (real_T)
          rtU_DATAttkf->setup.MP.verPosN;

        /* 'GUID_Embedded_Guid_TTKF_1D:329' setup.hVN =  double(fromDATA.setup.MP.horVelN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN = (real_T)
          rtU_DATAttkf->setup.MP.horVelN;

        /* 'GUID_Embedded_Guid_TTKF_1D:330' setup.vVN =  double(fromDATA.setup.MP.verVelN); */
        TTKF_100Hz_Function_1D_M->dwork.setup.vVN = (real_T)
          rtU_DATAttkf->setup.MP.verVelN;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:333' setup.Q   = zeros(6,6); */
      memset(&TTKF_100Hz_Function_1D_M->dwork.setup.Q[0], 0, 36U * (sizeof
              (real_T)));

      /* 'GUID_Embedded_Guid_TTKF_1D:334' setup.Q = [setup.hPN^2 0.0     0.0     0.0     0.0     0.0; ... */
      /* 'GUID_Embedded_Guid_TTKF_1D:335'                   0.0  setup.hPN^2 0.0     0.0     0.0     0.0; ... */
      /* 'GUID_Embedded_Guid_TTKF_1D:336'                   0.0      0.0 setup.vPN^2 0.0     0.0     0.0; ... */
      /* 'GUID_Embedded_Guid_TTKF_1D:337'                   0.0      0.0     0.0 setup.hVN^2 0.0     0.0; ... */
      /* 'GUID_Embedded_Guid_TTKF_1D:338'                   0.0      0.0     0.0     0.0 setup.hVN^2 0.0; ... */
      /* 'GUID_Embedded_Guid_TTKF_1D:339'                   0.0      0.0     0.0     0.0     0.0 setup.vVN^2]; */
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[0] =
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN *
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[6] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[12] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[18] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[24] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[30] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[1] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[7] =
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN *
        TTKF_100Hz_Function_1D_M->dwork.setup.hPN;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[13] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[19] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[25] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[31] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[2] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[8] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[14] =
        TTKF_100Hz_Function_1D_M->dwork.setup.vPN *
        TTKF_100Hz_Function_1D_M->dwork.setup.vPN;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[20] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[26] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[32] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[3] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[9] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[15] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[21] =
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN *
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[27] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[33] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[4] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[10] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[16] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[22] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[28] =
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN *
        TTKF_100Hz_Function_1D_M->dwork.setup.hVN;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[34] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[5] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[11] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[17] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[23] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[29] = 0.0;
      TTKF_100Hz_Function_1D_M->dwork.setup.Q[35] =
        TTKF_100Hz_Function_1D_M->dwork.setup.vVN *
        TTKF_100Hz_Function_1D_M->dwork.setup.vVN;

      /* 'GUID_Embedded_Guid_TTKF_1D:341' ftime = 0.0; */
      ftime = 0.0;

      /* 'GUID_Embedded_Guid_TTKF_1D:342' ftime0 = LIB_checkNAN(SysTime_in,ftime0,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(rtU_SysTimein)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        TTKF_100Hz_Function_1D_M->dwork.ftime0 = rtU_SysTimein;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:344' t_clamp = 0.0; */
      TTKF_100Hz_Function_1D_M->dwork.t_clamp = 0.0;

      /* 'GUID_Embedded_Guid_TTKF_1D:347' MposNED.x = 0.0; */
      /* 'GUID_Embedded_Guid_TTKF_1D:348' MposNED.y = 0.0; */
      /* 'GUID_Embedded_Guid_TTKF_1D:349' MposNED.z = 0.0; */
      /* 'GUID_Embedded_Guid_TTKF_1D:350' if (input.mode == def.MP_OPPORTUNITY) */
      if (TTKF_100Hz_Function_1D_M->dwork.input.mode == 0.0) {
        /* 'GUID_Embedded_Guid_TTKF_1D:353' if (MP_in.PriTgtAltitude == 0.0) */
        if (rtU_MPin->PriTgtAltitude == 0.0F) {
          /* 'GUID_Embedded_Guid_TTKF_1D:358' delAlt = 1524.0; */
          Tgt_Alt = 1524.0;
        } else {
          /* 'GUID_Embedded_Guid_TTKF_1D:359' else */
          /* 'GUID_Embedded_Guid_TTKF_1D:361' delAlt = input.malt - double(MP_in.PriTgtAltitude); */
          Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.input.malt - ((real_T)
            rtU_MPin->PriTgtAltitude);
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:363' Vm = sqrt(input.mvex^2 + ... */
        /* 'GUID_Embedded_Guid_TTKF_1D:364'                 input.mvey^2 + ... */
        /* 'GUID_Embedded_Guid_TTKF_1D:365'                 input.mvez^2); */
        /* 'GUID_Embedded_Guid_TTKF_1D:366' if (Vm < 20.0) */
        if (sqrt(((TTKF_100Hz_Function_1D_M->dwork.input.mvex *
                   TTKF_100Hz_Function_1D_M->dwork.input.mvex) +
                  (TTKF_100Hz_Function_1D_M->dwork.input.mvey *
                   TTKF_100Hz_Function_1D_M->dwork.input.mvey)) +
                 (TTKF_100Hz_Function_1D_M->dwork.input.mvez *
                  TTKF_100Hz_Function_1D_M->dwork.input.mvez)) < 20.0) {
          /* 'GUID_Embedded_Guid_TTKF_1D:367' NavAlt0 = input.malt; */
          TTKF_100Hz_Function_1D_M->dwork.NavAlt0 =
            TTKF_100Hz_Function_1D_M->dwork.input.malt;
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:369' Mach = Vm/341; */
        /* 'GUID_Embedded_Guid_TTKF_1D:371' GndRge = double(fromDATA.BallisticRange); */
        /* 'GUID_Embedded_Guid_TTKF_1D:372' TposNED.x = MposNED.x + GndRge*cos(input.mpsi); */
        /* 'GUID_Embedded_Guid_TTKF_1D:373' TposNED.y = MposNED.y + GndRge*sin(input.mpsi); */
        /* 'GUID_Embedded_Guid_TTKF_1D:374' TposNED.z = MposNED.z + delAlt; */
        /* 'GUID_Embedded_Guid_TTKF_1D:375' temp_out = (DCMecef2ned')*[TposNED.x TposNED.y TposNED.z]'; */
        rgexy = ((real_T)rtU_DATAttkf->BallisticRange) * cos
          (TTKF_100Hz_Function_1D_M->dwork.input.mpsi);
        posDdiff0 = ((real_T)rtU_DATAttkf->BallisticRange) * sin
          (TTKF_100Hz_Function_1D_M->dwork.input.mpsi);
        for (seeker_request = 0; seeker_request < 3; seeker_request++) {
          temp_out[seeker_request] = (DCMecef2ned[(3 * seeker_request) + 2] *
            Tgt_Alt) + ((DCMecef2ned[(3 * seeker_request) + 1] * posDdiff0) +
                        (DCMecef2ned[3 * seeker_request] * rgexy));
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:376' TposECEF.x = temp_out(1) + input.mrex; */
        k0 = temp_out[0] + TTKF_100Hz_Function_1D_M->dwork.input.mrex;

        /* 'GUID_Embedded_Guid_TTKF_1D:377' TposECEF.y = temp_out(2) + input.mrey; */
        k1 = temp_out[1] + TTKF_100Hz_Function_1D_M->dwork.input.mrey;

        /* 'GUID_Embedded_Guid_TTKF_1D:378' TposECEF.z = temp_out(3) + input.mrez; */
        time_diff = temp_out[2] + TTKF_100Hz_Function_1D_M->dwork.input.mrez;

        /* 'GUID_Embedded_Guid_TTKF_1D:379' Tgt_Alt = input.malt - delAlt; */
        Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.input.malt - Tgt_Alt;
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:383' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:385' temp_lla.lat = MP_in.PriTgtLatitude; */
        /* 'GUID_Embedded_Guid_TTKF_1D:386' temp_lla.lon = MP_in.PriTgtLongitude; */
        /* 'GUID_Embedded_Guid_TTKF_1D:387' temp_lla.alt = double(MP_in.PriTgtAltitude); */
        /* 'GUID_Embedded_Guid_TTKF_1D:390' TposECEF = LIB_lla2ecef(temp_lla); */
        LIB_lla2ecef(rtU_MPin->PriTgtLatitude, rtU_MPin->PriTgtLongitude,
                     (real_T)rtU_MPin->PriTgtAltitude, &k0, &k1, &time_diff);

        /* 'GUID_Embedded_Guid_TTKF_1D:391' Tgt_Alt = temp_lla.alt; */
        Tgt_Alt = (real_T)rtU_MPin->PriTgtAltitude;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:397' if (time > 10.0) */
      /* 'GUID_Embedded_Guid_TTKF_1D:401' input.freq  = LIB_limit(1.0,100.0,double(MP_in.SAL_Frequency)); */
      /* 'LIB_limit:27' if (in > max) */
      if (rtU_MPin->SAL_Frequency > 100.0F) {
        /* 'LIB_limit:28' out = max; */
        TTKF_100Hz_Function_1D_M->dwork.input.freq = 100.0;
      } else if (rtU_MPin->SAL_Frequency < 1.0F) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TTKF_100Hz_Function_1D_M->dwork.input.freq = 1.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TTKF_100Hz_Function_1D_M->dwork.input.freq = (real_T)
          rtU_MPin->SAL_Frequency;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:402' input.talt0 = LIB_checkNAN(Tgt_Alt   ,input.talt0,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(Tgt_Alt)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        TTKF_100Hz_Function_1D_M->dwork.input.talt0 = Tgt_Alt;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:403' input.trex  = LIB_checkNAN(TposECEF.x,input.trex ,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(k0)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        TTKF_100Hz_Function_1D_M->dwork.input.trex = k0;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:404' input.trey  = LIB_checkNAN(TposECEF.y,input.trey ,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(k1)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        TTKF_100Hz_Function_1D_M->dwork.input.trey = k1;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:405' input.trez  = LIB_checkNAN(TposECEF.z,input.trez ,1,0); */
      /* 'LIB_checkNAN:4' if (isnan(in)) */
      if (!rtIsNaN(time_diff)) {
        /* 'LIB_checkNAN:13' else */
        /* 'LIB_checkNAN:14' state = in; */
        /* 'LIB_checkNAN:15' out = state; */
        TTKF_100Hz_Function_1D_M->dwork.input.trez = time_diff;
      } else {
        /* 'LIB_checkNAN:5' if (typehold == 0) */
        /* 'LIB_checkNAN:10' else */
        /* 'LIB_checkNAN:11' out = state; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:406' if (t_no_input >= 1.0) */
      if (TTKF_100Hz_Function_1D_M->dwork.t_no_input >= 1.0) {
        /* 'GUID_Embedded_Guid_TTKF_1D:407' mre0.x = input.mrex; */
        TTKF_100Hz_Function_1D_M->dwork.mre0.x =
          TTKF_100Hz_Function_1D_M->dwork.input.mrex;

        /* 'GUID_Embedded_Guid_TTKF_1D:408' mre0.y = input.mrey; */
        TTKF_100Hz_Function_1D_M->dwork.mre0.y =
          TTKF_100Hz_Function_1D_M->dwork.input.mrey;

        /* 'GUID_Embedded_Guid_TTKF_1D:409' mre0.z = input.mrez; */
        TTKF_100Hz_Function_1D_M->dwork.mre0.z =
          TTKF_100Hz_Function_1D_M->dwork.input.mrez;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:411' if (MP_in.PriTgtLatitude*MP_in.PriTgtLongitude ~= lat_prev) */
      Tgt_Alt = rtU_MPin->PriTgtLatitude * rtU_MPin->PriTgtLongitude;
      if (Tgt_Alt != TTKF_100Hz_Function_1D_M->dwork.lat_prev) {
        /* 'GUID_Embedded_Guid_TTKF_1D:412' KF_internals.i = KF_internals.i + 1; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i++;

        /* 'GUID_Embedded_Guid_TTKF_1D:413' if (KF_internals.i > 10) */
        if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.i > 10.0) {
          /* 'GUID_Embedded_Guid_TTKF_1D:414' KF_internals.i = 1; */
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.i = 1.0;
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:416' t_no_input = 0.0; */
        TTKF_100Hz_Function_1D_M->dwork.t_no_input = 0.0;

        /* 'GUID_Embedded_Guid_TTKF_1D:417' KF_internals.Wfac(KF_internals.i) = 1.0; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[((int32_T)
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.i) - 1] = 1.0;

        /* 'GUID_Embedded_Guid_TTKF_1D:418' Tpos_NED = DCMecef2ned*([input.trex-mre0.x input.trey-mre0.y input.trez-mre0.z]'); */
        rgexy = TTKF_100Hz_Function_1D_M->dwork.input.trex -
          TTKF_100Hz_Function_1D_M->dwork.mre0.x;
        posDdiff0 = TTKF_100Hz_Function_1D_M->dwork.input.trey -
          TTKF_100Hz_Function_1D_M->dwork.mre0.y;
        k0 = TTKF_100Hz_Function_1D_M->dwork.input.trez -
          TTKF_100Hz_Function_1D_M->dwork.mre0.z;
        for (seeker_request = 0; seeker_request < 3; seeker_request++) {
          temp_out[seeker_request] = (DCMecef2ned[seeker_request + 6] * k0) +
            ((DCMecef2ned[seeker_request + 3] * posDdiff0) +
             (DCMecef2ned[seeker_request] * rgexy));
        }

        /* 'GUID_Embedded_Guid_TTKF_1D:419' KF_internals.time_vec(KF_internals.i) = time; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.time_vec[((int32_T)
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.i) - 1] =
          TTKF_100Hz_Function_1D_M->dwork.time;

        /* 'GUID_Embedded_Guid_TTKF_1D:420' KF_internals.tposn_vec(KF_internals.i) = Tpos_NED(1); */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.tposn_vec[((int32_T)
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.i) - 1] = temp_out[0];

        /* 'GUID_Embedded_Guid_TTKF_1D:421' KF_internals.tpose_vec(KF_internals.i) = Tpos_NED(2); */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.tpose_vec[((int32_T)
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.i) - 1] = temp_out[1];
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:422' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:423' t_no_input = t_no_input + delt; */
        TTKF_100Hz_Function_1D_M->dwork.t_no_input +=
          TTKF_100Hz_Function_1D_M->dwork.delt;

        /* 'GUID_Embedded_Guid_TTKF_1D:424' for j = 1:10 */
        for (newmeas = 0; newmeas < 10; newmeas++) {
          /* 'GUID_Embedded_Guid_TTKF_1D:425' KF_internals.Wfac(j) = KF_internals.Wfac(j)-0.5*delt; */
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[newmeas] -= 0.5 *
            TTKF_100Hz_Function_1D_M->dwork.delt;

          /* 'GUID_Embedded_Guid_TTKF_1D:426' if (KF_internals.Wfac(j) < 0.0) */
          if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[newmeas] < 0.0)
          {
            /* 'GUID_Embedded_Guid_TTKF_1D:427' KF_internals.Wfac(j) = 0.0; */
            TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[newmeas] = 0.0;
          }
        }
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:431' lat_prev = MP_in.PriTgtLatitude*MP_in.PriTgtLongitude; */
      TTKF_100Hz_Function_1D_M->dwork.lat_prev = Tgt_Alt;

      /* 'GUID_Embedded_Guid_TTKF_1D:432' veln_vec = zeros(9,1); */
      /* 'GUID_Embedded_Guid_TTKF_1D:433' vele_vec = zeros(9,1); */
      /* 'GUID_Embedded_Guid_TTKF_1D:434' Wfac_vec = zeros(9,1); */
      /* 'GUID_Embedded_Guid_TTKF_1D:435' Wfac_den = ones(9,1); */
      for (newmeas = 0; newmeas < 9; newmeas++) {
        temp_dcm_be[newmeas] = 0.0;
        temp_dcm_es[newmeas] = 0.0;
        Wfac_vec[newmeas] = 0.0;
        Wfac_den[newmeas] = 1.0;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:436' if (t_no_input < 1.0) */
      if (TTKF_100Hz_Function_1D_M->dwork.t_no_input < 1.0) {
        /* 'GUID_Embedded_Guid_TTKF_1D:437' if (KF_internals.i == 1 && KF_internals.tposn_vec(10) == 0.0) */
        if ((TTKF_100Hz_Function_1D_M->dwork.KF_internals.i == 1.0) &&
            (TTKF_100Hz_Function_1D_M->dwork.KF_internals.tposn_vec[9] == 0.0))
        {
          /* 'GUID_Embedded_Guid_TTKF_1D:438' KF_internals.Wfac(1) = 0.0; */
          TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[0] = 0.0;
        } else {
          /* 'GUID_Embedded_Guid_TTKF_1D:439' else */
          /* 'GUID_Embedded_Guid_TTKF_1D:440' for j = 1:9 */
          for (newmeas = 0; newmeas < 9; newmeas++) {
            /* 'GUID_Embedded_Guid_TTKF_1D:441' k0 = rem(11 + KF_internals.i - j , 10); */
            k0 = rt_remd_snf((11.0 + (TTKF_100Hz_Function_1D_M)
                              ->dwork.KF_internals.i) - (1.0 + ((real_T)newmeas)),
                             10.0);

            /* 'GUID_Embedded_Guid_TTKF_1D:442' if (k0==0) */
            if (k0 == 0.0) {
              /* 'GUID_Embedded_Guid_TTKF_1D:443' k0 = 10; */
              k0 = 10.0;
            }

            /* 'GUID_Embedded_Guid_TTKF_1D:445' k1 = k0 - 1; */
            k1 = k0 - 1.0;

            /* 'GUID_Embedded_Guid_TTKF_1D:446' if (k1==0) */
            if ((k0 - 1.0) == 0.0) {
              /* 'GUID_Embedded_Guid_TTKF_1D:447' k1 = 10; */
              k1 = 10.0;
            }

            /* 'GUID_Embedded_Guid_TTKF_1D:449' time_diff = KF_internals.time_vec(k0) -KF_internals.time_vec(k1); */
            seeker_request = ((int32_T)k0) - 1;
            seeker_standby = ((int32_T)k1) - 1;
            time_diff = TTKF_100Hz_Function_1D_M->
              dwork.KF_internals.time_vec[seeker_request] -
              TTKF_100Hz_Function_1D_M->
              dwork.KF_internals.time_vec[seeker_standby];

            /* 'GUID_Embedded_Guid_TTKF_1D:450' if (time_diff < delt) */
            if (time_diff < TTKF_100Hz_Function_1D_M->dwork.delt) {
              /* 'GUID_Embedded_Guid_TTKF_1D:451' time_diff = delt; */
              time_diff = TTKF_100Hz_Function_1D_M->dwork.delt;
            }

            /* 'GUID_Embedded_Guid_TTKF_1D:453' veln_vec(j) = (KF_internals.tposn_vec(k0)-KF_internals.tposn_vec(k1))/time_diff; */
            temp_dcm_be[newmeas] =
              (TTKF_100Hz_Function_1D_M->
               dwork.KF_internals.tposn_vec[seeker_request] -
               TTKF_100Hz_Function_1D_M->
               dwork.KF_internals.tposn_vec[seeker_standby]) / time_diff;

            /* 'GUID_Embedded_Guid_TTKF_1D:454' vele_vec(j) = (KF_internals.tpose_vec(k0)-KF_internals.tpose_vec(k1))/time_diff; */
            temp_dcm_es[newmeas] =
              (TTKF_100Hz_Function_1D_M->
               dwork.KF_internals.tpose_vec[seeker_request] -
               TTKF_100Hz_Function_1D_M->
               dwork.KF_internals.tpose_vec[seeker_standby]) / time_diff;

            /* 'GUID_Embedded_Guid_TTKF_1D:455' Wfac_vec(j) = KF_internals.Wfac(k0); */
            /* 'GUID_Embedded_Guid_TTKF_1D:456' if (Wfac_vec(j) < 0.001) */
            if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[seeker_request]
                < 0.001) {
              /* 'GUID_Embedded_Guid_TTKF_1D:457' Wfac_den(j) = 1.0; */
              Wfac_den[newmeas] = 1.0;
            } else {
              /* 'GUID_Embedded_Guid_TTKF_1D:458' else */
              /* 'GUID_Embedded_Guid_TTKF_1D:459' Wfac_den(j) = Wfac_vec(j); */
              Wfac_den[newmeas] =
                TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[seeker_request];
            }

            Wfac_vec[newmeas] =
              TTKF_100Hz_Function_1D_M->dwork.KF_internals.Wfac[seeker_request];
          }
        }
      } else {
        /* 'GUID_Embedded_Guid_TTKF_1D:463' else */
        /* 'GUID_Embedded_Guid_TTKF_1D:464' KF_internals.i = 0; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.i = 0.0;
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:466' Wfac_norm = Wfac_vec/(sum(Wfac_den)+1.0e-06); */
      Tgt_Alt = sum_sVbQhjnY(Wfac_den) + 1.0E-6;

      /* 'GUID_Embedded_Guid_TTKF_1D:467' temp_out = (DCMecef2ned')*[sum(veln_vec.*Wfac_norm) sum(vele_vec.*Wfac_norm) 0.0]'; */
      for (seeker_request = 0; seeker_request < 9; seeker_request++) {
        k0 = Wfac_vec[seeker_request] / Tgt_Alt;
        Wfac_den[seeker_request] = temp_dcm_be[seeker_request] * k0;
        temp_dcm_es_0[seeker_request] = temp_dcm_es[seeker_request] * k0;
        Wfac_vec[seeker_request] = k0;
      }

      rgexy = sum_sVbQhjnY(Wfac_den);
      posDdiff0 = sum_sVbQhjnY(temp_dcm_es_0);
      for (seeker_request = 0; seeker_request < 3; seeker_request++) {
        temp_out[seeker_request] = (DCMecef2ned[(3 * seeker_request) + 2] * 0.0)
          + ((DCMecef2ned[(3 * seeker_request) + 1] * posDdiff0) + (DCMecef2ned
              [3 * seeker_request] * rgexy));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:468' input.tvex  = temp_out(1); */
      TTKF_100Hz_Function_1D_M->dwork.input.tvex = temp_out[0];

      /* 'GUID_Embedded_Guid_TTKF_1D:469' input.tvey  = temp_out(2); */
      TTKF_100Hz_Function_1D_M->dwork.input.tvey = temp_out[1];

      /* 'GUID_Embedded_Guid_TTKF_1D:470' input.tvez  = temp_out(3); */
      TTKF_100Hz_Function_1D_M->dwork.input.tvez = temp_out[2];

      /* 'GUID_Embedded_Guid_TTKF_1D:476' if (NavMode_in >= 30) */
      if (((int32_T)rtU_navmodein) >= 30) {
        /* 'GUID_Embedded_Guid_TTKF_1D:477' if (t_no_lock > 30.0 || input.mode == def.MP_MISSIONLOAD) */
        if ((TTKF_100Hz_Function_1D_M->dwork.t_no_lock > 30.0) ||
            (TTKF_100Hz_Function_1D_M->dwork.input.mode == 1.0)) {
          /* 'GUID_Embedded_Guid_TTKF_1D:478' KF_internals = initiateKF(setup,input,KF_internals); */
          initiateKF((TTKF_100Hz_Function_1D_M)->dwork.setup.hP0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.vP0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.hV0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.vV0,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trex,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trey,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trez,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvex,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvey,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvez,
                     &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals);
        }
      } else {
        if (TTKF_100Hz_Function_1D_M->dwork.t_no_lock > 2.0) {
          /* 'GUID_Embedded_Guid_TTKF_1D:480' elseif (t_no_lock > 2.0) */
          /* 'GUID_Embedded_Guid_TTKF_1D:481' KF_internals = initiateKF(setup,input,KF_internals); */
          initiateKF((TTKF_100Hz_Function_1D_M)->dwork.setup.hP0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.vP0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.hV0,
                     (TTKF_100Hz_Function_1D_M)->dwork.setup.vV0,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trex,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trey,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.trez,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvex,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvey,
                     (TTKF_100Hz_Function_1D_M)->dwork.input.tvez,
                     &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals);
        }
      }
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:484' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:487' ftime = SysTime_in - ftime0; */
      ftime = rtU_SysTimein - TTKF_100Hz_Function_1D_M->dwork.ftime0;

      /* 'GUID_Embedded_Guid_TTKF_1D:489' if (ftime < 2.5 && input.mode == def.MP_MISSIONLOAD) */
      if ((ftime < 2.5) && (TTKF_100Hz_Function_1D_M->dwork.input.mode == 1.0))
      {
        /* 'GUID_Embedded_Guid_TTKF_1D:490' KF_internals.measweight = 0.0; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.measweight = 0.0;
      }
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:495' newmeas = uint32(0); */
    newmeas = 0;

    /* 'GUID_Embedded_Guid_TTKF_1D:496' t_no_lock = t_no_lock + delt; */
    TTKF_100Hz_Function_1D_M->dwork.t_no_lock +=
      TTKF_100Hz_Function_1D_M->dwork.delt;

    /* 'GUID_Embedded_Guid_TTKF_1D:497' if (MP_in.MissionType > 0 && Skr_in.TrackStatus && Skr_in.AnglesValid && input.epsm~=epsm_prev && input.etam~=etam_prev) */
    if (((((((int32_T)rtU_MPin->MissionType) > 0) && (rtU_seekerin->TrackStatus
            != 0.0)) && (rtU_seekerin->AnglesValid != 0.0)) &&
         (TTKF_100Hz_Function_1D_M->dwork.input.epsm !=
          TTKF_100Hz_Function_1D_M->dwork.epsm_prev)) &&
        (TTKF_100Hz_Function_1D_M->dwork.input.etam !=
         TTKF_100Hz_Function_1D_M->dwork.etam_prev)) {
      /* 'GUID_Embedded_Guid_TTKF_1D:498' newmeas = uint32(1); */
      newmeas = 1;

      /* 'GUID_Embedded_Guid_TTKF_1D:499' KF_internals.measweight = KF_internals.measweight + 1.0; */
      TTKF_100Hz_Function_1D_M->dwork.KF_internals.measweight++;

      /* 'GUID_Embedded_Guid_TTKF_1D:500' epsm_prev = input.epsm; */
      TTKF_100Hz_Function_1D_M->dwork.epsm_prev =
        TTKF_100Hz_Function_1D_M->dwork.input.epsm;

      /* 'GUID_Embedded_Guid_TTKF_1D:501' etam_prev = input.etam; */
      TTKF_100Hz_Function_1D_M->dwork.etam_prev =
        TTKF_100Hz_Function_1D_M->dwork.input.etam;

      /* 'GUID_Embedded_Guid_TTKF_1D:503' dT = LIB_limit(0.000,0.020,SysTime_in - Skr_in.MeasureTime); */
      k0 = rtU_SysTimein - rtU_seekerin->MeasureTime;

      /* 'LIB_limit:27' if (in > max) */
      if (k0 > 0.02) {
        /* 'LIB_limit:28' out = max; */
        k0 = 0.02;
      } else if (k0 < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        k0 = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:504' e_dT.phi = input.wbxm*dT; */
      /* 'GUID_Embedded_Guid_TTKF_1D:505' e_dT.the = input.wbym*dT; */
      /* 'GUID_Embedded_Guid_TTKF_1D:506' e_dT.psi = input.wbzm*dT; */
      /* 'GUID_Embedded_Guid_TTKF_1D:507' dcm_dT = LIB_e2d(e_dT); */
      /* 'GUID_Embedded_Guid_TTKF_1D:508' Ulos = zeros(3,1); */
      /* 'GUID_Embedded_Guid_TTKF_1D:509' Ulos(1,1) =  1.0*cos(input.epsm); */
      k1 = cos(TTKF_100Hz_Function_1D_M->dwork.input.epsm);

      /* 'GUID_Embedded_Guid_TTKF_1D:510' Ulos(2,1) =  1.0*cos(input.epsm)*sin(input.etam); */
      time_diff = k1 * sin(TTKF_100Hz_Function_1D_M->dwork.input.etam);

      /* 'GUID_Embedded_Guid_TTKF_1D:511' Ulos(3,1) = -1.0*sin(input.epsm); */
      Tgt_Alt = -sin(TTKF_100Hz_Function_1D_M->dwork.input.epsm);

      /* 'GUID_Embedded_Guid_TTKF_1D:512' Umod = dcm_dT'*Ulos; */
      LIB_e2d((TTKF_100Hz_Function_1D_M)->dwork.input.wbxm * k0,
              (TTKF_100Hz_Function_1D_M)->dwork.input.wbym * k0,
              (TTKF_100Hz_Function_1D_M)->dwork.input.wbzm * k0, temp_dcm_be);
      for (seeker_request = 0; seeker_request < 3; seeker_request++) {
        temp_out[seeker_request] = (temp_dcm_be[(3 * seeker_request) + 2] *
          Tgt_Alt) + ((temp_dcm_be[(3 * seeker_request) + 1] * time_diff) +
                      (temp_dcm_be[3 * seeker_request] * k1));
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:513' if (~isnan(Umod(1))) */
      if (!rtIsNaN(temp_out[0])) {
        /* 'GUID_Embedded_Guid_TTKF_1D:514' input.epsm = -asin(LIB_limit(-0.9999,0.9999,Umod(3))); */
        /* 'LIB_limit:27' if (in > max) */
        if (temp_out[2] > 0.9999) {
          /* 'LIB_limit:28' out = max; */
          k0 = 0.9999;
        } else if (temp_out[2] < -0.9999) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          k0 = -0.9999;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
          k0 = temp_out[2];
        }

        TTKF_100Hz_Function_1D_M->dwork.input.epsm = -asin(k0);

        /* 'GUID_Embedded_Guid_TTKF_1D:515' input.etam = atan2(Umod(2),Umod(1)); */
        TTKF_100Hz_Function_1D_M->dwork.input.etam = rt_atan2d_snf(temp_out[1],
          temp_out[0]);
      }

      /* 'GUID_Embedded_Guid_TTKF_1D:519' t_no_lock = 0.0; */
      TTKF_100Hz_Function_1D_M->dwork.t_no_lock = 0.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:522' if (KF_internals.rge_clamp) */
    if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge_clamp != 0.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:523' t_clamp = t_clamp + delt; */
      TTKF_100Hz_Function_1D_M->dwork.t_clamp +=
        TTKF_100Hz_Function_1D_M->dwork.delt;
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:524' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:525' t_clamp = 0.0; */
      TTKF_100Hz_Function_1D_M->dwork.t_clamp = 0.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:528' end_of_flight = uint8(0); */
    end_of_flight = 0U;

    /* 'GUID_Embedded_Guid_TTKF_1D:529' if (t_no_lock > 2.0) */
    if (TTKF_100Hz_Function_1D_M->dwork.t_no_lock > 2.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:531' KF_internals.measweight = floor(LIB_limit(0.0,1000000.0,12.0-t_no_lock)); */
      /* 'LIB_limit:27' if (in > max) */
      if ((12.0 - TTKF_100Hz_Function_1D_M->dwork.t_no_lock) > 1.0E+6) {
        /* 'LIB_limit:28' out = max; */
        k0 = 1.0E+6;
      } else if ((12.0 - TTKF_100Hz_Function_1D_M->dwork.t_no_lock) < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        k0 = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        k0 = 12.0 - TTKF_100Hz_Function_1D_M->dwork.t_no_lock;
      }

      TTKF_100Hz_Function_1D_M->dwork.KF_internals.measweight = floor(k0);

      /* 'GUID_Embedded_Guid_TTKF_1D:534' if (t_clamp > 3.0) */
      if (TTKF_100Hz_Function_1D_M->dwork.t_clamp > 3.0) {
        /* 'GUID_Embedded_Guid_TTKF_1D:535' end_of_flight = uint8(1); */
        end_of_flight = 1U;
      }
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:539' KF_internals = modelupdate(input,KF_internals); */
    modelupdate((TTKF_100Hz_Function_1D_M)->dwork.input.mrex,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mrey,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mrez,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mvex,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mvey,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mvez,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mphi,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mthe,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mpsi,
                (TTKF_100Hz_Function_1D_M)->dwork.input.dcm_eb,
                (TTKF_100Hz_Function_1D_M)->dwork.input.dcm_es,
                (TTKF_100Hz_Function_1D_M)->dwork.input.epsm,
                (TTKF_100Hz_Function_1D_M)->dwork.input.etam,
                &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals);

    /* 'GUID_Embedded_Guid_TTKF_1D:540' KF_internals = timeupdate(setup,input,KF_internals); */
    timeupdate((TTKF_100Hz_Function_1D_M)->dwork.setup.Q,
               &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals);

    /* 'GUID_Embedded_Guid_TTKF_1D:547' if (newmeas) */
    if (((uint32_T)newmeas) != 0U) {
      /* 'GUID_Embedded_Guid_TTKF_1D:548' input = rgemconstruct(input,KF_internals,def); */
      rgemconstruct(&(TTKF_100Hz_Function_1D_M)->dwork.input,
                    (TTKF_100Hz_Function_1D_M)->dwork.KF_internals.X,
                    (TTKF_100Hz_Function_1D_M)->dwork.KF_internals.them_g, 1.0);
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:552' if (SysTime_in > 15.4) */
    /* 'GUID_Embedded_Guid_TTKF_1D:555' KF_internals = noiseupdate(newmeas,time,input,KF_internals,fromDATA,def); */
    noiseupdate((uint32_T)newmeas, (TTKF_100Hz_Function_1D_M)->dwork.time,
                (TTKF_100Hz_Function_1D_M)->dwork.input.mode,
                (TTKF_100Hz_Function_1D_M)->dwork.input.freq,
                (TTKF_100Hz_Function_1D_M)->dwork.input.epsm,
                (TTKF_100Hz_Function_1D_M)->dwork.input.etam,
                &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals, rtU_DATAttkf,
                0.0, 1.0, &(TTKF_100Hz_Function_1D_M->dwork));

    /* 'GUID_Embedded_Guid_TTKF_1D:556' if (newmeas) */
    if (((uint32_T)newmeas) != 0U) {
      /* 'GUID_Embedded_Guid_TTKF_1D:557' [delX,KF_internals] = measupdate(input,KF_internals,def); */
      measupdate((TTKF_100Hz_Function_1D_M)->dwork.input.mode,
                 (TTKF_100Hz_Function_1D_M)->dwork.input.freq,
                 (TTKF_100Hz_Function_1D_M)->dwork.input.epsm,
                 (TTKF_100Hz_Function_1D_M)->dwork.input.etam,
                 (TTKF_100Hz_Function_1D_M)->dwork.input.rgem,
                 (TTKF_100Hz_Function_1D_M)->dwork.input.ValidGNSSNav,
                 &(TTKF_100Hz_Function_1D_M)->dwork.KF_internals, 0.0, 1.0,
                 (TTKF_100Hz_Function_1D_M)->dwork.delX);
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:559' if (t_no_lock < 0.2) */
    if (TTKF_100Hz_Function_1D_M->dwork.t_no_lock < 0.2) {
      /* 'GUID_Embedded_Guid_TTKF_1D:560' lock = uint8(1); */
      lock = 1U;

      /* 'GUID_Embedded_Guid_TTKF_1D:561' KF_internals.AZ = LIB_limit(0.0,1.0,KF_internals.AZ + delt); */
      k0 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ +
        TTKF_100Hz_Function_1D_M->dwork.delt;

      /* 'LIB_limit:27' if (in > max) */
      if (k0 > 1.0) {
        /* 'LIB_limit:28' out = max; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = 1.0;
      } else if (k0 < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = k0;
      }
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:562' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:563' lock = uint8(0); */
      lock = 0U;

      /* 'GUID_Embedded_Guid_TTKF_1D:564' KF_internals.AZ = LIB_limit(0.0,1.0,KF_internals.AZ - delt); */
      k0 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ -
        TTKF_100Hz_Function_1D_M->dwork.delt;

      /* 'LIB_limit:27' if (in > max) */
      if (k0 > 1.0) {
        /* 'LIB_limit:28' out = max; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = 1.0;
      } else if (k0 < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ = k0;
      }
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:566' eps_median = KF_internals.AZ*LIB_limit(-0.3491,0.3491,KF_internals.eps_median); */
    /* 'LIB_limit:27' if (in > max) */
    if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_median > 0.3491) {
      /* 'LIB_limit:28' out = max; */
      k0 = 0.3491;
    } else if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_median < -0.3491)
    {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      k0 = -0.3491;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      k0 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.eps_median;
    }

    k0 *= TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ;

    /* 'GUID_Embedded_Guid_TTKF_1D:567' eta_median = KF_internals.AZ*LIB_limit(-0.3491,0.3491,KF_internals.eta_median); */
    /* 'LIB_limit:27' if (in > max) */
    if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_median > 0.3491) {
      /* 'LIB_limit:28' out = max; */
      k1 = 0.3491;
    } else if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_median < -0.3491)
    {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      k1 = -0.3491;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      k1 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.eta_median;
    }

    k1 *= TTKF_100Hz_Function_1D_M->dwork.KF_internals.AZ;

    /* 'GUID_Embedded_Guid_TTKF_1D:570' if (t_no_lock > 1.0 && time > time_prev) */
    if ((TTKF_100Hz_Function_1D_M->dwork.t_no_lock > 1.0) &&
        (TTKF_100Hz_Function_1D_M->dwork.time >
         TTKF_100Hz_Function_1D_M->dwork.time_prev)) {
      /* 'GUID_Embedded_Guid_TTKF_1D:571' seeker_request = 1; */
      seeker_request = 1;

      /* 'GUID_Embedded_Guid_TTKF_1D:572' time_prev = time + 0.100; */
      TTKF_100Hz_Function_1D_M->dwork.time_prev =
        TTKF_100Hz_Function_1D_M->dwork.time + 0.1;
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:573' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:574' seeker_request = 0; */
      seeker_request = 0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:576' if (time > 5.0) */
    if (TTKF_100Hz_Function_1D_M->dwork.time > 5.0) {
      /* 'GUID_Embedded_Guid_TTKF_1D:577' seeker_standby = 0; */
      seeker_standby = 0;

      /* 'GUID_Embedded_Guid_TTKF_1D:578' seeker_opertnl = 1; */
      seeker_opertnl = 1;
    } else {
      /* 'GUID_Embedded_Guid_TTKF_1D:579' else */
      /* 'GUID_Embedded_Guid_TTKF_1D:580' seeker_standby = 1; */
      seeker_standby = 1;

      /* 'GUID_Embedded_Guid_TTKF_1D:581' seeker_opertnl = 0; */
      seeker_opertnl = 0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:583' status_seeker = uint16(0); */
    /* 'GUID_Embedded_Guid_TTKF_1D:584' status_seeker = bitset(status_seeker,1,seeker_request); */
    /* 'GUID_Embedded_Guid_TTKF_1D:585' status_seeker = bitset(status_seeker,2,seeker_standby); */
    status_seeker = (uint16_T)((seeker_request != 0) ? 1 : 0);

    /* 'GUID_Embedded_Guid_TTKF_1D:586' status_seeker = bitset(status_seeker,4,seeker_opertnl); */
    if (seeker_standby != 0) {
      status_seeker |= (uint16_T)2;
    }

    if (seeker_opertnl != 0) {
      status_seeker |= (uint16_T)8;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:588' status_ttkf = uint16(0); */
    /* 'GUID_Embedded_Guid_TTKF_1D:589' status_ttkf = bitset(status_ttkf,1,input.mode); */
    /* 'GUID_Embedded_Guid_TTKF_1D:590' status_ttkf = bitset(status_ttkf,2,1); */
    /* 'GUID_Embedded_Guid_TTKF_1D:591' status_ttkf = bitset(status_ttkf,3,lock); */
    status_ttkf = (uint16_T)((int32_T)(((int32_T)
      ((TTKF_100Hz_Function_1D_M->dwork.input.mode != 0.0) ? 1 : 0)) | ((uint8_T)
      2)));

    /* 'GUID_Embedded_Guid_TTKF_1D:592' status_ttkf = bitset(status_ttkf,4,end_of_flight); */
    if (((int32_T)lock) != 0) {
      status_ttkf |= (uint16_T)4;
    }

    if (((int32_T)end_of_flight) != 0) {
      status_ttkf |= (uint16_T)8;
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:599' eps_mod = eps_median; */
  /* 'GUID_Embedded_Guid_TTKF_1D:600' eta_mod = eta_median; */
  /* 'GUID_Embedded_Guid_TTKF_1D:602' eg.phi  = input.mphi; */
  /* 'GUID_Embedded_Guid_TTKF_1D:603' eg.the  = input.mthe; */
  /* 'GUID_Embedded_Guid_TTKF_1D:604' eg.psi  = input.mpsi; */
  /* 'GUID_Embedded_Guid_TTKF_1D:605' dcm_bn = LIB_e2d(eg); */
  /* 'GUID_Embedded_Guid_TTKF_1D:608' temp_out = DCMecef2ned*[input.mvex input.mvey input.mvez]'; */
  for (seeker_request = 0; seeker_request < 3; seeker_request++) {
    temp_out[seeker_request] = (DCMecef2ned[seeker_request + 6] *
      TTKF_100Hz_Function_1D_M->dwork.input.mvez) + ((DCMecef2ned[seeker_request
      + 3] * TTKF_100Hz_Function_1D_M->dwork.input.mvey) +
      (DCMecef2ned[seeker_request] * TTKF_100Hz_Function_1D_M->dwork.input.mvex));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:609' FPpsi = atan2(temp_out(2),temp_out(1)); */
  time_diff = rt_atan2d_snf(temp_out[1], temp_out[0]);

  /* 'GUID_Embedded_Guid_TTKF_1D:617' ubm.x = cos(KF_internals.the_b+eps_mod)*cos(KF_internals.psi_b+eta_mod); */
  /* 'GUID_Embedded_Guid_TTKF_1D:618' ubm.y = cos(KF_internals.the_b+eps_mod)*sin(KF_internals.psi_b+eta_mod); */
  /* 'GUID_Embedded_Guid_TTKF_1D:619' ubm.z =-sin(KF_internals.the_b+eps_mod); */
  /* 'GUID_Embedded_Guid_TTKF_1D:620' ugm = dcm_bn*[ubm.x ubm.y ubm.z]'; */
  LIB_e2d((TTKF_100Hz_Function_1D_M)->dwork.input.mphi,
          (TTKF_100Hz_Function_1D_M)->dwork.input.mthe,
          (TTKF_100Hz_Function_1D_M)->dwork.input.mpsi, temp_dcm_be);
  Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_b + k0;
  psi_rel = sin(Tgt_Alt);
  posDdiff0 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_b + k1;
  Tgt_Alt = cos(Tgt_Alt);
  rgexy = Tgt_Alt * cos(posDdiff0);
  posDdiff0 = Tgt_Alt * sin(posDdiff0);
  for (seeker_request = 0; seeker_request < 3; seeker_request++) {
    temp_out[seeker_request] = (temp_dcm_be[seeker_request + 6] * (-psi_rel)) +
      ((temp_dcm_be[seeker_request + 3] * posDdiff0) +
       (temp_dcm_be[seeker_request] * rgexy));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:621' themod_g = -asin(LIB_limit(-0.999999,0.999999,ugm(3))/1.0); */
  /* 'LIB_limit:27' if (in > max) */
  if (temp_out[2] > 0.999999) {
    /* 'LIB_limit:28' out = max; */
    Tgt_Alt = 0.999999;
  } else if (temp_out[2] < -0.999999) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    Tgt_Alt = -0.999999;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    Tgt_Alt = temp_out[2];
  }

  Tgt_Alt = asin(Tgt_Alt);

  /* 'GUID_Embedded_Guid_TTKF_1D:622' psimod_g = atan2(ugm(2),ugm(1)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:624' posDdiff0 = input.malt - input.talt0; */
  posDdiff0 = TTKF_100Hz_Function_1D_M->dwork.input.malt -
    TTKF_100Hz_Function_1D_M->dwork.input.talt0;

  /* 'GUID_Embedded_Guid_TTKF_1D:626' if (KF_internals.the_g < -0.001) */
  if (TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_g < -0.001) {
    /* 'GUID_Embedded_Guid_TTKF_1D:627' rgexy = posDdiff0/tan(-KF_internals.the_g); */
    rgexy = posDdiff0 / tan(-TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_g);
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:628' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:629' rgexy = posDdiff0/tan(0.001); */
    rgexy = posDdiff0 / 0.0010000003333334668;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:631' if (themod_g < -0.001) */
  if ((-Tgt_Alt) < -0.001) {
    /* 'GUID_Embedded_Guid_TTKF_1D:632' rgemodxy = posDdiff0/tan(-themod_g); */
    posDdiff0 /= tan(Tgt_Alt);
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:633' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:634' rgemodxy = posDdiff0/tan(0.001); */
    posDdiff0 /= 0.0010000003333334668;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:636' psi_rel = KF_internals.psi_g-FPpsi; */
  psi_rel = TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_g - time_diff;

  /* 'GUID_Embedded_Guid_TTKF_1D:637' rgex    =  rgexy*cos(psi_rel); */
  /* 'GUID_Embedded_Guid_TTKF_1D:638' rgey    =  rgexy*sin(psi_rel); */
  /* 'GUID_Embedded_Guid_TTKF_1D:639' rgemodx =  rgemodxy*cos(psimod_g-FPpsi); */
  /* 'GUID_Embedded_Guid_TTKF_1D:640' rgemody =  rgemodxy*sin(psimod_g-FPpsi); */
  /* 'GUID_Embedded_Guid_TTKF_1D:643' relx = rgemodx - rgex; */
  Tgt_Alt_tmp = rt_atan2d_snf(temp_out[1], temp_out[0]) - time_diff;
  Tgt_Alt = (cos(Tgt_Alt_tmp) * posDdiff0) - (rgexy * cos(psi_rel));

  /* 'GUID_Embedded_Guid_TTKF_1D:644' rely = rgemody - rgey; */
  psi_rel = (sin(Tgt_Alt_tmp) * posDdiff0) - (rgexy * sin(psi_rel));

  /* 'GUID_Embedded_Guid_TTKF_1D:648' relxy0 = norm([relx rely]); */
  Tgt_Alt_0[0] = Tgt_Alt;
  Tgt_Alt_0[1] = psi_rel;
  rgexy = norm_lpbhAiWU(Tgt_Alt_0);

  /* 'GUID_Embedded_Guid_TTKF_1D:649' if (relxy0 > 0.0) */
  if (rgexy > 0.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:650' relxy = LIB_limit(0.0,2000.0,relxy0-500.0); */
    /* 'LIB_limit:27' if (in > max) */
    if ((rgexy - 500.0) > 2000.0) {
      /* 'LIB_limit:28' out = max; */
      posDdiff0 = 2000.0;
    } else if ((rgexy - 500.0) < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      posDdiff0 = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
      posDdiff0 = rgexy - 500.0;
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:651' relx = relx*(relxy/relxy0); */
    Tgt_Alt *= posDdiff0 / rgexy;

    /* 'GUID_Embedded_Guid_TTKF_1D:652' rely = rely*(relxy/relxy0); */
    psi_rel *= posDdiff0 / rgexy;
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:653' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:654' relxy = relxy0; */
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:658' sr_cmp = 2.0; */
  /* 'GUID_Embedded_Guid_TTKF_1D:660' if (input.mode == def.MP_MISSIONLOAD) */
  if (TTKF_100Hz_Function_1D_M->dwork.input.mode == 1.0) {
    /* 'GUID_Embedded_Guid_TTKF_1D:662' pos_fac = LIB_limit(0.0,0.01,50000.0/(KF_internals.rge^2)); */
    rgexy = 50000.0 / (TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge *
                       TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge);

    /* 'LIB_limit:27' if (in > max) */
    if (rgexy > 0.01) {
      /* 'LIB_limit:28' out = max; */
      rgexy = 0.01;
    } else if (rgexy < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      rgexy = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:663' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:664' pos_fac = LIB_limit(0.0,0.01,0.0001/(KF_internals.conf^2)); */
    rgexy = 0.0001 / (TTKF_100Hz_Function_1D_M->dwork.KF_internals.conf *
                      TTKF_100Hz_Function_1D_M->dwork.KF_internals.conf);

    /* 'LIB_limit:27' if (in > max) */
    if (rgexy > 0.01) {
      /* 'LIB_limit:28' out = max; */
      rgexy = 0.01;
    } else if (rgexy < 0.0) {
      /* 'LIB_limit:29' elseif (in < min) */
      /* 'LIB_limit:30' out = min; */
      rgexy = 0.0;
    } else {
      /* 'LIB_limit:31' else */
      /* 'LIB_limit:32' out = in; */
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:668' if (t_no_lock >= 0.2 && ftime > 0.0) */
  if ((TTKF_100Hz_Function_1D_M->dwork.t_no_lock >= 0.2) && (ftime > 0.0)) {
    /* 'GUID_Embedded_Guid_TTKF_1D:669' if (relx < relx_prev && relx_prev > 0.0) */
    if ((Tgt_Alt < TTKF_100Hz_Function_1D_M->dwork.relx_prev) &&
        (TTKF_100Hz_Function_1D_M->dwork.relx_prev > 0.0)) {
      /* 'GUID_Embedded_Guid_TTKF_1D:670' relx = LIB_limit(relx_prev-sr_cmp,10000.0,relx); */
      /* 'LIB_limit:27' if (in > max) */
      if (Tgt_Alt > 10000.0) {
        /* 'LIB_limit:28' out = max; */
        Tgt_Alt = 10000.0;
      } else if (Tgt_Alt < (TTKF_100Hz_Function_1D_M->dwork.relx_prev - 2.0)) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.relx_prev - 2.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      if ((Tgt_Alt > TTKF_100Hz_Function_1D_M->dwork.relx_prev) &&
          (TTKF_100Hz_Function_1D_M->dwork.relx_prev < 0.0)) {
        /* 'GUID_Embedded_Guid_TTKF_1D:671' elseif (relx > relx_prev && relx_prev < 0.0) */
        /* 'GUID_Embedded_Guid_TTKF_1D:672' relx = LIB_limit(-10000.0,relx_prev+sr_cmp,relx); */
        /* 'LIB_limit:27' if (in > max) */
        if (Tgt_Alt > (TTKF_100Hz_Function_1D_M->dwork.relx_prev + 2.0)) {
          /* 'LIB_limit:28' out = max; */
          Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.relx_prev + 2.0;
        } else if (Tgt_Alt < -10000.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          Tgt_Alt = -10000.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }
      }
    }

    /* 'GUID_Embedded_Guid_TTKF_1D:674' if (rely < rely_prev && rely_prev > 0.0) */
    if ((psi_rel < TTKF_100Hz_Function_1D_M->dwork.rely_prev) &&
        (TTKF_100Hz_Function_1D_M->dwork.rely_prev > 0.0)) {
      /* 'GUID_Embedded_Guid_TTKF_1D:675' rely = LIB_limit(rely_prev-sr_cmp,10000.0,rely); */
      /* 'LIB_limit:27' if (in > max) */
      if (psi_rel > 10000.0) {
        /* 'LIB_limit:28' out = max; */
        psi_rel = 10000.0;
      } else if (psi_rel < (TTKF_100Hz_Function_1D_M->dwork.rely_prev - 2.0)) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        psi_rel = TTKF_100Hz_Function_1D_M->dwork.rely_prev - 2.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }
    } else {
      if ((psi_rel > TTKF_100Hz_Function_1D_M->dwork.rely_prev) &&
          (TTKF_100Hz_Function_1D_M->dwork.rely_prev < 0.0)) {
        /* 'GUID_Embedded_Guid_TTKF_1D:676' elseif (rely > rely_prev && rely_prev < 0.0) */
        /* 'GUID_Embedded_Guid_TTKF_1D:677' rely = LIB_limit(-10000.0,rely_prev+sr_cmp,rely); */
        /* 'LIB_limit:27' if (in > max) */
        if (psi_rel > (TTKF_100Hz_Function_1D_M->dwork.rely_prev + 2.0)) {
          /* 'LIB_limit:28' out = max; */
          psi_rel = TTKF_100Hz_Function_1D_M->dwork.rely_prev + 2.0;
        } else if (psi_rel < -10000.0) {
          /* 'LIB_limit:29' elseif (in < min) */
          /* 'LIB_limit:30' out = min; */
          psi_rel = -10000.0;
        } else {
          /* 'LIB_limit:31' else */
          /* 'LIB_limit:32' out = in; */
        }
      }
    }
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:688' reln = relx*cos(FPpsi) - rely*sin(FPpsi); */
  /* 'GUID_Embedded_Guid_TTKF_1D:689' rele = relx*sin(FPpsi) + rely*cos(FPpsi); */
  /* 'GUID_Embedded_Guid_TTKF_1D:690' temp_out = (DCMecef2ned')*[reln rele 0.0]'; */
  posDdiff0 = sin(time_diff);
  Tgt_Alt_tmp = cos(time_diff);
  time_diff = (Tgt_Alt * Tgt_Alt_tmp) - (psi_rel * posDdiff0);
  posDdiff0 = (Tgt_Alt * posDdiff0) + (psi_rel * Tgt_Alt_tmp);
  for (seeker_request = 0; seeker_request < 3; seeker_request++) {
    temp_out[seeker_request] = (DCMecef2ned[(3 * seeker_request) + 2] * 0.0) +
      ((DCMecef2ned[(3 * seeker_request) + 1] * posDdiff0) + (DCMecef2ned[3 *
        seeker_request] * time_diff));
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:692' if (t_no_lock < 0.2) */
  if (TTKF_100Hz_Function_1D_M->dwork.t_no_lock < 0.2) {
    /* 'GUID_Embedded_Guid_TTKF_1D:693' KF_internals.X(1) = KF_internals.X(1) + pos_fac*temp_out(1); */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[0] += rgexy * temp_out[0];

    /* 'GUID_Embedded_Guid_TTKF_1D:694' KF_internals.X(2) = KF_internals.X(2) + pos_fac*temp_out(2); */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[1] += rgexy * temp_out[1];

    /* 'GUID_Embedded_Guid_TTKF_1D:695' KF_internals.X(3) = KF_internals.X(3) + pos_fac*temp_out(3); */
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[2] += rgexy * temp_out[2];

    /* 'GUID_Embedded_Guid_TTKF_1D:696' d_tgtrex = (1.0-pos_fac)*temp_out(1); */
    time_diff = (1.0 - rgexy) * temp_out[0];

    /* 'GUID_Embedded_Guid_TTKF_1D:697' d_tgtrey = (1.0-pos_fac)*temp_out(2); */
    posDdiff0 = (1.0 - rgexy) * temp_out[1];

    /* 'GUID_Embedded_Guid_TTKF_1D:698' d_tgtrez = (1.0-pos_fac)*temp_out(3); */
    rgexy = (1.0 - rgexy) * temp_out[2];
  } else {
    /* 'GUID_Embedded_Guid_TTKF_1D:699' else */
    /* 'GUID_Embedded_Guid_TTKF_1D:700' d_tgtrex = temp_out(1); */
    time_diff = temp_out[0];

    /* 'GUID_Embedded_Guid_TTKF_1D:701' d_tgtrey = temp_out(2); */
    posDdiff0 = temp_out[1];

    /* 'GUID_Embedded_Guid_TTKF_1D:702' d_tgtrez = temp_out(3); */
    rgexy = temp_out[2];
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:710' relx_prev = relx; */
  TTKF_100Hz_Function_1D_M->dwork.relx_prev = Tgt_Alt;

  /* 'GUID_Embedded_Guid_TTKF_1D:711' rely_prev = rely; */
  TTKF_100Hz_Function_1D_M->dwork.rely_prev = psi_rel;

  /* 'GUID_Embedded_Guid_TTKF_1D:720' ttkfout.status = uint16(status_ttkf); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.status = status_ttkf;

  /* 'GUID_Embedded_Guid_TTKF_1D:721' ttkfout.confidence = uint16(floor(KF_internals.conf*100)); */
  Tgt_Alt = TTKF_100Hz_Function_1D_M->dwork.KF_internals.conf * 100.0;
  psi_rel = floor(Tgt_Alt);
  if (psi_rel < 65536.0) {
    if (psi_rel >= 0.0) {
      TTKF_100Hz_Function_1D_M->dwork.ttkfout.confidence = (uint16_T)psi_rel;
    } else {
      TTKF_100Hz_Function_1D_M->dwork.ttkfout.confidence = 0U;
    }
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkfout.confidence = MAX_uint16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:722' ttkfout.newmeas = uint16(newmeas); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.newmeas = (uint16_T)newmeas;

  /* 'GUID_Embedded_Guid_TTKF_1D:723' ttkfout.skrcmds = uint16(status_seeker); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.skrcmds = status_seeker;

  /* 'GUID_Embedded_Guid_TTKF_1D:724' ttkfout.tgtrex = KF_internals.X(1)+d_tgtrex; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrex =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[0] + time_diff;

  /* 'GUID_Embedded_Guid_TTKF_1D:725' ttkfout.tgtrey = KF_internals.X(2)+d_tgtrey; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrey =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[1] + posDdiff0;

  /* 'GUID_Embedded_Guid_TTKF_1D:726' ttkfout.tgtrez = KF_internals.X(3)+d_tgtrez; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrez =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[2] + rgexy;

  /* 'GUID_Embedded_Guid_TTKF_1D:727' ttkfout.tgtrex_uncond = KF_internals.X(1); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrex_uncond =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:728' ttkfout.tgtrey_uncond = KF_internals.X(2); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrey_uncond =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:729' ttkfout.tgtrez_uncond = KF_internals.X(3); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrez_uncond =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[2];

  /* 'GUID_Embedded_Guid_TTKF_1D:730' ttkfout.tgtvex = KF_internals.X(4); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtvex =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[3];

  /* 'GUID_Embedded_Guid_TTKF_1D:731' ttkfout.tgtvey = KF_internals.X(5); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtvey =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[4];

  /* 'GUID_Embedded_Guid_TTKF_1D:732' ttkfout.tgtvez = KF_internals.X(6); */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtvez =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[5];

  /* 'GUID_Embedded_Guid_TTKF_1D:733' ttkfout.tgtrge = KF_internals.rge; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.tgtrge =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:735' ttkfout.themedian = KF_internals.the_b + eps_median; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.themedian =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_b + k0;

  /* 'GUID_Embedded_Guid_TTKF_1D:736' ttkfout.psimedian = KF_internals.psi_b + eta_median; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.psimedian =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_b + k1;

  /* 'GUID_Embedded_Guid_TTKF_1D:738' ttkfout.the_b = KF_internals.the_b; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.the_b =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:739' ttkfout.psi_b = KF_internals.psi_b; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.psi_b =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:741' ttkfout.dthe_b = KF_internals.dthe_b; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.dthe_b =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dthe_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:742' ttkfout.dpsi_b = KF_internals.dpsi_b; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.dpsi_b =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dpsi_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:743' ttkfout.rge_clamp = KF_internals.rge_clamp; */
  TTKF_100Hz_Function_1D_M->dwork.ttkfout.rge_clamp =
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge_clamp;

  /* 'GUID_Embedded_Guid_TTKF_1D:746' ttkftel.TKF_Status      = uint16(status_ttkf); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_Status = status_ttkf;

  /* 'GUID_Embedded_Guid_TTKF_1D:747' ttkftel.TKF_Confidence  = uint16(LIB_limit(0,100,KF_internals.conf*100)); */
  k0 = Tgt_Alt;

  /* 'LIB_limit:27' if (in > max) */
  if (Tgt_Alt > 100.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 100.0;
  } else if (Tgt_Alt < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 65536.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_Confidence = (uint16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_Confidence = MAX_uint16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:748' ttkftel.TKF_NewMeas     = uint16(newmeas); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_NewMeas = (uint16_T)newmeas;

  /* 'GUID_Embedded_Guid_TTKF_1D:749' ttkftel.TKF_SkrStatus   = uint16(status_seeker); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SkrStatus = status_seeker;

  /* 'GUID_Embedded_Guid_TTKF_1D:750' ttkftel.TKF_SkrMeasTime = single(Skr_in.MeasureTime); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SkrMeasTime = (real32_T)
    rtU_seekerin->MeasureTime;

  /* 'GUID_Embedded_Guid_TTKF_1D:751' ttkftel.TKF_SkrMeasThe  = single(input.epsm); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SkrMeasThe = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.input.epsm;

  /* 'GUID_Embedded_Guid_TTKF_1D:752' ttkftel.TKF_SkrMeasPsi  = single(input.etam); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SkrMeasPsi = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.input.etam;

  /* 'GUID_Embedded_Guid_TTKF_1D:753' ttkftel.TKF_SkrMeasRge  = single(input.rgem); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SkrMeasRge = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.input.rgem;

  /* 'GUID_Embedded_Guid_TTKF_1D:754' ttkftel.TKF_PitchRate   = single(KF_internals.dthe_b); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_PitchRate = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dthe_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:755' ttkftel.TKF_YawRate     = single(KF_internals.dpsi_b); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_YawRate = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.dpsi_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:756' ttkftel.TKF_PitchAngle  = single(KF_internals.the_b); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_PitchAngle = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.the_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:757' ttkftel.TKF_YawAngle    = single(KF_internals.psi_b); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_YawAngle = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.psi_b;

  /* 'GUID_Embedded_Guid_TTKF_1D:758' ttkftel.TKF_TgtPosECEFX = single(KF_internals.X(1)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtPosECEFX = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[0];

  /* 'GUID_Embedded_Guid_TTKF_1D:759' ttkftel.TKF_TgtPosECEFY = single(KF_internals.X(2)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtPosECEFY = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[1];

  /* 'GUID_Embedded_Guid_TTKF_1D:760' ttkftel.TKF_TgtPosECEFZ = single(KF_internals.X(3)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtPosECEFZ = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[2];

  /* 'GUID_Embedded_Guid_TTKF_1D:761' ttkftel.TKF_TgtVelECEFX = single(KF_internals.X(4)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtVelECEFX = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[3];

  /* 'GUID_Embedded_Guid_TTKF_1D:762' ttkftel.TKF_TgtVelECEFY = single(KF_internals.X(5)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtVelECEFY = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[4];

  /* 'GUID_Embedded_Guid_TTKF_1D:763' ttkftel.TKF_TgtVelECEFZ = single(KF_internals.X(6)); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtVelECEFZ = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.X[5];

  /* 'GUID_Embedded_Guid_TTKF_1D:764' ttkftel.TKF_TgtSlantRge = single(KF_internals.rge); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_TgtSlantRge = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.KF_internals.rge;

  /* 'GUID_Embedded_Guid_TTKF_1D:765' ttkftel.TKF_P11         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(1,1))*10)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[0]) * 10.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P11 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P11 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:766' ttkftel.TKF_P22         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(2,2))*10)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[7]) * 10.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P22 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P22 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:767' ttkftel.TKF_P33         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(3,3))*10)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[14]) * 10.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P33 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P33 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:768' ttkftel.TKF_P44         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(4,4))*100)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[21]) * 100.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P44 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P44 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:769' ttkftel.TKF_P55         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(5,5))*100)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[28]) * 100.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P55 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P55 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:770' ttkftel.TKF_P66         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.P(6,6))*100)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.P[35]) * 100.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P66 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_P66 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:771' ttkftel.TKF_R11         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.R(1,1))*10000)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.R[0]) * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R11 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R11 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:772' ttkftel.TKF_R22         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.R(2,2))*10000)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.R[4]) * 10000.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R22 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R22 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:773' ttkftel.TKF_R33         = int16(LIB_limit(-32768,32767,sqrt(KF_internals.R(3,3))*10)); */
  k0 = sqrt(TTKF_100Hz_Function_1D_M->dwork.KF_internals.R[8]) * 10.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 32767.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 32767.0;
  } else if (k0 < -32768.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = -32768.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 32768.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R33 = (int16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_R33 = MAX_int16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:774' ttkftel.TKF_MeasWeight  = uint16(LIB_limit(0,65535,KF_internals.measweight*50)); */
  k0 = TTKF_100Hz_Function_1D_M->dwork.KF_internals.measweight * 50.0;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 65535.0) {
    /* 'LIB_limit:28' out = max; */
    k0 = 65535.0;
  } else if (k0 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 65536.0) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_MeasWeight = (uint16_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_MeasWeight = MAX_uint16_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:775' ttkftel.TKF_NavAlt0     = single(NavAlt0); */
  TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_NavAlt0 = (real32_T)
    TTKF_100Hz_Function_1D_M->dwork.NavAlt0;

  /* 'GUID_Embedded_Guid_TTKF_1D:776' ttkftel.TKF_SysTime_in  = uint32(LIB_limit(0,4294967295,SysTime_in*1000000)); */
  k0 = rtU_SysTimein * 1.0E+6;

  /* 'LIB_limit:27' if (in > max) */
  if (k0 > 4.294967295E+9) {
    /* 'LIB_limit:28' out = max; */
    k0 = 4.294967295E+9;
  } else if (k0 < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    k0 = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(k0);
  if (Tgt_Alt < 4.294967296E+9) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SysTime_in = (uint32_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_SysTime_in = MAX_uint32_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:777' ttkftel.TKF_FTime       = uint32(LIB_limit(0,4294967295,ftime*1000000)); */
  ftime *= 1.0E+6;

  /* 'LIB_limit:27' if (in > max) */
  if (ftime > 4.294967295E+9) {
    /* 'LIB_limit:28' out = max; */
    ftime = 4.294967295E+9;
  } else if (ftime < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    ftime = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(ftime);
  if (Tgt_Alt < 4.294967296E+9) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_FTime = (uint32_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_FTime = MAX_uint32_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:778' ttkftel.TKF_wbxm_in     = uint32(LIB_limit(0,4294967295,input.wbxm*1000*cnsts.r2d + 2000000000)); */
  ftime = ((TTKF_100Hz_Function_1D_M->dwork.input.wbxm * 1000.0) *
           57.295779513082323) + 2.0E+9;

  /* 'LIB_limit:27' if (in > max) */
  if (ftime > 4.294967295E+9) {
    /* 'LIB_limit:28' out = max; */
    ftime = 4.294967295E+9;
  } else if (ftime < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    ftime = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(ftime);
  if (Tgt_Alt < 4.294967296E+9) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbxm_in = (uint32_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbxm_in = MAX_uint32_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:779' ttkftel.TKF_wbym_in     = uint32(LIB_limit(0,4294967295,input.wbym*1000*cnsts.r2d + 2000000000)); */
  ftime = ((TTKF_100Hz_Function_1D_M->dwork.input.wbym * 1000.0) *
           57.295779513082323) + 2.0E+9;

  /* 'LIB_limit:27' if (in > max) */
  if (ftime > 4.294967295E+9) {
    /* 'LIB_limit:28' out = max; */
    ftime = 4.294967295E+9;
  } else if (ftime < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    ftime = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(ftime);
  if (Tgt_Alt < 4.294967296E+9) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbym_in = (uint32_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbym_in = MAX_uint32_T;
  }

  /* 'GUID_Embedded_Guid_TTKF_1D:780' ttkftel.TKF_wbzm_in     = uint32(LIB_limit(0,4294967295,input.wbzm*1000*cnsts.r2d + 2000000000)); */
  ftime = ((TTKF_100Hz_Function_1D_M->dwork.input.wbzm * 1000.0) *
           57.295779513082323) + 2.0E+9;

  /* 'LIB_limit:27' if (in > max) */
  if (ftime > 4.294967295E+9) {
    /* 'LIB_limit:28' out = max; */
    ftime = 4.294967295E+9;
  } else if (ftime < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    ftime = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  Tgt_Alt = rt_roundd_snf(ftime);
  if (Tgt_Alt < 4.294967296E+9) {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbzm_in = (uint32_T)Tgt_Alt;
  } else {
    TTKF_100Hz_Function_1D_M->dwork.ttkftel.TKF_wbzm_in = MAX_uint32_T;
  }

  /* Outport: '<Root>/ttkfout' incorporates:
   *  MATLAB Function: '<S1>/Guidance Estimator'
   */
  /* 'GUID_Embedded_Guid_TTKF_1D:782' ttkfana.p001 = input.epsm; */
  /* 'GUID_Embedded_Guid_TTKF_1D:783' ttkfana.p002 = input.etam; */
  /* 'GUID_Embedded_Guid_TTKF_1D:784' ttkfana.p003 = input.rgem; */
  /* 'GUID_Embedded_Guid_TTKF_1D:785' ttkfana.p004 = sqrt(KF_internals.R(1,1)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:786' ttkfana.p005 = sqrt(KF_internals.R(2,2)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:787' ttkfana.p006 = sqrt(KF_internals.R(3,3)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:788' ttkfana.p007 = sqrt(KF_internals.P(1,1)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:789' ttkfana.p008 = sqrt(KF_internals.P(2,2)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:790' ttkfana.p009 = sqrt(KF_internals.P(3,3)); */
  /* 'GUID_Embedded_Guid_TTKF_1D:791' ttkfana.p010 = KF_internals.eps; */
  /* 'GUID_Embedded_Guid_TTKF_1D:792' ttkfana.p011 = KF_internals.eta; */
  /* 'GUID_Embedded_Guid_TTKF_1D:793' ttkfana.p012 = KF_internals.rge; */
  /* 'GUID_Embedded_Guid_TTKF_1D:794' ttkfana.p013 = KF_internals.the_g; */
  /* 'GUID_Embedded_Guid_TTKF_1D:795' ttkfana.p014 = KF_internals.psi_g; */
  /* 'GUID_Embedded_Guid_TTKF_1D:796' ttkfana.p015 = KF_internals.them_g; */
  /* 'GUID_Embedded_Guid_TTKF_1D:797' ttkfana.p016 = KF_internals.psim_g; */
  /* 'GUID_Embedded_Guid_TTKF_1D:798' ttkfana.p017 = KF_internals.the_b + KF_internals.eps_median; */
  /* 'GUID_Embedded_Guid_TTKF_1D:799' ttkfana.p018 = KF_internals.the_lin_up; */
  /* 'GUID_Embedded_Guid_TTKF_1D:800' ttkfana.p019 = KF_internals.the_lin_lo; */
  /* 'GUID_Embedded_Guid_TTKF_1D:801' ttkfana.p020 = KF_internals.psi_b + KF_internals.eta_median; */
  /* 'GUID_Embedded_Guid_TTKF_1D:802' ttkfana.p021 = KF_internals.psi_lin_up; */
  /* 'GUID_Embedded_Guid_TTKF_1D:803' ttkfana.p022 = KF_internals.psi_lin_lo; */
  /* 'GUID_Embedded_Guid_TTKF_1D:804' ttkfana.p023 = KF_internals.delpsi; */
  /* 'GUID_Embedded_Guid_TTKF_1D:805' ttkfana.p024 = KF_internals.delthe; */
  /* 'GUID_Embedded_Guid_TTKF_1D:806' ttkfana.p025 = delX(3,3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:807' ttkfana.p026 = delX(4,1); */
  /* 'GUID_Embedded_Guid_TTKF_1D:808' ttkfana.p027 = delX(4,2); */
  /* 'GUID_Embedded_Guid_TTKF_1D:809' ttkfana.p028 = delX(4,3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:810' ttkfana.p029 = delX(5,1); */
  /* 'GUID_Embedded_Guid_TTKF_1D:811' ttkfana.p030 = delX(5,2); */
  /* 'GUID_Embedded_Guid_TTKF_1D:812' ttkfana.p031 = delX(5,3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:813' ttkfana.p032 = delX(6,1); */
  /* 'GUID_Embedded_Guid_TTKF_1D:814' ttkfana.p033 = delX(6,2); */
  /* 'GUID_Embedded_Guid_TTKF_1D:815' ttkfana.p034 = delX(6,3); */
  /* 'GUID_Embedded_Guid_TTKF_1D:816' ttkfana.p035 = KF_internals.eps_sample(KF_internals.k); */
  /* 'GUID_Embedded_Guid_TTKF_1D:817' ttkfana.p036 = KF_internals.eta_sample(KF_internals.k); */
  /* 'GUID_Embedded_Guid_TTKF_1D:818' ttkfana.p037 = KF_internals.eps_diff; */
  /* 'GUID_Embedded_Guid_TTKF_1D:819' ttkfana.p038 = KF_internals.eta_diff; */
  /* 'GUID_Embedded_Guid_TTKF_1D:820' ttkfana.p039 = KF_internals.nserge; */
  /* 'GUID_Embedded_Guid_TTKF_1D:821' ttkfana.p040 = KF_internals.conf*100; */
  /* 'GUID_Embedded_Guid_TTKF_1D:823' TTKF_out = ttkfout; */
  /* 'GUID_Embedded_Guid_TTKF_1D:825' TTKF_100Hz_tel = ttkftel; */
  /* 'GUID_Embedded_Guid_TTKF_1D:827' TTKF_ana = ttkfana; */
  *rtY_ttkfout = TTKF_100Hz_Function_1D_M->dwork.ttkfout;

  /* Outport: '<Root>/ttkftel_100Hz' incorporates:
   *  MATLAB Function: '<S1>/Guidance Estimator'
   */
  *rtY_ttkftel_100Hz = TTKF_100Hz_Function_1D_M->dwork.ttkftel;

  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_ttkf_at_outport_1' */
}

/* Model initialize function */
void TTKF_100Hz_Function_1D_initialize(RT_MODEL *const TTKF_100Hz_Function_1D_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    uint32_T r;
    int32_T mti;

    /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_ttkf_at_outport_1' incorporates:
     *  SubSystem: '<Root>/TTKF'
     */
    /* SystemInitialize for MATLAB Function: '<S1>/Guidance Estimator' */
    TTKF_100Hz_Function_1D_M->dwork.lat_prev = -1.0;
    memset(&TTKF_100Hz_Function_1D_M->dwork.state[0], 0, 625U * (sizeof(uint32_T)));
    r = 5489U;
    TTKF_100Hz_Function_1D_M->dwork.state[0] = 5489U;
    for (mti = 0; mti < 623; mti++) {
      r = ((((r >> 30U) ^ r) * 1812433253U) + ((uint32_T)mti)) + 1U;
      TTKF_100Hz_Function_1D_M->dwork.state[mti + 1] = r;
    }

    TTKF_100Hz_Function_1D_M->dwork.state[624] = 624U;

    /* End of SystemInitialize for MATLAB Function: '<S1>/Guidance Estimator' */
    /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_ttkf_at_outport_1' */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
