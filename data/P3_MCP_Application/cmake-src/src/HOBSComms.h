/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <HOBSComms.h>
 *
 *  Description:
 *  ------------
 *  Header file for the HOBSComms module.
 *
 ******************************************************************************/

#ifndef HOBS_COMMS_H_
#define HOBS_COMMS_H_

#define HOBS_MSG_SYNC_BYTE	    (uint8_t)(0x55)
#define HOBS_MSG_HEADER_SIZE	6

#define HOBS_ARR_WAVESAMPLE 	16
#define HOBS_ARR_FLASH      	256

#define HOBS_MAJOR_CONST     	4
#define HOBS_SET_MODE       	0xf

/* HOBS Subsystem Addresses */
#define INTERFACE_TESTER	(uint8_t)(0x00)
#define SOFT_PROCESSOR		(uint8_t)(0x01)
#define FLASH_LOADER		(uint8_t)(0x02)
#define SIGNAL_PROCESSOR	(uint8_t)(0x03)
#define DATA_INJECTOR		(uint8_t)(0x04)
#define GENERAL_NACK		(uint8_t)(0xFF)

/* MessageIDs */
#define MODE_CHANGE_CMD					(uint8_t)(0x81)
#define MODE_CHANGE_RESP				(uint8_t)(0x82)
#define MISSION_CONFIG_CMD				(uint8_t)(0x83)
#define MISSION_CONFIG_RESP				(uint8_t)(0x84)
#define GEN_GND_PROX_WARNING_CMD		(uint8_t)(0x85)
#define GEN_GND_PROX_WARNING_RESP		(uint8_t)(0x86)
#define DSP_FILTER_CONTROL_CMD			(uint8_t)(0x87)
#define DSP_FILTER_CONTROL_RESP			(uint8_t)(0x88)
#define REGISTER_WRITE_CMD				(uint8_t)(0x89)
#define REGISTER_WRITE_RESP				(uint8_t)(0x8A)
#define CBIT_CLEAR_CMD					(uint8_t)(0x8B)
#define CBIT_CLEAR_RESP					(uint8_t)(0x8C)
#define TRAMSMITTER_CONTROL_CMD			(uint8_t)(0x8D)
#define TRAMSMITTER_CONTROL_RESP		(uint8_t)(0x8E)
#define LOAD_GPW_WAVEFORM_CMD			(uint8_t)(0x8F)
#define LOAD_GPW_WAVEFORM_RESP			(uint8_t)(0x90)
#define FLASH_WRITE_CMD					(uint8_t)(0x81)
#define FLASH_WRITE_RESP				(uint8_t)(0x82)
#define WRITE_LOG_DATA_CMD				(uint8_t)(0x83)
#define WRITE_LOG_DATA_RESP				(uint8_t)(0x84)
#define FW_UPLOAD_START_CMD				(uint8_t)(0x85)
#define FW_UPLOAD_START_RESP			(uint8_t)(0x86)
#define FW_UPLOAD_END_CMD				(uint8_t)(0x87)
#define FW_UPLOAD_END_RESP				(uint8_t)(0x88)
#define SYSTEM_STATUS_CMD				(uint8_t)(0x01)
#define SYSTEM_STATUS_RESP				(uint8_t)(0x02)
#define FW_VERSION_CMD					(uint8_t)(0x03)
#define FW_VERSION_RESP					(uint8_t)(0x04)
#define REGISTER_READ_CMD				(uint8_t)(0x05)
#define REGISTER_READ_RESP				(uint8_t)(0x06)
#define LOGISTIC_DATA_READ_CMD			(uint8_t)(0x03)
#define LOGISTIC_DATA_READ_RESP	     	(uint8_t)(0x04)
#define PLOT_AND_TRACK_CMD				(uint8_t)(0x01)
#define PLOT_AND_TRACK_RESP				(uint8_t)(0x02)
#define NACK_RESP						(uint8_t)(0x00)

