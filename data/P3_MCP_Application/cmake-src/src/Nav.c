/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV.c>
 *
 *  Description:
 *  ------------
 *  Navigation module is responsible for computing the real-time position, velocity,
 *  and orientation of the platform based on data from GNSS, IMU, and other sensors.
 *
 *  Function(s):
 *  ------------
 *  - NavInit                        : Initializes the weapon navigation.
 *  - NAV_PressureToAltitude         : Calculates the altitude from atmosphere pressure .
 *  - NavDo100Hz                     : Run the Nav 100 Hz function.
 *  - NavDo2Hz                       : Run the Nav 2 Hz function.
 *  - NavDo50Hz                      : Run the Nav 50 Hz function.
 *  - NavStart50Hz                   : Starts the he 50 Hz Nav function.
 *  - NavForceFullNav                : Force NAV mode into FULL NAV.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "WIAC82_Comms.h"
#include "UART.h"
#include "FIFO.h"
#include "Nav.h"
#include "TypeDefines.h"
#include "Timer.h"
#include "MCP_Interface.h"
#include "Telemetry.h"
#include "Weapon.h"
#include "Guidance.h"
#include "TaskManager.h"

#define FORCE_ALIGN_ID      0x44735ED2

sNav100HzInput Nav100HzInput;
sNav50HzInput Nav50HzInput;
T_NAV_State Nav100HzOutput;
T_NAV_GenericInertialNavigationSensor NavINS_Data;
T_NAV_Tel100Hz NavTel100Hz;
T_NAV_Tel2Hz NavTel2Hz;
sNAV_WeaponRearLugToIMU_LeverArm NavWeaponLeverArm;

static TIME NAV_50HzTimestamp;
static unsigned int ForceAlign;
static volatile T_NAV_KFilterErrorEstimateState NavErrorEstimate;
static volatile T_TA_TransferAlignState TransferAlignState;

void NavInit(void)
{
  ForceAlign = 0;
  NAV_50HzTimestamp = 0;

  memset((T_NAV_State *) &Nav100HzOutput, 0, sizeof(T_NAV_State));
  memset((T_NAV_KFilterErrorEstimateState *) &NavErrorEstimate, 0, sizeof(T_NAV_KFilterErrorEstimateState));
  memset((T_TA_TransferAlignState *) &TransferAlignState, 0, sizeof(T_TA_TransferAlignState));

  NAV_100HzInit();
  NAV_2HzInit();
  NAV_50HzInit();
}

float NAV_PressureToAltitude(float Pressure)
{
  int index = 0;
  int j = 8;
  int k;
  float p;
  float tgrad;
  float tbase;
  float h_geo;
  float altitude;
  float sea_level_press = 101325.0; // [Pa]
  float earth_radius = 6369.0;      // [m]
  float gmr = 34.163195;
  float htab[8] = {0.0, 11.0, 20.0, 32.0, 47.0, 51.0, 71.0, 84.852};
  float ttab[8] = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65, 186.946};
  float ptab[8] = {1.0, 2.233611e-1, 5.403295e-2, 8.5666784e-3, 1.0945601e-3, 6.6063531e-4, 3.9046834e-5, 3.68501e-6};
  float gtab[8] = {-6.5, 0.0, 1.0, 2.8, 0.0, -2.8, -2.0, 0.0};


  if (Pressure < 0.1) {
    // Invalid pressure
    return 0;
  }

  p = Pressure / sea_level_press;

  while ((index + 1) < j) {
    k = (index + j) / 2;

    if (p > ptab[k]) {
      j = k;
    }
    else {
      index = k;
    }
  };

  tgrad = gtab[index];
  tbase = ttab[index];

  if (tgrad == 0.0) {
    h_geo = htab[index] - log(p / ptab[index]) * tbase / gmr;
  }
  else {
    h_geo = htab[index] + (pow(p / ptab[index], (float) (tgrad / (-1.0 * gmr))) * tbase - tbase) / tgrad;
  }

  altitude = 1000.0 * h_geo * earth_radius / (earth_radius - h_geo);

  return altitude;
}

