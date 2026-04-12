/*
 * File: NAV_DoNavigationMain_100Hz.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_100Hz'.
 *
 * Model version                  : 1.644
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Wed Jan 18 11:12:26 2023
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

#include "NAV_DoNavigationMain_100Hz.h"
#include "mpower_JIx9WxiC.h"
#include "norm_xgB9aDcJ.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"
#include "rt_roundd_snf.h"

/* Forward declaration for local functions */
static void LIB_ecef2lla(real_T re_x, real_T re_y, real_T re_z, real_T
  *pos_lla_lat, real_T *pos_lla_lon, real_T *pos_lla_alt);
static void init_ECEFnavigation(real_T deltaT, real_T we, const T_Cartesian er,
  const T_Cartesian ev, const T_Euler ge, T_NAV_NavigationState
  *pNavigationState);
static void init_KalmanFilter_16State(real_T *KalmanFilterstate_states, real_T
  *KalmanFilterstate_FeedbackWeight, real_T *KalmanFilterstate_timeupdateINS,
  real_T *KalmanFilterstate_timeupdateAttitude, uint8_T
  *KalmanFilterstate_updateINS, uint8_T *KalmanFilterstate_updateAttitude,
  uint16_T *KalmanFilterstate_Reserved, real_T KalmanFilterstate_X[16], real_T
  KalmanFilterstate_A[256], real_T KalmanFilterstate_P[256], real_T
  KalmanFilterstate_Q[256], T_NAV_MeasurementNoise *KalmanFilterstate_R, real_T
  KalmanFilterstate_B_gyro[3], real_T KalmanFilterstate_B_accel[3], uint8_T
  *KalmanFilterstate_align_valid);
static void init_Baro(real_T *baro_SystemTime, real_T *baro_alt, real_T
                      *baro_pressure, uint8_T *baro_valid, uint8_T
                      *baro_Reserved01, uint16_T *baro_Reserved02, real_T
                      *baro_comp_alt, real_T *baro_KalmanFilter_delta_baro);
static void init_Pylon(real_T *pylon_attitude_phi, real_T *pylon_attitude_the,
  real_T *pylon_attitude_psi, real_T *pylon_accel_limit_low, real_T
  *pylon_accel_limit_high, uint8_T *pylon_valid, uint8_T
  *pylon_initial_attitude_set);
static void LIB_StandardAtmospherePressure(real_T pressure, real_T
  *atmos_out_theta, real_T *atmos_out_delta, real_T *atmos_out_sigma, real_T
  *atmos_out_temperature, real_T *atmos_out_altitudeGeo, real_T
  *atmos_out_altitude, real_T *atmos_out_density);
static void LIB_q2d(real_T q_q0, real_T q_q1, real_T q_q2, real_T q_q3, real_T
                    dcm[9]);
static void INS_InertialNavigation(real_T time, real_T w_x, real_T w_y, real_T
  w_z, real_T f_x, real_T f_y, real_T f_z, T_NAV_NavigationState *pState, real_T
  baro_alt);
static void DoNavigationMain100Hz(real_T t, real_T w_x, real_T w_y, real_T w_z,
  real_T f_x, real_T f_y, real_T f_z, T_NAV_NavigationState *navState,
  T_NAV_KalmanFilterState *kfState, real_T baro_alt);
static void UpdateBiases(T_NAV_KalmanFilterState *kfState, const real_T delta[16]);
static void KalmanFilterMeasurementUpdate(const real_T measurementState_nav[16],
  const real_T measurementState_kalmanfilter_state[16], const real_T
  measurementState_kalmanfilter_covariance[256], uint8_T
  measurementState_valid_gnss, uint8_T measurementState_valid_baro, uint8_T
  measurementState_valid_ta, uint32_T measurementState_valid_mag, uint8_T
  measurementState_valid_feedback_nav, uint8_T
  measurementState_valid_feedback_att, uint8_T
  measurementState_valid_reset_navigation, T_NAV_NavigationState *navState,
  T_NAV_KalmanFilterState *kfState, T_NAV_BAROState *baroState, DW *dwork);
static void NAV_Embedded_DoNavigationMain100Hz(real_T
  InertialNavigationSensorGenericDataIn100Hz_SystemTime, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_x, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_y, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_z, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_x, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_y, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_z, real_T
  InertialNavigationSensorGenericDataIn100Hz_baro_SystemTime, real_T
  InertialNavigationSensorGenericDataIn100Hz_baro_pressure, const
  T_GUID_GuidCtrlFlags InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags,
  const T_Euler taState_attitude, const T_Cartesian taState_pos_e, const
  T_Cartesian taState_vel_e, uint8_T taState_valid, const
  T_NAV_KFilterErrorEstimateState *KalmanFilterDeltaState, const T_GNSS_Result
  *gnssState, real_T b_nav_prv_deltaT, real_T b_nav_prv_WE, uint16_T
  *navigationMode, T_NAV_NavigationState *navigationstateINS,
  T_NAV_KalmanFilterState *KalmanFilterState, T_NAV_BAROState *BaroState, DW
  *dwork);

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function  pos_lla = LIB_ecef2lla(re)
 */
