/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UBLOX.c>
 *
 *  Description:
 *  ------------
 *  UBLOX module.
 *
 *  Function(s):
 *  ------------
 *  - UBLOX_Init                          : Initializes the UBLOX GNSS module and sets up interfaces.
 *  - UBLOX_EnableSystems                 : Enables specified GNSS systems (e.g., GPS, GLONASS).
 *  - UBLOX_DisableSystems                : Disables specified GNSS systems.
 *  - UBLOX_CalculateChecksum             : Calculates UBX protocol message checksum.
 *  - UBLOX_HandleRxPacket                : Processes a received UBX message packet.
 *  - UBLOX_Service                       : Periodic service routine for handling incoming UBLOX data.
 *  - UBLOX_PPS_EventHandler              : Handles the Pulse-Per-Second (PPS) signal event.
 *  - UBLOX_FilterNAV_SAT_Satellites      : Filters satellite data from NAV-SAT messages.
 *  - UBLOX_FilterRXM_RaxXSatellites      : Filters satellite data from RXM-RAWX messages.
 *  - UBLOX_UpdateRMX_RAWX_Tel            : Updates telemetry from RXM-RAWX messages.
 *  - UBLOX_UpdateNAV_TimeGPS_Tel         : Updates telemetry from NAV-TIMEGPS message.
 *  - UBLOX_UpdateNAV_TimeGLO_Tel         : Updates telemetry from NAV-TIMEGLO message.
 *  - UBLOX_UpdateNAV_SOL_Tel             : Updates telemetry from NAV-SOL message.
 *  - UBLOX_UpdateNavSatTel               : Updates telemetry from NAV-SAT satellite data.
 *  - UBLOX_SendMessage                   : Sends a UBX-formatted message to the UBLOX module.
 *  - UBLOX_SetMessageRate                : Configures the output rate for specific UBX messages.
 *  - UBLOX_PollMessage                   : Requests a specific UBX message from the module.
 *  - UBLOX_ConfigureGNSS_Sytem           : Configures which GNSS systems the receiver should use.
 *  - UBLOX_ConfigureNAV                  : Configures navigation-related parameters.
 *  - UBLOX_SetRate                       : Sets the navigation/measurement update rate.
 *  - UBLOX_RequestVersion                : Requests firmware version information from the receiver.
 *  - UBLOX_WaitForACK                    : Waits for an acknowledgment from the UBLOX module.
 *  - UBLOX_ConfigurePort                 : Sets up communication port parameters (baud rate, protocol).
 *  - UBLOX_ResetConfig                   : Resets the UBLOX module to default configuration.
 *  - UBLOX_SaveConfiguration             : Saves current configuration to non-volatile memory.
 *  - UBLOX_Configure                     : Applies a full configuration to the UBLOX module.
 *  - UBLOX_ConfigureReceiver             : High-level configuration routine for receiver settings.
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
#include "UART.h"
#include "FIFO.h"
#include "WIAC82_Comms.h"
#include "Interrupts.h"
#include "Weapon.h"
#include "MCP.h"
#include "TaskManager.h"
#include "UBLOX.h"


// Refer to "u-blox 8 / u-blox M8 Receiver Description, UBX-13003221 - R16"

#define UBLOX_MAX_PAYLOAD_LENGTH      2048

#define UBLOX_SYNC_1                  0xB5
#define UBLOX_SYNC_2                  0x62

#define UBLOX_GPS_GNSS_ID             0x00
#define UBLOX_GLO_GNSS_ID             0x06

// UBLOX classes
#define UBLOX_CLASS_NAV               0x01
#define UBLOX_CLASS_RXM               0x02
#define UBLOX_CLASS_INF               0x04
#define UBLOX_CLASS_ACK               0x05
#define UBLOX_CLASS_CFG               0x06
#define UBLOX_CLASS_UPD               0x09
#define UBLOX_CLASS_MON               0x0A
#define UBLOX_CLASS_AID               0x0B
#define UBLOX_CLASS_TIM               0x0D
#define UBLOX_CLASS_ESF               0x10
#define UBLOX_CLASS_MGA               0x13
#define UBLOX_CLASS_LOG               0x21
#define UBLOX_CLASS_SEC               0x27
#define UBLOX_CLASS_HNR               0x28

// UBLOX NAV class id's used
#define UBLOX_NAV_POSECEF_ID          0x01
#define UBLOX_NAV_POSLLH_ID           0x02
#define UBLOX_NAV_DOP_ID              0x04
#define UBLOX_NAV_SOL                 0x06
#define UBLOX_NAV_VELECEF_ID          0x11
#define UBLOX_NAV_TIME_GPS_ID         0x20
#define UBLOX_NAV_TIME_GLO_ID         0x23
#define UBLOX_NAV_SAT_ID              0x35

// UBLOX RMX class id's used
#define UBLOX_RXM_SFRBX_ID            0x13
#define UBLOX_RXM_RAWX_ID             0x15

// UBLOX ACK class id's used
#define UBLOX_ACK_NACK_ID             0x00
#define UBLOX_ACK_ACK_ID              0x01

// UBLOX CFG class id's used
#define UBLOX_CFG_PRT_ID              0x00
#define UBLOX_CFG_MSG_ID              0x01
#define UBLOX_CFG_RATE_ID             0x08
#define UBLOX_CFG_CFG_ID              0x09
#define UBLOX_CFG_NAV5_ID             0x24
#define UBLOX_CFG_GNSS_ID             0x3E

// UBLOX MON class id's used
#define UBLOX_MON_VER_ID              0x04

// UBLOX TIM class id's used
#define UBLOX_TIM_TP_ID               0x01


typedef enum {
  ubloxGET_SYNC_1,
  ubloxGET_SYNC_2,
  ubloxGET_HEADER,
  ubloxGET_PAYLOAD
} eUBLOX_State;

typedef enum {
  ublox_ackWAITING,
  ublox_ackNACK,
  ublox_ackACK
} eUBLOX_ACK_State;


#pragma pack(1)

typedef struct
{
  unsigned char Class;
  unsigned char Id;
  unsigned short PayloadLength;
} sUBLOX_Header;

typedef struct
{
  sUBLOX_Header Header;
  unsigned char Data[UBLOX_MAX_PAYLOAD_LENGTH];
} sUBLOX_Packet;

typedef union
{
  unsigned int DWord;
  struct {
    unsigned int Reserved1    : 6;
    unsigned int CharLength   : 2;
    unsigned int Reserved2    : 1;
    unsigned int Parity       : 3;
    unsigned int StopBits     : 2;
    unsigned int Reserved3    : 18;
  } Bits;
} uUBLOX_PortMode;

typedef struct
{
  unsigned char   PortID;
  unsigned char   Reserved1;
  unsigned short  TxReady;
  uUBLOX_PortMode Mode;
  unsigned int    Baudrate;
  unsigned short  InProtoMask;
  unsigned short  OutProtoMask;
  unsigned short  Flags;
  unsigned short  Reserved2;
} sUBLOX_PortSettings;

