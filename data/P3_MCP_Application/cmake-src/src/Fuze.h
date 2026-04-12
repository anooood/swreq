/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Fuze.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Fuze module.
 *
 ******************************************************************************/

#ifndef FUZE_H_
#define FUZE_H_

#include "IF.h"

#define FUZE_SERVICE_INTERVAL		((uint32_t)(200000)) 	/* 200 milli-second */
#define FUZE_TIMEOUT_COUNT			((uint32_t)(20))		/* 2.7 seconds */

#define FUZE_FMU152_VALUE_3			((uint8_t)(3))
#define FUZE_FMU152_VALUE_5			((uint8_t)(5))
#define FUZE_FMU152_VALUE_10		((uint8_t)(10))
#define FUZE_FMU152_VALUE_15		((uint8_t)(15))
#define FUZE_FMU152_VALUE_20		((uint8_t)(20))
#define FUZE_FMU152_VALUE_25		((uint8_t)(25))
#define FUZE_FMU152_VALUE_30		((uint8_t)(30))
#define FUZE_FMU152_VALUE_35		((uint8_t)(35))
#define FUZE_FMU152_VALUE_40		((uint8_t)(40))
#define FUZE_FMU152_VALUE_45		((uint8_t)(45))
#define FUZE_FMU152_VALUE_50		((uint8_t)(50))
#define FUZE_FMU152_VALUE_60		((uint8_t)(60))
#define FUZE_FMU152_VALUE_75		((uint8_t)(75))
#define FUZE_FMU152_VALUE_90		((uint8_t)(90))
#define FUZE_FMU152_VALUE_135		((uint8_t)(135))
#define FUZE_FMU152_VALUE_180		((uint8_t)(180))
#define FUZE_FMU152_VALUE_210		((uint8_t)(210))
#define FUZE_FMU152_VALUE_240		((uint8_t)(240))

typedef enum
{
  fuzeMECHANICAL,
  fuzeFBM_21,
  fuzeFMU,
  fuzeLAST
} eFuzeType;

typedef enum
{
  fmNON_PROGRAMMABLE,
  fmDELAY,
  fmHEIGHT,
  fmHEIGHT_AND_DELAY,
  fmLAST
} eFuzeMode;

#define FUZE_ACK_ID                   (uint8_t)(0x08)
#define FUZE_NACK_ID                  (uint8_t)(0x09)
#define FUZE_SETTINGS_ID              (uint8_t)(0x20)
#define FUZE_STATUS_REQ_ID            (uint8_t)(0x21)
#define FUZE_TEST_CMD                 (uint8_t)(0x22)
#define FUZE_STATUS_ID                (uint8_t)(0x90)

typedef union
{
  uint16_t Word;
  struct
  {
    uint16_t InitComplete       : 1;
    uint16_t CommsOk            : 1;
    uint16_t UpdatingDelay      : 1;
    uint16_t FuzeDelayNotAsReq  : 1;
    uint16_t Fuze5V_On          : 1;
    uint16_t Fuze15V_On         : 1;
    uint16_t Fuze24V_On         : 1;
    uint16_t FuzeArmOn          : 1;
    uint16_t FuzeId             : 3;
    uint16_t Spare              : 5;
  } Bits;
} uFuzeStatus;

typedef struct
{
  uint16_t SW_Verion;
  uFuzeStatus Status;
  uint8_t Type;
  uint8_t Mode;
  uint8_t DelayInfo;            /* miliseconds */
  uint8_t SetDelayCmdCnt;
} sFuzeInfo;

typedef struct
{
  sIF_Header Header;
  sFuzeInfo FuzeInfo;
} sFuzeStatusMsg;

typedef struct
{
  uint8_t Type;
  uint8_t Mode;
  uFuzeStatus Status;
  uint8_t FuzeTimeDelay;        /* miliseconds */
  uint8_t SetDelayCmdCnt;
  uint16_t SW_Verion;
  uint8_t Updating;
  uint8_t UpdateSuccessful;
  uint8_t StatusRxCnt;
  uint8_t ACK_RxCnt;
  uint8_t NACK_RxCnt;
  uint8_t SetDelayTxCnt;
  uint8_t RequestStatusTxCnt;
  uint8_t InitComplete;
  TIME SystemTime;
  TIME OldSystemTime;
  uint32_t TimeDiff;
} sFuze;

typedef struct
{
  uint8_t FuzeDelay;
  uint8_t Spare1;
  uint8_t Spare2;
  uint8_t Spare3;
} sFuzeSettings;

typedef struct
{
	uint8_t FuzeResultDestNodeId;
	uint8_t FuzeExpectedCmdRxCnt;
	uint32_t FuzeFrameCnt;
} sFuze_CommsControls;

typedef struct
{
	TIME SystemTime;
	TIME OldSystemTime;
	TIME TimeDiff;
} sFuze_ServiceControl;

typedef struct
{
	sFuze 					Fuze;
	sFuzeSettings 			FuzeSettings;
	sFuze_CommsControls		CommsControls;
	sFuze_ServiceControl	ServiceControls;
} sFuze_StateSpace;

extern sFuze_StateSpace SS_FUZE;

void FuzeInit(char *newAuxStr);
void FuzeStatusMsgSendRequest(void);
void Fuze_HandleStatusMsg(const sFuzeStatusMsg *StatusMsg);
uint8_t FuzeFMU152_CheckDelay(uint8_t checkDelay);
extern uint8_t Fuze_SetDelay(uint8_t newFuzeDelay, uint8_t FromNodeId);
void FuzeHandleRxData(uint8_t *RxData);
void Fuze_Service(void);

#endif /* FUZE_H_ */
