#include "Autopilot_300Hz_Function_1D.h"
//---------------------------------------------------------------------------
static RT_MODEL Autopilot_300Hz_Function_1D_M_;
static RT_MODEL *const Autopilot_300Hz_Function_1D_M = &Autopilot_300Hz_Function_1D_M_;
//---------------------------------------------------------------------------
void Autopilot_300Hz_initialize(void)
{
  Autopilot_300Hz_Function_1D_initialize(Autopilot_300Hz_Function_1D_M);
}
//---------------------------------------------------------------------------
void Autopilot_300Hz_main(T_NAV_InertialMeasurementUnitAutopilot*  IMU300Hzin,
                          T_NAV_NavigationState*                   navstatein,
                          T_GUID_Guid_Out_IF1003*                  guidin,
                          T_Weapon_MCP_DATA_Autop*                 DATAautop,
                          T_Weapon_MCP_Out_IF103*                  deflcmd,
                          T_GUID_Autop_Tel300Hz*                   autoptel_300Hz,
                          T_GUID_Autop_Tel20Hz*                    autoptel_20Hz)
{
  Autopilot_300Hz_Function_main(Autopilot_300Hz_Function_1D_M, IMU300Hzin, navstatein, guidin, DATAautop,
                                deflcmd, autoptel_300Hz, autoptel_20Hz);
}
//---------------------------------------------------------------------------
