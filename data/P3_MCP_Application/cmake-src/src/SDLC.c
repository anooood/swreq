/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <SDLC.c>
 *
 *  Description:
 *  ------------
 *  SDLC.c implements the communication interface for the SDLC protocol.
 *
 *  Function(s):
 *  ------------
 *  - SDLC_Init:                   Initializes the SDLC communications interface
 *  - SDLC_InterruptHandler:       SDLC interrupt handler
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include "SDLC.h"
#include "TypeDefines.h"
#include "Interrupts.h"


// SDLC FPGA register offsets
#define SDLC_RX_RAM                 0x0000
#define SDLC_SETUP                  0x0200
#define SDLC_INTERRUPT              0x0201
#define SDLC_INTERRUPT_MASK         0x0202         // Read-only, use SET or CLEAR registers to update
#define SDLC_INTERRUPT_MASK_SET     0x0203
#define SDLC_INTERRUPT_MASK_CLEAR   0x0204
#define SDLC_RESET                  0x0205

#define SDLC_PAGE_SIZE              0x80           // Number of 32-bit DWORDS


typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned char PageCnt       : 6;
    unsigned char Encoding      : 1;
    unsigned char PAD           : 1;
    unsigned char Version;
    unsigned short Reserved;
  };
} sSDLC_Setup;

typedef union
{
  unsigned int DWord;
  struct
  {
    unsigned short Length;
    unsigned short SeqNo      : 15;
    unsigned short CrcError   : 1;
  };
} sSDLC_RxStatus;


static void SDLC_InterruptHandler(unsigned int Input);

void SDLC_Init(sSDLC *SDLC, unsigned BaseAddr, void (*Callback)(unsigned char*, unsigned short, eSDLC_Result))
{
  unsigned int index;
  unsigned int interrupt_mask = 0;
  sSDLC_Setup setup;


  memset(SDLC, 0, sizeof(sSDLC));

  SDLC->Base = (volatile unsigned int *) BaseAddr;
  SDLC->Callback = Callback;

  setup.DWord = SDLC->Base[SDLC_SETUP];

  SDLC->PageCnt = setup.PageCnt;

  for (index = 0; index < SDLC->PageCnt; index++) {
    interrupt_mask += (1 << (index + 1));
  }

  // Reset SDLC receiver logic
  SDLC->Base[SDLC_RESET] = 1;
  // Set interrupts for all receive pages
  SDLC->Base[SDLC_INTERRUPT_MASK_SET] = interrupt_mask;
  // Enable SDLC interrupt
  EnableHardwareInterrupt(SDLC_InterruptHandler, (unsigned int) SDLC, intSDLC, FALSE);
}

void SDLC_InterruptHandler(unsigned int Input)
{
  unsigned char page;
  unsigned char dword_length;
  unsigned int index;
  unsigned int interrupt_mask;
  unsigned int page_offset;
  unsigned int rx_data[SDLC_PAGE_SIZE];
  sSDLC_RxStatus status;
  eSDLC_Result result;
  sSDLC *sdlc = (sSDLC *) Input;


  // Get active interrupts
  interrupt_mask = sdlc->Base[SDLC_INTERRUPT];
  // Clear all active interrupts
  sdlc->Base[SDLC_INTERRUPT] = interrupt_mask;

  // Right shift mask by 1 so that page 0 will be represented by bit 0, page 1 by bit 1 and so on
  interrupt_mask >>= 1;

  // Handle interrupt. Only a single interrupt should be active. If more than one interrupts is active then
  // something went wrong and only the first interrupt detected would be handled
  for (index = 0; index < sdlc->PageCnt; index++) {
    // Start with the expected page (PageIndex) to minimize the number of loops
    page = sdlc->PageIndex + index;

    if (page >= sdlc->PageCnt) {
      page -= sdlc->PageCnt;
    }

    if (((interrupt_mask >> page) & 0x01) == 0x01) {
      page_offset = SDLC_PAGE_SIZE * page;

      status = *((sSDLC_RxStatus *) &sdlc->Base[page_offset]);

      if (status.CrcError) {
        // Ignore packets with CRC errors
        status.Length = 0;
        result = sdlcCRC_ERR;
        sdlc->ErrorCnt++;
      }
      else {
        page_offset++;
        dword_length = (status.Length + 3) >> 2;

        // Copy DWORD for DWORD (memcpy does not work from FPGA ram to CPU ram)
        for (index = 0; index < dword_length; index++) {
          rx_data[index] = sdlc->Base[page_offset + index];
        }

        if (status.SeqNo == sdlc->SeqNo) {
          result = sdlcOK;
        }
        else {
          // Sequence number mismatch. A packet was lost
          result = sdlcSEQ_ERR;
          sdlc->ErrorCnt++;
        }

      }

      sdlc->Callback((unsigned char *) rx_data, status.Length, result);
      sdlc->RxCnt++;

      // Increment the sequence number
      sdlc->SeqNo = (status.SeqNo + 1) & 0x7FFF;
      sdlc->PageIndex = page + 1;

      break;
    }
  }
}
