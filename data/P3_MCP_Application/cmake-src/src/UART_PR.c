/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UART_PR.c>
 *
 *  Description:
 *  ------------
 *  UART_PR module.
 *
 *  Function(s):
 *  ------------
 *  - UART_CheckFW_Version  : Checks the version of the currently loaded firmware via UART.
 *  - UART_PR_Init          : Initializes the UART_PR (Peripheral UART) interface.
 *  - UART_PR_Write         : Sends data over the UART_PR interface.
 *  - CalcBytesToRead       : Calculates the number of bytes available to read from the UART buffer.
 *  - UART_PR_Read          : Reads incoming data from the UART_PR interface.
 *  ...
 *
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "xil_printf.h"
#include "FIFO.h"
#include "UART.h"
#include "UART_PR.h"

//typedef struct _suUartTxConfig_t
typedef struct
{
	u16 baudRate;
	u8 dataWidth;
	u8 parity 		: 2;
	u8 stops		: 1;
	u8 interruptEn	: 1;
	u8 extSyncEn	: 1;
	u8 spare		: 3;
} suUartTxConfig_t;

typedef union _uUartTxConfig_t
{
	u32 singleWord;
	suUartTxConfig_t suUartConfig;
} uUartTxConfig_t;

typedef struct _suUartTx_t
{
	u32 txBufMemory[512];
	u32 uartConfig;
	u32 txConfig;
	u32 txStart;
	u32 nextTxPtr;
} suUartTx_t;

typedef struct _suUartRxConfig_t
{
	u16 baudRate;
	u8 dataWidth;
	u8 parity 		: 2;
	u8 interruptEn	: 1;
	u8 halfDupEn	: 1;
	u8 intLbEn  	: 1;
	u8 txBufDis  	: 1;
	u8 spare		: 2;
} suUartRxConfig_t;

typedef union _uUartRxConfig_t
{
	u32 singleWord;
	suUartRxConfig_t suUartConfig;
} uUartRxConfig_t;

typedef struct _suUartRxIrqConfig_t
{
	u8 timeout;
	u8 rxEn			: 1;
	u8 pktRxEn		: 1;
	u8 reserved		: 6;
	u16 rxIrqPtr;
} suUartRxIrqConfig_t;

typedef union _uUartRxIrqConfig_t
{
	u32 singleWord;
	suUartRxIrqConfig_t suUartRxIrqConfig;
} uUartRxIrqConfig_t;

typedef struct _suUartRx_t
{
	u32 rxBufMemory[512];
	u32 uartConfig;
	u32 irqConfig;
	u32 rxStatus;
	u32 version;
} suUartRx_t;
/*****************************************************************************/
/*                                                                           */
/* FUNCTION    : UART_CheckFW_Version                                        */
/*                                                                           */
/* CREATED     : 24 July 2024                                                */
/*                                                                           */
/* CODED BY    : S.A. Eliasov                                                */
/*                                                                           */
/* DESCRIPTION : Checks the verion  of the Firmware loaded                   */
/*                                                                           */
/* INPUTS      : sUART* - UART structure                                     */
/*               unsigned int - Firmware version ID					         */
/*                                                                           */
/* RETURNS     : 0 - Version ID matches                                      */
/* 		        >0 - mismatched Version ID read                              */
/*                                                                           */
/*****************************************************************************/
unsigned int UART_CheckFW_Version(sUART *UART, unsigned int ExpectedVersionID) {
  unsigned int VersionID;

  VersionID = UART->Base[UART_FIRMWARE_VERSION_OFFSET] |
		  ((UART->Base[UART_FIRMWARE_VERSION_OFFSET+1])<<8) |
		  ((UART->Base[UART_FIRMWARE_VERSION_OFFSET+2])<<16) |
		  ((UART->Base[UART_FIRMWARE_VERSION_OFFSET+3])<<24);

  if (VersionID == ExpectedVersionID)
	  return(0);
  else
	  return(VersionID);
}

