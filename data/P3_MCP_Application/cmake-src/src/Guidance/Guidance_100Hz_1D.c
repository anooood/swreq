#include "Guidance_100Hz_Function_1D.h"
//---------------------------------------------------------------------------
static RT_MODEL Guidance_100Hz_Function_1D_M_;
static RT_MODEL *const Guidance_100Hz_Function_1D_M = &Guidance_100Hz_Function_1D_M_;
//---------------------------------------------------------------------------
void Guidance_100Hz_initialize(void)
{
  Guidance_100Hz_Function_1D_initialize(Guidance_100Hz_Function_1D_M);
}
//---------------------------------------------------------------------------
void Guidance_100Hz_main(real_T                                 SysTimein,
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
                         T_GUID_Guid_Tel20HzB*                  guidtel_20HzB)
{
  Guidance_100Hz_Function_main(Guidance_100Hz_Function_1D_M, SysTimein, Discretesin, ttkfin,
                               navmodein, navstatein, IMUin, MPin, Windin, Pitotin, DATAguid,
							   WeaCfgin, TestCfgin,
                               guid2AP, guidCtrlFlags, guid2DATA,
                               guidtel_100Hz, guidtel_20HzA, guidtel_20HzB);
}
//---------------------------------------------------------------------------
