/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <ETE.h>
 *
 *  Description:
 *  ------------
 *  Header file for ETE Module.
 *
 ******************************************************************************/

#ifndef ETE_H
#define ETE_H

void TestEquipmentInit(void);
void TestEquipmentHandleRxData(unsigned char *RxData, unsigned int DataLength, unsigned char FromNodeId);
void TestEquipmentSendAcknowledgeMessage(unsigned char ToNodeId, unsigned char Result);

#endif
