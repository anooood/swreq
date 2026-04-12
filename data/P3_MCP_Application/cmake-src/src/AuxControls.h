/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <AuxControls.h>
 *
 *  Description:
 *  ------------
 *  Header file for the AuxControls module.
 *
 ******************************************************************************/

#ifndef AUX_CONTROLS_H
#define AUX_CONTROLS_H

#include <stdint.h>

typedef struct
{
	char		AuxOn;              // Auxiliary Port Variable to debug.
	char		SchedulerAuxOn;     // Auxiliary Port Scheduler to debug.
	char		Enabled;            // Auxiliary Port ServiceAUXTx is Enabled
	char		ErrDetectOn;        // Auxiliary Port Error Detection to debug.
}sAUX_AuxControls;

typedef struct
{
	char		AuxOn;              // Battery Auxiliary Variable to debug.
	char		SchedulerAuxOn;     // Battery Auxiliary Scheduler to debug.
	char		AuxToCsv;           // Battery Data to CSV file Flag.
	char		RxAuxOn;            // Battery RX Flag.
	char		RxDetailAuxOn;      // Battery RX with details Flag.
	char		TxAuxOn;            // Battery TX Flag.
	char		TxDetailAuxOn;      // Battery TX with details Flag.
	char		ErrDetectOn;        // Battery Error Detection Flag.
}sBATTERY_AuxControls;

typedef struct
{
	char		AuxOn;                    // DBAS Auxiliary Variable to debug.
	char		SchedulerAuxOn;           // DBAS Auxiliary Scheduler to debug.
	char		RxAuxOn;                  // DBAS RX Flag.
	char		RxDetailAuxOn;            // DBAS RX with details Flag.
	char        RxDetailStatusAux;        // DBAS RX Status details Flag.
	char        RxDetailMissionAux;       // DBAS RX Mission details Flag.
	char        RxDetailWindAux;          // DBAS RX Wind details Flag.
	char		RxDetailtTransAlignAux;   // DBAS RX Transfer Alignment details Flag.
	char		TxAuxOn;                  // DBAS TX Flag.
	char		TxDetailAuxOn;            // DBAS TX with details Flag.
	char		TxDetailLARAux;           // DBAS TX LAR details Flag.
	char		TxDetailFlagsAux;         // DBAS TX WeaponFlag details Flag.
	char		TxDetailMissionAux;       // DBAS TX Mission details Flag.
	char		TxDetailLARInRangeAux;    // DBAS TX LARInRange details Flag.
	char		TxDetailWeaponCfgAux;     // DBAS TX Weapon Configuration details Flag.
	char		ErrDetectOn;              // DBAS Error Detection Flag.
}sDBAS_AuxControls;

typedef struct
{
	uint8_t		AuxOn;              // FUZE Auxiliary Variable to debug.
	uint8_t		SchedulerAuxOn;     // FUZE Auxiliary Scheduler to debug.
	uint8_t		RxAuxOn;            // FUZE RX Flag.
	uint8_t		RxDetailAuxOn;      // FUZE RX with details Flag.
	uint8_t		TxAuxOn;            // FUZE TX Flag.
	uint8_t		TxDetailAuxOn;      // FUZE TX with details Flag.
	uint8_t		ExtRxOn;            // FUZE External RX Flag. (Set Delay)
	uint8_t		ErrDetectOn;        // FUZE Error Detection Flag.
}sFuze_AuxControls;

typedef struct
{
	uint8_t		AuxOn;              // HOBS Auxiliary Variable to debug.
	uint8_t		SchedulerAuxOn;     // HOBS Auxiliary Scheduler to debug.
	uint8_t		RxAuxOn;            // HOBS RX Flag.
	uint8_t		RxDetailAuxOn;      // HOBS RX with details Flag.
	uint8_t		TxAuxOn;            // HOBS TX Flag.
	uint8_t		TxDetailAuxOn;      // HOBS TX with details Flag.
	uint8_t		ExtRxAuxOn;         // HOBS External RX Flag. (Activate and Burst Height)
	uint8_t		ErrDetectOn;        // HOBS Error Detection Flag.
}sHOBS_AuxControls;

