/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NVRAM.c>
 *
 *  Description:
 *  ------------
 *  NVRAM module is responsible for managing the access to non-volatile memory used for storing critical system parameters,
 *  configuration data, logs, and persistent state across power cycles.
 *
 *  Function(s):
 *  ------------
 *  - NVRAM_Write                    : Write data to NVRAM.
 *  - NVRAM_Read                     : Read saved data from NVRAM.
 *  ...
 *
 *
 ******************************************************************************/

#include "NVRAM.h"
#include "Typedefines.h"
#include "FPGA.h"
#include "I2C_Controller.h"

#define NVRAM_I2C_DEVICE_ID       0x50
#define NVRAM_MAX_ADDR            0x7FFF

unsigned char NVRAM_Write(unsigned short Address, unsigned char *Data, unsigned short Length, unsigned char WaitToComplete)
{
  unsigned char sub_data[2];


  if ((Address + Length) > NVRAM_MAX_ADDR) {
    return FALSE;
  }

  sub_data[0] = (Address >> 8) & 0xFF;
  sub_data[1] = Address & 0xFF;

  if (I2C_Write(&I2C[2], NVRAM_I2C_DEVICE_ID, sub_data, 2, Data, Length, WaitToComplete)) {
    return TRUE;
  }

  return FALSE;
}

unsigned char NVRAM_Read(unsigned short Address, unsigned char *Data, unsigned short Length)
{
  if ((Address + Length) > NVRAM_MAX_ADDR) {
    return FALSE;
  }

  if (NVRAM_Write(Address, NULL, 0, TRUE)) {
    if (I2C_Read(&I2C[2], NVRAM_I2C_DEVICE_ID, NULL, 0, Data, Length, TRUE)) {
      return TRUE;
    }
  }

  return FALSE;
}
