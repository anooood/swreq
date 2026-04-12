/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <WIAC82_Comms.h>
 *
 *  Description:
 *  ------------
 *  Header file for the WIAC82_Comms module.
 *
 ******************************************************************************/

#include <math.h>
#include <stdint.h>

#ifndef WIAC82_COMMS_H_
#define WIAC82_COMMS_H_

#define WIAC82_MLAN_HEADER_SIZE             12
#define WIAC82_MLAN_VERSION         		2
#define WIAC82_BC_MESSAGE_SIZE_WORDS   		32 //16 - bit words
#define WIAC82_BC_MESSAGE_SIZE_BYTES   		64 //8 - bit words

// 1760 Messages
// BC to Weapon
#define WIAC82_AIRCRAFT_DESCIPTION_MESSAGE             	1
#define WIAC82_SYSTEM_TIME_MESSAGE			           	2
#define WIAC82_GENERAL_REFUSAL_MESSAGE	             	3
#define WIAC82_WEAPON_CONFIGURATION_MESSAGE_8R          8
#define WIAC82_WEAPON_CONFIGURATION_MESSAGE             10
#define WIAC82_STORE_CONTROL_MESSAGE             		11
#define WIAC82_STATE_VECTOR_50HZ			            14
#define WIAC82_STATE_VECTOR_6P25HZ_MESSAGE             	16
#define WIAC82_FUNCTIONAL_EXCHANGE_1D_MESSAGE           21
#define WIAC82_FUNCTIONAL_EXCHANGE_3D_MESSAGE           22
#define WIAC82_SATELLITE_USE_PARAMETERS		            26
#define WIAC82_GPS_INITIALIZATION_MESSAGE   			29
#define WIAC82_GPS_INITIALIZATION_EPHEMERIDES_MESSAGE   29
#define WIAC82_GPS_INITIALIZATION_ALMANAC_MESSAGE   	29
#define WIAC82_EQUIPMENT_DIALOGUE_TEST_MESSAGE			30

// Weapon to BC

#define WIAC82_STORES_DESCRIPTION_REPORT_MESSAGE_1T     1
#define WIAC82_STATUS_WORD_WEAPON_MESSAGE_8T           	8
#define WIAC82_FUNCTIONAL_EXCHANGE_3M_MESSAGE_17T      	17
#define WIAC82_STORE_MONITOR_REPORT_MESSAGE_11T        	11
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_MESSAGE_23T       23
#define WIAC82_RETURN_DIALOGUE_TEST_MESSAGE_30T        	30


#define WIAC82_STORES_DESCRIPTION_HEADER             0x0421				// Applies to (1T) and (1R)


typedef struct
{
  uint8_t SourceAddr;
  uint8_t DestAddr;
  uint8_t Opcode;
  uint8_t Version;
  uint16_t DataLength;
  uint8_t SequenceNo;
  uint8_t SequenceCnt;
  uint32_t FrameCnt;
} sWIAC82_MLAN_Header;

typedef union
{
  uint8_t Byte;
  struct
  {
    uint8_t Address         : 5;
    uint8_t AddressParity   : 1;
    uint8_t ReleaseConsent  : 1;
    uint8_t WpnActivate     : 1;
  } Bits;
} bWIAC82_MIL1553_Discretes;

typedef union
{
  uint16_t Byte;
  struct
  {
	uint16_t ParErr                  : 1;
	uint16_t ErrSpare                : 6;
	uint16_t ErrBusy                 : 1;
	uint16_t WrnSpare                : 7;
	uint16_t WrnBusy                 : 1;
  } Bits;
} bWIAC82_SBIT;

// Transmitted messages to WIAC82
typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;               // 0
} sWIAC82_SBIT_Request_Msg;                  //

typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;               // 0
} sWIAC82_StatusRequest_Msg;                  //
typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;               // 0
} sWIAC82_BrokenWrap_Msg;                  //
typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;               // 0
} sWIAC82_NormalWrap_Msg;                  //
typedef struct
{
	uint8_t SubAddr;
	uint8_t RTAddr;
	uint8_t Spare[2];
	int32_t DiffTime;
} sWIAC82_1553_Header;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;
	sWIAC82_1553_Header MIL1553Header;// 0
	union {
		uint16_t Words[32];
		uint8_t  Bytes[64];
	} buf;
} sWIAC82_1553_MLAN_Message;

// Received messages from WIAC82
typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;               // 0
  uint32_t SerialNo;                        // 12
  uint16_t LoaderVer;                       // 16
  uint16_t LoaderCRC;                       // 18
  uint16_t AppVer;                          // 20
  uint16_t AppCRC;                          // 22
  uint8_t  RTAddr;                          // 24
  uint8_t  Spare1;                          // 25
  uint16_t Spare2;                          // 26
  bWIAC82_SBIT    SBIT;                     // 28
  uint16_t Spare3;                          // 30
} sWIAC82_SBIT_Msg;                           // Size = 20 (without MLAN header)

typedef struct
{
  sWIAC82_MLAN_Header MLANHeader;                      // 0
  int8_t   Temperature;                     // 12
  uint8_t  ActivationCnt;                   // 13
  uint8_t  ArcnetRxMsgErrCnt;               // 14
  uint8_t  ArcnetTxMsgErrCnt;               // 15
  uint16_t ArcnetMyReconCnt;                // 16
  uint16_t NotUsed1; //uITU_CBIT CBIT;                           // 18
  uint16_t NotUsed2; //uDiscreteInputs InputDiscretesStatus;     // 20
  uint16_t NotUsed3; //uDiscreteOutputs OutputDiscretesStatus;   // 22
  uint16_t NotUsed4; //uSTC_Inputs STC_Status;                   // 24
  uint16_t NotUsed5; //uITU_Command CommandReceived;             // 26
  uint8_t  NotUsed6; //uOBH_Inputs OBH_Id;                       // 28
  uint8_t FuzeId;                           // 29
  bWIAC82_MIL1553_Discretes MIL1553_Discretes;     // 30
  uint8_t Spare;                            // 31
} sWIAC82_StatusMsg;                           // Size = 20 (without MLAN header)

// 1760 Messages **********************************************************************************
// BC to Weapon ***********************************************************************************
// AIRCRAFT DESCRIPTION MESSAGE (1R) *******************************************************************************************
// Station number and pylon/bay identifier
#define 	INVALID_STATION_NUMBER				0b0000
#define 	CENTRE_LINE_STATION_NUMBER			0b1000
#define 	LEFT_WING_STATION_NUMBER			0b1101
#define 	RIGHT_WING_STATION_NUMBER			0b1110
// Adaptor Identity
#define 	INVALID_ADAPTOR_ID					0b0000
#define 	CLB900								0b0001
#define 	AT730								0b0010
// Position Under Adaptor
#define 	INVALID_POSITION					0b0000
#define 	CENTRE_POSITION						0b0001
#define 	LEFT_POSITION						0b0010
#define 	RIGHT_POSITION						0b0011

//#define 	TIME_SCALE_FACTOR					(float)100000			//1E5 [s]
//#define 	FIGHTER_SPEED_SCALE_FACTOR			(float)2097152			//2^21
//#define 	INS_ANGLE_SCALE_FACTOR				(float)65536			//2^16
//#define 	INS_RATE_SCALE_FACTOR				(float)64				//2^6
//#define 	D_FIGHTER_SPEED_SCALE_FACTOR		(double)2097152			//2^21

