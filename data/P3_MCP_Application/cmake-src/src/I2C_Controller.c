/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <I2C_Controller.c>
 *
 *  Description:
 *  ------------
 *  I2C Controller module.
 *
 *  Function(s):
 *  ------------
 *  - I2C_ControllerInit:            Initializes the I2C controller
 *  - I2C_ControllerAddDeviceToPoll: Adds a I2C device that will be cyclical polled
 *  - I2C_ControllerPoll:            Control of the poll
 *  - I2C_ControllerGetRecievedData: Copy data received during polling
 *  - I2C_ControllerReadETI:         Reads the elapsed time indicator (ETI) from ETI device
 *  - I2C_ControllerResetETI:        Reset the elapsed time indicator (ETI) from ETI device
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include "I2C_Controller.h"
#include "TypeDefines.h"
#include "FPGA.h"
#include "PressureSensor.h"

// ETI device: DS1682
// PSU Monitor device: INA220

#define I2C_MAX_POLLED_DEVICES            20

#define I2C_DEVICE_ID_ETI                 0x6B
#define I2C_DEVICE_ID_PSU_5V              0x40
#define I2C_DEVICE_ID_PSU_1V              0x42
#define I2C_DEVICE_ID_PSU_1V35            0x48
#define I2C_DEVICE_ID_PSU_1V8             0x43
#define I2C_DEVICE_ID_PSU_3V3             0x4C
#define I2C_DEVICE_ID_PSU_28V             0x41
#define I2C_DEVICE_ID_PSU_GCA_6V          0x46
#define I2C_DEVICE_ID_PSU_IMU_5V          0x44
#define I2C_DEVICE_ID_PSU_EFUZE_5V        0x45
#define I2C_DEVICE_ID_PRESSURE_SENSOR_1   0x76   // Front sensor
#define I2C_DEVICE_ID_PRESSURE_SENSOR_2   0x77   // Rear sensor
#define I2C_DEVICE_ID_PRESSURE_SENS       0x60   // New pressure sensor


typedef enum
{
  i2c_stateWRITE,
  i2c_stateREAD,
  i2c_stateGET_DATA
} eI2C_DevicePollCycles;


#pragma pack(1)

typedef struct
{
  unsigned char Cnt   : 7;
  unsigned char Set   : 1;
} sI2C_ControllerPollFailure;

typedef struct
{
  volatile sI2C_DeviceInfo *Device;
  unsigned char SubAddr;
  unsigned char TxCnt;
  unsigned char RxCnt;
  sI2C_ControllerPollFailure Failure;
  unsigned char TxBuf[4];
  unsigned char RxBuf[4];
} sI2C_ControllerPollInfo;

#pragma pack()


volatile sI2C_Controller I2C_Controller;
volatile sI2C I2C[3];

static volatile unsigned char I2C_PollIndex;
static volatile unsigned char I2C_DevicesToPollCnt;
static volatile eI2C_DevicePollCycles I2C_PollCycle;
static volatile sI2C_ControllerPollInfo I2C_DevicesToPoll[I2C_MAX_POLLED_DEVICES];

