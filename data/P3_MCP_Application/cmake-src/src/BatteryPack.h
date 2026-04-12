/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <BatteryPack.h>
 *
 *  Description:
 *  ------------
 *  Header file for the BatteryPack module.
 *
 ******************************************************************************/

#ifndef BATTERY_PACK_H
#define BATTERY_PACK_H

#define BATTERY_MAX_ON_TIME             5040  // [s] (= 84 minutes)

#define BATTERY_PACK_TEMP_SENSOR_CNT    8


typedef enum
{
  batSET_CONTROL_MODE        = 0x00, // SET HEATER CONTROL MODE (0-Auto 1-On 2-Off)
  batSET_CONTROL_TEMPERATURE = 0x55, // SET CONTROL TEMPERATURE (-128 .. 127)
  batSET_BATTERY_USAGE       = 0xAA, // CUMULATIVE BATTERY USAGE (0..10)
  batSET_NUMBER_ACTIVATIONS  = 0xBB, // NUMBER OF BATTERY ACTIVATIONS (0..255)
  batSET_CAPACITY            = 0xCC  // SET TOTAL BATTERY CAPACITY (0..10)
} eBatteryPackCommands;

typedef enum
{
  btcAUTO,
  btcON,
  btcOFF
} eBatteryPackTemperatureControl;

#pragma pack(1)

typedef union
{
  unsigned short Word;
  struct
  {
    unsigned short BatteryConfig_Fail : 1;  // Bit 0
    unsigned short Spare0             : 1;  // Bit 1
    unsigned short Spare1             : 1;  // Bit 2
    unsigned short Spare2             : 1;  // Bit 3
    unsigned short Spare3             : 1;  // Bit 4
    unsigned short Spare4             : 1;  // Bit 5
    unsigned short Spare5             : 1;  // Bit 6
    unsigned short Spare6             : 1;  // Bit 7
    unsigned short Spare7             : 1;  // Bit 8
    unsigned short Spare8             : 1;  // Bit 9
    unsigned short Spare9             : 1;  // Bit 10
    unsigned short Spare10            : 1;  // Bit 11
    unsigned short Spare11            : 1;  // Bit 12
    unsigned short Spare12            : 1;  // Bit 13
    unsigned short Spare13            : 1;  // Bit 14
    unsigned short Spare15            : 1;  // Bit 15
  } Bits;
} sBatteryPackSBIT;

typedef struct
{
  char PartNoLeft[5];
  char PartNoRight[5];
  char IssueNo[3];
  char SerialNo[8];
  unsigned char ManufacturedYear;
  unsigned char ManufacturedMonth;
  unsigned char ManufacturedDay;
  unsigned char LastTestYear;
  unsigned char LastTestMonth;
  unsigned char LastTestDay;
  unsigned char TestCnt;
  unsigned char Spares[18];
  unsigned short CRC;
} sBatteryPackLog;

typedef struct
{
  TIME RxTimestamp;
  unsigned int TxCnt;
  unsigned int RxCnt;
  unsigned int ErrCnt;
  float Capacity;
  float CumulativeUsage;
  float AverageCurrent;
  unsigned char HeaterCtrl;
  unsigned char CurrentHeaterState;
  signed char TemperatureSetPoint;
  unsigned char ActivationCnt;
  unsigned char RawCapacity;
  unsigned short RawCumulativeUsage;
  signed char Temperature1;
  signed char Temperature2;
  signed char Temperature3;
  signed char Temperature4;
  signed char Temperature5;
  signed char Temperature6;
  signed char Temperature7;
  signed char Temperature8;
  unsigned char RawAveCurrent_0_10ms;
  unsigned char RawAveCurrent_10_20ms;
  unsigned char RawAveCurrent_20_30ms;
  unsigned char RawAveCurrent_30_40ms;
  unsigned char RawAveCurrent_40_50ms;
  unsigned char LogReportValid;
  sBatteryPackLog LogReport;
  unsigned char BatteryWaitForRxMsg;
  unsigned char isBatteryConfigChecked;
  sBatteryPackSBIT SBIT;
} sBatteryPack;

typedef struct
{
	char 						AuxStr[AUX_FIFO_SIZE];		//
} sBATTERY_StateSpace;

#pragma pack()

extern sBATTERY_StateSpace BATT_SS;

extern sBatteryPack BatteryPack;

void BatteryPackInit(char *AuxStr);
void BatteryPackSetCommand(eBatteryPackCommands CommandId, unsigned char CommandValue);
void BatteryPackSetLog(sBatteryPackLog *Log);
void BatteryPackRequestLog(void);
void BatteryPackSendMsg(void);
void BatteryPackService(void);


#endif