static void LIB_ecef2lla(real_T re_x, real_T re_y, real_T re_z, real_T
  *pos_lla_lat, real_T *pos_lla_lon, real_T *pos_lla_alt)
{
  real_T r2;
  real_T r;
  real_T F;
  real_T g;
  real_T c;
  real_T a;
  real_T F_tmp;
  real_T g_tmp;

  /* 'LIB_ecef2lla:29' RE = 6378137.0; */
  /* 'LIB_ecef2lla:30' f = 1/298.257223563; */
  /* 'LIB_ecef2lla:33' ucalc_limit = 1.0e-15; */
  /* 'LIB_ecef2lla:36' sre = RE*(1.0 - f); */
  /* 'LIB_ecef2lla:39' ecc1_sqr = (2.0*f) - f^2; */
  /* 'LIB_ecef2lla:42' ecc2_sqr = f*(2.0 - f)/((1 - f)^2); */
  /* 'LIB_ecef2lla:45' sdre  = RE^2 - sre^2; */
  /* 'LIB_ecef2lla:48' r2  = re.x^2 + re.y^2; */
  r2 = (re_x * re_x) + (re_y * re_y);

  /* 'LIB_ecef2lla:49' r   = sqrt(r2); */
  r = sqrt(r2);

  /* 'LIB_ecef2lla:51' if (r < ucalc_limit) */
  if (r < 1.0E-15) {
    /* 'LIB_ecef2lla:52' r = ucalc_limit; */
    r = 1.0E-15;
  }

  /* 'LIB_ecef2lla:56' F   = 54*sre^2*re.z^2; */
  F_tmp = re_z * re_z;
  F = F_tmp * 2.182048199171718E+15;

  /* 'LIB_ecef2lla:57' g   = r2 + ((1 - ecc1_sqr)*re.z^2) - (ecc1_sqr*sdre); */
  g_tmp = F_tmp * 0.99330562000985867;
  g = (g_tmp + r2) - 1.8230912546094608E+9;

  /* 'LIB_ecef2lla:60' if (abs(g) < ucalc_limit) && (g > 0) */
  c = fabs(g);
  if ((c < 1.0E-15) && (g > 0.0)) {
    /* 'LIB_ecef2lla:61' g = ucalc_limit; */
    g = 1.0E-15;
  } else {
    if ((c < 1.0E-15) && (g < 0.0)) {
      /* 'LIB_ecef2lla:62' elseif (abs(g) < ucalc_limit) && (g < 0) */
      /* 'LIB_ecef2lla:63' g = -ucalc_limit; */
      g = -1.0E-15;
    }
  }

  /* 'LIB_ecef2lla:67' c   = (ecc1_sqr^2*F*r2)/(g^3); */
  c = ((4.481472345240445E-5 * F) * r2) / rt_powd_snf(g, 3.0);

  /* 'LIB_ecef2lla:70' s   = (1 + c + sqrt((c^2) + (2.0 * c)) )^(1.0/3.0); */
  c = rt_powd_snf(sqrt((c * c) + (2.0 * c)) + (1.0 + c), 0.33333333333333331);

  /* 'LIB_ecef2lla:73' temps = 3.0*(s + (1/s) + 1)^2 * g^2; */
  a = ((1.0 / c) + c) + 1.0;
  g = ((a * a) * 3.0) * (g * g);

  /* 'LIB_ecef2lla:74' if (abs(temps) < ucalc_limit) && (temps > 0) */
  if ((fabs(g) < 1.0E-15) && (g > 0.0)) {
    /* 'LIB_ecef2lla:75' temps = ucalc_limit; */
    g = 1.0E-15;
  } else {
    if ((fabs(c) < 1.0E-15) && (g < 0.0)) {
      /* 'LIB_ecef2lla:76' elseif (abs(s) < ucalc_limit) && (temps < 0) */
      /* 'LIB_ecef2lla:77' temps = -ucalc_limit; */
      g = -1.0E-15;
    }
  }

  /* 'LIB_ecef2lla:81' p   = F/temps; */
  F /= g;

  /* 'LIB_ecef2lla:82' temp = 1 + (2.0*ecc1_sqr^2*p); */
  g = (8.96294469048089E-5 * F) + 1.0;

  /* 'LIB_ecef2lla:85' if temp < ucalc_limit */
  if (g < 1.0E-15) {
    /* 'LIB_ecef2lla:86' temp = ucalc_limit; */
    g = 1.0E-15;
  }

  /* 'LIB_ecef2lla:90' q  = sqrt(temp); */
  g = sqrt(g);

  /* 'LIB_ecef2lla:91' q1 = 1.0 + (1/q); */
  /* 'LIB_ecef2lla:92' q2 = q*(1.0 + q); */
  /* 'LIB_ecef2lla:93' dEccen = 1 - ecc1_sqr; */
  /* 'LIB_ecef2lla:94' roTemp = ((RE^2/2)*q1) - ((dEccen*p*re.z^2)/q2) - (p*r2/2); */
  r2 = ((((1.0 / g) + 1.0) * 2.03403157953845E+13) - (((0.99330562000985867 * F)
          * F_tmp) / ((1.0 + g) * g))) - ((F * r2) / 2.0);

  /* 'LIB_ecef2lla:97' if roTemp < ucalc_limit */
  if (r2 < 1.0E-15) {
    /* 'LIB_ecef2lla:98' roTemp = ucalc_limit; */
    r2 = 1.0E-15;
  }

  /* 'LIB_ecef2lla:102' ro  = -((ecc1_sqr*p*r)/(1 + q)) + sqrt(roTemp); */
  /* 'LIB_ecef2lla:103' tmp = (r - (ecc1_sqr*ro))^2; */
  r2 = r - (((-(((0.0066943799901413165 * F) * r) / (1.0 + g))) + sqrt(r2)) *
            0.0066943799901413165);
  r2 *= r2;

  /* 'LIB_ecef2lla:104' u   = sqrt(tmp + re.z^2); */
  /* 'LIB_ecef2lla:105' v   = sqrt(tmp + ((1 - ecc1_sqr)*re.z^2)); */
  /* 'LIB_ecef2lla:106' den_z = RE*v; */
  F = sqrt(g_tmp + r2) * 6.378137E+6;

  /* 'LIB_ecef2lla:109' if den_z < ucalc_limit */
  if (F < 1.0E-15) {
    /* 'LIB_ecef2lla:110' den_z = ucalc_limit; */
    F = 1.0E-15;
  }

  /* 'LIB_ecef2lla:114' zo  = (sre^2*re.z)/den_z; */
  /* 'LIB_ecef2lla:115' pos_lla.lat = atan2((re.z + (ecc2_sqr*zo)), r); */
  *pos_lla_lat = rt_atan2d_snf((((4.0408299984661445E+13 * re_z) / F) *
    0.0067394967422764341) + re_z, r);

  /* 'LIB_ecef2lla:116' pos_lla.lon = atan2(re.y, re.x); */
  *pos_lla_lon = rt_atan2d_snf(re_y, re_x);

  /* 'LIB_ecef2lla:117' pos_lla.alt = u*(1 - (sre^2/den_z)); */
  *pos_lla_alt = sqrt(F_tmp + r2) * (1.0 - (4.0408299984661445E+13 / F));
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function pNavigationState = init_ECEFnavigation(deltaT, we, er, ev, ge)
 */
static void init_ECEFnavigation(real_T deltaT, real_T we, const T_Cartesian er,
  const T_Cartesian ev, const T_Euler ge, T_NAV_NavigationState
  *pNavigationState)
{
  real_T dcm_bn[9];
  real_T dcm_be[9];
  real_T q0;
  real_T q1;
  int32_T switch_count;
  real_T nom_q;
  real_T tmp[9];
  int32_T i;
  real_T qsq_idx_2;
  real_T qsq_idx_3;
  real_T dcm_bn_tmp;
  real_T dcm_bn_tmp_0;
  real_T dcm_bn_tmp_1;
  real_T dcm_bn_tmp_2;
  real_T dcm_bn_tmp_3;
  int32_T dcm_be_tmp;

  /* 'init_ECEFnavigation:33' pNavigationState.systemTime = 0.0; */
  pNavigationState->systemTime = 0.0;

  /* 'init_ECEFnavigation:36' pNavigationState.wib_b = CreateVecStruct3; */
  /* 'init_ECEFnavigation:105' v.x = 0.0; */
  pNavigationState->wib_b.x = 0.0;

  /* 'init_ECEFnavigation:106' v.y = 0.0; */
  pNavigationState->wib_b.y = 0.0;

  /* 'init_ECEFnavigation:107' v.z = 0.0; */
  pNavigationState->wib_b.z = 0.0;

  /* 'init_ECEFnavigation:110' coder.cstructname(v, 'T_Cartesian'); */
  /* 'init_ECEFnavigation:39' pNavigationState.fib_e = CreateVecStruct3; */
  /* 'init_ECEFnavigation:105' v.x = 0.0; */
  pNavigationState->fib_e.x = 0.0;

  /* 'init_ECEFnavigation:106' v.y = 0.0; */
  pNavigationState->fib_e.y = 0.0;

  /* 'init_ECEFnavigation:107' v.z = 0.0; */
  pNavigationState->fib_e.z = 0.0;

  /* 'init_ECEFnavigation:110' coder.cstructname(v, 'T_Cartesian'); */
  /* 'init_ECEFnavigation:42' pNavigationState.grav_e = CreateVecStruct3; */
  /* 'init_ECEFnavigation:105' v.x = 0.0; */
  pNavigationState->grav_e.x = 0.0;

  /* 'init_ECEFnavigation:106' v.y = 0.0; */
  pNavigationState->grav_e.y = 0.0;

  /* 'init_ECEFnavigation:107' v.z = 0.0; */
  pNavigationState->grav_e.z = 0.0;

  /* 'init_ECEFnavigation:110' coder.cstructname(v, 'T_Cartesian'); */
  /* 'init_ECEFnavigation:45' pNavigationState.pos_e = CreateVecStruct3; */
  /* 'init_ECEFnavigation:46' pNavigationState.pos_e = er; */
  pNavigationState->pos_e = er;

  /* 'init_ECEFnavigation:49' pNavigationState.pos_g = CreateGeographicPosStruct; */
  /* 'init_ECEFnavigation:50' lla = LIB_ecef2lla(er); */
  LIB_ecef2lla(er.x, er.y, er.z, &nom_q, &q0, &q1);

  /* 'init_ECEFnavigation:51' pNavigationState.pos_g = lla; */
  pNavigationState->pos_g.lat = nom_q;
  pNavigationState->pos_g.lon = q0;
  pNavigationState->pos_g.alt = q1;

  /* 'init_ECEFnavigation:54' pNavigationState.baro_alt = lla.alt; */
  pNavigationState->baro_alt = q1;

  /* 'init_ECEFnavigation:57' pNavigationState.vel_e = CreateVecStruct3; */
  /* 'init_ECEFnavigation:58' pNavigationState.vel_e = ev; */
  pNavigationState->vel_e = ev;

  /* 'init_ECEFnavigation:61' dcm_en = LIB_dcm_en(lla); */
  /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
  /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
  /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
  /* 'init_ECEFnavigation:62' dcm_ne = transpose(dcm_en); */
  /* 'init_ECEFnavigation:65' pNavigationState.att_bn = CreateEulerStruct; */
  /* 'init_ECEFnavigation:66' dcm_bn = LIB_e2d(ge); */
  /* 'LIB_e2d:26' dcm = zeros(3,3); */
  /* 'LIB_e2d:28' phi   = eg.phi; */
  /* 'LIB_e2d:29' theta = eg.the; */
  /* 'LIB_e2d:30' psi   = eg.psi; */
  /* 'LIB_e2d:32' dcm(1,1) = cos(theta)*cos(psi); */
  q1 = cos(ge.psi);
  qsq_idx_2 = cos(ge.the);
  dcm_bn[0] = qsq_idx_2 * q1;

  /* 'LIB_e2d:33' dcm(1,2) = (sin(phi)*sin(theta)*cos(psi)) - (cos(phi)*sin(psi)); */
  qsq_idx_3 = cos(ge.phi);
  dcm_bn_tmp = sin(ge.the);
  dcm_bn_tmp_0 = sin(ge.phi);
  dcm_bn_tmp_1 = sin(ge.psi);
  dcm_bn_tmp_2 = dcm_bn_tmp_0 * dcm_bn_tmp;
  dcm_bn[3] = (dcm_bn_tmp_2 * q1) - (qsq_idx_3 * dcm_bn_tmp_1);

  /* 'LIB_e2d:34' dcm(1,3) = (cos(phi)*sin(theta)*cos(psi)) + (sin(phi)*sin(psi)); */
  dcm_bn_tmp_3 = qsq_idx_3 * dcm_bn_tmp;
  dcm_bn[6] = (dcm_bn_tmp_3 * q1) + (dcm_bn_tmp_0 * dcm_bn_tmp_1);

  /* 'LIB_e2d:35' dcm(2,1) = cos(theta)*sin(psi); */
  dcm_bn[1] = qsq_idx_2 * dcm_bn_tmp_1;

  /* 'LIB_e2d:36' dcm(2,2) = (sin(phi)*sin(theta)*sin(psi)) + (cos(phi)*cos(psi)); */
  dcm_bn[4] = (dcm_bn_tmp_2 * dcm_bn_tmp_1) + (qsq_idx_3 * q1);

  /* 'LIB_e2d:37' dcm(2,3) = (cos(phi)*sin(theta)*sin(psi)) - (sin(phi)*cos(psi)); */
  dcm_bn[7] = (dcm_bn_tmp_3 * dcm_bn_tmp_1) - (dcm_bn_tmp_0 * q1);

  /* 'LIB_e2d:38' dcm(3,1) = -sin(theta); */
  dcm_bn[2] = -dcm_bn_tmp;

  /* 'LIB_e2d:39' dcm(3,2) =  sin(phi)*cos(theta); */
  dcm_bn[5] = dcm_bn_tmp_0 * qsq_idx_2;

  /* 'LIB_e2d:40' dcm(3,3) =  cos(phi)*cos(theta); */
  dcm_bn[8] = qsq_idx_3 * qsq_idx_2;

  /* 'init_ECEFnavigation:67' dcm_be = dcm_ne*dcm_bn; */
  q1 = -sin(nom_q);
  qsq_idx_2 = cos(q0);
  tmp[0] = q1 * qsq_idx_2;
  q0 = sin(q0);
  tmp[1] = q1 * q0;
  qsq_idx_3 = cos(nom_q);
  tmp[2] = qsq_idx_3;
  tmp[3] = -q0;
  tmp[4] = qsq_idx_2;
  tmp[5] = 0.0;
  tmp[6] = (-qsq_idx_3) * qsq_idx_2;
  tmp[7] = (-cos(nom_q)) * q0;
  tmp[8] = q1;
  for (switch_count = 0; switch_count < 3; switch_count++) {
    for (i = 0; i < 3; i++) {
      dcm_be_tmp = i + (3 * switch_count);
      dcm_be[dcm_be_tmp] = 0.0;
      dcm_be[dcm_be_tmp] += dcm_bn[3 * switch_count] * tmp[i];
      dcm_be[dcm_be_tmp] += dcm_bn[(3 * switch_count) + 1] * tmp[i + 3];
      dcm_be[dcm_be_tmp] += dcm_bn[(3 * switch_count) + 2] * tmp[i + 6];
    }
  }

  /* 'init_ECEFnavigation:68' pNavigationState.dcm_be = dcm_be; */
  memcpy(&pNavigationState->dcm_be[0], &dcm_be[0], 9U * (sizeof(real_T)));

  /* 'init_ECEFnavigation:69' pNavigationState.att_bn = ge; */
  pNavigationState->att_bn = ge;

  /* 'init_ECEFnavigation:72' pNavigationState.q_be = CreateQuaternionStruct; */
  /* 'init_ECEFnavigation:73' pNavigationState.q_be = LIB_d2q(dcm_be); */
  /* 'LIB_d2q:25' q0 = 1.0; */
  /* 'LIB_d2q:26' q1 = 0.0; */
  /* 'LIB_d2q:27' q2 = 0.0; */
  /* 'LIB_d2q:28' q3 = 0.0; */
  /* 'LIB_d2q:31' d11 = dcm(1,1); */
  /* 'LIB_d2q:32' d12 = dcm(1,2); */
  /* 'LIB_d2q:33' d13 = dcm(1,3); */
  /* 'LIB_d2q:34' d21 = dcm(2,1); */
  /* 'LIB_d2q:35' d22 = dcm(2,2); */
  /* 'LIB_d2q:36' d23 = dcm(2,3); */
  /* 'LIB_d2q:37' d31 = dcm(3,1); */
  /* 'LIB_d2q:38' d32 = dcm(3,2); */
  /* 'LIB_d2q:39' d33 = dcm(3,3); */
  /* 'LIB_d2q:42' qsq = [1 + d11 + d22 + d33;... */
  /* 'LIB_d2q:43'        1 + d11 - d22 - d33;... */
  /* 'LIB_d2q:44'        1 - d11 + d22 - d33;... */
  /* 'LIB_d2q:45'        1 - d11 - d22 + d33]; */
  q0 = ((1.0 + dcm_be[0]) + dcm_be[4]) + dcm_be[8];
  q1 = ((1.0 + dcm_be[0]) - dcm_be[4]) - dcm_be[8];
  qsq_idx_2 = ((1.0 - dcm_be[0]) + dcm_be[4]) - dcm_be[8];
  qsq_idx_3 = ((1.0 - dcm_be[0]) - dcm_be[4]) + dcm_be[8];

  /* 'LIB_d2q:48' switch_count = 1; */
  switch_count = 1;

  /* 'LIB_d2q:49' nom_q = qsq(1,1); */
  nom_q = q0;

  /* 'LIB_d2q:50' for i = 2:4 */
  /* 'LIB_d2q:51' if qsq(i,1) > nom_q */
  if (q1 > q0) {
    /* 'LIB_d2q:52' switch_count = i; */
    switch_count = 2;

    /* 'LIB_d2q:53' nom_q = qsq(i,1); */
    nom_q = q1;
  }

  /* 'LIB_d2q:51' if qsq(i,1) > nom_q */
  if (qsq_idx_2 > nom_q) {
    /* 'LIB_d2q:52' switch_count = i; */
    switch_count = 3;

    /* 'LIB_d2q:53' nom_q = qsq(i,1); */
    nom_q = qsq_idx_2;
  }

  /* 'LIB_d2q:51' if qsq(i,1) > nom_q */
  if (qsq_idx_3 > nom_q) {
    /* 'LIB_d2q:52' switch_count = i; */
    switch_count = 4;

    /* 'LIB_d2q:53' nom_q = qsq(i,1); */
  }

  /* 'LIB_d2q:57' switch switch_count */
  switch (switch_count) {
   case 1:
    /* 'LIB_d2q:58' case 1 */
    /* 'LIB_d2q:60' if qsq(1,1) <= 0.0 */
    if (q0 <= 0.0) {
      /* 'LIB_d2q:61' qsq(1,1) = 1.0; */
      q0 = 1.0;
    }

    /* 'LIB_d2q:63' q0 = 0.5*sqrt(qsq(1,1)); */
    q0 = 0.5 * sqrt(q0);

    /* 'LIB_d2q:64' q1 = (d32 - d23)/(4*q0); */
    q1 = (dcm_be[5] - dcm_be[7]) / (4.0 * q0);

    /* 'LIB_d2q:65' q2 = (d13 - d31)/(4*q0); */
    qsq_idx_2 = (dcm_be[6] - dcm_be[2]) / (4.0 * q0);

    /* 'LIB_d2q:66' q3 = (d21 - d12)/(4*q0); */
    nom_q = (dcm_be[1] - dcm_be[3]) / (4.0 * q0);
    break;

   case 2:
    /* 'LIB_d2q:67' case 2 */
    /* 'LIB_d2q:69' if qsq(2,1) <= 0.0 */
    if (q1 <= 0.0) {
      /* 'LIB_d2q:70' qsq(2,1) = 1.0; */
      q1 = 1.0;
    }

    /* 'LIB_d2q:72' q1 = 0.5*sqrt(qsq(2,1)); */
    q1 = 0.5 * sqrt(q1);

    /* 'LIB_d2q:73' q0 = (d32 - d23)/(4*q1); */
    q0 = (dcm_be[5] - dcm_be[7]) / (4.0 * q1);

    /* 'LIB_d2q:74' q2 = (d12 + d21)/(4*q1); */
    qsq_idx_2 = (dcm_be[3] + dcm_be[1]) / (4.0 * q1);

    /* 'LIB_d2q:75' q3 = (d13 + d31)/(4*q1); */
    nom_q = (dcm_be[6] + dcm_be[2]) / (4.0 * q1);
    break;

   case 3:
    /* 'LIB_d2q:76' case 3 */
    /* 'LIB_d2q:78' if qsq(3,1) <= 0.0 */
    if (qsq_idx_2 <= 0.0) {
      /* 'LIB_d2q:79' qsq(3,1) = 1.0; */
      qsq_idx_2 = 1.0;
    }

    /* 'LIB_d2q:81' q2 = 0.5*sqrt(qsq(3,1)); */
    qsq_idx_2 = 0.5 * sqrt(qsq_idx_2);

    /* 'LIB_d2q:82' q0 = (d13 - d31)/(4*q2); */
    q0 = (dcm_be[6] - dcm_be[2]) / (4.0 * qsq_idx_2);

    /* 'LIB_d2q:83' q1 = (d12 + d21)/(4*q2); */
    q1 = (dcm_be[3] + dcm_be[1]) / (4.0 * qsq_idx_2);

    /* 'LIB_d2q:84' q3 = (d23 + d32)/(4*q2); */
    nom_q = (dcm_be[7] + dcm_be[5]) / (4.0 * qsq_idx_2);
    break;

   default:
    /* 'LIB_d2q:85' case 4 */
    /* 'LIB_d2q:87' if qsq(4,1) <= 0.0 */
    if (qsq_idx_3 <= 0.0) {
      /* 'LIB_d2q:88' qsq(4,1) = 1.0; */
      qsq_idx_3 = 1.0;
    }

    /* 'LIB_d2q:90' q3 = 0.5*sqrt(qsq(4,1)); */
    nom_q = 0.5 * sqrt(qsq_idx_3);

    /* 'LIB_d2q:91' q0 = (d21 - d12)/(4*q3); */
    q0 = (dcm_be[1] - dcm_be[3]) / (4.0 * nom_q);

    /* 'LIB_d2q:92' q1 = (d13 + d31)/(4*q3); */
    q1 = (dcm_be[6] + dcm_be[2]) / (4.0 * nom_q);

    /* 'LIB_d2q:93' q2 = (d23 + d32)/(4*q3); */
    qsq_idx_2 = (dcm_be[7] + dcm_be[5]) / (4.0 * nom_q);
    break;
  }

  /* 'LIB_d2q:97' q.q0 = q0; */
  pNavigationState->q_be.q0 = q0;

  /* 'LIB_d2q:98' q.q1 = q1; */
  pNavigationState->q_be.q1 = q1;

  /* 'LIB_d2q:99' q.q2 = q2; */
  pNavigationState->q_be.q2 = qsq_idx_2;

  /* 'LIB_d2q:100' q.q3 = q3; */
  pNavigationState->q_be.q3 = nom_q;

  /* 'init_ECEFnavigation:75' pNavigationState.deltaT = deltaT; */
  pNavigationState->deltaT = deltaT;

  /* 'init_ECEFnavigation:76' pNavigationState.WE = we; */
  pNavigationState->WE = we;

  /* 'init_ECEFnavigation:79' pNavigationState.store = CreateStoreStruct; */
  /* 'init_ECEFnavigation:140' v.storesize = 120; */
  pNavigationState->store.storesize = 120.0;

  /* 'init_ECEFnavigation:141' v.storeplace = 0.0; */
  pNavigationState->store.storeplace = 0.0;

  /* 'init_ECEFnavigation:142' v.store = zeros(120,17); */
  memset(&pNavigationState->store.store[0], 0, 2040U * (sizeof(real_T)));

  /* 'init_ECEFnavigation:145' coder.cstructname(v, 'T_NAV_Store'); */
  /* 'init_ECEFnavigation:82' pNavigationState.GuidCtrlFlags.WingsOpen = uint8(0); */
  pNavigationState->GuidCtrlFlags.WingsOpen = 0U;

  /* 'init_ECEFnavigation:83' pNavigationState.GuidCtrlFlags.WingsRelease = uint8(0); */
  pNavigationState->GuidCtrlFlags.WingsRelease = 0U;

  /* 'init_ECEFnavigation:84' pNavigationState.GuidCtrlFlags.SADFuze = uint8(0); */
  pNavigationState->GuidCtrlFlags.SADFuze = 0U;

  /* 'init_ECEFnavigation:85' pNavigationState.GuidCtrlFlags.SwitchforBunt = uint8(0); */
  pNavigationState->GuidCtrlFlags.SwitchforBunt = 0U;

  /* 'init_ECEFnavigation:86' pNavigationState.GuidCtrlFlags.NavCtrlFlag = uint8(0); */
  pNavigationState->GuidCtrlFlags.NavCtrlFlag = 0U;

  /* 'init_ECEFnavigation:87' pNavigationState.GuidCtrlFlags.Umbilical = uint8(0); */
  pNavigationState->GuidCtrlFlags.Umbilical = 0U;

  /* 'init_ECEFnavigation:88' pNavigationState.GuidCtrlFlags.BatteryOn = uint8(0); */
  pNavigationState->GuidCtrlFlags.BatteryOn = 0U;

  /* 'init_ECEFnavigation:89' pNavigationState.GuidCtrlFlags.PstaticValid = uint8(0); */
  pNavigationState->GuidCtrlFlags.PstaticValid = 0U;

  /* 'init_ECEFnavigation:92' coder.cstructname(pNavigationState, 'T_NAV_NavigationState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function KalmanFilterstate = init_KalmanFilter_16State
 */
static void init_KalmanFilter_16State(real_T *KalmanFilterstate_states, real_T
  *KalmanFilterstate_FeedbackWeight, real_T *KalmanFilterstate_timeupdateINS,
  real_T *KalmanFilterstate_timeupdateAttitude, uint8_T
  *KalmanFilterstate_updateINS, uint8_T *KalmanFilterstate_updateAttitude,
  uint16_T *KalmanFilterstate_Reserved, real_T KalmanFilterstate_X[16], real_T
  KalmanFilterstate_A[256], real_T KalmanFilterstate_P[256], real_T
  KalmanFilterstate_Q[256], T_NAV_MeasurementNoise *KalmanFilterstate_R, real_T
  KalmanFilterstate_B_gyro[3], real_T KalmanFilterstate_B_accel[3], uint8_T
  *KalmanFilterstate_align_valid)
{
  /* 'init_KalmanFilter_16State:27' gravity = 9.81; */
  /* 'init_KalmanFilter_16State:28' d2r = pi/180.0; */
  /* 'init_KalmanFilter_16State:29' dph2rps = d2r/3600.0; */
  /* 'init_KalmanFilter_16State:30' mg2mps2 = gravity/1000.0; */
  /* 'init_KalmanFilter_16State:31' NumberKalmanFilterStates = 16; */
  /* 'init_KalmanFilter_16State:34' KalmanFilterstate.states = NumberKalmanFilterStates; */
  *KalmanFilterstate_states = 16.0;

  /* 'init_KalmanFilter_16State:35' KalmanFilterstate.FeedbackWeight = 0.2; */
  *KalmanFilterstate_FeedbackWeight = 0.2;

  /* 'init_KalmanFilter_16State:36' KalmanFilterstate.timeupdateINS = 1.0; */
  *KalmanFilterstate_timeupdateINS = 1.0;

  /* 'init_KalmanFilter_16State:37' KalmanFilterstate.timeupdateAttitude = 1.0; */
  *KalmanFilterstate_timeupdateAttitude = 1.0;

  /* 'init_KalmanFilter_16State:38' KalmanFilterstate.updateINS = uint8(0); */
  *KalmanFilterstate_updateINS = 0U;

  /* 'init_KalmanFilter_16State:39' KalmanFilterstate.updateAttitude = uint8(0); */
  *KalmanFilterstate_updateAttitude = 0U;

  /* 'init_KalmanFilter_16State:42' KalmanFilterstate.Reserved = uint16(0); */
  *KalmanFilterstate_Reserved = 0U;

  /* 'init_KalmanFilter_16State:44' KalmanFilterstate.X = zeros(NumberKalmanFilterStates,1); */
  memset(&KalmanFilterstate_X[0], 0, (sizeof(real_T)) << 4U);

  /* 'init_KalmanFilter_16State:45' KalmanFilterstate.A = zeros(NumberKalmanFilterStates,NumberKalmanFilterStates); */
  /* 'init_KalmanFilter_16State:46' KalmanFilterstate.P = zeros(NumberKalmanFilterStates,NumberKalmanFilterStates); */
  /* 'init_KalmanFilter_16State:47' KalmanFilterstate.Q = zeros(NumberKalmanFilterStates,NumberKalmanFilterStates); */
  memset(&KalmanFilterstate_A[0], 0, (sizeof(real_T)) << 8U);
  memset(&KalmanFilterstate_P[0], 0, (sizeof(real_T)) << 8U);
  memset(&KalmanFilterstate_Q[0], 0, (sizeof(real_T)) << 8U);

  /* 'init_KalmanFilter_16State:49' KalmanFilterstate.P( 1, 1) = (100.0)^2; */
  KalmanFilterstate_P[0] = 10000.0;

  /* 'init_KalmanFilter_16State:50' KalmanFilterstate.P( 2, 2) = (100.0)^2; */
  KalmanFilterstate_P[17] = 10000.0;

  /* 'init_KalmanFilter_16State:51' KalmanFilterstate.P( 3, 3) = (100.0)^2; */
  KalmanFilterstate_P[34] = 10000.0;

  /* 'init_KalmanFilter_16State:52' KalmanFilterstate.P( 4, 4) = (2.5)^2; */
  KalmanFilterstate_P[51] = 6.25;

  /* 'init_KalmanFilter_16State:53' KalmanFilterstate.P( 5, 5) = (2.5)^2; */
  KalmanFilterstate_P[68] = 6.25;

  /* 'init_KalmanFilter_16State:54' KalmanFilterstate.P( 6, 6) = (2.5)^2; */
  KalmanFilterstate_P[85] = 6.25;

  /* 'init_KalmanFilter_16State:55' KalmanFilterstate.P( 7, 7) = (10.0*d2r)^2; */
  KalmanFilterstate_P[102] = 0.030461741978670857;

  /* 'init_KalmanFilter_16State:56' KalmanFilterstate.P( 8, 8) = (10.0*d2r)^2; */
  KalmanFilterstate_P[119] = 0.030461741978670857;

  /* 'init_KalmanFilter_16State:57' KalmanFilterstate.P( 9, 9) = (10.0*d2r)^2; */
  KalmanFilterstate_P[136] = 0.030461741978670857;

  /* 'init_KalmanFilter_16State:58' KalmanFilterstate.P(10,10) = (120.0*dph2rps)^2; */
  KalmanFilterstate_P[153] = 3.3846379976300949E-7;

  /* 'init_KalmanFilter_16State:59' KalmanFilterstate.P(11,11) = (120.0*dph2rps)^2; */
  KalmanFilterstate_P[170] = 3.3846379976300949E-7;

  /* 'init_KalmanFilter_16State:60' KalmanFilterstate.P(12,12) = (120.0*dph2rps)^2; */
  KalmanFilterstate_P[187] = 3.3846379976300949E-7;

  /* 'init_KalmanFilter_16State:61' KalmanFilterstate.P(13,13) = (50.0*mg2mps2)^2; */
  KalmanFilterstate_P[204] = 0.24059025000000003;

  /* 'init_KalmanFilter_16State:62' KalmanFilterstate.P(14,14) = (50.0*mg2mps2)^2; */
  KalmanFilterstate_P[221] = 0.24059025000000003;

  /* 'init_KalmanFilter_16State:63' KalmanFilterstate.P(15,15) = (50.0*mg2mps2)^2; */
  KalmanFilterstate_P[238] = 0.24059025000000003;

  /* 'init_KalmanFilter_16State:64' KalmanFilterstate.P(16,16) = (100.0)^2; */
  KalmanFilterstate_P[255] = 10000.0;

  /* 'init_KalmanFilter_16State:66' KalmanFilterstate.Q( 1, 1) = (2.5)^2; */
  KalmanFilterstate_Q[0] = 6.25;

  /* 'init_KalmanFilter_16State:67' KalmanFilterstate.Q( 2, 2) = (2.5)^2; */
  KalmanFilterstate_Q[17] = 6.25;

  /* 'init_KalmanFilter_16State:68' KalmanFilterstate.Q( 3, 3) = (2.5)^2; */
  KalmanFilterstate_Q[34] = 6.25;

  /* 'init_KalmanFilter_16State:69' KalmanFilterstate.Q( 4, 4) = (0.1)^2; */
  KalmanFilterstate_Q[51] = 0.010000000000000002;

  /* 'init_KalmanFilter_16State:70' KalmanFilterstate.Q( 5, 5) = (0.1)^2; */
  KalmanFilterstate_Q[68] = 0.010000000000000002;

  /* 'init_KalmanFilter_16State:71' KalmanFilterstate.Q( 6, 6) = (0.1)^2; */
  KalmanFilterstate_Q[85] = 0.010000000000000002;

  /* 'init_KalmanFilter_16State:72' KalmanFilterstate.Q( 7, 7) = (0.1*d2r)^2; */
  KalmanFilterstate_Q[102] = 3.0461741978670861E-6;

  /* 'init_KalmanFilter_16State:73' KalmanFilterstate.Q( 8, 8) = (0.1*d2r)^2; */
  KalmanFilterstate_Q[119] = 3.0461741978670861E-6;

  /* 'init_KalmanFilter_16State:74' KalmanFilterstate.Q( 9, 9) = (0.1*d2r)^2; */
  KalmanFilterstate_Q[136] = 3.0461741978670861E-6;

  /* 'init_KalmanFilter_16State:75' KalmanFilterstate.Q(10,10) = (5.0*dph2rps)^2; */
  KalmanFilterstate_Q[153] = 5.8761076347744711E-10;

  /* 'init_KalmanFilter_16State:76' KalmanFilterstate.Q(11,11) = (5.0*dph2rps)^2; */
  KalmanFilterstate_Q[170] = 5.8761076347744711E-10;

  /* 'init_KalmanFilter_16State:77' KalmanFilterstate.Q(12,12) = (5.0*dph2rps)^2; */
  KalmanFilterstate_Q[187] = 5.8761076347744711E-10;

  /* 'init_KalmanFilter_16State:78' KalmanFilterstate.Q(13,13) = (0.2*mg2mps2)^2; */
  KalmanFilterstate_Q[204] = 3.8494440000000012E-6;

  /* 'init_KalmanFilter_16State:79' KalmanFilterstate.Q(14,14) = (0.2*mg2mps2)^2; */
  KalmanFilterstate_Q[221] = 3.8494440000000012E-6;

  /* 'init_KalmanFilter_16State:80' KalmanFilterstate.Q(15,15) = (0.2*mg2mps2)^2; */
  KalmanFilterstate_Q[238] = 3.8494440000000012E-6;

  /* 'init_KalmanFilter_16State:81' KalmanFilterstate.Q(16,16) = (2.5)^2; */
  KalmanFilterstate_Q[255] = 6.25;

  /* 'init_KalmanFilter_16State:83' KalmanFilterstate.R = CreateMeasurementNoiseStruct; */
  /* 'init_KalmanFilter_16State:97' d2r = pi/180.0; */
  /* 'init_KalmanFilter_16State:98' v.range        = (10.0)^2; */
  KalmanFilterstate_R->range = 100.0;

  /* 'init_KalmanFilter_16State:99' v.rrate        = (0.5)^2; */
  KalmanFilterstate_R->rrate = 0.25;

  /* 'init_KalmanFilter_16State:100' v.ta_position  = (10.0)^2; */
  KalmanFilterstate_R->ta_position = 100.0;

  /* 'init_KalmanFilter_16State:101' v.ta_velocity  = (0.5)^2; */
  KalmanFilterstate_R->ta_velocity = 0.25;

  /* 'init_KalmanFilter_16State:102' v.ta_attitude  = (0.5*d2r)^2; */
  KalmanFilterstate_R->ta_attitude = 7.6154354946677142E-5;

  /* 'init_KalmanFilter_16State:103' v.mag_attitude = (2.0*d2r)^2; */
  KalmanFilterstate_R->mag_attitude = 0.0012184696791468343;

  /* 'init_KalmanFilter_16State:104' v.baroG        = (10.0)^2; */
  KalmanFilterstate_R->baroG = 100.0;

  /* 'init_KalmanFilter_16State:105' v.baro_P_N     = (10.0)^2; */
  KalmanFilterstate_R->baro_P_N = 100.0;

  /* 'init_KalmanFilter_16State:108' coder.cstructname(v, 'T_NAV_MeasurementNoise'); */
  /* 'init_KalmanFilter_16State:85' KalmanFilterstate.B_gyro  = zeros(3,1); */
  /* 'init_KalmanFilter_16State:86' KalmanFilterstate.B_accel = zeros(3,1); */
  KalmanFilterstate_B_gyro[0] = 0.0;
  KalmanFilterstate_B_accel[0] = 0.0;
  KalmanFilterstate_B_gyro[1] = 0.0;
  KalmanFilterstate_B_accel[1] = 0.0;
  KalmanFilterstate_B_gyro[2] = 0.0;
  KalmanFilterstate_B_accel[2] = 0.0;

  /* 'init_KalmanFilter_16State:89' KalmanFilterstate.align_valid = uint8(0); */
  *KalmanFilterstate_align_valid = 0U;

  /* 'init_KalmanFilter_16State:92' coder.cstructname(KalmanFilterstate, 'T_NAV_KalmanFilterState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function baro = init_Baro
 */
static void init_Baro(real_T *baro_SystemTime, real_T *baro_alt, real_T
                      *baro_pressure, uint8_T *baro_valid, uint8_T
                      *baro_Reserved01, uint16_T *baro_Reserved02, real_T
                      *baro_comp_alt, real_T *baro_KalmanFilter_delta_baro)
{
  /* 'init_Baro:29' baro.SystemTime = 0.0; */
  *baro_SystemTime = 0.0;

  /* 'init_Baro:32' baro.alt = 0.0; */
  *baro_alt = 0.0;

  /* 'init_Baro:33' baro.pressure = 0.0; */
  *baro_pressure = 0.0;

  /* 'init_Baro:36' baro.valid = uint8(0); */
  *baro_valid = 0U;

  /* 'init_Baro:39' baro.Reserved01 = uint8(0); */
  *baro_Reserved01 = 0U;

  /* 'init_Baro:40' baro.Reserved02 = uint16(0); */
  *baro_Reserved02 = 0U;

  /* 'init_Baro:43' baro.comp_alt = 0.0; */
  *baro_comp_alt = 0.0;

  /* 'init_Baro:46' baro.KalmanFilter_delta_baro = 0.0; */
  *baro_KalmanFilter_delta_baro = 0.0;

  /* 'init_Baro:49' coder.cstructname(baro, 'T_NAV_BAROState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function pylon = init_Pylon
 */
static void init_Pylon(real_T *pylon_attitude_phi, real_T *pylon_attitude_the,
  real_T *pylon_attitude_psi, real_T *pylon_accel_limit_low, real_T
  *pylon_accel_limit_high, uint8_T *pylon_valid, uint8_T
  *pylon_initial_attitude_set)
{
  /* 'init_Pylon:26' pylon.attitude.phi = 0.0; */
  *pylon_attitude_phi = 0.0;

  /* 'init_Pylon:27' pylon.attitude.the = 0.0; */
  *pylon_attitude_the = 0.0;

  /* 'init_Pylon:28' pylon.attitude.psi = 0.0; */
  *pylon_attitude_psi = 0.0;

  /* 'init_Pylon:31' pylon.accel_limit_low = 8.829; */
  *pylon_accel_limit_low = 8.829;

  /* 'init_Pylon:32' pylon.accel_limit_high = 10.791; */
  *pylon_accel_limit_high = 10.791;

  /* 'init_Pylon:35' pylon.valid = uint8(0); */
  *pylon_valid = 0U;

  /* 'init_Pylon:38' pylon.initial_attitude_set = uint8(0); */
  *pylon_initial_attitude_set = 0U;

  /* 'init_Pylon:41' coder.cstructname(pylon, 'T_TA_PylonState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function atmos_out = LIB_StandardAtmospherePressure(pressure)
 */
static void LIB_StandardAtmospherePressure(real_T pressure, real_T
  *atmos_out_theta, real_T *atmos_out_delta, real_T *atmos_out_sigma, real_T
  *atmos_out_temperature, real_T *atmos_out_altitudeGeo, real_T
  *atmos_out_altitude, real_T *atmos_out_density)
{
  real_T b_index;
  real_T j;
  real_T k;
  static const real_T b[8] = { -6.5, 0.0, 1.0, 2.8, 0.0, -2.8, -2.0, 0.0 };

  static const real_T c[8] = { 288.15, 216.65, 216.65, 228.65, 270.65, 270.65,
    214.65, 186.946 };

  static const real_T ptab[8] = { 1.0, 0.2233611, 0.05403295, 0.0085666784,
    0.0010945601, 0.00066063531, 3.9046834E-5, 3.68501E-6 };

  static const real_T d[8] = { 0.0, 11.0, 20.0, 32.0, 47.0, 51.0, 71.0, 84.852 };

  static const real_T e[8] = { 1.0, 0.2233611, 0.05403295, 0.0085666784,
    0.0010945601, 0.00066063531, 3.9046834E-5, 3.68501E-6 };

  int32_T tmp;

  /* 'LIB_StandardAtmospherePressure:49' REARTH = 6369.0; */
  /* 'LIB_StandardAtmospherePressure:50' GMR = 34.163195; */
  /* 'LIB_StandardAtmospherePressure:51' NTAB = 8; */
  /* 'LIB_StandardAtmospherePressure:52' P0 = 1.01325; */
  /* 'LIB_StandardAtmospherePressure:53' T0 = 288.15; */
  /* 'LIB_StandardAtmospherePressure:54' RHO0 = 1.225; */
  /* 'LIB_StandardAtmospherePressure:58' p = pressure/(100000*P0); */
  *atmos_out_delta = pressure / 101325.0;

  /* 'LIB_StandardAtmospherePressure:61' htab = [0.0; 11.0; 20.0; 32.0; 47.0; 51.0; 71.0; 84.852]; */
  /* 'LIB_StandardAtmospherePressure:62' ttab = [288.15; 216.65; 216.65; 228.65; 270.65; 270.65; 214.65; 186.946]; */
  /* 'LIB_StandardAtmospherePressure:63' ptab = [1.0; 2.233611e-1; 5.403295e-2; 8.5666784e-3; 1.0945601e-3; 6.6063531e-4; 3.9046834e-5; 3.68501e-6]; */
  /* 'LIB_StandardAtmospherePressure:64' gtab = [-6.5; 0.0; 1.0; 2.8; 0.0; -2.8; -2.0; 0.0]; */
  /* 'LIB_StandardAtmospherePressure:68' index = 1.0; */
  b_index = 1.0;

  /* 'LIB_StandardAtmospherePressure:69' j = NTAB; */
  j = 8.0;

  /* 'LIB_StandardAtmospherePressure:70' while ((index+1) < j) */
  while ((b_index + 1.0) < j) {
    /* 'LIB_StandardAtmospherePressure:71' k = floor((index+j)/2); */
    k = floor((b_index + j) / 2.0);

    /* 'LIB_StandardAtmospherePressure:72' if (p > ptab(k)) */
    if ((*atmos_out_delta) > ptab[((int32_T)k) - 1]) {
      /* 'LIB_StandardAtmospherePressure:73' j = k; */
      j = k;
    } else {
      /* 'LIB_StandardAtmospherePressure:74' else */
      /* 'LIB_StandardAtmospherePressure:75' index = k; */
      b_index = k;
    }
  }

  /* 'LIB_StandardAtmospherePressure:79' tgrad = gtab(index,1); */
  /* 'LIB_StandardAtmospherePressure:80' tbase = ttab(index,1); */
  /* 'LIB_StandardAtmospherePressure:83' if tgrad == 0.0 */
  tmp = ((int32_T)b_index) - 1;
  if (b[tmp] == 0.0) {
    /* 'LIB_StandardAtmospherePressure:84' hGeo = htab(index,1)-log(p/ptab(index,1))*tbase/GMR; */
    *atmos_out_altitudeGeo = d[tmp] - ((log((*atmos_out_delta) / e[tmp]) * c[tmp])
      / 34.163195);
  } else {
    /* 'LIB_StandardAtmospherePressure:85' else */
    /* 'LIB_StandardAtmospherePressure:86' hGeo = htab(index,1)+((p/ptab(index,1))^(tgrad/(-1.0*GMR))*tbase - tbase)/tgrad; */
    *atmos_out_altitudeGeo = (((rt_powd_snf((*atmos_out_delta) / e[((int32_T)
      b_index) - 1], b[((int32_T)b_index) - 1] / -34.163195) * c[((int32_T)
      b_index) - 1]) - c[((int32_T)b_index) - 1]) / b[((int32_T)b_index) - 1]) +
      d[((int32_T)b_index) - 1];
  }

  /* 'LIB_StandardAtmospherePressure:89' deltah = hGeo-htab(index,1); */
  /* 'LIB_StandardAtmospherePressure:90' tlocal = tbase+tgrad*deltah; */
  *atmos_out_temperature = (((*atmos_out_altitudeGeo) - d[((int32_T)b_index) - 1])
    * b[((int32_T)b_index) - 1]) + c[((int32_T)b_index) - 1];

  /* 'LIB_StandardAtmospherePressure:91' atmos_out.theta = tlocal/T0; */
  *atmos_out_theta = (*atmos_out_temperature) / 288.15;

  /* 'LIB_StandardAtmospherePressure:92' atmos_out.delta = p; */
  /* 'LIB_StandardAtmospherePressure:93' atmos_out.sigma = p/(tlocal/T0); */
  *atmos_out_sigma = (*atmos_out_delta) / ((*atmos_out_temperature) / 288.15);

  /* 'LIB_StandardAtmospherePressure:94' atmos_out.temperature = tlocal; */
  /* 'LIB_StandardAtmospherePressure:95' atmos_out.altitudeGeo = hGeo; */
  /* 'LIB_StandardAtmospherePressure:96' atmos_out.altitude = 1000.0*hGeo*REARTH/(REARTH-hGeo); */
  *atmos_out_altitude = ((1000.0 * (*atmos_out_altitudeGeo)) * 6369.0) / (6369.0
    - (*atmos_out_altitudeGeo));

  /* 'LIB_StandardAtmospherePressure:97' atmos_out.density = (p/(tlocal/T0))*RHO0; */
  *atmos_out_density = (*atmos_out_sigma) * 1.225;
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function pState = INS_InertialNavigation(time, w, f, pState, baro_alt)
 */
static void INS_InertialNavigation(real_T time, real_T w_x, real_T w_y, real_T
  w_z, real_T f_x, real_T f_y, real_T f_z, T_NAV_NavigationState *pState, real_T
  baro_alt)
{
  real_T dcm_eb[9];
  real_T Wie_e[9];
  real_T Wie_b[9];
  real_T dAngle_b[3];
  real_T acc_e[3];
  real_T b_x;
  real_T b_y;
  real_T b_z;
  real_T cossigma;
  real_T qtemp[4];
  real_T scale;
  real_T absxk;
  real_T t;
  real_T sin_lat;
  real_T cos_lat;
  real_T sin_2lon;
  real_T cos_2lon;
  real_T P22;
  real_T dcm_eb_0[9];
  real_T cossigma_0[16];
  real_T pState_0[3];
  real_T pState_1[3];
  int32_T i;
  int32_T k;
  real_T dAngle_b_0;
  int32_T dcm_eb_tmp;
  real_T t_tmp;

  /* 'INS_InertialNavigation:34' pState.systemTime = time; */
  pState->systemTime = time;

  /* 'INS_InertialNavigation:39' dcm_eb = transpose(pState.dcm_be); */
  /* 'INS_InertialNavigation:45' Wie_e = [0, -pState.WE, 0; pState.WE, 0, 0; 0, 0, 0]; */
  Wie_e[0] = 0.0;
  Wie_e[3] = -pState->WE;
  Wie_e[6] = 0.0;
  Wie_e[1] = pState->WE;
  Wie_e[4] = 0.0;
  Wie_e[7] = 0.0;
  for (k = 0; k < 3; k++) {
    dcm_eb[3 * k] = pState->dcm_be[k];
    dcm_eb[1 + (3 * k)] = pState->dcm_be[k + 3];
    dcm_eb_tmp = 2 + (3 * k);
    dcm_eb[dcm_eb_tmp] = pState->dcm_be[k + 6];
    Wie_e[dcm_eb_tmp] = 0.0;
  }

  /* 'INS_InertialNavigation:46' Wie_b = dcm_eb*Wie_e*dcm_eb'; */
  for (k = 0; k < 3; k++) {
    for (i = 0; i < 3; i++) {
      dcm_eb_tmp = k + (3 * i);
      dcm_eb_0[dcm_eb_tmp] = 0.0;
      dcm_eb_0[dcm_eb_tmp] += Wie_e[3 * i] * dcm_eb[k];
      dcm_eb_0[dcm_eb_tmp] += Wie_e[(3 * i) + 1] * dcm_eb[k + 3];
      dcm_eb_0[dcm_eb_tmp] += Wie_e[(3 * i) + 2] * dcm_eb[k + 6];
    }

    for (i = 0; i < 3; i++) {
      dcm_eb_tmp = k + (3 * i);
      Wie_b[dcm_eb_tmp] = 0.0;
      Wie_b[dcm_eb_tmp] += dcm_eb_0[k] * dcm_eb[i];
      Wie_b[dcm_eb_tmp] += dcm_eb_0[k + 3] * dcm_eb[i + 3];
      Wie_b[dcm_eb_tmp] += dcm_eb_0[k + 6] * dcm_eb[i + 6];
    }
  }

  /* 'INS_InertialNavigation:47' vector_wie_b = [Wie_b(3,2); Wie_b(1,3); Wie_b(2,1)]; */
  /* 'INS_InertialNavigation:51' dAngle_b = [w.x; w.y; w.z] - vector_wie_b*pState.deltaT; */
  cossigma = Wie_b[5] * pState->deltaT;
  sin_lat = Wie_b[6] * pState->deltaT;
  cos_lat = Wie_b[1] * pState->deltaT;

  /* 'INS_InertialNavigation:52' pState.wib_b = struct('x', (w.x/pState.deltaT), 'y', (w.y/pState.deltaT), 'z', (w.z/pState.deltaT)); */
  b_x = w_x / pState->deltaT;
  b_y = w_y / pState->deltaT;
  b_z = w_z / pState->deltaT;
  pState->wib_b.x = b_x;
  pState->wib_b.y = b_y;
  pState->wib_b.z = b_z;

  /* 'INS_InertialNavigation:55' pState.q_be = LIB_propagateQuaternion(dAngle_b, pState.q_be); */
  /* 'LIB_propagateQuaternion:27' sigma = 0.5 * norm(deltaAngle); */
  scale = 3.3121686421112381E-170;
  dAngle_b_0 = w_x - cossigma;
  absxk = fabs(dAngle_b_0);
  if (absxk > 3.3121686421112381E-170) {
    cossigma = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    cossigma = t * t;
  }

  dAngle_b[0] = dAngle_b_0;
  dAngle_b_0 = w_y - sin_lat;
  absxk = fabs(dAngle_b_0);
  if (absxk > scale) {
    t = scale / absxk;
    cossigma = ((cossigma * t) * t) + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    cossigma += t * t;
  }

  dAngle_b[1] = dAngle_b_0;
  dAngle_b_0 = w_z - cos_lat;
  absxk = fabs(dAngle_b_0);
  if (absxk > scale) {
    t = scale / absxk;
    cossigma = ((cossigma * t) * t) + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    cossigma += t * t;
  }

  cossigma = scale * sqrt(cossigma);
  scale = 0.5 * cossigma;

  /* 'LIB_propagateQuaternion:29' if (abs(sigma) < eps) */
  if (scale < 2.2204460492503131E-16) {
    /* 'LIB_propagateQuaternion:30' cossigma = 1; */
    cossigma = 1.0;

    /* 'LIB_propagateQuaternion:31' sinsigma = 0.5; */
    scale = 0.5;
  } else {
    /* 'LIB_propagateQuaternion:32' else */
    /* 'LIB_propagateQuaternion:33' cossigma = cos(sigma); */
    cossigma = cos(scale);

    /* 'LIB_propagateQuaternion:34' sinsigma = sin(sigma)/(2.0*sigma); */
    scale = sin(scale) / (2.0 * scale);
  }

  /* 'LIB_propagateQuaternion:38' propMatrix = [                cossigma, -deltaAngle(1,1)*sinsigma, -deltaAngle(2,1)*sinsigma, -deltaAngle(3,1)*sinsigma; ... */
  /* 'LIB_propagateQuaternion:39'                 deltaAngle(1,1)*sinsigma,                  cossigma,  deltaAngle(3,1)*sinsigma, -deltaAngle(2,1)*sinsigma; ... */
  /* 'LIB_propagateQuaternion:40'                 deltaAngle(2,1)*sinsigma, -deltaAngle(3,1)*sinsigma,                  cossigma,  deltaAngle(1,1)*sinsigma; ... */
  /* 'LIB_propagateQuaternion:41'                 deltaAngle(3,1)*sinsigma,  deltaAngle(2,1)*sinsigma, -deltaAngle(1,1)*sinsigma,                  cossigma]; */
  /* 'LIB_propagateQuaternion:43' qtemp = propMatrix * [q.q0; ... */
  /* 'LIB_propagateQuaternion:44'                         q.q1; ... */
  /* 'LIB_propagateQuaternion:45'                         q.q2; ... */
  /* 'LIB_propagateQuaternion:46'                         q.q3]; */
  cossigma_0[0] = cossigma;
  absxk = (-dAngle_b[0]) * scale;
  cossigma_0[4] = absxk;
  t = (-dAngle_b[1]) * scale;
  cossigma_0[8] = t;
  sin_lat = (-dAngle_b_0) * scale;
  cossigma_0[12] = sin_lat;
  cos_lat = dAngle_b[0] * scale;
  cossigma_0[1] = cos_lat;
  cossigma_0[5] = cossigma;
  dAngle_b_0 *= scale;
  cossigma_0[9] = dAngle_b_0;
  cossigma_0[13] = t;
  t = dAngle_b[1] * scale;
  cossigma_0[2] = t;
  cossigma_0[6] = sin_lat;
  cossigma_0[10] = cossigma;
  cossigma_0[14] = cos_lat;
  cossigma_0[3] = dAngle_b_0;
  cossigma_0[7] = t;
  cossigma_0[11] = absxk;
  cossigma_0[15] = cossigma;

  /* 'LIB_propagateQuaternion:49' qtemp = qtemp / norm(qtemp); */
  cossigma = 0.0;
  scale = 3.3121686421112381E-170;
  for (k = 0; k < 4; k++) {
    dAngle_b_0 = (cossigma_0[k + 12] * pState->q_be.q3) + ((cossigma_0[k + 8] *
      pState->q_be.q2) + ((cossigma_0[k + 4] * pState->q_be.q1) + (cossigma_0[k]
      * pState->q_be.q0)));
    absxk = fabs(dAngle_b_0);
    if (absxk > scale) {
      t = scale / absxk;
      cossigma = ((cossigma * t) * t) + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      cossigma += t * t;
    }

    qtemp[k] = dAngle_b_0;
  }

  cossigma = scale * sqrt(cossigma);
  qtemp[0] /= cossigma;
  qtemp[1] /= cossigma;
  qtemp[2] /= cossigma;
  dAngle_b_0 = qtemp[3] / cossigma;

  /* 'LIB_propagateQuaternion:51' q.q0 = qtemp(1,1); */
  pState->q_be.q0 = qtemp[0];

  /* 'LIB_propagateQuaternion:52' q.q1 = qtemp(2,1); */
  pState->q_be.q1 = qtemp[1];

  /* 'LIB_propagateQuaternion:53' q.q2 = qtemp(3,1); */
  pState->q_be.q2 = qtemp[2];

  /* 'LIB_propagateQuaternion:54' q.q3 = qtemp(4,1); */
  pState->q_be.q3 = dAngle_b_0;

  /* 'INS_InertialNavigation:58' pState.dcm_be = LIB_q2d(pState.q_be); */
  LIB_q2d(pState->q_be.q0, pState->q_be.q1, pState->q_be.q2, pState->q_be.q3,
          pState->dcm_be);

  /* 'INS_InertialNavigation:67' fib_b = [f.x/pState.deltaT; f.y/pState.deltaT; f.z/pState.deltaT]; */
  /* 'INS_InertialNavigation:70' fib_e = pState.dcm_be*fib_b; */
  cossigma = f_x / pState->deltaT;
  scale = f_y / pState->deltaT;
  absxk = f_z / pState->deltaT;
  for (k = 0; k < 3; k++) {
    dAngle_b[k] = (pState->dcm_be[k + 6] * absxk) + ((pState->dcm_be[k + 3] *
      scale) + (pState->dcm_be[k] * cossigma));
  }

  /* 'INS_InertialNavigation:73' pState.grav_e = LIB_EarthGravity(pState.pos_e); */
  /* 'LIB_EarthGravity:38' RE = 6378137.0; */
  /* 'LIB_EarthGravity:39' GM = 3.986004418e+14; */
  /* 'LIB_EarthGravity:42' C20  = -4.841651437908150e-4; */
  /* 'LIB_EarthGravity:43' C22  =  2.439383573283130e-6; */
  /* 'LIB_EarthGravity:44' S22  = -1.400273703859340e-6; */
  /* 'LIB_EarthGravity:49' z2 = re.z*re.z; */
  /* 'LIB_EarthGravity:50' p2 = re.x*re.x + re.y*re.y; */
  cossigma = (pState->pos_e.x * pState->pos_e.x) + (pState->pos_e.y *
    pState->pos_e.y);

  /* 'LIB_EarthGravity:51' r2 = p2 + z2; */
  scale = (pState->pos_e.z * pState->pos_e.z) + cossigma;

  /* 'LIB_EarthGravity:52' r  = sqrt(r2); */
  absxk = sqrt(scale);

  /* 'LIB_EarthGravity:57' lat = asin(re.z/r); */
  /* 'LIB_EarthGravity:58' lon = atan2(re.y,re.x); */
  t = rt_atan2d_snf(pState->pos_e.y, pState->pos_e.x);

  /* 'LIB_EarthGravity:63' sin_lat = re.z/r; */
  cos_lat = pState->pos_e.z / absxk;
  sin_lat = cos_lat;

  /* 'LIB_EarthGravity:64' if (abs(sin_lat) < eps) */
  if (fabs(cos_lat) < 2.2204460492503131E-16) {
    /* 'LIB_EarthGravity:65' sin_lat = 0.0; */
    sin_lat = 0.0;
  }

  /* 'LIB_EarthGravity:67' cos_lat = cos(lat); */
  cos_lat = cos(asin(cos_lat));

  /* 'LIB_EarthGravity:68' if (abs(cos_lat) < eps) */
  if (fabs(cos_lat) < 2.2204460492503131E-16) {
    /* 'LIB_EarthGravity:69' cos_lat = 0.0; */
    cos_lat = 0.0;
  }

  /* 'LIB_EarthGravity:71' sin_2lon = sin(2.0*lon); */
  sin_2lon = sin(2.0 * t);

  /* 'LIB_EarthGravity:72' cos_2lon = cos(2.0*lon); */
  cos_2lon = cos(2.0 * t);

  /* 'LIB_EarthGravity:77' P20  =  sqrt(5.0)*(3.0*sin_lat*sin_lat - 1.0)/2.0; */
  /* 'LIB_EarthGravity:78' P22  =  sqrt(5.0/12.0)*3.0*cos_lat*cos_lat; */
  P22 = (1.9364916731037085 * cos_lat) * cos_lat;

  /* 'LIB_EarthGravity:79' dP20 =  sqrt(5.0)*3.0*sin_lat*cos_lat; */
  /* 'LIB_EarthGravity:80' dP22 = -sqrt(5.0/12.0)*6.0*sin_lat*cos_lat; */
  /* 'LIB_EarthGravity:85' dV_dr   = -GM/r2*(1.0 + 3.0*RE*RE/r2*(C20*P20  + (C22*cos_2lon + S22*sin_2lon)*P22)); */
  t_tmp = (2.43938357328313E-6 * cos_2lon) + (-1.40027370385934E-6 * sin_2lon);
  t = (((((((((3.0 * sin_lat) * sin_lat) - 1.0) * 2.23606797749979) / 2.0) *
          -0.000484165143790815) + (t_tmp * P22)) * (1.22041894772307E+14 /
         scale)) + 1.0) * (-3.986004418E+14 / scale);

  /* 'LIB_EarthGravity:86' dV_dLat =  GM/r *(          RE*RE/r2*(C20*dP20 + (C22*cos_2lon + S22*sin_2lon)*dP22)); */
  cos_lat = (((((6.7082039324993694 * sin_lat) * cos_lat) *
               -0.000484165143790815) + (t_tmp * ((-3.872983346207417 * sin_lat)
    * cos_lat))) * (4.0680631590769E+13 / scale)) * (3.986004418E+14 / absxk);

  /* 'LIB_EarthGravity:87' dV_dLon =  GM/r *(          RE*RE/r2*(      2.0*(-C22*sin_2lon + S22*cos_2lon)*P22)); */
  sin_2lon = (((((-2.43938357328313E-6 * sin_2lon) + (-1.40027370385934E-6 *
    cos_2lon)) * 2.0) * P22) * (4.0680631590769E+13 / scale)) * (3.986004418E+14
    / absxk);

  /* 'LIB_EarthGravity:92' if (p2 > 1.0e-3) */
  if (cossigma > 0.001) {
    /* 'LIB_EarthGravity:93' p = sqrt(p2); */
    sin_lat = sqrt(cossigma);

    /* 'LIB_EarthGravity:94' tmp = dV_dLat*re.z/(r2*p); */
    cos_2lon = (cos_lat * pState->pos_e.z) / (scale * sin_lat);

    /* 'LIB_EarthGravity:95' gravity_e.x = dV_dr*re.x/r - tmp*re.x - dV_dLon*re.y/p2; */
    pState->grav_e.x = (((t * pState->pos_e.x) / absxk) - (cos_2lon *
      pState->pos_e.x)) - ((sin_2lon * pState->pos_e.y) / cossigma);

    /* 'LIB_EarthGravity:96' gravity_e.y = dV_dr*re.y/r - tmp*re.y + dV_dLon*re.x/p2; */
    pState->grav_e.y = (((t * pState->pos_e.y) / absxk) - (cos_2lon *
      pState->pos_e.y)) + ((sin_2lon * pState->pos_e.x) / cossigma);

    /* 'LIB_EarthGravity:97' gravity_e.z = dV_dr*re.z/r + (dV_dLat*p/r2); */
    pState->grav_e.z = ((t * pState->pos_e.z) / absxk) + ((cos_lat * sin_lat) /
      scale);
  } else {
    /* 'LIB_EarthGravity:100' else */
    /* 'LIB_EarthGravity:101' gravity_e.x = 0; */
    pState->grav_e.x = 0.0;

    /* 'LIB_EarthGravity:102' gravity_e.y = 0; */
    pState->grav_e.y = 0.0;

    /* 'LIB_EarthGravity:103' gravity_e.z = dV_dr*sin_lat; */
    pState->grav_e.z = t * sin_lat;
  }

  /* 'INS_InertialNavigation:76' acc_e =  fib_e + [pState.grav_e.x; pState.grav_e.y; pState.grav_e.z] - ... */
  /* 'INS_InertialNavigation:77'           (2.0*Wie_e*[pState.vel_e.x; pState.vel_e.y; pState.vel_e.z]) - ... */
  /* 'INS_InertialNavigation:78'           (Wie_e*Wie_e*[pState.pos_e.x; pState.pos_e.y; pState.pos_e.z]); */
  pState_0[0] = pState->grav_e.x;
  pState_0[1] = pState->grav_e.y;
  pState_0[2] = pState->grav_e.z;
  pState_1[0] = pState->vel_e.x;
  pState_1[1] = pState->vel_e.y;
  pState_1[2] = pState->vel_e.z;
  for (k = 0; k < 3; k++) {
    cossigma = 0.0;
    for (i = 0; i < 3; i++) {
      dcm_eb_tmp = k + (3 * i);
      dcm_eb[dcm_eb_tmp] = 0.0;
      dcm_eb[dcm_eb_tmp] += Wie_e[3 * i] * Wie_e[k];
      dcm_eb[dcm_eb_tmp] += Wie_e[(3 * i) + 1] * Wie_e[k + 3];
      dcm_eb[dcm_eb_tmp] += Wie_e[(3 * i) + 2] * 0.0;
      cossigma += (Wie_e[dcm_eb_tmp] * 2.0) * pState_1[i];
    }

    acc_e[k] = ((dAngle_b[k] + pState_0[k]) - cossigma) - ((dcm_eb[k + 6] *
      pState->pos_e.z) + ((dcm_eb[k + 3] * pState->pos_e.y) + (dcm_eb[k] *
      pState->pos_e.x)));
  }

  /* 'INS_InertialNavigation:79' pState.fib_e.x = fib_e(1,1); */
  pState->fib_e.x = dAngle_b[0];

  /* 'INS_InertialNavigation:80' pState.fib_e.y = fib_e(2,1); */
  pState->fib_e.y = dAngle_b[1];

  /* 'INS_InertialNavigation:81' pState.fib_e.z = fib_e(3,1); */
  pState->fib_e.z = dAngle_b[2];

  /* 'INS_InertialNavigation:84' pos_e = [pState.pos_e.x; pState.pos_e.y; pState.pos_e.z] + ... */
  /* 'INS_InertialNavigation:85'           [pState.vel_e.x; pState.vel_e.y; pState.vel_e.z]*pState.deltaT + (0.5*acc_e*pState.deltaT*pState.deltaT); */
  dAngle_b[0] = (((0.5 * acc_e[0]) * pState->deltaT) * pState->deltaT) +
    ((pState->vel_e.x * pState->deltaT) + pState->pos_e.x);
  dAngle_b[1] = (((0.5 * acc_e[1]) * pState->deltaT) * pState->deltaT) +
    ((pState->vel_e.y * pState->deltaT) + pState->pos_e.y);
  dAngle_b[2] = (((0.5 * acc_e[2]) * pState->deltaT) * pState->deltaT) +
    ((pState->vel_e.z * pState->deltaT) + pState->pos_e.z);

  /* 'INS_InertialNavigation:86' pState.pos_e.x = pos_e(1,1); */
  pState->pos_e.x = dAngle_b[0];

  /* 'INS_InertialNavigation:87' pState.pos_e.y = pos_e(2,1); */
  pState->pos_e.y = dAngle_b[1];

  /* 'INS_InertialNavigation:88' pState.pos_e.z = pos_e(3,1); */
  pState->pos_e.z = dAngle_b[2];

  /* 'INS_InertialNavigation:89' pState.pos_g = LIB_ecef2lla(pState.pos_e); */
  LIB_ecef2lla(pState->pos_e.x, pState->pos_e.y, pState->pos_e.z,
               &pState->pos_g.lat, &pState->pos_g.lon, &pState->pos_g.alt);

  /* 'INS_InertialNavigation:90' dcm_en = LIB_dcm_en(pState.pos_g); */
  /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
  /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
  /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
  /* 'INS_InertialNavigation:91' pState.baro_alt = baro_alt; */
  pState->baro_alt = baro_alt;

  /* 'INS_InertialNavigation:94' vel_e = [pState.vel_e.x; pState.vel_e.y; pState.vel_e.z] + (acc_e*pState.deltaT); */
  scale = (acc_e[0] * pState->deltaT) + pState->vel_e.x;
  absxk = (acc_e[1] * pState->deltaT) + pState->vel_e.y;
  t = (acc_e[2] * pState->deltaT) + pState->vel_e.z;

  /* 'INS_InertialNavigation:95' pState.vel_e.x = vel_e(1,1); */
  pState->vel_e.x = scale;

  /* 'INS_InertialNavigation:96' pState.vel_e.y = vel_e(2,1); */
  pState->vel_e.y = absxk;

  /* 'INS_InertialNavigation:97' pState.vel_e.z = vel_e(3,1); */
  pState->vel_e.z = t;

  /* 'INS_InertialNavigation:100' geo_att_dcm = dcm_en*pState.dcm_be; */
  cossigma = -sin(pState->pos_g.lat);
  sin_lat = cos(pState->pos_g.lon);
  Wie_e[0] = cossigma * sin_lat;
  cos_lat = sin(pState->pos_g.lon);
  Wie_e[3] = cossigma * cos_lat;
  sin_2lon = cos(pState->pos_g.lat);
  Wie_e[6] = sin_2lon;
  Wie_e[1] = -cos_lat;
  Wie_e[4] = sin_lat;
  Wie_e[7] = 0.0;
  Wie_e[2] = (-sin_2lon) * sin_lat;
  Wie_e[5] = (-cos(pState->pos_g.lat)) * cos_lat;
  Wie_e[8] = cossigma;
  for (k = 0; k < 3; k++) {
    for (i = 0; i < 3; i++) {
      dcm_eb_tmp = i + (3 * k);
      dcm_eb[dcm_eb_tmp] = 0.0;
      dcm_eb[dcm_eb_tmp] += pState->dcm_be[3 * k] * Wie_e[i];
      dcm_eb[dcm_eb_tmp] += pState->dcm_be[(3 * k) + 1] * Wie_e[i + 3];
      dcm_eb[dcm_eb_tmp] += pState->dcm_be[(3 * k) + 2] * Wie_e[i + 6];
    }
  }

  /* 'INS_InertialNavigation:101' pState.att_bn = LIB_d2e(geo_att_dcm); */
  /* 'LIB_d2e:28' phi = atan2(dcm(3,2),dcm(3,3)); */
  /* 'LIB_d2e:31' if abs(dcm(3,1))<1.0 */
  if (fabs(dcm_eb[2]) < 1.0) {
    /* 'LIB_d2e:32' theta = -asin(dcm(3,1)); */
    cossigma = -asin(dcm_eb[2]);
  } else if (dcm_eb[2] >= 1.0) {
    /* 'LIB_d2e:33' elseif dcm(3,1) >= 1.0 */
    /* 'LIB_d2e:34' theta = -pi/2; */
    cossigma = -1.5707963267948966;
  } else if (dcm_eb[2] <= -1.0) {
    /* 'LIB_d2e:35' elseif dcm(3,1) <= -1.0 */
    /* 'LIB_d2e:36' theta = pi/2; */
    cossigma = 1.5707963267948966;
  } else {
    /* 'LIB_d2e:37' else */
    /* 'LIB_d2e:38' theta = 0.0; */
    cossigma = 0.0;
  }

  /* 'LIB_d2e:42' psi = atan2(dcm(2,1),dcm(1,1)); */
  /* 'LIB_d2e:44' eg.phi = phi; */
  pState->att_bn.phi = rt_atan2d_snf(dcm_eb[5], dcm_eb[8]);

  /* 'LIB_d2e:45' eg.the = theta; */
  pState->att_bn.the = cossigma;

  /* 'LIB_d2e:46' eg.psi = psi; */
  pState->att_bn.psi = rt_atan2d_snf(dcm_eb[1], dcm_eb[0]);

  /* 'INS_InertialNavigation:104' if pState.store.storeplace >= pState.store.storesize */
  if (pState->store.storeplace >= pState->store.storesize) {
    /* 'INS_InertialNavigation:105' pState.store.storeplace = 0; */
    pState->store.storeplace = 0.0;
  }

  /* 'INS_InertialNavigation:107' pState.store.storeplace = pState.store.storeplace + 1; */
  pState->store.storeplace++;

  /* 'INS_InertialNavigation:108' pState.store.store(pState.store.storeplace,1)  = pState.systemTime; */
  pState->store.store[((int32_T)pState->store.storeplace) - 1] = time;

  /* 'INS_InertialNavigation:109' pState.store.store(pState.store.storeplace,2)  = pState.pos_e.x; */
  pState->store.store[((int32_T)pState->store.storeplace) + 119] = dAngle_b[0];

  /* 'INS_InertialNavigation:110' pState.store.store(pState.store.storeplace,3)  = pState.pos_e.y; */
  pState->store.store[((int32_T)pState->store.storeplace) + 239] = dAngle_b[1];

  /* 'INS_InertialNavigation:111' pState.store.store(pState.store.storeplace,4)  = pState.pos_e.z; */
  pState->store.store[((int32_T)pState->store.storeplace) + 359] = dAngle_b[2];

  /* 'INS_InertialNavigation:112' pState.store.store(pState.store.storeplace,5)  = pState.vel_e.x; */
  pState->store.store[((int32_T)pState->store.storeplace) + 479] = scale;

  /* 'INS_InertialNavigation:113' pState.store.store(pState.store.storeplace,6)  = pState.vel_e.y; */
  pState->store.store[((int32_T)pState->store.storeplace) + 599] = absxk;

  /* 'INS_InertialNavigation:114' pState.store.store(pState.store.storeplace,7)  = pState.vel_e.z; */
  pState->store.store[((int32_T)pState->store.storeplace) + 719] = t;

  /* 'INS_InertialNavigation:115' pState.store.store(pState.store.storeplace,8)  = pState.baro_alt; */
  pState->store.store[((int32_T)pState->store.storeplace) + 839] = baro_alt;

  /* 'INS_InertialNavigation:116' pState.store.store(pState.store.storeplace,9)  = pState.pos_g.lat; */
  pState->store.store[((int32_T)pState->store.storeplace) + 959] =
    pState->pos_g.lat;

  /* 'INS_InertialNavigation:117' pState.store.store(pState.store.storeplace,10) = pState.pos_g.lon; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1079] =
    pState->pos_g.lon;

  /* 'INS_InertialNavigation:118' pState.store.store(pState.store.storeplace,11) = pState.q_be.q0; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1199] = qtemp[0];

  /* 'INS_InertialNavigation:119' pState.store.store(pState.store.storeplace,12) = pState.q_be.q1; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1319] = qtemp[1];

  /* 'INS_InertialNavigation:120' pState.store.store(pState.store.storeplace,13) = pState.q_be.q2; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1439] = qtemp[2];

  /* 'INS_InertialNavigation:121' pState.store.store(pState.store.storeplace,14) = pState.q_be.q3; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1559] = dAngle_b_0;

  /* 'INS_InertialNavigation:122' pState.store.store(pState.store.storeplace,15) = pState.wib_b.x; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1679] = b_x;

  /* 'INS_InertialNavigation:123' pState.store.store(pState.store.storeplace,16) = pState.wib_b.y; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1799] = b_y;

  /* 'INS_InertialNavigation:124' pState.store.store(pState.store.storeplace,17) = pState.wib_b.z; */
  pState->store.store[((int32_T)pState->store.storeplace) + 1919] = b_z;
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function [navState, kfState] = DoNavigationMain100Hz(t, w, f, navState, kfState, baro_alt)
 */
static void DoNavigationMain100Hz(real_T t, real_T w_x, real_T w_y, real_T w_z,
  real_T f_x, real_T f_y, real_T f_z, T_NAV_NavigationState *navState,
  T_NAV_KalmanFilterState *kfState, real_T baro_alt)
{
  real_T PHI[256];
  real_T r;
  real_T omegaS;
  int8_T b_I[256];
  int32_T k;
  real_T y_tmp[256];
  real_T tmp[256];
  real_T PHI_0[16];
  int32_T i;
  int32_T i_0;
  real_T tmp_0;
  real_T tmp_1;
  int32_T PHI_tmp;
  int32_T PHI_tmp_0;

  /* 'DoNavigationMain100Hz:33' coder.cstructname(navState, 'T_NAV_NavigationState'); */
  /* 'DoNavigationMain100Hz:34' coder.cstructname(navState.pos_g, 'T_GeographicPos'); */
  /* 'DoNavigationMain100Hz:35' coder.cstructname(navState.fib_e, 'T_Cartesian'); */
  /* 'DoNavigationMain100Hz:36' coder.cstructname(navState.grav_e, 'T_Cartesian'); */
  /* 'DoNavigationMain100Hz:37' coder.cstructname(navState.pos_e, 'T_Cartesian'); */
  /* 'DoNavigationMain100Hz:38' coder.cstructname(navState.vel_e, 'T_Cartesian'); */
  /* 'DoNavigationMain100Hz:39' coder.cstructname(navState.att_bn, 'T_Euler'); */
  /* 'DoNavigationMain100Hz:40' coder.cstructname(navState.q_be, 'T_Quaternion'); */
  /* 'DoNavigationMain100Hz:41' coder.cstructname(navState.store, 'T_NAV_Store'); */
  /* 'DoNavigationMain100Hz:43' coder.cstructname(kfState, 'T_NAV_KalmanFilterState'); */
  /* 'DoNavigationMain100Hz:47' [wc,fc]  = BiasCorrection(w, f, kfState)                        ; */
  /* 'DoNavigationMain100Hz:56' dt = 1/100; */
  /* 'DoNavigationMain100Hz:58' w.x = w.x - kfState.B_gyro(1).*dt; */
  /* 'DoNavigationMain100Hz:59' w.y = w.y - kfState.B_gyro(2).*dt; */
  /* 'DoNavigationMain100Hz:60' w.z = w.z - kfState.B_gyro(3).*dt; */
  /* 'DoNavigationMain100Hz:62' f.x = f.x - kfState.B_accel(1).*dt; */
  /* 'DoNavigationMain100Hz:63' f.y = f.y - kfState.B_accel(2).*dt; */
  /* 'DoNavigationMain100Hz:64' f.z = f.z - kfState.B_accel(3).*dt; */
  /* 'DoNavigationMain100Hz:48' navState = INS_InertialNavigation(t, wc, fc, navState, baro_alt); */
  INS_InertialNavigation(t, w_x - (kfState->B_gyro[0] * 0.01), w_y -
    (kfState->B_gyro[1] * 0.01), w_z - (kfState->B_gyro[2] * 0.01), f_x -
    (kfState->B_accel[0] * 0.01), f_y - (kfState->B_accel[1] * 0.01), f_z -
    (kfState->B_accel[2] * 0.01), navState, baro_alt);

  /* 'DoNavigationMain100Hz:51' kfState = KalmanFilterExtrapolate(kfState, navState); */
  /* 'KalmanFilterExtrapolate:27' kfState.A = KalmanFilter_StateTransitionMatrix(navState); */
  /* 'KalmanFilterExtrapolate:67' A = zeros(16,16); */
  /* 'KalmanFilterExtrapolate:70' r = (navState.pos_e.x^2 + navState.pos_e.y^2 + navState.pos_e.z^2)^0.5; */
  r = sqrt(((navState->pos_e.x * navState->pos_e.x) + (navState->pos_e.y *
             navState->pos_e.y)) + (navState->pos_e.z * navState->pos_e.z));

  /* 'KalmanFilterExtrapolate:73' omegaS = ((navState.grav_e.x^2 + navState.grav_e.y^2 + navState.grav_e.z^2)^0.5) / r; */
  omegaS = sqrt(((navState->grav_e.x * navState->grav_e.x) + (navState->grav_e.y
    * navState->grav_e.y)) + (navState->grav_e.z * navState->grav_e.z)) / r;

  /* 'KalmanFilterExtrapolate:76' A(1,2)   = -1.0*abs(navState.WE); */
  memset(&kfState->A[0], 0, (sizeof(real_T)) << 8U);
  memset(&b_I[0], 0, (sizeof(int8_T)) << 8U);
  tmp_0 = fabs(navState->WE);
  kfState->A[16] = -tmp_0;

  /* 'KalmanFilterExtrapolate:77' A(1,4)   = 1.0; */
  kfState->A[48] = 1.0;

  /* 'KalmanFilterExtrapolate:79' A(2,1)   = abs(navState.WE); */
  kfState->A[1] = tmp_0;

  /* 'KalmanFilterExtrapolate:80' A(2,5)   = 1.0; */
  kfState->A[65] = 1.0;

  /* 'KalmanFilterExtrapolate:82' A(3,6)   = 1.0; */
  kfState->A[82] = 1.0;

  /* 'KalmanFilterExtrapolate:84' A(4,1)   = -1.0*omegaS^2*(1.0 - 3.0*(navState.pos_e.x/r)) - navState.WE^2; */
  omegaS = -(omegaS * omegaS);
  tmp_1 = navState->WE * navState->WE;
  kfState->A[3] = ((1.0 - ((navState->pos_e.x / r) * 3.0)) * omegaS) - tmp_1;

  /* 'KalmanFilterExtrapolate:85' A(4,5)   = 2.0*abs(navState.WE); */
  kfState->A[67] = 2.0 * tmp_0;

  /* 'KalmanFilterExtrapolate:86' A(4,8)   = -1.0*navState.fib_e.z; */
  kfState->A[115] = -navState->fib_e.z;

  /* 'KalmanFilterExtrapolate:87' A(4,9)   = navState.fib_e.y; */
  kfState->A[131] = navState->fib_e.y;

  /* 'KalmanFilterExtrapolate:88' A(4,13)  = navState.dcm_be(1,1); */
  kfState->A[195] = navState->dcm_be[0];

  /* 'KalmanFilterExtrapolate:89' A(4,14)  = navState.dcm_be(1,2); */
  kfState->A[211] = navState->dcm_be[3];

  /* 'KalmanFilterExtrapolate:90' A(4,15)  = navState.dcm_be(1,3); */
  kfState->A[227] = navState->dcm_be[6];

  /* 'KalmanFilterExtrapolate:92' A(5,2)   = -1.0*omegaS^2*(1.0 - 3.0*(navState.pos_e.y/r)) - navState.WE^2; */
  kfState->A[20] = ((1.0 - ((navState->pos_e.y / r) * 3.0)) * omegaS) - tmp_1;

  /* 'KalmanFilterExtrapolate:93' A(5,4)   = -2.0*abs(navState.WE); */
  kfState->A[52] = -2.0 * tmp_0;

  /* 'KalmanFilterExtrapolate:94' A(5,7)   = navState.fib_e.z; */
  kfState->A[100] = navState->fib_e.z;

  /* 'KalmanFilterExtrapolate:95' A(5,9)   = -1.0*navState.fib_e.x; */
  kfState->A[132] = -navState->fib_e.x;

  /* 'KalmanFilterExtrapolate:96' A(5,13)  = navState.dcm_be(2,1); */
  kfState->A[196] = navState->dcm_be[1];

  /* 'KalmanFilterExtrapolate:97' A(5,14)  = navState.dcm_be(2,2); */
  kfState->A[212] = navState->dcm_be[4];

  /* 'KalmanFilterExtrapolate:98' A(5,15)  = navState.dcm_be(2,3); */
  kfState->A[228] = navState->dcm_be[7];

  /* 'KalmanFilterExtrapolate:100' A(6,3)   = -1.0*omegaS^2*(1.0 - 3.0*(navState.pos_e.z/r)); */
  kfState->A[37] = (1.0 - ((navState->pos_e.z / r) * 3.0)) * omegaS;

  /* 'KalmanFilterExtrapolate:101' A(6,7)   = -1.0*navState.fib_e.y; */
  kfState->A[101] = -navState->fib_e.y;

  /* 'KalmanFilterExtrapolate:102' A(6,8)   = navState.fib_e.x; */
  kfState->A[117] = navState->fib_e.x;

  /* 'KalmanFilterExtrapolate:103' A(6,13)  = navState.dcm_be(3,1); */
  kfState->A[197] = navState->dcm_be[2];

  /* 'KalmanFilterExtrapolate:104' A(6,14)  = navState.dcm_be(3,2); */
  kfState->A[213] = navState->dcm_be[5];

  /* 'KalmanFilterExtrapolate:105' A(6,15)  = navState.dcm_be(3,3); */
  kfState->A[229] = navState->dcm_be[8];

  /* 'KalmanFilterExtrapolate:107' A(7,8)   = abs(navState.WE); */
  kfState->A[118] = tmp_0;

  /* 'KalmanFilterExtrapolate:108' A(7,10)  = -navState.dcm_be(1,1); */
  kfState->A[150] = -navState->dcm_be[0];

  /* 'KalmanFilterExtrapolate:109' A(7,11)  = -navState.dcm_be(1,2); */
  kfState->A[166] = -navState->dcm_be[3];

  /* 'KalmanFilterExtrapolate:110' A(7,12)  = -navState.dcm_be(1,3); */
  kfState->A[182] = -navState->dcm_be[6];

  /* 'KalmanFilterExtrapolate:112' A(8,7)   = -1.0*abs(navState.WE); */
  kfState->A[103] = -fabs(navState->WE);

  /* 'KalmanFilterExtrapolate:113' A(8,10)  = -navState.dcm_be(2,1); */
  kfState->A[151] = -navState->dcm_be[1];

  /* 'KalmanFilterExtrapolate:114' A(8,11)  = -navState.dcm_be(2,2); */
  kfState->A[167] = -navState->dcm_be[4];

  /* 'KalmanFilterExtrapolate:115' A(8,12)  = -navState.dcm_be(2,3); */
  kfState->A[183] = -navState->dcm_be[7];

  /* 'KalmanFilterExtrapolate:117' A(9,10)  = -navState.dcm_be(3,1); */
  kfState->A[152] = -navState->dcm_be[2];

  /* 'KalmanFilterExtrapolate:118' A(9,11)  = -navState.dcm_be(3,2); */
  kfState->A[168] = -navState->dcm_be[5];

  /* 'KalmanFilterExtrapolate:119' A(9,12)  = -navState.dcm_be(3,3); */
  kfState->A[184] = -navState->dcm_be[8];

  /* 'KalmanFilterExtrapolate:122' A(10,10) = -0.01; */
  kfState->A[153] = -0.01;

  /* 'KalmanFilterExtrapolate:123' A(11,11) = -0.01; */
  kfState->A[170] = -0.01;

  /* 'KalmanFilterExtrapolate:124' A(12,12) = -0.01; */
  kfState->A[187] = -0.01;

  /* 'KalmanFilterExtrapolate:125' A(13,13) = -0.01; */
  kfState->A[204] = -0.01;

  /* 'KalmanFilterExtrapolate:126' A(14,14) = -0.01; */
  kfState->A[221] = -0.01;

  /* 'KalmanFilterExtrapolate:127' A(15,15) = -0.01; */
  kfState->A[238] = -0.01;

  /* 'KalmanFilterExtrapolate:30' PHI = eye(16) + (kfState.A*navState.deltaT) + ... */
  /* 'KalmanFilterExtrapolate:31'                   (kfState.A*navState.deltaT)^2/factorial(2) + ... */
  /* 'KalmanFilterExtrapolate:32'                   (kfState.A*navState.deltaT)^3/factorial(3); */
  for (k = 0; k < 16; k++) {
    b_I[k + (k << ((uint64_T)4))] = 1;
  }

  for (k = 0; k < 256; k++) {
    y_tmp[k] = kfState->A[k] * navState->deltaT;
  }

  mpower_JIx9WxiC(y_tmp, tmp);

  /* 'KalmanFilterExtrapolate:33' kfState.X = PHI*kfState.X; */
  for (k = 0; k < 16; k++) {
    PHI_0[k] = 0.0;
    for (i = 0; i < 16; i++) {
      tmp_0 = 0.0;
      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_0 += y_tmp[(i_0 << ((uint64_T)4)) + k] * y_tmp[(i << ((uint64_T)4))
          + i_0];
      }

      PHI_tmp = (i << ((uint64_T)4)) + k;
      r = ((((real_T)b_I[PHI_tmp]) + y_tmp[PHI_tmp]) + (tmp_0 / 2.0)) +
        (tmp[PHI_tmp] / 6.0);
      PHI[PHI_tmp] = r;
      PHI_0[k] += r * kfState->X[i];
    }
  }

  /* 'KalmanFilterExtrapolate:36' kfState.P = PHI*kfState.P*PHI' + kfState.Q*navState.deltaT; */
  for (k = 0; k < 16; k++) {
    kfState->X[k] = PHI_0[k];
    for (i = 0; i < 16; i++) {
      PHI_tmp = (i << ((uint64_T)4));
      PHI_tmp_0 = k + PHI_tmp;
      y_tmp[PHI_tmp_0] = 0.0;
      for (i_0 = 0; i_0 < 16; i_0++) {
        y_tmp[PHI_tmp_0] += PHI[(i_0 << ((uint64_T)4)) + k] * kfState->P[PHI_tmp
          + i_0];
      }
    }
  }

  for (k = 0; k < 16; k++) {
    for (i = 0; i < 16; i++) {
      tmp_0 = 0.0;
      for (i_0 = 0; i_0 < 16; i_0++) {
        PHI_tmp = (i_0 << ((uint64_T)4));
        tmp_0 += y_tmp[PHI_tmp + k] * PHI[PHI_tmp + i];
      }

      i_0 = (i << ((uint64_T)4)) + k;
      kfState->P[i_0] = (kfState->Q[i_0] * navState->deltaT) + tmp_0;
    }
  }
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function kfState = UpdateBiases(kfState, delta)
 */
static void UpdateBiases(T_NAV_KalmanFilterState *kfState, const real_T delta[16])
{
  real_T out;
  int32_T i;

  /* 'KalmanFilterMeasurementUpdate:235' kfState.X    (10:15,1) = kfState.X    (10:15,1) - delta(10:15,1); */
  for (i = 0; i < 6; i++) {
    kfState->X[9 + i] -= delta[9 + i];
  }

  /* 'KalmanFilterMeasurementUpdate:236' kfState.B_gyro (1:3,1) = kfState.B_gyro (1:3,1) + delta(10:12,1); */
  /* 'KalmanFilterMeasurementUpdate:237' kfState.B_accel(1:3,1) = kfState.B_accel(1:3,1) + delta(13:15,1); */
  /* 'KalmanFilterMeasurementUpdate:240' gravity = 9.81          ; */
  /* 'KalmanFilterMeasurementUpdate:241' d2r     = pi/180.0      ; */
  /* 'KalmanFilterMeasurementUpdate:242' dph2rps = d2r/3600.0    ; */
  /* 'KalmanFilterMeasurementUpdate:243' mg2mps2 = gravity/1000.0; */
  /* 'KalmanFilterMeasurementUpdate:246' LimitGyro  = 480.0 * dph2rps; */
  /* 'KalmanFilterMeasurementUpdate:247' LimitAccel =  60.0 * mg2mps2; */
  /* 'KalmanFilterMeasurementUpdate:249' for I=1:3 */
  kfState->B_gyro[0] += delta[9];
  kfState->B_accel[0] += delta[12];

  /* 'KalmanFilterMeasurementUpdate:250' kfState.B_gyro (I) = LIB_limit(-LimitGyro , LimitGyro , kfState.B_gyro (I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_gyro[0] > 0.0023271056693257726) {
    /* 'LIB_limit:28' out = max; */
    out = 0.0023271056693257726;
  } else if (kfState->B_gyro[0] < -0.0023271056693257726) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.0023271056693257726;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_gyro[0];
  }

  kfState->B_gyro[0] = out;

  /* 'KalmanFilterMeasurementUpdate:251' kfState.B_accel(I) = LIB_limit(-LimitAccel, LimitAccel, kfState.B_accel(I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_accel[0] > 0.5886) {
    /* 'LIB_limit:28' out = max; */
    out = 0.5886;
  } else if (kfState->B_accel[0] < -0.5886) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.5886;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_accel[0];
  }

  kfState->B_accel[0] = out;
  kfState->B_gyro[1] += delta[10];
  kfState->B_accel[1] += delta[13];

  /* 'KalmanFilterMeasurementUpdate:250' kfState.B_gyro (I) = LIB_limit(-LimitGyro , LimitGyro , kfState.B_gyro (I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_gyro[1] > 0.0023271056693257726) {
    /* 'LIB_limit:28' out = max; */
    out = 0.0023271056693257726;
  } else if (kfState->B_gyro[1] < -0.0023271056693257726) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.0023271056693257726;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_gyro[1];
  }

  kfState->B_gyro[1] = out;

  /* 'KalmanFilterMeasurementUpdate:251' kfState.B_accel(I) = LIB_limit(-LimitAccel, LimitAccel, kfState.B_accel(I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_accel[1] > 0.5886) {
    /* 'LIB_limit:28' out = max; */
    out = 0.5886;
  } else if (kfState->B_accel[1] < -0.5886) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.5886;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_accel[1];
  }

  kfState->B_accel[1] = out;
  kfState->B_gyro[2] += delta[11];
  kfState->B_accel[2] += delta[14];

  /* 'KalmanFilterMeasurementUpdate:250' kfState.B_gyro (I) = LIB_limit(-LimitGyro , LimitGyro , kfState.B_gyro (I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_gyro[2] > 0.0023271056693257726) {
    /* 'LIB_limit:28' out = max; */
    out = 0.0023271056693257726;
  } else if (kfState->B_gyro[2] < -0.0023271056693257726) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.0023271056693257726;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_gyro[2];
  }

  kfState->B_gyro[2] = out;

  /* 'KalmanFilterMeasurementUpdate:251' kfState.B_accel(I) = LIB_limit(-LimitAccel, LimitAccel, kfState.B_accel(I)); */
  /* 'LIB_limit:27' if (in > max) */
  if (kfState->B_accel[2] > 0.5886) {
    /* 'LIB_limit:28' out = max; */
    out = 0.5886;
  } else if (kfState->B_accel[2] < -0.5886) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    out = -0.5886;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    out = kfState->B_accel[2];
  }

  kfState->B_accel[2] = out;
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function [navState, kfState, baroState] = KalmanFilterMeasurementUpdate(measurementState, navState, kfState, baroState)
 */