/* Message Payload Lengths */
#define MODE_CHANGE_CMD_PAYLOAD_LENGTH				(uint8_t)(1)
#define MODE_CHANGE_RESP_PAYLOAD_LENGTH				(uint8_t)(1)
#define MISSION_CFG_CMD_PAYLOAD_LENGTH			    (uint8_t)(6)
#define MISSION_CFG_RESP_PAYLOAD_LENGTH  			(uint8_t)(1)
#define GND_PROXWAR_CMD_PAYLOAD_LENGTH		        (uint8_t)(1)
#define GND_PROXWAR_RESP_PAYLOAD_LENGTH	            (uint8_t)(1)
#define DSPFILT_CT_CMD_PAYLOAD_LENGTH		        (uint8_t)(1)
#define DSPFILT_CT_RESP_PAYLOAD_LENGTH		        (uint8_t)(1)
#define REG_WRITE_CMD_PAYLOAD_LENGTH			    (uint8_t)(8)
#define REG_WRITE_RESP_PAYLOAD_LENGTH		     	(uint8_t)(1)
#define CBIT_CLEAR_CMD_PAYLOAD_LENGTH				(uint8_t)(1)
#define CBIT_CLEAR_RESP_PAYLOAD_LENGTH				(uint8_t)(1)
#define TRAMS_CT_CMD_PAYLOAD_LENGTH		            (uint8_t)(1)
#define TRAMS_CT_RESP_PAYLOAD_LENGTH		        (uint8_t)(1)
#define LOADGPW_WAV_CMD_PAYLOAD_LENGTH		        (uint8_t)(0x20)
#define LOADGPW_WAV_RESP_PAYLOAD_LENGTH		        (uint8_t)(1)
#define FLASH_WRITE_CMD_PAYLOAD_LENGTH				(uint8_t)(0)  		/* 256 bytes */
#define FLASH_WRITE_RESP_PAYLOAD_LENGTH				(uint8_t)(1)
#define WRITE_LOG_CMD_PAYLOAD_LENGTH			    (uint8_t)(0x1A)
#define WRITE_LOG_RESP_PAYLOAD_LENGTH			    (uint8_t)(1)
#define FW_UPLOAD_S_CMD_PAYLOAD_LENGTH			    (uint8_t)(1)
#define FW_UPLOAD_S_RESP_PAYLOAD_LENGTH			    (uint8_t)(1)
#define FW_UPLOAD_E_CMD_PAYLOAD_LENGTH			    (uint8_t)(1)
#define FW_UPLOAD_E_RESP_PAYLOAD_LENGTH			    (uint8_t)(4)
#define SYSTEM_STAT_CMD_PAYLOAD_LENGTH			    (uint8_t)(1)
#define SYSTEM_STAT_RESP_PAYLOAD_LENGTH			    (uint8_t)(0x1E)
#define FW_VERSION_CMD_PAYLOAD_LENGTH				(uint8_t)(1)
#define FW_VERSION_RESP_PAYLOAD_LENGTH				(uint8_t)(5)
#define REG_READ_CMD_PAYLOAD_LENGTH			        (uint8_t)(4)
#define REG_READ_RESP_PAYLOAD_LENGTH			    (uint8_t)(4)
#define LOGDAT_READ_CMD_PAYLOAD_LENGTH              (uint8_t)(1)
#define LOGDAT_READ_RESP_PAYLOAD_LENGTH	    	    (uint8_t)(0x1A)
#define PLOTTRACK_CMD_PAYLOAD_LENGTH			    (uint8_t)(1)
#define PLOTTRACK_RESP_PAYLOAD_LENGTH			    (uint8_t)(0x11)
#define NACK_RESP_PAYLOAD_LENGTH					(uint8_t)(1)

typedef struct
{
	uint8_t SyncByte;
	uint8_t SubSystemAddress;
	uint8_t MessageID;
	uint8_t FragmentOffset;
	uint8_t FragmentOffset2;
	uint8_t PayloadLength;
} sHOBS_Msg_Header;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Mode;
	uint8_t Checksum;
} sMCP_HOBS_MODE_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_MODE_RESP;

#define WARN_SIGNAL_WAVEFOR_1				(uint8_t)(0x01)
#define WARN_SIGNAL_WAVEFOR_2				(uint8_t)(0x02)
#define WARN_SIGNAL_USER_DEF				(uint8_t)(0x03)

