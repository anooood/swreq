/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <SAL_Seeker.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control data between the SAL_Seeker and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - CRC_Initialise                           : Initializes CRC calculation parameters and tables.
 *  - Calculate_CRC                            : Computes CRC value over a given data buffer.
 *  - ResetRxStates                            : Resets the reception state machine and related flags.
 *  - qsal_Isrcb                               : Interrupt Service Routine callback for QSAL data reception.
 *  - QSAL_SeekerInit                          : Initializes the QSAL seeker communication and state.
 *  - QSAL_SeekerSend                          : Sends a message or command to the QSAL seeker.
 *  - QSAL_HandleSBIT_Msg                      : Processes Seeker Built-In Test (SBIT) messages.
 *  - QSAL_HandleDetectionMsg                  : Handles detection event messages from the QSAL seeker.
 *  - ProcessrxByte                            : Processes a single received byte within the QSAL protocol.
 *  - QSAL_SeekerHandleRxDataBytes             : Processes raw incoming data bytes from the seeker.
 *  - QSAL_SeekerHandleRxDataMsgs              : Parses and dispatches complete seeker messages.
 *  - QSAL_RxService                           : Manages reception and processing of all QSAL data traffic.
 *  - QSAL_SeekerSetPRI                        : Sets the seeker's PRI configuration.
 *  - QSAL_SeekerSetAccuracy                   : Updates the seeker's accuracy parameters.
 *  - QSAL_SeekerSetJitter                     : Configures seeker timing jitter parameters.
 *  - QSAL_SeekerSetNewParams                  : Applies a full set of new seeker configuration parameters.
 *  - QSAL_SeekerSetEstTargetClosingVelocity   : Sets estimated target closing velocity for seeker tracking.
 *  - QSAL_SeekerSetEstTargetRange             : Sets estimated range to the target for seeker engagement logic.
 *  - QSAL_SeekerSetEstTargetPitchYaw          : Sets estimated target pitch and yaw angles for seeker guidance.
 *  - QSAL_SeekerSetSeekerToHostOffsets        : Configures physical offsets between seeker and host platform.
 *  - QSAL_GetFrequency                        : Retrieves the current operating frequency setting from the seeker.
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
#include "WIAC82_Comms.h"
#include "UART.h"
#include "FIFO.h"
#include "SAL_Seeker.h"
#include "MLAN.h"
#include "TypeDefines.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "BIT.h"


// SAL Seeker message ID's
#define SAL_SEEKER_ID_SBIT_REQ_ID             0x00
#define SAL_SEEKER_ID_SBIT                    0x01
#define SAL_SEEKER_ID_CLEAR_CBIT              0x04
#define SAL_SEEKER_ID_STATUS_REQ              0x0F
#define SAL_SEEKER_ID_OPERATION_CMD           0x10
#define SAL_SEEKER_ID_STANDBY_CMD             0x11
#define SAL_SEEKER_ID_DATA                    0x80
#define SAL_SEEKER_ID_TEL                     0x81

#define SAL_SEEKER_FIXED_DETECTION_LATENCY    3000 // [us]


#pragma pack(1)

typedef struct
{
  unsigned short AppSW_Ver;
  unsigned short AppSW_CRC;
  unsigned short AppFW_Ver;
  unsigned short AppFW_CRC;
  unsigned short LoaderSW_Ver;
  unsigned short LoaderSW_CRC;
  unsigned short LoaderFW_Ver;
  unsigned short LoaderFW_CRC;
  unsigned short CalDataCRC;
  unsigned char CalYear;
  unsigned char CalMonth;
  unsigned char CalDay;
  unsigned char CalHour;
  unsigned char CalMinute;
  unsigned char Spare;
  unsigned int SerialNo;
  unsigned int ETI;
  unsigned int SBIT;
  short P3V0ALevel;
  short N3V0ALevel;
  short P3V3DLevel;
  short P2V5DLevel;
  short P1V8DLevel;
  short P1V2DLevel;
  short P2V5ALevel;
  short M2V5ALevel;
  short HiVLevel;
  short DetCurrentA;
  short DetCurrentB;
  short DetCurrentC;
  short DetCurrentD;
  unsigned short Reserved0;
  int DetPeakA;
  int DetPeakB;
  int DetPeakC;
  int DetPeakD;
  unsigned int DetNoiseA;
  unsigned int DetNoiseB;
  unsigned int DetNoiseC;
  unsigned int DetNoiseD;
  unsigned char HeaterCurrentA;
  unsigned char HeaterCurrentB;
  unsigned char HeaterCurrentC;
  unsigned char HeaterCurrentD;
  unsigned int Reserved1[2];
} sSAL_SeekerSBIT;

