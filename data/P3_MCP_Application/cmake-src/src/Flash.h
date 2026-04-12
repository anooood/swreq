/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Flash.h>
 *
 *  Description:
 *  ------------
 *  Header file for Flash Module.
 *
 ******************************************************************************/

#ifndef FLASH2_H_
#define FLASH2_H_


#define FLASH_MAX_SECTORS     1024


unsigned int FlashInit(void);
void FlashReadData(unsigned int Address, unsigned char *Data, unsigned int ByteCnt);
void FlashWriteData(unsigned int Address, unsigned char *Data, unsigned int ByteCnt);
unsigned char FlashEraseSector(unsigned int Address);
void FlashEraseDevice(void);

#endif /* FLASH_H_ */
