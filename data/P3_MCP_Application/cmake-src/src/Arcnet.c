/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Arcnet.c>
 *
 *  Description:
 *  ------------
 *  Arcnet.c implements the communication interface for the ARCNET protocol.
 *
 *  Function(s):
 *  ------------
 *  - ArcnetInit:                  Initializes the Arcnet core
 *  - ArcnetDisable:               Disable the Arcnet transmitter
 *  - ArcnetSendTxMsg:             Transmits a Tx packet
 *  - ArcnetSendTxQueueMsg:        Transmits the next Tx packet in the Tx queue
 *  - ArcnetWrite:                 Inserts a new Tx packet into the Tx queue
 *  - ArcnetGetRxMsg:              Gets a new Rx packet from Arcnet core
 *  - ArcnetAddToRxQueue:          Inserts a new Rx packet into the Rx queue
 *  - ArcnetRead:                  Gets the next received packet from Rx queue
 *  - ArcnetInterruptHandler:      Handles all Arcnet generated interrupts
 *  - ArcnetBuildMap:              Builds the ARCNET node bitmap
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include "TypeDefines.h"
#include "Interrupts.h"
#include "Timer.h"
#include "Arcnet.h"

// Refer to COM20020 data sheet

// Read registers
#define ARC_RD_REG_STATUS               0x00
#define ARC_RD_REG_DIAG                 0x01
#define ARC_RD_REG_ADDR_HI              0x02
#define ARC_RD_REG_ADDR_LO              0x03
#define ARC_RD_REG_DATA                 0x04
#define ARC_RD_REG_SUB_ADDR             0x05
#define ARC_RD_REG_CONFIG               0x06
#define ARC_RD_REG_TENT_ID              0x07
#define ARC_RD_REG_NODE_ID              0x07
#define ARC_RD_REG_SETUP1               0x07
#define ARC_RD_REG_NEXT_ID              0x07
#define ARC_RD_REG_SETUP2               0x07

// Write registers
#define ARC_WR_REG_INTERRUPT            0x00
#define ARC_WR_REG_COMMAND              0x01
#define ARC_WR_REG_ADDR_HI              0x02
#define ARC_WR_REG_ADDR_LO              0x03
#define ARC_WR_REG_DATA                 0x04
#define ARC_WR_REG_SUB_ADDR             0x05
#define ARC_WR_REG_CONFIG               0x06
#define ARC_WR_REG_TENT_ID              0x07
#define ARC_WR_REG_NODE_ID              0x07
#define ARC_WR_REG_SETUP1               0x07
#define ARC_WR_REG_TEST                 0x07
#define ARC_WR_REG_SETUP2               0x07

// Read sub registers
#define ARC_RD_SUB_REG_TENT_ID          0x00
#define ARC_RD_SUB_REG_NODE_ID          0x01
#define ARC_RD_SUB_REG_SETUP1           0x02
#define ARC_RD_SUB_REG_NEXT_ID          0x03
#define ARC_RD_SUB_REG_SETUP2		        0x04

// Write sub registers
#define ARC_WR_SUB_REG_TENT_ID          0x00
#define ARC_WR_SUB_REG_NODE_ID          0x01
#define ARC_WR_SUB_REG_SETUP1           0x02
#define ARC_WR_SUB_REG_TEST             0x03
#define ARC_WR_SUB_REG_SETUP2           0x04

// Commands
#define ARC_CMD_CLEAR_TX_INT            0x00
#define ARC_CMD_CLEAR_RX_INT         	  0x08
#define ARC_CMD_DISABLE_TX              0x01
#define ARC_CMD_DISABLE_RX              0x02
#define ARC_CMD_DISABLE_LONG_PACKETS    0x05
#define ARC_CMD_ENABLE_LONG_PACKETS	    0x0D
#define ARC_CMD_RESTART         	      0x18
#define ARC_CMD_CLEAR_EXNACK_FLAG       0x0E 
#define ARC_CMD_CLEAR_RECON_FLAG        0x16 
#define ARC_CMD_CLEAR_ALL_FLAGS         0x1E 


typedef enum {
  abmIDLE,
  abmRECON,
  abmTEST_NODE
} eArcnetBuildMapState;


