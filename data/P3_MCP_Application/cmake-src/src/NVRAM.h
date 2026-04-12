/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NVRAM.h>
 *
 *  Description:
 *  ------------
 *  Header file for the NVRAM module.
 *
 ******************************************************************************/

#ifndef NVRAM_H
#define NVRAM_H


unsigned char NVRAM_Write(unsigned short Address, unsigned char *Data, unsigned short Length, unsigned char WaitToComplete);
unsigned char NVRAM_Read(unsigned short Address, unsigned char *Data, unsigned short Length);

#endif
