/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Wind.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Wind module.
 *
 ******************************************************************************/

#ifndef WIND_H_
#define WIND_H_

#include <stdint.h>

#define WIND_MAX_ENTRY_CNT        (uint16_t)(8)

#define WIND_CONST_6              (uint16_t)(6)
#define WIND_CONST_2              (uint16_t)(2)

#pragma pack(1)

typedef struct
{
  uint16_t Altitude;                          /* meters */
  uint16_t Speed;                             /* meters/sec */
  uint16_t Direction;                         /* degrees */
} sWindEntry;

typedef struct
{
  uint16_t WindEntryCnt;                      /* 12 */
  sWindEntry WindEntries[WIND_MAX_ENTRY_CNT]; /* 14 */
  uint16_t CRC;                               /* 62 */
  uint16_t spare1;
  uint16_t spare2;
} sWind;                                      /* size = 52 (without MLAN header) */

#pragma pack()

uint8_t WindSet(sWind *WindData);

#endif /* WIND_H_ */
