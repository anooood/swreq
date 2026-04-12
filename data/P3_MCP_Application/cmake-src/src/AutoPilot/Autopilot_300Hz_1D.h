#ifndef AUTOPILOT_300HZ_1D_H
#define AUTOPILOT_300HZ_1D_H
//-------------------------------------------------------------------------------
#ifndef Autopilot_300Hz_Function_1D_COMMON_INCLUDES_
# define Autopilot_300Hz_Function_1D_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif
//-------------------------------------------------------------------------------
#include "MCP_Interface.h"
#include "Nav100Hz_Output.h"
#include "DataControl_Output.h"
#include "GUID_Output.h"
#include "AUTOP_Output.h"
//-------------------------------------------------------------------------------
//  The functions below are called at elevated priority (get/set)
//-------------------------------------------------------------------------------
extern void Autopilot_300Hz_initialize(void);
//---------------------------------------------------------------------------
extern void Autopilot_300Hz_main(T_NAV_InertialMeasurementUnitAutopilot*  IMU300Hzin,
                                 T_NAV_NavigationState*                   navstatein,
                                 T_GUID_Guid_Out_IF1003*                  guidin,
                                 T_Weapon_MCP_DATA_Autop*                 DATAautop,
                                 T_Weapon_MCP_Out_IF103*                  deflcmd,
                                 T_GUID_Autop_Tel300Hz*                   autoptel_300Hz,
                                 T_GUID_Autop_Tel20Hz*                    autoptel_20Hz);
//---------------------------------------------------------------------------
#endif
