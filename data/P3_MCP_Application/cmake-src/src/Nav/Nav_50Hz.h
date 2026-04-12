/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_50Hz.h>
 *
 *  Description:
 *  ------------
 *  Header file for the NAV_50Hz module.
 *
 ******************************************************************************/

#ifndef NAV_50HZ_H
#define NAV_50HZ_H
//---------------------------------------------------------------------------
#include "Nav50Hz_Common.h"
#include "Nav100Hz_Common.h"
//---------------------------------------------------------------------------
extern void NAV_50HzInit(void);
extern void NAV_50HzExecute(real_T                                 Time     ,
                            T_NAV_GenericInertialNavigationSensor* INS      ,
                            T_RTAC_TransferAlignment             * RTAC_Data,
                            T_TA_TransferAlignState              * TA_State );
//---------------------------------------------------------------------------
#endif /* NAV_50HZ_H */
