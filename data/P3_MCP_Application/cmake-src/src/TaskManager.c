/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <TaskManager.c>
 *
 *  Description:
 *  ------------
 *  The TaskManager module is responsible for managing and scheduling multiple
 *  tasks or processes within the system. It provides mechanisms to create,
 *  start, pause, resume, and terminate tasks, ensuring efficient use of CPU
 *  resources and timely execution.
 *
 *  Function(s):
 *  ------------
 *  - TaskManagerInit                 : Initialize task manager
 *  - TaskManagerAdd                  : Initialize a task
 *  - TaskManager600HzCommon          : Executes 600 Hz common tasks
 *  - TaskManager600HzSlot1           : Executes 600 Hz slot 1 tasks
 *  - TaskManager600HzSlot2           : Executes 600 Hz slot 2 tasks
 *  - TaskManager600HzSlot3           : Executes 600 Hz slot 3 tasks
 *  - TaskManager600HzSlot4           : Executes 600 Hz slot 4 tasks
 *  - TaskManager600HzSlot5           : Executes 600 Hz slot 5 tasks
 *  - TaskManager600HzSlot6           : Executes 600 Hz slot 6 tasks
 *  - TaskManager600Hz                : Executes 600 Hz tasks
 *  - TaskManager100Hz                : Executes 100 Hz tasks
 *  - TaskManager50Hz                 : Executes 50 Hz tasks
 *  - TaskManager2Hz                  : Executes 2 Hz tasks
 *  ...
 *
 *
 ******************************************************************************/

#include <xil_exception.h>
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "TypeDefines.h"
#include "ScheduleControl.h"
#include "FPGA.h"
#include "Interrupts.h"
#include "Timer.h"
#include "IMU.h"
#include "MCP.h"
#include "FIFO.h"
#include "UART.h"
#include "MLAN.h"
#include "Servos.h"
#include "Telemetry.h"
#include "I2C_Controller.h"
#include "WiFi.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"
#include "SAL_Seeker.h"
#include "AuxiliaryPort.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "UBLOX.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "AircraftInterface.h"
#include "AuxControls.h"
#include "Nav.h"
#include "Guidance.h"
#include "BIT.h"
#include "BatteryPack.h"
#include "HOBSComms.h"
#include "HOBS.h"
#include "IF.h"
#include "Fuze.h"
#include "TaskManager.h"

unsigned char TaskMananger100HzSlot3Cnt;
unsigned char HOBSTaskMananger100HzSlot3Cnt;
volatile sTaskMananger TaskMananger;

static void TaskManager600HzCommon(unsigned char Cycle)
{
  if (I2C_ControllerPoll()) {
    MCP_UpdateVoltages();
    MCP_UpdatePressureSensorData();
    MCP.I2C_Updated = TRUE;
  }
  else {
    MCP.I2C_Updated = FALSE;
  }

  // Once every second
  if ((Weapon.Weapon600HzCnt % 600) == 0) {
    MCP_ReadTemperature();
    TelemetryBuild2HzBlock1();
  }

  if (strlen(MCP_SS.AuxStr)>0) {
	AUXsprintf("%s",MCP_SS.AuxStr);
	MCP_SS.AuxStr[0] = '\0'; //clear buffer
  }

  WeaponUpdate();
  AUXSetMode(Weapon.Discretes.Bits.HILS_Mode);
  if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
	  if (AUXCTRL_SS.MAINControls.MPSAL_On)
	  {
		  MPSAL_RxService(Cycle);  // Polled routine to service MP SAL Received messages
		  if (strlen(SAL_SS.AuxStr)>0) {
			  AUXsprintf("%s",SAL_SS.AuxStr);
			  SAL_SS.AuxStr[0] = '\0'; //clear buffer
		  }
	  }
	  else if (AUXCTRL_SS.MAINControls.QSAL_On) {
		  QSAL_RxService(Cycle);  // Polled routine to service MP SAL Received messages
		  //  	  if (strlen(QSAL_SS.AuxControls.AuxStr)>1) {
		  //  		  AUXprintf(&HILS_ADS_RxUART,"%s",QSAL_SS.AuxControls.AuxStr);
		  //  		  sprintf(QSAL_SS.AuxControls.AuxStr,""); //clear buffer
		  //  	  }
	  }
  }
  TelemetryBuild600Hz();
  UBLOX_Service();
  MLAN_Service();
  BIT_Update();

  if (Weapon.Discretes.Bits.HILS_Mode) {
    MCP_ServiceHILS_ADS();
  }
  else {
	  ServiceAUXRx(&HILS_ADS_RxUART);
  }
}

