/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <QSAL.h>
 *
 *  Description:
 *  ------------
 *  Header file for the QSAL module.
 *
 ******************************************************************************/

#ifndef QSAL_SEEKER_H_
#define QSAL_SEEKER_H_

typedef unsigned char  Uint8;
typedef unsigned short Uint16;
typedef unsigned long  Uint32;

typedef signed char    Sint8;
typedef signed short   Sint16;
typedef signed long    Sint32;

// SAL Seeker message ID's
//sent msgs
#define QSAL_SEEKER_ID_SBIT_REQ_ID		0x00
#define QSAL_SEEKER_ID_CLEAR_CBIT       0x11
#define QSAL_SEEKER_ID_STANDBY_CMD     	0x22
#define QSAL_SEEKER_ID_ACTIVATION_CMD  	0x33

//received msgs
#define QSAL_SEEKER_ID_SBIT_REPORT_ID   0x8800
#define QSAL_SEEKER_ID_STATUS	        0x8811

#pragma pack(1)
// QSAL message structures taken from INTERFACE CONTROL DOCUMENT FOR THE QUADRANT SAL SEEKER (EXTERNAL - SOFTWARE) A0047-00000-521-002 Issue 4

typedef struct
{
	Uint16 SyncWord1;			// 0xBEEF
	Uint16 SyncWord2;			// 0xBABE
	Uint16 CRC;
	Uint16 MsgId;
	Uint16 PayloadLength;
	Uint16 MsgCount;
} sQSAL_MsgHeader;

//QSAL Host to Seeker Messages *************************
typedef struct
{
	sQSAL_MsgHeader Hdr;
} sMCP_QSAL_GetPBITResultsCmd;

typedef struct
{
	sQSAL_MsgHeader Hdr;
} sMCP_QSAL_ClearCBITCmd;

typedef struct
{
	sQSAL_MsgHeader Hdr;
} sMCP_QSAL_StandbyCmd;

typedef union
{
	Uint8			  byte;
	struct
	{
		Uint8         RangeValid       		: 1;  // 0		Set if (and only if) the estimated target range is valid
		Uint8         VelocityValid      	: 1;  // 1      Set if (and only if) the estimated target closing velocity is valid
		Uint8         AnglesValid        	: 1;  // 2      Set if (and only if) the estimated target LOS angles are valid
		Uint8         AnglesCoast      		: 1;  // 3      Set if (and only if) estimated target LOS angles were deduced from previous seeker measurements.
		Uint8         Reserved         		: 4;  // 4 - 7  Not used at present
	} Bits;
} sMCP_QSAL_GenSettingsVFlags;

typedef struct
{
	sQSAL_MsgHeader 				Hdr					;
	Uint32      					DesigPRI			;//Expected PRI of laser designator. Lsb = 1 ns.40 to 125 ms
	Uint16       					DesigAccuracy		;//Maximum difference between the designatorÃ¢â‚¬â„¢s actual PRI and DesPRI. Lsb = 1 ns.1 to 50 us
	Uint16      					DesigJitter         ;//Expected maximum jitter (radius, not peak-to-peak) on the designatorÃ¢â‚¬â„¢s PRI. Lsb = 1 ns.100 to 5 000 ns
	//Estimated Target States
	Uint16                  		TargetRange         ;//Estimated slant range to target. Lsb = 1m. Set to 65 535 if the range is more than 65 536 m.0 to 65 535m
	Uint16                  		ClosingVelocity     ;//Estimated closing velocity of weapon on target. Lsb = 1m/s. 0 to 65 535 m/s
	Sint16                  		EstTgtYaw           ;//Estimated yaw & pitch LOS angles to the target in the hostÃ¢â‚¬â„¢s reference axis. Lsb = 0.1 mrad. -Ã�â‚¬ to +Ã�â‚¬ rad
	Sint16                  		EstTgtPitch         ;//-Ã�â‚¬/2 to Ã�â‚¬/2 rad
	//Seeker To Host Offset
	Sint16 							SkrOffYaw           ;//The seeker attitude with respect to the hostÃ¢â‚¬â„¢s reference axis. -Ã�â‚¬ to +Ã�â‚¬ rad
	Sint16 							SkrOffPitch         ;//The seeker attitude with respect to the hostÃ¢â‚¬â„¢s reference axis. -Ã�â‚¬/2 to Ã�â‚¬/2 rad
 	Sint16 							SkrOffRoll          ;//The seeker attitude with respect to the hostÃ¢â‚¬â„¢s reference axis. -Ã�â‚¬ to +Ã�â‚¬ rad
	//General Settings
	sMCP_QSAL_GenSettingsVFlags 	ValidityFlags       ;//Information validity flags:
	Uint8       					TgtGateLead         ;//The lead time of the rising edge of the TARGET_GATE discrete output with respect to the expected arrival time of the target laser pulse.Lsb = 10 us. 0 to 2.55 ms
	Uint8       					TgtGateWidth        ;//The width of the TARGET_GATE discrete output pulse Lsb = 10 us. 0 to 2.55 ms
	Uint8       					Spare[7]            ;//Unused at present
} sMCP_QSAL_ActivationCmd;

