/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Arcnet.h>
 *
 *  Description:
 *  ------------
 *  Header file for ARCNET Protocol.
 *
 ******************************************************************************/

#ifndef ARCNET_H
#define ARCNET_H


#include "TypeDefines.h"


#define ARCNET_MAX_DATA_LENGTH	  508
#define ARCNET_TX_QUEUE_SIZE	  8
#define ARCNET_RX_QUEUE_SIZE	  8
#define ARCNET_BROADCAST_ID		  0


typedef enum
{
  arcSUCCESS,
  arcTX_ERR,
  arcDATA_SIZE_ERR,
  arcMUTEX_LOCKED,
  arcQUEUE_OVERFLOW
} eArcnetErrors;


typedef struct
{
  unsigned char FromNodeId;
  unsigned char ToNodeId;
  unsigned short DataLength;
  unsigned int Timestamp;
  unsigned char Data[ARCNET_MAX_DATA_LENGTH];
} sArcnetQueueEntry;

typedef struct
{
  volatile unsigned int *Base;
  unsigned char OwnNodeId;
  unsigned char ReceiveBroatcasts;
  unsigned char RxPage;
  unsigned char RxQueueCnt;
  unsigned char RxQueueIndex;          // Index within RX queue where next unread message is located
  unsigned char TxQueueCnt;
  unsigned char TxQueueIndex;          // Index within TX queue where new received message will be copied
  unsigned char RxMutexLock;
  unsigned char TxMutexLock;
  unsigned char InterruptMask;
  unsigned char Spare1;
  unsigned char BuildNodeMapState;
  unsigned short NodesOnNetwork;
  unsigned short Spare2;
  unsigned int RxCnt;
  unsigned int TxCnt;
  unsigned int RxErrCnt;
  unsigned int TxErrCnt;
  unsigned int MyReconCnt;
  unsigned int ReconCnt;
  sArcnetQueueEntry RxQueue[ARCNET_RX_QUEUE_SIZE];
  sArcnetQueueEntry TxQueue[ARCNET_TX_QUEUE_SIZE];
} sArcnet;


void ArcnetInit(sArcnet *Arcnet, unsigned int BaseAddr, unsigned char HardwareInterrupt, unsigned char NodeId, unsigned char ReceiveAll, unsigned char ReceiveBroatcasts);
void ArcnetDisable(sArcnet *Arcnet);
unsigned short ArcnetRead(sArcnet *Arcnet, unsigned int *Timestamp, unsigned char *FromNodeId, unsigned char *RxData);
eArcnetErrors ArcnetWrite(sArcnet *Arcnet, unsigned char ToNodeId, unsigned char *Data, unsigned short DataLength);
void ArcnetBuildMap(sArcnet *Arcnet);

#endif
