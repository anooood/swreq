/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV.h>
 *
 *  Description:
 *  ------------
 *  Header file for the NAV module.
 *
 ******************************************************************************/

#ifndef NAV_H_
#define NAV_H_


#include "Nav100Hz_Output.h"
#include "Nav2Hz_Output.h"
#include "GNSS.h"
#include "IMU.h"
#include "Nav_100Hz.h"
#include "Nav_2Hz.h"
#include "Nav_50Hz.h"


typedef struct
{
  float X;
  float Y;
  float Z;
} sNAV_WeaponRearLugToIMU_LeverArm;

typedef struct
{
  double SystemTime;
  double BaroSystemTime;
  float BaroPressure;
  unsigned char IMU_DataValid;
  unsigned char PressureValid;
  unsigned char Spare[2];
  T_GNSS_Result GNSS_Result;
  sIMU_InertialData IMU_Data;
} sNav100HzInput;

typedef struct
{
  double SystemTime;
  T_RTAC_TransferAlignment TransferAlignment;
} sNav50HzInput;


extern sNav100HzInput Nav100HzInput;
extern sNav50HzInput Nav50HzInput;
extern T_NAV_State Nav100HzOutput;
extern T_NAV_GenericInertialNavigationSensor NavINS_Data;
extern T_NAV_Tel100Hz NavTel100Hz;
extern T_NAV_Tel2Hz NavTel2Hz;
extern sNAV_WeaponRearLugToIMU_LeverArm NavWeaponLeverArm;

void NavInit(void);
void NavDo100Hz(sNav100HzInput *Input);
void NavDo2Hz(T_GNSS_Result *GNSS_Result);
void NavDo50Hz(void);
void NavStart50Hz(T_RTAC_TransferAlignment *TransferAlignment);
void NavForceFullNav(float Heading);
float NAV_PressureToAltitude(float Pressure);

#endif /* NAV_H_ */