typedef struct
{
	uint16_t Reserved 					:4;		// 0
	uint16_t admPositionUnderAdaptor 	:4;		// 0000b = Invalid
												// 0001b = Center
												// 0010b = Left
												// 0011b = Right
	uint16_t admAdaptorIdentity 		:4;		// 0000b = Invalid
												// 0001b = CLB900
												// 0010b = AT730
	uint16_t admStationNumber 			:4;		// 0000b = Invalid
												// 1000b = Center Line
												// 1101b = Left Wing (P2G)
												// 1110b = Right Wing (P2D)
} sWIAC82_Pylon_Bay_ID;
/*
typedef struct
{
	uint16_t admStationNumber 			:4;		// 0000b = Invalid
												// 1000b = Center Line
												// 1101b = Left Wing (P2G)
												// 1110b = Right Wing (P2D)
	uint16_t admAdaptorIdentity 		:4;		// 0000b = Invalid
												// 0001b = CLB900
												// 0010b = AT730
	uint16_t admPositionUnderAdaptor 	:4;		// 0000b = Invalid
												// 0001b = Center
												// 0010b = Left
												// 0011b = Right
	uint16_t Reserved 					:4;		// 0
} sWIAC82_Pylon_Bay_ID;
*/

#define WIAC82_AIRCRAFT_DESCRIPTION_MESSAGE_RESERVED_SIZE  	16
typedef struct
{
	uint16_t Header;						 	// 0x0421
	uint16_t admInvalidity1;					// Invalidity for words 1 - 16
	uint16_t admInvalidity2;					// Invalidity for words 17 - 30
	uint16_t CountryCode;						// ASCII ='FR'
	uint16_t admAircraftID1;					// ASCII ='RA'
	uint16_t admAircraftID2;					// ASCII ='FA'
	uint16_t admAircraftID3;					// ASCII ='LE'
	uint16_t admAircraftID4;					// ASCII ='  '
	uint16_t admAircraftID5;					// ASCII ='  '
	uint16_t admAircraftID6;					// ASCII ='  '
	uint16_t admAircraftID7;					// ASCII ='  '
	uint16_t admAircraftID8;					// ASCII ='  '
	uint16_t PBI;								// sWIAC82_Pylon_Bay_ID
	uint16_t Reserved[WIAC82_AIRCRAFT_DESCRIPTION_MESSAGE_RESERVED_SIZE];
	uint16_t CRC;
}sWIAC82_1553_Aircraft_Description_Message;

typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;// 0
	sWIAC82_1553_Aircraft_Description_Message AC_Descr_Msg;
} sWIAC82_1553_MLAN_Aircraft_Description_Message;

// SYSTEM TIME MESSAGE (2R) *******************************************************************************************
typedef struct
{
	uint16_t SystemTime1;						// Max=42949.67295 Min=0 LSB=0.00001 [s]
	uint16_t SystemTime2;
}sWIAC82_1553_System_Time_Message;

typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_System_Time_Message AC_1553_System_Time;
} sWIAC82_1553_MLAN_System_Time_Message;

// GENERAL REFUSAL MESSAGE (3R)		CUSTOM DASSAULT *******************************************************************************************
#define WIAC82_GLOBAL_REFUSAL	0b1111

typedef struct
{
	uint16_t Reserved		:12;
	uint16_t Cause			:4;					// 1111b - Global Refusal
}sWIAC82_1553_Cause;
/*
typedef struct
{
	uint16_t Cause			:4;					// 1111b - Global Refusal
	uint16_t Reserved		:12;
}sWIAC82_1553_Cause;
*/
typedef struct
{
	uint16_t Reserved;
	uint16_t Cause;		// sWIAC82_1553_Cause
	uint16_t Global_ID;
}sWIAC82_1553_Refusal_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_Refusal_Message General;
} sWIAC82_1553_MLAN_Refusal_Message;

// CONFIGURATION MESSAGE (10R) *******************************************************************************************
#define	WIAC82_AIRPLANE_RAFALE		0b0001
#define	WIAC82_AIRPLANE_M2000		0b0100

#define	WIAC82_AIRPLANE_POSITION_REAR_LEFT			0b0001
#define	WIAC82_AIRPLANE_POSITION_REAR_RIGHT			0b0001
#define	WIAC82_AIRPLANE_POSITION_FRONT_LEFT			0b0001
#define	WIAC82_AIRPLANE_POSITION_FRONT_RIGHT		0b0001
#define	WIAC82_AIRPLANE_POSITION_LEFT_ADAPTOR		0b0001
#define	WIAC82_AIRPLANE_POSITION_CENTRAL_ADAPTOR	0b0001
#define	WIAC82_AIRPLANE_POSITION_RIGHT_ADAPTOR		0b0001

typedef struct
{
	uint16_t PosD			:4;		// See PosA
	uint16_t PosC			:4;		// See PosA
	uint16_t PosB			:4;		// See PosA
	uint16_t PosA 			:4;		// 0000b = Undefined Address
									// 0001b = Rear Left Side
									// 0010b = Rear Right Side
									// 0011b = Front Left Side
									// 0100b = Front Right Side
									// 0101b = Left Adaptor Charge
									// 0110b = Central Adaptor Charge
									// 0111b = Right Adaptor Charge
									// 1000b = Unused 8
									// 1101b = Unused 13
									// 1110b = Unused 14
}sWIAC82_Charge_Position_Address;
/*
typedef struct
{
	uint16_t PosA 			:4;		// 0000b = Undefined Address
									// 0001b = Rear Left Side
									// 0010b = Rear Right Side
									// 0011b = Front Left Side
									// 0100b = Front Right Side
									// 0101b = Left Adaptor Charge
									// 0110b = Central Adaptor Charge
									// 0111b = Right Adaptor Charge
									// 1000b = Unused 8
									// 1101b = Unused 13
									// 1110b = Unused 14
	uint16_t PosB			:4;		// See PosA
	uint16_t PosC			:4;		// See PosA
	uint16_t PosD			:4;		// See PosA
}sWIAC82_Charge_Position_Address;
*/
typedef struct
{
	uint16_t Reserved		:11;
	uint16_t TestOrder		:1;
	uint16_t Position		:4;
}sWIAC82_Position_Adaptor;
/*
typedef struct
{
	uint16_t Position		:4;
	uint16_t TestOrder		:1;
	uint16_t Reserved		:11;
}sWIAC82_Position_Adaptor;
 */
typedef struct
{
	uint16_t CP2								:4;		// sWIAC82_Charge_Position_Address
	uint16_t CP1								:4;		// sWIAC82_Charge_Position_Address
	uint16_t Airplane_Type						:4;		// 0000b = InValid
														// 0001b = Rafale
														// 0100b = M2000
														// 0101b = Unused 5
														// 0111b = Unused 7
	uint16_t Reserved							:2;
	uint16_t Validity_Integrity_Test			:1;		// 0= InValid, 1=Valid
	uint16_t Validity_Setup						:1;		// 0= InValid, 1=Valid
} sWIAC82_Mixed_Config;
/*
typedef struct
{
	uint16_t Validity_Setup						:1;		// 0= InValid, 1=Valid
	uint16_t Validity_Integrity_Test			:1;		// 0= InValid, 1=Valid
	uint16_t Reserved							:2;
	uint16_t Airplane_Type						:4;		// 0000b = InValid
														// 0001b = Rafale
														// 0100b = M2000
														// 0101b = Unused 5
														// 0111b = Unused 7
	uint16_t CP1								:4;		// sWIAC82_Charge_Position_Address
	uint16_t CP2								:4;		// sWIAC82_Charge_Position_Address
} sWIAC82_Mixed_Config;
*/
typedef struct
{
	uint16_t Identifier;
	uint16_t Length;							// bytes default 22
	uint16_t Mixed1;							// sWIAC82_Mixed_Config
	uint16_t CP3_6;								// sWIAC82_Charge_Position_Address
	uint16_t CP7_10;
	uint16_t CP11_14;
	uint16_t CP15_18;
	uint16_t CP19_22;
	uint16_t CP23_26;
	uint16_t CP27_30;
	uint16_t Mixed2;							// sWIAC82_Position_Adaptor
	uint16_t Reserved;
	uint16_t CRC;								// From word 3 to word 12
} sWIAC82_1553_Configuration_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;
	sWIAC82_1553_Configuration_Message Config;
} sWIAC82_1553_MLAN_Configuration_Message;

