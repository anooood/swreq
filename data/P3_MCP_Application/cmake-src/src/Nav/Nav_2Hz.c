/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_2Hz.c>
 *
 *  Description:
 *  ------------
 *  NAV_2Hz Module.
 *
 *  Function(s):
 *  ------------
 *  - NAV_2HzInit                            : Initializes Navigation 2Hz
 *  - NAV_2HzExecute                         : Executes Navigation 2Hz
 *  ...
 *
 *
 ******************************************************************************/

#include "NAV_DoNavigationMain_2Hz.h"

static RT_MODEL NAV_2Hz_M;

void NAV_2HzInit(void)
{
  memset(&NAV_2Hz_M, 0, sizeof(NAV_2Hz_M));
  NAV_DoNavigationMain_2Hz_initialize(&NAV_2Hz_M);
}

void NAV_2HzExecute(T_GNSS_Result                    *GNSS_Result    ,
                    T_NAV_MagnetometerMeasurement    *MAG_Measurement,
                    T_TA_TransferAlignState          *TA_State       ,
                    T_NAV_State                      *NavState       ,
                    T_NAV_KFilterErrorEstimateState  *ErrorEst       ,
                    T_NAV_Tel2Hz                     *Tel2Hz         ,
                    T_MAG_Tel                        *TelMag         )
{
  static uint32_T NAV_2HzCount = 1;
  T_NAV_StoreInfo StoreInfo;

  NAV_DoNavigation_2Hz(&NAV_2Hz_M, GNSS_Result, NavState, TA_State, MAG_Measurement, ErrorEst, &StoreInfo, TelMag);

//Tel2Hz->NAV_2HzTimestamp   = MCP_SystemTime     ;  // TODO: Verify this parameter is populated externally
  Tel2Hz->NAV_2HzCount       = NAV_2HzCount++     ;
  Tel2Hz->NAV_StoreTimeReq   = StoreInfo.TimeReq  ;
  Tel2Hz->NAV_StoreCase      = StoreInfo.Case     ;
  Tel2Hz->NAV_StoreIdxLow    = StoreInfo.IdxLow   ;
  Tel2Hz->NAV_StoreIdxHi     = StoreInfo.IdxHi    ;
  Tel2Hz->NAV_StoreReserved1 = StoreInfo.Reserved1;
  Tel2Hz->NAV_StoreTimeLow   = StoreInfo.TimeLow  ;
  Tel2Hz->NAV_StoreTimeHi    = StoreInfo.TimeHi   ;
  Tel2Hz->NAV_StorePos[0]    = StoreInfo.Pos[0]   ;
  Tel2Hz->NAV_StorePos[1]    = StoreInfo.Pos[1]   ;
  Tel2Hz->NAV_StorePos[2]    = StoreInfo.Pos[2]   ;
  Tel2Hz->NAV_StoreReserved2 = StoreInfo.Reserved2;
}

