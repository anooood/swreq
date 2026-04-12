/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_2Hz.h>
 *
 *  Description:
 *  ------------
 *  Header file for the NAV_2Hz module.
 *
 ******************************************************************************/

#ifndef NAV_2HZ_H
#define NAV_2HZ_H


#include "GNSS_Result.h"
#include "Nav100Hz_Output.h"
#include "Nav100Hz_Common.h"
#include "Nav2Hz_Output.h"
#include "Nav2Hz_Common.h"


extern void NAV_2HzInit(void);

extern void NAV_2HzExecute(T_GNSS_Result                    *GNSS_Result    ,
                           T_NAV_MagnetometerMeasurement    *MAG_Measurement,
                           T_TA_TransferAlignState          *TA_State       ,
                           T_NAV_State                      *NavState       ,
                           T_NAV_KFilterErrorEstimateState  *ErrorEst       ,
                           T_NAV_Tel2Hz                     *Tel2Hz         ,
                           T_MAG_Tel                        *TelMag         );

#endif /* NAV_2HZ_H */