//QSAL Seeker to Host Messages *************************
typedef union
{
	Uint32            DWord;
	struct
	{
		//Flash Data Integrity
	    Uint32        CalData         : 1;  //Bit 0: Calibration data error
	    Uint32        AppFirmware     : 1;  //Bit 1: Application firmware error
	    Uint32        AppSoftware     : 1;  //Bit 2: Application software error
	    Uint32        HardwareInfo    : 1;  //Bit 3: Hardware information error
	    //PSU Board Outputs
	    Uint32        PsuPcbTemp      : 1;  //Bit 4: PSU board temperature out of range or sensor error
	    Uint32        PsuLvPos        : 1;  //Bit 5: PSU positive low voltage out of range
	    Uint32        PsuLvNeg        : 1;  //Bit 6: PSU negative low voltage out of range
	    Uint32        DetSvNeg        : 1;  //Bit 7: Detector switch voltage out of range (This is the negative voltage as generated on the Detector PCB
	    Uint32        PsuHvNeg        : 1;  //Bit 8: PSU high voltage out of range
	    Uint32        PsuHeaterPos    : 1;  //Bit 9: PSU heater voltage out of range
	    Uint32        HvOvrCurr       : 1;  //Bit 10: Detector high voltage over current detected
	    //Processor Board Functions
	    Uint32        Humidity        : 1;  //Bit 11: Humidity within seeker too high or sensor error
	    Uint32        Pressure        : 1;  //Bit 12: Pressure within seeker out of range or sensor error
	    Uint32        ProcPcbTemp     : 1;  //Bit 13: Processor board temperature out of range or sensor error
	    Uint32        HeaterCtl       : 1;  //Bit 14: Heater control/switch error
	    Uint32        HighSpeedAdc    : 1;  //Bit 15: High speed ADC device error
	    Uint32        BitAdc          : 1;  //Bit 16: BIT ADC device(s) error
	    Uint32        DetCurrAdc      : 1;  //Bit 17: Detector currents ADC device error
	    Uint32        HighVoltCtl     : 1;  //Bit 18: High voltage control error
	    Uint32        ProcPcbVolt     : 1;  //Bit 19: Processor board internal voltages out of range
	    //Detector Board Functions
	    Uint32        DetElvPos       : 1;  //Bit 20: Detector extra low voltage out of range
	    Uint32        Heater          : 1;  //Bit 21: Detector heater error
	    Uint32        DetTemp         : 1;  //Bit 22: Detector temperature out of range or sensor error
	    Uint32        DetPcbTemp      : 1;  //Bit 23: Detector board temperature out of range or sensor error
	    Uint32        QuadALed        : 1;  //Bit 24: Quadrant A did not respond correctly to test LED
	    Uint32        QuadBLed        : 1;  //Bit 25: Quadrant B did not respond correctly to test LED
		Uint32        QuadCLed        : 1;  //Bit 26: Quadrant C did not respond correctly to test LED
		Uint32        QuadDLed        : 1;  //Bit 27: Quadrant D did not respond correctly to test LED
		Uint32        GRingLed        : 1;  //Bit 28: Guard Ring did not respond correctly to test LED
		Uint32        DetNoise        : 1;  //Bit 29: Detector noise out of range
		Uint32        GainSwitch      : 1;  //Bit 30: Gain stage switching error
		Uint32        Spare           : 1;  //Bit 31: Not used at present
	} Bits;
} sMCP_QSAL_SBITStatus;


