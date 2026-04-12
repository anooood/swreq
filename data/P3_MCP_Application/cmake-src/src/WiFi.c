/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WiFi.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication between the Wireless Link and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - WiFiInit                            : Initializes WiFi interface
 *  - WiFiSend                            : Transmit health & status and SBIT messages to Wireless Link
 *  - WiFiSendClearCBIT                   : Transmits a 'Clear CBIT' command message to the Wireless Link
 *  - WiFiSendLogDataRequest              : Request the Data Log to the Wireless Link
 *  - WiFiSendMission                     : Sends the mission
 *  - WiFiSendWind                        : Sends the Wind
 *  - WiFiSendLogData                     : Sends the Log Data
 *  - WiFiHandleSBIT_Msg                  : Handles received Wireless Link SBIT message
 *  - WiFiHandleStatusMsg                 : Handles received Wireless Link Status message
 *  - WiFiHandleBatteryControl            : Handles received Wireless Link Battery Contol message
 *  - WiFiHandleGNSS_Usage                : Handles received Wireless Link GNSS Usage message
 *  - WiFiHandleClearCBIT_Msg             : Handles received Wireless Link Clear CBIT message
 *  - WiFiHandleClearMissionMsg           : Handles received Wireless Link Clear Mission message
 *  - WiFiHandleMissionMsg                : Handles received Wireless Link Mission message
 *  - WiFiHandleWindMsg                   : Handles received Wireless Link Wind message
 *  - WiFiHandleUserLogMsg                : Handles received Wireless Link User Log message
 *  - WiFiHandleLogDataMsg                : Handles received Wireless Link Log Data message
 *  - WiFiHandleRxData                    : Handles received Wireless Link messages
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
#include "Timer.h"
#include "Wind.h"
#include "CRC.h"
#include "UART.h"
#include "MLAN.h"
#include "BIT.h"
#include "UART.h"
#include "FIFO.h"
#include "WIAC82_Comms.h"
#include "UART_PR.h"
#include "Fuze.h"
#include "IMU.h"
#include "MCP.h"
#include "AircraftInterface.h"
#include "Mission.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "UBLOX.h"
#include "ETE.h"
#include "Servos.h"
#include "Nav.h"
#include "PressureSensor.h"
#include "BatteryPack.h"
#include "Telemetry.h"
#include "HOBS.h"
#include "HOBSComms.h"
#include "WiFi.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"

#define WIFI_BATTERY_ON_ID              0x00001234
#define WIFI_BATTERY_OFF_ID             0x0000ABCD


typedef struct
{
  sWeaponHealthAndStatus HealthAndStatus;
  sWeaponTelemetry Telemetry;
} sWiFiWeaponStatus;

typedef struct
{
  sWeaponHealthAndStatusDummy HealthAndStatusDummy;
  sWeaponTelemetryDummy TelemetryDummy;
} sWiFiWeaponStatusDummy;

typedef struct
{
  sMLAN_Header Header;
  union {
	sWiFiWeaponStatusDummy WeaponStatusDummy;
	sWiFiWeaponStatus WeaponStatus;
    sRawMission Mission;
    sWind Wind;
  };
} sWiFiTxMsg;

typedef struct
{
  sMLAN_Header Header;
  unsigned int SerialNo;
  unsigned short SW_AppVersion;
  unsigned short SW_AppCRC;
  unsigned int TotalElapsedTime;
  uWiFiSBIT SBIT;
  unsigned short PIC_3p3V_Level;
  unsigned short Main3p3V_Level;
  unsigned short Temperature;
  unsigned int ValidFlashEntryCnt;
} sWiFiSBIT_Msg;