typedef struct
{
	uint8_t BurstHeight;  		/* i. Height in meters ii. Valid Range 1 to 25. iii. Default = 25 */
	uint8_t EstImpactAngle;		/* i. Estimated impact angle in degrees. ii. Valid Range 25 to 90. iii. Default: 60. */
	uint16_t EstWeaponSpeed;		/* i. Estimated speed(ground approach) from weapon in m/s ii. Valid Range 50 to 450. iii. Default: 250.Note : The speed is represented as a velocity vector aligned with the direction of gravity, rather than the direction of travel. */
	uint8_t WarnSignalWaveform;
	uint8_t ColocationID;
} sHOBS_MISSION_CONFIG;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_MISSION_CONFIG Config;
	uint8_t Checksum;
} sMCP_HOBS_MISSION_CONFIG_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_MISSION_CONFIG_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_GROUND_PROX_WARN_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_GROUND_PROX_WARN_RESP;

typedef struct {
	uint8_t MTI              	: 1;
	uint8_t RFI              	: 1;
	uint8_t SlopeDetector    	: 1;
	uint8_t Spare    			: 5;
} bHOBS_DSP_Filters;

typedef struct
{
	sHOBS_Msg_Header Header;
	bHOBS_DSP_Filters DSPFilterSetting;
	uint8_t Checksum;
} sMCP_HOBS_DSPFILTER_CTL_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_DSPFILTER_CTL_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint32_t Address;
	uint32_t Data;
	uint8_t Checksum;
} sMCP_HOBS_REG_WRITE_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_REG_WRITE_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_CBIT_CLEAR_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_CBIT_CLEAR_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t TxEnable;
	uint8_t Checksum;
} sMCP_HOBS_TRANSM_CTL_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_TRANSM_CTL_RESP;

typedef struct
{
	uint8_t LogicLevel;
	uint8_t Duration;			/* micro seconds */
} aHOBS_GPW_WAVEFORM_SAMPLE;

typedef struct
{
	sHOBS_Msg_Header Header;
	aHOBS_GPW_WAVEFORM_SAMPLE WaveformSample[HOBS_ARR_WAVESAMPLE];
	uint8_t Checksum;
} sMCP_HOBS_GPW_WAVE_LOAD_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_GPW_WAVE_LOAD_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Payload[HOBS_ARR_FLASH];
	uint8_t Checksum;
} sMCP_HOBS_FLASH_WRITE_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_FLASH_WRITE_RESP;

#define HOBS_PART_NUMBER_LENGTH 	(uint8_t)(22)
#define HOBS_SERIAL_NUMBER_LENGTH 	(uint8_t)(4)

typedef struct
{
	uint8_t PartNumber[HOBS_PART_NUMBER_LENGTH];
	uint8_t SerialNumber[HOBS_SERIAL_NUMBER_LENGTH];
} sHOBS_LOG_DATA;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_LOG_DATA LogData;
	uint8_t Checksum;
} sMCP_HOBS_LOG_DATA_WRITE_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_LOG_DATA_WRITE_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_FW_UPLOOAD_START_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sHOBS_MCP_FW_UPLOOAD_START_RESP;
typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_FW_UPLOOAD_END_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint32_t FirmwareCRC;
	uint8_t Checksum;
} sHOBS_MCP_FW_UPLOOAD_END_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_SYSTEM_STATUS_CMD;

typedef union
{
  uint8_t DWord;
  struct
  {
    uint8_t External_Comm      : 1;  /* 0 */
    uint8_t Local_Bim_Comm     : 1;  /* 1 */
    uint8_t TBD1               : 1;  /* 2 */
    uint8_t TBD2               : 1;  /* 3 */
    uint8_t TBD3               : 1;  /* 4 */
    uint8_t TBD4               : 1;  /* 5 */
    uint8_t TBD5               : 1;  /* 6 */
    uint8_t TBD6               : 1;  /* 7 */
  } Bits;
} uHOBS_SBIT;

typedef union
{
  uint8_t DWord;
  struct
  {
    uint8_t GPW_Signal_event   : 1;  /* 0 */
    uint8_t Local_Bim_Comm     : 1;  /* 1 */
    uint8_t Ramp_Lock          : 1;  /* 2 */
    uint8_t PSU_Status         : 1;  /* 3 */
    uint8_t Output_RF          : 1;  /* 4 */
    uint8_t TBD1               : 1;  /* 5 */
    uint8_t TBD2               : 1;  /* 6 */
    uint8_t TBD3               : 1;  /* 7 */
  } Bits;
} uHOBS_CBIT;

