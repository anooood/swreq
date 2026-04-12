/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Telemetry.c>
 *
 *  Description:
 *  ------------
 *  The Telemetry module handles the collection, formatting, transmission, and
 *  reception of data between the system and a remote monitoring station or
 *  control unit.
 *
 *  Function(s):
 *  ------------
 *  - TelemetryInit                   : Initialize Telemetry
 *  - TelemetryBuild600Hz             : Builds either the first or second 600 Hz data
 *  - TelemetryBuild2HzBlock1         : Build 2Hz block 1 telemetry message
 *  - TelemetryBuild2HzBlock2         : Build 2Hz block 2 telemetry message
 *  - TelemetryBuild2HzBlock3         : Build 2Hz block 3 telemetry message
 *  - TelemetryBuild2HzBlock4         : Build 2Hz block 4 telemetry message
 *  - TelemetryBuild2HzBlock5         : Build 2Hz block 5 telemetry message
 *  - TelemetryBuild2HzBlock7         : Build 2Hz block 7 telemetry message
 *  - TelemetryBuild2HzBlock8         : Build 2Hz block 8 telemetry message
 *  - TelemetryBuild2HzBlock10        : Build 2Hz block 10 telemetry message
 *  - TelemetrySend                   : Build and transmit telemetry messages
 *  - TelUseFixedData                 : Enable or disable fixed telemetry
 *  - TelSendFixedTelemetry           : Force telemetry data to fixed values for easy verification
 *  - TelFix2HzData                   : Force 2Hz telemtry data to fixed values
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "TypeDefines.h"
#include "FPGA.h"
#include "Interrupts.h"
#include "Arcnet.h"
#include "MLAN.h"
#include "BIT.h"
#include "CRC.h"
#include "MCP.h"
#include "FIFO.h"
#include "WIAC82_Comms.h"
#include "WeaponSettings.h"
#include "TaskManager.h"
#include "Telemetry.h"
#include "TelemetryDef.h"
#include "IMU.h"
#include "Servos.h"
#include "WiFi.h"
#include "UART.h"
#include "Fuze.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"
#include "SAL_Seeker.h"
#include "UBLOX.h"
#include "Nav.h"
#include "Guidance.h"
#include "BatteryPack.h"
#include "PressureSensor.h"
#include "HOBS.h"
#include "HOBSComms.h"
#include "AuxControls.h"

#define TEL_FIXED_ID      0x12345678


typedef struct
{
  sMLAN_Header Header;
  sTelemetry Telemetry;
} sTelemetryMsg;


unsigned int FixedTelemetry;

static unsigned char Tel2HzIndex;
static sArcnet TLAN;
static sMLAN_Header TelemetryHeader;
static sTelemetry Telemetry;

static uTel2HzBlock1 Tel2HzBlock1;
static uTel2HzBlock2 Tel2HzBlock2;
static uTel2HzBlock3 Tel2HzBlock3;
static uTel2HzBlock4 Tel2HzBlock4;
static uTel2HzBlock5 Tel2HzBlock5;
static uTel2HzBlock6 Tel2HzBlock6;
static uTel2HzBlock7 Tel2HzBlock7;
static uTel2HzBlock8 Tel2HzBlock8;
static uTel2HzBlock9 Tel2HzBlock9;
static uTel2HzBlock10 Tel2HzBlock10;
static sSAL_SeekerTel SeekerTelBuf2;
static sSAL_SeekerTel SeekerTelBuf3;
static sSAL_SeekerTel SeekerTelBuf4;
static sSAL_SeekerTel SeekerTelBuf5;
static sSAL_SeekerTel SeekerTelBuf6;

static void TelSendFixedTelemetry(unsigned char Tel100HzIndex);
static void TelFix2HzData(void);

void TelemetryInit(void)
{
  FixedTelemetry = 0;
  Tel2HzIndex = 0;

  memset(&Telemetry, 0, sizeof(sTelemetry));

  TelemetryHeader.SourceId = 0; //0x10;
  TelemetryHeader.DestinationId = 0; //0x11;
  TelemetryHeader.DataLength = sizeof(sTelemetry);
  TelemetryHeader.SequenceNo = 1;
  TelemetryHeader.SequenceCnt = 1;

  ArcnetInit(&TLAN, FPGA_ARCNET_2_ADDR, intARCNET_2,  MLAN_TEL_NODE_ID, FALSE, FALSE);
}

void TelemetryBuild600Hz(void)
{
  if ((IMU.Cycle & 0x01) == 0) {
    memcpy(&Telemetry.Tel600Hz_1, (void *) &IMU.RawData, 12);
    Telemetry.Tel600Hz_1.IMU_RxCnt = IMU.RxCnt;
    Telemetry.Tel600Hz_1.IMU_ErrorCnt = IMU.ErrorCnt;
  }
  else {
    memcpy(&Telemetry.Tel600Hz_2, (void *) &IMU.RawData, 12);
    Telemetry.Tel600Hz_2.IMU_RxCnt = IMU.RxCnt;
    Telemetry.Tel600Hz_2.IMU_ErrorCnt = IMU.ErrorCnt;
  }
}

void TelemetryBuild2HzBlock1(void)
{
  Tel2HzBlock1.PowerCycleCnt = WeaponSettings.MCP_Log.PowerCycleCnt;
  Tel2HzBlock1.MCP_ETI = MCP.ETI;
  Tel2HzBlock1.MCP_PLD_StatusReg = MCP.PLD_StatusReg;
  Tel2HzBlock1.WeaponSBIT = Weapon.SBIT.DWord;
  Tel2HzBlock1.MCP_SBIT = MCP.SBIT.Word;
  Tel2HzBlock1.WL_SBIT = WiFi.SBIT.Word;
  if (WeaponSettings.WeaponConfig.HOBS_Present)
  {
	  Tel2HzBlock1.HOBS_SBIT = SS_HOBS.Status.SBIT.DWord;
  } else {
	  Tel2HzBlock1.HOBS_SBIT = 0;
  }
  Tel2HzBlock1.ServoSBIT = Servo.SBIT;
  Tel2HzBlock1.BatteryPackSBIT = BatteryPack.SBIT.Word;

  switch (WeaponSettings.WeaponConfig.SeekerType) {
    case seekerNONE : Tel2HzBlock1.SeekerSBIT = 0; break;
    //case seekerSAL : Tel2HzBlock1.SeekerSBIT = SAL_Seeker.SBIT; break;
    case seekerSAL :
    {
    	if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	{
    		Tel2HzBlock1.SeekerSBIT = SAL_SS.SBIT.PBITStatus;
    	}
    	else if (AUXCTRL_SS.MAINControls.QSAL_On)
    	{
    		Tel2HzBlock1.SeekerSBIT = QSAL_SS.SharedData.SBIT.DWord;
    	}
    	break;
    }
    default : Tel2HzBlock1.SeekerSBIT = 0; break;
  }

  Tel2HzBlock1.WeaponType = WeaponSettings.WeaponConfig.WeaponType;
  Tel2HzBlock1.SeekerType = WeaponSettings.WeaponConfig.SeekerType;
  Tel2HzBlock1.BombType = WeaponSettings.WeaponConfig.BombType;
  Tel2HzBlock1.HOBS_Present = WeaponSettings.WeaponConfig.HOBS_Present;
  Tel2HzBlock1.DTAC_Present = WeaponSettings.WeaponConfig.DTAC_Present;
  Tel2HzBlock1.GuidanceAndControlMode = WeaponSettings.WeaponConfig.GuidanceAndControlMode;
  Tel2HzBlock1.FuzeType = SS_FUZE.Fuze.Type;
  Tel2HzBlock1.FuzeMode = SS_FUZE.Fuze.Mode;
  Tel2HzBlock1.FuzeDelay = SS_FUZE.Fuze.FuzeTimeDelay;
  Tel2HzBlock1.FuzeStatus = SS_FUZE.Fuze.Status.Word;
}

void TelemetryBuild2HzBlock2(void)
{
  Tel2HzBlock2.RawMission = Weapon.RawMission;
  Tel2HzBlock2.WindEntryCnt = Weapon.Wind.WindEntryCnt;
  Tel2HzBlock2.WindCRC = Weapon.Wind.CRC;
  memcpy(Tel2HzBlock2.WindEntries, (void *) Weapon.Wind.WindEntries, 48);
  Tel2HzBlock2.MissionLatitude = Weapon.Mission.PriTgtLatitude;
  Tel2HzBlock2.MissionLongitude = Weapon.Mission.PriTgtLongitude;
  Tel2HzBlock2.MissionAltitude = Weapon.Mission.PriTgtAltitude;
  Tel2HzBlock2.MissionSource = Weapon.Mission.MissionSource;
  Tel2HzBlock2.MissionSetResult = Weapon.Mission.MissionSetResult;
}