void I2C_ControllerInit(void)
{
  unsigned char tx_buf[4];


  memset((void *) &I2C_Controller, 0, sizeof(sI2C_Controller));
  memset((void *) I2C_DevicesToPoll, 0, sizeof(I2C_DevicesToPoll));

  I2C_PollIndex = 0;
  I2C_DevicesToPollCnt = 0;
  I2C_PollCycle = i2c_stateWRITE;

  // Initialize I2C controllers
  I2C_Init(&I2C[0], FPGA_GPIO_I2C_1_ADDR);
  I2C_Init(&I2C[1], FPGA_GPIO_I2C_2_ADDR);
  I2C_Init(&I2C[2], FPGA_GPIO_I2C_3_ADDR);

  // Initialize I2C devices
  I2C_Controller.I2C_Devices[i2cETI].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cETI].DeviceId = I2C_DEVICE_ID_ETI;

  I2C_Controller.I2C_Devices[i2cPSU_5V].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cPSU_5V].DeviceId = I2C_DEVICE_ID_PSU_5V;

  I2C_Controller.I2C_Devices[i2cPSU_1V].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cPSU_1V].DeviceId = I2C_DEVICE_ID_PSU_1V;

  I2C_Controller.I2C_Devices[i2cPSU_1V35].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cPSU_1V35].DeviceId = I2C_DEVICE_ID_PSU_1V35;

  I2C_Controller.I2C_Devices[i2cPSU_1V8].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cPSU_1V8].DeviceId = I2C_DEVICE_ID_PSU_1V8;

  I2C_Controller.I2C_Devices[i2cPSU_3V3].I2C = &I2C[0];
  I2C_Controller.I2C_Devices[i2cPSU_3V3].DeviceId = I2C_DEVICE_ID_PSU_3V3;

  I2C_Controller.I2C_Devices[i2cPSU_28V].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPSU_28V].DeviceId = I2C_DEVICE_ID_PSU_28V;

  I2C_Controller.I2C_Devices[i2cPSU_GCA_6V].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPSU_GCA_6V].DeviceId = I2C_DEVICE_ID_PSU_GCA_6V;

  I2C_Controller.I2C_Devices[i2cPSU_IMU_5V].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPSU_IMU_5V].DeviceId = I2C_DEVICE_ID_PSU_IMU_5V;

  I2C_Controller.I2C_Devices[i2cPSU_EFUZE_5V].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPSU_EFUZE_5V].DeviceId = I2C_DEVICE_ID_PSU_EFUZE_5V;

  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_1_PRESS].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_1_PRESS].DeviceId = I2C_DEVICE_ID_PRESSURE_SENSOR_1;

  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_1_TEMP].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_1_TEMP].DeviceId = I2C_DEVICE_ID_PRESSURE_SENSOR_1;

  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_2_PRESS].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_2_PRESS].DeviceId = I2C_DEVICE_ID_PRESSURE_SENSOR_2;

  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_2_TEMP].I2C = &I2C[1];
  I2C_Controller.I2C_Devices[i2cPRESSURE_SENSOR_2_TEMP].DeviceId = I2C_DEVICE_ID_PRESSURE_SENSOR_2;

  // Add devices to poll
  tx_buf[0] = PRESSURE_SENSOR_CONVERT_PRESS_CMD;

  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_1_PRESS, 1, tx_buf, 0, 0);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_2_PRESS, 1, tx_buf, 0, 0);
  I2C_ControllerAddDeviceToPoll(i2cPSU_5V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_1V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_1V35, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_1V8, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_1_PRESS, 0, NULL, 0, 3);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_2_PRESS, 0, NULL, 0, 3);

  tx_buf[0] = PRESSURE_SENSOR_CONVERT_TEMP_CMD;

  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_1_TEMP, 1, tx_buf, 0, 0);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_2_TEMP, 1, tx_buf, 0, 0);
  I2C_ControllerAddDeviceToPoll(i2cPSU_3V3, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_28V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_GCA_6V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_IMU_5V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPSU_EFUZE_5V, 0, NULL, 2, 2);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_1_TEMP, 0, NULL, 0, 3);
  I2C_ControllerAddDeviceToPoll(i2cPRESSURE_SENSOR_2_TEMP, 0, NULL, 0, 3);

  PressureSensorInit(&PressureSensor1, (sI2C *) &I2C[1], I2C_DEVICE_ID_PRESSURE_SENSOR_1);
  PressureSensorInit(&PressureSensor2, (sI2C *) &I2C[1], I2C_DEVICE_ID_PRESSURE_SENSOR_2);
}

void I2C_ControllerAddDeviceToPoll(eI2C_Devices Device, unsigned char TxCnt, unsigned char *TxBuf, unsigned char SubAddr,
        unsigned char RxCnt)
{
  if (I2C_DevicesToPollCnt < I2C_MAX_POLLED_DEVICES) {
    I2C_DevicesToPoll[I2C_DevicesToPollCnt].Device = &I2C_Controller.I2C_Devices[Device];
    I2C_DevicesToPoll[I2C_DevicesToPollCnt].SubAddr = SubAddr;
    I2C_DevicesToPoll[I2C_DevicesToPollCnt].TxCnt = TxCnt;
    I2C_DevicesToPoll[I2C_DevicesToPollCnt].RxCnt = RxCnt;
    memcpy((void *) I2C_DevicesToPoll[I2C_DevicesToPollCnt].TxBuf, TxBuf, TxCnt);

    I2C_Controller.I2C_Devices[Device].PollIndex = I2C_DevicesToPollCnt;

    I2C_DevicesToPollCnt++;
  }
}