typedef struct
{
	sQSAL_MsgHeader			Hdr				;
//	Software And Firmware Information																												   Range
	Uint16      			AppSWVersion    ;//Version of Application Software. Lsb = 0.01.                                                            0.00 to 655.35
	Uint16      			AppSWCrc        ;//CRC of Application Software.                                                                            0 to 0xFFFF
	Uint16      			AppFWVersion    ;//Version of Application Firmware. Lsb = 0.01.                                                            0.00 to 655.35
	Uint16      			AppFWCrc        ;//CRC of Application Firmware.                                                                            0 to 0xFFFF
	Uint16      			CalDataCRC      ;//CRC of Calibration Data.                                                                                0 to 0xFFFF
	Uint16      			CalDataVersion	;//Calibration Data version, as indicated within the calibration data (and not the Meta Data). Lsb = 0.01. 0.00 to 655.35
//	Seeker Hardware Information
	Sint8                   PartNumber	[12];//Seeker part number as ASCII string (excluding issue) (can be, but not necessarily null terminated)
	Sint8                   SerialNumber[10];//Seeker serial number as ASCII string (can be, but not necessarily null terminated)
	Sint8                   Issue		 [2];//Seeker issue as ASCII string (eg. Ã¢â‚¬Å“01Ã¢â‚¬ï¿½ or Ã¢â‚¬Å“13Ã¢â‚¬ï¿½) (can be, but not necessarily null terminated)
//	Startup BIT Results Summary
	sMCP_QSAL_SBITStatus	SBITStatus 		;//SBIT status word. Each bit within this status word indicates whether a certain SBIT test has passed (0) or failed (1), unless stated otherwise. Refer to note 1.
//	PSU Board Outputs
	Sint8    				PsuPcbTemp      ;//PSU board temperature. Lsb = 1 Ã‚Â°C.                                 -128 to 127 Ã‚Â°C
    Uint8       			PsuLvPos        ;//PSU positive low voltage level.         Lsb = 0.03 V               0 to 7.7 V
    Uint8       			PsuLvNeg        ;//PSU negative low voltage level.        Lsb = -0.03 V               0 to -7.7 V
    Uint8       			DetSvNeg        ;//Detector negative switching voltage. Lsb = -40/256 V               0 to -40 V
    Uint8       			PsuHvNegOn      ;//PSU negative high voltage level, when activated.     Lsb  = -1 V   0 to -255 V
    Uint8       			PsuHvNegOff     ;//PSU negative high voltage level, when de-activated.  Lsb  = -1 V   0 to -255 V
    Uint8       			PsuHeaterPos    ;//PSU heater voltage level.                  Lsb = 40/256 V          0 to 40 V
//  Processor Board Measurements
    Uint8                   SkrHumidity     ;//Seeker humidity. Lsb = 1 %.                                                           0 to 100%
    Uint8                   SkrPressure     ;//Seeker pressure. Lsb = 1 kPa.                                                         0 to 255 kPa
    Sint8                   ProcPcbTemp     ;//Processor board temperature. Lsb = 1 Ã‚Â°C                                               -128 to 127 Ã‚Â°C
    Uint8                   ProcHeaterVOn   ;//Heater power supply level on detector PCB when heaters switched on. Lsb = 40/256 V    0 to 40 V
    Uint8                   ProcHeaterVOff  ;//Heater power supply level on detector PCB when heaters switched off. Lsb = 40/256 V   0 to 40 V
    Uint8                   Proc3V3Pos      ;//Processor Digital 3.3 V Level. Lsb = 0.02 V                                           0 to 5.1V
	Uint8                   Proc1V8Pos      ;//Processor Digital 1.8 V Level. Lsb = 0.01 V                                           0 to 2.55 V
	Uint8                   Proc1V5Pos      ;//Processor Digital 1.5 V Level. Lsb = 0.01 V                                           0 to 2.55 V
	Uint8                   Proc1V0Pos      ;//Processor Digital 1.0 V Level. Lsb = 0.01 V                                           0 to 2.55 V
//	Detector Board Measurements
	Uint8       			DetElvPos       ;//Detector very low voltage level.    Lsb = 0.02 V                                            0 to 5.1V
	Uint8       			HeaterVoltDrop  ;//The drop in PSU Heater Voltage when the heater is switched on.  Lsb = 0.1 V.                0 to 25 V
	Sint8        			DetTemp         ;//Detector temperature.            Lsb = 1 Ã‚Â°C                                                 -128 to 127 Ã‚Â°C
	Sint8        			DetPcbTemp      ;//Detector board temperature.  Lsb = 1 Ã‚Â°C                                                     -128 to 127 Ã‚Â°C
	Sint32                  QuadACorr       ;//Detector quadrant A, B, C, D and guard ring peak absolute correlation values with Test LED. -5e7 to +10e7
	Sint32                  QuadBCorr       ;//See note 3.                                                                                 -5e7 to +10e7
	Sint32                  QuadCCorr       ;//                                                                                            -5e7 to +10e7
	Sint32                  QuadDCorr       ;//                                                                                            -5e7 to +10e7
	Sint32                  GuardRCorr      ;//                                                                                            -5e7 to +10e7
	Uint8                   QuadANoise0     ;//                                                                                            0.0 to 25.5
	Uint8                   QuadBNoise0     ;//Detector quadrant A, B, C and D average noise RMS in gain stage 0.                          0.0 to 25.5
	Uint8                   QuadCNoise0     ;//Lsb = 0.1 ADC bit.                                                                          0.0 to 25.5
	Uint8                   QuadDNoise0     ;//                                                                                            0.0 to 25.5
	Uint8                   QuadANoise1     ;//                                                                                            0.0 to 25.5
	Uint8                   QuadBNoise1     ;//                                                                                            0.0 to 25.5
	Uint8                   QuadCNoise1     ;//Detector quadrant A, B, C and D average noise RMS in gain stage 1.                          0.0 to 25.5
	Uint8                   QuadDNoise1     ;//Lsb = 0.1 ADC bit.                                                                          0.0 to 25.5
}sMCP_QSAL_SBITReportMessage;


