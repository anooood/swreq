/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MCP.h>
 *
 *  Description:
 *  ------------
 *  Header file for the MCP Module.
 *
 ******************************************************************************/

#ifndef MCP_H
#define MCP_H


#include "UART.h"
#include "FIFO.h"

#define VERSION                     31     // Ver 0.31
#define GUIDANCE_SVN_REVISION       2668

#define MCP_FW_FLASH_ADDR           0x00020000
#define MCP_FW_META_FLASH_ADDR      0x003FFFF0
#define MCP_SW_FLASH_ADDR           0x00400000
#define MCP_SW_META_FLASH_ADDR      0x007FFFF0
#define MCP_DATA_FLASH_ADDR         0x00C00000
#define MCP_DATA_META_FLASH_ADDR    0x01FFFFF0


typedef enum
{
  antREAR,
  antFRONT
} eGNSS_Antenna;

typedef enum
{
  pressNONE,
  pressFRONT,
  pressREAR
} ePressureSensor;


typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char GPS_NotUsed     : 1;
    unsigned char GlonassNotUsed  : 1;
    unsigned char Reserved        : 6;
  };
} uGNSS_Usage;

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char LED1_On                 : 1;
    unsigned char LED2_On                 : 1;
    unsigned char LED3_On                 : 1;
    unsigned char LED4_On                 : 1;
  };
} uLED_Control;

typedef struct
{
  unsigned int Count;
  float StaticPressure;
} sHILS_ADS_RxMsg;

typedef struct
{
  unsigned int STC1_State                 : 3;  // 0 - 2
  unsigned int STC1_100Hz_Wave            : 1;  // 3
  unsigned int STC2_State                 : 3;  // 4 - 6
  unsigned int STC2_50Hz_Wave             : 1;  // 7
  unsigned int Weapon28V_Present          : 1;  // 8
  unsigned int LaunchLinkPresent          : 1;  // 9
  unsigned int ActivateWeapon             : 1;  // 10
  unsigned int LanyardPulled              : 1;  // 11
  unsigned int BatterySwitchedOn          : 1;  // 12
  unsigned int LanyardPresent             : 1;  // 13
  unsigned int Reserved1                  : 2;  // 14 - 15
  unsigned int DownloadCablePresent       : 1;  // 16
  unsigned int PCB_ModificationStatus     : 3;  // 17 - 19
  unsigned int PCB_VersionStatus          : 4;  // 20 - 23
  unsigned int GCA_Mode                   : 3;  // 24 - 26
  unsigned int HILS_Mode                  : 1;  // 27
  unsigned int GNSS_FrontAntennaSelected  : 1;  // 28
  unsigned int GNSS_RearAntennaSelected   : 1;  // 29
  unsigned int Reserved2                  : 2;  // 30 - 31
} sMCP_StatusRegister;

typedef struct
{
  unsigned int STC1_WaitForLaunch         : 1;  // 0
  unsigned int STC1_SAD_Active            : 1;  // 1
  unsigned int STC1_Control               : 2;  // 2 - 3
  unsigned int STC2_WaitForLaunch         : 1;  // 4
  unsigned int STC2_SAD_Active            : 1;  // 5
  unsigned int STC2_Control               : 2;  // 6 - 7
  unsigned int SwitchOnBattery            : 1;  // 8
  unsigned int GNSS_Antenna               : 1;  // 9       0 = rear, 1 = front
  unsigned int Reserved1                  : 6;  // 10
  unsigned int LED_1_On                   : 1;  // 16
  unsigned int LED_2_On                   : 1;  // 17
  unsigned int LED_3_On                   : 1;  // 18
  unsigned int LED_4_On                   : 1;  // 19
  unsigned int Reserved2                  : 12; // 20 - 31
} sMCP_ControlRegister;

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short PSU_5V_Fail          : 1;  // Bit 0
    unsigned short PSU_1V_Fail          : 1;  // Bit 1
    unsigned short PSU_1V35_Fail        : 1;  // Bit 2
    unsigned short PSU_1V8_Fail         : 1;  // Bit 3
    unsigned short PSU_3V3_Fail         : 1;  // Bit 4
    unsigned short PSU_28V_Fail         : 1;  // Bit 5
    unsigned short PSU_IMU_5V_Fail      : 1;  // Bit 6
    unsigned short Reserved1            : 1;  // Bit 7
    unsigned short TemperatureFail      : 1;  // Bit 8
    unsigned short ArcnetMyReconFail    : 1;  // Bit 9
    unsigned short ExecutionOverrunFail : 1;  // Bit 10
    unsigned short StackOverrun         : 1;  // Bit 11
    unsigned short Reserved2            : 3;  // Bit 12-14
    unsigned short CriticalFail         : 1;  // Bit 15
  } Bits;
} sMCP_BIT;

typedef struct
{
  TIME PressSensTimestamp;
  unsigned long long SerialNo;
  unsigned int PLD_StatusReg;
  unsigned int ETI;
  unsigned short SW_CRC;
  unsigned short SW_Version;
  unsigned short FW_CRC;
  unsigned short FW_Version;
  unsigned char PressureSensToUse;
  unsigned char I2C_Updated;
  sMCP_BIT SBIT;
  sMCP_BIT CBIT;
  float Temperature;
  float PSU_5V;
  float PSU_1V;
  float PSU_1V35;
  float PSU_1V8;
  float PSU_3V3;
  float PSU_28V;
  float PSU_GCA_6V;
  float PSU_IMU_5V;
  float PSU_EFUZE_5V;
  float PressSensPressure;
  float PressSensTemperature;
  unsigned int DebugInt[10];
  float DebugFloat[10];
} sMCP;

typedef struct
{
	TIME StartTime;
} sMCP_ServiceControl;

typedef struct
{
	sMCP_ServiceControl ServiceControls;
	char 				AuxStr[AUX_FIFO_SIZE]; //Validation Control
} sMCP_StateSpace;

extern sMCP_StateSpace MCP_SS;
extern sMCP MCP;
extern unsigned int HILS_ADS_RxMsgCnt;
extern sHILS_ADS_RxMsg HILS_ADS_RxMsg;
extern sUART HILS_ADS_RxUART;


void MCP_Init(char *AuxStr);
void MCP_KickWatchdog(void);
void MCP_Reset(void);
void MCP_ReadTemperature(void);
void MCP_UpdateVoltages(void);
void MCP_UpdatePressureSensorData(void);
void MCP_WeaponBatteryCtrl(unsigned char BatteryOn);
void MCP_SetWaitForLaunch(unsigned char On);
void MCP_SelectGNSS_Antenna(eGNSS_Antenna GNSS_Antenna);
void MCP_SetLEDs(uLED_Control LEDs);
void MCP_ServiceHILS_ADS(void);

#endif