// Status register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char TA                  : 1;  // Transmitter available
    unsigned char TMA                 : 1;  // Transmitter message acknowledged
    unsigned char Recon               : 1;
    unsigned char Test                : 1;
    unsigned char POR                 : 1;  // Power on reset
    unsigned char NotUsed             : 2;
    unsigned char RI                  : 1;  // Receiver inhibited
  };
} uArcnetStatus;

// Diagnostic register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char NotUsed             : 1;
    unsigned char NewNextId           : 1;
    unsigned char TentativeId         : 1;
    unsigned char ExcessiveNACK       : 1;
    unsigned char TokenSeen           : 1;
    unsigned char ReceiveAvtivity     : 1;
    unsigned char DuplicateId         : 1;
    unsigned char MyRecon             : 1;
  };
} uArcnetDiagnostic;

// Config register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char SubAddr0            : 1;
    unsigned char SubAddr1            : 1;
    unsigned char Backplane           : 1;
    unsigned char ET2                 : 1;
    unsigned char ET1                 : 1;
    unsigned char TxEnable            : 1;
    unsigned char CmdChaining         : 1;
    unsigned char Reset               : 1;
  };
} uArcnetConfig;

// Setup 1 register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char SlowARB             : 1;
    unsigned char ClockPrescaler      : 3;
    unsigned char ReceiveAll          : 1;
    unsigned char Reserved            : 1;
    unsigned char FourNACKS           : 1;
    unsigned char Pulse1Mode          : 1;
  };
} uArcnetSetup1;

// Setup 2 register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char ReconTimer          : 2;  // RCNTM0 and RCNTM1
    unsigned char NoSync              : 1;
    unsigned char EnhancedFunctions   : 1;
    unsigned char ClockMultiplier     : 2;
    unsigned char Reserved            : 1;
    unsigned char ReadBusTimingSelect : 1;
  };
} uArcnetSetup2;

// Interrupt register
typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char TA                  : 1;
    unsigned char NewNextId           : 1;
    unsigned char Recon               : 1;
    unsigned char ExcessiveNACK       : 1;
    unsigned char NotUsed             : 3;
    unsigned char RI                  : 1;
  };
} uArcnetInterrupt;


static TIME ArcnetBuildMapTimeout;

static void ArcnetInterruptHandler(unsigned int Arcnet);

void ArcnetInit(sArcnet *Arcnet, unsigned int BaseAddr, unsigned char HardwareInterrupt, unsigned char NodeId, unsigned char ReceiveAll, unsigned char ReceiveBroatcasts)
{
  uArcnetConfig config_reg;
  uArcnetSetup1 setup1_reg;
  uArcnetSetup2 setup2_reg;
  uArcnetInterrupt interrupt_reg;


  Arcnet->Base = (volatile unsigned int *) BaseAddr;
  Arcnet->OwnNodeId = NodeId;
  Arcnet->ReceiveBroatcasts = ReceiveBroatcasts & 0x01;
  Arcnet->RxPage = 1;
  Arcnet->RxQueueCnt = 0;
  Arcnet->RxQueueIndex = 0;
  Arcnet->TxQueueCnt = 0;
  Arcnet->TxQueueIndex = 0;
  Arcnet->RxMutexLock = FALSE;
  Arcnet->TxMutexLock = FALSE;
  Arcnet->RxCnt = 0;
  Arcnet->TxCnt = 0;
  Arcnet->RxErrCnt = 0;
  Arcnet->TxErrCnt = 0;
  Arcnet->MyReconCnt = 0;
  Arcnet->ReconCnt = 0;
  Arcnet->InterruptMask = 0;

  setup2_reg.Byte = 0;
  setup2_reg.ReconTimer = 3;
  setup2_reg.NoSync = 1;
  setup2_reg.ClockMultiplier = 3;
  setup2_reg.EnhancedFunctions = 1;
  Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_SETUP2;
  Arcnet->Base[ARC_WR_REG_SETUP2] = setup2_reg.Byte;

  // Restart Arcnet
  Arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_RESTART;

  // Disable all Arcnet interrupts
  Arcnet->Base[ARC_WR_REG_INTERRUPT] = 0;

  // Reset Arcnet
  config_reg.Byte = 0;
  config_reg.Reset = 1;
  Arcnet->Base[ARC_WR_REG_CONFIG] = config_reg.Byte;

  // Put the core to sleep by setting the node ID to 0
  Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_NODE_ID;
  Arcnet->Base[ARC_WR_REG_NODE_ID] = 0;

  // Set the extended timeout
  config_reg.Reset = 0;
  config_reg.ET1 = 1;
  config_reg.ET2 = 1;
  Arcnet->Base[ARC_WR_REG_CONFIG] = config_reg.Byte;

  setup1_reg.Byte = 0;
  setup1_reg.SlowARB = 1;
  setup1_reg.FourNACKS = 1;
  setup1_reg.Pulse1Mode = 1;
  setup1_reg.ReceiveAll = ReceiveAll;
  Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_SETUP1;
  Arcnet->Base[ARC_WR_REG_SETUP1] = setup1_reg.Byte;

  // Set node ID
  Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_NODE_ID;
  Arcnet->Base[ARC_WR_REG_NODE_ID] = NodeId;

  // Clear all flags
  Arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_CLEAR_ALL_FLAGS;

  // Enabling long packet handling
  Arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_ENABLE_LONG_PACKETS;

  // Set the receive page
  Arcnet->Base[ARC_WR_REG_COMMAND] = (Arcnet->ReceiveBroatcasts << 7) + (Arcnet->RxPage << 3) + 0x04;

  // Enable transmitter to join node to network.
  config_reg.TxEnable = 1;
  Arcnet->Base[ARC_WR_REG_CONFIG] = config_reg.Byte;

  // Enable the RI and recon interrupts
  interrupt_reg.Byte = 0;
  interrupt_reg.RI = 1;
  interrupt_reg.Recon = 1;
  interrupt_reg.NewNextId = 1;
  interrupt_reg.ExcessiveNACK = 1;
  Arcnet->Base[ARC_WR_REG_INTERRUPT] = interrupt_reg.Byte;
  Arcnet->InterruptMask = interrupt_reg.Byte;

  EnableHardwareInterrupt(ArcnetInterruptHandler, (unsigned int) Arcnet, HardwareInterrupt, FALSE);
}

