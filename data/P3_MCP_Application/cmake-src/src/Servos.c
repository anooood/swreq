/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Servos.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control between the Servos System and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - ServosInit             : Initialize the servo control system.
 *  - ServoHandleRxData      : Process incoming data received for servo control.
 *  - ServoOnSBIT            : Enable servo operation based on SBIT (Start BIT) signal.
 *  - ServoOnStatus          : Monitor and update the servo system status.
 *  - ServosUpdatePositions  : Update servo motor positions as required.
 *  - ServosSend             : Send commands or data to the servo motors.
 *  - ServosClearCBIT        : Clear Continuous Built-In Test (CBIT) fault flags.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "Servos.h"
#include "TypeDefines.h"
#include "Timer.h"
#include "Telemetry.h"
#include "UART.h"
#include "FIFO.h"
#include "WIAC82_Comms.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "MLAN.h"
#include "BIT.h"
#include "Weapon.h"
#include "WeaponSettings.h"

#define SERVO_UPDATE_RATE               300

#define SERVO_SBIT_REQ                  0x00
#define SERVO_SBIT                      0x01
#define SERVO_CLEAR_CBIT                0x04
#define SERVO_DATA                      0x80
#define SERVO_COMMAND                   0x10
#define SERVO_SET_ZERO_POS              0x2A
#define SERVO_SET_FIN_LOCK_ZERO         0x25

#define SERVO_FINLOCK_POS_SF            0.0054931640625      // 360 / 65536

#define SERVO_TEST_COMMAND0				10000 	//10 degrees
#define SERVO_TEST_COMMAND1				-10000 //-10 degrees

typedef struct
{
  unsigned short SwVersion;
  unsigned short SwCRC;
  uServoSBIT SBIT;
  unsigned char CurrentMode;
  unsigned char NextMode;
} sServoSBIT_Msg;

typedef struct
{
  short TailFinPosition[SERVO_CNT];
  signed char TailFinCurrent[SERVO_CNT];
  signed char PCB_Temperature;
  unsigned char MotorVoltage;
  unsigned char SupplyVoltage;
  unsigned char Spare;
  uServoDiscretes Discretes;
  uServoCBIT CBIT;
  unsigned short FinLockPosition;
} sServoStatusMsg;

typedef struct
{
  sMLAN_Header Header;
  union {
    sServoSBIT_Msg  SBIT;
    sServoStatusMsg StatusMsg;
  } Payload;
} sServoRxData;

typedef struct
{
  sMLAN_Header Header;
  short Command[SERVO_CNT];
} sServoCommandMsg;


sServos Servo;
sSERVO_StateSpace SERVO_SS;

static unsigned char ServoSBIT_Completed;
static int ServoCommsCheckCnt;
static unsigned int ServoFrameCnt;
static sMLAN_Header ServoSBIT_ReqMsg;
static sServoCommandMsg ServoCommandMsg;

static void ServoOnSBIT(sServoSBIT_Msg *SBIT);
static void ServoOnStatus(sServoStatusMsg *ServoStatusMsg);

void ServosInit(char *AuxStr)
{
  ServoSBIT_Completed = FALSE;
  ServoCommsCheckCnt = 0;
  ServoFrameCnt = 0;

  ServoSBIT_ReqMsg.SourceId = 0;
  ServoSBIT_ReqMsg.DestinationId = 0;
  ServoSBIT_ReqMsg.Opcode = SERVO_SBIT_REQ;
  ServoSBIT_ReqMsg.DataLength = 0;
  ServoSBIT_ReqMsg.SequenceNo = 1;
  ServoSBIT_ReqMsg.SequenceCnt = 1;
  ServoSBIT_ReqMsg.FrameCnt = 0;

  ServoCommandMsg.Header.SourceId = 0;
  ServoCommandMsg.Header.DestinationId = 0;
  ServoCommandMsg.Header.Opcode = SERVO_COMMAND;
  ServoCommandMsg.Header.DataLength = sizeof(sServoCommandMsg) - sizeof(sMLAN_Header);
  ServoCommandMsg.Header.SequenceNo = 1;
  ServoCommandMsg.Header.SequenceCnt = 1;
  ServoCommandMsg.Header.FrameCnt = 0;

  memset(&Servo, 0, sizeof(sServos));

  Servo.FinLocked = TRUE;

//  memset(&SERVO_SS, 0, sizeof(sSERVO_StateSpace));

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.ServoControls.AuxOn 				= 1;
	  AUXCTRL_SS.ServoControls.SchedulerAuxOn		= 0;
	  AUXCTRL_SS.ServoControls.RxAuxOn				= 0;
	  AUXCTRL_SS.ServoControls.RxDetailAuxOn		= 0;
	  AUXCTRL_SS.ServoControls.TxAuxOn				= 0;
	  AUXCTRL_SS.ServoControls.TxDetailAuxOn		= 0;
	  AUXCTRL_SS.ServoControls.ServoTestAuxOn 		= 0;
	  AUXCTRL_SS.ServoControls.ErrDetectOn          = 1;
  }

  SERVO_SS.ServiceControls.TestCnt 			= 0;
  SERVO_SS.ServiceControls.OldSystemTime	= 0;

  if (AUXCTRL_SS.ServoControls.AuxOn) {
	sprintf(AuxStr,"%s%u:SERVO Parameters Initialized\n",AuxStr,GetTimeUs());
  }
}

