/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Fuze.c>
 *
 *  Description:
 *  ------------
 *  Interface module for communication and control between the Fuze and the P3 Weapon.
 *
 *  Function(s):
 *  ------------
 *  - FuzeInit:                    Initializes the fuze interface
 *  - FuzeSendRequestStatusMsg:    Sends a request for fuze status message to the fuze
 *  - FuseFMU152_CheckDelay:       Check delay for FMU152
 *  - FuzeSetDelay:                Sends new fuze settings to the fuze
 *  - FuzeHandleStatusMsg:         Handle new fuze status data
 *  - FuzeHandleRxData:            Handle new fuze status data
 *  - FuzeService:                 Services fuze interface
 *  ...
 *
 *
 ******************************************************************************/

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include "TypeDefines.h"
#include "Timer.h"
#include "UART.h"
#include "FIFO.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Fuze.h"
#include "WeaponSettings.h"
#include "Telemetry.h"
#include "MLAN.h"
#include "ETE.h"

sFuze_StateSpace SS_FUZE;

void FuzeInit(char *newAuxStr)
{
	SS_FUZE.CommsControls.FuzeResultDestNodeId = 0;
	SS_FUZE.CommsControls.FuzeExpectedCmdRxCnt = 0;
	SS_FUZE.CommsControls.FuzeFrameCnt = 0;

	memset(&SS_FUZE.Fuze, 0, sizeof(sFuzeInfo));
	memset(&SS_FUZE.FuzeSettings, 0, sizeof(sFuzeSettings));

	SS_FUZE.Fuze.InitComplete = FALSE;

	if (WeaponSettings.DataFileValid == 0U) {
		AUXCTRL_SS.FuzeControls.AuxOn 			 = 1U;
		AUXCTRL_SS.FuzeControls.SchedulerAuxOn	 = 0U;
		AUXCTRL_SS.FuzeControls.RxAuxOn			 = 0U;
		AUXCTRL_SS.FuzeControls.RxDetailAuxOn	 = 0U;
		AUXCTRL_SS.FuzeControls.TxAuxOn			 = 0U;
		AUXCTRL_SS.FuzeControls.TxDetailAuxOn	 = 0U;
		AUXCTRL_SS.FuzeControls.ErrDetectOn		 = 1U;
		AUXCTRL_SS.FuzeControls.ExtRxOn			 = 1U;
	}

	SS_FUZE.Fuze.RequestStatusTxCnt     = 0;
	SS_FUZE.Fuze.SetDelayCmdCnt         = 0;

	if (AUXCTRL_SS.FuzeControls.AuxOn != 0U) {
		sprintf(newAuxStr,"%s%u:Fuze Parameters Initialized\n",newAuxStr,GetTimeUs());
	}
}

void FuzeStatusMsgSendRequest(void)
{
	IF_SendMsg(FUZE_STATUS_REQ_ID, NULL, 0);
	SS_FUZE.Fuze.RequestStatusTxCnt++;

	if (AUXCTRL_SS.FuzeControls.TxAuxOn != 0U){
		AUXsprintf("%u:FUZE Request Status Command Sent %d\n",GetTimeUs(),SS_FUZE.Fuze.RequestStatusTxCnt);
	}

}

uint8_t FuzeFMU152_CheckDelay(uint8_t checkDelay) {
	/* FMU152 valid delays[ms]: */
	/* 0, 5, 15, 25, 35, 45, 60, 90, 180, 240 */
	uint8_t FilteredDelay;

	if (checkDelay < FUZE_FMU152_VALUE_3) {
		FilteredDelay = 0;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_10) {
		FilteredDelay = FUZE_FMU152_VALUE_5;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_20) {
		FilteredDelay = FUZE_FMU152_VALUE_15;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_30) {
		FilteredDelay = FUZE_FMU152_VALUE_25;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_40) {
		FilteredDelay = FUZE_FMU152_VALUE_35;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_50) {
		FilteredDelay = FUZE_FMU152_VALUE_45;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_75) {
		FilteredDelay = FUZE_FMU152_VALUE_60;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_135) {
		FilteredDelay = FUZE_FMU152_VALUE_90;
	}
	else if (checkDelay < FUZE_FMU152_VALUE_210) {
		FilteredDelay = FUZE_FMU152_VALUE_180;
	}
	else {
		FilteredDelay = FUZE_FMU152_VALUE_240;
	}
	return(FilteredDelay);
}