void ArcnetDisable(sArcnet *Arcnet)
{
  uArcnetConfig config_reg;


  config_reg.Byte = Arcnet->Base[ARC_WR_REG_CONFIG];

  config_reg.TxEnable = 0;
  Arcnet->Base[ARC_WR_REG_CONFIG] = config_reg.Byte;
}

static eArcnetErrors ArcnetSendTxMsg(sArcnet *Arcnet, unsigned char ToNodeId, unsigned char *TxData, unsigned short DataLength)
{
  unsigned int index;
  unsigned int count;
  unsigned int offset;
  unsigned int word_cnt;
  unsigned int arcnet_header;
  unsigned char *byte_ptr;
  unsigned int tmp_tx_buffer[ARCNET_MAX_DATA_LENGTH >> 2];
  uArcnetStatus status;
  uArcnetInterrupt interrupt_reg;


  status.Byte = Arcnet->Base[ARC_RD_REG_STATUS];

  if (status.TA == FALSE) {
    return arcTX_ERR;
  }

  if (DataLength > ARCNET_MAX_DATA_LENGTH) {
    return arcDATA_SIZE_ERR;
  }

  // Arcnet length is not allowed to be 254, 255 or 256
  if ((DataLength > 253) && (DataLength < 257)) {
    DataLength = 257;
  }

  // The data to transmit is first copied to a 32-bit temporary buffer and then to the Arcnet data buffer which is also 32-bit. The reason a
  // temporary buffer is used is to cater for Arcnet messages with lengths that is not a multiple of 4.

  // Determine the offset of the first byte within the first word of the temporary 32-bit buffer
  offset = (4 - (DataLength & 0x03)) & 0x03;
  // Copy the input tx data to the temporary Arcnet buffer
  byte_ptr = (unsigned char *) tmp_tx_buffer;
  memcpy(&byte_ptr[offset], TxData, DataLength);

  // Populate Arcnet header
  if (DataLength < 254) {
    // Short packet
    count = 0x100 - DataLength;
    arcnet_header = (count << 16) + (ToNodeId << 8) + Arcnet->OwnNodeId;;
  }
  else {
    // Long packet
    count = 0x200 - DataLength;
    arcnet_header = (count << 24) + (ToNodeId << 8) + Arcnet->OwnNodeId;
  }

  // Calculate the number of 32-bit words to copy
  word_cnt = (DataLength + 3) >> 2;

  // Offset = (0x800 + (0x200 * page)) / 4     (only page 0 is used for transmit)
  offset = 0x200;

  // Write Arcnet header
  Arcnet->Base[offset] = arcnet_header;

  offset += (count >> 2);

  // Copy data from the tmp buffer to the Arcnet data buffer word for word (Note: memcpy does not work when ram is copied to FPGA)
  for (index = 0; index < word_cnt; index++) {
    Arcnet->Base[offset + index] = tmp_tx_buffer[index];
  }

  // Enable transmit from page 0
  Arcnet->Base[ARC_WR_REG_COMMAND] = 0x03;

  Arcnet->TxCnt++;

  // Enable TA interrupt
  interrupt_reg.Byte = Arcnet->InterruptMask;
  interrupt_reg.TA = TRUE;
  Arcnet->Base[ARC_WR_REG_INTERRUPT] = interrupt_reg.Byte;
  Arcnet->InterruptMask = interrupt_reg.Byte;

  return arcSUCCESS;
}