void ServoHandleRxData(void *Data)
{
  sServoRxData *rx_data = (sServoRxData *) Data;


  Servo.RxCnt++;
  Servo.RxTimestamp = GetSystemTime();

  switch (rx_data->Header.Opcode & 0x00FF) {
    case SERVO_SBIT : {
      ServoOnSBIT(&rx_data->Payload.SBIT);
    } break;
    case SERVO_DATA : {
      ServoOnStatus(&rx_data->Payload.StatusMsg);
    } break;
  }
}

static void ServoOnSBIT(sServoSBIT_Msg *SBIT)
{
  if (!SBIT->SBIT.Bits.Busy) {
    ServoSBIT_Completed = TRUE;

	if (AUXCTRL_SS.ServoControls.RxAuxOn)
		AUXsprintf("%u:SERVO on SBIT is completed \n",GetTimeUs());

    Servo.SwVersion = SBIT->SwVersion;
    Servo.SwCRC 	= SBIT->SwCRC;
    Servo.SBIT  	= SBIT->SBIT.Word;
  }
}

static void ServoOnStatus(sServoStatusMsg *ServoStatusMsg)
{
  float fin_lock_pos;

  ServoCommsCheckCnt--;

  Servo.ScaledFeedbacks[snSERVO1] = ServoStatusMsg->TailFinPosition[snSERVO1];
  Servo.ScaledFeedbacks[snSERVO2] = ServoStatusMsg->TailFinPosition[snSERVO2];
  Servo.ScaledFeedbacks[snSERVO3] = ServoStatusMsg->TailFinPosition[snSERVO3];
  Servo.ScaledFeedbacks[snSERVO4] = ServoStatusMsg->TailFinPosition[snSERVO4];
  Servo.Currents[snSERVO1]        = ServoStatusMsg->TailFinCurrent[snSERVO1];
  Servo.Currents[snSERVO2]        = ServoStatusMsg->TailFinCurrent[snSERVO2];
  Servo.Currents[snSERVO3]        = ServoStatusMsg->TailFinCurrent[snSERVO3];
  Servo.Currents[snSERVO4]        = ServoStatusMsg->TailFinCurrent[snSERVO4];
  Servo.PCB_Temperature           = ServoStatusMsg->PCB_Temperature;
  Servo.ScaledMotorVoltage        = ServoStatusMsg->MotorVoltage;
  Servo.ScaledSupplyVoltage       = ServoStatusMsg->SupplyVoltage;
  Servo.ScaledFinLockPosition     = ServoStatusMsg->FinLockPosition;
  Servo.Discretes                 = ServoStatusMsg->Discretes;
  Servo.CBIT                      = ServoStatusMsg->CBIT;

  fin_lock_pos = (float) ServoStatusMsg->FinLockPosition * SERVO_FINLOCK_POS_SF;

  if ((fin_lock_pos > 355.0) || (fin_lock_pos < 10.0)) {
    Servo.FinLocked = TRUE;
  }
  else if (fin_lock_pos > 25.0) {
    Servo.FinLocked = FALSE;
  }

  if (AUXCTRL_SS.ServoControls.RxAuxOn) {
	  AUXsprintf("%u:SERVO on Status Rx received \n",GetTimeUs());
	  if (AUXCTRL_SS.ServoControls.RxDetailAuxOn) {
		  AUXsprintf(" SERVO Feedbacks - S1:%d S2:%d S3:%d S4:%d \n",Servo.ScaledFeedbacks[snSERVO1],
				  Servo.ScaledFeedbacks[snSERVO2],Servo.ScaledFeedbacks[snSERVO3],Servo.ScaledFeedbacks[snSERVO4]);
		  AUXsprintf(" SERVO PCB Temp - %d \n",Servo.PCB_Temperature);
		  AUXsprintf(" SERVO CBIT - 0x%x \n",Servo.CBIT);
		  AUXsprintf(" Fin Lock Position = ");
		  AUXsprintf("%s\n",AUXsprintfFloat(fin_lock_pos));
	  }
  }

  // Verify servo fins start position (6s after power-on)
  if (!Servo.ServosReady) {
    if (Weapon.SystemTime > 6000000) {
      Servo.ServosReady = TRUE;

      Servo.StartPositions[snSERVO1] = (float) Servo.ScaledFeedbacks[snSERVO1] * 0.001;
      Servo.StartPositions[snSERVO2] = (float) Servo.ScaledFeedbacks[snSERVO2] * 0.001;
      Servo.StartPositions[snSERVO3] = (float) Servo.ScaledFeedbacks[snSERVO3] * 0.001;
      Servo.StartPositions[snSERVO4] = (float) Servo.ScaledFeedbacks[snSERVO4] * 0.001;

      // If servo positions are not within +-0.4deg on startup, set BIT error
      if (((Servo.StartPositions[snSERVO1] > 0.4) || (Servo.StartPositions[snSERVO1] < -0.4)) &&
          ((Servo.StartPositions[snSERVO2] > 0.4) || (Servo.StartPositions[snSERVO2] < -0.4)) &&
          ((Servo.StartPositions[snSERVO3] > 0.4) || (Servo.StartPositions[snSERVO3] < -0.4)) &&
          ((Servo.StartPositions[snSERVO4] > 0.4) || (Servo.StartPositions[snSERVO4] < -0.4))   ) {

    	  Servo.TailFinsError = TRUE;
    	  if (AUXCTRL_SS.ServoControls.ErrDetectOn){
    		  AUXsprintf("%u:SERVO Tail Fin Error - S1:%s S2:%s S3:%s S4:%s \n",GetTimeUs(),AUXsprintfFloat(Servo.StartPositions[snSERVO1]),
    				  AUXsprintfFloat(Servo.StartPositions[snSERVO2]),AUXsprintfFloat(Servo.StartPositions[snSERVO3]),
					  AUXsprintfFloat(Servo.StartPositions[snSERVO4]));
    	  }
      }
    }
  }
}

