/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Servos.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Servos module.
 *
 ******************************************************************************/

#ifndef SERVOS_H
#define SERVOS_H


#include "Timer.h"


#define SERVO_CNT        4
#define SERVO_UART_DATA_BUFFER_SIZE         0x800
#define SERVO_TEST_CHECK_SERVICE		((unsigned int)(2000000)) // 2 second


typedef enum
{
  snSERVO1,
  snSERVO2,
  snSERVO3,
  snSERVO4
} eServoNumber;


#pragma pack(1)

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short Busy                         : 1;
    unsigned short CalibrationFail              : 1;
    unsigned short PositionSensor1InitFail      : 1;
    unsigned short PositionSensor2InitFail      : 1;
    unsigned short PositionSensor3InitFail      : 1;
    unsigned short PositionSensor4InitFail      : 1;
    unsigned short PositionSensor5InitFail      : 1;
    unsigned short PositionSensor1ZeroPosFail   : 1;
    unsigned short PositionSensor2ZeroPosFail   : 1;
    unsigned short PositionSensor3ZeroPosFail   : 1;
    unsigned short PositionSensor4ZeroPosFail   : 1;
    unsigned short PositionSensor5ZeroPosFail   : 1;
    unsigned short Spare                        : 4;
  } Bits;
} uServoSBIT;

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char TempSensFail                  : 1;
    unsigned char PCB_TempFail                  : 1;
    unsigned char MyReconFail                   : 1;
    unsigned char HallSensorFail                : 1;
    unsigned char PositionSensorFail            : 1;
    unsigned char Spare                         : 2;
    unsigned char CriticalFail                  : 1;
  } Bits;
} uServoCBIT;

typedef struct
{
  unsigned char  MovementProfile;
  unsigned char  Frequency;
  short          Offset;
  unsigned short Amplitude;
  unsigned short Spare;
} sServoManualCtrlRawInputs;

typedef struct
{
  sServoManualCtrlRawInputs ServoInputs[SERVO_CNT];
} sServoManualCtrlRaw;

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char MotorsEnabled  : 1;
    unsigned char TailFinUnlock  : 1;
    unsigned char Spare          : 6;
  } Bits;
} uServoDiscretes;

typedef struct
{
  TIME RxTimestamp;
  unsigned int RxCnt;
  unsigned int CommsErrorCnt;
  short ScaledFeedbacks[4];                 // [deg] * 1000
  short ScaledCommands[4];                  // [deg] * 1000
  float StartPositions[4];                  // [deg]
  signed char Currents[4];                  // [A] * 10
  unsigned short ScaledFinLockPosition;     // [deg] * (65536 / 360)
  signed char PCB_Temperature;
  unsigned char ScaledMotorVoltage;         // [V] * 5
  unsigned char ScaledSupplyVoltage;        // [V] * 5
  uServoCBIT CBIT;
  unsigned char FinLocked;
  unsigned char ServosReady;
  unsigned short SwVersion;
  unsigned short SwCRC;
  unsigned short SBIT;
  unsigned char TailFinsError;
  unsigned char Spare[3];
  uServoDiscretes Discretes;
} sServos;

typedef struct
{
	unsigned int SystemTime;   			// System Time stamp
	unsigned int OldSystemTime;   		// Previous System Time stamp
	unsigned int TimeDiff;   			// Time difference
	unsigned int TestCnt;   			//
	short int Command;   			//
} sSERVO_ServiceControl;

typedef struct
{
	sSERVO_ServiceControl 		ServiceControls;			// Service Controls
} sSERVO_StateSpace;


#pragma pack()

extern sServos Servo;
extern sSERVO_StateSpace SERVO_SS;

void ServosInit(char *AuxStr);
void ServoHandleRxData(void *Data);
void ServosUpdatePositions(double Servo1Cmd, double Servo2Cmd, double Servo3Cmd, double Servo4Cmd);
void ServosStatusCheck(void);
void ServosSend(void);
void ServosClearCBIT(void);

#endif


