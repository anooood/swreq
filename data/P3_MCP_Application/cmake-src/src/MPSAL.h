/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MPSAL.h>
 *
 *  Description:
 *  ------------
 *  Header file for the MPSAL module.
 *
 ******************************************************************************/

#ifndef MPSAL_SEEKER_H_
#define MPSAL_SEEKER_H_

#pragma pack(1)

#define TELEMETRY_PARAMETER_DEFAULT		(unsigned short)(1)
#define MAX_MESSAGES_2mS 				((unsigned int)(4))
#define MAX_DATA_2mS 					((unsigned int)(550))

#define SEEKER_DETECTION_CBIT_RESULTS 	(unsigned short)((*(unsigned short *)(&SAL_SS.Detection.CBITResults)))
#define SEEKER_SBIT_RESULTS 			(unsigned int)((*(unsigned int *)(&SAL_SS.SBIT.PBITResults)))
#define MPSAL_RX_TIMEOUT_TIME 			400000  // 400 milliseconds

#define MPSAL_TO_RAD(X)   (X * 0.0000625f)        // 1/16 milliradians to radians (integer -> float32)
#define MPSAL_TO_MRAD(X)   (X * 0.0625f)        // 1/16 milliradians to mradians (integer -> float32)

#define NS_TO_US(X) (X/1000)
#define NS_TO_100NS(X) (X/100)
typedef struct
{
  unsigned int RxMessageCnt;
  unsigned int RxACKMessageCnt;
  unsigned int RxNAKMessageCnt;
  unsigned int RxSBITMessageCnt;
  unsigned int RxFPBITMessageCnt;
  unsigned int RxDetectionDataMessageCnt;
  unsigned int RxTelemetryDataMessageCnt;
  unsigned int RxTelemetrySmallImageMessageCnt;
  unsigned int RxTelemetryLargeImageMessageCnt;
  unsigned int RxTimeoutErrCnt;							// Number of Rx Data timeouts
  unsigned int RxMsgSizeErrCnt;
  unsigned int RxMsgIdErrCnt;
  unsigned int RxCrcErrCnt;
} sSAL_RxCounters;

typedef struct
{
  unsigned int TxMessageCnt;
  unsigned int TxSetParmsMessageCmdCnt;
  unsigned int TxSBITMessageCmdCnt;
  unsigned int TxPFBITMessageCmdCnt;
} sSAL_TxCounters;

typedef struct
{
	unsigned short SetParms_Complete;	// 0 - not complete 1-complete
	unsigned short SetParms_Sent;		// 0 - not sent 1-sent
	unsigned short SBIT_Complete; 		// 0 - not complete 1-complete
	unsigned short SBIT_Sent; 			// 0 - not sent 1-sent
	unsigned short DoPreFlightBIT;		// 0 - do not perform 1-perform
	unsigned short DoPreFlightBIT_Complete;
	unsigned short Stop_Telem_Complete; // 0 - not complete 1-complete
	unsigned short Stop_Telem_Sent; 	// 0 - not sent 1-sent
} sSAL_SM;

typedef struct
{
	unsigned int Cnt10ms;   			// Counts the number of timer scheduled 600Hz Slot3
	unsigned short PollCycle;			// 0 - perform poll cycle anything else = non-poll cycle
	unsigned int NonTxPollCycleCnt;		// Number of Tx Non Poll Cycle calls
	unsigned int TxPollCycleCnt;		// Number of Tx Poll Cycle calls
} sSAL_ServiceControl;

typedef struct
{
	TIME RxTimestamp;
	TIME RxAckTimestamp;
	TIME RxNackTimestamp;
	TIME RxSBITTimestamp;
	TIME RxPFBITTimestamp;
	TIME RxDetectionDataTimestamp;
	TIME RxTelemetryDataTimestamp;
	TIME RxTelemetrySmallImageTimestamp;
	TIME RxTelemetryLargeImageTimestamp;
} sSAL_TimeStamps;

typedef struct
{
	sMCP_SAL_GetPBITResultsCmd 			*GetPBITResultsCmd;
	sMCP_SAL_ExececutePreFlightBITCmd 	*ExececutePreFlightBITCmd;
	sMCP_SAL_GetPreFlightBITResultsCmd 	*GetPreFlightBITResultsCmd;
	sMCP_SAL_SetParametersCmd 			*SetParametersCmd;
	sMCP_SAL_StartTelemetryCmd 			*StartTelemetryCmd;
	sMCP_SAL_StopTelemetryCmd 			*StopTelemetryCmd;
}sSAL_TxMsgPointers;

