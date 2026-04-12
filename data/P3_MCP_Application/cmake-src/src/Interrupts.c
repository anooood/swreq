/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Interrupts.c>
 *
 *  Description:
 *  ------------
 *  Interrupts module.
 *
 *  Function(s):
 *  ------------
 *  - InitInterrups:                       Initializes the processor interrupts
 *  - EnableInterrupts:                    Enables all interrupts
 *  - DisableInterrupts:                   Disable all interrupts
 *  - AddInterrupt:                        Adds and enables an interrupt
 *  - EnableInterrupt:                     Enables an interrupt
 *  - DisableInterrupt:                    Disable an interrupt
 *  - TriggerSoftwareInterrupt:            Triggers a software interrupt
 *  - ClearEdgeTriggeredHardwareInterrupt: Clear edge triggered hardware interrupts
 *  - EnableHardwareInterrupt:             Enables a hardware interrupt
 *  - DisableHardwareInterrupt:            Disables a hardware interrupt
 *  - HardwareInterruptHandler:            Handles and clear all active hardware interrupts
 *  ...
 *
 *
 ******************************************************************************/


#include <xscugic.h>
#include <xparameters.h>
#include "Interrupts.h"
#include "FPGA.h"


// NOTE: The default stack sizes (_IRQ_STACK_SIZE when implementing nested interrupts) are not enough and need to
// be increased substantially in the linker script file lscript.ld to avoid unpredictable behavior

#define HARDWARE_INTERRUPT_ID       0x1F


typedef struct
{
  unsigned int FunctionPtr;
  unsigned int FunctionInput;
  unsigned short Mask;
  unsigned char IsEdgeTriggered;
  unsigned char Spare;
} sHarwareInterrupt;


static XScuGic InterrupsController;
static XScuGic_Config *InterruptsConfig;
static unsigned int HardwareInterruptCnt;
static sHarwareInterrupt HardwarwInterruptHandlers[intLAST];

static void ClearEdgeTriggeredHardwareInterrupt(unsigned int ClearMask);
static void HardwareInterruptHandler(void);


//Xil_ExceptionEnableMask
//XIL_EXCEPTION_ID_DIV_BY_ZERO
//#define XIL_EXCEPTION_CRITICAL        0x00020000
//#define XIL_EXCEPTION_NON_CRITICAL    0x00008000
//#define XIL_EXCEPTION_ALL             0x00028000
//#define XIL_EXCEPTION_ID_FIRST0#define XIL_EXCEPTION_ID_JUMP_TO_ZERO0#define XIL_EXCEPTION_ID_CRITICAL_INT 1#define XIL_EXCEPTION_ID_MACHINE_CHECK2#define XIL_EXCEPTION_ID_DATA_STORAGE_INT 3

// Kyk na doc UG585

/*void EXCEPTION_DataAbortHandler(void *CallBackRef)
{
  static volatile int a = 0;

  a++;
}*/

void InitInterrups(void)
{
  HardwareInterruptCnt = 0;
  memset(HardwarwInterruptHandlers, 0, sizeof(HardwarwInterruptHandlers));

  Xil_ExceptionDisable();

  InterruptsConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);

  XScuGic_CfgInitialize(&InterrupsController, InterruptsConfig, InterruptsConfig->CpuBaseAddress);

  XScuGic_SelfTest(&InterrupsController);

  XScuGic_CPUWriteReg(&InterrupsController, XSCUGIC_BIN_PT_OFFSET, 0x03);

  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler, &InterrupsController);

  Xil_ExceptionInit();

  // Disable all hardware interrupts
  *((volatile uint32_t *) FPGA_INTERRUPT_MASK_ADDR) = 0;
  // Clear all edge triggered hardware interrupts
  ClearEdgeTriggeredHardwareInterrupt(0xFFFF);

  // Add hardware interrupt with highest priority
  AddInterrupt(HardwareInterruptHandler, HARDWARE_INTERRUPT_ID, ipHIGHEST);
}

void EnableInterrupts(void)
{
  Xil_ExceptionEnable();
}

void DisableInterrupts(void)
{
  Xil_ExceptionDisable();
}

void AddInterrupt(void (*Handler)(void), unsigned int Id, eInterruptPriorities Priority)
{
  XScuGic_Connect(&InterrupsController, Id, (Xil_ExceptionHandler) Handler, &InterrupsController);

  // Set interrupt priority
  XScuGic_SetPriorityTriggerType(&InterrupsController, Id, ((Priority << 4) & 0xF0), 1);

  // Enable the interrupt
  XScuGic_Enable(&InterrupsController, Id);
}

void EnableInterrupt(unsigned int InterruptId)
{
  XScuGic_Enable(&InterrupsController, InterruptId);
}

void DisableInterrupt(unsigned int InterruptId)
{
  XScuGic_Disable(&InterrupsController, InterruptId);
}

void TriggerSoftwareInterrupt(unsigned int InterruptId)
{
  XScuGic_SoftwareIntr(&InterrupsController, InterruptId, 1);
}

void ClearEdgeTriggeredHardwareInterrupt(unsigned int ClearMask)
{
  *((volatile unsigned int *) FPGA_INTERRUPT_CLEAR_ADDR) = ClearMask;
}

void EnableHardwareInterrupt(void (*Handler)(unsigned int), unsigned int HandlerInput, eHarwareInterrupts HardwareInterrupt, unsigned char EdgeTriggered)
{
  unsigned int mask;


  if ((Handler != NULL) && (HardwareInterrupt < intLAST)) {
    HardwarwInterruptHandlers[HardwareInterruptCnt].FunctionInput = HandlerInput;
    HardwarwInterruptHandlers[HardwareInterruptCnt].FunctionPtr = (unsigned int) Handler;
    HardwarwInterruptHandlers[HardwareInterruptCnt].Mask = 1 << HardwareInterrupt;
    HardwarwInterruptHandlers[HardwareInterruptCnt].IsEdgeTriggered = EdgeTriggered;

    HardwareInterruptCnt++;

    // Get current enabled hardware interrupts
    mask = *((volatile unsigned int *) FPGA_INTERRUPT_MASK_ADDR);

    // Enable the new interrupt
    mask |= (1 << HardwareInterrupt);
    *((volatile unsigned int *) FPGA_INTERRUPT_MASK_ADDR) = mask;
  }
}

void DisableHardwareInterrupt(eHarwareInterrupts HardwareInterrupt)
{
  unsigned int mask;


  // Get current enabled hardware interrupts
  mask = *((volatile unsigned int *) FPGA_INTERRUPT_MASK_ADDR);

  // Disable the interrupt
  mask &= (~(1 << HardwareInterrupt));
  *((volatile unsigned int *) FPGA_INTERRUPT_MASK_ADDR) = mask;
}

void HardwareInterruptHandler(void)
{
  unsigned int index = 0;
  unsigned int status;
  void (*handler)(unsigned int);


  // Handle all active hardware interrupts. Time critical interrupts will be handled first such as the IMU interrupt
  while (((status = *((volatile unsigned int *) FPGA_INTERRUPT_STATUS_ADDR) & 0xFFFF) > 0) && (index < HardwareInterruptCnt)) {
    if ((status & HardwarwInterruptHandlers[index].Mask) != 0) {
      if (HardwarwInterruptHandlers[index].IsEdgeTriggered) {
        ClearEdgeTriggeredHardwareInterrupt(HardwarwInterruptHandlers[index].Mask);
      }

      handler = (void (*)(unsigned int)) HardwarwInterruptHandlers[index].FunctionPtr;
      handler(HardwarwInterruptHandlers[index].FunctionInput);
    }

    index++;
  };
}