typedef union
{
	Uint16			   Word;
	struct
	{
		Uint16         SeekerMode       : 1;//Bit 0:       Current seeker mode. 0 = Standby 1 = Active
		Uint16         Tracking         : 1;//Bit 1:       Set (1) if the seeker has locked onto a target. Also referred to as Tracking sub-mode.
		Uint16         TargetPulse      : 1;//Bit 2:       Set (1) if a target pulse has been detected (irrespective of whether it has resulted in a valid angle measurement or not)
		Uint16         AnglesValid      : 1;//Bit 3:       Set (1) if the measured target LOS angles as provided in this message are valid (irrespective of whether it is in the accurate or inaccurate region).
		Uint16         AnglesAccurate   : 1;//Bit 4:       Set (1) if the measured target LOS angles as provided in this message are within the accurate measurement region.
		Uint16         GainStage        : 4;//Bits 5-8:    Current gain stage of seeker.
		Uint16         TempGood         : 1;//Bit 9:       Set (1) if the detectorÃ¢â‚¬â„¢s temperature is adequate for operation, to achieve full seeker performance.
		Uint16         Heaters          : 1;//Bit 10:      Set (1) if the detector heaters are on.
		Uint16         Spare			: 5;//Bits 11-15   Not used at present
	}Bits;
}sMCP_QSAL_ModesAndStates;

