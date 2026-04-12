/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <SAL_Seeker.h>
 *
 *  Description:
 *  ------------
 *  Header file for the SAL_Seeker module.
 *
 ******************************************************************************/

#ifndef SAL_SEEKER_H_
#define SAL_SEEKER_H_

#include "Timer.h"


typedef struct
{
  unsigned char Data[200];
} sSAL_SeekerTel;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int P3V0A              : 1;  // 0
    unsigned int N3V0A              : 1;  // 1
    unsigned int P3V3D              : 1;  // 2
    unsigned int P2V5D              : 1;  // 3
    unsigned int P1V8D              : 1;  // 4
    unsigned int P1V2D              : 1;  // 5
    unsigned int P2V5A              : 1;  // 6
    unsigned int N2V5A              : 1;  // 7
    unsigned int DetVoltage         : 1;  // 8
    unsigned int PSUCurrent         : 1;  // 9
    unsigned int NoTimeSync         : 1;  // 10
    unsigned int DetASat            : 1;  // 11
    unsigned int DetBSat            : 1;  // 12
    unsigned int DetCSat            : 1;  // 13
    unsigned int DetDSat            : 1;  // 14
    unsigned int FIFO_Overrun       : 1;  // 15
    unsigned int I2C_Failed         : 1;  // 16
    unsigned int NoLaserSync        : 1;  // 17
    unsigned int Reserved0          : 4;  // 18 - 21
    unsigned int DetHeaterA         : 1;  // 22
    unsigned int DetHeaterB         : 1;  // 23
    unsigned int DetHeaterC         : 1;  // 24
    unsigned int DetHeaterD         : 1;  // 25
    unsigned int Reserved1          : 5;  // 26 - 30
    unsigned int CriticalFail       : 1;  // 31
  } Bits;
} uSAL_SeekerCBIT;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short SeekerMode       : 1;  // 0
    unsigned short TrackStatus      : 1;  // 1
    unsigned short Detection        : 1;  // 2
    unsigned short AnglesValid      : 1;  // 3
    unsigned short Window           : 1;  // 4
    unsigned short TempStable       : 1;  // 5
    unsigned short Reserved         : 2;  // 6 - 7
    unsigned short Gain             : 4;  // 8 - 11
    unsigned short HeaterDetA       : 1;  // 12
    unsigned short HeaterDetB       : 1;  // 13
    unsigned short HeaterDetC       : 1;  // 14
    unsigned short HeaterDetD       : 1;  // 15
  } Bits;
} uSAL_SeekerStatus;

typedef struct
{
  TIME RxTimestamp;
  double TargetYaw;
  double TargetPitch;
  double DetectionTime;
  unsigned int TxCnt;
  unsigned int RxCnt;
  unsigned int ErrCnt;
  unsigned int EstPRI;
  unsigned int RangeCmd;
  uSAL_SeekerCBIT CBIT;
  unsigned int SBIT;
  uSAL_SeekerStatus Status;
  unsigned short TargetSNR;
  signed char ProcessorTemperature;
  signed char DetectorTemperature;
  unsigned short JitterCmd;        // [ns]
  unsigned short AccuracyCmd;      // [ns]
  unsigned short SW_CRC;
  unsigned short CalDataCRC;
  unsigned short Spare1;
  unsigned int Spare2;
  sSAL_SeekerTel Telemetry[6];
} sSAL_Seeker;


extern sSAL_Seeker SAL_Seeker;

void SAL_SeekerInit(void);
void SAL_SeekerSend(void);
void SAL_SeekerSendClearCBIT(void);
void SAL_SeekerHandleRxData(unsigned char *RxData);
void SAL_SeekerSetPRI(double Frequency);
void SAL_SeekerSetAccuracy(unsigned short Accuracy);
void SAL_SeekerSetJitter(unsigned short Jitter);
void SAL_SeekerUpdateCommandMsg(float Range, float Velocity);
unsigned char SAL_GetFrequency(float *Frequency, unsigned int Code);

#endif /* SAL_SEEKER_H_ */
