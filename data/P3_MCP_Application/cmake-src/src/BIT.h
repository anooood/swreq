/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <BIT.h>
 *
 *  Description:
 *  ------------
 *  Header file for BIT Module.
 *
 ******************************************************************************/

#ifndef BIT_H_
#define BIT_H_


#include "Timer.h"


#define BIT_CBIT_EVENT_LOG_MAX_ENTRIES    8
#define MIN_BATT_VOLT_LEVEL 			  19.0


#pragma pack(1)

typedef struct
{
  TIME Timestamp;
  unsigned char IMU_Cycle;
  unsigned char BIT_Index;
  unsigned short Spare;
} sBIT_Event;

typedef struct
{
  unsigned char SBIT_Complete;
  unsigned char Spare1;
  unsigned char MCP_CBIT_EventCnt;
  unsigned char WeaponCBIT_EventCnt;
  sBIT_Event MCP_CBIT_Events[BIT_CBIT_EVENT_LOG_MAX_ENTRIES];
  sBIT_Event WeaponCBIT_Events[BIT_CBIT_EVENT_LOG_MAX_ENTRIES];
} sBIT;

#pragma pack()


extern sBIT BIT;

void BIT_Init(void);
void BIT_Update(void);
void BIT_ClearCBIT(void);
void BIT_ClearSBIT(void);

#endif /* BIT_H_ */
