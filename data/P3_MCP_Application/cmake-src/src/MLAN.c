/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MLAN.c>
 *
 *  Description:
 *  ------------
 *  MLAN Module provides initialization, message transmission, reception servicing, and
 *  network status utilities
 *
 *  Function(s):
 *  ------------
 *  - MLAN_Init            : Initializes the MLAN communication interface.
 *  - MLAN_Service         : Services and processes received MLAN messages.
 *  - MLAN_Send            : Transmits a message over the MLAN to a specified node.
 *  - MLAN_IsNodeOnNet     : Checks whether a given node ID is currently active on the MLAN.
 *  - MLAN_Disable         : Disables the MLAN interface and stops message handling.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "FPGA.h"
#include "ScheduleControl.h"
#include "Arcnet.h"
#include "Interrupts.h"
#include "ETE.h"
#include "Timer.h"
#include "UART.h"
#include "MLAN.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "WiFi.h"
#include "WIAC82_Comms.h"
#include "WIAC82.h"
#include "SAL_Seeker.h"
#include "Servos.h"
#include "WeaponSettings.h"
#include "AircraftInterface.h"


sMLAN MLAN;

static unsigned char MLAN_RxMutex;
static sArcnet MLAN_Arcnet;
sMLAN_StateSpace MLAN_SS;

void MLAN_Init(char *AuxStr)
{
  MLAN_RxMutex = 0;

  ArcnetInit(&MLAN_Arcnet, FPGA_ARCNET_1_ADDR, intARCNET_1,  MLAN_MCP_NODE_ID, FALSE, TRUE);

  if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.MLANControls.AuxOn 			= 1;
		AUXCTRL_SS.MLANControls.SchedulerAuxOn 	= 0;
		AUXCTRL_SS.MLANControls.RxAuxOn			= 0;
		AUXCTRL_SS.MLANControls.RxDetailAuxOn	= 0;
		AUXCTRL_SS.MLANControls.TxAuxOn			= 0;
		AUXCTRL_SS.MLANControls.TxDetailAuxOn	= 0;
		AUXCTRL_SS.MLANControls.WiFi_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.ETE_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.HILS_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.DTAC_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.Servo_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.Tel_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.ITU_AuxOn		= 0;
		AUXCTRL_SS.MLANControls.ErrDetectOn		= 1;
  }
	//Counter Initialization
	MLAN_SS.Counters.TxCount			= 0;
	MLAN_SS.Counters.TxErrCount			= 0;
	MLAN_SS.Counters.RxCount			= 0;
	MLAN_SS.Counters.RxErrCount			= 0;
	MLAN_SS.Counters.WiFiRxCount		= 0;
	MLAN_SS.Counters.ETE_RxCount		= 0;
	MLAN_SS.Counters.HILS_RxCount		= 0;
	MLAN_SS.Counters.DTAC_RxCount		= 0;
	MLAN_SS.Counters.ServoRxCount		= 0;
	MLAN_SS.Counters.TelRxCount			= 0;
	MLAN_SS.Counters.ITURxCount			= 0;

  if (AUXCTRL_SS.MLANControls.AuxOn) {
		sprintf(AuxStr,"%s%u:MLAN Initialized\n",AuxStr,GetTimeUs());
		if (AUXCTRL_SS.MLANControls.SchedulerAuxOn){
			sprintf(AuxStr,"%s MLAN Node ID=%d 0x%x\n",AuxStr,MLAN_MCP_NODE_ID,MLAN_MCP_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_MCP_NODE_ID          =%d 0x%x\n",AuxStr,MLAN_MCP_NODE_ID,MLAN_MCP_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_DATA_LOGGER_NODE_ID  =%d 0x%x\n",AuxStr,MLAN_DATA_LOGGER_NODE_ID,MLAN_DATA_LOGGER_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_INTERFACE_NODE_ID    =%d 0x%x\n",AuxStr,MLAN_INTERFACE_NODE_ID,MLAN_INTERFACE_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_PITOT_NODE_ID        =%d 0x%x\n",AuxStr,MLAN_PITOT_NODE_ID,MLAN_PITOT_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_WIFI_NODE_ID         =%d 0x%x\n",AuxStr,MLAN_WIFI_NODE_ID,MLAN_WIFI_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_ETE_NODE_ID          =%d 0x%x\n",AuxStr,MLAN_ETE_NODE_ID,MLAN_ETE_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_ITU_NODE_ID          =%d 0x%x\n",AuxStr,MLAN_ITU_NODE_ID,MLAN_ITU_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_SAL_SEEKER_NODE_ID   =%d 0x%x\n",AuxStr,MLAN_SAL_SEEKER_NODE_ID,MLAN_SAL_SEEKER_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_HILS_PC_NODE_ID      =%d 0x%x\n",AuxStr,MLAN_HILS_PC_NODE_ID,MLAN_HILS_PC_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_DTAC_NODE_ID         =%d 0x%x\n",AuxStr,MLAN_DTAC_NODE_ID,MLAN_DTAC_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_IIR_SEEKER_NODE_ID   =%d 0x%x\n",AuxStr,MLAN_IIR_SEEKER_NODE_ID,MLAN_IIR_SEEKER_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_SERVO_NODE_ID        =%d 0x%x\n",AuxStr,MLAN_SERVO_NODE_ID,MLAN_SERVO_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_TEL_NODE_ID          =%d 0x%x\n",AuxStr,MLAN_TEL_NODE_ID,MLAN_TEL_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_HOBS_NODE_ID         =%d 0x%x\n",AuxStr,MLAN_HOBS_NODE_ID,MLAN_HOBS_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_SPARE2_NODE_ID       =%d 0x%x\n",AuxStr,MLAN_SPARE2_NODE_ID,MLAN_SPARE2_NODE_ID);
			sprintf(AuxStr,"%s  MLAN_LOADER_PC_NODE_ID    =%d 0x%x\n",AuxStr,MLAN_LOADER_PC_NODE_ID,MLAN_LOADER_PC_NODE_ID);
		}
  }

}

