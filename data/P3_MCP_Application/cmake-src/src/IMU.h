/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <IMU.h>
 *
 *  Description:
 *  ------------
 *  Header file for the IMU module.
 *
 ******************************************************************************/

#ifndef IMU_H
#define IMU_H

#include "Timer.h"

#pragma pack(1)

typedef struct
{
  short AngularRateX;
  short AngularRateY;
  short AngularRateZ;
  short AccelerationX;
  short AccelerationY;
  short AccelerationZ;
  unsigned short Status1;
  unsigned short Status2;
  int DeltaAngleX;
  int DeltaAngleY;
  int DeltaAngleZ;
  int DeltaVelocityX;
  int DeltaVelocityY;
  int DeltaVelocityZ;
} sIMU_RawData;

typedef struct
{
  double AngularRate[3];        // [rad / s]
  double Acceleration[3];       // [m / s ^ 2]
  double DeltaAngle[3];         // [rad]
  double DeltaVelocity[3];      // [m / s]
} sIMU_InertialData;

typedef struct
{
  TIME RxTimestamp;
  unsigned char Cycle;
  unsigned char DataValid;
  unsigned short ErrorCnt;
  unsigned int RxCnt;
  sIMU_RawData RawData;
  sIMU_InertialData InertialData;
} sIMU;

#pragma pack()


extern volatile sIMU IMU;

void IMU_Init(char *AuxStr);
void IMU_CheckComms(void);

#endif
