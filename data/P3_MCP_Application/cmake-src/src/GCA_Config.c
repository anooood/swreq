/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <GCA_Config.c>
 *
 *  Description:
 *  ------------
 *  Configuration data of P3 Weapon using the AUX PORT.
 *
 *  Function(s):
 *  ------------
 *  - PrintGCAStr            : Prints a formatted GCA debug string to the diagnostic output.
 *  - PrintGCA_Config        : Outputs the current GCA configuration parameters for verification.
 *  - GetGcaCfg              : Retrieves the GCA configuration data.
 *  - LoadGCA_Config         : Loads the GCA configuration.
 *  - InitGCA_AircraftIF     : Initializes the GCA IF.
 *  - InitGCA_GNSS           : Initializes the GCA GNSS subsystem for positioning data.
 *  - InitGCA_IMU            : Initializes the GCA Inertial Measurement Unit subsystem.
 *  - GenerateGCA_Config     : Generates a default or updated GCA configuration set.
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
#include "ScheduleControl.h"
#include "Timer.h"
#include "MCP.h"
#include "MCP_Interface.h"
#include "CRC.h"
#include "WIAC82_Comms.h"
#include "FIFO.h"
#include "WeaponSettings.h"
#include "Weapon.h"
#include "SAL_Seeker.h"
#include "UART.h"
#include "MLAN.h"
#include "WiFi.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "MPSAL_Comms.h"
#include "MPSAL.h"
#include "QSAL.h"
#include "IMU.h"
#include "IF.h"
#include "UBLOX.h"
#include "HOBS.h"
#include "Servos.h"
#include "BatteryPack.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Main.h"
#include "GCA_Config.h"
#include "Flash.h"

sGCA_Config  GCA_Config;

void PrintGCAStr(char *cfgid,char * pAux,int iAuxStructSize)
{
	AUXsprintf("- %s",cfgid);
	for(int n=0;n<iAuxStructSize;n++)
	{
		AUXsprintf("%d",pAux[n]);
	}
	AUXsprintf("\n");

}

#define PrintGcaStr(x,y) PrintGCAStr(x,	    (char*)&y,	sizeof(y));

void PrintGCA_Config(unsigned short AuxOn) {

	if (AuxOn==0)
		return;

	AUXsprintf("GCA Configuration is:\n");
	PrintGcaStr("AUX",	    AUXCTRL_SS.AuxiliaryControls);
	PrintGcaStr("BATTERY",	AUXCTRL_SS.BatteryControls  );
	PrintGcaStr("DBAS",	    AUXCTRL_SS.DBASControls		);
	PrintGcaStr("Fuze",	    AUXCTRL_SS.FuzeControls		);
	PrintGcaStr("HOBS",	    AUXCTRL_SS.HOBSControls		);
	PrintGcaStr("IF",	    AUXCTRL_SS.IFControls		);
	PrintGcaStr("IMU",	    AUXCTRL_SS.IMUControls		);
	PrintGcaStr("QSAL",     AUXCTRL_SS.QSALControls	    );
	PrintGcaStr("MAIN",     AUXCTRL_SS.MAINControls	    );
	PrintGcaStr("MCP",	    AUXCTRL_SS.MCPControls		);
	PrintGcaStr("MISSION",	AUXCTRL_SS.MissionControls	);
	PrintGcaStr("MLAN",	    AUXCTRL_SS.MLANControls		);
	PrintGcaStr("MPSAL",	AUXCTRL_SS.MPSALControls	);
	PrintGcaStr("SERVO",	AUXCTRL_SS.ServoControls    );
	PrintGcaStr("TIM",	    AUXCTRL_SS.TimeControls	    );
	PrintGcaStr("WIAC82",   AUXCTRL_SS.WIAC82Controls   );
	PrintGcaStr("WiFi",	    AUXCTRL_SS.WiFiControls		);
	AUXsprintf("\n");
}

char *pDbgStr;