// STORE CONTROL MESSAGE (11R) *****************************************************************************
#define WIAC82_STORES_CONTROL_HEADER			0x0400

#define WIAC82_SCM_MISSION_STORE 				1
#define WIAC82_SCM_CARRIAGE_STORE 				2

typedef struct
{
	uint16_t RTaddress					:5;
	uint16_t scmIdentifier				:3;		// D0 D1 D2
	uint16_t ReleaseModeD3				:1;
	uint16_t InitiateIBitD4				:1;
	uint16_t SelectStoreD5				:1;
	uint16_t PreSetArmingD6				:1;
	uint16_t ExecuteArmingD7			:1;
	uint16_t CommitToSeperateD8			:1;
	uint16_t JettisonD9					:1;
	uint16_t ReleaseD10					:1;		// Fire, Launch and Release
} sWIAC82_Store_Critical_Control_1;
/*
typedef struct
{
	uint16_t Release					:1;		// Fire, Launch and Release
	uint16_t Jettison					:1;
	uint16_t CommitToSeperate			:1;
	uint16_t ExecuteArming				:1;
	uint16_t PreSetArming				:1;
	uint16_t SelectStore				:1;
	uint16_t InitiateInterruptiveBit	:1;
	uint16_t ReleaseMode				:1;
	uint16_t scmIdentifier				:3;
	uint16_t RTaddress					:5;
} sWIAC82_Store_Critical_Control_1;
*/
typedef struct
{
	uint16_t RTaddress					:5;
	uint16_t scmIdentifier				:3; // D0 - D2
	uint16_t ReservedD3					:1;
	uint16_t ReservedD4					:1;
	uint16_t ActivateReleaseFunctionsD5	:1;
	uint16_t AbortReleaseD6				:1;
	uint16_t PreLaunchInhibitD7			:1;
	uint16_t RFemissionD8				:1;
	uint16_t RFjamD9					:1;
	uint16_t EraseD10					:1;		//
} sWIAC82_Store_Critical_Control_2;
/*
typedef struct
{
	uint16_t Erase						:1;		//
	uint16_t RFjam						:1;
	uint16_t RFemission					:1;
	uint16_t PreLaunchInhibit			:1;
	uint16_t AbortRelease				:1;
	uint16_t SelectStore				:1;
	uint16_t ActivateReleaseFunctions	:1;
	uint16_t Reserved					:1;
	uint16_t scmIdentifier				:3;
	uint16_t RTaddress					:5;
} sWIAC82_Store_Critical_Control_2;
*/
#define WIAC82_STORE_CONTROL_MESSAGE_RESERVED_SIZE 22
typedef struct
{
	uint16_t Header;							// 0x0400
	uint16_t InValidity1to16;
	uint16_t InValidity17t30;
	uint16_t Control1;							// sWIAC82_Store_Critical_Control_1
	uint16_t scmCCCA1;							// Coded check for Critical Control
	uint16_t Control2;							// sWIAC82_Store_Critical_Control_2
	uint16_t scmCCCA2;							// Coded check for Critical Control
//	uint16_t Reserved1;
	uint16_t Reserved[WIAC82_STORE_CONTROL_MESSAGE_RESERVED_SIZE];
	uint16_t CRC;
} sWIAC82_1553_Store_Control_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;// 0
	sWIAC82_1553_Store_Control_Message Store;
} sWIAC82_1553_MLAN_Store_Control_Message;

// STATE VECTOR 50Hz MESSAGE (14R) ***********************************************************
#define WIAC82_IMU1_VALIDITY	0b00
#define WIAC82_IMU2_VALIDITY	0b01

typedef struct
{
	uint16_t AC_IMU_ReferenceNumber	:2; // OOb = IMU1 01b = IMU2
	uint16_t Reserved				:6;
	uint16_t AngleOfSideslip 		:1;
	uint16_t AngleOfAttack 			:1;
	uint16_t FighterDownSpeed 		:1;
	uint16_t FighterEastSpeed		:1;
	uint16_t FighterNorthSpeed 		:1;
	uint16_t INS_PQR				:1;
	uint16_t INS_RollPitchAngle		:1;
	uint16_t INS_TrueHeading		:1;
}sWIAC82_ValidityAndIMUNumber;
/*
typedef struct
{
	uint16_t INS_TrueHeading		:1;
	uint16_t INS_RollPitchAngle		:1;
	uint16_t INS_PQR				:1;
	uint16_t FighterNorthSpeed 		:1;
	uint16_t FighterEastSpeed		:1;
	uint16_t FighterDownSpeed 		:1;
	uint16_t AngleOfAttack 			:1;
	uint16_t AngleOfSideslip 		:1;
	uint16_t Reserved				:6;
	uint16_t AC_IMU_ReferenceNumber	:2; // OOb = IMU1 01b = IMU2
}sWIAC82_ValidityAndIMUNumber;
*/

typedef struct
{
	uint16_t HeadingTime1;
	uint16_t HeadingTime2;
	uint16_t RollPitchTime1;
	uint16_t RollPitchTime2;
	uint16_t HorzSpeedTime1;
	uint16_t HorzSpeedTime2;
	uint16_t VertSpeedTime1;
	uint16_t VertSpeedTime2;
	uint16_t ABSpeedTime1;
	uint16_t ABSpeedTime2;
	uint16_t ValidityAndACIMUNumber;			// sWIAC82_ValidityAndIMUNumber
	uint16_t INS_TrueHeading;
	uint16_t INS_RollAngle;
	uint16_t INS_PitchAngle;
	uint16_t INS_RollRate;
	uint16_t INS_PitchRate;
	uint16_t INS_YawRate;
	uint16_t FighterNorthSpeed1;
	uint16_t FighterNorthSpeed2;
	uint16_t FighterEastSpeed1;
	uint16_t FighterEastSpeed2;
	uint16_t FighterDownSpeed1;
	uint16_t FighterDownSpeed2;
	uint16_t AngleOfAttack;
	uint16_t AngleOfSideslip;
} sWIAC82_1553_StateVector50Hz_Message;

typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_StateVector50Hz_Message Vec50Hz;
} sWIAC82_1553_MLAN_StateVector50Hz_Message;

// STATE VECTOR 6.25HZ MESSAGE (16R)
#define WIAC82_IMU_REFERENCE_NUMBER1		0
#define WIAC82_IMU_REFERENCE_NUMBER2		1

typedef struct
{
	uint16_t AC_IMU_ReferenceNumber			:2; // OOb = IMU1 01b = IMU2
	uint16_t Reserved						:7;
	uint16_t WindDirection 					:1;
	uint16_t WindSpeed  					:1;
	uint16_t FighterAltitude 				:1;
	uint16_t FighterLongitude  				:1;
	uint16_t FighterLatitude 				:1;
	uint16_t AircraftTrueAirspeed 			:1;
	uint16_t AircraftCalibratedAirspeed		:1;
}sWIAC82_StateVector6p25HzValidity;
/*
typedef struct
{
	uint16_t AircraftCalibratedAirspeed		:1;
	uint16_t AircraftTrueAirspeed 			:1;
	uint16_t FighterLatitude 				:1;
	uint16_t FighterLongitude  				:1;
	uint16_t FighterAltitude 				:1;
	uint16_t WindSpeed  					:1;
	uint16_t WindDirection 					:1;
	uint16_t Reserved						:7;
	uint16_t AC_IMU_ReferenceNumber			:2; // OOb = IMU1 01b = IMU2
}sWIAC82_StateVector6p25HzValidity;

 */
