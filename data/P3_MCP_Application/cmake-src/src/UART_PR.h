/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UART_PR.h>
 *
 *  Description:
 *  ------------
 *  Header file for the UART_PR module.
 *
 ******************************************************************************/

#ifndef UART_PR_H
#define UART_PR_H

#define UART_FIRMWARE_VERSION_OFFSET  (unsigned int)(0x180C)

unsigned int UART_CheckFW_Version(sUART *UART, unsigned int ExpectedVersionID);
void UART_PR_Init(sUART *UART, unsigned int BaseAddress, eUART_BaudRates BaudRate, eUART_Parity Parity, unsigned char StopBits, unsigned char PacketMode,unsigned short PerformLoopBackTest, unsigned short SetInternalLoopBack, unsigned char RxInteruptEnable);
unsigned short UART_PR_Read(sUART *UART, sCharFIFO *RxFIFO, unsigned int MaxLength, unsigned char PacketMode);
unsigned int UART_PR_Write(sUART *UART, char *TxData, unsigned int BytesToWrite);
unsigned int CalcBytesToRead(unsigned int UART_Index, unsigned int ReadIndex);

#endif