static eArcnetErrors ArcnetSendTxQueueMsg(sArcnet *Arcnet)
{
  unsigned int index;


  // Check mutex lock
  if (Arcnet->TxMutexLock) {
    return arcMUTEX_LOCKED;
  }

  if (Arcnet->TxQueueCnt > 0) {
    index = Arcnet->TxQueueIndex;

    if (ArcnetSendTxMsg(Arcnet, Arcnet->TxQueue[index].ToNodeId, Arcnet->TxQueue[index].Data, Arcnet->TxQueue[index].DataLength) == arcSUCCESS) {
      Arcnet->TxQueueIndex++;

      if (Arcnet->TxQueueIndex >= ARCNET_TX_QUEUE_SIZE) {
        Arcnet->TxQueueIndex = 0;
      }

      Arcnet->TxQueueCnt--;
    }
  }

  return arcSUCCESS;
}

eArcnetErrors ArcnetWrite(sArcnet *Arcnet, unsigned char ToNodeId, unsigned char *Data, unsigned short DataLength)
{
  unsigned int index;
  uArcnetStatus status;
  uArcnetInterrupt interrupt_reg;
  eArcnetErrors result = arcSUCCESS;


  // Check mutex lock
  if (Arcnet->TxMutexLock) {
    Arcnet->TxErrCnt++;
    return arcMUTEX_LOCKED;
  }

  status.Byte = Arcnet->Base[ARC_RD_REG_STATUS];

  if ((Arcnet->TxQueueCnt == 0) && (status.TA == TRUE)) {
    // Queue is empty and transmitter available
    ArcnetSendTxMsg(Arcnet, ToNodeId, Data, DataLength);
  }
  else {
    if (Arcnet->TxQueueCnt < ARCNET_TX_QUEUE_SIZE) {
      Arcnet->TxMutexLock = TRUE;

      // Add message to Tx queue
      index = Arcnet->TxQueueIndex + Arcnet->TxQueueCnt;

      if (index >= ARCNET_TX_QUEUE_SIZE) {
        index -= ARCNET_TX_QUEUE_SIZE;
      }

      Arcnet->TxQueue[index].DataLength = DataLength;
      Arcnet->TxQueue[index].ToNodeId = ToNodeId;
      memcpy(Arcnet->TxQueue[index].Data, Data, DataLength);

      Arcnet->TxQueueCnt++;

      Arcnet->TxMutexLock = FALSE;

      interrupt_reg.Byte = Arcnet->InterruptMask;
      interrupt_reg.TA = TRUE;
      Arcnet->Base[ARC_WR_REG_INTERRUPT] = interrupt_reg.Byte;
      Arcnet->InterruptMask = interrupt_reg.Byte;
    }
    else {
      // Tx queue is full
      Arcnet->TxErrCnt++;
      result = arcQUEUE_OVERFLOW;
    }
  }

  return result;
}