uint8_t Fuze_SetDelay(uint8_t newFuzeDelay, uint8_t FromNodeId)
{
  uint8_t result = 0U;

  if (AUXCTRL_SS.FuzeControls.ExtRxOn != 0U) {
	  AUXsprintf("%d:FUZE Set Delay=%d Command Received from node=%d[0x%x] Count=%d",GetTimeUs(),newFuzeDelay,FromNodeId,FromNodeId,SS_FUZE.Fuze.SetDelayTxCnt);
  }

  /* Fuze settings can only be sent once it is confirmed that the fuze initialization is complete, and that the fuze can accept new settings */
  if ((SS_FUZE.Fuze.Status.Bits.InitComplete== 1U) && (SS_FUZE.Fuze.Status.Bits.CommsOk == 1U)) {
    if ((SS_FUZE.Fuze.Type != (uint8_t)fuzeMECHANICAL) && (SS_FUZE.Fuze.Mode != (uint8_t)fmNON_PROGRAMMABLE) && (SS_FUZE.Fuze.Mode != (uint8_t)fmHEIGHT) && (SS_FUZE.Fuze.FuzeTimeDelay != newFuzeDelay)) {
    	SS_FUZE.Fuze.Updating = 1U; /* TRUE */
    	SS_FUZE.Fuze.UpdateSuccessful = 0U; /* FALSE */

      /* The FMU does not support a fuze delay of 10ms, therefore the delay is set at 15ms */
      if (SS_FUZE.Fuze.Type == (uint8_t)fuzeFMU) {
    	  SS_FUZE.FuzeSettings.FuzeDelay = FuzeFMU152_CheckDelay(newFuzeDelay);
      } else {
    	  SS_FUZE.FuzeSettings.FuzeDelay = newFuzeDelay;
      }

      IF_SendMsg(FUZE_SETTINGS_ID, (uint8_t *) &SS_FUZE.FuzeSettings, sizeof(SS_FUZE.FuzeSettings));

      SS_FUZE.CommsControls.FuzeExpectedCmdRxCnt = SS_FUZE.Fuze.SetDelayCmdCnt + 1U;

      SS_FUZE.Fuze.SetDelayTxCnt++;

      SS_FUZE.CommsControls.FuzeResultDestNodeId = FromNodeId;

      result = 1U; /* TRUE */

      if (AUXCTRL_SS.FuzeControls.ExtRxOn != 0U){
  		AUXsprintf("\n%d:FUZE Set Delay Command Received %d",GetTimeUs(),SS_FUZE.Fuze.SetDelayTxCnt);
  		AUXsprintf(" Delay= %d",SS_FUZE.FuzeSettings.FuzeDelay);
  		AUXsprintf(" From Node ID= %d\n",SS_FUZE.CommsControls.FuzeResultDestNodeId);
      }

    }
  }
  if (AUXCTRL_SS.FuzeControls.ExtRxOn != 0U) {
	  AUXsprintf(" Result=%d\n",result);
  }

  return result;
}