typedef struct
{
	uint8_t		AuxOn;              // IF Auxiliary Variable to debug.
	uint8_t		SchedulerAuxOn;     // IF Auxiliary Scheduler to debug.
	uint8_t		RxAuxOn;            // IF RX Flag.
	uint8_t		TxAuxOn;            // IF TX Flag.
	uint8_t		ErrDetectOn;        // IF Error Detection Flag.
}sIF_AuxControls;

typedef struct
{
	char		AuxOn;              // IMU Auxiliary Variable to debug.
	char		AuxToCsv;			// IMU Data to CSV file Flag.
	char		RxAuxOn;            // IMU RX Flag.
	char		RxDetailAuxOn;      // IMU RX with details Flag.
	char		ErrDetectOn;        // IMU Error Detection Flag.
}sIMU_AuxControls;

typedef struct
{
	char		AuxOn;              // QSAL Auxiliary Variable to debug.
	char		SchedulerAuxOn;     // QSAL Auxiliary Scheduler to debug.
	char		RxAuxOn;            // QSAL RX Flag.
	char		RxDetailAuxOn;      // QSAL RX with details Flag.
	char		TxAuxOn;            // QSAL TX Flag.
	char		ErrDetectOn;        // QSAL Error Detection Flag.
}sQSAL_AuxControls;

typedef struct
{
	char		AuxOn;              // Auxiliary Variable to debug.
	char		NodeCheckOn;        // Validation of the nodes on the net.
	char		X_Config_On;        // Indicates weapon has X Configuration active. (Default is + Configuration)
	char		DBAS_On;            // Indicates DigiBus (DBAS) Comms is active. (Aicraft interface must be present) - MIRAGE2000-9
	char		WIAC82_On;			// Indicates WIAC82 Comms is active. (Aicraft interface must be present) - RAFALE
	char		P3LIGHT_On;         // Indicates P3 Light Variant.
	char		ShallowOn;          // Indicates P3 Shallow. (Wifi enabled/Tablet)
	char		MPSAL_On;			// Indicates the kind of SAL is MPSAL. (SAL must be present)
	char		QSAL_On;			// Indicates the kind of SAL is QSAL. (SAL must be present)
	char		DSAL_On;			// Indicates the kind of SAL is DSAL. (SAL must be present)
	char		IMU20_On;			// Indicates the kind of IMU is 20.
	char		IMU15_On;			// Indicates the kind of IMU is 15.
	char		GNSS_On;			// Indicates GNSS is active.
	uint8_t		FuzeCardFittedOn;   // Indicates whether the weapon has FuzeCard.
}sMAIN_AuxControls;

typedef struct
{
	char		AuxOn;              // MCP Auxiliary Variable to debug.
	char		SchedulerAuxOn;     // MCP Auxiliary Scheduler to debug.
	char		MCPPresAuxOn;       // MCP Pressure sensor details Flag.
	char		MCPTempAuxOn;		// MCP Temperature details Flag.
	char		MCPAnteAuxOn;       // MCP Antenna Selection details Flag.
	char		MCPBattAuxOn;       // MCP Battery Activation details Flag.
	char		MCPVoltAuxOn;       // MCP Voltage details Flag.
	char		ErrDetectOn;        // MCP Error Detection Flag.
}sMCP_AuxControls;

typedef struct
{
	char		AuxOn;                // Mission Auxiliary Variable to debug.
	char		MissionAuxOn;         // Mission Flag.
	char		MissionDetail1AuxOn;  // Mission details (Level 1) Flag.
	char		MissionDetail2AuxOn;  // Mission details (Level 2) Flag.
}sMISSION_AuxControls;

typedef struct
{
	char		AuxOn;            // MLAN (ARCNET) Auxiliary Variable to debug.
	char		SchedulerAuxOn;   // MLAN (ARCNET) Auxiliary Scheduler to debug.
	char		RxAuxOn;		  // MLAN (ARCNET) RX Flag.
	char		RxDetailAuxOn;    // MLAN (ARCNET) RX with details Flag.
	char		TxAuxOn;          // MLAN (ARCNET) TX Flag.
	char		TxDetailAuxOn;	  // MLAN (ARCNET) TX with details Flag.
	char		WiFi_AuxOn;		  // MLAN (ARCNET) WIFI Flag.
	char		ETE_AuxOn;		  // MLAN (ARCNET) External Test Equipment Flag.
	char		Seeker_AuxOn;     // MLAN (ARCNET) Seeker (DSAL) Flag.
	char		HILS_AuxOn;		  // MLAN (ARCNET) HILS Flag.
	char		DTAC_AuxOn;       // MLAN (ARCNET) DBAS Flag.
	char		Servo_AuxOn;      // MLAN (ARCNET) Servo Flag.
	char		Tel_AuxOn;        // MLAN (ARCNET) Telemetry Flag.
	char		ITU_AuxOn;        // MLAN (ARCNET) ITU (WIAC82) Flag.
	char		ErrDetectOn;      // MLAN (ARCNET) Error Detection Flag.
}sMLAN_AuxControls;

