/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MPSAL_Comms.h>
 *
 *  Description:
 *  ------------
 *  Header file for the MPSAL_Comms module.
 *
 ******************************************************************************/

#ifndef SAL_SEEKER_COMMS_H_
#define SAL_SEEKER_COMMS_H_

#pragma pack(1)

// MPSAL Host to Seeker message data lengths excluding Header 1 and Header 2
#define SAL_SEEKER_LENGTH_SBIT_REQ_ID             		(unsigned short)(6)
#define SAL_SEEKER_LENGTH_EXECUTE_PRE_FLIGHT_BIT  		(unsigned short)(6)
#define SAL_SEEKER_LENGTH_GET_PRE_FLIGHT_BIT_RESULTS	(unsigned short)(6)
#define SAL_SEEKER_LENGTH_SET_PARAMETERS				(unsigned short)(22)
#define SAL_SEEKER_LENGTH_START_TELEMETRY				(unsigned short)(8)
#define SAL_SEEKER_LENGTH_STOP_TELEMETRY				(unsigned short)(6)

#define SAL_SEEKER_HEADER_LENGTH						(unsigned short)(4)  // byte size of header 1 and header 2

// Seeker parameters
#define SAL_PULSED_DESIGNATOR			(unsigned short)(1)
#define SAL_CW_DESIGNATOR				(unsigned short)(2)
#define SAL_DEFAULT_DESIGNATOR_CODE		(unsigned int)(50000) 		// 100mSec
#define SAL_DEFAULT_LASER_PRF_ACCURACY	(unsigned short)(15) 	    // 15uSec
#define SAL_DEFAULT_LASER_JITTER		(unsigned short)(15) 	    // 1.5uSec
#define SAL_DEFAULT_SEEKER_YAW			(short)(1000) 				// (1000/16) millirad
#define SAL_DEFAULT_SEEKER_PITCH		(short)(2500) 				// (2500/16) millirad
#define SAL_DEFAULT_SEEKER_ROLL			(short)(5000) 				// (5000/16) millirad
#define FREQ_CODE_SF         			(double)(0.000001)

// MPSAL Message Headers
#define MPSAL_MESSAGE_HEADER_1             			(unsigned short)(0x1234)
#define MPSAL_MESSAGE_HEADER_2             			(unsigned short)(0xABCD)

// MPSAL Host to Seeker Message IDs
#define SAL_SEEKER_ID_SBIT_REQ_ID             		(unsigned short)(0x01) //Payload = 0
#define SAL_SEEKER_ID_EXECUTE_PRE_FLIGHT_BIT  		(unsigned short)(0x02) //Payload = 0
#define SAL_SEEKER_ID_GET_PRE_FLIGHT_BIT_RESULTS	(unsigned short)(0x03) //Payload = 0
#define SAL_SEEKER_ID_SET_PARAMETERS				(unsigned short)(0x04) //Payload = 16
#define SAL_SEEKER_ID_START_TELEMETRY				(unsigned short)(0x06) //Payload = 2
#define SAL_SEEKER_ID_STOP_TELEMETRY				(unsigned short)(0x07) //Payload = 0

// MPSAL Seeker to Host Message IDs
#define SAL_SEEKER_ID_ACK             				(unsigned short)(0x8000) //Payload = 0
#define SAL_SEEKER_ID_NACK             				(unsigned short)(0xC000) //Payload = 0
#define SAL_SEEKER_ID_SBIT                    		(unsigned short)(0x8001) //Payload = 4
#define SAL_SEEKER_ID_PRE_FLIGHT_BIT_RESULTS  		(unsigned short)(0x8003) //Payload = 50
#define SAL_SEEKER_ID_DATA                    		(unsigned short)(0x0011) //Payload = 22
#define SAL_SEEKER_ID_TEL                     		(unsigned short)(0x0012) //Payload = 190
#define SAL_SEEKER_ID_TEL_SMALL_IMAGE          		(unsigned short)(0x0013) //Payload = 604
#define SAL_SEEKER_ID_TEL_LARGE_IMAGE          		(unsigned short)(0x0014) //Payload = 838

//Seeker telemetry messages
#define SAL_SEEKER_TEL_SMALL_IMAGE_SIZE          	 600
#define SAL_SEEKER_TEL_LARGE_IMAGE_SIZE          	 834