typedef struct
{
  uSAL_SeekerStatus Status;
  short TargetYaw;
  short TargetPitch;
  unsigned short TargetSNR;
  unsigned int EstPRI;
  unsigned int MeasuredTime;
  unsigned int TimeReference;
  unsigned char Humidity;
  unsigned char Reserved0;
  short ProcessorTemp;
  short SensorTemp;
  short DetTempA;
  short DetTempB;
  short DetTempC;
  short DetTempD;
  unsigned char Reserved1[6];
  uSAL_SeekerCBIT CBIT;
} sSAL_SeekerData;

typedef struct
{
  unsigned int PRI;             // [ns]
  unsigned short Jitter;        // [ns]
  unsigned short Accuracy;      // [ns]
  unsigned int Range;           // [m]
  unsigned short Velocity;      // [m/s]
  unsigned short TestControl;
  unsigned int Reserved[4];
} sSAL_SeekerCommand;

typedef struct
{
  sMLAN_Header Header;
  union
  {
    sSAL_SeekerSBIT SBIT;
    sSAL_SeekerData Data;
    sSAL_SeekerTel Tel;
  } Payload;
} sSAL_SeekerRxMsg;

typedef struct
{
  sMLAN_Header Header;
  sSAL_SeekerCommand Command;
} sSAL_SeekerTxMsg;

#pragma pack()

sSAL_Seeker SAL_Seeker;


