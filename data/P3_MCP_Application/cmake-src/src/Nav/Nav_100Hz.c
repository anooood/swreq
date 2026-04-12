/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <NAV_100Hz.c>
 *
 *  Description:
 *  ------------
 *  NAV_100Hz Module.
 *
 *  Function(s):
 *  ------------
 *  - NAV_100HzInit                            : Initializes Navigation 100Hz
 *  - NAV_100HzExecute                         : Executes Navigation 100Hz
 *  ...
 *
 *
 ******************************************************************************/

#include "NAV_DoNavigationMain_100Hz.h"

static RT_MODEL NAV_100Hz_M;

void NAV_100HzInit(void)
{
  memset(&NAV_100Hz_M, 0, sizeof(NAV_100Hz_M));
  NAV_DoNavigationMain_100Hz_initialize(&NAV_100Hz_M);
}

void NAV_100HzExecute(T_NAV_GenericInertialNavigationSensor *INS        ,
                      T_GNSS_Result                         *GNSS_Result,
                      T_TA_TransferAlignState               *TA_State   ,
                      T_NAV_KFilterErrorEstimateState       *ErrorEst   ,
                      T_NAV_State                           *NavState   ,
                      T_NAV_Tel100Hz                        *Tel100Hz   )
{
  uint16_T i;
  
  static uint32_T NAV_100HzCount      = 0;
  static uint32_T NAV_Align_CallCount = 0;

  NAV_DoNavigation_100Hz(&NAV_100Hz_M, INS, TA_State, ErrorEst, GNSS_Result, NavState);

  NAV_100HzCount++;
  NAV_Align_CallCount++;
  
  Tel100Hz->T100B1.NAV_SystemTime        = (uint32_T)(INS->SystemTime                           * 1e6);
  Tel100Hz->T100B1.NAV_baro_SystemTime   = (uint32_T)(INS->baro_SystemTime                      * 1e6);
  Tel100Hz->T100B1.NAV_baro_alt          = (real32_T) INS->baro_alt                                   ;
  Tel100Hz->T100B1.NAV_baro_pressure     = (real32_T) INS->baro_pressure                              ;
  Tel100Hz->T100B1.NAV_Reserved1         =            0                                               ;
  Tel100Hz->T100B1.NAV_Reserved2         =            0                                               ;
  Tel100Hz->T100B1.NAV_Reserved3         =            0                                               ;
  Tel100Hz->T100B1.NAV_100HzTimestamp    =            0                                               ;
  Tel100Hz->T100B1.NAV_100HzCount        =            NAV_100HzCount                                  ;
  Tel100Hz->T100B1.NAV_AngPsi            = (real32_T) NavState->NAV_NavigationState.att_bn.psi        ;
  Tel100Hz->T100B1.NAV_AngTheta          = (real32_T) NavState->NAV_NavigationState.att_bn.the        ;
  Tel100Hz->T100B1.NAV_AngPhi            = (real32_T) NavState->NAV_NavigationState.att_bn.phi        ;
  Tel100Hz->T100B1.NAV_PosXecef          = (int32_T )(NavState->NAV_NavigationState.pos_e.x   * 256.0);
  Tel100Hz->T100B1.NAV_PosYecef          = (int32_T )(NavState->NAV_NavigationState.pos_e.y   * 256.0);
  Tel100Hz->T100B1.NAV_PosZecef          = (int32_T )(NavState->NAV_NavigationState.pos_e.z   * 256.0);
  Tel100Hz->T100B1.NAV_VelXecef          = (real32_T) NavState->NAV_NavigationState.vel_e.x           ;
  Tel100Hz->T100B1.NAV_VelYecef          = (real32_T) NavState->NAV_NavigationState.vel_e.y           ;
  Tel100Hz->T100B1.NAV_VelZecef          = (real32_T) NavState->NAV_NavigationState.vel_e.z           ;
  Tel100Hz->T100B1.NAV_LatLLA            = (real32_T) NavState->NAV_NavigationState.pos_g.lat         ;
  Tel100Hz->T100B1.NAV_LongLLA           = (real32_T) NavState->NAV_NavigationState.pos_g.lon         ;
  Tel100Hz->T100B1.NAV_AltLLA            = (real32_T) NavState->NAV_NavigationState.pos_g.alt         ;
  Tel100Hz->T100B1.NAV_PressAlt          = (real32_T) NavState->NAV_NavigationState.baro_alt          ;
  Tel100Hz->T100B1.NAV_Mode              = (uint16_T) NavState->NavigationModeOut                     ;
  Tel100Hz->T100B1.NAV_Version           = (uint16_T) NavState->versionNumberNAV                      ;
  Tel100Hz->T100B1.NAV_Align_Timestamp   = (uint32_T)(TA_State->time.attitude                   * 1e6);
  Tel100Hz->T100B1.NAV_Align_CallCount   =            NAV_Align_CallCount                             ;
  Tel100Hz->T100B1.NAV_Align_phi         = (real32_T) TA_State->attitude.phi                          ;
  Tel100Hz->T100B1.NAV_Align_the         = (real32_T) TA_State->attitude.the                          ;
  Tel100Hz->T100B1.NAV_Align_psi         = (real32_T) TA_State->attitude.psi                          ;
  Tel100Hz->T100B1.NAV_Align_pos_x       =            TA_State->pos_e.x                               ;
  Tel100Hz->T100B1.NAV_Align_pos_y       =            TA_State->pos_e.y                               ;
  Tel100Hz->T100B1.NAV_Align_pos_z       =            TA_State->pos_e.z                               ;
  Tel100Hz->T100B1.NAV_Align_vel_x       = (real32_T) TA_State->vel_e.x                               ;
  Tel100Hz->T100B1.NAV_Align_vel_y       = (real32_T) TA_State->vel_e.y                               ;
  Tel100Hz->T100B1.NAV_Align_vel_z       = (real32_T) TA_State->vel_e.z                               ;
  Tel100Hz->T100B1.NAV_Align_valid_pos   =            TA_State->valid                                 ;
  Tel100Hz->T100B1.NAV_Align_valid_vel   =            TA_State->valid                                 ;
  Tel100Hz->T100B1.NAV_Align_valid_euler =            TA_State->valid                                 ;
  Tel100Hz->T100B1.NAV_Align_valid       =            NavState->NAV_KalmanFilterState.align_valid     ;
  Tel100Hz->T100B1.NAV_Align_Reserved1   =            0                                               ;
  Tel100Hz->T100B1.NAV_Align_Reserved2   =            0                                               ;

  for(i=0; i<16; i++)
  {
    Tel100Hz->T100B1.NAV_CovMatrix[i] = (real32_T)NavState->NAV_KalmanFilterState.P[i*16+i];
    Tel100Hz->T100B1.NAV_xhatneg_K[i] = (real32_T)NavState->NAV_KalmanFilterState.X[i]     ;
  }

  for(i=0; i<3; i++)
  {
    Tel100Hz->T100B1.NAV_BiasGyro [i] = (real32_T)NavState->NAV_KalmanFilterState.B_gyro [i];
    Tel100Hz->T100B1.NAV_BiasAccel[i] = (real32_T)NavState->NAV_KalmanFilterState.B_accel[i];
  }

  Tel100Hz->T100B1.NAV_BiasBaroAlt = (real32_T)NavState->NAV_BaroState.KalmanFilter_delta_baro;

  // 100Hz data but send at 2Hz - only packed at 100Hz
  Tel100Hz->T2Bx.NAV_100HzCountAux =           NAV_100HzCount                         ;
  Tel100Hz->T2Bx.NAV_GeVecXecef    = (real32_T)NavState->NAV_NavigationState.grav_e.x ;
  Tel100Hz->T2Bx.NAV_GeVecYecef    = (real32_T)NavState->NAV_NavigationState.grav_e.y ;
  Tel100Hz->T2Bx.NAV_GeVecZecef    = (real32_T)NavState->NAV_NavigationState.grav_e.z ;
  Tel100Hz->T2Bx.NAV_FeVecXecef    = (real32_T)NavState->NAV_NavigationState.fib_e.x  ;
  Tel100Hz->T2Bx.NAV_FeVecYecef    = (real32_T)NavState->NAV_NavigationState.fib_e.y  ;
  Tel100Hz->T2Bx.NAV_FeVecZecef    = (real32_T)NavState->NAV_NavigationState.fib_e.z  ;
  Tel100Hz->T2Bx.NAV_DCMbe11       = (real32_T)NavState->NAV_NavigationState.dcm_be[0];
  Tel100Hz->T2Bx.NAV_DCMbe12       = (real32_T)NavState->NAV_NavigationState.dcm_be[1];
  Tel100Hz->T2Bx.NAV_DCMbe13       = (real32_T)NavState->NAV_NavigationState.dcm_be[2];
  Tel100Hz->T2Bx.NAV_DCMbe21       = (real32_T)NavState->NAV_NavigationState.dcm_be[3];
  Tel100Hz->T2Bx.NAV_DCMbe22       = (real32_T)NavState->NAV_NavigationState.dcm_be[4];
  Tel100Hz->T2Bx.NAV_DCMbe23       = (real32_T)NavState->NAV_NavigationState.dcm_be[5];
  Tel100Hz->T2Bx.NAV_DCMbe31       = (real32_T)NavState->NAV_NavigationState.dcm_be[6];
  Tel100Hz->T2Bx.NAV_DCMbe32       = (real32_T)NavState->NAV_NavigationState.dcm_be[7];
  Tel100Hz->T2Bx.NAV_DCMbe33       = (real32_T)NavState->NAV_NavigationState.dcm_be[8];
  Tel100Hz->T2Bx.NAV_InitXpos      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitYpos      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitZpos      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitXvel      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitYvel      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitZvel      =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitPhi       =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitTheta     =           0.0                                    ;
  Tel100Hz->T2Bx.NAV_InitPsi       =           0.0                                    ;
}