typedef struct
{
	uint16_t AeroTimeTag1;					// LSB=1E-5 Max Val =42949.67295
	uint16_t AeroTimeTag2;
	uint16_t BestPositionAndWindTimeTag1; 	// LSB=1E-5 Max Val =42949.67295
	uint16_t BestPositionAndWindTimeTag2; 	// LSB=1E-5 Max Val =42949.67295
	uint16_t Validity; 						// sWIAC82_StateVector6p25HzValidity
	uint16_t AircraftCalibratedAirspeed;
	uint16_t AircraftTrueAirspeed;
	uint16_t FighterLatitude1;
	uint16_t FighterLatitude2;
	uint16_t FighterLongitude1;
	uint16_t FighterLongitude2;
	uint16_t FighterAltitude1;
	uint16_t FighterAltitude2;
	uint16_t WindSpeed;
	uint16_t WindDirection;
	uint16_t Reserved1;
	uint16_t Reserved2;
} sWIAC82_1553_StateVector6p25Hz_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_StateVector6p25Hz_Message Vec6p25Hz;
} sWIAC82_1553_MLAN_StateVector6p25Hz_Message;

// FUNCTIONAL EXCHANGE 1D MESSAGE (21R)
//#define 	FUNCTIONAL_EXCHANGE_ANGLE_SCALE_FACTOR					(float)4294967296			//2^32 [turn]
//#define 	FUNCTIONAL_EXCHANGE_POSITION_SCALE_FACTOR				(float)1024					//2^10 [m]

typedef struct
{
	uint16_t Reserved				:14;
	uint16_t LeverArmValid			:1;
	uint16_t AngleOffsetValid		:1;

}FunctionalExchange1ValidityWord;

typedef struct
{
	uint16_t Identifier;				// default 1
	uint16_t UsefulLength;				// Wagon header not to be considered as part of CRC
	uint16_t Validity; 					// FunctionalExchange1ValidityWord
	uint16_t ACLevArmRollAngOffset1;
	uint16_t ACLevArmRollAngOffset2;
	uint16_t ACLevArmPitchAngOffset1;
	uint16_t ACLevArmPitchAngOffset2;
	uint16_t ACLevArmYawAngOffset1;
	uint16_t ACLevArmYawAngOffset2;
	uint16_t ACLeverArmXoffsetlMU1;
	uint16_t ACLeverArmYoffsetlMU1;
	uint16_t ACLeverArmZoffsetlMU1;
	uint16_t Reserve1;
	uint16_t Reserve2;
	uint16_t Reserve3;
	uint16_t Reserve4;
	uint16_t Reserve5;
	uint16_t Reserve6;
	uint16_t ACLeverArmXoffsetlMU2;
	uint16_t ACLeverArmYoffsetlMU2;
	uint16_t ACLeverArmZoffsetlMU2;
	uint16_t GPSLeverArmXoffset;
	uint16_t GPSLeverArmYoffset;
	uint16_t GPSLeverArmZoffset;
	uint16_t CRC;
} sWIAC82_1553_FunctionalExchange1D_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_FunctionalExchange1D_Message FuncExch;
} sWIAC82_1553_MLAN_FunctionalExchange1D_Message;

// FUNCTIONAL EXCHANGE 3D MESSAGE (22R) *************************************************************************
#define WIAC82_SEEKER_SETTING_GNSS_ONLY			0b00
#define WIAC82_SEEKER_SETTING_SAL				0b01
#define WIAC82_SEEKER_SETTING_IIR				0b10

#define WIAC82_GNSS_MODE_ALL_OFF				0b000
#define WIAC82_GNSS_MODE_GLONASS_OFF_GPS_ON		0b001
#define WIAC82_GNSS_MODE_GLONASS_ON_GPS_OFF		0b010
#define WIAC82_GNSS_MODE_GLONASS_ON_GPS_ON		0b011
#define WIAC82_GNSS_MODE_RESERVED				0b100

#define WIAC82_TARGET_FIXED						0
#define WIAC82_TARGET_MOVING					1

#define WIAC82_FUZE_IMPACT						0b00
#define WIAC82_FUZE_DELAY						0b01
#define WIAC82_FUZE_AIRBURST					0b10
#define WIAC82_FUZE_PROGRAMMING					0b11

#define WIAC82_TARGET_ABSOLUTE					0
#define WIAC82_TARGET_RELATIVE					1

//#define TARGET_LAT_LON_SCALE_FACTOR			(float)4294967296		//2^32 [turn]
//#define TARGET_ALT_SCALE_FACTOR				(float)65536			//2^16 [m]
//#define DIVE_ANGLE_SCALE_FACTOR				(float)131072			//2^17 [turn]
//#define IMPACT_HEADING_SCALE_FACTOR			(float)65536			//2^16 [turn]

//Fuze settings
#define WIAC82_FUZE_SETTING_AT_IMPACT				0
#define WIAC82_FUZE_SETTING_TIME_AFTER_IMPACT		1   //Delay
#define WIAC82_FUZE_SETTING_AT_ALTITUDE				2   //Airburst
#define WIAC82_FUZE_SETTING_IN_PROGRAMMING			3

typedef struct
{
	uint16_t Reserved						:7;
	uint16_t TargetRelative 				:1;			// 0 = absolute, 1 = relative
	uint16_t FuzeSetting					:2;			// 00b - At Impact
														// 01b - Time after Impact (Delay)
														// 10b -At Altitude (Airburst)
														// 11b = In programming
	uint16_t MovingTargetActive				:1;			// 0 = Fixed Target, 1 = Moving Target
	uint16_t GNSSMode						:3;			// 000b - All Constellations OFF
														// 001b - GLONASS OFF, GPS ON
														// 01Ob - GLONASS ON, GPS OFF
														// 011b - GLONASS ON, GPS ON
														// 1xxb - Reserved
	uint16_t SeekerSetting					:2;			// 00b - GNSS Only
														// 01b- SAL
														// 10b - IIR (Al Tariq)
}bFunctionalExchange2MissionSettings;
/*
typedef struct
{
	uint16_t SeekerSetting					:2;			// 00b - GNSS Only
														// 01b- SAL
														// 10b - IIR (Al Tariq)
	uint16_t GNSSMode						:3;			// 000b - All Constellations OFF
														// 001b - GLONASS OFF, GPS ON
														// 01Ob - GLONASS ON, GPS OFF
														// 011b - GLONASS ON, GPS ON
														// 1xxb - Reserved
	uint16_t MovingTargetActive				:1;			// 0 = Fixed Target, 1 = Moving Target
	uint16_t FuzeSetting					:2;			// 00b - At Impact
														// 01b - Time after Impact (Delay)
														// 10b -At Altitude (Airburst)
														// 11b = In programming
	uint16_t TargetRelative 				:1;			// 0 = absolute, 1 = relative
	uint16_t Reserved						:7;
}bFunctionalExchange2MissionSettings;
*/
typedef struct
{
	uint16_t Reserved						:7;
	uint16_t GNSS_HybridationActivation		:1;
	uint16_t FuzeSettingDelayHeight			:1;
	uint16_t MovingTargetMode 				:1;
	uint16_t ImageIndexCRC					:1;
	uint16_t SALCode						:1;
	uint16_t ImpactSpeed 					:1;
	uint16_t ImpactHeading 					:1;
	uint16_t DiveAngle						:1;
	uint16_t TargetLongLatAlt				:1;
}bFunctionalExchange2ValidityWord;
/*
typedef struct
{
	uint16_t TargetLongLatAlt				:1;
	uint16_t DiveAngle						:1;
	uint16_t ImpactHeading 					:1;
	uint16_t ImpactSpeed 					:1;
	uint16_t SALCode						:1;
	uint16_t ImageIndexCRC					:1;
	uint16_t MovingTargetMode 				:1;
	uint16_t FuzeSettingDelayHeight			:1;
	uint16_t GNSS_HybridationActivation		:1;
	uint16_t Reserved						:7;
}bFunctionalExchange2ValidityWord;
*/
#define WIAC82_MAX_FUSE_DELAY				240

