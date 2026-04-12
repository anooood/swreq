/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <ETE.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control between the External Test Equipment and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - TestEquipmentInit:                   Initializes test equipment interface
 *  - TestEquipmentSendShortMessage:       Send a message with no payload to the requesting node
 *  - TestEquipmentSendAcknowledgeMessage: Sends a ACK/NACK message to the requesting node
 *  - TestEquipmentSendSBIT:               Send SBIT message to the requesting node
 *  - TestEquipmentSendMission:            Send raw mission data to the requesting node
 *  - TestEquipmentHandleMission:          Handle received mission data
 *  - TestEquipmentSendWind:               Send raw wind data to the requesting node
 *  - TestEquipmentHandleWind:             Handle received wind data
 *  - TestEquipmentClearMission:           Clears the mission
 *  - TestEquipmentClearWind:              Clears the wind
 *  - TestEquipmentSendWeaponConfig:       Send weapon data to the requesting node
 *  - TestEquipmentHandleWeaponConfig:     Handle received weapon data
 *  - TestEquipmentConfigureUBLOX:         Configure UBLOX Module
 *  - TestEquipmentForceFullNav:           Force Full Navigation
 *  - TestEquipmentGNSS_Usage:             Update the GNSS constellation usage
 *  - TestEquipmentSendHealthAndStatus:    Send health and status to the requesting node
 *  - TestEquipmentSendStackInfo:          Send the stack usage info
 *  - TestEquipmentHandleBatteryRemainingCapacity:         *
 *  - TestEquipmentSendFuzeStatus:         Send the fuze status
 *  - TestEquipmentSetFuzeDelay:           Set Fuze Delay
 *  - TestEquipmentHandleRxData:           Handle received data from test equipment
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
#include "ETE.h"
#include "TypeDefines.h"
#include "FIFO.h"
#include "UART.h"
#include "MLAN.h"
#include "BIT.h"
#include "IMU.h"
#include "Telemetry.h"
#include "UBLOX.h"
#include "WIAC82_Comms.h"
#include "Weapon.h"
#include "WeaponSettings.h"
#include "MCP.h"
#include "Servos.h"
#include "Nav.h"
#include "Stack.h"


#define ETE_ID_SBIT_REQ                   0x00
#define ETE_ID_SBIT                       0x01
#define ETE_ID_CLEAR_BIT                  0x04
#define ETE_ID_DISABLE_ARCNET_NOTIFY      0x05
#define ETE_ID_DISABLE_ARCNET_CMD         0x06
#define ETE_ID_MISSION_REQ                0x10
#define ETE_ID_WIND_REQ                   0x11
#define ETE_ID_WEAPON_CONFIQ_REQ          0x12
#define ETE_ID_HEALTH_STATUS_REQ          0x13
#define ETE_ID_GNSS_ANT_SELECT            0x14
#define ETE_ID_GNSS_ANT_POWER             0x15
#define ETE_ID_UBLOX_CONFIGURE            0x16
#define ETE_ID_FLIGHT_PLAN                0x17
#define ETE_ID_FLIGHT_PLAN_REQ            0x18
#define ETE_ID_MANUAL_SERVO_CTRL          0x19
#define ETE_ID_FORCE_FULL_NAV             0x20
#define ETE_ID_GNSS_USSAGE                0x21
#define ETE_ID_CLEAR_MISSION              0x22
#define ETE_ID_CLEAR_WIND                 0x23
#define ETE_ID_ACK                        0x24
#define ETE_ID_NACK                       0x25
#define ETE_ID_STACK_INFO_REQ             0x26
#define ETE_ID_STACK_INFO                 0x27
#define ETE_ID_SET_BATT_REMAINING_CAP     0x28
#define ETE_ID_FORCE_TELEMETRY            0x30
#define ETE_ID_IMU_DATA_REQ               0x40
#define ETE_ID_IMU_DATA                   0x41
#define ETE_ID_MISSION                    0x82
#define ETE_ID_WIND                       0x83
#define ETE_ID_WEAPON_CONFIG              0x88
#define ETE_ID_FLIGHT_PLAN_REPORT         0x89
#define ETE_ID_HEALTH_STATUS              0x90
#define ETE_ID_REQ_FUZE_STATUS            0x91
#define ETE_ID_FUZE_STATUS                0x92
#define ETE_ID_SET_FUZE_DELAY             0x93

#define DISABLE_ARCNET_CMD                0xAABBCCDD


typedef struct
{
  sMLAN_Header Header;
  union
  {
    unsigned char Bytes[508];
    unsigned char GNSS_AntennaSelect;
  } Data;
} sETE_RxMessage;

typedef struct
{
  sMLAN_Header Header;
  union
  {
    unsigned char Bytes[508];
  } Data;
} sETE_TxMessage;

typedef struct
{
  sWeaponHealthAndStatus HealthAndStatus;
  sWeaponTelemetry Telemetry;
} sETE_WeaponStatus;


sETE_TxMessage ETE_TxMessage;

static unsigned int DisableArcnet = 0;

