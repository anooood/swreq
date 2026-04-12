/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <HOBS.c>
 *
 *  Description:
 *  ------------
 *  Interface module for control data between the HoBs and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - HOBS_Init:                   Initializes the HOBS interface
 *  - HOBS_Service:                Services the transmission to the HOBS
 *  - HOBS_SetBurstHeight:         Set detonation height required
 *  - HOBS_Activate:               Set flag to arm the HOBS
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "Timer.h"
#include "UART.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "HOBSComms.h"
#include "HOBS.h"
#include "TypeDefines.h"
#include "WiFi.h"

sHOBS_StateSpace SS_HOBS;

void HOBS_Init(char *newHobsAuxStr)
{
	SS_HOBS.StateMachine.Mode = HOBS_MODE_DIAGNOSTIC; /* 0xF replaced by named constant */
	SS_HOBS.StateMachine.NewMode = HOBS_STATE_DORMANT;
	SS_HOBS.Configuration.BurstHeight = HOBS_DEFAULT_BURST_HEIGHT;
	SS_HOBS.Status.BurstHeight = 0;
	SS_HOBS.StateMachine.NewBurstHeight = HOBS_DEFAULT_BURST_HEIGHT;
	SS_HOBS.Configuration.EstImpactAngle = HOBS_DEFAULT_IMPACT_ANGLE;
	SS_HOBS.Configuration.EstWeaponSpeed = HOBS_DEFAULT_WEAPON_SPEED;
	SS_HOBS.Configuration.WarnSignalWaveform = HOBS_DEFAULT_WARN_SIGNAL_WAVE;
	SS_HOBS.Configuration.ColocationID = HOBS_DEFAULT_COLOCATION_ID;

	if (WeaponSettings.DataFileValid == 0U) {
		AUXCTRL_SS.HOBSControls.AuxOn 			= 1U;
		AUXCTRL_SS.HOBSControls.SchedulerAuxOn	= 0U;
		AUXCTRL_SS.HOBSControls.RxAuxOn			= 0U;
		AUXCTRL_SS.HOBSControls.RxDetailAuxOn	= 0U;
		AUXCTRL_SS.HOBSControls.TxAuxOn			= 0U;
		AUXCTRL_SS.HOBSControls.TxDetailAuxOn	= 0U;
		AUXCTRL_SS.HOBSControls.ExtRxAuxOn      = 0U;
		AUXCTRL_SS.HOBSControls.ErrDetectOn		= 1U;
	}

	SS_HOBS.Counters.RxVersionRespChecksumOKCnt = 0;
	SS_HOBS.Counters.RxLogisticRespChecksumOKCnt = 0;

	SS_HOBS.HOBSWaitForRxMsg = FALSE;
	SS_HOBS.RxTimestamp	= 0;

	SS_HOBS.AuxStr[0]='\0'; /* clear string */

	if (AUXCTRL_SS.HOBSControls.AuxOn != 0U) {
		sprintf(newHobsAuxStr,"%s%u:HOBS Parameters Initialized\n",newHobsAuxStr,GetTimeUs());
	}
}

