/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <TaskManager.h>
 *
 *  Description:
 *  ------------
 *  Header file for the TaskManager module.
 *
 ******************************************************************************/


#ifndef TASKMANAGER_H
#define TASKMANAGER_H


typedef enum
{
  task600HZ,
  task100HZ,
  task50HZ,
  task2HZ,
  taskLAST
} eTasks;


typedef struct
{
  unsigned char Busy;
  unsigned char OverrunCnt;
  unsigned short Count;
  unsigned int ExecutionTime;
} sTaskInfo;

typedef struct
{
  sTaskInfo Tasks[taskLAST];
} sTaskMananger;


extern volatile sTaskMananger TaskMananger;

void TaskManagerInit(void);
void TaskManagerAdd(eTasks Task);

#endif
