/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <I2C.c>
 *
 *  Description:
 *  ------------
 *  I2C module.
 *
 *  Function(s):
 *  ------------
 *  - I2C_Init:                      Initializes the I2C
 *  - I2C_Write:                     Adds a I2C device that will be cyclical polled
 *  - I2C_GetRxData:                 Reads the I2C RAM buffer
 *  - I2C_CheckNACK_Status:          Checks for and clear NACK's
 *  ...
 *
 *
 ******************************************************************************/

#include "I2C.h"
#include "TypeDefines.h"
#include "Timer.h"

#define I2C_CONTROL_BYTE_OFFSET     0x800

#define I2C_WRITE                   0x00
#define I2C_READ                    0x01


typedef struct
{
  unsigned char ReadWrite           : 1;
  unsigned char DeviceId            : 7;
} sI2C_Command;

typedef struct
{
  unsigned char Start               : 1;
  unsigned char ClearInterrupt      : 1;
  unsigned char ClearNACK           : 1;
  unsigned char Reset               : 1;
  unsigned char Reserved            : 4;
} sI2C_WriteFlags;

typedef struct
{
  unsigned char Busy                : 1;
  unsigned char InterruptActive     : 1;
  unsigned char NACK_Received       : 1;
  unsigned char Reserved            : 5;
} sI2C_ReadFlags;

typedef union
{
  sI2C_WriteFlags Write;
  sI2C_ReadFlags Read;
} uI2C_Flags;

typedef struct
{
  unsigned short ClockRate;
  sI2C_Command Command;
  unsigned char SubAddrCnt;
  unsigned short RamAddr;
  unsigned short BytesToTransfer;
  unsigned short BytesTransfered;
  uI2C_Flags Flags;
} sI2C_Control;

void I2C_Init(volatile sI2C *I2C, unsigned int BaseAddress)
{
  volatile sI2C_Control *i2c_ctrl;


  I2C->Base = (volatile unsigned char *) BaseAddress;

  i2c_ctrl = (volatile sI2C_Control *) &I2C->Base[I2C_CONTROL_BYTE_OFFSET];

  i2c_ctrl->ClockRate = 249;   // = 100 MHz / (4 * I2C clock rate) - 1 = 100000000 / (4 * 100000) - 1
  i2c_ctrl->SubAddrCnt = 0;
  i2c_ctrl->RamAddr = 0;
  i2c_ctrl->BytesToTransfer = 0;
  i2c_ctrl->Flags.Write.ClearInterrupt = TRUE;
  i2c_ctrl->Flags.Write.ClearNACK = TRUE;
}

unsigned char I2C_Write(volatile sI2C *I2C, unsigned char DeviceId, unsigned char *SubAddress, unsigned short SubAddrLength,
    unsigned char *TxData, unsigned short Length, unsigned char WaitToComplete)
{
  unsigned short index;
  unsigned short cnt;
  unsigned short timeout;
  volatile sI2C_Control *i2c_ctrl;


  i2c_ctrl = (volatile sI2C_Control *) &I2C->Base[I2C_CONTROL_BYTE_OFFSET];

  if (!i2c_ctrl->Flags.Read.Busy) {
    i2c_ctrl->Command.ReadWrite = I2C_WRITE;
    i2c_ctrl->Command.DeviceId = DeviceId;
    i2c_ctrl->SubAddrCnt = SubAddrLength;
    i2c_ctrl->RamAddr = 0;
    i2c_ctrl->BytesToTransfer = SubAddrLength + Length;

    for (index = 0; index < SubAddrLength; index++) {
      I2C->Base[index] = SubAddress[index];
    }

    for (index = 0; index < Length; index++) {
      I2C->Base[SubAddrLength + index] = TxData[index];
    }

    i2c_ctrl->Flags.Write.Start = TRUE;

    if (WaitToComplete) {
      cnt = 0;
      timeout = Length + 50;

      // Wait for the transaction to complete
      while (i2c_ctrl->Flags.Read.Busy && (cnt < timeout)) {
        cnt++;
        Delay(100);
      }

      if (cnt >= timeout) {
        i2c_ctrl->Flags.Write.Reset = TRUE;
        return FALSE;
      }

      if (i2c_ctrl->Flags.Read.NACK_Received) {
        i2c_ctrl->Flags.Write.ClearNACK = TRUE;
        return FALSE;
      }
    }

    return TRUE;
  }

  return FALSE;
}

unsigned char I2C_Read(volatile sI2C *I2C, unsigned char DeviceId, unsigned char SubAddress, unsigned char SendSubAddress, unsigned char *RxData,
    unsigned short Length, unsigned char WaitForData)
{
  unsigned short index;
  unsigned short cnt;
  unsigned short timeout;
  volatile sI2C_Control *i2c_ctrl;


  i2c_ctrl = (volatile sI2C_Control *) &I2C->Base[I2C_CONTROL_BYTE_OFFSET];

  if (!i2c_ctrl->Flags.Read.Busy) {
    i2c_ctrl->Command.ReadWrite = I2C_READ;
    i2c_ctrl->Command.DeviceId = DeviceId;
    i2c_ctrl->SubAddrCnt = SendSubAddress;
    i2c_ctrl->RamAddr = 0;
    i2c_ctrl->BytesToTransfer = Length + SendSubAddress;

    if (SendSubAddress) {
      I2C->Base[0] = SubAddress;
    }

    i2c_ctrl->Flags.Write.Start = TRUE;

    if (WaitForData && (RxData != NULL)) {
      cnt = 0;
      timeout = Length + 50;

      // Wait for the transaction to complete.
      while (i2c_ctrl->Flags.Read.Busy && (cnt < timeout)) {
        cnt++;
        Delay(100);
      }

      if (cnt >= timeout) {
        i2c_ctrl->Flags.Write.Reset = TRUE;
        return FALSE;
      }

      if (i2c_ctrl->Flags.Read.NACK_Received) {
        i2c_ctrl->Flags.Write.ClearNACK = TRUE;
        return FALSE;
      }

      for (index = 0; index < Length; index++) {
        RxData[index] = I2C->Base[SendSubAddress + index];
      }
    }
  }
  else {
    return FALSE;
  }

  return TRUE;
}

unsigned char I2C_GetRxData(volatile sI2C *I2C, unsigned char *RxData, unsigned short Length, unsigned short Offset)
{
  unsigned short index;
  volatile sI2C_Control *i2c_ctrl;


  i2c_ctrl = (volatile sI2C_Control *) &I2C->Base[I2C_CONTROL_BYTE_OFFSET];

  if (!i2c_ctrl->Flags.Read.Busy) {
    if (i2c_ctrl->Flags.Read.NACK_Received) {
      i2c_ctrl->Flags.Write.ClearNACK = TRUE;
      return FALSE;
    }

    for (index = 0; index < Length; index++) {
      RxData[index] = I2C->Base[Offset + index];
    }
  }
  else {
    return FALSE;
  }

  return TRUE;
}

unsigned char I2C_CheckNACK_Status(volatile sI2C *I2C)
{
  volatile sI2C_Control *i2c_ctrl;


  i2c_ctrl = (volatile sI2C_Control *) &I2C->Base[I2C_CONTROL_BYTE_OFFSET];

  if (i2c_ctrl->Flags.Read.NACK_Received) {
    i2c_ctrl->Flags.Write.ClearNACK = TRUE;
    return TRUE;
  }

  return FALSE;
}