typedef struct
{
  sMLAN_Header Header;
  unsigned short PIC_3V3;         // mV
  unsigned short Main3V3;         // mV
  unsigned char Power28V;         // LSB = 0.25V
  signed char Temperature;        // ºC
  unsigned char RSSI;             // dbm
  unsigned char Reserved;
  uWiFiCBIT CBIT;
  unsigned char ArcnetMyReconCnt;
  uWirelessSettings Settings;
  unsigned char WiFiModuleState;
  unsigned char WeaponId;
  unsigned short WiFiTxCnt;
  unsigned short WiFiRxCnt;
  unsigned short MLAN_TxCnt;
  unsigned short MLAN_RxCnt;
  unsigned short ArcnetRxErrCnt;
  unsigned short WiFiRxErrCnt;
  unsigned char WiFiRebootCnt;
  unsigned char WiFiFreeBuffersCnt;
} sWiFiStatusMsg;

typedef struct
{
  sMLAN_Header Header;
  unsigned short LogDataCRC;
  unsigned short Spare;
  sWiFiUserLogEntry UserLogEntries[8];
} sWiFiUserLogMsg;

typedef struct
{
  sMLAN_Header Header;
  sWiFiLogData LogData;
  unsigned short LogDataCRC;
  unsigned short Spare;
} sWiFiLogDataMsg;

sWiFi WiFi;

static unsigned char WiFiDataLogFlag;
static unsigned char WiFiSBIT_Completed;
static unsigned char WiFiWaitForRxMsg;
static unsigned int Wireless10msCnt;
static sWiFiTxMsg WiFiTxMsg;

void TestEquipmentSendHealthAndStatus(unsigned char ToNodeId);

void WiFiInit(char *AuxStr)
{
  WiFiSBIT_Completed = FALSE;
  WiFiWaitForRxMsg = FALSE;
  WiFiDataLogFlag = FALSE;
  Wireless10msCnt = 0;

  memset(&WiFiTxMsg, 0, sizeof(sWiFiTxMsg));
//  memset(&WiFi, 0, sizeof(sWiFi));

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.WiFiControls.AuxOn          = 1;
	  AUXCTRL_SS.WiFiControls.SchedulerAuxOn = 0;
	  AUXCTRL_SS.WiFiControls.TxAuxOn        = 0;
	  AUXCTRL_SS.WiFiControls.TxDetailAuxOn  = 0;
	  AUXCTRL_SS.WiFiControls.RxAuxOn        = 0;
	  AUXCTRL_SS.WiFiControls.RxDetailAuxOn  = 0;
	  AUXCTRL_SS.WiFiControls.ErrDetectOn    = 1;
  }

  WiFiTxMsg.Header.SourceId = 0x20;
  WiFiTxMsg.Header.DestinationId = 0x21;
  WiFiTxMsg.Header.SequenceNo = 1;
  WiFiTxMsg.Header.SequenceCnt = 1;

  if (AUXCTRL_SS.WiFiControls.AuxOn){
	  sprintf(AuxStr,"%s%u:WiFi Initialized - SourceID = %d 0x%x / DestID = %d 0x%x\n",AuxStr,GetTimeUs()
			  ,WiFiTxMsg.Header.SourceId,WiFiTxMsg.Header.SourceId,WiFiTxMsg.Header.DestinationId,WiFiTxMsg.Header.DestinationId);
 }

}