typedef union
{
  uint8_t DWord;
  struct
  {
    uint8_t Transmitter_Status : 1;  /* 0 */
    uint8_t Filter_Status_MTI  : 1;  /* 1 (0=Disabled/1=Enabled) */
    uint8_t Filter_Status_RFI  : 1;  /* 2 (0=Disabled/1=Enabled) */
    uint8_t Filter_Status_Slope: 1;  /* 3 (0=Disabled/1=Enabled) */
    uint8_t TBD1               : 1;  /* 4 */
    uint8_t TBD2               : 1;  /* 5 */
    uint8_t TBD3               : 1;  /* 6 */
    uint8_t TBD4               : 1;  /* 7 */
  } Bits;
} uHOBS_TestSettings;

typedef struct
{
	uint8_t Mode; 						/* 0x00 = Dormant 0x01 = Active 0x02 = Function-Test 0x03 = Loader */
	uHOBS_SBIT SBIT;							/* Bit 0 = External Communication Bit 1 = Local BIM Communication Bit 2 = TBD Bit 3 = TBD Bit 4 = TBD Bit 5 = TBD Bit 6 = TBD Bit 7 = TBD 0 = Okay,1 = Error */
	uHOBS_CBIT CBIT;							/* Bit 0 = GPW signal event latch i. 0 = Event has not occurred ii. 1 = Event as occurred Bit 1 = Local BIM Communication Bit 2 = Ramp Lock Bit 3 = PSU Status Bit 4 = Output RF Power Level Status Bit 5 = Firmware CRC Busy Bit 6 = TBD Bit 7 = TBD Bit 1 to 7 0 = Okay,1 = Error */
	uHOBS_TestSettings TestSettings;			/* Bit 0 =Transmitter Status i. 1 = On ii. 0 = Off Bit 1 to 3 = DSP Filter Status Bit 1 = MTI Bit 2 = RFI Bit 3 = Slope Detector 0 = Disabled,1 = Enabled Bit 4 to 7 =TBD */
	uint8_t BurstHeight;					/* Burst height setting in meters */
	uint8_t EstImpactAngle;				/* Estimated impact angle setting in degrees */
	uint16_t WeaponSpeed;					/* Estimated weapon’s ground approach speed */
	uint8_t WarningSignalWaveform;		/* Warning Signal Waveform setting */
	uint8_t CoLocationID;					/* Co-Location ID settings */
	int16_t PSU_1V8;								/* Conversion = PSU 1V8 * 305.18 microV (V) */
	int16_t PSU_4V1;								/* Conversion = PSU 4V1* 610.36 microV (V) */
	int16_t PSU_5V;								/* Conversion = PSU 5V * 610.36 microV (V) */
	int16_t PSU_5V5;								/* Conversion = PSU 5V5 * 610.36 microV (V) */
	int16_t PSU_10V;								/* Conversion = PSU 10V * 1.83108 milliV (V) */
	int16_t PSU_10V5;								/* Conversion = PSU 10V5 * 1.83108 milliV (V) */
	int16_t PSU_InputVoltage;						/* Conversion = Input Voltage * 3.66216 milliV (V) */
	int16_t OutputDetectorLevel;					/* Conversion = Detector level * 305.18 microV (V) */
	int16_t PSU_3V3;								/* Conversion = PSU 3V3 * 305.18 microV + 2.5 (V) */
	int16_t PSU_PCB__Temp;						/* Conversion = Temperature * 0.0625 (degree C) */
} sHOBS_SYSTEM_STATUS;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_SYSTEM_STATUS SystemStatus;
	uint8_t Checksum;
} sHOBS_MCP_SYSTEM_STATUS_RESP;

#pragma pack(1)

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_FW_VERSION_CMD;

typedef struct
{
	uint8_t Version;					/* Bit 0 to 3 = Minor Version Bit 4 to 7 = Major Version */
	uint32_t CRC;						/* CRC-32 For more information, refer to the Ethernet standard IEEE 802.3-2018, section 1, clause 3.2.9 */
} sHOBS_FW_VERSION;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_FW_VERSION FirmwareVersion;
	uint8_t Checksum;
} sHOBS_MCP_FW_VERSION_RESP;