void GetGcaCfg(char *cfg,char *cfgid,char * pAux,int iAuxStructSize)
{
	char *tmpstr,*tmpstr2,eosfound;
	tmpstr=strstr(cfg,cfgid);
	if(tmpstr==NULL)
	{
		sprintf(pDbgStr,"%sGetGcaCfg - %s not found!!!\n\r",pDbgStr,cfgid);
		return;
	}

	eosfound=0;
	if((tmpstr2=strchr(tmpstr,'\r'))!=NULL)
	{
		eosfound=1;
	}
	else if((tmpstr2=strchr(tmpstr,'\0'))!=NULL)
	{
		eosfound=1;
	}
	if(eosfound==0)
	{
		sprintf(pDbgStr,"%sGetGcaCfg - %s end not found!!!\n\r",pDbgStr,cfgid);
		return;
	}
	tmpstr+=strlen(cfgid);
	int stlen =tmpstr2-tmpstr;

	if(stlen!=iAuxStructSize)
	{
		sprintf(pDbgStr,"%sGetGcaCfg - %s not loaded due to mismatch!!!\n\r",pDbgStr,cfgid);
		return;
	}
	for(int n=0;n<stlen;n++)
	{
		pAux[n]=tmpstr[n]-0x30;
	}
}

 void LoadGCA_Config(char *aux)
{
	 pDbgStr=aux;
	 int foundcfg=0;
	 unsigned int idx,addr,magicnumber;
	 //try and find gca cfg in flash
	 idx=0;
	 magicnumber=0;
	 while(1)
	 {
		 addr = MCP_DATA_FLASH_ADDR + sizeof(sDataFileHeader) + (idx * sizeof(T_Weapon_MCP_GlobalMemory));
		 if(addr>=MCP_DATA_META_FLASH_ADDR)
		 {
			 break;
		 }
		 FlashReadData(addr, (unsigned char *) &magicnumber, 4);
		 if(magicnumber!=DATA_FILE_BLOCK_START_ID)
		 {
			 FlashReadData(addr, (unsigned char *) &GCA_Config, sizeof(GCA_Config));
			 if( Calculate32BitCRC((unsigned char *) &GCA_Config, sizeof(GCA_Config) - 4)==GCA_Config.CRC)
			 {
				 foundcfg=1;
			 }
			 break;
		 }
		 idx++;
	 }

	 if(foundcfg==0)
	 {
			sprintf(pDbgStr,"%sLoadGCA_Config - nothing in flash!!!\n\r",pDbgStr);
			return;
	 }
#define GetGCACfg(x,y) GetGcaCfg(ptmp,x,(char*)&y,sizeof(y));

	 char *ptmp=(char*)&GCA_Config;

	 GetGCACfg("AUX",	    AUXCTRL_SS.AuxiliaryControls);
	 GetGCACfg("BATTERY",	AUXCTRL_SS.BatteryControls  );
	 GetGCACfg("DBAS",	    AUXCTRL_SS.DBASControls		);
	 GetGCACfg("Fuze",	    AUXCTRL_SS.FuzeControls		);
	 GetGCACfg("HOBS",	    AUXCTRL_SS.HOBSControls		);
	 GetGCACfg("IF",	    AUXCTRL_SS.IFControls		);
	 GetGCACfg("IMU",	    AUXCTRL_SS.IMUControls		);
	 GetGCACfg("QSAL",      AUXCTRL_SS.QSALControls	    );
	 GetGCACfg("MAIN",      AUXCTRL_SS.MAINControls	    );
	 GetGCACfg("MCP",	    AUXCTRL_SS.MCPControls		);
	 GetGCACfg("MISSION",	AUXCTRL_SS.MissionControls	);
	 GetGCACfg("MLAN",	    AUXCTRL_SS.MLANControls		);
	 GetGCACfg("MPSAL",	    AUXCTRL_SS.MPSALControls	);
	 GetGCACfg("SERVO",	    AUXCTRL_SS.ServoControls    );
	 GetGCACfg("TIM",	    AUXCTRL_SS.TimeControls	    );
	 GetGCACfg("WIAC82",    AUXCTRL_SS.WIAC82Controls   );
	 GetGCACfg("WiFi",	    AUXCTRL_SS.WiFiControls		);

}