typedef union
{
	Uint32			   DWord;
	struct
	{
//		Spare Bits
		Uint32         Spare0           : 1;//Bit 0:Not used at present.
		Uint32         Spare1           : 1;//Bit 1:Not used at present.
		Uint32         Spare2           : 1;//Bit 2:Not used at present.
		Uint32         Spare3           : 1;//Bit 3:Not used at present.
//		PSU Board Outputs
		Uint32         PsuPcbTemp       : 1;//Bit 4: PSU board temperature out of range or sensor error. See Note 4.
		Uint32         PsuLvPos         : 1;//Bit 5: PSU positive low voltage out of range.
		Uint32         PsuLvNeg         : 1;//Bit 6: PSU negative low voltage out of range.
		Uint32         DetSvNeg         : 1;//Bit 7: Detector switch voltage out of range                                    (as generated on the Detector PCB).
		Uint32         PsuHvNeg         : 1;//Bit 8: PSU high voltage out of range .     See Note 5.
		Uint32         PsuHeaterPos     : 1;//Bit 9: PSU heater voltage out of range.
		Uint32         HvOverCurr       : 1;//Bit 10:Detector high voltage over current. See Note 4.
//		Processor Board Functions
        Uint32         Humidity         : 1;//Bit 11:Relative humidity within seeker too high or sensor error.
        Uint32         Pressure         : 1;//Bit 12:Pressure within seeker out of range or sensor error.
        Uint32         ProcPcbTemp      : 1;//Bit 13:Processor board temperature out of range or sensor error.
        Uint32         HeaterCtl        : 1;//Bit 14:Heater control / switch error.
        Uint32         HighSpeedAdc     : 1;//Bit 15:High speed ADC (for detector data) device error.
        Uint32         BitAdc           : 1;//Bit 16:BIT ADC device(s) error.
        Uint32         DetCurrAdc       : 1;//Bit 17:Detector currents ADC device error. See Note 4.
        Uint32         HighVoltCtl      : 1;//Bit 18:High voltage control error.
        Uint32         ProcPcbVolt      : 1;//Bit 19:Processor board internal voltages out of range.
//      Detector Board Functions
        Uint32         DetElvPos        : 1;//Bit 20:Detector extra low voltage out of range.
        Uint32         Heater           : 1;//Bit 21:Detector heater error.
        Uint32         DetTemp          : 1;//Bit 22:Detector temperature out of range or sensor error.
        Uint32         DetPcbTemp       : 1;//Bit 23:Detector board temperature out of range or sensor error.
        Uint32         QuadASaturate    : 1;//Bit 24:Target pulse saturation on quadrant A. See Note 4.
        Uint32         QuadBSaturate    : 1;//Bit 25:Target pulse saturation on quadrant B. See Note 4.
        Uint32         QuadCSaturate    : 1;//Bit 26:Target pulse saturation on quadrant C. See Note 4.
        Uint32         QuadDSaturate    : 1;//Bit 27:Target pulse saturation on quadrant D. See Note 4.
        Uint32         TooManyPulses    : 1;//Bit 28:Too many pulses to process in PRI.      See Note 4.
        Uint32         Spare4           : 1;//Bit 29:Not used at present
        Uint32         Spare5           : 1;//Bit 30:Not used at present
        Uint32         GoNoGo           : 1;//Bit 31:GoNoGo   (0 = Go     1 = NoGo). See Note 3.
	}Bits;
}sMCP_QSAL_CBITStatus;

typedef struct
{
	sQSAL_MsgHeader					Hdr				;
//	Primary Operational Outputs
	sMCP_QSAL_ModesAndStates		ModesAndStates	;//Primary seeker modes and states:
	Sint16                          TargetYaw       ;//Measured target yaw LOS angle in hostÃ¢â‚¬â„¢s reference axis (See note 1). Lsb = 0.1 mrad.                          -Ã�â‚¬ to +Ã�â‚¬ rad
	Sint16                          TargetPitch     ;//Measured target pitch LOS angle in hostÃ¢â‚¬â„¢s reference axis (See note 1). Lsb = 0.1 mrad.                        -Ã�â‚¬/2 to +Ã�â‚¬/2 rad
	Uint8                           YawAccuracy     ;//Estimated accuracy of measured yaw and pitch angles respectively. See note 6. Lsb = 0.2 mrad.                 0 to 255
	Uint8                           PitchAccuracy   ;//Estimated accuracy of measured yaw and pitch angles respectively. See note 6. Lsb = 0.2 mrad.                 0 to 255
	Uint16                          Latency         ;//The latency in the measured angles as provided by this message.                                               0 to 3 000 us
													 //This is the time from the arrival of the laser pulse at the seeker,
	                                                 //to the time that the first bit of this message is transmitted. The accuracy of this latency is within 50 us.
													 //Lsb = 1us.If the AnglesValid flag is not set in this message, this field is not applicable.
//	Additional Information
	Uint8                           Humidity        ;//Relative humidity within seeker.	Lsb = 1%.                 0 to 100%
	Uint8                           Pressure        ;//Absolute pressure within seeker.    Lsb = 1 kPa.           0 to 255 kPa
	Sint8                           PSUTemp         ;//Power supply PCB temperature. 	Lsb = 1 Ã‚ÂºC.               -128 to 127 Ã‚ÂºC (See note 7)
	Sint8                           ProcessorTemp   ;//Processor PCB temperature. 	Lsb = 1 Ã‚ÂºC.                   -128 to 127 Ã‚ÂºC (See note 7)
	Sint8                           DetectorPcbTemp ;//Detector PCB temperature. 	Lsb = 1 Ã‚ÂºC.                   -128 to 127 Ã‚ÂºC (See note 7)
	Sint8                           DetectorTemp    ;//Detector temperature. 	Lsb = 1 Ã‚ÂºC.                       -128 to 127 Ã‚ÂºC (See note 7)
	Uint32                          SeekerPRI       ;//PRI currently used within seeker. Lsb = 1ns.  See note 2.  39.9 to 125.1 ms
//	Continuous BIT Results Summary
	sMCP_QSAL_CBITStatus			CBITStatus		;//Continuous BIT results. Each bit indicates the result of a specific CBIT condition: 0 = OK, 1 = Fail.(Also refer to note 3)
}sMCP_QSAL_StatusMessage;