static unsigned short ArcnetGetRxMsg(sArcnet *Arcnet, unsigned char *FromNodeId, unsigned char *RxData)
{
  unsigned int index;
  unsigned int count;
  unsigned int offset;
  unsigned int page;
  unsigned int length;
  unsigned int word_cnt;
  unsigned int header;
  unsigned char *byte_ptr;
  unsigned int tmp_rx_buffer[ARCNET_MAX_DATA_LENGTH >> 2];
  uArcnetStatus status;


  status.Byte = Arcnet->Base[ARC_RD_REG_STATUS];

  if (status.RI == FALSE) {
    // No received messages
    return 0;
  }

  page = Arcnet->RxPage;

  // Set the next page to receive data
  Arcnet->RxPage++;

  if (Arcnet->RxPage >= 4) {
    Arcnet->RxPage = 1;
  }

  // Enable the receive to page
  Arcnet->Base[ARC_WR_REG_COMMAND] = (Arcnet->ReceiveBroatcasts << 7) + (Arcnet->RxPage << 3) + 0x04;

  // Offset = (0x800 + (0x200 * page)) / 4
  offset = (0x800 + (page << 9)) >> 2;

  // Read the Arcnet header
  header = Arcnet->Base[offset];

  if (FromNodeId != NULL) {
    *FromNodeId = header & 0xFF;
  }

  if ((header & 0xFF0000) == 0x00) {
    // Long packet
    count = (header >> 24) & 0xFF;
    length = 0x200 - count;
  }
  else {
    // Short packet
    count = (header >> 16) & 0xFF;
    length = 0x100 - count;
  }

  // Calculate the number of 32-bit words to copy
  word_cnt = (length + 3) >> 2;
  // Update offset within Arcnet data ram
  offset += (count >> 2);

  // Copy Arcnet received data to temporary buffer
  for (index = 0; index < word_cnt; index++) {
    tmp_rx_buffer[index] = Arcnet->Base[offset + index];
  }

  // Copy data from temporary 32-bit buffer to output
  byte_ptr = (unsigned char *) tmp_rx_buffer;
  offset = (4 - (length & 0x03)) & 0x03;

  memcpy(RxData, &byte_ptr[offset], length);

  Arcnet->RxCnt++;

  return length;
}

static eArcnetErrors ArcnetAddToRxQueue(sArcnet *Arcnet)
{
  unsigned char index;
  unsigned short length;
  eArcnetErrors result = arcSUCCESS;


  // Check mutex lock
  if (Arcnet->RxMutexLock) {
    return arcMUTEX_LOCKED;
  }

  Arcnet->RxMutexLock = TRUE;

  index = (Arcnet->RxQueueIndex + Arcnet->RxQueueCnt);

  if (index >= ARCNET_RX_QUEUE_SIZE) {
    index -= ARCNET_RX_QUEUE_SIZE;
  }

  length = ArcnetGetRxMsg(Arcnet, &Arcnet->RxQueue[index].FromNodeId, Arcnet->RxQueue[index].Data);

  if (length > 0) {
    Arcnet->RxQueue[index].Timestamp = GetSystemTime();
    Arcnet->RxQueue[index].DataLength = length;

    if (Arcnet->RxQueueCnt < ARCNET_RX_QUEUE_SIZE) {
      Arcnet->RxQueueCnt++;
    }
    else {
      // Rx queue full. Oldest message was overwritten
      Arcnet->RxErrCnt++;

      Arcnet->RxQueueIndex++;

      if (Arcnet->RxQueueIndex >= ARCNET_RX_QUEUE_SIZE) {
        Arcnet->RxQueueIndex = 0;
      }

      result = arcQUEUE_OVERFLOW;
    }
  }

  Arcnet->RxMutexLock = FALSE;

  return result;
}

unsigned short ArcnetRead(sArcnet *Arcnet, unsigned int *Timestamp, unsigned char *FromNodeId, unsigned char *RxData)
{
  unsigned int index;


  // Check mutex lock
  if (Arcnet->RxMutexLock) {
    return 0;
  }

  index = Arcnet->RxQueueIndex;

  if (Arcnet->RxQueueCnt > 0) {
    *Timestamp = Arcnet->RxQueue[index].Timestamp;
    *FromNodeId = Arcnet->RxQueue[index].FromNodeId;

    memcpy(RxData, Arcnet->RxQueue[index].Data, Arcnet->RxQueue[index].DataLength);

    Arcnet->RxQueueIndex++;

    if (Arcnet->RxQueueIndex >= ARCNET_RX_QUEUE_SIZE) {
      Arcnet->RxQueueIndex = 0;
    }

    Arcnet->RxQueueCnt--;

    return Arcnet->RxQueue[index].DataLength;
  }
  else {
    return 0;
  }
}

