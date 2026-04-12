#include "DataControl_10Hz_Function.h"
//---------------------------------------------------------------------------
static RT_MODEL DataControl_10Hz_Function_M_;
static RT_MODEL *const DataControl_10Hz_Function_M = &DataControl_10Hz_Function_M_;
//-------------------------------------------------------------------------------
void DataControl_10Hz_initialize(void)
{
  DataControl_10Hz_Function_initialize(DataControl_10Hz_Function_M);
}
//-------------------------------------------------------------------------------
void DataControl_10Hz_main(T_GUID_Guid2DATA*     Guidin,
                           T_GUID_WeaponConfig*  WeaCfgin,
                           T_GUID_TestConfig*    TestCfgin,
                           T_GUID_Discretes*     Discretesin,
                           uint8_T               calcLAR,
                           T_Weapon_MCP_DATA*    MCPdata,
                           T_Weapon_MCP_LAR*     LARout)
{
  DataControl_10Hz_Function_main(DataControl_10Hz_Function_M, Guidin, WeaCfgin,
						          TestCfgin, Discretesin, calcLAR, MCPdata, LARout);
}
//-------------------------------------------------------------------------------