typedef struct
{
	uint16_t Identifier;						// default 1
	uint16_t UsefulLength;						// Wagon header not to be considered as part of CRC
	uint16_t Validity;  						// FunctionalExchange2ValidityWord
	uint16_t TargetLatitude1;
	uint16_t TargetLatitude2;
	uint16_t TargetLongitude1;
	uint16_t TargetLongitude2;
	uint16_t TargetAltitude1;
	uint16_t TargetAltitude2;
	uint16_t DiveAngle;
	uint16_t ImpactHeading;
	uint16_t ImpactSpeed;
	uint16_t SALCode;
	uint16_t ImageIndex;
	uint16_t ImageChecksum;
	uint16_t MissionSettings; 					// FunctionalExchange2MissionSettings
	uint16_t FuzeDelay;							// [ms] Max = 240
	uint16_t FuzeHeight;						// [m] Max = 200
	uint16_t Reserved1;
	uint16_t Reserved2;
	uint16_t CRC;								// Calculated from word 3 to Word 20
} sWIAC82_1553_FunctionalExchange3D_Message;

typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_FunctionalExchange3D_Message FuncExch;
} sWIAC82_1553_MLAN_FunctionalExchange3D_Message;

// SATELLITE USE PARAMETERS (26R)
// SATELLITE STATUS PARAMETERS
#define WAIC82_SATELLITE_STATUS_UNUSED					0
#define WAIC82_SATELLITE_STATUS_SEARCHING				1
#define WAIC82_SATELLITE_STATUS_BIT_SYNC				2
#define WAIC82_SATELLITE_STATUS_SUB_FRAME_SYNC			3
#define WAIC82_SATELLITE_STATUS_LOCKED_C_A				4
#define WAIC82_SATELLITE_STATUS_NEW_ACQUISITION_C_A		5
#define WAIC82_SATELLITE_STATUS_LOCKED_P_Y				6
#define WAIC82_SATELLITE_STATUS_NEW_ACQUISITION_P_Y		7

// SATELLITE EPHEMERIS STATUS PARAMETERS
#define WAIC82_SATELLITE_EPHEMERIS_STATUS_INVALID		0
#define WAIC82_SATELLITE_EPHEMERIS_STATUS_VALID_UNUSED	1
#define WAIC82_SATELLITE_EPHEMERIS_STATUS_VALID_IN_USE	2
#define WAIC82_SATELLITE_EPHEMERIS_STATUS_UNKNOWN		3

typedef struct
{
	uint16_t Reserved					:6;
	uint16_t ChanBEphemerisStatus		:2;
	uint16_t ChanBStatus				:3;
	uint16_t ChanAEphemerisStatus		:2;
	uint16_t ChanAStatus				:3;
}sWIAC82_SatelliteAB_Status;
/*
typedef struct
{
	uint16_t ChanAStatus				:3;
	uint16_t ChanAEphemerisStatus		:2;
	uint16_t ChanBStatus				:3;
	uint16_t ChanBEphemerisStatus		:2;
	uint16_t Reserved					:6;
}sWIAC82_SatelliteAB_Status;

 */
typedef struct
{
	uint16_t Byte2				:8;
	uint16_t Byte1				:8;
}sWIAC82_Byte12;
/*
typedef struct
{
	uint16_t Byte1				:8;
	uint16_t Byte2				:8;
}sWIAC82_Byte12;

 */
typedef struct
{
	uint16_t Satellite16				:1;
	uint16_t Satellite15				:1;
	uint16_t Satellite14				:1;
	uint16_t Satellite13				:1;
	uint16_t Satellite12				:1;
	uint16_t Satellite11				:1;
	uint16_t Satellite10				:1;
	uint16_t Satellite09				:1;
	uint16_t Satellite08				:1;
	uint16_t Satellite07				:1;
	uint16_t Satellite06				:1;
	uint16_t Satellite05				:1;
	uint16_t Satellite04				:1;
	uint16_t Satellite03				:1;
	uint16_t Satellite02				:1;
	uint16_t Satellite01				:1;
}sWIAC82_Satellite_Used1;
/*
typedef struct
{
	uint16_t Satellite01				:1;
	uint16_t Satellite02				:1;
	uint16_t Satellite03				:1;
	uint16_t Satellite04				:1;
	uint16_t Satellite05				:1;
	uint16_t Satellite06				:1;
	uint16_t Satellite07				:1;
	uint16_t Satellite08				:1;
	uint16_t Satellite09				:1;
	uint16_t Satellite10				:1;
	uint16_t Satellite11				:1;
	uint16_t Satellite12				:1;
	uint16_t Satellite13				:1;
	uint16_t Satellite14				:1;
	uint16_t Satellite15				:1;
	uint16_t Satellite16				:1;
}sWIAC82_Satellite_Used1;
 */

typedef struct
{
	uint16_t Reserved					:7;
	uint16_t valHGPS 					:1;
	uint16_t valDPACI					:1;
	uint16_t valNSATC11_12				:1;
	uint16_t valNSATC09_10				:1;
	uint16_t valNSATC07_08				:1;
	uint16_t valNSATC05_06				:1;
	uint16_t valNSATC03_04				:1;
	uint16_t valNSATC01_02				:1;
	uint16_t valNumSAT					:1;
}sWIAC82_Satellite_Use_Parameters_Validity;
/*
  {
	uint16_t valNumSAT					:1;
	uint16_t valNSATC01_02				:1;
	uint16_t valNSATC03_04				:1;
	uint16_t valNSATC05_06				:1;
	uint16_t valNSATC07_08				:1;
	uint16_t valNSATC09_10				:1;
	uint16_t valNSATC11_12				:1;
	uint16_t valDPACI					:1;
	uint16_t valHGPS 					:1;
	uint16_t Reserved					:7;
}sWIAC82_Satellite_Use_Parameters_Validity;
 */
typedef struct
{
	uint16_t SatelliteValidity;			// sWIAC82_Satellite_Use_Parameters_Validity
	uint16_t UsedSatellites1;			// Each bit position set indicates satellite used.
	uint16_t UsedSatellites2;			// Bit 0 set = Satellite 01 used
										// Bit 8 set = Satellite 09 used
										// Bit 31 set= Satellite 32 used
	uint16_t SatChannelsID01p02;
	uint16_t ReceptionHealth01p02;
	uint16_t SignalNoise01p02;
	uint16_t SatChannelsID03p04;
	uint16_t ReceptionHealth03p04;
	uint16_t SignalNoise03p04;
	uint16_t SatChannelsID05p06;
	uint16_t ReceptionHealth05p06;
	uint16_t SignalNoise05p06;
	uint16_t SatChannelsID07p08;
	uint16_t ReceptionHealth07p08;
	uint16_t SignalNoise07p08;
	uint16_t SatChannelsID09p10;
	uint16_t ReceptionHealth09p10;
	uint16_t SignalNoise09p10;
	uint16_t SatChannelsID11p12;
	uint16_t ReceptionHealth11p12;
	uint16_t SignalNoise11p12;
	uint16_t TimeTagGPSInfo1;
	uint16_t TimeTagGPSInfo2;
	uint16_t GPSHour1;
	uint16_t GPSHour2;
	uint16_t GPSHourFraction;
	uint16_t DayMonth;
	uint16_t YearReserve;
}sWIAC82_1553_Satellite_Use_Parameters_Message;

typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_Satellite_Use_Parameters_Message SatParms;
} sWIAC82_1553_MLAN_Satellite_Use_Parameters_Message;

// GPS INITIALIZATION, ALMANAC (29R) CUSTOM DASSAULT MESSAGE
typedef struct
{
	uint16_t Identifier;						// default 2
	uint16_t UsefulLength;						// 38
	uint16_t Validity;
	uint16_t PseudoRandomNoise;
	uint16_t WeekNumber;
	uint16_t ReferenceTime;
	uint16_t SpaceVehicleHealth;
	uint16_t InclinationCorrection;
	uint16_t EccentricAnomaly;
	uint16_t SQRTSemiMajorAxis1;
	uint16_t SQRTSemiMajorAxis2;
	uint16_t MeanAnomaly1;
	uint16_t MeanAnomaly2;
	uint16_t RateRightAscension;
	uint16_t LongitudeAscendingNode1;
	uint16_t LongitudeAscendingNode2;
	uint16_t ArgumentPerigee1;
	uint16_t ArgumentPerigee2;
	uint16_t AlmanacParametersSubframeA;
	uint16_t AlmanacParametersSubframeB;
	uint16_t Almanac1ParametersSubframeA;
	uint16_t Almanac1ParametersSubframeB;
}sWIAC82_1553_GPS_InitializationALMANAC_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_GPS_InitializationALMANAC_Message Init;
} sWIAC82_1553_MLAN_GPS_InitializationALMANAC_Message;

