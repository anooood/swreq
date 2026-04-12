/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WIAC82.h>
 *
 *  Description:
 *  ------------
 *  Header file for the WIAC82 module.
 *
 ******************************************************************************/

#include "AircraftInterface.h"

#ifndef WIAC82_H_
#define WIAC82_H_

#define WIAC82_MLAN_MCP_NODE_ID              255
#define WIAC82_MLAN_ETE_NODE_ID              250
#define WIAC82_MLAN_ITUC_NODE_ID             249
#define WIAC82_MLAN_RTAC_NODE_ID             249
#define WIAC82_MLAN_WIAC_NODE_ID             249
#define WIAC82_MLAN_HILS_NODE_ID             247
#define WIAC82_MLAN_LOADER_NODE_ID           247
#define WIAC82_TX_SERVICE				((unsigned int)(10000)) // 10 milli-second
//-----------------------------------------------------------------------------
// WIAC relevant OP-Codes
//-----------------------------------------------------------------------------
#define WIAC82_TX_ID_SBIT_REQ                 0x00  //MCP, ETE
#define WIAC82_RX_ID_SBIT                     0x01  //MCP, ETE
#define WIAC82_RX_ID_STATUS                   0x80  //MCP, ETE
#define WIAC82_TX_ID_CLEAR_CBIT               0x04  //MCP, ETE
#define WIAC82_TX_ID_DIS_ARCNET_NOTIFY        0x05  //ETE, HILS
#define WIAC82_TX_ID_DIS_ARCNET               0x06  //ETE, HILS
#define WIAC82_TX_ID_STATUS_REQ               0x11  //MCP, ETE
#define WIAC82_TX_ID_BATTERY_DEACT_REQ        0x22  //MCP, ETE
#define WIAC82_TX_ID_RF_DEACT_REQ       	  0x23  //MCP, ETE
#define WIAC82_TX_ID_BATTERY_ACT_REQ          0x99  //MCP, ETE
#define WIAC82_TX_ID_NORMAL_WRAP              0xF0
#define WIAC82_TX_ID_BROKEN_WRAP              0xF1
//-----------------------------------------------------------------------------
// 1553 BUS specific OP-Codes
//-----------------------------------------------------------------------------
// Transmit
#define WIAC82_TX_MLAN_WPN_BC_MESSAGE         0x8A
// Receive
#define WIAC82_RX_MLAN_1553_BC_WPN_MESSAGE	  0x8B
#define WIAC82_RX_MLAN_1553_BC_WPN_DATA_REQ   0x8C
#define WIAC82_RX_MLAN_1553_BC_WPN_CMD        0x8D

#define WIAC82_SBIT_SERVICE_INTERVAL							 	300000	//300 milli-sec = 3.3Hz
#define WIAC82_STATUS_SERVICE_INTERVAL							 	300000	//300 milli-sec = 3.3Hz
#define WIAC82_STORE_DESCRIPTION_1T_SERVICE_INTERVAL			 	160000  //160 milli-sec = 6.25Hz
#define WIAC82_WEAPON_STATUS_WORD_8T_SERVICE_INTERVAL			 	160000  //160 milli-sec = 6.25Hz
#define WIAC82_WEAPON_STORE_MONITOR_REPORT_11T_SERVICE_INTERVAL	  	 20000  // 20 milli-sec = 50Hz
#define WIAC82_WEAPON_FUNCTIONAL_EXCHANGE_3M_17T_SERVICE_INTERVAL	160000 //160 milli-sec = 6.25Hz
#define WIAC82_WEAPON_FUNCTIONAL_EXCHANGE_1M_23T_SERVICE_INTERVAL	160000 //160 milli-sec = 6.25Hz