void WiFiSend(void)
{
  Wireless10msCnt++;

  if (!WiFiSBIT_Completed) {
    if ((Wireless10msCnt % 10) == 0) {
      if (Wireless10msCnt > 499) {
        // SBIT request sent for the first 5 seconds after power-on only
        WiFiSBIT_Completed = TRUE;
      }

      WiFi.TxCnt++;

      WiFiTxMsg.Header.Opcode = WIFI_ID_SBIT_REQ;
      WiFiTxMsg.Header.DataLength = 0;
      WiFiTxMsg.Header.FrameCnt++;

		if (AUXCTRL_SS.WiFiControls.TxAuxOn){
			sprintf(WiFi.AuxStr,"%s%u:WiFi SBIT Request Command Sent\n",WiFi.AuxStr,GetTimeUs());
		}

      MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
    }
  }
  else if ((Wireless10msCnt % 5) == 0) {  // Every 50ms after SBIT request
    if ((Wireless10msCnt & 0x01) == 0) {
      if (WeaponSettings.WeaponConfig.HOBS_Present){
          if ((WiFiWaitForRxMsg && BIT.SBIT_Complete) && SS_HOBS.HOBSWaitForRxMsg) {
            WiFi.ErrCnt++;
    		if (AUXCTRL_SS.WiFiControls.ErrDetectOn){
    			sprintf(WiFi.AuxStr,"%s%u:WiFi/HOBS Rx Message Timeout ERROR - %d\n",WiFi.AuxStr,GetTimeUs(),WiFi.ErrCnt);
    		}
          }
      }
      else {
          if (WiFiWaitForRxMsg && BIT.SBIT_Complete) {
            WiFi.ErrCnt++;
    		if (AUXCTRL_SS.WiFiControls.ErrDetectOn){
    			sprintf(WiFi.AuxStr,"%s%u:WiFi Rx Message Timeout ERROR - %d\n",WiFi.AuxStr,GetTimeUs(),WiFi.ErrCnt);
    		}
          }
      }

      WiFi.TxCnt++;

      WiFiWaitForRxMsg = TRUE;

      WiFiTxMsg.Header.Opcode = WIFI_ID_HEALTH_STATUS;
      WiFiTxMsg.Header.DataLength = sizeof(sWiFiWeaponStatusDummy);
      WiFiTxMsg.Header.FrameCnt++;

      // Build
      WeaponGetHealthAndStatus((sWeaponHealthAndStatus *) &Weapon.HealthAndStatus);
      WeaponGetTelemetry((sWeaponTelemetry *) &Weapon.Telemetry);

      // Superloop structure NOT compatible with Wifi SW (240 + 12)
      //WiFiTxMsg.WeaponStatus.HealthAndStatus = Weapon.HealthAndStatus;
      //WiFiTxMsg.WeaponStatus.Telemetry = Weapon.Telemetry;

      TelemetryBuild2HzBlock10();

      // Superloop structure COMPATIBLE with Wifi SW (240 + 12)
      WeaponGetDummyHealthStatusAndTelemetry((sWeaponHealthAndStatusDummy *) &Weapon.HealthAndStatusDummy,(sWeaponTelemetryDummy *) &Weapon.TelemetryDummy);

      WiFiTxMsg.WeaponStatusDummy.HealthAndStatusDummy = Weapon.HealthAndStatusDummy;
      WiFiTxMsg.WeaponStatusDummy.TelemetryDummy       = Weapon.TelemetryDummy;

      if (AUXCTRL_SS.WiFiControls.TxAuxOn){
    	  sprintf(WiFi.AuxStr,"%s%u:WiFi Health and Status Message Sent\n",WiFi.AuxStr,GetTimeUs());
    	  if (AUXCTRL_SS.WiFiControls.TxDetailAuxOn){
    		  sprintf(WiFi.AuxStr,"%s Data: %d Header %d\n",WiFi.AuxStr,WiFiTxMsg.Header.DataLength,sizeof(sMLAN_Header));
    	  }
      }

      MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
    }
    else {

      if(!WiFiDataLogFlag)
      {
  		if (AUXCTRL_SS.WiFiControls.TxAuxOn){
  			sprintf(WiFi.AuxStr,"%s%u:WiFi Log Data Sent Command Sent\n",WiFi.AuxStr,GetTimeUs());
  		}

          WiFiTxMsg.Header.Opcode = WIFI_ID_LOG_DATA_REQ;
          WiFiTxMsg.Header.DataLength = 0;
          WiFiTxMsg.Header.FrameCnt++;

          WiFi.TxCnt++;

          MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
      }

      // Send Health & Status to ETE
      //if (MLAN_IsNodeOnNet(MLAN_ETE_NODE_ID)) {
      //  TestEquipmentSendHealthAndStatus(MLAN_ETE_NODE_ID);
      //}
    }
  }
}

