/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <I2C_Controller.h>
 *
 *  Description:
 *  ------------
 *  Header file for the I2C_Controller module.
 *
 ******************************************************************************/

#ifndef I2C_CONTROLLER_H
#define I2C_CONTROLLER_H


#include "I2C.h"

typedef enum
{
  i2cETI,
  i2cPSU_5V,
  i2cPSU_1V,
  i2cPSU_1V35,
  i2cPSU_1V8,
  i2cPSU_3V3,
  i2cPSU_28V,
  i2cPSU_GCA_6V,
  i2cPSU_IMU_5V,
  i2cPSU_EFUZE_5V,
  i2cPRESSURE_SENSOR_1_PRESS,
  i2cPRESSURE_SENSOR_1_TEMP,
  i2cPRESSURE_SENSOR_2_PRESS,
  i2cPRESSURE_SENSOR_2_TEMP,
  i2cLAST_DEVICE
} eI2C_Devices;


#pragma pack(1)

typedef struct
{
  volatile sI2C *I2C;
  unsigned char DeviceId;
  unsigned char PollIndex;
  unsigned char Spare[2];
} sI2C_DeviceInfo;

typedef struct
{
  sI2C_DeviceInfo I2C_Devices[i2cLAST_DEVICE];
} sI2C_Controller;

#pragma pack()


extern volatile sI2C_Controller I2C_Controller;
extern volatile sI2C I2C[3];

void I2C_ControllerInit(void);
void I2C_ControllerAddDeviceToPoll(eI2C_Devices Device, unsigned char TxCnt, unsigned char *TxBuf, unsigned char SubAddr,
        unsigned char RxCnt);
unsigned char I2C_ControllerPoll(void);
unsigned char I2C_ControllerGetRecievedData(unsigned char *RxData, eI2C_Devices Device);
unsigned int I2C_ControllerReadETI(void);
void I2C_ControllerResetETI(void);

#endif