void HOBS_TService(void)
{
	uint16_t ServiceCase;

	if (SS_HOBS.HOBSWaitForRxMsg != 0U)
	{
		SS_HOBS.Counters.ErrCnt++;
		SS_HOBS.HOBSWaitForRxMsg = FALSE;

		if (AUXCTRL_SS.HOBSControls.ErrDetectOn != 0U) {
			AUXsprintf("%u:HOBS Receive Message Timeout ERROR - %d \n",GetTimeUs(),SS_HOBS.Counters.ErrCnt);
		}
	}
	else
	{
		if ((Weapon.SystemTime > HOBS_WEAPONMAXTIME_WIFI_SBIT) && (WiFi.SBIT.Bits.Busy != 1U)) { /* WiFi Not Busy doing SBIT */

			SS_HOBS.Counters.TxServiceCnt++;

			ServiceCase = (uint16_t)(SS_HOBS.Counters.TxServiceCnt % HOBS_NUM_SERVICE_CASES);

			if (ServiceCase == HOBS_SERVICE_CASE_0){

				if (SS_HOBS.StateMachine.Mode != SS_HOBS.StateMachine.NewMode) {
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBS_SetMode(SS_HOBS.StateMachine.NewMode);
				}
			}
			if (ServiceCase == HOBS_SERVICE_CASE_1){
				if (SS_HOBS.StateMachine.NewBurstHeight != SS_HOBS.Status.BurstHeight)	{
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBS_XConfigurationSet(&SS_HOBS.Configuration,SS_HOBS.StateMachine.NewBurstHeight);
				}
			}
			if (ServiceCase == HOBS_SERVICE_CASE_2){
				if (Weapon.Status.Bits.State == (unsigned int)wsLAUNCHED) {
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBS_UPlot_Track_Request();
				}
			}

			if (ServiceCase == HOBS_SERVICE_CASE_3){
				if (SS_HOBS.StateMachine.Mode != HOBS_STATE_LOADER)
				{
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBSStatus_Request();
				}
			}
			if (ServiceCase == HOBS_SERVICE_CASE_4){
				if ((SS_HOBS.StateMachine.Mode != HOBS_STATE_LOADER) && (SS_HOBS.Counters.RxVersionRespChecksumOKCnt == 0U))
				{
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBSFW_Version_Request();
				}
			}
			if (ServiceCase == HOBS_SERVICE_CASE_5){
				if ((SS_HOBS.StateMachine.Mode != HOBS_STATE_LOADER) &&
						(SS_HOBS.StateMachine.Mode != HOBS_STATE_ACTIVE) &&
						(SS_HOBS.Counters.RxLogisticRespChecksumOKCnt == 0U))
				{
					SS_HOBS.HOBSWaitForRxMsg = 1U; /* TRUE*/
					HOBSLogistic_Data_Request();
				}
			}
		}
	}
}

void HOBS_SetBurstHeight(uint8_t NewDetHeight)
{
	if (NewDetHeight < 1U)
	{
	    SS_HOBS.StateMachine.NewBurstHeight = 1U;
	}
	else if (NewDetHeight > HOBS_MAX_HEIGHT)
	{
	    SS_HOBS.StateMachine.NewBurstHeight = HOBS_MAX_HEIGHT;
	}
	else
	{
	    SS_HOBS.StateMachine.NewBurstHeight = NewDetHeight;
	}

	if (AUXCTRL_SS.HOBSControls.ExtRxAuxOn != 0U){
		AUXsprintf("%u:HOBS New Burst Height Received %d[m]:",GetTimeUs(),NewDetHeight);
		AUXsprintf(" Old=%d[m]",SS_HOBS.Status.BurstHeight);
		AUXsprintf(" New=%d[m]",NewDetHeight,NewDetHeight);
		AUXsprintf(" Set to %d[m]\n",SS_HOBS.StateMachine.NewBurstHeight);
	}
}

void HOBS_Activate(void)
{
	SS_HOBS.StateMachine.NewMode = HOBS_STATE_ACTIVE;

	if (AUXCTRL_SS.HOBSControls.ExtRxAuxOn != 0U){
		sprintf(SS_HOBS.AuxStr,"%s%u:HOBS Activate Command Received:\n",SS_HOBS.AuxStr,GetTimeUs());
		sprintf(SS_HOBS.AuxStr,"%s  Old HOBS Mode =[%d]\n",SS_HOBS.AuxStr,SS_HOBS.StateMachine.Mode);
		sprintf(SS_HOBS.AuxStr,"%s  New HOBS Mode to be set = [%d]\n",SS_HOBS.AuxStr,SS_HOBS.StateMachine.NewMode);
	}
}