void Fuze_HandleStatusMsg(const sFuzeStatusMsg *StatusMsg)
{
	SS_FUZE.Fuze.StatusRxCnt++;

	SS_FUZE.Fuze.SW_Verion = StatusMsg->FuzeInfo.SW_Verion;
	SS_FUZE.Fuze.Status = StatusMsg->FuzeInfo.Status;
	SS_FUZE.Fuze.SetDelayCmdCnt = StatusMsg->FuzeInfo.SetDelayCmdCnt;

	if (AUXCTRL_SS.FuzeControls.RxDetailAuxOn != 0U) {
		AUXsprintf("%u:FUZE Status Message number %d received\n", GetTimeUs(),SS_FUZE.Fuze.StatusRxCnt);
		if (AUXCTRL_SS.FuzeControls.RxDetailAuxOn != 0U) {
			AUXsprintf("  Software Version=%d\n", SS_FUZE.Fuze.SW_Verion);
			AUXsprintf("  Status=0x%x\n", SS_FUZE.Fuze.Status.Word);
			AUXsprintf("   InitComplete=%d\n", SS_FUZE.Fuze.Status.Bits.InitComplete);
			AUXsprintf("   CommsOk=%d\n", SS_FUZE.Fuze.Status.Bits.CommsOk);
			AUXsprintf("   Updating Delay=%d\n", SS_FUZE.Fuze.Status.Bits.UpdatingDelay);
			AUXsprintf("   Fuze Delay Not As Req=%d\n", SS_FUZE.Fuze.Status.Bits.FuzeDelayNotAsReq);
			AUXsprintf("   Fuze 5V On=%d\n", SS_FUZE.Fuze.Status.Bits.Fuze5V_On);
			AUXsprintf("   Fuze 15V On=%d\n", SS_FUZE.Fuze.Status.Bits.Fuze15V_On);
			AUXsprintf("   Fuze24V On=%d\n", SS_FUZE.Fuze.Status.Bits.Fuze24V_On);
			AUXsprintf("   Fuze Arm On=%d\n", SS_FUZE.Fuze.Status.Bits.FuzeArmOn);
			AUXsprintf("   Fuze Id=%d\n", SS_FUZE.Fuze.Status.Bits.FuzeId);
			AUXsprintf("   Spare=%d\n", SS_FUZE.Fuze.Status.Bits.Spare);
			AUXsprintf("  Set Delay Count=%d\n", SS_FUZE.Fuze.SetDelayCmdCnt);
		}
	}

	/* Update fuze status if fuze initialization is complete */
	if (SS_FUZE.Fuze.Status.Bits.InitComplete  == 1U) {
		SS_FUZE.Fuze.InitComplete = 1U; /* TRUE */
		/* Update fuze status if settings are valid */
		if ((StatusMsg->FuzeInfo.Type < (uint8_t)fuzeLAST) && (StatusMsg->FuzeInfo.Mode < (uint8_t)fmLAST)) {
			SS_FUZE.Fuze.Type = StatusMsg->FuzeInfo.Type;
			SS_FUZE.Fuze.Mode = (SS_FUZE.Fuze.Status.Bits.CommsOk == 1U) ? StatusMsg->FuzeInfo.Mode : (uint8_t)fmNON_PROGRAMMABLE;
			SS_FUZE.Fuze.FuzeTimeDelay = StatusMsg->FuzeInfo.DelayInfo;

			if (AUXCTRL_SS.FuzeControls.RxDetailAuxOn != 0U) {
				AUXsprintf(" Type=0x%x\n", SS_FUZE.Fuze.Type);
				AUXsprintf(" Mode=%d\n", SS_FUZE.Fuze.Mode);
				AUXsprintf(" Delay=%dms\n", SS_FUZE.Fuze.FuzeTimeDelay);
			}
			TelemetryBuild2HzBlock1();
		}
	}

	if (SS_FUZE.Fuze.Updating == 1U) {
		if (SS_FUZE.CommsControls.FuzeExpectedCmdRxCnt != SS_FUZE.Fuze.SetDelayCmdCnt) {
    		SS_FUZE.CommsControls.FuzeExpectedCmdRxCnt = SS_FUZE.Fuze.SetDelayCmdCnt + 1U;

    		/* Retransmit the fuze settings to the IF as the previous command was not received */
    		IF_SendMsg(FUZE_SETTINGS_ID, (uint8_t *) &SS_FUZE.FuzeSettings, sizeof(SS_FUZE.FuzeSettings));

    		if (AUXCTRL_SS.FuzeControls.TxAuxOn != 0U) {
    			AUXsprintf("%u:FUZE Retransmit Settings Command Sent \n",GetTimeUs());
    		}
    	}

    	if (SS_FUZE.Fuze.Status.Bits.UpdatingDelay == 0U) {
    		SS_FUZE.Fuze.Updating = 0U; /* FALSE */
    	}
	}
}

