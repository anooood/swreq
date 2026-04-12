/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Interrupts.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Interrupts module.
 *
 ******************************************************************************/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H


typedef enum
{
  ipHIGHEST,
  ipHIGH,
  ipMEDIUM,
  ipLOW,
  ipLOWEST
} eInterruptPriorities;

// Hardware interrupts
typedef enum
{
  intSYNC,            // 0
  intARCNET_1,        // 1 - MLAN
  intARCNET_2,        // 2 - TLAN
  intTIMER,           // 3 (Edge triggered)
  intGNSS_PPS,        // 4 (Edge triggered)
  intI2C,             // 5
  intSPARE_1,         // 6
  intSDLC,            // 7 - IMU
  intSPARE_2,         // 8
  intRS485_BIT,       // 9
  intUART_1,          // 10 - GNSS
  intUART_2,          // 11
  intUART_3,          // 12 - EFUZE
  intUART_4,          // 13 - FTS
  intUART_5,          // 14 - Battery pack controller
  intSPARE_3,         // 15
  intLAST             // 16
} eHarwareInterrupts;


void InitInterrups(void);
void EnableInterrupts(void);
void DisableInterrupts(void);
void AddInterrupt(void (*Handler)(void), unsigned int Id, eInterruptPriorities Priority);
void EnableInterrupt(unsigned int InterruptId);
void DisableInterrupt(unsigned int InterruptId);
void TriggerSoftwareInterrupt(unsigned int InterruptId);
void EnableHardwareInterrupt(void (*Handler)(unsigned int), unsigned int HandlerInput, eHarwareInterrupts HardwareInterrupt, unsigned char EdgeTriggered);
void DisableHardwareInterrupt(eHarwareInterrupts HardwareInterrupt);

#endif