void WiFiSendClearCBIT(void)
{
  WiFiTxMsg.Header.Opcode = WIFI_ID_CLEAR_CBIT;
  WiFiTxMsg.Header.DataLength = 0;
  WiFiTxMsg.Header.FrameCnt++;

  if (AUXCTRL_SS.WiFiControls.TxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi 'CLEAR CBIT' Sent Command Message\n",WiFi.AuxStr,GetTimeUs());
  }

  MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
}

void WiFiSendLogDataRequest(void)
{
  WiFiTxMsg.Header.Opcode = WIFI_ID_LOG_DATA_REQ;
  WiFiTxMsg.Header.DataLength = 0;
  WiFiTxMsg.Header.FrameCnt++;

  if (AUXCTRL_SS.WiFiControls.TxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi 'LOG DATA REQUEST' Sent Command Message\n",WiFi.AuxStr,GetTimeUs());
  }

  MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
}

void WiFiSendMission(void)
{
  WiFiTxMsg.Header.Opcode = WIFI_ID_MISSION_REPORT;
  WiFiTxMsg.Header.DataLength = sizeof(sRawMission);
  WiFiTxMsg.Header.FrameCnt++;

  WiFiTxMsg.Mission = Weapon.RawMission;

  if (AUXCTRL_SS.WiFiControls.TxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi 'MISSION' Sent Command Message\n",WiFi.AuxStr,GetTimeUs());
  }

  MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
}

void WiFiSendWind(void)
{
  WiFiTxMsg.Header.Opcode = WIFI_ID_WIND_REPORT;
  WiFiTxMsg.Header.DataLength = sizeof(sWind);
  WiFiTxMsg.Header.FrameCnt++;

  WiFiTxMsg.Wind = Weapon.Wind;

  if (AUXCTRL_SS.WiFiControls.TxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi 'WIND' Sent Command Message\n",WiFi.AuxStr,GetTimeUs());
  }

  MLAN_Send(MLAN_WIFI_NODE_ID, (unsigned char *) &WiFiTxMsg, WiFiTxMsg.Header.DataLength + sizeof(sMLAN_Header));
}

void WiFiSendLogData(void)
{
  //ToDo
}

void WiFiHandleSBIT_Msg(sWiFiSBIT_Msg *SBIT_Msg)
{
  if (!SBIT_Msg->SBIT.Bits.Busy) {
    WiFiSBIT_Completed = TRUE;
  }

  WiFi.SBIT = SBIT_Msg->SBIT;
  WiFi.SW_Version = SBIT_Msg->SW_AppVersion;
  WiFi.SW_CRC = SBIT_Msg->SW_AppCRC;

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi SBIT Message Received\n",WiFi.AuxStr,GetTimeUs());

	  if (AUXCTRL_SS.WiFiControls.RxDetailAuxOn){
		  sprintf(WiFi.AuxStr,"%s WiFi SBIT = 0x%x\n",WiFi.AuxStr,WiFi.SBIT.Word);
		  sprintf(WiFi.AuxStr,"%s WiFi SW Version = %d\n",WiFi.AuxStr,WiFi.SW_Version);
		  sprintf(WiFi.AuxStr,"%s WiFi SW CRC = 0x%x\n",WiFi.AuxStr,WiFi.SW_CRC);
	  }
  }
}

