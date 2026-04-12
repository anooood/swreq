/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_50Hz.c>
 *
 *  Description:
 *  ------------
 *  NAV_50Hz Module.
 *
 *  Function(s):
 *  ------------
 *  - NAV_50HzInit                            : Initializes Navigation 50Hz
 *  - NAV_50HzExecute                         : Executes Navigation 50Hz
 *  ...
 *
 *
 ******************************************************************************/

#include "NAV_DoTransferAlignment_50Hz.h"
#include <string.h>
//---------------------------------------------------------------------------
static RT_MODEL NAV_50Hz_M;
//---------------------------------------------------------------------------
void NAV_50HzInit(void)
{
  memset(&NAV_50Hz_M, 0, sizeof(NAV_50Hz_M));
  NAV_DoTransferAlignment_50Hz_initialize(&NAV_50Hz_M);
}
//---------------------------------------------------------------------------
void NAV_50HzExecute(real_T                                 Time     ,
                     T_NAV_GenericInertialNavigationSensor* INS      ,
                     T_RTAC_TransferAlignment             * RTAC_Data,
                     T_TA_TransferAlignState              * TA_State )
{
  NAV_DoNavigation_50Hz(&NAV_50Hz_M, RTAC_Data, Time, INS, TA_State);
}
//---------------------------------------------------------------------------
