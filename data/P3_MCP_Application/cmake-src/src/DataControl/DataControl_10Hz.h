#ifndef DataControl_10HZ_H
#define DataControl_10HZ_H
//-------------------------------------------------------------------------------
#ifndef DataControl_10Hz_Function_COMMON_INCLUDES_
# define DataControl_10Hz_Function_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif
//-------------------------------------------------------------------------------
#include "MCP_Interface.h"
#include "GUID_Output.h"
#include "DataControl_Output.h"
#include "commontypes.h"
//-------------------------------------------------------------------------------
//   Configuration File Version and Checksums at the time of this Compilation
//-------------------------------------------------------------------------------
// SEJEEL:
// Sejeel_Mk81 Weapon Configuration Ver No & CheckSum:       1  FF8B8E44
// Sejeel_Mk82 Weapon Configuration Ver No & CheckSum:       1  F80F527E
// Sejeel  MCP Config File Ver No & CheckSum:             2.00  FD4E6E68
// AL-TARIQ:
// AlTariq_Mk81S Weapon Configuration Ver No & CheckSum:     3  F9D400A3
// AlTariq_Mk82S Weapon Configuration Ver No & CheckSum:     2  FDC019A2
// AlTariq_Mk83S Weapon Configuration Ver No & CheckSum:     3  FAF47520
// AlTariq_Mk84S Weapon Configuration Ver No & CheckSum:     0  F8B1BBB1
// AlTariq_Mk81L Weapon Configuration Ver No & CheckSum:     3  F8593F87
// AlTariq_Mk82L Weapon Configuration Ver No & CheckSum:     3  F931B20F
// AlTariq_Mk83L Weapon Configuration Ver No & CheckSum:     0  F9053D56
// Al-Tariq  MCP Config File Ver No & CheckSum:           2.01  FAFCFE9D
//-------------------------------------------------------------------------------
//  The functions below are called at elevated priority (get/set)
//-------------------------------------------------------------------------------
extern void DataControl_10Hz_initialize(void);
//---------------------------------------------------------------------------------
extern void DataControl_10Hz_main(T_GUID_Guid2DATA*     Guidin,
                                  T_GUID_WeaponConfig*  WeaCfgin,
                                  T_GUID_TestConfig*    TestCfgin,
                                  T_GUID_Discretes*     Discretesin,
                                  uint8_T               calcLAR,
                                  T_Weapon_MCP_DATA*    MCPdata,
                                  T_Weapon_MCP_LAR*     LARout);
//---------------------------------------------------------------------------------
#endif