void ArcnetInterruptHandler(unsigned int Arcnet)
{
  uArcnetStatus status;
  uArcnetDiagnostic diagnostic;
  uArcnetInterrupt interrupt_reg;
  sArcnet *arcnet = (sArcnet *) Arcnet;


  status.Byte = arcnet->Base[ARC_RD_REG_STATUS];
  diagnostic.Byte = arcnet->Base[ARC_RD_REG_DIAG];

  // Handle TA interrupt
  if (status.TA == TRUE) {
    // Disable TA interrupt
    interrupt_reg.Byte = arcnet->InterruptMask;
    interrupt_reg.TA = FALSE;
    arcnet->Base[ARC_WR_REG_INTERRUPT] = interrupt_reg.Byte;
    arcnet->InterruptMask = interrupt_reg.Byte;
    // Write next message in tx queue
    ArcnetSendTxQueueMsg(arcnet);
  }

  // Handle RI interrupt
  if (status.RI == TRUE) {
    ArcnetAddToRxQueue(arcnet);
  }

  // Handle excessive NACK's
  if (diagnostic.ExcessiveNACK == TRUE) {
    arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_DISABLE_TX;
    arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_CLEAR_EXNACK_FLAG;
  }

  // Handle RECON interrupt
  if (status.Recon == TRUE) {
    if (diagnostic.MyRecon == TRUE) {
      arcnet->MyReconCnt++;
    }

    arcnet->ReconCnt++;

    // Clear status flag
    arcnet->Base[ARC_WR_REG_COMMAND] = ARC_CMD_CLEAR_RECON_FLAG;

    arcnet->BuildNodeMapState = abmRECON;
  }

  // Handle NEWNEXTID interrupt
  if (diagnostic.NewNextId == TRUE) {
    arcnet->BuildNodeMapState = abmRECON;
    ArcnetBuildMap(arcnet);
  }
}

void ArcnetBuildMap(sArcnet *Arcnet)
{
  unsigned char  next_id;
  unsigned char  tent_id;
  unsigned short mask = 1;
  uArcnetDiagnostic diagnostic;


  switch (Arcnet->BuildNodeMapState) {
    case abmRECON : {
      Arcnet->Base[ARC_RD_REG_SUB_ADDR] = ARC_RD_SUB_REG_NEXT_ID;
      next_id = Arcnet->Base[ARC_RD_REG_NEXT_ID];

      if (next_id < 240) {
        Arcnet->BuildNodeMapState = abmIDLE;
      }
      else {
        mask <<= (next_id - 240);
        Arcnet->NodesOnNetwork |= mask;

        if (next_id == Arcnet->OwnNodeId) {
          Arcnet->BuildNodeMapState = abmIDLE;
        }
        else {
          Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_TENT_ID;
          Arcnet->Base[ARC_WR_REG_TENT_ID] = Arcnet->OwnNodeId;

          Arcnet->BuildNodeMapState = abmTEST_NODE;
          ArcnetBuildMapTimeout = SetTimeout(10000);
        }
      }
    } break;
    case abmTEST_NODE : {
      Arcnet->Base[ARC_RD_REG_SUB_ADDR] = ARC_RD_SUB_REG_TENT_ID;
      tent_id = Arcnet->Base[ARC_RD_REG_NEXT_ID];

      diagnostic.Byte = Arcnet->Base[ARC_RD_REG_DIAG];

      if (diagnostic.TentativeId) {
        mask <<= (tent_id - 240);
        Arcnet->NodesOnNetwork |= mask;
      }

      if (diagnostic.TentativeId || Timeout(ArcnetBuildMapTimeout)) {
        tent_id--;

        Arcnet->Base[ARC_RD_REG_SUB_ADDR] = ARC_RD_SUB_REG_NEXT_ID;
        next_id = Arcnet->Base[ARC_RD_REG_NEXT_ID];

        if (tent_id == next_id) {
          Arcnet->BuildNodeMapState = abmIDLE;
        }
        else {
          Arcnet->Base[ARC_WR_REG_SUB_ADDR] = ARC_WR_SUB_REG_TENT_ID;
          Arcnet->Base[ARC_WR_REG_TENT_ID] = tent_id;
          ArcnetBuildMapTimeout = SetTimeout(10000);
        }
      }
    } break;
    default : break;
  }
}







