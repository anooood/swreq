/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UART.c>
 *
 *  Description:
 *  ------------
 *  UART module.
 *
 *  Function(s):
 *  ------------
 *  - UART_Init                 : Initializes the UART.
 *  - UART_Read                 : Reads data.
 *  - UART_Write                : Transmit data.
 *  - UART_Flush                : Flush data.
 *  - UART_SetBaudRate          : Changes UART baud rate.

 *  ...
 *
 *
 ******************************************************************************/

#include "UART.h"

// UART FPGA register offsets
#define UART_TX_BUFFER_OFFSET         0x0000
#define UART_TX_SETTINGS_OFFSET       0x0800
#define UART_TX_CONTROL_OFFSET        0x0804
#define UART_RX_BUFFER_OFFSET         0x1000
#define UART_RX_SETTINGS_OFFSET       0x1800
#define UART_RX_CONTROL_1_OFFSET      0x1804
#define UART_RX_STATUS                0x1808
#define UART_RX_CONTROL_2_OFFSET      0x180B
#define UART_RX_STATUS0               (unsigned int)(0x1808)
#define UART_RX_STATUS1               (unsigned int)(0x1809)
#define UART_RX_STATUS2               (unsigned int)(0x180A)
#define UART_RX_STATUS3               (unsigned int)(0x180B)

#define UART_DATA_BUFFER_SIZE         0x800
#define UART_DATA_BUFFER_SIZE_MASK    0x7FF


typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int BaudRate             : 16;
    unsigned int DataBits             : 3;
    unsigned int Reserved1            : 5;
    unsigned int Parity               : 2;
    unsigned int StopBits             : 1;
    unsigned int InterruptEnable      : 1;
    unsigned int UseExternalSync      : 1;
    unsigned int Reserved2            : 3;
  };
} uUART_TxSettings;

typedef union
{
  unsigned int DWord[2];
  struct
  {
    unsigned short TxBufferOffset;
    unsigned short Length;
    unsigned char StartTx;
    unsigned char InterruptActive;
    unsigned char ClearInterrupt;
    unsigned char TxPointerPosition;
  };
} uUART_TxControl;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int BaudRate             : 16;
    unsigned int DataBits             : 3;
    unsigned int Reserved1            : 5;
    unsigned int Parity               : 2;
    unsigned int InterruptEnable      : 1;
    unsigned int HalfDuplexMode       : 1;
    unsigned int LoopbackEnable       : 1;
    unsigned int LoopbackDisableTx    : 1;
    unsigned int Reserved2            : 2;
  };
} uUART_RxSettings;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned int RxBufferOffset       : 11;
    unsigned int Reserved1            : 5;
    unsigned int RxTimeout            : 1;
    unsigned int RxInterruptActive    : 1;
    unsigned int ParityError          : 1;
    unsigned int FramingError         : 1;
    unsigned int Reserved2            : 12;
  };
} uUART_RxStatus;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned char TimeoutCnt;
    unsigned char RxEnable;
    unsigned short InterruptTriggerPos;
  };
} uUART_RxControl1;

typedef union
{
  unsigned char Byte;
  struct
  {
    unsigned char ClearStatusRegister : 1;
    unsigned char ResetBufferPtr      : 1;
    unsigned char ClearInterrupt      : 1;
    unsigned char Reserved            : 5;
  };
} uUART_RxControl2;

void UART_Init(sUART *UART, unsigned int BaseAddress, eUART_BaudRates BaudRate, eUART_Parity Parity, unsigned char StopBits, unsigned char PacketMode)
{
  uUART_TxSettings tx_settings;
  uUART_TxControl tx_control;
  uUART_RxSettings rx_settings;
  uUART_RxControl1 rx_control1;
  uUART_RxControl2 rx_control2;


  UART->Base = (volatile unsigned char *) BaseAddress;
  UART->RxReadIndex = 0;

  tx_settings.DWord = 0;
  tx_settings.BaudRate = (100000000 / BaudRate) - 1;
  tx_settings.Parity = Parity;
  tx_settings.DataBits = 7;
  tx_settings.StopBits = StopBits - 1;
  tx_control.DWord[0] = 0;
  tx_control.DWord[1] = 0;
  tx_control.ClearInterrupt = 1;

  rx_settings.DWord = 0;
  rx_settings.BaudRate = tx_settings.BaudRate;
  rx_settings.Parity = tx_settings.Parity;
  rx_settings.DataBits = tx_settings.DataBits;
  if (PacketMode) {
	  rx_settings.HalfDuplexMode = 0;
	  rx_settings.LoopbackEnable = 1;

  }
  rx_control1.DWord = 0;
  rx_control1.TimeoutCnt = 2;
  rx_control1.RxEnable = 1;
  if (PacketMode) {
	  rx_control1.RxEnable = rx_control1.RxEnable | 2;
  }
  rx_control2.Byte = 0;
  rx_control2.ClearInterrupt = 1;
  rx_control2.ClearStatusRegister = 1;
  rx_control2.ResetBufferPtr = 1;

  *((volatile uUART_TxSettings *) &UART->Base[UART_TX_SETTINGS_OFFSET]) = tx_settings;
  *((volatile uUART_TxControl *) &UART->Base[UART_TX_CONTROL_OFFSET]) = tx_control;
  *((volatile uUART_RxSettings *) &UART->Base[UART_RX_SETTINGS_OFFSET]) = rx_settings;
  *((volatile uUART_RxControl1 *) &UART->Base[UART_RX_CONTROL_1_OFFSET]) = rx_control1;
  *((volatile uUART_RxControl2 *) &UART->Base[UART_RX_CONTROL_2_OFFSET]) = rx_control2;
}

