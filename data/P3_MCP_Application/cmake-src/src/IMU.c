/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <IMU.c>
 *
 *  Description:
 *  ------------
 *  Interface module for control data between the IMU and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - IMU_Init:                   Initializes the IMU
 *  - IMU_InterruptHandler:       IMU interrupt handler (called from SDLC interrupt handler)
 *  - IMU_CheckComms:             This function ensures that 600Hz interrupts will be
 *                                generated if if no IMU connected to the MCP
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
#include "IMU.h"
#include "TypeDefines.h"
#include "FPGA.h"
#include "Interrupts.h"
#include "SDLC.h"
#include "TaskManager.h"
#include "WeaponSettings.h"

#ifdef LITIS_IMU

#define IMU_ADDRESS               0x0E

#define IMU_SF_ANGULAR_RATE_X     0.00057220458984375   // 600 / (2 ^ 20)  [rads / s]
#define IMU_SF_ANGULAR_RATE_Y     0.00057220458984375   // 600 / (2 ^ 20)  [rads / s]
#define IMU_SF_ANGULAR_RATE_Z     0.00057220458984375   // 600 / (2 ^ 20)  [rads / s]
#define IMU_SF_ACCELERATION       0.011162109375        // (600 / (2 ^ 14)) * 0.3048  [m / (s ^ 2)]
#define IMU_SF_ANGLE_X            (1.0 / 8589934592)    // 1 / (2 ^ 33)  [rad]
#define IMU_SF_ANGLE_Y            (1.0 / 8589934592)    // 1 / (2 ^ 33)  [rad]
#define IMU_SF_ANGLE_Z            (1.0 / 8589934592)    // 1 / (2 ^ 33)  [rad]
#define IMU_SF_VELOCITY           (0.3048 / 134217728)  // (1 / (2 ^ 27)) * 0.3048  [m / s]

#else

#define IMU_ADDRESS               0x0A

#define IMU_SF_ANGULAR_RATE_X     0.00457763671875      // 600 / (2 ^ 17)  [rads / s]
#define IMU_SF_ANGULAR_RATE_Y     0.0011444091796875    // 600 / (2 ^ 19)  [rads / s]
#define IMU_SF_ANGULAR_RATE_Z     0.0011444091796875    // 600 / (2 ^ 19)  [rads / s]
#define IMU_SF_ACCELERATION       0.02232421875         // (600 / (2 ^ 13)) * 0.3048  [m / (s ^ 2)]
#define IMU_SF_ANGLE_X            (1.0 / 1073741824)    // 1 / (2 ^ 30)  [rad]
#define IMU_SF_ANGLE_Y            (1.0 / 4294967296)    // 1 / (2 ^ 32)  [rad]
#define IMU_SF_ANGLE_Z            (1.0 / 4294967296)    // 1 / (2 ^ 32)  [rad]
#define IMU_SF_VELOCITY           (0.3048 / 67108864)   // (1 / (2 ^ 26)) * 0.3048  [m / s]

#endif

#define IMU_MESSAGE_1_ID          0x01
#define IMU_MESSAGE_2_ID          0x02


#pragma pack(1)

typedef struct
{
  unsigned char Address;            // 0
  unsigned char Id;                 // 1
} sIMU_Header;

typedef struct
{
  unsigned short Reserved1    : 8;
  unsigned short FrameCnt     : 4;
  unsigned short Reserved     : 4;
} sIMU_Status2;

typedef struct
{
  short AngularRateX;
  short AngularRateY;
  short AngularRateZ;
  short AccelerationX;
  short AccelerationY;
  short AccelerationZ;
  unsigned short Status1;
  sIMU_Status2 Status2;
} sIMU_Message1Data;

typedef struct
{
  short AngularRateX;
  short AngularRateY;
  short AngularRateZ;
  short AccelerationX;
  short AccelerationY;
  short AccelerationZ;
  unsigned short Status1;
  sIMU_Status2 Status2;
  int DeltaAngleX;
  int DeltaAngleY;
  int DeltaAngleZ;
  int DeltaVelocityX;
  int DeltaVelocityY;
  int DeltaVelocityZ;
} sIMU_Message2Data;

typedef struct
{
  sIMU_Header Header;
  sIMU_Message1Data Data;
} sIMU_Message1;

typedef struct
{
  sIMU_Header Header;
  sIMU_Message2Data Data;
} sIMU_Message2;

#pragma pack()


volatile sIMU IMU;

static volatile TIME IMU_Timestamp;
static volatile sSDLC IMU_SDLC;

static void IMU_InterruptHandler(unsigned char *RxData, unsigned short Length, eSDLC_Result Status);

