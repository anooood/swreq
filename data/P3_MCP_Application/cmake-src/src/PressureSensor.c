/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <PressureSensor.c>
 *
 *  Description:
 *  ------------
 *  PressureSensor module performs sensor initialization, data acquisition, scaling,
 *  and filtering to produce accurate pressure readings.
 *
 *  Function(s):
 *  ------------
 *  - PressureSensorCalculateCRC4    : Calculates the CRC of the pressure sensor EPROM data.
 *  - PressureSensorInit             : Initializes the I2C controller.
 *  - PressureSensorUpdate           : Acquires the latest data from the pressure sensor.
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include "PressureSensor.h"
#include "TypeDefines.h"
#include "Timer.h"

#define PRESSURE_SENSOR_RESET_CMD     0x1E
#define PRESSURE_SENSOR_READ_PROM     0xA0

sPressureSensor PressureSensor1;
sPressureSensor PressureSensor2;

unsigned char PressureSensorCalculateCRC4(unsigned char *PromData)
{
  unsigned char index;
  unsigned char bit_index;
  unsigned char orig_crc;
  unsigned short crc;


  crc = 0;

  // Save the CRC as stored as part of the EPROM data before setting it to 0
  orig_crc = PromData[15];
  // Set the stored CRC to 0
  PromData[15] = 0;

  for (index = 0; index < 16; index++) {
    crc ^= (unsigned short) PromData[index];

    for (bit_index = 0; bit_index < 8; bit_index++) {
      if (crc & (0x8000)) {
        crc = (crc << 1) ^ 0x3000;
      }
      else {
        crc = (crc << 1);
      }
    }
  }

  crc = (crc >> 12) & 0x000F;

  // Restore the original CRC to its original place
  PromData[15] = orig_crc;

  return crc;
}

void PressureSensorInit(sPressureSensor *Sensor, sI2C *I2C, unsigned char DeviceId)
{
  unsigned char rx_data[42];
  unsigned char index;
  unsigned char reset_command = PRESSURE_SENSOR_RESET_CMD;
  unsigned char crc;
  unsigned char calculated_crc;


  memset(Sensor, 0, sizeof(sPressureSensor));
  memset(rx_data, 0, sizeof(rx_data));

  Sensor->I2C = I2C;
  Sensor->DeviceId = DeviceId;
  Sensor->DeviceOk = FALSE;

  // Send RESET command
  if (!I2C_Write(Sensor->I2C, DeviceId, NULL, 0,  &reset_command, 1, TRUE)) {
    return;
  }

  Delay(5000);

  // Read calibration data
  for (index = 0; index < 8; index++) {
    if (!I2C_Read(Sensor->I2C, DeviceId, PRESSURE_SENSOR_READ_PROM + (index << 1), TRUE, &rx_data[(index << 1)], 2, TRUE)) {
      return;
    }
  }

  // Verify calibration data
  calculated_crc = PressureSensorCalculateCRC4(rx_data);
  crc = rx_data[15] & 0x0F;

  if (calculated_crc != crc) {
    return;
  }

  Sensor->CalibrationData[0] = (rx_data[2] << 8) + rx_data[3];
  Sensor->CalibrationData[1] = (rx_data[4] << 8) + rx_data[5];
  Sensor->CalibrationData[2] = (rx_data[6] << 8) + rx_data[7];
  Sensor->CalibrationData[3] = (rx_data[8] << 8) + rx_data[9];
  Sensor->CalibrationData[4] = (rx_data[10] << 8) + rx_data[11];
  Sensor->CalibrationData[5] = (rx_data[12] << 8) + rx_data[13];

  Sensor->DeviceOk = TRUE;
}

void PressureSensorUpdate(sPressureSensor *Sensor, unsigned char *RawPressureData, unsigned char *RawTemperatureData)
{
  unsigned int raw_press;
  unsigned int raw_temp;
  int delta_temp;
  int temp;
  long long press_offset;
  long long sens;
  int t2;
  long long press_offset2;
  long long sens2;


  // Refer to MS5803 data sheet on how to calculate pressure and temperature

  if (Sensor->DeviceOk) {
    raw_press = (RawPressureData[0] << 16) + (RawPressureData[1] << 8) + RawPressureData[2];
    raw_temp = (RawTemperatureData[0] << 16) + (RawTemperatureData[1] << 8) + RawTemperatureData[2];

    delta_temp = raw_temp - ((unsigned int) Sensor->CalibrationData[4] << 8);
    temp = 2000 + ((long long) delta_temp * (long long) Sensor->CalibrationData[5]) / 8388608;
    press_offset = (long long) Sensor->CalibrationData[1] * 65536 + ((long long) Sensor->CalibrationData[3] * (long long) delta_temp) / 128;
    sens = (long long) Sensor->CalibrationData[0] * 32768 + ((long long) Sensor->CalibrationData[2] * (long long) delta_temp) / 256;

    if (temp < -1500) {
      t2 = ((long long) delta_temp * (long long) delta_temp) / 2147483648u;
      press_offset2 = 3 * (((long long) temp - 2000) * ((long long) temp - 2000));
      sens2 = 7 * (((long long) temp - 2000) * ((long long) temp - 2000)) / 8;
      sens2 = sens2 + 2 * (((long long) temp + 1500) * ((long long) temp + 1500));
    }
    else if (temp < 2000) {
      t2 = ((long long) delta_temp * (long long) delta_temp) / 2147483648u;
      press_offset2 = 3 * (((long long) temp - 2000) * ((long long) temp - 2000));
      sens2 = 7 * (((long long) temp - 2000) * ((long long) temp - 2000)) / 8;
    }
    else if (temp < 4500) {
      t2 = 0;
      press_offset2 = 0;
      sens2 = 0;
    }
    else {
      t2 = 0;
      press_offset2 = 0;
      sens2 = -(((long long) temp - 4500) * ((long long) temp - 4500)) / 8;
    }

    press_offset = press_offset - press_offset2;
    sens = sens - sens2;

    Sensor->Pressure = (((long long) raw_press * (long long) sens) / 2097152 - press_offset) / 32768;
    Sensor->Temperature = (float) (temp - t2) / 100.0;
  }
}

