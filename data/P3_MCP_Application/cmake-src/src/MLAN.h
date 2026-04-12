/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MLAN.h>
 *
 *  Description:
 *  ------------
 *  Header file for the MLAN module.
 *
 ******************************************************************************/

#ifndef MLAN_H
#define MLAN_H


#define MLAN_MCP_NODE_ID          255
#define MLAN_DATA_LOGGER_NODE_ID  254
#define MLAN_INTERFACE_NODE_ID    253
#define MLAN_PITOT_NODE_ID        252
#define MLAN_WIFI_NODE_ID         251
#define MLAN_ETE_NODE_ID          250
#define MLAN_ITU_NODE_ID          249
#define MLAN_SAL_SEEKER_NODE_ID   248
#define MLAN_HILS_PC_NODE_ID      247
#define MLAN_DTAC_NODE_ID         246
#define MLAN_IIR_SEEKER_NODE_ID   245
#define MLAN_SERVO_NODE_ID        244
#define MLAN_TEL_NODE_ID          243
#define MLAN_HOBS_NODE_ID         242
#define MLAN_SPARE2_NODE_ID       241
#define MLAN_LOADER_PC_NODE_ID    240

#define MLAN_HEADER_SIZE          12


typedef struct
{
  unsigned char SourceId;
  unsigned char DestinationId;
  unsigned short Opcode;
  unsigned short DataLength;
  unsigned char SequenceNo;
  unsigned char SequenceCnt;
  unsigned int FrameCnt;
} sMLAN_Header;

typedef struct
{
  unsigned int ReconCnt;
  unsigned int MyReconCnt;
  unsigned short NodesOnNetwork;
  unsigned short Spare;
} sMLAN;


extern sMLAN MLAN;

typedef struct
{
	unsigned int TxCount;
	unsigned int TxErrCount;
	unsigned int RxCount;
	unsigned int RxErrCount;
	unsigned int WiFiRxCount;
	unsigned int ETE_RxCount;
	unsigned int HILS_RxCount;
	unsigned int DTAC_RxCount;
	unsigned int ServoRxCount;
	unsigned int TelRxCount;
	unsigned int ITURxCount;
}sMLAN_Counters;

typedef struct
{
	sMLAN_Counters			Counters;
} sMLAN_StateSpace;

extern sMLAN_StateSpace MLAN_SS;

void MLAN_Init(char *AuxStr);
void MLAN_Service(void);
void MLAN_Send(unsigned char ToNodeId, unsigned char *TxData, unsigned int Length);
unsigned char MLAN_IsNodeOnNet(unsigned char NodeID);
void MLAN_Disable(void);

#endif
