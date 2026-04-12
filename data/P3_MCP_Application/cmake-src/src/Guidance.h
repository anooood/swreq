/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Guidance.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Guidance module.
 *
 ******************************************************************************/

#ifndef GUIDANCE_H_
#define GUIDANCE_H_

#include "Autopilot_300Hz_1D.h"
#include "Guidance_100Hz_1D.h"
#include "TTKF_100Hz_1D.h"
#include "Mission.h"


extern unsigned int Guidance10HzCnt;
extern T_GUID_Autop_Tel300Hz AutoPilot300HzTelemetry;
extern T_GUID_Autop_Tel20Hz AutoPilot20HzTelemetry;
extern T_Weapon_MCP_LAR LAR;
extern T_GUID_GuidCtrlFlags GuidCtrl;
extern T_GUID_Guid_Out_IF1003 GuidanceOut;
extern T_GUID_Guid_Tel100Hz GuidTel100Hz;
extern T_GUID_Guid_Tel20HzA GuidTel20HzA;
extern T_GUID_Guid_Tel20HzB GuidTel20HzB;
extern T_GUID_TTKF_Tel100Hz TTKF_Telemetry;
extern T_Weapon_MCP_Out_IF103 ServoCmd;

void GuidanceInit(void);
void GuidanceSetMission(sMission *WeaponMission);
void RunAutoPilot(void);
void Guidance100Hz(void);
void Guidance10Hz(void);

#endif /* GUIDANCE_H_ */