typedef struct
{
	unsigned int RxSBITReportMsgCnt;						//Seeker SBIT Report Message
	unsigned int RxStatusMessageQSalMsgCnt;		//Seeker Status Message count from power up reported by qsal
	unsigned int RxStatusMessageRxByMcpCnt;				   //Seeker Status Message count received by mcp from power up
	unsigned int RxStatusMessageMissCnt;					//number of missed status msgs. Aim for 0.
	unsigned int RxTimeoutErrCnt;							//Number of Rx Data timeouts
	unsigned int RxCrcErrCnt;
	unsigned int RxMsgSizeErrCnt;
	unsigned int RxMsgIdErrCnt;
} sQSAL_RxCounters;

typedef struct
{
	unsigned int TxSBITRequestCnt;
	unsigned int TxStandbyCmdCnt;
	unsigned int TxClearCBITCnt;
	unsigned int TxActiveCmd;
} sQSAL_TxCounters;

typedef struct
{

	//flags used to tell send fcn what to cmd send
	unsigned char SendSBITReqCmd; 			// 0 - dont send 1-send. Sent every cycle until sbit report received
	unsigned char SendStandbyCmd;	 		// 0 - dont send 1-send. Sent every cycle
	unsigned char SendActiveCmd;	 		// 0 - dont send 1-send. Sent every cycle
	unsigned char SendClearCBITCmd;	        // set to one by client/requester, set to zero after sent once

	//receiver state vars
	unsigned char RxState;
	unsigned char RxStateSync1Val[2];
	unsigned char RxStateSync2Val[2];
	int RxStateSync1Idx;
	int RxStateSync2Idx;
	int RxStateRxBufferIdx;
} sQSAL_SM;


typedef struct
{
	unsigned int 	Cnt10ms;   			// Counts the number of timer scheduled 600Hz Slot3
//	unsigned short 	PollCycle;			// 0 - perform poll cycle anything else = non-poll cycle
//	unsigned int	NonTxPollCycleCnt;	// Number of Tx Non Poll Cycle calls
//	unsigned int	TxPollCycleCnt;		// Number of Tx Poll Cycle calls
} sQSAL_ServiceControl;

typedef struct
{
//	TIME RxTimestamp;
//	TIME RxAckTimestamp;
//	TIME RxNackTimestamp;
	TIME RxSBITTimestamp;
//	TIME RxPFBITTimestamp;
	TIME RxStatusTimestamp;
//	TIME RxTelemetryDataTimestamp;
//	TIME RxTelemetrySmallImageTimestamp;
//	TIME RxTelemetryLargeImageTimestamp;
} sQSAL_TimeStamps;

typedef struct
{
	sMCP_QSAL_GetPBITResultsCmd 			SBITReqCmd;
	sMCP_QSAL_ClearCBITCmd      			ClearCBITCmd;
	sMCP_QSAL_StandbyCmd                    StandbyCmd;
	sMCP_QSAL_ActivationCmd					ActivationCmd;
}sQSAL_TxMsgs;

typedef struct
{
	sQSAL_MsgHeader  				*SeekerMsgStart;
	sMCP_QSAL_SBITReportMessage		*SBITRx;
	sMCP_QSAL_StatusMessage 		*DetectionData;
}sQSAL_RxMsgPointers;

#define QSAL_UART_DATA_BUFFER_SIZE        	0x800