void MLAN_Service(void)
{
  unsigned int timestamp;
  unsigned char from_node_id;
  unsigned short length;
  unsigned char rx_data[ARCNET_MAX_DATA_LENGTH];
  sMLAN_Header *MLAN_Header;

  if (MLAN_RxMutex) {
    return;
  }

  MLAN_RxMutex = 1;

  while ((length = ArcnetRead(&MLAN_Arcnet, &timestamp, &from_node_id, rx_data)) > 0) {
	MLAN_Header = (sMLAN_Header *)rx_data;

	if (AUXCTRL_SS.MLANControls.RxAuxOn) {
		AUXsprintf("%u:MLAN ARCNET Message Received from Node=[%d 0x%x] Opcode=[%d 0x%x]\n",GetTimeUs(),from_node_id,from_node_id,MLAN_Header->Opcode,MLAN_Header->Opcode);
	}
	MLAN_SS.Counters.RxCount++;
    switch (from_node_id) {
      case MLAN_WIFI_NODE_ID :
    	  WiFiHandleRxData(rx_data, length);
    	  MLAN_SS.Counters.WiFiRxCount++;
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.WiFi_AuxOn)) {
    		  AUXsprintf("%u:MLAN ARCNET Message Received from WIFI Length=%d Opcode=[%d 0x%x] Count=%d\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode,MLAN_SS.Counters.WiFiRxCount);
    	  }
    	  break;

      case MLAN_ETE_NODE_ID : {
       	  TestEquipmentHandleRxData(rx_data, length, from_node_id);
       	  MLAN_SS.Counters.ETE_RxCount++;
  	  	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.ETE_AuxOn)) {
    	  		  AUXsprintf("%u:MLAN ARCNET Message Received from ETE Length=%d Opcode=[%d 0x%x] Count=%d\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode,MLAN_SS.Counters.ETE_RxCount);
    	  	  }
      	  }
    	  break;
      case MLAN_SAL_SEEKER_NODE_ID :
    	  SAL_SeekerHandleRxData(rx_data);
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.Seeker_AuxOn)) {
    		  AUXsprintf("%u:MLAN PC ARCNET Message Received from SEEKER of length =%d Opcode=[%d 0x%x]\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode);
    	  }
   	  break;
      case MLAN_HILS_PC_NODE_ID :
    	  TestEquipmentHandleRxData(rx_data, length, from_node_id);
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.HILS_AuxOn)) {
    		  AUXsprintf("%u:MLAN PC ARCNET Message Received from HILS of length =%d Opcode=[%d 0x%x]\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode);
    	  }
    	  break;
      case MLAN_DTAC_NODE_ID :
    	  AircraftHandleRxData(rx_data, timestamp);
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.DTAC_AuxOn)) {
    		  AUXsprintf("%u:MLAN ARCNET Message Received from DTAC of length =%d Opcode=[%d 0x%x]\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode);
    	  }
    	  break;
      case MLAN_SERVO_NODE_ID :
    	  ServoHandleRxData(rx_data);
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.Servo_AuxOn)) {
    		  AUXsprintf("%u:MLAN ARCNET Message Received from SERVO of length =%d Opcode=[%d 0x%x]\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode);
    	  }
    	  break;
      case MLAN_ITU_NODE_ID :
    	  WIAC82HandleRxData(rx_data, timestamp);
    	  if ((AUXCTRL_SS.MLANControls.RxAuxOn)||(AUXCTRL_SS.MLANControls.ITU_AuxOn)) {
    		  AUXsprintf("%u:MLAN ARCNET Message Received from ITU (WIAC82) of length =%d Opcode=[%d 0x%x]\n",GetTimeUs(),length,MLAN_Header->Opcode,MLAN_Header->Opcode);
    	  }
    	  break;
//      case MLAN_HOBS_NODE_ID : break;
      default :
    	  MLAN_SS.Counters.RxErrCount++;
    	  if (AUXCTRL_SS.MLANControls.ErrDetectOn) {
  				AUXsprintf("%u:ERROR MLAN ARCNET RX_ERR: FROM [%d:0x%x] LEN=%d\n",GetTimeUs(),from_node_id,from_node_id,length);
    	  }
    	  break;
    }
  };

  ArcnetBuildMap(&MLAN_Arcnet);

  MLAN.ReconCnt = MLAN_Arcnet.ReconCnt;
  MLAN.MyReconCnt = MLAN_Arcnet.MyReconCnt;
  MLAN.NodesOnNetwork = MLAN_Arcnet.NodesOnNetwork;

  if (AUXCTRL_SS.MLANControls.SchedulerAuxOn) {
	  AUXsprintf("%u:Servicing MLAN ", GetTimeUs());
	  AUXsprintf(" ARCNET ReconCnt=%d ", MLAN.ReconCnt);
	  AUXsprintf(" MyReconCnt=%d ",MLAN.MyReconCnt);
	  AUXsprintf(" Nodes in Network=0x%x\n",MLAN.NodesOnNetwork);
  }

  MLAN_RxMutex = 0;
}

