/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WeaponSettings.c>
 *
 *  Description:
 *  ------------
 *  The Weapon Settings module manages the configuration of the weapon system.
 *
 *  Function(s):
 *  ------------
 *  - WeaponSettingsInit                       : Initializes weapon settings
 *  - SaveMCP_Log                              : Saves the MCP log data in NVRAM
 *  - SaveWeaponConfig                         : Saves the weapon config data in NVRAM
 *  - SaveRawMission                           : Saves the raw mission data in NVRAM
 *  - SaveWind                                 : Saves the wind data in NVRAM
 *  - GetWeaponDefinitionIndex                 : Determine the weapon definition (see definitions below)
 *                                               Weapon Definitions
 *                                               0: P3     Mk81 S SAL&GPS
 *                                               1: P3     Mk82 S SAL&GPS
 *                                               2 - 15:   Spare
 *                                               16 - 255: Invalid
 *  - LoadWeaponData                           : Load weapon data file from flash memory
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
#include "Wind.h"
#include "NVRAM.h"
#include "CRC.h"
#include "UART.h"
#include "FIFO.h"
#include "WIAC82_Comms.h"
#include "Timer.h"
#include "Fuze.h"
#include "UBLOX.h"
#include "MCP.h"
#include "AircraftInterface.h"
#include "Flash.h"
#include "Weapon.h"
#include "DataControl_10Hz.h"
#include "DataControl_10Hz_Function.h"
#include "WeaponSettings.h"
#include "GCA_Config.h"

#define NVRAM_MCP_LOG_ADDR              0x0000
#define NVRAM_WEAPON_CONFIG_ADDR        0x0200
#define NVRAM_MISSION_ADDR              0x0400
#define NVRAM_WIND_ADDR                 0x0600

#define WEAPON_SETTINGS_ID              0x1234

sWeaponSettings WeaponSettings;

void WeaponSettingsInit(void)
{
  unsigned short crc;
  sWeaponSettingsHeader header;
  sMCP_Log mcp_log;
  sWeaponConfig weapon_config;
  sRawMission raw_mission;
  sWind wind;


  memset(&WeaponSettings, 0, sizeof(WeaponSettings));

  WeaponSettings.WeaponConfig.WeaponType = weaponP3;
  WeaponSettings.WeaponConfig.BombType = bombMK82;
  WeaponSettings.WeaponConfig.HOBS_Present = FALSE;
  WeaponSettings.WeaponConfig.BatteryActivation = baANY;

  if (NVRAM_Read(NVRAM_MCP_LOG_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader))) {
    if (NVRAM_Read((NVRAM_MCP_LOG_ADDR + sizeof(sWeaponSettingsHeader)), (unsigned char *) &mcp_log, sizeof(sMCP_Log))) {
      crc = Calculate16BitCRC((unsigned char *) &mcp_log, sizeof(sMCP_Log));

      if ((header.Id == WEAPON_SETTINGS_ID) && (header.CRC == crc)) {
        WeaponSettings.MCP_LogValid = TRUE;
        WeaponSettings.MCP_Log = mcp_log;
      }
    }
  }

  if (NVRAM_Read(NVRAM_WEAPON_CONFIG_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader))) {
    if (NVRAM_Read((NVRAM_WEAPON_CONFIG_ADDR + sizeof(sWeaponSettingsHeader)), (unsigned char *) &weapon_config, sizeof(sWeaponConfig))) {
      crc = Calculate16BitCRC((unsigned char *) &weapon_config, sizeof(sWeaponConfig));

      if ((header.Id == WEAPON_SETTINGS_ID) && (header.CRC == crc)) {
        WeaponSettings.WeaponConfigValid = TRUE;
        WeaponSettings.WeaponConfig = weapon_config;
        WeaponSettings.WeaponConfig.WeaponType = weaponP3;
      }
    }
  }

  if (NVRAM_Read(NVRAM_MISSION_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader))) {
    if (NVRAM_Read((NVRAM_MISSION_ADDR + sizeof(sWeaponSettingsHeader)), (unsigned char *) &raw_mission, sizeof(sRawMission))) {
      crc = Calculate16BitCRC((unsigned char *) &raw_mission, sizeof(sRawMission));

      if ((header.Id == WEAPON_SETTINGS_ID) && (header.CRC == crc)) {
        WeaponSettings.RawMissionValid = TRUE;
        WeaponSettings.RawMission = raw_mission;
        MissionSet(&raw_mission, msEEPROM);
      }
    }
  }

  if (NVRAM_Read(NVRAM_WIND_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader))) {
    if (NVRAM_Read((NVRAM_WIND_ADDR + sizeof(sWeaponSettingsHeader)), (unsigned char *) &wind, sizeof(sWind))) {
      crc = Calculate16BitCRC((unsigned char *) &wind, sizeof(sWind));

      if ((header.Id == WEAPON_SETTINGS_ID) && (header.CRC == crc)) {
        WeaponSettings.WindValid = TRUE;
        WeaponSettings.Wind = wind;
        WindSet(&wind);
      }
    }
  }

  WeaponSettings.MCP_Log.PowerCycleCnt++;

  SaveMCP_Log(&WeaponSettings.MCP_Log);

  WeaponSettings.DataFileLoaded = FALSE;
  WeaponSettings.DataFileValid = FALSE;
  WeaponSettings.WeaponDefIndex = 0xFF;    // Set to an invalid index
}

