#include "GNSS.h"
#include <string.h>
//---------------------------------------------------------------------------------
static const uint16_T GNSS_VERSION = 107;  // 1.07
//---------------------------------------------------------------------------------
const real_T LIMIT_pAcc = 20.0;
const real_T LIMIT_sAcc =  2.0;
//---------------------------------------------------------------------------------
typedef struct
{
  T_GNSS_Setup  GNSS_Setup ;
  T_UBX_NAV_SOL UBX_NAV_SOL;
  // Valid Flags
  uint8_T TP_Valid;
  uint8_T TO_Valid;
  // Time Sync
  real_T TP_TOW      ;
  real_T TP_Timestamp;
  real_T TO_Offset   ;
}T_State;
//---------------------------------------------------------------------------------
static T_State State;
//---------------------------------------------------------------------------------
uint16_T GNSS_Version(void)
{
  return GNSS_VERSION;
}
//---------------------------------------------------------------------------------
void GNSS_Initialize(void)
{
  memset(&State, 0, sizeof(T_State));
  
  State.GNSS_Setup.SystemsToUse = 3;
}
//---------------------------------------------------------------------------------
void GNSS_ChangeSetup(T_GNSS_Setup* Setup)
{
  State.GNSS_Setup = *Setup;
}
//---------------------------------------------------------------------------------
void GNSS_OnUbxTimTp(real_T SystemTime, T_UBX_TIM_TP* UBX_TIM_TP)
{
  State.TP_Valid     = 1                       ;
  State.TP_TOW       = UBX_TIM_TP->towMS * 1e-3;
  State.TP_Timestamp = SystemTime              ;
}
//---------------------------------------------------------------------------------
void GNSS_OnPPS(real_T SystemTime)
{
  // PPS should arrive less than a second after UBX-NAV-TP
  if((SystemTime - State.TP_Timestamp) < 1.0)
  {
    if(State.TP_Valid)
    {
      State.TO_Valid  = 1                        ;
      State.TO_Offset = SystemTime - State.TP_TOW;
    }
  }
}
//---------------------------------------------------------------------------------
void GNSS_OnUbxNavSol(T_UBX_NAV_SOL* UBX_NAV_SOL)
{
  State.UBX_NAV_SOL = *UBX_NAV_SOL;
}
//---------------------------------------------------------------------------------
void GNSS_GetResult(T_GNSS_Result* Result)
{
  real_T pAcc = State.UBX_NAV_SOL.pAcc * 1e-2;
  real_T sAcc = State.UBX_NAV_SOL.sAcc * 1e-2;
  
  memset(Result, 0, sizeof(T_GNSS_Result));
  
  if(State.TO_Valid && State.GNSS_Setup.SystemsToUse)
  {
    Result->T_meas.GPS = State.UBX_NAV_SOL.iTOW * 1e-3       ;
    Result->T_meas.GLO = 0                                   ;
    Result->T_meas.Sys = Result->T_meas.GPS + State.TO_Offset;
    
    Result->Pos[0] = State.UBX_NAV_SOL.ecefX * 1e-2;
    Result->Pos[1] = State.UBX_NAV_SOL.ecefY * 1e-2;
    Result->Pos[2] = State.UBX_NAV_SOL.ecefZ * 1e-2;

    Result->Vel[0] = (real32_T)(State.UBX_NAV_SOL.ecefVX * 1e-2);
    Result->Vel[1] = (real32_T)(State.UBX_NAV_SOL.ecefVY * 1e-2);
    Result->Vel[2] = (real32_T)(State.UBX_NAV_SOL.ecefVZ * 1e-2);
    
    if((State.UBX_NAV_SOL.gpsFix == 0x03) && (pAcc <= LIMIT_pAcc) && (sAcc <= LIMIT_sAcc))
    {
      Result->Valid      = 1                      ;
      Result->NoSVsInFix = State.UBX_NAV_SOL.numSV;
    }
    else
    {
      Result->Valid      = 0;
      Result->NoSVsInFix = 0;
    }
  }
}
//---------------------------------------------------------------------------------