typedef struct
{
	char		AuxOn;                 // MPSAL Auxiliary Variable to debug.
	char		SchedulerAuxOn;        // MPSAL Auxiliary Scheduler to debug.
	char		RxAuxOn;               // MPSAL RX Flag.
	char		RxDetailAuxOn;         // MPSAL RX with detail Flag.
	char		TxAuxOn;               // MPSAL TX Flag.
	char		TxDetailAuxOn;         // MPSAL TX with detail Flag.
	char		PerformLoopBackTest;   // MPSAL PerformLoopBackTest (UART) Flag. (Use 0)
	char		SetInternalLoopBack;   // MPSAL SetInternalLoopBack (UART) Flag. (Use 0)
	char		ExtRxAuxOn;            // MPSAL External RX Flag. (Set New Parameters)
	char		ErrDetectOn;           // MPSAL Error Detection Flag.
	char		WarningOn;             // MPSAL Warning Flag. (TX Disabled)
}sMPSAL_AuxControls;

typedef struct
{
	char		AuxOn;              // Servo Auxiliary Variable to debug.
	char		SchedulerAuxOn;     // Servo Auxiliary Scheduler to debug.
	char		RxAuxOn;            // Servo RX Flag.
	char		RxDetailAuxOn;      // Servo RX with details Flag.
	char		TxAuxOn;            // Servo TX Flag.
	char		TxDetailAuxOn;      // Servo TX with details Flag.
	char		ServoTestAuxOn;     // Servo Test Flag. (ONLY for testing)
	char		ErrDetectOn;        // Servo Error detection Flag.
}sSERVO_AuxControls;

typedef struct
{
	char		AuxOn;              // TIME Auxiliary Variable to debug.
	char		ErrDetectOn;		// TIME Error detection Flag.
}sTIM_AuxControls;