// GPS INITIALIZATION, EPHEMERIDES (29R) CUSTOM DASSAULT MESSAGE
typedef struct
{
	uint16_t Identifier;						// default 3
	uint16_t UsefulLength;						// 56
	uint16_t Validity;
	uint16_t SatelliteID;
	uint16_t WeekNumber;
	uint16_t ReferenceTime;
	uint16_t SpaceVehicleHealth;
	uint16_t Eccentricity1;
	uint16_t Eccentricity2;
	uint16_t SQRTSemiMajorAxis1;
	uint16_t SQRTSemiMajorAxis2;
	uint16_t MeanAnomaly1;
	uint16_t MeanAnomaly2;
	uint16_t RateRightAscension1;
	uint16_t RateRightAscension2;
	uint16_t LongitudeAscendingNode1;
	uint16_t LongitudeAscendingNode2;
	uint16_t ArgumentPerigee1;
	uint16_t ArgumentPerigee2;
	uint16_t InclinationAngleReferenceTime1;
	uint16_t InclinationAngleReferenceTime2;
	uint16_t DataIssue;
	uint16_t InclinationAngleRate;
	uint16_t MeanMotionDiff;
	uint16_t LatCosHarmonicCorrection;
	uint16_t LatSinHarmonicCorrection;
	uint16_t RadiusCosHarmonicCorrection;
	uint16_t RadiusSinHarmonicCorrection;
	uint16_t InclinationCosHarmonicCorrection;
	uint16_t InclinationSinsHarmonicCorrection;
} sWIAC82_1553_GPS_InitializationEPHEMERIDES_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_GPS_InitializationEPHEMERIDES_Message Init;
} sWIAC82_1553_MLAN_GPS_InitializationEPHEMERIDES_Message;

// EQUIPMENT DIALOGUE TEST (30R)
typedef struct
{
	uint16_t Word; 		//Cycling of the three words each time a message is sent
						//0xA55A
						//0x5AA5
						//OxAAAA
						//Ask message circulates in the short cycle 1 to 2
						//Return message circulates in the short cycle 3 to 8
} sWIAC82_1553_EquipmentDiaglogueTest_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_EquipmentDiaglogueTest_Message Test;
} sWIAC82_1553_MLAN_EquipmentDiaglogueTest_Message;

// Weapon to BC ***********************************************************************************
// STORE DESCRIPTION REPORT (1T)

#define WIAC82_STORE_DESCRIPTION_REPORT_HEADER 		0x0421
#define WIAC82_STORE_DESCRIPTION_COUNTRY_CODE  		0x4145
#define WIAC82_STORE_DESCRIPTION_ID_WEAPON_TYPE  	0x5033
#define WIAC82_STORE_DESCRIPTION_ID_WEAPON_RANGE  	0x2020
#define WIAC82_STORE_DESCRIPTION_MAX_IBIT_TIME 		0x0000
#define WIAC82_STORE_DESCRIPTION_RESERVED_SIZE 		14

typedef struct
{
	uint16_t Header;										// 0x0421
	uint16_t CountryCode;									// ASCII ='AE'
	uint16_t StoreIdentityBinary;							// Default=0
	uint16_t IDWeaponType;									// 'P3'
	uint16_t IDWeaponRange;									// '  '
	uint16_t ID_BombGuidance;								// Default='3L'
															// 1 = Mk-81 bomb (AT & P3)
															// 2 = Mk-82 bomb (AT & P3)
															// 3 = Mk-83 bomb (AT)
															// G = GNSS only (AT & P3)
															// L = SAL seeker head (AT & P3)
															// R = IIR seeker head (AT)
	uint16_t ID_FuzeType;									// Default='F0'
															// F0 - Non-programmable fuze
															// F1 - Impact delay programmable
															// F2 - Impact delay + arm time programmable
															// F3 - Proximity distance programmable
															// F4 - Proximity distance + impact delay programmable
															// F5 - Proximity distance + impact delay + arm time programmable

	uint16_t StoreID5;										// Default='  '
	uint16_t StoreID6;										// Default='  '
	uint16_t StoreID7;										// Default='  '
	uint16_t StoreID8;										// Default='  '
	uint16_t MaxInterruptiveBIT_Time;						//
	uint16_t StoreConfigID1;								// Default='  '
	uint16_t StoreConfigID2;								// SW ICD Minor Version
	uint16_t StoreConfigID3;								// SW ICD Major Version
	uint16_t Reserved[WIAC82_STORE_DESCRIPTION_RESERVED_SIZE];
	uint16_t CRC;
} sWIAC82_1553_StoreDescriptionReport_Message;

typedef struct
{
	sWIAC82_MLAN_Header HeaderMLAN;               	// 0
	sWIAC82_1553_Header	Header1553;
	sWIAC82_1553_StoreDescriptionReport_Message Store;
} sWIAC82_1553_MLAN_StoreDescriptionReport_Message;

// STATUS WORD WEAPON (8T) **************************************************************************************
typedef struct
{
	uint16_t Reserved					:11;
	uint16_t PositionStatusValid		:1;
	uint16_t StatusPBITvalid			:1;
	uint16_t BatterySpareCapacityValid	:1;
	uint16_t StatusAlTariqValid 		:1;
	uint16_t StatusP32Valid				:1;
} sWIAC82_StatusWordWeapon_Validity;

/*
typedef struct
{
	uint16_t StatusP32Valid				:1;
	uint16_t StatusAlTariqValid 		:1;
	uint16_t BatterySpareCapacityValid	:1;
	uint16_t StatusPBITvalid			:1;
	uint16_t PositionStatusValid		:1;
	uint16_t Reserved					:11;
} sWIAC82_StatusWordWeapon_Validity;
*/

typedef struct
{
	uint16_t HOBS						:1;
	uint16_t Reserved3					:1;
	uint16_t GNSS_Receiver				:1;
	uint16_t IMU						:1;
	uint16_t FinLockPBIT				:1;
	uint16_t Reserved2					:1;
	uint16_t BatteryPBITandCBIT			:1;
	uint16_t SAL_PBITandCBIT			:1;
	uint16_t Reserved1					:1;
	uint16_t ServoPBITandCBIT			:1;
	uint16_t Config_BIT					:1;
	uint16_t HOBS_PBIT					:1;
	uint16_t GCA_BIT					:1;
	uint16_t GAINS_PBIT					:1;
	uint16_t IMU_PBIT					:1;
	uint16_t SWExecutionOverrun			:1;
} sWIAC82_Status1P32;

/*
typedef struct
{
	uint16_t SWExecutionOverrun			:1;
	uint16_t IMU_PBIT					:1;
	uint16_t GAINS_PBIT					:1;
	uint16_t Reserved1					:1;
	uint16_t HOBS_PBIT					:1;
	uint16_t Reserved2					:1;
	uint16_t ServoPBITandCBIT			:1;
	uint16_t Reserved3					:1;
	uint16_t SAL_PBITandCBIT			:1;
	uint16_t BatteryPBITandCBIT			:1;
	uint16_t Reserved4					:1;
	uint16_t FinLockPBIT				:1;
	uint16_t IMU						:1;
	uint16_t GNSS_Receiver				:1;
	uint16_t Reserved5					:1;
	uint16_t HOBS						:1;
} sWIAC82_Status1P32;
*/