typedef struct
{
  unsigned int ClearMask;
  unsigned int SaveMask;
  unsigned int LoadMask;
  unsigned char DeviceMask;
} sUBLOX_Config;

typedef struct
{
  unsigned short ApplySettingMask;        // 0
  unsigned char DynPlatformModal;         // 2
  unsigned char PosFixingMode;            // 3
  int FixedAltitude;                      // 4
  unsigned int FixedAltVar;               // 8
  signed char MinSatElev;                 // 12
  unsigned char Reserved1;                // 13
  unsigned short PDOP_Mask;               // 14
  unsigned short TDOP_Mask;               // 16
  unsigned short PosAccMask;              // 18
  unsigned short TimeAccMask;             // 20
  unsigned char StaticHoldThresh;         // 22
  unsigned char DGNSS_Timeout;            // 23
  unsigned char CNO_ThreshNumSV;          // 24
  unsigned char CNO_Thresh;               // 25
  unsigned char Reserved2;                // 26
  unsigned char Reserved3;                // 27
  unsigned short StaticHoldMaxDist;       // 28
  unsigned char UTC_Standard;             // 30
  unsigned char Reserved4;                // 31
  unsigned char Reserved5;                // 32
  unsigned char Reserved6;                // 33
  unsigned char Reserved7;                // 34
  unsigned char Reserved8;                // 35
} sUBLOX_NAV_Settings;

#pragma pack()


volatile sUBLOX UBLOX;
volatile sUBLOX_GNSS_20HzBlock1 UBLOX_GNSS_20HzBlock1;
volatile sUBLOX_GNSS_20HzBlock2 UBLOX_GNSS_20HzBlock2;
volatile sUBLOX_GNSS_20HzBlock3 UBLOX_GNSS_20HzBlock3;

static volatile eUBLOX_ACK_State UBLOX_ACK_State;
static unsigned char RxData[1024];
static volatile TIME UBLOX_PPS_Timestamp;
static volatile unsigned int UBLOX_Index;
static volatile unsigned int UBLOX_PacketLength;
static volatile sUBLOX_Packet UBLOX_RxPacket;
static volatile eUBLOX_State UBLOX_State;
static volatile sUART UBLOX_UART;
static volatile T_GNSS_Setup GNSS_Setup;
static T_UBX_NAV_SAT UBLOX_NAV_SAT;
static T_UBX_RXM_RAWX UBLOX_RXM_RAW_X;
static unsigned char UBLOX_RMX_SFRBX_GPS_SV_Index;
static unsigned char UBLOX_RMX_SFRBX_GLO_SV_Index;
static unsigned int UBLOX_RMX_SFRBX_GPS_SV_Timestamp;
static unsigned int UBLOX_RMX_SFRBX_GLO_SV_Timestamp;
static unsigned int UBLOX_RMX_SFRBX_PacketCnt;

static void UBLOX_PPS_EventHandler(unsigned int Input);
static void UBLOX_FilterNAV_SAT_Satellites(T_UBX_NAV_SAT *NAV_SAT, unsigned char *Data);
static void UBLOX_FilterRXM_RaxXSatellites(T_UBX_RXM_RAWX *RXM_RAW_X, unsigned char *Data);
static void UBLOX_UpdateRMX_RAWX_Tel(T_UBX_RXM_RAWX *RXM_RAWX);
static void UBLOX_UpdateNAV_TimeGLO_Tel(T_UBX_NAV_TIMEGLO *TimeGLO);
static void UBLOX_UpdateNAV_TimeGPS_Tel(T_UBX_NAV_TIMEGPS *TimeGPS);
static void UBLOX_UpdateNAV_SOL_Tel(T_UBX_NAV_SOL *NavSol);
static void UBLOX_UpdateNavSatTel(T_UBX_NAV_SAT *NavSat);