typedef struct
{
	unsigned char 					PacketMode;
	unsigned char 					RxInteruptEnable;
	unsigned int 					TxMsgEnabled;
	unsigned short 					Length;			// Length of bytes to be transmitted including Header 1 and Header 2
	unsigned short 					TxMsgOpCode;
	unsigned short					MessageCRC;
	unsigned short 					TxMsgHeader1;
	unsigned short 					TxMsgHeader2;
	unsigned short 					BytesRead;
	unsigned short 					BytesProcessed;
	sCharFIFO						RxFIFO;
	char 							TxBuffer[QSAL_UART_DATA_BUFFER_SIZE];		// Max Transmit message payload size
	char 							RxBuffer[QSAL_UART_DATA_BUFFER_SIZE];		// Max Receiver message payload size
}sQSAL_CommsControls;

typedef struct
{
  unsigned char Data[200];
} sQSAL_SeekerTel;

typedef struct
{
	unsigned long long 				RxTimestamp;
	double 							TargetYaw;
	double 							TargetPitch;
	double 							DetectionTime;
	unsigned int					TxCnt;
	unsigned int					RxCnt;
	unsigned int					ErrCnt;
	unsigned int					EstPRI;
	unsigned int					RangeCmd;
	sMCP_QSAL_CBITStatus			CBIT;
	sMCP_QSAL_SBITStatus			SBIT;
	sMCP_QSAL_ModesAndStates		Status;
	unsigned short 					TargetSNR;
	signed char 					ProcessorTemperature;
	signed char 					DetectorTemperature;
	unsigned short 					JitterCmd;        // [ns]
	unsigned short 					AccuracyCmd;      // [ns]
	unsigned short 					FW_CRC;
	unsigned short 					SW_CRC;
	unsigned short 					CalDataCRC;
	unsigned short 					AppFWVersion;
	unsigned short 					AppSWVersion;
	unsigned short 					Spare1;
	unsigned int 					Spare2;
	sQSAL_SeekerTel 				Telemetry[6];
}sQSAL_SharedData;


typedef struct
{
	sQSAL_SharedData 			SharedData;		// Seeker received Data - to be shared with other modules
	sQSAL_RxCounters 			RxCounters;		// Counters for message types received
	sQSAL_TxCounters 			TxCounters;		// Counters for message types sent
	sQSAL_SM 					StateMachine;	// State machine control
//	sQSAL_ServiceControl 		ServiceControl;	// Service Control variables
	sQSAL_TimeStamps 			TimeStamps;		// Time stamps of received messages
	sQSAL_TxMsgs				TxMsgs;			// Tx message
//	sQSAL_RxMsgPointers 		RxPtr;			// Rx message pointers
	sQSAL_CommsControls 		CommsControls;	// Communication Controls
	char 							AuxStr[QSAL_UART_DATA_BUFFER_SIZE];		//
} sQSAL_StateSpace;

#pragma pack()

extern sQSAL_StateSpace QSAL_SS;

#define SAL_SKR_NO_ERROR				(unsigned int)(0)			// No error
#define SAL_SKR_WEAPON_CONFIG_ERR		(unsigned int)(0x1)			// Weapon Configuration does not includes SAL Seeker
#define SAL_SKR_NO_RX_DATA_ERR			(unsigned int)(0x2)			// No data received from SAL Seeker in 400 mSec

unsigned int	QSAL_SeekerInit	(char *AuxStr, unsigned int FW_Version);
void 			QSAL_RxService(unsigned char Cycle);
unsigned int 	QSAL_SeekerSend(void);
void 			QSAL_SeekerHandleRxData(void);
void 			QSAL_SetStandbyMode(void);
void 			QSAL_SetActiveMode(void);
void 			QSAL_SeekerSetNewParms(double Frequency, unsigned short Accuracy, unsigned short Jitter);
unsigned char 	QSAL_GetFrequency(float *Frequency, unsigned int Code);
void 			QSAL_HandleSBIT_Msg(void);
void 			QSAL_HandleDetectionMsg(void);
void 			QSAL_SeekerSetPRI(double Frequency);
void 			QSAL_SeekerSetAccuracy(unsigned short Accuracy);
void            QSAL_SeekerSetJitter(unsigned short Jitter);




#endif /* QSAL_SEEKER_H_ */
