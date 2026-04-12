/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <SDLC.h>
 *
 *  Description:
 *  ------------
 *  Header file for the SDLC module.
 *
 ******************************************************************************/

#ifndef SDLC_H
#define SDLC_H


typedef enum
{
  sdlcOK,
  sdlcCRC_ERR,
  sdlcSEQ_ERR
} eSDLC_Result;


typedef struct
{
  volatile unsigned int *Base;
  void (*Callback)(unsigned char*, unsigned short, eSDLC_Result Status);
  unsigned char PageCnt;
  unsigned char PageIndex;
  unsigned short SeqNo;
  unsigned int RxCnt;
  unsigned int ErrorCnt;
} sSDLC;


void SDLC_Init(sSDLC *SDLC, unsigned BaseAddr, void (*Callback)(unsigned char*, unsigned short, eSDLC_Result));

#endif