static void TaskManager600HzSlot1(void)
{
  TaskManager600HzCommon(1);
  TaskManagerAdd(task100HZ);

  // Set telemetry sync to ensure that the sync pulse will be active (= 5) when the next IMU packet is received
  *((unsigned int *) FPGA_SYNC_TEL_VALUE_SET_ADDR) = 4;
}

static void TaskManager600HzSlot2(void)
{
  TaskManager600HzCommon(2);
  RunAutoPilot();
  ServosSend();
  TelemetrySend(0);
  WiFiSend();
  if (strlen(WiFi.AuxStr)>0) {
	  AUXsprintf("%s",WiFi.AuxStr);
	  WiFi.AuxStr[0] = '\0'; //clear buffer
  }
}

static void TaskManager600HzSlot3(void)
{
  TaskManager600HzCommon(3);

  if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
	  if (AUXCTRL_SS.MAINControls.MPSAL_On)
	  {
		  MPSAL_SeekerSend();
	  }
	  else if (AUXCTRL_SS.MAINControls.QSAL_On)
	  {
		  QSAL_SeekerSend();
	  }
  }

  TaskMananger100HzSlot3Cnt++;

  if (TaskMananger100HzSlot3Cnt >= 10) {
    TaskMananger100HzSlot3Cnt = 0;
  }

  IF_Service();

  switch (TaskMananger100HzSlot3Cnt % 10) {
    case 0 : {
    	Fuze_Service();
   } break;
    case 5 : {
      if (WeaponSettings.WeaponConfig.HOBS_Present) {
    	  HOBSTaskMananger100HzSlot3Cnt++;
    	  if (HOBSTaskMananger100HzSlot3Cnt % 2 == 0) //200ms
    	  {
			  HOBS_TService();
			  if (strlen(SS_HOBS.AuxStr)>0) {
	  			  AUXsprintf("%s",SS_HOBS.AuxStr);
				  SS_HOBS.AuxStr[0] = '\0'; //clear buffer
			  }
			  HOBSTaskMananger100HzSlot3Cnt = 0;
    	  }
     }
    } break;
    default : break;
  }
}

static void TaskManager600HzSlot4(void)
{
  TaskManager600HzCommon(4);
  RunAutoPilot();
  ServosSend();
  TelemetrySend(1);
}

static void TaskManager600HzSlot5(void)
{
  TaskManager600HzCommon(5);
  BatteryPackSendMsg();
  if (strlen(BATT_SS.AuxStr)>0) {
	  AUXsprintf("%s",BATT_SS.AuxStr);
	  BATT_SS.AuxStr[0] = '\0'; //clear buffer
  }
}

static void TaskManager600HzSlot6(void)
{
  TaskManager600HzCommon(6);
  RunAutoPilot();
  ServosSend();
  TelemetrySend(2);
}

static void TaskManager600Hz(void)
{
  TIME start_time;


  TaskMananger.Tasks[task600HZ].Busy = TRUE;
  TaskMananger.Tasks[task600HZ].Count++;

  // IMU cycle 1 takes an average of 195 us longer to execute as more data is transfered
  start_time = (IMU.Cycle == 0) ? GetSystemTime() - 195 : GetSystemTime();

  Weapon.SystemTime = start_time;
  Weapon.Weapon600HzCnt++;

  Xil_EnableNestedInterrupts();

  switch (IMU.Cycle) {
    case 0 : TaskManager600HzSlot1(); break;
    case 1 : TaskManager600HzSlot2(); break;
    case 2 : TaskManager600HzSlot3(); break;
    case 3 : TaskManager600HzSlot4(); break;
    case 4 : TaskManager600HzSlot5(); break;
    case 5 : TaskManager600HzSlot6(); break;
    default : break;
  }

  Xil_DisableNestedInterrupts();

  TaskMananger.Tasks[task600HZ].ExecutionTime = GetElapsedTime(start_time);
  TaskMananger.Tasks[task600HZ].Busy = FALSE;
}