void NavDo100Hz(sNav100HzInput *Input)
{
  T_RTAC_TransferAlignment ta;


  NavINS_Data.SystemTime = Input->SystemTime;
  NavINS_Data.GuidCtrlFlags = GuidCtrl;

  if (Input->PressureValid) {
    NavINS_Data.baro_SystemTime = Input->BaroSystemTime;
    NavINS_Data.baro_pressure = Input->BaroPressure;
    NavINS_Data.baro_alt = NAV_PressureToAltitude(Input->BaroPressure);
  }

  if (Input->IMU_DataValid) {
    NavINS_Data.dangle.x = Input->IMU_Data.DeltaAngle[0];
    NavINS_Data.dangle.y = Input->IMU_Data.DeltaAngle[1];
    NavINS_Data.dangle.z = Input->IMU_Data.DeltaAngle[2];
    NavINS_Data.dvelocity.x = Input->IMU_Data.DeltaVelocity[0];
    NavINS_Data.dvelocity.y = Input->IMU_Data.DeltaVelocity[1];
    NavINS_Data.dvelocity.z = Input->IMU_Data.DeltaVelocity[2];
  }

  if (ForceAlign == FORCE_ALIGN_ID) {
    Input->GNSS_Result.Vel[0] = 4;
    Input->GNSS_Result.Vel[1] = 0;
    Input->GNSS_Result.Vel[2] = 1;
  }

  NAV_100HzExecute((T_NAV_GenericInertialNavigationSensor *) &NavINS_Data, &Input->GNSS_Result, (T_TA_TransferAlignState *) &TransferAlignState,
        (T_NAV_KFilterErrorEstimateState *) &NavErrorEstimate, (T_NAV_State *) &Nav100HzOutput, (T_NAV_Tel100Hz *) &NavTel100Hz);

  NavTel100Hz.T100B1.NAV_100HzTimestamp = GetSystemTime();

  Weapon.Status.Bits.NAV_Mode = (int) (Nav100HzOutput.NavigationModeOut + 0.5);
  Weapon.Status.Bits.NAV_Aligned = Nav100HzOutput.NAV_KalmanFilterState.align_valid;

  Weapon.Pitch = NavTel100Hz.T100B1.NAV_AngTheta;
  Weapon.Yaw = NavTel100Hz.T100B1.NAV_AngPsi;
  Weapon.Roll = NavTel100Hz.T100B1.NAV_AngPhi;
  Weapon.Latitude = NavTel100Hz.T100B1.NAV_LatLLA;
  Weapon.Longitude = NavTel100Hz.T100B1.NAV_LongLLA;
  Weapon.Altitude = NavTel100Hz.T100B1.NAV_AltLLA;

  TelemetryBuild2HzBlock5();

  if (GetElapsedTime(NAV_50HzTimestamp) > 100000) {
    memset(&ta, 0, sizeof(T_RTAC_TransferAlignment));
    NavStart50Hz(&ta);
  }
}

void NavDo2Hz(T_GNSS_Result *GNSS_Result)
{
  T_NAV_MagnetometerMeasurement mag;
  T_MAG_Tel mag_tel;


  mag.Valid = FALSE;

  NAV_2HzExecute(GNSS_Result, &mag, (T_TA_TransferAlignState *) &TransferAlignState, (T_NAV_State *) &Nav100HzOutput,
        (T_NAV_KFilterErrorEstimateState *) &NavErrorEstimate, (T_NAV_Tel2Hz *) &NavTel2Hz, &mag_tel);

  NavTel2Hz.NAV_2HzTimestamp = GetSystemTime();
}

void NavDo50Hz(void)
{
  T_TA_TransferAlignState state;


  NAV_50HzExecute(Nav50HzInput.SystemTime, (T_NAV_GenericInertialNavigationSensor *) &NavINS_Data,
          (T_RTAC_TransferAlignment *) &Nav50HzInput.TransferAlignment, &state);

  TransferAlignState = state;
}

void NavStart50Hz(T_RTAC_TransferAlignment *TransferAlignment)
{
  NAV_50HzTimestamp = Weapon.SystemTime;

  Nav50HzInput.SystemTime = (double) Weapon.SystemTime * 1E-6;
  Nav50HzInput.TransferAlignment = *TransferAlignment;

  Nav50HzInput.TransferAlignment.LeverArmX += NavWeaponLeverArm.X;
  Nav50HzInput.TransferAlignment.LeverArmY += NavWeaponLeverArm.Y;
  Nav50HzInput.TransferAlignment.LeverArmZ += NavWeaponLeverArm.Z;

  TaskManagerAdd(task50HZ);
}

void NavForceFullNav(float Heading)
{
  ForceAlign = FORCE_ALIGN_ID;
}