void UBLOX_Init(void)
{
  UBLOX_State = ubloxGET_SYNC_1;
  UBLOX_PacketLength = 0;

  UBLOX_ACK_State = ublox_ackWAITING;

  memset((void *) &UBLOX, 0, sizeof(sUBLOX));

  GNSS_Setup.SystemsToUse = 3;
  GNSS_Setup.MinElev = 0.174533;  // 10 degrees
  GNSS_Setup.MinSigStr = 25.0;
  GNSS_Setup.MinLockTime = 1.0;
  GNSS_Setup.MinGDOP = 10;

  UBLOX_PPS_Timestamp = 0;

  UBLOX_RMX_SFRBX_GPS_SV_Index = 0;
  UBLOX_RMX_SFRBX_GLO_SV_Index = 0;
  UBLOX_RMX_SFRBX_GPS_SV_Timestamp = 0;
  UBLOX_RMX_SFRBX_GLO_SV_Timestamp = 0;
  UBLOX_RMX_SFRBX_PacketCnt = 0;

  memset((void *) &UBLOX_GNSS_20HzBlock1, 0, sizeof(sUBLOX_GNSS_20HzBlock1));
  memset((void *) &UBLOX_GNSS_20HzBlock2, 0, sizeof(sUBLOX_GNSS_20HzBlock2));
  memset((void *) &UBLOX_GNSS_20HzBlock3, 0, sizeof(sUBLOX_GNSS_20HzBlock3));

  UBLOX_GNSS_20HzBlock1.Slot1.Header.Identifier = 0x61;
  UBLOX_GNSS_20HzBlock1.Slot1.Header.Id = 0;
  UBLOX_GNSS_20HzBlock1.Slot2.Header.Identifier = 0x62;
  UBLOX_GNSS_20HzBlock1.Slot2.Header.Id = 1;
  UBLOX_GNSS_20HzBlock1.Slot3.Header.Identifier = 0x63;
  UBLOX_GNSS_20HzBlock1.Slot3.Header.Id = 2;
  UBLOX_GNSS_20HzBlock1.Slot4.Header.Identifier = 0x64;
  UBLOX_GNSS_20HzBlock1.Slot4.Header.Id = 3;
  UBLOX_GNSS_20HzBlock1.Slot5.Header.Identifier = 0x65;
  UBLOX_GNSS_20HzBlock1.Slot5.Header.Id = 4;
  UBLOX_GNSS_20HzBlock1.Slot6.Header.Identifier = 0x66;
  UBLOX_GNSS_20HzBlock1.Slot6.Header.Id = 5;
  UBLOX_GNSS_20HzBlock1.Slot7.Header.Identifier = 0x67;
  UBLOX_GNSS_20HzBlock1.Slot7.Header.Id = 6;
  UBLOX_GNSS_20HzBlock1.Slot8.Header.Identifier = 0x68;
  UBLOX_GNSS_20HzBlock1.Slot8.Header.Id = 7;
  UBLOX_GNSS_20HzBlock1.Slot9.Header.Identifier = 0x69;
  UBLOX_GNSS_20HzBlock1.Slot9.Header.Id = 8;
  UBLOX_GNSS_20HzBlock1.Slot10.Header.Identifier = 0x6A;
  UBLOX_GNSS_20HzBlock1.Slot10.Header.Id = 9;

  UBLOX_GNSS_20HzBlock2.Slot1.Header.Identifier = 0x71;
  UBLOX_GNSS_20HzBlock2.Slot1.Header.Id = 0;
  UBLOX_GNSS_20HzBlock2.Slot2.Header.Identifier = 0x72;
  UBLOX_GNSS_20HzBlock2.Slot2.Header.Id = 1;
  UBLOX_GNSS_20HzBlock2.Slot3.Header.Identifier = 0x73;
  UBLOX_GNSS_20HzBlock2.Slot3.Header.Id = 2;
  UBLOX_GNSS_20HzBlock2.Slot4.Header.Identifier = 0x74;
  UBLOX_GNSS_20HzBlock2.Slot4.Header.Id = 3;
  UBLOX_GNSS_20HzBlock2.Slot5.Header.Identifier = 0x75;
  UBLOX_GNSS_20HzBlock2.Slot5.Header.Id = 4;
  UBLOX_GNSS_20HzBlock2.Slot6.Header.Identifier = 0x76;
  UBLOX_GNSS_20HzBlock2.Slot6.Header.Id = 5;
  UBLOX_GNSS_20HzBlock2.Slot7.Header.Identifier = 0x77;
  UBLOX_GNSS_20HzBlock2.Slot7.Header.Id = 6;
  UBLOX_GNSS_20HzBlock2.Slot8.Header.Identifier = 0x78;
  UBLOX_GNSS_20HzBlock2.Slot8.Header.Id = 7;
  UBLOX_GNSS_20HzBlock2.Slot9.Header.Identifier = 0x79;
  UBLOX_GNSS_20HzBlock2.Slot9.Header.Id = 8;
  UBLOX_GNSS_20HzBlock2.Slot10.Header.Identifier = 0x7A;
  UBLOX_GNSS_20HzBlock2.Slot10.Header.Id = 9;

  UBLOX_GNSS_20HzBlock3.Slot1.Header.Identifier = 0x81;
  UBLOX_GNSS_20HzBlock3.Slot1.Header.Id = 0;
  UBLOX_GNSS_20HzBlock3.Slot2.Header.Identifier = 0x82;
  UBLOX_GNSS_20HzBlock3.Slot2.Header.Id = 1;
  UBLOX_GNSS_20HzBlock3.Slot3.Header.Identifier = 0x83;
  UBLOX_GNSS_20HzBlock3.Slot3.Header.Id = 2;
  UBLOX_GNSS_20HzBlock3.Slot4.Header.Identifier = 0x84;
  UBLOX_GNSS_20HzBlock3.Slot4.Header.Id = 3;
  UBLOX_GNSS_20HzBlock3.Slot5.Header.Identifier = 0x85;
  UBLOX_GNSS_20HzBlock3.Slot5.Header.Id = 4;
  UBLOX_GNSS_20HzBlock3.Slot6.Header.Identifier = 0x86;
  UBLOX_GNSS_20HzBlock3.Slot6.Header.Id = 5;
  UBLOX_GNSS_20HzBlock3.Slot7.Header.Identifier = 0x87;
  UBLOX_GNSS_20HzBlock3.Slot7.Header.Id = 6;
  UBLOX_GNSS_20HzBlock3.Slot8.Header.Identifier = 0x88;
  UBLOX_GNSS_20HzBlock3.Slot8.Header.Id = 7;
  UBLOX_GNSS_20HzBlock3.Slot9.Header.Identifier = 0x89;
  UBLOX_GNSS_20HzBlock3.Slot9.Header.Id = 8;
  UBLOX_GNSS_20HzBlock3.Slot10.Header.Identifier = 0x8A;
  UBLOX_GNSS_20HzBlock3.Slot10.Header.Id = 9;

  GNSS_Initialize();
  GNSS_ChangeSetup((T_GNSS_Setup *) &GNSS_Setup);

  // Initiate UBLOX communication
  UART_Init((sUART *) &UBLOX_UART, FPGA_UART_GNSS_ADDR, br460800, parNONE, 1, 0);
  //UART_Init((sUART *) &UBLOX_UART, FPGA_UART_GNSS_ADDR, br9600, parNONE, 1);

  // Enable UBLOX PPS interrupt
  EnableHardwareInterrupt(UBLOX_PPS_EventHandler, 0, intGNSS_PPS, TRUE);
}

void UBLOX_EnableSystems(unsigned short Systems)
{
  GNSS_Setup.SystemsToUse |= (1 << Systems);
  GNSS_ChangeSetup((T_GNSS_Setup *) &GNSS_Setup);
}

void UBLOX_DisableSystems(unsigned short Systems)
{
  GNSS_Setup.SystemsToUse &= (~(1 << Systems));
  GNSS_ChangeSetup((T_GNSS_Setup *) &GNSS_Setup);
}

static unsigned short UBLOX_CalculateChecksum(unsigned char *Data, unsigned int Length)
{
  unsigned int index;
  unsigned char checksum_a = 0;
  unsigned char checksum_b = 0;


  for (index = 0; index < Length; index++) {
    checksum_a = checksum_a + Data[index];
    checksum_b = checksum_b + checksum_a;
  }

  return ((checksum_b << 8) + checksum_a);
}