static void KalmanFilterMeasurementUpdate(const real_T measurementState_nav[16],
  const real_T measurementState_kalmanfilter_state[16], const real_T
  measurementState_kalmanfilter_covariance[256], uint8_T
  measurementState_valid_gnss, uint8_T measurementState_valid_baro, uint8_T
  measurementState_valid_ta, uint32_T measurementState_valid_mag, uint8_T
  measurementState_valid_feedback_nav, uint8_T
  measurementState_valid_feedback_att, uint8_T
  measurementState_valid_reset_navigation, T_NAV_NavigationState *navState,
  T_NAV_KalmanFilterState *kfState, T_NAV_BAROState *baroState, DW *dwork)
{
  real_T r1;
  real_T r2;
  real_T r3;
  real_T Q_q0;
  real_T normQuat;
  int32_T i;
  real_T Q_q0_tmp;
  real_T Q_q0_tmp_0;
  real_T Q_q0_tmp_1;
  real_T Q_q0_tmp_tmp;
  real_T Q_q0_tmp_tmp_0;
  real_T Q_q0_tmp_tmp_1;

  /* 'KalmanFilterMeasurementUpdate:34' if isempty(BaroScaleFactor) */
  /* 'KalmanFilterMeasurementUpdate:37' if isempty(LocalBaroBias) */
  /* 'KalmanFilterMeasurementUpdate:42' kfState.X = kfState.X + measurementState.kalmanfilter.state; */
  for (i = 0; i < 16; i++) {
    kfState->X[i] += measurementState_kalmanfilter_state[i];
  }

  /* 'KalmanFilterMeasurementUpdate:43' kfState.P = kfState.P + measurementState.kalmanfilter.covariance; */
  for (i = 0; i < 256; i++) {
    kfState->P[i] += measurementState_kalmanfilter_covariance[i];
  }

  /* 'KalmanFilterMeasurementUpdate:46' if measurementState.valid.reset_navigation */
  if (((int32_T)measurementState_valid_reset_navigation) != 0) {
    /* 'KalmanFilterMeasurementUpdate:48' navState.pos_e.x = navState.pos_e.x - measurementState.nav(1,1); */
    navState->pos_e.x -= measurementState_nav[0];

    /* 'KalmanFilterMeasurementUpdate:49' navState.pos_e.y = navState.pos_e.y - measurementState.nav(2,1); */
    navState->pos_e.y -= measurementState_nav[1];

    /* 'KalmanFilterMeasurementUpdate:50' navState.pos_e.z = navState.pos_e.z - measurementState.nav(3,1); */
    navState->pos_e.z -= measurementState_nav[2];

    /* 'KalmanFilterMeasurementUpdate:51' navState.vel_e.x = navState.vel_e.x - measurementState.nav(4,1); */
    navState->vel_e.x -= measurementState_nav[3];

    /* 'KalmanFilterMeasurementUpdate:52' navState.vel_e.y = navState.vel_e.y - measurementState.nav(5,1); */
    navState->vel_e.y -= measurementState_nav[4];

    /* 'KalmanFilterMeasurementUpdate:53' navState.vel_e.z = navState.vel_e.z - measurementState.nav(6,1); */
    navState->vel_e.z -= measurementState_nav[5];
  } else {
    /* 'KalmanFilterMeasurementUpdate:54' else */
    /* 'KalmanFilterMeasurementUpdate:56' if ((measurementState.valid.gnss) || (measurementState.valid.ta)) */
    if ((((int32_T)measurementState_valid_gnss) != 0) || (((int32_T)
          measurementState_valid_ta) != 0)) {
      /* 'KalmanFilterMeasurementUpdate:57' delta = measurementState.nav; */
      /* 'KalmanFilterMeasurementUpdate:58' if measurementState.valid.feedback_nav */
      if (((int32_T)measurementState_valid_feedback_nav) != 0) {
        /* 'KalmanFilterMeasurementUpdate:59' kfState.X(1:6,1) = kfState.X(1:6,1) - delta(1:6,1); */
        for (i = 0; i < 6; i++) {
          kfState->X[i] -= measurementState_nav[i];
        }

        /* 'KalmanFilterMeasurementUpdate:60' navState.pos_e.x = navState.pos_e.x - delta(1,1); */
        navState->pos_e.x -= measurementState_nav[0];

        /* 'KalmanFilterMeasurementUpdate:61' navState.pos_e.y = navState.pos_e.y - delta(2,1); */
        navState->pos_e.y -= measurementState_nav[1];

        /* 'KalmanFilterMeasurementUpdate:62' navState.pos_e.z = navState.pos_e.z - delta(3,1); */
        navState->pos_e.z -= measurementState_nav[2];

        /* 'KalmanFilterMeasurementUpdate:63' navState.vel_e.x = navState.vel_e.x - delta(4,1); */
        navState->vel_e.x -= measurementState_nav[3];

        /* 'KalmanFilterMeasurementUpdate:64' navState.vel_e.y = navState.vel_e.y - delta(5,1); */
        navState->vel_e.y -= measurementState_nav[4];

        /* 'KalmanFilterMeasurementUpdate:65' navState.vel_e.z = navState.vel_e.z - delta(6,1); */
        navState->vel_e.z -= measurementState_nav[5];

        /* 'KalmanFilterMeasurementUpdate:67' if (measurementState.valid.baro) */
        if (((int32_T)measurementState_valid_baro) != 0) {
          /* 'KalmanFilterMeasurementUpdate:68' kfState.X(16,1) = kfState.X(16,1) - delta(16,1); */
          kfState->X[15] -= measurementState_nav[15];

          /* 'KalmanFilterMeasurementUpdate:69' baroState.KalmanFilter_delta_baro = baroState.KalmanFilter_delta_baro + delta(16,1); */
          baroState->KalmanFilter_delta_baro += measurementState_nav[15];

          /* 'KalmanFilterMeasurementUpdate:70' BaroScaleFactor = 2.818913332005712e-17*baroState.comp_alt^4 - ... */
          /* 'KalmanFilterMeasurementUpdate:71'                               3.863927801707316e-13*baroState.comp_alt^3 + ... */
          /* 'KalmanFilterMeasurementUpdate:72'                               3.284001514864467e-09*baroState.comp_alt^2 + ... */
          /* 'KalmanFilterMeasurementUpdate:73'                               1.485650642383993e-05*baroState.comp_alt +   ... */
          /* 'KalmanFilterMeasurementUpdate:74'                               1.796515109540371e-01; */
          dwork->BaroScaleFactor = ((((2.8189133320057122E-17 * rt_powd_snf
            (baroState->comp_alt, 4.0)) - (3.8639278017073163E-13 * rt_powd_snf
            (baroState->comp_alt, 3.0))) + ((baroState->comp_alt *
            baroState->comp_alt) * 3.2840015148644669E-9)) +
            (1.4856506423839931E-5 * baroState->comp_alt)) + 0.17965151095403711;

          /* 'KalmanFilterMeasurementUpdate:75' LocalBaroBias = baroState.KalmanFilter_delta_baro; */
          dwork->LocalBaroBias = baroState->KalmanFilter_delta_baro;
        }

        /* 'KalmanFilterMeasurementUpdate:78' if measurementState.valid.feedback_att */
        if (((int32_T)measurementState_valid_feedback_att) != 0) {
          /* 'KalmanFilterMeasurementUpdate:79' kfState.X(7:9,1) = kfState.X(7:9,1) - delta(7:9,1); */
          kfState->X[6] -= measurementState_nav[6];
          kfState->X[7] -= measurementState_nav[7];
          kfState->X[8] -= measurementState_nav[8];

          /* 'KalmanFilterMeasurementUpdate:80' kfState = UpdateBiases(kfState, delta); */
          UpdateBiases(kfState, measurementState_nav);

          /* 'KalmanFilterMeasurementUpdate:82' r1 = delta(9,1)/2; */
          r1 = measurementState_nav[8] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:83' r2 = delta(8,1)/2; */
          r2 = measurementState_nav[7] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:84' r3 = delta(7,1)/2; */
          r3 = measurementState_nav[6] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:85' Q.q0 = cos(r1)*cos(r2)*cos(r3) + sin(r1)*sin(r2)*sin(r3); */
          Q_q0_tmp = cos(r1);
          Q_q0_tmp_0 = sin(r3);
          Q_q0_tmp_1 = sin(r1);
          r1 = cos(r3);
          Q_q0 = ((Q_q0_tmp * cos(r2)) * r1) + ((Q_q0_tmp_1 * sin(r2)) *
            Q_q0_tmp_0);

          /* 'KalmanFilterMeasurementUpdate:86' Q.q1 = cos(r1)*cos(r2)*sin(r3) - sin(r1)*sin(r2)*cos(r3); */
          Q_q0_tmp_tmp = sin(r2);
          r2 = cos(r2);
          normQuat = ((Q_q0_tmp * r2) * Q_q0_tmp_0) - ((Q_q0_tmp_1 *
            Q_q0_tmp_tmp) * r1);

          /* 'KalmanFilterMeasurementUpdate:87' Q.q2 = cos(r1)*sin(r2)*cos(r3) + sin(r1)*cos(r2)*sin(r3); */
          Q_q0_tmp_1 *= r2;
          r2 = Q_q0_tmp * Q_q0_tmp_tmp;
          Q_q0_tmp = (r2 * r1) + (Q_q0_tmp_1 * Q_q0_tmp_0);

          /* 'KalmanFilterMeasurementUpdate:88' Q.q3 = sin(r1)*cos(r2)*cos(r3) - cos(r1)*sin(r2)*sin(r3); */
          r3 = (Q_q0_tmp_1 * cos(r3)) - (r2 * sin(r3));

          /* 'KalmanFilterMeasurementUpdate:89' Q1.q0   =    Q.q0*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:90'                        - Q.q1*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:91'                        - Q.q2*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:92'                        - Q.q3*navState.q_be.q3; */
          r1 = (((Q_q0 * navState->q_be.q0) - (normQuat * navState->q_be.q1)) -
                (Q_q0_tmp * navState->q_be.q2)) - (r3 * navState->q_be.q3);

          /* 'KalmanFilterMeasurementUpdate:94' Q1.q1   =    Q.q0*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:95'                        + Q.q1*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:96'                        + Q.q2*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:97'                        - Q.q3*navState.q_be.q2; */
          Q_q0_tmp_0 = (((Q_q0 * navState->q_be.q1) + (normQuat *
            navState->q_be.q0)) + (Q_q0_tmp * navState->q_be.q3)) - (r3 *
            navState->q_be.q2);

          /* 'KalmanFilterMeasurementUpdate:99' Q1.q2   =    Q.q0*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:100'                        - Q.q1*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:101'                        + Q.q2*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:102'                        + Q.q3*navState.q_be.q1; */
          r2 = (((Q_q0 * navState->q_be.q2) - (normQuat * navState->q_be.q3)) +
                (Q_q0_tmp * navState->q_be.q0)) + (r3 * navState->q_be.q1);

          /* 'KalmanFilterMeasurementUpdate:104' Q1.q3   =    Q.q0*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:105'                        + Q.q1*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:106'                        - Q.q2*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:107'                        + Q.q3*navState.q_be.q0; */
          Q_q0 = (((Q_q0 * navState->q_be.q3) + (normQuat * navState->q_be.q2))
                  - (Q_q0_tmp * navState->q_be.q1)) + (r3 * navState->q_be.q0);

          /* 'KalmanFilterMeasurementUpdate:109' normQuat = 1/((Q1.q0)^2+(Q1.q1)^2+(Q1.q2)^2+(Q1.q3)^2)^0.5; */
          normQuat = 1.0 / sqrt((((r1 * r1) + (Q_q0_tmp_0 * Q_q0_tmp_0)) + (r2 *
            r2)) + (Q_q0 * Q_q0));

          /* 'KalmanFilterMeasurementUpdate:110' navState.q_be.q0 = Q1.q0*normQuat; */
          navState->q_be.q0 = r1 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:111' navState.q_be.q1 = Q1.q1*normQuat; */
          navState->q_be.q1 = Q_q0_tmp_0 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:112' navState.q_be.q2 = Q1.q2*normQuat; */
          navState->q_be.q2 = r2 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:113' navState.q_be.q3 = Q1.q3*normQuat; */
          navState->q_be.q3 = Q_q0 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:114' navState.dcm_be = LIB_q2d(navState.q_be); */
          LIB_q2d(navState->q_be.q0, navState->q_be.q1, navState->q_be.q2,
                  navState->q_be.q3, navState->dcm_be);
        }
      }
    } else if ((((int32_T)measurementState_valid_baro) != 0) &&
               (measurementState_valid_mag != 0U)) {
      /* 'KalmanFilterMeasurementUpdate:117' elseif ((~measurementState.valid.gnss) && (~measurementState.valid.ta) && (measurementState.valid.baro) && (measurementState.valid.mag)) */
      /* 'KalmanFilterMeasurementUpdate:118' if measurementState.valid.feedback_nav */
      if (((int32_T)measurementState_valid_feedback_nav) != 0) {
        /* 'KalmanFilterMeasurementUpdate:119' delta = measurementState.nav; */
        /* 'KalmanFilterMeasurementUpdate:120' kfState.X(1:6,1) = kfState.X(1:6,1) - delta(1:6,1); */
        for (i = 0; i < 6; i++) {
          kfState->X[i] -= measurementState_nav[i];
        }

        /* 'KalmanFilterMeasurementUpdate:121' navState.pos_e.x = navState.pos_e.x - delta(1,1); */
        navState->pos_e.x -= measurementState_nav[0];

        /* 'KalmanFilterMeasurementUpdate:122' navState.pos_e.y = navState.pos_e.y - delta(2,1); */
        navState->pos_e.y -= measurementState_nav[1];

        /* 'KalmanFilterMeasurementUpdate:123' navState.pos_e.z = navState.pos_e.z - delta(3,1); */
        navState->pos_e.z -= measurementState_nav[2];

        /* 'KalmanFilterMeasurementUpdate:124' navState.vel_e.x = navState.vel_e.x - delta(4,1); */
        navState->vel_e.x -= measurementState_nav[3];

        /* 'KalmanFilterMeasurementUpdate:125' navState.vel_e.y = navState.vel_e.y - delta(5,1); */
        navState->vel_e.y -= measurementState_nav[4];

        /* 'KalmanFilterMeasurementUpdate:126' navState.vel_e.z = navState.vel_e.z - delta(6,1); */
        navState->vel_e.z -= measurementState_nav[5];

        /* 'KalmanFilterMeasurementUpdate:127' baroState.KalmanFilter_delta_baro = LocalBaroBias * (2.818913332005712e-17*baroState.comp_alt^4 - ... */
        /* 'KalmanFilterMeasurementUpdate:128'                                                                  3.863927801707316e-13*baroState.comp_alt^3 + ... */
        /* 'KalmanFilterMeasurementUpdate:129'                                                                  3.284001514864467e-09*baroState.comp_alt^2 + ... */
        /* 'KalmanFilterMeasurementUpdate:130'                                                                  1.485650642383993e-05*baroState.comp_alt +   ... */
        /* 'KalmanFilterMeasurementUpdate:131'                                                                  1.796515109540371e-01)/BaroScaleFactor; */
        baroState->KalmanFilter_delta_baro = ((((((2.8189133320057122E-17 *
          rt_powd_snf(baroState->comp_alt, 4.0)) - (3.8639278017073163E-13 *
          rt_powd_snf(baroState->comp_alt, 3.0))) + ((baroState->comp_alt *
          baroState->comp_alt) * 3.2840015148644669E-9)) +
          (1.4856506423839931E-5 * baroState->comp_alt)) + 0.17965151095403711) *
          dwork->LocalBaroBias) / dwork->BaroScaleFactor;

        /* 'KalmanFilterMeasurementUpdate:132' if measurementState.valid.feedback_att */
        if (((int32_T)measurementState_valid_feedback_att) != 0) {
          /* 'KalmanFilterMeasurementUpdate:133' kfState.X(7:9,1) = kfState.X(7:9,1) - delta(7:9,1); */
          kfState->X[6] -= measurementState_nav[6];
          kfState->X[7] -= measurementState_nav[7];
          kfState->X[8] -= measurementState_nav[8];

          /* 'KalmanFilterMeasurementUpdate:134' kfState = UpdateBiases(kfState, delta); */
          UpdateBiases(kfState, measurementState_nav);

          /* 'KalmanFilterMeasurementUpdate:136' r1 = delta(9,1)/2; */
          r1 = measurementState_nav[8] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:137' r2 = delta(8,1)/2; */
          r2 = measurementState_nav[7] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:138' r3 = delta(7,1)/2; */
          r3 = measurementState_nav[6] / 2.0;

          /* 'KalmanFilterMeasurementUpdate:139' Q.q0 = cos(r1)*cos(r2)*cos(r3) + sin(r1)*sin(r2)*sin(r3); */
          Q_q0_tmp = cos(r3);
          Q_q0_tmp_0 = sin(r3);
          Q_q0 = ((cos(r1) * cos(r2)) * Q_q0_tmp) + ((sin(r1) * sin(r2)) *
            Q_q0_tmp_0);

          /* 'KalmanFilterMeasurementUpdate:140' Q.q1 = cos(r1)*cos(r2)*sin(r3) - sin(r1)*sin(r2)*cos(r3); */
          normQuat = ((cos(r1) * cos(r2)) * Q_q0_tmp_0) - ((sin(r1) * sin(r2)) *
            Q_q0_tmp);

          /* 'KalmanFilterMeasurementUpdate:141' Q.q2 = cos(r1)*sin(r2)*cos(r3) + sin(r1)*cos(r2)*sin(r3); */
          Q_q0_tmp = ((cos(r1) * sin(r2)) * Q_q0_tmp) + ((sin(r1) * cos(r2)) *
            Q_q0_tmp_0);

          /* 'KalmanFilterMeasurementUpdate:142' Q.q3 = sin(r1)*cos(r2)*cos(r3) - cos(r1)*sin(r2)*sin(r3); */
          Q_q0_tmp_0 = ((sin(r1) * cos(r2)) * cos(r3)) - ((cos(r1) * sin(r2)) *
            sin(r3));

          /* 'KalmanFilterMeasurementUpdate:143' Q1.q0   =    Q.q0*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:144'                        - Q.q1*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:145'                        - Q.q2*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:146'                        - Q.q3*navState.q_be.q3; */
          r1 = (((Q_q0 * navState->q_be.q0) - (normQuat * navState->q_be.q1)) -
                (Q_q0_tmp * navState->q_be.q2)) - (Q_q0_tmp_0 *
            navState->q_be.q3);

          /* 'KalmanFilterMeasurementUpdate:148' Q1.q1   =    Q.q0*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:149'                        + Q.q1*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:150'                        + Q.q2*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:151'                        - Q.q3*navState.q_be.q2; */
          r3 = (((Q_q0 * navState->q_be.q1) + (normQuat * navState->q_be.q0)) +
                (Q_q0_tmp * navState->q_be.q3)) - (Q_q0_tmp_0 *
            navState->q_be.q2);

          /* 'KalmanFilterMeasurementUpdate:153' Q1.q2   =    Q.q0*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:154'                        - Q.q1*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:155'                        + Q.q2*navState.q_be.q0 ... */
          /* 'KalmanFilterMeasurementUpdate:156'                        + Q.q3*navState.q_be.q1; */
          r2 = (((Q_q0 * navState->q_be.q2) - (normQuat * navState->q_be.q3)) +
                (Q_q0_tmp * navState->q_be.q0)) + (Q_q0_tmp_0 *
            navState->q_be.q1);

          /* 'KalmanFilterMeasurementUpdate:158' Q1.q3   =    Q.q0*navState.q_be.q3 ... */
          /* 'KalmanFilterMeasurementUpdate:159'                        + Q.q1*navState.q_be.q2 ... */
          /* 'KalmanFilterMeasurementUpdate:160'                        - Q.q2*navState.q_be.q1 ... */
          /* 'KalmanFilterMeasurementUpdate:161'                        + Q.q3*navState.q_be.q0; */
          Q_q0 = (((Q_q0 * navState->q_be.q3) + (normQuat * navState->q_be.q2))
                  - (Q_q0_tmp * navState->q_be.q1)) + (Q_q0_tmp_0 *
            navState->q_be.q0);

          /* 'KalmanFilterMeasurementUpdate:163' normQuat = 1/((Q1.q0)^2+(Q1.q1)^2+(Q1.q2)^2+(Q1.q3)^2)^0.5; */
          normQuat = 1.0 / sqrt((((r1 * r1) + (r3 * r3)) + (r2 * r2)) + (Q_q0 *
            Q_q0));

          /* 'KalmanFilterMeasurementUpdate:164' navState.q_be.q0 = Q1.q0*normQuat; */
          navState->q_be.q0 = r1 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:165' navState.q_be.q1 = Q1.q1*normQuat; */
          navState->q_be.q1 = r3 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:166' navState.q_be.q2 = Q1.q2*normQuat; */
          navState->q_be.q2 = r2 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:167' navState.q_be.q3 = Q1.q3*normQuat; */
          navState->q_be.q3 = Q_q0 * normQuat;

          /* 'KalmanFilterMeasurementUpdate:168' navState.dcm_be = LIB_q2d(navState.q_be); */
          LIB_q2d(navState->q_be.q0, navState->q_be.q1, navState->q_be.q2,
                  navState->q_be.q3, navState->dcm_be);
        }
      }
    } else if ((((int32_T)measurementState_valid_baro) == 0) &&
               (measurementState_valid_mag != 0U)) {
      /* 'KalmanFilterMeasurementUpdate:171' elseif ((~measurementState.valid.gnss) && (~measurementState.valid.ta) && (~measurementState.valid.baro) && (measurementState.valid.mag)) */
      /* 'KalmanFilterMeasurementUpdate:172' if measurementState.valid.feedback_nav */
      if ((((int32_T)measurementState_valid_feedback_nav) != 0) && (((int32_T)
            measurementState_valid_feedback_att) != 0)) {
        /* 'KalmanFilterMeasurementUpdate:173' delta = measurementState.nav; */
        /* 'KalmanFilterMeasurementUpdate:174' if measurementState.valid.feedback_att */
        /* 'KalmanFilterMeasurementUpdate:175' kfState.X(7:9,1) = kfState.X(7:9,1) - delta(7:9,1); */
        kfState->X[6] -= measurementState_nav[6];
        kfState->X[7] -= measurementState_nav[7];
        kfState->X[8] -= measurementState_nav[8];

        /* 'KalmanFilterMeasurementUpdate:176' kfState = UpdateBiases(kfState, delta); */
        UpdateBiases(kfState, measurementState_nav);

        /* 'KalmanFilterMeasurementUpdate:178' r1 = delta(9,1)/2; */
        r1 = measurementState_nav[8] / 2.0;

        /* 'KalmanFilterMeasurementUpdate:179' r2 = delta(8,1)/2; */
        r2 = measurementState_nav[7] / 2.0;

        /* 'KalmanFilterMeasurementUpdate:180' r3 = delta(7,1)/2; */
        r3 = measurementState_nav[6] / 2.0;

        /* 'KalmanFilterMeasurementUpdate:181' Q.q0 = cos(r1)*cos(r2)*cos(r3) + sin(r1)*sin(r2)*sin(r3); */
        Q_q0_tmp_tmp = cos(r1);
        Q_q0_tmp_tmp_1 = cos(r2);
        Q_q0_tmp = Q_q0_tmp_tmp * Q_q0_tmp_tmp_1;
        Q_q0_tmp_0 = sin(r3);
        r2 = sin(r2);
        Q_q0_tmp_tmp_0 = sin(r1);
        Q_q0_tmp_1 = Q_q0_tmp_tmp_0 * r2;
        r1 = cos(r3);
        Q_q0 = (Q_q0_tmp * r1) + (Q_q0_tmp_1 * Q_q0_tmp_0);

        /* 'KalmanFilterMeasurementUpdate:182' Q.q1 = cos(r1)*cos(r2)*sin(r3) - sin(r1)*sin(r2)*cos(r3); */
        normQuat = (Q_q0_tmp * Q_q0_tmp_0) - (Q_q0_tmp_1 * r1);

        /* 'KalmanFilterMeasurementUpdate:183' Q.q2 = cos(r1)*sin(r2)*cos(r3) + sin(r1)*cos(r2)*sin(r3); */
        Q_q0_tmp_1 = Q_q0_tmp_tmp_0 * Q_q0_tmp_tmp_1;
        r2 *= Q_q0_tmp_tmp;
        Q_q0_tmp = (r2 * r1) + (Q_q0_tmp_1 * Q_q0_tmp_0);

        /* 'KalmanFilterMeasurementUpdate:184' Q.q3 = sin(r1)*cos(r2)*cos(r3) - cos(r1)*sin(r2)*sin(r3); */
        Q_q0_tmp_0 = (Q_q0_tmp_1 * r1) - (r2 * Q_q0_tmp_0);

        /* 'KalmanFilterMeasurementUpdate:185' Q1.q0   =    Q.q0*navState.q_be.q0 ... */
        /* 'KalmanFilterMeasurementUpdate:186'                        - Q.q1*navState.q_be.q1 ... */
        /* 'KalmanFilterMeasurementUpdate:187'                        - Q.q2*navState.q_be.q2 ... */
        /* 'KalmanFilterMeasurementUpdate:188'                        - Q.q3*navState.q_be.q3; */
        r1 = (((Q_q0 * navState->q_be.q0) - (normQuat * navState->q_be.q1)) -
              (Q_q0_tmp * navState->q_be.q2)) - (Q_q0_tmp_0 * navState->q_be.q3);

        /* 'KalmanFilterMeasurementUpdate:190' Q1.q1   =    Q.q0*navState.q_be.q1 ... */
        /* 'KalmanFilterMeasurementUpdate:191'                        + Q.q1*navState.q_be.q0 ... */
        /* 'KalmanFilterMeasurementUpdate:192'                        + Q.q2*navState.q_be.q3 ... */
        /* 'KalmanFilterMeasurementUpdate:193'                        - Q.q3*navState.q_be.q2; */
        r2 = (((Q_q0 * navState->q_be.q1) + (normQuat * navState->q_be.q0)) +
              (Q_q0_tmp * navState->q_be.q3)) - (Q_q0_tmp_0 * navState->q_be.q2);

        /* 'KalmanFilterMeasurementUpdate:195' Q1.q2   =    Q.q0*navState.q_be.q2 ... */
        /* 'KalmanFilterMeasurementUpdate:196'                        - Q.q1*navState.q_be.q3 ... */
        /* 'KalmanFilterMeasurementUpdate:197'                        + Q.q2*navState.q_be.q0 ... */
        /* 'KalmanFilterMeasurementUpdate:198'                        + Q.q3*navState.q_be.q1; */
        r3 = (((Q_q0 * navState->q_be.q2) - (normQuat * navState->q_be.q3)) +
              (Q_q0_tmp * navState->q_be.q0)) + (Q_q0_tmp_0 * navState->q_be.q1);

        /* 'KalmanFilterMeasurementUpdate:200' Q1.q3   =    Q.q0*navState.q_be.q3 ... */
        /* 'KalmanFilterMeasurementUpdate:201'                        + Q.q1*navState.q_be.q2 ... */
        /* 'KalmanFilterMeasurementUpdate:202'                        - Q.q2*navState.q_be.q1 ... */
        /* 'KalmanFilterMeasurementUpdate:203'                        + Q.q3*navState.q_be.q0; */
        Q_q0 = (((Q_q0 * navState->q_be.q3) + (normQuat * navState->q_be.q2)) -
                (Q_q0_tmp * navState->q_be.q1)) + (Q_q0_tmp_0 *
          navState->q_be.q0);

        /* 'KalmanFilterMeasurementUpdate:205' normQuat = 1/((Q1.q0)^2+(Q1.q1)^2+(Q1.q2)^2+(Q1.q3)^2)^0.5; */
        normQuat = 1.0 / sqrt((((r1 * r1) + (r2 * r2)) + (r3 * r3)) + (Q_q0 *
          Q_q0));

        /* 'KalmanFilterMeasurementUpdate:206' navState.q_be.q0 = Q1.q0*normQuat; */
        navState->q_be.q0 = r1 * normQuat;

        /* 'KalmanFilterMeasurementUpdate:207' navState.q_be.q1 = Q1.q1*normQuat; */
        navState->q_be.q1 = r2 * normQuat;

        /* 'KalmanFilterMeasurementUpdate:208' navState.q_be.q2 = Q1.q2*normQuat; */
        navState->q_be.q2 = r3 * normQuat;

        /* 'KalmanFilterMeasurementUpdate:209' navState.q_be.q3 = Q1.q3*normQuat; */
        navState->q_be.q3 = Q_q0 * normQuat;

        /* 'KalmanFilterMeasurementUpdate:210' navState.dcm_be = LIB_q2d(navState.q_be); */
        LIB_q2d(navState->q_be.q0, navState->q_be.q1, navState->q_be.q2,
                navState->q_be.q3, navState->dcm_be);
      }
    } else {
      if (((((int32_T)measurementState_valid_baro) != 0) &&
           (measurementState_valid_mag == 0U)) && (((int32_T)
            measurementState_valid_feedback_nav) != 0)) {
        /* 'KalmanFilterMeasurementUpdate:213' elseif ((~measurementState.valid.gnss) && (~measurementState.valid.ta) && (measurementState.valid.baro) && (~measurementState.valid.mag)) */
        /* 'KalmanFilterMeasurementUpdate:214' if measurementState.valid.feedback_nav */
        /* 'KalmanFilterMeasurementUpdate:215' delta = measurementState.nav; */
        /* 'KalmanFilterMeasurementUpdate:216' kfState.X(1:6,1) = kfState.X(1:6,1) - delta(1:6,1); */
        for (i = 0; i < 6; i++) {
          kfState->X[i] -= measurementState_nav[i];
        }

        /* 'KalmanFilterMeasurementUpdate:217' navState.pos_e.x = navState.pos_e.x - delta(1,1); */
        navState->pos_e.x -= measurementState_nav[0];

        /* 'KalmanFilterMeasurementUpdate:218' navState.pos_e.y = navState.pos_e.y - delta(2,1); */
        navState->pos_e.y -= measurementState_nav[1];

        /* 'KalmanFilterMeasurementUpdate:219' navState.pos_e.z = navState.pos_e.z - delta(3,1); */
        navState->pos_e.z -= measurementState_nav[2];

        /* 'KalmanFilterMeasurementUpdate:220' navState.vel_e.x = navState.vel_e.x - delta(4,1); */
        navState->vel_e.x -= measurementState_nav[3];

        /* 'KalmanFilterMeasurementUpdate:221' navState.vel_e.y = navState.vel_e.y - delta(5,1); */
        navState->vel_e.y -= measurementState_nav[4];

        /* 'KalmanFilterMeasurementUpdate:222' navState.vel_e.z = navState.vel_e.z - delta(6,1); */
        navState->vel_e.z -= measurementState_nav[5];

        /* 'KalmanFilterMeasurementUpdate:223' baroState.KalmanFilter_delta_baro = LocalBaroBias * (2.818913332005712e-17*baroState.comp_alt^4 - ... */
        /* 'KalmanFilterMeasurementUpdate:224'                                                                  3.863927801707316e-13*baroState.comp_alt^3 + ... */
        /* 'KalmanFilterMeasurementUpdate:225'                                                                  3.284001514864467e-09*baroState.comp_alt^2 + ... */
        /* 'KalmanFilterMeasurementUpdate:226'                                                                  1.485650642383993e-05*baroState.comp_alt +   ... */
        /* 'KalmanFilterMeasurementUpdate:227'                                                                  1.796515109540371e-01)/BaroScaleFactor; */
        baroState->KalmanFilter_delta_baro = ((((((2.8189133320057122E-17 *
          rt_powd_snf(baroState->comp_alt, 4.0)) - (3.8639278017073163E-13 *
          rt_powd_snf(baroState->comp_alt, 3.0))) + ((baroState->comp_alt *
          baroState->comp_alt) * 3.2840015148644669E-9)) +
          (1.4856506423839931E-5 * baroState->comp_alt)) + 0.17965151095403711) *
          dwork->LocalBaroBias) / dwork->BaroScaleFactor;
      }
    }
  }
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz'
 * function [versionNumberNAV, navigationMode, navigationstateINS, KalmanFilterState, BaroState] = NAV_Embedded_DoNavigationMain100Hz(InertialNavigationSensorGenericDataIn100Hz, taState, KalmanFilterDeltaState, gnssState, nav_prv)
 */
static void NAV_Embedded_DoNavigationMain100Hz(real_T
  InertialNavigationSensorGenericDataIn100Hz_SystemTime, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_x, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_y, real_T
  InertialNavigationSensorGenericDataIn100Hz_dangle_z, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_x, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_y, real_T
  InertialNavigationSensorGenericDataIn100Hz_dvelocity_z, real_T
  InertialNavigationSensorGenericDataIn100Hz_baro_SystemTime, real_T
  InertialNavigationSensorGenericDataIn100Hz_baro_pressure, const
  T_GUID_GuidCtrlFlags InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags,
  const T_Euler taState_attitude, const T_Cartesian taState_pos_e, const
  T_Cartesian taState_vel_e, uint8_T taState_valid, const
  T_NAV_KFilterErrorEstimateState *KalmanFilterDeltaState, const T_GNSS_Result
  *gnssState, real_T b_nav_prv_deltaT, real_T b_nav_prv_WE, uint16_T
  *navigationMode, T_NAV_NavigationState *navigationstateINS,
  T_NAV_KalmanFilterState *KalmanFilterState, T_NAV_BAROState *BaroState, DW
  *dwork)
{
  int32_T navigationQualityINS_private;
  real_T norm_imu_dvelocity;
  real_T gnssState_Vel[3];
  real_T temp_altitude;
  real_T b_a;
  real_T c_a;
  T_Cartesian expl_temp;
  T_Cartesian expl_temp_0;
  T_Euler expl_temp_1;
  real_T expl_temp_2;
  real_T expl_temp_3;
  real_T expl_temp_4;
  T_Cartesian expl_temp_5;
  T_Cartesian expl_temp_6;
  real_T tmp[9];
  uint32_T tmp_0;

  /* 'NAV_Embedded_DoNavigationMain100Hz:17' if isempty(navigationstateINS_private) */
  if (!dwork->navigationstateINS_private_not_empty) {
    /* 'NAV_Embedded_DoNavigationMain100Hz:18' default_pos = struct('x', 0.0, 'y', 0.0, 'z', 0.0); */
    /* 'NAV_Embedded_DoNavigationMain100Hz:19' default_vel = struct('x', 0.0, 'y', 0.0, 'z', 0.0); */
    /* 'NAV_Embedded_DoNavigationMain100Hz:20' default_euler = struct('phi', 0.0, 'the', 0.0, 'psi', 0.0); */
    /* 'NAV_Embedded_DoNavigationMain100Hz:21' navigationstateINS_private = init_ECEFnavigation(nav_prv.deltaT, nav_prv.WE, default_pos, default_vel, default_euler); */
    expl_temp.z = 0.0;
    expl_temp.y = 0.0;
    expl_temp.x = 0.0;
    expl_temp_0.z = 0.0;
    expl_temp_0.y = 0.0;
    expl_temp_0.x = 0.0;
    expl_temp_1.psi = 0.0;
    expl_temp_1.the = 0.0;
    expl_temp_1.phi = 0.0;
    init_ECEFnavigation(b_nav_prv_deltaT, b_nav_prv_WE, expl_temp, expl_temp_0,
                        expl_temp_1, &dwork->navigationstateINS_private);
    dwork->navigationstateINS_private_not_empty = true;

    /* 'NAV_Embedded_DoNavigationMain100Hz:22' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
    dwork->navigationstateINS_private.GuidCtrlFlags =
      InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

    /* 'NAV_Embedded_DoNavigationMain100Hz:23' KalmanFilterstate_private = init_KalmanFilter_16State; */
    init_KalmanFilter_16State(&dwork->KalmanFilterstate_private.states,
      &dwork->KalmanFilterstate_private.FeedbackWeight,
      &dwork->KalmanFilterstate_private.timeupdateINS,
      &dwork->KalmanFilterstate_private.timeupdateAttitude,
      &dwork->KalmanFilterstate_private.updateINS,
      &dwork->KalmanFilterstate_private.updateAttitude,
      &dwork->KalmanFilterstate_private.Reserved,
      dwork->KalmanFilterstate_private.X, dwork->KalmanFilterstate_private.A,
      dwork->KalmanFilterstate_private.P, dwork->KalmanFilterstate_private.Q,
      &dwork->KalmanFilterstate_private.R,
      dwork->KalmanFilterstate_private.B_gyro,
      dwork->KalmanFilterstate_private.B_accel,
      &dwork->KalmanFilterstate_private.align_valid);

    /* 'NAV_Embedded_DoNavigationMain100Hz:24' KalmanFilterDeltaState_private = struct('trade', uint32(0)); */
    dwork->KalmanFilterDeltaState_private.trade = 0U;

    /* 'NAV_Embedded_DoNavigationMain100Hz:25' BaroState_private = init_Baro; */
    init_Baro(&dwork->BaroState_private.SystemTime,
              &dwork->BaroState_private.alt, &dwork->BaroState_private.pressure,
              &dwork->BaroState_private.valid,
              &dwork->BaroState_private.Reserved01,
              &dwork->BaroState_private.Reserved02,
              &dwork->BaroState_private.comp_alt,
              &dwork->BaroState_private.KalmanFilter_delta_baro);

    /* 'NAV_Embedded_DoNavigationMain100Hz:26' BaroValidityTimeOut = 100.0; */
    /* 'NAV_Embedded_DoNavigationMain100Hz:27' BaroValidityTimeOutCounter = 0.0; */
    /* 'NAV_Embedded_DoNavigationMain100Hz:28' PreviousUmbilicalState = navigationstateINS_private.GuidCtrlFlags.Umbilical; */
    dwork->PreviousUmbilicalState =
      dwork->navigationstateINS_private.GuidCtrlFlags.Umbilical;

    /* 'NAV_Embedded_DoNavigationMain100Hz:29' nominal_orientation_private = init_Pylon; */
    init_Pylon(&dwork->nominal_orientation_private.attitude.phi,
               &dwork->nominal_orientation_private.attitude.the,
               &dwork->nominal_orientation_private.attitude.psi,
               &dwork->nominal_orientation_private.accel_limit_low,
               &dwork->nominal_orientation_private.accel_limit_high,
               &dwork->nominal_orientation_private.valid,
               &dwork->nominal_orientation_private.initial_attitude_set);

    /* 'NAV_Embedded_DoNavigationMain100Hz:30' number_imu_dvelocity_average_private = 1.0; */
    /* 'NAV_Embedded_DoNavigationMain100Hz:31' imu_dvelocity_x = 0.0; */
    /* 'NAV_Embedded_DoNavigationMain100Hz:32' imu_dvelocity_y = 0.0; */
    /* 'NAV_Embedded_DoNavigationMain100Hz:33' imu_dvelocity_z = 0.0; */
  }

  /* 'NAV_Embedded_DoNavigationMain100Hz:35' if isempty(navigationModeINS_private) */
  /* 'NAV_Embedded_DoNavigationMain100Hz:39' switch navigationModeINS_private */
  switch ((int32_T)dwork->navigationModeINS_private) {
   case 1:
   case 2:
    /* 'NAV_Embedded_DoNavigationMain100Hz:40' case {1, 2} */
    /* 'NAV_Embedded_DoNavigationMain100Hz:41' if taState.valid */
    if (((int32_T)taState_valid) != 0) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:42' navigationstateINS_private = init_ECEFnavigation(nav_prv.deltaT, nav_prv.WE, taState.pos_e, taState.vel_e, taState.attitude); */
      init_ECEFnavigation(b_nav_prv_deltaT, b_nav_prv_WE, taState_pos_e,
                          taState_vel_e, taState_attitude,
                          &dwork->navigationstateINS_private);

      /* 'NAV_Embedded_DoNavigationMain100Hz:43' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
      dwork->navigationstateINS_private.GuidCtrlFlags =
        InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

      /* 'NAV_Embedded_DoNavigationMain100Hz:44' navigationstateINS = navigationstateINS_private; */
      *navigationstateINS = dwork->navigationstateINS_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:45' KalmanFilterState = KalmanFilterstate_private; */
      *KalmanFilterState = dwork->KalmanFilterstate_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:46' BaroState = BaroState_private; */
      *BaroState = dwork->BaroState_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:47' navigationModeINS_private = 3; */
      dwork->navigationModeINS_private = 3.0;

      /* 'NAV_Embedded_DoNavigationMain100Hz:48' navigationQualityINS_private = 9; */
      navigationQualityINS_private = 9;
    } else if (((int32_T)gnssState->Valid) != 0) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:49' elseif gnssState.Valid */
      /* 'NAV_Embedded_DoNavigationMain100Hz:50' norm_imu_dvelocity = ((InertialNavigationSensorGenericDataIn100Hz.dvelocity.x*100.0)^2.0 + (InertialNavigationSensorGenericDataIn100Hz.dvelocity.y*100.0)^2.0 + (InertialNavigationSensorGenericDataIn100Hz.dvelocity.z*100.0)^2.0)^0.5; */
      norm_imu_dvelocity =
        InertialNavigationSensorGenericDataIn100Hz_dvelocity_x * 100.0;
      b_a = InertialNavigationSensorGenericDataIn100Hz_dvelocity_y * 100.0;
      c_a = InertialNavigationSensorGenericDataIn100Hz_dvelocity_z * 100.0;
      norm_imu_dvelocity = sqrt(((norm_imu_dvelocity * norm_imu_dvelocity) +
        (b_a * b_a)) + (c_a * c_a));

      /* 'NAV_Embedded_DoNavigationMain100Hz:51' if norm_imu_dvelocity ~= 0.0 */
      if (norm_imu_dvelocity != 0.0) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:52' if number_imu_dvelocity_average_private == 1 */
        if (dwork->number_imu_dvelocity_average_private == 1.0) {
          /* 'NAV_Embedded_DoNavigationMain100Hz:53' imu_dvelocity_x = (InertialNavigationSensorGenericDataIn100Hz.dvelocity.x*100.0)/-norm_imu_dvelocity; */
          dwork->imu_dvelocity_x =
            (InertialNavigationSensorGenericDataIn100Hz_dvelocity_x * 100.0) / (
            -norm_imu_dvelocity);

          /* 'NAV_Embedded_DoNavigationMain100Hz:54' imu_dvelocity_y = (InertialNavigationSensorGenericDataIn100Hz.dvelocity.y*100.0)/-norm_imu_dvelocity; */
          dwork->imu_dvelocity_y =
            (InertialNavigationSensorGenericDataIn100Hz_dvelocity_y * 100.0) / (
            -norm_imu_dvelocity);

          /* 'NAV_Embedded_DoNavigationMain100Hz:55' imu_dvelocity_z = (InertialNavigationSensorGenericDataIn100Hz.dvelocity.z*100.0)/-norm_imu_dvelocity; */
          dwork->imu_dvelocity_z =
            (InertialNavigationSensorGenericDataIn100Hz_dvelocity_z * 100.0) / (
            -norm_imu_dvelocity);

          /* 'NAV_Embedded_DoNavigationMain100Hz:56' number_imu_dvelocity_average_private = number_imu_dvelocity_average_private + 1.0; */
          dwork->number_imu_dvelocity_average_private++;
        } else {
          /* 'NAV_Embedded_DoNavigationMain100Hz:57' else */
          /* 'NAV_Embedded_DoNavigationMain100Hz:58' imu_dvelocity_x = imu_dvelocity_x + (((InertialNavigationSensorGenericDataIn100Hz.dvelocity.x*100.0)/-norm_imu_dvelocity) - imu_dvelocity_x)/number_imu_dvelocity_average_private; */
          dwork->imu_dvelocity_x +=
            (((InertialNavigationSensorGenericDataIn100Hz_dvelocity_x * 100.0) /
              (-norm_imu_dvelocity)) - dwork->imu_dvelocity_x) /
            dwork->number_imu_dvelocity_average_private;

          /* 'NAV_Embedded_DoNavigationMain100Hz:59' imu_dvelocity_y = imu_dvelocity_y + (((InertialNavigationSensorGenericDataIn100Hz.dvelocity.y*100.0)/-norm_imu_dvelocity) - imu_dvelocity_y)/number_imu_dvelocity_average_private; */
          dwork->imu_dvelocity_y +=
            (((InertialNavigationSensorGenericDataIn100Hz_dvelocity_y * 100.0) /
              (-norm_imu_dvelocity)) - dwork->imu_dvelocity_y) /
            dwork->number_imu_dvelocity_average_private;

          /* 'NAV_Embedded_DoNavigationMain100Hz:60' imu_dvelocity_z = imu_dvelocity_z + (((InertialNavigationSensorGenericDataIn100Hz.dvelocity.z*100.0)/-norm_imu_dvelocity) - imu_dvelocity_z)/number_imu_dvelocity_average_private; */
          dwork->imu_dvelocity_z +=
            (((InertialNavigationSensorGenericDataIn100Hz_dvelocity_z * 100.0) /
              (-norm_imu_dvelocity)) - dwork->imu_dvelocity_z) /
            dwork->number_imu_dvelocity_average_private;

          /* 'NAV_Embedded_DoNavigationMain100Hz:61' number_imu_dvelocity_average_private = number_imu_dvelocity_average_private + 1.0; */
          dwork->number_imu_dvelocity_average_private++;
        }
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:63' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:64' nominal_orientation_private.attitude.phi = 0.0; */
        dwork->nominal_orientation_private.attitude.phi = 0.0;

        /* 'NAV_Embedded_DoNavigationMain100Hz:65' nominal_orientation_private.attitude.the = 0.0; */
        dwork->nominal_orientation_private.attitude.the = 0.0;

        /* 'NAV_Embedded_DoNavigationMain100Hz:66' nominal_orientation_private.attitude.psi = 0.0; */
        dwork->nominal_orientation_private.attitude.psi = 0.0;
      }

      /* 'NAV_Embedded_DoNavigationMain100Hz:68' if norm(gnssState.Vel) > 3.0 */
      if (norm_xgB9aDcJ(gnssState->Vel) > 3.0F) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:69' gnssState_Vel = double(gnssState.Vel); */
        /* 'NAV_Embedded_DoNavigationMain100Hz:70' gnssState_ecef_pos = struct('x', gnssState.Pos(1), 'y', gnssState.Pos(2), 'z', gnssState.Pos(3)); */
        /* 'NAV_Embedded_DoNavigationMain100Hz:71' gnssState_ecef_vel = struct('x', gnssState_Vel(1), 'y', gnssState_Vel(2), 'z', gnssState_Vel(3)); */
        expl_temp_6.x = (real_T)gnssState->Vel[0];
        expl_temp_6.y = (real_T)gnssState->Vel[1];
        expl_temp_6.z = (real_T)gnssState->Vel[2];

        /* 'NAV_Embedded_DoNavigationMain100Hz:72' gnssState_geo_pos  = LIB_ecef2lla(gnssState_ecef_pos)              ; */
        LIB_ecef2lla(gnssState->Pos[0], gnssState->Pos[1], gnssState->Pos[2],
                     &b_a, &c_a, &norm_imu_dvelocity);

        /* 'NAV_Embedded_DoNavigationMain100Hz:73' Cen = LIB_dcm_en(gnssState_geo_pos)               ; */
        /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
        /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
        /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
        /* 'NAV_Embedded_DoNavigationMain100Hz:74' gnssState_geo_vel  = Cen * double(gnssState.Vel)'; */
        norm_imu_dvelocity = -sin(b_a);
        expl_temp_2 = cos(c_a);
        tmp[0] = norm_imu_dvelocity * expl_temp_2;
        c_a = sin(c_a);
        tmp[3] = norm_imu_dvelocity * c_a;
        expl_temp_3 = cos(b_a);
        tmp[6] = expl_temp_3;
        tmp[1] = -c_a;
        tmp[4] = expl_temp_2;
        tmp[7] = 0.0;
        tmp[2] = (-expl_temp_3) * expl_temp_2;
        tmp[5] = (-cos(b_a)) * c_a;
        tmp[8] = norm_imu_dvelocity;
        for (navigationQualityINS_private = 0; navigationQualityINS_private < 3;
             navigationQualityINS_private++) {
          gnssState_Vel[navigationQualityINS_private] =
            (tmp[navigationQualityINS_private + 6] * ((real_T)gnssState->Vel[2]))
            + ((tmp[navigationQualityINS_private + 3] * ((real_T)gnssState->Vel
                 [1])) + (tmp[navigationQualityINS_private] * ((real_T)
                 gnssState->Vel[0])));
        }

        /* 'NAV_Embedded_DoNavigationMain100Hz:75' nominal_orientation_private.attitude.phi = atan2((imu_dvelocity_y), (imu_dvelocity_z)); */
        dwork->nominal_orientation_private.attitude.phi = rt_atan2d_snf
          (dwork->imu_dvelocity_y, dwork->imu_dvelocity_z);

        /* 'NAV_Embedded_DoNavigationMain100Hz:76' nominal_orientation_private.attitude.the = atan2((-imu_dvelocity_x),(((imu_dvelocity_y)^2.0+(imu_dvelocity_z)^2.0)^0.5)); */
        dwork->nominal_orientation_private.attitude.the = rt_atan2d_snf
          (-dwork->imu_dvelocity_x, sqrt((dwork->imu_dvelocity_y *
             dwork->imu_dvelocity_y) + (dwork->imu_dvelocity_z *
             dwork->imu_dvelocity_z)));

        /* 'NAV_Embedded_DoNavigationMain100Hz:77' nominal_orientation_private.attitude.psi = atan2(gnssState_geo_vel(2), gnssState_geo_vel(1)); */
        dwork->nominal_orientation_private.attitude.psi = rt_atan2d_snf
          (gnssState_Vel[1], gnssState_Vel[0]);

        /* 'NAV_Embedded_DoNavigationMain100Hz:78' navigationstateINS_private = init_ECEFnavigation(nav_prv.deltaT, nav_prv.WE, gnssState_ecef_pos, gnssState_ecef_vel, nominal_orientation_private.attitude); */
        expl_temp_5.z = gnssState->Pos[2];
        expl_temp_5.y = gnssState->Pos[1];
        expl_temp_5.x = gnssState->Pos[0];
        init_ECEFnavigation(b_nav_prv_deltaT, b_nav_prv_WE, expl_temp_5,
                            expl_temp_6,
                            dwork->nominal_orientation_private.attitude,
                            &dwork->navigationstateINS_private);

        /* 'NAV_Embedded_DoNavigationMain100Hz:79' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
        dwork->navigationstateINS_private.GuidCtrlFlags =
          InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

        /* 'NAV_Embedded_DoNavigationMain100Hz:80' navigationstateINS = navigationstateINS_private; */
        *navigationstateINS = dwork->navigationstateINS_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:81' KalmanFilterState = KalmanFilterstate_private; */
        *KalmanFilterState = dwork->KalmanFilterstate_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:82' BaroState = BaroState_private; */
        *BaroState = dwork->BaroState_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:83' navigationModeINS_private = 3; */
        dwork->navigationModeINS_private = 3.0;

        /* 'NAV_Embedded_DoNavigationMain100Hz:84' navigationQualityINS_private = 9; */
        navigationQualityINS_private = 9;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:85' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:86' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
        dwork->navigationstateINS_private.GuidCtrlFlags =
          InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

        /* 'NAV_Embedded_DoNavigationMain100Hz:87' navigationstateINS = navigationstateINS_private; */
        *navigationstateINS = dwork->navigationstateINS_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:88' KalmanFilterState = KalmanFilterstate_private; */
        *KalmanFilterState = dwork->KalmanFilterstate_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:89' BaroState = BaroState_private; */
        *BaroState = dwork->BaroState_private;

        /* 'NAV_Embedded_DoNavigationMain100Hz:90' navigationModeINS_private = 2; */
        dwork->navigationModeINS_private = 2.0;

        /* 'NAV_Embedded_DoNavigationMain100Hz:91' navigationQualityINS_private = 9; */
        navigationQualityINS_private = 9;
      }
    } else {
      /* 'NAV_Embedded_DoNavigationMain100Hz:93' else */
      /* 'NAV_Embedded_DoNavigationMain100Hz:94' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
      dwork->navigationstateINS_private.GuidCtrlFlags =
        InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

      /* 'NAV_Embedded_DoNavigationMain100Hz:95' navigationstateINS = navigationstateINS_private; */
      *navigationstateINS = dwork->navigationstateINS_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:96' KalmanFilterState = KalmanFilterstate_private; */
      *KalmanFilterState = dwork->KalmanFilterstate_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:97' BaroState = BaroState_private; */
      *BaroState = dwork->BaroState_private;

      /* 'NAV_Embedded_DoNavigationMain100Hz:98' navigationModeINS_private = 1; */
      dwork->navigationModeINS_private = 1.0;

      /* 'NAV_Embedded_DoNavigationMain100Hz:99' navigationQualityINS_private = 9; */
      navigationQualityINS_private = 9;
    }
    break;

   case 3:
    /* 'NAV_Embedded_DoNavigationMain100Hz:101' case 3 */
    /* 'NAV_Embedded_DoNavigationMain100Hz:102' navigationstateINS_private.GuidCtrlFlags = InertialNavigationSensorGenericDataIn100Hz.GuidCtrlFlags; */
    dwork->navigationstateINS_private.GuidCtrlFlags =
      InertialNavigationSensorGenericDataIn100Hz_GuidCtrlFlags;

    /* 'NAV_Embedded_DoNavigationMain100Hz:103' if PreviousUmbilicalState ~= navigationstateINS_private.GuidCtrlFlags.Umbilical */
    if (dwork->PreviousUmbilicalState !=
        dwork->navigationstateINS_private.GuidCtrlFlags.Umbilical) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:104' BaroState_private.KalmanFilter_delta_baro = 0.0; */
      dwork->BaroState_private.KalmanFilter_delta_baro = 0.0;
    }

    /* 'NAV_Embedded_DoNavigationMain100Hz:106' PreviousUmbilicalState =  navigationstateINS_private.GuidCtrlFlags.Umbilical; */
    dwork->PreviousUmbilicalState =
      dwork->navigationstateINS_private.GuidCtrlFlags.Umbilical;

    /* 'NAV_Embedded_DoNavigationMain100Hz:108' if InertialNavigationSensorGenericDataIn100Hz.baro_SystemTime > BaroState_private.SystemTime */
    if (InertialNavigationSensorGenericDataIn100Hz_baro_SystemTime >
        dwork->BaroState_private.SystemTime) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:111' temp = LIB_StandardAtmospherePressure(InertialNavigationSensorGenericDataIn100Hz.baro_pressure); */
      LIB_StandardAtmospherePressure
        (InertialNavigationSensorGenericDataIn100Hz_baro_pressure,
         &norm_imu_dvelocity, &b_a, &c_a, &expl_temp_2, &expl_temp_3,
         &temp_altitude, &expl_temp_4);

      /* 'NAV_Embedded_DoNavigationMain100Hz:112' InertialNavigationSensorGenericDataIn100Hz.baro_alt = temp.altitude; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:113' BaroState_private.SystemTime = InertialNavigationSensorGenericDataIn100Hz.baro_SystemTime; */
      dwork->BaroState_private.SystemTime =
        InertialNavigationSensorGenericDataIn100Hz_baro_SystemTime;

      /* 'NAV_Embedded_DoNavigationMain100Hz:114' BaroState_private.alt = InertialNavigationSensorGenericDataIn100Hz.baro_alt; */
      dwork->BaroState_private.alt = temp_altitude;

      /* 'NAV_Embedded_DoNavigationMain100Hz:115' BaroState_private.pressure = InertialNavigationSensorGenericDataIn100Hz.baro_pressure; */
      dwork->BaroState_private.pressure =
        InertialNavigationSensorGenericDataIn100Hz_baro_pressure;

      /* 'NAV_Embedded_DoNavigationMain100Hz:116' BaroState_private.valid = uint8(1); */
      dwork->BaroState_private.valid = 1U;

      /* 'NAV_Embedded_DoNavigationMain100Hz:117' BaroState_private.comp_alt = InertialNavigationSensorGenericDataIn100Hz.baro_alt - BaroState_private.KalmanFilter_delta_baro; */
      dwork->BaroState_private.comp_alt = temp_altitude -
        dwork->BaroState_private.KalmanFilter_delta_baro;

      /* 'NAV_Embedded_DoNavigationMain100Hz:118' BaroState_private.KalmanFilter_delta_baro = BaroState_private.KalmanFilter_delta_baro; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:119' BaroValidityTimeOutCounter = 0.0; */
      dwork->BaroValidityTimeOutCounter = 0.0;
    } else {
      /* 'NAV_Embedded_DoNavigationMain100Hz:120' else */
      /* 'NAV_Embedded_DoNavigationMain100Hz:121' BaroState_private.SystemTime = BaroState_private.SystemTime; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:122' BaroState_private.alt = BaroState_private.alt; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:123' BaroState_private.pressure = BaroState_private.pressure; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:124' BaroValidityTimeOutCounter = BaroValidityTimeOutCounter + 1.0; */
      dwork->BaroValidityTimeOutCounter++;

      /* 'NAV_Embedded_DoNavigationMain100Hz:125' if BaroValidityTimeOutCounter >= BaroValidityTimeOut */
      if (dwork->BaroValidityTimeOutCounter >= dwork->BaroValidityTimeOut) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:126' BaroState_private.valid = uint8(0); */
        dwork->BaroState_private.valid = 0U;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:127' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:128' BaroState_private.valid = BaroState_private.valid; */
      }

      /* 'NAV_Embedded_DoNavigationMain100Hz:130' BaroState_private.comp_alt = BaroState_private.comp_alt; */
      /* 'NAV_Embedded_DoNavigationMain100Hz:131' BaroState_private.KalmanFilter_delta_baro = BaroState_private.KalmanFilter_delta_baro; */
    }

    /* 'NAV_Embedded_DoNavigationMain100Hz:134' [navigationstateINS_private, KalmanFilterstate_private] = DoNavigationMain100Hz(InertialNavigationSensorGenericDataIn100Hz.SystemTime, ... */
    /* 'NAV_Embedded_DoNavigationMain100Hz:135'                                                                                       InertialNavigationSensorGenericDataIn100Hz.dangle, ... */
    /* 'NAV_Embedded_DoNavigationMain100Hz:136'                                                                                       InertialNavigationSensorGenericDataIn100Hz.dvelocity, ... */
    /* 'NAV_Embedded_DoNavigationMain100Hz:137'                                                                                       navigationstateINS_private, ... */
    /* 'NAV_Embedded_DoNavigationMain100Hz:138'                                                                                       KalmanFilterstate_private, ... */
    /* 'NAV_Embedded_DoNavigationMain100Hz:139'                                                                                       BaroState_private.comp_alt); */
    DoNavigationMain100Hz(InertialNavigationSensorGenericDataIn100Hz_SystemTime,
                          InertialNavigationSensorGenericDataIn100Hz_dangle_x,
                          InertialNavigationSensorGenericDataIn100Hz_dangle_y,
                          InertialNavigationSensorGenericDataIn100Hz_dangle_z,
                          InertialNavigationSensorGenericDataIn100Hz_dvelocity_x,
                          InertialNavigationSensorGenericDataIn100Hz_dvelocity_y,
                          InertialNavigationSensorGenericDataIn100Hz_dvelocity_z,
                          &dwork->navigationstateINS_private,
                          &dwork->KalmanFilterstate_private,
                          dwork->BaroState_private.comp_alt);

    /* 'NAV_Embedded_DoNavigationMain100Hz:142' if KalmanFilterDeltaState.trade ~= KalmanFilterDeltaState_private.trade */
    if (KalmanFilterDeltaState->trade !=
        dwork->KalmanFilterDeltaState_private.trade) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:143' KalmanFilterDeltaState_private.trade = KalmanFilterDeltaState.trade; */
      dwork->KalmanFilterDeltaState_private.trade =
        KalmanFilterDeltaState->trade;

      /* 'NAV_Embedded_DoNavigationMain100Hz:144' [navigationstateINS_private, KalmanFilterstate_private, BaroState_private] = KalmanFilterMeasurementUpdate(KalmanFilterDeltaState, ... */
      /* 'NAV_Embedded_DoNavigationMain100Hz:145'                                                                                                                    navigationstateINS_private, ... */
      /* 'NAV_Embedded_DoNavigationMain100Hz:146'                                                                                                                    KalmanFilterstate_private, ... */
      /* 'NAV_Embedded_DoNavigationMain100Hz:147'                                                                                                                    BaroState_private); */
      KalmanFilterMeasurementUpdate(KalmanFilterDeltaState->nav,
        KalmanFilterDeltaState->kalmanfilter.state,
        KalmanFilterDeltaState->kalmanfilter.covariance,
        KalmanFilterDeltaState->valid.gnss, KalmanFilterDeltaState->valid.baro,
        KalmanFilterDeltaState->valid.ta, KalmanFilterDeltaState->valid.mag,
        KalmanFilterDeltaState->valid.feedback_nav,
        KalmanFilterDeltaState->valid.feedback_att,
        KalmanFilterDeltaState->valid.reset_navigation,
        &dwork->navigationstateINS_private, &dwork->KalmanFilterstate_private,
        &dwork->BaroState_private, dwork);
    }

    /* 'NAV_Embedded_DoNavigationMain100Hz:151' if (((KalmanFilterstate_private.P(7,7) + KalmanFilterstate_private.P(8,8) + KalmanFilterstate_private.P(9,9))^0.5) <= 0.09) */
    if (sqrt((dwork->KalmanFilterstate_private.P[102] +
              dwork->KalmanFilterstate_private.P[119]) +
             dwork->KalmanFilterstate_private.P[136]) <= 0.09) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:152' KalmanFilterstate_private.align_valid = uint8(1); */
      dwork->KalmanFilterstate_private.align_valid = 1U;
    } else {
      /* 'NAV_Embedded_DoNavigationMain100Hz:153' else */
      /* 'NAV_Embedded_DoNavigationMain100Hz:154' KalmanFilterstate_private.align_valid = uint8(0); */
      dwork->KalmanFilterstate_private.align_valid = 0U;
    }

    /* 'NAV_Embedded_DoNavigationMain100Hz:157' navigationstateINS = navigationstateINS_private; */
    *navigationstateINS = dwork->navigationstateINS_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:158' KalmanFilterState = KalmanFilterstate_private; */
    *KalmanFilterState = dwork->KalmanFilterstate_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:159' BaroState = BaroState_private; */
    *BaroState = dwork->BaroState_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:160' navigationModeINS_private = 3.0; */
    dwork->navigationModeINS_private = 3.0;

    /* 'NAV_Embedded_DoNavigationMain100Hz:161' if ((KalmanFilterDeltaState.valid.gnss) && (KalmanFilterDeltaState.valid.ta)) */
    if ((((int32_T)KalmanFilterDeltaState->valid.gnss) != 0) && (((int32_T)
          KalmanFilterDeltaState->valid.ta) != 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:162' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      tmp_0 = ((uint32_T)
               dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U;
      if (tmp_0 != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:163' navigationQualityINS_private = 30.0; */
        navigationQualityINS_private = 30;
      } else if ((tmp_0 != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:164' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:165' navigationQualityINS_private = 60.0; */
        navigationQualityINS_private = 60;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:166' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:167' navigationQualityINS_private = 0.0; */
        navigationQualityINS_private = 0;
      }
    } else if ((((int32_T)KalmanFilterDeltaState->valid.gnss) != 0) &&
               (((int32_T)KalmanFilterDeltaState->valid.ta) == 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:169' elseif ((KalmanFilterDeltaState.valid.gnss) && (~KalmanFilterDeltaState.valid.ta)) */
      /* 'NAV_Embedded_DoNavigationMain100Hz:170' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:171' navigationQualityINS_private = 31.0; */
        navigationQualityINS_private = 31;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:172' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:173' navigationQualityINS_private = 61.0; */
        navigationQualityINS_private = 61;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:174' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:175' navigationQualityINS_private = 1.0; */
        navigationQualityINS_private = 1;
      }
    } else if ((((int32_T)KalmanFilterDeltaState->valid.gnss) == 0) &&
               (((int32_T)KalmanFilterDeltaState->valid.ta) != 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:177' elseif ((~KalmanFilterDeltaState.valid.gnss) && (KalmanFilterDeltaState.valid.ta)) */
      /* 'NAV_Embedded_DoNavigationMain100Hz:178' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:179' navigationQualityINS_private = 32.0; */
        navigationQualityINS_private = 32;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:180' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:181' navigationQualityINS_private = 62.0; */
        navigationQualityINS_private = 62;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:182' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:183' navigationQualityINS_private = 2.0; */
        navigationQualityINS_private = 2;
      }
    } else if ((((KalmanFilterDeltaState->valid.mag != 0U) && (((int32_T)
        KalmanFilterDeltaState->valid.baro) != 0)) && (((int32_T)
                  KalmanFilterDeltaState->valid.gnss) == 0)) && (((int32_T)
                 KalmanFilterDeltaState->valid.ta) == 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:185' elseif ((KalmanFilterDeltaState.valid.mag) && (KalmanFilterDeltaState.valid.baro) && (~KalmanFilterDeltaState.valid.gnss) && (~KalmanFilterDeltaState.valid.ta)) */
      /* 'NAV_Embedded_DoNavigationMain100Hz:186' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:187' navigationQualityINS_private = 33.0; */
        navigationQualityINS_private = 33;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:188' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:189' navigationQualityINS_private = 63.0; */
        navigationQualityINS_private = 63;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:190' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:191' navigationQualityINS_private = 3.0; */
        navigationQualityINS_private = 3;
      }
    } else if ((((KalmanFilterDeltaState->valid.mag == 0U) && (((int32_T)
        KalmanFilterDeltaState->valid.baro) != 0)) && (((int32_T)
                  KalmanFilterDeltaState->valid.gnss) == 0)) && (((int32_T)
                 KalmanFilterDeltaState->valid.ta) == 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:193' elseif ((~KalmanFilterDeltaState.valid.mag) && (KalmanFilterDeltaState.valid.baro) && (~KalmanFilterDeltaState.valid.gnss) && (~KalmanFilterDeltaState.valid.ta)) */
      /* 'NAV_Embedded_DoNavigationMain100Hz:194' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:195' navigationQualityINS_private = 34.0; */
        navigationQualityINS_private = 34;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:196' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:197' navigationQualityINS_private = 64.0; */
        navigationQualityINS_private = 64;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:198' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:199' navigationQualityINS_private = 4.0; */
        navigationQualityINS_private = 4;
      }
    } else if ((((KalmanFilterDeltaState->valid.mag != 0U) && (((int32_T)
        KalmanFilterDeltaState->valid.baro) == 0)) && (((int32_T)
                  KalmanFilterDeltaState->valid.gnss) == 0)) && (((int32_T)
                 KalmanFilterDeltaState->valid.ta) == 0)) {
      /* 'NAV_Embedded_DoNavigationMain100Hz:201' elseif ((KalmanFilterDeltaState.valid.mag) && (~KalmanFilterDeltaState.valid.baro) && (~KalmanFilterDeltaState.valid.gnss) && (~KalmanFilterDeltaState.valid.ta)) */
      /* 'NAV_Embedded_DoNavigationMain100Hz:202' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:203' navigationQualityINS_private = 35.0; */
        navigationQualityINS_private = 35;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:204' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:205' navigationQualityINS_private = 65.0; */
        navigationQualityINS_private = 65;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:206' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:207' navigationQualityINS_private = 5.0; */
        navigationQualityINS_private = 5;
      }
    } else {
      /* 'NAV_Embedded_DoNavigationMain100Hz:209' else */
      /* 'NAV_Embedded_DoNavigationMain100Hz:210' if (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) */
      if ((((uint32_T)
            dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag) & 1U)
          != 0U) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:211' navigationQualityINS_private = 39.0; */
        navigationQualityINS_private = 39;
      } else if (((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
                   & 1U) != 0U) && ((((uint32_T)
                    dwork->navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag)
        & 2U) != 0U)) {
        /* 'NAV_Embedded_DoNavigationMain100Hz:212' elseif ((bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navigationstateINS_private.GuidCtrlFlags.NavCtrlFlag,2))) */
        /* 'NAV_Embedded_DoNavigationMain100Hz:213' navigationQualityINS_private = 69.0; */
        navigationQualityINS_private = 69;
      } else {
        /* 'NAV_Embedded_DoNavigationMain100Hz:214' else */
        /* 'NAV_Embedded_DoNavigationMain100Hz:215' navigationQualityINS_private = 9.0; */
        navigationQualityINS_private = 9;
      }
    }
    break;

   default:
    /* 'NAV_Embedded_DoNavigationMain100Hz:218' otherwise */
    /* 'NAV_Embedded_DoNavigationMain100Hz:219' navigationstateINS = navigationstateINS_private; */
    *navigationstateINS = dwork->navigationstateINS_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:220' KalmanFilterState = KalmanFilterstate_private; */
    *KalmanFilterState = dwork->KalmanFilterstate_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:221' BaroState = BaroState_private; */
    *BaroState = dwork->BaroState_private;

    /* 'NAV_Embedded_DoNavigationMain100Hz:222' navigationModeINS_private = 1.0; */
    dwork->navigationModeINS_private = 1.0;

    /* 'NAV_Embedded_DoNavigationMain100Hz:223' navigationQualityINS_private = 0.0; */
    navigationQualityINS_private = 0;
    break;
  }

  /* 'NAV_Embedded_DoNavigationMain100Hz:225' versionNumberNAV = define_VersionNumberNAV; */
  /* 'NAV_Embedded_DoNavigationMain100Hz:226' navigationMode = uint16(navigationModeINS_private*10.0 + navigationQualityINS_private); */
  norm_imu_dvelocity = rt_roundd_snf((dwork->navigationModeINS_private * 10.0) +
    ((real_T)navigationQualityINS_private));
  if (norm_imu_dvelocity < 65536.0) {
    if (norm_imu_dvelocity >= 0.0) {
      *navigationMode = (uint16_T)norm_imu_dvelocity;
    } else {
      *navigationMode = 0U;
    }
  } else {
    *navigationMode = MAX_uint16_T;
  }
}