typedef struct
{
	  uint16_t CalcCriticalAuthority1;
	  uint16_t CalcCriticalAuthority2;
	  uint16_t Msg11R_CAuthority1_OK;
	  uint16_t Msg11R_CAuthority2_OK;
	  uint16_t Msg11R_CalcCRC;
	  uint16_t Msg11R_CRC_NOK;
	  uint16_t Msg11R_Header_NOK;
	  uint16_t Msg11R_CC1_NOK;
	  uint16_t Msg11R_CC2_NOK;
	  uint16_t Msg11R_RTAddress;
	  uint16_t ARM_Weapon;
	  uint16_t CommitToSeparate;
	  TIME EraseStartTime;
	  uint16_t EraseStart;
	  uint8_t CritCtrl1Result;
	  uint8_t CritCtrl2Result;
	  uint8_t Rx11R_ErrCnt;
//	  sRawMission Mission;

}sWIAC82_CriticalControls;
typedef struct
{
	unsigned int TxServiceCnt;
	unsigned int SBITReqMsgCnt;
	unsigned int StatusReqMsgCnt;
	unsigned int BatteryActReqMsgCnt;
	unsigned int BatteryDeactReqMsgCnt;
	unsigned int BCMsgCnt;
	unsigned int NormalWrapMsgCnt;
	unsigned int BrokenWrapMsgCnt;
	unsigned int StoresDescriptionMsgCnt;
	unsigned int WeaponStsMsgCnt;
	unsigned int StoresMonRepMsgCnt;
	unsigned int FunctionalExchane1MMsgCnt;
	unsigned int FunctionalExchane3MMsgCnt;
} sWIAC82_TxCounters;

typedef struct
{
	unsigned int SBIT_NotSent;
	unsigned int SBIT_Received;
	unsigned int Trigger23T;
	unsigned int Trigger17T;
}sWIAC82_CommsControls;

typedef struct
{
	unsigned int Rx1RMsgCnt;
	unsigned int Rx2RMsgCnt;
	unsigned int Rx3RMsgCnt;
	unsigned int Rx10RMsgCnt;
	unsigned int Rx11RMsgCnt;
	unsigned int Rx14RMsgCnt;
	unsigned int Rx16RMsgCnt;
	unsigned int Rx21RMsgCnt;
	unsigned int Rx22RMsgCnt;
	unsigned int Rx26RMsgCnt;
	unsigned int Rx29RMsgCnt;
	unsigned int Rx30RMsgCnt;
} sWIAC82_RxCounters;

typedef struct
{
	sSchedule_Time_Controls	SBIT_Service;
	sSchedule_Time_Controls	Status_Service;
	sSchedule_Time_Controls	StoreDescription1T;
	sSchedule_Time_Controls	WeaponStatusWord8T;
	sSchedule_Time_Controls	StoreMonitorReport11T;
	sSchedule_Time_Controls	FunctionalExchange3M_17T;
	sSchedule_Time_Controls	FunctionalExchange1M_23T;
	uint8_t RTAddr;
	uint8_t SubAddr;
	uint16_t WpnStatusWordSum;
} sWIAC82_ServiceControl;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int INS_TrueHeading      : 1;    // 0
    unsigned int INS_RollPitchAngle   : 1;    // 1
    unsigned int INS_PQR              : 1;    // 2
    unsigned int VelocityNorth        : 1;    // 3
    unsigned int VelocityEast         : 1;    // 4
    unsigned int VelocityDown         : 1;    // 5
    unsigned int AngleOfAttack        : 1;    // 6
    unsigned int AngleOfSideslip      : 1;    // 7
    unsigned int CalibratedAirspeed   : 1;    // 8
    unsigned int TrueAirspeed         : 1;    // 9
    unsigned int FighterLatitude      : 1;    // 10
	unsigned int FighterLongitude     : 1;    // 11
    unsigned int FighterAltitude      : 1;    // 12
    unsigned int WindSpeed 		      : 1;    // 13
    unsigned int WindDirection	      : 1;    // 14
    unsigned int AngleOffset          : 1;    // 15
    unsigned int LeverArm             : 1;    // 16
    unsigned int Reserved             : 15;   // 17 - 31
  };
} uWIAC82_TransferAlignmentValidity;

