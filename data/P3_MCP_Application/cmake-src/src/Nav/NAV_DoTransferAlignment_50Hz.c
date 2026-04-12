/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * File: NAV_DoTransferAlignment_50Hz.c
 *
 * Code generated for Simulink model 'NAV_DoTransferAlignment_50Hz'.
 *
 * Model version                  : 1.67
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Tue Sep 13 10:47:39 2022
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

#include "NAV_DoTransferAlignment_50Hz.h"
#include "rt_atan2d_snf.h"
#include "rt_powd_snf.h"

/* Forward declaration for local functions */
static void LIB_e2d(real_T eg_phi, real_T eg_the, real_T eg_psi, real_T dcm[9]);
static void LIB_d2e(const real_T dcm[9], real_T *eg_phi, real_T *eg_the, real_T *
                    eg_psi);

/*
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz'
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
 * Function for MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz'
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

/* Model step function */
void NAV_DoNavigation_50Hz(RT_MODEL *const NAV_DoTransferAlignment_50Hz_M,
  T_RTAC_TransferAlignment *rtU_RTAC_Data, real_T rtU_Time,
  T_NAV_GenericInertialNavigationSensor *rtU_GenericINSensor100Hz,
  T_TA_TransferAlignState *rtY_TransferAlignState)
{
  real_T norm_imu_dvelocity;
  real_T rate_temp[9];
  real_T dcm_ne[9];
  real_T lever_posECEF_temp[3];
  real_T cosLat;
  real_T r_n;
  real_T g;
  real_T c;
  real_T d_a;
  real_T tmp[9];
  real_T tmp_0[9];
  real_T dcm_ne_0[9];
  real_T tmp_1[3];
  int32_T i;
  int32_T i_0;
  real_T r_n_tmp;
  int32_T dcm_ne_tmp;
  real_T g_tmp;

  /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_50Hz_at_outport_1' incorporates:
   *  SubSystem: '<Root>/NAV_Simulink_DoTransferAlignment'
   */
  /* MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz' incorporates:
   *  Inport: '<Root>/GenericINSensor100Hz'
   *  Inport: '<Root>/RTAC_Data'
   *  Inport: '<Root>/Time'
   */
  /* :  TransferAlignState = NAV_Embedded_DoTransferAlignment_50Hz(RTACin, Inertial, ACtime); */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:10' if isempty(TransferAlignState_private) */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:20' if ~nominal_orientation_private.valid */
  if (((int32_T)
       NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid) ==
      0) {
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:21' norm_imu_dvelocity = ((imu.dvelocity.x*100.0)^2.0 + (imu.dvelocity.y*100.0)^2.0 + (imu.dvelocity.z*100.0)^2.0)^0.5; */
    cosLat = rtU_GenericINSensor100Hz->dvelocity.x * 100.0;
    c = rtU_GenericINSensor100Hz->dvelocity.y * 100.0;
    r_n_tmp = rtU_GenericINSensor100Hz->dvelocity.z * 100.0;
    norm_imu_dvelocity = sqrt(((cosLat * cosLat) + (c * c)) + (r_n_tmp * r_n_tmp));

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:22' if norm_imu_dvelocity ~= 0.0 */
    if (norm_imu_dvelocity != 0.0) {
      /* 'NAV_Embedded_DoTransferAlignment_50Hz:23' if number_imu_dvelocity_average_private == 1 */
      if (NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private
          == 1.0) {
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:24' imu_dvelocity_x = (imu.dvelocity.x*100.0)/-norm_imu_dvelocity; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_x = cosLat /
          (-norm_imu_dvelocity);

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:25' imu_dvelocity_y = (imu.dvelocity.y*100.0)/-norm_imu_dvelocity; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_y = c /
          (-norm_imu_dvelocity);

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:26' imu_dvelocity_z = (imu.dvelocity.z*100.0)/-norm_imu_dvelocity; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_z = r_n_tmp /
          (-norm_imu_dvelocity);

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:27' number_imu_dvelocity_average_private = number_imu_dvelocity_average_private + 1.0; */
        NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private
          ++;
      } else {
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:28' else */
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:29' imu_dvelocity_x = imu_dvelocity_x + (((imu.dvelocity.x*100.0)/-norm_imu_dvelocity) - imu_dvelocity_x)/number_imu_dvelocity_average_private; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_x +=
          (((rtU_GenericINSensor100Hz->dvelocity.x * 100.0) /
            (-norm_imu_dvelocity)) -
           NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_x) /
          NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private;

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:30' imu_dvelocity_y = imu_dvelocity_y + (((imu.dvelocity.y*100.0)/-norm_imu_dvelocity) - imu_dvelocity_y)/number_imu_dvelocity_average_private; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_y +=
          (((rtU_GenericINSensor100Hz->dvelocity.y * 100.0) /
            (-norm_imu_dvelocity)) -
           NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_y) /
          NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private;

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:31' imu_dvelocity_z = imu_dvelocity_z + (((imu.dvelocity.z*100.0)/-norm_imu_dvelocity) - imu_dvelocity_z)/number_imu_dvelocity_average_private; */
        NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_z +=
          (((rtU_GenericINSensor100Hz->dvelocity.z * 100.0) /
            (-norm_imu_dvelocity)) -
           NAV_DoTransferAlignment_50Hz_M->dwork.imu_dvelocity_z) /
          NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private;

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:32' number_imu_dvelocity_average_private = number_imu_dvelocity_average_private + 1.0; */
        NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private
          ++;
      }

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:34' if number_imu_dvelocity_average_private > 50 */
      if (NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private
          > 50.0) {
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:35' nominal_orientation_private.attitude.phi = atan2((imu_dvelocity_y), (imu_dvelocity_z)); */
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.phi
          = rt_atan2d_snf((NAV_DoTransferAlignment_50Hz_M)
                          ->dwork.imu_dvelocity_y,
                          (NAV_DoTransferAlignment_50Hz_M)
                          ->dwork.imu_dvelocity_z);

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:36' nominal_orientation_private.attitude.the = atan2((-imu_dvelocity_x),(((imu_dvelocity_y)^2.0+(imu_dvelocity_z)^2.0)^0.5)); */
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.the
          = rt_atan2d_snf(-(NAV_DoTransferAlignment_50Hz_M)
                          ->dwork.imu_dvelocity_x, sqrt
                          (((NAV_DoTransferAlignment_50Hz_M)
                            ->dwork.imu_dvelocity_y *
                            (NAV_DoTransferAlignment_50Hz_M)
                            ->dwork.imu_dvelocity_y) +
                           ((NAV_DoTransferAlignment_50Hz_M)
                            ->dwork.imu_dvelocity_z *
                            (NAV_DoTransferAlignment_50Hz_M)
                            ->dwork.imu_dvelocity_z)));

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:37' nominal_orientation_private.attitude.psi = 0.0; */
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.psi
          = 0.0;

        /* 'NAV_Embedded_DoTransferAlignment_50Hz:38' nominal_orientation_private.valid = uint8(1); */
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid =
          1U;
      } else {
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:39' else */
        /* 'NAV_Embedded_DoTransferAlignment_50Hz:40' nominal_orientation_private.valid = uint8(0); */
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid =
          0U;
      }
    } else {
      /* 'NAV_Embedded_DoTransferAlignment_50Hz:42' else */
      /* 'NAV_Embedded_DoTransferAlignment_50Hz:43' nominal_orientation_private.attitude.phi = 0.0; */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.phi
        = 0.0;

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:44' nominal_orientation_private.attitude.the = 0.0; */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.the
        = 0.0;

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:45' nominal_orientation_private.attitude.psi = 0.0; */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.psi
        = 0.0;

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:46' nominal_orientation_private.valid = uint8(0); */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid =
        0U;
    }
  }

  /* 'NAV_Embedded_DoTransferAlignment_50Hz:57' if (((bitand(RTACin.ValidityWord, 32767)) == 32767) && (nominal_orientation_private.valid)) */
  if ((((int32_T)((uint32_T)(rtU_RTAC_Data->ValidityWord & 32767U))) == 32767) &&
      (((int32_T)
        NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid)
       != 0)) {
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:58' taAircraft.phi    = RTACin.Roll; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:59' taAircraft.the    = RTACin.Pitch; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:60' taAircraft.psi    = RTACin.Heading; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:61' taAircraft.wx     = RTACin.RollRate; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:62' taAircraft.wy     = RTACin.PitchRate; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:63' taAircraft.wz     = RTACin.YawRate; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:64' taAircraft.vn     = RTACin.VelocityNorth; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:65' taAircraft.ve     = RTACin.VelocityEast; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:66' taAircraft.vd     = RTACin.VelocityDown; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:67' taAircraft.lat    = RTACin.Latitude; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:68' taAircraft.lon    = RTACin.Longitude; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:69' taAircraft.alt    = RTACin.Altitude; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:70' taAircraft.leverx = RTACin.LeverArmX; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:71' taAircraft.levery = RTACin.LeverArmY; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:72' taAircraft.leverz = RTACin.LeverArmZ; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:73' taAircraft.late1  = RTACin.Latency1; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:74' taAircraft.late2  = RTACin.Latency2; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:75' taAircraft.late3  = RTACin.Latency3; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:76' taAircraft.late4  = RTACin.Latency4; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:77' taAircraft.tas    = RTACin.TrueAirspeed; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:78' taAircraft.cas    = RTACin.CalibratedAirspeed; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:79' taAircraft.sp     = RTACin.StaticPressure; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:80' taAircraft.aoa    = RTACin.AngleOfAttack; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:81' taAircraft.aos    = RTACin.AngleOfSideslip; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:84' if ~nominal_orientation_private.initial_attitude_set */
    if (((int32_T)
         NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.initial_attitude_set)
        == 0) {
      /* 'NAV_Embedded_DoTransferAlignment_50Hz:85' nominal_orientation_private.attitude.psi = taAircraft.psi; */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.psi
        = rtU_RTAC_Data->Heading;

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:86' nominal_orientation_private.attitude = LIB_d2e((LIB_e2d(taAircraft))'*(LIB_e2d(nominal_orientation_private.attitude))); */
      LIB_e2d(rtU_RTAC_Data->Roll, rtU_RTAC_Data->Pitch, rtU_RTAC_Data->Heading,
              tmp);
      LIB_e2d((NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.phi,
              (NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.the,
              (NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.psi, dcm_ne_0);
      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          dcm_ne_tmp = i + (3 * i_0);
          tmp_0[dcm_ne_tmp] = 0.0;
          tmp_0[dcm_ne_tmp] += tmp[3 * i] * dcm_ne_0[3 * i_0];
          tmp_0[dcm_ne_tmp] += tmp[(3 * i) + 1] * dcm_ne_0[(3 * i_0) + 1];
          tmp_0[dcm_ne_tmp] += tmp[(3 * i) + 2] * dcm_ne_0[(3 * i_0) + 2];
        }
      }

      LIB_d2e(tmp_0, &(NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.phi,
              &(NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.the,
              &(NAV_DoTransferAlignment_50Hz_M)
              ->dwork.nominal_orientation_private.attitude.psi);

      /* 'NAV_Embedded_DoTransferAlignment_50Hz:87' nominal_orientation_private.initial_attitude_set = uint8(1); */
      NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.initial_attitude_set
        = 1U;
    }

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:91' TransferAlignState_private.time.attitude = ACtime - taAircraft.late1; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.attitude
      = rtU_Time - rtU_RTAC_Data->Latency1;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:92' TransferAlignState_private.time.rate     = ACtime - taAircraft.late2; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.rate =
      rtU_Time - rtU_RTAC_Data->Latency2;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:93' TransferAlignState_private.time.velocity = ACtime - taAircraft.late3; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.velocity
      = rtU_Time - rtU_RTAC_Data->Latency3;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:94' TransferAlignState_private.time.position = ACtime - taAircraft.late4; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.position
      = rtU_Time - rtU_RTAC_Data->Latency4;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:97' rate_skew = [ 0             -taAircraft.wz   taAircraft.wy; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:98'                   taAircraft.wz  0              -taAircraft.wx; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:99'                  -taAircraft.wy  taAircraft.wx   0]; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:100' rate_temp = (LIB_e2d(nominal_orientation_private.attitude))'*rate_skew*(LIB_e2d(nominal_orientation_private.attitude)); */
    LIB_e2d((NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.phi,
            (NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.the,
            (NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.psi, dcm_ne);
    tmp[0] = 0.0;
    tmp[3] = -rtU_RTAC_Data->YawRate;
    tmp[6] = rtU_RTAC_Data->PitchRate;
    tmp[1] = rtU_RTAC_Data->YawRate;
    tmp[4] = 0.0;
    tmp[7] = -rtU_RTAC_Data->RollRate;
    tmp[2] = -rtU_RTAC_Data->PitchRate;
    tmp[5] = rtU_RTAC_Data->RollRate;
    tmp[8] = 0.0;
    for (i = 0; i < 3; i++) {
      for (i_0 = 0; i_0 < 3; i_0++) {
        dcm_ne_tmp = i + (3 * i_0);
        dcm_ne_0[dcm_ne_tmp] = 0.0;
        dcm_ne_0[dcm_ne_tmp] += dcm_ne[3 * i] * tmp[3 * i_0];
        dcm_ne_0[dcm_ne_tmp] += dcm_ne[(3 * i) + 1] * tmp[(3 * i_0) + 1];
        dcm_ne_0[dcm_ne_tmp] += dcm_ne[(3 * i) + 2] * tmp[(3 * i_0) + 2];
      }

      for (i_0 = 0; i_0 < 3; i_0++) {
        dcm_ne_tmp = i + (3 * i_0);
        rate_temp[dcm_ne_tmp] = 0.0;
        rate_temp[dcm_ne_tmp] += dcm_ne[3 * i_0] * dcm_ne_0[i];
        rate_temp[dcm_ne_tmp] += dcm_ne[(3 * i_0) + 1] * dcm_ne_0[i + 3];
        rate_temp[dcm_ne_tmp] += dcm_ne[(3 * i_0) + 2] * dcm_ne_0[i + 6];
      }
    }

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:101' TransferAlignState_private.rate.x = (rate_temp(3,2)-rate_temp(2,3))/2.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.x =
      (rate_temp[5] - rate_temp[7]) / 2.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:102' TransferAlignState_private.rate.y = (rate_temp(1,3)-rate_temp(3,1))/2.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.y =
      (rate_temp[6] - rate_temp[2]) / 2.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:103' TransferAlignState_private.rate.z = (rate_temp(2,1)-rate_temp(1,2))/2.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.z =
      (rate_temp[1] - rate_temp[3]) / 2.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:106' TransferAlignState_private.attitude = LIB_d2e(LIB_e2d(taAircraft)*LIB_e2d(nominal_orientation_private.attitude)); */
    LIB_e2d(rtU_RTAC_Data->Roll, rtU_RTAC_Data->Pitch, rtU_RTAC_Data->Heading,
            tmp);
    LIB_e2d((NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.phi,
            (NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.the,
            (NAV_DoTransferAlignment_50Hz_M)
            ->dwork.nominal_orientation_private.attitude.psi, dcm_ne_0);
    for (i = 0; i < 3; i++) {
      for (i_0 = 0; i_0 < 3; i_0++) {
        dcm_ne_tmp = i_0 + (3 * i);
        tmp_0[dcm_ne_tmp] = 0.0;
        tmp_0[dcm_ne_tmp] += dcm_ne_0[3 * i] * tmp[i_0];
        tmp_0[dcm_ne_tmp] += dcm_ne_0[(3 * i) + 1] * tmp[i_0 + 3];
        tmp_0[dcm_ne_tmp] += dcm_ne_0[(3 * i) + 2] * tmp[i_0 + 6];
      }
    }

    LIB_d2e(tmp_0, &(NAV_DoTransferAlignment_50Hz_M)
            ->dwork.TransferAlignState_private.attitude.phi,
            &(NAV_DoTransferAlignment_50Hz_M)
            ->dwork.TransferAlignState_private.attitude.the,
            &(NAV_DoTransferAlignment_50Hz_M)
            ->dwork.TransferAlignState_private.attitude.psi);

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:109' TransferAlignState_private.ac_attitude.phi = taAircraft.phi; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.phi
      = rtU_RTAC_Data->Roll;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:110' TransferAlignState_private.ac_attitude.the = taAircraft.the; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.the
      = rtU_RTAC_Data->Pitch;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:111' TransferAlignState_private.ac_attitude.psi = taAircraft.psi; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.psi
      = rtU_RTAC_Data->Heading;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:114' ac_posECEF_temp = LIB_lla2ecef(taAircraft); */
    /* 'LIB_lla2ecef:28' RE           = 6378137.0; */
    /* 'LIB_lla2ecef:29' E2           = 0.006694379990141316; */
    /* 'LIB_lla2ecef:30' ONE_MIN_E2   = 0.9933056200098587; */
    /* 'LIB_lla2ecef:32' sinLat = sin(lla.lat); */
    cosLat = sin(rtU_RTAC_Data->Latitude);

    /* 'LIB_lla2ecef:33' cosLat = cos(lla.lat); */
    c = cos(rtU_RTAC_Data->Latitude);

    /* 'LIB_lla2ecef:36' r_n = RE / sqrt(1.0 - E2 * sinLat * sinLat); */
    r_n = 6.378137E+6 / sqrt(1.0 - ((0.0066943799901413156 * cosLat) * cosLat));

    /* 'LIB_lla2ecef:39' pos_e.x = (r_n + lla.alt) * cosLat * cos(lla.lon); */
    /* 'LIB_lla2ecef:40' pos_e.y = (r_n + lla.alt) * cosLat * sin(lla.lon); */
    /* 'LIB_lla2ecef:41' pos_e.z = (ONE_MIN_E2 * r_n + lla.alt) * sinLat; */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:115' dcm_ne = transpose(LIB_dcm_en(taAircraft)); */
    /* 'LIB_dcm_en:27' dcm_en = [-sin(lla.lat)*cos(lla.lon), -sin(lla.lat)*sin(lla.lon),  cos(lla.lat); ... */
    /* 'LIB_dcm_en:28'             -sin(lla.lon),               cos(lla.lon),               0; ... */
    /* 'LIB_dcm_en:29'             -cos(lla.lat)*cos(lla.lon), -cos(lla.lat)*sin(lla.lon), -sin(lla.lat)]; */
    r_n_tmp = cos(rtU_RTAC_Data->Longitude);
    dcm_ne[0] = (-cosLat) * r_n_tmp;
    norm_imu_dvelocity = sin(rtU_RTAC_Data->Longitude);
    dcm_ne[1] = (-sin(rtU_RTAC_Data->Latitude)) * norm_imu_dvelocity;
    dcm_ne[2] = c;
    dcm_ne[3] = -norm_imu_dvelocity;
    dcm_ne[4] = r_n_tmp;
    dcm_ne[5] = 0.0;
    dcm_ne[6] = (-c) * r_n_tmp;
    dcm_ne[7] = (-cos(rtU_RTAC_Data->Latitude)) * norm_imu_dvelocity;
    dcm_ne[8] = -sin(rtU_RTAC_Data->Latitude);

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:116' dcm_bn = LIB_e2d(taAircraft); */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:117' dcm_be = dcm_ne*dcm_bn; */
    LIB_e2d(rtU_RTAC_Data->Roll, rtU_RTAC_Data->Pitch, rtU_RTAC_Data->Heading,
            tmp);

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:118' lever_posECEF_temp = dcm_be*[taAircraft.leverx; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:119'                                  taAircraft.levery; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:120'                                  taAircraft.leverz]; */
    tmp_1[0] = rtU_RTAC_Data->LeverArmX;
    tmp_1[1] = rtU_RTAC_Data->LeverArmY;
    tmp_1[2] = rtU_RTAC_Data->LeverArmZ;
    for (i = 0; i < 3; i++) {
      lever_posECEF_temp[i] = 0.0;
      for (i_0 = 0; i_0 < 3; i_0++) {
        dcm_ne_tmp = i + (3 * i_0);
        rate_temp[dcm_ne_tmp] = 0.0;
        rate_temp[dcm_ne_tmp] += tmp[3 * i_0] * dcm_ne[i];
        rate_temp[dcm_ne_tmp] += tmp[(3 * i_0) + 1] * dcm_ne[i + 3];
        rate_temp[dcm_ne_tmp] += tmp[(3 * i_0) + 2] * dcm_ne[i + 6];
        lever_posECEF_temp[i] += rate_temp[dcm_ne_tmp] * tmp_1[i_0];
      }
    }

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:121' TransferAlignState_private.pos_e.x = ac_posECEF_temp.x + lever_posECEF_temp(1,1); */
    c *= r_n + rtU_RTAC_Data->Altitude;
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.x =
      (c * r_n_tmp) + lever_posECEF_temp[0];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:122' TransferAlignState_private.pos_e.y = ac_posECEF_temp.y + lever_posECEF_temp(2,1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.y =
      (c * norm_imu_dvelocity) + lever_posECEF_temp[1];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:123' TransferAlignState_private.pos_e.z = ac_posECEF_temp.z + lever_posECEF_temp(3,1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z =
      (((0.99330562000985867 * r_n) + rtU_RTAC_Data->Altitude) * cosLat) +
      lever_posECEF_temp[2];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:124' TransferAlignState_private.pos_g = LIB_ecef2lla(TransferAlignState_private.pos_e); */
    /* 'LIB_ecef2lla:29' RE = 6378137.0; */
    /* 'LIB_ecef2lla:30' f = 1/298.257223563; */
    /* 'LIB_ecef2lla:33' ucalc_limit = 1.0e-15; */
    /* 'LIB_ecef2lla:36' sre = RE*(1.0 - f); */
    /* 'LIB_ecef2lla:39' ecc1_sqr = (2.0*f) - f^2; */
    /* 'LIB_ecef2lla:42' ecc2_sqr = f*(2.0 - f)/((1 - f)^2); */
    /* 'LIB_ecef2lla:45' sdre  = RE^2 - sre^2; */
    /* 'LIB_ecef2lla:48' r2  = re.x^2 + re.y^2; */
    cosLat =
      (NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.x *
       NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.x)
      + (NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.y
         * NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.y);

    /* 'LIB_ecef2lla:49' r   = sqrt(r2); */
    norm_imu_dvelocity = sqrt(cosLat);

    /* 'LIB_ecef2lla:51' if (r < ucalc_limit) */
    if (norm_imu_dvelocity < 1.0E-15) {
      /* 'LIB_ecef2lla:52' r = ucalc_limit; */
      norm_imu_dvelocity = 1.0E-15;
    }

    /* 'LIB_ecef2lla:56' F   = 54*sre^2*re.z^2; */
    r_n_tmp =
      NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z *
      NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z;
    r_n = r_n_tmp * 2.182048199171718E+15;

    /* 'LIB_ecef2lla:57' g   = r2 + ((1 - ecc1_sqr)*re.z^2) - (ecc1_sqr*sdre); */
    g_tmp = r_n_tmp * 0.99330562000985867;
    g = (g_tmp + cosLat) - 1.8230912546094608E+9;

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
    c = ((4.481472345240445E-5 * r_n) * cosLat) / rt_powd_snf(g, 3.0);

    /* 'LIB_ecef2lla:70' s   = (1 + c + sqrt((c^2) + (2.0 * c)) )^(1.0/3.0); */
    c = rt_powd_snf(sqrt((c * c) + (2.0 * c)) + (1.0 + c), 0.33333333333333331);

    /* 'LIB_ecef2lla:73' temps = 3.0*(s + (1/s) + 1)^2 * g^2; */
    d_a = ((1.0 / c) + c) + 1.0;
    g = ((d_a * d_a) * 3.0) * (g * g);

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
    r_n /= g;

    /* 'LIB_ecef2lla:82' temp = 1 + (2.0*ecc1_sqr^2*p); */
    g = (8.96294469048089E-5 * r_n) + 1.0;

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
    cosLat = ((((1.0 / g) + 1.0) * 2.03403157953845E+13) -
              (((0.99330562000985867 * r_n) * r_n_tmp) / ((1.0 + g) * g))) -
      ((r_n * cosLat) / 2.0);

    /* 'LIB_ecef2lla:97' if roTemp < ucalc_limit */
    if (cosLat < 1.0E-15) {
      /* 'LIB_ecef2lla:98' roTemp = ucalc_limit; */
      cosLat = 1.0E-15;
    }

    /* 'LIB_ecef2lla:102' ro  = -((ecc1_sqr*p*r)/(1 + q)) + sqrt(roTemp); */
    /* 'LIB_ecef2lla:103' tmp = (r - (ecc1_sqr*ro))^2; */
    cosLat = norm_imu_dvelocity - (((-(((0.0066943799901413165 * r_n) *
      norm_imu_dvelocity) / (1.0 + g))) + sqrt(cosLat)) * 0.0066943799901413165);
    cosLat *= cosLat;

    /* 'LIB_ecef2lla:104' u   = sqrt(tmp + re.z^2); */
    /* 'LIB_ecef2lla:105' v   = sqrt(tmp + ((1 - ecc1_sqr)*re.z^2)); */
    /* 'LIB_ecef2lla:106' den_z = RE*v; */
    r_n = sqrt(g_tmp + cosLat) * 6.378137E+6;

    /* 'LIB_ecef2lla:109' if den_z < ucalc_limit */
    if (r_n < 1.0E-15) {
      /* 'LIB_ecef2lla:110' den_z = ucalc_limit; */
      r_n = 1.0E-15;
    }

    /* 'LIB_ecef2lla:114' zo  = (sre^2*re.z)/den_z; */
    /* 'LIB_ecef2lla:115' pos_lla.lat = atan2((re.z + (ecc2_sqr*zo)), r); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lat =
      rt_atan2d_snf((((4.0408299984661445E+13 * (NAV_DoTransferAlignment_50Hz_M
                       )->dwork.TransferAlignState_private.pos_e.z) / r_n) *
                     0.0067394967422764341) + (NAV_DoTransferAlignment_50Hz_M)
                    ->dwork.TransferAlignState_private.pos_e.z,
                    norm_imu_dvelocity);

    /* 'LIB_ecef2lla:116' pos_lla.lon = atan2(re.y, re.x); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lon =
      rt_atan2d_snf((NAV_DoTransferAlignment_50Hz_M)
                    ->dwork.TransferAlignState_private.pos_e.y,
                    (NAV_DoTransferAlignment_50Hz_M)
                    ->dwork.TransferAlignState_private.pos_e.x);

    /* 'LIB_ecef2lla:117' pos_lla.alt = u*(1 - (sre^2/den_z)); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.alt =
      sqrt
      ((NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z
        * NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z)
       + cosLat) * (1.0 - (4.0408299984661445E+13 / r_n));

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:127' velocity_temp = dcm_ne*[taAircraft.vn; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:128'                             taAircraft.ve; ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:129'                             taAircraft.vd]; */
    for (i = 0; i < 3; i++) {
      lever_posECEF_temp[i] = (dcm_ne[i + 6] * rtU_RTAC_Data->VelocityDown) +
        ((dcm_ne[i + 3] * rtU_RTAC_Data->VelocityEast) + (dcm_ne[i] *
          rtU_RTAC_Data->VelocityNorth));
    }

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:130' ac_velECEF_temp.x = velocity_temp(1,1); */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:131' ac_velECEF_temp.y = velocity_temp(2,1); */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:132' ac_velECEF_temp.z = velocity_temp(3,1); */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:133' velocity_temp = [ac_velECEF_temp.x; ac_velECEF_temp.y; ac_velECEF_temp.z] + ... */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:134'                     dcm_be*(cross([taAircraft.wx, taAircraft.wy, taAircraft.wz], [taAircraft.leverx, taAircraft.levery, taAircraft.leverz]))'; */
    r_n = (rtU_RTAC_Data->PitchRate * rtU_RTAC_Data->LeverArmZ) -
      (rtU_RTAC_Data->YawRate * rtU_RTAC_Data->LeverArmY);
    r_n_tmp = (rtU_RTAC_Data->YawRate * rtU_RTAC_Data->LeverArmX) -
      (rtU_RTAC_Data->RollRate * rtU_RTAC_Data->LeverArmZ);
    norm_imu_dvelocity = (rtU_RTAC_Data->RollRate * rtU_RTAC_Data->LeverArmY) -
      (rtU_RTAC_Data->PitchRate * rtU_RTAC_Data->LeverArmX);
    tmp_1[0] = lever_posECEF_temp[0];
    tmp_1[1] = lever_posECEF_temp[1];
    tmp_1[2] = lever_posECEF_temp[2];
    for (i = 0; i < 3; i++) {
      lever_posECEF_temp[i] = tmp_1[i] + ((rate_temp[i + 6] * norm_imu_dvelocity)
        + ((rate_temp[i + 3] * r_n_tmp) + (rate_temp[i] * r_n)));
    }

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:135' TransferAlignState_private.vel_e.x = velocity_temp(1,1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.x =
      lever_posECEF_temp[0];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:136' TransferAlignState_private.vel_e.y = velocity_temp(2,1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.y =
      lever_posECEF_temp[1];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:137' TransferAlignState_private.vel_e.z = velocity_temp(3,1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.z =
      lever_posECEF_temp[2];

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:140' TransferAlignState_private.valid = uint8(1); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.valid = 1U;
  } else {
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:141' else */
    /* 'NAV_Embedded_DoTransferAlignment_50Hz:142' TransferAlignState_private.time.attitude = ACtime; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.attitude
      = rtU_Time;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:143' TransferAlignState_private.time.rate     = ACtime; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.rate =
      rtU_Time;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:144' TransferAlignState_private.time.velocity = ACtime; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.velocity
      = rtU_Time;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:145' TransferAlignState_private.time.position = ACtime; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.position
      = rtU_Time;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:146' TransferAlignState_private.rate.x        = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.x =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:147' TransferAlignState_private.rate.y        = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.y =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:148' TransferAlignState_private.rate.z        = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.z =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:149' TransferAlignState_private.attitude.phi  = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.phi
      = 0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:150' TransferAlignState_private.attitude.the  = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.the
      = 0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:151' TransferAlignState_private.attitude.psi  = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.psi
      = 0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:152' TransferAlignState_private.pos_g.lat     = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lat =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:153' TransferAlignState_private.pos_g.lon     = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lon =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:154' TransferAlignState_private.pos_g.alt     = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.alt =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:155' TransferAlignState_private.pos_e.x       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.x =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:156' TransferAlignState_private.pos_e.y       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.y =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:157' TransferAlignState_private.pos_e.z       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:158' TransferAlignState_private.vel_e.x       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.x =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:159' TransferAlignState_private.vel_e.y       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.y =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:160' TransferAlignState_private.vel_e.z       = 0.0; */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.z =
      0.0;

    /* 'NAV_Embedded_DoTransferAlignment_50Hz:161' TransferAlignState_private.valid         = uint8(0); */
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.valid = 0U;
  }

  /* Outport: '<Root>/TransferAlignState' incorporates:
   *  MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz'
   */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:164' TransferAlignState = TransferAlignState_private; */
  *rtY_TransferAlignState =
    NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private;

  /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_50Hz_at_outport_1' */
}

/* Model initialize function */
void NAV_DoTransferAlignment_50Hz_initialize(RT_MODEL *const
  NAV_DoTransferAlignment_50Hz_M)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_50Hz_at_outport_1' incorporates:
   *  SubSystem: '<Root>/NAV_Simulink_DoTransferAlignment'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/NAV_Embedded_DoTransferAlignment50Hz' */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:11' TransferAlignState_private = init_TransferAlignment; */
  /* 'init_TransferAlignment:22' pTransferAlignState.time.attitude    = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.attitude
    = 0.0;

  /* 'init_TransferAlignment:23' pTransferAlignState.time.rate        = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.rate =
    0.0;

  /* 'init_TransferAlignment:24' pTransferAlignState.time.velocity    = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.velocity
    = 0.0;

  /* 'init_TransferAlignment:25' pTransferAlignState.time.position    = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.time.position
    = 0.0;

  /* 'init_TransferAlignment:26' pTransferAlignState.rate.x           = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.x = 0.0;

  /* 'init_TransferAlignment:27' pTransferAlignState.rate.y           = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.y = 0.0;

  /* 'init_TransferAlignment:28' pTransferAlignState.rate.z           = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.rate.z = 0.0;

  /* 'init_TransferAlignment:29' pTransferAlignState.attitude.phi     = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.phi =
    0.0;

  /* 'init_TransferAlignment:30' pTransferAlignState.attitude.the     = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.the =
    0.0;

  /* 'init_TransferAlignment:31' pTransferAlignState.attitude.psi     = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.attitude.psi =
    0.0;

  /* 'init_TransferAlignment:32' pTransferAlignState.ac_attitude.phi  = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.phi
    = 0.0;

  /* 'init_TransferAlignment:33' pTransferAlignState.ac_attitude.the  = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.the
    = 0.0;

  /* 'init_TransferAlignment:34' pTransferAlignState.ac_attitude.psi  = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.ac_attitude.psi
    = 0.0;

  /* 'init_TransferAlignment:35' pTransferAlignState.pos_g.lat        = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lat =
    0.0;

  /* 'init_TransferAlignment:36' pTransferAlignState.pos_g.lon        = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.lon =
    0.0;

  /* 'init_TransferAlignment:37' pTransferAlignState.pos_g.alt        = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_g.alt =
    0.0;

  /* 'init_TransferAlignment:38' pTransferAlignState.pos_e.x          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.x = 0.0;

  /* 'init_TransferAlignment:39' pTransferAlignState.pos_e.y          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.y = 0.0;

  /* 'init_TransferAlignment:40' pTransferAlignState.pos_e.z          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.pos_e.z = 0.0;

  /* 'init_TransferAlignment:41' pTransferAlignState.vel_e.x          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.x = 0.0;

  /* 'init_TransferAlignment:42' pTransferAlignState.vel_e.y          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.y = 0.0;

  /* 'init_TransferAlignment:43' pTransferAlignState.vel_e.z          = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.vel_e.z = 0.0;

  /* 'init_TransferAlignment:44' pTransferAlignState.valid            = uint8(0); */
  NAV_DoTransferAlignment_50Hz_M->dwork.TransferAlignState_private.valid = 0U;

  /* 'init_TransferAlignment:47' coder.cstructname(pTransferAlignState, 'T_TA_TransferAlignState'); */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:12' nominal_orientation_private = init_Pylon; */
  /* 'init_Pylon:26' pylon.attitude.phi = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.phi
    = 0.0;

  /* 'init_Pylon:27' pylon.attitude.the = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.the
    = 0.0;

  /* 'init_Pylon:28' pylon.attitude.psi = 0.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.attitude.psi
    = 0.0;

  /* 'init_Pylon:31' pylon.accel_limit_low = 8.829; */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.accel_limit_low
    = 8.829;

  /* 'init_Pylon:32' pylon.accel_limit_high = 10.791; */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.accel_limit_high
    = 10.791;

  /* 'init_Pylon:35' pylon.valid = uint8(0); */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.valid = 0U;

  /* 'init_Pylon:38' pylon.initial_attitude_set = uint8(0); */
  NAV_DoTransferAlignment_50Hz_M->dwork.nominal_orientation_private.initial_attitude_set
    = 0U;

  /* 'init_Pylon:41' coder.cstructname(pylon, 'T_TA_PylonState'); */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:13' number_imu_dvelocity_average_private = 1.0; */
  NAV_DoTransferAlignment_50Hz_M->dwork.number_imu_dvelocity_average_private =
    1.0;

  /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Trigger_50Hz_at_outport_1' */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:14' imu_dvelocity_x = 0.0; */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:15' imu_dvelocity_y = 0.0; */
  /* 'NAV_Embedded_DoTransferAlignment_50Hz:16' imu_dvelocity_z = 0.0; */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