typedef struct
{
	uint16_t Reserved5					:1;
	uint16_t WeaponDegraded				:1;
	uint16_t NAV						:1;
	uint16_t IF_PCB						:1;
	uint16_t Reserved4					:4;
	uint16_t TailSensor					:1;
	uint16_t Battery					:1;
	uint16_t Reserved3					:1;
	uint16_t ArcnetCBITErr				:1;
	uint16_t SAL						:1;
	uint16_t Reserved2					:1;
	uint16_t Servo						:1;
	uint16_t Reserved1					:1;
} sWIAC82_Status2P32;

/*
typedef struct
{
	uint16_t Reserved1					:1;
	uint16_t Servo						:1;
	uint16_t Reserved2					:1;
	uint16_t SAL						:1;
	uint16_t ArcnetCBITErr				:1;
	uint16_t Reserved3					:1;
	uint16_t Battery					:1;
	uint16_t TailSensor					:1;
	uint16_t Reserved4					:4;
	uint16_t IF_PCB						:1;
	uint16_t NAV						:1;
	uint16_t Reserved5					:2;
} sWIAC82_Status2P32;
*/

#define WIAC82_OVERALL_PBIT_NO_RESULT		0b000
#define WIAC82_OVERALL_PBIT_NO_GO			0b001
#define WIAC82_OVERALL_PBIT_DEGRADED		0b010
#define WIAC82_OVERALL_PBIT_GO				0b100

typedef struct
{
	uint16_t Reserved					:12;
	uint16_t PBIT_Result				:3; 	// 000b = No Results
												// 001b = NoGo
												// 010b = Go Degraded
												// 100b = Go
	uint16_t InProgress					:1;
} sWIAC82_StatusPBIT;

/*
typedef struct
{
	uint16_t InProgress					:1;
	uint16_t PBIT_Result				:3; 	// 000b = No Results
												// 001b = NoGo
												// 010b = Go Degraded
												// 100b = Go
	uint16_t Reserved					:12;
} sWIAC82_StatusPBIT;
*/
#define WIAC82_NO_GNSS_LOCK					0b000
#define WIAC82_GPS_LOCKED					0b001
#define WIAC82_GLONASS_LOCKED				0b010
#define WIAC82_GPS_GLONASS_LOCKED			0b011

typedef struct
{
	uint16_t Reserved					:4;
	uint16_t GNSS_Locked				:3;				// 000b = No lock
														// 001b = GPS Locked
														// 010b = GLONASS Locked
														// 011b = GPS and GLONASS Locked
	uint16_t BatterySpareCapacity		:8;
	uint16_t WepaonReady				:1;
} sWIAC82_StatusMode1;

/*
typedef struct
{
	uint16_t WepaonReady				:1;
	uint16_t BatterySpareCapacity		:8;
	uint16_t GNSS_Locked				:3;				// 000b = No lock
														// 001b = GPS Locked
														// 010b = GLONASS Locked
														// 011b = GPS and GLONASS Locked
	uint16_t Reserved					:4;
} sWIAC82_StatusMode1;
*/

typedef struct
{
	uint16_t GLONASS_SAT_Locked			:8;
	uint16_t GPS_SAT_Locked				:8;
} sWIAC82_StatusMode2;

/*
typedef struct
{
	uint16_t GPS_SAT_Locked				:8;
	uint16_t GLONASS_SAT_Locked			:8;
} sWIAC82_StatusMode2;
*/

typedef struct
{
	uint16_t Reserved					:14;
	uint16_t WeaponAlignment			:2;				// 00b = KO
														// 01b = Near Alignment Loss
														// 10b = OK
} sWIAC82_StatusMode3;

/*
typedef struct
{
	uint16_t WeaponAlignment			:2;				// 00b = KO
														// 01b = Near Alignment Loss
														// 10b = OK
	uint16_t Reserved					:14;
} sWIAC82_StatusMode3;
*/

#define WIAC82_STATUS_WORD_WEAON_RESERVED_WORDS		5

typedef struct
{
	uint16_t DataChanged;
	uint16_t ValidityStatusWord;
	uint16_t Status1P32;
	uint16_t Status2P32;
	uint16_t Status1AlTariq;
	uint16_t Status2AlTariq;
	uint16_t StatusPBIT;
	uint16_t StatusMode1;
	uint16_t StatusMode2;
	uint16_t StatusMode3;
	uint16_t PositionStatus;
	uint16_t Reserved[WIAC82_STATUS_WORD_WEAON_RESERVED_WORDS];
}sWIAC82_1553_StatusWordWeapon_Message;

typedef struct
{
	sWIAC82_MLAN_Header HeaderMLAN;               	// 0
	sWIAC82_1553_Header	Header1553;
	sWIAC82_1553_StatusWordWeapon_Message Status;
} sWIAC82_1553_MLAN_StatusWordWeapon_Message;


// STORE MONITOR REPORT (11T) *********************************************************************************
#define WIAC82_STORE_MONITOR_REPORT_HEADER 				0x0420
#define WIAC82_STORE_MONITOR_REPORT_RESERVED2_WORDS		22
typedef struct
{
	uint16_t ValReserved2			:9;
	uint16_t ValProtocolStatus		:1;
	uint16_t ValReserved1			:1;
	uint16_t ValCriticalMonitor2	:1;
	uint16_t ValCriticalMonitor1	:1;
	uint16_t InValidity2			:1;
	uint16_t InValidity1			:1;
	uint16_t ValHeader				:1;
} sWIAC82_1553_StoreMonitorReportValidity1;
/*
typedef struct
{
	uint16_t ValHeader				:1;
	uint16_t InValidity1			:1;
	uint16_t InValidity2			:1;
	uint16_t ValCriticalMonitor1	:1;
	uint16_t ValCriticalMonitor2	:1;
	uint16_t ValReserved1			:1;
	uint16_t ValProtocolStatus		:1;
	uint16_t ValReserved2			:9;
} sWIAC82_1553_StoreMonitorReportValidity1;

 */
typedef struct
{
	uint16_t NU						:2;
	uint16_t ValCRC					:1;
	uint16_t ValReserved2			:13;
} sWIAC82_1553_StoreMonitorReportValidity2;
/*
typedef struct
{
	uint16_t ValReserved2			:13;
	uint16_t ValCRC					:1;
	uint16_t NU						:2;
} sWIAC82_1553_StoreMonitorReportValidity2;

 */