void WiFiHandleStatusMsg(sWiFiStatusMsg *WiFiStatusMsg)
{
  WiFi.ArcnetRxErrCnt = WiFiStatusMsg->ArcnetRxErrCnt;
  WiFi.WiFiRxErrCnt = WiFiStatusMsg->WiFiRxErrCnt;
  WiFi.ArcnetMyReconCnt = WiFiStatusMsg->ArcnetMyReconCnt;
  WiFi.RSSI = WiFiStatusMsg->RSSI;
  WiFi.Temperature = WiFiStatusMsg->Temperature;
  WiFi.WeaponId = WiFiStatusMsg->WeaponId;
  WiFi.WiFiModuleState = WiFiStatusMsg->WiFiModuleState;
  WiFi.CBIT = WiFiStatusMsg->CBIT;
  WiFi.Settings = WiFiStatusMsg->Settings;
  WiFi.PIC_3V3 = WiFiStatusMsg->PIC_3V3;
  WiFi.Main3V3 = WiFiStatusMsg->Main3V3;

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Status Message Received\n",WiFi.AuxStr,GetTimeUs());

	  if (AUXCTRL_SS.WiFiControls.RxDetailAuxOn){
		  sprintf(WiFi.AuxStr,"%s\n WiFi Arcnet Rx Error Cnt = %d\n",WiFi.AuxStr,WiFi.ArcnetMyReconCnt);
		  sprintf(WiFi.AuxStr,"%s\n WiFi WiFi Rx Error Cnt = %d\n",WiFi.AuxStr,WiFi.WiFiRxErrCnt);
		  sprintf(WiFi.AuxStr,"%s\n WiFi ArcnetMyRecon Cnt = %d\n",WiFi.AuxStr,WiFi.ArcnetMyReconCnt);
		  sprintf(WiFi.AuxStr,"%s\n WiFi RSSI = %d\n",WiFi.AuxStr,WiFi.RSSI);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Temperature = %d\n",WiFi.AuxStr,WiFi.Temperature);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Weapon Id = %d\n",WiFi.AuxStr,WiFi.WeaponId);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Module State = 0x%x\n",WiFi.AuxStr,WiFi.WiFiModuleState);
		  sprintf(WiFi.AuxStr,"%s\n WiFi CBIT = 0x%x\n",WiFi.AuxStr,WiFi.CBIT.Word);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Antenna = %d (0 = port, 1 = starboard)\n",WiFi.AuxStr,WiFi.Settings.Bits.WiFiAntenna);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Enabled = %d (0 = disabled, 1 = enabled)\n",WiFi.AuxStr,WiFi.Settings.Bits.WiFiEnabled);
		  sprintf(WiFi.AuxStr,"%s\n WiFi PIC3V3 = %d\n",WiFi.AuxStr,WiFi.PIC_3V3);
		  sprintf(WiFi.AuxStr,"%s\n WiFi Main3V3 = %d\n",WiFi.AuxStr,WiFi.Main3V3);
	  }
  }
}

void WiFiHandleBatteryControl(unsigned int ControlId)
{
  if (ControlId == WIFI_BATTERY_ON_ID) {
    WiFi.ActivateWeapon = TRUE;
  }
  else if (ControlId == WIFI_BATTERY_OFF_ID) {
    WiFi.ActivateWeapon = FALSE;
  }

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Switch Battery ON/OFF Message Received\n",WiFi.AuxStr,GetTimeUs());

	  if (AUXCTRL_SS.WiFiControls.RxDetailAuxOn){
		  sprintf(WiFi.AuxStr,"%s\n WiFi Activate Weapon = %d (0 - Battery off 1 - Battery on)\n",WiFi.AuxStr,WiFi.ActivateWeapon);
	  }
  }
}

void WiFiHandleGNSS_Usage(uGNSS_Usage *GNSS_Usage)
{
  Weapon.GNSS_Usage = *GNSS_Usage;

  if (GNSS_Usage->GPS_NotUsed) {
    UBLOX_DisableSystems(0);
  }
  else {
    UBLOX_EnableSystems(0);
  }

  if (GNSS_Usage->GlonassNotUsed) {
    UBLOX_DisableSystems(1);
  }
  else {
    UBLOX_EnableSystems(1);
  }
}

void WiFiHandleClearCBIT_Msg(void)
{
  BIT_ClearSBIT();
  BIT_ClearCBIT();

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Clear CBIT Errors Message Received\n",WiFi.AuxStr,GetTimeUs());
  }
}

