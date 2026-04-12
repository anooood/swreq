/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WiFi.h>
 *
 *  Description:
 *  ------------
 *  Header file for the WiFi module.
 *
 ******************************************************************************/

#ifndef WIFI_H
#define WIFI_H

// WiFi MLAN Op Codes
#define WIFI_ID_SBIT_REQ                (unsigned short)(0x00)
#define WIFI_ID_SBIT                    (unsigned short)(0x01)
#define WIFI_ID_CLEAR_CBIT              (unsigned short)(0x04)
#define WIFI_ID_BATTERY_CTRL            (unsigned short)(0x07)
#define WIFI_ID_GNSS_USAGE              (unsigned short)(0x08)
#define WIFI_ID_MISSION_REQ             (unsigned short)(0x10)
#define WIFI_ID_WIND_REQ                (unsigned short)(0x11)
#define WIFI_ID_CLEAR_MISSION           (unsigned short)(0x12)
#define WIFI_ID_HEALTH_STATUS           (unsigned short)(0x80)
#define WIFI_ID_WIFI_STATUS             (unsigned short)(0x81)
#define WIFI_ID_NEW_MISSION             (unsigned short)(0x82)
#define WIFI_ID_NEW_WIND                (unsigned short)(0x83)
#define WIFI_ID_ANTENNA_SELECTION       (unsigned short)(0x85)
#define WIFI_ID_MISSION_REPORT          (unsigned short)(0x86)
#define WIFI_ID_WIND_REPORT             (unsigned short)(0x87)
#define WIFI_ID_USER_LOG                (unsigned short)(0x90)
#define WIFI_ID_USER_LOG                (unsigned short)(0x90)
#define WIFI_ID_HOBS_MESSAGES           (unsigned short)(0xA5)
#define WIFI_ID_LOG_DATA_WRITE          (unsigned short)(0x5A)
#define WIFI_ID_LOG_DATA_REQ            (unsigned short)(0x5B)
#define WIFI_ID_LOG_DATA_REPORT         (unsigned short)(0x5C)

typedef union
{
  unsigned short Word;
  struct {
    unsigned short PIC_3p3V_PowerFail   : 1;
    unsigned short Main3p3V_PowerFail   : 1;
    unsigned short WiFiSettingsInvalid  : 1;
    unsigned short Spare1		        : 1;
    unsigned short ETI_DeviceErr        : 1;
    unsigned short TempSensorErr        : 1;
    unsigned short Spare2               : 9;
    unsigned short Busy                 : 1;
  } Bits;
} uWiFiSBIT;

typedef union
{
  unsigned short Word;
  struct {
    unsigned short WiFiModuleFail       : 1;
    unsigned short PIC_3p3V_PowerFail   : 1;
    unsigned short Main3p3V_PowerFail   : 1;
    unsigned short TemeratureErr        : 1;
    unsigned short ArcnetCommsErr       : 1;
    unsigned short TabletCommsErr       : 1;
    unsigned short Spare                : 9;
    unsigned short CriticalFail         : 1;
  } Bits;
} uWiFiCBIT;

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char WiFiEnabled   : 1; // 0 = disabled, 1 = enabled
    unsigned char WiFiAntenna   : 1; // 0 = port, 1 = starboard
    unsigned char Spare         : 6;
  } Bits;
} uWirelessSettings;

typedef struct
{
  unsigned char WeaponId;
  unsigned char PageId;
  unsigned char ButtonCounter;
  unsigned char EventType;
  unsigned short ScreenPosX;
  unsigned short ScreenPosY;
  unsigned int Spare;
} sWiFiUserLogEntry;

typedef struct
{
  unsigned char WifiPartNumberLeft[5];
  unsigned char WifiPartNumberRight[5];
  unsigned char WifiIssueNumber[3];
  unsigned char WifiSerialNumber[8];
  unsigned char Spare1[11];
  unsigned char ISUPartNumberLeft[5];
  unsigned char ISUPartNumberRight[5];
  unsigned char ISUIssueNumber[3];
  unsigned char ISUSerialNumber[8];
  unsigned char Spare2[11];
} sWiFiLogData;

typedef struct
{
  TIME RxTimestamp;
//  unsigned int 					TxTestMsgCnt;
  unsigned int TxCnt;
  unsigned int RxCnt;
  unsigned int ErrCnt;
  unsigned short SW_Version;
  unsigned short SW_CRC;
  unsigned short PIC_3V3;
  unsigned short Main3V3;
  unsigned short ArcnetRxErrCnt;
  unsigned short WiFiRxErrCnt;
  unsigned char ArcnetMyReconCnt;
  unsigned char RSSI;
  signed char Temperature;
  unsigned char WiFiModuleState;
  unsigned char WeaponId;
  unsigned char ActivateWeapon;
  unsigned char Spare;
  uWirelessSettings Settings;
  uWiFiSBIT SBIT;
  uWiFiCBIT CBIT;
  sWiFiUserLogEntry UserLogEntries[8];
  sWiFiLogData LogData;
  char AuxStr[AUX_FIFO_SIZE];		//
} sWiFi;


extern sWiFi WiFi;

void WiFiInit(char *AuxStr);
void WiFiSend(void);
void WiFiSendClearCBIT(void);
void WiFiHandleRxData(unsigned char *RxData, unsigned short DataLength);
void WiFiSendLogData(void);

#endif
