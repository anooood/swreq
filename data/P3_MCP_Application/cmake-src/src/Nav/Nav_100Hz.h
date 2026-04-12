/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_100Hz.h>
 *
 *  Description:
 *  ------------
 *  Header file for the NAV_100Hz module.
 *
 ******************************************************************************/

#ifndef NAV_100HZ_H
#define NAV_100HZ_H


#include "GNSS_Result.h"
#include "Nav2Hz_Output.h"
#include "Nav100Hz_Output.h"
#include "Nav100Hz_Common.h"

extern void NAV_100HzInit(void);

void NAV_100HzExecute(T_NAV_GenericInertialNavigationSensor *INS        ,
                      T_GNSS_Result                         *GNSS_Result,
                      T_TA_TransferAlignState               *TA_State   ,
                      T_NAV_KFilterErrorEstimateState       *ErrorEst   ,
                      T_NAV_State                           *NavState   ,
                      T_NAV_Tel100Hz                        *Tel100Hz   );

#endif /* NAV_100HZ_H */
