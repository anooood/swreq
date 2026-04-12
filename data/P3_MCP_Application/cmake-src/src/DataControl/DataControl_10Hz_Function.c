/*
 * File: DataControl_10Hz_Function.c
 *
 * Code generated for Simulink model 'DataControl_10Hz_Function'.
 *
 * Model version                  : 1.1833
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sun May  7 22:32:38 2023
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

#include "DataControl_10Hz_Function.h"
#include "f_ee_aoa441JQ.h"
#include "interp1_bFwkvuIc.h"
#include "interp1_uaZNzuL5.h"
#include "interp2_NJA6JVlk.h"
#include "interp2_OIomwLVf.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"
#include "rt_roundd_snf.h"
#include "sqrt_pNDOZDUg.h"

/* Forward declaration for local functions */
static void LIB_ecef2lla(real_T re_x, real_T re_y, real_T re_z, real_T
  *pos_lla_lat, real_T *pos_lla_lon, real_T *pos_lla_alt);
static void LARlookup(uint32_T tgtvalid, real_T alt, real_T mach, real_T dive,
                      real_T relang, const b_sYbzGJxaGNvEV1Jh0D0xgEF *LARdata,
                      real_T *distx, real_T *disty, real_T *lr, real_T *er,
                      real_T *Rmax);
static void power(const int32_T a_data[], const int32_T *a_size, int32_T y_data[],
                  int32_T *y_size, DW *dwork);
static int32_T nonSingletonDim(const int32_T *x_size);
static void merge(int32_T idx_data[], int32_T x_data[], int32_T offset, int32_T
                  np, int32_T nq, int32_T iwork_data[], int32_T xwork_data[]);
static void merge_block(int32_T idx_data[], int32_T x_data[], int32_T offset,
  int32_T n, int32_T preSortLevel, int32_T iwork_data[], int32_T xwork_data[]);
static void merge_pow2_block(int32_T idx_data[], int32_T x_data[], int32_T
  offset);
static void sortIdx(int32_T x_data[], int32_T *x_size, int32_T idx_data[],
                    int32_T *idx_size, DW *dwork);
static void sort(int32_T x_data[], int32_T *x_size, int32_T idx_data[], int32_T *
                 idx_size, DW *dwork);
static real_T norm(const real_T x_data[], const int32_T *x_size);
static real_T sum(const real_T x_data[], const int32_T *x_size);
static void LARTimings(uint32_T tgtvalid, real_T mach, real_T gspd, const real_T
  relenu[3], real_T GndDistTgt, real_T RelBearTgt, real_T GndDistLAR, real_T
  RelBearLAR, real_T radius, real_T excl_radius, real_T diveangle, real_T
  attackhead, const int16_T LARtraj_TOF070[192000], const int16_T
  LARtraj_TOF080[192000], const int16_T LARtraj_TOF090[192000],
  stE9DGVPdbGPhyouEV4GctG *ct, snFb1SHWmbC64DSgfqhsaYG *traj, DW *dwork);
static void LARCalcs(real_T launch_alt, real_T launch_rex, real_T launch_rey,
                     real_T launch_rez, real_T launch_mach, real_T launch_track,
                     real_T launch_wind_spd, real_T launch_wind_dir, uint32_T
                     tgt_valid, real_T tgt_rex, real_T tgt_rey, real_T tgt_rez,
                     real_T tgt_alt, real_T tgt_dive, real_T tgt_arrive, const
                     b_sYbzGJxaGNvEV1Jh0D0xgEF *LARdata, const
                     T_Weapon_MCP_GlobalMemory_LARtraj *LARtraj,
                     sW6z4ntaPZV6RXLllkgQXpF *lar, DW *dwork);
static void LARCalculations(uint32_T tgt_data_valid, real_T tgt_data_rgx, real_T
  tgt_data_rgy, real_T tgt_data_rgz, real_T tgt_data_rex, real_T tgt_data_rey,
  real_T tgt_data_rez, real_T tgt_data_alt, real_T tgt_data_dive, real_T
  tgt_data_arrive, const T_GUID_Guid2DATA_launchstruct *launch_data, const
  b_sYbzGJxaGNvEV1Jh0D0xgEF *LAR_data, const T_Weapon_MCP_GlobalMemory_LARtraj
  *LAR_traj, const T_GUID_TestConfig TestCfg_in, s0qQI8FPZFwxQDcpW3UrxsD *LAR,
  DW *dwork);
static real_T interp1(const real_T varargin_1_data[], const int32_T
                      varargin_1_size[2], const real_T varargin_2_data[], const
                      int32_T varargin_2_size[2], real_T varargin_3);

/*
 * Function for MATLAB Function: '<S1>/Data Controller'
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
 * Function for MATLAB Function: '<S1>/Data Controller'
 * function [distx,disty,lr,er,Rmax] = LARlookup(tgtvalid,alt,mach,dive,relang,LARdata)
 */