#pragma pack()

typedef struct
{
	sHOBS_Msg_Header Header;
	uint32_t RegisterAddress;
	uint8_t Checksum;
} sMCP_HOBS_REG_READ_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint32_t RegisterData;
	uint8_t Checksum;
} sHOBS_MCP_REG_READ_RESP;
typedef struct
{
	sHOBS_Msg_Header Header;
	uint16_t PageAddress;
	uint8_t Checksum;
} sMCP_HOBS_FLASH_READ_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Data[HOBS_ARR_FLASH];
	uint8_t Checksum;
} sHOBS_MCP_FLASH_READ_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_LOG_DATA_READ_CMD;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_LOG_DATA RRS_Data;
	uint8_t Checksum;
} sHOBS_MCP_LOG_DATA_READ_RESP;

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t Spare;
	uint8_t Checksum;
} sMCP_HOBS_PLOT_AND_TRACK_CMD;

typedef struct
{
	uint32_t  TimeStamp;
	uHOBS_CBIT CBIT;							/* Bit 0 = GPW signal event latch i. 0 = Event has not occurred ii. 1 = Event as occurred Bit 1 = Local BIM Communication Bit 2 = Ramp Lock Bit 3 = PSU Status Bit 4 = Output RF Power Level Status Bit 5 = Firmware CRC Busy Bit 6 = TBD Bit 7 = TBD Bit 1 to 7 0 = Okay,1 = Error */
	uHOBS_TestSettings TestSettings;			/* Bit 0 =Transmitter Status i. 1 = On ii. 0 = Off Bit 1 to 3 = DSP Filter Status Bit 1 = MTI Bit 2 = RFI Bit 3 = Slope Detector 0 = Disabled,1 = Enabled Bit 4 to 7 =TBD */
	uint16_t DetectorHeight;				/* Current Detection height Conversion = Detector Height *154.6875e-3 (m) */
	int32_t TrackHeight;							/* Conversion = Track Height / 5242883 (m) */
	int32_t TrackSpeed;								/* Track Speed /524288 (m/s) */
	uint8_t GPW_HeightSetting;			/* Range : 1 to 25 in meters */
} sHOBS_PLOT_AND_TRACK;

typedef struct
{
	sHOBS_Msg_Header Header;
	sHOBS_PLOT_AND_TRACK Track;
	uint8_t Checksum;
} sHOBS_MCP_PLOT_AND_TRACK_RESP;

#define HOBS_COMMS_PARITY_ERROR				(uint8_t)(0x01)
#define HOBS_COMMS_CHECKSUM_ERROR			(uint8_t)(0x02)
#define HOBS_COMMS_INVALID_HEADER_ERROR		(uint8_t)(0x03)

typedef struct
{
	sHOBS_Msg_Header Header;
	uint8_t NAK_Response;					/* 0x01 = Parity Bit Error. 0x02 = Checksum Error. 0x03 = Invalid header field */
	uint8_t Checksum;
} sHOBS_MCP_NACK_RESP;


extern void HOBS_SetMode(uint8_t newMode);
extern void HOBS_XConfigurationSet(const sHOBS_MISSION_CONFIG *newConfiguration, uint8_t newBurstHeight);
extern void HOBS_UPlot_Track_Request(void);
extern void HOBSStatus_Request(void);
extern void HOBS_HandleRxData(uint8_t *RxData);
extern void HOBSLogistic_Data_Request(void);
extern void HOBSFW_Version_Request(void);

void HOBS_ChangeModeRespHandler(uint8_t *RxData);
void HOBS_MissionConfigRespHandler(uint8_t *RxData);
void HOBS_SystemStatusRespHandler(uint8_t *RxData);
void HOBS_FWVersionRespHandler(uint8_t *RxData);
void HOBS_GeneralNACKRespHandler(uint8_t *RxData);
void HOBS_PlotTrackRespHandler(uint8_t *RxData);
void HOBS_LogisticDataRespHandler(uint8_t *RxData);

#endif /*HOBS_COMMS_H_ */