// MPSAL BIT Status
typedef struct
{
	unsigned short SensorCommunication              	: 1;  	// 0 	0 = Pass, 1 = Fail
	unsigned short SensorVideo              		  	: 1;  	// 1	0 = Pass, 1 = Fail
	unsigned short SensorNoise		              		: 1;  	// 2	0 = Pass, 1 = Fail
	unsigned short SensorTemperature                	: 1;  	// 3	0 = Pass, 1 = Fail
	unsigned short FPGATemperature              	  	: 1;  	// 4	0 = Pass, 1 = Fail
	unsigned short DRAM				              		: 1;  	// 5	0 = Pass, 1 = Fail
	unsigned short FLASH				              	: 1;  	// 6	0 = Pass, 1 = Fail
	unsigned short Spare				              	: 9; 	// 7-15
} bMPSAL_BITResults;

// MPSAL message structures
typedef struct
{
  unsigned short MsgHeader1;			// 0x1234
  unsigned short MsgHeader2;			// 0xABCD
  unsigned short DataLength;		// Number of bytes in the message, excluding Header 1 and Header 2
  unsigned short Opcode;			// MessageID - message type identifier
} sSAL_MsgStart;

//MPSAL Host to Seeker Messages *************************
typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sMCP_SAL_GetPBITResultsCmd;

typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sMCP_SAL_ExececutePreFlightBITCmd;

typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sMCP_SAL_GetPreFlightBITResultsCmd;

typedef struct
{
	unsigned short DesignatorType; 	// 1 = Pulsed 2 = CW [for future optional use
	unsigned int DesignatorCode;  	// For Pulsed: pulse interval in microSec. Range is 50000-125000
	unsigned short PRFaccuracy;  	// Units of microSec. Range 1-50
	unsigned short LaserJitter;		// Units of 0.1 microSec. Range 1-50
	short SeekerYaw;  				// Seeker yaw angle XS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian.
	  	  	  	  	  	  	  	  	// Range -32768 to 32767. Positive is counterclockwise around ZS
	short SeekerPitch;  			// Seeker pitch angle YS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian.
	   	  	  	  	  	  	  	  	// Range -32768 to 32767. Positive is counterclockwise around YS
	short SeekerRoll;  				// Seeker roll angle ZS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian.
} sSAL_Parameters;


typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_Parameters Parameters;
  unsigned short Checksum;
} sMCP_SAL_SetParametersCmd;

typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short TelemetryContrtolParameter;	// 1 = Send only "Telemetry Data" messages (default)
  	  	  	  	  	  	  	  	  				// 10 = Send the "Telemetry Data" message followed by real image data (spread over 5 "Telemetry Image" messages
  	  	  	  	  	  	  	  	  				//      – one “small” + four “large” messages)
  unsigned short Checksum;
} sMCP_SAL_StartTelemetryCmd;

typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sMCP_SAL_StopTelemetryCmd;

// MPSAL Seeker to Host Messages ****************************
typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sSAL_MCP_ACK;

typedef struct
{
  sSAL_MsgStart MsgStart;
  unsigned short Checksum;
} sSAL_MCP_NACK;

typedef struct
{
	unsigned short PBITStatus; 	// 1-16 0 = Pass 1 = Busy (in first 0.5 Sec after FPGA loading) 2 = Fail
	bMPSAL_BITResults PBITResults;
} sSAL_SBITResults;

typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_SBITResults SBIT;
  unsigned short Checksum;
} sSAL_MCP_SBITResults;

