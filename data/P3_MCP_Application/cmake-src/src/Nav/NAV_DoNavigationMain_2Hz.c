/*
 * File: NAV_DoNavigationMain_2Hz.c
 *
 * Code generated for Simulink model 'NAV_DoNavigationMain_2Hz'.
 *
 * Model version                  : 1.619
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Wed Jan 18 11:09:50 2023
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

#include "NAV_DoNavigationMain_2Hz.h"
#include "det_U5whr0XH.h"
#include "eye_iBsVrrYi.h"
#include "inv_GAaZsA6r.h"
#include "inv_dM5u0dSj.h"
#include "inv_ga6ZmLc1.h"
#include "inv_h7eIJkDj.h"
#include "inv_l0Es7Mh4.h"
#include "norm_1SDG5Mdv.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"
#include "rt_roundd_snf.h"
#include "svd_8vZPMSfD.h"
#include <math.h>

/* Forward declaration for local functions */
static void MAG_WorldMagneticModel(real_T Height, real_T Lat, real_T Lon, real_T
  XYZ[3], DW *dwork);
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9]);
static void LIB_d2e(const real_T dcm[9], real_T *eg_phi, real_T *eg_the, real_T *
                    eg_psi);
static void MAG_Run(T_MAG_State *State, real_T Input_TimeStamp, uint32_T
                    Input_Flags, const real_T Input_FieldMeas[3], const real_T
                    Input_FieldWMM[3], const real_T Input_Attitude[3], uint32_T *
                    Output_Valid, real_T Output_Attitude[3], real_T Output_P[9],
                    T_MAG_Tel *Tel);
static void MAG_DoMagnetometerMain2Hz(uint8_T magMeasurement_Valid, real_T
  magMeasurement_SystemTime, real_T magMeasurement_Field_x, real_T
  magMeasurement_Field_y, real_T magMeasurement_Field_z, const
  T_NAV_NavigationState *navState, uint16_T NavigationMode, const
  T_NAV_KalmanFilterState *kfState, uint32_T *magOutput_Valid, real_T
  magOutput_Attitude[3], real_T magOutput_P[9], T_MAG_Tel *magTelemetry, DW
  *dwork);
static void init_KalmanFilterErrorEstimates(real_T difference_nav[16], real_T
  difference_kalmanfilter_state[16], real_T difference_kalmanfilter_covariance
  [256], uint16_T *difference_numbersatellites, uint16_T *difference_Reserved,
  uint8_T *difference_valid_gnss, uint8_T *difference_valid_baro, uint8_T
  *difference_valid_ta, uint32_T *difference_valid_mag, uint8_T
  *difference_valid_feedback_nav, uint8_T *difference_valid_feedback_att,
  uint8_T *difference_valid_reset_navigation, uint32_T *difference_trade);
static void init_GNSSreceiver(real_T gnss_range[28], real_T gnss_rrate[28],
  real_T *gnss_ecef_pos_x, real_T *gnss_ecef_pos_y, real_T *gnss_ecef_pos_z,
  real_T *gnss_ecef_vel_x, real_T *gnss_ecef_vel_y, real_T *gnss_ecef_vel_z,
  real_T gnss_pos[84], real_T gnss_vel[84], uint16_T *gnss_numberSatellites,
  uint8_T *gnss_valid, uint8_T *gnss_Reserved, real_T *gnss_time);
static void GetCorrespondingNavigationData(const T_NAV_Store *store, real_T time,
  real_T data[16], uint32_T *info_TimeReq, uint8_T *info_Case, uint8_T
  *info_IdxLow, uint8_T *info_IdxHi, uint8_T *info_Reserved1, uint32_T
  *info_TimeLow, uint32_T *info_TimeHi, int32_T info_Pos[3], uint32_T
  *info_Reserved2);
static void LIB_lla2ecef(real_T lla_lat, real_T lla_lon, real_T lla_alt, real_T *
  pos_e_x, real_T *pos_e_y, real_T *pos_e_z);
static void LIB_ecef2lla(real_T re_x, real_T re_y, real_T re_z, real_T
  *pos_lla_lat, real_T *pos_lla_lon, real_T *pos_lla_alt);
static void LIB_q2d(real_T q_q0, real_T q_q1, real_T q_q2, real_T q_q3, real_T
                    dcm[9]);
static void GetECEFAttitudeErrorForGNSS(const real_T nav[16], const
  T_NAV_NavigationState *nav_state, real_T gnss_ecef_pos_x, real_T
  gnss_ecef_pos_y, real_T gnss_ecef_pos_z, real_T gnss_ecef_vel_x, real_T
  gnss_ecef_vel_y, real_T gnss_ecef_vel_z, real_T *attitude_error_x, real_T
  *attitude_error_y, real_T *attitude_error_z);
static void GetECEFAttitudeErrorForTA(const real_T nav[16], real_T
  ta_ac_attitude_psi, real_T ta_pos_g_lat, real_T ta_pos_g_lon, real_T
  *attitude_error_x, real_T *attitude_error_y, real_T *attitude_error_z);
static void GetECEFAttitudeErrorForMAG(const T_NAV_NavigationState *nav, const
  real_T mag_Attitude[3], real_T *attitude_error_x, real_T *attitude_error_y,
  real_T *attitude_error_z);

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [XYZ, H, D, I, F] = MAG_WorldMagneticModel(Height, Lat, Lon)
 */
