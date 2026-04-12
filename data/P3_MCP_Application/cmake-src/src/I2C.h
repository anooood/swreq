/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <I2C.h>
 *
 *  Description:
 *  ------------
 *  Header file for the I2C module.
 *
 ******************************************************************************/

#ifndef I2C_H
#define I2C_H


typedef struct
{
  volatile unsigned char *Base;
} sI2C;


void I2C_Init(volatile sI2C *I2C, unsigned int BaseAddress);
unsigned char I2C_Write(volatile sI2C *I2C, unsigned char DeviceId, unsigned char *SubAddress, unsigned short SubAddrLength,
    unsigned char *TxData, unsigned short Length, unsigned char WaitToComplete);
unsigned char I2C_Read(volatile sI2C *I2C, unsigned char DeviceId, unsigned char SubAddress, unsigned char SendSubAddress, unsigned char *RxData,
    unsigned short Length, unsigned char WaitForData);
unsigned char I2C_GetRxData(volatile sI2C *I2C, unsigned char *RxData, unsigned short Length, unsigned short Offset);
unsigned char I2C_CheckNACK_Status(volatile sI2C *I2C);

#endif