typedef struct
{
	uint16_t ReleaseModeD3				:1;
	uint16_t InitiateIBitD4				:1;
	uint16_t SelectStoreD5				:1;
	uint16_t PreSetArmingD6				:1;
	uint16_t ExecuteArmingD7			:1;
	uint16_t CommitToSeperateD8			:1;
	uint16_t JettisonD9					:1;
	uint16_t ReleaseD10					:1;		// Fire, Launch and Release
	uint16_t LaunchModeR				:1;
	uint16_t InitiateIBitR				:1;
	uint16_t SelectStoreR				:1;
	uint16_t PresetArmingR				:1;
	uint16_t ArmedR						:1;
	uint16_t CommittedToStoreSeparateR	:1;
	uint16_t JettisonR					:1;
	uint16_t ReleasedR					:1;
} sWIAC82_1553_StoreMonitorReportCriticalMonitor1;
/*
typedef struct
{
	uint16_t Released					:1;
	uint16_t JettisonR					:1;
	uint16_t CommittedToSeparate		:1;
	uint16_t ExecuteArmingR				:1;
	uint16_t PresetArmingR				:1;
	uint16_t SelectStoreR				:1;
	uint16_t InitiateInterruptiveBitR	:1;
	uint16_t LaunchMode					:1;

	uint16_t Release					:1;		// Fire, Launch and Release
	uint16_t Jettison					:1;
	uint16_t CommitToSeperate			:1;
	uint16_t ExecuteArming				:1;
	uint16_t PreSetArming				:1;
	uint16_t SelectStore				:1;
	uint16_t InitiateInterruptiveBit	:1;
	uint16_t ReleaseMode				:1;
} sWIAC82_1553_StoreMonitorReportCriticalMonitor1;
*/
typedef struct
{
	uint16_t CommandedStoreState			:8;
	uint16_t GlobalDeletionReport 			:2;
	uint16_t ActivateReleaseFunctions		:1;
	uint16_t AbortRelease					:1;
	uint16_t ControlSurfaceInhibit			:1;
	uint16_t RFemissionCommand				:1;
	uint16_t RFjamCommand					:1;
	uint16_t EraseCommand					:1;
} sWIAC82_1553_StoreMonitorReportCriticalMonitor2;
/*
typedef struct
{
	uint16_t EraseCommand					:1;
	uint16_t RFjamCommand					:1;
	uint16_t RFemissionCommand				:1;
	uint16_t ControlSurfaceInhibit			:1;
	uint16_t AbortRelease					:1;
	uint16_t ActivateReleaseFunctions		:1;
	uint16_t GlobalDeletionReport 			:2;
	uint16_t CommandedStoreState			:8;
} sWIAC82_1553_StoreMonitorReportCriticalMonitor2;
*/
typedef struct
{
	uint16_t SubAddressOfError				:5;
	uint16_t Reserved						:6;
	uint16_t CriticalControl2Error			:1;
	uint16_t CriticalControl1Error			:1;
	uint16_t CheckSumError					:1;
	uint16_t IllegalHeaderSubAddress		:1;
	uint16_t NoCommandWordCount				:1;
} sWIAC82_1553_StoreMonitorReportProtocolStatus;
/*
typedef struct
{
	uint16_t NoCommandWordCount				:1;
	uint16_t IllegalHeaderSubAddress		:1;
	uint16_t CheckSumError					:1;
	uint16_t CriticalControl1Error			:1;
	uint16_t CriticalControl2Error			:1;
	uint16_t Reserved						:6;
	uint16_t SubAddressOfError				:5;
} sWIAC82_1553_StoreMonitorReportProtocolStatus;
*/

typedef struct
{

	uint16_t Header;										// 0x0420
	uint16_t InValidity1;
	uint16_t InValidity2;
	uint16_t CriticalMonitor1;
	uint16_t CriticalMonitor2;
	uint16_t Reserved1;
	uint16_t ProtocolStatus;
	uint16_t Reserved2[WIAC82_STORE_MONITOR_REPORT_RESERVED2_WORDS];
	uint16_t CRC;
} sWIAC82_1553_StoreMonitorReport_Message;;

typedef struct
{
	sWIAC82_MLAN_Header HeaderMLAN;               	// 0
	sWIAC82_1553_Header	Header1553;
	sWIAC82_1553_StoreMonitorReport_Message Stores;
} sWIAC82_1553_MLAN_StoreMonitorReport_Message;

// FUNCTIONAL EXCHANGE 3M (17T) *********************************************************
#define WIAC82_FUNCTIONAL_EXCHANGE_3M_HEADER      		1
#define WIAC82_FUNCTIONAL_EXCHANGE_3M_USEFUL_LENGTH  	16
#define WIAC82_FUNCTIONAL_EXCHANGE_3M_RESERVED_SIZE  	4
typedef struct
{
	uint16_t Reserved				:14;
	uint16_t ImpactSpeed			:1;
	uint16_t ImpactHeading			:1;
} sWIAC82_FunctionalExchange3MValidity;
/*
typedef struct
{
	uint16_t ImpactHeading			:1;
	uint16_t ImpactSpeed			:1;
	uint16_t Reserved				:14;
} sWIAC82_FunctionalExchange3MValidity;
*/
typedef struct
{
	uint16_t Identifier;						//1
	uint16_t UsefulLength;						//16
	uint16_t Validity;
	uint16_t ImpactHeading;
	uint16_t ImpactSpeed;
	uint16_t Reserved[WIAC82_FUNCTIONAL_EXCHANGE_3M_RESERVED_SIZE];
	uint16_t CRC;
} sWIAC82_1553_FunctionalExchange3M_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_FunctionalExchange3M_Message Funct3M;
} sWIAC82_1553_MLAN_FunctionalExchange3M_Message;

// FUNCTIONAL EXCHANGE 1M (23T) ***********************************************************
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_HEADER      		1
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_USEFUL_LENGTH  	58
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_RESERVED_SIZE  	14
/*typedef struct
{
	uint16_t Target				:1;
	uint16_t DriveAngle			:1;
	uint16_t SAL_Code 				:1;
	uint16_t ImageCRC				:1;
	uint16_t SeekerType			:1;
	uint16_t FuzeSetting			:1;
	uint16_t Reserved				:1;
	uint16_t TimeOfFlight			:1;
	uint16_t Reserved					:8;
} sWIAC82_FunctionalExchange1MValidity;
 */
typedef struct
{
	uint16_t Reserved2			:8;
	uint16_t TimeOfFlight		:1;
	uint16_t Reserved1			:1;
	uint16_t FuzeSetting		:1;
	uint16_t SeekerType			:1;
	uint16_t ImageCRC			:1;
	uint16_t SAL_Code 			:1;
	uint16_t DiveAngle			:1;
	uint16_t Target				:1;
} sWIAC82_FunctionalExchange1MValidity;

#define WIAC82_FUNCTIONAL_EXCHANGE_1M_IMAGE_CHECKSUM_KO					0
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_IMAGE_CHECKSUM_IN_PROCESS			1
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_IMAGE_CHECKSUM_OK					2
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_IMAGE_CHECKSUM_NO_IMAGE			3
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_SEEKER_TYPE_GNSS					0
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_SEEKER_TYPE_SAL					1
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_SEEKER_TYPE_IIR					2
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_IMPACT				0
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_DELAY				1
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_AIRBURST				2
#define WIAC82_FUNCTIONAL_EXCHANGE_1M_FUZE_SETTING_IN_PROGRAMMING		3

typedef struct
{
	uint16_t Reserved					:10;
	uint16_t FuzeSettings 				:2;
	uint16_t SeekerType					:2;
	uint16_t ImageChecksum				:2;
} sWIAC82_FunctionalExchange1MWeaponSettings;
/*
typedef struct
{
	uint16_t ImageChecksum				:2;
	uint16_t SeekerType					:2;
	uint16_t FuzeSettings 				:2;
	uint16_t Reserved					:10;
} sWIAC82_FunctionalExchange1MWeaponSettings;
*/
typedef struct
{
	uint16_t Identifier;						//1
	uint16_t UsefulLength;						//58
	uint16_t Validity;
	uint16_t TargetLatitude1;
	uint16_t TargetLatitude2;
	uint16_t TargetLongitude1;
	uint16_t TargetLongitude2;
	uint16_t TargetAltitude1;
	uint16_t TargetAltitude2;
	uint16_t FuzeDelay;
	uint16_t FuzeHeight;
	uint16_t GuidanceInformation;
	uint16_t SALCode;
	uint16_t WeaponSettings;
	uint16_t TimeOfFlight;
	uint16_t Reserved[WIAC82_FUNCTIONAL_EXCHANGE_1M_RESERVED_SIZE];
	uint16_t CRC;
} sWIAC82_1553_FunctionalExchange1M_Message;
typedef struct
{
	sWIAC82_MLAN_Header MLANHeader;               	// 0
	sWIAC82_1553_Header MIL1553Header;               	// 0
	sWIAC82_1553_FunctionalExchange1M_Message Funct1M;
} sWIAC82_1553_MLAN_FunctionalExchange1M_Message;

// RETURN DIALOGUE TEST (30T)
typedef struct
{
	uint16_t Word;
}sWIAC82_1553_ReturnDialogueTest_Message;
typedef struct
{
	sWIAC82_MLAN_Header Header;               	// 0
	uint8_t SubAddr;
	uint8_t RTAddr;
	uint8_t Spare[2];
	int32_t DiffTime;
	sWIAC82_1553_ReturnDialogueTest_Message Test;
} sWIAC82_1553_MLAN_ReturnDialogueTest_Message;

#endif /*WIAC82_COMMS_H_ */