static void UBLOX_HandleRxPacket(void)
{
  switch (UBLOX_RxPacket.Header.Class) {
    case UBLOX_CLASS_NAV : {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_NAV_POSECEF_ID : {
          UBLOX_GNSS_20HzBlock2.Slot4.NavPOSECEF_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
          UBLOX_GNSS_20HzBlock2.Slot4.NavPOSECEF_Timestamp.PacketCnt++;
          memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot4.NavPOSECEF, (void *) UBLOX_RxPacket.Data, sizeof(sUBLOX_TelNavPOSECEF));
        } break;
        case UBLOX_NAV_POSLLH_ID : {
          UBLOX_GNSS_20HzBlock2.Slot10.NAV_POSLLH_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
          UBLOX_GNSS_20HzBlock2.Slot10.NAV_POSLLH_Timestamp.PacketCnt++;
          memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_POSLLH, (void *) UBLOX_RxPacket.Data, sizeof(sUBLOX_TelNAV_POSLLH));
        } break;
        case UBLOX_NAV_DOP_ID : {
          UBLOX_GNSS_20HzBlock2.Slot3.NAV_DOP_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
          UBLOX_GNSS_20HzBlock2.Slot3.NAV_DOP_Timestamp.PacketCnt++;
          memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot3.NAV_DOP, (void *) UBLOX_RxPacket.Data, sizeof(sUBLOX_TelNAV_DOP));
        } break;
        case UBLOX_NAV_VELECEF_ID : {
          UBLOX_GNSS_20HzBlock2.Slot5.NavVELECEF_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
          UBLOX_GNSS_20HzBlock2.Slot5.NavVELECEF_Timestamp.PacketCnt++;
          memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot5.NavVELECEF, (void *) UBLOX_RxPacket.Data, sizeof(sUBLOX_TelNavVELECEF));
        } break;
        case UBLOX_NAV_SAT_ID : {
          UBLOX_FilterNAV_SAT_Satellites(&UBLOX_NAV_SAT, (unsigned char *) UBLOX_RxPacket.Data);
          UBLOX_UpdateNavSatTel(&UBLOX_NAV_SAT);
        } break;
        case UBLOX_NAV_TIME_GLO_ID : {
          UBLOX_UpdateNAV_TimeGLO_Tel((T_UBX_NAV_TIMEGLO *) UBLOX_RxPacket.Data);
        } break;
        case UBLOX_NAV_TIME_GPS_ID : {
          UBLOX_UpdateNAV_TimeGPS_Tel((T_UBX_NAV_TIMEGPS *) UBLOX_RxPacket.Data);
        } break;
        case UBLOX_NAV_SOL : {
          GNSS_OnUbxNavSol((T_UBX_NAV_SOL *) UBLOX_RxPacket.Data);
          UBLOX_UpdateNAV_SOL_Tel((T_UBX_NAV_SOL *) UBLOX_RxPacket.Data);

          GNSS_GetResult((T_GNSS_Result *) &UBLOX.GNSS_Result);

          UBLOX_GNSS_20HzBlock3.Slot1.Timestamp = Weapon.SystemTime;
          UBLOX_GNSS_20HzBlock3.Slot1.PacketCnt++;
          UBLOX_GNSS_20HzBlock3.Slot1.GNSS_Result = UBLOX.GNSS_Result;
          /*
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultValid = UBLOX.GNSS_Result.Valid;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultReserved0 = UBLOX.GNSS_Result.Reserved0;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultNoSVsInFix = UBLOX.GNSS_Result.NoSVsInFix;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultGPS_TimeOfMeas = UBLOX.GNSS_Result.T_meas.GPS;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultGLO_TimeOfMeas = UBLOX.GNSS_Result.T_meas.GLO;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultSysTimeOfMeas = UBLOX.GNSS_Result.T_meas.Sys;
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultPos[0] = UBLOX.GNSS_Result.Pos[0];
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultPos[1] = UBLOX.GNSS_Result.Pos[1];
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultPos[2] = UBLOX.GNSS_Result.Pos[2];
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultVel[0] = UBLOX.GNSS_Result.Vel[0];
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultVel[1] = UBLOX.GNSS_Result.Vel[1];
          UBLOX_GNSS_20HzBlock3.Slot1.GNNS_ResultVel[2] = UBLOX.GNSS_Result.Vel[2];*/

          UBLOX_GNSS_20HzBlock3.Slot6 = UBLOX_GNSS_20HzBlock3.Slot1;
          /*UBLOX_GNSS_20HzBlock3.Slot6.Timestamp = UBLOX_GNSS_20HzBlock3.Slot1.Timestamp;
          UBLOX_GNSS_20HzBlock3.Slot6.PacketCnt = UBLOX_GNSS_20HzBlock3.Slot1.PacketCnt;
          UBLOX_GNSS_20HzBlock3.Slot6.GNSS_Result = UBLOX.GNSS_Result;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultValid = UBLOX.GNSS_Result.Valid;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultReserved0 = UBLOX.GNSS_Result.Reserved0;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultNoSVsInFix = UBLOX.GNSS_Result.NoSVsInFix;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultGPS_TimeOfMeas = UBLOX.GNSS_Result.T_meas.GPS;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultGLO_TimeOfMeas = UBLOX.GNSS_Result.T_meas.GLO;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultSysTimeOfMeas = UBLOX.GNSS_Result.T_meas.Sys;
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultPos[0] = UBLOX.GNSS_Result.Pos[0];
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultPos[1] = UBLOX.GNSS_Result.Pos[1];
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultPos[2] = UBLOX.GNSS_Result.Pos[2];
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultVel[0] = UBLOX.GNSS_Result.Vel[0];
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultVel[1] = UBLOX.GNSS_Result.Vel[1];
          UBLOX_GNSS_20HzBlock3.Slot6.GNNS_ResultVel[2] = UBLOX.GNSS_Result.Vel[2];*/

          // Activate 2 Hz software interrupt
          TaskManagerAdd(task2HZ);
        } break;
        default : break;
      }
    } break;
    case UBLOX_CLASS_RXM : {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_RXM_SFRBX_ID : {
        } break;
        case UBLOX_RXM_RAWX_ID : {
          UBLOX_FilterRXM_RaxXSatellites(&UBLOX_RXM_RAW_X, (unsigned char *) UBLOX_RxPacket.Data);

          // Update telemetry
          UBLOX_UpdateRMX_RAWX_Tel((T_UBX_RXM_RAWX *) UBLOX_RxPacket.Data);
        } break;
        default : break;
      }
    } break;
    case UBLOX_CLASS_TIM : {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_TIM_TP_ID : {
          GNSS_OnUbxTimTp((double) Weapon.SystemTime * 1E-6, (T_UBX_TIM_TP *) UBLOX_RxPacket.Data);

          // Update telemetry
          UBLOX_GNSS_20HzBlock1.Slot1.TIM_TP_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
          UBLOX_GNSS_20HzBlock1.Slot1.TIM_TP_Timestamp.PacketCnt++;
          memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.TIM_TP, (void *) UBLOX_RxPacket.Data, sizeof(sUBLOX_TelTIM_TP));
          UBLOX_GNSS_20HzBlock1.Slot6.TIM_TP_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.TIM_TP_Timestamp;
          UBLOX_GNSS_20HzBlock1.Slot6.TIM_TP = UBLOX_GNSS_20HzBlock1.Slot1.TIM_TP;
        } break;
        default : break;
      }
    } break;
    case UBLOX_CLASS_MON : {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_MON_VER_ID : {

        } break;
      }
    }
    case UBLOX_CLASS_ACK : {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_ACK_NACK_ID : {
          UBLOX_ACK_State = ublox_ackNACK;
        } break;
        case UBLOX_ACK_ACK_ID : {
          UBLOX_ACK_State = ublox_ackACK;
        } break;
        default : break;
      }
    } break;
    case UBLOX_CLASS_CFG: {
      switch (UBLOX_RxPacket.Header.Id) {
        case UBLOX_CFG_GNSS_ID : {
          UBLOX_RxPacket.Data[0] = 0;
        } break;
        default : {
          UBLOX_RxPacket.Data[0] = 0;
        } break;
      }
    } break;
    default : {
    } break;
  }
}