void UART_PR_Init(sUART *UART, unsigned int BaseAddress, eUART_BaudRates BaudRate, eUART_Parity Parity, unsigned char StopBits, unsigned char PacketMode,unsigned short PerformLoopBackTest, unsigned short SetInternalLoopBack, unsigned char RxInteruptEnable)
{
	uUartTxConfig_t uUartTxConfig;
	uUartRxConfig_t uUartRxConfig;
	uUartRxIrqConfig_t uUartRxIrqConfig;

	volatile suUartTx_t* pUartTx = (suUartTx_t*)BaseAddress;
	volatile suUartRx_t* pUartRx = (suUartRx_t*)(BaseAddress+0x1000);

	UART->RxReadIndex = 0; // Clear last read marker
	UART->Base = (unsigned char *) BaseAddress;  //set base address for UART
	(*(u8*)(UART->Base + 0x808)) = 0;  // Disable Tx

	// Transmitter configuration
    uUartTxConfig.suUartConfig.baudRate = ((100000000 / BaudRate) - 1);
    uUartTxConfig.suUartConfig.dataWidth = 7;
    uUartTxConfig.suUartConfig.parity = Parity;
    uUartTxConfig.suUartConfig.stops = StopBits;
    uUartTxConfig.suUartConfig.interruptEn = 0;
    uUartTxConfig.suUartConfig.extSyncEn = 0;
    pUartTx->uartConfig = uUartTxConfig.singleWord;

	// Receiver configuration
    UART->RxReadIndex = (*(u16*)(UART->Base+0x1808));  // Flush Rx Buffer

    uUartRxConfig.suUartConfig.baudRate = uUartTxConfig.suUartConfig.baudRate;
    uUartRxConfig.suUartConfig.dataWidth = uUartTxConfig.suUartConfig.dataWidth;

    uUartRxConfig.suUartConfig.parity = uUartTxConfig.suUartConfig.parity;
    uUartRxConfig.suUartConfig.interruptEn = RxInteruptEnable;

    if (PerformLoopBackTest) {
    	if (SetInternalLoopBack) {
    		uUartRxConfig.suUartConfig.halfDupEn 	= 0; //0
    		uUartRxConfig.suUartConfig.intLbEn 		= 1;//1
    		uUartRxConfig.suUartConfig.txBufDis 	= 0;//0
        }
    	else {
         	uUartRxConfig.suUartConfig.halfDupEn	= 1; //0
         	uUartRxConfig.suUartConfig.intLbEn 		= 0;//1
         	uUartRxConfig.suUartConfig.txBufDis 	= 0;//0
    	}
    }
    else {
     	uUartRxConfig.suUartConfig.halfDupEn 		= 0; //0
     	uUartRxConfig.suUartConfig.intLbEn 			= 0;//1
     	uUartRxConfig.suUartConfig.txBufDis 		= 0;//0

    }
    pUartRx->uartConfig = uUartRxConfig.singleWord;

    uUartRxIrqConfig.suUartRxIrqConfig.timeout = 0;
    uUartRxIrqConfig.suUartRxIrqConfig.rxEn = 1;
    uUartRxIrqConfig.suUartRxIrqConfig.pktRxEn = PacketMode;

    pUartRx->irqConfig = uUartRxIrqConfig.singleWord;

}

unsigned int UART_PR_Write(sUART *UART, char *TxData, unsigned int BytesToWrite)
{
	volatile unsigned int Cnt;

	(*(u8*)(UART->Base + 0x808)) = 0;  // Disable Tx

    (*(u8*)(UART->Base + 0x80A)) = 1;  // Clear Tx Interrupt register

    for (Cnt = 0;Cnt < BytesToWrite; Cnt++)
    	(*(u8*)(UART->Base + Cnt)) = TxData[Cnt];  //load message

    (*(u16 *)(UART->Base + 0x806)) = (u16)(BytesToWrite); //set length

    (*(u8*)(UART->Base + 0x808)) = 1;  // Enable Tx

    return(Cnt);
}

unsigned int CalcBytesToRead(unsigned int UART_Index, unsigned int ReadIndex) {

	if (UART_Index == ReadIndex)
		return(0);
	else if (UART_Index < ReadIndex)
		return(ReadIndex - UART_Index);
	else
		return ((UART_DATA_BUFFER_SIZE - UART_Index) + ReadIndex);
}


unsigned short UART_PR_Read(sUART *UART, sCharFIFO *RxFIFO, unsigned int MaxLength, unsigned char PacketMode)
{
	volatile u32 BytesToRead;
	volatile u32 MemIndex;
	volatile u32 MaxedOut;
	volatile u32 Cnt;
	volatile unsigned short RxBufPtr;
	volatile unsigned char RxStatus;
	unsigned char ByteRead;

	BytesToRead = 0;
	MaxedOut = 0; // False
/**/
	if (PacketMode) {
		RxStatus = (*(u8*)(UART->Base+0x180A));
		RxBufPtr = (*(u16*)(UART->Base+0x1808));
		BytesToRead = CalcBytesToRead(UART->RxReadIndex, RxBufPtr);
		if (RxStatus) {
			(*(u8*)(UART->Base+0x180B))=0x5;  //Clear Rx status
			if (BytesToRead > MaxLength)
			{
				BytesToRead = MaxLength;
				MaxedOut = 1; // True
			}
			MemIndex = UART->RxReadIndex;  // get last location read to start from
			for (Cnt = 0;Cnt < BytesToRead; Cnt++)
			{
				ByteRead = (*(u8*)(UART->Base + 0x1000 + MemIndex));
				PUT_FIFO_Ptr(RxFIFO,ByteRead);
				MemIndex++;
				if (MemIndex >= UART_DATA_BUFFER_SIZE)
					MemIndex = 0;
			}

			if (MaxedOut)
				UART->RxReadIndex = RxBufPtr; // flush buffer
			else
				UART->RxReadIndex = MemIndex; // save last location read
		}
	}
	else {
/**/
		BytesToRead = CalcBytesToRead(UART->RxReadIndex, (*(u16*)(UART->Base+0x1808)));
		if (BytesToRead) {
			MemIndex = UART->RxReadIndex;
			for (Cnt = 0;Cnt < BytesToRead; Cnt++)
			{
				ByteRead = (*(u8*)(UART->Base + 0x1000 + MemIndex));
				PUT_FIFO_Ptr(RxFIFO,ByteRead);
				MemIndex++;
				if (MemIndex >= UART_DATA_BUFFER_SIZE)
					MemIndex = 0;
			}
			UART->RxReadIndex = MemIndex;//(*(u16*)(UART->Base+0x1808));
		}
	}

    return(BytesToRead);
}