static void MAG_WorldMagneticModel(real_T Height, real_T Lat, real_T Lon, real_T
  XYZ[3], DW *dwork)
{
  real_T snorm_data[169];
  real_T dp_data[169];
  real_T pp_data[13];
  real_T sp_data[13];
  real_T cp_data[13];
  real_T srlat;
  real_T crlat;
  real_T srlat2;
  real_T crlat2;
  real_T q;
  real_T q1;
  real_T ct;
  real_T st;
  real_T tc[169];
  real_T ar;
  real_T bpp;
  real_T par;
  real_T temp1;
  real_T temp2;
  int32_T m;
  int32_T b_m;
  int32_T snorm_data_tmp;

  /* 'MAG_WorldMagneticModel:37' if isempty(epoch) */
  /* 'MAG_WorldMagneticModel:51' dt = dyear - epoch; */
  /* 'MAG_WorldMagneticModel:54' if Lat > pi */
  if (Lat > 3.1415926535897931) {
    /* 'MAG_WorldMagneticModel:55' Lat = Lat - 2*pi; */
    Lat -= 6.2831853071795862;
  }

  /* 'MAG_WorldMagneticModel:59' if Lon > pi */
  if (Lon > 3.1415926535897931) {
    /* 'MAG_WorldMagneticModel:60' Lon = Lon - 2*pi; */
    Lon -= 6.2831853071795862;
  }

  /* 'MAG_WorldMagneticModel:63' maxord = maxdef + 1; */
  /* 'MAG_WorldMagneticModel:65' zeroMaxordM = zeros(maxord,maxord); */
  /* 'MAG_WorldMagneticModel:66' zeroMaxordA = zeros(1,maxord); */
  /* 'MAG_WorldMagneticModel:67' snorm = zeroMaxordM; */
  /* 'MAG_WorldMagneticModel:68' dp = zeroMaxordM; */
  memset(&snorm_data[0], 0, 169U * (sizeof(real_T)));
  memset(&dp_data[0], 0, 169U * (sizeof(real_T)));

  /* 'MAG_WorldMagneticModel:69' pp = ones(maxord,1); */
  /* 'MAG_WorldMagneticModel:70' sp = zeroMaxordA; */
  /* 'MAG_WorldMagneticModel:71' cp = zeroMaxordA; */
  for (m = 0; m < 13; m++) {
    pp_data[m] = 1.0;
    sp_data[m] = 0.0;
    cp_data[m] = 0.0;
  }

  /* 'MAG_WorldMagneticModel:74' Height = Height*0.001; */
  Height *= 0.001;

  /* 'MAG_WorldMagneticModel:77' re = 6371.2; */
  /* 'MAG_WorldMagneticModel:78' a = 6378.137; */
  /* 'MAG_WorldMagneticModel:79' b = 6356.7523142; */
  /* 'MAG_WorldMagneticModel:81' a2 = a*a; */
  /* 'MAG_WorldMagneticModel:82' b2 = b*b; */
  /* 'MAG_WorldMagneticModel:83' c2 = a2-b2; */
  /* 'MAG_WorldMagneticModel:84' a4 = a2*a2; */
  /* 'MAG_WorldMagneticModel:85' b4 = b2*b2; */
  /* 'MAG_WorldMagneticModel:86' c4 = a4 - b4; */
  /* 'MAG_WorldMagneticModel:88' rlon = Lon; */
  /* 'MAG_WorldMagneticModel:89' rlat = Lat; */
  /* 'MAG_WorldMagneticModel:90' srlon = sin(rlon); */
  /* 'MAG_WorldMagneticModel:91' srlat = sin(rlat); */
  srlat = sin(Lat);

  /* 'MAG_WorldMagneticModel:92' crlon = cos(rlon); */
  /* 'MAG_WorldMagneticModel:93' crlat = cos(rlat); */
  crlat = cos(Lat);

  /* 'MAG_WorldMagneticModel:94' srlat2 = srlat*srlat; */
  srlat2 = srlat * srlat;

  /* 'MAG_WorldMagneticModel:95' crlat2 = crlat*crlat; */
  crlat2 = crlat * crlat;

  /* 'MAG_WorldMagneticModel:98' q = sqrt(a2-c2*srlat2); */
  q = sqrt(4.0680631590768993E+7 - (272331.60668193549 * srlat2));

  /* 'MAG_WorldMagneticModel:99' q1 = Height*q; */
  q1 = Height * q;

  /* 'MAG_WorldMagneticModel:100' q2 = ((q1+a2)/(q1+b2))*((q1+a2)/(q1+b2)); */
  /* 'MAG_WorldMagneticModel:101' ct = srlat/sqrt(q2*crlat2+srlat2); */
  ct = (q1 + 4.0680631590768993E+7) / (q1 + 4.0408299984087057E+7);
  ct = srlat / sqrt(((ct * ct) * crlat2) + srlat2);

  /* 'MAG_WorldMagneticModel:102' st = sqrt(1.0-(ct*ct)); */
  st = sqrt(1.0 - (ct * ct));

  /* 'MAG_WorldMagneticModel:103' r2 = (Height*Height)+2.0*q1+(a4-c4*srlat2)/(q*q); */
  /* 'MAG_WorldMagneticModel:104' r = sqrt(r2); */
  q = sqrt(((1.6549137866238723E+15 - (2.208307901990225E+13 * srlat2)) / (q * q))
           + ((Height * Height) + (2.0 * q1)));

  /* 'MAG_WorldMagneticModel:105' d = sqrt(a2*crlat2+b2*srlat2); */
  crlat2 = sqrt((4.0680631590768993E+7 * crlat2) + (4.0408299984087057E+7 *
    srlat2));

  /* 'MAG_WorldMagneticModel:106' ca = (Height+d)/r; */
  srlat2 = (Height + crlat2) / q;

  /* 'MAG_WorldMagneticModel:107' sa = c2*crlat*srlat/(r*d); */
  crlat = ((272331.60668193549 * crlat) * srlat) / (q * crlat2);

  /* 'MAG_WorldMagneticModel:110' tc = c+dt*dc; */
  for (m = 0; m < 169; m++) {
    tc[m] = (3.5 * dwork->dc[m]) + dwork->c[m];
  }

  /* 'MAG_WorldMagneticModel:112' cp(1) = 1.0; */
  cp_data[0] = 1.0;

  /* 'MAG_WorldMagneticModel:113' pp(1) = 1.0; */
  pp_data[0] = 1.0;

  /* 'MAG_WorldMagneticModel:114' sp(2) = srlon; */
  sp_data[1] = sin(Lon);

  /* 'MAG_WorldMagneticModel:115' cp(2) = crlon; */
  cp_data[1] = cos(Lon);

  /* 'MAG_WorldMagneticModel:117' for m = 3:maxord */
  for (m = 0; m < 11; m++) {
    /* 'MAG_WorldMagneticModel:118' sp(m) = sp(2)*cp(m-1)+cp(2)*sp(m-1); */
    srlat = cp_data[m + 1];
    sp_data[m + 2] = (srlat * sp_data[1]) + (sp_data[m + 1] * cp_data[1]);

    /* 'MAG_WorldMagneticModel:119' cp(m) = cp(2)*cp(m-1)-sp(2)*sp(m-1); */
    cp_data[m + 2] = (srlat * cp_data[1]) - (sp_data[m + 1] * sp_data[1]);
  }

  /* 'MAG_WorldMagneticModel:123' snorm(1,1) = 1.0; */
  snorm_data[0] = 1.0;

  /* 'MAG_WorldMagneticModel:124' snorm(2,1) = ct; */
  snorm_data[1] = ct;

  /* 'MAG_WorldMagneticModel:125' dp(1,1) = 0.0; */
  dp_data[0] = 0.0;

  /* 'MAG_WorldMagneticModel:126' dp(1,2) = -st; */
  dp_data[13] = -st;

  /* 'MAG_WorldMagneticModel:128' aor = re/r; */
  q1 = 6371.2 / q;

  /* 'MAG_WorldMagneticModel:129' ar = aor*aor; */
  ar = q1 * q1;

  /* 'MAG_WorldMagneticModel:130' br = 0.0; */
  q = 0.0;

  /* 'MAG_WorldMagneticModel:130' bt = 0.0; */
  crlat2 = 0.0;

  /* 'MAG_WorldMagneticModel:130' bp = 0.0; */
  srlat = 0.0;

  /* 'MAG_WorldMagneticModel:130' bpp = 0.0; */
  bpp = 0.0;

  /* 'MAG_WorldMagneticModel:131' for n = 1:maxord-1 */
  for (m = 0; m < 12; m++) {
    /* 'MAG_WorldMagneticModel:132' ar = ar*aor; */
    ar *= q1;

    /* 'MAG_WorldMagneticModel:133' for m = 0:n */
    for (b_m = 0; b_m <= (1 + m); b_m++) {
      /* 'MAG_WorldMagneticModel:136' if (n == m) */
      if ((1 + m) == b_m) {
        /* 'MAG_WorldMagneticModel:137' snorm(n+1, m+1) = st*snorm(n, m); */
        snorm_data_tmp = ((b_m - 1) * 13) + m;
        snorm_data[(m + (13 * b_m)) + 1] = snorm_data[snorm_data_tmp] * st;

        /* 'MAG_WorldMagneticModel:138' dp(m+1, n+1) = st*dp(m, n)+ct*snorm(n, m); */
        dp_data[b_m + (13 * (1 + m))] = (dp_data[((13 * m) + b_m) - 1] * st) +
          (snorm_data[snorm_data_tmp] * ct);
      } else {
        if ((1 + m) > 1) {
          /* 'MAG_WorldMagneticModel:139' elseif (n > 1) */
          /* 'MAG_WorldMagneticModel:140' snorm(n+1, m+1) = ct*snorm(n, m+1)-k(m+1, n+1)*snorm(n-1, m+1); */
          snorm_data_tmp = ((1 + m) * 13) + b_m;
          snorm_data[(m + (13 * b_m)) + 1] = (snorm_data[(13 * b_m) + m] * ct) -
            (dwork->k[snorm_data_tmp] * snorm_data[((13 * b_m) + m) - 1]);

          /* 'MAG_WorldMagneticModel:141' dp(m+1, n+1) = ct*dp(m+1, n) - st*snorm(n, m+1)-k(m+1, n+1)*dp(m+1, n-1); */
          dp_data[snorm_data_tmp] = ((dp_data[(13 * m) + b_m] * ct) -
            (snorm_data[(13 * b_m) + m] * st)) - (dwork->k[((1 + m) * 13) + b_m]
            * dp_data[((m - 1) * 13) + b_m]);
        }
      }

      /* 'MAG_WorldMagneticModel:145' par = ar*snorm(n+1, m+1); */
      par = snorm_data[((13 * b_m) + m) + 1] * ar;

      /* 'MAG_WorldMagneticModel:146' if (m == 0) */
      if (b_m == 0) {
        /* 'MAG_WorldMagneticModel:147' temp1 = tc(m+1, n+1)*cp(m+1); */
        temp1 = tc[(1 + m) * 13] * cp_data[0];

        /* 'MAG_WorldMagneticModel:148' temp2 = tc(m+1, n+1)*sp(m+1); */
        temp2 = tc[(1 + m) * 13] * sp_data[0];
      } else {
        /* 'MAG_WorldMagneticModel:149' else */
        /* 'MAG_WorldMagneticModel:150' temp1 = tc(m+1, n+1)*cp(m+1)+tc(n+1, m)*sp(m+1); */
        temp2 = tc[(((b_m - 1) * 13) + m) + 1];
        temp1 = (temp2 * sp_data[b_m]) + (tc[((1 + m) * 13) + b_m] * cp_data[b_m]);

        /* 'MAG_WorldMagneticModel:151' temp2 = tc(m+1, n+1)*sp(m+1)-tc(n+1, m)*cp(m+1); */
        temp2 = (tc[((1 + m) * 13) + b_m] * sp_data[b_m]) - (temp2 * cp_data[b_m]);
      }

      /* 'MAG_WorldMagneticModel:154' bt = bt-ar*temp1*dp(m+1, n+1); */
      crlat2 -= dp_data[((1 + m) * 13) + b_m] * (ar * temp1);

      /* 'MAG_WorldMagneticModel:155' bp = bp+(fm(m+1)*temp2*par); */
      srlat += (dwork->fm[b_m] * temp2) * par;

      /* 'MAG_WorldMagneticModel:156' br = br+(fn(n+1)*temp1*par); */
      q += (dwork->fn[1 + m] * temp1) * par;

      /* 'MAG_WorldMagneticModel:159' if (st == 0.0 && m == 1) */
      if ((st == 0.0) && (b_m == 1)) {
        /* 'MAG_WorldMagneticModel:160' if (n == 1) */
        if ((1 + m) == 1) {
          /* 'MAG_WorldMagneticModel:161' pp(n+1) = pp(n); */
          pp_data[1] = pp_data[0];
        } else {
          /* 'MAG_WorldMagneticModel:162' else */
          /* 'MAG_WorldMagneticModel:163' pp(n+1) = ct*pp(n)-k(m+1,n+1)*pp(n-1); */
          pp_data[1 + m] = (ct * pp_data[m]) - (dwork->k[((1 + m) * 13) + 1] *
            pp_data[m - 1]);
        }

        /* 'MAG_WorldMagneticModel:165' parp = ar*pp(n+1); */
        /* 'MAG_WorldMagneticModel:166' bpp = bpp + (fm(m+1)*temp2*parp); */
        bpp += (pp_data[1 + m] * ar) * (dwork->fm[1] * temp2);
      }
    }
  }

  /* 'MAG_WorldMagneticModel:170' if (st == 0.0) */
  if (st == 0.0) {
    /* 'MAG_WorldMagneticModel:171' bp = bpp; */
    srlat = bpp;
  } else {
    /* 'MAG_WorldMagneticModel:172' else */
    /* 'MAG_WorldMagneticModel:173' bp = bp/st; */
    srlat /= st;
  }

  /* 'MAG_WorldMagneticModel:179' bx = -bt*ca-br*sa; */
  ct = ((-crlat2) * srlat2) - (q * crlat);

  /* 'MAG_WorldMagneticModel:180' by = bp; */
  /* 'MAG_WorldMagneticModel:181' bz = bt*sa-br*ca; */
  srlat2 = (crlat2 * crlat) - (q * srlat2);

  /* 'MAG_WorldMagneticModel:185' bh = sqrt((bx*bx)+(by*by)); */
  crlat = sqrt((ct * ct) + (srlat * srlat));

  /* 'MAG_WorldMagneticModel:186' F = sqrt((bh*bh)+(bz*bz)); */
  st = sqrt((crlat * crlat) + (srlat2 * srlat2));

  /* 'MAG_WorldMagneticModel:187' D = atan2(by,bx); */
  ct = rt_atan2d_snf(srlat, ct);

  /* 'MAG_WorldMagneticModel:188' I = atan2(bz,bh); */
  srlat2 = rt_atan2d_snf(srlat2, crlat);

  /* 'MAG_WorldMagneticModel:191' X = F*cos(D)*cos(I); */
  /* 'MAG_WorldMagneticModel:192' Y = F*cos(I)*sin(D); */
  /* 'MAG_WorldMagneticModel:193' Z = F*sin(I); */
  /* 'MAG_WorldMagneticModel:194' XYZ = [X; Y; Z]; */
  crlat = cos(srlat2);
  XYZ[0] = (st * cos(ct)) * crlat;
  XYZ[1] = (st * crlat) * sin(ct);
  XYZ[2] = st * sin(srlat2);

  /* 'MAG_WorldMagneticModel:196' H = F*cos(I); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function eg=LIB_d2e(dcm)
 */
static void LIB_d2e(const real_T dcm[9], real_T *eg_phi, real_T *eg_the, real_T *
                    eg_psi)
{
  /* 'LIB_d2e:28' phi = atan2(dcm(3,2),dcm(3,3)); */
  /* 'LIB_d2e:31' if abs(dcm(3,1))<1.0 */
  if (fabs(dcm[2]) < 1.0) {
    /* 'LIB_d2e:32' theta = -asin(dcm(3,1)); */
    *eg_the = -asin(dcm[2]);
  } else if (dcm[2] >= 1.0) {
    /* 'LIB_d2e:33' elseif dcm(3,1) >= 1.0 */
    /* 'LIB_d2e:34' theta = -pi/2; */
    *eg_the = -1.5707963267948966;
  } else if (dcm[2] <= -1.0) {
    /* 'LIB_d2e:35' elseif dcm(3,1) <= -1.0 */
    /* 'LIB_d2e:36' theta = pi/2; */
    *eg_the = 1.5707963267948966;
  } else {
    /* 'LIB_d2e:37' else */
    /* 'LIB_d2e:38' theta = 0.0; */
    *eg_the = 0.0;
  }

  /* 'LIB_d2e:42' psi = atan2(dcm(2,1),dcm(1,1)); */
  /* 'LIB_d2e:44' eg.phi = phi; */
  *eg_phi = rt_atan2d_snf(dcm[5], dcm[8]);

  /* 'LIB_d2e:45' eg.the = theta; */
  /* 'LIB_d2e:46' eg.psi = psi; */
  *eg_psi = rt_atan2d_snf(dcm[1], dcm[0]);
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [State, Output, Tel] = MAG_Run(State, Input)
 */
static void MAG_Run(T_MAG_State *State, real_T Input_TimeStamp, uint32_T
                    Input_Flags, const real_T Input_FieldMeas[3], const real_T
                    Input_FieldWMM[3], const real_T Input_Attitude[3], uint32_T *
                    Output_Valid, real_T Output_Attitude[3], real_T Output_P[9],
                    T_MAG_Tel *Tel)
{
  real_T MagMeas[3];
  real_T AccelBody[3];
  real_T B_0[9];
  real_T s3;
  real_T sum23;
  real_T sum12;
  real_T U[9];
  real_T V[9];
  boolean_T p;
  int32_T i;
  real_T U_0[9];
  real_T tmp;
  real_T tmp_0[3];
  real_T tmp_1[3];
  real_T U_1[9];
  int32_T i_0;
  real_T State_idx_2;
  uint32_T tmp_2;
  uint32_T tmp_3;
  int32_T U_tmp;

  /* 'MAG_Run:4' [~,~,Output,Tel] = MAG_CreateStructs; */
  *Output_Valid = 0U;
  Output_Attitude[0] = 0.0;
  Output_Attitude[1] = 0.0;
  Output_Attitude[2] = 0.0;
  memset(&Output_P[0], 0, 9U * (sizeof(real_T)));

  /* 'MAG_Run:4' ~ */
  /* 'MAG_Run:7' if FlagInput_FieldWMMValid(Input.Flags) */
  /* 'MAG_Run:68' Flag = bitget(Flags,2); */
  if ((Input_Flags & 2U) != 0U) {
    /* 'MAG_Run:8' State.FieldWMM = Input.FieldWMM                         ; */
    State->FieldWMM[0] = Input_FieldWMM[0];
    State->FieldWMM[1] = Input_FieldWMM[1];
    State->FieldWMM[2] = Input_FieldWMM[2];

    /* 'MAG_Run:9' State.Flags    = FlagStateSet_FieldWMMValid(State.Flags); */
    /* 'MAG_Run:98' Flags = bitset(Flags, 1); */
    State->Flags |= 1U;
  }

  /* 'MAG_Run:13' if FlagInput_DoCalibrate   (Input.Flags) && ... */
  /* 'MAG_Run:14'      FlagInput_RollPitchValid(Input.Flags) && ... */
  /* 'MAG_Run:15'      FlagInput_HeadingValid  (Input.Flags) && ... */
  /* 'MAG_Run:16'      FlagInput_FieldMeasValid(Input.Flags) && ... */
  /* 'MAG_Run:17'      FlagState_FieldWMMValid (State.Flags) */
  /* 'MAG_Run:83' Flag = bitget(Flags,5); */
  tmp_2 = Input_Flags & 1U;
  tmp_3 = Input_Flags & 4U;
  if ((((((Input_Flags & 16U) != 0U) && (tmp_3 != 0U)) && ((Input_Flags & 8U) !=
         0U)) && (tmp_2 != 0U)) && ((State->Flags & 1U) != 0U)) {
    /* 'MAG_Run:73' Flag = bitget(Flags,3); */
    /* 'MAG_Run:78' Flag = bitget(Flags,4); */
    /* 'MAG_Run:63' Flag = bitget(Flags,1); */
    /* 'MAG_Run:88' Flag = bitget(Flags, 1); */
    /* 'MAG_Run:19' eg.phi = Input.Attitude(1); */
    /* 'MAG_Run:20' eg.the = Input.Attitude(2); */
    /* 'MAG_Run:21' eg.psi = Input.Attitude(3); */
    /* 'MAG_Run:23' Cnb     = LIB_e2d(eg)'                        ; */
    /* 'MAG_Run:24' State.b = Input.FieldMeas - Cnb*State.FieldWMM; */
    LIB_e2d(Input_Attitude[0], Input_Attitude[1], Input_Attitude[2], U);
    for (i = 0; i < 3; i++) {
      State->b[i] = Input_FieldMeas[i] - (((U[(3 * i) + 1] * State->FieldWMM[1])
        + (U[3 * i] * State->FieldWMM[0])) + (U[(3 * i) + 2] * State->FieldWMM[2]));
    }

    /* 'MAG_Run:26' State.Flags = FlagStateSet_CalibrationValid(State.Flags); */
    /* 'MAG_Run:102' Flags = bitset(Flags, 2); */
    State->Flags |= 2U;
  }

  /* 'MAG_Run:30' if FlagInput_FieldMeasValid  (Input.Flags) && ... */
  /* 'MAG_Run:31'      FlagInput_RollPitchValid  (Input.Flags) && ...      */
  /* 'MAG_Run:32'      FlagState_CalibrationValid(State.Flags) */
  /* 'MAG_Run:63' Flag = bitget(Flags,1); */
  if (((tmp_2 != 0U) && (tmp_3 != 0U)) && ((State->Flags & 2U) != 0U)) {
    /* 'MAG_Run:73' Flag = bitget(Flags,3); */
    /* 'MAG_Run:93' Flag = bitget(Flags,2); */
    /* 'MAG_Run:34' phi = Input.Attitude(1); */
    /* 'MAG_Run:35' the = Input.Attitude(2); */
    /* 'MAG_Run:37' MagWMM  = State.FieldWMM           ; */
    /* 'MAG_Run:38' MagMeas = Input.FieldMeas - State.b; */
    /* 'MAG_Run:40' [Attitude,P] = MAG_EstimateAttitude(MagWMM, MagMeas, phi, the); */
    /* 'MAG_EstimateAttitude:38' eg.phi = EST_phi  ; */
    /* 'MAG_EstimateAttitude:39' eg.the = EST_theta; */
    /* 'MAG_EstimateAttitude:40' eg.psi = 0.0      ; */
    /* 'MAG_EstimateAttitude:42' Cnb       = LIB_e2d(eg)'   ; */
    /* 'MAG_EstimateAttitude:43' AccelBody = Cnb * [0,0,-1]'; */
    LIB_e2d(Input_Attitude[0], Input_Attitude[1], 0.0, U);
    for (i = 0; i < 3; i++) {
      MagMeas[i] = Input_FieldMeas[i] - State->b[i];
      AccelBody[i] = (-U[(3 * i) + 2]) + ((U[(3 * i) + 1] * 0.0) + (U[3 * i] *
        0.0));
    }

    /* 'MAG_EstimateAttitude:46' b1 = MagMeas  /norm(MagMeas  ); */
    /* 'MAG_EstimateAttitude:47' b2 = AccelBody/norm(AccelBody); */
    /* 'MAG_EstimateAttitude:49' n1 = MagWMM /norm(MagWMM); */
    /* 'MAG_EstimateAttitude:50' n2 = [0,0,-1]'           ; */
    /* 'MAG_EstimateAttitude:52' B       = 0.8*b1*n1' + 0.2*b2*n2'       ; */
    State_idx_2 = norm_1SDG5Mdv(MagMeas);
    tmp = norm_1SDG5Mdv(State->FieldWMM);
    s3 = norm_1SDG5Mdv(AccelBody);
    tmp_0[0] = (MagMeas[0] / State_idx_2) * 0.8;
    sum23 = State->FieldWMM[0] / tmp;
    tmp_1[0] = (AccelBody[0] / s3) * 0.2;
    tmp_0[1] = (MagMeas[1] / State_idx_2) * 0.8;
    sum12 = State->FieldWMM[1] / tmp;
    tmp_1[1] = (AccelBody[1] / s3) * 0.2;
    tmp_0[2] = (MagMeas[2] / State_idx_2) * 0.8;
    State_idx_2 = State->FieldWMM[2] / tmp;
    tmp_1[2] = (AccelBody[2] / s3) * 0.2;
    for (i = 0; i < 3; i++) {
      U[i] = tmp_0[i] * sum23;
      V[i] = tmp_1[i] * 0.0;
      U[i + 3] = tmp_0[i] * sum12;
      V[i + 3] = tmp_1[i] * 0.0;
      U[i + 6] = tmp_0[i] * State_idx_2;
      V[i + 6] = -tmp_1[i];
    }

    /* 'MAG_EstimateAttitude:53' [U,S,V] = svd(B)                        ; */
    p = true;
    for (i = 0; i < 9; i++) {
      s3 = U[i] + V[i];
      p = (p && ((!rtIsInf(s3)) && (!rtIsNaN(s3))));
      B_0[i] = s3;
    }

    if (p) {
      svd_8vZPMSfD(B_0, U, MagMeas, V);
    } else {
      MagMeas[0] = (rtNaN);
      MagMeas[1] = (rtNaN);
      MagMeas[2] = (rtNaN);
      for (i = 0; i < 9; i++) {
        U[i] = (rtNaN);
        V[i] = (rtNaN);
      }
    }

    /* 'MAG_EstimateAttitude:54' A       = U*diag([1,1,det(U)*det(V)])*V'; */
    memset(&B_0[0], 0, 9U * (sizeof(real_T)));
    B_0[0] = 1.0;
    B_0[4] = 1.0;
    s3 = det_U5whr0XH(U);
    sum23 = det_U5whr0XH(V);
    B_0[8] = s3 * sum23;

    /* 'MAG_EstimateAttitude:56' eg       = LIB_d2e(A')            ; */
    for (i = 0; i < 3; i++) {
      for (i_0 = 0; i_0 < 3; i_0++) {
        U_tmp = i_0 + (3 * i);
        U_1[U_tmp] = 0.0;
        U_1[U_tmp] += B_0[3 * i] * U[i_0];
        U_1[U_tmp] += B_0[(3 * i) + 1] * U[i_0 + 3];
        U_1[U_tmp] += B_0[(3 * i) + 2] * U[i_0 + 6];
      }
    }

    for (i = 0; i < 3; i++) {
      for (i_0 = 0; i_0 < 3; i_0++) {
        U_tmp = i + (3 * i_0);
        U_0[U_tmp] = 0.0;
        U_0[U_tmp] += U_1[i_0] * V[i];
        U_0[U_tmp] += U_1[i_0 + 3] * V[i + 3];
        U_0[U_tmp] += U_1[i_0 + 6] * V[i + 6];
      }
    }

    LIB_d2e(U_0, &Output_Attitude[0], &Output_Attitude[1], &Output_Attitude[2]);

    /* 'MAG_EstimateAttitude:57' Attitude = [eg.phi,eg.the,eg.psi]'; */
    /* 'MAG_EstimateAttitude:60' s1 = S(1,1)              ; */
    /* 'MAG_EstimateAttitude:61' s2 = S(2,2)              ; */
    /* 'MAG_EstimateAttitude:62' s3 = S(3,3)*det(U)*det(V); */
    s3 = (MagMeas[2] * s3) * sum23;

    /* 'MAG_EstimateAttitude:64' sum23 = s2+s3; */
    sum23 = MagMeas[1] + s3;

    /* 'MAG_EstimateAttitude:65' sum13 = s1+s3; */
    s3 += MagMeas[0];

    /* 'MAG_EstimateAttitude:66' sum12 = s1+s2; */
    sum12 = MagMeas[0] + MagMeas[1];

    /* 'MAG_EstimateAttitude:68' if abs(sum23) < eps */
    if (fabs(sum23) < 2.2204460492503131E-16) {
      /* 'MAG_EstimateAttitude:68' sum23 = eps; */
      sum23 = 2.2204460492503131E-16;
    }

    /* 'MAG_EstimateAttitude:69' if abs(sum13) < eps */
    if (fabs(s3) < 2.2204460492503131E-16) {
      /* 'MAG_EstimateAttitude:69' sum13 = eps; */
      s3 = 2.2204460492503131E-16;
    }

    /* 'MAG_EstimateAttitude:70' if abs(sum12) < eps */
    if (fabs(sum12) < 2.2204460492503131E-16) {
      /* 'MAG_EstimateAttitude:70' sum12 = eps; */
      sum12 = 2.2204460492503131E-16;
    }

    /* 'MAG_EstimateAttitude:72' d1 = (sum23)^-1; */
    /* 'MAG_EstimateAttitude:73' d2 = (sum13)^-1; */
    /* 'MAG_EstimateAttitude:74' d3 = (sum12)^-1; */
    /* 'MAG_EstimateAttitude:76' P = U*diag([d1,d2,d3])*U'; */
    memset(&B_0[0], 0, 9U * (sizeof(real_T)));
    B_0[0] = 1.0 / sum23;
    B_0[4] = 1.0 / s3;
    B_0[8] = 1.0 / sum12;

    /* 'MAG_Run:42' Output.Attitude = Attitude       ; */
    /* 'MAG_Run:43' Output.P        = P              ; */
    for (i = 0; i < 3; i++) {
      for (i_0 = 0; i_0 < 3; i_0++) {
        U_tmp = i + (3 * i_0);
        U_0[U_tmp] = 0.0;
        U_0[U_tmp] += B_0[3 * i_0] * U[i];
        U_0[U_tmp] += B_0[(3 * i_0) + 1] * U[i + 3];
        U_0[U_tmp] += B_0[(3 * i_0) + 2] * U[i + 6];
      }

      for (i_0 = 0; i_0 < 3; i_0++) {
        U_tmp = i + (3 * i_0);
        Output_P[U_tmp] = 0.0;
        Output_P[U_tmp] += U_0[i] * U[i_0];
        Output_P[U_tmp] += U_0[i + 3] * U[i_0 + 3];
        Output_P[U_tmp] += U_0[i + 6] * U[i_0 + 6];
      }
    }

    /* 'MAG_Run:44' Output.Valid    = uint32(1)      ; */
    *Output_Valid = 1U;
  }

  /* 'MAG_Run:48' Tel.MAG_Input_TimeStamp =        Input.TimeStamp ; */
  Tel->MAG_Input_TimeStamp = Input_TimeStamp;

  /* 'MAG_Run:49' Tel.MAG_Input_Flags     =        Input.Flags     ; */
  Tel->MAG_Input_Flags = Input_Flags;

  /* 'MAG_Run:50' Tel.MAG_Input_FieldMeas = single(Input.FieldMeas); */
  /* 'MAG_Run:51' Tel.MAG_Input_FieldWMM  = single(Input.FieldWMM ); */
  /* 'MAG_Run:52' Tel.MAG_Input_Attitude  = single(Input.Attitude ); */
  /* 'MAG_Run:53' Tel.MAG_State_Flags     =        State.Flags     ; */
  Tel->MAG_State_Flags = State->Flags;

  /* 'MAG_Run:54' Tel.MAG_State_FieldWMM  = single(State.FieldWMM ); */
  /* 'MAG_Run:55' Tel.MAG_State_b         = single(State.b        ); */
  /* 'MAG_Run:56' Tel.MAG_Output_Valid    = uint32(Output.Valid   ); */
  Tel->MAG_Output_Valid = *Output_Valid;

  /* 'MAG_Run:57' Tel.MAG_Output_Attitude = single(Output.Attitude); */
  Tel->MAG_Input_FieldMeas[0] = (real32_T)Input_FieldMeas[0];
  Tel->MAG_Input_FieldWMM[0] = (real32_T)Input_FieldWMM[0];
  Tel->MAG_Input_Attitude[0] = (real32_T)Input_Attitude[0];
  Tel->MAG_State_FieldWMM[0] = (real32_T)State->FieldWMM[0];
  Tel->MAG_State_b[0] = (real32_T)State->b[0];
  Tel->MAG_Output_Attitude[0] = (real32_T)Output_Attitude[0];
  Tel->MAG_Input_FieldMeas[1] = (real32_T)Input_FieldMeas[1];
  Tel->MAG_Input_FieldWMM[1] = (real32_T)Input_FieldWMM[1];
  Tel->MAG_Input_Attitude[1] = (real32_T)Input_Attitude[1];
  Tel->MAG_State_FieldWMM[1] = (real32_T)State->FieldWMM[1];
  Tel->MAG_State_b[1] = (real32_T)State->b[1];
  Tel->MAG_Output_Attitude[1] = (real32_T)Output_Attitude[1];
  Tel->MAG_Input_FieldMeas[2] = (real32_T)Input_FieldMeas[2];
  Tel->MAG_Input_FieldWMM[2] = (real32_T)Input_FieldWMM[2];
  Tel->MAG_Input_Attitude[2] = (real32_T)Input_Attitude[2];
  Tel->MAG_State_FieldWMM[2] = (real32_T)State->FieldWMM[2];
  Tel->MAG_State_b[2] = (real32_T)State->b[2];
  Tel->MAG_Output_Attitude[2] = (real32_T)Output_Attitude[2];

  /* 'MAG_Run:58' Tel.MAG_Output_P        = single(Output.P       ); */
  for (i = 0; i < 9; i++) {
    Tel->MAG_Output_P[i] = (real32_T)Output_P[i];
  }
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [magOutput, magTelemetry] = MAG_DoMagnetometerMain2Hz(magMeasurement, navState, NavigationMode, kfState)
 */
static void MAG_DoMagnetometerMain2Hz(uint8_T magMeasurement_Valid, real_T
  magMeasurement_SystemTime, real_T magMeasurement_Field_x, real_T
  magMeasurement_Field_y, real_T magMeasurement_Field_z, const
  T_NAV_NavigationState *navState, uint16_T NavigationMode, const
  T_NAV_KalmanFilterState *kfState, uint32_T *magOutput_Valid, real_T
  magOutput_Attitude[3], real_T magOutput_P[9], T_MAG_Tel *magTelemetry, DW
  *dwork)
{
  uint8_T NavValid;
  real_T FieldWMM[3];
  real_T FieldMeas[3];
  real_T Attitude[3];
  int32_T Flags;

  /* 'MAG_DoMagnetometerMain2Hz:6' if isempty(MAG_State) */
  /* 'MAG_DoMagnetometerMain2Hz:11' [~,magInput,~] = MAG_CreateStructs; */
  /* 'MAG_DoMagnetometerMain2Hz:11' ~ */
  /* 'MAG_DoMagnetometerMain2Hz:13' if NavigationMode >= 30 */
  if (((int32_T)NavigationMode) >= 30) {
    /* 'MAG_DoMagnetometerMain2Hz:14' NavValid = uint8(1); */
    NavValid = 1U;

    /* 'MAG_DoMagnetometerMain2Hz:15' FieldWMM = MAG_WorldMagneticModel(navState.pos_g.alt, ... % Height */
    /* 'MAG_DoMagnetometerMain2Hz:16'                                       navState.pos_g.lat, ... % Lat */
    /* 'MAG_DoMagnetometerMain2Hz:17'                                       navState.pos_g.lon); */
    MAG_WorldMagneticModel(navState->pos_g.alt, navState->pos_g.lat,
      navState->pos_g.lon, FieldWMM, dwork);
  } else {
    /* 'MAG_DoMagnetometerMain2Hz:18' else */
    /* 'MAG_DoMagnetometerMain2Hz:19' NavValid = uint8(0); */
    NavValid = 0U;

    /* 'MAG_DoMagnetometerMain2Hz:20' FieldWMM = zeros(3,1); */
    FieldWMM[0] = 0.0;
    FieldWMM[1] = 0.0;
    FieldWMM[2] = 0.0;
  }

  /* 'MAG_DoMagnetometerMain2Hz:23' TimeStamp = magMeasurement.SystemTime; */
  /* 'MAG_DoMagnetometerMain2Hz:25' FieldMeas    = zeros(3,1)            ; */
  /* 'MAG_DoMagnetometerMain2Hz:26' FieldMeas(1) = magMeasurement.Field.x; */
  FieldMeas[0] = magMeasurement_Field_x;

  /* 'MAG_DoMagnetometerMain2Hz:27' FieldMeas(2) = magMeasurement.Field.y; */
  FieldMeas[1] = magMeasurement_Field_y;

  /* 'MAG_DoMagnetometerMain2Hz:28' FieldMeas(3) = magMeasurement.Field.z; */
  FieldMeas[2] = magMeasurement_Field_z;

  /* 'MAG_DoMagnetometerMain2Hz:30' Attitude    = zeros(3,1)         ; */
  /* 'MAG_DoMagnetometerMain2Hz:31' Attitude(1) = navState.att_bn.phi; */
  Attitude[0] = navState->att_bn.phi;

  /* 'MAG_DoMagnetometerMain2Hz:32' Attitude(2) = navState.att_bn.the; */
  Attitude[1] = navState->att_bn.the;

  /* 'MAG_DoMagnetometerMain2Hz:33' Attitude(3) = navState.att_bn.psi; */
  Attitude[2] = navState->att_bn.psi;

  /* 'MAG_DoMagnetometerMain2Hz:35' Flags = uint32(0); */
  /* 'MAG_DoMagnetometerMain2Hz:36' Flags = bitset(Flags, 1, magMeasurement.Valid); */
  /* 'MAG_DoMagnetometerMain2Hz:37' Flags = bitset(Flags, 2, NavValid            ); */
  Flags = (((int32_T)magMeasurement_Valid) != 0);

  /* 'MAG_DoMagnetometerMain2Hz:38' Flags = bitset(Flags, 3, NavValid            ); */
  if (((int32_T)NavValid) != 0) {
    Flags = (int32_T)((uint32_T)(((uint32_T)Flags) | 2U));
    Flags = (int32_T)((uint32_T)(((uint32_T)Flags) | 4U));
  }

  /* 'MAG_DoMagnetometerMain2Hz:40' if (((kfState.P(7,7) + kfState.P(8,8) + kfState.P(9,9))^0.5) <= 0.02) && DoMagnetometerBiasCalibration && magMeasurement.Valid */
  if ((sqrt((kfState->P[102] + kfState->P[119]) + kfState->P[136]) <= 0.02) &&
      ((((int32_T)dwork->DoMagnetometerBiasCalibration) != 0) && (((int32_T)
         magMeasurement_Valid) != 0))) {
    /* 'MAG_DoMagnetometerMain2Hz:41' Flags = bitset(Flags, 4); */
    Flags = (int32_T)((uint32_T)(((uint32_T)Flags) | 8U));

    /* 'MAG_DoMagnetometerMain2Hz:42' Flags = bitset(Flags, 5); */
    Flags = (int32_T)((uint32_T)(((uint32_T)Flags) | 16U));

    /* 'MAG_DoMagnetometerMain2Hz:43' DoMagnetometerBiasCalibration = uint8(0); */
    dwork->DoMagnetometerBiasCalibration = 0U;
  }

  /* 'MAG_DoMagnetometerMain2Hz:46' magInput.TimeStamp = TimeStamp; */
  /* 'MAG_DoMagnetometerMain2Hz:47' magInput.Flags     = Flags    ; */
  /* 'MAG_DoMagnetometerMain2Hz:48' magInput.FieldMeas = FieldMeas; */
  /* 'MAG_DoMagnetometerMain2Hz:49' magInput.FieldWMM  = FieldWMM ; */
  /* 'MAG_DoMagnetometerMain2Hz:50' magInput.Attitude  = Attitude ; */
  /* 'MAG_DoMagnetometerMain2Hz:52' [MAG_State, magOutput, magTelemetry] = MAG_Run(MAG_State, magInput); */
  MAG_Run(&dwork->MAG_State, magMeasurement_SystemTime, (uint32_T)Flags,
          FieldMeas, FieldWMM, Attitude, magOutput_Valid, magOutput_Attitude,
          magOutput_P, magTelemetry);
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function difference = init_KalmanFilterErrorEstimates
 */
static void init_KalmanFilterErrorEstimates(real_T difference_nav[16], real_T
  difference_kalmanfilter_state[16], real_T difference_kalmanfilter_covariance
  [256], uint16_T *difference_numbersatellites, uint16_T *difference_Reserved,
  uint8_T *difference_valid_gnss, uint8_T *difference_valid_baro, uint8_T
  *difference_valid_ta, uint32_T *difference_valid_mag, uint8_T
  *difference_valid_feedback_nav, uint8_T *difference_valid_feedback_att,
  uint8_T *difference_valid_reset_navigation, uint32_T *difference_trade)
{
  /* 'init_KalmanFilterErrorEstimates:26' difference.nav = zeros(16,1); */
  /* 'init_KalmanFilterErrorEstimates:27' difference.kalmanfilter = CreateKalmanFilterStateStruct; */
  /* 'init_KalmanFilterErrorEstimates:42' v.state = zeros(16,1); */
  memset(&difference_nav[0], 0, (sizeof(real_T)) << 4U);
  memset(&difference_kalmanfilter_state[0], 0, (sizeof(real_T)) << 4U);

  /* 'init_KalmanFilterErrorEstimates:43' v.covariance = zeros(16,16); */
  memset(&difference_kalmanfilter_covariance[0], 0, (sizeof(real_T)) << 8U);

  /* 'init_KalmanFilterErrorEstimates:46' coder.cstructname(v, 'T_NAV_KalmanFilterStateError'); */
  /* 'init_KalmanFilterErrorEstimates:28' difference.numbersatellites = uint16(0.0); */
  *difference_numbersatellites = 0U;

  /* 'init_KalmanFilterErrorEstimates:31' difference.Reserved = uint16(0.0); */
  *difference_Reserved = 0U;

  /* 'init_KalmanFilterErrorEstimates:33' difference.valid = CreateKalmanFilterFeedbackValidityStruct; */
  /* 'init_KalmanFilterErrorEstimates:50' v.gnss = uint8(0); */
  *difference_valid_gnss = 0U;

  /* 'init_KalmanFilterErrorEstimates:51' v.baro = uint8(0); */
  *difference_valid_baro = 0U;

  /* 'init_KalmanFilterErrorEstimates:52' v.ta   = uint8(0); */
  *difference_valid_ta = 0U;

  /* 'init_KalmanFilterErrorEstimates:53' v.mag  = uint32(0); */
  *difference_valid_mag = 0U;

  /* 'init_KalmanFilterErrorEstimates:54' v.feedback_nav = uint8(0); */
  *difference_valid_feedback_nav = 0U;

  /* 'init_KalmanFilterErrorEstimates:55' v.feedback_att = uint8(0); */
  *difference_valid_feedback_att = 0U;

  /* 'init_KalmanFilterErrorEstimates:56' v.reset_navigation = uint8(0); */
  *difference_valid_reset_navigation = 0U;

  /* 'init_KalmanFilterErrorEstimates:59' coder.cstructname(v, 'T_NAV_KFilterFeedbackValidity'); */
  /* 'init_KalmanFilterErrorEstimates:34' difference.trade = uint32(0.0); */
  *difference_trade = 0U;

  /* 'init_KalmanFilterErrorEstimates:37' coder.cstructname(difference, 'T_NAV_KFilterErrorEstimateState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function gnss = init_GNSSreceiver
 */
static void init_GNSSreceiver(real_T gnss_range[28], real_T gnss_rrate[28],
  real_T *gnss_ecef_pos_x, real_T *gnss_ecef_pos_y, real_T *gnss_ecef_pos_z,
  real_T *gnss_ecef_vel_x, real_T *gnss_ecef_vel_y, real_T *gnss_ecef_vel_z,
  real_T gnss_pos[84], real_T gnss_vel[84], uint16_T *gnss_numberSatellites,
  uint8_T *gnss_valid, uint8_T *gnss_Reserved, real_T *gnss_time)
{
  /* 'init_GNSSreceiver:34' gnss.range = zeros(1,28); */
  /* 'init_GNSSreceiver:35' gnss.rrate = zeros(1,28); */
  memset(&gnss_range[0], 0, 28U * (sizeof(real_T)));
  memset(&gnss_rrate[0], 0, 28U * (sizeof(real_T)));

  /* 'init_GNSSreceiver:36' gnss.ecef_pos = CreateVecStruct3; */
  /* 'init_GNSSreceiver:57' v.x = 0.0; */
  *gnss_ecef_pos_x = 0.0;

  /* 'init_GNSSreceiver:58' v.y = 0.0; */
  *gnss_ecef_pos_y = 0.0;

  /* 'init_GNSSreceiver:59' v.z = 0.0; */
  *gnss_ecef_pos_z = 0.0;

  /* 'init_GNSSreceiver:62' coder.cstructname(v, 'T_Cartesian'); */
  /* 'init_GNSSreceiver:37' gnss.ecef_vel = CreateVecStruct3; */
  /* 'init_GNSSreceiver:57' v.x = 0.0; */
  *gnss_ecef_vel_x = 0.0;

  /* 'init_GNSSreceiver:58' v.y = 0.0; */
  *gnss_ecef_vel_y = 0.0;

  /* 'init_GNSSreceiver:59' v.z = 0.0; */
  *gnss_ecef_vel_z = 0.0;

  /* 'init_GNSSreceiver:62' coder.cstructname(v, 'T_Cartesian'); */
  /* 'init_GNSSreceiver:38' gnss.pos = zeros(3,28); */
  /* 'init_GNSSreceiver:39' gnss.vel = zeros(3,28); */
  memset(&gnss_pos[0], 0, 84U * (sizeof(real_T)));
  memset(&gnss_vel[0], 0, 84U * (sizeof(real_T)));

  /* 'init_GNSSreceiver:42' gnss.numberSatellites = uint16(0.0); */
  *gnss_numberSatellites = 0U;

  /* 'init_GNSSreceiver:43' gnss.valid = uint8(0); */
  *gnss_valid = 0U;

  /* 'init_GNSSreceiver:46' gnss.Reserved = uint8(0); */
  *gnss_Reserved = 0U;

  /* 'init_GNSSreceiver:49' gnss.time = 0.0; */
  *gnss_time = 0.0;

  /* 'init_GNSSreceiver:52' coder.cstructname(gnss, 'T_NAV_GNSSReceiverState'); */
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [data, info] = GetCorrespondingNavigationData(store, time)
 */
static void GetCorrespondingNavigationData(const T_NAV_Store *store, real_T time,
  real_T data[16], uint32_T *info_TimeReq, uint8_T *info_Case, uint8_T
  *info_IdxLow, uint8_T *info_IdxHi, uint8_T *info_Reserved1, uint32_T
  *info_TimeLow, uint32_T *info_TimeHi, int32_T info_Pos[3], uint32_T
  *info_Reserved2)
{
  real_T b_index;
  real_T j;
  real_T k;
  real_T time_0;
  int32_T i;
  int32_T store_0;
  int32_T time_tmp;
  int32_T data_tmp;
  real_T time_tmp_0;
  *info_Reserved1 = 0U;
  info_Pos[0] = 0;
  info_Pos[1] = 0;
  info_Pos[2] = 0;
  *info_Reserved2 = 0U;

  /* 'KalmanFilterMeasurement:1608' info = NAV_CreateStruct_T_NAV_StoreInfo; */
  /* 'KalmanFilterMeasurement:1609' if ((time >= store.store(store.storesize,1)) && (time <= store.store(1,1))) */
  store_0 = (int32_T)store->storesize;
  j = store->store[store_0 - 1];
  if ((time >= j) && (time <= store->store[0])) {
    /* 'KalmanFilterMeasurement:1610' data = (((time-store.store(store.storesize,1))/ ... */
    /* 'KalmanFilterMeasurement:1611'              (store.store(1,1)-store.store(store.storesize,1))).* ... */
    /* 'KalmanFilterMeasurement:1612'              (store.store(1,2:end)-store.store(store.storesize,2:end))) + ... */
    /* 'KalmanFilterMeasurement:1613'               store.store(store.storesize,2:end); */
    time_0 = (time - j) / (store->store[0] - j);
    for (i = 0; i < 16; i++) {
      data_tmp = (1 + i) * 120;
      data[i] = ((store->store[data_tmp] - store->store[(data_tmp + store_0) - 1])
                 * time_0) + store->store[(data_tmp + store_0) - 1];
    }

    /* 'KalmanFilterMeasurement:1615' info.TimeReq   = uint32(time*1e3); */
    b_index = rt_roundd_snf(time * 1000.0);
    if (b_index < 4.294967296E+9) {
      if (b_index >= 0.0) {
        *info_TimeReq = (uint32_T)b_index;
      } else {
        *info_TimeReq = 0U;
      }
    } else {
      *info_TimeReq = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1616' info.Case      = uint8(1); */
    *info_Case = 1U;

    /* 'KalmanFilterMeasurement:1617' info.IdxLow    = uint8(store.storesize); */
    b_index = rt_roundd_snf(store->storesize);
    if (b_index < 256.0) {
      if (b_index >= 0.0) {
        *info_IdxLow = (uint8_T)b_index;
      } else {
        *info_IdxLow = 0U;
      }
    } else {
      *info_IdxLow = MAX_uint8_T;
    }

    /* 'KalmanFilterMeasurement:1618' info.IdxHi     = uint8(1); */
    *info_IdxHi = 1U;

    /* 'KalmanFilterMeasurement:1619' info.TimeLow   = uint32(store.store(store.storesize,1)*1e3); */
    j = rt_roundd_snf(j * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeLow = (uint32_T)j;
      } else {
        *info_TimeLow = 0U;
      }
    } else {
      *info_TimeLow = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1620' info.TimeHi    = uint32(store.store(1,1)*1e3); */
    j = rt_roundd_snf(store->store[0] * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeHi = (uint32_T)j;
      } else {
        *info_TimeHi = 0U;
      }
    } else {
      *info_TimeHi = MAX_uint32_T;
    }
  } else if (time >= store->store[0]) {
    /* 'KalmanFilterMeasurement:1621' elseif (time >= store.store(1,1)) */
    /* 'KalmanFilterMeasurement:1622' index = 1; */
    b_index = 1.0;

    /* 'KalmanFilterMeasurement:1623' j = store.storeplace; */
    j = store->storeplace;

    /* 'KalmanFilterMeasurement:1624' while ((index+1) < j) */
    while ((b_index + 1.0) < j) {
      /* 'KalmanFilterMeasurement:1625' k = floor((index+j)/2); */
      k = floor((b_index + j) / 2.0);

      /* 'KalmanFilterMeasurement:1626' if (time < store.store(k,1)) */
      if (time < store->store[((int32_T)k) - 1]) {
        /* 'KalmanFilterMeasurement:1627' j = k; */
        j = k;
      } else {
        /* 'KalmanFilterMeasurement:1628' else */
        /* 'KalmanFilterMeasurement:1629' index = k; */
        b_index = k;
      }
    }

    /* 'KalmanFilterMeasurement:1632' data = (((time-store.store(index,1))/ ... */
    /* 'KalmanFilterMeasurement:1633'              (store.store(index+1,1)-store.store(index,1))).* ... */
    /* 'KalmanFilterMeasurement:1634'              (store.store(index+1,2:end)-store.store(index,2:end))) + ... */
    /* 'KalmanFilterMeasurement:1635'               store.store(index,2:end); */
    store_0 = (int32_T)b_index;
    k = store->store[store_0 - 1];
    time_tmp = (int32_T)((real_T)(b_index + 1.0));
    time_tmp_0 = store->store[time_tmp - 1];
    time_0 = (time - k) / (time_tmp_0 - k);
    for (i = 0; i < 16; i++) {
      data_tmp = (1 + i) * 120;
      data[i] = ((store->store[(data_tmp + time_tmp) - 1] - store->store
                  [(data_tmp + store_0) - 1]) * time_0) + store->store[(data_tmp
        + store_0) - 1];
    }

    /* 'KalmanFilterMeasurement:1637' info.TimeReq   = uint32(time*1e3); */
    j = rt_roundd_snf(time * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeReq = (uint32_T)j;
      } else {
        *info_TimeReq = 0U;
      }
    } else {
      *info_TimeReq = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1638' info.Case      = uint8(2); */
    *info_Case = 2U;

    /* 'KalmanFilterMeasurement:1639' info.IdxLow    = uint8(index); */
    if (b_index < 256.0) {
      *info_IdxLow = (uint8_T)b_index;
    } else {
      *info_IdxLow = MAX_uint8_T;
    }

    /* 'KalmanFilterMeasurement:1640' info.IdxHi     = uint8(index+1); */
    if ((b_index + 1.0) < 256.0) {
      *info_IdxHi = (uint8_T)((real_T)(b_index + 1.0));
    } else {
      *info_IdxHi = MAX_uint8_T;
    }

    /* 'KalmanFilterMeasurement:1641' info.TimeLow   = uint32(store.store(index,1)*1e3); */
    j = rt_roundd_snf(k * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeLow = (uint32_T)j;
      } else {
        *info_TimeLow = 0U;
      }
    } else {
      *info_TimeLow = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1642' info.TimeHi    = uint32(store.store(index+1,1)*1e3); */
    j = rt_roundd_snf(time_tmp_0 * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeHi = (uint32_T)j;
      } else {
        *info_TimeHi = 0U;
      }
    } else {
      *info_TimeHi = MAX_uint32_T;
    }
  } else {
    /* 'KalmanFilterMeasurement:1643' else */
    /* 'KalmanFilterMeasurement:1644' index = store.storeplace + 1; */
    b_index = store->storeplace + 1.0;

    /* 'KalmanFilterMeasurement:1645' j = store.storesize; */
    j = store->storesize;

    /* 'KalmanFilterMeasurement:1646' while ((index+1) < j) */
    while ((b_index + 1.0) < j) {
      /* 'KalmanFilterMeasurement:1647' k = floor((index+j)/2); */
      k = floor((b_index + j) / 2.0);

      /* 'KalmanFilterMeasurement:1648' if (time < store.store(k,1)) */
      if (time < store->store[((int32_T)k) - 1]) {
        /* 'KalmanFilterMeasurement:1649' j = k; */
        j = k;
      } else {
        /* 'KalmanFilterMeasurement:1650' else */
        /* 'KalmanFilterMeasurement:1651' index = k; */
        b_index = k;
      }
    }

    /* 'KalmanFilterMeasurement:1654' data = (((time-store.store(index,1))/ ... */
    /* 'KalmanFilterMeasurement:1655'              (store.store(index+1,1)-store.store(index,1))).* ... */
    /* 'KalmanFilterMeasurement:1656'              (store.store(index+1,2:end)-store.store(index,2:end))) + ... */
    /* 'KalmanFilterMeasurement:1657'               store.store(index,2:end); */
    store_0 = (int32_T)b_index;
    k = store->store[store_0 - 1];
    time_tmp = (int32_T)((real_T)(b_index + 1.0));
    time_tmp_0 = store->store[time_tmp - 1];
    time_0 = (time - k) / (time_tmp_0 - k);
    for (i = 0; i < 16; i++) {
      data_tmp = (1 + i) * 120;
      data[i] = ((store->store[(data_tmp + time_tmp) - 1] - store->store
                  [(data_tmp + store_0) - 1]) * time_0) + store->store[(data_tmp
        + store_0) - 1];
    }

    /* 'KalmanFilterMeasurement:1659' info.TimeReq   = uint32(time*1e3); */
    j = rt_roundd_snf(time * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeReq = (uint32_T)j;
      } else {
        *info_TimeReq = 0U;
      }
    } else {
      *info_TimeReq = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1660' info.Case      = uint8(3); */
    *info_Case = 3U;

    /* 'KalmanFilterMeasurement:1661' info.IdxLow    = uint8(index); */
    j = rt_roundd_snf(b_index);
    if (j < 256.0) {
      if (j >= 0.0) {
        *info_IdxLow = (uint8_T)j;
      } else {
        *info_IdxLow = 0U;
      }
    } else {
      *info_IdxLow = MAX_uint8_T;
    }

    /* 'KalmanFilterMeasurement:1662' info.IdxHi     = uint8(index+1); */
    j = rt_roundd_snf(b_index + 1.0);
    if (j < 256.0) {
      if (j >= 0.0) {
        *info_IdxHi = (uint8_T)j;
      } else {
        *info_IdxHi = 0U;
      }
    } else {
      *info_IdxHi = MAX_uint8_T;
    }

    /* 'KalmanFilterMeasurement:1663' info.TimeLow   = uint32(store.store(index,1)*1e3); */
    j = rt_roundd_snf(k * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeLow = (uint32_T)j;
      } else {
        *info_TimeLow = 0U;
      }
    } else {
      *info_TimeLow = MAX_uint32_T;
    }

    /* 'KalmanFilterMeasurement:1664' info.TimeHi    = uint32(store.store(index+1,1)*1e3); */
    j = rt_roundd_snf(time_tmp_0 * 1000.0);
    if (j < 4.294967296E+9) {
      if (j >= 0.0) {
        *info_TimeHi = (uint32_T)j;
      } else {
        *info_TimeHi = 0U;
      }
    } else {
      *info_TimeHi = MAX_uint32_T;
    }
  }
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [attitude_error] = GetECEFAttitudeErrorForGNSS(nav, nav_state, gnss)
 */
static void GetECEFAttitudeErrorForGNSS(const real_T nav[16], const
  T_NAV_NavigationState *nav_state, real_T gnss_ecef_pos_x, real_T
  gnss_ecef_pos_y, real_T gnss_ecef_pos_z, real_T gnss_ecef_vel_x, real_T
  gnss_ecef_vel_y, real_T gnss_ecef_vel_z, real_T *attitude_error_x, real_T
  *attitude_error_y, real_T *attitude_error_z)
{
  real_T dcm_en[9];
  real_T gnss_geo_vel[3];
  real_T dcm_nav_attitude_e[9];
  real_T lla_lat;
  real_T lla_lon;
  real_T expl_temp;
  real_T tmp[9];
  real_T dcm_en_0[9];
  real_T gnss_ecef_vel_x_0[3];
  int32_T i;
  int32_T i_0;
  real_T dcm_en_tmp;
  real_T dcm_en_tmp_0;
  int32_T dcm_en_tmp_1;
  boolean_T guard1 = false;

  /* 'KalmanFilterMeasurement:1708' gnss_geo_vel_temp = norm([gnss.ecef_vel.x, gnss.ecef_vel.y, gnss.ecef_vel.z]); */
  /* 'KalmanFilterMeasurement:1709' nav_body_rate_temp = norm([nav_state.wib_b.x, nav_state.wib_b.y, nav_state.wib_b.z]); */
  /* 'KalmanFilterMeasurement:1710' if ((gnss_geo_vel_temp > 10.0) && (nav_body_rate_temp < 0.06) && (~(bitget(nav_state.GuidCtrlFlags.NavCtrlFlag,3)))) */
  gnss_ecef_vel_x_0[0] = gnss_ecef_vel_x;
  gnss_ecef_vel_x_0[1] = gnss_ecef_vel_y;
  gnss_ecef_vel_x_0[2] = gnss_ecef_vel_z;
  guard1 = false;
  if (norm_1SDG5Mdv(gnss_ecef_vel_x_0) > 10.0) {
    gnss_ecef_vel_x_0[0] = nav_state->wib_b.x;
    gnss_ecef_vel_x_0[1] = nav_state->wib_b.y;
    gnss_ecef_vel_x_0[2] = nav_state->wib_b.z;
    if ((norm_1SDG5Mdv(gnss_ecef_vel_x_0) < 0.06) && ((((uint32_T)
           nav_state->GuidCtrlFlags.NavCtrlFlag) & 4U) == 0U)) {
      /* 'KalmanFilterMeasurement:1711' feedback_weight = 0.01; */
      /* 'KalmanFilterMeasurement:1712' dcm_en = LIB_dcm_en(LIB_ecef2lla(gnss.ecef_pos)); */
      LIB_ecef2lla(gnss_ecef_pos_x, gnss_ecef_pos_y, gnss_ecef_pos_z, &lla_lat,
                   &lla_lon, &expl_temp);

      /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
      /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
      /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
      expl_temp = -sin(lla_lat);
      dcm_en_tmp = cos(lla_lon);
      dcm_en[0] = expl_temp * dcm_en_tmp;
      lla_lon = sin(lla_lon);
      dcm_en[3] = expl_temp * lla_lon;
      dcm_en_tmp_0 = cos(lla_lat);
      dcm_en[6] = dcm_en_tmp_0;
      dcm_en[1] = -lla_lon;
      dcm_en[4] = dcm_en_tmp;
      dcm_en[7] = 0.0;
      dcm_en[2] = (-dcm_en_tmp_0) * dcm_en_tmp;
      dcm_en[5] = (-cos(lla_lat)) * lla_lon;
      dcm_en[8] = expl_temp;

      /* 'KalmanFilterMeasurement:1713' dcm_ne = dcm_en'; */
      /* 'KalmanFilterMeasurement:1714' gnss_geo_vel = dcm_en*[gnss.ecef_vel.x; gnss.ecef_vel.y; gnss.ecef_vel.z]; */
      gnss_ecef_vel_x_0[0] = gnss_ecef_vel_x;
      gnss_ecef_vel_x_0[1] = gnss_ecef_vel_y;
      gnss_ecef_vel_x_0[2] = gnss_ecef_vel_z;

      /* 'KalmanFilterMeasurement:1715' gnss_psi = atan2(gnss_geo_vel(2,1), gnss_geo_vel(1,1)); */
      /* 'KalmanFilterMeasurement:1716' dcm_nav_attitude_e = LIB_q2d(struct('q0', nav(1,10), 'q1', nav(1,11), 'q2', nav(1,12), 'q3', nav(1,13))); */
      LIB_q2d(nav[9], nav[10], nav[11], nav[12], dcm_nav_attitude_e);

      /* 'KalmanFilterMeasurement:1717' attitude_nav_attitude_g = LIB_d2e(dcm_en*dcm_nav_attitude_e); */
      for (i = 0; i < 3; i++) {
        gnss_geo_vel[i] = 0.0;
        for (i_0 = 0; i_0 < 3; i_0++) {
          dcm_en_tmp_1 = i + (3 * i_0);
          dcm_en_0[dcm_en_tmp_1] = 0.0;
          dcm_en_0[dcm_en_tmp_1] += dcm_nav_attitude_e[3 * i_0] * dcm_en[i];
          dcm_en_0[dcm_en_tmp_1] += dcm_nav_attitude_e[(3 * i_0) + 1] * dcm_en[i
            + 3];
          dcm_en_0[dcm_en_tmp_1] += dcm_nav_attitude_e[(3 * i_0) + 2] * dcm_en[i
            + 6];
          gnss_geo_vel[i] += dcm_en[dcm_en_tmp_1] * gnss_ecef_vel_x_0[i_0];
        }
      }

      LIB_d2e(dcm_en_0, &lla_lat, &lla_lon, &expl_temp);

      /* 'KalmanFilterMeasurement:1718' attitude_gnss_attitude_g  = struct('phi', attitude_nav_attitude_g.phi, 'the', attitude_nav_attitude_g.the, 'psi', gnss_psi); */
      /* 'KalmanFilterMeasurement:1719' dcm_gnss_attitude_e = dcm_ne*LIB_e2d(attitude_gnss_attitude_g); */
      /* 'KalmanFilterMeasurement:1720' attitude_error_e = dcm_nav_attitude_e*dcm_gnss_attitude_e'; */
      LIB_e2d(lla_lat, lla_lon, rt_atan2d_snf(gnss_geo_vel[1], gnss_geo_vel[0]),
              tmp);
      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          dcm_en_tmp_1 = i + (3 * i_0);
          dcm_en_0[dcm_en_tmp_1] = 0.0;
          dcm_en_0[dcm_en_tmp_1] += dcm_en[3 * i_0] * tmp[3 * i];
          dcm_en_0[dcm_en_tmp_1] += dcm_en[(3 * i_0) + 1] * tmp[(3 * i) + 1];
          dcm_en_0[dcm_en_tmp_1] += dcm_en[(3 * i_0) + 2] * tmp[(3 * i) + 2];
        }
      }

      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          dcm_en_tmp_1 = i_0 + (3 * i);
          dcm_en[dcm_en_tmp_1] = 0.0;
          dcm_en[dcm_en_tmp_1] += dcm_en_0[3 * i] * dcm_nav_attitude_e[i_0];
          dcm_en[dcm_en_tmp_1] += dcm_en_0[(3 * i) + 1] * dcm_nav_attitude_e[i_0
            + 3];
          dcm_en[dcm_en_tmp_1] += dcm_en_0[(3 * i) + 2] * dcm_nav_attitude_e[i_0
            + 6];
        }
      }

      /* 'KalmanFilterMeasurement:1721' attitude_error.x = -(attitude_error_e(3,2) - attitude_error_e(2,3))/2.0; */
      /* 'KalmanFilterMeasurement:1722' attitude_error.y = -(attitude_error_e(1,3) - attitude_error_e(3,1))/2.0; */
      /* 'KalmanFilterMeasurement:1723' attitude_error.z = -(attitude_error_e(2,1) - attitude_error_e(1,2))/2.0; */
      /* 'KalmanFilterMeasurement:1725' attitude_error.x = attitude_error.x * feedback_weight; */
      *attitude_error_x = ((-(dcm_en[5] - dcm_en[7])) / 2.0) * 0.01;

      /* 'KalmanFilterMeasurement:1726' attitude_error.y = attitude_error.y * feedback_weight; */
      *attitude_error_y = ((-(dcm_en[6] - dcm_en[2])) / 2.0) * 0.01;

      /* 'KalmanFilterMeasurement:1727' attitude_error.z = attitude_error.z * feedback_weight; */
      *attitude_error_z = ((-(dcm_en[1] - dcm_en[3])) / 2.0) * 0.01;
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    /* 'KalmanFilterMeasurement:1728' else */
    /* 'KalmanFilterMeasurement:1729' attitude_error.x = 0.0; */
    *attitude_error_x = 0.0;

    /* 'KalmanFilterMeasurement:1730' attitude_error.y = 0.0; */
    *attitude_error_y = 0.0;

    /* 'KalmanFilterMeasurement:1731' attitude_error.z = 0.0; */
    *attitude_error_z = 0.0;
  }
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [attitude_error] = GetECEFAttitudeErrorForTA(nav, ta)
 */
static void GetECEFAttitudeErrorForTA(const real_T nav[16], real_T
  ta_ac_attitude_psi, real_T ta_pos_g_lat, real_T ta_pos_g_lon, real_T
  *attitude_error_x, real_T *attitude_error_y, real_T *attitude_error_z)
{
  real_T dcm_en[9];
  real_T dcm_nav_attitude_e[9];
  real_T tmp[9];
  real_T dcm_en_0[9];
  int32_T i;
  int32_T i_0;
  real_T dcm_en_tmp;
  real_T dcm_en_tmp_0;
  real_T dcm_en_tmp_1;
  real_T dcm_en_tmp_2;
  int32_T dcm_en_tmp_3;

  /* 'KalmanFilterMeasurement:1670' feedback_weight = 0.5; */
  /* 'KalmanFilterMeasurement:1671' dcm_en = LIB_dcm_en(ta.pos_g); */
  /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
  /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
  /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
  dcm_en_tmp = -sin(ta_pos_g_lat);
  dcm_en_tmp_1 = cos(ta_pos_g_lon);
  dcm_en[0] = dcm_en_tmp * dcm_en_tmp_1;
  dcm_en_tmp_0 = sin(ta_pos_g_lon);
  dcm_en[3] = dcm_en_tmp * dcm_en_tmp_0;
  dcm_en_tmp_2 = cos(ta_pos_g_lat);
  dcm_en[6] = dcm_en_tmp_2;
  dcm_en[1] = -dcm_en_tmp_0;
  dcm_en[4] = dcm_en_tmp_1;
  dcm_en[7] = 0.0;
  dcm_en[2] = (-dcm_en_tmp_2) * dcm_en_tmp_1;
  dcm_en[5] = (-cos(ta_pos_g_lat)) * dcm_en_tmp_0;
  dcm_en[8] = dcm_en_tmp;

  /* 'KalmanFilterMeasurement:1672' dcm_ne = dcm_en'; */
  /* 'KalmanFilterMeasurement:1673' dcm_nav_attitude_e = LIB_q2d(struct('q0', nav(1,10), 'q1', nav(1,11), 'q2', nav(1,12), 'q3', nav(1,13))); */
  LIB_q2d(nav[9], nav[10], nav[11], nav[12], dcm_nav_attitude_e);

  /* 'KalmanFilterMeasurement:1674' attitude_nav_attitude_g = LIB_d2e(dcm_en*dcm_nav_attitude_e); */
  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i + (3 * i_0);
      dcm_en_0[dcm_en_tmp_3] = 0.0;
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[3 * i_0] * dcm_en[i];
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[(3 * i_0) + 1] * dcm_en[i + 3];
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[(3 * i_0) + 2] * dcm_en[i + 6];
    }
  }

  LIB_d2e(dcm_en_0, &dcm_en_tmp, &dcm_en_tmp_1, &dcm_en_tmp_0);

  /* 'KalmanFilterMeasurement:1675' attitude_ta_attitude_g  = struct('phi', attitude_nav_attitude_g.phi, 'the', attitude_nav_attitude_g.the, 'psi', ta.ac_attitude.psi); */
  /* 'KalmanFilterMeasurement:1676' dcm_ta_attitude_e = dcm_ne*LIB_e2d(attitude_ta_attitude_g); */
  /* 'KalmanFilterMeasurement:1677' attitude_error_e = dcm_nav_attitude_e*dcm_ta_attitude_e'; */
  LIB_e2d(dcm_en_tmp, dcm_en_tmp_1, ta_ac_attitude_psi, tmp);
  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i_0 + (3 * i);
      dcm_en_0[dcm_en_tmp_3] = 0.0;
      dcm_en_0[dcm_en_tmp_3] += dcm_en[3 * i] * tmp[3 * i_0];
      dcm_en_0[dcm_en_tmp_3] += dcm_en[(3 * i) + 1] * tmp[(3 * i_0) + 1];
      dcm_en_0[dcm_en_tmp_3] += dcm_en[(3 * i) + 2] * tmp[(3 * i_0) + 2];
    }
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i + (3 * i_0);
      dcm_en[dcm_en_tmp_3] = 0.0;
      dcm_en[dcm_en_tmp_3] += dcm_en_0[3 * i_0] * dcm_nav_attitude_e[i];
      dcm_en[dcm_en_tmp_3] += dcm_en_0[(3 * i_0) + 1] * dcm_nav_attitude_e[i + 3];
      dcm_en[dcm_en_tmp_3] += dcm_en_0[(3 * i_0) + 2] * dcm_nav_attitude_e[i + 6];
    }
  }

  /* 'KalmanFilterMeasurement:1678' attitude_error.x = -(attitude_error_e(3,2) - attitude_error_e(2,3))/2.0; */
  /* 'KalmanFilterMeasurement:1679' attitude_error.y = -(attitude_error_e(1,3) - attitude_error_e(3,1))/2.0; */
  /* 'KalmanFilterMeasurement:1680' attitude_error.z = -(attitude_error_e(2,1) - attitude_error_e(1,2))/2.0; */
  /* 'KalmanFilterMeasurement:1682' attitude_error.x = attitude_error.x * feedback_weight; */
  *attitude_error_x = ((-(dcm_en[5] - dcm_en[7])) / 2.0) * 0.5;

  /* 'KalmanFilterMeasurement:1683' attitude_error.y = attitude_error.y * feedback_weight; */
  *attitude_error_y = ((-(dcm_en[6] - dcm_en[2])) / 2.0) * 0.5;

  /* 'KalmanFilterMeasurement:1684' attitude_error.z = attitude_error.z * feedback_weight; */
  *attitude_error_z = ((-(dcm_en[1] - dcm_en[3])) / 2.0) * 0.5;
}

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz'
 * function [attitude_error] = GetECEFAttitudeErrorForMAG(nav, mag)
 */