void ServosUpdatePositions(double Servo1Cmd, double Servo2Cmd, double Servo3Cmd, double Servo4Cmd)
{
  Servo.ScaledCommands[snSERVO1] = RAD2DEG(Servo1Cmd) * 1000.0;
  Servo.ScaledCommands[snSERVO2] = RAD2DEG(Servo2Cmd) * 1000.0;
  Servo.ScaledCommands[snSERVO3] = RAD2DEG(Servo3Cmd) * 1000.0;
  Servo.ScaledCommands[snSERVO4] = RAD2DEG(Servo4Cmd) * 1000.0;

  if (AUXCTRL_SS.ServoControls.RxAuxOn) {
	  AUXsprintf("%u:SERVO Update Positions - Auto Pilot S1:%d S2:%d S3:%d S4:%d\n",GetTimeUs(),Servo.ScaledCommands[snSERVO1],
			  Servo.ScaledCommands[snSERVO2],Servo.ScaledCommands[snSERVO3],Servo.ScaledCommands[snSERVO4]);
  }
}

void ServosSend(void)
{
  if (ServoSBIT_Completed) {
    if (ServoCommsCheckCnt > 1) {
      if (BIT.SBIT_Complete) {
        Servo.CommsErrorCnt++;
    	if (AUXCTRL_SS.ServoControls.ErrDetectOn)
    		AUXsprintf("%u:SERVO Communication Error %d \n",GetTimeUs(),Servo.CommsErrorCnt);
      }
      ServoCommsCheckCnt = 0;
    }

    ServoCommsCheckCnt++;

    ServoCommandMsg.Header.FrameCnt = ++ServoFrameCnt;

    if (AUXCTRL_SS.ServoControls.ServoTestAuxOn){

        if (AUXCTRL_SS.ServoControls.TxDetailAuxOn)
           	AUXsprintf("%u:SERVO is in Test Mode\n",GetTimeUs());

    	SERVO_SS.ServiceControls.SystemTime = GetSystemTime();
    	SERVO_SS.ServiceControls.TimeDiff = SERVO_SS.ServiceControls.SystemTime - SERVO_SS.ServiceControls.OldSystemTime;
		if (SERVO_SS.ServiceControls.TimeDiff > SERVO_TEST_CHECK_SERVICE) {
			SERVO_SS.ServiceControls.OldSystemTime = SERVO_SS.ServiceControls.SystemTime;
			SERVO_SS.ServiceControls.TestCnt++;
			if ((SERVO_SS.ServiceControls.TestCnt&1)==0){
				SERVO_SS.ServiceControls.Command=SERVO_TEST_COMMAND0;
			}
			else {
				SERVO_SS.ServiceControls.Command=SERVO_TEST_COMMAND1;
			}
		}

		ServoCommandMsg.Command[snSERVO1] = SERVO_SS.ServiceControls.Command;
	    ServoCommandMsg.Command[snSERVO2] = SERVO_SS.ServiceControls.Command;
	    ServoCommandMsg.Command[snSERVO3] = SERVO_SS.ServiceControls.Command;
	    ServoCommandMsg.Command[snSERVO4] = SERVO_SS.ServiceControls.Command;

    } else {

        if (AUXCTRL_SS.ServoControls.TxDetailAuxOn)
           	AUXsprintf("%u:SERVO is in Normal Mode\n",GetTimeUs());

        ServoCommandMsg.Command[snSERVO1] = Servo.ScaledCommands[snSERVO1];
        ServoCommandMsg.Command[snSERVO2] = Servo.ScaledCommands[snSERVO2];
        ServoCommandMsg.Command[snSERVO3] = Servo.ScaledCommands[snSERVO3];
        ServoCommandMsg.Command[snSERVO4] = Servo.ScaledCommands[snSERVO4];
    }

    if (AUXCTRL_SS.ServoControls.TxAuxOn) {
       	AUXsprintf("%u:SERVO command sent\n",GetTimeUs());

        if (AUXCTRL_SS.ServoControls.TxDetailAuxOn) {
        	AUXsprintf(" SERVO1  :%d\n",ServoCommandMsg.Command[snSERVO1]);
        	AUXsprintf(" SERVO2  :%d\n",ServoCommandMsg.Command[snSERVO2]);
        	AUXsprintf(" SERVO3  :%d\n",ServoCommandMsg.Command[snSERVO3]);
        	AUXsprintf(" SERVO4  :%d\n",ServoCommandMsg.Command[snSERVO4]);
        }
    }

    MLAN_Send(MLAN_SERVO_NODE_ID, (unsigned char *) &ServoCommandMsg, sizeof(sMLAN_Header) + ServoCommandMsg.Header.DataLength);
  }
  else {
    if (BIT.SBIT_Complete) {
      ServoSBIT_Completed = TRUE;
    }

    ServoSBIT_ReqMsg.FrameCnt = ++ServoFrameCnt;

    if (AUXCTRL_SS.ServoControls.TxAuxOn)
       	AUXsprintf("%u:SERVO SBIT command request message sent\n",GetTimeUs());

    MLAN_Send(MLAN_SERVO_NODE_ID, (unsigned char *) &ServoSBIT_ReqMsg, sizeof(sMLAN_Header));
  }
}

void ServosClearCBIT(void)
{
  sMLAN_Header msg;


  msg.SourceId = 0;
  msg.DestinationId = 0;
  msg.Opcode = SERVO_CLEAR_CBIT;
  msg.DataLength = 0;
  msg.SequenceNo = 1;
  msg.SequenceCnt = 1;
  msg.FrameCnt = ++ServoFrameCnt;

  if (AUXCTRL_SS.ServoControls.TxAuxOn)
     	AUXsprintf("%u:SERVO CLEAR CBIT command request message sent\n",GetTimeUs());

  MLAN_Send(MLAN_SERVO_NODE_ID, (unsigned char *) &msg, sizeof(sMLAN_Header));
}