unsigned char SaveMCP_Log(sMCP_Log *MCP_Log)
{
  sWeaponSettingsHeader header;


  header.Id = WEAPON_SETTINGS_ID;
  header.CRC = Calculate16BitCRC((unsigned char *) MCP_Log, sizeof(sMCP_Log));

  if (NVRAM_Write(NVRAM_MCP_LOG_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader), TRUE)) {
    if (NVRAM_Write(NVRAM_MCP_LOG_ADDR + sizeof(sWeaponSettingsHeader), (unsigned char *) MCP_Log, sizeof(sMCP_Log), FALSE)) {
      return TRUE;
    }
  }

  return FALSE;
}

unsigned char  SaveWeaponConfig(sWeaponConfig *WeaponConfig)
{
  sWeaponSettingsHeader header;


  header.Id = WEAPON_SETTINGS_ID;
  header.CRC = Calculate16BitCRC((unsigned char *) WeaponConfig, sizeof(sWeaponConfig));

  if (NVRAM_Write(NVRAM_WEAPON_CONFIG_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader), TRUE)) {
    if (NVRAM_Write(NVRAM_WEAPON_CONFIG_ADDR + sizeof(sWeaponSettingsHeader), (unsigned char *) WeaponConfig, sizeof(sWeaponConfig), FALSE)) {
      WeaponSettings.WeaponConfig = *WeaponConfig;
      WeaponSettings.WeaponConfigValid = FALSE; // Ensure that a CBIT fail is generated to force the user to recycle power
      return TRUE;
    }
  }

  return FALSE;
}

unsigned char SaveRawMission(sRawMission *RawMission)
{
  sWeaponSettingsHeader header;


  header.Id = WEAPON_SETTINGS_ID;
  header.CRC = Calculate16BitCRC((unsigned char *) RawMission, sizeof(sRawMission));

  if (NVRAM_Write(NVRAM_MISSION_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader), TRUE)) {
    if (NVRAM_Write(NVRAM_MISSION_ADDR + sizeof(sWeaponSettingsHeader), (unsigned char *) RawMission, sizeof(sRawMission), FALSE)) {
      return TRUE;
    }
  }

  return FALSE;
}