static void GetECEFAttitudeErrorForMAG(const T_NAV_NavigationState *nav, const
  real_T mag_Attitude[3], real_T *attitude_error_x, real_T *attitude_error_y,
  real_T *attitude_error_z)
{
  real_T dcm_en[9];
  real_T dcm_nav_attitude_e[9];
  real_T tmp[9];
  real_T dcm_en_0[9];
  int32_T i;
  int32_T i_0;
  real_T dcm_en_tmp;
  real_T dcm_en_tmp_0;
  real_T dcm_en_tmp_1;
  real_T dcm_en_tmp_2;
  int32_T dcm_en_tmp_3;

  /* 'KalmanFilterMeasurement:1689' feedback_weight = 0.01; */
  /* 'KalmanFilterMeasurement:1690' dcm_en = LIB_dcm_en(nav.pos_g); */
  /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
  /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
  /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
  dcm_en_tmp = -sin(nav->pos_g.lat);
  dcm_en_tmp_1 = cos(nav->pos_g.lon);
  dcm_en[0] = dcm_en_tmp * dcm_en_tmp_1;
  dcm_en_tmp_0 = sin(nav->pos_g.lon);
  dcm_en[3] = dcm_en_tmp * dcm_en_tmp_0;
  dcm_en_tmp_2 = cos(nav->pos_g.lat);
  dcm_en[6] = dcm_en_tmp_2;
  dcm_en[1] = -dcm_en_tmp_0;
  dcm_en[4] = dcm_en_tmp_1;
  dcm_en[7] = 0.0;
  dcm_en[2] = (-dcm_en_tmp_2) * dcm_en_tmp_1;
  dcm_en[5] = (-cos(nav->pos_g.lat)) * dcm_en_tmp_0;
  dcm_en[8] = dcm_en_tmp;

  /* 'KalmanFilterMeasurement:1691' dcm_ne = dcm_en'; */
  /* 'KalmanFilterMeasurement:1692' dcm_nav_attitude_e = LIB_q2d(nav.q_be); */
  LIB_q2d(nav->q_be.q0, nav->q_be.q1, nav->q_be.q2, nav->q_be.q3,
          dcm_nav_attitude_e);

  /* 'KalmanFilterMeasurement:1693' attitude_nav_attitude_g = LIB_d2e(dcm_en*dcm_nav_attitude_e); */
  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i + (3 * i_0);
      dcm_en_0[dcm_en_tmp_3] = 0.0;
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[3 * i_0] * dcm_en[i];
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[(3 * i_0) + 1] * dcm_en[i + 3];
      dcm_en_0[dcm_en_tmp_3] += dcm_nav_attitude_e[(3 * i_0) + 2] * dcm_en[i + 6];
    }
  }

  LIB_d2e(dcm_en_0, &dcm_en_tmp, &dcm_en_tmp_1, &dcm_en_tmp_0);

  /* 'KalmanFilterMeasurement:1694' attitude_mag_attitude_g  = struct('phi', attitude_nav_attitude_g.phi, 'the', attitude_nav_attitude_g.the, 'psi', mag.Attitude(3)); */
  /* 'KalmanFilterMeasurement:1695' dcm_mag_attitude_e = dcm_ne*LIB_e2d(attitude_mag_attitude_g); */
  /* 'KalmanFilterMeasurement:1696' attitude_error_e = dcm_nav_attitude_e*dcm_mag_attitude_e'; */
  LIB_e2d(dcm_en_tmp, dcm_en_tmp_1, mag_Attitude[2], tmp);
  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i_0 + (3 * i);
      dcm_en_0[dcm_en_tmp_3] = 0.0;
      dcm_en_0[dcm_en_tmp_3] += dcm_en[3 * i] * tmp[3 * i_0];
      dcm_en_0[dcm_en_tmp_3] += dcm_en[(3 * i) + 1] * tmp[(3 * i_0) + 1];
      dcm_en_0[dcm_en_tmp_3] += dcm_en[(3 * i) + 2] * tmp[(3 * i_0) + 2];
    }
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    for (i = 0; i < 3; i++) {
      dcm_en_tmp_3 = i + (3 * i_0);
      dcm_en[dcm_en_tmp_3] = 0.0;
      dcm_en[dcm_en_tmp_3] += dcm_en_0[3 * i_0] * dcm_nav_attitude_e[i];
      dcm_en[dcm_en_tmp_3] += dcm_en_0[(3 * i_0) + 1] * dcm_nav_attitude_e[i + 3];
      dcm_en[dcm_en_tmp_3] += dcm_en_0[(3 * i_0) + 2] * dcm_nav_attitude_e[i + 6];
    }
  }

  /* 'KalmanFilterMeasurement:1697' attitude_error.x = -(attitude_error_e(3,2) - attitude_error_e(2,3))/2.0; */
  /* 'KalmanFilterMeasurement:1698' attitude_error.y = -(attitude_error_e(1,3) - attitude_error_e(3,1))/2.0; */
  /* 'KalmanFilterMeasurement:1699' attitude_error.z = -(attitude_error_e(2,1) - attitude_error_e(1,2))/2.0; */
  /* 'KalmanFilterMeasurement:1701' attitude_error.x = attitude_error.x * feedback_weight; */
  *attitude_error_x = ((-(dcm_en[5] - dcm_en[7])) / 2.0) * 0.01;

  /* 'KalmanFilterMeasurement:1702' attitude_error.y = attitude_error.y * feedback_weight; */
  *attitude_error_y = ((-(dcm_en[6] - dcm_en[2])) / 2.0) * 0.01;

  /* 'KalmanFilterMeasurement:1703' attitude_error.z = attitude_error.z * feedback_weight; */
  *attitude_error_z = ((-(dcm_en[1] - dcm_en[3])) / 2.0) * 0.01;
}

