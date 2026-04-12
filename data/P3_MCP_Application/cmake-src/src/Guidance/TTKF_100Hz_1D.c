#include "TTKF_100Hz_Function_1D.h"
//---------------------------------------------------------------------------
static RT_MODEL TTKF_100Hz_Function_1D_M_;
static RT_MODEL *const TTKF_100Hz_Function_1D_M = &TTKF_100Hz_Function_1D_M_;
//---------------------------------------------------------------------------
void TTKF_100Hz_initialize(void)
{
  TTKF_100Hz_Function_1D_initialize(TTKF_100Hz_Function_1D_M);
}
//---------------------------------------------------------------------------
void TTKF_100Hz_main(real_T                                  SysTimein,
                     T_GUID_Discretes*                       Discretesin,
                     T_Weapon_SALSeeker_IF102*               seekerin,
                     T_NAV_GenericInertialNavigationSensor*  IMU100Hzin,
                     uint8_T                                 navmodein,
                     T_NAV_NavigationState*                  navstatein,
                     T_GUID_MissionPlanning*                 MPin,
                     T_Weapon_MCP_DATA_TTKF*                 DATAttkf,
                     T_GUID_TTKF_Out_IF1002*                 ttkfout,
                     T_GUID_TTKF_Tel100Hz*                   ttkftel_100Hz)
{
  TTKF_100Hz_Function_main(TTKF_100Hz_Function_1D_M, SysTimein, Discretesin,
                           seekerin, IMU100Hzin, navmodein, navstatein, MPin, DATAttkf,
						   ttkfout, ttkftel_100Hz);
}
//---------------------------------------------------------------------------