void TestEquipmentInit(void)
{
  DisableArcnet = 0;

  memset(&ETE_TxMessage, 0, sizeof(sETE_TxMessage));

  ETE_TxMessage.Header.SourceId = MLAN_MCP_NODE_ID;
  ETE_TxMessage.Header.DestinationId = MLAN_ETE_NODE_ID;
  ETE_TxMessage.Header.SequenceCnt = 1;
  ETE_TxMessage.Header.SequenceNo = 1;
}

void TestEquipmentSendShortMessage(unsigned char ToNodeId, unsigned short MsgId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = MsgId;
  ETE_TxMessage.Header.DataLength = 0;
  ETE_TxMessage.Header.FrameCnt++;

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header));
}

void TestEquipmentSendAcknowledgeMessage(unsigned char ToNodeId, unsigned char Result)
{
  if (Result) {
    TestEquipmentSendShortMessage(MLAN_ETE_NODE_ID, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(MLAN_ETE_NODE_ID, ETE_ID_NACK);
  }
}

void TestEquipmentSendSBIT(unsigned char ToNodeId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_SBIT;
  ETE_TxMessage.Header.DataLength = sizeof(sMCP_BIT);
  ETE_TxMessage.Header.FrameCnt++;

  memcpy(&ETE_TxMessage.Data, &MCP.SBIT, sizeof(sMCP_BIT));

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentSendMission(unsigned char ToNodeId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_MISSION;
  ETE_TxMessage.Header.DataLength = sizeof(sRawMission);
  ETE_TxMessage.Header.FrameCnt++;

  memcpy(&ETE_TxMessage.Data, (sRawMission *) &Weapon.RawMission, sizeof(sRawMission));

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentHandleMission(unsigned char ToNodeId, unsigned char *RxData)
{
  sRawMission *raw_mission;


  raw_mission = (sRawMission *) &RxData[12];

  if (MissionSet(raw_mission, msETE)) {
    SaveRawMission(raw_mission);
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentSendWind(unsigned char ToNodeId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_WIND;
  ETE_TxMessage.Header.DataLength = sizeof(sWind);
  ETE_TxMessage.Header.FrameCnt++;

  memcpy(&ETE_TxMessage.Data, &WeaponSettings.Wind, sizeof(sWind));

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentHandleWind(unsigned char ToNodeId, unsigned char *RxData)
{
  sWind *wind;


  wind = (sWind *) &RxData[12];

  if (WindSet(wind)) {
    SaveWind(wind);
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentClearMission(unsigned char ToNodeId)
{
  sRawMission raw_mission;


  memset(&raw_mission, 0, sizeof(sRawMission));

  MissionSet(&raw_mission, msETE);

  if (SaveRawMission(&raw_mission)) {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentClearWind(unsigned char ToNodeId)
{
  sWind wind;


  memset(&wind, 0, sizeof(sWind));

  WindSet(&wind);

  if (SaveWind(&wind)) {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentSendWeaponConfig(unsigned char ToNodeId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_WEAPON_CONFIG;
  ETE_TxMessage.Header.DataLength = sizeof(sWeaponConfig);
  ETE_TxMessage.Header.FrameCnt++;

  memcpy(&ETE_TxMessage.Data, &WeaponSettings.WeaponConfig, sizeof(sWeaponConfig));

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentHandleWeaponConfig(unsigned char ToNodeId, unsigned char *RxData)
{
  if (SaveWeaponConfig((sWeaponConfig *) &RxData[12])) {
    TelemetryBuild2HzBlock1();
    TelemetryBuild2HzBlock2();

    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentConfigureUBLOX(unsigned char ToNodeId)
{
  if (UBLOX_Configure()) {
    WeaponSettings.MCP_Log.UBLOX_Configured = TRUE;
    SaveMCP_Log(&WeaponSettings.MCP_Log);
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
  }
  else {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentForceFullNav(unsigned char ToNodeId, unsigned char *RxData)
{
  short val;
  float heading;


  val = *((short *) RxData);

  heading = (float) val / 32768;

  NavForceFullNav(heading);

  TestEquipmentSendShortMessage(ToNodeId, ETE_ID_ACK);
}

void TestEquipmentGNSS_Usage(unsigned char RxByte)
{
  if (Weapon.GNSS_Usage.Byte != RxByte) {
    Weapon.GNSS_Usage.Byte = RxByte;
    Weapon.ConfigureUBLOX_ReceiverCnt = 0;
    Weapon.ConfigureUBLOX_Receiver = TRUE;
  }
}

void TestEquipmentSendHealthAndStatus(unsigned char ToNodeId)
{
  sETE_WeaponStatus status;


  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_HEALTH_STATUS;
  ETE_TxMessage.Header.DataLength = sizeof(sETE_WeaponStatus);
  ETE_TxMessage.Header.FrameCnt++;

  status.HealthAndStatus = Weapon.HealthAndStatus;
  status.Telemetry = Weapon.Telemetry;

  memcpy(&ETE_TxMessage.Data, (sETE_WeaponStatus *) &status, sizeof(sETE_WeaponStatus));

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentSendStackInfo(unsigned char ToNodeId)
{
  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_STACK_INFO;
  ETE_TxMessage.Header.DataLength = sizeof(sStack);
  ETE_TxMessage.Header.FrameCnt++;

  StackGetUssage(Stack.Info);

  memcpy(&ETE_TxMessage.Data, &Stack, ETE_TxMessage.Header.DataLength);

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}


void TestEquipmentHandleBatteryRemainingCapacity(unsigned char ToNodeId, unsigned char NewCapacity)
{

}

void TestEquipmentSendFuzeStatus(unsigned char ToNodeId)
{
  sFuzeInfo fuze_info;


  ETE_TxMessage.Header.DestinationId = ToNodeId;
  ETE_TxMessage.Header.Opcode = ETE_ID_FUZE_STATUS;
  ETE_TxMessage.Header.DataLength = sizeof(sFuzeInfo);
  ETE_TxMessage.Header.FrameCnt++;

  fuze_info.Type = SS_FUZE.Fuze.Type;
  fuze_info.Mode = SS_FUZE.Fuze.Mode;
  fuze_info.DelayInfo = SS_FUZE.Fuze.FuzeTimeDelay;
  fuze_info.Status = SS_FUZE.Fuze.Status;
  fuze_info.SW_Verion = SS_FUZE.Fuze.SW_Verion;
  fuze_info.SetDelayCmdCnt = SS_FUZE.Fuze.SetDelayCmdCnt;

  memcpy(&ETE_TxMessage.Data, &fuze_info, ETE_TxMessage.Header.DataLength);

  MLAN_Send(ToNodeId, (unsigned char *) &ETE_TxMessage, sizeof(sMLAN_Header) + ETE_TxMessage.Header.DataLength);
}

void TestEquipmentSetFuzeDelay(unsigned char ToNodeId, unsigned char FuzeDelay)
{
  if (!Fuze_SetDelay(FuzeDelay, ToNodeId)) {
    TestEquipmentSendShortMessage(ToNodeId, ETE_ID_NACK);
  }
}

void TestEquipmentHandleRxData(unsigned char *RxData, unsigned int DataLength, unsigned char FromNodeId)
{
  sMLAN_Header *header;


  header = (sMLAN_Header *) RxData;

  switch (header->Opcode) {
    case ETE_ID_SBIT_REQ : TestEquipmentSendSBIT(FromNodeId); break;
    case ETE_ID_CLEAR_BIT : {
      BIT_ClearSBIT();
      BIT_ClearCBIT();
    } break;
    case ETE_ID_DISABLE_ARCNET_NOTIFY : DisableArcnet = DISABLE_ARCNET_CMD; break;
    case ETE_ID_DISABLE_ARCNET_CMD : {
      if (DisableArcnet == DISABLE_ARCNET_CMD) {
        MLAN_Disable();
      }
    } break;
    case ETE_ID_MISSION_REQ : TestEquipmentSendMission(FromNodeId); break;
    case ETE_ID_WIND_REQ : TestEquipmentSendWind(FromNodeId); break;
    case ETE_ID_MISSION : TestEquipmentHandleMission(FromNodeId, RxData); break;
    case ETE_ID_WIND : TestEquipmentHandleWind(FromNodeId, RxData); break;
    case ETE_ID_CLEAR_MISSION : TestEquipmentClearMission(FromNodeId); break;
    case ETE_ID_CLEAR_WIND : TestEquipmentClearWind(FromNodeId); break;
    case ETE_ID_WEAPON_CONFIQ_REQ : TestEquipmentSendWeaponConfig(FromNodeId); break;
    case ETE_ID_WEAPON_CONFIG : TestEquipmentHandleWeaponConfig(FromNodeId, RxData); break;
    case ETE_ID_GNSS_ANT_SELECT : MCP_SelectGNSS_Antenna((eGNSS_Antenna) RxData[MLAN_HEADER_SIZE]); break;
    case ETE_ID_UBLOX_CONFIGURE : TestEquipmentConfigureUBLOX(FromNodeId); break;
    case ETE_ID_FORCE_FULL_NAV : TestEquipmentForceFullNav(FromNodeId, &RxData[MLAN_HEADER_SIZE]); break;
    case ETE_ID_FORCE_TELEMETRY : TelUseFixedData(*((unsigned int *) &RxData[MLAN_HEADER_SIZE])); break;
    case ETE_ID_GNSS_USSAGE : TestEquipmentGNSS_Usage(RxData[MLAN_HEADER_SIZE]); break;
    case ETE_ID_HEALTH_STATUS_REQ : TestEquipmentSendHealthAndStatus(FromNodeId); break;
    case ETE_ID_STACK_INFO_REQ : TestEquipmentSendStackInfo(FromNodeId); break;
    case ETE_ID_SET_BATT_REMAINING_CAP : TestEquipmentHandleBatteryRemainingCapacity(FromNodeId, RxData[MLAN_HEADER_SIZE]); break;
    case ETE_ID_REQ_FUZE_STATUS : TestEquipmentSendFuzeStatus(FromNodeId); break;
    case ETE_ID_SET_FUZE_DELAY : TestEquipmentSetFuzeDelay(FromNodeId, RxData[MLAN_HEADER_SIZE]); break;
    default : break;
  }
}