typedef struct
{
  unsigned short PreFlightBITStatus; 		// 1-16 0 = Pass 1 = Busy (in first 0.5 Sec after FPGA loading) 2 = Fail
  bMPSAL_BITResults PFBITResults;
  unsigned short SensorCommsTestResults;
  unsigned short SensorVideoTestResults;
  unsigned short SensorNoiseTestResults;
  unsigned short SensorTempValue;
  unsigned short FPGATempValue;
  unsigned short DRAMTestResults;
  unsigned short FLASHTestResults;
  unsigned short Spare1;					// Always 0
  unsigned short Spare2;					// Always 0
  unsigned short Spare3;					// Always 0
  unsigned short SerialNo;
  unsigned short FirmwareVersionNo;
  unsigned short FirmwareReleaseDay;
  unsigned short FirmwareReleaseMonth;
  unsigned short FirmwareReleaseYear;
  unsigned short DesignatorType;
  unsigned int DesignatorCode;
  unsigned short PRFAccuracy;				// Units of microSec. Range 1-50
  unsigned short LaserHitter;				// Units of 0.1 microSec. Range 1-50.
  short Yaw; 								// Seeker yaw angle XS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian
  short Pitch;								// Seeker pitch angle YS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian
  short Roll;								// Seeker roll angle ZS, relative to the weapon’s seeker axis, units of 1/16th of a milliradian
} sSAL_PFBITResults;

typedef struct
{
    sSAL_MsgStart MsgStart;
    sSAL_PFBITResults PFBIT;
    unsigned short Checksum;
} sSAL_MCP_PreFlightBITResults;

typedef struct
{
	unsigned short FrameCount;
	unsigned short CBITStatus;				// 0 = Pass 1 = Fail
	bMPSAL_BITResults CBITResults;
	short SpotAzimuth;
	short SpotElevation;
	unsigned short DetectionStatus;			// 0 = No Valid Detection 1 = Valid Detection (both code & position)
	unsigned short SpotEnergy;				// Spot total energy, arbitrary units
	unsigned short SpotSize;				// Spot size in pixels
	unsigned int UsedDesignatorCode;		// For Pulsed: pulse interval in MicroSec. For CW: laser code word. Actual value used
	unsigned short TimeStamp;				// Time elapsed since the rising edge of the SAL_SYNC pulse (which is the time tag of the current frame), in MicroSec. Typically ~5msec. Always less than 6msec.
} sSAL_DetectionData;

typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_DetectionData Detection;
  unsigned short Checksum;
} sSAL_MCP_DetectionData;

typedef struct
{
	unsigned short SpotX;					// Spot coordinates relative to image top left corner [1/8] of a pixel
	unsigned short SpotY;					// Spot coordinates relative to image top left corner [1/8] of a pixel
	unsigned short SpotSize;				// Size in pixels
	unsigned short SpotEnergy;				// Spot total energy, arbitrary units
} sSAL_TelemetrySpot;

typedef struct
{
	unsigned short FrameCount;				// Same value as in the Detection Data message
	unsigned short TelemetryCount;			// Always 0 for this message
	unsigned char Reserved1[58];
	sSAL_TelemetrySpot Spot1;
	unsigned char Reserved2[24];
	sSAL_TelemetrySpot Spot2;
	unsigned char Reserved3[24];
	sSAL_TelemetrySpot Spot3;
	unsigned char Reserved4[24];
	sSAL_TelemetrySpot Spot4;
	unsigned char Reserved5[24];
} sSAL_TelemetryData;

typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_TelemetryData TelemetryData;
  unsigned short Checksum;
} sSAL_MCP_TelemetryData;

typedef struct
{
  unsigned short FrameCount;				// Same value as in the Detection Data message
  unsigned short TelemetryCount;			// Always 1 for this message
  unsigned char SmallImageData[SAL_SEEKER_TEL_SMALL_IMAGE_SIZE];
} sSAL_TelemetrySmallImage;

typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_TelemetrySmallImage SmallImage;
  unsigned short Checksum;
} sSAL_MCP_TelemetrySmallImage;

typedef struct
{
  unsigned short FrameCount;				// Same value as in the Detection Data message
  unsigned short TelemetryCount;			// Always 1 for this message
  unsigned char LargeImageData[SAL_SEEKER_TEL_LARGE_IMAGE_SIZE];
} sSAL_TelemetryLargeImage;

typedef struct
{
  sSAL_MsgStart MsgStart;
  sSAL_TelemetryLargeImage LargeImage;
  unsigned short Checksum;
} sSAL_MCP_TelemetryLargeImage;

#pragma pack()

unsigned short           // checksum
utCrc16Skr
(
	char * ptr,  // pointer to start of block
    unsigned int size  // size of block in bytes
);

#endif /* SAL_SEEKER_COMMS_H_ */


