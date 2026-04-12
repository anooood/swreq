/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <PressureSensor.h>
 *
 *  Description:
 *  ------------
 *  Header file for the PressureSensor module.
 *
 ******************************************************************************/

#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "I2C.h"


#define PRESSURE_SENSOR_CALIB_CNT           6

#define PRESSURE_SENSOR_CONVERT_PRESS_CMD   0x46    // OSR = 2048
#define PRESSURE_SENSOR_CONVERT_TEMP_CMD    0x56    // OSR = 2048


typedef struct
{
  sI2C *I2C;
  unsigned char DeviceId;
  unsigned char DeviceOk;
  unsigned char Spare[2];
  unsigned short CalibrationData[PRESSURE_SENSOR_CALIB_CNT];
  float Pressure;
  float Temperature;
} sPressureSensor;


extern sPressureSensor PressureSensor1;
extern sPressureSensor PressureSensor2;

void PressureSensorInit(sPressureSensor *Sensor, sI2C *I2C, unsigned char DeviceId);
void PressureSensorUpdate(sPressureSensor *Sensor, unsigned char *RawPressureData, unsigned char *RawTemperatureData);

#endif