void UBLOX_Service(void)
{
  unsigned int rx_cnt;
  unsigned int offset = 0;
  unsigned short received_checksum;
  unsigned short calculated_checksum;
  volatile unsigned char *data;


  rx_cnt = UART_Read((sUART *) &UBLOX_UART, RxData, sizeof(RxData));

  if (rx_cnt > 0) {
    data = (unsigned char *) &UBLOX_RxPacket;

    do {
      switch (UBLOX_State) {
        case ubloxGET_SYNC_1 : {
          if (RxData[offset] == UBLOX_SYNC_1) {
            UBLOX_State = ubloxGET_SYNC_2;
          }
        } break;
        case ubloxGET_SYNC_2 : {
          if (RxData[offset] == UBLOX_SYNC_2) {
            UBLOX_Index = 0;
            UBLOX_State = ubloxGET_HEADER;
          }
          else {
            UBLOX_State = ubloxGET_SYNC_1;
            offset--;
          }
        } break;
        case ubloxGET_HEADER : {
          data[UBLOX_Index] = RxData[offset];
          UBLOX_Index++;

          if (UBLOX_Index == sizeof(sUBLOX_Header)) {
            if (UBLOX_RxPacket.Header.PayloadLength < (UBLOX_MAX_PAYLOAD_LENGTH - sizeof(sUBLOX_Header))) {
              UBLOX_PacketLength = sizeof(sUBLOX_Header) + UBLOX_RxPacket.Header.PayloadLength + 2;  // The "+ 2" is the checksum bytes
              UBLOX_State = ubloxGET_PAYLOAD;
            }
            else {
              UBLOX_State = ubloxGET_SYNC_1;
            }
          }
        } break;
        case ubloxGET_PAYLOAD : {
          data[UBLOX_Index] = RxData[offset];
          UBLOX_Index++;

          if (UBLOX_Index == UBLOX_PacketLength) {
            received_checksum = (data[UBLOX_Index - 1] << 8) + data[UBLOX_Index - 2];
            calculated_checksum = UBLOX_CalculateChecksum((unsigned char *) data, UBLOX_PacketLength - 2);

            // Verify received packet checksum
            if (received_checksum == calculated_checksum) {
              UBLOX.RxTimestamp = Weapon.SystemTime;
              UBLOX.RxCnt++;

              UBLOX_HandleRxPacket();
            }
            else {
              UBLOX.ErrCnt++;
            }

            UBLOX_State = ubloxGET_SYNC_1;
          }
        } break;
        default : {
          UBLOX_State = ubloxGET_SYNC_1;
        } break;
      }

      offset++;
    } while (offset < rx_cnt);
  }
}

void UBLOX_PPS_EventHandler(unsigned int Input)
{
  UBLOX_PPS_Timestamp = GetSystemTime();
  GNSS_OnPPS((double) UBLOX_PPS_Timestamp * 1E-6);
}

void UBLOX_FilterNAV_SAT_Satellites(T_UBX_NAV_SAT *NAV_SAT, unsigned char *Data)
{
  /*int index;
  int sat_index;
  int cnt;
  int offset;
  T_UBX_NAV_SAT_Entry *entry;


  memcpy(NAV_SAT, Data, 8);

  cnt = NAV_SAT->numSvs;
  sat_index = 0;
  offset = 8;

  for (index = 0; index < cnt; index++) {
    entry = (T_UBX_NAV_SAT_Entry *) &Data[offset];

    // Only use GPS satellites with SV's between 1 and 32 and Glonass satellites with SV's between 1 and 24
    if (entry->gnssId == UBLOX_GPS_GNSS_ID) {
      if ((entry->svId > 0) && (entry->svId < 33)) {
        NAV_SAT->Entries[sat_index] = *entry;
        sat_index++;
      }
    }
    else if (entry->gnssId == UBLOX_GLO_GNSS_ID) {
      if ((entry->svId > 0) && (entry->svId < 25)) {
        NAV_SAT->Entries[sat_index] = *entry;
        sat_index++;
      }
    }

    if (sat_index >= 28) {
      break;
    }

    offset += sizeof(T_UBX_NAV_SAT_Entry);
  }

  // Update the satellite count
  NAV_SAT->numSvs = sat_index;*/


  unsigned char gps_sat_cnt = 0;
  unsigned char glo_sat_cnt = 0;
  unsigned int index;
  unsigned int tel_sat_index = 0;
  unsigned int offset = sizeof(sUBLOX_TelNAV_SAT_Header);
  float gps_average = 0.0;
  float glo_average = 0.0;
  sUBLOX_TelNAV_SAT_Header nav_sat_header;
  T_UBX_NAV_SAT_Entry entry;


  memcpy(&nav_sat_header, Data, 8);

  for (index = 0; index < nav_sat_header.NumSvs; index++) {
    memcpy(&entry, &Data[offset], sizeof(T_UBX_NAV_SAT_Entry));

    switch (entry.gnssId) {
      case UBLOX_GPS_GNSS_ID : {
        if ((entry.flags & 0x08) == 0x08) {
          gps_sat_cnt++;
          gps_average += entry.cn0;
        }

        if ((entry.svId > 0) && (entry.svId < 33)) {
          if (tel_sat_index < 28) {
            NAV_SAT->Entries[tel_sat_index] = entry;
            tel_sat_index++;
          }
        }
      } break;
       case UBLOX_GLO_GNSS_ID : {
        if ((entry.flags & 0x08) == 0x08) {
          glo_sat_cnt++;
          glo_average += entry.cn0;
        }

        if ((entry.svId > 0) && (entry.svId < 25)) {
          if (tel_sat_index < 28) {
            NAV_SAT->Entries[tel_sat_index] = entry;
            tel_sat_index++;
          }
        }
      } break;
    }

    offset += sizeof(T_UBX_NAV_SAT_Entry);
  }

  NAV_SAT->numSvs = tel_sat_index;
  NAV_SAT->iTOW = nav_sat_header.iTOW;
  NAV_SAT->version = nav_sat_header.Version;
  NAV_SAT->reserved1 = nav_sat_header.Reserved;

  if (gps_sat_cnt > 0) {
    gps_average /= gps_sat_cnt;
  }

  if (glo_sat_cnt > 0) {
    glo_average /= glo_sat_cnt;
  }

  UBLOX.GPS_SatUsedCnt = gps_sat_cnt;
  UBLOX.GLO_SatUsedCnt = glo_sat_cnt;

  UBLOX.GPS_AveSignalStrenth = gps_average;
  UBLOX.GLO_AveSignalStrenth = glo_average;
}

void UBLOX_FilterRXM_RaxXSatellites(T_UBX_RXM_RAWX *RXM_RAW_X, unsigned char *Data)
{
  int index;
  int sat_index;
  int cnt;
  int offset;
  T_UBX_RXM_RAWX_Entry *entry;


  memcpy(RXM_RAW_X, Data, 12);

  cnt = RXM_RAW_X->numMeas;
  sat_index = 0;
  offset = 16;

  for (index = 0; index < cnt; index++) {
    entry = (T_UBX_RXM_RAWX_Entry *) &Data[offset];

    // Only use GPS satellites with SV's between 1 and 32 and Glonass satellites with SV's between 1 and 24
    if (entry->gnssId == UBLOX_GPS_GNSS_ID) {
      if ((entry->svId > 0) && (entry->svId < 33)) {
        RXM_RAW_X->Entries[sat_index] = *entry;
        sat_index++;
      }
    }
    else if (entry->gnssId == UBLOX_GLO_GNSS_ID) {
      if ((entry->svId > 0) && (entry->svId < 25)) {
        RXM_RAW_X->Entries[sat_index] = *entry;
        sat_index++;
      }
    }

    if (sat_index >= 28) {
      break;
    }

    offset += sizeof(T_UBX_RXM_RAWX_Entry);
  }

  // Update the satellite count
  RXM_RAW_X->numMeas = sat_index;
}

