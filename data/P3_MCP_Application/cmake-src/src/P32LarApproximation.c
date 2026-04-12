/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <P32LarApproximation.c>
 *
 *  Description:
 *  ------------
 *  P32LarApproximation module is responsible for calculating the P32 Launch acceptable and exclusion region parameters,
 *  estimate flight parameters and situational parameters.
 *  This file is intended to be used in simulation software or as part of a DLL package.
 *
 *  Function(s):
 *  ------------
 *  Each function is defined below.
 *  ...
 *
 *
 ******************************************************************************/

//--[ INCLUDE FILES ]----------------------------------------------------------
//--Standart Includes-----------------//
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "P32LarApproximation.h"

//--[ DEFINES ]----------------------------------------------------------------
#define M_Pi 3.14159265358979323846
#define ubColSize 4
#define ubRowSize 3

//--[ DATA TYPES ]-------------------------------------------------------------
typedef struct{
    double dLarRadius_m;            //[m]
    double dExclRadius_m;           //[m]
    double dGndDistToTgt_m;         //[m]
    double dGndDistToLAR_m;         //[m]
    double dRelBearToTgt_rad;       //[rad]
    double dRelBearToLAR;           //[rad]
}sLarSubSet_t;
typedef struct{
    double dLat_rad;                //[rad]
    double dLong_rad;               //[rad]
    double dHeight_m;               //[m]
}sLLH_t;
typedef struct{
    double dX;                      //[m]
    double dY;                      //[m]
    double dZ;                      //[m]
}sXYZ_t;
typedef struct{
    double dEast;                   //[m]
    double dNorth;                  //[m]
    double dUp;                     //[m]
}sENU_t;
typedef struct{
    uint16_t uwInsideLar;           //[-]
    uint16_t uwReserved;            //[-]
    double dTimeToImpact;           //[sec]
    double dTimeToIlluminate;       //[sec]
    double dTimeToGo;               //[sec]
    double dTimeToExit;             //[sec]
}sTimingParams;
typedef struct
{
    double dPoint1;                 //[-]
    double dPoint2;                 //[-]
    double dPoint3;                 //[-]
    double dPoint4;                 //[-]
    double dPoint5;                 //[-]
}sLarPoints_t;


//--[ LOOKUP TABLES ]----------------------------------------------------------

//-----------------------------------------|-----|------|-------|-------|-------|-----------------|
//--------------------------HEADING [deg]->|  0  |  90  |  180  |  270  |  360  |v-Radius ration-v|
//-----------------------------------------|-----|------|-------|-------|-------|-----------------|
static double daTofLut_[2][5]          = { { 0   ,   0  ,   0   ,   0   ,   0   },    // 0%       |
                                           { 0   ,   0  ,   0   ,   0   ,   0   } };  //100%      |
//-----------------------------------------|-----|------|-------|-------|-------|-----------------|