typedef union
{
  unsigned short Word;
  struct
  {
	unsigned short Msg14RPacked   : 1;    // 0
    unsigned short Msg16RPacked   : 1;    // 1
    unsigned short Msg21RPacked   : 1;    // 2
    unsigned short IMU1		      : 1;    // 3
    unsigned short IMU2		      : 1;    // 4
    unsigned short Reserved3      : 1;    // 5
    unsigned short Reserved4      : 1;    // 6
    unsigned short Reserved5      : 1;    // 7
    unsigned short Reserved6      : 1;    // 8
    unsigned short Reserved7      : 1;    // 9
    unsigned short Reserved8      : 1;    // 10
    unsigned short Reserved9      : 1;    // 11
    unsigned short Reserved10     : 1;    // 12
    unsigned short Reserved11     : 1;    // 13
    unsigned short Reserved12     : 1;    // 14
    unsigned short Reserved13     : 1;    // 15
  };
} uWIAC82_TransferAlignmentMsgFlag;

typedef struct
{
	uWIAC82_TransferAlignmentValidity Validity; // 12
	short Roll;                                 // 16
	short Pitch;                                // 18
	short Yaw;                                  // 20
	short RollRate;                             // 22
	short PitchRate;                            // 24
	short YawRate;                              // 26
	short VelocityNorth;                        // 28
	short VelocityEast;                         // 30
	short VelocityDown;                         // 32
	unsigned short Reserved1;                   // 34
	int Latitude;                               // 36
	int Longitude;                              // 40
	short Altitude;                             // 44
	short LeverArmX;                            // 46
	short LeverArmY;                            // 48
	short LeverArmZ;                            // 50
	unsigned short Latency1;                    // 52
	unsigned short Latency2;                    // 54
	unsigned short Latency3;                    // 56
	unsigned short Latency4;                    // 58
	unsigned short TrueAirspeed;                // 60
	unsigned short CalibratedAirspeed;          // 62
	unsigned short StaticPressure;              // 64
	short AngleOfAttack;                        // 66
	short AngleOfSideslip;                      // 68
	uWIAC82_TransferAlignmentMsgFlag WICA82Flag;// 70
} sWIAC82_TransferAlignment;

