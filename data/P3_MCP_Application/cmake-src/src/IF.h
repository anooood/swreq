/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <IF.h>
 *
 *  Description:
 *  ------------
 *  Header file for the IF module.
 *
 ******************************************************************************/

#ifndef IF_H_
#define IF_H_

#include <stdint.h>

#define IF_SYNC_1                           (uint8_t)(0xEB)
#define IF_SYNC_2                           (uint8_t)(0x90)

#define IF_MAX_MSG_PAYLOAD_SIZE             (uint8_t)(8)
#define IF_H_CHECKSUM                       (uint8_t)(3)
#define IF_RATE_100MS                       (uint32_t)(100)

typedef struct
{
  uint8_t Sync1;
  uint8_t Sync2;
  uint8_t Checksum;
  uint8_t MsgId;
  uint8_t DataSize;
  uint8_t Spare1;
  uint8_t Spare2;
  uint8_t Spare3;
} sIF_Header;

typedef struct
{
  TIME RxTimstamp;
  uint32_t TxCnt;
  uint32_t RxCnt;
  uint32_t ErrCnt;
} sIF;

extern sIF IF;

void IF_Init(char *newAuxStr);
void IF_SendMsg(uint8_t IFMsgId, const uint8_t *Data, uint8_t IFDataSize);
void IF_Service(void);

#endif /* IF_H_ */