void InitGCA_SAL(char *AuxStr, unsigned short AuxOn) {
	if (AUXCTRL_SS.MAINControls.MPSAL_On) {
		MAIN_SS.DeviceControls.FW_VersionRead = MPSAL_SeekerInit(AuxStr,FIRMWARE_VERSION); // 0 if correct
		if (AuxOn) {
			if (MAIN_SS.DeviceControls.FW_VersionRead)
				sprintf(AuxStr,"%s FW Version=0x%x FAIL\n", AuxStr, MAIN_SS.DeviceControls.FW_VersionRead);
			else
				sprintf(AuxStr,"%s FW Version=0x%x PASS\n", AuxStr, FIRMWARE_VERSION);
		}
	}
	else if (AUXCTRL_SS.MAINControls.QSAL_On) {
		MAIN_SS.DeviceControls.FW_VersionRead = QSAL_SeekerInit(AuxStr,FIRMWARE_VERSION); // 0 if correct
		if (AuxOn) {
			sprintf(AuxStr,"%s%u:CONFIG GCA is set for QSAL\n", AuxStr,GetTimeUs());
			if (MAIN_SS.DeviceControls.FW_VersionRead)
				sprintf(AuxStr,"%s FW Version=0x%x FAIL\n", AuxStr, MAIN_SS.DeviceControls.FW_VersionRead);
			else
				sprintf(AuxStr,"%s FW Version=0x%x PASS\n", AuxStr, FIRMWARE_VERSION);

		}
		//QSALInitialition
	}
	else if (AUXCTRL_SS.MAINControls.DSAL_On) {
		if (AuxOn) {
			sprintf(AuxStr,"%s%u:CONFIG GCA is set for DSAL\n", AuxStr,GetTimeUs());
		}
		SAL_SeekerInit();
	}
	else {
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG UNKNOWN SAL Type (xx - INVALID - xx)\n", AuxStr,GetTimeUs());
	}
}

void InitGCA_AircraftIF(char *AuxStr, unsigned short AuxOn) {
	if (WeaponSettings.WeaponConfig.DTAC_Present)
	{
		if (AUXCTRL_SS.MAINControls.WIAC82_On)
		{
			if (AuxOn)
				sprintf(AuxStr,"%s%u:CONFIG GCA is set for WIAC82 (Dassault Rafale)\n", AuxStr,GetTimeUs());
			WIAC82_Init(AuxStr);
		}
		else
		{
			if (AuxOn)
				sprintf(AuxStr,"%s%u:CONFIG GCA is set for DBAS (Dassault Mirage 2000-9)\n", AuxStr,GetTimeUs());
			AircraftInit(AuxStr);
		}
	}
	else if (AUXCTRL_SS.MAINControls.P3LIGHT_On)
	{
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA is set for P3 Light\n", AuxStr,GetTimeUs());
	}
	else
	{
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA is set for Shallow Interface\n", AuxStr,GetTimeUs());
	}
}

void InitGCA_GNSS(char *AuxStr, unsigned short AuxOn) {
	if (AUXCTRL_SS.MAINControls.P3LIGHT_On)
	{
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA GNSS NOT Initialized\n", AuxStr,GetTimeUs());
	}
	else {
		UBLOX_Init();
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA GNSS (UBLOX) Initialized\n", AuxStr,GetTimeUs());
	}
}

void InitGCA_IMU(char *AuxStr, unsigned short AuxOn) {
	if (AUXCTRL_SS.MAINControls.IMU20_On)
	{
		IMU_Init(AuxStr);
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA IMU20 Initialized\n", AuxStr,GetTimeUs());
	}
	else {
		if (AuxOn)
			sprintf(AuxStr,"%s%u:CONFIG GCA IMU15 Initialized\n", AuxStr,GetTimeUs());
	}
}

