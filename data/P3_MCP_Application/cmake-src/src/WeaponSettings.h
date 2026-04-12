/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WeaponSettings.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Weapon Settings module.
 *
 ******************************************************************************/

#ifndef WEAPON_SETTINGS_H_
#define WEAPON_SETTINGS_H_

#include "Mission.h"

#define DATA_FILE_STRUCT_ID             1
#define DATA_FILE_BLOCK_START_ID        0x12345678

typedef enum
{
  weaponAL_TARIQ,
  weaponP3
} eWeaponType;

typedef enum
{
  bombMK81,
  bombMK82,
  bombMK83,
  bombMK84
} eBombType;

typedef enum
{
  seekerNONE,
  seekerSAL,
  seekerIIR
} eSeekerType;

typedef enum
{
  baANY,
  baAIRCRAFT,
  baWIRELESS_CONTROLLER
} eBatteryActivation;


typedef struct
{
  char PartNo[10];
  char IssueNo[3];
  char SerialNo[8];
} sLog;

typedef struct
{
  unsigned short PowerCycleCnt;
  unsigned char UBLOX_Configured;
  sLog Log;
} sMCP_Log;

typedef struct
{
  unsigned char WeaponType;
  unsigned char BombType;
  unsigned char SeekerType;
  unsigned char HOBS_Present;
  unsigned char Spare;
  unsigned char GuidanceAndControlMode;
  unsigned char DTAC_Present;
  unsigned char BatteryActivation;
} sWeaponConfig;


typedef struct
{
  unsigned char MCP_LogValid;
  unsigned char WeaponConfigValid;
  unsigned char RawMissionValid;
  unsigned char WindValid;
  unsigned char Spare;
  unsigned char DataFileLoaded;
  unsigned char DataFileValid;
  unsigned char DataFileStructId;
  unsigned short DataFileVersion;
  unsigned short DataFileCRC;
  unsigned char WeaponDefIndex;
  unsigned char WeaponDefBombType;
  unsigned short WeaponDefVersion;
  unsigned int WeaponDefCRC;
  unsigned int WeaponDefSize;
  unsigned int WeaponDefSizeInDataFileHeader;
  unsigned int WeaponDefCalculatedCRC;
  unsigned int WeaponDefStartID;
  unsigned char WeaponDefWeaponType;
  unsigned char Spare1;
  unsigned short Spare2;
  sMCP_Log MCP_Log;
  sWeaponConfig WeaponConfig;
  sRawMission RawMission;
  sWind Wind;
} sWeaponSettings;

typedef struct
{
  unsigned short Id;
  unsigned short CRC;
} sWeaponSettingsHeader;

typedef struct
{
  unsigned int CRC;
  unsigned short Version;
  unsigned char StructId;           // This ID will increment if the structure changes
  unsigned char Spare;
  unsigned int StructureSize;
} sDataFileHeader;



extern sWeaponSettings WeaponSettings;

void WeaponSettingsInit(void);
unsigned char  SaveMCP_Log(sMCP_Log *MCP_Log);
unsigned char  SaveWeaponConfig(sWeaponConfig *WeaponConfig);
unsigned char  SaveRawMission(sRawMission *RawMission);
unsigned char  SaveWind(sWind *Wind);
void LoadWeaponData(char *AuxStr);

#endif /* WEAPON_SETTINGS_H_ */