void UBLOX_UpdateRMX_RAWX_Tel(T_UBX_RXM_RAWX *RXM_RAWX)
{
  UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
  UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp.PacketCnt++;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Header, RXM_RAWX, sizeof(sUBLOX_TelRXM_RAWX_Header));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV1, &RXM_RAWX->Entries[0], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV2, &RXM_RAWX->Entries[1], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV3, &RXM_RAWX->Entries[2], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV4, &RXM_RAWX->Entries[3], sizeof(sUBLOX_TelRXM_RAWX_SV));
  UBLOX_GNSS_20HzBlock1.Slot1.PPS_Timestamp = UBLOX_PPS_Timestamp;

  UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAW_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV5, &RXM_RAWX->Entries[4], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV6, &RXM_RAWX->Entries[5], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV7, &RXM_RAWX->Entries[6], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV8, &RXM_RAWX->Entries[7], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV9, &RXM_RAWX->Entries[8], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot2.RXM_RAWX_SV10, &RXM_RAWX->Entries[9], sizeof(sUBLOX_TelRXM_RAWX_SV));

  UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAW_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV11, &RXM_RAWX->Entries[10], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV12, &RXM_RAWX->Entries[11], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV13, &RXM_RAWX->Entries[12], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV14, &RXM_RAWX->Entries[13], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV15, &RXM_RAWX->Entries[14], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot3.RXM_RAWX_SV16, &RXM_RAWX->Entries[15], sizeof(sUBLOX_TelRXM_RAWX_SV));

  UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAW_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV17, &RXM_RAWX->Entries[16], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV18, &RXM_RAWX->Entries[17], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV19, &RXM_RAWX->Entries[18], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV20, &RXM_RAWX->Entries[19], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV21, &RXM_RAWX->Entries[20], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot4.RXM_RAWX_SV22, &RXM_RAWX->Entries[21], sizeof(sUBLOX_TelRXM_RAWX_SV));

  UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAW_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV23, &RXM_RAWX->Entries[22], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV24, &RXM_RAWX->Entries[23], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV25, &RXM_RAWX->Entries[24], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV26, &RXM_RAWX->Entries[25], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV27, &RXM_RAWX->Entries[26], sizeof(sUBLOX_TelRXM_RAWX_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot5.RXM_RAWX_SV28, &RXM_RAWX->Entries[27], sizeof(sUBLOX_TelRXM_RAWX_SV));

  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAW_Timestamp = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Timestamp;
  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAW_Header = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAW_Header;
  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAWX_SV1 = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV1;
  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAWX_SV2 = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV2;
  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAWX_SV3 = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV3;
  UBLOX_GNSS_20HzBlock1.Slot6.RXM_RAWX_SV4 = UBLOX_GNSS_20HzBlock1.Slot1.RXM_RAWX_SV4;
  UBLOX_GNSS_20HzBlock1.Slot6.PPS_Timestamp = UBLOX_PPS_Timestamp;

  UBLOX_GNSS_20HzBlock1.Slot7 = UBLOX_GNSS_20HzBlock1.Slot2;
  UBLOX_GNSS_20HzBlock1.Slot7.Header.Identifier = 0x67;
  UBLOX_GNSS_20HzBlock1.Slot7.Header.Id = 6;

  UBLOX_GNSS_20HzBlock1.Slot8 = UBLOX_GNSS_20HzBlock1.Slot3;
  UBLOX_GNSS_20HzBlock1.Slot8.Header.Identifier = 0x68;
  UBLOX_GNSS_20HzBlock1.Slot8.Header.Id = 7;

  UBLOX_GNSS_20HzBlock1.Slot9 = UBLOX_GNSS_20HzBlock1.Slot4;
  UBLOX_GNSS_20HzBlock1.Slot9.Header.Identifier = 0x69;
  UBLOX_GNSS_20HzBlock1.Slot9.Header.Id = 8;

  UBLOX_GNSS_20HzBlock1.Slot10 = UBLOX_GNSS_20HzBlock1.Slot5;
  UBLOX_GNSS_20HzBlock1.Slot10.Header.Identifier = 0x6A;
  UBLOX_GNSS_20HzBlock1.Slot10.Header.Id = 9;
}

void UBLOX_UpdateNAV_TimeGPS_Tel(T_UBX_NAV_TIMEGPS *TimeGPS)
{
  UBLOX_GNSS_20HzBlock1.Slot1.NAV_TimeGPS_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us;
  UBLOX_GNSS_20HzBlock1.Slot1.NAV_TimeGPS_Timestamp.PacketCnt++;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot1.NAV_TimeGPS, TimeGPS, sizeof(sUBLOX_TelNAV_TimeGPS));
}

void UBLOX_UpdateNAV_TimeGLO_Tel(T_UBX_NAV_TIMEGLO *TimeGLO)
{
  UBLOX_GNSS_20HzBlock1.Slot6.NAV_TimeGLO_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us;
  UBLOX_GNSS_20HzBlock1.Slot6.NAV_TimeGLO_Timestamp.PacketCnt++;
  memcpy((void *) &UBLOX_GNSS_20HzBlock1.Slot6.NAV_TimeGLO, TimeGLO, sizeof(sUBLOX_TelNAV_TimeGLO));
}

void UBLOX_UpdateNAV_SOL_Tel(T_UBX_NAV_SOL *NavSol)
{
  UBLOX_GNSS_20HzBlock2.Slot7.NAV_SOL.TimeStamp = Weapon.SystemTime;
  UBLOX_GNSS_20HzBlock2.Slot7.NAV_SOL.PacketCnt++;
  UBLOX_GNSS_20HzBlock2.Slot7.NAV_SOL.NAV_SOL = *NavSol;
}

void UBLOX_UpdateNavSatTel(T_UBX_NAV_SAT *NavSat)
{
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Timestamp.Time = (Weapon.SystemTime >> 8);   // LSB = 256 us
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Timestamp.PacketCnt++;
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Header.iTOW = NavSat->iTOW;
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Header.Version = NavSat->version;
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Header.NumSvs = NavSat->numSvs;
  UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Header.Reserved = NavSat->reserved1;

  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV01, &NavSat->Entries[0], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV02, &NavSat->Entries[1], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV03, &NavSat->Entries[2], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV04, &NavSat->Entries[3], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV05, &NavSat->Entries[4], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV06, &NavSat->Entries[5], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV07, &NavSat->Entries[6], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV08, &NavSat->Entries[7], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV09, &NavSat->Entries[8], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV10, &NavSat->Entries[9], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV11, &NavSat->Entries[10], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV12, &NavSat->Entries[11], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV13, &NavSat->Entries[12], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV14, &NavSat->Entries[13], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_SV15, &NavSat->Entries[14], sizeof(sUBLOX_TelNAV_SAT_SV));

  UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_Timestamp = UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Timestamp;
  UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_iTOW = UBLOX_GNSS_20HzBlock2.Slot9.NAV_SAT_Header.iTOW;
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV16, &NavSat->Entries[15], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV17, &NavSat->Entries[16], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV18, &NavSat->Entries[17], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV19, &NavSat->Entries[18], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV20, &NavSat->Entries[19], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV21, &NavSat->Entries[20], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV22, &NavSat->Entries[21], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV23, &NavSat->Entries[22], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV24, &NavSat->Entries[23], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV25, &NavSat->Entries[24], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV26, &NavSat->Entries[25], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV27, &NavSat->Entries[26], sizeof(sUBLOX_TelNAV_SAT_SV));
  memcpy((void *) &UBLOX_GNSS_20HzBlock2.Slot10.NAV_SAT_SV28, &NavSat->Entries[27], sizeof(sUBLOX_TelNAV_SAT_SV));
}