void TelemetryBuild2HzBlock3(void)
{
  Tel2HzBlock3.TelListVersion = TEL_VERSION;

  Tel2HzBlock3.MCP_LogInfo.SW_Version = MCP.SW_Version;
  Tel2HzBlock3.MCP_LogInfo.FW_Version = MCP.FW_Version;
  Tel2HzBlock3.MCP_LogInfo.SW_CRC = MCP.SW_CRC;
  Tel2HzBlock3.MCP_LogInfo.FW_CRC = MCP.FW_CRC;
  memcpy(Tel2HzBlock3.MCP_LogInfo.PartNo, WeaponSettings.MCP_Log.Log.PartNo, 10);
  memcpy(Tel2HzBlock3.MCP_LogInfo.IssueNo, WeaponSettings.MCP_Log.Log.IssueNo, 4);
  memcpy(Tel2HzBlock3.MCP_LogInfo.SerialNo, WeaponSettings.MCP_Log.Log.PartNo, 9);

  Tel2HzBlock3.ServoLogInfo.SW_Version = Servo.SwVersion;
  Tel2HzBlock3.ServoLogInfo.SW_CRC = Servo.SwCRC;
  Tel2HzBlock3.ServoLogInfo.FW_Version = 0; //Not Applicable
  Tel2HzBlock3.ServoLogInfo.FW_CRC = 0; //Not Applicable
  //memcpy(Tel2HzBlock3.ServoLogInfo.PartNo, Servo.ServoPartNumberLeft,5);
  //memcpy(Tel2HzBlock3.ServoLogInfo.PartNo + 5, Servo.ServoPartNumberRight,5);
  //memcpy(Tel2HzBlock3.ServoLogInfo.IssueNo, Servo.ServoIssueNumber, 4);
  //memcpy(Tel2HzBlock3.ServoLogInfo.SerialNo, Servo.ServoSerialNumber, 9);


  if(WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
  {
	  if (AUXCTRL_SS.MAINControls.MPSAL_On)
	  {
		  Tel2HzBlock3.SeekerLogInfo.SW_Version = 0; //Not Applicable
		  Tel2HzBlock3.SeekerLogInfo.SW_CRC = 0; //Not Applicable
		  Tel2HzBlock3.SeekerLogInfo.FW_Version = SAL_SS.PFBIT.FirmwareVersionNo;
		  Tel2HzBlock3.SeekerLogInfo.FW_CRC = 0; //Not Applicable
	  }
	  else if(AUXCTRL_SS.MAINControls.QSAL_On)
	  {
		  Tel2HzBlock3.SeekerLogInfo.SW_Version = QSAL_SS.SharedData.AppSWVersion;
		  Tel2HzBlock3.SeekerLogInfo.SW_CRC = QSAL_SS.SharedData.SW_CRC;
		  Tel2HzBlock3.SeekerLogInfo.FW_Version = QSAL_SS.SharedData.AppFWVersion;
		  Tel2HzBlock3.SeekerLogInfo.FW_CRC = QSAL_SS.SharedData.FW_CRC;
	  }
  }

  Tel2HzBlock3.WiFiLogInfo.SW_Version = WiFi.SW_Version;
  Tel2HzBlock3.WiFiLogInfo.SW_CRC = WiFi.SW_CRC;
  Tel2HzBlock3.WiFiLogInfo.FW_Version = 0; //Not Applicable
  Tel2HzBlock3.WiFiLogInfo.FW_CRC = 0; //Not Applicable

  memcpy(Tel2HzBlock3.WiFiLogInfo.PartNo, WiFi.LogData.WifiPartNumberLeft, 5);
  memcpy(Tel2HzBlock3.WiFiLogInfo.PartNo + 5, WiFi.LogData.WifiPartNumberRight, 5);
  memcpy(Tel2HzBlock3.WiFiLogInfo.IssueNo, WiFi.LogData.WifiIssueNumber, 4);
  memcpy(Tel2HzBlock3.WiFiLogInfo.SerialNo, WiFi.LogData.WifiSerialNumber, 9);

}

void TelemetryBuild2HzBlock4(void)
{
  Tel2HzBlock4.GuidanceSVN_Revision = GUIDANCE_SVN_REVISION;
  Tel2HzBlock4.DataFileCRC = WeaponSettings.DataFileCRC;
  Tel2HzBlock4.DataFileVersion = WeaponSettings.DataFileVersion;
  Tel2HzBlock4.DataFileStructId = WeaponSettings.DataFileStructId;
  Tel2HzBlock4.DataFileLoaded = WeaponSettings.DataFileLoaded;
  Tel2HzBlock4.DataFileValid = WeaponSettings.DataFileValid;
  Tel2HzBlock4.WeaponDefIndex = WeaponSettings.WeaponDefIndex;
  Tel2HzBlock4.WeaponDefCRC = WeaponSettings.WeaponDefCRC;
  Tel2HzBlock4.WeaponDefVersion = WeaponSettings.WeaponDefVersion;
  Tel2HzBlock4.WeaponDefBombType = WeaponSettings.WeaponDefBombType;
  Tel2HzBlock4.WeaponDefWeaponType = WeaponSettings.WeaponDefWeaponType;
  Tel2HzBlock4.WeaponDefCalculatedCRC = WeaponSettings.WeaponDefSize;
  Tel2HzBlock4.WeaponDefSize = WeaponSettings.WeaponDefSizeInDataFileHeader;
  Tel2HzBlock4.WeaponDefSizeInDataFileHeader = WeaponSettings.WeaponDefCalculatedCRC;
  Tel2HzBlock4.WeaponDefStartID = WeaponSettings.WeaponDefStartID;

  Tel2HzBlock4.HOBS_LogInfo.SW_Version = 0; // Not Applicable
  Tel2HzBlock4.HOBS_LogInfo.SW_CRC = 0; // Not Applicable
  Tel2HzBlock4.HOBS_LogInfo.FW_Version = SS_HOBS.FW_HOBS_Version;
  Tel2HzBlock4.HOBS_LogInfo.FW_CRC = SS_HOBS.Version.CRC;

}

void TelemetryBuild2HzBlock5(void)
{
  Tel2HzBlock5.NAV_Tel100at2Hz = NavTel100Hz.T2Bx;
}

void TelemetryBuild2HzBlock7(void)
{
  Tel2HzBlock7.MCP_CBIT_EventCnt = BIT.MCP_CBIT_EventCnt;
  Tel2HzBlock7.WeaponCBIT_EventCnt = BIT.WeaponCBIT_EventCnt;

  memcpy(Tel2HzBlock7.MCP_CBIT_Events, BIT.MCP_CBIT_Events, sizeof(Tel2HzBlock7.MCP_CBIT_Events));
  memcpy(Tel2HzBlock7.WeaponCBIT_Events, BIT.WeaponCBIT_Events, sizeof(Tel2HzBlock7.WeaponCBIT_Events));
}

void TelemetryBuild2HzBlock8(void)
{
  memcpy(Tel2HzBlock8.WC_UserLogEntries, WiFi.UserLogEntries, sizeof(Tel2HzBlock8.WC_UserLogEntries));
}

void TelemetryBuild2HzBlock10(void)
{
  Tel2HzBlock10.WPN_HS = Weapon.HealthAndStatus;
  Tel2HzBlock10.WPN_HS_Telemetry = Weapon.Telemetry;
}

void TelemetrySend(unsigned char Tel100HzIndex)
{
  sTelemetryMsg tel_msg;


  if (FixedTelemetry == TEL_FIXED_ID) {
    TelSendFixedTelemetry(Tel100HzIndex);
    return;
  }

  Telemetry.Tel100HzIndex = Tel100HzIndex;
  Telemetry.Tel20HzIndex++;

  if (Telemetry.Tel20HzIndex > 14) {
    Telemetry.Tel20HzIndex = Telemetry.Tel100HzIndex;
  }

  Telemetry.Tel300Hz.WeaponTime = Weapon.SystemTime;
  Telemetry.Tel300Hz.IMU_Cycle = IMU.Cycle;
  Telemetry.Tel300Hz.Task600HzExecutionTime = TaskMananger.Tasks[task600HZ].ExecutionTime;
  Telemetry.Tel300Hz.Task100HzExecutionTime = TaskMananger.Tasks[task100HZ].ExecutionTime;
  Telemetry.Tel300Hz.Task600HzOverrunCnt = TaskMananger.Tasks[task600HZ].OverrunCnt;
  Telemetry.Tel300Hz.Task100HzOverrunCnt = TaskMananger.Tasks[task100HZ].OverrunCnt;
  Telemetry.Tel300Hz.Task50HzOverrunCnt = TaskMananger.Tasks[task50HZ].OverrunCnt;
  Telemetry.Tel300Hz.Task600HzCount = TaskMananger.Tasks[task600HZ].Count;
  Telemetry.Tel300Hz.Task100HzCount = TaskMananger.Tasks[task100HZ].Count;
  Telemetry.Tel300Hz.Task50HzCount = TaskMananger.Tasks[task50HZ].Count;
  Telemetry.Tel300Hz.Task2HzCount = TaskMananger.Tasks[task2HZ].Count;
  Telemetry.Tel300Hz.ServoFrameCnt = 0;
  Telemetry.Tel300Hz.ITUC_FrameCnt = 0;
  Telemetry.Tel300Hz.PitotFrameCnt = 0;
  Telemetry.Tel300Hz.FrameCnt++;
  Telemetry.Tel300Hz.WeaponStatus = Weapon.Status.DWord;
  Telemetry.Tel300Hz.WeaponDiscretes = Weapon.Discretes.Word;
  Telemetry.Tel300Hz.WeaponFlags = Weapon.Flags.Word;
  Telemetry.Tel300Hz.TimeSinceLaunch = Weapon.TimeSinceLaunch;
  Telemetry.Tel300Hz.SV_DeflCmd1 = Servo.ScaledCommands[0];
  Telemetry.Tel300Hz.SV_DeflCmd2 = Servo.ScaledCommands[1];
  Telemetry.Tel300Hz.SV_DeflCmd3 = Servo.ScaledCommands[2];
  Telemetry.Tel300Hz.SV_DeflCmd4 = Servo.ScaledCommands[3];
  Telemetry.Tel300Hz.SV_Feedback1 = Servo.ScaledFeedbacks[0];
  Telemetry.Tel300Hz.SV_Feedback2 = Servo.ScaledFeedbacks[1];
  Telemetry.Tel300Hz.SV_Feedback3 = Servo.ScaledFeedbacks[2];
  Telemetry.Tel300Hz.SV_Feedback4 = Servo.ScaledFeedbacks[3];
  Telemetry.Tel300Hz.SV_Current1 = Servo.Currents[0];
  Telemetry.Tel300Hz.SV_Current2 = Servo.Currents[1];
  Telemetry.Tel300Hz.SV_Current3 = Servo.Currents[2];
  Telemetry.Tel300Hz.SV_Current4 = Servo.Currents[3];
  Telemetry.Tel300Hz.SV_MotorVoltage = Servo.ScaledMotorVoltage;
  Telemetry.Tel300Hz.SV_SupplyVoltage = Servo.ScaledSupplyVoltage;
  Telemetry.Tel300Hz.SV_Discretes = Servo.Discretes.Byte;
  Telemetry.Tel300Hz.SV_CBIT = Servo.CBIT.Byte;
  Telemetry.Tel300Hz.SV_FinLockPos = Servo.ScaledFinLockPosition;
  Telemetry.Tel300Hz.SV_Temperature = Servo.PCB_Temperature;
  Telemetry.Tel300Hz.SV_FinLocked = Servo.FinLocked;
  Telemetry.Tel300Hz.SV_Spare1 = 0;
  Telemetry.Tel300Hz.SV_Spare2 = 0;
  Telemetry.Tel300Hz.SV_Spare3 = 0;
  Telemetry.Tel300Hz.AutoPilot300HzTel = AutoPilot300HzTelemetry;

  // 100 Hz Tel
  switch (Telemetry.Tel100HzIndex) {
    case 0 : {
      Telemetry.Tel100HzBlock1.Weapon100HzTimestamp = Weapon.Weapon100HzTimestamp;
      Telemetry.Tel100HzBlock1.TTKF_Telemetry = TTKF_Telemetry;
    } break;
    case 1 : {
      Telemetry.Tel100HzBlock2.Weapon100HzTimestamp = Weapon.Weapon100HzTimestamp;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleX = IMU.RawData.DeltaAngleX;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleY = IMU.RawData.DeltaAngleY;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleZ = IMU.RawData.DeltaAngleZ;
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityX = IMU.RawData.DeltaVelocityX;
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityY = IMU.RawData.DeltaVelocityY;
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityZ = IMU.RawData.DeltaVelocityZ;

      Telemetry.Tel100HzBlock2.NAV_SystemTime      = NavTel100Hz.T100B1.NAV_SystemTime     ;
      Telemetry.Tel100HzBlock2.NAV_baro_SystemTime = NavTel100Hz.T100B1.NAV_baro_SystemTime;
      Telemetry.Tel100HzBlock2.NAV_baro_alt        = NavTel100Hz.T100B1.NAV_baro_alt       ;
      Telemetry.Tel100HzBlock2.NAV_baro_pressure   = NavTel100Hz.T100B1.NAV_baro_pressure  ;
      Telemetry.Tel100HzBlock2.NAV_Reserved1       = NavTel100Hz.T100B1.NAV_Reserved1      ;
      Telemetry.Tel100HzBlock2.NAV_Reserved2       = NavTel100Hz.T100B1.NAV_Reserved2      ;
      Telemetry.Tel100HzBlock2.NAV_Reserved3       = NavTel100Hz.T100B1.NAV_Reserved3      ;
      Telemetry.Tel100HzBlock2.NAV_100HzTimestamp  = NavTel100Hz.T100B1.NAV_100HzTimestamp ;
      Telemetry.Tel100HzBlock2.NAV_100HzCount      = NavTel100Hz.T100B1.NAV_100HzCount     ;
      Telemetry.Tel100HzBlock2.NAV_AngPsi          = NavTel100Hz.T100B1.NAV_AngPsi         ;
      Telemetry.Tel100HzBlock2.NAV_AngTheta        = NavTel100Hz.T100B1.NAV_AngTheta       ;
      Telemetry.Tel100HzBlock2.NAV_AngPhi          = NavTel100Hz.T100B1.NAV_AngPhi         ;
      Telemetry.Tel100HzBlock2.NAV_PosXecef        = NavTel100Hz.T100B1.NAV_PosXecef       ;
      Telemetry.Tel100HzBlock2.NAV_PosYecef        = NavTel100Hz.T100B1.NAV_PosYecef       ;
      Telemetry.Tel100HzBlock2.NAV_PosZecef        = NavTel100Hz.T100B1.NAV_PosZecef       ;
      Telemetry.Tel100HzBlock2.NAV_VelXecef        = NavTel100Hz.T100B1.NAV_VelXecef       ;
      Telemetry.Tel100HzBlock2.NAV_VelYecef        = NavTel100Hz.T100B1.NAV_VelYecef       ;
      Telemetry.Tel100HzBlock2.NAV_VelZecef        = NavTel100Hz.T100B1.NAV_VelZecef       ;
      Telemetry.Tel100HzBlock2.NAV_LatLLA          = NavTel100Hz.T100B1.NAV_LatLLA         ;
      Telemetry.Tel100HzBlock2.NAV_LongLLA         = NavTel100Hz.T100B1.NAV_LongLLA        ;
      Telemetry.Tel100HzBlock2.NAV_AltLLA          = NavTel100Hz.T100B1.NAV_AltLLA         ;
      Telemetry.Tel100HzBlock2.NAV_PressAlt        = NavTel100Hz.T100B1.NAV_PressAlt       ;
      Telemetry.Tel100HzBlock2.NAV_Mode            = NavTel100Hz.T100B1.NAV_Mode           ;
      Telemetry.Tel100HzBlock2.NAV_Version         = NavTel100Hz.T100B1.NAV_Version        ;
    } break;
    case 2 : {
      Telemetry.Tel100HzBlock3.Weapon100HzTimestamp = Weapon.Weapon100HzTimestamp;

      Telemetry.Tel100HzBlock3.NAV_Align_Timestamp   = NavTel100Hz.T100B1.NAV_Align_Timestamp  ;
      Telemetry.Tel100HzBlock3.NAV_Align_CallCount   = NavTel100Hz.T100B1.NAV_Align_CallCount  ;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_x       = NavTel100Hz.T100B1.NAV_Align_pos_x      ;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_y       = NavTel100Hz.T100B1.NAV_Align_pos_y      ;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_z       = NavTel100Hz.T100B1.NAV_Align_pos_z      ;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_x       = NavTel100Hz.T100B1.NAV_Align_vel_x      ;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_y       = NavTel100Hz.T100B1.NAV_Align_vel_y      ;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_z       = NavTel100Hz.T100B1.NAV_Align_vel_z      ;
      Telemetry.Tel100HzBlock3.NAV_Align_phi         = NavTel100Hz.T100B1.NAV_Align_phi        ;
      Telemetry.Tel100HzBlock3.NAV_Align_the         = NavTel100Hz.T100B1.NAV_Align_the        ;
      Telemetry.Tel100HzBlock3.NAV_Align_psi         = NavTel100Hz.T100B1.NAV_Align_psi        ;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_pos   = NavTel100Hz.T100B1.NAV_Align_valid_pos  ;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_vel   = NavTel100Hz.T100B1.NAV_Align_valid_vel  ;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_euler = NavTel100Hz.T100B1.NAV_Align_valid_euler;
      Telemetry.Tel100HzBlock3.NAV_Align_valid       = NavTel100Hz.T100B1.NAV_Align_valid      ;
      Telemetry.Tel100HzBlock3.NAV_Reserved1         = NavTel100Hz.T100B1.NAV_Reserved1        ;
      Telemetry.Tel100HzBlock3.NAV_Reserved2         = NavTel100Hz.T100B1.NAV_Reserved2        ;
      Telemetry.Tel100HzBlock3.NAV_Reserved3         = 0;
      Telemetry.Tel100HzBlock3.NAV_Reserved4         = 0;
      Telemetry.Tel100HzBlock3.NAV_Reserved5         = 0;

      Telemetry.Tel100HzBlock3.GuidTel100Hz = GuidTel100Hz;
    } break;
  }

  // 20 Hz Tel
  switch (Telemetry.Tel20HzIndex) {
    case 0 : {
      Telemetry.Tel20HzBlock1.NAV_2HzTimestamp      = NavTel2Hz.NAV_2HzTimestamp  ;
      Telemetry.Tel20HzBlock1.NAV_2HzCount          = NavTel2Hz.NAV_2HzCount      ;
      Telemetry.Tel20HzBlock1.NAV_StoreTimeReq      = NavTel2Hz.NAV_StoreTimeReq  ;
      Telemetry.Tel20HzBlock1.NAV_StoreCase         = NavTel2Hz.NAV_StoreCase     ;
      Telemetry.Tel20HzBlock1.NAV_StoreIdxLow       = NavTel2Hz.NAV_StoreIdxLow   ;
      Telemetry.Tel20HzBlock1.NAV_StoreIdxHi        = NavTel2Hz.NAV_StoreIdxHi    ;
      Telemetry.Tel20HzBlock1.NAV_StoreReserved1    = NavTel2Hz.NAV_StoreReserved1;
      Telemetry.Tel20HzBlock1.NAV_StoreTimeLow      = NavTel2Hz.NAV_StoreTimeLow  ;
      Telemetry.Tel20HzBlock1.NAV_StoreTimeHi       = NavTel2Hz.NAV_StoreTimeHi   ;
      Telemetry.Tel20HzBlock1.NAV_StorePos[0]       = NavTel2Hz.NAV_StorePos[0]   ;
      Telemetry.Tel20HzBlock1.NAV_StorePos[1]       = NavTel2Hz.NAV_StorePos[1]   ;
      Telemetry.Tel20HzBlock1.NAV_StorePos[2]       = NavTel2Hz.NAV_StorePos[2]   ;
      Telemetry.Tel20HzBlock1.NAV_StoreReserved2    = NavTel2Hz.NAV_StoreReserved2;

      memcpy(Telemetry.Tel20HzBlock1.NAV_CovMatrix, NavTel100Hz.T100B1.NAV_CovMatrix, 156);
    } break;
    case 1 : {
      Telemetry.Tel20HzBlock2.GuidTel20HzA = GuidTel20HzA;
    } break;
    case 2 : {
      Telemetry.Tel20HzBlock3.GuidTel20HzB = GuidTel20HzB;
      Telemetry.Tel20HzBlock3.Guidance10HzCnt = Guidance10HzCnt;
      //Telemetry.Tel20HzBlock3.GuidLAR = LAR;
      Telemetry.Tel20HzBlock3.GUID_Autop_Tel20Hz = AutoPilot20HzTelemetry;

      Telemetry.Tel20HzBlock3.GuidLAR.radius = Weapon.LAR.LAR_Rad;
      Telemetry.Tel20HzBlock3.GuidLAR.exclrad = Weapon.LAR.LAR_Rad_Excl;
      Telemetry.Tel20HzBlock3.GuidLAR.Psi2LAR = Weapon.LAR.RelBrngToLAR;
      Telemetry.Tel20HzBlock3.GuidLAR.Psi2Tgt = Weapon.LAR.RelBrngToTGT;
      Telemetry.Tel20HzBlock3.GuidLAR.Rge2LAR = Weapon.LAR.DistToLAR;
      Telemetry.Tel20HzBlock3.GuidLAR.Rge2Tgt = Weapon.LAR.DistToTGT;
      Telemetry.Tel20HzBlock3.GuidLAR.Time2Go = Weapon.LAR.TimeToGo;
      Telemetry.Tel20HzBlock3.GuidLAR.Time2Imp = Weapon.LAR.TimeToImpact;
      Telemetry.Tel20HzBlock3.GuidLAR.Time2Lase = Weapon.LAR.TimeToIllum;
    } break;
    case 3 : {
      Telemetry.Tel20HzBlock4.AC_TA_Timestamp = Aircraft.TransferAlignmentRxTimestamp;
      Telemetry.Tel20HzBlock4.AC_TA = Aircraft.TransferAlignment;
      Telemetry.Tel20HzBlock4.AC_MissionTimestamp = Aircraft.MissionRxTimestamp;
      Telemetry.Tel20HzBlock4.AC_MissionCRC = Aircraft.AircraftMissionCRC;
      Telemetry.Tel20HzBlock4.AC_Mission = Aircraft.RawMission;

      Telemetry.Tel20HzBlock4.HOBS_Mode = SS_HOBS.Status.Mode;
      Telemetry.Tel20HzBlock4.HOBS_BurstHeight = SS_HOBS.Status.BurstHeight;
      Telemetry.Tel20HzBlock4.HOBS_SBIT.DWord = SS_HOBS.Status.SBIT.DWord;
      Telemetry.Tel20HzBlock4.HOBS_CBIT.DWord = SS_HOBS.Status.CBIT.DWord;
      Telemetry.Tel20HzBlock4.HOBS_PSU_10V = SS_HOBS.Status.PSU_10V;
      Telemetry.Tel20HzBlock4.HOBS_PSU_5V = SS_HOBS.Status.PSU_5V;
      Telemetry.Tel20HzBlock4.HOBS_CoLocationID = SS_HOBS.Status.CoLocationID;
      Telemetry.Tel20HzBlock4.HOBS_EstImpactAngle = SS_HOBS.Status.EstImpactAngle;
      Telemetry.Tel20HzBlock4.HOBS_OutputDetectorLevel = SS_HOBS.Status.OutputDetectorLevel;
      Telemetry.Tel20HzBlock4.HOBS_PSU_PCB__Temp = SS_HOBS.Status.PSU_PCB__Temp;
      Telemetry.Tel20HzBlock4.HOBS_Status_TestSettings = SS_HOBS.Status.TestSettings;
      Telemetry.Tel20HzBlock4.HOBS_Track_TestSettings = SS_HOBS.Track.TestSettings;
      Telemetry.Tel20HzBlock4.HOBS_TrackDetectorHeight = SS_HOBS.Track.DetectorHeight;
      Telemetry.Tel20HzBlock4.HOBS_TrackGPW_HeightSetting = SS_HOBS.Track.GPW_HeightSetting;
      Telemetry.Tel20HzBlock4.HOBS_TrackHeight = SS_HOBS.Track.TrackHeight;
      Telemetry.Tel20HzBlock4.HOBS_TrackSpeed = SS_HOBS.Track.TrackSpeed;
      Telemetry.Tel20HzBlock4.HOBS_TrackTimestamp = SS_HOBS.Track.TimeStamp;
      Telemetry.Tel20HzBlock4.HOBS_WarningSignalWaveform = SS_HOBS.Status.WarningSignalWaveform;
      Telemetry.Tel20HzBlock4.HOBS_WeaponSpeed = SS_HOBS.Status.WeaponSpeed;
      Telemetry.Tel20HzBlock4.HOBS_PSU_InputVoltage = SS_HOBS.Status.PSU_InputVoltage;
 /*
      Telemetry.Tel20HzBlock4.HOBS_State = HOBS.Status.StatusData.State;
      Telemetry.Tel20HzBlock4.HOBS_DetHeight = HOBS.Status.StatusData.DetHeight;
      Telemetry.Tel20HzBlock4.HOBS_WeaponId = HOBS.Status.StatusData.WeaponId;
      Telemetry.Tel20HzBlock4.HOBS_CBIT = HOBS.Status.StatusData.CBIT;
      Telemetry.Tel20HzBlock4.HOBS_PSU_10V = HOBS.Status.StatusData.PSU_10V;
      Telemetry.Tel20HzBlock4.HOBS_PSU_5V = HOBS.Status.StatusData.PSU_5V;
      Telemetry.Tel20HzBlock4.HOBS_DetectorPower = HOBS.Status.StatusData.DetectorPower;
      Telemetry.Tel20HzBlock4.HOBS_OutputPulse = HOBS.Status.StatusData.OutputPulse;
      Telemetry.Tel20HzBlock4.HOBS_OutputPulseCnt = HOBS.Status.StatusData.OutputPulseCnt;
      Telemetry.Tel20HzBlock4.HOBS_ClosingVelocity = HOBS.Status.StatusData.ClosingVelocity;
      Telemetry.Tel20HzBlock4.HOBS_TimeToDet = HOBS.Status.StatusData.TimeToDet;
      Telemetry.Tel20HzBlock4.HOBS_TrackGroundHeight = HOBS.Status.StatusData.TrackGroundHeight;
      Telemetry.Tel20HzBlock4.HOBS_InstantaneousGroundHeight = HOBS.Status.StatusData.InstantaneousGroundHeight;
      Telemetry.Tel20HzBlock4.HOBS_NumberOfDetections = HOBS.Status.StatusData.NumberOfDetections;
      Telemetry.Tel20HzBlock4.HOBS_Timestamp = HOBS.Status.StatusData.HOBS_Time;
      Telemetry.Tel20HzBlock4.HOBS_DetectionTimestamp = HOBS.Status.StatusData.DetectionTimestamp;
      Telemetry.Tel20HzBlock4.HOBS_MCP_SystemTime = HOBS.Status.StatusData.MCP_SystemTime;
*/
    } break;
    case 4 : {
      Telemetry.Tel20HzBlock5.WeaponCBIT = Weapon.CBIT.DWord;
      Telemetry.Tel20HzBlock5.WeaponNodesOnNet = MLAN.NodesOnNetwork;
      Telemetry.Tel20HzBlock5.WeaponReconCnt = MLAN.ReconCnt;
      Telemetry.Tel20HzBlock5.WL_CommsErrCnt = WiFi.ErrCnt;
      Telemetry.Tel20HzBlock5.ServoCommsErrCnt = Servo.CommsErrorCnt;
      if(WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
      {
    	  if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	  {
    		  Telemetry.Tel20HzBlock5.SeekerCommsErrCnt = SAL_SS.RxCounters.RxTimeoutErrCnt+SAL_SS.RxCounters.RxMsgIdErrCnt+SAL_SS.RxCounters.RxMsgSizeErrCnt+SAL_SS.RxCounters.RxCrcErrCnt;
    	  }
    	  else if(AUXCTRL_SS.MAINControls.QSAL_On)
    	  {
    		  Telemetry.Tel20HzBlock5.SeekerCommsErrCnt = QSAL_SS.RxCounters.RxTimeoutErrCnt+ QSAL_SS.RxCounters.RxCrcErrCnt+ QSAL_SS.RxCounters.RxMsgIdErrCnt+ QSAL_SS.RxCounters.RxMsgSizeErrCnt;
    	  }
      }
      Telemetry.Tel20HzBlock5.BatteryPackCommsErrCnt = BatteryPack.ErrCnt;
      Telemetry.Tel20HzBlock5.HOBS_CommsErrCnt = SS_HOBS.Counters.ErrCnt;
      Telemetry.Tel20HzBlock5.MCP_CBIT = MCP.CBIT.Word;
      Telemetry.Tel20HzBlock5.PSU_5V = MCP.PSU_5V * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V = MCP.PSU_1V * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V35 = MCP.PSU_1V35 * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V8 = MCP.PSU_1V8 * 1000;
      Telemetry.Tel20HzBlock5.PSU_3V3 = MCP.PSU_3V3 * 1000;
      Telemetry.Tel20HzBlock5.IF_SW_Version = SS_FUZE.Fuze.SW_Verion;
      Telemetry.Tel20HzBlock5.IF_FuzeStatus = SS_FUZE.Fuze.Status.Word;
      Telemetry.Tel20HzBlock5.IF_StatusRxCnt = SS_FUZE.Fuze.StatusRxCnt;
      Telemetry.Tel20HzBlock5.IF_ACK_RxCnt = SS_FUZE.Fuze.ACK_RxCnt;
      Telemetry.Tel20HzBlock5.IF_NACK_RxCnt = SS_FUZE.Fuze.NACK_RxCnt;
      Telemetry.Tel20HzBlock5.IF_SetDelayTxCnt = SS_FUZE.Fuze.SetDelayTxCnt;
      Telemetry.Tel20HzBlock5.IF_RequestStatusTxCnt = SS_FUZE.Fuze.RequestStatusTxCnt;
      Telemetry.Tel20HzBlock5.IF_FuzeType = SS_FUZE.Fuze.Type;
      Telemetry.Tel20HzBlock5.IF_FuzeMode = SS_FUZE.Fuze.Mode;
      Telemetry.Tel20HzBlock5.IF_FuzeDelay = SS_FUZE.Fuze.FuzeTimeDelay;
      Telemetry.Tel20HzBlock5.IF_UpdatingFuze = SS_FUZE.Fuze.Updating;
      Telemetry.Tel20HzBlock5.MCP_Temperature = MCP.Temperature;
      Telemetry.Tel20HzBlock5.GNSS_GPS_AveSatStrength = UBLOX.GPS_AveSignalStrenth * 10;
      Telemetry.Tel20HzBlock5.GNSS_GLO_AveSatStrength = UBLOX.GLO_AveSignalStrenth * 10;
      Telemetry.Tel20HzBlock5.GNSS_GPS_Used = !Weapon.GNSS_Usage.GPS_NotUsed;
      Telemetry.Tel20HzBlock5.GNSS_GLO_Used = !Weapon.GNSS_Usage.GlonassNotUsed;
      Telemetry.Tel20HzBlock5.GNSS_GPS_SatUsedCnt = UBLOX.GPS_SatUsedCnt;
      Telemetry.Tel20HzBlock5.GNSS_GLO_SatUsedCnt = UBLOX.GLO_SatUsedCnt;

      if(WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
      {
    	  if (AUXCTRL_SS.MAINControls.MPSAL_On)
    	  {
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_CBIT = SAL_SS.Detection.CBITResults;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_DesignatorCode = SAL_SS.Detection.UsedDesignatorCode;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_DesignatorType = SAL_SS.Parameters.DesignatorType;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_FrameCount = SAL_SS.Detection.FrameCount;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_LaserJitter = SAL_SS.Parameters.LaserJitter;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_PRFaccuracy = SAL_SS.Parameters.PRFaccuracy;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_SBIT = SAL_SS.SBIT.PBITResults;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_SeekerPitch = MPSAL_TO_MRAD((float)SAL_SS.Detection.SpotElevation);
//              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_SeekerRoll = SAL_SS.Detection.SpotElevation;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_SeekerYaw =  MPSAL_TO_MRAD((float)SAL_SS.Detection.SpotAzimuth);
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_SensorTempValue = SAL_SS.PFBIT.SensorTempValue;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_TimeStamp = SAL_SS.Detection.TimeStamp;
              Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.MPSAL_DetectionStatus=(SAL_SS.Detection.DetectionStatus<<2)|(SAL_SS.Detection.DetectionStatus<<3);

    	  }
    	  else if(AUXCTRL_SS.MAINControls.QSAL_On)
    	  {
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_RangeCmd 		 = QSAL_SS.SharedData.RangeCmd;
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_PRIaccuracy_us  = (QSAL_SS.SharedData.AccuracyCmd/1000);
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_CBIT   		 = QSAL_SS.SharedData.CBIT;
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_DetectionStatus = (((unsigned short)QSAL_SS.SharedData.Status.Bits.TargetPulse<<2)|((unsigned short)QSAL_SS.SharedData.Status.Bits.AnglesValid<<3));
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_PRI 			 = QSAL_SS.SharedData.EstPRI;
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_PRIJitter_ns    = QSAL_SS.SharedData.JitterCmd;
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_TargetYaw       = (short)(QSAL_SS.SharedData.TargetYaw*1000.0);
    		  Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_TargetPitch     = (short)(QSAL_SS.SharedData.TargetPitch*1000.0);
    		  //          Telemetry.Tel20HzBlock5.SALSeeker_TelInfo.QSAL_RxCount         = QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt;
    	  }
      }


 /*
      Telemetry.Tel20HzBlock5.SeekerRangeCmd = SAL_Seeker.RangeCmd;
      Telemetry.Tel20HzBlock5.SeekerAccuracyCmd = SAL_Seeker.AccuracyCmd;
      Telemetry.Tel20HzBlock5.SeekerJitterCmd = SAL_Seeker.JitterCmd;
      Telemetry.Tel20HzBlock5.SeekerCBIT = SAL_Seeker.CBIT.DWord;
      Telemetry.Tel20HzBlock5.SeekerStatus = SAL_Seeker.Status.Word;
      Telemetry.Tel20HzBlock5.SeekerProcTemperature = SAL_Seeker.ProcessorTemperature;
      Telemetry.Tel20HzBlock5.SeekerDetTemperature = SAL_Seeker.DetectorTemperature;
      Telemetry.Tel20HzBlock5.SeekerTargetSNR = SAL_Seeker.TargetSNR;
      Telemetry.Tel20HzBlock5.SeekerTargetYaw = SAL_Seeker.TargetYaw * 65536.0;
      Telemetry.Tel20HzBlock5.SeekerTargetPitch = SAL_Seeker.TargetPitch * 65536.0;
      Telemetry.Tel20HzBlock5.SeekerEstPRI = SAL_Seeker.EstPRI;
      Telemetry.Tel20HzBlock5.SeekerMeasureTime = SAL_Seeker.DetectionTime * 1000000;
*/
      Telemetry.Tel20HzBlock5.WL_ArcnetErrorCnt = WiFi.ArcnetRxErrCnt;
      Telemetry.Tel20HzBlock5.WL_TabletErrorCnt = WiFi.WiFiRxErrCnt;
      Telemetry.Tel20HzBlock5.WL_CBIT = WiFi.CBIT.Word;
      Telemetry.Tel20HzBlock5.WL_StatusFlags = WiFi.Settings.Byte;
      Telemetry.Tel20HzBlock5.WL_MyReconCnt = WiFi.ArcnetMyReconCnt;
      Telemetry.Tel20HzBlock5.WL_RSSI = WiFi.RSSI;
      Telemetry.Tel20HzBlock5.WL_Temperature = WiFi.Temperature;
      Telemetry.Tel20HzBlock5.WL_State = WiFi.WiFiModuleState;
      Telemetry.Tel20HzBlock5.WL_WeaponId = WiFi.WeaponId;
      Telemetry.Tel20HzBlock5.WL_PIC_3V3 = WiFi.PIC_3V3;
      Telemetry.Tel20HzBlock5.WL_Main3V3 = WiFi.Main3V3;
      Telemetry.Tel20HzBlock5.WL_RxCnt = WiFi.RxCnt;
      Telemetry.Tel20HzBlock5.BAT_HeaterCtrl = BatteryPack.HeaterCtrl;
      Telemetry.Tel20HzBlock5.BAT_TempSetPoint = BatteryPack.TemperatureSetPoint;
      Telemetry.Tel20HzBlock5.BAT_Capacity = BatteryPack.RawCapacity;
      Telemetry.Tel20HzBlock5.BAT_CapacityUsed = BatteryPack.RawCumulativeUsage;
      Telemetry.Tel20HzBlock5.BAT_Activations = BatteryPack.ActivationCnt;
      Telemetry.Tel20HzBlock5.BAT_Temp1 = BatteryPack.Temperature1;
      Telemetry.Tel20HzBlock5.BAT_Temp2 = BatteryPack.Temperature2;
      Telemetry.Tel20HzBlock5.BAT_Temp3 = BatteryPack.Temperature3;
      Telemetry.Tel20HzBlock5.BAT_Temp4 = BatteryPack.Temperature4;
      Telemetry.Tel20HzBlock5.BAT_Temp5 = BatteryPack.Temperature5;
      Telemetry.Tel20HzBlock5.BAT_Temp6 = BatteryPack.Temperature6;
      Telemetry.Tel20HzBlock5.BAT_Temp7 = BatteryPack.Temperature7;
      Telemetry.Tel20HzBlock5.BAT_Temp8 = BatteryPack.Temperature8;
      Telemetry.Tel20HzBlock5.BAT_Current1 = BatteryPack.RawAveCurrent_0_10ms;
      Telemetry.Tel20HzBlock5.BAT_Current2 = BatteryPack.RawAveCurrent_10_20ms;
      Telemetry.Tel20HzBlock5.BAT_Current3 = BatteryPack.RawAveCurrent_20_30ms;
      Telemetry.Tel20HzBlock5.BAT_Current4 = BatteryPack.RawAveCurrent_30_40ms;
      Telemetry.Tel20HzBlock5.BAT_Current5 = BatteryPack.RawAveCurrent_40_50ms;
      Telemetry.Tel20HzBlock5.PressSensorUsed = MCP.PressureSensToUse;
      Telemetry.Tel20HzBlock5.PressSensTimestamp = MCP.PressSensTimestamp / 1000;
      Telemetry.Tel20HzBlock5.PressSensPressure = MCP.PressSensPressure;
      Telemetry.Tel20HzBlock5.PressSensTemperature = MCP.PressSensTemperature;
//      Telemetry.Tel20HzBlock5.SeekerRxCnt = (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) ? SAL_Seeker.RxCnt : 0;
      if(WeaponSettings.WeaponConfig.SeekerType == seekerSAL)
      {

    	  if(AUXCTRL_SS.MAINControls.MPSAL_On)
    	  {
    		  Telemetry.Tel20HzBlock5.SeekerRxCnt = SAL_SS.RxCounters.RxMessageCnt;
    	  }
    	  else if(AUXCTRL_SS.MAINControls.QSAL_On)
    	  {
    		  Telemetry.Tel20HzBlock5.SeekerRxCnt = QSAL_SS.RxCounters.RxStatusMessageRxByMcpCnt;
    	  }
    	  else
    	  {
    		  Telemetry.Tel20HzBlock5.SeekerRxCnt=0;
    	  }
      }
      Telemetry.Tel20HzBlock5.BatteryPackRxCnt = BatteryPack.RxCnt;
      Telemetry.Tel20HzBlock5.ServoRxCnt = Servo.RxCnt;
      Telemetry.Tel20HzBlock5.HOBS_RxCnt = SS_HOBS.Counters.RxMessageCnt;
      Telemetry.Tel20HzBlock5.UBLOX_RxCnt = UBLOX.RxCnt;
      Telemetry.Tel20HzBlock5.DebugInt1 = MCP.DebugInt[0];
      Telemetry.Tel20HzBlock5.PressSensPressure1 = (int)PressureSensor1.Pressure;
      Telemetry.Tel20HzBlock5.PressSensPressure2 = (int)PressureSensor2.Pressure;
      Telemetry.Tel20HzBlock5.PressSensTemperature2 = (int)PressureSensor2.Temperature;
      Telemetry.Tel20HzBlock5.PressSensTemperature1 = (int)PressureSensor1.Temperature;
    } break;
    case 5 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 6 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 7 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 8 : {
      switch (Tel2HzIndex) {
        case 0 : Telemetry.Tel20HzBlock9.Tel2HzBlock1 = Tel2HzBlock1; break;
        case 1 : Telemetry.Tel20HzBlock9.Tel2HzBlock2 = Tel2HzBlock2; break;
        case 2 : Telemetry.Tel20HzBlock9.Tel2HzBlock3 = Tel2HzBlock3; break;
        case 3 : Telemetry.Tel20HzBlock9.Tel2HzBlock4 = Tel2HzBlock4; break;
        case 4 : Telemetry.Tel20HzBlock9.Tel2HzBlock5 = Tel2HzBlock5; break;
        case 5 : Telemetry.Tel20HzBlock9.Tel2HzBlock6 = Tel2HzBlock6; break;
        case 6 : Telemetry.Tel20HzBlock9.Tel2HzBlock7 = Tel2HzBlock7; break;
        case 7 : Telemetry.Tel20HzBlock9.Tel2HzBlock8 = Tel2HzBlock8; break;
        case 8 : Telemetry.Tel20HzBlock9.Tel2HzBlock9 = Tel2HzBlock9; break;
        case 9 : Telemetry.Tel20HzBlock9.Tel2HzBlock10 = Tel2HzBlock10; break;
        default : Tel2HzIndex = 0; break;
      }

      Telemetry.Tel20HzBlock9.ConstID = 0x91 + Tel2HzIndex;
      Telemetry.Tel20HzBlock9.Tel2HzIndex = Tel2HzIndex;

      Tel2HzIndex++;

      if (Tel2HzIndex > 9) {
        Tel2HzIndex = 0;
      }
    } break;
    case  9 : {
      if (WeaponSettings.WeaponConfig.SeekerType == seekerSAL) {
        memcpy(Telemetry.Tel20HzBlock10.Bytes, &SAL_Seeker.Telemetry[0], sizeof(sTel20Hz));
        memcpy(&SeekerTelBuf2, &SAL_Seeker.Telemetry[1], sizeof(sSAL_SeekerTel));
        memcpy(&SeekerTelBuf3, &SAL_Seeker.Telemetry[2], sizeof(sSAL_SeekerTel));
        memcpy(&SeekerTelBuf4, &SAL_Seeker.Telemetry[3], sizeof(sSAL_SeekerTel));
        memcpy(&SeekerTelBuf5, &SAL_Seeker.Telemetry[4], sizeof(sSAL_SeekerTel));
        memcpy(&SeekerTelBuf6, &SAL_Seeker.Telemetry[5], sizeof(sSAL_SeekerTel));
      }
    } break;
    case 10 : memcpy(Telemetry.Tel20HzBlock11.Bytes, &SeekerTelBuf2, sizeof(sTel20Hz)); break;
    case 11 : memcpy(Telemetry.Tel20HzBlock12.Bytes, &SeekerTelBuf3, sizeof(sTel20Hz)); break;
    case 12 : memcpy(Telemetry.Tel20HzBlock13.Bytes, &SeekerTelBuf4, sizeof(sTel20Hz)); break;
    case 13 : memcpy(Telemetry.Tel20HzBlock14.Bytes, &SeekerTelBuf5, sizeof(sTel20Hz)); break;
    case 14 : memcpy(Telemetry.Tel20HzBlock15.Bytes, &SeekerTelBuf6, sizeof(sTel20Hz)); break;
  }

  Telemetry.Tel600HzAnd300HzCRC = Calculate16BitCRC((unsigned char *) &Telemetry.Tel600Hz_1, sizeof(uTel600Hz) * 2 + sizeof(uTel300Hz));
  Telemetry.Tel100Hz.CRC = Calculate16BitCRC(&Telemetry.Tel100Hz.Bytes[2], sizeof(sTel100Hz) - 2);

  // 20 Hz data CRC (not SAL seeker tel)
  if (Telemetry.Tel20HzIndex < 9) {
    Telemetry.Tel20Hz.CRC = Calculate16BitCRC(&Telemetry.Tel20Hz.Bytes[2], sizeof(sTel20Hz) - 2);
  }

  //Telemetry.CRC32 = Calculate32BitCRC((unsigned char *) &Telemetry.Tel100HzIndex, sizeof(sTelemetry) - 4);
  Telemetry.CRC32 = Calculate32BitCRC((unsigned char *) &Telemetry.Tel600Hz_1, sizeof(sTelemetry) - 8);

  TelemetryHeader.Opcode = 0xC0 + Telemetry.Tel100HzIndex;
  TelemetryHeader.FrameCnt++;

  tel_msg.Header = TelemetryHeader;
  tel_msg.Telemetry = Telemetry;

  MLAN_Send(MLAN_TEL_NODE_ID, (unsigned char *) &tel_msg, sizeof(sTelemetryMsg));

  //if (MLAN_IsNodeOnNet(MLAN_ETE_NODE_ID)) {
  //  MLAN_Send(MLAN_ETE_NODE_ID, (unsigned char *) &tel_msg, sizeof(sTelemetryMsg));
  //}
}

void TelUseFixedData(unsigned int FixedTelId)
{
  FixedTelemetry = FixedTelId;

  if (FixedTelemetry == TEL_FIXED_ID) {
    TelFix2HzData();
  }
}

void TelSendFixedTelemetry(unsigned char Tel100HzIndex)
{
  sTelemetryMsg tel_msg;


  Telemetry.Tel100HzIndex = Tel100HzIndex;
  Telemetry.Tel20HzIndex++;

  if (Telemetry.Tel20HzIndex > 14) {
    Telemetry.Tel20HzIndex = Telemetry.Tel100HzIndex;
  }

  Telemetry.Tel600Hz_1.IMU_AngularRateX = DEG2RAD(100.0) / 0.00457763671875;
  Telemetry.Tel600Hz_1.IMU_AngularRateY = DEG2RAD(-200.0) / 0.0011444091796875;
  Telemetry.Tel600Hz_1.IMU_AngularRateZ = DEG2RAD(300.0) / 0.0011444091796875;
  Telemetry.Tel600Hz_1.IMU_AccelerationX = (500 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_1.IMU_AccelerationY = (-1000.0 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_1.IMU_AccelerationZ = (1500.0 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_1.IMU_RxCnt = 799;
  Telemetry.Tel600Hz_1.IMU_ErrorCnt = 5;

  Telemetry.Tel600Hz_2.IMU_AngularRateX = DEG2RAD(1.0) / 0.00457763671875;
  Telemetry.Tel600Hz_2.IMU_AngularRateY = DEG2RAD(-2.0) / 0.0011444091796875;
  Telemetry.Tel600Hz_2.IMU_AngularRateZ = DEG2RAD(3.0) / 0.0011444091796875;
  Telemetry.Tel600Hz_2.IMU_AccelerationX = (50.0 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_2.IMU_AccelerationY = (-100.0 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_2.IMU_AccelerationZ = (150.0 * 9.81) / (0.02232421875 * 1000);
  Telemetry.Tel600Hz_2.IMU_RxCnt = 800;
  Telemetry.Tel600Hz_2.IMU_ErrorCnt = 6;

  Telemetry.Tel300Hz.WeaponTime = 111222333;
  Telemetry.Tel300Hz.IMU_Cycle = 5;
  Telemetry.Tel300Hz.Task600HzExecutionTime = 500;
  Telemetry.Tel300Hz.Task100HzExecutionTime = 1000;
  Telemetry.Tel300Hz.Task600HzOverrunCnt = 1;
  Telemetry.Tel300Hz.Task100HzOverrunCnt = 2;
  Telemetry.Tel300Hz.Task600HzCount = 20000;
  Telemetry.Tel300Hz.Task100HzCount = 25000;
  Telemetry.Tel300Hz.Task2HzCount = 30000;
  Telemetry.Tel300Hz.WeaponStatus = 0x55667788;
  Telemetry.Tel300Hz.WeaponDiscretes = 123;
  Telemetry.Tel300Hz.WeaponFlags = 0x4321;
  Telemetry.Tel300Hz.SV_DeflCmd1 = 1.2 * 1024;
  Telemetry.Tel300Hz.SV_DeflCmd2 = -2.4 * 1024;
  Telemetry.Tel300Hz.SV_DeflCmd3 = 3.6 * 1024;
  Telemetry.Tel300Hz.SV_DeflCmd4 = -4.8 * 1024;
  Telemetry.Tel300Hz.SV_Feedback1 = 11.2 * 1024;
  Telemetry.Tel300Hz.SV_Feedback2 = -12.4 * 1024;
  Telemetry.Tel300Hz.SV_Feedback3 = 13.6 * 1024;
  Telemetry.Tel300Hz.SV_Feedback4 = -14.8 * 1024;
  Telemetry.Tel300Hz.SV_Current1 = -1.5 * 10;
  Telemetry.Tel300Hz.SV_Current2 = 1.4 * 10;
  Telemetry.Tel300Hz.SV_Current3 = -1.3 * 10;
  Telemetry.Tel300Hz.SV_Current4 = 1.2 * 10;
  Telemetry.Tel300Hz.FrameCnt = 32;

  // 100 Hz Tel
  switch (Telemetry.Tel100HzIndex) {
    case 0 : {
      Telemetry.Tel100HzBlock1.Weapon100HzTimestamp = 1100;
    } break;
    case 1 : {
      Telemetry.Tel100HzBlock2.Weapon100HzTimestamp = 2200;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleX = DEG2RAD(10.0) * 1073741824.0 * 0.01;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleY = DEG2RAD(-20.0) * 4294967296.0 * 0.01;
      Telemetry.Tel100HzBlock2.IMU_DeltaAngleZ = DEG2RAD(30.0) * 4294967296.0 * 0.01;
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityX = 2000.0 * (67108864.0 * 9.81) / (0.3048 * 1000.0 * 100.0);
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityY = -3000.0 * (67108864.0 * 9.81) / (0.3048 * 1000.0 * 100.0);
      Telemetry.Tel100HzBlock2.IMU_DeltaVelocityZ = 4000.0 * (67108864.0 * 9.81) / (0.3048 * 1000.0 * 100.0);
      Telemetry.Tel100HzBlock2.NAV_SystemTime = 360000000;
      Telemetry.Tel100HzBlock2.NAV_baro_SystemTime = 410000000;
      Telemetry.Tel100HzBlock2.NAV_baro_alt = -154;
      Telemetry.Tel100HzBlock2.NAV_baro_pressure = 30000;
      Telemetry.Tel100HzBlock2.NAV_Reserved1 = 213;
      Telemetry.Tel100HzBlock2.NAV_Reserved2 = 214;
      Telemetry.Tel100HzBlock2.NAV_100HzTimestamp = 360000000;
      Telemetry.Tel100HzBlock2.NAV_100HzCount = 480983;
      Telemetry.Tel100HzBlock2.NAV_AngPsi = 5.60810940580071;
      Telemetry.Tel100HzBlock2.NAV_AngTheta = -1.551248639;
      Telemetry.Tel100HzBlock2.NAV_AngPhi = 2.148901735;
      Telemetry.Tel100HzBlock2.NAV_PosXecef = -5000 * 256;
      Telemetry.Tel100HzBlock2.NAV_PosYecef = -6000 * 256;
      Telemetry.Tel100HzBlock2.NAV_PosZecef = -7000 * 256;
      Telemetry.Tel100HzBlock2.NAV_VelXecef = 100.1;
      Telemetry.Tel100HzBlock2.NAV_VelYecef = -200.2;
      Telemetry.Tel100HzBlock2.NAV_VelZecef = 300.3;
      Telemetry.Tel100HzBlock2.NAV_LatLLA = 1.35747695;
      Telemetry.Tel100HzBlock2.NAV_LongLLA = 2.908880923;
      Telemetry.Tel100HzBlock2.NAV_AltLLA = 11111.11;
      Telemetry.Tel100HzBlock2.NAV_PressAlt = 9850;
      Telemetry.Tel100HzBlock2.NAV_Mode = 20;
    } break;
    case 2 : {
      Telemetry.Tel100HzBlock3.Weapon100HzTimestamp = 3300;
      Telemetry.Tel100HzBlock3.NAV_Align_Timestamp = 35000800;
      Telemetry.Tel100HzBlock3.NAV_Align_CallCount = 8456;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_x = 5.608109406;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_y = -1.551248639;
      Telemetry.Tel100HzBlock3.NAV_Align_pos_z = 2.148901735;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_x = 5123000;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_y = 6123000;
      Telemetry.Tel100HzBlock3.NAV_Align_vel_z = 7123000;
      Telemetry.Tel100HzBlock3.NAV_Align_phi = -120.1212;
      Telemetry.Tel100HzBlock3.NAV_Align_the = 60.60606;
      Telemetry.Tel100HzBlock3.NAV_Align_psi = 180.1818;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_pos = 1;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_vel = 1;
      Telemetry.Tel100HzBlock3.NAV_Align_valid_euler = 0;
      Telemetry.Tel100HzBlock3.NAV_Reserved1 = 323;
    } break;
  }

  // 20 Hz Tel
  switch (Telemetry.Tel20HzIndex) {
    case 0 : {

    } break;
    case 1 : {

    } break;
    case 2 : {

    } break;
    case 3 : {

    } break;
    case 4 : {
      Telemetry.Tel20HzBlock5.WeaponCBIT = 0x11223344;
      Telemetry.Tel20HzBlock5.WeaponNodesOnNet = 0xADCB;
      Telemetry.Tel20HzBlock5.WeaponReconCnt = 7777;
      Telemetry.Tel20HzBlock5.ServoCommsErrCnt = 100;
      Telemetry.Tel20HzBlock5.WL_CommsErrCnt = 200;
      Telemetry.Tel20HzBlock5.SeekerCommsErrCnt = 300;
      Telemetry.Tel20HzBlock5.UBLOX_ErrCnt = 400;
      Telemetry.Tel20HzBlock5.BatteryPackCommsErrCnt = 600;
      Telemetry.Tel20HzBlock5.HOBS_CommsErrCnt = 700;
      Telemetry.Tel20HzBlock5.MCP_CBIT = MCP.CBIT.Word = 0x5D01;
      Telemetry.Tel20HzBlock5.PSU_5V = 9.5 * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V = 2.44 * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V35 = 4.8 * 1000;
      Telemetry.Tel20HzBlock5.PSU_1V8 = 5.67 * 1000;
      Telemetry.Tel20HzBlock5.PSU_3V3 = 2.22 * 1000;
      Telemetry.Tel20HzBlock5.MCP_Temperature = 23;
      Telemetry.Tel20HzBlock5.GNSS_GPS_AveSatStrength = 402;
      Telemetry.Tel20HzBlock5.GNSS_GLO_AveSatStrength = 376;
      Telemetry.Tel20HzBlock5.GNSS_GPS_Used = 1;
      Telemetry.Tel20HzBlock5.GNSS_GLO_Used = 1;
      Telemetry.Tel20HzBlock5.GNSS_GPS_SatUsedCnt = 12;
      Telemetry.Tel20HzBlock5.GNSS_GLO_SatUsedCnt = 9;
      /*Telemetry.Tel20HzBlock5.SeekerCBIT = 0x48100C11;
      Telemetry.Tel20HzBlock5.SeekerStatus = 0x0251;
      Telemetry.Tel20HzBlock5.SeekerProcTemperature = 11;
      Telemetry.Tel20HzBlock5.SeekerDetTemperature = 22;
      Telemetry.Tel20HzBlock5.SeekerTargetSNR = 4440;
      Telemetry.Tel20HzBlock5.SeekerTargetYaw = -0.097738438 * 65536.0;
      Telemetry.Tel20HzBlock5.SeekerTargetPitch = 0.11693706 * 65536.0;
      Telemetry.Tel20HzBlock5.SeekerEstPRI = 0.000089102 * 1E+9;
      Telemetry.Tel20HzBlock5.SeekerMeasureTime = 5000321;*/
      Telemetry.Tel20HzBlock5.WL_ArcnetErrorCnt = 45001;
      Telemetry.Tel20HzBlock5.WL_TabletErrorCnt = 10054;
      Telemetry.Tel20HzBlock5.WL_CBIT = 0xB218;
      Telemetry.Tel20HzBlock5.WL_StatusFlags = 0x03;
      Telemetry.Tel20HzBlock5.WL_MyReconCnt =  220;
      Telemetry.Tel20HzBlock5.WL_RSSI = 60;
      Telemetry.Tel20HzBlock5.WL_Temperature = -38;
      Telemetry.Tel20HzBlock5.WL_State = 4;
      Telemetry.Tel20HzBlock5.WL_WeaponId = 2;
      Telemetry.Tel20HzBlock5.WL_PIC_3V3 = 3188;
      Telemetry.Tel20HzBlock5.WL_Main3V3 = 3412;
      Telemetry.Tel20HzBlock5.ServoRxCnt = 870;
      Telemetry.Tel20HzBlock5.WL_RxCnt = 871;
      Telemetry.Tel20HzBlock5.SeekerRxCnt = 872;
      Telemetry.Tel20HzBlock5.ITU_RxCnt = 873;
      Telemetry.Tel20HzBlock5.PitotRxCnt = 874;
      Telemetry.Tel20HzBlock5.BatteryPackRxCnt = 875;
      Telemetry.Tel20HzBlock5.HOBS_RxCnt = 876;
      Telemetry.Tel20HzBlock5.UBLOX_RxCnt = 877;
    } break;
    case 5 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock6.Bytes, (void *) &UBLOX_GNSS_20HzBlock1.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 6 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock7.Bytes, (void *) &UBLOX_GNSS_20HzBlock2.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 7 : {
      switch (Tel2HzIndex) {
        case 0 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot1, sizeof(sTel20Hz)); break;
        case 1 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot2, sizeof(sTel20Hz)); break;
        case 2 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot3, sizeof(sTel20Hz)); break;
        case 3 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot4, sizeof(sTel20Hz)); break;
        case 4 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot5, sizeof(sTel20Hz)); break;
        case 5 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot6, sizeof(sTel20Hz)); break;
        case 6 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot7, sizeof(sTel20Hz)); break;
        case 7 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot8, sizeof(sTel20Hz)); break;
        case 8 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot9, sizeof(sTel20Hz)); break;
        case 9 : memcpy(Telemetry.Tel20HzBlock8.Bytes, (void *) &UBLOX_GNSS_20HzBlock3.Slot10, sizeof(sTel20Hz)); break;
        default : Tel2HzIndex = 0; break;
      }
    } break;
    case 8 : {
      switch (Tel2HzIndex) {
        case 0 : Telemetry.Tel20HzBlock9.Tel2HzBlock1 = Tel2HzBlock1; break;
        case 1 : Telemetry.Tel20HzBlock9.Tel2HzBlock2 = Tel2HzBlock2; break;
        case 2 : Telemetry.Tel20HzBlock9.Tel2HzBlock3 = Tel2HzBlock3; break;
        case 3 : Telemetry.Tel20HzBlock9.Tel2HzBlock4 = Tel2HzBlock4; break;
        case 4 : Telemetry.Tel20HzBlock9.Tel2HzBlock5 = Tel2HzBlock5; break;
        case 5 : Telemetry.Tel20HzBlock9.Tel2HzBlock6 = Tel2HzBlock6; break;
        case 6 : Telemetry.Tel20HzBlock9.Tel2HzBlock7 = Tel2HzBlock7; break;
        case 7 : Telemetry.Tel20HzBlock9.Tel2HzBlock8 = Tel2HzBlock8; break;
        case 8 : Telemetry.Tel20HzBlock9.Tel2HzBlock9 = Tel2HzBlock9; break;
        case 9 : Telemetry.Tel20HzBlock9.Tel2HzBlock10 = Tel2HzBlock10; break;
        default : Tel2HzIndex = 0; break;
      }

      Telemetry.Tel20HzBlock9.ConstID = 0x91 + Tel2HzIndex;
      Telemetry.Tel20HzBlock9.Tel2HzIndex = Tel2HzIndex;

      Tel2HzIndex++;

      if (Tel2HzIndex > 9) {
        Tel2HzIndex = 0;
      }
    } break;
  }

  Telemetry.Tel600HzAnd300HzCRC = Calculate16BitCRC((unsigned char *) &Telemetry.Tel600Hz_1, sizeof(uTel600Hz) * 2 + sizeof(uTel300Hz));
  Telemetry.Tel100Hz.CRC = Calculate16BitCRC(&Telemetry.Tel100Hz.Bytes[2], sizeof(sTel100Hz) - 2);
  Telemetry.Tel20Hz.CRC = Calculate16BitCRC(&Telemetry.Tel20Hz.Bytes[2], sizeof(sTel20Hz) - 2);
  Telemetry.CRC32 = Calculate32BitCRC((unsigned char *) &Telemetry.Tel100HzIndex, sizeof(sTelemetry) - 4);

  TelemetryHeader.Opcode = 0x80 + Telemetry.Tel100HzIndex;
  TelemetryHeader.FrameCnt++;

  tel_msg.Header = TelemetryHeader;
  tel_msg.Telemetry = Telemetry;

  MLAN_Send(MLAN_ETE_NODE_ID, (unsigned char *) &tel_msg, sizeof(sTelemetryMsg));
}

