/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Stack.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Stack module.
 *
 ******************************************************************************/

#ifndef STACK_H_
#define STACK_H_

#include <stdint.h>

#define MCP_STACK_CLEAR_BYTE    (uint8_t)(0xBB)
#define MCP_STACK_CHK_GUARD     (uint32_t)(0xDEADBEEFU)
#define MCP_STACK_FIRST_BYTES   (uint16_t)(0x100)
#define MCP_STACK_ADDR_1        (uint8_t)(1)
#define MCP_STACK_ADDR_2        (uint8_t)(2)
#define MCP_STACK_ADDR_3        (uint8_t)(3)
#define MCP_STACK_ADDR_4        (uint8_t)(4)
#define MCP_STACK_ADDR_5        (uint8_t)(5)
#define MCP_STACK_ADDR_6        (uint8_t)(6)
#define MCP_STACK_ADDR_7        (uint8_t)(7)

typedef enum
{
  stackNORMAL,
  stackIRQ,
  stackSUPERVISOR,
  stackABORT,
  stackFIQ,
  stackUNDEF,
  stackLAST
} eStackTypes;


typedef struct
{
  uint32_t Size;
  uint32_t MemUsed;
} sStackInfo;

typedef struct
{
  uint8_t Overrun;
  uint8_t Spare[MCP_STACK_ADDR_3];
  uint32_t OverrunProgramCounter;
  sStackInfo Info[stackLAST];
} sStack;

extern sStack Stack;

void StackInit(void);
void StackGetUssage(sStackInfo *StackInfo);
void __stack_chk_fail(void);

#endif /* STACK_H_ */
