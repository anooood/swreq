/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <ScheduleControl.h>
 *
 *  Description:
 *  ------------
 *  Header file for controlling the scheduling of capability elements.
 *
 ******************************************************************************/

#ifndef SCHEDULE_CONTROL_H
#define SCHEDULE_CONTROL_H

typedef struct
{
	unsigned int TimeNow;
	unsigned int StartTime;
	unsigned int TimeDiff;
} sSchedule_Time_Controls;

#endif //SCHEDULE_CONTROL_H