typedef struct
{
	sSAL_MsgStart 					*SeekerMsgStart;
	sSAL_MCP_ACK					*ACK_Msg;
	sSAL_MCP_NACK					*NACK_Msg;
	sSAL_MCP_SBITResults 			*SBITRx;
	sSAL_MCP_PreFlightBITResults 	*PFBITRx;
	sSAL_MCP_DetectionData 			*DetectionData;
	sSAL_MCP_TelemetryData 			*Telemetry;
	sSAL_MCP_TelemetrySmallImage 	*SmallImage;
	sSAL_MCP_TelemetryLargeImage	*LargeImage;
}sSAL_RxMsgPointers;

//#define SAL_TX_BUFFER_LEN		100
//#define UART_DATA_BUFFER_SIZE         0x800

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
	unsigned int 					TxTestMsgCnt;
	sCharFIFO						RxFIFO;
	char 							TxBuffer[UART_DATA_BUFFER_SIZE];		// Max Transmit message payload size
	char 							RxBuffer[UART_DATA_BUFFER_SIZE];		// Max Receiver message payload size
}sSAL_CommsControls;

typedef struct
{
	double 							TargetYaw;
	double 							TargetPitch;
	double 							DetectionTime;
}sSAL_SharedData;


typedef struct
{
	unsigned short TelemetryControlParameter;	// 1= Send only "Telemetry Data" messages (default)
												// 10 = Send the "Telemetry Data" message followed by real image data (spread over 5 "Telemetry Image" messages – one “small” + four “large” messages)
												// 11 = Same messages as 10, but the image is a synthetic test pattern
	sSAL_Parameters Parameters;					// SAL Seeker setup parameters
	sSAL_SBITResults SBIT;						// Seeker PBIT Results
	sSAL_PFBITResults PFBIT;					// Seeker Pre-Flight BIT results
	sSAL_DetectionData Detection;				// Seeker Detection Data
	sSAL_SharedData GuidanceData;                 // si units data to be shared
	sSAL_TelemetryData TelemetryData;			// Seeker Telemetry Data
	sSAL_TelemetrySmallImage SmallImage;		// Seeker Telemetry Small Image
	sSAL_TelemetryLargeImage LargeImage;		// Seeker Telemetry Large Image
	sSAL_RxCounters RxCounters;					// Counters for message types received
	sSAL_TxCounters TxCounters;					// Counters for message types sent
	sSAL_SM StateMachine;						// State machine control
	sSAL_ServiceControl ServiceControl;			// Service Control variables
	sSAL_TimeStamps TimeStamps;					// Time stamps of received messages
	sSAL_TxMsgPointers TxPtr;					// Tx message pointers
	sSAL_RxMsgPointers RxPtr;					// Rx message pointers
	sSAL_CommsControls CommsControls;			// Communication Controls
	char AuxStr[AUX_FIFO_SIZE];		            //
} sSAL_StateSpace;

#pragma pack()

extern sSAL_StateSpace SAL_SS;

#define SAL_SKR_NO_ERROR				(unsigned int)(0)			// No error
#define SAL_SKR_WEAPON_CONFIG_ERR		(unsigned int)(0x1)			// Weapon Configuration does not includes SAL Seeker
#define SAL_SKR_NO_RX_DATA_ERR			(unsigned int)(0x2)			// No data received from SAL Seeker in 400 mSec

unsigned int MPSAL_SeekerInit(char *AuxStr, unsigned int FW_Version);
void MPSAL_RxService(unsigned char Cycle);
unsigned int MPSAL_SeekerSend(void);
unsigned short MPSAL_SeekerHandleRxData(sCharFIFO *RxFIFO, unsigned int *BytesProcessed);
void MPSAL_SeekerSetNewParms(double Frequency, unsigned short Accuracy, unsigned short Jitter);
unsigned short MPSAL_Pack(unsigned short OpCode, sSAL_StateSpace *SAL_SS);
void MPSAL_HandleACK_Msg(void);
void MPSAL_HandleNACK_Msg(void);
void MPSAL_HandleSBIT_Msg(void);
void MPSAL_HandlePreFlightBIT_Msg(void);
void MPSAL_HandleDetectionMsg(void);
void MPSAL_HandleTelemetryMsg(void);
void MPSAL_HandleSmallImageMsg(void);
void MPSAL_HandleLargeImageMsg(void);




#endif /* MPSAL_SEEKER_H_ */
