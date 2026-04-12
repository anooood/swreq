#ifndef TTKF_100HZ_1D_H
#define TTKF_100HZ_1D_H
//-------------------------------------------------------------------------------
#ifndef TTKF_100Hz_Function_1D_COMMON_INCLUDES_
# define TTKF_100Hz_Function_1D_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif
//-------------------------------------------------------------------------------
#include "MCP_Interface.h"
#include "DataControl_Output.h"
#include "Nav100Hz_Output.h"
#include "TTKF_Output.h"
//-------------------------------------------------------------------------------
//  The functions below are called at elevated priority (get/set)
//-------------------------------------------------------------------------------
extern void TTKF_100Hz_initialize(void);
//---------------------------------------------------------------------------
extern void TTKF_100Hz_main(real_T                                  SysTimein,
                            T_GUID_Discretes*                       Discretesin,
                            T_Weapon_SALSeeker_IF102*               seekerin,
                            T_NAV_GenericInertialNavigationSensor*  IMU100Hzin,
                            uint8_T                                 navmodein,
                            T_NAV_NavigationState*                  navstatein,
                            T_GUID_MissionPlanning*                 MPin,
                            T_Weapon_MCP_DATA_TTKF*                 DATAttkf,
                            T_GUID_TTKF_Out_IF1002*                 ttkfout,
                            T_GUID_TTKF_Tel100Hz*                   ttkftel_100Hz);
//---------------------------------------------------------------------------
#endif