char GenerateGCA_Config(void) {
	char *Settings;
	int Count;
	unsigned int crc32;

  	if (GetTimeUs() < GCA_STARTUP_TIMEOUT_TIME)
  		return(1);

/**/
	AUXsprintf("%d:Generating GCA Configuration Parameters\n",GetTimeUs());
	// Format of DATA definition

	AUXsprintf(" Opening ID of the Data Block = {\n");
	AUXsprintf(" Opening ID of Sub Block = ( followed by Sub-Block ID:\n");
	AUXsprintf("  Block MAIN ID           = M\n");
	AUXsprintf("  Block MPSAL ID          = S\n");
	AUXsprintf("  Block QSAL ID           = Q\n");
	AUXsprintf("  Block WIAC82 ID         = W\n");
	AUXsprintf("  Block DTAC ID           = D\n");
	AUXsprintf("  Block WIFI ID           = I\n");
	AUXsprintf("  Block IMU ID            = U\n");
	AUXsprintf("  Block IF ID             = E\n");
	AUXsprintf("  Block MLAN ID           = L\n");
	AUXsprintf("  Block HOBS ID           = H\n");
	AUXsprintf("  Block BATTERY ID        = B\n");
	AUXsprintf("  Block FUZE ID           = F\n");
	AUXsprintf("  Block SERVO ID          = V\n");
	AUXsprintf("  Block AUXILIARY PORT ID = A\n");
	AUXsprintf("  Block MCP ID            = M\n");
	AUXsprintf("  Block MISSION ID        = X\n");
	AUXsprintf("  Block TIMER ID          = T\n");
	AUXsprintf(" Sub Block ID is followed by the number of parameters to be set\n");
	AUXsprintf(" After the number of parameters comes the actual parameters where:\n");
	AUXsprintf("  0 = Parameter is NOT set\n");
	AUXsprintf("  1 = Parameter is set\n");
	AUXsprintf(" After the parameter settings comes the CRC for the parameter Sub Block\n");
	AUXsprintf(" Followed by the closing ID of Sub Block = )\n");
	AUXsprintf(" After all blocks generated comes the closing ID of the Data Block = }\n");

	// GENERATE DATA
	AUXsprintf("{");

	//MAIN SS
	Settings = (char *)&AUXCTRL_SS.MAINControls;
	AUXsprintf("(M%3.3d",sizeof(sMAIN_AuxControls));
	for (Count=0;Count<sizeof(sMAIN_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.MAINControls, sizeof(sMAIN_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//MPSAL SS
	Settings = (char *)&AUXCTRL_SS.MPSALControls;
	AUXsprintf("(S%3.3d",sizeof(sMPSAL_AuxControls));
	for (Count=0;Count<sizeof(sMPSAL_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.MPSALControls, sizeof(sMPSAL_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//MPSAL SS
	Settings = (char *)&AUXCTRL_SS.QSALControls;
	AUXsprintf("(S%3.3d",sizeof(sQSAL_AuxControls));
	for (Count=0;Count<sizeof(sQSAL_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.QSALControls, sizeof(sQSAL_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//WIAC82 SS
	/*Settings = (char *)&AUXCTRL_SS.WIAC82Controls;
	AUXsprintf("(W%3.3d",sizeof(sWIAC82_AuxControls));
	for (Count=0;Count<sizeof(sWIAC82_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.WIAC82Controls, sizeof(sWIAC82_AuxControls));
	AUXsprintf("%8.8x)",crc32); */

	//DBAS SS
	Settings = (char *)&AUXCTRL_SS.DBASControls;
	AUXsprintf("(I%3.3d",sizeof(sDBAS_AuxControls));
	for (Count=0;Count<sizeof(sDBAS_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.DBASControls, sizeof(sDBAS_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//WIFI SS
	Settings = (char *)&AUXCTRL_SS.WiFiControls;
	AUXsprintf("(I%3.3d",sizeof(sWiFi_AuxControls));
	for (Count=0;Count<sizeof(sWiFi_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.WiFiControls, sizeof(sWiFi_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//IMU SS
	Settings = (char *)&AUXCTRL_SS.IMUControls;
	AUXsprintf("(U%3.3d",sizeof(sIMU_AuxControls));
	for (Count=0;Count<sizeof(sIMU_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.IMUControls, sizeof(sIMU_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//IF SS
	Settings = (char *)&AUXCTRL_SS.IFControls;
	AUXsprintf("(E%3.3d",sizeof(sIF_AuxControls));
	for (Count=0;Count<sizeof(sIF_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.IFControls, sizeof(sIF_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//MLAN SS
	Settings = (char *)&AUXCTRL_SS.MLANControls;
	AUXsprintf("(L%3.3d",sizeof(sMLAN_AuxControls));
	for (Count=0;Count<sizeof(sMLAN_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.MLANControls, sizeof(sMLAN_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//HOBS SS
	Settings = (char *)&AUXCTRL_SS.HOBSControls;
	AUXsprintf("(H%3.3d",sizeof(sHOBS_AuxControls));
	for (Count=0;Count<sizeof(sHOBS_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.HOBSControls, sizeof(sHOBS_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//BATT_SS
	Settings = (char *)&AUXCTRL_SS.BatteryControls;
	AUXsprintf("(B%3.3d",sizeof(sBATTERY_AuxControls));
	for (Count=0;Count<sizeof(sBATTERY_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.BatteryControls, sizeof(sBATTERY_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//FUZE_SS
	Settings = (char *)&AUXCTRL_SS.FuzeControls;
	AUXsprintf("(F%3.3d",sizeof(sFuze_AuxControls));
	for (Count=0;Count<sizeof(sFuze_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.FuzeControls, sizeof(sFuze_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//SERVO_SS
	Settings = (char *)&AUXCTRL_SS.ServoControls;
	AUXsprintf("(V%3.3d",sizeof(sSERVO_AuxControls));
	for (Count=0;Count<sizeof(sSERVO_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.ServoControls, sizeof(sSERVO_AuxControls));
	AUXsprintf("%8.8x)",crc32);
	//AUX_SS
	Settings = (char *)&AUXCTRL_SS.AuxiliaryControls;
	AUXsprintf("(A%3.3d",sizeof(sAUX_AuxControls));
	for (Count=0;Count<sizeof(sAUX_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.AuxiliaryControls, sizeof(sAUX_AuxControls));
	AUXsprintf("%8.8x)",crc32);
	//MCP_SS
	Settings = (char *)&AUXCTRL_SS.MCPControls;
	AUXsprintf("(M%3.3d",sizeof(sMCP_AuxControls));
	for (Count=0;Count<sizeof(sMCP_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.MCPControls, sizeof(sMCP_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//Mission_SS
	Settings = (char *)&AUXCTRL_SS.MissionControls;
	AUXsprintf("(M%3.3d",sizeof(sMISSION_AuxControls));
	for (Count=0;Count<sizeof(sMISSION_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.MissionControls, sizeof(sMISSION_AuxControls));
	AUXsprintf("%8.8x)",crc32);

	//TIM_SS
	Settings = (char *)&AUXCTRL_SS.TimeControls;
	AUXsprintf("(T%3.3d",sizeof(sTIM_AuxControls));
	for (Count=0;Count<sizeof(sTIM_AuxControls);Count++)
		AUXsprintf("%c", Settings[Count]+0x30);
    // Calculate 32-bit CRC
    crc32 = Calculate32BitCRC((unsigned char *) &AUXCTRL_SS.TimeControls, sizeof(sTIM_AuxControls));
	AUXsprintf("%8.8x)",crc32);
	AUXsprintf("}\nMCP>");

	return(0);

}
