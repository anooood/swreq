/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Stack.c>
 *
 *  Description:
 *  ------------
 *  Stack Module.
 *
 *  Function(s):
 *  ------------
 *  - StackInit                      : Clears the stack region by setting all bytes to MCP_STACK_CLEAR_BYTE.
 *  - StackGetUssage                 : Determines the stack usage.
 *  - __stack_chk_fail               : DStack overrun check callback
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include "Stack.h"
#include "TypeDefines.h"

sStack Stack;

/* If the compiler flag -fstack-protector-all is used, then the variable __stack_chk_guard and
   the function __stack_chk_fail have to be declared to avoid compiler errors. If the compiler flag
   is used then stack overrun checks are also inserted in all function calls (seen when viewing the
   assembler code) */
/* MISRA C:2012 Rule 5.1 deviation (UNDESCORE) */
uint32_t __stack_chk_guard __attribute__((section(".StackProtectorSegment"))) = MCP_STACK_CHK_GUARD;

/* The following are defined in the linker script (lscript.ld) */
/* MISRA C:2012 Rule 5.1 deviation (UNDESCORE) */
extern uint8_t *_stack_protector;
extern uint8_t *_stack_end;
extern uint8_t *_irq_stack_end;
extern uint8_t *_supervisor_stack_end;
extern uint8_t *_abort_stack_end;
extern uint8_t *_fiq_stack_end;
extern uint8_t *_undef_stack_end;
extern uint8_t *__undef_stack;

void StackInit(void)
{
  uint32_t stack_addr[MCP_STACK_ADDR_7];


  memset(&Stack, 0, sizeof(sStack));

  stack_addr[0]                = (uint32_t) &_stack_end;
  stack_addr[MCP_STACK_ADDR_1] = (uint32_t) &_irq_stack_end;
  stack_addr[MCP_STACK_ADDR_2] = (uint32_t) &_supervisor_stack_end;
  stack_addr[MCP_STACK_ADDR_3] = (uint32_t) &_abort_stack_end;
  stack_addr[MCP_STACK_ADDR_4] = (uint32_t) &_fiq_stack_end;
  stack_addr[MCP_STACK_ADDR_5] = (uint32_t) &_undef_stack_end;
  stack_addr[MCP_STACK_ADDR_6] = (uint32_t) &__undef_stack;

  /* Set all bytes in the stack region to the MCP_STACK_CLEAR_BYTE. Do not clear the stack protector as well as
     the first 0x100 bytes of the normal stack */
  /* Clear normal stack */
  memset((void *) stack_addr[0], MCP_STACK_CLEAR_BYTE, (stack_addr[1] - stack_addr[0]) - MCP_STACK_FIRST_BYTES);
  /* Clear all other stacks */
  memset((void *) stack_addr[1], MCP_STACK_CLEAR_BYTE, (stack_addr[MCP_STACK_ADDR_6] - stack_addr[1]));
}

void StackGetUssage(sStackInfo *StackInfo)
{
  uint32_t index;
  int32_t stack_index;
  uint32_t stack_addr[MCP_STACK_ADDR_7];
  uint8_t *byte;


  stack_addr[0]                = (uint32_t) &_stack_end;
  stack_addr[MCP_STACK_ADDR_1] = (uint32_t) &_irq_stack_end;
  stack_addr[MCP_STACK_ADDR_2] = (uint32_t) &_supervisor_stack_end;
  stack_addr[MCP_STACK_ADDR_3] = (uint32_t) &_abort_stack_end;
  stack_addr[MCP_STACK_ADDR_4] = (uint32_t) &_fiq_stack_end;
  stack_addr[MCP_STACK_ADDR_5] = (uint32_t) &_undef_stack_end;
  stack_addr[MCP_STACK_ADDR_6] = (uint32_t) &__undef_stack;

  for (stack_index = 0; stack_index < (int32_t)stackLAST; stack_index++) {
    StackInfo[stack_index].Size = stack_addr[stack_index + 1] - stack_addr[stack_index];

    byte = (uint8_t *) stack_addr[stack_index];

    for (index = 0; index < StackInfo[stack_index].Size; index++) {
      if (*byte != MCP_STACK_CLEAR_BYTE) {
        break;
      }

      byte++;
    }

    StackInfo[stack_index].MemUsed = StackInfo[stack_index].Size - index;
  }
}

void __stack_chk_fail(void)
{
  Stack.Overrun = 1U; /* TRUE */
  Stack.OverrunProgramCounter = (uint32_t) __builtin_return_address(0);
}