typedef struct
{
	char		AuxOn;						// WIAC82 Auxiliary Variable to debug.
	char		SchedulerAuxOn;				// WIAC82 Auxiliary Scheduler to debug.
	char		RxAuxOn;					// WIAC82 RX Flag.
	char		RxDetailAuxOn;				// WIAC82 RX with details Flag.
	char		RxDetail1AuxOn;				// WIAC82 RX with details (Level 1) Flag.
	char		RxDetail2AuxOn;				// WIAC82 RX with details (Level 2) Flag.
	char		TxAuxOn;					// WIAC82 TX Flag.
	char		TxDetailAuxOn;				// WIAC82 TX with details Flag.
	char		TxDetail1AuxOn;				// WIAC82 TX with details (Level 1) Flag.
	char		TxDetail2AuxOn;				// WIAC82 TX with details (Level 2) Flag.
	char		ErrDetectOn;				// WIAC82 Error detection Flag.
	char		PrnMLANHeaderOn;			// WIAC82 MLAN Header Flag.
	char		Prn1553HeaderOn;			// WIAC82 MILBUS1553 Header Flag.
	char		TxAuxSBITReq;				// SBIT Request - WIAC82 Related
	char		TxAuxStatusReq;				// Status Request - WIAC82 Related
	char		TxAuxBCMessage;				// Bus Controller Message - WIAC82 Related
	char		TxAuxNormalWrap;			// Normal Wrap Message - WIAC82 Related
	char		TxAuxBrokenWrap;			// Broken Wrap Message - WIAC82 Related
	char		RxAuxSBIT;					// WIAC82 SBIT Message Received - WIAC82 Related
	char		RxAuxStatus;				// WIAC82 Status Message Received - WIAC82 Related
	char		RxAuxBC_WPN;				// WIAC82 BC-WPN Message Received - WIAC82 Related
	char		RxAuxBCDataReq;				// WIAC82 BC-WPN Data Request Message Received - WIAC82 Related
	char		RxAuxBCWPNcmd;				// WIAC82 BC-WPN Command Message Received - WIAC82 Related
	char		RxAuxAC_Description;		// AIRCRAFT DESCRIPTION MESSAGE (1R) - 1553 Related
	char		RxAuxAC_Time;				// SYSTEM TIME MESSAGE (2R) - 1553 Related
	char		RxAuxRefusal;				// GENERAL REFUSAL MESSAGE (3R) - 1553 Related
	char		RxAuxConfiguration;			// CONFIGURATION MESSAGE (10R) - 1553 Related
	char		RxAuxStoresCtl;				// STORE CONTROL MESSAGE (11R) - 1553 Related
	char		RxAuxVector50Hz;			// STATE VECTOR 50Hz MESSAGE (14R) - 1553 Related
	char		RxAuxVector6p25Hz;			// STATE VECTOR 6.25HZ MESSAGE (16R) - 1553 Related
	char		RxAuxFunction1D;			// FUNCTIONAL EXCHANGE 1D MESSAGE  (21R) - 1553 Related
	char		RxAuxFunction3D;			// FUNCTIONAL EXCHANGE 3D MESSAGE  (22R) - 1553 Related
	char		RxAuxSatUse;				// SATELLITE USE PARAMETERS (26R) - 1553 Related
	char		RxAuxALMAC;					// GPS INITIALIZATION, ALMANAC (29R) CUSTOM DASSAULT MESSAGE - 1553 Related
	char		RxAuxEPHEMERIDES;			// GPS INITIALIZATION, EPHEMERIDES (29R) CUSTOM DASSAULT MESSAGE - 1553 Related
	char		RxAuxBC_TEST;				// EQUIPMENT DIALOGUE TEST (30R) - 1553 Related
	char		TxAuxStoresRep;				// STORE DESCRIPTION REPORT (1T) - 1553 Related
	char		TxAuxStatusWord;			// STATUS WORD WEAPON (8T) - 1553 Related
	char		TxAuxMonitorRep;			// STORE MONITOR REPORT (11T) - 1553 Related
	char		TxAuxFunction3M;			// FUNCTIONAL EXCHANGE 3M (17T) - 1553 Related
	char		TxAuxFunction1M;			// FUNCTIONAL EXCHANGE 1M (23T) - 1553 Related
	char		TxAuxWPN_TEST;				// RETURN DIALOGUE TEST (30T) - 1553 Related
	char		ReleaseSequence;			// Weapon Release Sequence (WIAC-82)
	char		MissionData;				// Weapon Mission Data Validation (WIAC-82)
	char		TransferAlignment;			// Weapon Transfer Alignment (WIAC-82)
}sWIAC82_AuxControls;

typedef struct
{
	char		AuxOn;             // WiFi Auxiliary Variable to debug.
	char		SchedulerAuxOn;    // WiFi Auxiliary Scheduler to debug.
	char		RxAuxOn;           // WiFi RX Flag.
	char		RxDetailAuxOn;     // WiFi RX with details Flag.
	char		TxAuxOn;           // WiFi TX Flag.
	char		TxDetailAuxOn;     // WiFi TX with details Flag.
	char		ErrDetectOn;       // WiFi Error detection Flag.
}sWiFi_AuxControls;

typedef struct
{
	sAUX_AuxControls     AuxiliaryControls;
	sBATTERY_AuxControls BatteryControls;
	sDBAS_AuxControls    DBASControls;
	sFuze_AuxControls    FuzeControls;
	sHOBS_AuxControls    HOBSControls;
	sIF_AuxControls      IFControls;
	sIMU_AuxControls     IMUControls;
	sQSAL_AuxControls    QSALControls;
	sMAIN_AuxControls    MAINControls;
	sMCP_AuxControls     MCPControls;
	sMISSION_AuxControls MissionControls;
	sMLAN_AuxControls    MLANControls;
	sMPSAL_AuxControls   MPSALControls;
	sSERVO_AuxControls   ServoControls;
	sWiFi_AuxControls    WiFiControls;
	sWIAC82_AuxControls  WIAC82Controls;
	sTIM_AuxControls     TimeControls;

} sAUXCtrls;

extern sAUXCtrls AUXCTRL_SS;

#endif // AUX_CONTROLS_H