static void TaskManager100Hz(void)
{
  TIME start_time;


  TaskMananger.Tasks[task100HZ].Busy = TRUE;
  TaskMananger.Tasks[task100HZ].Count++;

  start_time = GetSystemTime();

  Weapon.Weapon100HzTimestamp = Weapon.SystemTime;
  Weapon.Weapon100HzCnt++;

  if (Weapon.Discretes.Bits.HILS_Mode) {
    Nav100HzInput.PressureValid = FALSE;
    //Nav100HzInput.PressureValid = Pitot.StaticPressureValid;
    //Nav100HzInput.BaroPressure = Pitot.StaticPressure;
    //Nav100HzInput.BaroSystemTime = Pitot.PressureTimestamp * 1E-6;
  }
  else {
    Nav100HzInput.PressureValid = (MCP.PressureSensToUse == pressNONE) ? FALSE : TRUE;
    Nav100HzInput.BaroPressure = MCP.PressSensPressure;
    Nav100HzInput.BaroSystemTime = (double) MCP.PressSensTimestamp * 1E-6;
  }

  Nav100HzInput.SystemTime = (double) Weapon.SystemTime * 1E-6;
  Nav100HzInput.GNSS_Result = UBLOX.GNSS_Result;
  Nav100HzInput.IMU_Data = IMU.InertialData;
  Nav100HzInput.IMU_DataValid = IMU.DataValid;

  Xil_EnableNestedInterrupts();

  NavDo100Hz((sNav100HzInput *) &Nav100HzInput);
  Guidance100Hz();

  MCP_KickWatchdog();

  Xil_DisableNestedInterrupts();

  TaskMananger.Tasks[task100HZ].ExecutionTime = GetElapsedTime(start_time);
  TaskMananger.Tasks[task100HZ].Busy = FALSE;
}


static void TaskManager50Hz(void)
{
  TIME start_time;


  TaskMananger.Tasks[task50HZ].Busy = TRUE;
  TaskMananger.Tasks[task50HZ].Count++;

  start_time = GetSystemTime();

  Xil_EnableNestedInterrupts();

  NavDo50Hz();

  Xil_DisableNestedInterrupts();

  TaskMananger.Tasks[task50HZ].ExecutionTime = GetElapsedTime(start_time);
  TaskMananger.Tasks[task50HZ].Busy = FALSE;
}

static void TaskManager2Hz(void)
{
  TIME start_time;


  TaskMananger.Tasks[task2HZ].Busy = TRUE;
  TaskMananger.Tasks[task2HZ].Count++;

  start_time = GetSystemTime();

  Weapon.Weapon2HzTimestamp = Weapon.SystemTime;
  Weapon.Weapon2HzCnt++;

  if (BIT.SBIT_Complete && Weapon.CBIT.Bits.GNSS_CommsFail) {
    // Force the GNSS result to FALSE as the 2 Hz interrupt was called due to a
    // timeout and not valid GNSS data
    UBLOX.GNSS_Result.Valid = FALSE;
  }

  Xil_EnableNestedInterrupts();

  NavDo2Hz((T_GNSS_Result *) &UBLOX.GNSS_Result);

  if (Weapon.ConfigureUBLOX_Receiver) {
    Weapon.ConfigureUBLOX_ReceiverCnt++;

    if (UBLOX_ConfigureReceiver(!Weapon.GNSS_Usage.GPS_NotUsed, !Weapon.GNSS_Usage.GlonassNotUsed)) {
      // UBLOX successfully configured. Reset flag
      Weapon.ConfigureUBLOX_Receiver = FALSE;
      Weapon.ConfigureUBLOX_ReceiverCnt = 0;
    }

    if (Weapon.ConfigureUBLOX_ReceiverCnt == 5) {
      // Could not configure UBLOX receiver
      Weapon.ConfigureUBLOX_Receiver = FALSE;
      Weapon.ConfigureUBLOX_ReceiverCnt = 0;
    }
  }

  Xil_DisableNestedInterrupts();

  Weapon.Flags.Bits.GNSS_Lock = UBLOX.GNSS_Result.Valid;

  TaskMananger.Tasks[task2HZ].ExecutionTime = GetElapsedTime(start_time);
  TaskMananger.Tasks[task2HZ].Busy = FALSE;
}

void TaskManagerInit(void)
{
  TaskMananger100HzSlot3Cnt = 0;

  memset((sTaskMananger *) &TaskMananger, 0, sizeof(sTaskMananger));

  AddInterrupt(TaskManager600Hz, task600HZ, ipHIGH);
  AddInterrupt(TaskManager100Hz, task100HZ, ipMEDIUM);
  AddInterrupt(TaskManager50Hz, task50HZ, ipMEDIUM);
  AddInterrupt(TaskManager2Hz, task2HZ, ipLOW);
}

void TaskManagerAdd(eTasks Task)
{
  if (Task >= taskLAST) {
    return;
  }

  if (!TaskMananger.Tasks[Task].Busy) {
    TriggerSoftwareInterrupt(Task);
  }
  else {
    // Task busy. Do not add interrupt
    TaskMananger.Tasks[Task].OverrunCnt++;
  }
}
