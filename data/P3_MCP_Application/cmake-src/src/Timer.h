/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Timer.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Timer module.
 *
 ******************************************************************************/

#ifndef TIMER_H
#define TIMER_H


typedef unsigned long long TIME;             // Units is microseconds [us]

typedef struct
{
	TIME InitTime;
} sTIM_ServiceControl;

typedef struct
{
	sTIM_ServiceControl ServiceControls;
} sTIM_StateSpace;
#ifndef QT_TELEMETRY_APP
extern sTIM_StateSpace TIM_SS;

void TIM_Init(char *AuxStr,TIME InitTime);
unsigned int GetTimeUs(void);
unsigned int GetTimeMs(void);
TIME GetSystemTime(void);
TIME GetElapsedTime(TIME StartTime);
TIME SetTimeout(unsigned int TimeoutValue);
unsigned char Timeout(TIME Time);
void Delay(unsigned int DelayTime);
void StartTimer(void (*TimerEventHandler)(unsigned int), unsigned int Interval);
void StopTimer(void);
#endif
#endif /*QT_TELEMETRY_APP*/