/* Model step function */
void NAV_DoNavigation_2Hz(RT_MODEL *const NAV_DoNavigationMain_2Hz_M,
  T_GNSS_Result *rtU_GNSS_Result, T_NAV_State *rtU_NavigationState,
  T_TA_TransferAlignState *rtU_TransferAlignState, T_NAV_MagnetometerMeasurement
  *rtU_MagnetometerMeasurement, T_NAV_KFilterErrorEstimateState
  *rtY_KalmanFilterErrorEstimatesOut, T_NAV_StoreInfo *rtY_StoreInfo, T_MAG_Tel *
  rtY_MagnetometerState)
{
  uint32_T magOutput_Valid;
  real_T magOutput_Attitude[3];
  real_T kfState_X[16];
  uint8_T baroState_valid;
  uint8_T gnss_Valid;
  uint8_T taState_valid;
  real_T gnssState_ecef_pos_x;
  real_T gnssState_ecef_pos_y;
  real_T gnssState_ecef_pos_z;
  real_T gnssState_ecef_vel_x;
  real_T gnssState_ecef_vel_y;
  real_T gnssState_ecef_vel_z;
  uint16_T gnssState_numberSatellites;
  real_T gnssState_time;
  real_T useNavDataForTA_attitude[16];
  real_T useNavDataForTA_velocity[16];
  real_T useNavDataForTA_position[16];
  real_T useNavDataForGNSS[16];
  real_T RR[9];
  real_T z_K[3];
  real_T ta_angle_error_x;
  real_T ta_angle_error_y;
  real_T ta_angle_error_z;
  real_T mag_angle_error_x;
  real_T mag_angle_error_y;
  real_T mag_angle_error_z;
  real_T baro_pos_e_x;
  real_T baro_pos_e_y;
  real_T baro_pos_e_z;
  real_T b_RR[36];
  real_T b_z_K[6];
  real_T c_RR[49];
  real_T c_z_K[7];
  real_T d_RR[144];
  real_T d_z_K[12];
  real_T e_RR[169];
  real_T e_z_K[13];
  real_T e_b[169];
  static const int8_T g_b[48] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 };

  static const int8_T h_b[96] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static const int8_T i_b[112] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T j_b[192] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static const int8_T k_b[208] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T a[48] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 };

  static const int8_T b_a[96] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static const int8_T c_a[112] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T d_a[192] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static const int8_T e_a[208] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  real_T expl_temp[28];
  real_T expl_temp_0[28];
  real_T expl_temp_1[84];
  real_T expl_temp_2[84];
  uint8_T expl_temp_3;
  uint32_T expl_temp_4;
  uint8_T expl_temp_5;
  uint8_T expl_temp_6;
  uint8_T expl_temp_7;
  uint32_T expl_temp_8;
  uint32_T expl_temp_9;
  int32_T expl_temp_a[3];
  uint32_T expl_temp_b;
  real_T K_K_tmp[48];
  int8_T K_K_tmp_0[48];
  real_T K_K_tmp_1[48];
  real_T b_K_K_tmp[96];
  int8_T b_K_K_tmp_tmp[96];
  real_T c_K_K_tmp[112];
  int8_T c_K_K_tmp_0[112];
  real_T kfState_P[256];
  real_T d_K_K_tmp[192];
  int8_T d_K_K_tmp_0[192];
  real_T kfState_P_tmp[256];
  real_T e_K_K_tmp[208];
  real_T K_K_tmp_2[9];
  real_T tmp[9];
  real_T b_a_0[36];
  real_T tmp_0[36];
  real_T c_a_0[49];
  real_T tmp_1[49];
  real_T d_a_0[144];
  real_T tmp_2[144];
  real_T e_a_0[169];
  real_T K_K_tmp_3[48];
  real_T b_K_K_tmp_tmp_0[96];
  real_T kfState_P_tmp_0[256];
  real_T kfState_P_tmp_1[256];
  real_T kfState_P_tmp_2[256];
  real_T b_a_1[96];
  real_T b_z_K_0[6];
  real_T c_a_1[112];
  real_T c_z_K_0[7];
  real_T c_K_K_tmp_1[112];
  real_T d_a_1[192];
  real_T d_z_K_0[12];
  real_T e_a_1[208];
  real_T e_z_K_0[13];
  real_T d_K_K_tmp_1[192];
  int32_T i;
  int32_T i_0;
  int32_T useNavDataForTA_attitude_tmp;
  int32_T K_K_tmp_tmp;
  int32_T kfState_P_tmp_tmp;
  const int8_T *kfState_X_tmp;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_2Hz_at_outport_1' incorporates:
   *  SubSystem: '<Root>/Subsystem'
   */
  /* MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' incorporates:
   *  Inport: '<Root>/GNSSReceiverStateIn'
   *  Inport: '<Root>/MagnetometerMeasurement'
   *  Inport: '<Root>/NavigationState'
   *  Inport: '<Root>/TransferAlignState'
   *  Outport: '<Root>/KalmanFilterErrorEstimatesOut'
   *  Outport: '<Root>/MagnetometerState'
   *  Outport: '<Root>/StoreInfo'
   */
  /* :  [KalmanFilterErrorEstimates, StoreInfo, MagnetometerState] = NAV_Embedded_DoNavigationMain2Hz(navState, kfState, baroState, gnss, NavigationMode, taState, magMeasurement); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:32' coder.cstructname(navState, 'T_NAV_NavigationState'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:33' coder.cstructname(navState.pos_g, 'T_GeographicPos'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:34' coder.cstructname(navState.fib_e, 'T_Cartesian'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:35' coder.cstructname(navState.grav_e, 'T_Cartesian'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:36' coder.cstructname(navState.pos_e, 'T_Cartesian'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:37' coder.cstructname(navState.vel_e, 'T_Cartesian'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:38' coder.cstructname(navState.att_bn, 'T_Euler'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:39' coder.cstructname(navState.q_be, 'T_Quaternion'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:40' coder.cstructname(navState.store, 'T_NAV_Store'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:41' coder.cstructname(kfState, 'T_NAV_KalmanFilterState'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:42' coder.cstructname(kfState.R, 'T_NAV_MeasurementNoise'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:43' coder.cstructname(baroState, 'T_NAV_BaroState'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:44' coder.cstructname(taState, 'T_TA_TransferAlignState'); */
  /* 'NAV_Embedded_DoNavigationMain2Hz:47' [magOutput, magState] = MAG_DoMagnetometerMain2Hz(magMeasurement, navState, NavigationMode, kfState); */
  MAG_DoMagnetometerMain2Hz(rtU_MagnetometerMeasurement->Valid,
    rtU_MagnetometerMeasurement->SystemTime,
    rtU_MagnetometerMeasurement->Field.x, rtU_MagnetometerMeasurement->Field.y,
    rtU_MagnetometerMeasurement->Field.z,
    &rtU_NavigationState->NAV_NavigationState,
    rtU_NavigationState->NavigationModeOut,
    &rtU_NavigationState->NAV_KalmanFilterState, &magOutput_Valid,
    magOutput_Attitude, RR, rtY_MagnetometerState,
    &(NAV_DoNavigationMain_2Hz_M->dwork));

  /* 'NAV_Embedded_DoNavigationMain2Hz:50' if NavigationMode >= 30 */
  if (((int32_T)rtU_NavigationState->NavigationModeOut) >= 30) {
    /* 'NAV_Embedded_DoNavigationMain2Hz:51' [KalmanFilterErrorEstimates, StoreInfo] = KalmanFilterMeasurement(kfState, navState, baroState, gnss, taState, magOutput); */
    memcpy(&kfState_X[0], &rtU_NavigationState->NAV_KalmanFilterState.X[0],
           (sizeof(real_T)) << 4U);
    baroState_valid = rtU_NavigationState->NAV_BaroState.valid;
    gnss_Valid = rtU_GNSS_Result->Valid;
    taState_valid = rtU_TransferAlignState->valid;

    /* 'KalmanFilterMeasurement:37' if isempty(delta_private) */
    /* 'KalmanFilterMeasurement:42' if isempty(KalmanFilterState_private) */
    /* 'KalmanFilterMeasurement:45' if isempty(KalmanFilterCovariance_private) */
    /* 'KalmanFilterMeasurement:48' if isempty(gnss_valid_private) */
    /* 'KalmanFilterMeasurement:53' if ~firstTimeMeasurement */
    if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.firstTimeMeasurement) == 0)
    {
      /* 'KalmanFilterMeasurement:54' gnss.Valid = uint8(0); */
      gnss_Valid = 0U;

      /* 'KalmanFilterMeasurement:55' baroState.valid = uint8(0); */
      baroState_valid = 0U;

      /* 'KalmanFilterMeasurement:56' taState.valid = uint8(0); */
      taState_valid = 0U;

      /* 'KalmanFilterMeasurement:57' magState.Valid = uint32(0); */
      magOutput_Valid = 0U;

      /* 'KalmanFilterMeasurement:58' gnss_valid_private = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 0U;

      /* 'KalmanFilterMeasurement:59' firstTimeMeasurement = uint8(1); */
      NAV_DoNavigationMain_2Hz_M->dwork.firstTimeMeasurement = 1U;

      /* 'KalmanFilterMeasurement:60' KalmanFilterState_private = kfState.X; */
      memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
             &rtU_NavigationState->NAV_KalmanFilterState.X[0], (sizeof(real_T)) <<
             4U);

      /* 'KalmanFilterMeasurement:61' KalmanFilterCovariance_private = kfState.P; */
      memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[0],
             &rtU_NavigationState->NAV_KalmanFilterState.P[0], (sizeof(real_T)) <<
             8U);
    }

    /* 'KalmanFilterMeasurement:65' KalmanFilterState_privateIn = kfState.X; */
    /* 'KalmanFilterMeasurement:66' KalmanFilterCovariance_privateIn = kfState.P; */
    /* 'KalmanFilterMeasurement:69' if gnss.Valid */
    if (((int32_T)gnss_Valid) != 0) {
      /* 'KalmanFilterMeasurement:70' gnssState = init_GNSSreceiver; */
      /* 'KalmanFilterMeasurement:71' gnssState.ecef_pos = struct('x', gnss.Pos(1), 'y', gnss.Pos(2), 'z', gnss.Pos(3)); */
      gnssState_ecef_pos_x = rtU_GNSS_Result->Pos[0];
      gnssState_ecef_pos_y = rtU_GNSS_Result->Pos[1];
      gnssState_ecef_pos_z = rtU_GNSS_Result->Pos[2];

      /* 'KalmanFilterMeasurement:72' gnssState.ecef_vel = struct('x', double(gnss.Vel(1)), 'y', double(gnss.Vel(2)), 'z', double(gnss.Vel(3))); */
      gnssState_ecef_vel_x = (real_T)rtU_GNSS_Result->Vel[0];
      gnssState_ecef_vel_y = (real_T)rtU_GNSS_Result->Vel[1];
      gnssState_ecef_vel_z = (real_T)rtU_GNSS_Result->Vel[2];

      /* 'KalmanFilterMeasurement:73' gnssState.numberSatellites = gnss.NoSVsInFix; */
      gnssState_numberSatellites = rtU_GNSS_Result->NoSVsInFix;

      /* 'KalmanFilterMeasurement:74' gnssState.valid = gnss.Valid; */
      /* 'KalmanFilterMeasurement:75' gnssState.time = gnss.T_meas.Sys; */
      gnssState_time = rtU_GNSS_Result->T_meas.Sys;

      /* 'KalmanFilterMeasurement:76' if time_updateINS == 0.0 */
      if (NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS == 0.0) {
        /* 'KalmanFilterMeasurement:77' time_updateINS = gnssState.time; */
        NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS =
          rtU_GNSS_Result->T_meas.Sys;
      }
    } else {
      /* 'KalmanFilterMeasurement:79' else */
      /* 'KalmanFilterMeasurement:80' gnssState = init_GNSSreceiver; */
      init_GNSSreceiver(expl_temp, expl_temp_0, &gnssState_ecef_pos_x,
                        &gnssState_ecef_pos_y, &gnssState_ecef_pos_z,
                        &gnssState_ecef_vel_x, &gnssState_ecef_vel_y,
                        &gnssState_ecef_vel_z, expl_temp_1, expl_temp_2,
                        &gnssState_numberSatellites, &gnss_Valid, &expl_temp_3,
                        &gnssState_time);

      /* 'KalmanFilterMeasurement:81' time_updateINS = 0.0; */
      NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS = 0.0;
    }

    /* 'KalmanFilterMeasurement:85' if ~navState.GuidCtrlFlags.Umbilical */
    if (((int32_T)
         rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.Umbilical) == 0)
    {
      /* 'KalmanFilterMeasurement:86' taState.valid = uint8(0); */
      taState_valid = 0U;
    }

    /* Outport: '<Root>/StoreInfo' incorporates:
     *  Inport: '<Root>/GNSSReceiverStateIn'
     *  Inport: '<Root>/TransferAlignState'
     */
    /* 'KalmanFilterMeasurement:91' StoreInfo     = NAV_CreateStruct_T_NAV_StoreInfo; */
    rtY_StoreInfo->TimeReq = 0U;
    rtY_StoreInfo->Case = 0U;
    rtY_StoreInfo->IdxLow = 0U;
    rtY_StoreInfo->IdxHi = 0U;
    rtY_StoreInfo->Reserved1 = 0U;
    rtY_StoreInfo->TimeLow = 0U;
    rtY_StoreInfo->TimeHi = 0U;
    rtY_StoreInfo->Pos[0] = 0;
    rtY_StoreInfo->Pos[1] = 0;
    rtY_StoreInfo->Pos[2] = 0;
    rtY_StoreInfo->Reserved2 = 0U;

    /* 'KalmanFilterMeasurement:92' gnssStoreInfo = NAV_CreateStruct_T_NAV_StoreInfo; */
    /* 'KalmanFilterMeasurement:95' if taState.valid */
    if (((int32_T)taState_valid) != 0) {
      /* 'KalmanFilterMeasurement:96' [useNavDataForTA_attitude, ~] = GetCorrespondingNavigationData(navState.store, taState.time.attitude); */
      GetCorrespondingNavigationData
        (&rtU_NavigationState->NAV_NavigationState.store,
         rtU_TransferAlignState->time.attitude, useNavDataForTA_attitude,
         &expl_temp_4, &expl_temp_3, &expl_temp_5, &expl_temp_6, &expl_temp_7,
         &expl_temp_8, &expl_temp_9, expl_temp_a, &expl_temp_b);

      /* 'KalmanFilterMeasurement:96' ~ */
      /* 'KalmanFilterMeasurement:97' [useNavDataForTA_velocity, ~] = GetCorrespondingNavigationData(navState.store, taState.time.velocity); */
      GetCorrespondingNavigationData
        (&rtU_NavigationState->NAV_NavigationState.store,
         rtU_TransferAlignState->time.velocity, useNavDataForTA_velocity,
         &expl_temp_4, &expl_temp_3, &expl_temp_5, &expl_temp_6, &expl_temp_7,
         &expl_temp_8, &expl_temp_9, expl_temp_a, &expl_temp_b);

      /* 'KalmanFilterMeasurement:97' ~ */
      /* 'KalmanFilterMeasurement:98' [useNavDataForTA_position, ~] = GetCorrespondingNavigationData(navState.store, taState.time.position); */
      GetCorrespondingNavigationData
        (&rtU_NavigationState->NAV_NavigationState.store,
         rtU_TransferAlignState->time.position, useNavDataForTA_position,
         &expl_temp_4, &expl_temp_3, &expl_temp_5, &expl_temp_6, &expl_temp_7,
         &expl_temp_8, &expl_temp_9, expl_temp_a, &expl_temp_b);

      /* 'KalmanFilterMeasurement:98' ~ */
    } else {
      /* 'KalmanFilterMeasurement:99' else */
      /* 'KalmanFilterMeasurement:100' useNavDataForTA_attitude = navState.store.store(navState.store.storeplace,2:end); */
      i_0 = (int32_T)rtU_NavigationState->NAV_NavigationState.store.storeplace;

      /* 'KalmanFilterMeasurement:101' useNavDataForTA_velocity = navState.store.store(navState.store.storeplace,2:end); */
      /* 'KalmanFilterMeasurement:102' useNavDataForTA_position = navState.store.store(navState.store.storeplace,2:end); */
      for (i = 0; i < 16; i++) {
        useNavDataForTA_attitude_tmp = (1 + i) * 120;
        useNavDataForTA_attitude[i] =
          rtU_NavigationState->NAV_NavigationState.store.store
          [(useNavDataForTA_attitude_tmp + i_0) - 1];
        useNavDataForTA_velocity[i] =
          rtU_NavigationState->NAV_NavigationState.store.store
          [(useNavDataForTA_attitude_tmp + i_0) - 1];
        useNavDataForTA_position[i] =
          rtU_NavigationState->NAV_NavigationState.store.store
          [(useNavDataForTA_attitude_tmp + i_0) - 1];
      }
    }

    /* 'KalmanFilterMeasurement:104' if gnssState.valid */
    if (((int32_T)gnss_Valid) != 0) {
      /* 'KalmanFilterMeasurement:105' [useNavDataForGNSS, StoreInfo] = GetCorrespondingNavigationData(navState.store, gnssState.time); */
      GetCorrespondingNavigationData
        (&rtU_NavigationState->NAV_NavigationState.store, gnssState_time,
         useNavDataForGNSS, &rtY_StoreInfo->TimeReq, &rtY_StoreInfo->Case,
         &rtY_StoreInfo->IdxLow, &rtY_StoreInfo->IdxHi,
         &rtY_StoreInfo->Reserved1, &rtY_StoreInfo->TimeLow,
         &rtY_StoreInfo->TimeHi, rtY_StoreInfo->Pos, &rtY_StoreInfo->Reserved2);

      /* 'KalmanFilterMeasurement:106' StoreInfo.Pos = int32(useNavDataForGNSS(1:3)'.*256); */
      baro_pos_e_x = rt_roundd_snf(useNavDataForGNSS[0] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[0] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[0] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[0] = MAX_int32_T;
      }

      baro_pos_e_x = rt_roundd_snf(useNavDataForGNSS[1] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[1] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[1] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[1] = MAX_int32_T;
      }

      baro_pos_e_x = rt_roundd_snf(useNavDataForGNSS[2] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[2] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[2] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[2] = MAX_int32_T;
      }
    } else {
      /* 'KalmanFilterMeasurement:107' else */
      /* 'KalmanFilterMeasurement:108' useNavDataForGNSS = navState.store.store(navState.store.storeplace,2:end); */
      i_0 = (int32_T)rtU_NavigationState->NAV_NavigationState.store.storeplace;
      for (i = 0; i < 16; i++) {
        useNavDataForGNSS[i] =
          rtU_NavigationState->NAV_NavigationState.store.store[(((1 + i) * 120)
          + i_0) - 1];
      }

      /* 'KalmanFilterMeasurement:109' gnssStoreInfo.Pos  = int32(useNavDataForGNSS(1:3)'.*256); */
      /* 'KalmanFilterMeasurement:110' gnssStoreInfo.Case = uint8(5); */
    }

    /* 'KalmanFilterMeasurement:112' if ((~taState.valid) && (~gnssState.valid)) */
    if ((((int32_T)taState_valid) == 0) && (((int32_T)gnss_Valid) == 0)) {
      /* 'KalmanFilterMeasurement:113' useNavData = navState.store.store(navState.store.storeplace,2:end); */
      /* 'KalmanFilterMeasurement:114' StoreInfo.Pos  = int32(useNavData(1:3)'.*256); */
      i_0 = (int32_T)rtU_NavigationState->NAV_NavigationState.store.storeplace;
      baro_pos_e_x = rt_roundd_snf
        (rtU_NavigationState->NAV_NavigationState.store.store[i_0 + 119] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[0] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[0] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[0] = MAX_int32_T;
      }

      baro_pos_e_x = rt_roundd_snf
        (rtU_NavigationState->NAV_NavigationState.store.store[i_0 + 239] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[1] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[1] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[1] = MAX_int32_T;
      }

      baro_pos_e_x = rt_roundd_snf
        (rtU_NavigationState->NAV_NavigationState.store.store[i_0 + 359] * 256.0);
      if (baro_pos_e_x < 2.147483648E+9) {
        if (baro_pos_e_x >= -2.147483648E+9) {
          rtY_StoreInfo->Pos[2] = (int32_T)baro_pos_e_x;
        } else {
          rtY_StoreInfo->Pos[2] = MIN_int32_T;
        }
      } else {
        rtY_StoreInfo->Pos[2] = MAX_int32_T;
      }

      /* 'KalmanFilterMeasurement:115' StoreInfo.Case = uint8(6); */
      rtY_StoreInfo->Case = 6U;
    }

    /* 'KalmanFilterMeasurement:120' if ((gnssState.valid) && (baroState.valid) && (taState.valid) && (magState.Valid)) */
    if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) != 0)) &&
         (((int32_T)taState_valid) != 0)) && (magOutput_Valid != 0U)) {
      /* 'KalmanFilterMeasurement:122' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:123' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:124'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:125'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:126'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:127'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:128'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:129'                                0.0; ... */
        /* 'KalmanFilterMeasurement:130'                                0.0; ... */
        /* 'KalmanFilterMeasurement:131'                                0.0; ... */
        /* 'KalmanFilterMeasurement:132'                                0.0; ... */
        /* 'KalmanFilterMeasurement:133'                                0.0; ... */
        /* 'KalmanFilterMeasurement:134'                                0.0; ... */
        /* 'KalmanFilterMeasurement:135'                                0.0; ... */
        /* 'KalmanFilterMeasurement:136'                                0.0; ... */
        /* 'KalmanFilterMeasurement:137'                                0.0; ... */
        /* 'KalmanFilterMeasurement:138'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:139' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:140' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:141' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:142' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:143' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:144' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:145' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:146' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:147' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:148' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:149' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:150' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:151' else */
        /* 'KalmanFilterMeasurement:153' numberMeasurements = (6 + 6 + 1); */
        /* 'KalmanFilterMeasurement:154' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:155' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:156' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:157' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:158' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:159' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:160' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:161' HH(7,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:162' HH(8,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:163' HH(9,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:164' HH(10,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:165' HH(11,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:166' HH(12,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:167' HH(13,16) = 1.0; */
        /* 'KalmanFilterMeasurement:170' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&e_RR[0], 0, 169U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:171' RR(1,1)   = kfState.R.range; */
        e_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:172' RR(2,2)   = kfState.R.range; */
        e_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:173' RR(3,3)   = kfState.R.range; */
        e_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:174' RR(4,4)   = kfState.R.rrate; */
        e_RR[42] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:175' RR(5,5)   = kfState.R.rrate; */
        e_RR[56] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:176' RR(6,6)   = kfState.R.rrate; */
        e_RR[70] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:177' RR(7,7)   = kfState.R.ta_position; */
        e_RR[84] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:178' RR(8,8)   = kfState.R.ta_position; */
        e_RR[98] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:179' RR(9,9)   = kfState.R.ta_position; */
        e_RR[112] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:180' RR(10,10) = kfState.R.ta_velocity; */
        e_RR[126] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:181' RR(11,11) = kfState.R.ta_velocity; */
        e_RR[140] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:182' RR(12,12) = kfState.R.ta_velocity; */
        e_RR[154] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:183' RR(13,13) = kfState.R.baroG; */
        e_RR[168] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:186' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:187' gnss_geo_pos_temp = LIB_ecef2lla(gnssState.ecef_pos); */
        /* 'KalmanFilterMeasurement:189' z_K(1,1)  = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        e_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:190' z_K(2,1)  = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        e_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:191' z_K(3,1)  = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        e_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:192' z_K(4,1)  = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        e_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:193' z_K(5,1)  = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        e_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:194' z_K(6,1)  = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        e_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:195' z_K(7,1)  = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        e_z_K[6] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:196' z_K(8,1)  = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        e_z_K[7] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:197' z_K(9,1)  = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        e_z_K[8] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:198' z_K(10,1) = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        e_z_K[9] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:199' z_K(11,1) = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        e_z_K[10] = useNavDataForTA_velocity[4] -
          rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:200' z_K(12,1) = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        e_z_K[11] = useNavDataForTA_velocity[5] -
          rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:201' z_K(13,1) = ((useNavDataForGNSS(1,7) - gnss_geo_pos_temp.alt) + (useNavDataForTA_position(1,7) - taState.pos_g.alt))/2.0; */
        LIB_ecef2lla(gnssState_ecef_pos_x, gnssState_ecef_pos_y,
                     gnssState_ecef_pos_z, &baro_pos_e_x, &baro_pos_e_y,
                     &baro_pos_e_z);
        e_z_K[12] = ((useNavDataForGNSS[6] - baro_pos_e_z) +
                     (useNavDataForTA_position[6] -
                      rtU_TransferAlignState->pos_g.alt)) / 2.0;

        /* 'KalmanFilterMeasurement:204' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:205' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:206' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:207' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + mag_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = ((ta_angle_error_x + mag_angle_error_x) + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:208' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + mag_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = ((ta_angle_error_y + mag_angle_error_y) + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:209' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + mag_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = ((ta_angle_error_z + mag_angle_error_z) + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:212' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 13; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (13 * i);
            e_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              e_a_1[K_K_tmp_tmp] += ((real_T)e_a[(13 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 13; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += e_a_1[(13 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)k_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (13 * i) + i_0;
            e_a_0[K_K_tmp_tmp] = e_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_ga6ZmLc1(e_a_0, e_b);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 13; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            e_a_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              e_a_1[K_K_tmp_tmp] += rtU_NavigationState->
                NAV_KalmanFilterState.P[(kfState_P_tmp_tmp << ((uint64_T)4)) +
                i_0] * ((real_T)k_b[useNavDataForTA_attitude_tmp +
                        kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 13; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            e_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              e_K_K_tmp[kfState_P_tmp_tmp] += e_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * e_b
                [(13 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:213' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        kfState_X_tmp = &e_a[0];
        for (i_0 = 0; i_0 < 13; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)kfState_X_tmp[(13 * i) + i_0]) *
              kfState_X[i];
          }

          e_z_K_0[i_0] = e_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:214' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 13; i++) {
            baro_pos_e_x += e_K_K_tmp[(i << ((uint64_T)4)) + i_0] * e_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += e_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)kfState_X_tmp[(13 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 13; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            e_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              e_a_1[kfState_P_tmp_tmp] += e_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * e_RR
                [(13 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += e_a_1[kfState_P_tmp_tmp + i_0] *
                e_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:216' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:217' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:218' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:219' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:220' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:221' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:222' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:223' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:224' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:225' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:226' else */
          /* 'KalmanFilterMeasurement:227' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:229' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:230' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:231' else */
          /* 'KalmanFilterMeasurement:232' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:234' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:235' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:236' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:237' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:238' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:243' elseif ((gnssState.valid) && (~baroState.valid) && (taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:245' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:246' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:247'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:248'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:249'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:250'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:251'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:252'                                0.0; ... */
        /* 'KalmanFilterMeasurement:253'                                0.0; ... */
        /* 'KalmanFilterMeasurement:254'                                0.0; ... */
        /* 'KalmanFilterMeasurement:255'                                0.0; ... */
        /* 'KalmanFilterMeasurement:256'                                0.0; ... */
        /* 'KalmanFilterMeasurement:257'                                0.0; ... */
        /* 'KalmanFilterMeasurement:258'                                0.0; ... */
        /* 'KalmanFilterMeasurement:259'                                0.0; ... */
        /* 'KalmanFilterMeasurement:260'                                0.0; ... */
        /* 'KalmanFilterMeasurement:261'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:262' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:263' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:264' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:265' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:266' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:267' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:268' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:269' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:270' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:271' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:272' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:273' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:274' else */
        /* 'KalmanFilterMeasurement:276' numberMeasurements = (6 + 6); */
        /* 'KalmanFilterMeasurement:277' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:278' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:279' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:280' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:281' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:282' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:283' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:284' HH(7,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:285' HH(8,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:286' HH(9,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:287' HH(10,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:288' HH(11,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:289' HH(12,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:292' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&d_RR[0], 0, 144U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:293' RR(1,1)   = kfState.R.range; */
        d_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:294' RR(2,2)   = kfState.R.range; */
        d_RR[13] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:295' RR(3,3)   = kfState.R.range; */
        d_RR[26] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:296' RR(4,4)   = kfState.R.rrate; */
        d_RR[39] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:297' RR(5,5)   = kfState.R.rrate; */
        d_RR[52] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:298' RR(6,6)   = kfState.R.rrate; */
        d_RR[65] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:299' RR(7,7)   = kfState.R.ta_position; */
        d_RR[78] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:300' RR(8,8)   = kfState.R.ta_position; */
        d_RR[91] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:301' RR(9,9)   = kfState.R.ta_position; */
        d_RR[104] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:302' RR(10,10) = kfState.R.ta_velocity; */
        d_RR[117] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:303' RR(11,11) = kfState.R.ta_velocity; */
        d_RR[130] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:304' RR(12,12) = kfState.R.ta_velocity; */
        d_RR[143] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:307' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:309' z_K(1,1)  = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        d_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:310' z_K(2,1)  = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        d_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:311' z_K(3,1)  = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        d_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:312' z_K(4,1)  = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        d_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:313' z_K(5,1)  = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        d_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:314' z_K(6,1)  = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        d_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:315' z_K(7,1)  = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        d_z_K[6] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:316' z_K(8,1)  = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        d_z_K[7] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:317' z_K(9,1)  = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        d_z_K[8] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:318' z_K(10,1) = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        d_z_K[9] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:319' z_K(11,1) = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        d_z_K[10] = useNavDataForTA_velocity[4] -
          rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:320' z_K(12,1) = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        d_z_K[11] = useNavDataForTA_velocity[5] -
          rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:323' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:324' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:325' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:326' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + mag_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = ((ta_angle_error_x + mag_angle_error_x) + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:327' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + mag_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = ((ta_angle_error_y + mag_angle_error_y) + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:328' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + mag_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = ((ta_angle_error_z + mag_angle_error_z) + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:331' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 192; i_0++) {
          d_K_K_tmp[i_0] = (real_T)j_b[i_0];
          d_K_K_tmp_0[i_0] = d_a[i_0];
        }

        for (i_0 = 0; i_0 < 12; i_0++) {
          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (12 * i);
            d_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              d_K_K_tmp_1[kfState_P_tmp_tmp] += ((real_T)d_K_K_tmp_0[(12 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 12; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += d_K_K_tmp_1[(12 * useNavDataForTA_attitude_tmp) +
                i_0] * d_K_K_tmp[(i << ((uint64_T)4)) +
                useNavDataForTA_attitude_tmp];
            }

            kfState_P_tmp_tmp = (12 * i) + i_0;
            d_a_0[kfState_P_tmp_tmp] = d_RR[kfState_P_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_l0Es7Mh4(d_a_0, tmp_2);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 12; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            d_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              d_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * d_K_K_tmp[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          for (i = 0; i < 12; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            d_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              d_a_1[kfState_P_tmp_tmp] += d_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_2
                [(12 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:332' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 12; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)d_K_K_tmp_0[(12 * i) + i_0]) * kfState_X[i];
          }

          d_z_K_0[i_0] = d_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:333' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 12; i++) {
            baro_pos_e_x += d_a_1[(i << ((uint64_T)4)) + i_0] * d_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += d_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)d_K_K_tmp_0[(12 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 12; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            d_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              d_K_K_tmp[kfState_P_tmp_tmp] += d_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * d_RR
                [(12 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += d_K_K_tmp[kfState_P_tmp_tmp + i_0] *
                d_a_1[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:335' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:336' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:337' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:338' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:339' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:340' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:341' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:342' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:343' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:344' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:345' else */
          /* 'KalmanFilterMeasurement:346' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:348' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:349' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:350' else */
          /* 'KalmanFilterMeasurement:351' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:353' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:354' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:355' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:356' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:357' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:362' elseif ((~gnssState.valid) && (baroState.valid) && (taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:364' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:365' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:366'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:367'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:368'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:369'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:370'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:371'                                0.0; ... */
        /* 'KalmanFilterMeasurement:372'                                0.0; ... */
        /* 'KalmanFilterMeasurement:373'                                0.0; ... */
        /* 'KalmanFilterMeasurement:374'                                0.0; ... */
        /* 'KalmanFilterMeasurement:375'                                0.0; ... */
        /* 'KalmanFilterMeasurement:376'                                0.0; ... */
        /* 'KalmanFilterMeasurement:377'                                0.0; ... */
        /* 'KalmanFilterMeasurement:378'                                0.0; ... */
        /* 'KalmanFilterMeasurement:379'                                0.0; ... */
        /* 'KalmanFilterMeasurement:380'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:381' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:382' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:383' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:384' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:385' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:386' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:387' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:388' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:389' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:390' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:391' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:392' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:393' else */
        /* 'KalmanFilterMeasurement:395' numberMeasurements = (6 + 1); */
        /* 'KalmanFilterMeasurement:396' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:397' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:398' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:399' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:400' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:401' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:402' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:403' HH(7,16) = 1.0; */
        /* 'KalmanFilterMeasurement:406' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&c_RR[0], 0, 49U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:407' RR(1,1)   = kfState.R.ta_position; */
        c_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:408' RR(2,2)   = kfState.R.ta_position; */
        c_RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:409' RR(3,3)   = kfState.R.ta_position; */
        c_RR[16] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:410' RR(4,4)   = kfState.R.ta_velocity; */
        c_RR[24] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:411' RR(5,5)   = kfState.R.ta_velocity; */
        c_RR[32] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:412' RR(6,6)   = kfState.R.ta_velocity; */
        c_RR[40] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:413' RR(7,7)   = kfState.R.baroG; */
        c_RR[48] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:416' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:418' z_K(1,1)  = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        c_z_K[0] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:419' z_K(2,1)  = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        c_z_K[1] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:420' z_K(3,1)  = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        c_z_K[2] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:421' z_K(4,1)  = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        c_z_K[3] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:422' z_K(5,1)  = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        c_z_K[4] = useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:423' z_K(6,1)  = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        c_z_K[5] = useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:424' z_K(7,1)  = useNavDataForTA_position(1,7) - taState.pos_g.alt; */
        c_z_K[6] = useNavDataForTA_position[6] -
          rtU_TransferAlignState->pos_g.alt;

        /* 'KalmanFilterMeasurement:427' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:428' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:429' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + mag_angle_error.x); */
        kfState_X[6] = (ta_angle_error_x + mag_angle_error_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:430' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + mag_angle_error.y); */
        kfState_X[7] = (ta_angle_error_y + mag_angle_error_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:431' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + mag_angle_error.z); */
        kfState_X[8] = (ta_angle_error_z + mag_angle_error_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:435' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 112; i_0++) {
          c_K_K_tmp[i_0] = (real_T)i_b[i_0];
          c_K_K_tmp_0[i_0] = c_a[i_0];
        }

        for (i_0 = 0; i_0 < 7; i_0++) {
          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (7 * i);
            c_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp_1[kfState_P_tmp_tmp] += ((real_T)c_K_K_tmp_0[(7 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 7; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_K_K_tmp_1[(7 * useNavDataForTA_attitude_tmp) +
                i_0] * c_K_K_tmp[(i << ((uint64_T)4)) +
                useNavDataForTA_attitude_tmp];
            }

            kfState_P_tmp_tmp = (7 * i) + i_0;
            c_a_0[kfState_P_tmp_tmp] = c_RR[kfState_P_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_dM5u0dSj(c_a_0, tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 7; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            c_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              c_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * c_K_K_tmp[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_a_1[kfState_P_tmp_tmp] += c_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_1
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:436' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)c_K_K_tmp_0[(7 * i) + i_0]) * kfState_X[i];
          }

          c_z_K_0[i_0] = c_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:437' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 7; i++) {
            baro_pos_e_x += c_a_1[(i << ((uint64_T)4)) + i_0] * c_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += c_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)c_K_K_tmp_0[(7 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp[kfState_P_tmp_tmp] += c_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * c_RR
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += c_K_K_tmp[kfState_P_tmp_tmp + i_0] *
                c_a_1[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:439' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:440' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:441' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:442' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:443' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:444' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:445' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:446' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:447' if ((taState.time.velocity - time_updateINS) >= kfState.timeupdateINS) */
        if ((rtU_TransferAlignState->time.velocity -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:448' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:449' else */
          /* 'KalmanFilterMeasurement:450' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:452' if ((taState.time.attitude - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((rtU_TransferAlignState->time.attitude -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:453' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:454' else */
          /* 'KalmanFilterMeasurement:455' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:457' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:458' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:459' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:460' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:461' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:466' elseif ((~gnssState.valid) && (~baroState.valid) && (taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:468' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:469' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:470'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:471'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:472'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:473'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:474'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:475'                                0.0; ... */
        /* 'KalmanFilterMeasurement:476'                                0.0; ... */
        /* 'KalmanFilterMeasurement:477'                                0.0; ... */
        /* 'KalmanFilterMeasurement:478'                                0.0; ... */
        /* 'KalmanFilterMeasurement:479'                                0.0; ... */
        /* 'KalmanFilterMeasurement:480'                                0.0; ... */
        /* 'KalmanFilterMeasurement:481'                                0.0; ... */
        /* 'KalmanFilterMeasurement:482'                                0.0; ... */
        /* 'KalmanFilterMeasurement:483'                                0.0; ... */
        /* 'KalmanFilterMeasurement:484'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:485' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:486' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:487' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:488' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:489' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:490' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:491' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:492' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:493' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:494' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:495' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:496' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:497' else */
        /* 'KalmanFilterMeasurement:499' numberMeasurements = (6); */
        /* 'KalmanFilterMeasurement:500' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:501' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:502' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:503' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:504' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:505' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:506' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:509' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&b_RR[0], 0, 36U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:510' RR(1,1)   = kfState.R.ta_position; */
        b_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:511' RR(2,2)   = kfState.R.ta_position; */
        b_RR[7] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:512' RR(3,3)   = kfState.R.ta_position; */
        b_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:513' RR(4,4)   = kfState.R.ta_velocity; */
        b_RR[21] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:514' RR(5,5)   = kfState.R.ta_velocity; */
        b_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:515' RR(6,6)   = kfState.R.ta_velocity; */
        b_RR[35] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:518' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:520' z_K(1,1)  = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        b_z_K[0] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:521' z_K(2,1)  = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        b_z_K[1] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:522' z_K(3,1)  = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        b_z_K[2] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:523' z_K(4,1)  = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        b_z_K[3] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:524' z_K(5,1)  = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        b_z_K[4] = useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:525' z_K(6,1)  = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        b_z_K[5] = useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:528' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:529' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:530' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + mag_angle_error.x); */
        kfState_X[6] = (ta_angle_error_x + mag_angle_error_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:531' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + mag_angle_error.y); */
        kfState_X[7] = (ta_angle_error_y + mag_angle_error_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:532' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + mag_angle_error.z); */
        kfState_X[8] = (ta_angle_error_z + mag_angle_error_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:535' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 96; i_0++) {
          b_K_K_tmp[i_0] = (real_T)h_b[i_0];
          b_K_K_tmp_tmp[i_0] = b_a[i_0];
        }

        for (i_0 = 0; i_0 < 6; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (6 * i);
            b_K_K_tmp_tmp_0[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              b_K_K_tmp_tmp_0[K_K_tmp_tmp] += ((real_T)b_K_K_tmp_tmp[(6 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 6; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_K_K_tmp_tmp_0[(6 * useNavDataForTA_attitude_tmp)
                + i_0] * b_K_K_tmp[(i << ((uint64_T)4)) +
                useNavDataForTA_attitude_tmp];
            }

            K_K_tmp_tmp = (6 * i) + i_0;
            b_a_0[K_K_tmp_tmp] = b_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_GAaZsA6r(b_a_0, tmp_0);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 6; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            b_K_K_tmp_tmp_0[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              b_K_K_tmp_tmp_0[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * b_K_K_tmp[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_a_1[kfState_P_tmp_tmp] += b_K_K_tmp_tmp_0
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_0
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:536' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)b_K_K_tmp_tmp[(6 * i) + i_0]) * kfState_X[i];
          }

          b_z_K_0[i_0] = b_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:537' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 6; i++) {
            baro_pos_e_x += b_a_1[(i << ((uint64_T)4)) + i_0] * b_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += b_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)b_K_K_tmp_tmp[(6 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_K_K_tmp[kfState_P_tmp_tmp] += b_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * b_RR
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += b_K_K_tmp[kfState_P_tmp_tmp + i_0] *
                b_a_1[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:539' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:540' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:541' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:542' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:543' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:544' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:545' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:546' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:547' if ((taState.time.velocity - time_updateINS) >= kfState.timeupdateINS) */
        if ((rtU_TransferAlignState->time.velocity -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:548' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:549' else */
          /* 'KalmanFilterMeasurement:550' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:552' if ((taState.time.attitude - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((rtU_TransferAlignState->time.attitude -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:553' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:554' else */
          /* 'KalmanFilterMeasurement:555' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:557' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:558' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:559' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:560' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:561' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:566' elseif ((gnssState.valid) && (baroState.valid) && (~taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:568' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:569' delta_private.nav = [useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; ... */
        /* 'KalmanFilterMeasurement:570'                              useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; ... */
        /* 'KalmanFilterMeasurement:571'                              useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; ... */
        /* 'KalmanFilterMeasurement:572'                              useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; ... */
        /* 'KalmanFilterMeasurement:573'                              useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; ... */
        /* 'KalmanFilterMeasurement:574'                              useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; ... */
        /* 'KalmanFilterMeasurement:575'                              0.0; ... */
        /* 'KalmanFilterMeasurement:576'                              0.0; ... */
        /* 'KalmanFilterMeasurement:577'                              0.0; ... */
        /* 'KalmanFilterMeasurement:578'                              0.0; ... */
        /* 'KalmanFilterMeasurement:579'                              0.0; ... */
        /* 'KalmanFilterMeasurement:580'                              0.0; ... */
        /* 'KalmanFilterMeasurement:581'                              0.0; ... */
        /* 'KalmanFilterMeasurement:582'                              0.0; ... */
        /* 'KalmanFilterMeasurement:583'                              0.0; ... */
        /* 'KalmanFilterMeasurement:584'                              0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForGNSS[0] - gnssState_ecef_pos_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForGNSS[1] - gnssState_ecef_pos_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForGNSS[2] - gnssState_ecef_pos_z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForGNSS[3] - gnssState_ecef_vel_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForGNSS[4] - gnssState_ecef_vel_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForGNSS[5] - gnssState_ecef_vel_z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:585' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:586' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:587' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:588' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:589' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:590' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:591' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:592' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:593' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:594' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:595' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:596' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:597' else */
        /* 'KalmanFilterMeasurement:599' numberMeasurements = (6 + 1); */
        /* 'KalmanFilterMeasurement:600' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:601' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:602' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:603' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:604' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:605' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:606' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:607' HH(7,16) = 1.0; */
        /* 'KalmanFilterMeasurement:610' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&c_RR[0], 0, 49U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:611' RR(1,1)   = kfState.R.range; */
        c_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:612' RR(2,2)   = kfState.R.range; */
        c_RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:613' RR(3,3)   = kfState.R.range; */
        c_RR[16] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:614' RR(4,4)   = kfState.R.rrate; */
        c_RR[24] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:615' RR(5,5)   = kfState.R.rrate; */
        c_RR[32] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:616' RR(6,6)   = kfState.R.rrate; */
        c_RR[40] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:617' RR(7,7)   = kfState.R.baroG; */
        c_RR[48] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:620' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:621' gnss_geo_pos_temp = LIB_ecef2lla(gnssState.ecef_pos); */
        /* 'KalmanFilterMeasurement:623' z_K(1,1)  = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        c_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:624' z_K(2,1)  = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        c_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:625' z_K(3,1)  = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        c_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:626' z_K(4,1)  = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        c_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:627' z_K(5,1)  = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        c_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:628' z_K(6,1)  = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        c_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:629' z_K(7,1)  = useNavDataForGNSS(1,7) - gnss_geo_pos_temp.alt; */
        LIB_ecef2lla(gnssState_ecef_pos_x, gnssState_ecef_pos_y,
                     gnssState_ecef_pos_z, &baro_pos_e_x, &baro_pos_e_y,
                     &baro_pos_e_z);
        c_z_K[6] = useNavDataForGNSS[6] - baro_pos_e_z;

        /* 'KalmanFilterMeasurement:632' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:633' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:634' kfState.X(7,1) = kfState.X(7,1) + (mag_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = (mag_angle_error_x + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:635' kfState.X(8,1) = kfState.X(8,1) + (mag_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = (mag_angle_error_y + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:636' kfState.X(9,1) = kfState.X(9,1) + (mag_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = (mag_angle_error_z + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:639' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (7 * i);
            c_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              c_a_1[K_K_tmp_tmp] += ((real_T)c_a[(7 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 7; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_a_1[(7 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)i_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (7 * i) + i_0;
            c_a_0[K_K_tmp_tmp] = c_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_dM5u0dSj(c_a_0, tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 7; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            c_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              c_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                i_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp[kfState_P_tmp_tmp] += c_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_1
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:640' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)c_a[(7 * i) + i_0]) * kfState_X[i];
          }

          c_z_K_0[i_0] = c_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:641' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 7; i++) {
            baro_pos_e_x += c_K_K_tmp[(i << ((uint64_T)4)) + i_0] * c_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)c_a[(7 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)c_a[(7 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp_1[kfState_P_tmp_tmp] += c_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * c_RR
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += c_K_K_tmp_1[kfState_P_tmp_tmp + i_0]
                * c_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:643' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:644' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:645' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:646' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:647' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:648' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:649' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:650' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:651' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:652' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:653' else */
          /* 'KalmanFilterMeasurement:654' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:656' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:657' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:658' else */
          /* 'KalmanFilterMeasurement:659' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:661' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:662' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:663' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:664' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:665' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:670' elseif ((gnssState.valid) && (~baroState.valid) && (~taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:672' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:673' delta_private.nav = [useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; ... */
        /* 'KalmanFilterMeasurement:674'                              useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; ... */
        /* 'KalmanFilterMeasurement:675'                              useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; ... */
        /* 'KalmanFilterMeasurement:676'                              useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; ... */
        /* 'KalmanFilterMeasurement:677'                              useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; ... */
        /* 'KalmanFilterMeasurement:678'                              useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; ... */
        /* 'KalmanFilterMeasurement:679'                              0.0; ... */
        /* 'KalmanFilterMeasurement:680'                              0.0; ... */
        /* 'KalmanFilterMeasurement:681'                              0.0; ... */
        /* 'KalmanFilterMeasurement:682'                              0.0; ... */
        /* 'KalmanFilterMeasurement:683'                              0.0; ... */
        /* 'KalmanFilterMeasurement:684'                              0.0; ... */
        /* 'KalmanFilterMeasurement:685'                              0.0; ... */
        /* 'KalmanFilterMeasurement:686'                              0.0; ... */
        /* 'KalmanFilterMeasurement:687'                              0.0; ... */
        /* 'KalmanFilterMeasurement:688'                              0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForGNSS[0] - gnssState_ecef_pos_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForGNSS[1] - gnssState_ecef_pos_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForGNSS[2] - gnssState_ecef_pos_z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForGNSS[3] - gnssState_ecef_vel_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForGNSS[4] - gnssState_ecef_vel_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForGNSS[5] - gnssState_ecef_vel_z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:689' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:690' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:691' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:692' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:693' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:694' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:695' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:696' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:697' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:698' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:699' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:700' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:701' else */
        /* 'KalmanFilterMeasurement:703' numberMeasurements = (6); */
        /* 'KalmanFilterMeasurement:704' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:705' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:706' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:707' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:708' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:709' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:710' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:713' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&b_RR[0], 0, 36U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:714' RR(1,1) = kfState.R.range; */
        b_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:715' RR(2,2) = kfState.R.range; */
        b_RR[7] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:716' RR(3,3) = kfState.R.range; */
        b_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:717' RR(4,4) = kfState.R.rrate; */
        b_RR[21] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:718' RR(5,5) = kfState.R.rrate; */
        b_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:719' RR(6,6) = kfState.R.rrate; */
        b_RR[35] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:722' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:724' z_K(1,1) = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        b_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:725' z_K(2,1) = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        b_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:726' z_K(3,1) = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        b_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:727' z_K(4,1) = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        b_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:728' z_K(5,1) = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        b_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:729' z_K(6,1) = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        b_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:732' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
        GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
          magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
          &mag_angle_error_z);

        /* 'KalmanFilterMeasurement:733' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:734' kfState.X(7,1) = kfState.X(7,1) + (mag_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = (mag_angle_error_x + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:735' kfState.X(8,1) = kfState.X(8,1) + (mag_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = (mag_angle_error_y + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:736' kfState.X(9,1) = kfState.X(9,1) + (mag_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = (mag_angle_error_z + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:739' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (6 * i);
            b_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              b_a_1[K_K_tmp_tmp] += ((real_T)b_a[(6 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 6; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_a_1[(6 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)h_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (6 * i) + i_0;
            b_a_0[K_K_tmp_tmp] = b_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_GAaZsA6r(b_a_0, tmp_0);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 6; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            b_K_K_tmp_tmp_0[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              b_K_K_tmp_tmp_0[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                h_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_K_K_tmp[kfState_P_tmp_tmp] += b_K_K_tmp_tmp_0
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_0
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:740' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)b_a[(6 * i) + i_0]) * kfState_X[i];
          }

          b_z_K_0[i_0] = b_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:741' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 6; i++) {
            baro_pos_e_x += b_K_K_tmp[(i << ((uint64_T)4)) + i_0] * b_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)b_a[(6 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)b_a[(6 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_a_1[kfState_P_tmp_tmp] += b_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * b_RR
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += b_a_1[kfState_P_tmp_tmp + i_0] *
                b_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:743' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:744' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:745' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:746' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:747' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:748' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:749' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:750' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

        /* 'KalmanFilterMeasurement:751' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        baro_pos_e_x = gnssState_time -
          NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS;
        if (baro_pos_e_x >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:752' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:753' else */
          /* 'KalmanFilterMeasurement:754' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:756' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if (baro_pos_e_x >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:757' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:758' else */
          /* 'KalmanFilterMeasurement:759' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:761' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:762' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:763' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:764' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:765' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid !=
                0U)) {
      /* 'KalmanFilterMeasurement:770' elseif ((~gnssState.valid) && (baroState.valid) && (~taState.valid) && (magState.Valid)) */
      /* 'KalmanFilterMeasurement:772' baro_pos_g.lat = navState.pos_g.lat; */
      /* 'KalmanFilterMeasurement:773' baro_pos_g.lon = navState.pos_g.lon; */
      /* 'KalmanFilterMeasurement:774' baro_pos_g.alt = navState.baro_alt; */
      /* 'KalmanFilterMeasurement:775' baro_pos_e = LIB_lla2ecef(baro_pos_g); */
      LIB_lla2ecef(rtU_NavigationState->NAV_NavigationState.pos_g.lat,
                   rtU_NavigationState->NAV_NavigationState.pos_g.lon,
                   rtU_NavigationState->NAV_NavigationState.baro_alt,
                   &baro_pos_e_x, &baro_pos_e_y, &baro_pos_e_z);

      /* 'KalmanFilterMeasurement:778' numberMeasurements = (3); */
      /* 'KalmanFilterMeasurement:779' HH = zeros(numberMeasurements, 16); */
      /* 'KalmanFilterMeasurement:780' HH(1,1) = 1.0; */
      /* 'KalmanFilterMeasurement:781' HH(2,2) = 1.0; */
      /* 'KalmanFilterMeasurement:782' HH(3,3) = 1.0; */
      /* 'KalmanFilterMeasurement:785' RR = zeros(numberMeasurements, numberMeasurements); */
      memset(&RR[0], 0, 9U * (sizeof(real_T)));

      /* 'KalmanFilterMeasurement:786' if ((double(navState.GuidCtrlFlags.PstaticValid) >= 1.0) && (double(navState.GuidCtrlFlags.PstaticValid) <= 9.0)) */
      if ((((int32_T)
            rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid)
           >= 1) && (((int32_T)
                      rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid)
                     <= 9)) {
        /* 'KalmanFilterMeasurement:787' RR(1,1) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        gnssState_time = (5.0 - ((5.0 * ((real_T)
          rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid)) /
          10.0)) * rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;
        RR[0] = gnssState_time;

        /* 'KalmanFilterMeasurement:788' RR(2,2) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        RR[4] = gnssState_time;

        /* 'KalmanFilterMeasurement:789' RR(3,3) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        RR[8] = gnssState_time;
      } else {
        /* 'KalmanFilterMeasurement:790' else */
        /* 'KalmanFilterMeasurement:791' RR(1,1) = kfState.R.baro_P_N; */
        RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:792' RR(2,2) = kfState.R.baro_P_N; */
        RR[4] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:793' RR(3,3) = kfState.R.baro_P_N; */
        RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;
      }

      /* 'KalmanFilterMeasurement:797' z_K = zeros(numberMeasurements,1); */
      /* 'KalmanFilterMeasurement:799' z_K(1,1) = navState.pos_e.x - baro_pos_e.x; */
      z_K[0] = rtU_NavigationState->NAV_NavigationState.pos_e.x - baro_pos_e_x;

      /* 'KalmanFilterMeasurement:800' z_K(2,1) = navState.pos_e.y - baro_pos_e.y; */
      z_K[1] = rtU_NavigationState->NAV_NavigationState.pos_e.y - baro_pos_e_y;

      /* 'KalmanFilterMeasurement:801' z_K(3,1) = navState.pos_e.z - baro_pos_e.z; */
      z_K[2] = rtU_NavigationState->NAV_NavigationState.pos_e.z - baro_pos_e_z;

      /* 'KalmanFilterMeasurement:804' mag_angle_error = GetECEFAttitudeErrorForMAG(navState, magState); */
      GetECEFAttitudeErrorForMAG(&rtU_NavigationState->NAV_NavigationState,
        magOutput_Attitude, &mag_angle_error_x, &mag_angle_error_y,
        &mag_angle_error_z);

      /* 'KalmanFilterMeasurement:805' kfState.X(7,1) = kfState.X(7,1) + (mag_angle_error.x); */
      kfState_X[6] = rtU_NavigationState->NAV_KalmanFilterState.X[6] +
        mag_angle_error_x;

      /* 'KalmanFilterMeasurement:806' kfState.X(8,1) = kfState.X(8,1) + (mag_angle_error.y); */
      kfState_X[7] = rtU_NavigationState->NAV_KalmanFilterState.X[7] +
        mag_angle_error_y;

      /* 'KalmanFilterMeasurement:807' kfState.X(9,1) = kfState.X(9,1) + (mag_angle_error.z); */
      kfState_X[8] = rtU_NavigationState->NAV_KalmanFilterState.X[8] +
        mag_angle_error_z;

      /* 'KalmanFilterMeasurement:810' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
      for (i_0 = 0; i_0 < 3; i_0++) {
        for (i = 0; i < 16; i++) {
          K_K_tmp_tmp = i_0 + (3 * i);
          K_K_tmp_1[K_K_tmp_tmp] = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            K_K_tmp_1[K_K_tmp_tmp] += ((real_T)a[(3 *
              useNavDataForTA_attitude_tmp) + i_0]) *
              rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
              + useNavDataForTA_attitude_tmp];
          }
        }

        for (i = 0; i < 3; i++) {
          baro_pos_e_x = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            baro_pos_e_x += K_K_tmp_1[(3 * useNavDataForTA_attitude_tmp) + i_0] *
              ((real_T)g_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
          }

          K_K_tmp_tmp = (3 * i) + i_0;
          K_K_tmp_2[K_K_tmp_tmp] = RR[K_K_tmp_tmp] + baro_pos_e_x;
        }
      }

      inv_h7eIJkDj(K_K_tmp_2, tmp);
      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 3; i++) {
          useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
          K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
          K_K_tmp_3[K_K_tmp_tmp] = 0.0;
          for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16; kfState_P_tmp_tmp
               ++) {
            K_K_tmp_3[K_K_tmp_tmp] +=
              rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
              ((uint64_T)4)) + i_0] * ((real_T)g_b[useNavDataForTA_attitude_tmp
              + kfState_P_tmp_tmp]);
          }
        }

        for (i = 0; i < 3; i++) {
          K_K_tmp_tmp = i_0 + (i << ((uint64_T)4));
          K_K_tmp[K_K_tmp_tmp] = 0.0;
          K_K_tmp[K_K_tmp_tmp] += tmp[3 * i] * K_K_tmp_3[i_0];
          K_K_tmp[K_K_tmp_tmp] += tmp[(3 * i) + 1] * K_K_tmp_3[i_0 + 16];
          K_K_tmp[K_K_tmp_tmp] += tmp[(3 * i) + 2] * K_K_tmp_3[i_0 + 32];
        }
      }

      /* 'KalmanFilterMeasurement:811' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
      for (i_0 = 0; i_0 < 3; i_0++) {
        baro_pos_e_x = 0.0;
        for (i = 0; i < 16; i++) {
          baro_pos_e_x += ((real_T)a[(3 * i) + i_0]) * kfState_X[i];
        }

        magOutput_Attitude[i_0] = z_K[i_0] - baro_pos_e_x;
      }

      /* 'KalmanFilterMeasurement:812' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
      /* 'KalmanFilterMeasurement:814' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
      /* 'KalmanFilterMeasurement:815' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
      /* 'KalmanFilterMeasurement:816' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
      eye_iBsVrrYi(kfState_P);
      eye_iBsVrrYi(kfState_P_tmp_1);
      for (i_0 = 0; i_0 < 16; i_0++) {
        gnssState_time = K_K_tmp[i_0 + 16];
        gnssState_ecef_pos_x = K_K_tmp[i_0 + 32];
        gnssState_ecef_pos_y = (((gnssState_time * magOutput_Attitude[1]) +
          (K_K_tmp[i_0] * magOutput_Attitude[0])) + (gnssState_ecef_pos_x *
          magOutput_Attitude[2])) + kfState_X[i_0];
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
          gnssState_ecef_pos_y *
          rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0] =
          gnssState_ecef_pos_y - rtU_NavigationState->
          NAV_KalmanFilterState.X[i_0];
        for (i = 0; i < 16; i++) {
          useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
          kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
            kfState_P[useNavDataForTA_attitude_tmp] - (((((real_T)a[(3 * i) + 1])
            * gnssState_time) + (((real_T)a[3 * i]) * K_K_tmp[i_0])) + (((real_T)
            a[(3 * i) + 2]) * gnssState_ecef_pos_x));
        }

        for (i = 0; i < 16; i++) {
          useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
          K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
          kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
          for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16; kfState_P_tmp_tmp
               ++) {
            kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
              ((uint64_T)4)) + i_0] *
              rtU_NavigationState->
              NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
              kfState_P_tmp_tmp];
          }
        }

        kfState_X[i_0] = gnssState_ecef_pos_y;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 16; i++) {
          kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
            ((uint64_T)4)) + i] - (((((real_T)a[(3 * i_0) + 1]) * K_K_tmp[i + 16])
            + (((real_T)a[3 * i_0]) * K_K_tmp[i])) + (((real_T)a[(3 * i_0) + 2])
            * K_K_tmp[i + 32]));
        }

        for (i = 0; i < 3; i++) {
          K_K_tmp_tmp = i_0 + (i << ((uint64_T)4));
          K_K_tmp_3[K_K_tmp_tmp] = 0.0;
          K_K_tmp_3[K_K_tmp_tmp] += RR[3 * i] * K_K_tmp[i_0];
          K_K_tmp_3[K_K_tmp_tmp] += RR[(3 * i) + 1] * K_K_tmp[i_0 + 16];
          K_K_tmp_3[K_K_tmp_tmp] += RR[(3 * i) + 2] * K_K_tmp[i_0 + 32];
        }
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 16; i++) {
          useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
          K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
          kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
          for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16; kfState_P_tmp_tmp
               ++) {
            kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp <<
              ((uint64_T)4)) + i_0] * kfState_P[useNavDataForTA_attitude_tmp +
              kfState_P_tmp_tmp];
          }

          kfState_P_tmp_0[K_K_tmp_tmp] = 0.0;
          kfState_P_tmp_0[K_K_tmp_tmp] += K_K_tmp_3[i_0] * K_K_tmp[i];
          kfState_P_tmp_0[K_K_tmp_tmp] += K_K_tmp_3[i_0 + 16] * K_K_tmp[i + 16];
          kfState_P_tmp_0[K_K_tmp_tmp] += K_K_tmp_3[i_0 + 32] * K_K_tmp[i + 32];
        }
      }

      for (i_0 = 0; i_0 < 256; i_0++) {
        NAV_DoNavigationMain_2Hz_M->
          dwork.delta_private.kalmanfilter.covariance[i_0] =
          (kfState_P_tmp_1[i_0] + kfState_P_tmp_0[i_0]) -
          rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
      }

      /* 'KalmanFilterMeasurement:817' delta_private.numbersatellites = uint16(0.0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites = 0U;

      /* 'KalmanFilterMeasurement:818' delta_private.valid.gnss = gnssState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

      /* 'KalmanFilterMeasurement:819' delta_private.valid.baro = baroState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
        baroState_valid;

      /* 'KalmanFilterMeasurement:820' delta_private.valid.ta   = taState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

      /* 'KalmanFilterMeasurement:821' delta_private.valid.mag  = magState.Valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 1U;

      /* 'KalmanFilterMeasurement:822' if (double(navState.GuidCtrlFlags.PstaticValid) > 0.0) */
      if (((int32_T)
           rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid) >
          0) {
        /* 'KalmanFilterMeasurement:823' delta_private.valid.feedback_nav = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 1U;

        /* 'KalmanFilterMeasurement:824' delta_private.valid.feedback_att = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 1U;
      } else {
        /* 'KalmanFilterMeasurement:825' else */
        /* 'KalmanFilterMeasurement:826' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;
      }

      /* 'KalmanFilterMeasurement:828' delta_private.valid.reset_navigation = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
        0U;

      /* 'KalmanFilterMeasurement:829' delta_private.trade = delta_private.trade + 1; */
      magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade +
        /*MW:OvSatOk*/ 1U;
      if (magOutput_Valid <
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
        magOutput_Valid = MAX_uint32_T;
      }

      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

      /* 'KalmanFilterMeasurement:830' gnss_valid_private = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 0U;

      /* 'KalmanFilterMeasurement:831' time_updateINS = 0.0; */
      NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS = 0.0;
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:880' elseif ((gnssState.valid) && (baroState.valid) && (taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:882' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:883' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:884'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:885'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:886'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:887'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:888'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:889'                                0.0; ... */
        /* 'KalmanFilterMeasurement:890'                                0.0; ... */
        /* 'KalmanFilterMeasurement:891'                                0.0; ... */
        /* 'KalmanFilterMeasurement:892'                                0.0; ... */
        /* 'KalmanFilterMeasurement:893'                                0.0; ... */
        /* 'KalmanFilterMeasurement:894'                                0.0; ... */
        /* 'KalmanFilterMeasurement:895'                                0.0; ... */
        /* 'KalmanFilterMeasurement:896'                                0.0; ... */
        /* 'KalmanFilterMeasurement:897'                                0.0; ... */
        /* 'KalmanFilterMeasurement:898'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:899' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:900' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:901' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:902' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:903' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:904' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:905' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:906' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:907' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:908' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:909' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:910' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:911' else */
        /* 'KalmanFilterMeasurement:913' numberMeasurements = (6 + 6 + 1); */
        /* 'KalmanFilterMeasurement:914' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:915' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:916' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:917' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:918' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:919' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:920' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:921' HH(7,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:922' HH(8,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:923' HH(9,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:924' HH(10,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:925' HH(11,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:926' HH(12,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:927' HH(13,16) = 1.0; */
        /* 'KalmanFilterMeasurement:930' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&e_RR[0], 0, 169U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:931' RR(1,1)   = kfState.R.range; */
        e_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:932' RR(2,2)   = kfState.R.range; */
        e_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:933' RR(3,3)   = kfState.R.range; */
        e_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:934' RR(4,4)   = kfState.R.rrate; */
        e_RR[42] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:935' RR(5,5)   = kfState.R.rrate; */
        e_RR[56] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:936' RR(6,6)   = kfState.R.rrate; */
        e_RR[70] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:937' RR(7,7)   = kfState.R.ta_position; */
        e_RR[84] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:938' RR(8,8)   = kfState.R.ta_position; */
        e_RR[98] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:939' RR(9,9)   = kfState.R.ta_position; */
        e_RR[112] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:940' RR(10,10) = kfState.R.ta_velocity; */
        e_RR[126] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:941' RR(11,11) = kfState.R.ta_velocity; */
        e_RR[140] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:942' RR(12,12) = kfState.R.ta_velocity; */
        e_RR[154] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:943' RR(13,13) = kfState.R.baroG; */
        e_RR[168] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:946' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:947' gnss_geo_pos_temp = LIB_ecef2lla(gnssState.ecef_pos); */
        /* 'KalmanFilterMeasurement:949' z_K(1,1)  = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        e_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:950' z_K(2,1)  = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        e_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:951' z_K(3,1)  = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        e_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:952' z_K(4,1)  = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        e_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:953' z_K(5,1)  = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        e_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:954' z_K(6,1)  = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        e_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:955' z_K(7,1)  = useNavDataForTA_position(1,1)   - taState.pos_e.x; */
        e_z_K[6] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:956' z_K(8,1)  = useNavDataForTA_position(1,2)   - taState.pos_e.y; */
        e_z_K[7] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:957' z_K(9,1)  = useNavDataForTA_position(1,3)   - taState.pos_e.z; */
        e_z_K[8] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:958' z_K(10,1) = useNavDataForTA_velocity(1,4)   - taState.vel_e.x; */
        e_z_K[9] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:959' z_K(11,1) = useNavDataForTA_velocity(1,5)   - taState.vel_e.y; */
        e_z_K[10] = useNavDataForTA_velocity[4] -
          rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:960' z_K(12,1) = useNavDataForTA_velocity(1,6)   - taState.vel_e.z; */
        e_z_K[11] = useNavDataForTA_velocity[5] -
          rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:961' z_K(13,1) = ((useNavDataForGNSS(1,7) - gnss_geo_pos_temp.alt) + (useNavDataForTA_position(1,7) - taState.pos_g.alt))/2.0; */
        LIB_ecef2lla(gnssState_ecef_pos_x, gnssState_ecef_pos_y,
                     gnssState_ecef_pos_z, &baro_pos_e_x, &baro_pos_e_y,
                     &baro_pos_e_z);
        e_z_K[12] = ((useNavDataForGNSS[6] - baro_pos_e_z) +
                     (useNavDataForTA_position[6] -
                      rtU_TransferAlignState->pos_g.alt)) / 2.0;

        /* 'KalmanFilterMeasurement:964' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:965' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:966' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = (ta_angle_error_x + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:967' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = (ta_angle_error_y + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:968' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = (ta_angle_error_z + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:971' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 13; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (13 * i);
            e_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              e_a_1[K_K_tmp_tmp] += ((real_T)e_a[(13 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 13; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += e_a_1[(13 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)k_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (13 * i) + i_0;
            e_a_0[K_K_tmp_tmp] = e_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_ga6ZmLc1(e_a_0, e_b);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 13; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            e_a_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              e_a_1[K_K_tmp_tmp] += rtU_NavigationState->
                NAV_KalmanFilterState.P[(kfState_P_tmp_tmp << ((uint64_T)4)) +
                i_0] * ((real_T)k_b[useNavDataForTA_attitude_tmp +
                        kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 13; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            e_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              e_K_K_tmp[kfState_P_tmp_tmp] += e_a_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * e_b
                [(13 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:972' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 13; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)e_a[(13 * i) + i_0]) * kfState_X[i];
          }

          e_z_K_0[i_0] = e_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:973' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 13; i++) {
            baro_pos_e_x += e_K_K_tmp[(i << ((uint64_T)4)) + i_0] * e_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += e_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)e_a[(13 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 13; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            e_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              e_a_1[kfState_P_tmp_tmp] += e_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * e_RR
                [(13 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 13; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += e_a_1[kfState_P_tmp_tmp + i_0] *
                e_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:975' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:976' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:977' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:978' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:979' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:980' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:981' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:982' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:983' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:984' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:985' else */
          /* 'KalmanFilterMeasurement:986' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:988' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:989' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:990' else */
          /* 'KalmanFilterMeasurement:991' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:993' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:994' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:995' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:996' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:997' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1002' elseif ((gnssState.valid) && (~baroState.valid) && (taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1004' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:1005' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:1006'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:1007'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:1008'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:1009'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:1010'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:1011'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1012'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1013'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1014'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1015'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1016'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1017'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1018'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1019'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1020'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1021' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1022' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1023' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1024' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1025' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1026' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1027' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1028' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:1029' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:1030' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:1031' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1032' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1033' else */
        /* 'KalmanFilterMeasurement:1035' numberMeasurements = (6 + 6); */
        /* 'KalmanFilterMeasurement:1036' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:1037' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:1038' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:1039' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:1040' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:1041' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:1042' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:1043' HH(7,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:1044' HH(8,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:1045' HH(9,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:1046' HH(10,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:1047' HH(11,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:1048' HH(12,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:1051' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&d_RR[0], 0, 144U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1052' RR(1,1)   = kfState.R.range; */
        d_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1053' RR(2,2)   = kfState.R.range; */
        d_RR[13] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1054' RR(3,3)   = kfState.R.range; */
        d_RR[26] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1055' RR(4,4)   = kfState.R.rrate; */
        d_RR[39] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1056' RR(5,5)   = kfState.R.rrate; */
        d_RR[52] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1057' RR(6,6)   = kfState.R.rrate; */
        d_RR[65] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1058' RR(7,7)   = kfState.R.ta_position; */
        d_RR[78] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1059' RR(8,8)   = kfState.R.ta_position; */
        d_RR[91] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1060' RR(9,9)   = kfState.R.ta_position; */
        d_RR[104] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1061' RR(10,10) = kfState.R.ta_velocity; */
        d_RR[117] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1062' RR(11,11) = kfState.R.ta_velocity; */
        d_RR[130] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1063' RR(12,12) = kfState.R.ta_velocity; */
        d_RR[143] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1066' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:1068' z_K(1,1)  = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        d_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:1069' z_K(2,1)  = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        d_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:1070' z_K(3,1)  = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        d_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:1071' z_K(4,1)  = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        d_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:1072' z_K(5,1)  = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        d_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:1073' z_K(6,1)  = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        d_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:1074' z_K(7,1)  = useNavDataForTA_position(1,1)   - taState.pos_e.x; */
        d_z_K[6] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:1075' z_K(8,1)  = useNavDataForTA_position(1,2)   - taState.pos_e.y; */
        d_z_K[7] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:1076' z_K(9,1)  = useNavDataForTA_position(1,3)   - taState.pos_e.z; */
        d_z_K[8] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:1077' z_K(10,1) = useNavDataForTA_velocity(1,4)   - taState.vel_e.x; */
        d_z_K[9] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:1078' z_K(11,1) = useNavDataForTA_velocity(1,5)   - taState.vel_e.y; */
        d_z_K[10] = useNavDataForTA_velocity[4] -
          rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:1079' z_K(12,1) = useNavDataForTA_velocity(1,6)   - taState.vel_e.z; */
        d_z_K[11] = useNavDataForTA_velocity[5] -
          rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:1082' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:1083' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:1084' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x + gnss_angle_error.x); */
        kfState_X[6] = (ta_angle_error_x + baro_pos_e_x) +
          rtU_NavigationState->NAV_KalmanFilterState.X[6];

        /* 'KalmanFilterMeasurement:1085' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y + gnss_angle_error.y); */
        kfState_X[7] = (ta_angle_error_y + baro_pos_e_y) +
          rtU_NavigationState->NAV_KalmanFilterState.X[7];

        /* 'KalmanFilterMeasurement:1086' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z + gnss_angle_error.z); */
        kfState_X[8] = (ta_angle_error_z + baro_pos_e_z) +
          rtU_NavigationState->NAV_KalmanFilterState.X[8];

        /* 'KalmanFilterMeasurement:1089' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 12; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (12 * i);
            d_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              d_a_1[K_K_tmp_tmp] += ((real_T)d_a[(12 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 12; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += d_a_1[(12 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)j_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (12 * i) + i_0;
            d_a_0[K_K_tmp_tmp] = d_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_l0Es7Mh4(d_a_0, tmp_2);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 12; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            d_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              d_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                j_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 12; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            d_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              d_K_K_tmp[kfState_P_tmp_tmp] += d_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_2
                [(12 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:1090' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 12; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)d_a[(12 * i) + i_0]) * kfState_X[i];
          }

          d_z_K_0[i_0] = d_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:1091' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 12; i++) {
            baro_pos_e_x += d_K_K_tmp[(i << ((uint64_T)4)) + i_0] * d_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp[kfState_P_tmp_tmp] += d_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
                ((real_T)d_a[(12 * i) + useNavDataForTA_attitude_tmp]);
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P_tmp_0[i_0] = kfState_P[i_0] - kfState_P_tmp[i_0];
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }

            kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
              kfState_P_tmp_1[K_K_tmp_tmp] - kfState_P_tmp[K_K_tmp_tmp];
          }

          for (i = 0; i < 12; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            d_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              d_K_K_tmp_1[kfState_P_tmp_tmp] += d_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * d_RR
                [(12 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] *
                kfState_P_tmp_2[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 12; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += d_K_K_tmp_1[kfState_P_tmp_tmp + i_0]
                * d_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:1093' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:1094' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1095' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1096' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1097' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1098' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1099' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1100' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1101' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:1102' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1103' else */
          /* 'KalmanFilterMeasurement:1104' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:1106' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:1107' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1108' else */
          /* 'KalmanFilterMeasurement:1109' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:1111' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:1112' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1113' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:1114' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:1115' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1120' elseif ((~gnssState.valid) && (baroState.valid) && (taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1122' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:1123' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:1124'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:1125'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:1126'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:1127'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:1128'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:1129'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1130'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1131'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1132'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1133'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1134'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1135'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1136'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1137'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1138'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1139' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1140' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1141' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1142' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:1143' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:1144' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1145' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1146' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:1147' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:1148' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:1149' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1150' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1151' else */
        /* 'KalmanFilterMeasurement:1153' numberMeasurements = (6 + 1); */
        /* 'KalmanFilterMeasurement:1154' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:1155' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:1156' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:1157' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:1158' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:1159' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:1160' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:1161' HH(7,16) = 1.0; */
        /* 'KalmanFilterMeasurement:1164' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&c_RR[0], 0, 49U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1165' RR(1,1)   = kfState.R.ta_position; */
        c_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1166' RR(2,2)   = kfState.R.ta_position; */
        c_RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1167' RR(3,3)   = kfState.R.ta_position; */
        c_RR[16] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1168' RR(4,4)   = kfState.R.ta_velocity; */
        c_RR[24] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1169' RR(5,5)   = kfState.R.ta_velocity; */
        c_RR[32] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1170' RR(6,6)   = kfState.R.ta_velocity; */
        c_RR[40] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1171' RR(7,7)   = kfState.R.baroG; */
        c_RR[48] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:1174' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:1176' z_K(1,1)  = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        c_z_K[0] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:1177' z_K(2,1)  = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        c_z_K[1] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:1178' z_K(3,1)  = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        c_z_K[2] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:1179' z_K(4,1)  = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        c_z_K[3] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:1180' z_K(5,1)  = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        c_z_K[4] = useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:1181' z_K(6,1)  = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        c_z_K[5] = useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:1182' z_K(7,1)  = useNavDataForTA_position(1,7) - taState.pos_g.alt; */
        c_z_K[6] = useNavDataForTA_position[6] -
          rtU_TransferAlignState->pos_g.alt;

        /* 'KalmanFilterMeasurement:1185' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:1186' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x); */
        kfState_X[6] = rtU_NavigationState->NAV_KalmanFilterState.X[6] +
          ta_angle_error_x;

        /* 'KalmanFilterMeasurement:1187' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y); */
        kfState_X[7] = rtU_NavigationState->NAV_KalmanFilterState.X[7] +
          ta_angle_error_y;

        /* 'KalmanFilterMeasurement:1188' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z); */
        kfState_X[8] = rtU_NavigationState->NAV_KalmanFilterState.X[8] +
          ta_angle_error_z;

        /* 'KalmanFilterMeasurement:1191' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (7 * i);
            c_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              c_a_1[K_K_tmp_tmp] += ((real_T)c_a[(7 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 7; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_a_1[(7 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)i_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (7 * i) + i_0;
            c_a_0[K_K_tmp_tmp] = c_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_dM5u0dSj(c_a_0, tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 7; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            c_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              c_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                i_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp[kfState_P_tmp_tmp] += c_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_1
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:1192' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)c_a[(7 * i) + i_0]) * kfState_X[i];
          }

          c_z_K_0[i_0] = c_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:1193' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 7; i++) {
            baro_pos_e_x += c_K_K_tmp[(i << ((uint64_T)4)) + i_0] * c_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)c_a[(7 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)c_a[(7 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp_1[kfState_P_tmp_tmp] += c_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * c_RR
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += c_K_K_tmp_1[kfState_P_tmp_tmp + i_0]
                * c_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:1195' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:1196' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1197' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1198' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1199' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:1200' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:1201' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1202' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1203' if ((taState.time.velocity - time_updateINS) >= kfState.timeupdateINS) */
        if ((rtU_TransferAlignState->time.velocity -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:1204' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1205' else */
          /* 'KalmanFilterMeasurement:1206' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:1208' if ((taState.time.attitude - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((rtU_TransferAlignState->time.attitude -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:1209' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1210' else */
          /* 'KalmanFilterMeasurement:1211' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:1213' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:1214' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1215' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:1216' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:1217' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) != 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1222' elseif ((~gnssState.valid) && (~baroState.valid) && (taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1224' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:1225' delta_private.nav = [useNavDataForTA_position(1,1) - taState.pos_e.x; ... */
        /* 'KalmanFilterMeasurement:1226'                              useNavDataForTA_position(1,2) - taState.pos_e.y; ... */
        /* 'KalmanFilterMeasurement:1227'                              useNavDataForTA_position(1,3) - taState.pos_e.z; ... */
        /* 'KalmanFilterMeasurement:1228'                              useNavDataForTA_velocity(1,4) - taState.vel_e.x; ... */
        /* 'KalmanFilterMeasurement:1229'                              useNavDataForTA_velocity(1,5) - taState.vel_e.y; ... */
        /* 'KalmanFilterMeasurement:1230'                              useNavDataForTA_velocity(1,6) - taState.vel_e.z; ... */
        /* 'KalmanFilterMeasurement:1231'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1232'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1233'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1234'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1235'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1236'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1237'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1238'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1239'                                0.0; ... */
        /* 'KalmanFilterMeasurement:1240'                                0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1241' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1242' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1243' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1244' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:1245' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1246' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1247' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1248' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:1249' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:1250' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:1251' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1252' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1253' else */
        /* 'KalmanFilterMeasurement:1255' numberMeasurements = (6); */
        /* 'KalmanFilterMeasurement:1256' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:1257' HH(1,1)   = 1.0; */
        /* 'KalmanFilterMeasurement:1258' HH(2,2)   = 1.0; */
        /* 'KalmanFilterMeasurement:1259' HH(3,3)   = 1.0; */
        /* 'KalmanFilterMeasurement:1260' HH(4,4)   = 1.0; */
        /* 'KalmanFilterMeasurement:1261' HH(5,5)   = 1.0; */
        /* 'KalmanFilterMeasurement:1262' HH(6,6)   = 1.0; */
        /* 'KalmanFilterMeasurement:1265' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&b_RR[0], 0, 36U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1266' RR(1,1)   = kfState.R.ta_position; */
        b_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1267' RR(2,2)   = kfState.R.ta_position; */
        b_RR[7] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1268' RR(3,3)   = kfState.R.ta_position; */
        b_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_position;

        /* 'KalmanFilterMeasurement:1269' RR(4,4)   = kfState.R.ta_velocity; */
        b_RR[21] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1270' RR(5,5)   = kfState.R.ta_velocity; */
        b_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1271' RR(6,6)   = kfState.R.ta_velocity; */
        b_RR[35] = rtU_NavigationState->NAV_KalmanFilterState.R.ta_velocity;

        /* 'KalmanFilterMeasurement:1274' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:1276' z_K(1,1) = useNavDataForTA_position(1,1) - taState.pos_e.x; */
        b_z_K[0] = useNavDataForTA_position[0] - rtU_TransferAlignState->pos_e.x;

        /* 'KalmanFilterMeasurement:1277' z_K(2,1) = useNavDataForTA_position(1,2) - taState.pos_e.y; */
        b_z_K[1] = useNavDataForTA_position[1] - rtU_TransferAlignState->pos_e.y;

        /* 'KalmanFilterMeasurement:1278' z_K(3,1) = useNavDataForTA_position(1,3) - taState.pos_e.z; */
        b_z_K[2] = useNavDataForTA_position[2] - rtU_TransferAlignState->pos_e.z;

        /* 'KalmanFilterMeasurement:1279' z_K(4,1) = useNavDataForTA_velocity(1,4) - taState.vel_e.x; */
        b_z_K[3] = useNavDataForTA_velocity[3] - rtU_TransferAlignState->vel_e.x;

        /* 'KalmanFilterMeasurement:1280' z_K(5,1) = useNavDataForTA_velocity(1,5) - taState.vel_e.y; */
        b_z_K[4] = useNavDataForTA_velocity[4] - rtU_TransferAlignState->vel_e.y;

        /* 'KalmanFilterMeasurement:1281' z_K(6,1) = useNavDataForTA_velocity(1,6) - taState.vel_e.z; */
        b_z_K[5] = useNavDataForTA_velocity[5] - rtU_TransferAlignState->vel_e.z;

        /* 'KalmanFilterMeasurement:1284' ta_angle_error = GetECEFAttitudeErrorForTA(useNavDataForTA_attitude, taState); */
        GetECEFAttitudeErrorForTA(useNavDataForTA_attitude,
          rtU_TransferAlignState->ac_attitude.psi,
          rtU_TransferAlignState->pos_g.lat, rtU_TransferAlignState->pos_g.lon,
          &ta_angle_error_x, &ta_angle_error_y, &ta_angle_error_z);

        /* 'KalmanFilterMeasurement:1285' kfState.X(7,1) = kfState.X(7,1) + (ta_angle_error.x); */
        kfState_X[6] = rtU_NavigationState->NAV_KalmanFilterState.X[6] +
          ta_angle_error_x;

        /* 'KalmanFilterMeasurement:1286' kfState.X(8,1) = kfState.X(8,1) + (ta_angle_error.y); */
        kfState_X[7] = rtU_NavigationState->NAV_KalmanFilterState.X[7] +
          ta_angle_error_y;

        /* 'KalmanFilterMeasurement:1287' kfState.X(9,1) = kfState.X(9,1) + (ta_angle_error.z); */
        kfState_X[8] = rtU_NavigationState->NAV_KalmanFilterState.X[8] +
          ta_angle_error_z;

        /* 'KalmanFilterMeasurement:1290' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (6 * i);
            b_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              b_a_1[K_K_tmp_tmp] += ((real_T)b_a[(6 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 6; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_a_1[(6 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)h_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (6 * i) + i_0;
            b_a_0[K_K_tmp_tmp] = b_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_GAaZsA6r(b_a_0, tmp_0);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 6; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            b_K_K_tmp_tmp_0[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              b_K_K_tmp_tmp_0[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                h_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_K_K_tmp[kfState_P_tmp_tmp] += b_K_K_tmp_tmp_0
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_0
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:1291' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)b_a[(6 * i) + i_0]) * kfState_X[i];
          }

          b_z_K_0[i_0] = b_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:1292' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 6; i++) {
            baro_pos_e_x += b_K_K_tmp[(i << ((uint64_T)4)) + i_0] * b_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)b_a[(6 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)b_a[(6 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_a_1[kfState_P_tmp_tmp] += b_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * b_RR
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += b_a_1[kfState_P_tmp_tmp + i_0] *
                b_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:1294' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:1295' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1296' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1297' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1298' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

        /* 'KalmanFilterMeasurement:1299' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1300' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = taState_valid;

        /* 'KalmanFilterMeasurement:1301' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1302' if ((taState.time.velocity - time_updateINS) >= kfState.timeupdateINS) */
        if ((rtU_TransferAlignState->time.velocity -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:1303' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1304' else */
          /* 'KalmanFilterMeasurement:1305' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:1307' if ((taState.time.attitude - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((rtU_TransferAlignState->time.attitude -
             NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:1308' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1309' else */
          /* 'KalmanFilterMeasurement:1310' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:1312' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:1313' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1314' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:1315' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:1316' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1321' elseif ((gnssState.valid) && (baroState.valid) && (~taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1323' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:1324' delta_private.nav = [useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; ... */
        /* 'KalmanFilterMeasurement:1325'                              useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; ... */
        /* 'KalmanFilterMeasurement:1326'                              useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; ... */
        /* 'KalmanFilterMeasurement:1327'                              useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; ... */
        /* 'KalmanFilterMeasurement:1328'                              useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; ... */
        /* 'KalmanFilterMeasurement:1329'                              useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; ... */
        /* 'KalmanFilterMeasurement:1330'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1331'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1332'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1333'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1334'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1335'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1336'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1337'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1338'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1339'                              0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForGNSS[0] - gnssState_ecef_pos_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForGNSS[1] - gnssState_ecef_pos_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForGNSS[2] - gnssState_ecef_pos_z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForGNSS[3] - gnssState_ecef_vel_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForGNSS[4] - gnssState_ecef_vel_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForGNSS[5] - gnssState_ecef_vel_z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1340' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1341' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1342' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1343' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1344' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:1345' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:1346' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1347' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:1348' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:1349' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:1350' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1351' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1352' else */
        /* 'KalmanFilterMeasurement:1354' numberMeasurements = (6 + 1); */
        /* 'KalmanFilterMeasurement:1355' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:1356' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:1357' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:1358' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:1359' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:1360' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:1361' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:1362' HH(7,16) = 1.0; */
        /* 'KalmanFilterMeasurement:1365' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&c_RR[0], 0, 49U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1366' RR(1,1) = kfState.R.range; */
        c_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1367' RR(2,2) = kfState.R.range; */
        c_RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1368' RR(3,3) = kfState.R.range; */
        c_RR[16] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1369' RR(4,4) = kfState.R.rrate; */
        c_RR[24] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1370' RR(5,5) = kfState.R.rrate; */
        c_RR[32] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1371' RR(6,6) = kfState.R.rrate; */
        c_RR[40] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1372' RR(7,7) = kfState.R.baroG; */
        c_RR[48] = rtU_NavigationState->NAV_KalmanFilterState.R.baroG;

        /* 'KalmanFilterMeasurement:1375' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:1376' gnss_geo_pos_temp = LIB_ecef2lla(gnssState.ecef_pos); */
        /* 'KalmanFilterMeasurement:1378' z_K(1,1) = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        c_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:1379' z_K(2,1) = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        c_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:1380' z_K(3,1) = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        c_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:1381' z_K(4,1) = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        c_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:1382' z_K(5,1) = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        c_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:1383' z_K(6,1) = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        c_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:1384' z_K(7,1) = useNavDataForGNSS(1,7) - gnss_geo_pos_temp.alt; */
        LIB_ecef2lla(gnssState_ecef_pos_x, gnssState_ecef_pos_y,
                     gnssState_ecef_pos_z, &baro_pos_e_x, &baro_pos_e_y,
                     &baro_pos_e_z);
        c_z_K[6] = useNavDataForGNSS[6] - baro_pos_e_z;

        /* 'KalmanFilterMeasurement:1387' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:1388' kfState.X(7,1) = kfState.X(7,1) + (gnss_angle_error.x); */
        kfState_X[6] = rtU_NavigationState->NAV_KalmanFilterState.X[6] +
          baro_pos_e_x;

        /* 'KalmanFilterMeasurement:1389' kfState.X(8,1) = kfState.X(8,1) + (gnss_angle_error.y); */
        kfState_X[7] = rtU_NavigationState->NAV_KalmanFilterState.X[7] +
          baro_pos_e_y;

        /* 'KalmanFilterMeasurement:1390' kfState.X(9,1) = kfState.X(9,1) + (gnss_angle_error.z); */
        kfState_X[8] = rtU_NavigationState->NAV_KalmanFilterState.X[8] +
          baro_pos_e_z;

        /* 'KalmanFilterMeasurement:1393' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (7 * i);
            c_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              c_a_1[K_K_tmp_tmp] += ((real_T)c_a[(7 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 7; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_a_1[(7 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)i_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (7 * i) + i_0;
            c_a_0[K_K_tmp_tmp] = c_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_dM5u0dSj(c_a_0, tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 7; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            c_K_K_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              c_K_K_tmp_1[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                i_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp[kfState_P_tmp_tmp] += c_K_K_tmp_1
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_1
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:1394' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 7; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)c_a[(7 * i) + i_0]) * kfState_X[i];
          }

          c_z_K_0[i_0] = c_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:1395' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 7; i++) {
            baro_pos_e_x += c_K_K_tmp[(i << ((uint64_T)4)) + i_0] * c_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)c_a[(7 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += c_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)c_a[(7 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 7; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            c_K_K_tmp_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              c_K_K_tmp_1[kfState_P_tmp_tmp] += c_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * c_RR
                [(7 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 7; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += c_K_K_tmp_1[kfState_P_tmp_tmp + i_0]
                * c_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:1397' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:1398' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1399' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1400' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1401' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1402' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
          baroState_valid;

        /* 'KalmanFilterMeasurement:1403' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:1404' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1405' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:1406' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1407' else */
          /* 'KalmanFilterMeasurement:1408' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:1410' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:1411' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1412' else */
          /* 'KalmanFilterMeasurement:1413' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:1415' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:1416' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1417' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:1418' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:1419' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) != 0) && (((int32_T)baroState_valid) ==
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1424' elseif ((gnssState.valid) && (~baroState.valid) && (~taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1426' if ~gnss_valid_private */
      if (((int32_T)NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private) == 0)
      {
        /* 'KalmanFilterMeasurement:1427' delta_private.nav = [useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; ... */
        /* 'KalmanFilterMeasurement:1428'                              useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; ... */
        /* 'KalmanFilterMeasurement:1429'                              useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; ... */
        /* 'KalmanFilterMeasurement:1430'                              useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; ... */
        /* 'KalmanFilterMeasurement:1431'                              useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; ... */
        /* 'KalmanFilterMeasurement:1432'                              useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; ... */
        /* 'KalmanFilterMeasurement:1433'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1434'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1435'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1436'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1437'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1438'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1439'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1440'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1441'                              0.0; ... */
        /* 'KalmanFilterMeasurement:1442'                              0.0]; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0] =
          useNavDataForGNSS[0] - gnssState_ecef_pos_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[1] =
          useNavDataForGNSS[1] - gnssState_ecef_pos_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[2] =
          useNavDataForGNSS[2] - gnssState_ecef_pos_z;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[3] =
          useNavDataForGNSS[3] - gnssState_ecef_vel_x;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[4] =
          useNavDataForGNSS[4] - gnssState_ecef_vel_y;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[5] =
          useNavDataForGNSS[5] - gnssState_ecef_vel_z;
        memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[6], 0, 10U *
               (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1443' delta_private.kalmanfilter.state = KalmanFilterState_private - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1444' delta_private.kalmanfilter.covariance = KalmanFilterCovariance_private - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] =
            NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterCovariance_private[i_0]
            - rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1445' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1446' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1447' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1448' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:1449' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1450' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

        /* 'KalmanFilterMeasurement:1451' delta_private.valid.feedback_att = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

        /* 'KalmanFilterMeasurement:1452' delta_private.valid.reset_navigation = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          1U;

        /* 'KalmanFilterMeasurement:1453' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1454' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1455' else */
        /* 'KalmanFilterMeasurement:1457' numberMeasurements = (6); */
        /* 'KalmanFilterMeasurement:1458' HH = zeros(numberMeasurements, 16); */
        /* 'KalmanFilterMeasurement:1459' HH(1,1)  = 1.0; */
        /* 'KalmanFilterMeasurement:1460' HH(2,2)  = 1.0; */
        /* 'KalmanFilterMeasurement:1461' HH(3,3)  = 1.0; */
        /* 'KalmanFilterMeasurement:1462' HH(4,4)  = 1.0; */
        /* 'KalmanFilterMeasurement:1463' HH(5,5)  = 1.0; */
        /* 'KalmanFilterMeasurement:1464' HH(6,6)  = 1.0; */
        /* 'KalmanFilterMeasurement:1467' RR = zeros(numberMeasurements, numberMeasurements); */
        memset(&b_RR[0], 0, 36U * (sizeof(real_T)));

        /* 'KalmanFilterMeasurement:1468' RR(1,1) = kfState.R.range; */
        b_RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1469' RR(2,2) = kfState.R.range; */
        b_RR[7] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1470' RR(3,3) = kfState.R.range; */
        b_RR[14] = rtU_NavigationState->NAV_KalmanFilterState.R.range;

        /* 'KalmanFilterMeasurement:1471' RR(4,4) = kfState.R.rrate; */
        b_RR[21] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1472' RR(5,5) = kfState.R.rrate; */
        b_RR[28] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1473' RR(6,6) = kfState.R.rrate; */
        b_RR[35] = rtU_NavigationState->NAV_KalmanFilterState.R.rrate;

        /* 'KalmanFilterMeasurement:1476' z_K = zeros(numberMeasurements,1); */
        /* 'KalmanFilterMeasurement:1478' z_K(1,1) = useNavDataForGNSS(1,1) - gnssState.ecef_pos.x; */
        b_z_K[0] = useNavDataForGNSS[0] - gnssState_ecef_pos_x;

        /* 'KalmanFilterMeasurement:1479' z_K(2,1) = useNavDataForGNSS(1,2) - gnssState.ecef_pos.y; */
        b_z_K[1] = useNavDataForGNSS[1] - gnssState_ecef_pos_y;

        /* 'KalmanFilterMeasurement:1480' z_K(3,1) = useNavDataForGNSS(1,3) - gnssState.ecef_pos.z; */
        b_z_K[2] = useNavDataForGNSS[2] - gnssState_ecef_pos_z;

        /* 'KalmanFilterMeasurement:1481' z_K(4,1) = useNavDataForGNSS(1,4) - gnssState.ecef_vel.x; */
        b_z_K[3] = useNavDataForGNSS[3] - gnssState_ecef_vel_x;

        /* 'KalmanFilterMeasurement:1482' z_K(5,1) = useNavDataForGNSS(1,5) - gnssState.ecef_vel.y; */
        b_z_K[4] = useNavDataForGNSS[4] - gnssState_ecef_vel_y;

        /* 'KalmanFilterMeasurement:1483' z_K(6,1) = useNavDataForGNSS(1,6) - gnssState.ecef_vel.z; */
        b_z_K[5] = useNavDataForGNSS[5] - gnssState_ecef_vel_z;

        /* 'KalmanFilterMeasurement:1486' gnss_angle_error = GetECEFAttitudeErrorForGNSS(useNavDataForGNSS, navState, gnssState); */
        GetECEFAttitudeErrorForGNSS(useNavDataForGNSS,
          &rtU_NavigationState->NAV_NavigationState, gnssState_ecef_pos_x,
          gnssState_ecef_pos_y, gnssState_ecef_pos_z, gnssState_ecef_vel_x,
          gnssState_ecef_vel_y, gnssState_ecef_vel_z, &baro_pos_e_x,
          &baro_pos_e_y, &baro_pos_e_z);

        /* 'KalmanFilterMeasurement:1487' kfState.X(7,1) = kfState.X(7,1) + (gnss_angle_error.x); */
        kfState_X[6] = rtU_NavigationState->NAV_KalmanFilterState.X[6] +
          baro_pos_e_x;

        /* 'KalmanFilterMeasurement:1488' kfState.X(8,1) = kfState.X(8,1) + (gnss_angle_error.y); */
        kfState_X[7] = rtU_NavigationState->NAV_KalmanFilterState.X[7] +
          baro_pos_e_y;

        /* 'KalmanFilterMeasurement:1489' kfState.X(9,1) = kfState.X(9,1) + (gnss_angle_error.z); */
        kfState_X[8] = rtU_NavigationState->NAV_KalmanFilterState.X[8] +
          baro_pos_e_z;

        /* 'KalmanFilterMeasurement:1492' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          for (i = 0; i < 16; i++) {
            K_K_tmp_tmp = i_0 + (6 * i);
            b_a_1[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              b_a_1[K_K_tmp_tmp] += ((real_T)b_a[(6 *
                useNavDataForTA_attitude_tmp) + i_0]) *
                rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
                + useNavDataForTA_attitude_tmp];
            }
          }

          for (i = 0; i < 6; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 16; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_a_1[(6 * useNavDataForTA_attitude_tmp) + i_0] *
                ((real_T)h_b[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp]);
            }

            K_K_tmp_tmp = (6 * i) + i_0;
            b_a_0[K_K_tmp_tmp] = b_RR[K_K_tmp_tmp] + baro_pos_e_x;
          }
        }

        inv_GAaZsA6r(b_a_0, tmp_0);
        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 6; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            b_K_K_tmp_tmp_0[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              b_K_K_tmp_tmp_0[K_K_tmp_tmp] +=
                rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)
                h_b[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp]);
            }
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_K_K_tmp[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_K_K_tmp[kfState_P_tmp_tmp] += b_K_K_tmp_tmp_0
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * tmp_0
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        /* 'KalmanFilterMeasurement:1493' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
        for (i_0 = 0; i_0 < 6; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 16; i++) {
            baro_pos_e_x += ((real_T)b_a[(6 * i) + i_0]) * kfState_X[i];
          }

          b_z_K_0[i_0] = b_z_K[i_0] - baro_pos_e_x;
        }

        /* 'KalmanFilterMeasurement:1494' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
        eye_iBsVrrYi(kfState_P);
        eye_iBsVrrYi(kfState_P_tmp_1);
        for (i_0 = 0; i_0 < 16; i_0++) {
          baro_pos_e_x = 0.0;
          for (i = 0; i < 6; i++) {
            baro_pos_e_x += b_K_K_tmp[(i << ((uint64_T)4)) + i_0] * b_z_K_0[i];
          }

          for (i = 0; i < 16; i++) {
            gnssState_ecef_pos_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              gnssState_ecef_pos_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i_0] * ((real_T)b_a[(6 * i) +
                useNavDataForTA_attitude_tmp]);
            }

            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4)) + i_0;
            kfState_P_tmp_0[useNavDataForTA_attitude_tmp] =
              kfState_P[useNavDataForTA_attitude_tmp] - gnssState_ecef_pos_x;
          }

          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_2[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_2[K_K_tmp_tmp] += kfState_P_tmp_0[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                rtU_NavigationState->
                NAV_KalmanFilterState.P[useNavDataForTA_attitude_tmp +
                kfState_P_tmp_tmp];
            }
          }

          kfState_X[i_0] += baro_pos_e_x;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            baro_pos_e_x = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              baro_pos_e_x += b_K_K_tmp[(useNavDataForTA_attitude_tmp <<
                ((uint64_T)4)) + i] * ((real_T)b_a[(6 * i_0) +
                useNavDataForTA_attitude_tmp]);
            }

            kfState_P[i_0 + (i << ((uint64_T)4))] = kfState_P_tmp_1[(i_0 <<
              ((uint64_T)4)) + i] - baro_pos_e_x;
          }

          for (i = 0; i < 6; i++) {
            kfState_P_tmp_tmp = i_0 + (i << ((uint64_T)4));
            b_a_1[kfState_P_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              b_a_1[kfState_P_tmp_tmp] += b_K_K_tmp
                [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] * b_RR
                [(6 * i) + useNavDataForTA_attitude_tmp];
            }
          }
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          for (i = 0; i < 16; i++) {
            useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
            K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
            kfState_P_tmp_1[K_K_tmp_tmp] = 0.0;
            for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16;
                 kfState_P_tmp_tmp++) {
              kfState_P_tmp_1[K_K_tmp_tmp] += kfState_P_tmp_2[(kfState_P_tmp_tmp
                << ((uint64_T)4)) + i_0] *
                kfState_P[useNavDataForTA_attitude_tmp + kfState_P_tmp_tmp];
            }

            kfState_P_tmp[K_K_tmp_tmp] = 0.0;
            for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
                 6; useNavDataForTA_attitude_tmp++) {
              kfState_P_tmp_tmp = (useNavDataForTA_attitude_tmp << ((uint64_T)4));
              kfState_P_tmp[K_K_tmp_tmp] += b_a_1[kfState_P_tmp_tmp + i_0] *
                b_K_K_tmp[kfState_P_tmp_tmp + i];
            }
          }
        }

        for (i_0 = 0; i_0 < 256; i_0++) {
          kfState_P[i_0] = kfState_P_tmp_1[i_0] + kfState_P_tmp[i_0];
        }

        /* 'KalmanFilterMeasurement:1496' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
        /* 'KalmanFilterMeasurement:1497' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
        for (i_0 = 0; i_0 < 16; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
            kfState_X[i_0] *
            rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0]
            = kfState_X[i_0] - rtU_NavigationState->NAV_KalmanFilterState.X[i_0];
        }

        /* 'KalmanFilterMeasurement:1498' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
        for (i_0 = 0; i_0 < 256; i_0++) {
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
            [i_0] = kfState_P[i_0] -
            rtU_NavigationState->NAV_KalmanFilterState.P[i_0];
        }

        /* 'KalmanFilterMeasurement:1499' delta_private.numbersatellites = gnssState.numberSatellites; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites =
          gnssState_numberSatellites;

        /* 'KalmanFilterMeasurement:1500' delta_private.valid.gnss = gnssState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = gnss_Valid;

        /* 'KalmanFilterMeasurement:1501' delta_private.valid.baro = baroState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

        /* 'KalmanFilterMeasurement:1502' delta_private.valid.ta   = taState.valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

        /* 'KalmanFilterMeasurement:1503' delta_private.valid.mag  = magState.Valid; */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

        /* 'KalmanFilterMeasurement:1504' if ((gnssState.time - time_updateINS) >= kfState.timeupdateINS) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateINS) {
          /* 'KalmanFilterMeasurement:1505' delta_private.valid.feedback_nav = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1506' else */
          /* 'KalmanFilterMeasurement:1507' delta_private.valid.feedback_nav = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav =
            0U;
        }

        /* 'KalmanFilterMeasurement:1509' if ((gnssState.time - time_updateINS) >= kfState.timeupdateAttitude) */
        if ((gnssState_time - NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS) >=
            rtU_NavigationState->NAV_KalmanFilterState.timeupdateAttitude) {
          /* 'KalmanFilterMeasurement:1510' delta_private.valid.feedback_att = uint8(1); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            1U;
        } else {
          /* 'KalmanFilterMeasurement:1511' else */
          /* 'KalmanFilterMeasurement:1512' delta_private.valid.feedback_att = uint8(0); */
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att =
            0U;
        }

        /* 'KalmanFilterMeasurement:1514' delta_private.valid.reset_navigation = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
          0U;

        /* 'KalmanFilterMeasurement:1515' delta_private.trade = delta_private.trade + 1; */
        magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade
          + /*MW:OvSatOk*/ 1U;
        if (magOutput_Valid <
            NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
          magOutput_Valid = MAX_uint32_T;
        }

        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

        /* 'KalmanFilterMeasurement:1516' KalmanFilterState_private = kfState.X; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.KalmanFilterState_private[0],
               &kfState_X[0], (sizeof(real_T)) << 4U);

        /* 'KalmanFilterMeasurement:1517' KalmanFilterCovariance_private = kfState.P; */
        memcpy(&NAV_DoNavigationMain_2Hz_M->
               dwork.KalmanFilterCovariance_private[0], &kfState_P[0], (sizeof
                (real_T)) << 8U);

        /* 'KalmanFilterMeasurement:1518' gnss_valid_private = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 1U;
      }
    } else if ((((((int32_T)gnss_Valid) == 0) && (((int32_T)baroState_valid) !=
                  0)) && (((int32_T)taState_valid) == 0)) && (magOutput_Valid ==
                0U)) {
      /* 'KalmanFilterMeasurement:1523' elseif ((~gnssState.valid) && (baroState.valid) && (~taState.valid) && (~magState.Valid)) */
      /* 'KalmanFilterMeasurement:1525' baro_pos_g.lat = navState.pos_g.lat; */
      /* 'KalmanFilterMeasurement:1526' baro_pos_g.lon = navState.pos_g.lon; */
      /* 'KalmanFilterMeasurement:1527' baro_pos_g.alt = navState.baro_alt; */
      /* 'KalmanFilterMeasurement:1528' baro_pos_e = LIB_lla2ecef(baro_pos_g); */
      LIB_lla2ecef(rtU_NavigationState->NAV_NavigationState.pos_g.lat,
                   rtU_NavigationState->NAV_NavigationState.pos_g.lon,
                   rtU_NavigationState->NAV_NavigationState.baro_alt,
                   &baro_pos_e_x, &baro_pos_e_y, &baro_pos_e_z);

      /* 'KalmanFilterMeasurement:1529' HH = zeros(3,16); */
      /* 'KalmanFilterMeasurement:1530' HH(1,1) = 1.0; */
      /* 'KalmanFilterMeasurement:1531' HH(2,2) = 1.0; */
      /* 'KalmanFilterMeasurement:1532' HH(3,3) = 1.0; */
      /* 'KalmanFilterMeasurement:1533' RR = zeros(3,3); */
      memset(&RR[0], 0, 9U * (sizeof(real_T)));

      /* 'KalmanFilterMeasurement:1534' if ((double(navState.GuidCtrlFlags.PstaticValid) >= 1.0) && (double(navState.GuidCtrlFlags.PstaticValid) <= 9.0)) */
      if ((((int32_T)
            rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid)
           >= 1) && (((int32_T)
                      rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid)
                     <= 9)) {
        /* 'KalmanFilterMeasurement:1535' RR(1,1) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        RR[0] = (5.0 - ((5.0 * ((real_T)
                    rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid))
                        / 10.0)) *
          rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:1536' RR(2,2) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        RR[4] = (5.0 - ((5.0 * ((real_T)
                    rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid))
                        / 10.0)) *
          rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:1537' RR(3,3) = kfState.R.baro_P_N * (5.0 - 5.0*double(navState.GuidCtrlFlags.PstaticValid)/10.0); */
        RR[8] = (5.0 - ((5.0 * ((real_T)
                    rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid))
                        / 10.0)) *
          rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;
      } else {
        /* 'KalmanFilterMeasurement:1538' else */
        /* 'KalmanFilterMeasurement:1539' RR(1,1) = kfState.R.baro_P_N; */
        RR[0] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:1540' RR(2,2) = kfState.R.baro_P_N; */
        RR[4] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;

        /* 'KalmanFilterMeasurement:1541' RR(3,3) = kfState.R.baro_P_N; */
        RR[8] = rtU_NavigationState->NAV_KalmanFilterState.R.baro_P_N;
      }

      /* 'KalmanFilterMeasurement:1543' z_K = zeros(3,1); */
      /* 'KalmanFilterMeasurement:1544' z_K(1,1) = navState.pos_e.x - baro_pos_e.x; */
      z_K[0] = rtU_NavigationState->NAV_NavigationState.pos_e.x - baro_pos_e_x;

      /* 'KalmanFilterMeasurement:1545' z_K(2,1) = navState.pos_e.y - baro_pos_e.y; */
      z_K[1] = rtU_NavigationState->NAV_NavigationState.pos_e.y - baro_pos_e_y;

      /* 'KalmanFilterMeasurement:1546' z_K(3,1) = navState.pos_e.z - baro_pos_e.z; */
      z_K[2] = rtU_NavigationState->NAV_NavigationState.pos_e.z - baro_pos_e_z;

      /* 'KalmanFilterMeasurement:1549' K_K = kfState.P*HH'*(inv(HH*kfState.P*HH' + RR)); */
      for (i_0 = 0; i_0 < 48; i_0++) {
        K_K_tmp[i_0] = (real_T)g_b[i_0];
        K_K_tmp_0[i_0] = a[i_0];
      }

      for (i_0 = 0; i_0 < 3; i_0++) {
        for (i = 0; i < 16; i++) {
          K_K_tmp_tmp = i_0 + (3 * i);
          K_K_tmp_3[K_K_tmp_tmp] = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            K_K_tmp_3[K_K_tmp_tmp] += ((real_T)K_K_tmp_0[(3 *
              useNavDataForTA_attitude_tmp) + i_0]) *
              rtU_NavigationState->NAV_KalmanFilterState.P[(i << ((uint64_T)4))
              + useNavDataForTA_attitude_tmp];
          }
        }

        for (i = 0; i < 3; i++) {
          baro_pos_e_x = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            baro_pos_e_x += K_K_tmp_3[(3 * useNavDataForTA_attitude_tmp) + i_0] *
              K_K_tmp[(i << ((uint64_T)4)) + useNavDataForTA_attitude_tmp];
          }

          K_K_tmp_tmp = (3 * i) + i_0;
          K_K_tmp_2[K_K_tmp_tmp] = RR[K_K_tmp_tmp] + baro_pos_e_x;
        }
      }

      inv_h7eIJkDj(K_K_tmp_2, tmp);
      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 3; i++) {
          useNavDataForTA_attitude_tmp = (i << ((uint64_T)4));
          K_K_tmp_tmp = i_0 + useNavDataForTA_attitude_tmp;
          K_K_tmp_3[K_K_tmp_tmp] = 0.0;
          for (kfState_P_tmp_tmp = 0; kfState_P_tmp_tmp < 16; kfState_P_tmp_tmp
               ++) {
            K_K_tmp_3[K_K_tmp_tmp] +=
              rtU_NavigationState->NAV_KalmanFilterState.P[(kfState_P_tmp_tmp <<
              ((uint64_T)4)) + i_0] * K_K_tmp[useNavDataForTA_attitude_tmp +
              kfState_P_tmp_tmp];
          }
        }

        for (i = 0; i < 3; i++) {
          K_K_tmp_tmp = i_0 + (i << ((uint64_T)4));
          K_K_tmp_1[K_K_tmp_tmp] = 0.0;
          K_K_tmp_1[K_K_tmp_tmp] += tmp[3 * i] * K_K_tmp_3[i_0];
          K_K_tmp_1[K_K_tmp_tmp] += tmp[(3 * i) + 1] * K_K_tmp_3[i_0 + 16];
          K_K_tmp_1[K_K_tmp_tmp] += tmp[(3 * i) + 2] * K_K_tmp_3[i_0 + 32];
        }
      }

      /* 'KalmanFilterMeasurement:1550' kfState.X = kfState.X + K_K*(z_K - HH*kfState.X); */
      for (i_0 = 0; i_0 < 3; i_0++) {
        baro_pos_e_x = 0.0;
        for (i = 0; i < 16; i++) {
          baro_pos_e_x += ((real_T)K_K_tmp_0[(3 * i) + i_0]) *
            rtU_NavigationState->NAV_KalmanFilterState.X[i];
        }

        magOutput_Attitude[i_0] = z_K[i_0] - baro_pos_e_x;
      }

      /* 'KalmanFilterMeasurement:1551' kfState.P = (eye(16) - K_K*HH)*kfState.P*(eye(16) - K_K*HH)' + K_K*(RR)*K_K'; */
      /* 'KalmanFilterMeasurement:1553' delta_private.nav = kfState.X*kfState.FeedbackWeight; */
      /* 'KalmanFilterMeasurement:1554' delta_private.kalmanfilter.state = kfState.X - KalmanFilterState_privateIn; */
      /* 'KalmanFilterMeasurement:1555' delta_private.kalmanfilter.covariance = kfState.P - KalmanFilterCovariance_privateIn; */
      eye_iBsVrrYi(kfState_P_tmp);
      for (i_0 = 0; i_0 < 16; i_0++) {
        gnssState_time = K_K_tmp_1[i_0 + 16];
        gnssState_ecef_pos_x = K_K_tmp_1[i_0 + 32];
        gnssState_ecef_pos_y = (((gnssState_time * magOutput_Attitude[1]) +
          (K_K_tmp_1[i_0] * magOutput_Attitude[0])) + (gnssState_ecef_pos_x *
          magOutput_Attitude[2])) + rtU_NavigationState->
          NAV_KalmanFilterState.X[i_0];
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[i_0] =
          gnssState_ecef_pos_y *
          rtU_NavigationState->NAV_KalmanFilterState.FeedbackWeight;
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.state[i_0] =
          gnssState_ecef_pos_y - rtU_NavigationState->
          NAV_KalmanFilterState.X[i_0];
        for (i = 0; i < 16; i++) {
          useNavDataForTA_attitude_tmp = i_0 + (i << ((uint64_T)4));
          kfState_P[useNavDataForTA_attitude_tmp] = 0.0;
          kfState_P[useNavDataForTA_attitude_tmp] += ((real_T)K_K_tmp_0[3 * i]) *
            K_K_tmp_1[i_0];
          kfState_P[useNavDataForTA_attitude_tmp] += ((real_T)K_K_tmp_0[(3 * i)
            + 1]) * gnssState_time;
          kfState_P[useNavDataForTA_attitude_tmp] += ((real_T)K_K_tmp_0[(3 * i)
            + 2]) * gnssState_ecef_pos_x;
        }
      }

      for (i_0 = 0; i_0 < 256; i_0++) {
        kfState_P_tmp_0[i_0] = kfState_P_tmp[i_0] - kfState_P[i_0];
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 16; i++) {
          K_K_tmp_tmp = (i << ((uint64_T)4));
          kfState_P_tmp_tmp = i_0 + K_K_tmp_tmp;
          kfState_P_tmp_1[kfState_P_tmp_tmp] = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            kfState_P_tmp_1[kfState_P_tmp_tmp] += kfState_P_tmp_0
              [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
              rtU_NavigationState->NAV_KalmanFilterState.P[K_K_tmp_tmp +
              useNavDataForTA_attitude_tmp];
          }

          kfState_P_tmp_2[i + (i_0 << ((uint64_T)4))] =
            kfState_P_tmp[kfState_P_tmp_tmp] - kfState_P[kfState_P_tmp_tmp];
        }

        for (i = 0; i < 3; i++) {
          K_K_tmp_tmp = i_0 + (i << ((uint64_T)4));
          K_K_tmp[K_K_tmp_tmp] = 0.0;
          K_K_tmp[K_K_tmp_tmp] += RR[3 * i] * K_K_tmp_1[i_0];
          K_K_tmp[K_K_tmp_tmp] += RR[(3 * i) + 1] * K_K_tmp_1[i_0 + 16];
          K_K_tmp[K_K_tmp_tmp] += RR[(3 * i) + 2] * K_K_tmp_1[i_0 + 32];
        }
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        for (i = 0; i < 16; i++) {
          K_K_tmp_tmp = (i << ((uint64_T)4));
          kfState_P_tmp_tmp = i_0 + K_K_tmp_tmp;
          kfState_P_tmp[kfState_P_tmp_tmp] = 0.0;
          for (useNavDataForTA_attitude_tmp = 0; useNavDataForTA_attitude_tmp <
               16; useNavDataForTA_attitude_tmp++) {
            kfState_P_tmp[kfState_P_tmp_tmp] += kfState_P_tmp_1
              [(useNavDataForTA_attitude_tmp << ((uint64_T)4)) + i_0] *
              kfState_P_tmp_2[K_K_tmp_tmp + useNavDataForTA_attitude_tmp];
          }

          kfState_P_tmp_0[kfState_P_tmp_tmp] = 0.0;
          kfState_P_tmp_0[kfState_P_tmp_tmp] += K_K_tmp[i_0] * K_K_tmp_1[i];
          kfState_P_tmp_0[kfState_P_tmp_tmp] += K_K_tmp[i_0 + 16] * K_K_tmp_1[i
            + 16];
          kfState_P_tmp_0[kfState_P_tmp_tmp] += K_K_tmp[i_0 + 32] * K_K_tmp_1[i
            + 32];
        }
      }

      for (i_0 = 0; i_0 < 256; i_0++) {
        NAV_DoNavigationMain_2Hz_M->
          dwork.delta_private.kalmanfilter.covariance[i_0] = (kfState_P_tmp[i_0]
          + kfState_P_tmp_0[i_0]) - rtU_NavigationState->
          NAV_KalmanFilterState.P[i_0];
      }

      /* 'KalmanFilterMeasurement:1556' delta_private.numbersatellites = uint16(0.0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites = 0U;

      /* 'KalmanFilterMeasurement:1557' delta_private.valid.gnss = gnssState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

      /* 'KalmanFilterMeasurement:1558' delta_private.valid.baro = baroState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro =
        baroState_valid;

      /* 'KalmanFilterMeasurement:1559' delta_private.valid.ta   = taState.valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

      /* 'KalmanFilterMeasurement:1560' delta_private.valid.mag  = magState.Valid; */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

      /* 'KalmanFilterMeasurement:1561' if (double(navState.GuidCtrlFlags.PstaticValid) > 0.0) */
      if (((int32_T)
           rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.PstaticValid) >
          0) {
        /* 'KalmanFilterMeasurement:1562' delta_private.valid.feedback_nav = uint8(1); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 1U;
      } else {
        /* 'KalmanFilterMeasurement:1563' else */
        /* 'KalmanFilterMeasurement:1564' delta_private.valid.feedback_nav = uint8(0); */
        NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;
      }

      /* 'KalmanFilterMeasurement:1566' delta_private.valid.feedback_att = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

      /* 'KalmanFilterMeasurement:1567' delta_private.valid.reset_navigation = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
        0U;

      /* 'KalmanFilterMeasurement:1568' delta_private.trade = delta_private.trade + 1; */
      magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade +
        /*MW:OvSatOk*/ 1U;
      if (magOutput_Valid <
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
        magOutput_Valid = MAX_uint32_T;
      }

      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

      /* 'KalmanFilterMeasurement:1569' gnss_valid_private = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 0U;

      /* 'KalmanFilterMeasurement:1570' time_updateINS = 0.0; */
      NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS = 0.0;
    } else {
      /* 'KalmanFilterMeasurement:1573' else */
      /* 'KalmanFilterMeasurement:1574' delta_private.nav = zeros(16,1); */
      /* 'KalmanFilterMeasurement:1575' delta_private.kalmanfilter.state = zeros(16,1); */
      memset(&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.nav[0], 0, (sizeof
              (real_T)) << 4U);
      memset(&NAV_DoNavigationMain_2Hz_M->
             dwork.delta_private.kalmanfilter.state[0], 0, (sizeof(real_T)) <<
             4U);

      /* 'KalmanFilterMeasurement:1576' delta_private.kalmanfilter.covariance = zeros(16,16); */
      memset
        (&NAV_DoNavigationMain_2Hz_M->dwork.delta_private.kalmanfilter.covariance
         [0], 0, (sizeof(real_T)) << 8U);

      /* 'KalmanFilterMeasurement:1577' delta_private.numbersatellites = uint16(0.0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.numbersatellites = 0U;

      /* 'KalmanFilterMeasurement:1578' delta_private.valid.gnss = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.gnss = 0U;

      /* 'KalmanFilterMeasurement:1579' delta_private.valid.baro = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.baro = 0U;

      /* 'KalmanFilterMeasurement:1580' delta_private.valid.ta   = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.ta = 0U;

      /* 'KalmanFilterMeasurement:1581' delta_private.valid.mag  = uint32(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.mag = 0U;

      /* 'KalmanFilterMeasurement:1582' delta_private.valid.feedback_nav = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

      /* 'KalmanFilterMeasurement:1583' delta_private.valid.feedback_att = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;

      /* 'KalmanFilterMeasurement:1584' delta_private.valid.reset_navigation = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
        0U;

      /* 'KalmanFilterMeasurement:1585' delta_private.trade = delta_private.trade + 1; */
      magOutput_Valid = NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade +
        /*MW:OvSatOk*/ 1U;
      if (magOutput_Valid <
          NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade) {
        magOutput_Valid = MAX_uint32_T;
      }

      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.trade = magOutput_Valid;

      /* 'KalmanFilterMeasurement:1586' gnss_valid_private = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.gnss_valid_private = 0U;

      /* 'KalmanFilterMeasurement:1587' time_updateINS = 0.0; */
      NAV_DoNavigationMain_2Hz_M->dwork.time_updateINS = 0.0;
    }

    /* 'KalmanFilterMeasurement:1591' if (bitget(navState.GuidCtrlFlags.NavCtrlFlag,1)) */
    magOutput_Valid = ((uint32_T)
                       rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.NavCtrlFlag)
      & 1U;
    if (magOutput_Valid != 0U) {
      /* 'KalmanFilterMeasurement:1592' delta_private.valid.feedback_att = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;
    }

    /* 'KalmanFilterMeasurement:1595' if ((bitget(navState.GuidCtrlFlags.NavCtrlFlag,1)) && (bitget(navState.GuidCtrlFlags.NavCtrlFlag,2))) */
    if ((magOutput_Valid != 0U) && ((((uint32_T)
           rtU_NavigationState->NAV_NavigationState.GuidCtrlFlags.NavCtrlFlag) &
          2U) != 0U)) {
      /* 'KalmanFilterMeasurement:1596' delta_private.valid.reset_navigation = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.reset_navigation =
        0U;

      /* 'KalmanFilterMeasurement:1597' delta_private.valid.feedback_nav = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_nav = 0U;

      /* 'KalmanFilterMeasurement:1598' delta_private.valid.feedback_att = uint8(0); */
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private.valid.feedback_att = 0U;
    }

    /* Outport: '<Root>/KalmanFilterErrorEstimatesOut' incorporates:
     *  Inport: '<Root>/TransferAlignState'
     *  Outport: '<Root>/StoreInfo'
     */
    /* 'KalmanFilterMeasurement:1601' delta = delta_private; */
    *rtY_KalmanFilterErrorEstimatesOut =
      NAV_DoNavigationMain_2Hz_M->dwork.delta_private;
  } else {
    /* Outport: '<Root>/StoreInfo' */
    /* 'NAV_Embedded_DoNavigationMain2Hz:52' else */
    /* 'NAV_Embedded_DoNavigationMain2Hz:53' StoreInfo                  = NAV_CreateStruct_T_NAV_StoreInfo; */
    rtY_StoreInfo->TimeReq = 0U;
    rtY_StoreInfo->Case = 0U;
    rtY_StoreInfo->IdxLow = 0U;
    rtY_StoreInfo->IdxHi = 0U;
    rtY_StoreInfo->Reserved1 = 0U;
    rtY_StoreInfo->TimeLow = 0U;
    rtY_StoreInfo->TimeHi = 0U;
    rtY_StoreInfo->Pos[0] = 0;
    rtY_StoreInfo->Pos[1] = 0;
    rtY_StoreInfo->Pos[2] = 0;
    rtY_StoreInfo->Reserved2 = 0U;

    /* 'NAV_Embedded_DoNavigationMain2Hz:54' KalmanFilterErrorEstimates = init_KalmanFilterErrorEstimates ; */
    init_KalmanFilterErrorEstimates(rtY_KalmanFilterErrorEstimatesOut->nav,
      rtY_KalmanFilterErrorEstimatesOut->kalmanfilter.state,
      rtY_KalmanFilterErrorEstimatesOut->kalmanfilter.covariance,
      &rtY_KalmanFilterErrorEstimatesOut->numbersatellites,
      &rtY_KalmanFilterErrorEstimatesOut->Reserved,
      &rtY_KalmanFilterErrorEstimatesOut->valid.gnss,
      &rtY_KalmanFilterErrorEstimatesOut->valid.baro,
      &rtY_KalmanFilterErrorEstimatesOut->valid.ta,
      &rtY_KalmanFilterErrorEstimatesOut->valid.mag,
      &rtY_KalmanFilterErrorEstimatesOut->valid.feedback_nav,
      &rtY_KalmanFilterErrorEstimatesOut->valid.feedback_att,
      &rtY_KalmanFilterErrorEstimatesOut->valid.reset_navigation,
      &rtY_KalmanFilterErrorEstimatesOut->trade);
  }

  /* End of MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_2Hz_at_outport_1' */
}

/* Model initialize function */
void NAV_DoNavigationMain_2Hz_initialize(RT_MODEL *const
  NAV_DoNavigationMain_2Hz_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    static const real_T tmp[169] = { 0.0, 4652.9, -5181.60319592305,
      -251.685071070972, 1560.5840252931, 484.945927236996, -361.049682566333,
      -1823.13762061753, 563.0625, -2968.50077817038, 827.172650944296, 0.0,
      -1076.4329539023, -29404.5, -1450.7, -636.355466700805, 468.243686556477,
      -619.838043362942, 1601.59840471949, 373.605806737696, -486.54160793297,
      -858.060221275268, 1206.01546192791, -42.1383840607929, 1063.52473077468,
      397.564930348733, -3750.0, 5164.97550817039, 1452.15139706575,
      -429.200135426353, 417.911683253771, -570.863594354531, 525.040693735376,
      47.100285814718, 530.170538600553, 813.231831936986, 578.481191607981,
      -163.984004048895, 843.987056448926, 3409.75, -7290.29384695843,
      2393.8910062908, 415.602341487629, -258.902441507897, 71.4366633809278,
      -351.421917892154, 290.19987555724, -315.487391490457, -287.516265695614,
      560.980077771261, -95.8055872934383, -876.448097081577, 3951.0625,
      4479.2081917455, 337.310854405843, -647.156530524107, 35.422527701309,
      69.5246713179933, 20.9413242776096, -13.5838239622538, 220.975735398178,
      -208.88387566, -635.674291771916, 95.0541593208478, 33.4021352445188,
      -1845.9, 3691.48566414577, 1443.28301538541, -662.164119750062,
      -335.441723701748, 9.61138241227556, 45.7423130068731, -65.8736290179917,
      24.7148187455421, 135.703858482474, -4.13200851148558, -18.8235284602502,
      145.809237083622, 951.43125, 1240.04498305505, 1090.92895567407,
      -1210.48281383014, -197.53840726236, 31.4119864164144, -43.4585558229763,
      -1.22979371364672, -17.2971036570252, 3.0134099701895, -84.1813784270426,
      -84.3274000084731, -11.705388227149, 2161.0875, -2724.0655498721,
      -240.374722966884, 1157.02876023112, 195.113107821463, 39.5165787992837,
      -17.4371370929978, 6.34314652301995, 1.75477863187212, -3.87596659756372,
      -27.8208269114018, -36.416060909817, 35.116164681447, 1186.453125,
      656.90625, -981.441429563214, -16.5678293312673, -564.134233936326,
      226.907969905512, 94.0536157816464, -41.3626391798429, -0.188011996272013,
      5.90777910410258, -0.265478475211798, -26.4447745196618, 5.10865024664336,
      474.8046875, 1044.70842836898, 315.085120683867, -116.175975990998,
      -62.0133122088579, -448.089604238386, 19.1377236321437, 67.0483718367164,
      -24.0309929048951, -7.24768776688873, -5.22392567080184, -5.44068972983464,
      -8.48922357264211, -342.808984375, -1508.37365760431, -21.0691920303964,
      280.976578781019, -105.183764582111, 44.3493691933895, -37.1880766033702,
      38.0820521455669, 11.4556346105772, -6.37148340508315, -2.31514887683264,
      -1.50794663223017, 0.0, 1033.34765625, -652.941025700099,
      -1022.61993343719, 787.123219434697, -215.562571410236, 47.5270796604239,
      -65.8823496108755, -4.96043529461606, 31.8640532960898, -5.28895490393235,
      0.544068972983464, 1.7979363691975, 0.283884006063428, -1320.388671875,
      -89.7027461585248, 397.564930348733, 843.987056448926, -584.298731387718,
      233.814946711632, 62.489673035838, 58.5269411357451, -11.705388227149,
      -12.7716256166084, 0.943247063626901, -3.05963222836729,
      -0.170330403638057 };

    static const real_T tmp_0[169] = { 0.0, -25.1, -52.3079343885801,
      17.4526144173301, 1.10679718105893, 1.01665812837945, 1.89031247416928,
      17.7348017569798, -20.109375, -38.2210400622796, 0.0, 0.0, 0.0, 6.7, 7.7,
      -20.6980071504481, -1.93649167310371, 27.00052082831, 19.2130326861742,
      -26.8996180851141, 17.3764859976061, 39.2576571825286, 21.7300083230253,
      21.0691920303964, 40.9047973374878, 0.0, -17.25, -12.297560733739,
      -1.90525588832577, 0.869626356546304, 7.7391052454402, -4.23559138432876,
      -13.9479501182073, -14.3348695957837, -8.28391466563363, -33.193135997428,
      -49.5841021378269, 0.0, -64.922081265302, 7.0, -18.9835455065696,
      6.58407168855261, -9.64494686351356, -4.14125584816973, 6.65558975598707,
      4.91117587116365, -2.46978617495523, 13.3681098089177, 22.5502953486756,
      11.6870849535679, 47.9027936467191, 48.6915609489765, -4.8125,
      -8.85437744847146, -23.4787137637478, 11.29491035821, -4.06730485088099,
      0.350780380010057, 0.232681380862329, -7.4093585248657, -4.44917588050023,
      3.36909476870967, -14.7831230644632, 0.0, 0.0, -2.3625, 6.09994877027668,
      -5.37964915212879, 0.470621264925417, 2.66223590239483, 0.701560760020114,
      0.671693289381396, 0.484364919249939, -3.43261371465863, 0.0,
      4.13200851148558, 0.0, 0.0, -8.6625, -7.56124989667714, 7.47211613475393,
      13.9479501182073, -7.639606910699, 0.0, 0.537354631505117,
      0.194177954786325, 1.00273064678407, -1.50670498509475, 0.0,
      4.96043529461606, 0.0, -2.68125, -10.6408810541879, -2.89608099960101,
      14.3348695957837, 2.46978617495523, -3.08723271869404, -1.93745967699976,
      0.64725984928775, 0.0626706654240044, 1.29198886585457, -0.81825961504123,
      0.0, 5.85269411357451, -5.02734375, 6.703125, -5.60823674036123,
      20.7097866640841, -2.67362196178354, 5.93223450733364, 3.43261371465863,
      0.0, 0.250682661696018, 0.121809878435105, 0.530956950423596,
      -0.881492483988725, 0.0, -9.49609375, -25.4806933748531, 0.0,
      33.193135997428, -16.9127215115067, 0.0, 5.21937917240283, 0.0, 0.0,
      -0.243619756870209, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 33.0560680918846,
      -11.6870849535679, -14.7831230644632, 0.0, -2.0043185339772,
      -1.63651923008246, -0.265478475211798, 0.0, 0.0, 0.0, 0.0,
      -46.6386446928642, 0.0, 0.0, 0.0, -15.842359886808, 0.0, 0.0,
      -2.27600380686356, -0.881492483988725, -0.272034486491732,
      -0.0579979473934679, -0.0567768012126856, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.0567768012126856 };

    static const int8_T tmp_1[13] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    };

    static const real_T tmp_2[169] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.333333333333333, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.266666666666667, 0.2, 0.0, -0.333333333333333, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.257142857142857,
      0.228571428571429, 0.142857142857143, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.253968253968254, 0.238095238095238, 0.19047619047619,
      0.111111111111111, 0.0, -0.142857142857143, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.252525252525253, 0.242424242424242, 0.212121212121212,
      0.161616161616162, 0.0909090909090909, 0.0, -0.111111111111111, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.251748251748252, 0.244755244755245,
      0.223776223776224, 0.188811188811189, 0.13986013986014, 0.0769230769230769,
      0.0, -0.0909090909090909, 0.0, 0.0, 0.0, 0.0, 0.0, 0.251282051282051,
      0.246153846153846, 0.230769230769231, 0.205128205128205, 0.169230769230769,
      0.123076923076923, 0.0666666666666667, 0.0, -0.0769230769230769, 0.0, 0.0,
      0.0, 0.0, 0.250980392156863, 0.247058823529412, 0.235294117647059,
      0.215686274509804, 0.188235294117647, 0.152941176470588, 0.109803921568627,
      0.0588235294117647, 0.0, -0.0666666666666667, 0.0, 0.0, 0.0,
      0.25077399380805, 0.247678018575851, 0.238390092879257, 0.222910216718266,
      0.201238390092879, 0.173374613003096, 0.139318885448916,
      0.0990712074303406, 0.0526315789473684, 0.0, -0.0588235294117647, 0.0, 0.0,
      0.25062656641604, 0.24812030075188, 0.240601503759398, 0.228070175438596,
      0.210526315789474, 0.18796992481203, 0.160401002506266, 0.12781954887218,
      0.0902255639097744, 0.0476190476190476, 0.0, -0.0526315789473684, 0.0,
      0.250517598343685, 0.248447204968944, 0.242236024844721, 0.231884057971014,
      0.217391304347826, 0.198757763975155, 0.175983436853002, 0.149068322981366,
      0.118012422360248, 0.082815734989648, 0.0434782608695652, 0.0,
      -0.0476190476190476 };

    int32_T i;

    /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_2Hz_at_outport_1' incorporates:
     *  SubSystem: '<Root>/Subsystem'
     */
    /* SystemInitialize for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
    memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.c[0], &tmp[0], 169U * (sizeof
            (real_T)));
    memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.dc[0], &tmp_0[0], 169U * (sizeof
            (real_T)));
    for (i = 0; i < 13; i++) {
      NAV_DoNavigationMain_2Hz_M->dwork.fn[i] = (real_T)tmp_1[i];
    }

    memcpy(&NAV_DoNavigationMain_2Hz_M->dwork.k[0], &tmp_2[0], 169U * (sizeof
            (real_T)));

    /* 'MAG_DoMagnetometerMain2Hz:7' [MAG_State,~,~] = MAG_CreateStructs; */
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.Flags = 0U;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.FieldWMM[0] = 0.0;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.b[0] = 0.0;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.FieldWMM[1] = 0.0;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.b[1] = 0.0;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.FieldWMM[2] = 0.0;
    NAV_DoNavigationMain_2Hz_M->dwork.MAG_State.b[2] = 0.0;

    /* 'MAG_DoMagnetometerMain2Hz:7' ~ */
    /* 'MAG_DoMagnetometerMain2Hz:8' DoMagnetometerBiasCalibration = uint8(1); */
    NAV_DoNavigationMain_2Hz_M->dwork.DoMagnetometerBiasCalibration = 1U;

    /* 'MAG_WorldMagneticModel:38' dyear = 2023.5; */
    /* 'MAG_WorldMagneticModel:40' epoch  = 2020; */
    /* 'MAG_WorldMagneticModel:41' maxdef =   12; */
    /* 'MAG_WorldMagneticModel:43' c  = coder.const(Load_c ); */
    /* 'MAG_WorldMagneticModel:44' dc = coder.const(Load_dc); */
    /* 'MAG_WorldMagneticModel:45' fm = coder.const(Load_fm); */
    for (i = 0; i < 13; i++) {
      NAV_DoNavigationMain_2Hz_M->dwork.fm[i] = (real_T)i;
    }

    /* 'MAG_WorldMagneticModel:46' fn = coder.const(Load_fn); */
    /* 'MAG_WorldMagneticModel:47' k  = coder.const(Load_k ); */
    /* 'KalmanFilterMeasurement:38' delta_private = init_KalmanFilterErrorEstimates; */
    init_KalmanFilterErrorEstimates((NAV_DoNavigationMain_2Hz_M)
      ->dwork.delta_private.nav, (NAV_DoNavigationMain_2Hz_M)
      ->dwork.delta_private.kalmanfilter.state, (NAV_DoNavigationMain_2Hz_M)
      ->dwork.delta_private.kalmanfilter.covariance,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.numbersatellites,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.Reserved,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.gnss,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.baro,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.ta,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.mag,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.feedback_nav,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.feedback_att,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.valid.reset_navigation,
      &(NAV_DoNavigationMain_2Hz_M)->dwork.delta_private.trade);

    /* End of SystemInitialize for MATLAB Function: '<S1>/NAV_Embedded_DoNavigationMain2Hz' */
    /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_2Hz_at_outport_1' */
    /* 'KalmanFilterMeasurement:39' time_updateINS = 0.0; */
    /* 'KalmanFilterMeasurement:40' firstTimeMeasurement = uint8(0); */
    /* 'KalmanFilterMeasurement:43' KalmanFilterState_private = zeros(16,1); */
    /* 'KalmanFilterMeasurement:46' KalmanFilterCovariance_private = zeros(16,16); */
    /* 'KalmanFilterMeasurement:49' gnss_valid_private = uint8(0); */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
