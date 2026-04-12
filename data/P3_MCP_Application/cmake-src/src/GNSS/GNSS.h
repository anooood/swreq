#ifndef GNSS_H
#define GNSS_H
//-----------------------------------------------------------------------------------------
#include "UBX.h"
#include "GNSS_Setup.h"
#include "GNSS_Result.h"
//-----------------------------------------------------------------------------------------
//
//  The functions below are called at elevated priority (get/set)
//
//-----------------------------------------------------------------------------------------
extern uint16_T GNSS_Version(void);
//-----------------------------------------------------------------------------------------
extern void GNSS_Initialize(void);
extern void GNSS_ChangeSetup(T_GNSS_Setup* Setup);
//-----------------------------------------------------------------------------------------
extern void GNSS_OnUbxTimTp (real_T SystemTime, T_UBX_TIM_TP* UBX_TIM_TP);
extern void GNSS_OnUbxNavSol(T_UBX_NAV_SOL* UBX_NAV_SOL                 );
//-----------------------------------------------------------------------------------------
extern void GNSS_OnPPS(real_T SystemTime);
//-----------------------------------------------------------------------------------------
extern void GNSS_GetResult(T_GNSS_Result* Result);
//-----------------------------------------------------------------------------------------
//
//  The required accuracy limits on PVT solutions are accessed by these variables
//
//-----------------------------------------------------------------------------------------
extern const real_T LIMIT_pAcc;
extern const real_T LIMIT_sAcc;
//-----------------------------------------------------------------------------------------
#endif