typedef struct
{
	sWIAC82_CriticalControls Critical;
	uint8_t RTAddr; // Common RTAdress
	// 1553 BC to Weapon messages ************************************
	sWIAC82_1553_MLAN_Aircraft_Description_Message 			AC_Description;		// AIRCRAFT DESCRIPTION MESSAGE (1R)
	sWIAC82_1553_MLAN_System_Time_Message 					AC_Time;			// SYSTEM TIME MESSAGE (2R)
	sWIAC82_1553_MLAN_Refusal_Message						Refusal;			// GENERAL REFUSAL MESSAGE (3R)
	sWIAC82_1553_MLAN_Configuration_Message 				Configuration;		// CONFIGURATION MESSAGE (10R)
	sWIAC82_1553_MLAN_Store_Control_Message					StoresCtl;			// STORE CONTROL MESSAGE (11R)
	sWIAC82_1553_MLAN_StateVector50Hz_Message				Vector50Hz;			// STATE VECTOR 50Hz MESSAGE (14R)
	sWIAC82_1553_MLAN_StateVector6p25Hz_Message				Vector6p25Hz;		// STATE VECTOR 6.25HZ MESSAGE (16R)
	sWIAC82_1553_MLAN_FunctionalExchange1D_Message			Func1D;				// FUNCTIONAL EXCHANGE 1D MESSAGE (21R)
	sWIAC82_1553_MLAN_FunctionalExchange3D_Message			Func3D;				// FUNCTIONAL EXCHANGE 3D MESSAGE (22R)
	sWIAC82_1553_MLAN_Satellite_Use_Parameters_Message 		SatUse;				// SATELLITE USE PARAMETERS (26R)
	sWIAC82_1553_MLAN_GPS_InitializationALMANAC_Message		ALMAC;				// GPS INITIALIZATION, ALMANAC (29R) CUSTOM DASSAULT MESSAGE
	sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message	EPHEMERIDES;		// GPS INITIALIZATION, EPHEMERIDES (29R) CUSTOM DASSAULT MESSAGE
	sWIAC82_1553_MLAN_EquipmentDiaglogueTest_Message		BC_TEST;			// EQUIPMENT DIALOGUE TEST (30R)

	// 1553 Weapon to BC messages ************************************
	sWIAC82_1553_MLAN_StoreDescriptionReport_Message		StoresRep;			// STORE DESCRIPTION REPORT (1T)
	sWIAC82_1553_MLAN_StatusWordWeapon_Message				StatusWord;			// STATUS WORD WEAPON (8T)
	sWIAC82_1553_MLAN_StoreMonitorReport_Message			MonitorRep;			// STORE MONITOR REPORT (11T)
	sWIAC82_1553_MLAN_FunctionalExchange3M_Message			Function3M;			// FUNCTIONAL EXCHANGE 3M (17T)
	sWIAC82_1553_MLAN_FunctionalExchange1M_Message			Function1M;			// FUNCTIONAL EXCHANGE 1M (23T)
	sWIAC82_1553_MLAN_ReturnDialogueTest_Message			WPN_TEST;			// RETURN DIALOGUE TEST (30T)

	sWIAC82_TransferAlignment   TransferAlignmentData;

	sWIAC82_SBIT_Request_Msg	SBIT_Request;
	sWIAC82_StatusRequest_Msg  	status_req_msg;
	sWIAC82_NormalWrap_Msg  	normal_wrap_msg;
	sWIAC82_BrokenWrap_Msg  	broken_wrap_msg;

	sWIAC82_SBIT_Msg 			SBIT_Message;
	sWIAC82_StatusMsg 			STATUS_Message;

	sWIAC82_1553_MLAN_Message  	bc_msg;
	sWIAC82_TxCounters 			TxCounters;					// Transmit Counters
	sWIAC82_RxCounters 			RxCounters;					// Receive Counters
	sWIAC82_ServiceControl 		ServiceControls;			// Service Controls
	sWIAC82_CommsControls 		CommsControls;				// Communication Controls
	TIME                        RxTimestamp;
	unsigned int                RxCnt;
	unsigned char 	            WIAC82WaitForRxMsg;
	TIME                        TxTimestamp;
	TIME                        DataChangedTimer;
	TIME                        OffsetTimeLatency;
	TIME                        VelocityLatency;			// FighterNorth/East/Down Speed
	TIME                        AttitudeLatency;			// INS True Heading/Pitch/Roll Angle
	TIME                        PositionLatency;			// FighterLatitude/Longitude/Altitude
	TIME                        RatesLatency; 				// INS Roll/Yaw/Pitch Rate
	unsigned int                ErrCnt;
	unsigned char 				WIAC82ExecuteArming;
	unsigned char               PreviousCriticalMonitorArmed;
	unsigned char               PreviousReleaseConsent;
	unsigned char 				WIAC82ReleaseConsent;
	unsigned char 				WIAC82CommitToSeparate;
	char 						AuxStr[AUX_FIFO_SIZE];
} sWIAC82_StateSpace;

extern sWIAC82_StateSpace WIAC82_SS;
//float Get1WordSFloat(int16_t Word,float ScaleFactor);
//float Get1WordFloat(uint16_t Word,float ScaleFactor);
//double Get1WordDouble(uint16_t Word,double ScaleFactor);
//float Get2WordFloat(uint16_t *Word1,float ScaleFactor);
//double Get2WordDouble(uint16_t *Word1,double ScaleFactor);
void WIAC82_Init(char *AuxStr);
void WIAC82_SendSBIT_Request(void);
void WIAC82_SendStatus_Request(void);
void WIAC82_SendNormalWrap(void);
void WIAC82_SendBrokenWrap(void);
void WIAC82_SendStoreDescriptionReport(void);
void WIAC82_SendWeaponStatusWord(void);
void WIAC82_SendStoreMonitorReport(void);
void WIAC82HandleRxData(unsigned char *RxData, unsigned int Timestamp);
void WIAC82_Send(void);
void WIAC82_MisfiredWeapon(void);
char *DecodeValidity(uint16_t Flag, uint16_t Inverted);
void WIAC82_SendBatteryDeactivation(void);
void WIAC82_SendExecuteArmingBattery(void);


#endif /* WIAC82_H_ */
