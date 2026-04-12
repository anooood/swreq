/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <P32LarApproximation.h>
 *
 *  Description:
 *  ------------
 *  Header file for the P32LarApproximation module.
 *
 ******************************************************************************/

//--[ PREPROCESSOR ]-----------------------------------------------------------
#ifndef LARAPPROXIMATION_H
#define LARAPPROXIMATION_H
//--[ INCLUDE FILES ]----------------------------------------------------------
//--Standart Includes-----------------//
#include <stdint.h>
//--[ DATA TYPES ]-------------------------------------------------------------
typedef struct{
    double dLat_rad;                //[rad]
    double dLong_rad;               //[rad]
    double dAlt_m;                  //[m]
    double dTrack_rad;              //[rad]
    double dMach;                   //[-]
    double dWindSpeed_m_s;          //[m/s]
    double dWindDir_rad;            //[rad]
    uint32_t uiWindValidFlag;       //[-]
}sLaunchParams_t;
typedef struct{
    double dLat_rad;                //[rad]
    double dLong_rad;               //[rad]
    double dAlt_m;                  //[m]
    double dDiveAng_rad;            //[rad]
}sTgtParams_t;
typedef struct{
    double dLarCtrLat_rad;          //[rad]
    double dLarCtrLong_rad;         //[rad]
    double dLarRadius_m;            //[m]
    double dExclRadius_m;           //[m]
    double dGndDistToTgt_m;         //[m]
    double dRelBearToTgt_rad;       //[rad]
    double dGndDistToLAR_m;         //[m]
    double dRelBearToLAR;           //[rad]
    double dRelX_DistToLar;         //[m]
    double dRelY_DistToLar;         //[m]
    double dTimeToImpact_s;         //[sec]
    double dTimeToIllum_s;          //[sec]
    double dTimeToGo_s;             //[sec]
    double dTimeToExit_s;           //[sec]
    double dTrajLat_rad[32];        //[rad]
    double dTrajLong_rad[32];       //[rad]
    double dEstGroundRange_m;       //[m]
    double dEstImpactSpeed_m_s;     //[m/s]
    double dEstImpactTrack_rad;     //[rad]
    double dEstImpactDiveAng_rad;   //[rad]
    double dBallisticRange_m;       //[m]
    double dBallisticFlightTime_s;  //[s]
    uint16_t uwInsideLar;           //[-]
    uint16_t uwExclRadValidFlag;    //[-]

}sLar_t;
typedef struct{
    double dBallisticFlightTime_s;  //[s]
    double dBallisticRange_m;       //[m]
}sBallisticParams_t;

/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : calculateP32Lar
 *
 * DESCRIPTION          : Calculate LAR parameters for P32 based on launch and
 *                        target parameters and conditions
 *
 * INPUT PARAMETERS     : sLaunchParams_t
 *                           dLat_rad;       //[rad] - (-π/2 : π/2)
 *                           dLong_rad;      //[rad] - (-π : π)
 *                           dAlt_m;         //[m]   - (5220 : 12192)
 *                           dTrack_rad;     //[rad] - (0 : 2π)
 *                           dMach;          //[-]   - (0.7 : 0.9)
 *                           dWindSpeed_m_s; //[m/s] - (0 : 80)
 *                           dWindDir_rad;   //[rad] - (0 : 2π)
 * 	                      sTgtParams_t
 *                           dLat_rad;       //[rad] - (-π/2 : π/2)
 *                           dLong_rad;      //[rad] - (-π : π)
 *                           dAlt_m;         //[m]   - (-500 : 5000)
 *                           dDiveAng_rad;   //[rad] - (π/6 : π/2)
 *
 * OUTPUT PARAMETERS    : sLar_t
 *                           dLarCtrLat_rad;          //[rad]
 *                           dLarCtrLong_rad;         //[rad]
 *                           dLarRadius_m;            //[m]
 *                           dExclRadius_m;           //[m]
 *                           bInsideLar;              //[-]
 *                           dGndDistToTgt_m;         //[m]
 *                           dRelBearToTgt_rad;       //[rad]
 *                           dGndDistToLAR_m;         //[m]
 *                           dRelBearToLAR;           //[rad]
 *                           dTimeToImpact_s;         //[sec]
 *                           dTimeToIllum_s;          //[sec]
 *                           dTimeToGo_s;             //[sec]
 *                           dTimeToExit_s;           //[sec]
 *                           dTrajLat_rad[32];        //[rad]
 *                           dTrajLong_rad[32];       //[rad]
 *                           dEstGroundRange_m;       //[m]
 *                           dEstImpactSpeed_m_s;     //[m/s]
 *                           dEstImpactTrack_rad;     //[rad]
 *                           dEstImpactDiveAng_rad;   //[deg]
 *
 * RETURN PARAMETERS    : int32_t - 0 Success
 *                                  1 Failure
 *
 * REMARKS              : Range checks should be done prior to calling this
 *                        function
 *
 *----------------------------------------------------------------------------*/
int32_t calculateP32Lar(sLaunchParams_t sLaunchParams, sTgtParams_t sTgtParams, sLar_t *spLar);
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : calculateBallisticParams
 *
 * DESCRIPTION          : Calculate the ballistic flight time and range
 *
 * INPUT PARAMETERS     : dAlt_m;         //[m]   - (5220 : 12192)
 *                        dMach;          //[-]   - (0.7 : 0.9)
 *                        dAlt_m;         //[m]   - (-500 : 5000)
 *
 * OUTPUT PARAMETERS    : sBallisticParams_t
 *                           dBallisticFlightTime_s;    //[s]
 *                           dBallisticRange_m;         //[m]
 *
 * RETURN PARAMETERS    : int32_t - 0 Success
 *                                  1 Failure
 *
 * REMARKS              : Range checks should be done prior to calling this
 *                        function
 *----------------------------------------------------------------------------*/
int32_t calculateBallisticParams(double dTgtAlt_m, double dAircraftAlt_m,
                                 double dAircraftSpeed_mach, sBallisticParams_t* spParams);


#endif // LARAPPROXIMATION_H