unsigned char I2C_ControllerPoll(void)
{
  unsigned char result = FALSE;
  volatile sI2C_ControllerPollInfo *poll_info;


  poll_info = &I2C_DevicesToPoll[I2C_PollIndex];

  if (poll_info->Failure.Cnt < 3) {
    switch (I2C_PollCycle) {
      case i2c_stateWRITE : {
        if (poll_info->TxCnt > 0) {
          result = I2C_Write(poll_info->Device->I2C, poll_info->Device->DeviceId, NULL, 0, (unsigned char *) poll_info->TxBuf, poll_info->TxCnt, FALSE);
          break;
        }
        else {
          I2C_PollCycle = i2c_stateREAD;
        }
      } // No break;
      case i2c_stateREAD : {
        if (!I2C_CheckNACK_Status(poll_info->Device->I2C)) {
          if (poll_info->RxCnt > 0) {
            result = I2C_Read(poll_info->Device->I2C, poll_info->Device->DeviceId, poll_info->SubAddr, TRUE, NULL, poll_info->RxCnt, FALSE);
          }
          else {
            I2C_PollCycle = i2c_stateGET_DATA;
            result = TRUE;
          }
        }
        else {
          result = FALSE;
        }
      } break;
      case i2c_stateGET_DATA : {
        if (poll_info->RxCnt > 0) {
          result = I2C_GetRxData(poll_info->Device->I2C, (unsigned char *) poll_info->RxBuf, poll_info->RxCnt, 1);
        }
      } break;
      default : {
        I2C_PollIndex = 0;
        I2C_PollCycle = i2c_stateWRITE;
      } return FALSE;
    }

    if (result) {
      I2C_PollCycle++;
      poll_info->Failure.Set = FALSE;
      poll_info->Failure.Cnt = 0;
    }
    else {
      // Device communication failure or not connected. Flag and move on to next device
      poll_info->Failure.Set = TRUE;
      poll_info->Failure.Cnt++;
      I2C_PollCycle = i2c_stateGET_DATA + 1;
    }
  }
  else {
    // Skip this device as it has failed more than 3 consecutive times
    I2C_PollCycle = i2c_stateGET_DATA + 1;
  }

  if (I2C_PollCycle > i2c_stateGET_DATA) {
    // Device serviced. Move on to next device
    I2C_PollIndex++;
    I2C_PollCycle = i2c_stateWRITE;

    if (I2C_PollIndex >= I2C_DevicesToPollCnt) {
      // All the devices have been serviced. Start again
      I2C_PollIndex = (eI2C_Devices) 0;
      // Indicate that all I2C devices have been serviced
      return TRUE;
    }
  }

  // Busy servicing I2C devices
  return FALSE;
}

unsigned char I2C_ControllerGetRecievedData(unsigned char *RxData, eI2C_Devices Device)
{
  volatile sI2C_ControllerPollInfo *poll_info;


  if (I2C_Controller.I2C_Devices[Device].PollIndex < I2C_DevicesToPollCnt) {
    poll_info = &I2C_DevicesToPoll[I2C_Controller.I2C_Devices[Device].PollIndex];

    if (!poll_info->Failure.Set) {
      memcpy(RxData, (unsigned char *) poll_info->RxBuf, poll_info->RxCnt);
      return poll_info->RxCnt;
    }
  }

  return 0;
}

unsigned int I2C_ControllerReadETI(void)
{
  unsigned char sub_addr;
  unsigned char rx_data[4];
  unsigned int eti;


  sub_addr = 5;

  if (I2C_Read(I2C_Controller.I2C_Devices[i2cETI].I2C, I2C_Controller.I2C_Devices[i2cETI].DeviceId, sub_addr, TRUE, rx_data, 4, TRUE)) {
    eti = rx_data[0] + (rx_data[1] << 8) + (rx_data[2] << 16) + (rx_data[3] << 24);
    eti >>= 2;

    return eti;
  }

  return 0xFFFFFFFF;
}

void I2C_ControllerResetETI(void)
{
  unsigned char tx_data1[2] = {0x00, 0x04};
  //unsigned char tx_data2[2] = {0x1D, 0x55};


  I2C_Write(I2C_Controller.I2C_Devices[i2cETI].I2C, I2C_Controller.I2C_Devices[i2cETI].DeviceId, NULL, 0, tx_data1, 2, TRUE);
  //I2C_Write(I2C_Controller.I2C_Devices[i2cETI].I2C, I2C_Controller.I2C_Devices[i2cETI].DeviceId, NULL, 0, tx_data2, 2, TRUE);
  //I2C_Write(I2C_Controller.I2C_Devices[i2cETI].I2C, I2C_Controller.I2C_Devices[i2cETI].DeviceId, NULL, 0, tx_data2, 2, TRUE);
}