static void LARlookup(uint32_T tgtvalid, real_T alt, real_T mach, real_T dive,
                      real_T relang, const b_sYbzGJxaGNvEV1Jh0D0xgEF *LARdata,
                      real_T *distx, real_T *disty, real_T *lr, real_T *er,
                      real_T *Rmax)
{
  int32_T sgn_ang;
  int32_T valid;
  real_T machlim;
  real_T CX1[48];
  real_T CX2[48];
  real_T CY1[48];
  real_T CY2[48];
  real_T RAD1[48];
  real_T RAD2[48];
  real_T EXCL1[48];
  real_T EXCL2[48];
  real_T disty1;
  real_T distx1;
  real_T lr1;
  real_T er1;
  real_T Rmax1;
  real_T LARdata_0[6];
  real_T LARdata_1[8];
  real_T LARdata_2[6];
  int32_T i;
  int32_T i_0;
  int32_T CX1_tmp;
  int32_T CX1_tmp_0;

  /* 'LARlookup:21' sgn_ang = 1; */
  sgn_ang = 1;

  /* 'LARlookup:22' if (bitget(tgtvalid,1)) */
  if ((tgtvalid & 1U) != 0U) {
    /* 'LARlookup:23' valid = 1; */
    valid = 1;

    /* 'LARlookup:24' if relang < 0 */
    if (relang < 0.0) {
      /* 'LARlookup:25' sgn_ang = -1; */
      sgn_ang = -1;
    }

    /* 'LARlookup:27' relang = abs(relang); */
    relang = fabs(relang);

    /* 'LARlookup:28' if (~bitget(tgtvalid,2)) */
    if ((tgtvalid & 2U) == 0U) {
      /* 'LARlookup:30' relang = 210; */
      relang = 210.0;
    } else {
      if (relang > (((real_T)LARdata->psilim) + 1.0)) {
        /* 'LARlookup:31' elseif (abs(relang) > double(LARdata.psilim)+1.0) */
        /* 'LARlookup:32' valid = 0; */
        valid = 0;
      }
    }

    /* 'LARlookup:35' if (alt < double(LARdata.altmin)*0.3048) || (alt > double(LARdata.altmax*0.3048)) */
    if ((alt < (((real_T)LARdata->altmin) * 0.3048)) || (alt > ((real_T)
          ((real32_T)(LARdata->altmax * 0.3048F))))) {
      /* 'LARlookup:35' valid = 0; */
      valid = 0;
    }

    /* 'LARlookup:36' if (mach < 0.68) || (mach > 0.92) */
    if ((mach < 0.68) || (mach > 0.92)) {
      /* 'LARlookup:36' valid = 0; */
      valid = 0;
    }
  } else {
    /* 'LARlookup:37' else */
    /* 'LARlookup:38' valid = 0; */
    valid = 0;
  }

  /* 'LARlookup:41' machlim = LIB_limit(0.7,0.9,mach); */
  /* 'LIB_limit:27' if (in > max) */
  if (mach > 0.9) {
    /* 'LIB_limit:28' out = max; */
    machlim = 0.9;
  } else if (mach < 0.7) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    machlim = 0.7;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
    machlim = mach;
  }

  /* 'LARlookup:42' if valid */
  if (valid != 0) {
    /* 'LARlookup:44' if dive < 45 */
    if (dive < 45.0) {
      /* 'LARlookup:45' n = 1; */
      valid = 0;
    } else if (dive < 60.0) {
      /* 'LARlookup:46' elseif dive < 60 */
      /* 'LARlookup:47' n = 1; */
      valid = 0;
    } else if ((!(dive < 75.0)) && (!(dive < 90.0))) {
      /* 'LARlookup:52' else */
      /* 'LARlookup:53' n = 3; */
      valid = 2;
    } else {
      /* 'LARlookup:48' elseif dive < 75 */
      /* 'LARlookup:49' n = 2; */
      /* 'LARlookup:50' elseif dive < 90 */
      /* 'LARlookup:51' n = 2; */
    }

    /* 'LARlookup:57' if machlim < 0.8 */
    if (machlim < 0.8) {
      /* 'LARlookup:58' mrat = (machlim - 0.7)/0.1; */
      machlim = (machlim - 0.7) / 0.1;

      /* 'LARlookup:59' CX1   = double(LARdata.CX070(:,:,n))'; */
      /* 'LARlookup:60' CX2   = double(LARdata.CX080(:,:,n))'; */
      /* 'LARlookup:61' CY1   = double(LARdata.CY070(:,:,n))'; */
      /* 'LARlookup:62' CY2   = double(LARdata.CY080(:,:,n))'; */
      /* 'LARlookup:63' RAD1  = double(LARdata.RAD070(:,:,n))'; */
      /* 'LARlookup:64' RAD2  = double(LARdata.RAD080(:,:,n))'; */
      /* 'LARlookup:65' EXCL1 = double(LARdata.EXCL070(:,:,n))'; */
      /* 'LARlookup:66' EXCL2 = double(LARdata.EXCL080(:,:,n))'; */
      for (i_0 = 0; i_0 < 8; i_0++) {
        for (i = 0; i < 6; i++) {
          CX1_tmp = ((i << ((uint64_T)3)) + i_0) + (48 * valid);
          CX1_tmp_0 = i + (6 * i_0);
          CX1[CX1_tmp_0] = (real_T)LARdata->CX070[CX1_tmp];
          CX2[CX1_tmp_0] = (real_T)LARdata->CX080[CX1_tmp];
          CY1[CX1_tmp_0] = (real_T)LARdata->CY070[CX1_tmp];
          CY2[CX1_tmp_0] = (real_T)LARdata->CY080[CX1_tmp];
          RAD1[CX1_tmp_0] = (real_T)LARdata->RAD070[CX1_tmp];
          RAD2[CX1_tmp_0] = (real_T)LARdata->RAD080[CX1_tmp];
          EXCL1[CX1_tmp_0] = (real_T)LARdata->EXCL070[CX1_tmp];
          EXCL2[CX1_tmp_0] = (real_T)LARdata->EXCL080[CX1_tmp];
        }
      }
    } else {
      /* 'LARlookup:67' else */
      /* 'LARlookup:68' mrat = (machlim - 0.8)/0.1; */
      machlim = (machlim - 0.8) / 0.1;

      /* 'LARlookup:69' CX1   = double(LARdata.CX080(:,:,n))'; */
      /* 'LARlookup:70' CX2   = double(LARdata.CX090(:,:,n))'; */
      /* 'LARlookup:71' CY1   = double(LARdata.CY080(:,:,n))'; */
      /* 'LARlookup:72' CY2   = double(LARdata.CY090(:,:,n))'; */
      /* 'LARlookup:73' RAD1  = double(LARdata.RAD080(:,:,n))'; */
      /* 'LARlookup:74' RAD2  = double(LARdata.RAD090(:,:,n))'; */
      /* 'LARlookup:75' EXCL1 = double(LARdata.EXCL080(:,:,n))'; */
      /* 'LARlookup:76' EXCL2 = double(LARdata.EXCL090(:,:,n))'; */
      for (i_0 = 0; i_0 < 8; i_0++) {
        for (i = 0; i < 6; i++) {
          CX1_tmp = ((i << ((uint64_T)3)) + i_0) + (48 * valid);
          CX1_tmp_0 = i + (6 * i_0);
          CX1[CX1_tmp_0] = (real_T)LARdata->CX080[CX1_tmp];
          CX2[CX1_tmp_0] = (real_T)LARdata->CX090[CX1_tmp];
          CY1[CX1_tmp_0] = (real_T)LARdata->CY080[CX1_tmp];
          CY2[CX1_tmp_0] = (real_T)LARdata->CY090[CX1_tmp];
          RAD1[CX1_tmp_0] = (real_T)LARdata->RAD080[CX1_tmp];
          RAD2[CX1_tmp_0] = (real_T)LARdata->RAD090[CX1_tmp];
          EXCL1[CX1_tmp_0] = (real_T)LARdata->EXCL080[CX1_tmp];
          EXCL2[CX1_tmp_0] = (real_T)LARdata->EXCL090[CX1_tmp];
        }
      }
    }

    /* 'LARlookup:80' disty1 = interp2(double(LARdata.psivec),double(LARdata.altvec),CX1,relang,alt/0.3048); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    disty1 = interp2_NJA6JVlk(LARdata_1, LARdata_2, CX1, relang, alt / 0.3048);

    /* 'LARlookup:81' distx1 = sgn_ang*interp2(double(LARdata.psivec),double(LARdata.altvec),CY1,relang,alt/0.3048); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    distx1 = interp2_NJA6JVlk(LARdata_1, LARdata_2, CY1, relang, alt / 0.3048) *
      ((real_T)sgn_ang);

    /* 'LARlookup:82' lr1    = interp2(double(LARdata.psivec),double(LARdata.altvec),RAD1,relang,alt/0.3048); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    lr1 = interp2_NJA6JVlk(LARdata_1, LARdata_2, RAD1, relang, alt / 0.3048);

    /* 'LARlookup:83' er1    = interp2(double(LARdata.psivec),double(LARdata.altvec),EXCL1,relang,alt/0.3048); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    er1 = interp2_NJA6JVlk(LARdata_1, LARdata_2, EXCL1, relang, alt / 0.3048);

    /* 'LARlookup:84' Rmax1  = interp1(double(LARdata.altvec),CX1(:,1),alt/0.3048) + ... */
    /* 'LARlookup:85' 	         interp1(double(LARdata.altvec),RAD1(:,1),alt/0.3048); */
    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
      LARdata_0[i_0] = (real_T)LARdata->altvec[i_0];
    }

    Rmax1 = interp1_uaZNzuL5(LARdata_2, &CX1[0], alt / 0.3048) +
      interp1_uaZNzuL5(LARdata_0, &RAD1[0], alt / 0.3048);

    /* 'LARlookup:87' disty2 = interp2(double(LARdata.psivec),double(LARdata.altvec),CX2,relang,alt/0.3048); */
    /* 'LARlookup:88' distx2 = sgn_ang*interp2(double(LARdata.psivec),double(LARdata.altvec),CY2,relang,alt/0.3048); */
    /* 'LARlookup:89' lr2    = interp2(double(LARdata.psivec),double(LARdata.altvec),RAD2,relang,alt/0.3048); */
    /* 'LARlookup:90' er2    = interp2(double(LARdata.psivec),double(LARdata.altvec),EXCL2,relang,alt/0.3048); */
    /* 'LARlookup:91' Rmax2  = interp1(double(LARdata.altvec),CX2(:,1),alt/0.3048) + ... */
    /* 'LARlookup:92' 	         interp1(double(LARdata.altvec),RAD2(:,1),alt/0.3048); */
    /* 'LARlookup:95' disty = disty1 + mrat*(disty2 - disty1); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    *disty = ((interp2_NJA6JVlk(LARdata_1, LARdata_2, CX2, relang, alt / 0.3048)
               - disty1) * machlim) + disty1;

    /* 'LARlookup:96' distx = distx1 + mrat*(distx2 - distx1); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    *distx = (((interp2_NJA6JVlk(LARdata_1, LARdata_2, CY2, relang, alt / 0.3048)
                * ((real_T)sgn_ang)) - distx1) * machlim) + distx1;

    /* 'LARlookup:97' lr    = lr1 + mrat*(lr2 - lr1); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    *lr = ((interp2_NJA6JVlk(LARdata_1, LARdata_2, RAD2, relang, alt / 0.3048) -
            lr1) * machlim) + lr1;

    /* 'LARlookup:98' er    = er1 + mrat*(er2 - er1); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      LARdata_1[i_0] = (real_T)LARdata->psivec[i_0];
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
    }

    *er = ((interp2_NJA6JVlk(LARdata_1, LARdata_2, EXCL2, relang, alt / 0.3048)
            - er1) * machlim) + er1;

    /* 'LARlookup:99' Rmax  = Rmax1 + mrat*(Rmax2 - Rmax1); */
    for (i_0 = 0; i_0 < 6; i_0++) {
      LARdata_2[i_0] = (real_T)LARdata->altvec[i_0];
      LARdata_0[i_0] = (real_T)LARdata->altvec[i_0];
    }

    *Rmax = (((interp1_uaZNzuL5(LARdata_2, &CX2[0], alt / 0.3048) +
               interp1_uaZNzuL5(LARdata_0, &RAD2[0], alt / 0.3048)) - Rmax1) *
             machlim) + Rmax1;
  } else {
    /* 'LARlookup:100' else */
    /* 'LARlookup:101' distx = 0; */
    *distx = 0.0;

    /* 'LARlookup:102' disty = 0; */
    *disty = 0.0;

    /* 'LARlookup:103' lr    = 0; */
    *lr = 0.0;

    /* 'LARlookup:104' er    = 0; */
    *er = 0.0;

    /* 'LARlookup:105' Rmax  = 0; */
    *Rmax = 0.0;
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void power(const int32_T a_data[], const int32_T *a_size, int32_T y_data[],
                  int32_T *y_size, DW *dwork)
{
  int32_T b_y;
  int32_T b_a;
  uint32_T bu;
  int32_T loop_ub;
  int16_T a_idx_0;
  int64_T tmp;
  int32_T exitg1;
  a_idx_0 = (int16_T)(*a_size);
  if (0 <= (a_idx_0 - 1)) {
    memcpy(&dwork->z1_data[0], &y_data[0], ((uint32_T)a_idx_0) * (sizeof(int32_T)));
  }

  for (loop_ub = 0; loop_ub < ((int32_T)a_idx_0); loop_ub++) {
    b_a = a_data[loop_ub];
    b_y = 1;
    bu = 2U;
    do {
      exitg1 = 0;
      if ((bu & 1U) != 0U) {
        tmp = ((int64_T)b_a) * ((int64_T)b_y);
        if (tmp > 2147483647LL) {
          tmp = 2147483647LL;
        } else {
          if (tmp < -2147483648LL) {
            tmp = -2147483648LL;
          }
        }

        b_y = (int32_T)tmp;
      }

      bu >>= 1U;
      if (((int32_T)bu) == 0) {
        exitg1 = 1;
      } else {
        tmp = ((int64_T)b_a) * ((int64_T)b_a);
        if (tmp > 2147483647LL) {
          tmp = 2147483647LL;
        } else {
          if (tmp < -2147483648LL) {
            tmp = -2147483648LL;
          }
        }

        b_a = (int32_T)tmp;
      }
    } while (exitg1 == 0);

    dwork->z1_data[loop_ub] = b_y;
  }

  *y_size = (int32_T)((int16_T)(*a_size));
  if (0 <= (a_idx_0 - 1)) {
    memcpy(&y_data[0], &dwork->z1_data[0], ((uint32_T)a_idx_0) * (sizeof(int32_T)));
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static int32_T nonSingletonDim(const int32_T *x_size)
{
  int32_T dim;
  dim = 2;
  if ((*x_size) != 1) {
    dim = 1;
  }

  return dim;
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void merge(int32_T idx_data[], int32_T x_data[], int32_T offset, int32_T
                  np, int32_T nq, int32_T iwork_data[], int32_T xwork_data[])
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

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void merge_block(int32_T idx_data[], int32_T x_data[], int32_T offset,
  int32_T n, int32_T preSortLevel, int32_T iwork_data[], int32_T xwork_data[])
{
  int32_T bLen;
  int32_T tailOffset;
  int32_T nTail;
  int32_T nPairs;
  nPairs = (n >> ((uint64_T)preSortLevel));
  bLen = (1 << ((uint64_T)preSortLevel));
  while (nPairs > 1) {
    if ((((uint32_T)nPairs) & 1U) != 0U) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx_data, x_data, offset + tailOffset, bLen, nTail - bLen,
              iwork_data, xwork_data);
      }
    }

    tailOffset = (bLen << ((uint64_T)1));
    nPairs >>= (uint64_T)1;
    for (nTail = 0; nTail < nPairs; nTail++) {
      merge(idx_data, x_data, offset + (nTail * tailOffset), bLen, bLen,
            iwork_data, xwork_data);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx_data, x_data, offset, bLen, n - bLen, iwork_data, xwork_data);
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void merge_pow2_block(int32_T idx_data[], int32_T x_data[], int32_T
  offset)
{
  int32_T iwork[256];
  int32_T xwork[256];
  int32_T bLen;
  int32_T bLen2;
  int32_T nPairs;
  int32_T blockOffset;
  int32_T p;
  int32_T q;
  int32_T b;
  int32_T k;
  int32_T exitg1;
  for (b = 0; b < 6; b++) {
    bLen = (1 << ((uint64_T)((int32_T)(b + 2))));
    bLen2 = (bLen << ((uint64_T)1));
    nPairs = (256 >> ((uint64_T)((int32_T)(b + 3))));
    for (k = 0; k < nPairs; k++) {
      blockOffset = ((k * bLen2) + offset) - 1;
      for (p = 0; p < bLen2; p++) {
        q = (blockOffset + p) + 1;
        iwork[p] = idx_data[q];
        xwork[p] = x_data[q];
      }

      p = 0;
      q = bLen;
      do {
        exitg1 = 0;
        blockOffset++;
        if (xwork[p] <= xwork[q]) {
          idx_data[blockOffset] = iwork[p];
          x_data[blockOffset] = xwork[p];
          if ((p + 1) < bLen) {
            p++;
          } else {
            exitg1 = 1;
          }
        } else {
          idx_data[blockOffset] = iwork[q];
          x_data[blockOffset] = xwork[q];
          if ((q + 1) < bLen2) {
            q++;
          } else {
            blockOffset -= p;
            while ((p + 1) <= bLen) {
              q = (blockOffset + p) + 1;
              idx_data[q] = iwork[p];
              x_data[q] = xwork[p];
              p++;
            }

            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
    }
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void sortIdx(int32_T x_data[], int32_T *x_size, int32_T idx_data[],
                    int32_T *idx_size, DW *dwork)
{
  int32_T x4[4];
  int16_T idx4[4];
  int8_T perm[4];
  int32_T nQuartets;
  int32_T i;
  int32_T nLeft;
  int32_T i1;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T c_x_size;
  int16_T b_x_idx_0;
  int16_T b_idx_0;
  int32_T tmp;
  int32_T tmp_0;
  b_x_idx_0 = (int16_T)(*x_size);
  b_idx_0 = (int16_T)(*x_size);
  *idx_size = (int32_T)b_x_idx_0;
  if (0 <= (b_x_idx_0 - 1)) {
    memset(&idx_data[0], 0, ((uint32_T)b_x_idx_0) * (sizeof(int32_T)));
  }

  if ((*x_size) != 0) {
    c_x_size = *x_size;
    if (0 <= ((*x_size) - 1)) {
      memcpy(&dwork->c_x_data[0], &x_data[0], ((uint32_T)(*x_size)) * (sizeof
              (int32_T)));
    }

    *idx_size = (int32_T)b_idx_0;
    if (0 <= (b_idx_0 - 1)) {
      memset(&idx_data[0], 0, ((uint32_T)b_idx_0) * (sizeof(int32_T)));
    }

    x4[0] = 0;
    idx4[0] = 0;
    x4[1] = 0;
    idx4[1] = 0;
    x4[2] = 0;
    idx4[2] = 0;
    x4[3] = 0;
    idx4[3] = 0;
    nQuartets = ((*x_size) >> ((uint64_T)2));
    for (nLeft = 0; nLeft < nQuartets; nLeft++) {
      i = (nLeft << ((uint64_T)2));
      idx4[0] = (int16_T)(i + 1);
      idx4[1] = (int16_T)(i + 2);
      idx4[2] = (int16_T)(i + 3);
      idx4[3] = (int16_T)(i + 4);
      x4[0] = dwork->c_x_data[i];
      x4[1] = dwork->c_x_data[i + 1];
      x4[2] = dwork->c_x_data[i + 2];
      x4[3] = dwork->c_x_data[i + 3];
      if (dwork->c_x_data[i] <= dwork->c_x_data[i + 1]) {
        i1 = 1;
        i2 = 2;
      } else {
        i1 = 2;
        i2 = 1;
      }

      if (dwork->c_x_data[i + 2] <= dwork->c_x_data[i + 3]) {
        i3 = 3;
        i4 = 4;
      } else {
        i3 = 4;
        i4 = 3;
      }

      tmp = x4[i1 - 1];
      tmp_0 = x4[i3 - 1];
      if (tmp <= tmp_0) {
        tmp = x4[i2 - 1];
        if (tmp <= tmp_0) {
          perm[0] = (int8_T)i1;
          perm[1] = (int8_T)i2;
          perm[2] = (int8_T)i3;
          perm[3] = (int8_T)i4;
        } else if (tmp <= x4[i4 - 1]) {
          perm[0] = (int8_T)i1;
          perm[1] = (int8_T)i3;
          perm[2] = (int8_T)i2;
          perm[3] = (int8_T)i4;
        } else {
          perm[0] = (int8_T)i1;
          perm[1] = (int8_T)i3;
          perm[2] = (int8_T)i4;
          perm[3] = (int8_T)i2;
        }
      } else {
        tmp_0 = x4[i4 - 1];
        if (tmp <= tmp_0) {
          if (x4[i2 - 1] <= tmp_0) {
            perm[0] = (int8_T)i3;
            perm[1] = (int8_T)i1;
            perm[2] = (int8_T)i2;
            perm[3] = (int8_T)i4;
          } else {
            perm[0] = (int8_T)i3;
            perm[1] = (int8_T)i1;
            perm[2] = (int8_T)i4;
            perm[3] = (int8_T)i2;
          }
        } else {
          perm[0] = (int8_T)i3;
          perm[1] = (int8_T)i4;
          perm[2] = (int8_T)i1;
          perm[3] = (int8_T)i2;
        }
      }

      i1 = ((int32_T)perm[0]) - 1;
      idx_data[i] = (int32_T)idx4[i1];
      i2 = ((int32_T)perm[1]) - 1;
      idx_data[i + 1] = (int32_T)idx4[i2];
      i3 = ((int32_T)perm[2]) - 1;
      idx_data[i + 2] = (int32_T)idx4[i3];
      i4 = ((int32_T)perm[3]) - 1;
      idx_data[i + 3] = (int32_T)idx4[i4];
      dwork->c_x_data[i] = x4[i1];
      dwork->c_x_data[i + 1] = x4[i2];
      dwork->c_x_data[i + 2] = x4[i3];
      dwork->c_x_data[i + 3] = x4[i4];
    }

    nQuartets <<= (uint64_T)2;
    nLeft = ((*x_size) - nQuartets) - 1;
    if ((nLeft + 1) > 0) {
      for (i = 0; i <= nLeft; i++) {
        i1 = nQuartets + i;
        idx4[i] = (int16_T)(i1 + 1);
        x4[i] = dwork->c_x_data[i1];
      }

      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      switch (nLeft + 1) {
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

      for (i = 0; i <= nLeft; i++) {
        i1 = ((int32_T)perm[i]) - 1;
        idx_data[nQuartets + i] = (int32_T)idx4[i1];
        dwork->c_x_data[nQuartets + i] = x4[i1];
      }
    }

    if (0 <= (b_x_idx_0 - 1)) {
      memset(&dwork->iwork_data[0], 0, ((uint32_T)b_x_idx_0) * (sizeof(int32_T)));
    }

    b_idx_0 = (int16_T)(*x_size);
    if (0 <= (b_idx_0 - 1)) {
      memset(&dwork->xwork_data[0], 0, ((uint32_T)b_idx_0) * (sizeof(int32_T)));
    }

    nLeft = 2;
    if ((*x_size) > 1) {
      if ((*x_size) >= 256) {
        nQuartets = ((*x_size) >> ((uint64_T)8));
        if (nQuartets > 0) {
          for (nLeft = 0; nLeft < nQuartets; nLeft++) {
            merge_pow2_block(idx_data, dwork->c_x_data, nLeft << ((uint64_T)8));
          }

          nQuartets <<= (uint64_T)8;
          nLeft = (*x_size) - nQuartets;
          if (nLeft > 0) {
            if (0 <= (b_x_idx_0 - 1)) {
              memset(&dwork->iwork_data[0], 0, ((uint32_T)b_x_idx_0) * (sizeof
                      (int32_T)));
            }

            b_idx_0 = (int16_T)(*x_size);
            if (0 <= (b_idx_0 - 1)) {
              memset(&dwork->xwork_data[0], 0, ((uint32_T)b_idx_0) * (sizeof
                      (int32_T)));
            }

            merge_block(idx_data, dwork->c_x_data, nQuartets, nLeft, 2,
                        dwork->iwork_data, dwork->xwork_data);
          }

          nLeft = 8;
        }
      }

      merge_block(idx_data, dwork->c_x_data, 0, *x_size, nLeft,
                  dwork->iwork_data, dwork->xwork_data);
    }

    if (0 <= (c_x_size - 1)) {
      memcpy(&x_data[0], &dwork->c_x_data[0], ((uint32_T)c_x_size) * (sizeof
              (int32_T)));
    }
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static void sort(int32_T x_data[], int32_T *x_size, int32_T idx_data[], int32_T *
                 idx_size, DW *dwork)
{
  int32_T dim;
  int32_T vstride;
  int32_T b;
  int32_T c_k;
  int32_T vwork_size;
  int32_T tmp;
  dim = nonSingletonDim(x_size);
  if (dim <= 1) {
    b = (*x_size) - 1;
  } else {
    b = 0;
  }

  vwork_size = (int32_T)((int16_T)(b + 1));
  *idx_size = (int32_T)((int16_T)(*x_size));
  vstride = 1;
  for (c_k = 0; c_k <= (dim - 2); c_k++) {
    vstride *= *x_size;
  }

  for (dim = 0; dim < vstride; dim++) {
    for (c_k = 0; c_k <= b; c_k++) {
      dwork->vwork_data[c_k] = x_data[(c_k * vstride) + dim];
    }

    sortIdx(dwork->vwork_data, &vwork_size, dwork->iidx_data, &c_k, dwork);
    for (c_k = 0; c_k <= b; c_k++) {
      tmp = dim + (c_k * vstride);
      x_data[tmp] = dwork->vwork_data[c_k];
      idx_data[tmp] = dwork->iidx_data[c_k];
    }
  }
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static real_T norm(const real_T x_data[], const int32_T *x_size)
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;
  int32_T k;
  if ((*x_size) == 0) {
    y = 0.0;
  } else {
    y = 0.0;
    if ((*x_size) == 1) {
      y = fabs(x_data[0]);
    } else {
      scale = 3.3121686421112381E-170;
      for (k = 0; k < (*x_size); k++) {
        absxk = fabs(x_data[k]);
        if (absxk > scale) {
          t = scale / absxk;
          y = ((y * t) * t) + 1.0;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrt(y);
    }
  }

  return y;
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static real_T sum(const real_T x_data[], const int32_T *x_size)
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
 * Function for MATLAB Function: '<S1>/Data Controller'
 * function [ct,traj] = LARTimings(tgtvalid,mach,gspd,relenu,GndDistTgt,RelBearTgt,GndDistLAR,RelBearLAR,radius,excl_radius,diveangle,attackhead,LARtraj,return_traj)
 */
static void LARTimings(uint32_T tgtvalid, real_T mach, real_T gspd, const real_T
  relenu[3], real_T GndDistTgt, real_T RelBearTgt, real_T GndDistLAR, real_T
  RelBearLAR, real_T radius, real_T excl_radius, real_T diveangle, real_T
  attackhead, const int16_T LARtraj_TOF070[192000], const int16_T
  LARtraj_TOF080[192000], const int16_T LARtraj_TOF090[192000],
  stE9DGVPdbGPhyouEV4GctG *ct, snFb1SHWmbC64DSgfqhsaYG *traj, DW *dwork)
{
  real_T ent;
  real_T exd;
  int32_T lar_int;
  int32_T exc_int;
  real_T lard1;
  real_T lard2;
  real_T exd1;
  real_T exd2;
  real_T ti1tmp1[2];
  real_T ti1tmp2[2];
  real_T ti1tmp[2];
  real_T tmp1;
  int16_T head;
  real_T ratio;
  uint8_T ind1_valid;
  uint8_T ind2_valid;
  real_T dvec[10];
  real_T tmp[10];
  int32_T idx;
  int32_T b_ii;
  boolean_T e_x[10];
  int8_T n_ii_data[10];
  int32_T i;
  real_T dvec_data[10];
  int32_T i_0;
  int32_T ind1_size_idx_0;
  int16_T tmp_0;
  real_T tmp1_tmp;
  uint32_T tmp_1;
  int32_T head_tmp;
  int32_T dive;
  int32_T d_x_tmp;
  int32_T d_x_tmp_0;
  boolean_T exitg1;
  int32_T exitg2;

  /* 'LARTimings:24' ent = 0; */
  ent = 0.0;

  /* 'LARTimings:25' exd = 0; */
  exd = 0.0;

  /* 'LARTimings:26' lar_int = 0; */
  lar_int = 0;

  /* 'LARTimings:27' exc_int = 0; */
  exc_int = 0;

  /* 'LARTimings:28' ct.inside = 0; */
  ct->inside = 0.0;

  /* 'LARTimings:29' lard1 = 0; */
  lard1 = 0.0;

  /* 'LARTimings:30' lard2 = 0; */
  lard2 = 0.0;

  /* 'LARTimings:31' exd1 = 0; */
  exd1 = 0.0;

  /* 'LARTimings:32' exd2 = 0; */
  exd2 = 0.0;

  /* 'LARTimings:33' len1 = 8000; */
  /* 'LARTimings:34' len2 = 8000; */
  /* 'LARTimings:35' ind1 = 0; */
  ind1_size_idx_0 = 1;
  dwork->ind1_data[0] = 0;

  /* 'LARTimings:36' ind2 = 0; */
  /* 'LARTimings:41' dataMAT1 = int16(zeros(8000,24)); */
  /* 'LARTimings:42' dataMAT2 = int16(zeros(8000,24)); */
  memset(&dwork->dataMAT1[0], 0, 192000U * (sizeof(int16_T)));
  memset(&dwork->dataMAT2[0], 0, 192000U * (sizeof(int16_T)));

  /* 'LARTimings:43' dTgt2 = int32(ones(8000,1)); */
  for (i = 0; i < 8000; i++) {
    dwork->dTgt2[i] = 1;
  }

  /* 'LARTimings:46' posNtmp1 = zeros(11,2); */
  /* 'LARTimings:47' posEtmp1 = zeros(11,2); */
  /* 'LARTimings:48' gndRtmp1 = [0.0 0.0]; */
  /* 'LARTimings:49' ti1tmp1  = [0.0 0.0]; */
  /* 'LARTimings:50' posNtmp2 = zeros(11,2); */
  /* 'LARTimings:51' posEtmp2 = zeros(11,2); */
  /* 'LARTimings:52' gndRtmp2 = [0.0 0.0]; */
  /* 'LARTimings:53' ti1tmp2  = [0.0 0.0]; */
  ti1tmp1[0] = 0.0;
  ti1tmp2[0] = 0.0;
  ti1tmp1[1] = 0.0;
  ti1tmp2[1] = 0.0;

  /* 'LARTimings:54' posNtmp = zeros(11,2); */
  /* 'LARTimings:55' posEtmp = zeros(11,2); */
  /* 'LARTimings:56' gndRtmp = [0.0 0.0]; */
  /* 'LARTimings:57' ti1tmp  = [0.0 0.0]; */
  ti1tmp[0] = 0.0;
  ti1tmp[1] = 0.0;

  /* 'LARTimings:74' tmp1 = GndDistLAR^2*(cos(RelBearLAR)^2 - 1) + radius^2; */
  tmp1_tmp = cos(RelBearLAR);
  tmp1 = (((tmp1_tmp * tmp1_tmp) - 1.0) * (GndDistLAR * GndDistLAR)) + (radius *
    radius);

  /* 'LARTimings:75' if tmp1 > 0 */
  if (tmp1 > 0.0) {
    /* 'LARTimings:76' lar_int = 1; */
    lar_int = 1;

    /* 'LARTimings:77' lard1 = GndDistLAR*cos(RelBearLAR) - sqrt(tmp1); */
    lard2 = GndDistLAR * tmp1_tmp;
    tmp1 = sqrt(tmp1);
    lard1 = lard2 - tmp1;

    /* 'LARTimings:78' lard2 = GndDistLAR*cos(RelBearLAR) + sqrt(tmp1); */
    lard2 += tmp1;
  }

  /* 'LARTimings:82' tmp2 = GndDistTgt^2*(cos(RelBearTgt)^2 - 1) + excl_radius^2; */
  tmp1_tmp = cos(RelBearTgt);
  tmp1 = (((tmp1_tmp * tmp1_tmp) - 1.0) * (GndDistTgt * GndDistTgt)) +
    (excl_radius * excl_radius);

  /* 'LARTimings:83' if tmp2 > 0 */
  if (tmp1 > 0.0) {
    /* 'LARTimings:84' exc_int = 1; */
    exc_int = 1;

    /* 'LARTimings:85' exd1 = GndDistTgt*cos(RelBearTgt) - sqrt(tmp2); */
    exd2 = GndDistTgt * tmp1_tmp;
    exd1 = exd2 - sqrt(tmp1);

    /* 'LARTimings:86' exd2 = GndDistTgt*cos(RelBearTgt) + sqrt(tmp2); */
    exd2 += sqrt(tmp1);
  }

  /* 'LARTimings:90' if lar_int && ~exc_int */
  if ((lar_int != 0) && (exc_int == 0)) {
    /* 'LARTimings:91' [ent, exd, ct.inside] = f_ee(lard1,lard2); */
    f_ee_aoa441JQ(lard1, lard2, &ent, &exd, &ct->inside);
  } else {
    if ((lar_int != 0) && (exc_int != 0)) {
      /* 'LARTimings:92' elseif lar_int && exc_int */
      /* 'LARTimings:93' if lard1 < exd1 */
      if (lard1 < exd1) {
        /* 'LARTimings:94' if lard2 < exd1 */
        if (lard2 < exd1) {
          /* 'LARTimings:95' [ent, exd, ct.inside] = f_ee(lard1,lard2); */
          f_ee_aoa441JQ(lard1, lard2, &ent, &exd, &ct->inside);
        } else if (lard2 < exd2) {
          /* 'LARTimings:96' elseif lard2 < exd2 */
          /* 'LARTimings:97' [ent, exd, ct.inside] = f_ee(lard1,exd1); */
          f_ee_aoa441JQ(lard1, exd1, &ent, &exd, &ct->inside);
        } else if (exd1 > 0.0) {
          /* 'LARTimings:98' elseif exd1 > 0 */
          /* 'LARTimings:99' [ent, exd, ct.inside] = f_ee(lard1,exd1); */
          f_ee_aoa441JQ(lard1, exd1, &ent, &exd, &ct->inside);
        } else {
          /* 'LARTimings:100' else */
          /* 'LARTimings:101' [ent, exd, ct.inside] = f_ee(exd2,lard2); */
          f_ee_aoa441JQ(exd2, lard2, &ent, &exd, &ct->inside);
        }
      } else if (lard1 < exd2) {
        /* 'LARTimings:103' elseif lard1 < exd2 */
        /* 'LARTimings:104' [ent, exd, ct.inside] = f_ee(exd2,lard2); */
        f_ee_aoa441JQ(exd2, lard2, &ent, &exd, &ct->inside);
      } else {
        /* 'LARTimings:105' else */
        /* 'LARTimings:106' [ent, exd, ct.inside] = f_ee(lard1,lard2); */
        f_ee_aoa441JQ(lard1, lard2, &ent, &exd, &ct->inside);
      }
    }
  }

  /* 'LARTimings:110' TgtX = GndDistTgt*cos(RelBearTgt); */
  lard1 = GndDistTgt * cos(RelBearTgt);

  /* 'LARTimings:111' TgtY = GndDistTgt*sin(RelBearTgt); */
  lard2 = GndDistTgt * sin(RelBearTgt);

  /* 'LARTimings:112' dive = int16(diveangle); */
  exd1 = rt_roundd_snf(diveangle);
  if (exd1 < 32768.0) {
    if (exd1 >= -32768.0) {
      tmp_0 = (int16_T)exd1;
    } else {
      tmp_0 = MIN_int16_T;
    }
  } else {
    tmp_0 = MAX_int16_T;
  }

  /* 'LARTimings:113' if (TgtY >= 0.0) */
  if (lard2 >= 0.0) {
    /* 'LARTimings:114' head = int16(attackhead); */
    exd1 = rt_roundd_snf(attackhead);
    if (exd1 < 32768.0) {
      if (exd1 >= -32768.0) {
        head = (int16_T)exd1;
      } else {
        head = MIN_int16_T;
      }
    } else {
      head = MAX_int16_T;
    }

    /* 'LARTimings:115' pol  = 1.0; */
  } else {
    /* 'LARTimings:116' else */
    /* 'LARTimings:117' TgtY = -TgtY; */
    lard2 = -lard2;

    /* 'LARTimings:118' head = int16(360 - attackhead); */
    exd1 = rt_roundd_snf(360.0 - attackhead);
    if (exd1 < 32768.0) {
      if (exd1 >= -32768.0) {
        head = (int16_T)exd1;
      } else {
        head = MIN_int16_T;
      }
    } else {
      head = MAX_int16_T;
    }

    /* 'LARTimings:119' pol  = -1.0; */
  }

  /* 'LARTimings:122' if ((ent == 0 && exd == 0) || ~bitget(tgtvalid,1)) */
  tmp_1 = tgtvalid & 1U;
  if (((ent == 0.0) && (exd == 0.0)) || (tmp_1 == 0U)) {
    /* 'LARTimings:123' ct.tg  = 0; */
    ct->tg = 0.0;

    /* 'LARTimings:124' ct.te  = 0; */
    ct->te = 0.0;

    /* 'LARTimings:125' ct.ti  = 0; */
    ct->ti = 0.0;

    /* 'LARTimings:126' ct.ts  = 0; */
    ct->ts = 0.0;
  } else {
    /* 'LARTimings:127' else */
    /* 'LARTimings:129' ct.tg   = ent/gspd; */
    ct->tg = ent / gspd;

    /* 'LARTimings:132' ct.te   = exd/gspd; */
    ct->te = exd / gspd;

    /* 'LARTimings:134' ct.ti  = 0; */
    ct->ti = 0.0;

    /* 'LARTimings:135' ct.ts  = 0; */
    ct->ts = 0.0;
  }

  /* 'LARTimings:146' traj.N = zeros(11,1); */
  /* 'LARTimings:147' traj.E = zeros(11,1); */
  memset(&traj->N[0], 0, 11U * (sizeof(real_T)));
  memset(&traj->E[0], 0, 11U * (sizeof(real_T)));

  /* 'LARTimings:148' traj.diveangle = 0.0; */
  traj->diveangle = 0.0;

  /* 'LARTimings:149' traj.attackhead = 0.0; */
  traj->attackhead = 0.0;

  /* 'LARTimings:150' traj.impspeed = 0.0; */
  traj->impspeed = 0.0;

  /* 'LARTimings:151' traj.groundrange = 0.0; */
  traj->groundrange = 0.0;

  /* 'LARTimings:152' if (GndDistLAR < 4900) */
  /* 'LARTimings:155' if (bitget(tgtvalid,1) && GndDistLAR < 1.25*radius) */
  if ((tmp_1 != 0U) && (GndDistLAR < (1.25 * radius))) {
    /* 'LARTimings:158' for inc = 1:2 */
    for (lar_int = 0; lar_int < 2; lar_int++) {
      /* 'LARTimings:159' len1 = 1; */
      /* 'LARTimings:160' len2 = 1; */
      /* 'LARTimings:161' ratioA = 0.0; */
      ent = 0.0;

      /* 'LARTimings:162' if (mach < 0.8) */
      if (mach < 0.8) {
        /* 'LARTimings:163' ratio = 10.0*(mach-0.7); */
        ratio = (mach - 0.7) * 10.0;

        /* 'LARTimings:164' if (inc == 1) */
        if ((1 + lar_int) == 1) {
          /* 'LARTimings:165' if (bitget(tgtvalid,2)) */
          tmp_1 = tgtvalid & 2U;
          if (tmp_1 != 0U) {
            /* 'LARTimings:166' ind1 = find(LARtraj.TOF070(:,4) >  dive-1  & LARtraj.TOF070(:,4) < dive+1 & ... */
            /* 'LARTimings:167' 	                    LARtraj.TOF070(:,5) >= head-30 & LARtraj.TOF070(:,5) < head); */
            b_ii = ((int32_T)tmp_0) - 1;
            idx = b_ii;
            if (b_ii < -32768) {
              idx = -32768;
            }

            i = ((int32_T)tmp_0) + 1;
            dive = i;
            if (i > 32767) {
              dive = 32767;
            }

            head_tmp = ((int32_T)head) - 30;
            if (head_tmp < -32768) {
              head_tmp = -32768;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF070[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF070[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= head_tmp)) && (d_x_tmp_0 <
                ((int32_T)head)));
            }

            exc_int = 0;
            idx = 0;
            exitg1 = false;
            while ((!exitg1) && (idx < 8000)) {
              if (dwork->d_x[idx]) {
                exc_int++;
                dwork->ii_data[exc_int - 1] = idx + 1;
                if (exc_int >= 8000) {
                  exitg1 = true;
                } else {
                  idx++;
                }
              } else {
                idx++;
              }
            }

            if (1 > exc_int) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = exc_int;
            }

            ind1_size_idx_0 = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind1_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:168' if (isempty(ind1)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:169' len1 = 0; */
              exc_int = 0;
            } else {
              /* 'LARTimings:170' else */
              /* 'LARTimings:171' len1 = length(ind1); */
              exc_int = d_x_tmp;
            }

            /* 'LARTimings:173' ind2 = find(LARtraj.TOF070(:,4) >  dive-1  & LARtraj.TOF070(:,4) < dive+1 & ... */
            /* 'LARTimings:174' 	                    LARtraj.TOF070(:,5) >= head    & LARtraj.TOF070(:,5) < head+30); */
            if (b_ii < -32768) {
              b_ii = -32768;
            }

            if (i > 32767) {
              i = 32767;
            }

            head_tmp = ((int32_T)head) + 30;
            if (head_tmp > 32767) {
              head_tmp = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF070[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF070[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > b_ii) && (d_x_tmp < i)) &&
                                  (d_x_tmp_0 >= ((int32_T)head))) && (d_x_tmp_0 <
                head_tmp));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:175' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:176' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:177' else */
              /* 'LARTimings:178' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          } else {
            /* 'LARTimings:180' else */
            /* 'LARTimings:181' len1 = 0; */
            exc_int = 0;

            /* 'LARTimings:182' ind2 = find(LARtraj.TOF070(:,4) >  dive-1  & LARtraj.TOF070(:,4) < dive+1 & ... */
            /* 'LARTimings:183' 	                    LARtraj.TOF070(:,5) == 1000); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF070[24000 + i_0];
              dwork->d_x[i_0] = (((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                 (LARtraj_TOF070[32000 + i_0] == 1000));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:184' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:185' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:186' else */
              /* 'LARTimings:187' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          }

          /* 'LARTimings:190' ind1_valid = uint8(len1>0); */
          ind1_valid = (uint8_T)((exc_int > 0) ? 1 : 0);

          /* 'LARTimings:191' ind2_valid = uint8(len2>0); */
          ind2_valid = (uint8_T)((idx > 0) ? 1 : 0);

          /* 'LARTimings:192' if ((~ind1_valid && ind2_valid && head == 30) || ~bitget(tgtvalid,2)) */
          if ((((((int32_T)ind1_valid) == 0) && (((int32_T)ind2_valid) != 0)) &&
               (head == 30)) || (tmp_1 == 0U)) {
            /* 'LARTimings:193' ratioA = 1.0; */
            ent = 1.0;

            /* 'LARTimings:194' dataMAT2(1:len2,:) = LARtraj.TOF070(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF070[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          } else if ((((int32_T)ind1_valid) == 0) || (((int32_T)ind2_valid) == 0))
          {
            /* 'LARTimings:195' elseif (~ind1_valid || ~ind2_valid) */
            /* 'LARTimings:196' ti1tmp(inc) = 0.0; */
            ti1tmp[0] = 0.0;
          } else {
            /* 'LARTimings:197' else */
            /* 'LARTimings:198' ratioA = double(head-LARtraj.TOF070(ind1(1),5))/30.0; */
            i_0 = ((int32_T)head) - ((int32_T)LARtraj_TOF070[dwork->ind1_data[0]
              + 31999]);
            if (i_0 > 32767) {
              i_0 = 32767;
            } else {
              if (i_0 < -32768) {
                i_0 = -32768;
              }
            }

            ent = ((real_T)i_0) / 30.0;

            /* 'LARTimings:199' dataMAT1(1:len1,:) = LARtraj.TOF070(ind1,:); */
            /* 'LARTimings:200' dataMAT2(1:len2,:) = LARtraj.TOF070(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < ind1_size_idx_0; i++) {
                dwork->dataMAT1[i + (8000 * i_0)] = LARtraj_TOF070[((8000 * i_0)
                  + ((int32_T)dwork->ind1_data[i])) - 1];
              }

              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF070[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          }
        } else {
          /* 'LARTimings:202' else */
          /* 'LARTimings:203' if (bitget(tgtvalid,2)) */
          if ((tgtvalid & 2U) != 0U) {
            /* 'LARTimings:204' ind1 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:205' 	                    LARtraj.TOF080(:,5) >= head-30 & LARtraj.TOF080(:,5) < head); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) - 30;
            if (b_ii < -32768) {
              b_ii = -32768;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF080[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= b_ii)) && (d_x_tmp_0 < ((int32_T)
                head)));
            }

            exc_int = 0;
            idx = 0;
            exitg1 = false;
            while ((!exitg1) && (idx < 8000)) {
              if (dwork->d_x[idx]) {
                exc_int++;
                dwork->ii_data[exc_int - 1] = idx + 1;
                if (exc_int >= 8000) {
                  exitg1 = true;
                } else {
                  idx++;
                }
              } else {
                idx++;
              }
            }

            if (1 > exc_int) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = exc_int;
            }

            ind1_size_idx_0 = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind1_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:206' if (isempty(ind1)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:207' len1 = 0; */
              exc_int = 0;
            } else {
              /* 'LARTimings:208' else */
              /* 'LARTimings:209' len1 = length(ind1); */
              exc_int = d_x_tmp;
            }

            /* 'LARTimings:211' ind2 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:212' 	                    LARtraj.TOF080(:,5) >= head    & LARtraj.TOF080(:,5) < head+30); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) + 30;
            if (b_ii > 32767) {
              b_ii = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF080[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= ((int32_T)head))) && (d_x_tmp_0 <
                b_ii));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:213' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:214' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:215' else */
              /* 'LARTimings:216' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          } else {
            /* 'LARTimings:218' else */
            /* 'LARTimings:219' len1 = 0; */
            exc_int = 0;

            /* 'LARTimings:220' ind2 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:221' 	                    LARtraj.TOF080(:,5) == 1000); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              dwork->d_x[i_0] = (((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                 (LARtraj_TOF080[32000 + i_0] == 1000));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:222' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:223' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:224' else */
              /* 'LARTimings:225' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          }

          /* 'LARTimings:228' ind1_valid = uint8(len1>0); */
          ind1_valid = (uint8_T)((exc_int > 0) ? 1 : 0);

          /* 'LARTimings:229' ind2_valid = uint8(len2>0); */
          ind2_valid = (uint8_T)((idx > 0) ? 1 : 0);

          /* 'LARTimings:230' if ((~ind1_valid && ind2_valid && head == 30) || ~bitget(tgtvalid,2)) */
          if ((((((int32_T)ind1_valid) == 0) && (((int32_T)ind2_valid) != 0)) &&
               (head == 30)) || ((tgtvalid & 2U) == 0U)) {
            /* 'LARTimings:231' ratioA = 1.0; */
            ent = 1.0;

            /* 'LARTimings:232' dataMAT2(1:len2,:) = LARtraj.TOF080(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          } else if ((((int32_T)ind1_valid) == 0) || (((int32_T)ind2_valid) == 0))
          {
            /* 'LARTimings:233' elseif (~ind1_valid || ~ind2_valid) */
            /* 'LARTimings:234' ti1tmp(inc) = 0.0; */
            ti1tmp[1] = 0.0;
          } else {
            /* 'LARTimings:235' else */
            /* 'LARTimings:236' ratioA = double(head-LARtraj.TOF080(ind1(1),5))/30.0; */
            i_0 = ((int32_T)head) - ((int32_T)LARtraj_TOF080[dwork->ind1_data[0]
              + 31999]);
            if (i_0 > 32767) {
              i_0 = 32767;
            } else {
              if (i_0 < -32768) {
                i_0 = -32768;
              }
            }

            ent = ((real_T)i_0) / 30.0;

            /* 'LARTimings:237' dataMAT1(1:len1,:) = LARtraj.TOF080(ind1,:); */
            /* 'LARTimings:238' dataMAT2(1:len2,:) = LARtraj.TOF080(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < ind1_size_idx_0; i++) {
                dwork->dataMAT1[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind1_data[i])) - 1];
              }

              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          }
        }
      } else {
        /* 'LARTimings:241' else */
        /* 'LARTimings:242' ratio = 10.0*(mach-0.8); */
        ratio = (mach - 0.8) * 10.0;

        /* 'LARTimings:243' if (inc == 1) */
        if ((1 + lar_int) == 1) {
          /* 'LARTimings:244' if (bitget(tgtvalid,2)) */
          if ((tgtvalid & 2U) != 0U) {
            /* 'LARTimings:245' ind1 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:246' 	                    LARtraj.TOF080(:,5) >= head-30 & LARtraj.TOF080(:,5) < head); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) - 30;
            if (b_ii < -32768) {
              b_ii = -32768;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF080[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= b_ii)) && (d_x_tmp_0 < ((int32_T)
                head)));
            }

            exc_int = 0;
            idx = 0;
            exitg1 = false;
            while ((!exitg1) && (idx < 8000)) {
              if (dwork->d_x[idx]) {
                exc_int++;
                dwork->ii_data[exc_int - 1] = idx + 1;
                if (exc_int >= 8000) {
                  exitg1 = true;
                } else {
                  idx++;
                }
              } else {
                idx++;
              }
            }

            if (1 > exc_int) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = exc_int;
            }

            ind1_size_idx_0 = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind1_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:247' if (isempty(ind1)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:248' len1 = 0; */
              exc_int = 0;
            } else {
              /* 'LARTimings:249' else */
              /* 'LARTimings:250' len1 = length(ind1); */
              exc_int = d_x_tmp;
            }

            /* 'LARTimings:252' ind2 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:253' 	                    LARtraj.TOF080(:,5) >= head    & LARtraj.TOF080(:,5) < head+30); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) + 30;
            if (b_ii > 32767) {
              b_ii = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF080[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= ((int32_T)head))) && (d_x_tmp_0 <
                b_ii));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:254' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:255' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:256' else */
              /* 'LARTimings:257' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          } else {
            /* 'LARTimings:259' else */
            /* 'LARTimings:260' len1 = 0; */
            exc_int = 0;

            /* 'LARTimings:261' ind2 = find(LARtraj.TOF080(:,4) >  dive-1  & LARtraj.TOF080(:,4) < dive+1 & ... */
            /* 'LARTimings:262' 	                    LARtraj.TOF080(:,5) == 1000); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF080[24000 + i_0];
              dwork->d_x[i_0] = (((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                 (LARtraj_TOF080[32000 + i_0] == 1000));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:263' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:264' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:265' else */
              /* 'LARTimings:266' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          }

          /* 'LARTimings:269' ind1_valid = uint8(len1>0); */
          ind1_valid = (uint8_T)((exc_int > 0) ? 1 : 0);

          /* 'LARTimings:270' ind2_valid = uint8(len2>0); */
          ind2_valid = (uint8_T)((idx > 0) ? 1 : 0);

          /* 'LARTimings:271' if ((~ind1_valid && ind2_valid && head == 30) || ~bitget(tgtvalid,2)) */
          if ((((((int32_T)ind1_valid) == 0) && (((int32_T)ind2_valid) != 0)) &&
               (head == 30)) || ((tgtvalid & 2U) == 0U)) {
            /* 'LARTimings:272' ratioA = 1.0; */
            ent = 1.0;

            /* 'LARTimings:273' dataMAT2(1:len2,:) = LARtraj.TOF080(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          } else if ((((int32_T)ind1_valid) == 0) || (((int32_T)ind2_valid) == 0))
          {
            /* 'LARTimings:274' elseif (~ind1_valid || ~ind2_valid) */
            /* 'LARTimings:275' ti1tmp(inc) = 0.0; */
            ti1tmp[0] = 0.0;
          } else {
            /* 'LARTimings:276' else */
            /* 'LARTimings:277' ratioA = double(head-LARtraj.TOF080(ind1(1),5))/30.0; */
            i_0 = ((int32_T)head) - ((int32_T)LARtraj_TOF080[dwork->ind1_data[0]
              + 31999]);
            if (i_0 > 32767) {
              i_0 = 32767;
            } else {
              if (i_0 < -32768) {
                i_0 = -32768;
              }
            }

            ent = ((real_T)i_0) / 30.0;

            /* 'LARTimings:278' dataMAT1(1:len1,:) = LARtraj.TOF080(ind1,:); */
            /* 'LARTimings:279' dataMAT2(1:len2,:) = LARtraj.TOF080(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < ind1_size_idx_0; i++) {
                dwork->dataMAT1[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind1_data[i])) - 1];
              }

              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF080[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          }
        } else {
          /* 'LARTimings:281' else */
          /* 'LARTimings:282' if (bitget(tgtvalid,2)) */
          if ((tgtvalid & 2U) != 0U) {
            /* 'LARTimings:283' ind1 = find(LARtraj.TOF090(:,4) >  dive-1  & LARtraj.TOF090(:,4) < dive+1 & ... */
            /* 'LARTimings:284' 	                    LARtraj.TOF090(:,5) >= head-30 & LARtraj.TOF090(:,5) < head); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) - 30;
            if (b_ii < -32768) {
              b_ii = -32768;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF090[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF090[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= b_ii)) && (d_x_tmp_0 < ((int32_T)
                head)));
            }

            exc_int = 0;
            idx = 0;
            exitg1 = false;
            while ((!exitg1) && (idx < 8000)) {
              if (dwork->d_x[idx]) {
                exc_int++;
                dwork->ii_data[exc_int - 1] = idx + 1;
                if (exc_int >= 8000) {
                  exitg1 = true;
                } else {
                  idx++;
                }
              } else {
                idx++;
              }
            }

            if (1 > exc_int) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = exc_int;
            }

            ind1_size_idx_0 = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind1_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:285' if (isempty(ind1)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:286' len1 = 0; */
              exc_int = 0;
            } else {
              /* 'LARTimings:287' else */
              /* 'LARTimings:288' len1 = length(ind1); */
              exc_int = d_x_tmp;
            }

            /* 'LARTimings:290' ind2 = find(LARtraj.TOF090(:,4) >  dive-1  & LARtraj.TOF090(:,4) < dive+1 & ... */
            /* 'LARTimings:291' 	                    LARtraj.TOF090(:,5) >= head    & LARtraj.TOF090(:,5) < head+30); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            b_ii = ((int32_T)head) + 30;
            if (b_ii > 32767) {
              b_ii = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF090[24000 + i_0];
              d_x_tmp_0 = (int32_T)LARtraj_TOF090[32000 + i_0];
              dwork->d_x[i_0] = ((((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                  (d_x_tmp_0 >= ((int32_T)head))) && (d_x_tmp_0 <
                b_ii));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:292' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:293' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:294' else */
              /* 'LARTimings:295' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          } else {
            /* 'LARTimings:297' else */
            /* 'LARTimings:298' len1 = 0; */
            exc_int = 0;

            /* 'LARTimings:299' ind2 = find(LARtraj.TOF090(:,4) >  dive-1  & LARtraj.TOF090(:,4) < dive+1 & ... */
            /* 'LARTimings:300' 	                    LARtraj.TOF090(:,5) == 1000); */
            idx = ((int32_T)tmp_0) - 1;
            if (idx < -32768) {
              idx = -32768;
            }

            dive = ((int32_T)tmp_0) + 1;
            if (dive > 32767) {
              dive = 32767;
            }

            for (i_0 = 0; i_0 < 8000; i_0++) {
              d_x_tmp = (int32_T)LARtraj_TOF090[24000 + i_0];
              dwork->d_x[i_0] = (((d_x_tmp > idx) && (d_x_tmp < dive)) &&
                                 (LARtraj_TOF090[32000 + i_0] == 1000));
            }

            idx = 0;
            b_ii = 0;
            exitg1 = false;
            while ((!exitg1) && (b_ii < 8000)) {
              if (dwork->d_x[b_ii]) {
                idx++;
                dwork->ii_data[idx - 1] = b_ii + 1;
                if (idx >= 8000) {
                  exitg1 = true;
                } else {
                  b_ii++;
                }
              } else {
                b_ii++;
              }
            }

            if (1 > idx) {
              d_x_tmp = 0;
            } else {
              d_x_tmp = idx;
            }

            b_ii = d_x_tmp;
            for (i_0 = 0; i_0 < d_x_tmp; i_0++) {
              dwork->ind2_data[i_0] = (int16_T)dwork->ii_data[i_0];
            }

            /* 'LARTimings:301' if (isempty(ind2)) */
            if (d_x_tmp == 0) {
              /* 'LARTimings:302' len2 = 0; */
              idx = 0;
            } else {
              /* 'LARTimings:303' else */
              /* 'LARTimings:304' len2 = length(ind2); */
              idx = d_x_tmp;
            }
          }

          /* 'LARTimings:307' ind1_valid = uint8(len1>0); */
          ind1_valid = (uint8_T)((exc_int > 0) ? 1 : 0);

          /* 'LARTimings:308' ind2_valid = uint8(len2>0); */
          ind2_valid = (uint8_T)((idx > 0) ? 1 : 0);

          /* 'LARTimings:309' if ((~ind1_valid && ind2_valid && head == 30) || ~bitget(tgtvalid,2)) */
          if ((((((int32_T)ind1_valid) == 0) && (((int32_T)ind2_valid) != 0)) &&
               (head == 30)) || ((tgtvalid & 2U) == 0U)) {
            /* 'LARTimings:310' ratioA = 1.0; */
            ent = 1.0;

            /* 'LARTimings:311' dataMAT2(1:len2,:) = LARtraj.TOF090(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF090[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          } else if ((((int32_T)ind1_valid) == 0) || (((int32_T)ind2_valid) == 0))
          {
            /* 'LARTimings:312' elseif (~ind1_valid || ~ind2_valid) */
            /* 'LARTimings:313' ti1tmp(inc) = 0.0; */
            ti1tmp[1] = 0.0;
          } else {
            /* 'LARTimings:314' else */
            /* 'LARTimings:315' ratioA = double(head-LARtraj.TOF090(ind1(1),5))/30.0; */
            i_0 = ((int32_T)head) - ((int32_T)LARtraj_TOF090[dwork->ind1_data[0]
              + 31999]);
            if (i_0 > 32767) {
              i_0 = 32767;
            } else {
              if (i_0 < -32768) {
                i_0 = -32768;
              }
            }

            ent = ((real_T)i_0) / 30.0;

            /* 'LARTimings:316' dataMAT1(1:len1,:) = LARtraj.TOF090(ind1,:); */
            /* 'LARTimings:317' dataMAT2(1:len2,:) = LARtraj.TOF090(ind2,:); */
            for (i_0 = 0; i_0 < 24; i_0++) {
              for (i = 0; i < ind1_size_idx_0; i++) {
                dwork->dataMAT1[i + (8000 * i_0)] = LARtraj_TOF090[((8000 * i_0)
                  + ((int32_T)dwork->ind1_data[i])) - 1];
              }

              for (i = 0; i < b_ii; i++) {
                dwork->dataMAT2[i + (8000 * i_0)] = LARtraj_TOF090[((8000 * i_0)
                  + ((int32_T)dwork->ind2_data[i])) - 1];
              }
            }
          }
        }
      }

      /* 'LARTimings:321' if (ind1_valid) */
      if (((int32_T)ind1_valid) != 0) {
        /* 'LARTimings:327' dTgt2(1:len1) =                 (int32(dataMAT1(1:len1,1))-int32(0.1*TgtX)).^2; */
        if (1 > exc_int) {
          i_0 = -1;
        } else {
          i_0 = exc_int - 1;
        }

        b_ii = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * lard1);
        if (exd1 < 2.147483648E+9) {
          if (exd1 >= -2.147483648E+9) {
            i = (int32_T)exd1;
          } else {
            i = MIN_int32_T;
          }
        } else {
          i = MAX_int32_T;
        }

        if (1 > exc_int) {
          dive = -1;
        } else {
          dive = exc_int - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          if ((dwork->dataMAT1[i_0] >= 0) && (i < (((int32_T)dwork->dataMAT1[i_0])
                - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((dwork->dataMAT1[i_0] < 0) && (i > (((int32_T)
                        dwork->dataMAT1[i_0]) - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = ((int32_T)dwork->dataMAT1[i_0]) - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        if (0 <= (b_ii - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->tmp_data[0], ((uint32_T)b_ii) *
                 (sizeof(int32_T)));
        }

        /* 'LARTimings:328' dTgt2(1:len1) = dTgt2(1:len1) + (int32(dataMAT1(1:len1,2))-int32(0.1*TgtY)).^2; */
        if (1 > exc_int) {
          i_0 = -1;
        } else {
          i_0 = exc_int - 1;
        }

        b_ii = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * lard2);
        if (exd1 < 2.147483648E+9) {
          i = (int32_T)exd1;
        } else {
          i = MAX_int32_T;
        }

        if (1 > exc_int) {
          dive = -1;
        } else {
          dive = exc_int - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          d_x_tmp = (int32_T)dwork->dataMAT1[8000 + i_0];
          if ((d_x_tmp >= 0) && (i < (d_x_tmp - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((d_x_tmp < 0) && (i > (d_x_tmp - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = d_x_tmp - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          if ((dwork->dTgt2[i_0] < 0) && (dwork->tmp_data[i_0] < (MIN_int32_T
                - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else if ((dwork->dTgt2[i_0] > 0) && (dwork->tmp_data[i_0] >
                      (MAX_int32_T - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = dwork->dTgt2[i_0] + dwork->tmp_data[i_0];
          }
        }

        if (0 <= (b_ii - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->dataMAT1_data[0], ((uint32_T)b_ii) *
                 (sizeof(int32_T)));
        }

        /* 'LARTimings:329' dTgt2(1:len1) = dTgt2(1:len1) + (int32(double(dataMAT1(1:len1,3))*0.1)-int32(0.1*relenu(3))).^2; */
        if (1 > exc_int) {
          i_0 = -1;
        } else {
          i_0 = exc_int - 1;
        }

        b_ii = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * relenu[2]);
        if (exd1 < 2.147483648E+9) {
          if (exd1 >= -2.147483648E+9) {
            i = (int32_T)exd1;
          } else {
            i = MIN_int32_T;
          }
        } else {
          i = MAX_int32_T;
        }

        if (1 > exc_int) {
          dive = -1;
        } else {
          dive = exc_int - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          d_x_tmp = (int32_T)rt_roundd_snf(((real_T)dwork->dataMAT1[16000 + i_0])
            * 0.1);
          if ((d_x_tmp >= 0) && (i < (d_x_tmp - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((d_x_tmp < 0) && (i > (d_x_tmp - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = d_x_tmp - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          if ((dwork->dTgt2[i_0] < 0) && (dwork->tmp_data[i_0] < (MIN_int32_T
                - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else if ((dwork->dTgt2[i_0] > 0) && (dwork->tmp_data[i_0] >
                      (MAX_int32_T - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = dwork->dTgt2[i_0] + dwork->tmp_data[i_0];
          }
        }

        if (0 <= (b_ii - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->dataMAT1_data[0], ((uint32_T)b_ii) *
                 (sizeof(int32_T)));
        }

        /* 'LARTimings:330' [dval,dind] = sort(dTgt2(1:len1)); */
        if (1 > exc_int) {
          dive = -1;
        } else {
          dive = exc_int - 1;
        }

        d_x_tmp = dive + 1;
        if (0 <= dive) {
          memcpy(&dwork->ii_data[0], &dwork->dTgt2[0], ((uint32_T)((int32_T)
                   (dive + 1))) * (sizeof(int32_T)));
        }

        sort(dwork->ii_data, &d_x_tmp, dwork->tmp_data, &d_x_tmp_0, dwork);

        /* 'LARTimings:332' dvec = sqrt(double(dval(1:10))); */
        for (i_0 = 0; i_0 < 10; i_0++) {
          dvec[i_0] = (real_T)dwork->ii_data[i_0];
        }

        sqrt_pNDOZDUg(dvec);

        /* 'LARTimings:333' indp = find(dvec < 2*dvec(1)); */
        for (i = 0; i < 10; i++) {
          e_x[i] = (dvec[i] < (2.0 * dvec[0]));
        }

        exc_int = 0;
        b_ii = 0;
        exitg1 = false;
        while ((!exitg1) && (b_ii < 10)) {
          if (e_x[b_ii]) {
            exc_int++;
            n_ii_data[exc_int - 1] = (int8_T)(b_ii + 1);
            if (exc_int >= 10) {
              exitg1 = true;
            } else {
              b_ii++;
            }
          } else {
            b_ii++;
          }
        }

        if (1 > exc_int) {
          exc_int = 0;
        }

        b_ii = exc_int;

        /* 'LARTimings:335' if (isempty('indp')) */
        /* 'LARTimings:337' else */
        /* 'LARTimings:338' if (indp(end) > 10) */
        if (n_ii_data[exc_int - 1] < 3) {
          /* 'LARTimings:340' elseif (indp(end) < 3) */
          /* 'LARTimings:341' indp = (1:3)'; */
          b_ii = 3;
          n_ii_data[0] = 1;
          n_ii_data[1] = 2;
          n_ii_data[2] = 3;
        }

        /* 'LARTimings:344' tmp = ones(10,1); */
        for (i = 0; i < 10; i++) {
          tmp[i] = 1.0;
        }

        /* 'LARTimings:345' den = norm(dvec(indp)); */
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          dvec_data[i_0] = dvec[n_ii_data[i_0] - 1];
        }

        exd = norm(dvec_data, &b_ii);

        /* 'LARTimings:346' for i = 1:indp(end) */
        exc_int = 0;
        do {
          exitg2 = 0;
          i_0 = ((int32_T)n_ii_data[b_ii - 1]) - 1;
          if (exc_int <= i_0) {
            /* 'LARTimings:347' tmp(i) = den/dvec(i); */
            tmp[exc_int] = exd / dvec[exc_int];
            exc_int++;
          } else {
            exitg2 = 1;
          }
        } while (exitg2 == 0);

        /* 'LARTimings:349' ratvec = ones(10,1); */
        for (i = 0; i < 10; i++) {
          dvec[i] = 1.0;
        }

        /* 'LARTimings:350' sumtmp = sum(tmp(indp)); */
        for (i = 0; i < b_ii; i++) {
          dvec_data[i] = tmp[n_ii_data[i] - 1];
        }

        exd = sum(dvec_data, &b_ii);

        /* 'LARTimings:351' for i = 1:indp(end) */
        for (exc_int = 0; exc_int <= i_0; exc_int++) {
          /* 'LARTimings:352' ratvec(i) = tmp(i)/sumtmp; */
          dvec[exc_int] = tmp[exc_int] / exd;
        }

        /* 'LARTimings:354' ti1tmp1(inc) = sum(ratvec(indp).*double(dataMAT1(dind(indp),6))); */
        d_x_tmp = b_ii;
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          exc_int = ((int32_T)n_ii_data[i_0]) - 1;
          dvec_data[i_0] = ((real_T)dwork->dataMAT1[dwork->tmp_data[exc_int] +
                            39999]) * dvec[exc_int];
        }

        ti1tmp1[lar_int] = sum(dvec_data, &b_ii);

        /* 'LARTimings:355' if (return_traj == 2) */
      }

      /* 'LARTimings:374' if (ind2_valid) */
      if (((int32_T)ind2_valid) != 0) {
        /* 'LARTimings:376' dTgt2(1:len2) =                 (int32(dataMAT2(1:len2,1))-int32(0.1*TgtX)).^2; */
        if (1 > idx) {
          i_0 = -1;
        } else {
          i_0 = idx - 1;
        }

        exc_int = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * lard1);
        if (exd1 < 2.147483648E+9) {
          if (exd1 >= -2.147483648E+9) {
            i = (int32_T)exd1;
          } else {
            i = MIN_int32_T;
          }
        } else {
          i = MAX_int32_T;
        }

        if (1 > idx) {
          dive = -1;
        } else {
          dive = idx - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          if ((dwork->dataMAT2[i_0] >= 0) && (i < (((int32_T)dwork->dataMAT2[i_0])
                - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((dwork->dataMAT2[i_0] < 0) && (i > (((int32_T)
                        dwork->dataMAT2[i_0]) - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = ((int32_T)dwork->dataMAT2[i_0]) - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        if (0 <= (exc_int - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->tmp_data[0], ((uint32_T)exc_int) *
                 (sizeof(int32_T)));
        }

        /* 'LARTimings:377' dTgt2(1:len2) = dTgt2(1:len2) + (int32(dataMAT2(1:len2,2))-int32(0.1*TgtY)).^2; */
        if (1 > idx) {
          i_0 = -1;
        } else {
          i_0 = idx - 1;
        }

        exc_int = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * lard2);
        if (exd1 < 2.147483648E+9) {
          i = (int32_T)exd1;
        } else {
          i = MAX_int32_T;
        }

        if (1 > idx) {
          dive = -1;
        } else {
          dive = idx - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          d_x_tmp = (int32_T)dwork->dataMAT2[8000 + i_0];
          if ((d_x_tmp >= 0) && (i < (d_x_tmp - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((d_x_tmp < 0) && (i > (d_x_tmp - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = d_x_tmp - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        for (i_0 = 0; i_0 < exc_int; i_0++) {
          if ((dwork->dTgt2[i_0] < 0) && (dwork->tmp_data[i_0] < (MIN_int32_T
                - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else if ((dwork->dTgt2[i_0] > 0) && (dwork->tmp_data[i_0] >
                      (MAX_int32_T - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = dwork->dTgt2[i_0] + dwork->tmp_data[i_0];
          }
        }

        if (0 <= (exc_int - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->dataMAT1_data[0], ((uint32_T)exc_int)
                 * (sizeof(int32_T)));
        }

        /* 'LARTimings:378' dTgt2(1:len2) = dTgt2(1:len2) + (int32(double(dataMAT2(1:len2,3))*0.1)-int32(0.1*relenu(3))).^2; */
        if (1 > idx) {
          i_0 = -1;
        } else {
          i_0 = idx - 1;
        }

        exc_int = i_0 + 1;
        exd1 = rt_roundd_snf(0.1 * relenu[2]);
        if (exd1 < 2.147483648E+9) {
          if (exd1 >= -2.147483648E+9) {
            i = (int32_T)exd1;
          } else {
            i = MIN_int32_T;
          }
        } else {
          i = MAX_int32_T;
        }

        if (1 > idx) {
          dive = -1;
        } else {
          dive = idx - 1;
        }

        d_x_tmp_0 = dive + 1;
        for (i_0 = 0; i_0 <= dive; i_0++) {
          d_x_tmp = (int32_T)rt_roundd_snf(((real_T)dwork->dataMAT2[16000 + i_0])
            * 0.1);
          if ((d_x_tmp >= 0) && (i < (d_x_tmp - MAX_int32_T))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else if ((d_x_tmp < 0) && (i > (d_x_tmp - MIN_int32_T))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = d_x_tmp - i;
          }
        }

        power(dwork->dataMAT1_data, &d_x_tmp_0, dwork->tmp_data, &d_x_tmp, dwork);
        for (i_0 = 0; i_0 < exc_int; i_0++) {
          if ((dwork->dTgt2[i_0] < 0) && (dwork->tmp_data[i_0] < (MIN_int32_T
                - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MIN_int32_T;
          } else if ((dwork->dTgt2[i_0] > 0) && (dwork->tmp_data[i_0] >
                      (MAX_int32_T - dwork->dTgt2[i_0]))) {
            dwork->dataMAT1_data[i_0] = MAX_int32_T;
          } else {
            dwork->dataMAT1_data[i_0] = dwork->dTgt2[i_0] + dwork->tmp_data[i_0];
          }
        }

        if (0 <= (exc_int - 1)) {
          memcpy(&dwork->dTgt2[0], &dwork->dataMAT1_data[0], ((uint32_T)exc_int)
                 * (sizeof(int32_T)));
        }

        /* 'LARTimings:379' [dval,dind] = sort(dTgt2(1:len2)); */
        if (1 > idx) {
          dive = -1;
        } else {
          dive = idx - 1;
        }

        d_x_tmp = dive + 1;
        if (0 <= dive) {
          memcpy(&dwork->ii_data[0], &dwork->dTgt2[0], ((uint32_T)((int32_T)
                   (dive + 1))) * (sizeof(int32_T)));
        }

        sort(dwork->ii_data, &d_x_tmp, dwork->tmp_data, &d_x_tmp_0, dwork);

        /* 'LARTimings:381' dvec = sqrt(double(dval(1:10))); */
        for (i_0 = 0; i_0 < 10; i_0++) {
          dvec[i_0] = (real_T)dwork->ii_data[i_0];
        }

        sqrt_pNDOZDUg(dvec);

        /* 'LARTimings:382' indp = find(dvec < 2*dvec(1)); */
        for (i = 0; i < 10; i++) {
          e_x[i] = (dvec[i] < (2.0 * dvec[0]));
        }

        exc_int = 0;
        idx = 0;
        exitg1 = false;
        while ((!exitg1) && (idx < 10)) {
          if (e_x[idx]) {
            exc_int++;
            n_ii_data[exc_int - 1] = (int8_T)(idx + 1);
            if (exc_int >= 10) {
              exitg1 = true;
            } else {
              idx++;
            }
          } else {
            idx++;
          }
        }

        if (1 > exc_int) {
          exc_int = 0;
        }

        b_ii = exc_int;

        /* 'LARTimings:384' if (isempty('indp')) */
        /* 'LARTimings:386' else */
        /* 'LARTimings:387' if (indp(end) > 10) */
        if (n_ii_data[exc_int - 1] < 3) {
          /* 'LARTimings:389' elseif (indp(end) < 3) */
          /* 'LARTimings:390' indp = (1:3)'; */
          b_ii = 3;
          n_ii_data[0] = 1;
          n_ii_data[1] = 2;
          n_ii_data[2] = 3;
        }

        /* 'LARTimings:393' tmp = ones(10,1); */
        for (i = 0; i < 10; i++) {
          tmp[i] = 1.0;
        }

        /* 'LARTimings:394' den = norm(dvec(indp)); */
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          dvec_data[i_0] = dvec[n_ii_data[i_0] - 1];
        }

        exd = norm(dvec_data, &b_ii);

        /* 'LARTimings:395' for i = 1:indp(end) */
        exc_int = 0;
        do {
          exitg2 = 0;
          i_0 = ((int32_T)n_ii_data[b_ii - 1]) - 1;
          if (exc_int <= i_0) {
            /* 'LARTimings:396' tmp(i) = den/dvec(i); */
            tmp[exc_int] = exd / dvec[exc_int];
            exc_int++;
          } else {
            exitg2 = 1;
          }
        } while (exitg2 == 0);

        /* 'LARTimings:398' ratvec = ones(10,1); */
        for (i = 0; i < 10; i++) {
          dvec[i] = 1.0;
        }

        /* 'LARTimings:399' sumtmp = sum(tmp(indp)); */
        for (i = 0; i < b_ii; i++) {
          dvec_data[i] = tmp[n_ii_data[i] - 1];
        }

        exd = sum(dvec_data, &b_ii);

        /* 'LARTimings:400' for i = 1:indp(end) */
        for (exc_int = 0; exc_int <= i_0; exc_int++) {
          /* 'LARTimings:401' ratvec(i) = tmp(i)/sumtmp; */
          dvec[exc_int] = tmp[exc_int] / exd;
        }

        /* 'LARTimings:403' ti1tmp2(inc) = sum(ratvec(indp).*double(dataMAT2(dind(indp),6))); */
        for (i_0 = 0; i_0 < b_ii; i_0++) {
          exc_int = ((int32_T)n_ii_data[i_0]) - 1;
          dvec_data[i_0] = ((real_T)dwork->dataMAT2[dwork->tmp_data[exc_int] +
                            39999]) * dvec[exc_int];
        }

        ti1tmp2[lar_int] = sum(dvec_data, &b_ii);

        /* 'LARTimings:404' if (return_traj == 2) */
        /* 'LARTimings:422' posNtmp(:,inc) = (1.0-ratioA)*posNtmp1(:,inc) + ratioA*posNtmp2(:,inc); */
        /* 'LARTimings:423' posEtmp(:,inc) = (1.0-ratioA)*posEtmp1(:,inc) + ratioA*posEtmp2(:,inc); */
        /* 'LARTimings:424' gndRtmp(inc)   = (1.0-ratioA)*gndRtmp1(inc)   + ratioA*gndRtmp2(inc); */
        /* 'LARTimings:425' ti1tmp(inc)    = (1.0-ratioA)*ti1tmp1(inc)    + ratioA*ti1tmp2(inc); */
        ti1tmp[lar_int] = ((1.0 - ent) * ti1tmp1[lar_int]) + (ent *
          ti1tmp2[lar_int]);
      }
    }

    /* 'LARTimings:430' posN = (1.0-ratio)*posNtmp(:,1) + ratio*posNtmp(:,2); */
    /* 'LARTimings:431' posE = (1.0-ratio)*posEtmp(:,1) + ratio*posEtmp(:,2); */
    /* 'LARTimings:432' gndR = (1.0-ratio)*gndRtmp(1)   + ratio*gndRtmp(2); */
    /* 'LARTimings:433' ti1  = (1.0-ratio)*ti1tmp(1)    + ratio*ti1tmp(2); */
    /* 'LARTimings:436' ct.ti  = ti1; */
    ent = ((1.0 - ratio) * ti1tmp[0]) + (ratio * ti1tmp[1]);
    ct->ti = ent;

    /* 'LARTimings:439' ct.ts  = max([0,ct.ti-45]); */
    if (0.0 < (ent - 45.0)) {
      ct->ts = ent - 45.0;
    } else {
      ct->ts = 0.0;
    }

    /* 'LARTimings:441' traj.N = posN; */
    /* 'LARTimings:442' traj.E = posE; */
    for (i_0 = 0; i_0 < 11; i_0++) {
      lard1 = ((1.0 - ratio) * 0.0) + (ratio * 0.0);
      traj->N[i_0] = lard1;
      traj->E[i_0] = lard1;
    }

    /* 'LARTimings:443' traj.groundrange = gndR; */
    traj->groundrange = ((1.0 - ratio) * 0.0) + (ratio * 0.0);
  }
}

/*
 * Function for MATLAB Function: '<S1>/Data Controller'
 * function [lar] = LARCalcs(launch,tgt,LARdata,LARtraj,return_traj)
 */
static void LARCalcs(real_T launch_alt, real_T launch_rex, real_T launch_rey,
                     real_T launch_rez, real_T launch_mach, real_T launch_track,
                     real_T launch_wind_spd, real_T launch_wind_dir, uint32_T
                     tgt_valid, real_T tgt_rex, real_T tgt_rey, real_T tgt_rez,
                     real_T tgt_alt, real_T tgt_dive, real_T tgt_arrive, const
                     b_sYbzGJxaGNvEV1Jh0D0xgEF *LARdata, const
                     T_Weapon_MCP_GlobalMemory_LARtraj *LARtraj,
                     sW6z4ntaPZV6RXLllkgQXpF *lar, DW *dwork)
{
  real_T relenu[3];
  real_T px1[5];
  real_T py1[5];
  real_T lr1;
  real_T larn;
  real_T lare;
  real_T RelBearLAR;
  real_T sinphi;
  real_T cosphi;
  real_T sinlam;
  real_T coslam;
  real_T R[9];
  real_T tmpenu[3];
  real_T b_cosphi;
  int32_T r1;
  int32_T r2;
  int32_T r3;
  int32_T rtemp;
  stE9DGVPdbGPhyouEV4GctG ct_data;
  snFb1SHWmbC64DSgfqhsaYG traj_data;
  real_T RelBearLAR_0[5];
  real_T lr1_0[5];
  real_T R_0;
  real_T tmpxyz_idx_0;
  real_T larn_tmp;

  /* 'LARCalcs:53' Orgecef.x = tgt.rex; */
  /* 'LARCalcs:53' Orgecef.y = tgt.rey; */
  /* 'LARCalcs:53' Orgecef.z = tgt.rez; */
  /* 'LARCalcs:56' relecef.x = launch.rex; */
  /* 'LARCalcs:56' relecef.y = launch.rey; */
  /* 'LARCalcs:56' relecef.z = launch.rez; */
  /* 'LARCalcs:57' tempout = LARecef2enu(relecef,Orgecef); */
  /* 'LARecef2enu:26' tmpxyz = zeros(1,3); */
  /* 'LARecef2enu:27' tmpxyz(1) = ecefin.x; */
  /* 'LARecef2enu:28' tmpxyz(2) = ecefin.y; */
  /* 'LARecef2enu:29' tmpxyz(3) = ecefin.z; */
  /* 'LARecef2enu:30' tmporg = zeros(1,3); */
  /* 'LARecef2enu:31' tmporg(1) = orgxyz.x; */
  /* 'LARecef2enu:32' tmporg(2) = orgxyz.y; */
  /* 'LARecef2enu:33' tmporg(3) = orgxyz.z; */
  /* 'LARecef2enu:34' difxyz = tmpxyz - tmporg; */
  /* 'LARecef2enu:36' orglla = LIB_ecef2lla(orgxyz); */
  LIB_ecef2lla(tgt_rex, tgt_rey, tgt_rez, &b_cosphi, &larn, &lare);

  /* 'LARecef2enu:40' sinphi = sin(orglla.lat); */
  sinphi = sin(b_cosphi);

  /* 'LARecef2enu:41' cosphi = cos(orglla.lat); */
  lr1 = cos(b_cosphi);

  /* 'LARecef2enu:42' sinlam = sin(orglla.lon); */
  sinlam = sin(larn);

  /* 'LARecef2enu:43' coslam = cos(orglla.lon); */
  coslam = cos(larn);

  /* 'LARecef2enu:44' R = [ -sinlam          coslam         0     ; ... */
  /* 'LARecef2enu:45'       -sinphi*coslam  -sinphi*sinlam  cosphi; ... */
  /* 'LARecef2enu:46'        cosphi*coslam   cosphi*sinlam  sinphi]; */
  /* 'LARecef2enu:47' ENU.x = R(1,:)*difxyz'; */
  /* 'LARecef2enu:48' ENU.y = R(2,:)*difxyz'; */
  /* 'LARecef2enu:49' ENU.z = R(3,:)*difxyz'; */
  /* 'LARCalcs:58' relenu = zeros(3,1); */
  /* 'LARCalcs:59' relenu(1,1) = tempout.x; */
  cosphi = launch_rex - tgt_rex;
  R_0 = (-sinlam) * cosphi;
  tmpxyz_idx_0 = cosphi;
  cosphi = launch_rey - tgt_rey;
  R_0 += coslam * cosphi;
  larn = cosphi;
  cosphi = launch_rez - tgt_rez;
  R_0 += 0.0 * cosphi;
  relenu[0] = R_0;

  /* 'LARCalcs:60' relenu(2,1) = tempout.y; */
  b_cosphi = ((((-sinphi) * coslam) * tmpxyz_idx_0) + (((-sinphi) * sinlam) *
    larn)) + (lr1 * cosphi);
  relenu[1] = b_cosphi;

  /* 'LARCalcs:61' relenu(3,1) = tempout.z; */
  relenu[2] = (((lr1 * coslam) * tmpxyz_idx_0) + ((lr1 * sinlam) * larn)) +
    (sinphi * cosphi);

  /* 'LARCalcs:64' if (bitget(tgt.valid,1)) */
  if ((tgt_valid & 1U) != 0U) {
    /* 'LARCalcs:65' if (bitget(tgt.valid,2)) */
    if ((tgt_valid & 2U) != 0U) {
      /* 'LARCalcs:67' relang = tgt.arrive - launch.track*180/pi; */
      sinphi = tgt_arrive - ((launch_track * 180.0) / 3.1415926535897931);
    } else {
      /* 'LARCalcs:68' else */
      /* 'LARCalcs:70' tgt.arrive = atan2(relenu(2,1),relenu(1,1))*180/pi; */
      /* 'LARCalcs:71' relang = tgt.arrive - launch.track*180/pi; */
      sinphi = ((rt_atan2d_snf(b_cosphi, R_0) * 180.0) / 3.1415926535897931) -
        ((launch_track * 180.0) / 3.1415926535897931);
    }
  } else {
    /* 'LARCalcs:73' else */
    /* 'LARCalcs:74' relang = 0; */
    sinphi = 0.0;
  }

  /* 'LARCalcs:76' if (relang > 180) */
  if (sinphi > 180.0) {
    /* 'LARCalcs:77' relang = relang - 360; */
    sinphi -= 360.0;
  } else {
    if (sinphi < -180.0) {
      /* 'LARCalcs:78' elseif (relang < -180) */
      /* 'LARCalcs:79' relang = relang + 360; */
      sinphi += 360.0;
    }
  }

  /* 'LARCalcs:83' [distx,disty,lr,er,Rmax] = LARlookup(tgt.valid,launch.alt-tgt.alt,launch.mach,... */
  /* 'LARCalcs:84' 							         tgt.dive,relang,LARdata); */
  LARlookup(tgt_valid, launch_alt - tgt_alt, launch_mach, tgt_dive, sinphi,
            LARdata, &RelBearLAR, &lr1, &tmpxyz_idx_0, &cosphi, &sinlam);

  /* 'LARCalcs:87' tf = [0,0,0,0,0]; */
  /* 'LARCalcs:90' excl_radius = er; */
  /* 'LARCalcs:91' lar.excl_radius = excl_radius; */
  lar->excl_radius = cosphi;

  /* 'LARCalcs:104' GndDistTgt = sqrt(relenu(1)^2+relenu(2)^2); */
  sinlam = sqrt((R_0 * R_0) + (b_cosphi * b_cosphi));

  /* 'LARCalcs:105' lar.GndDistTgt = GndDistTgt; */
  lar->GndDistTgt = sinlam;

  /* 'LARCalcs:106' RelBearTgt = atan2(-relenu(1),-relenu(2))-launch.track; */
  coslam = rt_atan2d_snf(-R_0, -b_cosphi) - launch_track;

  /* 'LARCalcs:112' if (RelBearTgt < 0.0) */
  if (coslam < 0.0) {
    /* 'LARCalcs:113' RelBearTgt = RelBearTgt + 2*pi; */
    coslam += 6.2831853071795862;
  }

  /* 'LARCalcs:115' lar.RelBearTgt = RelBearTgt; */
  lar->RelBearTgt = coslam;

  /* 'LARCalcs:118' px = [-distx, -distx, -distx+lr, -distx, -distx-lr]; */
  /* 'LARCalcs:119' py = [-disty, -disty+lr, -disty, -disty-lr, -disty]; */
  /* 'LARCalcs:120' wind_factor = 0.5; */
  /* 'LARCalcs:121' ws = launch.wind_spd; */
  /* 'LARCalcs:122' wd = launch.wind_dir; */
  /* 'LARCalcs:123' px1 = px + wind_factor*ws*sin(wd-launch.track)*tf; */
  lare = launch_wind_dir - launch_track;
  larn = (0.5 * launch_wind_spd) * sin(lare);
  RelBearLAR_0[0] = -RelBearLAR;
  RelBearLAR_0[1] = -RelBearLAR;
  RelBearLAR_0[2] = (-RelBearLAR) + tmpxyz_idx_0;
  RelBearLAR_0[3] = -RelBearLAR;
  RelBearLAR_0[4] = (-RelBearLAR) - tmpxyz_idx_0;

  /* 'LARCalcs:124' py1 = py + wind_factor*ws*cos(wd-launch.track)*tf; */
  RelBearLAR = (0.5 * launch_wind_spd) * cos(lare);
  lr1_0[0] = -lr1;
  lr1_0[1] = (-lr1) + tmpxyz_idx_0;
  lr1_0[2] = -lr1;
  lr1_0[3] = (-lr1) - tmpxyz_idx_0;
  lr1_0[4] = -lr1;
  for (r1 = 0; r1 < 5; r1++) {
    px1[r1] = (larn * 0.0) + RelBearLAR_0[r1];
    py1[r1] = (RelBearLAR * 0.0) + lr1_0[r1];
  }

  /* 'LARCalcs:125' lr1 = min([(px1(3)-px1(5))/2,(py1(2)-py1(4))/2]); */
  tmpxyz_idx_0 = (px1[2] - px1[4]) / 2.0;
  RelBearLAR = (py1[1] - py1[3]) / 2.0;
  if (tmpxyz_idx_0 > RelBearLAR) {
    lr1 = RelBearLAR;
  } else if (rtIsNaN(tmpxyz_idx_0)) {
    if (!rtIsNaN(RelBearLAR)) {
      lr1 = RelBearLAR;
    } else {
      lr1 = tmpxyz_idx_0;
    }
  } else {
    lr1 = tmpxyz_idx_0;
  }

  /* 'LARCalcs:126' dist1 = [(px1(3)+px1(5))/2,(py1(2)+py1(4))/2]; */
  tmpxyz_idx_0 = (px1[2] + px1[4]) / 2.0;
  RelBearLAR = (py1[1] + py1[3]) / 2.0;

  /* 'LARCalcs:129' larn = -dist1(1)*sin(launch.track) + dist1(2)*cos(launch.track); */
  lare = cos(launch_track);
  larn_tmp = sin(launch_track);
  larn = ((-tmpxyz_idx_0) * larn_tmp) + (RelBearLAR * lare);

  /* 'LARCalcs:130' lare =  dist1(1)*cos(launch.track) + dist1(2)*sin(launch.track); */
  lare = (tmpxyz_idx_0 * lare) + (RelBearLAR * larn_tmp);

  /* 'LARCalcs:131' radius = lr1; */
  /* 'LARCalcs:132' lar.radius   = radius; */
  lar->radius = lr1;

  /* 'LARCalcs:133' if (radius >= 500.0) */
  if (lr1 >= 500.0) {
    /* 'LARCalcs:134' lar.radius_valid = uint8(1); */
    lar->radius_valid = 1U;
  } else {
    /* 'LARCalcs:135' else */
    /* 'LARCalcs:136' lar.radius_valid = uint8(0); */
    lar->radius_valid = 0U;
  }

  /* 'LARCalcs:140' larenu = [lare,larn,tgt.alt]; */
  /* 'LARCalcs:141' GndDistLAR = sqrt((larenu(1)-relenu(1))^2+(larenu(2)-relenu(2))^2); */
  R_0 = lare - R_0;
  b_cosphi = larn - b_cosphi;
  tmpxyz_idx_0 = sqrt((R_0 * R_0) + (b_cosphi * b_cosphi));

  /* 'LARCalcs:142' lar.GndDistLAR = GndDistLAR; */
  lar->GndDistLAR = tmpxyz_idx_0;

  /* 'LARCalcs:143' RelBearLAR = atan2(larenu(1)-relenu(1),larenu(2)-relenu(2))-launch.track; */
  RelBearLAR = rt_atan2d_snf(R_0, b_cosphi) - launch_track;

  /* 'LARCalcs:149' if (RelBearLAR < 0.0) */
  if (RelBearLAR < 0.0) {
    /* 'LARCalcs:150' RelBearLAR = RelBearLAR + 2*pi; */
    RelBearLAR += 6.2831853071795862;
  }

  /* 'LARCalcs:152' lar.RelBearLAR = RelBearLAR; */
  lar->RelBearLAR = RelBearLAR;

  /* 'LARCalcs:153' lar.RelDistX   = GndDistLAR*sin(RelBearLAR); */
  lar->RelDistX = tmpxyz_idx_0 * sin(RelBearLAR);

  /* 'LARCalcs:154' lar.RelDistY   = GndDistLAR*cos(RelBearLAR); */
  lar->RelDistY = tmpxyz_idx_0 * cos(RelBearLAR);

  /* 'LARCalcs:157' if (sqrt(lare^2 + larn^2) - radius) < excl_radius */
  if ((sqrt((lare * lare) + (larn * larn)) - lr1) < cosphi) {
    /* 'LARCalcs:158' lar.excl_valid = uint8(1); */
    lar->excl_valid = 1U;
  } else {
    /* 'LARCalcs:159' else */
    /* 'LARCalcs:160' lar.excl_valid = uint8(0); */
    lar->excl_valid = 0U;
  }

  /* 'LARCalcs:166' LARenu.x = larenu(1); */
  /* 'LARCalcs:166' LARenu.y = larenu(2); */
  /* 'LARCalcs:166' LARenu.z = larenu(3); */
  /* 'LARCalcs:167' LARecef = LARenu2ecef(LARenu,Orgecef); */
  /* 'LARenu2ecef:20' tmpenu = zeros(1,3); */
  /* 'LARenu2ecef:21' tmpenu(1) = enu.x; */
  tmpenu[0] = lare;

  /* 'LARenu2ecef:22' tmpenu(2) = enu.y; */
  tmpenu[1] = larn;

  /* 'LARenu2ecef:23' tmpenu(3) = enu.z; */
  tmpenu[2] = tgt_alt;

  /* 'LARenu2ecef:24' tmporg = zeros(1,3); */
  /* 'LARenu2ecef:25' tmporg(1) = orgecef.x; */
  /* 'LARenu2ecef:26' tmporg(2) = orgecef.y; */
  /* 'LARenu2ecef:27' tmporg(3) = orgecef.z; */
  /* 'LARenu2ecef:28' orglla = LIB_ecef2lla(orgecef); */
  LIB_ecef2lla(tgt_rex, tgt_rey, tgt_rez, &b_cosphi, &larn, &lare);

  /* 'LARenu2ecef:31' sinphi = sin(orglla.lat); */
  R_0 = sin(b_cosphi);

  /* 'LARenu2ecef:32' cosphi = cos(orglla.lat); */
  b_cosphi = cos(b_cosphi);

  /* 'LARenu2ecef:33' sinlam = sin(orglla.lon); */
  lare = sin(larn);

  /* 'LARenu2ecef:34' coslam = cos(orglla.lon); */
  larn = cos(larn);

  /* 'LARenu2ecef:35' R = [ -sinlam          coslam         0     ; ... */
  /* 'LARenu2ecef:36'       -sinphi*coslam  -sinphi*sinlam  cosphi; ... */
  /* 'LARenu2ecef:37'        cosphi*coslam   cosphi*sinlam  sinphi]; */
  R[0] = -lare;
  R[3] = larn;
  R[6] = 0.0;
  R[1] = (-R_0) * larn;
  R[4] = (-R_0) * lare;
  R[7] = b_cosphi;
  R[2] = b_cosphi * larn;
  R[5] = b_cosphi * lare;
  R[8] = R_0;

  /* 'LARenu2ecef:38' difecef = R\tmpenu'; */
  r1 = 0;
  r2 = 1;
  r3 = 2;
  larn = fabs(-lare);
  lare = fabs(R[1]);
  if (lare > larn) {
    larn = lare;
    r1 = 1;
    r2 = 0;
  }

  if (fabs(R[2]) > larn) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }

  R[r2] /= R[r1];
  R[r3] /= R[r1];
  R[3 + r2] -= R[3 + r1] * R[r2];
  R[3 + r3] -= R[3 + r1] * R[r3];
  R[6 + r2] -= R[6 + r1] * R[r2];
  R[6 + r3] -= R[6 + r1] * R[r3];
  if (fabs(R[3 + r3]) > fabs(R[3 + r2])) {
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }

  R[3 + r3] /= R[3 + r2];
  R[6 + r3] -= R[3 + r3] * R[6 + r2];
  larn = tmpenu[r2] - (tmpenu[r1] * R[r2]);
  b_cosphi = ((tmpenu[r3] - (tmpenu[r1] * R[r3])) - (R[3 + r3] * larn)) / R[6 +
    r3];
  larn -= R[6 + r2] * b_cosphi;
  larn /= R[3 + r2];

  /* 'LARenu2ecef:39' ECEF.x = tmporg(1) + difecef(1); */
  /* 'LARenu2ecef:40' ECEF.y = tmporg(2) + difecef(2); */
  /* 'LARenu2ecef:41' ECEF.z = tmporg(3) + difecef(3); */
  /* 'LARCalcs:171' LARlla = LIB_ecef2lla(LARecef); */
  LIB_ecef2lla(tgt_rex + (((tmpenu[r1] - (b_cosphi * R[6 + r1])) - (larn * R[3 +
    r1])) / R[r1]), tgt_rey + larn, tgt_rez + b_cosphi, &lar->ctr_lat,
               &lar->ctr_lon, &lare);

  /* 'LARCalcs:172' lar.ctr_lat = LARlla.lat; */
  /* 'LARCalcs:173' lar.ctr_lon = LARlla.lon; */
  /* 'LARCalcs:176' tas  = LIB_mach2tas(launch.mach,launch.alt); */
  /* 'LIB_mach2tas:14' if (dAltitude <= 11000) */
  if (launch_alt <= 11000.0) {
    /* 'LIB_mach2tas:15' T0 = 288.15; */
    larn = 288.15;

    /* 'LIB_mach2tas:16' lr = -0.0065; */
    lare = -0.0065;
  } else {
    /* 'LIB_mach2tas:17' else */
    /* 'LIB_mach2tas:18' T0 = 216.65; */
    larn = 216.65;

    /* 'LIB_mach2tas:19' lr = 0.0; */
    lare = 0.0;
  }

  /* 'LIB_mach2tas:21' Re = 6356766; */
  /* 'LIB_mach2tas:22' T  = T0+lr*(Re*dAltitude)/(Re+dAltitude); */
  /* 'LIB_mach2tas:23' vsound = sqrt(1.4*287.05287*T); */
  /* 'LIB_mach2tas:24' tas = mach*vsound; */
  /* 'LARCalcs:177' gspd = tas - ws*cos(launch.track-wd); */
  /* 'LARCalcs:179' atthead = relang + 180; */
  b_cosphi = sinphi + 180.0;

  /* 'LARCalcs:180' if (atthead > 360) */
  if ((sinphi + 180.0) > 360.0) {
    /* 'LARCalcs:181' atthead = atthead - 360; */
    b_cosphi = (sinphi + 180.0) - 360.0;
  }

  /* 'LARCalcs:184' [ct_data,traj_data] = LARTimings(tgt.valid,launch.mach,gspd,relenu,GndDistTgt,RelBearTgt,GndDistLAR,RelBearLAR,radius,excl_radius,tgt.dive,atthead,LARtraj,return_traj); */
  LARTimings(tgt_valid, launch_mach, (launch_mach * sqrt(401.874018 * (larn +
    ((lare * (6.356766E+6 * launch_alt)) / (6.356766E+6 + launch_alt))))) -
             (launch_wind_spd * cos(launch_track - launch_wind_dir)), relenu,
             sinlam, coslam, tmpxyz_idx_0, RelBearLAR, lr1, cosphi, tgt_dive,
             b_cosphi, LARtraj->TOF070, LARtraj->TOF080, LARtraj->TOF090,
             &ct_data, &traj_data, dwork);

  /* 'LARCalcs:187' lar.trajlat = zeros(1,32); */
  /* 'LARCalcs:188' lar.trajlon = zeros(1,32); */
  /* 'LARCalcs:189' if (ct_data.inside && return_traj > 0) */
  /* 'LARCalcs:252' else */
  /* 'LARCalcs:253' Becef.x = launch.rex; */
  /* 'LARCalcs:253' Becef.y = launch.rey; */
  /* 'LARCalcs:253' Becef.z = launch.rez; */
  /* 'LARCalcs:254' Blla = LIB_ecef2lla(Becef); */
  LIB_ecef2lla(launch_rex, launch_rey, launch_rez, &b_cosphi, &larn, &lare);

  /* 'LARCalcs:255' lar.trajlat(1:32) = Blla.lat; */
  /* 'LARCalcs:256' lar.trajlon(1:32) = Blla.lon; */
  for (r1 = 0; r1 < 32; r1++) {
    lar->trajlat[r1] = b_cosphi;
    lar->trajlon[r1] = larn;
  }

  /* 'LARCalcs:257' lar.est_GroundRange = 0; */
  lar->est_GroundRange = 0.0;

  /* 'LARCalcs:258' lar.est_ImpactSpd   = 0; */
  lar->est_ImpactSpd = 0.0;

  /* 'LARCalcs:259' lar.est_ImpactTrack = 0; */
  lar->est_ImpactTrack = 0.0;

  /* 'LARCalcs:260' lar.est_ImpactDive  = 0; */
  lar->est_ImpactDive = 0.0;

  /* 'LARCalcs:263' lar.inside   = ct_data.inside; */
  lar->inside = ct_data.inside;

  /* 'LARCalcs:264' lar.ToGo     = ct_data.tg; */
  lar->ToGo = ct_data.tg;

  /* 'LARCalcs:265' lar.ToExit   = ct_data.te; */
  lar->ToExit = ct_data.te;

  /* 'LARCalcs:266' lar.ToImpact = ct_data.ti; */
  lar->ToImpact = ct_data.ti;

  /* 'LARCalcs:267' lar.ToIllum  = ct_data.ts; */
  lar->ToIllum = ct_data.ts;
}

/*
 * Function for MATLAB Function: '<S1>/Data Controller'
 * function LAR = LARCalculations(tgt_data,launch_data,LAR_data,LAR_traj,TestCfg_in)
 */
static void LARCalculations(uint32_T tgt_data_valid, real_T tgt_data_rgx, real_T
  tgt_data_rgy, real_T tgt_data_rgz, real_T tgt_data_rex, real_T tgt_data_rey,
  real_T tgt_data_rez, real_T tgt_data_alt, real_T tgt_data_dive, real_T
  tgt_data_arrive, const T_GUID_Guid2DATA_launchstruct *launch_data, const
  b_sYbzGJxaGNvEV1Jh0D0xgEF *LAR_data, const T_Weapon_MCP_GlobalMemory_LARtraj
  *LAR_traj, const T_GUID_TestConfig TestCfg_in, s0qQI8FPZFwxQDcpW3UrxsD *LAR,
  DW *dwork)
{
  real_T relalt;
  real_T VECrelalt[7];
  real_T RelX;
  real_T RelY;
  sW6z4ntaPZV6RXLllkgQXpF expl_temp;
  real_T TestCfg_in_0[7];
  int32_T varargin_1_idx_1;
  real_T RelX_tmp;

  /* 'GUID_Embedded_Data_Control:661' lar_data = LARCalcs(launch_data,tgt_data,LAR_data,LAR_traj,0); */
  LARCalcs(launch_data->alt, launch_data->rex, launch_data->rey,
           launch_data->rez, launch_data->mach, launch_data->track,
           launch_data->wind_spd, launch_data->wind_dir, tgt_data_valid,
           tgt_data_rex, tgt_data_rey, tgt_data_rez, tgt_data_alt, tgt_data_dive,
           tgt_data_arrive, LAR_data, LAR_traj, &expl_temp, dwork);

  /* 'GUID_Embedded_Data_Control:664' LAR.radius   = lar_data.radius; */
  LAR->radius = expl_temp.radius;

  /* 'GUID_Embedded_Data_Control:665' LAR.exclrad  = lar_data.excl_radius; */
  LAR->exclrad = expl_temp.excl_radius;

  /* 'GUID_Embedded_Data_Control:666' LAR.RelX     = lar_data.RelDistY; */
  LAR->RelX = expl_temp.RelDistY;

  /* 'GUID_Embedded_Data_Control:667' LAR.RelY     = lar_data.RelDistX; */
  LAR->RelY = expl_temp.RelDistX;

  /* 'GUID_Embedded_Data_Control:668' LAR.Psi2LAR  = lar_data.RelBearLAR; */
  LAR->Psi2LAR = expl_temp.RelBearLAR;

  /* 'GUID_Embedded_Data_Control:669' LAR.Psi2Tgt  = lar_data.RelBearTgt; */
  LAR->Psi2Tgt = expl_temp.RelBearTgt;

  /* 'GUID_Embedded_Data_Control:671' LAR.Rge2LAR  = lar_data.GndDistLAR; */
  LAR->Rge2LAR = expl_temp.GndDistLAR;

  /* 'GUID_Embedded_Data_Control:672' LAR.Rge2Tgt  = lar_data.GndDistTgt; */
  LAR->Rge2Tgt = expl_temp.GndDistTgt;

  /* 'GUID_Embedded_Data_Control:673' LAR.Time2Imp = lar_data.ToImpact; */
  LAR->Time2Imp = expl_temp.ToImpact;

  /* 'GUID_Embedded_Data_Control:674' LAR.Time2Lase= lar_data.ToIllum; */
  LAR->Time2Lase = expl_temp.ToIllum;

  /* 'GUID_Embedded_Data_Control:675' LAR.Time2Go  = lar_data.ToGo; */
  LAR->Time2Go = expl_temp.ToGo;

  /* 'GUID_Embedded_Data_Control:676' LAR.LARFlags = uint32(0); */
  /* 'GUID_Embedded_Data_Control:680' relalt = LIB_limit(0.0,50000.0,tgt_data.rgz*3.28084); */
  relalt = tgt_data_rgz * 3.28084;

  /* 'LIB_limit:27' if (in > max) */
  if (relalt > 50000.0) {
    /* 'LIB_limit:28' out = max; */
    relalt = 50000.0;
  } else if (relalt < 0.0) {
    /* 'LIB_limit:29' elseif (in < min) */
    /* 'LIB_limit:30' out = min; */
    relalt = 0.0;
  } else {
    /* 'LIB_limit:31' else */
    /* 'LIB_limit:32' out = in; */
  }

  /* 'GUID_Embedded_Data_Control:682' rad  = double(TestCfg_in.LARradius); */
  /* 'GUID_Embedded_Data_Control:683' alt  = double(TestCfg_in.LARalt); */
  /* 'GUID_Embedded_Data_Control:684' relx = double(TestCfg_in.LARX); */
  /* 'GUID_Embedded_Data_Control:685' vel  = launch_data.Vgnd; */
  /* 'GUID_Embedded_Data_Control:686' t1   =  7.883; */
  /* 'GUID_Embedded_Data_Control:687' t3   = 13.654; */
  /* 'GUID_Embedded_Data_Control:688' VECrelalt =        [0.0   alt-3000  alt-1000   alt  alt+1000  alt+3000 100000.0]; */
  VECrelalt[0] = 0.0;
  VECrelalt[1] = ((real_T)TestCfg_in.LARalt) - 3000.0;
  VECrelalt[2] = ((real_T)TestCfg_in.LARalt) - 1000.0;
  VECrelalt[3] = (real_T)TestCfg_in.LARalt;
  VECrelalt[4] = ((real_T)TestCfg_in.LARalt) + 1000.0;
  VECrelalt[5] = ((real_T)TestCfg_in.LARalt) + 3000.0;
  VECrelalt[6] = 100000.0;

  /* 'GUID_Embedded_Data_Control:689' VECradius =        [0.0        0.0   0.9*rad   rad   0.9*rad       0.0      0.0]; */
  /* 'GUID_Embedded_Data_Control:690' VECrelX   = relx + [-relx  -vel*t3   -vel*t1   0.0    vel*t1    vel*t3 100000.0]; */
  /* 'GUID_Embedded_Data_Control:692' TPosX =  tgt_data.rgx*cos(launch_data.track) + tgt_data.rgy*sin(launch_data.track); */
  /* 'GUID_Embedded_Data_Control:693' TPosY = -tgt_data.rgx*sin(launch_data.track) + tgt_data.rgy*cos(launch_data.track); */
  /* 'GUID_Embedded_Data_Control:695' Radius      = interp1(VECrelalt,VECradius,relalt); */
  /* 'GUID_Embedded_Data_Control:696' centre_offX = interp1(VECrelalt,VECrelX,relalt); */
  /* 'GUID_Embedded_Data_Control:697' centre_offY = double(TestCfg_in.LARY); */
  /* 'GUID_Embedded_Data_Control:699' RelX = TPosX - centre_offX; */
  TestCfg_in_0[0] = ((real_T)TestCfg_in.LARX) + (-((real_T)TestCfg_in.LARX));
  TestCfg_in_0[1] = ((-launch_data->Vgnd) * 13.654) + ((real_T)TestCfg_in.LARX);
  TestCfg_in_0[2] = ((-launch_data->Vgnd) * 7.883) + ((real_T)TestCfg_in.LARX);
  TestCfg_in_0[3] = (real_T)TestCfg_in.LARX;
  TestCfg_in_0[4] = (launch_data->Vgnd * 7.883) + ((real_T)TestCfg_in.LARX);
  TestCfg_in_0[5] = (launch_data->Vgnd * 13.654) + ((real_T)TestCfg_in.LARX);
  TestCfg_in_0[6] = ((real_T)TestCfg_in.LARX) + 100000.0;
  RelY = sin(launch_data->track);
  RelX_tmp = cos(launch_data->track);
  RelX = ((tgt_data_rgx * RelX_tmp) + (tgt_data_rgy * RelY)) - interp1_bFwkvuIc
    (VECrelalt, TestCfg_in_0, relalt);

  /* 'GUID_Embedded_Data_Control:700' RelY = TPosY - centre_offY; */
  RelY = (((-tgt_data_rgx) * RelY) + (tgt_data_rgy * RelX_tmp)) - ((real_T)
    TestCfg_in.LARY);

  /* 'GUID_Embedded_Data_Control:702' Psi2LAR = -atan2(-RelY,RelX); */
  RelX_tmp = rt_atan2d_snf(-RelY, RelX);
  LAR->Psi2LAR_Test = -RelX_tmp;

  /* 'GUID_Embedded_Data_Control:703' if (Psi2LAR < 0.0) */
  if ((-RelX_tmp) < 0.0) {
    /* 'GUID_Embedded_Data_Control:704' Psi2LAR = Psi2LAR + 2*pi; */
    LAR->Psi2LAR_Test = (-RelX_tmp) + 6.2831853071795862;
  }

  /* 'GUID_Embedded_Data_Control:707' if (TestCfg_in.LARtimeimp > 0) */
  if (((int32_T)TestCfg_in.LARtimeimp) > 0) {
    /* 'GUID_Embedded_Data_Control:708' t2imp  = double(TestCfg_in.LARtimeimp); */
    /* 'GUID_Embedded_Data_Control:709' t2lase = max([0.0,t2imp-45.0]); */
    varargin_1_idx_1 = ((int32_T)TestCfg_in.LARtimeimp) - 45;

    /* 'GUID_Embedded_Data_Control:710' LAR.Time2Imp = t2imp; */
    LAR->Time2Imp = (real_T)TestCfg_in.LARtimeimp;

    /* 'GUID_Embedded_Data_Control:711' LAR.Time2Lase= t2lase; */
    if (0 < varargin_1_idx_1) {
      LAR->Time2Lase = (real_T)varargin_1_idx_1;
    } else {
      LAR->Time2Lase = 0.0;
    }
  }

  /* 'GUID_Embedded_Data_Control:713' LAR.radius_Test  = Radius; */
  TestCfg_in_0[0] = 0.0;
  TestCfg_in_0[1] = 0.0;
  RelX_tmp = 0.9 * ((real_T)TestCfg_in.LARradius);
  TestCfg_in_0[2] = RelX_tmp;
  TestCfg_in_0[3] = (real_T)TestCfg_in.LARradius;
  TestCfg_in_0[4] = RelX_tmp;
  TestCfg_in_0[5] = 0.0;
  TestCfg_in_0[6] = 0.0;
  LAR->radius_Test = interp1_bFwkvuIc(VECrelalt, TestCfg_in_0, relalt);

  /* 'GUID_Embedded_Data_Control:714' LAR.Psi2LAR_Test = Psi2LAR; */
  /* 'GUID_Embedded_Data_Control:716' LAR.Rge2LAR_Test = sqrt(RelX^2+RelY^2); */
  LAR->Rge2LAR_Test = sqrt((RelX * RelX) + (RelY * RelY));

  /* 'GUID_Embedded_Data_Control:718' LAR.LARFlags = bitset(LAR.LARFlags,1,lar_data.radius_valid); */
  LAR->LARFlags = (uint32_T)((((int32_T)expl_temp.radius_valid) != 0) ? 1 : 0);
}

/* Function for MATLAB Function: '<S1>/Data Controller' */
static real_T interp1(const real_T varargin_1_data[], const int32_T
                      varargin_1_size[2], const real_T varargin_2_data[], const
                      int32_T varargin_2_size[2], real_T varargin_3)
{
  real_T Vq;
  real_T y_data[3];
  real_T x_data[3];
  int32_T nyrows;
  int32_T nx;
  real_T r;
  int32_T high_i;
  int32_T mid_i;
  int32_T exitg1;
  nyrows = (varargin_2_size[0] * varargin_2_size[1]) - 1;
  if (0 <= nyrows) {
    memcpy(&y_data[0], &varargin_2_data[0], ((uint32_T)((int32_T)(nyrows + 1))) *
           (sizeof(real_T)));
  }

  mid_i = varargin_1_size[1];
  nyrows = (varargin_1_size[0] * varargin_1_size[1]) - 1;
  if (0 <= nyrows) {
    memcpy(&x_data[0], &varargin_1_data[0], ((uint32_T)((int32_T)(nyrows + 1))) *
           (sizeof(real_T)));
  }

  nyrows = varargin_2_size[1] - 1;
  nx = varargin_1_size[1] - 1;
  Vq = 0.0;
  high_i = 0;
  do {
    exitg1 = 0;
    if (high_i <= nx) {
      if (rtIsNaN(varargin_1_data[high_i])) {
        exitg1 = 1;
      } else {
        high_i++;
      }
    } else {
      if (varargin_1_data[1] < varargin_1_data[0]) {
        x_data[0] = varargin_1_data[nx];
        x_data[nx] = varargin_1_data[0];
        y_data[0] = varargin_2_data[varargin_2_size[1] - 1];
        y_data[varargin_2_size[1] - 1] = varargin_2_data[0];
      }

      if (rtIsNaN(varargin_3)) {
        Vq = (rtNaN);
      } else {
        r = x_data[mid_i - 1];
        if (varargin_3 > r) {
          Vq = (((varargin_3 - r) / (r - x_data[mid_i - 2])) * (y_data[nyrows] -
                 y_data[nyrows - 1])) + y_data[nyrows];
        } else if (varargin_3 < x_data[0]) {
          Vq = (((varargin_3 - x_data[0]) / (x_data[1] - x_data[0])) * (y_data[1]
                 - y_data[0])) + y_data[0];
        } else {
          nyrows = 1;
          nx = 2;
          high_i = mid_i;
          while (high_i > nx) {
            mid_i = (nyrows >> ((uint64_T)1)) + 1;
            if ((nyrows & 1) == 1) {
              mid_i++;
            }

            if (varargin_3 >= x_data[mid_i - 1]) {
              nyrows = mid_i;
              nx = mid_i + 1;
            } else {
              high_i = mid_i;
            }
          }

          r = x_data[nyrows - 1];
          r = (varargin_3 - r) / (x_data[nyrows] - r);
          if (r == 0.0) {
            Vq = y_data[nyrows - 1];
          } else if (r == 1.0) {
            Vq = y_data[nyrows];
          } else if (y_data[nyrows - 1] == y_data[nyrows]) {
            Vq = y_data[nyrows - 1];
          } else {
            Vq = ((1.0 - r) * y_data[nyrows - 1]) + (r * y_data[nyrows]);
          }
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return Vq;
}

/* Model step function */
void DataControl_10Hz_Function_main(RT_MODEL *const DataControl_10Hz_Function_M,
  T_GUID_Guid2DATA *rtU_Guidin, T_GUID_WeaponConfig *rtU_WeaCfgin,
  T_GUID_TestConfig *rtU_TestCfgin, T_GUID_Discretes *rtU_Discretesin, uint8_T
  rtU_calcLAR, T_Weapon_MCP_DATA *rtY_MCPdata, T_Weapon_MCP_LAR *rtY_LAR)
{
  uint8_T Guidin_umbilical;
  uint8_T Guidin_OpportunityMode;
  uint32_T Guidin_tgt_data_valid;
  T_GUID_Guid2DATA_launchstruct Guidin_launch_data;
  int32_T ind;
  int32_T indP;
  int32_T indY;
  real_T machlim;
  real_T RGE1[18];
  real_T RGE2[18];
  real_T TOF1[18];
  real_T TOF2[18];
  real_T del_alt;
  real_T LAR_Psi2Tgt;
  real_T radius;
  real_T centre_offX;
  real_T T_IMP_data[3];
  real_T R_IMP_data[3];
  real_T relalt;
  real_T TPosY;
  real_T Rge2Tgt;
  real_T RelX;
  real_T RelY;
  real_T Rge2LAR;
  static const uint16_T b[6] = { 5000U, 10000U, 15000U, 20000U, 30000U, 40000U };

  static const real_T e[7] = { 0.0, 12000.0, 14000.0, 15000.0, 16000.0, 18000.0,
    100000.0 };

  static const real_T f[7] = { 0.0, 0.0, 2750.0, 3000.0, 3250.0, 0.0, 0.0 };

  static const real_T g[7] = { 0.0, 3000.0, 4000.0, 5000.0, 6000.0, 8000.0,
    100000.0 };

  static const real_T h[7] = { 0.0, 0.0, 1250.0, 1500.0, 1500.0, 0.0, 0.0 };

  static const real_T i[7] = { 0.0, 8000.0, 11000.0, 12000.0, 13000.0, 15000.0,
    20000.0 };

  static const real_T j[7] = { 0.0, 0.0, 2250.0, 2500.0, 2500.0, 0.0, 0.0 };

  static const real_T k[7] = { 0.0, 12000.0, 14000.0, 15000.0, 18000.0, 20000.0,
    100000.0 };

  static const real_T l[7] = { 0.0, 0.0, 2750.0, 3000.0, 3500.0, 0.0, 0.0 };

  static const real_T m[7] = { 0.0, 4000.0, 7000.0, 8000.0, 9000.0, 11000.0,
    16000.0 };

  static const real_T n[7] = { 0.0, 4000.0, 5000.0, 6000.0, 7000.0, 8000.0,
    12000.0 };

  static const real_T o[7] = { 0.0, 1750.0, 2250.0, 2750.0, 3250.0, 5250.0,
    16000.0 };

  static const real_T p[7] = { 0.0, 10000.0, 16000.0, 18000.0, 19000.0, 21000.0,
    21000.0 };

  static const real_T q[7] = { 0.0, 5000.0, 6000.0, 6500.0, 8000.0, 8000.0,
    12000.0 };

  s0qQI8FPZFwxQDcpW3UrxsD expl_temp;
  int32_T i_0;
  real_T tmp[7];
  real_T tmp_0[6];
  int32_T i_1;
  int32_T T_IMP_size[2];
  int32_T R_IMP_size[2];
  uint32_T qY;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_datactrl_at_outport_1' incorporates:
   *  SubSystem: '<Root>/DataControl'
   */
  /* MATLAB Function: '<S1>/Data Controller' incorporates:
   *  Inport: '<Root>/Discretesin'
   *  Inport: '<Root>/Guidin'
   *  Inport: '<Root>/TestCfgin'
   *  Inport: '<Root>/WeaCfgin'
   *  Inport: '<Root>/calcLAR'
   *  Outport: '<Root>/MCPdata'
   */
  /* :  [DATA,LARout] = GUID_Embedded_Data_Control(Guidin,WeaCfgin,TestCfgin,Discretesin,calcLAR,MCP_Global,MCP_Global_VersionNo,def,cnsts); */
  Guidin_umbilical = rtU_Guidin->umbilical;
  Guidin_OpportunityMode = rtU_Guidin->OpportunityMode;
  Guidin_tgt_data_valid = rtU_Guidin->tgt_data.valid;
  RelX = rtU_Guidin->tgt_data.rgx;
  RelY = rtU_Guidin->tgt_data.rgy;
  machlim = rtU_Guidin->tgt_data.rgz;
  del_alt = rtU_Guidin->tgt_data.rex;
  Rge2Tgt = rtU_Guidin->tgt_data.rey;
  radius = rtU_Guidin->tgt_data.rez;
  relalt = rtU_Guidin->tgt_data.alt;
  centre_offX = rtU_Guidin->tgt_data.dive;
  TPosY = rtU_Guidin->tgt_data.arrive;
  Guidin_launch_data = rtU_Guidin->launch_data;

  /* 'GUID_Embedded_Data_Control:17' if (isempty(once)) */
  if (!DataControl_10Hz_Function_M->dwork.once_not_empty) {
    /* 'GUID_Embedded_Data_Control:19' Guidin.umbilical            = uint8(1); */
    Guidin_umbilical = 1U;

    /* 'GUID_Embedded_Data_Control:20' Guidin.NavActive            = uint8(0); */
    /* 'GUID_Embedded_Data_Control:21' Guidin.OpportunityMode      = uint8(0); */
    Guidin_OpportunityMode = 0U;

    /* 'GUID_Embedded_Data_Control:22' Guidin.Spare01              = uint8(0); */
    /* 'GUID_Embedded_Data_Control:23' Guidin.Spare02              = uint32(0); */
    /* 'GUID_Embedded_Data_Control:24' Guidin.tgt_data.valid       = uint32(0); */
    Guidin_tgt_data_valid = 0U;

    /* 'GUID_Embedded_Data_Control:25' Guidin.tgt_data.rgx         = 0.0; */
    RelX = 0.0;

    /* 'GUID_Embedded_Data_Control:26' Guidin.tgt_data.rgy         = 0.0; */
    RelY = 0.0;

    /* 'GUID_Embedded_Data_Control:27' Guidin.tgt_data.rgz         = 0.0; */
    machlim = 0.0;

    /* 'GUID_Embedded_Data_Control:28' Guidin.tgt_data.rex         = 6378137.0; */
    del_alt = 6.378137E+6;

    /* 'GUID_Embedded_Data_Control:29' Guidin.tgt_data.rey         = 0.0; */
    Rge2Tgt = 0.0;

    /* 'GUID_Embedded_Data_Control:30' Guidin.tgt_data.rez         = 0.0; */
    radius = 0.0;

    /* 'GUID_Embedded_Data_Control:31' Guidin.tgt_data.alt         = 100.0; */
    relalt = 100.0;

    /* 'GUID_Embedded_Data_Control:32' Guidin.tgt_data.dive        = 30.0; */
    centre_offX = 30.0;

    /* 'GUID_Embedded_Data_Control:33' Guidin.tgt_data.arrive      = 180.0; */
    TPosY = 180.0;

    /* 'GUID_Embedded_Data_Control:34' Guidin.tgt_data.speed       = 250.0; */
    /* 'GUID_Embedded_Data_Control:35' Guidin.launch_data.lat      = 0.0; */
    Guidin_launch_data.lat = 0.0;

    /* 'GUID_Embedded_Data_Control:36' Guidin.launch_data.lon      = 0.0; */
    Guidin_launch_data.lon = 0.0;

    /* 'GUID_Embedded_Data_Control:37' Guidin.launch_data.alt      = 6096.0; */
    Guidin_launch_data.alt = 6096.0;

    /* 'GUID_Embedded_Data_Control:38' Guidin.launch_data.rex      = 6378137.0+6096.0; */
    Guidin_launch_data.rex = 6.384233E+6;

    /* 'GUID_Embedded_Data_Control:39' Guidin.launch_data.rey      = 0.0; */
    Guidin_launch_data.rey = 0.0;

    /* 'GUID_Embedded_Data_Control:40' Guidin.launch_data.rez      = -20000.0; */
    Guidin_launch_data.rez = -20000.0;

    /* 'GUID_Embedded_Data_Control:41' Guidin.launch_data.Vgnd     = 250.0; */
    Guidin_launch_data.Vgnd = 250.0;

    /* 'GUID_Embedded_Data_Control:42' Guidin.launch_data.mach     = 0.8; */
    Guidin_launch_data.mach = 0.8;

    /* 'GUID_Embedded_Data_Control:43' Guidin.launch_data.track    = 0.0; */
    Guidin_launch_data.track = 0.0;

    /* 'GUID_Embedded_Data_Control:44' Guidin.launch_data.wind_spd = 0.0; */
    Guidin_launch_data.wind_spd = 0.0;

    /* 'GUID_Embedded_Data_Control:45' Guidin.launch_data.wind_dir = 0.0; */
    Guidin_launch_data.wind_dir = 0.0;

    /* 'GUID_Embedded_Data_Control:47' LAR_out.radius       = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.radius = 0U;

    /* 'GUID_Embedded_Data_Control:48' LAR_out.Psi2LAR      = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR = 0U;

    /* 'GUID_Embedded_Data_Control:49' LAR_out.Psi2Tgt      = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Psi2Tgt = 0U;

    /* 'GUID_Embedded_Data_Control:50' LAR_out.Rge2LAR      = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR = 0U;

    /* 'GUID_Embedded_Data_Control:51' LAR_out.Rge2Tgt      = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt = 0U;

    /* 'GUID_Embedded_Data_Control:52' LAR_out.exclrad      = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.exclrad = 0U;

    /* 'GUID_Embedded_Data_Control:53' LAR_out.Time2Imp     = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Time2Imp = 0U;

    /* 'GUID_Embedded_Data_Control:54' LAR_out.Time2Lase    = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Time2Lase = 0U;

    /* 'GUID_Embedded_Data_Control:55' LAR_out.Time2Go      = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Time2Go = 0U;

    /* 'GUID_Embedded_Data_Control:56' LAR_out.Psi2LAR_Test = uint16(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR_Test = 0U;

    /* 'GUID_Embedded_Data_Control:57' LAR_out.radius_Test  = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.radius_Test = 0U;

    /* 'GUID_Embedded_Data_Control:58' LAR_out.Rge2LAR_Test = uint32(0); */
    DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR_Test = 0U;

    /* 'GUID_Embedded_Data_Control:60' LAR_data.psilim  = single(180); */
    DataControl_10Hz_Function_M->dwork.LAR_data.psilim = 180.0F;

    /* 'GUID_Embedded_Data_Control:61' LAR_data.psivec  = single(0:30:210); */
    for (i_0 = 0; i_0 < 8; i_0++) {
      DataControl_10Hz_Function_M->dwork.LAR_data.psivec[i_0] = 30.0F *
        ((real32_T)i_0);
    }

    /* 'GUID_Embedded_Data_Control:62' LAR_data.altmin  = single(10000); */
    DataControl_10Hz_Function_M->dwork.LAR_data.altmin = 10000.0F;

    /* 'GUID_Embedded_Data_Control:63' LAR_data.altmax  = single(40000); */
    DataControl_10Hz_Function_M->dwork.LAR_data.altmax = 40000.0F;

    /* 'GUID_Embedded_Data_Control:64' LAR_data.altvec  = single([5000 10000 15000 20000 30000 40000]); */
    for (i_0 = 0; i_0 < 6; i_0++) {
      DataControl_10Hz_Function_M->dwork.LAR_data.altvec[i_0] = (real32_T)b[i_0];
    }

    /* 'GUID_Embedded_Data_Control:65' LAR_data.alttgt  = single([0 2000 4000]); */
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[0] = 0.0F;
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[1] = 2000.0F;
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[2] = 4000.0F;

    /* 'GUID_Embedded_Data_Control:66' LAR_data.CX070   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:67' LAR_data.CX080   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:68' LAR_data.CX090   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:69' LAR_data.CY070   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:70' LAR_data.CY080   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:71' LAR_data.CY090   = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:72' LAR_data.EXCL070 = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:73' LAR_data.EXCL080 = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:74' LAR_data.EXCL090 = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:75' LAR_data.RAD070  = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:76' LAR_data.RAD080  = single(zeros(8,6,3)); */
    /* 'GUID_Embedded_Data_Control:77' LAR_data.RAD090  = single(zeros(8,6,3)); */
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CX070[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CX080[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CX090[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CY070[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CY080[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.CY090[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL070[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL080[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL090[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD070[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD080[0], 0, 144U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD090[0], 0, 144U *
           (sizeof(real32_T)));

    /* 'GUID_Embedded_Data_Control:78' LAR_data.RBAL070 = single(zeros(3,6)); */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[2] = 1;

    /* 'GUID_Embedded_Data_Control:79' LAR_data.RBAL080 = single(zeros(3,6)); */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[2] = 1;

    /* 'GUID_Embedded_Data_Control:80' LAR_data.RBAL090 = single(zeros(3,6)); */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[2] = 1;
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.data[0], 0, 18U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[0], 0, 18U *
           (sizeof(real32_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.data[0], 0, 18U *
           (sizeof(real32_T)));

    /* 'GUID_Embedded_Data_Control:81' LAR_traj.TOF070  = int16(zeros(8000,24)); */
    /* 'GUID_Embedded_Data_Control:82' LAR_traj.TOF080  = int16(zeros(8000,24)); */
    /* 'GUID_Embedded_Data_Control:83' LAR_traj.TOF090  = int16(zeros(8000,24)); */
    memset(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF070[0], 0, 192000U *
           (sizeof(int16_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF080[0], 0, 192000U *
           (sizeof(int16_T)));
    memset(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF090[0], 0, 192000U *
           (sizeof(int16_T)));

    /* 'GUID_Embedded_Data_Control:85' LAR_data.psilim  = MCP_Global.LARdata.psilim; */
    DataControl_10Hz_Function_M->dwork.LAR_data.psilim =
      rtP->MCP_Global.LARdata.psilim;

    /* 'GUID_Embedded_Data_Control:86' LAR_data.psivec  = MCP_Global.LARdata.psivec; */
    for (i_0 = 0; i_0 < 8; i_0++) {
      DataControl_10Hz_Function_M->dwork.LAR_data.psivec[i_0] =
        rtP->MCP_Global.LARdata.psivec[i_0];
    }

    /* 'GUID_Embedded_Data_Control:87' LAR_data.altmin  = MCP_Global.LARdata.altmin; */
    DataControl_10Hz_Function_M->dwork.LAR_data.altmin =
      rtP->MCP_Global.LARdata.altmin;

    /* 'GUID_Embedded_Data_Control:88' LAR_data.altmax  = MCP_Global.LARdata.altmax; */
    DataControl_10Hz_Function_M->dwork.LAR_data.altmax =
      rtP->MCP_Global.LARdata.altmax;

    /* 'GUID_Embedded_Data_Control:89' LAR_data.altvec  = MCP_Global.LARdata.altvec; */
    for (i_0 = 0; i_0 < 6; i_0++) {
      DataControl_10Hz_Function_M->dwork.LAR_data.altvec[i_0] =
        rtP->MCP_Global.LARdata.altvec[i_0];
    }

    /* 'GUID_Embedded_Data_Control:90' LAR_data.alttgt  = MCP_Global.LARdata.alttgt; */
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[0] =
      rtP->MCP_Global.LARdata.alttgt[0];
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[1] =
      rtP->MCP_Global.LARdata.alttgt[1];
    DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[2] =
      rtP->MCP_Global.LARdata.alttgt[2];

    /* 'GUID_Embedded_Data_Control:91' LAR_data.CX070   = MCP_Global.LARdata.CX070; */
    /* 'GUID_Embedded_Data_Control:92' LAR_data.CX080   = MCP_Global.LARdata.CX080; */
    /* 'GUID_Embedded_Data_Control:93' LAR_data.CX090   = MCP_Global.LARdata.CX090; */
    /* 'GUID_Embedded_Data_Control:94' LAR_data.CY070   = MCP_Global.LARdata.CY070; */
    /* 'GUID_Embedded_Data_Control:95' LAR_data.CY080   = MCP_Global.LARdata.CY080; */
    /* 'GUID_Embedded_Data_Control:96' LAR_data.CY090   = MCP_Global.LARdata.CY090; */
    /* 'GUID_Embedded_Data_Control:97' LAR_data.EXCL070 = MCP_Global.LARdata.EXCL070; */
    /* 'GUID_Embedded_Data_Control:98' LAR_data.EXCL080 = MCP_Global.LARdata.EXCL080; */
    /* 'GUID_Embedded_Data_Control:99' LAR_data.EXCL090 = MCP_Global.LARdata.EXCL090; */
    /* 'GUID_Embedded_Data_Control:100' LAR_data.RAD070  = MCP_Global.LARdata.RAD070; */
    /* 'GUID_Embedded_Data_Control:101' LAR_data.RAD080  = MCP_Global.LARdata.RAD080; */
    /* 'GUID_Embedded_Data_Control:102' LAR_data.RAD090  = MCP_Global.LARdata.RAD090; */
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CX070[0],
           &rtP->MCP_Global.LARdata.CX070[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CX080[0],
           &rtP->MCP_Global.LARdata.CX080[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CX090[0],
           &rtP->MCP_Global.LARdata.CX090[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CY070[0],
           &rtP->MCP_Global.LARdata.CY070[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CY080[0],
           &rtP->MCP_Global.LARdata.CY080[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.CY090[0],
           &rtP->MCP_Global.LARdata.CY090[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL070[0],
           &rtP->MCP_Global.LARdata.EXCL070[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL080[0],
           &rtP->MCP_Global.LARdata.EXCL080[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.EXCL090[0],
           &rtP->MCP_Global.LARdata.EXCL090[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD070[0],
           &rtP->MCP_Global.LARdata.RAD070[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD080[0],
           &rtP->MCP_Global.LARdata.RAD080[0], 144U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RAD090[0],
           &rtP->MCP_Global.LARdata.RAD090[0], 144U * (sizeof(real32_T)));

    /* 'GUID_Embedded_Data_Control:103' LAR_data.RBAL070 = MCP_Global.LARdata.RBAL070; */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.size[2] = 2;

    /* 'GUID_Embedded_Data_Control:104' LAR_data.RBAL080 = MCP_Global.LARdata.RBAL080; */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.size[2] = 2;

    /* 'GUID_Embedded_Data_Control:105' LAR_data.RBAL090 = MCP_Global.LARdata.RBAL090; */
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[0] = 3;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[1] = 6;
    DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.size[2] = 2;
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.data[0],
           &rtP->MCP_Global.LARdata.RBAL070[0], 36U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[0],
           &rtP->MCP_Global.LARdata.RBAL080[0], 36U * (sizeof(real32_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.data[0],
           &rtP->MCP_Global.LARdata.RBAL090[0], 36U * (sizeof(real32_T)));

    /* 'GUID_Embedded_Data_Control:108' LAR_traj.TOF070  = MCP_Global.LARtraj.TOF070; */
    /* 'GUID_Embedded_Data_Control:109' LAR_traj.TOF080  = MCP_Global.LARtraj.TOF080; */
    /* 'GUID_Embedded_Data_Control:110' LAR_traj.TOF090  = MCP_Global.LARtraj.TOF090; */
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF070[0],
           &rtP->MCP_Global.LARtraj.TOF070[0], 192000U * (sizeof(int16_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF080[0],
           &rtP->MCP_Global.LARtraj.TOF080[0], 192000U * (sizeof(int16_T)));
    memcpy(&DataControl_10Hz_Function_M->dwork.LAR_traj.TOF090[0],
           &rtP->MCP_Global.LARtraj.TOF090[0], 192000U * (sizeof(int16_T)));

    /* 'GUID_Embedded_Data_Control:112' AFmode = uint8(0); */
    /* 'GUID_Embedded_Data_Control:117' WingsRelRcvd_prev = uint8(0); */
    /* 'GUID_Embedded_Data_Control:119' once = 1; */
    DataControl_10Hz_Function_M->dwork.once_not_empty = true;

    /* 'GUID_Embedded_Data_Control:121' cnt = uint32(0); */
  }

  /* 'GUID_Embedded_Data_Control:127' umbilical         = Guidin.umbilical; */
  /* 'GUID_Embedded_Data_Control:128' NavActive         = Guidin.NavActive; */
  /* 'GUID_Embedded_Data_Control:130' LARin.tgt_data    = Guidin.tgt_data; */
  /* 'GUID_Embedded_Data_Control:131' LARin.launch_data = Guidin.launch_data; */
  /* 'GUID_Embedded_Data_Control:137' if (Discretes_in.umbilical) */
  if (((int32_T)rtU_Discretesin->umbilical) != 0) {
    /* 'GUID_Embedded_Data_Control:141' WingsRelRcvd_prev = Discretes_in.WingsRelRcvd; */
    DataControl_10Hz_Function_M->dwork.WingsRelRcvd_prev =
      rtU_Discretesin->WingsRelRcvd;

    /* 'GUID_Embedded_Data_Control:143' if (WeaCfg_in.WingsPresent) */
    if (((int32_T)rtU_WeaCfgin->WingsPresent) != 0) {
      /* 'GUID_Embedded_Data_Control:144' AFmode = uint8(1); */
      DataControl_10Hz_Function_M->dwork.AFmode = 1U;
    } else {
      /* 'GUID_Embedded_Data_Control:145' else */
      /* 'GUID_Embedded_Data_Control:146' AFmode = uint8(0); */
      DataControl_10Hz_Function_M->dwork.AFmode = 0U;
    }

    /* 'GUID_Embedded_Data_Control:150' cnt = cnt + uint32(1); */
    qY = DataControl_10Hz_Function_M->dwork.cnt + /*MW:OvSatOk*/ 1U;
    if (qY < DataControl_10Hz_Function_M->dwork.cnt) {
      qY = MAX_uint32_T;
    }

    DataControl_10Hz_Function_M->dwork.cnt = qY;

    /* 'GUID_Embedded_Data_Control:152' if (cnt > 700) */
  } else {
    /* 'GUID_Embedded_Data_Control:156' else */
    /* 'GUID_Embedded_Data_Control:161' if (Discretes_in.WingsOpenRcvd && AFmode == 1) */
    if ((((int32_T)rtU_Discretesin->WingsOpenRcvd) != 0) && (((int32_T)
          DataControl_10Hz_Function_M->dwork.AFmode) == 1)) {
      /* 'GUID_Embedded_Data_Control:162' AFmode = uint8(2); */
      DataControl_10Hz_Function_M->dwork.AFmode = 2U;
    }

    /* 'GUID_Embedded_Data_Control:164' if (Discretes_in.WingsRelRcvd > WingsRelRcvd_prev) */
    if (rtU_Discretesin->WingsRelRcvd >
        DataControl_10Hz_Function_M->dwork.WingsRelRcvd_prev) {
      /* 'GUID_Embedded_Data_Control:165' AFmode = uint8(3); */
      DataControl_10Hz_Function_M->dwork.AFmode = 3U;

      /* 'GUID_Embedded_Data_Control:166' WingsRelRcvd_prev = Discretes_in.WingsRelRcvd; */
      DataControl_10Hz_Function_M->dwork.WingsRelRcvd_prev =
        rtU_Discretesin->WingsRelRcvd;
    }
  }

  /* 'GUID_Embedded_Data_Control:171' if (WeaCfg_in.WeaponType == 1) */
  if (((int32_T)rtU_WeaCfgin->WeaponType) == 1) {
    /* 'GUID_Embedded_Data_Control:172' if (WeaCfg_in.BombType == 0) */
    if (((int32_T)rtU_WeaCfgin->BombType) == 0) {
      /* 'GUID_Embedded_Data_Control:174' AirframeID = uint8(1); */
      rtY_MCPdata->toAutop.AirframeID = 1U;

      /* 'GUID_Embedded_Data_Control:175' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:176' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:177' indY = 6; */
      indY = 5;
    } else {
      /* 'GUID_Embedded_Data_Control:178' else */
      /* 'GUID_Embedded_Data_Control:180' AirframeID = uint8(2); */
      rtY_MCPdata->toAutop.AirframeID = 2U;

      /* 'GUID_Embedded_Data_Control:181' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:182' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:183' indY = 6; */
      indY = 5;
    }
  } else if ((((int32_T)rtU_WeaCfgin->WingsPresent) == 0) || ((((int32_T)
                rtU_WeaCfgin->WingsPresent) == 1) && (((int32_T)
                rtU_Discretesin->WingsRelRcvd) == 1))) {
    /* 'GUID_Embedded_Data_Control:186' elseif (WeaCfg_in.WingsPresent == 0 || (WeaCfg_in.WingsPresent == 1 && Discretes_in.WingsRelRcvd == 1)) */
    /* 'GUID_Embedded_Data_Control:187' if (WeaCfg_in.BombType == 0) */
    switch (rtU_WeaCfgin->BombType) {
     case 0:
      /* 'GUID_Embedded_Data_Control:189' AirframeID = uint8(3); */
      rtY_MCPdata->toAutop.AirframeID = 3U;

      /* 'GUID_Embedded_Data_Control:190' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:191' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:192' indY = 6; */
      indY = 5;
      break;

     case 1:
      /* 'GUID_Embedded_Data_Control:193' elseif (WeaCfg_in.BombType == 1) */
      /* 'GUID_Embedded_Data_Control:195' AirframeID = uint8(4); */
      rtY_MCPdata->toAutop.AirframeID = 4U;

      /* 'GUID_Embedded_Data_Control:196' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:197' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:198' indY = 6; */
      indY = 5;
      break;

     case 2:
      /* 'GUID_Embedded_Data_Control:199' elseif (WeaCfg_in.BombType == 2) */
      /* 'GUID_Embedded_Data_Control:201' AirframeID = uint8(5); */
      rtY_MCPdata->toAutop.AirframeID = 5U;

      /* 'GUID_Embedded_Data_Control:202' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:203' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:204' indY = 6; */
      indY = 5;
      break;

     case 3:
      /* 'GUID_Embedded_Data_Control:205' elseif (WeaCfg_in.BombType == 3) */
      /* 'GUID_Embedded_Data_Control:207' AirframeID = uint8(6); */
      rtY_MCPdata->toAutop.AirframeID = 6U;

      /* 'GUID_Embedded_Data_Control:208' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:209' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:210' indY = 6; */
      indY = 5;
      break;

     default:
      /* 'GUID_Embedded_Data_Control:211' else */
      /* 'GUID_Embedded_Data_Control:213' AirframeID = uint8(4); */
      rtY_MCPdata->toAutop.AirframeID = 4U;

      /* 'GUID_Embedded_Data_Control:214' ind = 3; */
      ind = 2;

      /* 'GUID_Embedded_Data_Control:215' indP = 5; */
      indP = 4;

      /* 'GUID_Embedded_Data_Control:216' indY = 6; */
      indY = 5;
      break;
    }
  } else if ((((int32_T)rtU_WeaCfgin->WingsPresent) == 1) && (((int32_T)
               rtU_Discretesin->WingsOpenRcvd) == 0)) {
    /* 'GUID_Embedded_Data_Control:219' elseif (WeaCfg_in.WingsPresent == 1 && Discretes_in.WingsOpenRcvd == 0) */
    /* 'GUID_Embedded_Data_Control:221' if (WeaCfg_in.BombType == 0) */
    switch (rtU_WeaCfgin->BombType) {
     case 0:
      /* 'GUID_Embedded_Data_Control:223' AirframeID = uint8(11); */
      rtY_MCPdata->toAutop.AirframeID = 11U;

      /* 'GUID_Embedded_Data_Control:224' ind = 1; */
      ind = 0;

      /* 'GUID_Embedded_Data_Control:225' indP = 1; */
      indP = 0;

      /* 'GUID_Embedded_Data_Control:226' indY = 2; */
      indY = 1;
      break;

     case 1:
      /* 'GUID_Embedded_Data_Control:227' elseif (WeaCfg_in.BombType == 1) */
      /* 'GUID_Embedded_Data_Control:229' AirframeID = uint8(13); */
      rtY_MCPdata->toAutop.AirframeID = 13U;

      /* 'GUID_Embedded_Data_Control:230' ind = 1; */
      ind = 0;

      /* 'GUID_Embedded_Data_Control:231' indP = 1; */
      indP = 0;

      /* 'GUID_Embedded_Data_Control:232' indY = 2; */
      indY = 1;
      break;

     case 2:
      /* 'GUID_Embedded_Data_Control:233' elseif (WeaCfg_in.BombType == 2) */
      /* 'GUID_Embedded_Data_Control:235' AirframeID = uint8(15); */
      rtY_MCPdata->toAutop.AirframeID = 15U;

      /* 'GUID_Embedded_Data_Control:236' ind = 1; */
      ind = 0;

      /* 'GUID_Embedded_Data_Control:237' indP = 1; */
      indP = 0;

      /* 'GUID_Embedded_Data_Control:238' indY = 2; */
      indY = 1;
      break;

     case 3:
      /* 'GUID_Embedded_Data_Control:239' elseif (WeaCfg_in.BombType == 3) */
      /* 'GUID_Embedded_Data_Control:241' AirframeID = uint8(17); */
      rtY_MCPdata->toAutop.AirframeID = 17U;

      /* 'GUID_Embedded_Data_Control:242' ind = 1; */
      ind = 0;

      /* 'GUID_Embedded_Data_Control:243' indP = 1; */
      indP = 0;

      /* 'GUID_Embedded_Data_Control:244' indY = 2; */
      indY = 1;
      break;

     default:
      /* 'GUID_Embedded_Data_Control:245' else */
      /* 'GUID_Embedded_Data_Control:247' AirframeID = uint8(13); */
      rtY_MCPdata->toAutop.AirframeID = 13U;

      /* 'GUID_Embedded_Data_Control:248' ind = 1; */
      ind = 0;

      /* 'GUID_Embedded_Data_Control:249' indP = 1; */
      indP = 0;

      /* 'GUID_Embedded_Data_Control:250' indY = 2; */
      indY = 1;
      break;
    }
  } else if ((((int32_T)rtU_WeaCfgin->WingsPresent) == 1) && (((int32_T)
               rtU_Discretesin->WingsOpenRcvd) == 1)) {
    /* 'GUID_Embedded_Data_Control:253' elseif (WeaCfg_in.WingsPresent == 1 && Discretes_in.WingsOpenRcvd == 1) */
    /* 'GUID_Embedded_Data_Control:254' if (WeaCfg_in.BombType == 0) */
    switch (rtU_WeaCfgin->BombType) {
     case 0:
      /* 'GUID_Embedded_Data_Control:256' AirframeID = uint8(21); */
      rtY_MCPdata->toAutop.AirframeID = 21U;

      /* 'GUID_Embedded_Data_Control:257' ind = 2; */
      ind = 1;

      /* 'GUID_Embedded_Data_Control:258' indP = 3; */
      indP = 2;

      /* 'GUID_Embedded_Data_Control:259' indY = 4; */
      indY = 3;
      break;

     case 1:
      /* 'GUID_Embedded_Data_Control:260' elseif (WeaCfg_in.BombType == 1) */
      /* 'GUID_Embedded_Data_Control:262' AirframeID = uint8(23); */
      rtY_MCPdata->toAutop.AirframeID = 23U;

      /* 'GUID_Embedded_Data_Control:263' ind = 2; */
      ind = 1;

      /* 'GUID_Embedded_Data_Control:264' indP = 3; */
      indP = 2;

      /* 'GUID_Embedded_Data_Control:265' indY = 4; */
      indY = 3;
      break;

     case 2:
      /* 'GUID_Embedded_Data_Control:266' elseif (WeaCfg_in.BombType == 2) */
      /* 'GUID_Embedded_Data_Control:268' AirframeID = uint8(25); */
      rtY_MCPdata->toAutop.AirframeID = 25U;

      /* 'GUID_Embedded_Data_Control:269' ind = 2; */
      ind = 1;

      /* 'GUID_Embedded_Data_Control:270' indP = 3; */
      indP = 2;

      /* 'GUID_Embedded_Data_Control:271' indY = 4; */
      indY = 3;
      break;

     case 3:
      /* 'GUID_Embedded_Data_Control:272' elseif (WeaCfg_in.BombType == 3) */
      /* 'GUID_Embedded_Data_Control:274' AirframeID = uint8(27); */
      rtY_MCPdata->toAutop.AirframeID = 27U;

      /* 'GUID_Embedded_Data_Control:275' ind = 2; */
      ind = 1;

      /* 'GUID_Embedded_Data_Control:276' indP = 3; */
      indP = 2;

      /* 'GUID_Embedded_Data_Control:277' indY = 4; */
      indY = 3;
      break;

     default:
      /* 'GUID_Embedded_Data_Control:278' else */
      /* 'GUID_Embedded_Data_Control:280' AirframeID = uint8(23); */
      rtY_MCPdata->toAutop.AirframeID = 23U;

      /* 'GUID_Embedded_Data_Control:281' ind = 2; */
      ind = 1;

      /* 'GUID_Embedded_Data_Control:282' indP = 3; */
      indP = 2;

      /* 'GUID_Embedded_Data_Control:283' indY = 4; */
      indY = 3;
      break;
    }
  } else {
    /* 'GUID_Embedded_Data_Control:285' else */
    /* 'GUID_Embedded_Data_Control:286' AirframeID = uint8(4); */
    rtY_MCPdata->toAutop.AirframeID = 4U;

    /* 'GUID_Embedded_Data_Control:287' ind = 3; */
    ind = 2;

    /* 'GUID_Embedded_Data_Control:288' indP = 5; */
    indP = 4;

    /* 'GUID_Embedded_Data_Control:289' indY = 6; */
    indY = 5;
  }

  /* 'GUID_Embedded_Data_Control:300' seekertype = WeaCfg_in.SeekerType + uint8(1); */
  i_0 = (int32_T)((uint32_T)(((uint32_T)rtU_WeaCfgin->SeekerType) + 1U));
  if (((uint32_T)i_0) > 255U) {
    i_0 = 255;
  }

  /* 'GUID_Embedded_Data_Control:302' DATA.toNav.WeaponProp.leverarmX   = single(MCP_Global.nav.weaponprop.leverarmX); */
  rtY_MCPdata->toNav.WeaponProp.leverarmX =
    rtP->MCP_Global.nav.weaponprop.leverarmX;

  /* 'GUID_Embedded_Data_Control:303' DATA.toNav.WeaponProp.leverarmY   = single(MCP_Global.nav.weaponprop.leverarmY); */
  rtY_MCPdata->toNav.WeaponProp.leverarmY =
    rtP->MCP_Global.nav.weaponprop.leverarmY;

  /* 'GUID_Embedded_Data_Control:304' DATA.toNav.WeaponProp.leverarmZ   = single(MCP_Global.nav.weaponprop.leverarmZ); */
  rtY_MCPdata->toNav.WeaponProp.leverarmZ =
    rtP->MCP_Global.nav.weaponprop.leverarmZ;

  /* 'GUID_Embedded_Data_Control:306' DATA.toGuid.AirframeMode          = uint8(AFmode); */
  rtY_MCPdata->toGuid.AirframeMode = DataControl_10Hz_Function_M->dwork.AFmode;

  /* 'GUID_Embedded_Data_Control:307' DATA.toGuid.AirframeType          = uint8(MCP_Global.AirframeType); */
  rtY_MCPdata->toGuid.AirframeType = rtP->MCP_Global.AirframeType;

  /* 'GUID_Embedded_Data_Control:308' DATA.toGuid.Spare02               = uint16(0); */
  rtY_MCPdata->toGuid.Spare02 = 0U;

  /* 'GUID_Embedded_Data_Control:309' DATA.toGuid.AirframeProp.Rarc     = single(MCP_Global.guid.AFprop(ind).Rarc); */
  rtY_MCPdata->toGuid.AirframeProp.Rarc = rtP->MCP_Global.guid.AFprop[ind].Rarc;

  /* 'GUID_Embedded_Data_Control:310' DATA.toGuid.AirframeProp.Xmrc     = single(MCP_Global.guid.AFprop(ind).Xmrc); */
  rtY_MCPdata->toGuid.AirframeProp.Xmrc = rtP->MCP_Global.guid.AFprop[ind].Xmrc;

  /* 'GUID_Embedded_Data_Control:311' DATA.toGuid.AirframeProp.Xcg      = single(MCP_Global.guid.AFprop(ind).Xcg); */
  rtY_MCPdata->toGuid.AirframeProp.Xcg = rtP->MCP_Global.guid.AFprop[ind].Xcg;

  /* 'GUID_Embedded_Data_Control:312' DATA.toGuid.AirframeProp.Xref     = single(MCP_Global.guid.AFprop(ind).Xref); */
  rtY_MCPdata->toGuid.AirframeProp.Xref = rtP->MCP_Global.guid.AFprop[ind].Xref;

  /* 'GUID_Embedded_Data_Control:313' DATA.toGuid.AirframeProp.Sref     = single(MCP_Global.guid.AFprop(ind).Sref); */
  rtY_MCPdata->toGuid.AirframeProp.Sref = rtP->MCP_Global.guid.AFprop[ind].Sref;

  /* 'GUID_Embedded_Data_Control:314' DATA.toGuid.AirframeProp.mass     = single(MCP_Global.guid.AFprop(ind).mass); */
  rtY_MCPdata->toGuid.AirframeProp.mass = rtP->MCP_Global.guid.AFprop[ind].mass;

  /* 'GUID_Embedded_Data_Control:315' DATA.toGuid.AirframeProp.Jxx      = single(MCP_Global.guid.AFprop(ind).Jxx); */
  rtY_MCPdata->toGuid.AirframeProp.Jxx = rtP->MCP_Global.guid.AFprop[ind].Jxx;

  /* 'GUID_Embedded_Data_Control:316' DATA.toGuid.AirframeProp.machvec  = single(MCP_Global.guid.AFprop(ind).machvec); */
  for (i_1 = 0; i_1 < 7; i_1++) {
    rtY_MCPdata->toGuid.AirframeProp.machvec[i_1] =
      rtP->MCP_Global.guid.AFprop[ind].machvec[i_1];
  }

  /* 'GUID_Embedded_Data_Control:317' DATA.toGuid.AirframeProp.alphavec = single(MCP_Global.guid.AFprop(ind).alphavec); */
  for (i_1 = 0; i_1 < 10; i_1++) {
    rtY_MCPdata->toGuid.AirframeProp.alphavec[i_1] =
      rtP->MCP_Global.guid.AFprop[ind].alphavec[i_1];
  }

  /* 'GUID_Embedded_Data_Control:318' DATA.toGuid.AirframeProp.CMXdefl  = single(MCP_Global.guid.AFprop(ind).CMXdefl); */
  for (i_1 = 0; i_1 < 7; i_1++) {
    rtY_MCPdata->toGuid.AirframeProp.CMXdefl[i_1] =
      rtP->MCP_Global.guid.AFprop[ind].CMXdefl[i_1];
  }

  /* 'GUID_Embedded_Data_Control:319' DATA.toGuid.AirframeProp.CLCDtrim = single(MCP_Global.guid.AFprop(ind).CLCDtrim); */
  /* 'GUID_Embedded_Data_Control:320' DATA.toGuid.AirframeProp.CLtrim   = single(MCP_Global.guid.AFprop(ind).CLtrim); */
  /* 'GUID_Embedded_Data_Control:321' DATA.toGuid.AirframeProp.CDtrim   = single(MCP_Global.guid.AFprop(ind).CDtrim); */
  memcpy(&rtY_MCPdata->toGuid.AirframeProp.CLCDtrim[0],
         &rtP->MCP_Global.guid.AFprop[ind].CLCDtrim[0], 70U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toGuid.AirframeProp.CLtrim[0],
         &rtP->MCP_Global.guid.AFprop[ind].CLtrim[0], 70U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toGuid.AirframeProp.CDtrim[0],
         &rtP->MCP_Global.guid.AFprop[ind].CDtrim[0], 70U * (sizeof(real32_T)));

  /* 'GUID_Embedded_Data_Control:322' DATA.toGuid.AirframeProp.Spare01  = uint32(zeros(1,10)); */
  /* 'GUID_Embedded_Data_Control:323' DATA.toGuid.seeker.Amax           = single(MCP_Global.ttkf.seeker(seekertype).Amax); */
  rtY_MCPdata->toGuid.seeker.Amax = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Amax;

  /* 'GUID_Embedded_Data_Control:324' DATA.toGuid.seeker.Alin           = single(MCP_Global.ttkf.seeker(seekertype).Alin); */
  rtY_MCPdata->toGuid.seeker.Alin = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Alin;

  /* 'GUID_Embedded_Data_Control:325' DATA.toGuid.seeker.Bele           = single(MCP_Global.ttkf.seeker(seekertype).Bele); */
  rtY_MCPdata->toGuid.seeker.Bele = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Bele;

  /* 'GUID_Embedded_Data_Control:326' DATA.toGuid.seeker.Spare01        = uint32(0); */
  rtY_MCPdata->toGuid.seeker.Spare01 = 0U;

  /* 'GUID_Embedded_Data_Control:327' DATA.toGuid.seeker.Spare02        = uint32(0); */
  rtY_MCPdata->toGuid.seeker.Spare02 = 0U;

  /* 'GUID_Embedded_Data_Control:328' DATA.toGuid.seeker.Spare03        = uint32(0); */
  rtY_MCPdata->toGuid.seeker.Spare03 = 0U;

  /* 'GUID_Embedded_Data_Control:330' DATA.toTTKF.seeker.Amax           = single(MCP_Global.ttkf.seeker(seekertype).Amax); */
  rtY_MCPdata->toTTKF.seeker.Amax = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Amax;

  /* 'GUID_Embedded_Data_Control:331' DATA.toTTKF.seeker.Alin           = single(MCP_Global.ttkf.seeker(seekertype).Alin); */
  rtY_MCPdata->toTTKF.seeker.Alin = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Alin;

  /* 'GUID_Embedded_Data_Control:332' DATA.toTTKF.seeker.Bele           = single(MCP_Global.ttkf.seeker(seekertype).Bele); */
  rtY_MCPdata->toTTKF.seeker.Bele = rtP->MCP_Global.ttkf.seeker[((int32_T)
    ((uint8_T)i_0)) - 1].Bele;

  /* 'GUID_Embedded_Data_Control:333' DATA.toTTKF.seeker.Spare01        = uint32(0); */
  rtY_MCPdata->toTTKF.seeker.Spare01 = 0U;

  /* 'GUID_Embedded_Data_Control:334' DATA.toTTKF.seeker.Spare02        = uint32(0); */
  rtY_MCPdata->toTTKF.seeker.Spare02 = 0U;

  /* 'GUID_Embedded_Data_Control:335' DATA.toTTKF.seeker.Spare03        = uint32(0); */
  rtY_MCPdata->toTTKF.seeker.Spare03 = 0U;

  /* 'GUID_Embedded_Data_Control:336' DATA.toTTKF.setup.MP.horPos0      = single(MCP_Global.ttkf.setup.MP.horPos0); */
  /* 'GUID_Embedded_Data_Control:337' DATA.toTTKF.setup.MP.verPos0      = single(MCP_Global.ttkf.setup.MP.verPos0); */
  /* 'GUID_Embedded_Data_Control:338' DATA.toTTKF.setup.MP.horVel0      = single(MCP_Global.ttkf.setup.MP.horVel0); */
  /* 'GUID_Embedded_Data_Control:339' DATA.toTTKF.setup.MP.verVel0      = single(MCP_Global.ttkf.setup.MP.verVel0); */
  rtY_MCPdata->toTTKF.setup.MP.horPos0[0] =
    rtP->MCP_Global.ttkf.setup.MP.horPos0[0];
  rtY_MCPdata->toTTKF.setup.MP.verPos0[0] =
    rtP->MCP_Global.ttkf.setup.MP.verPos0[0];
  rtY_MCPdata->toTTKF.setup.MP.horVel0[0] =
    rtP->MCP_Global.ttkf.setup.MP.horVel0[0];
  rtY_MCPdata->toTTKF.setup.MP.verVel0[0] =
    rtP->MCP_Global.ttkf.setup.MP.verVel0[0];
  rtY_MCPdata->toTTKF.setup.MP.horPos0[1] =
    rtP->MCP_Global.ttkf.setup.MP.horPos0[1];
  rtY_MCPdata->toTTKF.setup.MP.verPos0[1] =
    rtP->MCP_Global.ttkf.setup.MP.verPos0[1];
  rtY_MCPdata->toTTKF.setup.MP.horVel0[1] =
    rtP->MCP_Global.ttkf.setup.MP.horVel0[1];
  rtY_MCPdata->toTTKF.setup.MP.verVel0[1] =
    rtP->MCP_Global.ttkf.setup.MP.verVel0[1];

  /* 'GUID_Embedded_Data_Control:340' DATA.toTTKF.setup.MP.horPosN      = single(MCP_Global.ttkf.setup.MP.horPosN); */
  rtY_MCPdata->toTTKF.setup.MP.horPosN = rtP->MCP_Global.ttkf.setup.MP.horPosN;

  /* 'GUID_Embedded_Data_Control:341' DATA.toTTKF.setup.MP.verPosN      = single(MCP_Global.ttkf.setup.MP.verPosN); */
  rtY_MCPdata->toTTKF.setup.MP.verPosN = rtP->MCP_Global.ttkf.setup.MP.verPosN;

  /* 'GUID_Embedded_Data_Control:342' DATA.toTTKF.setup.MP.horVelN      = single(MCP_Global.ttkf.setup.MP.horVelN); */
  rtY_MCPdata->toTTKF.setup.MP.horVelN = rtP->MCP_Global.ttkf.setup.MP.horVelN;

  /* 'GUID_Embedded_Data_Control:343' DATA.toTTKF.setup.MP.verVelN      = single(MCP_Global.ttkf.setup.MP.verVelN); */
  rtY_MCPdata->toTTKF.setup.MP.verVelN = rtP->MCP_Global.ttkf.setup.MP.verVelN;

  /* 'GUID_Embedded_Data_Control:344' DATA.toTTKF.setup.MP.Spare01      = uint32(zeros(1,10)); */
  /* 'GUID_Embedded_Data_Control:345' DATA.toTTKF.setup.Opp.horPos0     = single(MCP_Global.ttkf.setup.Opp.horPos0); */
  /* 'GUID_Embedded_Data_Control:346' DATA.toTTKF.setup.Opp.verPos0     = single(MCP_Global.ttkf.setup.Opp.verPos0); */
  /* 'GUID_Embedded_Data_Control:347' DATA.toTTKF.setup.Opp.horVel0     = single(MCP_Global.ttkf.setup.Opp.horVel0); */
  /* 'GUID_Embedded_Data_Control:348' DATA.toTTKF.setup.Opp.verVel0     = single(MCP_Global.ttkf.setup.Opp.verVel0); */
  rtY_MCPdata->toTTKF.setup.Opp.horPos0[0] =
    rtP->MCP_Global.ttkf.setup.Opp.horPos0[0];
  rtY_MCPdata->toTTKF.setup.Opp.verPos0[0] =
    rtP->MCP_Global.ttkf.setup.Opp.verPos0[0];
  rtY_MCPdata->toTTKF.setup.Opp.horVel0[0] =
    rtP->MCP_Global.ttkf.setup.Opp.horVel0[0];
  rtY_MCPdata->toTTKF.setup.Opp.verVel0[0] =
    rtP->MCP_Global.ttkf.setup.Opp.verVel0[0];
  rtY_MCPdata->toTTKF.setup.Opp.horPos0[1] =
    rtP->MCP_Global.ttkf.setup.Opp.horPos0[1];
  rtY_MCPdata->toTTKF.setup.Opp.verPos0[1] =
    rtP->MCP_Global.ttkf.setup.Opp.verPos0[1];
  rtY_MCPdata->toTTKF.setup.Opp.horVel0[1] =
    rtP->MCP_Global.ttkf.setup.Opp.horVel0[1];
  rtY_MCPdata->toTTKF.setup.Opp.verVel0[1] =
    rtP->MCP_Global.ttkf.setup.Opp.verVel0[1];

  /* 'GUID_Embedded_Data_Control:349' DATA.toTTKF.setup.Opp.horPosN     = single(MCP_Global.ttkf.setup.Opp.horPosN); */
  rtY_MCPdata->toTTKF.setup.Opp.horPosN = rtP->MCP_Global.ttkf.setup.Opp.horPosN;

  /* 'GUID_Embedded_Data_Control:350' DATA.toTTKF.setup.Opp.verPosN     = single(MCP_Global.ttkf.setup.Opp.verPosN); */
  rtY_MCPdata->toTTKF.setup.Opp.verPosN = rtP->MCP_Global.ttkf.setup.Opp.verPosN;

  /* 'GUID_Embedded_Data_Control:351' DATA.toTTKF.setup.Opp.horVelN     = single(MCP_Global.ttkf.setup.Opp.horVelN); */
  rtY_MCPdata->toTTKF.setup.Opp.horVelN = rtP->MCP_Global.ttkf.setup.Opp.horVelN;

  /* 'GUID_Embedded_Data_Control:352' DATA.toTTKF.setup.Opp.verVelN     = single(MCP_Global.ttkf.setup.Opp.verVelN); */
  rtY_MCPdata->toTTKF.setup.Opp.verVelN = rtP->MCP_Global.ttkf.setup.Opp.verVelN;

  /* 'GUID_Embedded_Data_Control:353' DATA.toTTKF.setup.Opp.Spare01     = uint32(zeros(1,10)); */
  /* 'GUID_Embedded_Data_Control:354' DATA.toTTKF.setup.Spare01         = uint32(zeros(1,10)); */
  for (i_0 = 0; i_0 < 10; i_0++) {
    rtY_MCPdata->toGuid.AirframeProp.Spare01[i_0] = 0U;
    rtY_MCPdata->toTTKF.setup.MP.Spare01[i_0] = 0U;
    rtY_MCPdata->toTTKF.setup.Opp.Spare01[i_0] = 0U;
    rtY_MCPdata->toTTKF.setup.Spare01[i_0] = 0U;
  }

  /* 'GUID_Embedded_Data_Control:355' DATA.toTTKF.BallisticRange        = single(LAR_out.Rge2Tgt); */
  rtY_MCPdata->toTTKF.BallisticRange = (real32_T)
    DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt;

  /* 'GUID_Embedded_Data_Control:359' DATA.toAutop.AirframeID  = AirframeID; */
  /* 'GUID_Embedded_Data_Control:360' DATA.toAutop.Spare01     = uint8(0); */
  rtY_MCPdata->toAutop.Spare01 = 0U;

  /* 'GUID_Embedded_Data_Control:361' DATA.toAutop.Spare02     = uint16(0); */
  rtY_MCPdata->toAutop.Spare02 = 0U;

  /* 'GUID_Embedded_Data_Control:362' DATA.toAutop.defl_limit  = MCP_Global.autop.defl_limit; */
  rtY_MCPdata->toAutop.defl_limit = rtP->MCP_Global.autop.defl_limit;

  /* 'GUID_Embedded_Data_Control:363' DATA.toAutop.t_delt      = MCP_Global.autop.t_delt; */
  rtY_MCPdata->toAutop.t_delt = rtP->MCP_Global.autop.t_delt;

  /* 'GUID_Embedded_Data_Control:365' DATA.toAutop.gainsR.MachVec   = single(MCP_Global.autop.gains(indP).MachVec); */
  for (i_0 = 0; i_0 < 6; i_0++) {
    rtY_MCPdata->toAutop.gainsR.MachVec[i_0] = rtP->MCP_Global.autop.gains[indP].
      MachVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:366' DATA.toAutop.gainsR.AlphaVec  = single(MCP_Global.autop.gains(indP).AlphaVec); */
  for (i_0 = 0; i_0 < 5; i_0++) {
    rtY_MCPdata->toAutop.gainsR.AlphaVec[i_0] = rtP->MCP_Global.autop.gains[indP]
      .AlphaVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:367' DATA.toAutop.gainsR.Kp_phi    = single(MCP_Global.autop.gains(indP).Kp_phi); */
  /* 'GUID_Embedded_Data_Control:368' DATA.toAutop.gainsR.Ki_phi    = single(MCP_Global.autop.gains(indP).Ki_phi); */
  /* 'GUID_Embedded_Data_Control:369' DATA.toAutop.gainsR.Kp_wbx    = single(MCP_Global.autop.gains(indP).Kp_wbx); */
  /* 'GUID_Embedded_Data_Control:370' DATA.toAutop.gainsR.SIGMAfilt = single(MCP_Global.autop.gains(indP).SIGMAfilt); */
  rtY_MCPdata->toAutop.gainsR.SIGMAfilt = rtP->MCP_Global.autop.gains[indP].
    SIGMAfilt;

  /* 'GUID_Embedded_Data_Control:371' DATA.toAutop.gainsR.NCP       = single(MCP_Global.autop.gains(indP).NCProll); */
  /* 'GUID_Embedded_Data_Control:372' DATA.toAutop.gainsR.RPlagZ    = single(MCP_Global.autop.gains(indP).RPLagZ); */
  /* 'GUID_Embedded_Data_Control:373' DATA.toAutop.gainsR.RPlagP    = single(MCP_Global.autop.gains(indP).RPLagP); */
  /* 'GUID_Embedded_Data_Control:374' DATA.toAutop.gainsR.NCW       = single(MCP_Global.autop.gains(indP).NCWroll); */
  /* 'GUID_Embedded_Data_Control:375' DATA.toAutop.gainsR.RRleadZ   = single(MCP_Global.autop.gains(indP).RRLeadZ); */
  /* 'GUID_Embedded_Data_Control:376' DATA.toAutop.gainsR.RRleadP   = single(MCP_Global.autop.gains(indP).RRLeadP); */
  /* 'GUID_Embedded_Data_Control:377' DATA.toAutop.gainsR.RRlagZ    = single(MCP_Global.autop.gains(indP).RRLagZ); */
  /* 'GUID_Embedded_Data_Control:378' DATA.toAutop.gainsR.RRlagP    = single(MCP_Global.autop.gains(indP).RRLagP); */
  memcpy(&rtY_MCPdata->toAutop.gainsR.Kp_phi[0],
         &rtP->MCP_Global.autop.gains[indP].Kp_phi[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.Ki_phi[0],
         &rtP->MCP_Global.autop.gains[indP].Ki_phi[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.Kp_wbx[0],
         &rtP->MCP_Global.autop.gains[indP].Kp_wbx[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.NCP[0], &rtP->MCP_Global.autop.gains[indP].
         NCProll[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RPlagZ[0],
         &rtP->MCP_Global.autop.gains[indP].RPLagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RPlagP[0],
         &rtP->MCP_Global.autop.gains[indP].RPLagP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.NCW[0], &rtP->MCP_Global.autop.gains[indP].
         NCWroll[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RRleadZ[0],
         &rtP->MCP_Global.autop.gains[indP].RRLeadZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RRleadP[0],
         &rtP->MCP_Global.autop.gains[indP].RRLeadP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RRlagZ[0],
         &rtP->MCP_Global.autop.gains[indP].RRLagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsR.RRlagP[0],
         &rtP->MCP_Global.autop.gains[indP].RRLagP[0], 30U * (sizeof(real32_T)));

  /* 'GUID_Embedded_Data_Control:381' DATA.toAutop.gainsP.MachVec   = single(MCP_Global.autop.gains(indP).MachVec); */
  for (i_0 = 0; i_0 < 6; i_0++) {
    rtY_MCPdata->toAutop.gainsP.MachVec[i_0] = rtP->MCP_Global.autop.gains[indP].
      MachVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:382' DATA.toAutop.gainsP.AlphaVec  = single(MCP_Global.autop.gains(indP).AlphaVec); */
  for (i_0 = 0; i_0 < 5; i_0++) {
    rtY_MCPdata->toAutop.gainsP.AlphaVec[i_0] = rtP->MCP_Global.autop.gains[indP]
      .AlphaVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:383' DATA.toAutop.gainsP.Kpa       = single(MCP_Global.autop.gains(indP).Kpa); */
  /* 'GUID_Embedded_Data_Control:384' DATA.toAutop.gainsP.Kiw       = single(MCP_Global.autop.gains(indP).Kiw); */
  /* 'GUID_Embedded_Data_Control:385' DATA.toAutop.gainsP.Kpw       = single(MCP_Global.autop.gains(indP).Kpw); */
  /* 'GUID_Embedded_Data_Control:386' DATA.toAutop.gainsP.ALPHAfilt = single(MCP_Global.autop.gains(indP).ALPHAfilt); */
  rtY_MCPdata->toAutop.gainsP.ALPHAfilt = rtP->MCP_Global.autop.gains[indP].
    ALPHAfilt;

  /* 'GUID_Embedded_Data_Control:387' DATA.toAutop.gainsP.NCA       = single(MCP_Global.autop.gains(indP).NCAlat); */
  /* 'GUID_Embedded_Data_Control:388' DATA.toAutop.gainsP.LAlowpass = single(MCP_Global.autop.gains(indP).LAlowpass); */
  /* 'GUID_Embedded_Data_Control:389' DATA.toAutop.gainsP.LAleadZ   = single(MCP_Global.autop.gains(indP).LALeadZ); */
  /* 'GUID_Embedded_Data_Control:390' DATA.toAutop.gainsP.LAleadP   = single(MCP_Global.autop.gains(indP).LALeadP); */
  /* 'GUID_Embedded_Data_Control:391' DATA.toAutop.gainsP.LAlagZ    = single(MCP_Global.autop.gains(indP).LALagZ); */
  /* 'GUID_Embedded_Data_Control:392' DATA.toAutop.gainsP.LAlagP    = single(MCP_Global.autop.gains(indP).LALagP); */
  /* 'GUID_Embedded_Data_Control:393' DATA.toAutop.gainsP.NCW       = single(MCP_Global.autop.gains(indP).NCWlat); */
  /* 'GUID_Embedded_Data_Control:394' DATA.toAutop.gainsP.LWleadZ   = single(MCP_Global.autop.gains(indP).LWLeadZ); */
  /* 'GUID_Embedded_Data_Control:395' DATA.toAutop.gainsP.LWleadP   = single(MCP_Global.autop.gains(indP).LWLeadP); */
  /* 'GUID_Embedded_Data_Control:396' DATA.toAutop.gainsP.LWlagZ    = single(MCP_Global.autop.gains(indP).LWLagZ); */
  /* 'GUID_Embedded_Data_Control:397' DATA.toAutop.gainsP.LWlagP    = single(MCP_Global.autop.gains(indP).LWLagP); */
  memcpy(&rtY_MCPdata->toAutop.gainsP.Kpa[0], &rtP->MCP_Global.autop.gains[indP].
         Kpa[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.Kiw[0], &rtP->MCP_Global.autop.gains[indP].
         Kiw[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.Kpw[0], &rtP->MCP_Global.autop.gains[indP].
         Kpw[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.NCA[0], &rtP->MCP_Global.autop.gains[indP].
         NCAlat[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LAlowpass[0],
         &rtP->MCP_Global.autop.gains[indP].LAlowpass[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LAleadZ[0],
         &rtP->MCP_Global.autop.gains[indP].LALeadZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LAleadP[0],
         &rtP->MCP_Global.autop.gains[indP].LALeadP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LAlagZ[0],
         &rtP->MCP_Global.autop.gains[indP].LALagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LAlagP[0],
         &rtP->MCP_Global.autop.gains[indP].LALagP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.NCW[0], &rtP->MCP_Global.autop.gains[indP].
         NCWlat[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LWleadZ[0],
         &rtP->MCP_Global.autop.gains[indP].LWLeadZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LWleadP[0],
         &rtP->MCP_Global.autop.gains[indP].LWLeadP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LWlagZ[0],
         &rtP->MCP_Global.autop.gains[indP].LWLagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsP.LWlagP[0],
         &rtP->MCP_Global.autop.gains[indP].LWLagP[0], 30U * (sizeof(real32_T)));

  /* 'GUID_Embedded_Data_Control:400' DATA.toAutop.gainsY.MachVec   = single(MCP_Global.autop.gains(indY).MachVec); */
  for (i_0 = 0; i_0 < 6; i_0++) {
    rtY_MCPdata->toAutop.gainsY.MachVec[i_0] = rtP->MCP_Global.autop.gains[indY].
      MachVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:401' DATA.toAutop.gainsY.AlphaVec  = single(MCP_Global.autop.gains(indY).AlphaVec); */
  for (i_0 = 0; i_0 < 5; i_0++) {
    rtY_MCPdata->toAutop.gainsY.AlphaVec[i_0] = rtP->MCP_Global.autop.gains[indY]
      .AlphaVec[i_0];
  }

  /* 'GUID_Embedded_Data_Control:402' DATA.toAutop.gainsY.Kpa       = single(MCP_Global.autop.gains(indY).Kpa); */
  /* 'GUID_Embedded_Data_Control:403' DATA.toAutop.gainsY.Kiw       = single(MCP_Global.autop.gains(indY).Kiw); */
  /* 'GUID_Embedded_Data_Control:404' DATA.toAutop.gainsY.Kpw       = single(MCP_Global.autop.gains(indY).Kpw); */
  /* 'GUID_Embedded_Data_Control:405' DATA.toAutop.gainsY.ALPHAfilt = single(MCP_Global.autop.gains(indY).ALPHAfilt); */
  rtY_MCPdata->toAutop.gainsY.ALPHAfilt = rtP->MCP_Global.autop.gains[indY].
    ALPHAfilt;

  /* 'GUID_Embedded_Data_Control:406' DATA.toAutop.gainsY.NCA       = single(MCP_Global.autop.gains(indY).NCAlat); */
  /* 'GUID_Embedded_Data_Control:407' DATA.toAutop.gainsY.LAlowpass = single(MCP_Global.autop.gains(indY).LAlowpass); */
  /* 'GUID_Embedded_Data_Control:408' DATA.toAutop.gainsY.LAleadZ   = single(MCP_Global.autop.gains(indY).LALeadZ); */
  /* 'GUID_Embedded_Data_Control:409' DATA.toAutop.gainsY.LAleadP   = single(MCP_Global.autop.gains(indY).LALeadP); */
  /* 'GUID_Embedded_Data_Control:410' DATA.toAutop.gainsY.LAlagZ    = single(MCP_Global.autop.gains(indY).LALagZ); */
  /* 'GUID_Embedded_Data_Control:411' DATA.toAutop.gainsY.LAlagP    = single(MCP_Global.autop.gains(indY).LALagP); */
  /* 'GUID_Embedded_Data_Control:412' DATA.toAutop.gainsY.NCW       = single(MCP_Global.autop.gains(indY).NCWlat); */
  /* 'GUID_Embedded_Data_Control:413' DATA.toAutop.gainsY.LWleadZ   = single(MCP_Global.autop.gains(indY).LWLeadZ); */
  /* 'GUID_Embedded_Data_Control:414' DATA.toAutop.gainsY.LWleadP   = single(MCP_Global.autop.gains(indY).LWLeadP); */
  /* 'GUID_Embedded_Data_Control:415' DATA.toAutop.gainsY.LWlagZ    = single(MCP_Global.autop.gains(indY).LWLagZ); */
  /* 'GUID_Embedded_Data_Control:416' DATA.toAutop.gainsY.LWlagP    = single(MCP_Global.autop.gains(indY).LWLagP); */
  memcpy(&rtY_MCPdata->toAutop.gainsY.Kpa[0], &rtP->MCP_Global.autop.gains[indY].
         Kpa[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.Kiw[0], &rtP->MCP_Global.autop.gains[indY].
         Kiw[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.Kpw[0], &rtP->MCP_Global.autop.gains[indY].
         Kpw[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.NCA[0], &rtP->MCP_Global.autop.gains[indY].
         NCAlat[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LAlowpass[0],
         &rtP->MCP_Global.autop.gains[indY].LAlowpass[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LAleadZ[0],
         &rtP->MCP_Global.autop.gains[indY].LALeadZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LAleadP[0],
         &rtP->MCP_Global.autop.gains[indY].LALeadP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LAlagZ[0],
         &rtP->MCP_Global.autop.gains[indY].LALagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LAlagP[0],
         &rtP->MCP_Global.autop.gains[indY].LALagP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.NCW[0], &rtP->MCP_Global.autop.gains[indY].
         NCWlat[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LWleadZ[0],
         &rtP->MCP_Global.autop.gains[indY].LWLeadZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LWleadP[0],
         &rtP->MCP_Global.autop.gains[indY].LWLeadP[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LWlagZ[0],
         &rtP->MCP_Global.autop.gains[indY].LWLagZ[0], 30U * (sizeof(real32_T)));
  memcpy(&rtY_MCPdata->toAutop.gainsY.LWlagP[0],
         &rtP->MCP_Global.autop.gains[indY].LWLagP[0], 30U * (sizeof(real32_T)));

  /* 'GUID_Embedded_Data_Control:418' if (umbilical && calcLAR) */
  if ((((int32_T)Guidin_umbilical) != 0) && (((int32_T)rtU_calcLAR) != 0)) {
    /* 'GUID_Embedded_Data_Control:420' if (NavActive) */
    /* 'GUID_Embedded_Data_Control:424' if (Guidin.OpportunityMode) */
    if (((int32_T)Guidin_OpportunityMode) != 0) {
      /* 'GUID_Embedded_Data_Control:426' machlim = LIB_limit(0.7,0.9,LARin.launch_data.mach); */
      /* 'LIB_limit:27' if (in > max) */
      if (Guidin_launch_data.mach > 0.9) {
        /* 'LIB_limit:28' out = max; */
        machlim = 0.9;
      } else if (Guidin_launch_data.mach < 0.7) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        machlim = 0.7;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        machlim = Guidin_launch_data.mach;
      }

      /* 'GUID_Embedded_Data_Control:427' if machlim < 0.8 */
      if (machlim < 0.8) {
        /* 'GUID_Embedded_Data_Control:428' mrat = (machlim - 0.7)/0.1; */
        machlim = (machlim - 0.7) / 0.1;

        /* 'GUID_Embedded_Data_Control:429' RGE1 = double(LAR_data.RBAL070(:,:,1))'; */
        /* 'GUID_Embedded_Data_Control:430' RGE2 = double(LAR_data.RBAL080(:,:,1))'; */
        /* 'GUID_Embedded_Data_Control:431' TOF1 = double(LAR_data.RBAL070(:,:,2))'; */
        /* 'GUID_Embedded_Data_Control:432' TOF2 = double(LAR_data.RBAL080(:,:,2))'; */
        for (i_0 = 0; i_0 < 3; i_0++) {
          for (i_1 = 0; i_1 < 6; i_1++) {
            indY = (3 * i_1) + i_0;
            indP = i_1 + (6 * i_0);
            RGE1[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.data[indY];
            RGE2[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[indY];
            indY += 18;
            TOF1[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL070.data[indY];
            TOF2[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[indY];
          }
        }
      } else {
        /* 'GUID_Embedded_Data_Control:433' else */
        /* 'GUID_Embedded_Data_Control:434' mrat = (machlim - 0.8)/0.1; */
        machlim = (machlim - 0.8) / 0.1;

        /* 'GUID_Embedded_Data_Control:435' RGE1 = double(LAR_data.RBAL080(:,:,1))'; */
        /* 'GUID_Embedded_Data_Control:436' RGE2 = double(LAR_data.RBAL090(:,:,1))'; */
        /* 'GUID_Embedded_Data_Control:437' TOF1 = double(LAR_data.RBAL080(:,:,2))'; */
        /* 'GUID_Embedded_Data_Control:438' TOF2 = double(LAR_data.RBAL090(:,:,2))'; */
        for (i_0 = 0; i_0 < 3; i_0++) {
          for (i_1 = 0; i_1 < 6; i_1++) {
            indY = (3 * i_1) + i_0;
            indP = i_1 + (6 * i_0);
            RGE1[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[indY];
            RGE2[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.data[indY];
            indY += 18;
            TOF1[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL080.data[indY];
            TOF2[indP] = (real_T)
              DataControl_10Hz_Function_M->dwork.LAR_data.RBAL090.data[indY];
          }
        }
      }

      /* 'GUID_Embedded_Data_Control:440' tgt_alt = LIB_limit(double(LAR_data.alttgt(1)),double(LAR_data.alttgt(end)),LARin.tgt_data.alt); */
      /* 'LIB_limit:27' if (in > max) */
      if (relalt > ((real_T)DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[2]))
      {
        /* 'LIB_limit:28' out = max; */
        relalt = (real_T)DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[2];
      } else if (relalt < ((real_T)
                           DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[0]))
      {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = (real_T)DataControl_10Hz_Function_M->dwork.LAR_data.alttgt[0];
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'GUID_Embedded_Data_Control:441' del_alt = LIB_limit(double(LAR_data.altvec(1)),double(LAR_data.altvec(end)),(LARin.launch_data.alt-tgt_alt)/0.3048); */
      del_alt = (Guidin_launch_data.alt - relalt) / 0.3048;

      /* 'LIB_limit:27' if (in > max) */
      if (del_alt > ((real_T)DataControl_10Hz_Function_M->dwork.LAR_data.altvec
                     [5])) {
        /* 'LIB_limit:28' out = max; */
        del_alt = (real_T)DataControl_10Hz_Function_M->dwork.LAR_data.altvec[5];
      } else if (del_alt < ((real_T)
                            DataControl_10Hz_Function_M->dwork.LAR_data.altvec[0]))
      {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        del_alt = (real_T)DataControl_10Hz_Function_M->dwork.LAR_data.altvec[0];
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      /* 'GUID_Embedded_Data_Control:444' rge1 = interp2(double(LAR_data.alttgt),double(LAR_data.altvec),RGE1,tgt_alt,del_alt); */
      T_IMP_data[0] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[0];
      T_IMP_data[1] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[1];
      T_IMP_data[2] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[2];
      for (i_0 = 0; i_0 < 6; i_0++) {
        tmp_0[i_0] = (real_T)DataControl_10Hz_Function_M->
          dwork.LAR_data.altvec[i_0];
      }

      RelX = interp2_OIomwLVf(T_IMP_data, tmp_0, RGE1, relalt, del_alt);

      /* 'GUID_Embedded_Data_Control:445' tof1 = interp2(double(LAR_data.alttgt),double(LAR_data.altvec),TOF1,tgt_alt,del_alt); */
      T_IMP_data[0] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[0];
      T_IMP_data[1] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[1];
      T_IMP_data[2] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[2];
      for (i_0 = 0; i_0 < 6; i_0++) {
        tmp_0[i_0] = (real_T)DataControl_10Hz_Function_M->
          dwork.LAR_data.altvec[i_0];
      }

      Rge2Tgt = interp2_OIomwLVf(T_IMP_data, tmp_0, TOF1, relalt, del_alt);

      /* 'GUID_Embedded_Data_Control:447' rge2 = interp2(double(LAR_data.alttgt),double(LAR_data.altvec),RGE2,tgt_alt,del_alt); */
      /* 'GUID_Embedded_Data_Control:448' tof2 = interp2(double(LAR_data.alttgt),double(LAR_data.altvec),TOF2,tgt_alt,del_alt); */
      /* 'GUID_Embedded_Data_Control:451' BallisticRange = rge1 + mrat*(rge2 - rge1); */
      T_IMP_data[0] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[0];
      T_IMP_data[1] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[1];
      T_IMP_data[2] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[2];
      for (i_0 = 0; i_0 < 6; i_0++) {
        tmp_0[i_0] = (real_T)DataControl_10Hz_Function_M->
          dwork.LAR_data.altvec[i_0];
      }

      RelX += (interp2_OIomwLVf(T_IMP_data, tmp_0, RGE2, relalt, del_alt) - RelX)
        * machlim;

      /* 'GUID_Embedded_Data_Control:452' BallisticFTime = tof1 + mrat*(tof2 - tof1); */
      T_IMP_data[0] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[0];
      T_IMP_data[1] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[1];
      T_IMP_data[2] = (real_T)DataControl_10Hz_Function_M->
        dwork.LAR_data.alttgt[2];
      for (i_0 = 0; i_0 < 6; i_0++) {
        tmp_0[i_0] = (real_T)DataControl_10Hz_Function_M->
          dwork.LAR_data.altvec[i_0];
      }

      relalt = ((interp2_OIomwLVf(T_IMP_data, tmp_0, TOF2, relalt, del_alt) -
                 Rge2Tgt) * machlim) + Rge2Tgt;

      /* 'GUID_Embedded_Data_Control:454' LAR_out.Rge2Tgt  = uint32(LIB_limit(0.0,250000.0,BallisticRange)); */
      /* 'LIB_limit:27' if (in > max) */
      if (RelX > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        RelX = 250000.0;
      } else if (RelX < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        RelX = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(RelX);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:455' LAR_out.Time2Imp = uint16(LIB_limit(0.0,1200.0,BallisticFTime)); */
      /* 'LIB_limit:27' if (in > max) */
      if (relalt > 1200.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 1200.0;
      } else if (relalt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Imp = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Imp = MAX_uint16_T;
      }
    } else {
      /* 'GUID_Embedded_Data_Control:458' else */
      /* 'GUID_Embedded_Data_Control:460' LAR = LARCalculations(LARin.tgt_data,LARin.launch_data,LAR_data,LAR_traj,TestCfg_in); */
      LARCalculations(Guidin_tgt_data_valid, RelX, RelY, machlim, del_alt,
                      Rge2Tgt, radius, relalt, centre_offX, TPosY,
                      &Guidin_launch_data, &(DataControl_10Hz_Function_M)
                      ->dwork.LAR_data, &(DataControl_10Hz_Function_M)
                      ->dwork.LAR_traj, *rtU_TestCfgin, &expl_temp,
                      &(DataControl_10Hz_Function_M->dwork));
      radius = expl_temp.radius;
      TPosY = expl_temp.exclrad;
      centre_offX = expl_temp.Psi2LAR;
      LAR_Psi2Tgt = expl_temp.Psi2Tgt;
      Rge2LAR = expl_temp.Rge2LAR;
      Rge2Tgt = expl_temp.Rge2Tgt;
      machlim = expl_temp.Time2Imp;
      del_alt = expl_temp.Time2Lase;

      /* 'GUID_Embedded_Data_Control:461' if (TestCfg_in.GC_Mode > def.MC_OPER) */
      if (((int32_T)rtU_TestCfgin->GC_Mode) > 0) {
        /* 'GUID_Embedded_Data_Control:463' exclrad = 0.0; */
        /* 'GUID_Embedded_Data_Control:464' radius  = 0.0; */
        radius = 0.0;

        /* 'GUID_Embedded_Data_Control:465' centre_offX = 10000.0; */
        centre_offX = 10000.0;

        /* 'GUID_Embedded_Data_Control:466' centre_offY = 0.0; */
        ind = 0;

        /* 'GUID_Embedded_Data_Control:467' T_IMP = [56.34 65.00 73.51]; */
        T_IMP_size[0] = 1;
        T_IMP_size[1] = 3;

        /* 'GUID_Embedded_Data_Control:468' R_IMP = [11184 13004 14871]; */
        R_IMP_size[0] = 1;
        R_IMP_size[1] = 3;
        T_IMP_data[0] = 56.34;
        R_IMP_data[0] = 11184.0;
        T_IMP_data[1] = 65.0;
        R_IMP_data[1] = 13004.0;
        T_IMP_data[2] = 73.51;
        R_IMP_data[2] = 14871.0;

        /* 'GUID_Embedded_Data_Control:469' if (TestCfg_in.GC_Mode == def.QAT2S_PROF1) */
        if (((int32_T)rtU_TestCfgin->GC_Mode) == 44) {
          /* 'GUID_Embedded_Data_Control:471' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:473' radius = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:474'                            [0     0  2750  3000  3250     0      0],... */
          /* 'GUID_Embedded_Data_Control:475'                            relalt); */
          radius = interp1_bFwkvuIc(e, f, relalt);

          /* 'GUID_Embedded_Data_Control:476' centre_offX = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:477' 	             	              [0  8000 11000 12000 13000 15000  20000],... */
          /* 'GUID_Embedded_Data_Control:478'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(e, i, relalt);

          /* 'GUID_Embedded_Data_Control:479' centre_offY = 5000.0; */
          ind = 5000;

          /* 'GUID_Embedded_Data_Control:480' T_IMP = [56.34 65.00 73.51]; */
          /* 'GUID_Embedded_Data_Control:481' R_IMP = [11184 13004 14871]; */
        } else if (((int32_T)rtU_TestCfgin->GC_Mode) == 45) {
          /* 'GUID_Embedded_Data_Control:483' elseif (TestCfg_in.GC_Mode == def.QAT2S_PROF2) */
          /* 'GUID_Embedded_Data_Control:485' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:487' radius = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:488'                            [0     0  2750  3000  3250     0      0],... */
          /* 'GUID_Embedded_Data_Control:489'                            relalt); */
          radius = interp1_bFwkvuIc(e, f, relalt);

          /* 'GUID_Embedded_Data_Control:490' centre_offX = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:491' 	             	            [0  4000  7000  8000  9000 11000  16000],... */
          /* 'GUID_Embedded_Data_Control:492'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(e, m, relalt);

          /* 'GUID_Embedded_Data_Control:493' centre_offY = 0.0; */
          /* 'GUID_Embedded_Data_Control:494' T_IMP = [56.34 65.00 73.51]; */
          /* 'GUID_Embedded_Data_Control:495' R_IMP = [11184 13004 14871]; */
        } else if (((int32_T)rtU_TestCfgin->GC_Mode) == 46) {
          /* 'GUID_Embedded_Data_Control:497' elseif (TestCfg_in.GC_Mode == def.QAT2S_PROF3) */
          /* 'GUID_Embedded_Data_Control:499' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:501' radius = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:502'                            [0     0     0     0     0     0      0],... */
          /* 'GUID_Embedded_Data_Control:503'                            relalt); */
          for (i_0 = 0; i_0 < 7; i_0++) {
            tmp[i_0] = 0.0;
          }

          radius = interp1_bFwkvuIc(e, tmp, relalt);

          /* 'GUID_Embedded_Data_Control:504' centre_offX = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:505' 	             	            [0  4000  5000  6000  7000  8000  12000],... */
          /* 'GUID_Embedded_Data_Control:506'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(e, n, relalt);

          /* 'GUID_Embedded_Data_Control:507' centre_offY = 0.0; */
          /* 'GUID_Embedded_Data_Control:508' T_IMP = [56.34 65.00 73.51]; */
          /* 'GUID_Embedded_Data_Control:509' R_IMP = [11184 13004 14871]; */
        } else if (((int32_T)rtU_TestCfgin->GC_Mode) == 49) {
          /* 'GUID_Embedded_Data_Control:511' elseif (TestCfg_in.GC_Mode == def.QAT2S_PROF5) */
          /* 'GUID_Embedded_Data_Control:513' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:515' radius = interp1([0  3000  4000  5000  6000  8000 100000],... */
          /* 'GUID_Embedded_Data_Control:516'                            [0     0  1250  1500  1500     0      0],... */
          /* 'GUID_Embedded_Data_Control:517'                            relalt); */
          radius = interp1_bFwkvuIc(g, h, relalt);

          /* 'GUID_Embedded_Data_Control:518' centre_offX = interp1([0  3000  4000  5000  6000  8000 100000],... */
          /* 'GUID_Embedded_Data_Control:519' 	             	            [0  1750  2250  2750  3250  5250  16000],... */
          /* 'GUID_Embedded_Data_Control:520'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(g, o, relalt);

          /* 'GUID_Embedded_Data_Control:521' centre_offY = 0.0; */
          /* 'GUID_Embedded_Data_Control:522' T_IMP = [11.75 12.98 18.86]; */
          T_IMP_size[0] = 1;
          T_IMP_size[1] = 3;

          /* 'GUID_Embedded_Data_Control:523' R_IMP = [ 2157  2742  4238]; */
          R_IMP_size[0] = 1;
          R_IMP_size[1] = 3;
          T_IMP_data[0] = 11.75;
          R_IMP_data[0] = 2157.0;
          T_IMP_data[1] = 12.98;
          R_IMP_data[1] = 2742.0;
          T_IMP_data[2] = 18.86;
          R_IMP_data[2] = 4238.0;
        } else if (((int32_T)rtU_TestCfgin->GC_Mode) == 50) {
          /* 'GUID_Embedded_Data_Control:525' elseif (TestCfg_in.GC_Mode == def.QAT2S_PROF6) */
          /* 'GUID_Embedded_Data_Control:527' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:529' radius = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:530'                            [0     0  2250  2500  2500     0      0],... */
          /* 'GUID_Embedded_Data_Control:531'                            relalt); */
          radius = interp1_bFwkvuIc(e, j, relalt);

          /* 'GUID_Embedded_Data_Control:532' centre_offX = interp1([0 12000 14000 15000 16000 18000 100000],... */
          /* 'GUID_Embedded_Data_Control:533' 	             	            [0 10000 16000 18000 19000 21000  21000],... */
          /* 'GUID_Embedded_Data_Control:534'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(e, p, relalt);

          /* 'GUID_Embedded_Data_Control:535' centre_offY = 0.0; */
          /* 'GUID_Embedded_Data_Control:536' T_IMP = [64.77 75.83 89.07]; */
          T_IMP_size[0] = 1;
          T_IMP_size[1] = 3;

          /* 'GUID_Embedded_Data_Control:537' R_IMP = [16014 18517 21020]; */
          R_IMP_size[0] = 1;
          R_IMP_size[1] = 3;
          T_IMP_data[0] = 64.77;
          R_IMP_data[0] = 16014.0;
          T_IMP_data[1] = 75.83;
          R_IMP_data[1] = 18517.0;
          T_IMP_data[2] = 89.07;
          R_IMP_data[2] = 21020.0;
        } else if (((int32_T)rtU_TestCfgin->GC_Mode) == 47) {
          /* 'GUID_Embedded_Data_Control:539' elseif (TestCfg_in.GC_Mode == def.QAT3S_PROF4) */
          /* 'GUID_Embedded_Data_Control:541' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
          relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

          /* 'LIB_limit:27' if (in > max) */
          if (relalt > 50000.0) {
            /* 'LIB_limit:28' out = max; */
            relalt = 50000.0;
          } else if (relalt < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            relalt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
          }

          /* 'GUID_Embedded_Data_Control:543' radius = interp1([0 12000 14000 15000 18000 20000 100000],... */
          /* 'GUID_Embedded_Data_Control:544'                            [0     0  2750  3000  3500     0      0],... */
          /* 'GUID_Embedded_Data_Control:545'                            relalt); */
          radius = interp1_bFwkvuIc(k, l, relalt);

          /* 'GUID_Embedded_Data_Control:546' centre_offX = interp1([0 12000 14000 15000 18000 20000 100000],... */
          /* 'GUID_Embedded_Data_Control:547' 	             	            [0  5000  6000  6500  8000  8000  12000],... */
          /* 'GUID_Embedded_Data_Control:548'                                 relalt); */
          centre_offX = interp1_bFwkvuIc(k, q, relalt);

          /* 'GUID_Embedded_Data_Control:549' centre_offY = 0.0; */
          /* 'GUID_Embedded_Data_Control:550' T_IMP = [28.31 38.58]; */
          T_IMP_size[0] = 1;
          T_IMP_size[1] = 2;

          /* 'GUID_Embedded_Data_Control:551' R_IMP = [ 5640  7755]; */
          R_IMP_size[0] = 1;
          R_IMP_size[1] = 2;
          T_IMP_data[0] = 28.31;
          R_IMP_data[0] = 5640.0;
          T_IMP_data[1] = 38.58;
          R_IMP_data[1] = 7755.0;
        } else {
          if (((int32_T)rtU_TestCfgin->GC_Mode) == 48) {
            /* 'GUID_Embedded_Data_Control:553' elseif (TestCfg_in.GC_Mode == def.QAT3S_PROF5) */
            /* 'GUID_Embedded_Data_Control:555' relalt = LIB_limit(0.0,50000.0,(LARin.launch_data.alt - LARin.tgt_data.alt)/0.3048); */
            relalt = (Guidin_launch_data.alt - relalt) / 0.3048;

            /* 'LIB_limit:27' if (in > max) */
            if (relalt > 50000.0) {
              /* 'LIB_limit:28' out = max; */
              relalt = 50000.0;
            } else if (relalt < 0.0) {
              /* 'LIB_limit:29' elseif (in < min) */
              /* 'LIB_limit:30' out = min; */
              relalt = 0.0;
            } else {
              /* 'LIB_limit:31' else */
              /* 'LIB_limit:32' out = in; */
            }

            /* 'GUID_Embedded_Data_Control:557' radius = interp1([0 12000 14000 15000 16000 18000 100000],... */
            /* 'GUID_Embedded_Data_Control:558'                            [0     0     0     0     0     0      0],... */
            /* 'GUID_Embedded_Data_Control:559'                            relalt); */
            for (i_0 = 0; i_0 < 7; i_0++) {
              tmp[i_0] = 0.0;
            }

            radius = interp1_bFwkvuIc(e, tmp, relalt);

            /* 'GUID_Embedded_Data_Control:560' centre_offX = interp1([0 12000 14000 15000 16000 18000 100000],... */
            /* 'GUID_Embedded_Data_Control:561' 	             	            [0  4000  5000  6000  7000  8000  12000],... */
            /* 'GUID_Embedded_Data_Control:562'                                 relalt); */
            centre_offX = interp1_bFwkvuIc(e, n, relalt);

            /* 'GUID_Embedded_Data_Control:563' centre_offY = 0.0; */
            /* 'GUID_Embedded_Data_Control:564' T_IMP = [56.34 65.00 73.51]; */
            /* 'GUID_Embedded_Data_Control:565' R_IMP = [11184 13004 14871]; */
          }
        }

        /* 'GUID_Embedded_Data_Control:569' Vmag = LARin.launch_data.Vgnd; */
        /* 'GUID_Embedded_Data_Control:570' Vpsi = LARin.launch_data.track; */
        /* 'GUID_Embedded_Data_Control:572' TPosX =  LARin.tgt_data.rgx*cos(Vpsi) + LARin.tgt_data.rgy*sin(Vpsi); */
        Rge2Tgt = sin(Guidin_launch_data.track);
        TPosY = cos(Guidin_launch_data.track);
        relalt = (RelX * TPosY) + (RelY * Rge2Tgt);

        /* 'GUID_Embedded_Data_Control:573' TPosY = -LARin.tgt_data.rgx*sin(Vpsi) + LARin.tgt_data.rgy*cos(Vpsi); */
        TPosY = ((-RelX) * Rge2Tgt) + (RelY * TPosY);

        /* 'GUID_Embedded_Data_Control:574' Rge2Tgt = sqrt(TPosX^2+TPosY^2); */
        Rge2Tgt = sqrt((relalt * relalt) + (TPosY * TPosY));

        /* 'GUID_Embedded_Data_Control:576' RelX = TPosX - centre_offX; */
        RelX = relalt - centre_offX;

        /* 'GUID_Embedded_Data_Control:577' RelY = TPosY - centre_offY; */
        RelY = TPosY - ((real_T)ind);

        /* 'GUID_Embedded_Data_Control:579' Psi2LAR = -atan2(-RelY,RelX); */
        LAR_Psi2Tgt = rt_atan2d_snf(-RelY, RelX);
        centre_offX = -LAR_Psi2Tgt;

        /* 'GUID_Embedded_Data_Control:580' if (Psi2LAR < 0.0) */
        if ((-LAR_Psi2Tgt) < 0.0) {
          /* 'GUID_Embedded_Data_Control:581' Psi2LAR = Psi2LAR + 2*pi; */
          centre_offX = (-LAR_Psi2Tgt) + 6.2831853071795862;
        }

        /* 'GUID_Embedded_Data_Control:583' Psi2Tgt = -atan2(-TPosY,TPosX); */
        relalt = rt_atan2d_snf(-TPosY, relalt);
        LAR_Psi2Tgt = -relalt;

        /* 'GUID_Embedded_Data_Control:584' if (Psi2Tgt < 0.0) */
        if ((-relalt) < 0.0) {
          /* 'GUID_Embedded_Data_Control:585' Psi2Tgt = Psi2Tgt + 2*pi; */
          LAR_Psi2Tgt = (-relalt) + 6.2831853071795862;
        }

        /* 'GUID_Embedded_Data_Control:588' LAR.radius = radius; */
        /* 'GUID_Embedded_Data_Control:589' LAR.exclrad = exclrad; */
        TPosY = 0.0;

        /* 'GUID_Embedded_Data_Control:591' LAR.Psi2LAR = Psi2LAR; */
        /* 'GUID_Embedded_Data_Control:592' LAR.Psi2Tgt = Psi2Tgt; */
        /* 'GUID_Embedded_Data_Control:594' Rge2LAR = sqrt(RelX^2+RelY^2); */
        Rge2LAR = sqrt((RelX * RelX) + (RelY * RelY));

        /* 'GUID_Embedded_Data_Control:595' LAR.Rge2LAR = Rge2LAR; */
        /* 'GUID_Embedded_Data_Control:596' LAR.Rge2Tgt = Rge2Tgt; */
        /* 'GUID_Embedded_Data_Control:598' if (Rge2LAR < radius) */
        if (Rge2LAR < radius) {
          /* 'GUID_Embedded_Data_Control:599' Time2Imp = interp1(R_IMP,T_IMP,Rge2Tgt,'linear','extrap'); */
          relalt = interp1(R_IMP_data, R_IMP_size, T_IMP_data, T_IMP_size,
                           Rge2Tgt);
        } else {
          /* 'GUID_Embedded_Data_Control:600' else */
          /* 'GUID_Embedded_Data_Control:601' Time2Imp = 0.0; */
          relalt = 0.0;
        }

        /* 'GUID_Embedded_Data_Control:603' if (TestCfg_in.GC_Mode == def.QAT3S_PROF4 || ... */
        /* 'GUID_Embedded_Data_Control:604'             TestCfg_in.GC_Mode == def.QAT3S_PROF5 || ... */
        /* 'GUID_Embedded_Data_Control:605'             TestCfg_in.GC_Mode == def.QAT2S_PROF5 || ... */
        /* 'GUID_Embedded_Data_Control:606'             TestCfg_in.GC_Mode == def.QAT2S_PROF6) */
        if ((((((int32_T)rtU_TestCfgin->GC_Mode) == 47) || (((int32_T)
                rtU_TestCfgin->GC_Mode) == 48)) || (((int32_T)
               rtU_TestCfgin->GC_Mode) == 49)) || (((int32_T)
              rtU_TestCfgin->GC_Mode) == 50)) {
          /* 'GUID_Embedded_Data_Control:607' LAR.Time2Imp = Time2Imp; */
          machlim = relalt;

          /* 'GUID_Embedded_Data_Control:608' LAR.Time2Lase = LIB_limit(0.0,1000.0,Time2Imp-30.0); */
          /* 'LIB_limit:27' if (in > max) */
          if ((relalt - 30.0) > 1000.0) {
            /* 'LIB_limit:28' out = max; */
            del_alt = 1000.0;
          } else if ((relalt - 30.0) < 0.0) {
            /* 'LIB_limit:29' elseif (in < min) */
            /* 'LIB_limit:30' out = min; */
            del_alt = 0.0;
          } else {
            /* 'LIB_limit:31' else */
            /* 'LIB_limit:32' out = in; */
            del_alt = relalt - 30.0;
          }
        }
      }

      /* 'GUID_Embedded_Data_Control:612' LAR_out.radius       = uint32(LIB_limit(0.0,250000.0,LAR.radius)); */
      /* 'LIB_limit:27' if (in > max) */
      if (radius > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        radius = 250000.0;
      } else if (radius < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        radius = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(radius);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.radius = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.radius = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:613' LAR_out.Psi2LAR      = uint16(LIB_limit(0.0,360.0,LAR.Psi2LAR*cnsts.r2d)); */
      relalt = centre_offX * 57.295779513082323;

      /* 'LIB_limit:27' if (in > max) */
      if (relalt > 360.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 360.0;
      } else if (relalt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:614' LAR_out.Psi2Tgt      = uint16(LIB_limit(0.0,360.0,LAR.Psi2Tgt*cnsts.r2d)); */
      relalt = LAR_Psi2Tgt * 57.295779513082323;

      /* 'LIB_limit:27' if (in > max) */
      if (relalt > 360.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 360.0;
      } else if (relalt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2Tgt = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2Tgt = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:615' LAR_out.Rge2LAR      = uint32(LIB_limit(0.0,250000.0,LAR.Rge2LAR)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rge2LAR > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        Rge2LAR = 250000.0;
      } else if (Rge2LAR < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rge2LAR = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(Rge2LAR);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:616' LAR_out.Rge2Tgt      = uint32(LIB_limit(0.0,250000.0,LAR.Rge2Tgt)); */
      /* 'LIB_limit:27' if (in > max) */
      if (Rge2Tgt > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        Rge2Tgt = 250000.0;
      } else if (Rge2Tgt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        Rge2Tgt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(Rge2Tgt);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2Tgt = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:617' LAR_out.exclrad      = uint32(LIB_limit(0.0,250000.0,LAR.exclrad)); */
      /* 'LIB_limit:27' if (in > max) */
      if (TPosY > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        TPosY = 250000.0;
      } else if (TPosY < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        TPosY = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(TPosY);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.exclrad = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.exclrad = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:618' LAR_out.Time2Imp     = uint16(LIB_limit(0.0,1200.0,LAR.Time2Imp)); */
      /* 'LIB_limit:27' if (in > max) */
      if (machlim > 1200.0) {
        /* 'LIB_limit:28' out = max; */
        machlim = 1200.0;
      } else if (machlim < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        machlim = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(machlim);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Imp = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Imp = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:619' LAR_out.Time2Lase    = uint16(LIB_limit(0.0,1200.0,LAR.Time2Lase)); */
      /* 'LIB_limit:27' if (in > max) */
      if (del_alt > 1200.0) {
        /* 'LIB_limit:28' out = max; */
        del_alt = 1200.0;
      } else if (del_alt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        del_alt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(del_alt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Lase = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Lase = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:620' LAR_out.Time2Go      = uint16(LIB_limit(0.0,1200.0,LAR.Time2Go)); */
      /* 'LIB_limit:27' if (in > max) */
      if (expl_temp.Time2Go > 1200.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 1200.0;
      } else if (expl_temp.Time2Go < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        relalt = expl_temp.Time2Go;
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Go = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Time2Go = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:621' LAR_out.Psi2LAR_Test = uint16(LIB_limit(0.0,360.0,LAR.Psi2LAR_Test*cnsts.r2d)); */
      relalt = expl_temp.Psi2LAR_Test * 57.295779513082323;

      /* 'LIB_limit:27' if (in > max) */
      if (relalt > 360.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 360.0;
      } else if (relalt < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 65536.0) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR_Test = (uint16_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Psi2LAR_Test = MAX_uint16_T;
      }

      /* 'GUID_Embedded_Data_Control:622' LAR_out.radius_Test  = uint32(LIB_limit(0.0,250000.0,LAR.radius_Test)); */
      /* 'LIB_limit:27' if (in > max) */
      if (expl_temp.radius_Test > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 250000.0;
      } else if (expl_temp.radius_Test < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        relalt = expl_temp.radius_Test;
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.radius_Test = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.radius_Test = MAX_uint32_T;
      }

      /* 'GUID_Embedded_Data_Control:623' LAR_out.Rge2LAR_Test = uint32(LIB_limit(0.0,250000.0,LAR.Rge2LAR_Test)); */
      /* 'LIB_limit:27' if (in > max) */
      if (expl_temp.Rge2LAR_Test > 250000.0) {
        /* 'LIB_limit:28' out = max; */
        relalt = 250000.0;
      } else if (expl_temp.Rge2LAR_Test < 0.0) {
        /* 'LIB_limit:29' elseif (in < min) */
        /* 'LIB_limit:30' out = min; */
        relalt = 0.0;
      } else {
        /* 'LIB_limit:31' else */
        /* 'LIB_limit:32' out = in; */
        relalt = expl_temp.Rge2LAR_Test;
      }

      RelX = rt_roundd_snf(relalt);
      if (RelX < 4.294967296E+9) {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR_Test = (uint32_T)RelX;
      } else {
        DataControl_10Hz_Function_M->dwork.LAR_out.Rge2LAR_Test = MAX_uint32_T;
      }
    }

    /* 'GUID_Embedded_Data_Control:628' if (0) */
  }

  /* Outport: '<Root>/LAR' incorporates:
   *  MATLAB Function: '<S1>/Data Controller'
   */
  /* 'GUID_Embedded_Data_Control:654' LARout = LAR_out; */
  *rtY_LAR = DataControl_10Hz_Function_M->dwork.LAR_out;

  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_call_datactrl_at_outport_1' */
}

/* Model initialize function */
void DataControl_10Hz_Function_initialize(RT_MODEL *const
  DataControl_10Hz_Function_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
  UNUSED_PARAMETER(DataControl_10Hz_Function_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