eUART_Errors UART_Write(sUART *UART, unsigned char *TxData, unsigned short Length)
{
  unsigned int index;
  volatile uUART_TxControl *control;


  control = (volatile uUART_TxControl *) &UART->Base[UART_TX_CONTROL_OFFSET]; //(UART->BaseAddr + UART_TX_CONTROL_OFFSET);

  if (control->StartTx == 0) {
     for (index = 0; index < Length; index++) {
      UART->Base[UART_TX_BUFFER_OFFSET + index] = TxData[index];
    }

    control->Length = Length;
    control->StartTx = 1;

    return uartSUCCESS;
  }
  else {
    // UART did not complete previous transmission
    return uartTX_ERROR;
  }
}

unsigned short UART_Read(sUART *UART, unsigned char *RxData, unsigned int MaxLength)
{
  unsigned int index;
  int length;
  volatile uUART_RxStatus status;


  status = *((volatile uUART_RxStatus *) &UART->Base[UART_RX_STATUS]);

//  length = (status.RxBufferOffset - UART->RxReadIndex) & UART_DATA_BUFFER_SIZE_MASK; //formula incorrect
/**/
  if (status.RxBufferOffset == UART->RxReadIndex)
	  length = 0;
  else if (UART->RxReadIndex < status.RxBufferOffset)
	  length = (status.RxBufferOffset - UART->RxReadIndex);
  else
	  length = (UART_DATA_BUFFER_SIZE + status.RxBufferOffset - UART->RxReadIndex);
/**/

  if (length > 0) {
    // Limit the number of bytes to copy
    if (length > MaxLength) {
      length = MaxLength;
    }

    for (index = 0; index < length; index++) {
      RxData[index] = UART->Base[UART_RX_BUFFER_OFFSET + UART->RxReadIndex];
      UART->RxReadIndex = (UART->RxReadIndex + 1) & UART_DATA_BUFFER_SIZE_MASK;
    }
  }

  return length;
}


void UART_Flush(sUART *UART)
{
  volatile uUART_RxStatus status;


  status = *((volatile uUART_RxStatus *) &UART->Base[UART_RX_STATUS]);

  UART->RxReadIndex = status.RxBufferOffset;
}

void UART_SetBaudRate(sUART *UART, unsigned int NewBaudRate)
{
  uUART_TxSettings tx_settings;
  uUART_RxSettings rx_settings;
  uUART_RxControl2 rx_control2;
  volatile uUART_RxStatus *rx_status;


  tx_settings = *((volatile uUART_TxSettings *) &UART->Base[UART_TX_SETTINGS_OFFSET]);
  tx_settings.BaudRate = (100000000 / NewBaudRate) - 1;

  rx_settings = *((volatile uUART_RxSettings *) &UART->Base[UART_RX_SETTINGS_OFFSET]);
  rx_settings.BaudRate = (100000000 / NewBaudRate) - 1;

  rx_control2.Byte = 0;
  rx_control2.ClearInterrupt = 1;
  rx_control2.ClearStatusRegister = 1;
  rx_control2.ResetBufferPtr = 1;

  *((volatile uUART_TxSettings *) &UART->Base[UART_TX_SETTINGS_OFFSET]) = tx_settings;
  *((volatile uUART_RxSettings *) &UART->Base[UART_RX_SETTINGS_OFFSET]) = rx_settings;
  *((volatile uUART_RxControl2 *) &UART->Base[UART_RX_CONTROL_2_OFFSET]) = rx_control2;

  rx_status = (volatile uUART_RxStatus *) &UART->Base[UART_RX_STATUS];
  rx_status->RxBufferOffset = 0;

  UART->RxReadIndex = 0;
}