void WiFiHandleClearMissionMsg(void)
{
  sRawMission mission;


  memset(&mission, 0, sizeof(sRawMission));
  MissionSet(&mission, msWIRELESS_CONTROLLER);

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Clear Mission Message Received\n",WiFi.AuxStr,GetTimeUs());
  }
}

void WiFiHandleMissionMsg(sRawMission *RawMissionData)
{
  MissionSet(RawMissionData, msWIRELESS_CONTROLLER);

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Mission Message Received\n",WiFi.AuxStr,GetTimeUs());
  }
}

void WiFiHandleWindMsg(sWind *WindData)
{
  WindSet(WindData);
  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%u:WiFi Wind Data Message Received\n",WiFi.AuxStr,GetTimeUs());
  }
}

void WiFiHandleUserLogMsg(sWiFiUserLogMsg *WiFiUserLogMsg)
{
  unsigned short crc;


  crc = Calculate16BitCRC((unsigned char *) &WiFiUserLogMsg->Spare, sizeof(WiFiUserLogMsg->UserLogEntries) + 2);

  if (crc == WiFiUserLogMsg->LogDataCRC) {
    memcpy(WiFi.UserLogEntries, WiFiUserLogMsg->UserLogEntries, sizeof(WiFi.UserLogEntries));
    TelemetryBuild2HzBlock8();
  }

  if (AUXCTRL_SS.WiFiControls.RxAuxOn){
	  sprintf(WiFi.AuxStr,"%s%dWiFi User Log Data Message Received\n",WiFi.AuxStr,GetTimeUs());
  }
}