/* Model step function */
void NAV_DoNavigation_100Hz(RT_MODEL *const NAV_DoNavigationMain_100Hz_M,
  T_NAV_GenericInertialNavigationSensor *rtU_GenericINSensor100Hz,
  T_TA_TransferAlignState *rtU_TransferAlignState,
  T_NAV_KFilterErrorEstimateState *rtU_KalmanFilterErrorEstimates, T_GNSS_Result
  *rtU_GNSS_Result, T_NAV_State *rtY_Nav_State)
{
  T_NAV_BAROState BaroState;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_100Hz_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Subsystem'
   */
  /* MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz' incorporates:
   *  Inport: '<Root>/GNSS_Result'
   *  Inport: '<Root>/GenericINDataIn'
   *  Inport: '<Root>/KalmanFilterErrorEstimates'
   *  Inport: '<Root>/TransferAlignState'
   */
  /* :  [versionNumberNAV, navigationMode, navigationstateINS, KalmanFilterState, BaroState] = NAV_Embedded_DoNavigationMain100Hz(InertialNavigationSensorGenericDataIn100Hz, taState, KalmanFilterDeltaState, gnssState, nav_prv); */
  NAV_Embedded_DoNavigationMain100Hz(rtU_GenericINSensor100Hz->SystemTime,
    rtU_GenericINSensor100Hz->dangle.x, rtU_GenericINSensor100Hz->dangle.y,
    rtU_GenericINSensor100Hz->dangle.z, rtU_GenericINSensor100Hz->dvelocity.x,
    rtU_GenericINSensor100Hz->dvelocity.y, rtU_GenericINSensor100Hz->dvelocity.z,
    rtU_GenericINSensor100Hz->baro_SystemTime,
    rtU_GenericINSensor100Hz->baro_pressure,
    rtU_GenericINSensor100Hz->GuidCtrlFlags, rtU_TransferAlignState->attitude,
    rtU_TransferAlignState->pos_e, rtU_TransferAlignState->vel_e,
    rtU_TransferAlignState->valid, rtU_KalmanFilterErrorEstimates,
    rtU_GNSS_Result, 0.01, 7.2921151467E-5, &rtY_Nav_State->NavigationModeOut,
    &rtY_Nav_State->NAV_NavigationState, &(NAV_DoNavigationMain_100Hz_M)
    ->dwork.KalmanFilterState, &BaroState, &(NAV_DoNavigationMain_100Hz_M->dwork));
  rtY_Nav_State->versionNumberNAV = 100U;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.SystemTime =
    BaroState.SystemTime;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.alt = BaroState.alt;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.pressure = BaroState.pressure;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.valid = BaroState.valid;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.Reserved01 =
    BaroState.Reserved01;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.Reserved02 =
    BaroState.Reserved02;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.comp_alt = BaroState.comp_alt;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroState.KalmanFilter_delta_baro =
    BaroState.KalmanFilter_delta_baro;

  /* BusCreator: '<S1>/Bus Creator' incorporates:
   *  Outport: '<Root>/Nav_State'
   */
  rtY_Nav_State->NAV_KalmanFilterState =
    NAV_DoNavigationMain_100Hz_M->dwork.KalmanFilterState;
  rtY_Nav_State->NAV_BaroState = NAV_DoNavigationMain_100Hz_M->dwork.BaroState;

  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_100Hz_at_outport_1' */
}

/* Model initialize function */
void NAV_DoNavigationMain_100Hz_initialize(RT_MODEL *const
  NAV_DoNavigationMain_100Hz_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_100Hz_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Subsystem'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain100Hz' */
  /* 'NAV_Embedded_DoNavigationMain100Hz:36' navigationModeINS_private = 1; */
  NAV_DoNavigationMain_100Hz_M->dwork.navigationModeINS_private = 1.0;
  NAV_DoNavigationMain_100Hz_M->dwork.BaroValidityTimeOut = 100.0;
  NAV_DoNavigationMain_100Hz_M->dwork.number_imu_dvelocity_average_private = 1.0;

  /* 'KalmanFilterMeasurementUpdate:35' BaroScaleFactor = 1.0; */
  NAV_DoNavigationMain_100Hz_M->dwork.BaroScaleFactor = 1.0;

  /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_100Hz_at_outport_1' */
  /* 'KalmanFilterMeasurementUpdate:38' LocalBaroBias = 0.0; */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
