/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <GCA_Config.h>
 *
 *  Description:
 *  ------------
 *  Header file for the GCA_Config module.
 *
 ******************************************************************************/

#ifndef GCA_CONFIG_H
#define GCA_CONFIG_H

#define GCA_STARTUP_TIMEOUT_TIME			300000      // 300 milli-second
#pragma pack(1)
typedef struct
{
  char GCAConfigData[0x1000];
  unsigned int CRC;
} sGCA_Config;
#pragma pack()
void PrintGCA_Config(unsigned short AuxOn);
void LoadGCA_Config(char *AuxStr);
char GenerateGCA_Config(void);
//void GenerateGCA_Config(char *AuxStr);
void InitGCA_SAL(char *AuxStr, unsigned short AuxOn);
void InitGCA_AircraftIF(char *AuxStr, unsigned short AuxOn);
void InitGCA_GNSS(char *AuxStr, unsigned short AuxOn);
void InitGCA_IMU(char *AuxStr, unsigned short AuxOn);

#endif //GCA_CONFIG_H
