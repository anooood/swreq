/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MIL1553.h>
 *
 *  Description:
 *  ------------
 *  Header file for MIL1553 Critical Control and Critical Authority handling.
 *
 ******************************************************************************/

#ifndef MIL1553_H_
#define MIL1553_H_

#pragma pack(1)

/*
Critical Ctrl 1:

Bit 0 = D10
Bit 1 = D9
Bit 2 = D8
Bit 3 = D7
Bit 4 = D6
Bit 5 = D5
Bit 6 = D4
Bit 7 = D3
Bit 8 = D2
Bit 9 = D1
Bit 10 = D0

Critical Authority 1
Bit 0  = C14 = D10 + D9 + D6 + D1 + D0
Bit 1  = C13 = D9 + D8 + D5 + D0
Bit 2  = C12 = D8 + D7 + D4
Bit 3  = C11 = D7 + D6 + D3
Bit 4  = C10 = D10 + D9 + D5 + D2 + D1 + D0
Bit 5  = C9  = D10 + D8 + D6 + D4
Bit 6  = C8  = D10 + D7 + D6 + D5 + D3 + D1 + D0
Bit 7  = C7  = D10 + D5 + D4 + D2 + D1
Bit 8  = C6  = D10 + D6 + D4 + D3
Bit 9  = C5  = D9 + D5 + D3 + D2
Bit 10 = C4  = D10 + D9 + D8 + D6 + D4 + D2 + D0
Bit 11 = C3  = D9 + D8 + D7 + D5 + D3 + D1
Bit 12 = C2  = D10 + D9 + D8 + D7 + D4 + D2 + D1
Bit 13 = C1  = D10 + D8 + D7 + D3
Bit 14 = C0  = D10 + D7 + D2 + D1 + D0
Bit 15 = 0
 */


typedef union
{
  uint16_t Word;
  struct
  {
    uint16_t RemoteTerminalAddr     : 5;
    uint16_t Reserved2              : 7;
    uint16_t Arm                    : 1;
    uint16_t CommitToSeparate       : 1;
    uint16_t Reserved1              : 2;
  } Bits;
  struct
  {
    uint16_t NotUsed  : 5;
    uint16_t D0       : 1;
    uint16_t D1       : 1;
    uint16_t D2       : 1;
    uint16_t D3       : 1;
    uint16_t D4       : 1;
    uint16_t D5       : 1;
    uint16_t D6       : 1;
    uint16_t D7       : 1;
    uint16_t D8       : 1;
    uint16_t D9       : 1;
    uint16_t D10      : 1;
  } Ctrl;
} uMIL1553_CriticalCtrl1;

typedef union
{
  uint16_t Word;
  struct
  {
    uint16_t RemoteTerminalAddr     : 5;
    uint16_t Reserved               : 10;
    uint16_t EraseCmd               : 1;
  } Bits;
  struct
  {
    uint16_t NotUsed  : 5;
    uint16_t D0       : 1;
    uint16_t D1       : 1;
    uint16_t D2       : 1;
    uint16_t D3       : 1;
    uint16_t D4       : 1;
    uint16_t D5       : 1;
    uint16_t D6       : 1;
    uint16_t D7       : 1;
    uint16_t D8       : 1;
    uint16_t D9       : 1;
    uint16_t D10      : 1;
  } Ctrl;
} uMIL1553_CriticalCtrl2;

typedef union
{
  uint16_t Word;
  struct
  {
    uint16_t NotUsed  : 1;
    uint16_t C0       : 1;
    uint16_t C1       : 1;
    uint16_t C2       : 1;
    uint16_t C3       : 1;
    uint16_t C4       : 1;
    uint16_t C5       : 1;
    uint16_t C6       : 1;
    uint16_t C7       : 1;
    uint16_t C8       : 1;
    uint16_t C9       : 1;
    uint16_t C10      : 1;
    uint16_t C11      : 1;
    uint16_t C12      : 1;
    uint16_t C13      : 1;
    uint16_t C14      : 1;
  };
} uMIL1553_CriticalAuthority;



#pragma pack()


#endif /* MIL1553_H_ */