void MLAN_Send(unsigned char ToNodeId, unsigned char *TxData, unsigned int Length)
{
	eArcnetErrors ArcnetError;
	ArcnetError =  ArcnetWrite(&MLAN_Arcnet, ToNodeId, TxData, Length);
	if (ArcnetError) {
		MLAN_SS.Counters.TxErrCount++;
		if (AUXCTRL_SS.MLANControls.ErrDetectOn) {
			if (ArcnetError==arcTX_ERR) {
				AUXsprintf("%u:ERROR MLAN ARCNET TX_ERR: TO [%d:0x%x] LEN=%d\n",GetTimeUs(),ToNodeId,ToNodeId,Length);
			}
			else if (ArcnetError==arcDATA_SIZE_ERR){
				AUXsprintf("%u:ERROR MLAN DATA SIZE: TO [%d:0x%x] LEN=%d\n",GetTimeUs(),ToNodeId,ToNodeId,Length);
			}
			else if (ArcnetError==arcMUTEX_LOCKED){
				AUXsprintf("%u:ERROR MLAN MUTEX_LOCKED: TO [%d:0x%x] LEN=%d\n",GetTimeUs(),ToNodeId,ToNodeId,Length);
			}
			else if (ArcnetError==arcQUEUE_OVERFLOW){
				AUXsprintf("%u:ERROR MLAN QUEUE OVERFLOW: TO [%d:0x%x] LEN=%d\n",GetTimeUs(),ToNodeId,ToNodeId,Length);
			}
		}
	}
	else {
		MLAN_SS.Counters.TxCount++;
	}
  if (AUXCTRL_SS.MLANControls.TxAuxOn) {
	  if ((ToNodeId==MLAN_WIFI_NODE_ID)&&(AUXCTRL_SS.MLANControls.WiFi_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to WiFi:",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_ETE_NODE_ID)&&(AUXCTRL_SS.MLANControls.ETE_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to ETE:\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_HILS_PC_NODE_ID)&&(AUXCTRL_SS.MLANControls.HILS_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to HILS PC:\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_DTAC_NODE_ID)&&(AUXCTRL_SS.MLANControls.DTAC_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to DTAC:\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_SERVO_NODE_ID)&&(AUXCTRL_SS.MLANControls.Servo_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to SERVO:\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_TEL_NODE_ID)&&(AUXCTRL_SS.MLANControls.Tel_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to TELEMETRY:\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
	  else if ((ToNodeId==MLAN_ITU_NODE_ID)&&(AUXCTRL_SS.MLANControls.ITU_AuxOn)) {
		  AUXsprintf("%u:MLAN ARCNET Message sent to ITU (WIAC82):\n",GetTimeUs());
		  AUXsprintf(" From Node=%d 0x%x",MLAN_Arcnet.OwnNodeId,MLAN_Arcnet.OwnNodeId);
		  AUXsprintf(" To node=%d 0x%x", ToNodeId,ToNodeId);
		  AUXsprintf(" Length=%d\n",Length);
	  }
  }
}

unsigned char MLAN_IsNodeOnNet(unsigned char NodeID)
{
  if (NodeID < 240) {
    return FALSE;
  }

  if ((MLAN_Arcnet.NodesOnNetwork & (1 << (NodeID - 240))) != 0) {
    return TRUE;
  }

  return FALSE;
}

void MLAN_Disable(void)
{
  ArcnetDisable(&MLAN_Arcnet);
  if (AUXCTRL_SS.MLANControls.ETE_AuxOn) {
	  AUXsprintf("%u:MLAN ARCNET disable command received\n",GetTimeUs());
  }

}