void TelFix2HzData(void)
{
  int index;


  Tel2HzBlock1.SeekerType = 2;
  Tel2HzBlock1.BombType = 1;
  Tel2HzBlock1.PowerCycleCnt = 200;
  Tel2HzBlock1.MCP_ETI = 10000;
  Tel2HzBlock1.WeaponSBIT = 0x4E9211B5;
  Tel2HzBlock1.MCP_SBIT = 0x9918;
  Tel2HzBlock1.WL_SBIT = 0x1847;
  Tel2HzBlock1.ITUC_SBIT = 0x5932;
  Tel2HzBlock1.HOBS_SBIT = 0x228D;
  Tel2HzBlock1.SeekerSBIT = 0x779C4C20;
  Tel2HzBlock1.ServoSBIT = 0x6DA10435;
  Tel2HzBlock1.PitotSBIT = 0x4F0731A9;
  Tel2HzBlock1.BatteryPackSBIT = 0x6A41;

  Tel2HzBlock2.RawMission.PriTgtLatitude = -0.0977384381116826 * (2147483648.0 / PI);
  Tel2HzBlock2.RawMission.PriTgtLongitude = 0.363028484 * (2147483648.0 / PI);
  Tel2HzBlock2.RawMission.PriTgtAltitude = 1000;
  Tel2HzBlock2.RawMission.AttackHeading = 2.617993878 * (2147483648.0 / PI);
  Tel2HzBlock2.RawMission.DiveAngle = 75;
  Tel2HzBlock2.RawMission.ImpactSpeed = 240;
  Tel2HzBlock2.RawMission.GNSS_Usage.Byte = 0;
  Tel2HzBlock2.RawMission.AttackHeadingValid = 0;
  Tel2HzBlock2.RawMission.SecondarySwitch = 1;
  Tel2HzBlock2.RawMission.SecTgtLatitude = 0.530580093 * (2147483648.0 / PI);
  Tel2HzBlock2.RawMission.SecTgtLongitude = -1.05068821 * (2147483648.0 / PI);
  Tel2HzBlock2.RawMission.SecTgtAltitude = 50;
  Tel2HzBlock2.RawMission.SAL_Code = 15850000;
  Tel2HzBlock2.RawMission.SAL_Activation = 0;
  Tel2HzBlock2.RawMission.SAL_Accuracy = 5000;
  Tel2HzBlock2.RawMission.SAL_Jitter = 1000;
  Tel2HzBlock2.RawMission.WeaponType = 0;
  Tel2HzBlock2.RawMission.MissionType = 1;
  Tel2HzBlock2.RawMission.CRC = 0x1234;
  Tel2HzBlock2.WindEntryCnt = 12;
  Tel2HzBlock2.WindCRC = 0x8888;

  for (index = 0; index < 8; index++) {
    Tel2HzBlock2.WindEntries[index].Altitude = 7000 + 100 * index;
    Tel2HzBlock2.WindEntries[index].Speed = 80 + index;
    Tel2HzBlock2.WindEntries[index].Direction = 120 + 10 * index;
  }

  Tel2HzBlock3.MCP_LogInfo.SW_Version = 101;
  Tel2HzBlock3.MCP_LogInfo.SW_CRC = 0x11;
  Tel2HzBlock3.MCP_LogInfo.FW_Version = 202;
  Tel2HzBlock3.MCP_LogInfo.FW_CRC = 0x22;
  strcpy(Tel2HzBlock3.MCP_LogInfo.PartNo, "3021110010");
  strcpy(Tel2HzBlock3.MCP_LogInfo.IssueNo, "001");
  strcpy(Tel2HzBlock3.MCP_LogInfo.SerialNo, "SERIAL01");
  Tel2HzBlock3.ServoLogInfo.SW_Version = 303;
  Tel2HzBlock3.ServoLogInfo.SW_CRC = 0x33;
  Tel2HzBlock3.ServoLogInfo.FW_Version = 404;
  Tel2HzBlock3.ServoLogInfo.FW_CRC = 0x44;
  strcpy(Tel2HzBlock3.ServoLogInfo.PartNo, "3021110020");
  strcpy(Tel2HzBlock3.ServoLogInfo.IssueNo, "002");
  strcpy(Tel2HzBlock3.ServoLogInfo.SerialNo, "SERIAL02");
  Tel2HzBlock3.WiFiLogInfo.SW_Version = 505;
  Tel2HzBlock3.WiFiLogInfo.SW_CRC = 0x55;
  Tel2HzBlock3.WiFiLogInfo.FW_Version = 606;
  Tel2HzBlock3.WiFiLogInfo.FW_CRC = 0x66;
  strcpy(Tel2HzBlock3.WiFiLogInfo.PartNo, "3021110030");
  strcpy(Tel2HzBlock3.WiFiLogInfo.IssueNo, "003");
  strcpy(Tel2HzBlock3.WiFiLogInfo.SerialNo, "SERIAL03");
  Tel2HzBlock3.ITU_LogInfo.SW_Version = 707;
  Tel2HzBlock3.ITU_LogInfo.SW_CRC = 0x77;
  Tel2HzBlock3.ITU_LogInfo.FW_Version = 808;
  Tel2HzBlock3.ITU_LogInfo.FW_CRC = 0x88;
  strcpy(Tel2HzBlock3.ITU_LogInfo.PartNo, "3021110040");
  strcpy(Tel2HzBlock3.ITU_LogInfo.IssueNo, "004");
  strcpy(Tel2HzBlock3.ITU_LogInfo.SerialNo, "SERIAL04");
  Tel2HzBlock3.SeekerLogInfo.SW_Version = 909;
  Tel2HzBlock3.SeekerLogInfo.SW_CRC = 0x99;
  Tel2HzBlock3.SeekerLogInfo.FW_Version = 1010;
  Tel2HzBlock3.SeekerLogInfo.FW_CRC = 0xAA;
  strcpy(Tel2HzBlock3.SeekerLogInfo.PartNo, "3021110050");
  strcpy(Tel2HzBlock3.SeekerLogInfo.IssueNo, "005");
  strcpy(Tel2HzBlock3.SeekerLogInfo.SerialNo, "SERIAL05");
  Tel2HzBlock3.BatteryPackLogInfo.SW_Version = 1111;
  Tel2HzBlock3.BatteryPackLogInfo.SW_CRC = 0xBB;
  Tel2HzBlock3.BatteryPackLogInfo.FW_Version = 1212;
  Tel2HzBlock3.BatteryPackLogInfo.FW_CRC = 0xCC;
  strcpy(Tel2HzBlock3.BatteryPackLogInfo.PartNo, "3021110060");
  strcpy(Tel2HzBlock3.BatteryPackLogInfo.IssueNo, "006");
  strcpy(Tel2HzBlock3.BatteryPackLogInfo.SerialNo, "SERIAL06");

  Tel2HzBlock4.GuidanceSVN_Revision = 1762;
  Tel2HzBlock4.PitotLogInfo.SW_Version = 1313;
  Tel2HzBlock4.PitotLogInfo.SW_CRC = 0xDD;
  Tel2HzBlock4.PitotLogInfo.FW_Version = 1414;
  Tel2HzBlock4.PitotLogInfo.FW_CRC = 0xEE;
  strcpy(Tel2HzBlock4.PitotLogInfo.PartNo, "4021110070");
  strcpy(Tel2HzBlock4.PitotLogInfo.IssueNo, "007");
  strcpy(Tel2HzBlock4.PitotLogInfo.SerialNo, "SERIAL07");
  Tel2HzBlock4.HOBS_LogInfo.SW_Version = 1515;
  Tel2HzBlock4.HOBS_LogInfo.SW_CRC = 0xFF;
  Tel2HzBlock4.HOBS_LogInfo.FW_Version = 1616;
  Tel2HzBlock4.HOBS_LogInfo.FW_CRC = 0x1111;
  strcpy(Tel2HzBlock4.HOBS_LogInfo.PartNo, "4021110080");
  strcpy(Tel2HzBlock4.HOBS_LogInfo.IssueNo, "008");
  strcpy(Tel2HzBlock4.HOBS_LogInfo.SerialNo, "SERIAL08");

  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_GeVecXecef = -1.2222;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_GeVecYecef = -0.9999;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_GeVecZecef = -0.2222;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_FeVecXecef = -7.7777;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_FeVecYecef = 6.6666;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_FeVecZecef = -14.4444;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe11 = -1;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe12 = 1;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe13 = 0.1;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe21 = 0.2;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe22 = -0.3;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe23 = 0.4;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe31 = 0.5;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe32 = -0.6;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_DCMbe33 = 0.7;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitXpos = -5000;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitYpos = -6000;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitZpos = -7000;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitXvel = 100.1;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitYvel = -200.2;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitZvel = 300.3;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitPhi = 321.321;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitTheta = -88.88;
  Tel2HzBlock5.NAV_Tel100at2Hz.NAV_InitPsi = 123.123;

  for (index = 0; index < 8; index++) {

  }
}