void FuzeHandleRxData(uint8_t *RxData)
{
  sIF_Header *header;

  header = (sIF_Header *) RxData;

  if (AUXCTRL_SS.FuzeControls.RxAuxOn != 0U) {
	  AUXsprintf("%u:FUZE Message ID=0x%x Length=%d received from Fuse Card\n",GetTimeUs(),header->MsgId,header->DataSize);
  }

  switch (header->MsgId) {
    case FUZE_STATUS_ID : Fuze_HandleStatusMsg((sFuzeStatusMsg *) RxData); break;
    case FUZE_ACK_ID : {
    	/* Fuze settings update successful */
    	SS_FUZE.Fuze.UpdateSuccessful = 1U; /* TRUE */
    	SS_FUZE.Fuze.ACK_RxCnt++;
    	if (AUXCTRL_SS.FuzeControls.RxAuxOn != 0U) {
    		AUXsprintf("%u:FUZE Settings update ACK %d (Successful)\n",GetTimeUs(),SS_FUZE.Fuze.ACK_RxCnt);
    	}

    	if (SS_FUZE.CommsControls.FuzeResultDestNodeId == MLAN_ETE_NODE_ID) {
    		TestEquipmentSendAcknowledgeMessage(MLAN_ETE_NODE_ID, TRUE);
    	}
    } break;
    case FUZE_NACK_ID : {
    	/* Fuze settings update unsuccessful */
    	SS_FUZE.Fuze.UpdateSuccessful = 0U; /* FALSE */
    	SS_FUZE.Fuze.NACK_RxCnt++;
    	if (AUXCTRL_SS.FuzeControls.RxAuxOn != 0U) {
    		AUXsprintf("%u:FUZE Settings update NACK %d (Unsuccessful)\n",GetTimeUs(),SS_FUZE.Fuze.NACK_RxCnt);
    	}

    	if (SS_FUZE.CommsControls.FuzeResultDestNodeId == MLAN_ETE_NODE_ID) {
    		TestEquipmentSendAcknowledgeMessage(MLAN_ETE_NODE_ID, FALSE);
    	}
    } break;
    default : break;
  }
}

void Fuze_Service(void)
{
	if (AUXCTRL_SS.MAINControls.FuzeCardFittedOn == 1U) {
		SS_FUZE.ServiceControls.SystemTime = GetTimeUs();
		SS_FUZE.ServiceControls.TimeDiff = SS_FUZE.ServiceControls.SystemTime - SS_FUZE.ServiceControls.OldSystemTime;
		if (SS_FUZE.ServiceControls.TimeDiff > FUZE_SERVICE_INTERVAL) {
			SS_FUZE.ServiceControls.OldSystemTime = SS_FUZE.ServiceControls.SystemTime;
			if (AUXCTRL_SS.FuzeControls.SchedulerAuxOn != 0U) {
				AUXsprintf("%u:Servicing FUZE - Status is: ",GetTimeUs());
				AUXsprintf(" Updating = %d ",SS_FUZE.Fuze.Updating);
				AUXsprintf(" Updating Successful= %d\n",SS_FUZE.Fuze.UpdateSuccessful);
			}

			if (SS_FUZE.Fuze.InitComplete == 0U) {
				FuzeStatusMsgSendRequest();
				if (SS_FUZE.Fuze.RequestStatusTxCnt < FUZE_TIMEOUT_COUNT) {
					FuzeStatusMsgSendRequest();
				}
				else {
					SS_FUZE.Fuze.InitComplete = 1U; /* TRUE */
					if (AUXCTRL_SS.FuzeControls.ErrDetectOn != 0U) {
						AUXsprintf("\n%u:ERROR FUZE-non-response to status request message after %d sends Fuze Mode=%d\n",GetTimeUs(),FUZE_TIMEOUT_COUNT,SS_FUZE.Fuze.Mode);
					}

				}
			}
			else if (SS_FUZE.Fuze.Updating == 1U) {
				FuzeStatusMsgSendRequest();
			}
			else {
			    /* No action needed for other cases */
			}
		}
	}
}
