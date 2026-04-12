#ifndef GUIDANCE_100HZ_1D_H
#define GUIDANCE_100HZ_1D_H
//-------------------------------------------------------------------------------
#ifndef Guidance_100Hz_Function_1D_COMMON_INCLUDES_
# define Guidance_100Hz_Function_1D_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif
//-------------------------------------------------------------------------------
#include "MCP_Interface.h"
#include "Nav100Hz_Common.h"
#include "DataControl_Output.h"
#include "TTKF_Output.h"
#include "Nav100Hz_Output.h"
#include "Nav2Hz_Output.h"
#include "GUID_Output.h"
#include "commontypes.h"
//-------------------------------------------------------------------------------
//  The functions below are called at elevated priority (get/set)
//-------------------------------------------------------------------------------
extern void Guidance_100Hz_initialize(void);
//---------------------------------------------------------------------------
extern void Guidance_100Hz_main(real_T                                 SysTimein,
       								          T_GUID_Discretes*                      Discretesin,
                                T_GUID_TTKF_Out_IF1002*                ttkfin,
                                uint8_T                                navmodein,
                                T_NAV_NavigationState*                 navstatein,
                                T_NAV_GenericInertialNavigationSensor* IMUin,
                                T_GUID_MissionPlanning*                MPin,
                                T_GUID_WindParams*                     Windin,
                                T_GUID_FlightConditions*               Pitotin,
								T_Weapon_MCP_DATA_Guid*                DATAguid,
								T_GUID_WeaponConfig*                   WeaCfgin,
								T_GUID_TestConfig*                     TestCfgin,
                                T_GUID_Guid_Out_IF1003*                guid2AP,
                                T_GUID_GuidCtrlFlags*                  guidCtrlFlags,
                                T_GUID_Guid2DATA*                      guid2DATA,
                                T_GUID_Guid_Tel100Hz*                  guidtel_100Hz,
                                T_GUID_Guid_Tel20HzA*                  guidtel_20HzA,
                                T_GUID_Guid_Tel20HzB*                  guidtel_20HzB);
//---------------------------------------------------------------------------
#endif