void IMU_Init(char *AuxStr)
{
  //memset((void *) &IMU, 0, sizeof(sIMU));

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.IMUControls.AuxOn          = 1;
	  AUXCTRL_SS.IMUControls.AuxToCsv       = 0;
	  AUXCTRL_SS.IMUControls.RxAuxOn        = 0;
	  AUXCTRL_SS.IMUControls.RxDetailAuxOn  = 0;
	  AUXCTRL_SS.IMUControls.ErrDetectOn    = 1;
  }

  if (AUXCTRL_SS.IMUControls.AuxOn)
	  sprintf(AuxStr,"%s%u:SDLC Protocol Initialized\n", AuxStr,GetTimeUs());

  SDLC_Init((sSDLC *) &IMU_SDLC, FPGA_SDLC_IMU_ADDR, IMU_InterruptHandler);
}

void IMU_InterruptHandler(unsigned char *RxData, unsigned short Length, eSDLC_Result Status)
{
  sIMU_Header *header;
  sIMU_Message1 *msg1;
  sIMU_Message2 *msg2;


  IMU_Timestamp = GetSystemTime();

  header = (sIMU_Header *) RxData;

  IMU.DataValid = FALSE;

  if (Status != sdlcCRC_ERR) {
    if (header->Address == IMU_ADDRESS) {
      if ((header->Id == IMU_MESSAGE_1_ID) && (Length == sizeof(sIMU_Message1))) {
        if (Status == sdlcSEQ_ERR) {
          // Sequence number error
          IMU.ErrorCnt++;
          if (AUXCTRL_SS.IMUControls.ErrDetectOn)
        	  AUXsprintf("%u:IMU Sequence Number Error (Message ID = 0x%x)\n",GetTimeUs(),header->Id);
        }

        msg1 = (sIMU_Message1 *) RxData;

        memcpy((void *) &IMU.RawData, &msg1->Data, sizeof(sIMU_Message1Data));  // 8 * sizeof(short) = 16 bytes

        IMU.InertialData.AngularRate[0] = -msg1->Data.AngularRateX * IMU_SF_ANGULAR_RATE_X;
        IMU.InertialData.AngularRate[1] = msg1->Data.AngularRateZ * IMU_SF_ANGULAR_RATE_Z;
        IMU.InertialData.AngularRate[2] = msg1->Data.AngularRateY * IMU_SF_ANGULAR_RATE_Y;
        IMU.InertialData.Acceleration[0] = -msg1->Data.AccelerationX * IMU_SF_ACCELERATION;
        IMU.InertialData.Acceleration[1] = msg1->Data.AccelerationZ * IMU_SF_ACCELERATION;
        IMU.InertialData.Acceleration[2] = msg1->Data.AccelerationY * IMU_SF_ACCELERATION;

        IMU.DataValid = TRUE;

        IMU.Cycle++;

        if (IMU.Cycle >= 6) {
          IMU.Cycle = 0;
        }

        IMU.RxTimestamp = IMU_Timestamp;

        if (AUXCTRL_SS.IMUControls.RxAuxOn){
        	AUXsprintf("%u:IMU Rx (Message ID = 0x%x) Received \n",GetTimeUs(),header->Id);
        	if (AUXCTRL_SS.IMUControls.RxDetailAuxOn){
        		AUXsprintf(" AngularRate[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[0]));
        		AUXsprintf(" AngularRate[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[1]));
        		AUXsprintf(" AngularRate[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[2]));
        		AUXsprintf(" Acceleration[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[0]));
        		AUXsprintf(" Acceleration[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[1]));
        		AUXsprintf(" Acceleration[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[2]));
        	}
        }
      }
      else if ((header->Id == IMU_MESSAGE_2_ID) && (Length == sizeof(sIMU_Message2))) {
        if (Status == sdlcSEQ_ERR) {
          // Sequence number error
          IMU.ErrorCnt++;
          if (AUXCTRL_SS.IMUControls.ErrDetectOn)
        	  AUXsprintf("%u:IMU Sequence Number Error (Message ID = 0x%x)\n",GetTimeUs(),header->Id);
        }

        msg2 = (sIMU_Message2 *) RxData;

        memcpy((void *) &IMU.RawData, &msg2->Data, sizeof(sIMU_Message2Data));

        IMU.InertialData.AngularRate[0] = (double) -msg2->Data.AngularRateX * IMU_SF_ANGULAR_RATE_X;
        IMU.InertialData.AngularRate[1] = (double) msg2->Data.AngularRateZ * IMU_SF_ANGULAR_RATE_Z;
        IMU.InertialData.AngularRate[2] = (double) msg2->Data.AngularRateY * IMU_SF_ANGULAR_RATE_Y;
        IMU.InertialData.Acceleration[0] = (double) -msg2->Data.AccelerationX * IMU_SF_ACCELERATION;
        IMU.InertialData.Acceleration[1] = (double) msg2->Data.AccelerationZ * IMU_SF_ACCELERATION;
        IMU.InertialData.Acceleration[2] = (double) msg2->Data.AccelerationY * IMU_SF_ACCELERATION;
        IMU.InertialData.DeltaAngle[0] = (double) -msg2->Data.DeltaAngleX * IMU_SF_ANGLE_X;
        IMU.InertialData.DeltaAngle[1] = (double) msg2->Data.DeltaAngleZ * IMU_SF_ANGLE_Z;
        IMU.InertialData.DeltaAngle[2] = (double) msg2->Data.DeltaAngleY * IMU_SF_ANGLE_Y;
        IMU.InertialData.DeltaVelocity[0] = (double) -msg2->Data.DeltaVelocityX * IMU_SF_VELOCITY;
        IMU.InertialData.DeltaVelocity[1] = (double) msg2->Data.DeltaVelocityZ * IMU_SF_VELOCITY;
        IMU.InertialData.DeltaVelocity[2] = (double) msg2->Data.DeltaVelocityY * IMU_SF_VELOCITY;

        IMU.DataValid = TRUE;

        IMU.Cycle = 0;

        IMU.RxTimestamp = IMU_Timestamp;

        if (AUXCTRL_SS.IMUControls.AuxToCsv) {
        	AUXsprintf("%u,",GetTimeUs());
        	AUXsprintf("IMU,");
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.AngularRate[0]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.AngularRate[1]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.AngularRate[2]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.Acceleration[0]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.Acceleration[1]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.Acceleration[2]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.DeltaAngle[0]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.DeltaAngle[1]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.DeltaAngle[2]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[0]));
        	AUXsprintf("%s,",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[1]));
        	AUXsprintf("%s,\n",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[2]));
        }

        if (AUXCTRL_SS.IMUControls.RxAuxOn){
        	AUXsprintf("%u:IMU Rx (Message ID = 0x%x) Received \n",GetTimeUs(),header->Id);
        	if (AUXCTRL_SS.IMUControls.RxDetailAuxOn){
        		AUXsprintf(" AngularRate[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[0]));
        		AUXsprintf(" AngularRate[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[1]));
        		AUXsprintf(" AngularRate[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.AngularRate[2]));
        		AUXsprintf(" Acceleration[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[0]));
        		AUXsprintf(" Acceleration[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[1]));
        		AUXsprintf(" Acceleration[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.Acceleration[2]));
        		AUXsprintf(" DeltaAngle[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaAngle[0]));
        		AUXsprintf(" DeltaAngle[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaAngle[1]));
        		AUXsprintf(" DeltaAngle[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaAngle[2]));
        		AUXsprintf(" DeltaVelocity[0] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[0]));
        		AUXsprintf(" DeltaVelocity[1] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[1]));
        		AUXsprintf(" DeltaVelocity[2] = ");
        		AUXsprintf("%s\n",AUXsprintfDouble(IMU.InertialData.DeltaVelocity[2]));
        	}
        }
      }
      else {
        // Invalid message ID
        IMU.ErrorCnt++;
        if (AUXCTRL_SS.IMUControls.ErrDetectOn)
        	AUXsprintf("%u:IMU Invalid Message ID = 0x%x\n",GetTimeUs(),header->Id);
      }
    }
    else {
      // Invalid IMU address
      IMU.ErrorCnt++;
      if (AUXCTRL_SS.IMUControls.ErrDetectOn)
    	  AUXsprintf("%u:IMU Invalid Address\n",GetTimeUs());
    }
  }
  else {
    // CRC error
    IMU.ErrorCnt++;
    if (AUXCTRL_SS.IMUControls.ErrDetectOn)
    	AUXsprintf("%u:IMU CRC ERROR\n",GetTimeUs());
  }

  IMU.RxCnt++;

  TaskManagerAdd(task600HZ);
}

void IMU_CheckComms(void)
{
  DisableInterrupts();

  if (GetElapsedTime(IMU_Timestamp) >= 2100) {
    IMU_Timestamp = GetSystemTime();

    IMU.DataValid = FALSE;
    IMU.ErrorCnt++;
    IMU.Cycle++;

    if (AUXCTRL_SS.IMUControls.ErrDetectOn)
    	AUXsprintf("%u:IMU Data Invalid \n",GetTimeUs());

    if (IMU.Cycle >= 6) {
      IMU.Cycle = 0;
    }

    TaskManagerAdd(task600HZ);
  }

  EnableInterrupts();
}
