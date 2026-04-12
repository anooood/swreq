/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UART.h>
 *
 *  Description:
 *  ------------
 *  Header file for the UART module.
 *
 ******************************************************************************/

#ifndef UART_H
#define UART_H

#define UART_DATA_BUFFER_SIZE         0x800
#define UART_DATA_BUFFER_SIZE_MASK    0x7FF

typedef enum
{
  uartSUCCESS,
  uartTX_ERROR
} eUART_Errors;

typedef enum
{
  br9600   = 9600,
  br19200  = 19200,
  br38400  = 38400,
  br57600  = 57600,
  br115200 = 115200,
  br230400 = 230400,
  br460800 = 460800,
  br921600 = 921600,
  br3000000 = 3000000, // AuxPort Baud Rate
  br3125000 = 3125000	// MPSAL Baud Rate
} eUART_BaudRates;

typedef enum 
{
  parNONE = 0,
  parEVEN = 2,
  parODD  = 3
} eUART_Parity;

typedef struct
{
  volatile unsigned char *Base;
  unsigned int RxReadIndex;
} sUART;


void UART_Init(sUART *UART, unsigned int BaseAddress, eUART_BaudRates BaudRate, eUART_Parity Parity, unsigned char StopBits, unsigned char PacketMode);
eUART_Errors UART_Write(sUART *UART, unsigned char *TxData, unsigned short Length);
unsigned short UART_Read(sUART *UART, unsigned char *RxData, unsigned int MaxLength);
void UART_SetBaudRate(sUART *UART, unsigned int NewBaudRate);
void UART_Flush(sUART *UART);

#endif
