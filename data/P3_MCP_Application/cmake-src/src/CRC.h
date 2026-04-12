/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <CRC.h>
 *
 *  Description:
 *  ------------
 *  Header file for CRC Module.
 *
 ******************************************************************************/

#ifndef CRC_H
#define CRC_H


void InitCRC(void);
unsigned short Calculate16BitCRC(unsigned char *Data, unsigned int Length);
unsigned int Calculate32BitCRC(unsigned char *Data, unsigned int Length);
unsigned char CalculateXOR_Checksum(unsigned char *Data, unsigned int Length);
unsigned short CalculateMIL_STD_1760D_Crc(unsigned short *Words, unsigned int WordCnt);

#endif
