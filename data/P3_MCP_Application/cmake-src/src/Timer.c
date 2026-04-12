/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Timer.c>
 *
 *  Description:
 *  ------------
 *  The Timer module handles the collection, formatting, transmission, and
 *  reception of data between the system and a remote monitoring station or
 *  control unit.
 *
 *  Function(s):
 *  ------------
 *  - TIM_Init                    : Initialize TIM
 *  - GetTimeUs                   : Gets Time in microseconds
 *  - GetTimeMs                   : Gets Time in miliseconds
 *  - GetSystemTime               : Reads the elapsed system time since power on from FPGA
 *  - GetElapsedTime              : Calculates the time difference of current time and start input time
 *  - SetTimeout                  : Calculates a time in the future when a timeout should occur
 *  - Timeout                     : Checks if timeout has occurred
 *  - Delay                       : Inserts a time delay
 *  - StartTimer                  : Starts a cyclic timer interrupt
 *  - StopTimer                   : Stops timer interrupt
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include "TypeDefines.h"
#include "Timer.h"
#include "UART.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WeaponSettings.h"
#include "FPGA.h"
#include "Interrupts.h"

sTIM_StateSpace TIM_SS;

void TIM_Init(char *AuxStr,TIME InitTime) {
	unsigned int TimeOffset;

	if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.TimeControls.AuxOn 				= 1;
		AUXCTRL_SS.TimeControls.ErrDetectOn			= 1;
	}

	TIM_SS.ServiceControls.InitTime	= InitTime;
	if (AUXCTRL_SS.TimeControls.AuxOn) {
		TimeOffset = TIM_SS.ServiceControls.InitTime;
		sprintf(AuxStr,"%s%u:Timer Parameters Initialized - Offset = %d - 0x%x\n",AuxStr,GetTimeUs(),TimeOffset,TimeOffset);
	}
}

unsigned int GetTimeUs(void)
{
	unsigned int SysTime;
	unsigned int SysTimeNow;
	unsigned int SysTimeOffset;

	SysTimeNow = GetSystemTime();
	SysTimeOffset = TIM_SS.ServiceControls.InitTime;

	if (SysTimeNow < SysTimeOffset) {
		TIM_SS.ServiceControls.InitTime = GetSystemTime();
		SysTime = SysTimeNow;
		if (AUXCTRL_SS.TimeControls.ErrDetectOn) {
			AUXsprintf("%u:ERROR Timer Roll-Over Detected %d\n",SysTimeNow,SysTimeOffset);
		}
	}
	else
		SysTime = SysTimeNow - SysTimeOffset;

	return(SysTime);
}

unsigned int GetTimeMs(void)
{

	return((GetTimeUs()/1000));
}

TIME GetSystemTime(void)
{
  volatile unsigned int lo_word;
  volatile unsigned int hi_word;
  TIME val;


  // Read the 64-bit (32-bit high word + 32-bit low word) 10 nanosecond system time
  hi_word = *((volatile unsigned int *) FPGA_TIMER_HI_WORD_ADDR);
  lo_word = *((volatile unsigned int *) FPGA_TIMER_LO_WORD_ADDR);

  // Ensure that the high word did not change (when low word rolls over) just before the low word was read
  if (hi_word != *((volatile unsigned int *) FPGA_TIMER_HI_WORD_ADDR)) {
    // Read the time again. This time the high word will not change before the low word is read as it only increments once every 42.9 seconds
    hi_word = *((volatile unsigned int *) FPGA_TIMER_HI_WORD_ADDR);
    lo_word = *((volatile unsigned int *) FPGA_TIMER_LO_WORD_ADDR);
  }

  // Convert time to microseconds
  val = (((TIME) hi_word << 32) + lo_word) / 100;

  return val;
}

TIME GetElapsedTime(TIME StartTime)
{
  return (GetSystemTime() - StartTime);
}

TIME SetTimeout(unsigned int TimeoutValue)
{
  return (GetSystemTime() + TimeoutValue);
}

unsigned char Timeout(TIME Time)
{
  if (GetSystemTime() < Time) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

void Delay(unsigned int DelayTime)
{
  TIME start_time;


  start_time = GetSystemTime();

  while ((GetSystemTime() - start_time) < DelayTime) {};
}

void StartTimer(void (*TimerEventHandler)(unsigned int), unsigned int Interval)
{
  // Set timer interval (100 nanosecond resolution)
  *((volatile unsigned int *) FPGA_TIMER_PERIOD_ADDR) = (Interval * 10) - 1;
  // Set timer start time to 0
  *((volatile unsigned int *) FPGA_TIMER_SET_ADDR) = 0;

  // Enable interrupt
  EnableHardwareInterrupt(TimerEventHandler, 0, intTIMER, TRUE);
}

void StopTimer(void)
{
  DisableHardwareInterrupt(intTIMER);
}