static unsigned char SAL_SeekerSBIT_Complete;
static unsigned int SAL_Seeker10msCnt;
static sSAL_SeekerTxMsg SAL_SeekerTxMsg;
static sSAL_SeekerTel SAL_SeekerTelemetry[6];


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerInit                                              */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Initialize SAL seeker interface                             */
/*                                                                           */
/* INPUTS      : None                                                        */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerInit(void)
{
  SAL_SeekerSBIT_Complete = FALSE;
  SAL_Seeker10msCnt = 0;

  SAL_SeekerTxMsg.Header.SourceId = 0x40;
  SAL_SeekerTxMsg.Header.DestinationId = 0x41;
  SAL_SeekerTxMsg.Header.Opcode = SAL_SEEKER_ID_SBIT_REQ_ID;
  SAL_SeekerTxMsg.Header.DataLength = 0;
  SAL_SeekerTxMsg.Header.SequenceNo = 1;
  SAL_SeekerTxMsg.Header.SequenceCnt = 1;
  SAL_SeekerTxMsg.Header.FrameCnt = 0;

  SAL_SeekerTxMsg.Command.PRI = 100000000;     // 100 ms (10 Hz)
  SAL_SeekerTxMsg.Command.Jitter = 500;
  SAL_SeekerTxMsg.Command.Accuracy = 5000;
  SAL_SeekerTxMsg.Command.Range = 10000.0;
  SAL_SeekerTxMsg.Command.TestControl = 0;

  memset(&SAL_Seeker, 0, sizeof(sSAL_Seeker));

  SAL_Seeker.AccuracyCmd = SAL_SeekerTxMsg.Command.Accuracy;
  SAL_Seeker.JitterCmd = SAL_SeekerTxMsg.Command.Jitter;
  SAL_Seeker.RangeCmd = SAL_SeekerTxMsg.Command.Range;
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSend                                              */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Send command message to seeker                              */
/*                                                                           */
/* INPUTS      : None                                                        */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/* NOTE: Should be called every 10 ms                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerSend(void)
{
  SAL_Seeker10msCnt++;

  if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
    // Increment communication error count if nothing received for the past 400ms
    if (SAL_SeekerSBIT_Complete) {
      if ((Weapon.SystemTime - SAL_Seeker.RxTimestamp) >= 400000) {
        SAL_Seeker.ErrCnt++;
        SAL_Seeker.RxTimestamp = Weapon.SystemTime;
      }

      // Send SAL Seeker operational command message once every 20 ms
      if (SAL_Seeker10msCnt & 0x01) {
        SAL_SeekerTxMsg.Header.Opcode = SAL_SEEKER_ID_OPERATION_CMD;
        SAL_SeekerTxMsg.Header.DataLength = sizeof(sSAL_SeekerCommand);
        SAL_SeekerTxMsg.Header.FrameCnt++;

        MLAN_Send(MLAN_SAL_SEEKER_NODE_ID, (unsigned char *) &SAL_SeekerTxMsg, SAL_SeekerTxMsg.Header.DataLength + sizeof(sMLAN_Header));

        SAL_Seeker.TxCnt++;
      }
    }
    else if ((SAL_Seeker10msCnt % 10) == 0) {
      //if (BIT.SBIT_Complete) {
      //  SAL_SeekerSBIT_Complete = TRUE;
      //  SAL_Seeker.RxTimestamp = Weapon.SystemTime;
      //}

      // Request SAL Seeker SBIT once every 100 ms until received
      SAL_SeekerTxMsg.Header.Opcode = SAL_SEEKER_ID_SBIT_REQ_ID;
      SAL_SeekerTxMsg.Header.DataLength = 0;
      SAL_SeekerTxMsg.Header.FrameCnt++;

      MLAN_Send(MLAN_SAL_SEEKER_NODE_ID, (unsigned char *) &SAL_SeekerTxMsg, SAL_SeekerTxMsg.Header.DataLength + sizeof(sMLAN_Header));
    }
  }
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSendClearCBIT                                     */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Transmits a 'Clear CBIT' command message to the SAL Seeker  */
/*                                                                           */
/* INPUTS      : None                                                        */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerSendClearCBIT(void)
{
  SAL_SeekerTxMsg.Header.Opcode = SAL_SEEKER_ID_CLEAR_CBIT;
  SAL_SeekerTxMsg.Header.DataLength = 0;
  SAL_SeekerTxMsg.Header.FrameCnt++;

  MLAN_Send(MLAN_SAL_SEEKER_NODE_ID, (unsigned char *) &SAL_SeekerTxMsg, SAL_SeekerTxMsg.Header.DataLength + sizeof(sMLAN_Header));
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerCalculateDetectionTime                            */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Calculates the MCP time of SAL seeker detection             */
/*                                                                           */
/* INPUTS      : unsigned int - SAL seeker time of detection [10ns]          */
/*               unsigned int - SAL seeker time of seeker sync pulse [10ns]  */
/*                                                                           */
/* RETURNS     : double - MCP time of detection [s]                          */
/*                                                                           */
/*****************************************************************************/
static double SAL_SeekerCalculateDetectionTime(unsigned int MeasuredTime, unsigned int TimeReference)
{
  TIME calculated_time;
  TIME mcp_current_time;
  TIME measured_time;
  TIME reference_time;


  mcp_current_time = GetSystemTime();

  // Calculate the MCP time of detection: = SyncMCP_Time + (SeekerTime - SyncSeekerTime)
  // Three possible scenarios:
  // 1. Normal: no timer wrapping and MCP and Seeker use the same sync (abs(measured time - reference) < 100ms).
  // 2. The detection time or sync time wrapped (abs(measured time - reference) > 21374835480ns).
  // 3. The MCP and seeker referenced different sync signals

  measured_time  = (TIME) MeasuredTime;
  reference_time = (TIME) TimeReference;

  // Check for wrapping:
  if ((measured_time - reference_time) > 100000000) {
    reference_time += 0x7FFFFFFF; // += 2 ^ 31
  }

  if ((measured_time - reference_time) < -100000000) {
    measured_time += 0x7FFFFFFF; // += 2 ^ 31
  }

  calculated_time = Weapon.SeekerSyncEventTime + (measured_time - reference_time) / 100;

  // If the calculated measured time is greater than current system time (cannot be in the future), the
  // MCP and SAL Seeker referenced different syncs
  if (calculated_time >= mcp_current_time) {
    if ((calculated_time - mcp_current_time) < 100000) {
      // The SAL Seeker detection time measurements were done on a previous sync so subtract 100ms
      calculated_time -= 100000;
    }
    else {
      // Error - use fixed latency
      calculated_time =  mcp_current_time - SAL_SEEKER_FIXED_DETECTION_LATENCY;
    }
  }

  // Use a fixed latency if the difference between the system time and the calculated detection time is greater than 100ms
  if ((mcp_current_time - calculated_time) >= 100000) {
    calculated_time = mcp_current_time - SAL_SEEKER_FIXED_DETECTION_LATENCY;
  }

  // Return the time in seconds
  return ((double) calculated_time / 1000000.0);
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerHandleSBIT                                        */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Extract received seeker SBIT data                           */
/*                                                                           */
/* INPUTS      : sSAL_SeekerSBIT* - received seeker message                  */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
static void SAL_SeekerHandleSBIT(sSAL_SeekerSBIT *SBIT_Data)
{
  SAL_Seeker.RxCnt++;
  SAL_Seeker.RxTimestamp = Weapon.SystemTime;

  SAL_Seeker.SW_CRC = SBIT_Data->AppSW_CRC;
  SAL_Seeker.CalDataCRC = SBIT_Data->CalDataCRC;
  SAL_Seeker.SBIT = SBIT_Data->SBIT;

  if ((SBIT_Data->SBIT & 0x80000000) == 0) {
    SAL_SeekerSBIT_Complete = TRUE;
    SAL_Seeker.SBIT = SBIT_Data->SBIT;
  }
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerHandleData                                        */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Extract received seeker status data                         */
/*                                                                           */
/* INPUTS      : sSAL_SeekerData* - received seeker message                  */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
static void SAL_SeekerHandleData(sSAL_SeekerData *RxData)
{
  SAL_Seeker.RxCnt++;
  SAL_Seeker.RxTimestamp = Weapon.SystemTime;

  if ((RxData->TargetYaw == 0) && (RxData->TargetPitch == 0)) {
    RxData->Status.Bits.AnglesValid = FALSE;
  }

  SAL_Seeker.Status = RxData->Status;
  SAL_Seeker.TargetSNR = RxData->TargetSNR;
  SAL_Seeker.TargetYaw = RxData->TargetYaw   * 1.0e-4;
  SAL_Seeker.TargetPitch = RxData->TargetPitch * 1.0e-4;
  SAL_Seeker.EstPRI = RxData->EstPRI;
  SAL_Seeker.CBIT = RxData->CBIT;
  SAL_Seeker.ProcessorTemperature = RxData->ProcessorTemp / 10;
  SAL_Seeker.DetectorTemperature = RxData->SensorTemp / 10;
  SAL_Seeker.DetectionTime = (RxData->Status.Bits.AnglesValid) ? SAL_SeekerCalculateDetectionTime(RxData->MeasuredTime, RxData->TimeReference) : ((double) Weapon.SystemTime / 1000000.0);
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerHandleTelemetry                                   */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Extract received seeker telemetry data                      */
/*                                                                           */
/* INPUTS      : sSAL_SeekerRxMsg* - received seeker message                 */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
static void SAL_SeekerHandleTelemetry(sSAL_SeekerRxMsg *RxMsg)
{
  switch (RxMsg->Header.SequenceNo) {
    case 1 : memcpy(&SAL_SeekerTelemetry[0], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel)); break;
    case 2 : memcpy(&SAL_SeekerTelemetry[1], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel)); break;
    case 3 : memcpy(&SAL_SeekerTelemetry[2], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel)); break;
    case 4 : memcpy(&SAL_SeekerTelemetry[3], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel)); break;
    case 5 : memcpy(&SAL_SeekerTelemetry[4], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel)); break;
    case 6 : {
      memcpy(&SAL_SeekerTelemetry[5], &RxMsg->Payload.Tel, sizeof(sSAL_SeekerTel));
      memcpy(&SAL_Seeker.Telemetry, SAL_SeekerTelemetry, sizeof(SAL_SeekerTelemetry));
    } break;
  }
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerHandleRxData                                      */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Handle received seeker data                                 */
/*                                                                           */
/* INPUTS      : unsigned char* - received seeker data                       */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerHandleRxData(unsigned char *RxData)
{
  sSAL_SeekerRxMsg *rx_data = (sSAL_SeekerRxMsg *) RxData;


  switch (rx_data->Header.Opcode) {
    case SAL_SEEKER_ID_SBIT : SAL_SeekerHandleSBIT(&rx_data->Payload.SBIT); break;
    case SAL_SEEKER_ID_DATA : SAL_SeekerHandleData(&rx_data->Payload.Data); break;
    case SAL_SEEKER_ID_TEL: SAL_SeekerHandleTelemetry(rx_data); break;
    default : break;
  }
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSetPRI                                            */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Updates the seeker PRI                                      */
/*                                                                           */
/* INPUTS      : double - SAL frequency [Hz]                                 */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerSetPRI(double Frequency)
{
  double period;


  if((Frequency >= 1.0) && (Frequency <= 30.0))
  {
    period = 1000000000.0 / Frequency;
    SAL_SeekerTxMsg.Command.PRI = (unsigned int) period;
  }
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSetAccuracy                                       */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Updates seeker detection accuracy                           */
/*                                                                           */
/* INPUTS      : unsigned short - accuracy time [ns]                         */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerSetAccuracy(unsigned short Accuracy)
{
  SAL_SeekerTxMsg.Command.Accuracy = Accuracy;
  SAL_Seeker.AccuracyCmd = Accuracy;
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerSetJitter                                         */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Updates seeker jitter                                       */
/*                                                                           */
/* INPUTS      : unsigned short - jitter time [ns]                           */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerSetJitter(unsigned short Jitter)
{
  SAL_SeekerTxMsg.Command.Jitter = Jitter;
  SAL_Seeker.JitterCmd = Jitter;
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_SeekerUpdateCommandMsg                                  */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Update the range and velocity to the seeker                 */
/*                                                                           */
/* INPUTS      : float - range to target [m]                                 */
/*               float - velocity [m/s]                                      */
/*                                                                           */
/* RETURNS     : None                                                        */
/*                                                                           */
/*****************************************************************************/
void SAL_SeekerUpdateCommandMsg(float Range, float Velocity)
{
  SAL_SeekerTxMsg.Command.Range = Range;
  SAL_SeekerTxMsg.Command.Velocity = Velocity;

  SAL_Seeker.RangeCmd = Range;
}


/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : SAL_GetFrequency                                            */
/*                                                                           */
/* CREATED     : 2020/03/25                                                  */
/*                                                                           */
/* CODED BY    : W.P. Erasmus                                                */
/*                                                                           */
/* DESCRIPTION : Get the seeker laser frequency from NATO code               */
/*                                                                           */
/* INPUTS      : float* - frequency [Hz]                                     */
/*               unsigned int - NATO code                                    */
/*                                                                           */
/* RETURNS     : result - TRUE = successful                                  */
/*                                                                           */
/*****************************************************************************/
unsigned char SAL_GetFrequency(float *Frequency, unsigned int Code)
{
  unsigned int d1;
  unsigned int d2;
  unsigned int d3;
  unsigned int d4;
  unsigned int n;
  float period = 100.0;           // Default period is 100 ms (10 Hz)
  unsigned char result = TRUE;    // Default is successful


  d1 = Code % 10;
  d2 = (Code / 10) % 10;
  d3 = (Code / 100) % 10;
  d4 = (Code / 1000) % 10;

  if ((d1 > 0) && (d1 < 9) && (d2 > 0) && (d2 < 9) &&
      (d3 > 0) && (d3 < 5) && (d4 == 1)               ) {
    // Band 2 code
    n = d1 + ((d2 - 1) * 8) + ((d3 - 1) * 64);

    if ((n > 0) && (n < 257)) {
      period = 50.0 + (0.1 * n);
    }
    else {
      result = FALSE;
    }
  }
  else if ((d1 > 0) && (d1 < 9) && (d2 > 0) && (d2 < 9) &&
           (d3 > 4) && (d3 < 8) && (d4 == 1)               ) {
    // Band 1 code
    n = d1 + ((d2 - 1) * 8) + ((d3 - 5) * 64);

    if (n > 129) {
      n = n + (n - 129);
    }

    if ((n > 0) && (n < 256)) {
      period = 87.2 + (0.1 * n);
    }
    else {
      result = FALSE;
    }
  }
  else {
    result = FALSE;
  }

  *Frequency = 1000.0 / period;

  // Convert period [ms] to frequency [Hz] and return
  return result;
}