unsigned char SaveWind(sWind *Wind)
{
  sWeaponSettingsHeader header;


  header.Id = WEAPON_SETTINGS_ID;
  header.CRC = Calculate16BitCRC((unsigned char *) Wind, sizeof(sWind));

  if (NVRAM_Write(NVRAM_WIND_ADDR, (unsigned char *) &header, sizeof(sWeaponSettingsHeader), TRUE)) {
    if (NVRAM_Write(NVRAM_WIND_ADDR + sizeof(sWeaponSettingsHeader), (unsigned char *) Wind, sizeof(sWind), FALSE)) {
      return TRUE;
    }
  }

  return FALSE;
}

unsigned char GetWeaponDefinitionIndex(unsigned char BombType)
{
  unsigned char index = 0xFF;


  if (BombType <= bombMK82) {
    index = BombType;
  }

  return index;
}

void LoadWeaponData(char *AuxStr)
{
  unsigned char index;
  unsigned int addr;
  unsigned int crc32;
  sDataFileHeader data_file_header;
  sLoaderMetaData meta_data;


  // Invalidate data file
  WeaponSettings.DataFileValid = FALSE;

  // Get weapon definition index
  index = GetWeaponDefinitionIndex(WeaponSettings.WeaponConfig.BombType);

  WeaponSettings.WeaponDefIndex = index;

  FlashReadData(MCP_DATA_META_FLASH_ADDR, (unsigned char *) &meta_data, sizeof(sLoaderMetaData));
  WeaponSettings.DataFileVersion = meta_data.Version;
  WeaponSettings.DataFileCRC = meta_data.CRC;
  rtP=(P*)&weapon_config_data;
  //default data crc check
//  crc32 = Calculate32BitCRC((unsigned char *) &rtP->MCP_Global.WeaponType, sizeof(P) - 8);
//  if (crc32 != rtP->MCP_Global.SubCheckSum)
//  {
//	  int test=0;
//  }

  //uncomment for data file debugging
//  WeaponSettings.WeaponConfigValid=1;
//  index=0;

  if (index != 0xFF) {
    // Read data file header from flash
    FlashReadData(MCP_DATA_FLASH_ADDR, (unsigned char *) &data_file_header, sizeof(sDataFileHeader));

    //WeaponSettings.DataFileVersion = data_file_header.Version;
    WeaponSettings.DataFileStructId = data_file_header.StructId;

    if (WeaponSettings.WeaponConfigValid && (data_file_header.StructId == DATA_FILE_STRUCT_ID) && (data_file_header.StructureSize == sizeof(P))) {
      // Calculate flash start address
      addr = MCP_DATA_FLASH_ADDR + sizeof(sDataFileHeader) + (index * sizeof(P));

      // Read weapon data from flash
      FlashReadData(addr, (unsigned char *) rtP, sizeof(P));

      // Calculate 32-bit CRC
      crc32 = Calculate32BitCRC((unsigned char *) &rtP->MCP_Global.WeaponType, sizeof(P) - 8);

      // Verify weapon data
      if ((crc32 == rtP->MCP_Global.SubCheckSum)                     &&
          (rtP->MCP_Global.BegOfBlockID == DATA_FILE_BLOCK_START_ID) &&
          (rtP->MCP_Global.WeaponType == weaponP3)                   &&
          (rtP->MCP_Global.BombType == WeaponSettings.WeaponConfig.BombType)) {
        WeaponSettings.DataFileValid = TRUE;
        WeaponSettings.WeaponDefCRC = crc32;
        WeaponSettings.WeaponDefVersion = rtP->MCP_Global.SubVerNumber;
        WeaponSettings.WeaponDefBombType = WeaponSettings.WeaponConfig.BombType;
        LoadGCA_Config(AuxStr);

        sprintf(AuxStr,"%sWEAPON SETTINGS: GCA is loaded with CONFIG DATA FILE - Version: %d.%02d CRC: 0x%x\n",AuxStr,
        		(WeaponSettings.DataFileVersion/100),(WeaponSettings.DataFileVersion%100),WeaponSettings.DataFileCRC);
      }
    }
  }

  WeaponSettings.DataFileLoaded = TRUE;
}