void WiFiHandleLogDataMsg(sWiFiLogDataMsg *WiFiLogDataMsg)
{

	memcpy(WiFi.LogData.WifiPartNumberLeft, WiFiLogDataMsg->LogData.WifiPartNumberLeft, sizeof(WiFi.LogData.WifiPartNumberLeft));
	memcpy(WiFi.LogData.WifiPartNumberRight, WiFiLogDataMsg->LogData.WifiPartNumberRight, sizeof(WiFi.LogData.WifiPartNumberRight));
	memcpy(WiFi.LogData.WifiIssueNumber, WiFiLogDataMsg->LogData.WifiIssueNumber, sizeof(WiFi.LogData.WifiIssueNumber));
	memcpy(WiFi.LogData.WifiSerialNumber, WiFiLogDataMsg->LogData.WifiSerialNumber, sizeof(WiFi.LogData.WifiSerialNumber));

	memcpy(WiFi.LogData.ISUPartNumberLeft, WiFiLogDataMsg->LogData.ISUPartNumberLeft, sizeof(WiFi.LogData.ISUPartNumberLeft));
	memcpy(WiFi.LogData.ISUPartNumberRight, WiFiLogDataMsg->LogData.ISUPartNumberRight, sizeof(WiFi.LogData.ISUPartNumberRight));
	memcpy(WiFi.LogData.ISUIssueNumber, WiFiLogDataMsg->LogData.ISUIssueNumber, sizeof(WiFi.LogData.ISUIssueNumber));
	memcpy(WiFi.LogData.ISUSerialNumber, WiFiLogDataMsg->LogData.ISUSerialNumber, sizeof(WiFi.LogData.ISUSerialNumber));

	if (AUXCTRL_SS.WiFiControls.RxAuxOn){
		sprintf(WiFi.AuxStr,"%s%dWiFi Log Data Message Received\n",WiFi.AuxStr,GetTimeUs());

		if (AUXCTRL_SS.WiFiControls.RxDetailAuxOn){
			sprintf(WiFi.AuxStr,"%s\n WifiPartNumberLeft: ",WiFi.AuxStr);
			for (int i = 0; i < 5; i++) {
				if (WiFi.LogData.WifiPartNumberLeft[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.WifiPartNumberLeft[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n WifiPartNumberRight: ",WiFi.AuxStr);
			for (int i = 0; i < 5; i++) {
				if (WiFi.LogData.WifiPartNumberRight[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.WifiPartNumberRight[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n WifiIssueNumber: ",WiFi.AuxStr);
			for (int i = 0; i < 3; i++) {
				if (WiFi.LogData.WifiIssueNumber[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.WifiIssueNumber[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n WifiSerialNumber: ",WiFi.AuxStr);
			for (int i = 0; i < 8; i++) {
				if (WiFi.LogData.WifiSerialNumber[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.WifiSerialNumber[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n ISUPartNumberLeft: ",WiFi.AuxStr);
			for (int i = 0; i < 5; i++) {
				if (WiFi.LogData.ISUPartNumberLeft[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.ISUPartNumberLeft[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n ISUPartNumberRight: ",WiFi.AuxStr);
			for (int i = 0; i < 5; i++) {
				if (WiFi.LogData.ISUPartNumberRight[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.ISUPartNumberRight[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n ISUIssueNumber: ",WiFi.AuxStr);
			for (int i = 0; i < 3; i++) {
				if (WiFi.LogData.ISUIssueNumber[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.ISUIssueNumber[i]);
				} else {
					break;
				}

			}
			sprintf(WiFi.AuxStr,"%s\n ISUSerialNumber: ",WiFi.AuxStr);
			for (int i = 0; i < 8; i++) {
				if (WiFi.LogData.ISUSerialNumber[i] != '\0') {
					sprintf(WiFi.AuxStr,"%s%c",WiFi.AuxStr,WiFi.LogData.ISUSerialNumber[i]);
				} else {
					break;
				}

			}
		}
	}

	WiFiDataLogFlag = TRUE;

}

void WiFiHandleRxData(unsigned char *RxData, unsigned short DataLength)
{
  sMLAN_Header *header;

  WiFi.RxCnt++;
  WiFi.RxTimestamp = Weapon.SystemTime;

  header = (sMLAN_Header *) RxData;

  if (AUXCTRL_SS.WiFiControls.RxDetailAuxOn){
	sprintf(WiFi.AuxStr,"%s%u:Wifi Message Received - OpCode: 0x%x \n",WiFi.AuxStr,GetTimeUs(),header->Opcode);
  }

  if (header->Opcode == WIFI_ID_HOBS_MESSAGES){
	  SS_HOBS.HOBSWaitForRxMsg = FALSE;
  }
  else{
	  WiFiWaitForRxMsg = FALSE;
  }

  switch (header->Opcode) {
    case WIFI_ID_SBIT : WiFiHandleSBIT_Msg((sWiFiSBIT_Msg *) RxData); break;
    case WIFI_ID_BATTERY_CTRL : WiFiHandleBatteryControl(*((unsigned int *) &RxData[12])); break;
    case WIFI_ID_GNSS_USAGE : WiFiHandleGNSS_Usage((uGNSS_Usage *) &RxData[12]); break;
    case WIFI_ID_WIFI_STATUS : WiFiHandleStatusMsg((sWiFiStatusMsg *) RxData); break;
    case WIFI_ID_MISSION_REQ : WiFiSendMission(); break;
    case WIFI_ID_WIND_REQ : WiFiSendWind();  break;
    case WIFI_ID_CLEAR_MISSION : WiFiHandleClearMissionMsg(); break;
    case WIFI_ID_CLEAR_CBIT : WiFiHandleClearCBIT_Msg(); break;
    case WIFI_ID_NEW_MISSION : WiFiHandleMissionMsg((sRawMission *) &RxData[12]); break;
    case WIFI_ID_NEW_WIND : WiFiHandleWindMsg((sWind *) &RxData[12]); break;
    case WIFI_ID_USER_LOG : WiFiHandleUserLogMsg((sWiFiUserLogMsg *) RxData); break;
    case WIFI_ID_HOBS_MESSAGES : HOBS_HandleRxData((uint8_t *)RxData); break;
    case WIFI_ID_LOG_DATA_REPORT : WiFiHandleLogDataMsg((sWiFiLogDataMsg *) RxData); break;
default : break;
  }
}