void UBLOX_SendMessage(unsigned char Class, unsigned char Id, unsigned char *Data, unsigned short Size)
{
  unsigned char tx_data[UBLOX_MAX_PAYLOAD_LENGTH];
  unsigned short checksum;


  if (Size > (UBLOX_MAX_PAYLOAD_LENGTH - 8)) {
    Size = UBLOX_MAX_PAYLOAD_LENGTH - 8;
  }

  tx_data[0] = UBLOX_SYNC_1;
  tx_data[1] = UBLOX_SYNC_2;
  tx_data[2] = Class;
  tx_data[3] = Id;
  tx_data[4] = Size & 0xFF;
  tx_data[5] = (Size >> 8) & 0xFF;
  memcpy(&tx_data[6], Data, Size);

  checksum = UBLOX_CalculateChecksum(&tx_data[2], Size + 4);

  tx_data[Size + 6] = checksum & 0xFF;
  tx_data[Size + 7] = (checksum >> 8) & 0xFF;

  UART_Write((sUART *) &UBLOX_UART, tx_data, Size + 8);
}

void UBLOX_SetMessageRate(unsigned char Class, unsigned char Id, unsigned char Rate)
{
  unsigned char tx_data[8];


  tx_data[0] = Class;
  tx_data[1] = Id;
  tx_data[2] = 0;
  tx_data[3] = Rate;
  tx_data[4] = 0;
  tx_data[5] = 0;
  tx_data[6] = 0;
  tx_data[7] = 0;

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_MSG_ID, tx_data, 8);
}

void UBLOX_PollMessage(unsigned char Class, unsigned char Id)
{
  unsigned char tx_data[2];


  tx_data[0] = Class;
  tx_data[1] = Id;

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_MSG_ID, tx_data, 2);
}

void UBLOX_ConfigureGNSS_Sytem(void)
{
  unsigned char tx_data[60];


  tx_data[0] = 0;         // msgVer
  tx_data[1] = 0;         // numTrkChHw
  tx_data[2] = 0xFF;      // numTrkChUse
  tx_data[3] = 7;         // numConfigBlocks
  // GPS
  tx_data[4] = 0;         // gnssId
  tx_data[5] = 0;         // resTrkCh
  tx_data[6] = 16;        // maxTrkCh
  tx_data[7] = 0;         // reserved1
  tx_data[8] = 1;         // flags - byte 0
  tx_data[9] = 0;         // flags - byte 1
  tx_data[10] = 1;        // flags - byte 2
  tx_data[11] = 0;        // flags - byte 3
  // SBAS
  tx_data[12] = 1;        // gnssId
  tx_data[13] = 0;        // resTrkCh
  tx_data[14] = 0;        // maxTrkCh
  tx_data[15] = 0;        // reserved1
  tx_data[16] = 0;        // flags - byte 0
  tx_data[17] = 0;        // flags - byte 1
  tx_data[18] = 0;        // flags - byte 2
  tx_data[19] = 0;        // flags - byte 3
  // Galileo
  tx_data[20] = 2;        // gnssId
  tx_data[21] = 0;        // resTrkCh
  tx_data[22] = 0;        // maxTrkCh
  tx_data[23] = 0;        // reserved1
  tx_data[24] = 0;        // flags - byte 0
  tx_data[25] = 0;        // flags - byte 1
  tx_data[26] = 0;        // flags - byte 2
  tx_data[27] = 0;        // flags - byte 3
  // BeiDou
  tx_data[28] = 3;        // gnssId
  tx_data[29] = 0;        // resTrkCh
  tx_data[30] = 0;        // maxTrkCh
  tx_data[31] = 0;        // reserved1
  tx_data[32] = 0;        // flags - byte 0
  tx_data[33] = 0;        // flags - byte 1
  tx_data[34] = 0;        // flags - byte 2
  tx_data[35] = 0;        // flags - byte 3
  // IMES
  tx_data[36] = 4;        // gnssId
  tx_data[37] = 0;        // resTrkCh
  tx_data[38] = 0;        // maxTrkCh
  tx_data[39] = 0;        // reserved1
  tx_data[40] = 0;        // flags - byte 0
  tx_data[41] = 0;        // flags - byte 1
  tx_data[42] = 0;        // flags - byte 2
  tx_data[43] = 0;        // flags - byte 3
  // QZSS
  tx_data[44] = 5;        // gnssId
  tx_data[45] = 0;        // resTrkCh
  tx_data[46] = 4;        // maxTrkCh
  tx_data[47] = 0;        // reserved1
  tx_data[48] = 1;        // flags - byte 0
  tx_data[49] = 0;        // flags - byte 1
  tx_data[50] = 1;        // flags - byte 2
  tx_data[51] = 0;        // flags - byte 3
  // Glonass
  tx_data[52] = 6;        // gnssId
  tx_data[53] = 0;        // resTrkCh
  tx_data[54] = 12;       // maxTrkCh
  tx_data[55] = 0;        // reserved1
  tx_data[56] = 1;        // flags - byte 0
  tx_data[57] = 0;        // flags - byte 1
  tx_data[58] = 1;        // flags - byte 2
  tx_data[59] = 0;        // flags - byte 3

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_GNSS_ID, tx_data, 60);
}

static void UBLOX_ConfigureNAV(void)
{
  sUBLOX_NAV_Settings nav_settings;


  memset(&nav_settings, 0, sizeof(sUBLOX_NAV_Settings));

  nav_settings.ApplySettingMask = 0x0507;
  nav_settings.DynPlatformModal = 8;          // Airborne with <4g acceleration
  nav_settings.PosFixingMode = 2;             // 3D only
  nav_settings.MinSatElev = 10;
  nav_settings.CNO_ThreshNumSV = 4;
  nav_settings.CNO_Thresh = 25;
  nav_settings.UTC_Standard = 3;              // GPS time

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_NAV5_ID, (unsigned char *) &nav_settings, sizeof(sUBLOX_NAV_Settings));
}

void UBLOX_SetRate(void)
{
  unsigned char tx_data[6];


  // Set the rates as follow:
  //  measRate = 500 ms,
  //  navRate = 1,
  //  timeRef = 1

  tx_data[0] = 0xF4;  // measRate LSB
  tx_data[1] = 0x01;  // measRate MSB
  tx_data[2] = 0x01;  // navRate LSB
  tx_data[3] = 0x00;  // navRate MSB
  tx_data[4] = 0x01;  // timeRef LSB
  tx_data[5] = 0x00;  // timeRef MSB

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_RATE_ID, tx_data, 6);
}

