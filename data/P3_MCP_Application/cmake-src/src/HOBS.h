/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <HOBS.h>
 *
 *  Description:
 *  ------------
 *  Header file for the HoBs module.
 *
 ******************************************************************************/

#ifndef HOBS_H_
#define HOBS_H_

#include "HOBSComms.h"
#include "Timer.h"

#define HOBS_STATE_DORMANT						(unsigned char)(0)
#define HOBS_STATE_ACTIVE						(unsigned char)(1)
#define HOBS_STATE_FUNCTIONAL_TEST				(unsigned char)(2)
#define HOBS_STATE_LOADER						(unsigned char)(3)

#define HOBS_DEFAULT_BURST_HEIGHT 				(unsigned char)(1)
#define HOBS_DEFAULT_IMPACT_ANGLE 				(unsigned char)(45)
#define HOBS_DEFAULT_WEAPON_SPEED 				(unsigned short)(200)
#define HOBS_DEFAULT_WARN_SIGNAL_WAVE 	        (unsigned char)(1)
#define HOBS_DEFAULT_COLOCATION_ID 				(unsigned char)(1)

#define HOBS_MODE_DIAGNOSTIC                    (unsigned char)(15) /* 0xF */

#define HOBS_NUM_SERVICE_CASES                  (uint32_t)(6)
#define HOBS_SERVICE_CASE_0                     (uint16_t)(0)
#define HOBS_SERVICE_CASE_1                     (uint16_t)(1)
#define HOBS_SERVICE_CASE_2                     (uint16_t)(2)
#define HOBS_SERVICE_CASE_3                     (uint16_t)(3)
#define HOBS_SERVICE_CASE_4                     (uint16_t)(4)
#define HOBS_SERVICE_CASE_5                     (uint16_t)(5)

#define HOBS_MAX_HEIGHT                         (uint8_t)(25)

#define HOBS_WEAPONMAXTIME_WIFI_SBIT            (TIME)2000000

typedef struct
{
  uint32_t RxMessageCnt;
  uint32_t TxServiceCnt;
  uint32_t TxModeCmdCnt;
  uint32_t RxModeRespCnt;
  uint32_t TxConfigCmdCnt;
  uint32_t RxConfigRespCnt;
  uint32_t TxStatusCmdCnt;
  uint32_t RxStatusRespCnt;
  uint32_t RxNACKRespCnt;
  uint32_t TxTrackCmdCnt;
  uint32_t RxTrackRespCnt;
  uint32_t TxVersionCmdCnt;
  uint32_t RxVersionRespCnt;
  uint32_t TxLogisticCmdCnt;
  uint32_t RxLogisticRespCnt;
  uint32_t RxUnknownSoftProcMsgCnt;
  uint32_t RxUnknownSignalProcMsgCnt;
  uint32_t RxUnknownFlashLoaderProcMsgCnt;
  uint32_t RxUnknownNACKMsgCnt;
  uint32_t UnknownMsgCnt;

  uint32_t RxModeRespChecksumOKCnt;
  uint32_t RxConfigRespChecksumOKCnt;
  uint32_t RxStatusRespChecksumOKCnt;
  uint32_t RxTrackRespChecksumOKCnt;
  uint32_t RxVersionRespChecksumOKCnt;
  uint32_t RxLogisticRespChecksumOKCnt;
  uint32_t RxNACKRespChecksumOKCnt;

  uint32_t RxModeRespChecksumErrCnt;
  uint32_t RxConfigRespChecksumErrCnt;
  uint32_t RxStatusRespChecksumErrCnt;
  uint32_t RxTrackRespChecksumErrCnt;
  uint32_t RxVersionRespChecksumErrCnt;
  uint32_t RxLogisticRespChecksumErrCnt;
  uint32_t RxNACKRespChecksumErrCnt;
  uint16_t ErrCnt;

} sHOBS_Counters;

typedef struct
{
	uint8_t Mode;
	uint8_t NewMode;
	uint8_t NewBurstHeight;
	uint8_t NACKResponse;
	uint8_t ModeReceived;
	uint8_t BurstHeightReceived;
} sHOBS_SM;

typedef struct
{
	char 							TxBuffer[AUX_FIFO_SIZE];		/* Max Transmit message payload size */
	char 							RxBuffer[AUX_FIFO_SIZE];		/* Max Receiver message payload size */
}sHOBS_CommsControls;

typedef struct
{
	sHOBS_MISSION_CONFIG Configuration;
	sHOBS_SYSTEM_STATUS Status;
	sHOBS_PLOT_AND_TRACK Track;
	sHOBS_FW_VERSION Version;
	sHOBS_LOG_DATA Logistic;
	sHOBS_Counters Counters;						/* Communication Counters */
	sHOBS_SM StateMachine;							/* State machine control */
	sHOBS_CommsControls CommsControls;				/* Communication Controls */
	uint16_t FW_HOBS_Version;
	uint8_t HOBSWaitForRxMsg;
	TIME RxTimestamp;
	char AuxStr[AUX_FIFO_SIZE];
} sHOBS_StateSpace;

extern sHOBS_StateSpace SS_HOBS;

void HOBS_Init(char *newHobsAuxStr);
void HOBS_TService(void);
void HOBS_SetBurstHeight(uint8_t NewDetHeight);
void HOBS_Activate(void);

#endif /* HOBS_H_ */