const double dDistColumns[ubColSize] = {10000,20000,30000,40000};
const double dDistRows[ubRowSize] = {0.7,0.8,0.9};
//==30==
//-------------------------------------------------------|-------|-------|-------|-------|--------|
//----------------------------------------ALTITUDE [FT]->| 10000 | 20000 | 30000 | 40000 |v-MACH-v|
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarCenDistLut30[ubRowSize][ubColSize]={ { 6600,   9000,   13000,  18500},  //0.7 |
                                                         { 8200,   11000,  15500,  18500},  //0.8 |
                                                         { 9800,   12500,  17000,  19500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarRadLut30[ubRowSize][ubColSize]  = {  { 2400,   4000,   7000,   8000},   //0.7 |
                                                         { 3400,   5500,   9000,   11500},  //0.8 |
                                                         { 4600,   7000,   11000,  13500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiExclRadLut30[ubRowSize][ubColSize]  = { { 5000,   6000,   6500,   11500},  //0.7 |
                                                         { 5200,   6500,   7000,   14000},  //0.8 |
                                                         { 5500,   7000,   7500,   15000}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
//==45==
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarCenDistLut45[ubRowSize][ubColSize]={ { 5400,   8000,   10500,  11500},  //0.7 |
                                                         { 5600,   9500,   12000,  14500},  //0.8 |
                                                         { 5800,   11000,  14500,  19500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarRadLut45[ubRowSize][ubColSize]   = { { 1800,   3000,   5000,   7000},   //0.7 |
                                                         { 2000,   4500,   6500,   8000},   //0.8 |
                                                         { 2200,   6000,   9000,   9500}};  //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiExclRadLut45[ubRowSize][ubColSize]  = { { 4600,   5500,   7500,   13500},  //0.7 |
                                                         { 5000,   6500,   6500,   13500},  //0.8 |
                                                         { 5500,   7500,   8500,   13500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
//==60==
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarCenDistLut60[ubRowSize][ubColSize]={ { 0,      7000,   9000,   9500},   //0.7 |
                                                         { 0,      7500,   10000,  11500},  //0.8 |
                                                         { 0,      9500,   12500,  14500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarRadLut60[ubRowSize][ubColSize]   = { { 0,      2000,   4000,   5500},   //0.7 |
                                                         { 0,      3500,   5000,   6500},   //0.8 |
                                                         { 0,      4500,   6500,   7500}};  //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiExclRadLut60[ubRowSize][ubColSize]  = { { 0,      6000,   7500,   12500},  //0.7 |
                                                         { 0,      6500,   7500,   13000},  //0.8 |
                                                         { 0,      6500,   7500,   13000}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
//==70==
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarCenDistLut70[ubRowSize][ubColSize]={ { 0,      7000,   8000,   9500},   //0.7 |
                                                         { 0,      7500,   10000,  12500},  //0.8 |
                                                         { 0,      9500,   12500,  15500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarRadLut70[ubRowSize][ubColSize]   = { { 0,      1000,   3000,   4500},   //0.7 |
                                                         { 0,      2500,   4000,   4500},   //0.8 |
                                                         { 0,      3500,   5500,   5500}};  //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiExclRadLut70[ubRowSize][ubColSize]  = { { 0,      6500,   7500,   11500},  //0.7 |
                                                         { 0,      7500,   8000,   12000},  //0.8 |
                                                         { 0,      7000,   8500,   12500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
//==90==
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarCenDistLut90[ubRowSize][ubColSize]={ { 0,      0,      6500,   10500},  //0.7 |
                                                         { 0,      0,      8000,   11500},  //0.8 |
                                                         { 0,      0,      8500,   13500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiLarRadLut90[ubRowSize][ubColSize]   = { { 0,      0,      1500,   1500},   //0.7 |
                                                         { 0,      0,      1500,   1500},   //0.8 |
                                                         { 0,      0,      2000,   2500}};  //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|
const uint32_t uiExclRadLut90[ubRowSize][ubColSize]  = { { 0,      0,      8000,   12000},  //0.7 |
                                                         { 0,      0,      9000,   12500},  //0.8 |
                                                         { 0,      0,      9500,   12500}}; //0.9 |
//-------------------------------------------------------|-------|-------|-------|-------|--------|

const double dBalRangeColumns[8] = {5000,10000,15000,20000,25000,30000,35000,40000};
const double dBalRangeRows[3] = {0.7,0.8,0.9};
//----------------------------------------|-------|-------|-------|-------|-------|-------|-------|-------|--------|
//-------------------------ALTITUDE [FT]->|  5000 | 10000 | 15000 | 20000 | 25000 | 30000 | 35000 | 40000 |v-MACH-v|
//----------------------------------------|-------|-------|-------|-------|-------|-------|-------|-------|--------|
//----------------------------------------|-------|-------|-------|-------|-------|-------|-------|-------|--------|
const double daBallisticRange[3][8]    = { { 3.84,   5.23,   6.17,   6.85,   7.38,   7.82,   8.19,   8.52},  //0.7 |
                                           { 4.35,   5.89,   6.92,   7.67,   8.26,   8.76,   9.20,   9.58},  //0.8 |
                                           { 4.83,   6.51,   7.59,   8.40,   9.05,   9.61,   10.11, 10.56}}; //0.9 |
//----------------------------------------|-------|-------|-------|-------|-------|-------|-------|-------|--------|
const double dTofColumns[5] = {0,90,180,270,360};
const double dTofRows[2] = {0,1};

//--[ Function declarations ]----------------------------------------------------------
double interpolate2dBaseParametersLut( const uint32_t uiaLutArray[ubRowSize][ubColSize],
                                       double dAltAboveTgt_ft,
                                       double dAircraftSpeed_mach);
double getDistLarCenter_m( double dTgtAlt_m,
                           double dAircraftAlt_m,
                           double dAircraftSpeed_mach,
                           uint16_t uwDiveAngle_deg);
double getLarRadius_m( double dTgtAlt_m,
                       double dAircraftAlt_m,
                       double dAircraftSpeed_mach,
                       uint16_t uwDiveAngle_deg);
double getExclRegionRadius_m( double dTgtAlt_m,
                              double dAircraftAlt_m,
                              double dAircraftSpeed_mach,
                              uint16_t uwDiveAngle_deg);
double getBallisticRange_m( double dTgtAlt_ft,
                            double dAircraftAlt_ft,
                            double dAircraftSpeed_mach);
double getBallisticFlightTime_s( double dTgtAlt_ft,
                                 double dAircraftAlt_ft);
sLarPoints_t getSlantRanges( double dLarCenterDist_m,
                             double dLarRadius_m,
                             double dAltAboveTgt_m,
                             double dAircraftSpeed_mach);
sXYZ_t convertLlhToEcef( sLLH_t sLLH);
sLLH_t convertEcefToLlh( sXYZ_t sXYZ);
sENU_t convertEcefToEnu( sXYZ_t sEcef,sXYZ_t sEcefOrig);
sXYZ_t convertEnuToEcef( sENU_t sEnu,sXYZ_t sEcefOrig);
double convertMachToTas( double dMach, double dAltitude_m);
sTimingParams calculateP32Timings( double dGroundSpeed, sLarSubSet_t sLarSub);
double interpolate2dLutTiming( double daLutArray[2][5],
                                const double dapColums[],
                                const double dapRow[],
                                int32_t iColSize,
                                int32_t iRowSize,
                                double dColValue,
                                double dRowValue);
double interpolate2dLutBalRange( const double daLutArray[3][8],
                                const double dapColums[],
                                const double dapRow[],
                                int32_t iColSize,
                                int32_t iRowSize,
                                double dColValue,
                                double dRowValue);
//--[ Function Source ]---------------------------------------------------------
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getSlantRanges
 *
 * DESCRIPTION          : Calculate the Slant Ranges from the Target to the five
 *                        points on the edge of the LAR.
 *
 * INPUT PARAMETERS     : double dLarCenterDist_m - distance to LAR center point
 *                        double dLarRadius_m - Lar radius
                          double dAltAboveTgt - Altitude above target
                          double dAircraftSpeed_mach -  speed
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : sLarPoints_t
 *                           dPoint1 - center of lar
 *                           dPoint2 - top
 *                           dPoint3 - right
 *                           dPoint4 - bottom
 *                           dPoint5 - left
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
sLarPoints_t getSlantRanges(double dLarCenterDist_m, double dLarRadius_m,
                            double dAltAboveTgt_m, double dAircraftSpeed_mach)
{
    if (dAltAboveTgt_m > 40000.0*0.3048)
        dAltAboveTgt_m = 40000.0*0.3048;
    if (dAircraftSpeed_mach > 0.9)
        dAircraftSpeed_mach = 0.9;

    sLarPoints_t sSr;
    sLarPoints_t sTof;
    sSr.dPoint1 = sqrt(pow(dLarCenterDist_m,2) + pow(dAltAboveTgt_m,2))/1000;
    sSr.dPoint2 = sqrt(pow((dLarCenterDist_m-dLarRadius_m),2) + pow(dAltAboveTgt_m,2))/1000;
    sSr.dPoint3 = sqrt(pow(dLarCenterDist_m,2) + pow(dLarRadius_m,2) + pow(dAltAboveTgt_m,2))/1000;
    sSr.dPoint4 = sqrt(pow((dLarCenterDist_m+dLarRadius_m),2) + pow(dAltAboveTgt_m,2))/1000;
    sSr.dPoint5 = sqrt(pow(dLarCenterDist_m,2) + pow(dLarRadius_m,2) + pow(dAltAboveTgt_m,2))/1000;

    sTof.dPoint1 = -0.0916 * pow(sSr.dPoint1,2) + 6.1 * sSr.dPoint1 + 7.7 - 20 * dAircraftSpeed_mach;
    sTof.dPoint2 = -0.0916 * pow(sSr.dPoint2,2) + 6.1 * sSr.dPoint2 + 7.7 - 20 * dAircraftSpeed_mach;
    sTof.dPoint3 = -0.0916 * pow(sSr.dPoint3,2) + 6.1 * sSr.dPoint3 + 7.7 - 20 * dAircraftSpeed_mach;
    sTof.dPoint4 = -0.0916 * pow(sSr.dPoint4,2) + 6.1 * sSr.dPoint4 + 7.7 - 20 * dAircraftSpeed_mach;
    sTof.dPoint5 = -0.0916 * pow(sSr.dPoint5,2) + 6.1 * sSr.dPoint5 + 7.7 - 20 * dAircraftSpeed_mach;

    return sTof;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getLarCenterDist_m
 *
 * DESCRIPTION          : Selects lookup table (LUT) based on dive angle, query
 *                        LUT, return result if valid.
 *
 * INPUT PARAMETERS     : double dTgtAlt_m, double dAircraftAlt_m,
 *                        double dAircraftSpeed_mach, uint16_t uwDiveAngle_deg
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double - Distance to LAR center in meters
 *                               - (-1) if error
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
double getDistLarCenter_m(double dTgtAlt_m, double dAircraftAlt_m, double dAircraftSpeed_mach, uint16_t uwDiveAngle_deg)
{
    //Calculate the aircraft altitude above the target as
    double dAltAboveTgt_ft = dAircraftAlt_m/0.3048 - dTgtAlt_m/0.3048;
    double dLarCentDist = 0;
    if ((uwDiveAngle_deg >= 30) && (uwDiveAngle_deg < 45))
    {
        dLarCentDist = interpolate2dBaseParametersLut(uiLarCenDistLut30,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 45) && (uwDiveAngle_deg < 60))
    {
        dLarCentDist = interpolate2dBaseParametersLut(uiLarCenDistLut45,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 60) && (uwDiveAngle_deg < 70))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dLarCentDist = interpolate2dBaseParametersLut(uiLarCenDistLut60,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarCentDist = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg >= 70) && (uwDiveAngle_deg < 90))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dLarCentDist = interpolate2dBaseParametersLut(uiLarCenDistLut70,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarCentDist = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg == 90))
    {
        if ( dAltAboveTgt_ft >= 30000)
        {
            dLarCentDist = interpolate2dBaseParametersLut(uiLarCenDistLut90,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarCentDist = -1;
            //Alt not valid, return error
        }
    }
    else
    {
        dLarCentDist = -1;
        //Dive angle not valid, return error
    }
    return dLarCentDist;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getLarRadius_m
 *
 * DESCRIPTION          : Selects lookup table (LUT) based on dive angle, query
 *                        LUT, return result if valid.
 *
 * INPUT PARAMETERS     : double dTgtAlt_m, double dAircraftAlt_m,
 *                        double dAircraftSpeed_mach, uint16_t uwDiveAngle_deg
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double - LAR radius in meters
 *                               - (-1) if error
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
double getLarRadius_m(double dTgtAlt_m,
                      double dAircraftAlt_m,
                      double dAircraftSpeed_mach,
                      uint16_t uwDiveAngle_deg)
{
    //Calculate the aircraft altitude above the target as
    double dAltAboveTgt_ft = dAircraftAlt_m/0.3048 - dTgtAlt_m/0.3048;
    double dLarRadius_m = 0;
    if ((uwDiveAngle_deg >= 30) && (uwDiveAngle_deg < 45))
    {
        dLarRadius_m = interpolate2dBaseParametersLut(uiLarRadLut30,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 45) && (uwDiveAngle_deg < 60))
    {
        dLarRadius_m = interpolate2dBaseParametersLut(uiLarRadLut45,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 60) && (uwDiveAngle_deg < 70))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dLarRadius_m = interpolate2dBaseParametersLut(uiLarRadLut60,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg >= 70) && (uwDiveAngle_deg < 90))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dLarRadius_m = interpolate2dBaseParametersLut(uiLarRadLut70,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg == 90))
    {
        if ( dAltAboveTgt_ft >= 30000)
        {
            dLarRadius_m = interpolate2dBaseParametersLut(uiLarRadLut90,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dLarRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else
    {
        dLarRadius_m = -1;
        //Dive angle not valid, return error
    }
    return dLarRadius_m;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getExclRegionRadius_m
 *
 * DESCRIPTION          : Selects lookup table (LUT) based on dive angle, query
 *                        LUT, return result if valid.
 *
 * INPUT PARAMETERS     : double dTgtAlt_m, double dAircraftAlt_m,
 *                        double dAircraftSpeed_mach, uint16_t uwDiveAngle_deg
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double - Exclude region radius in meters
 *                               - (-1) if error
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
double getExclRegionRadius_m(double dTgtAlt_m,
                             double dAircraftAlt_m,
                             double dAircraftSpeed_mach,
                             uint16_t uwDiveAngle_deg)
{
    //Calculate the aircraft altitude above the target as
    double dAltAboveTgt_ft = dAircraftAlt_m/0.3048 - dTgtAlt_m/0.3048;
    double dExclRegionRadius_m = 0;
    if ((uwDiveAngle_deg >= 30) && (uwDiveAngle_deg < 45))
    {
        dExclRegionRadius_m = interpolate2dBaseParametersLut(uiExclRadLut30,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 45) && (uwDiveAngle_deg < 60))
    {
        dExclRegionRadius_m = interpolate2dBaseParametersLut(uiExclRadLut45,dAltAboveTgt_ft,dAircraftSpeed_mach);
    }
    else if ((uwDiveAngle_deg >= 60) && (uwDiveAngle_deg < 70))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dExclRegionRadius_m = interpolate2dBaseParametersLut(uiExclRadLut60,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dExclRegionRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg >= 70) && (uwDiveAngle_deg < 90))
    {
        if ( dAltAboveTgt_ft >= 20000)
        {
            dExclRegionRadius_m = interpolate2dBaseParametersLut(uiExclRadLut70,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dExclRegionRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else if ((uwDiveAngle_deg == 90))
    {
        if ( dAltAboveTgt_ft >= 30000)
        {
            dExclRegionRadius_m = interpolate2dBaseParametersLut(uiExclRadLut90,dAltAboveTgt_ft,dAircraftSpeed_mach);
        }
        else
        {
            dExclRegionRadius_m = -1;
            //Alt not valid, return error
        }
    }
    else
    {
        dExclRegionRadius_m = -1;
        //Dive angle not valid, return error
    }
    return dExclRegionRadius_m;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getBallisticRange_m
 *
 * DESCRIPTION          : The ballistic range shall be used when no target
 *                        coordinates are provided, The 2-D lookup table data
 *                        for the ballistic range is given in daBallisticRange
 *
 * INPUT PARAMETERS     : double dTgtAlt_ft, double dAircraftAlt_ft,
 *                        double dAircraftSpeed_mach
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double - Ballistic range in m
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
double getBallisticRange_m(double dTgtAlt_ft,
                            double dAircraftAlt_ft,
                            double dAircraftSpeed_mach)
{
    double dAlt_ft = dAircraftAlt_ft - dTgtAlt_ft;
    double dBalRange_km = interpolate2dLutBalRange(daBallisticRange, &dBalRangeColumns[0], &dBalRangeRows[0],
            (sizeof(dBalRangeColumns)/sizeof(dBalRangeColumns[0])),
            (sizeof(dBalRangeRows)/sizeof(dBalRangeRows[0])),dAlt_ft,dAircraftSpeed_mach);
    return dBalRange_km * 1000.0;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : interpolate2dBaseParametersLut
 *
 * DESCRIPTION          : Interpolation for 2-D gridded data in meshgrid format
 *
 * INPUT PARAMETERS     : const uint32_t uiaLutArray[ubRowSize][ubColSize]
 *                        -> LUT containing values required
 *                        double dAltAboveTgt_ft
 *                        -> Altitude delta between the target and aircraft
 *                           altitude
 *                        double dAircraftSpeed_mach
 *                        -> aircraft speed as mach number
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double -> interpolated value from lookup table
 *                        -1 if values out of range.
 *
 * REMARKS              :
 *
 *    Double interpolation Ratio eq:
 *
 *    (f(Xt) - f(X1))/(f(X2) - f(X1)) = (Xt - X1)/(X2 - X1)
 *
 *    (Step 1) -> X_ratio = (Xt - X1)/(X2 - X1)
 *    (Step 2) -> f(Xt) = f(X1) + (f(X2) - f(X1)) * X_ratio
 *    (Step 3) -> g(Xt) = g(X1) + (g(X2) - g(X1)) * X_ratio
 *    (Step 4) -> Y_ratio = (Yt - Y1)/(Y2 - Y1)
 *    (Step 5) -> h(Yt) = f(Xt) + (g(Xt) - f(Xt)) * Y_ratio
 *
 *    |-----------|-------------|-------|------------|
 *    |           | 15000 (X1)  |   Xt  | 20000 (X2) |
 *    |-----------|-------------|-------|------------|
 *    |  0.7 (Y1) | 7943  f(X1) | f(Xt) | 9240 f(X2) |
 *    |    Yt     |             | h(Yt) |            |
 *    |  0.8 (Y2) | 8993  g(X1) | g(Xt) | 10739 g(X2)|
 *    |-----------|-------------|-------|------------|
 *
 *    For altitude values larger than 40 000 ft, the look-up should return
 *    the value for 40 000 ft from the table.
 *    For altitude values, less than 15 000 ft, the solution is not valid,
 *    and no look-up should be performed.
 *    For speed values outside 0.7 - 0.9, the solution is not valid,
 *    and no look-up should be performed.
 *----------------------------------------------------------------------------*/
double interpolate2dBaseParametersLut(const uint32_t uiaLutArray[ubRowSize][ubColSize],
                                      double dAltAboveTgt_ft, double dAircraftSpeed_mach)
{
    double dXt = dAltAboveTgt_ft;
    double dYt = dAircraftSpeed_mach;
    double dX_Ratio;
    double f_Xt;
    double g_Xt;
    double dY_Ratio;
    double h_Yt = 0;
    double dNormilisedCol = 0.0;
    uint32_t uiColIndex = 0;
    double dNormilisedRow = 0.0;
    uint32_t uiRowIndex = 0;
    bool flagContinue = true;

    if (dAltAboveTgt_ft > 40000.0)
    {
        dAltAboveTgt_ft = 40000.0;
        dXt = 40000.0;
    }
    else if ((dAltAboveTgt_ft < 10000.0))
    {
        flagContinue = false;
        h_Yt = -1;
    }
    if (flagContinue) /* ((dAltAboveTgt_ft >= 10000.0) && (dAltAboveTgt_ft <= 40000.0))*/
    {
        //Test range of Aircraft speed
        if (dAircraftSpeed_mach > 0.9)
        {
            h_Yt = -1;
            flagContinue = false;
        }
        else if (dAircraftSpeed_mach < 0.7)
        {
            h_Yt = -1;
            flagContinue = false;
        }
        else
        {
            //Determin Colomn index based on the Altitude above target
            double dDeltaCol = fabs(dDistColumns[1] - dDistColumns[0]); //Difference between columns
            if ( dDeltaCol > 0.0 )
            {
                uint32_t uiColOffset = (uint32_t)(dDistColumns[0] / dDeltaCol + 0.5); //Offset to first column
                dNormilisedCol = dAltAboveTgt_ft / dDeltaCol - uiColOffset;
                uiColIndex = (uint32_t)(dNormilisedCol); //ALWAYS FLOOR
                //Index at end of table
                if (uiColIndex >= ubColSize - 1)
                    uiColIndex = ubColSize - 2;
            }
            else
            {
                h_Yt = -1;
                flagContinue = false;
            }

            //Determin row index based on the aircraft speed
            double dDeltaRow = dDistRows[1] - dDistRows[0]; //Difference between rows
            if (dDeltaRow > 0.0)
            {
                uint32_t uiOffsetRow = (uint32_t)(dDistRows[0] / dDeltaRow + 0.5); //Offset to first row
                dNormilisedRow = dAircraftSpeed_mach / dDeltaRow - uiOffsetRow;
                uiRowIndex = (uint32_t)(dNormilisedRow);  //ALWAYS FLOOR
                //Index at end of table
                if (uiRowIndex >= ubRowSize - 1)
                    uiRowIndex = ubRowSize - 2;
            }
            else
            {
                h_Yt = -1;
                flagContinue = false;
            }
        }
        if ( flagContinue )
        {
            // 2D Interpolation
            //(Step 1)
            dX_Ratio = (dXt - dDistColumns[uiColIndex]) / (dDistColumns[uiColIndex+1] - dDistColumns[uiColIndex]);
            //(Step 2)
            f_Xt = uiaLutArray[uiRowIndex][uiColIndex] +
                    (uiaLutArray[uiRowIndex][uiColIndex+1] - uiaLutArray[uiRowIndex][uiColIndex]) *
                    dX_Ratio;
            //(Step 3)
            g_Xt = uiaLutArray[uiRowIndex+1][uiColIndex] +
                    (uiaLutArray[uiRowIndex+1][uiColIndex+1] - uiaLutArray[uiRowIndex+1][uiColIndex]) *
                    dX_Ratio;
            //(Step 4)
            dY_Ratio = (dYt - dDistRows[uiRowIndex]) / (dDistRows[uiRowIndex+1] - dDistRows[uiRowIndex]);
            //(Step 5)
            h_Yt =  f_Xt + (g_Xt - f_Xt) * dY_Ratio;
         }
    }
    return h_Yt;
}

/*--[ FUNCTION ]----------------------------------------------------------------
*
* FUNCTION NAME        : convertLlhToEcef
*
* DESCRIPTION          : Convert from WGS-84 latitude, longitude and height
                         to ECEF cartesian coordinates.
*
* INPUT PARAMETERS     : dLat_rad = latitude in radians
*                        dLong_rad = longitude in radians
*                        dHeight_m = height above ellipsoid in meters
*
* OUTPUT PARAMETERS    : dX = ECEF x-coordinate in meters
*                        dY = ECEF y-coordinate in meters
*                        dZ = ECEF z-coordinate in meters
*
* RETURN PARAMETERS    : NONE
*
* REMARKS              : Reference: Understanding GPS: Principles and Applications,
*                        Elliott D. Kaplan, Editor, Artech House Publishers,
*                        Boston, 1996.
*
*                        M. & S. Braasch 10-96
*                        Copyright (c) 1996 by GPSoft
*                        All Rights Reserved.
*
*----------------------------------------------------------------------------*/
sXYZ_t convertLlhToEcef(sLLH_t sLLH)
{
    sXYZ_t sXYZ;

    double dPhi = sLLH.dLat_rad;
    double dLambda = sLLH.dLong_rad;
    double dH = sLLH.dHeight_m;

    double dA = 6378137.0000;	// earth semimajor axis in meters
    double dB = 6356752.3142;	// earth semiminor axis in meters
    double dE = sqrt(1-pow(dB/dA,2));

    double dSinphi = sin(dPhi);
    double dCosphi = cos(dPhi);
    double dCoslam = cos(dLambda);
    double dSinlam = sin(dLambda);
    double dTan2phi = pow((tan(dPhi)),2);
    double dTmp = 1 - dE*dE;
    double dTmpden = sqrt( 1 + dTmp*dTan2phi );

    double dX = (dA * dCoslam)/dTmpden + dH * dCoslam * dCosphi;
    double dY = (dA * dSinlam)/dTmpden + dH * dSinlam * dCosphi;

    double dTmp2 = sqrt(1 - dE * dE * dSinphi * dSinphi);
    double dZ = (dA * dTmp * dSinphi)/dTmp2 + dH * dSinphi;

    sXYZ.dX = dX;
    sXYZ.dY = dY;
    sXYZ.dZ = dZ;
    return sXYZ;
}

/*--[ FUNCTION ]----------------------------------------------------------------
*
* FUNCTION NAME        : convertEcefToLlh
*
* DESCRIPTION          : Convert from ECEF cartesian coordinates to
*                        WGS-84 latitude, longitude and height.
*
* INPUT PARAMETERS     : dX = ECEF x-coordinate in meters
*                        dY = ECEF y-coordinate in meters
*                        dZ = ECEF z-coordinate in meters

*
* OUTPUT PARAMETERS    : dLat_rad = latitude in radians
*                        dLong_rad = longitude in radians
*                        dHeight_m = height above ellipsoid in meters
*
* RETURN PARAMETERS    : NONE
*
* REMARKS              : Reference: Understanding GPS: Principles and Applications,
*                        Elliott D. Kaplan, Editor, Artech House Publishers,
*                        Boston, 1996.
*
*                        M. & S. Braasch 10-96
*                        Copyright (c) 1996 by GPSoft
*                        All Rights Reserved.
*
*----------------------------------------------------------------------------*/
sLLH_t convertEcefToLlh( sXYZ_t sXYZ)
{
    sLLH_t sLLH;

    double dX = sXYZ.dX;
    double dY = sXYZ.dY;
    double dZ = sXYZ.dZ;
    double dX2 = pow(dX,2);
    double dY2 = pow(dY,2);
    double dZ2 = pow(dZ,2);

    double dA = 6378137.0000;  // earth radius in meters
    double dB = 6356752.3142;	// earth semiminor in meters
    double dE = sqrt(1-pow(dB/dA,2));
    double dB2 = dB*dB;
    double dE2 = dE*dE;
    double dEp = dE*(dA/dB);
    double dR = sqrt(dX2+dY2);
    double dR2 = dR*dR;
    double d_E2 = pow(dA,2) - pow(dB,2);
    double d_F = 54 * dB2 * dZ2;
    double d_G = dR2 + (1 - dE2) * dZ2 - dE2 * d_E2;
    double dC = (dE2 * dE2 * d_F * dR2) / (d_G * d_G * d_G);
    double dS = pow(( 1 + dC + sqrt(dC * dC + 2 * dC) ),(1/3));
    double d_P = d_F / (3 * pow((dS + 1 / dS + 1 ),2) * d_G * d_G);
    double d_Q = sqrt( 1 + 2 * dE2 * dE2 * d_P);
    double dRo = -(d_P * dE2 * dR) / ( 1 + d_Q) + sqrt(( dA * dA/2 ) * (1 + 1/d_Q)
                                                       - (d_P * (1 - dE2) * dZ2)/(d_Q * (1+d_Q)) - d_P * dR2/2);
    double dTmp = pow((dR - dE2*dRo),2);
    double d_U = sqrt( dTmp + dZ2 );
    double d_V = sqrt( dTmp + ( 1 - dE2) * dZ2 );
    double dZo = ( dB2 * dZ ) / ( dA * d_V );

    double dHeight = d_U * ( 1 - dB2 / ( dA * d_V ) );
    double dLat = atan( (dZ + dEp * dEp * dZo) / dR );
    dTmp = atan(dY/dX);
    double dLong;
    if (dX >= 0)
        dLong = dTmp;
    else if ((dX < 0) && (dY >= 0))
        dLong = M_Pi + dTmp;
    else
        dLong = dTmp - M_Pi;

    sLLH.dLat_rad = dLat;
    sLLH.dLong_rad = dLong;
    sLLH.dHeight_m = dHeight;
    return sLLH;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : convertEcefToEnu
 *
 * DESCRIPTION          : Converts the Earth-Centered Earth-Fixed (ECEF) coordinates
 *                        (x, y, z) to East-North-Up coordinates in a Local Tangent
 *                        Plane that is centered at the ECEF origin coordinates.
 *
 * INPUT PARAMETERS     : dX = ECEF x-origin coordinate in meters
 *                        dY = ECEF y-origin coordinate in meters
 *                        dZ = ECEF z-origin coordinate in meters
 *
 *                        dX = ECEF x-coordinate in meters
 *                        dY = ECEF y-coordinate in meters
 *                        dZ = ECEF z-coordinate in meters
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : dEast
 *                        dNorth
 *                        dUp
 *
 * REMARKS              : Reference: Understanding GPS: Principles and Applications,
 *                        Elliott D. Kaplan, Editor, Artech House Publishers,
 *                        Boston, 1996.
 *
 *                        M. & S. Braasch 10-96
 *                        Copyright (c) 1996 by GPSoft
 *                        All Rights Reserved.
 *
 *----------------------------------------------------------------------------*/
sENU_t convertEcefToEnu(sXYZ_t sEcef,sXYZ_t sEcefOrig)
{
    sENU_t sENU;
    sLLH_t sLlhOrig = convertEcefToLlh(sEcefOrig);
    double dLambda = sLlhOrig.dLat_rad;
    double dPhi = sLlhOrig.dLong_rad;

    double dSin_lambda = sin(dLambda);
    double dCos_lambda = cos(dLambda);
    double dCos_phi = cos(dPhi);
    double dSin_phi = sin(dPhi);

    double dX, dY, dZ;
    dX = sEcef.dX - sEcefOrig.dX;
    dY = sEcef.dY - sEcefOrig.dY;
    dZ = sEcef.dZ - sEcefOrig.dZ;

    //matrix multiplication
    sENU.dEast = -dSin_phi * dX + dCos_phi * dY;
    sENU.dNorth = -dCos_phi * dSin_lambda * dX - dSin_lambda * dSin_phi * dY + dCos_lambda * dZ;
    sENU.dUp = dCos_lambda * dCos_phi * dX + dCos_lambda * dSin_phi * dY + dSin_lambda * dZ;
    return sENU;
}


/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : convertEnuToEcef
 *
 * DESCRIPTION          : Converts East-North-Up coordinates (xEast, yNorth, zUp)
 *                        in a Local Tangent Plane that is centered at the (WGS-84)
 *                        Geodetic point (lat0, lon0, h0) to the Earth-Centered
 *                        Earth-Fixed (ECEF) coordinates (x, y, z).
 *
 * INPUT PARAMETERS     : dX = ECEF x-origin coordinate in meters
 *                        dY = ECEF y-origin coordinate in meters
 *                        dZ = ECEF z-origin coordinate in meters
 *
 *                        dEast
 *                        dNorth
 *                        dUp
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : dX = ECEF x-coordinate in meters
 *                        dY = ECEF y-coordinate in meters
 *                        dZ = ECEF z-coordinate in meters
 *
 * REMARKS              : Reference: Understanding GPS: Principles and Applications,
 *                        Elliott D. Kaplan, Editor, Artech House Publishers,
 *                        Boston, 1996.
 *
 *                        M. & S. Braasch 10-96
 *                        Copyright (c) 1996 by GPSoft
 *                        All Rights Reserved.
 *
 *----------------------------------------------------------------------------*/
sXYZ_t convertEnuToEcef(sENU_t sEnu,sXYZ_t sEcefOrig)
{
    sXYZ_t sXYZ;
    sLLH_t sLlhOrig = convertEcefToLlh(sEcefOrig);
    double lambda = sLlhOrig.dLat_rad;
    double phi = sLlhOrig.dLong_rad;

    double sin_lambda = sin(lambda);
    double cos_lambda = cos(lambda);
    double cos_phi = cos(phi);
    double sin_phi = sin(phi);

    double xd = -sin_phi * sEnu.dEast - cos_phi * sin_lambda * sEnu.dNorth + cos_lambda * cos_phi * sEnu.dUp;
    double yd = cos_phi * sEnu.dEast - sin_lambda * sin_phi * sEnu.dNorth + cos_lambda * sin_phi * sEnu.dUp;
    double zd = cos_lambda * sEnu.dNorth + sin_lambda * sEnu.dUp;

    sXYZ.dX = xd + sEcefOrig.dX;
    sXYZ.dY = yd + sEcefOrig.dY;
    sXYZ.dZ = zd + sEcefOrig.dZ;
    return sXYZ;
}

/*--[ FUNCTION ]----------------------------------------------------------------
*
* FUNCTION NAME        : convertMachToTas
*
* DESCRIPTION          : Convert from Mach number to True Airspeed.
*                        Valid up to 20 000 m
*
* INPUT PARAMETERS     : double dMach - Air speed as mach number
*                        double dAltitude_m - Altitude in meters
*
* OUTPUT PARAMETERS    : NONE
*
* RETURN PARAMETERS    : double - True air speed in meters per second [m/s]
*                               - (-1) if failure
*
* REMARKS              : Valid up to 20 000 m
*
*----------------------------------------------------------------------------*/
double  convertMachToTas(double dMach, double dAltitude_m)
{
    double dT0, dLr, dRe, dT, dVsound, dTas;
    if (dAltitude_m <= 20000.0)
    {
        if (dAltitude_m <= 11000.0)
        {
            dT0 = 288.15;
            dLr = -0.0065;
        }
        else
        {
            dT0 = 216.65;
            dLr = 0.0;
        }
        dRe = 6356766;
        dT  = dT0 + dLr * (dRe * dAltitude_m) / (dRe + dAltitude_m);
        dVsound = sqrt(1.4 * 287.05287 * dT);
        dTas = dMach*dVsound;
    }
    else
    {
        dTas = -1;
    }
    return dTas;
}

/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : calculateP32Timings
 *
 * DESCRIPTION          : Calculate P32 LAR timing parameters.
 *
 * INPUT PARAMETERS     : sLarSubSet_t
 *                           dLarRadius_m;      [m]
 *                           dExclRadius_m;     [m]
 *                           dGndDistToTgt_m;   [m]
 *                           dGndDistToLAR_m;   [m]
 *                           dRelBearToTgt_rad; [rad]
 *                           dRelBearToLAR;     [rad]
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : sTimingParams
 *                           bInsideLar;          [-]
 *                           dTimeToImpact;       [sec]
 *                           dTimeToIlluminate;   [sec]
 *                           dTimeToGo;           [sec]
 *                           dTimeToExit;         [sec]
 *
 * REMARKS              : Uses global Radial flight time table daTofLut_[][]
 *
 *----------------------------------------------------------------------------*/
sTimingParams calculateP32Timings(double dGroundSpeed, sLarSubSet_t sLarSub)
{
    sTimingParams sTiming;
    int32_t iInterceptLar, iInterceptExclRegion;

    // Determine if release point is inside the LAR or
    // track is intercepting LAR or Exclusion Zone
    if (sLarSub.dGndDistToTgt_m <= sLarSub.dExclRadius_m)  // Inside Exclusion Zone
    {
        sTiming.uwInsideLar  = 0;
        iInterceptLar = 0;
        iInterceptExclRegion = 0;
    }
    else if (sLarSub.dGndDistToLAR_m <= sLarSub.dLarRadius_m)// Inside LAR
    {
        sTiming.uwInsideLar  = 1;
        iInterceptLar = 0;

        if ((sLarSub.dGndDistToTgt_m * sin(fabs(sLarSub.dRelBearToTgt_rad))) >= sLarSub.dExclRadius_m)
        {
            iInterceptExclRegion = 0;
        }
        else
        {
            iInterceptExclRegion = 1;
        }
    }
    else // Outside LAR
    {
        sTiming.uwInsideLar = 0;
        if (((sLarSub.dGndDistToLAR_m * sin(fabs(sLarSub.dRelBearToLAR))) >= sLarSub.dLarRadius_m) ||
                (fabs(sLarSub.dRelBearToLAR) >= M_Pi/2))
        {
            iInterceptLar = 0;
        }
        else
        {
            iInterceptLar = 1;
        }
        if (((sLarSub.dGndDistToTgt_m * sin(fabs(sLarSub.dRelBearToTgt_rad))) >= sLarSub.dExclRadius_m) ||
                (fabs(sLarSub.dRelBearToTgt_rad) >= M_Pi/2))
        {
            iInterceptExclRegion = 0;
        }
        else
        {
            iInterceptExclRegion = 1;
        }
    }

    // Determine distances of intercepts
    double exd = 0.0, exd1 = 0.0, exd2 = 0.0;
    double d = 0.0, c = 0.0;
    double a1 = 0.0, a2 = 0.0, c1 = 0.0, c2 = 0.0 ,ent = 0.0;
    if (sTiming.uwInsideLar)    // get exit distance
        // get distance to edge of LAR
    {
        exd1 = sLarSub.dGndDistToLAR_m*cos(sLarSub.dRelBearToLAR) +
                sqrt(pow(sLarSub.dGndDistToLAR_m,2) * (pow(cos(sLarSub.dRelBearToLAR),2)-1) + pow(sLarSub.dLarRadius_m,2));

        if (iInterceptExclRegion)   // get distance to edge of exclusion zone
            if (fabs(sLarSub.dRelBearToTgt_rad) < 0.015)
            {
                exd2 = sLarSub.dGndDistToTgt_m - sLarSub.dExclRadius_m;
            }
            else
            {
                d = sLarSub.dExclRadius_m/sin(fabs(sLarSub.dRelBearToTgt_rad));
                c = asin(sLarSub.dGndDistToTgt_m/d) - fabs(sLarSub.dRelBearToTgt_rad);
                exd2 = d*sin(c);
            }
        else
        {
            exd2 = exd1*2;  // artificially make it further than LAR edge
        }

        exd = (exd1>exd2)?exd2:exd1; // select closest one
    }
    else if (iInterceptLar)
    {
        // get distances to edges of LAR (enter/exit LAR)
        if (fabs(sLarSub.dRelBearToLAR) < 0.015)
        {
            ent  = sLarSub.dGndDistToLAR_m - sLarSub.dLarRadius_m;
            exd1 = sLarSub.dGndDistToLAR_m + sLarSub.dLarRadius_m;
            a2   = 0;
        }
        else
        {
            d = sLarSub.dLarRadius_m/sin(fabs(sLarSub.dRelBearToLAR));
            a2 = asin(sLarSub.dGndDistToLAR_m/d);
            a1 = M_Pi - a2;
            c1 = M_Pi - fabs(sLarSub.dRelBearToLAR) - a1;
            c2 = M_Pi - fabs(sLarSub.dRelBearToLAR) - a2;
            ent  = d*sin(c1);
            exd1 = d*sin(c2);
        }

        if (iInterceptExclRegion)   // get distance to edge of exclusion zone
        {
            if (fabs(sLarSub.dRelBearToTgt_rad) < 0.015)
            {
                exd2 = sLarSub.dGndDistToTgt_m - sLarSub.dExclRadius_m;
            }
            else
            {
                d = sLarSub.dExclRadius_m/sin(fabs(sLarSub.dRelBearToTgt_rad));
                c = asin(sLarSub.dGndDistToTgt_m/d) - fabs(sLarSub.dRelBearToTgt_rad);
                exd2 = d*sin(c);
            }
        }
        else
        {
            exd2 = exd1*2;  // artificially make it further than LAR edge
        }
        exd = (exd1>exd2)?exd2:exd1; // select closest one
    }

    //  Calculate all timings
    if (sTiming.uwInsideLar)
    {
        // Time to Go is always 0 inside the LAR
        sTiming.dTimeToGo = 0;
        // Calculate time to exit (always valid inside the LAR)
        sTiming.dTimeToExit = exd/dGroundSpeed;
        // Lookup flight time (always valid inside the LAR)
        double dAngle = (M_Pi - sLarSub.dRelBearToLAR)*180/M_Pi;
        double dRatio = sLarSub.dGndDistToLAR_m/sLarSub.dLarRadius_m;
        sTiming.dTimeToImpact = interpolate2dLutTiming(daTofLut_,&dTofColumns[0], &dTofRows[0],
                (sizeof(dTofColumns)/sizeof(dTofColumns[0])),
                (sizeof(dTofRows)/sizeof(dTofRows[0])),dAngle, dRatio);
        // Calculate Illuminate/Shine Time (always valid inside the LAR)
        sTiming.dTimeToIlluminate = (0 > sTiming.dTimeToImpact-45)?0:sTiming.dTimeToImpact-45;
    }
    else if (iInterceptLar)
    {
        // Calculate time to go
        sTiming.dTimeToGo   = ent/dGroundSpeed;
        // Calculate time to exit
        sTiming.dTimeToExit   = exd/dGroundSpeed;
        // Lookup flight time (on entry of LAR)
        int32_t iSign;
        if (sLarSub.dRelBearToLAR > 0)
            iSign = 1;
        else if (sLarSub.dRelBearToLAR < 0)
            iSign = -1;
        else
            iSign = 0;
        double arat = (M_Pi - iSign*a2)*180/M_Pi;
        double ti1 = interpolate2dLutTiming(daTofLut_, &dTofColumns[0], &dTofRows[0],
                (sizeof(dTofColumns)/sizeof(dTofColumns[0])),
                (sizeof(dTofRows)/sizeof(dTofRows[0])),arat, 1);
        sTiming.dTimeToImpact  = sTiming.dTimeToGo + ti1;
        // Calculate Illuminate/Shine Time
        sTiming.dTimeToIlluminate  = (0 > sTiming.dTimeToImpact-45)?0:sTiming.dTimeToImpact-45;
    }
    else    // not inside and not going to intercept LAR, so all 0
    {
        sTiming.dTimeToGo  = 0;
        sTiming.dTimeToExit  = 0;
        sTiming.dTimeToImpact  = 0;
        sTiming.dTimeToIlluminate  = 0;
    }
    return sTiming;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : interpolate2dLutTiming
 *
 * DESCRIPTION          : Interpolation for 2-D gridded data in meshgrid format
 *
 * INPUT PARAMETERS     : double daLutArray[2][5],
 *                        const double dapColums[],
 *                        const double dapRow[],
 *                        int32_t iColSize,
 *                        int32_t iRowSize,
 *                        double dColValue,
 *                        double dRowValue)
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double -> interpolated value from lookup table
 *
 * REMARKS              :
 *
 *    Double interpolation Ratio eq:
 *
 *    (f(Xt) - f(X1))/(f(X2) - f(X1)) = (Xt - X1)/(X2 - X1)
 *
 *    (Step 1) -> X_ratio = (Xt - X1)/(X2 - X1)
 *    (Step 2) -> f(Xt) = f(X1) + (f(X2) - f(X1)) * X_ratio
 *    (Step 3) -> g(Xt) = g(X1) + (g(X2) - g(X1)) * X_ratio
 *    (Step 4) -> Y_ratio = (Yt - Y1)/(Y2 - Y1)
 *    (Step 5) -> h(Yt) = f(Xt) + (g(Xt) - f(Xt)) * Y_ratio
 *
 *    |-----------|-------------|-------|------------|
 *    |           | 15000 (X1)  |   Xt  | 20000 (X2) |
 *    |-----------|-------------|-------|------------|
 *    |  0.7 (Y1) | 7943  f(X1) | f(Xt) | 9240 f(X2) |
 *    |    Yt     |             | h(Yt) |            |
 *    |  0.8 (Y2) | 8993  g(X1) | g(Xt) | 10739 g(X2)|
 *    |-----------|-------------|-------|------------|
 *
 *----------------------------------------------------------------------------*/
double interpolate2dLutTiming(double daLutArray[2][5],
const double dapColums[],
const double dapRow[],
int32_t iColSize,
int32_t iRowSize,
double dColValue,
double dRowValue)
{
    //rows and colums should be at least larger than two
    double f_Xt;
    double g_Xt;
    double dRow_Ratio, dCol_Ratio;
    double h_Yt = 0;
    double dNormilisedCol;
    uint32_t uiColIndex;
    double dNormilisedRow;
    double dDeltaRow, dDeltaCol;
    uint32_t uiRowIndex = 0;
    bool flagContinue = true;

    if ((dRowValue > dapRow[iRowSize-1]))
        dRowValue = dapRow[iRowSize-1];
    else if ((dRowValue < dapRow[0]))
        dRowValue = dapRow[0];
    //determine row index
    dDeltaRow = (dapRow[1] - dapRow[0]);
    if (dDeltaRow > 0.0)
    {
        uint32_t uiOffsetRow = (uint32_t)(dapRow[0] / dDeltaRow + 0.5); //Offset to first row
        dNormilisedRow = dRowValue /dDeltaRow - uiOffsetRow;
        uiRowIndex = (uint32_t)(dNormilisedRow); //ALWAYS FLOOR
        //Index at end of table
        if (uiRowIndex >= iRowSize-1)
            uiRowIndex = iRowSize-2;
    }
    else
    {
        flagContinue = false;
    }

    if ((dColValue > dapColums[iColSize-1]))
        dColValue = dapColums[iColSize-1];
    else if ((dColValue < dapColums[0]))
        dColValue = dapColums[0];
    //Determin Colomn index
    dDeltaCol = (dapColums[1] - dapColums[0]);
    if (dDeltaCol > 0.0)
    {
        uint32_t uiOffsetCol = (uint32_t)(dapColums[0] / dDeltaCol + 0.5); //Offset to first row
        dNormilisedCol = dColValue / dDeltaCol - uiOffsetCol;
        uiColIndex = (uint32_t)(dNormilisedCol); //ALWAYS FLOOR
        //Index at end of table
        if (uiColIndex >= iColSize - 1)
            uiColIndex = iColSize - 2;
    }
    else
    {
        flagContinue = false;
    }

    if (flagContinue)
    {
        // 2D Interpolation
        //(Step 1)
        dCol_Ratio = (dColValue - dapColums[uiColIndex]) / (dapColums[uiColIndex+1] - dapColums[uiColIndex]);
        //(Step 2)
        f_Xt = daLutArray[uiRowIndex][uiColIndex] +
                (daLutArray[uiRowIndex][uiColIndex+1] - daLutArray[uiRowIndex][uiColIndex]) *
                dCol_Ratio;
        //(Step 3)
        g_Xt = daLutArray[uiRowIndex+1][uiColIndex] +
                (daLutArray[uiRowIndex+1][uiColIndex+1] - daLutArray[uiRowIndex+1][uiColIndex]) *
                dCol_Ratio;
        //(Step 4)
        dRow_Ratio = (dRowValue - dapRow[uiRowIndex]) / (dapRow[uiRowIndex+1] - dapRow[uiRowIndex]);
        //(Step 5)
        h_Yt =  f_Xt + (g_Xt - f_Xt) * dRow_Ratio;
    }
    return h_Yt;
}
/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : interpolate2dLutBalRange
 *
 * DESCRIPTION          : Interpolation for 2-D gridded data in meshgrid format
 *
 * INPUT PARAMETERS     : double daLutArray[3][8],
 *                        const double dapColums[],
 *                        const double dapRow[],
 *                        int32_t iColSize,
 *                        int32_t iRowSize,
 *                        double dColValue,
 *                        double dRowValue)
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double -> interpolated value from lookup table
 *
 * REMARKS              :
 *
 *    Double interpolation Ratio eq:
 *
 *    (f(Xt) - f(X1))/(f(X2) - f(X1)) = (Xt - X1)/(X2 - X1)
 *
 *    (Step 1) -> X_ratio = (Xt - X1)/(X2 - X1)
 *    (Step 2) -> f(Xt) = f(X1) + (f(X2) - f(X1)) * X_ratio
 *    (Step 3) -> g(Xt) = g(X1) + (g(X2) - g(X1)) * X_ratio
 *    (Step 4) -> Y_ratio = (Yt - Y1)/(Y2 - Y1)
 *    (Step 5) -> h(Yt) = f(Xt) + (g(Xt) - f(Xt)) * Y_ratio
 *
 *    |-----------|-------------|-------|------------|
 *    |           | 15000 (X1)  |   Xt  | 20000 (X2) |
 *    |-----------|-------------|-------|------------|
 *    |  0.7 (Y1) | 7943  f(X1) | f(Xt) | 9240 f(X2) |
 *    |    Yt     |             | h(Yt) |            |
 *    |  0.8 (Y2) | 8993  g(X1) | g(Xt) | 10739 g(X2)|
 *    |-----------|-------------|-------|------------|
 *
 *----------------------------------------------------------------------------*/
double interpolate2dLutBalRange(const double daLutArray[3][8],
const double dapColums[],
const double dapRow[],
int32_t iColSize,
int32_t iRowSize,
double dColValue,
double dRowValue)
{

    //rows and colums should be at least larger than two
    double f_Xt;
    double g_Xt;
    double dRow_Ratio, dCol_Ratio;
    double h_Yt = 0;
    double dNormilisedCol;
    uint32_t uiColIndex;
    double dNormilisedRow;
    double dDeltaRow, dDeltaCol;
    uint32_t uiRowIndex = 0;
    bool flagContinue = true;

    if ((dRowValue > dapRow[iRowSize-1]))
        dRowValue = dapRow[iRowSize-1];
    else if ((dRowValue < dapRow[0]))
        dRowValue = dapRow[0];
    //determine row index
    dDeltaRow = (dapRow[1] - dapRow[0]);
    if (dDeltaRow > 0.0)
    {
        uint32_t uiOffsetRow = (uint32_t)(dapRow[0] / dDeltaRow + 0.5); //Offset to first row
        dNormilisedRow = dRowValue /dDeltaRow - uiOffsetRow;
        uiRowIndex = (uint32_t)(dNormilisedRow); //ALWAYS FLOOR
        //Index at end of table
        if (uiRowIndex >= iRowSize-1)
            uiRowIndex = iRowSize-2;
    }
    else
    {
        flagContinue = false;
    }

    if ((dColValue > dapColums[iColSize-1]))
        dColValue = dapColums[iColSize-1];
    else if ((dColValue < dapColums[0]))
        dColValue = dapColums[0];
    //Determin Colomn index
    dDeltaCol = (dapColums[1] - dapColums[0]);
    if (dDeltaCol > 0.0)
    {
        uint32_t uiOffsetCol = (uint32_t)(dapColums[0] / dDeltaCol + 0.5); //Offset to first row
        dNormilisedCol = dColValue / dDeltaCol - uiOffsetCol;
        uiColIndex = (uint32_t)(dNormilisedCol); //ALWAYS FLOOR
        //Index at end of table
        if (uiColIndex >= iColSize - 1)
            uiColIndex = iColSize - 2;
    }
    else
    {
        flagContinue = false;
    }

    if (flagContinue)
    {
        // 2D Interpolation
        //(Step 1)
        dCol_Ratio = (dColValue - dapColums[uiColIndex]) / (dapColums[uiColIndex+1] - dapColums[uiColIndex]);
        //(Step 2)
        f_Xt = daLutArray[uiRowIndex][uiColIndex] +
                (daLutArray[uiRowIndex][uiColIndex+1] - daLutArray[uiRowIndex][uiColIndex]) *
                dCol_Ratio;
        //(Step 3)
        g_Xt = daLutArray[uiRowIndex+1][uiColIndex] +
                (daLutArray[uiRowIndex+1][uiColIndex+1] - daLutArray[uiRowIndex+1][uiColIndex]) *
                dCol_Ratio;
        //(Step 4)
        dRow_Ratio = (dRowValue - dapRow[uiRowIndex]) / (dapRow[uiRowIndex+1] - dapRow[uiRowIndex]);
        //(Step 5)
        h_Yt =  f_Xt + (g_Xt - f_Xt) * dRow_Ratio;
    }
    return h_Yt;
}
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
int32_t calculateP32Lar(sLaunchParams_t sLaunchParams,sTgtParams_t sTgtParams, sLar_t *spLar)
{
   int32_t iReturn = 1;
    if (spLar)
    {
        sLar_t sLar;
        memset(&sLar,0,sizeof(sLar_t));
        sLarPoints_t sTof;
        double dDistToLarCenter_m, dLarRadius_m, dExcludeRadius_m;
        uint16_t uwDiveAngle_deg = (uint16_t)(round(sTgtParams.dDiveAng_rad*180/M_Pi));

        // Do raw LAR lookup
        //----------------------------------------------
        dDistToLarCenter_m = getDistLarCenter_m(sTgtParams.dAlt_m, sLaunchParams.dAlt_m, sLaunchParams.dMach, uwDiveAngle_deg);
        dLarRadius_m = getLarRadius_m(sTgtParams.dAlt_m, sLaunchParams.dAlt_m, sLaunchParams.dMach, uwDiveAngle_deg);
        dExcludeRadius_m = getExclRegionRadius_m(sTgtParams.dAlt_m, sLaunchParams.dAlt_m, sLaunchParams.dMach, uwDiveAngle_deg);

        if ((round(dLarRadius_m) == -1.0 ) || (round(dExcludeRadius_m) == -1.0 ))
        {
            //invalid solution, stop calculation
            iReturn = 1;
        }
        else
        {
            sTof = getSlantRanges(dDistToLarCenter_m, dLarRadius_m, sLaunchParams.dAlt_m - sTgtParams.dAlt_m, sLaunchParams.dMach);
            sLar.dExclRadius_m = dExcludeRadius_m;

            // Update radial LUT for time of flight (TOF)
            //----------------------------------------------
            // Row 0:
            for (int32_t i = 0; i < 5; i++)
            {
                daTofLut_[0][i] = sTof.dPoint1;
            }
            // Row 1:
            daTofLut_[1][0] = sTof.dPoint2;
            daTofLut_[1][1] = sTof.dPoint3;
            daTofLut_[1][2] = sTof.dPoint4;
            daTofLut_[1][3] = sTof.dPoint5;
            daTofLut_[1][4] = sTof.dPoint2;


            // Use Target as origin
            //----------------------------------------------
            sLLH_t sLLH_Tgt;
            sLLH_Tgt.dLat_rad = sTgtParams.dLat_rad;
            sLLH_Tgt.dLong_rad = sTgtParams.dLong_rad;
            sLLH_Tgt.dHeight_m = sTgtParams.dAlt_m;
            sXYZ_t sXYZ_Origin = convertLlhToEcef(sLLH_Tgt);

            // Convert release point to LLLN
            //----------------------------------------------
            sLLH_t sLLH_Launch;
            sLLH_Launch.dLat_rad = sLaunchParams.dLat_rad;
            sLLH_Launch.dLong_rad = sLaunchParams.dLong_rad;
            sLLH_Launch.dHeight_m = sLaunchParams.dAlt_m;
            sXYZ_t sXYZ_ReleasePoint = convertLlhToEcef(sLLH_Launch);
            sENU_t sENU_ReleasePoint = convertEcefToEnu(sXYZ_ReleasePoint,sXYZ_Origin);
            sLar.dGndDistToTgt_m = sqrt(pow(sENU_ReleasePoint.dEast,2)+pow(sENU_ReleasePoint.dNorth,2));
            sLar.dRelBearToTgt_rad = atan2(-sENU_ReleasePoint.dEast, -sENU_ReleasePoint.dNorth) - sLaunchParams.dTrack_rad;
            while ( sLar.dRelBearToTgt_rad < -1 * M_Pi )
                sLar.dRelBearToTgt_rad += 2*M_Pi;
            while( sLar.dRelBearToTgt_rad > M_Pi )
                sLar.dRelBearToTgt_rad -= 2*M_Pi;
            // Adjust LAR due to wind
            //----------------------------------------------
            double dWindSpeed_m_s = 0;
            double dWindDir_rad = 0;
            double dAltAboveTgt_ft = sLaunchParams.dAlt_m/0.3048 - sTgtParams.dAlt_m/0.3048;
            if (sLaunchParams.uiWindValidFlag == 0)
            {
                //If wind is not valid, a 100% head wind penalty shall be applied
                dWindSpeed_m_s = ((1.1759 * dAltAboveTgt_ft) / 1000.0 ) + 5.145;
                //limit between 12.86 and 51.44
                if (dWindSpeed_m_s < 12.86)
                    dWindSpeed_m_s = 12.86;
                if (dWindSpeed_m_s > 51.44)
                    dWindSpeed_m_s = 51.44;
                dWindDir_rad = -(sLaunchParams.dTrack_rad + sLar.dRelBearToTgt_rad);
            }
            else
            {
                dWindSpeed_m_s = sLaunchParams.dWindSpeed_m_s;
                dWindDir_rad = sLaunchParams.dWindDir_rad;
            }

            double dWindFactor = 0.5;
            double daPointsX[5] = {0, 0, dLarRadius_m, 0, -dLarRadius_m};
            double daPointsY[5] = {-dDistToLarCenter_m, -dDistToLarCenter_m+dLarRadius_m, -dDistToLarCenter_m, -dDistToLarCenter_m-dLarRadius_m, -dDistToLarCenter_m};
            double daTof[5] = {sTof.dPoint1, sTof.dPoint2, sTof.dPoint3, sTof.dPoint4, sTof.dPoint5};

            double px1[5];
            double py1[5];
            for (int32_t i = 0; i < 5; i++)
            {
                px1[i] = daPointsX[i] + dWindFactor * dWindSpeed_m_s * sin(dWindDir_rad - sLaunchParams.dTrack_rad) * daTof[i];
                py1[i] = daPointsY[i] + dWindFactor * dWindSpeed_m_s * cos(dWindDir_rad - sLaunchParams.dTrack_rad) * daTof[i];
            }
            double dNewLarRadX = (px1[2]-px1[4])/2;
            double dNewLarRadY = (py1[1]-py1[3])/2;
            double dNewLarPosX = (px1[2]+px1[4])/2;
            double dNewLarPosY = (py1[1]+py1[3])/2;
            double dNewLarRadius = (dNewLarRadX>dNewLarRadY)?dNewLarRadY:dNewLarRadX;

            // Determine LAR center in LLLN using release track
            sENU_t sENU_NewLar;
            sENU_NewLar.dNorth = -dNewLarPosX * sin(sLaunchParams.dTrack_rad) + dNewLarPosY * cos(sLaunchParams.dTrack_rad);
            sENU_NewLar.dEast =  dNewLarPosX * cos(sLaunchParams.dTrack_rad) + dNewLarPosY * sin(sLaunchParams.dTrack_rad);
            sENU_NewLar.dUp = sTgtParams.dAlt_m;
            sLar.dLarRadius_m = dNewLarRadius;

            sLar.dGndDistToLAR_m = sqrt(pow((sENU_NewLar.dEast - sENU_ReleasePoint.dEast),2) +
                                        pow((sENU_NewLar.dNorth - sENU_ReleasePoint.dNorth),2));
            sLar.dRelBearToLAR = atan2(sENU_NewLar.dEast - sENU_ReleasePoint.dEast,
                                       sENU_NewLar.dNorth - sENU_ReleasePoint.dNorth) - sLaunchParams.dTrack_rad;
            while ( sLar.dRelBearToLAR < -M_Pi )
                sLar.dRelBearToLAR += 2*M_Pi;
            while( sLar.dRelBearToLAR > M_Pi )
                sLar.dRelBearToLAR -= 2*M_Pi;


            //Relative distances (Used in LAR display)
            sLar.dRelX_DistToLar = sLar.dGndDistToLAR_m * sin(sLar.dRelBearToLAR);
            sLar.dRelY_DistToLar = sLar.dGndDistToLAR_m * cos(sLar.dRelBearToLAR);

            //exclusion radius shall be considered valid if there is an
            //overlap between the exclusion zone and the LAR
            if (( sqrt((pow(sENU_NewLar.dNorth,2) + pow(sENU_NewLar.dEast,2))) - sLar.dLarRadius_m) < sLar.dExclRadius_m )
            {
                sLar.uwExclRadValidFlag = 1;
            }
            else
            {
                sLar.uwExclRadValidFlag = 0;
            }
            // Convert LAR to LLA
            sXYZ_t sXYZ_NewLar = convertEnuToEcef(sENU_NewLar,sXYZ_Origin);
            sLLH_t sLLH_NewLar = convertEcefToLlh(sXYZ_NewLar);
            sLar.dLarCtrLat_rad     = sLLH_NewLar.dLat_rad;
            sLar.dLarCtrLong_rad    = sLLH_NewLar.dLong_rad;

            // Calculate timings
            double dTas  = convertMachToTas(sLaunchParams.dMach, sLaunchParams.dAlt_m);
            double dGndSpeed_m_s = dTas - dWindSpeed_m_s * cos(sLaunchParams.dTrack_rad - dWindDir_rad);

            double dNorm = sqrt(pow(dNewLarPosX,2) + pow(dNewLarPosY,2));
            if ((dNorm+dNewLarRadius) <= dExcludeRadius_m)  // lar inside exclusion zone
            {
                sLar.uwInsideLar   = 0;
                sLar.dTimeToGo_s     = 0;
                sLar.dTimeToExit_s   = 0;
                sLar.dTimeToImpact_s = 0;
                sLar.dTimeToIllum_s  = 0;
            }
            else
            {
                sLarSubSet_t sLarSubSet;
                sLarSubSet.dExclRadius_m = sLar.dExclRadius_m;
                sLarSubSet.dGndDistToLAR_m = sLar.dGndDistToLAR_m;
                sLarSubSet.dGndDistToTgt_m = sLar.dGndDistToTgt_m;
                sLarSubSet.dLarRadius_m = sLar.dLarRadius_m;
                sLarSubSet.dRelBearToLAR = sLar.dRelBearToLAR;
                sLarSubSet.dRelBearToTgt_rad = sLar.dRelBearToTgt_rad;
                sTimingParams ct_data = calculateP32Timings(dGndSpeed_m_s, sLarSubSet);
                sLar.uwInsideLar = ct_data.uwInsideLar;
                sLar.dTimeToGo_s = ct_data.dTimeToGo;
                sLar.dTimeToExit_s = ct_data.dTimeToExit;
                sLar.dTimeToImpact_s = ct_data.dTimeToImpact;
                sLar.dTimeToIllum_s = ct_data.dTimeToIlluminate;
            }
            //Calculate estimate trajectory
            double dPoint0_x;
            double dPoint0_y;
            double dPoint1_x = 0.0;
            double dPoint1_y = 0.0;
            double daSteps[32];
            double daEstTrajPoints[2][32];
            if (sLar.uwInsideLar)
            {
                // Estimated Trajectory
                for (int32_t i = 0; i < 32; i++)
                {
                    daSteps[i] = (i + 1)/32.0;
                }
                dPoint0_x = sENU_ReleasePoint.dEast;
                dPoint0_y = sENU_ReleasePoint.dNorth;
                dPoint1_x = dPoint0_x + 5000 * sin(sLaunchParams.dTrack_rad);
                dPoint1_y = dPoint0_y + 5000 * cos(sLaunchParams.dTrack_rad);

                for (int32_t i = 0; i < 32; i++)
                {
                    daEstTrajPoints[0][i]  = pow((1-daSteps[i]),2) * dPoint0_x + 2 * (1-daSteps[i]) * daSteps[i] * dPoint1_x;
                    daEstTrajPoints[1][i]  = pow((1-daSteps[i]),2) * dPoint0_y + 2 * (1-daSteps[i]) * daSteps[i] * dPoint1_y;
                }

                for (int32_t i = 0; i < 32; i++)
                {
                    sENU_t sENU;
                    sENU.dEast = daEstTrajPoints[0][i];
                    sENU.dNorth = daEstTrajPoints[1][i];
                    sENU.dUp = 0;

                    sXYZ_t sBxyz = convertEnuToEcef(sENU,sXYZ_Origin);
                    sLLH_t sBllh = convertEcefToLlh(sBxyz);
                    sLar.dTrajLat_rad[i] = sBllh.dLat_rad;
                    sLar.dTrajLong_rad[i] = sBllh.dLong_rad;
                }
                // Calculate estimated end conditions
                // Estimated Ground Range
                double dTemp = 0;
                dTemp += sqrt(pow((daEstTrajPoints[0][0] - dPoint0_x),2) +
                        pow((daEstTrajPoints[1][0] - dPoint0_y),2));
                for (int32_t i = 1; i < 32; i++)
                {
                    dTemp += sqrt(pow((daEstTrajPoints[0][i] - daEstTrajPoints[0][i-1]),2) +
                            pow((daEstTrajPoints[1][i] - daEstTrajPoints[1][i-1]),2));
                }
                sLar.dEstGroundRange_m = dTemp;
                // Estimated Impact Speed
                double dAltScaled_ft = sLaunchParams.dAlt_m/0.3048/1000;
                double dGndDistToTgt_km = sLar.dGndDistToTgt_m/1000;
                sLar.dEstImpactSpeed_m_s   = (0.0136 * pow(dAltScaled_ft,2) - 0.473 * dAltScaled_ft - 6.66)* dGndDistToTgt_km
                        + (-0.452 * pow(dAltScaled_ft,2) + 23.98 * dAltScaled_ft + 160);
                // Estimated Attack Heading
                sLar.dEstImpactTrack_rad = atan2(-dPoint1_x,-dPoint1_y);
                // Estimated Impact Dive Angle
                sLar.dEstImpactDiveAng_rad = sTgtParams.dDiveAng_rad;
            }
            else //Outside LAR
            {
                for (int32_t i = 0; i < 32; i++)
                {
                    sLar.dTrajLat_rad[i] = sLaunchParams.dLat_rad;
                    sLar.dTrajLong_rad[i] = sLaunchParams.dLong_rad;
                }
                sLar.dEstGroundRange_m = 0;
                sLar.dEstImpactSpeed_m_s   = 0;
                sLar.dEstImpactTrack_rad = 0;
                sLar.dEstImpactDiveAng_rad  = 0;
            }

            //Calculate ballistic values for given input
            sLar.dBallisticRange_m = getBallisticRange_m(sTgtParams.dAlt_m/0.3048,
                                                         sLaunchParams.dAlt_m/0.3048,
                                                         sLaunchParams.dMach);
            sLar.dBallisticFlightTime_s = getBallisticFlightTime_s(sTgtParams.dAlt_m/0.3048,
                                                         sLaunchParams.dAlt_m/0.3048);

            *spLar = sLar;
            iReturn = 0;
        }
    }

    return iReturn;
}

/*--[ FUNCTION ]----------------------------------------------------------------
 *
 * FUNCTION NAME        : getBallisticFlightTime_s
 *
 * DESCRIPTION          : Calculate the ballistic flight time
 *
 * INPUT PARAMETERS     : double dTgtAlt_ft, double dAircraftAlt_ft
 *
 * OUTPUT PARAMETERS    : NONE
 *
 * RETURN PARAMETERS    : double - Ballistic flight time in seconds
 *
 * REMARKS              : NONE
 *
 *----------------------------------------------------------------------------*/
double getBallisticFlightTime_s(double dTgtAlt_ft, double dAircraftAlt_ft)
{
    double dX = (dAircraftAlt_ft - dTgtAlt_ft)/1000.0;
    double dBalFlightTime = (-1)*(0.012*(pow(dX,2))) + 1.334 * dX + 1.486;
    return dBalFlightTime;
}

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
                                 double dAircraftSpeed_mach, sBallisticParams_t *spParams)
{
    int32_t iReturn = 1;
    if (spParams)
    {
        sBallisticParams_t sParams;
        double dTgtAlt_ft = dTgtAlt_m/0.3048;
        double dAircraftAlt_ft = dAircraftAlt_m/0.3048;
        sParams.dBallisticFlightTime_s = getBallisticFlightTime_s(dTgtAlt_ft, dAircraftAlt_ft);
        sParams.dBallisticRange_m = getBallisticRange_m(dTgtAlt_ft, dAircraftAlt_ft,dAircraftSpeed_mach);
        *spParams = sParams;
        iReturn = 0;
    }
    return iReturn;
}