void UBLOX_RequestVersion(void)
{
  UBLOX_SendMessage(UBLOX_CLASS_MON, UBLOX_MON_VER_ID, NULL, 0);
}

eUBLOX_ACK_State UBLOX_WaitForACK(void)
{
  unsigned int index;


  UBLOX_ACK_State = ublox_ackWAITING;

  for (index = 0; index < 5; index++) {
    Delay(20000);
    UBLOX_Service();

    if (UBLOX_ACK_State == ublox_ackACK) {
      return ublox_ackACK;
    }
    else if (UBLOX_ACK_State == ublox_ackNACK) {
      return ublox_ackNACK;
    }
  }

  return ublox_ackWAITING;
}

static void UBLOX_ConfigurePort(void)
{
  sUBLOX_PortSettings port;


  port.PortID               = 0x01;
  port.Reserved1            = 0x00;
  port.TxReady              = 0x00;
  port.Mode.DWord           = 0;
  port.Mode.Bits.CharLength = 3;
  port.Mode.Bits.Parity     = 4;
  port.Mode.Bits.StopBits   = 0;
  port.Baudrate             = 460800;
  port.InProtoMask          = 0x0001;
  port.OutProtoMask         = 0x0001;
  port.Flags                = 0x0000;
  port.Reserved2            = 0x0000;

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_PRT_ID, (unsigned char *) &port, sizeof(sUBLOX_PortSettings));
  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_PRT_ID, (unsigned char *) &port, sizeof(sUBLOX_PortSettings));
  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_PRT_ID, (unsigned char *) &port, sizeof(sUBLOX_PortSettings));
}

static void UBLOX_ResetConfig(void)
{
  sUBLOX_Config config;


  config.ClearMask  = 0x1F1F;
  config.SaveMask   = 0x0000;
  config.LoadMask   = 0x1F1F;
  config.DeviceMask = 0x00;

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_CFG_ID, (unsigned char *) &config, sizeof(sUBLOX_Config));
}

static void UBLOX_SaveConfiguration(void)
{
  sUBLOX_Config config;


  config.ClearMask  = 0x0000;
  config.SaveMask   = 0x1F1F;
  config.LoadMask   = 0x0000;
  config.DeviceMask = 0x16;        // Save to flash

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_CFG_ID, (unsigned char *) &config, sizeof(sUBLOX_Config));
}

unsigned char UBLOX_Configure(void)
{
  int index;
  int msg_index = 0;
  eUBLOX_ACK_State ack_result;


  DisableInterrupts();

  // Clear receive buffer
  UBLOX_Service();

  UBLOX_ResetConfig();
  Delay(200000);
  UBLOX_Service();
  UART_SetBaudRate((sUART *) &UBLOX_UART, br9600);
  Delay(200000);
  UBLOX_ConfigurePort();
  Delay(200000);
  UART_SetBaudRate((sUART *) &UBLOX_UART, br460800);
  Delay(200000);

  UBLOX_Service();

  for (index = 0; index < 3; index++) {
    switch (msg_index) {
      case 0 : UBLOX_ConfigureGNSS_Sytem(); break;
      case 1 : UBLOX_ConfigureNAV(); break;
      case 2 : UBLOX_SetRate(); break;
      case 3 : UBLOX_SetMessageRate(UBLOX_CLASS_TIM, UBLOX_TIM_TP_ID, 1); break;
      case 4 : UBLOX_SetMessageRate(UBLOX_CLASS_RXM, UBLOX_RXM_SFRBX_ID, 1); break;
      case 5 : UBLOX_SetMessageRate(UBLOX_CLASS_RXM, UBLOX_RXM_RAWX_ID, 1); break;
      case 6 : UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_POSECEF_ID, 1); break;
      case 7 : UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_POSLLH_ID, 1); break;
      case 8 : UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_DOP_ID, 1); break;
      case 9 : UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_VELECEF_ID, 1); break;
      case 10: UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_TIME_GPS_ID, 1); break;
      case 11: UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_TIME_GLO_ID, 1); break;
      case 12: UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_SAT_ID, 1); break;
      case 13: UBLOX_SetMessageRate(UBLOX_CLASS_NAV, UBLOX_NAV_SOL, 1); break;
      case 14: UBLOX_SaveConfiguration(); break;
      case 15: {
        EnableInterrupts();
        return TRUE;
      }
      default : return FALSE;
    }

    ack_result = UBLOX_WaitForACK();

    // Verify result: If the message was acknowledged, then send the next message by incrementing msg_index and resetting index.
    // If a NACK was received, return with error. Resend message up to 3 times if no ACK or NACK was received.
    if (ack_result == ublox_ackACK) {
      index = -1;
      msg_index++;
    }
    else if (ack_result == ublox_ackNACK) {
      break;
    }

    MCP_KickWatchdog();
  }

  EnableInterrupts();

  return FALSE;
}

unsigned char UBLOX_ConfigureReceiver(unsigned char UseGPS, unsigned char UseGlonass)
{
  unsigned char tx_data[28];
  eUBLOX_ACK_State ack_result;


  UseGPS = UseGPS & 0x01;
  UseGlonass = UseGlonass & 0x01;

  tx_data[0] = 0;                     // msgVer
  tx_data[1] = 0;                     // numTrkChHw
  tx_data[2] = 0xFF;                  // numTrkChUse
  tx_data[3] = 3;                     // numConfigBlocks
  // GPS
  tx_data[4] = 0;                     // gnssId
  tx_data[5] = 0;                     // resTrkCh
  tx_data[6] = 16 * UseGPS;           // maxTrkCh
  tx_data[7] = 0;                     // reserved1
  tx_data[8] = UseGPS;                // flags - byte 0
  tx_data[9] = 0;                     // flags - byte 1
  tx_data[10] = 1;                    // flags - byte 2
  tx_data[11] = 0;                    // flags - byte 3
  // QZSS
  tx_data[12] = 5;                    // gnssId
  tx_data[13] = 0;                    // resTrkCh
  tx_data[14] = 4 * UseGPS;           // maxTrkCh
  tx_data[15] = 0;                    // reserved1
  tx_data[16] = UseGPS;               // flags - byte 0
  tx_data[17] = 0;                    // flags - byte 1
  tx_data[18] = 1;                    // flags - byte 2
  tx_data[19] = 0;                    // flags - byte 3
  // Glonass
  tx_data[20] = 6;                    // gnssId
  tx_data[21] = 0;                    // resTrkCh
  tx_data[22] = 12 * UseGlonass;      // maxTrkCh
  tx_data[23] = 0;                    // reserved1
  tx_data[24] = UseGlonass;           // flags - byte 0
  tx_data[25] = 0;                    // flags - byte 1
  tx_data[26] = 1;                    // flags - byte 2
  tx_data[27] = 0;                    // flags - byte 3

  UBLOX_SendMessage(UBLOX_CLASS_CFG, UBLOX_CFG_GNSS_ID, tx_data, 28);

  ack_result = UBLOX_WaitForACK();

  if (ack_result == ublox_ackACK) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
