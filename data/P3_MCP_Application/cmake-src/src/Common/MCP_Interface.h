#ifndef MCP_INTERFACE_H
#define MCP_INTERFACE_H

#include "commontypes.h"
#include "Nav100Hz_Common.h"

//---------------------------------------------------------------------------
typedef struct {
  uint32_T Spare01[100];
} T_Weapon_MCP_GlobalMemory_gnss;
//---------------------------------------------------------------------------
typedef struct {
  real32_T leverarmX;
  real32_T leverarmY;
  real32_T leverarmZ;
} T_Weapon_MCP_GlobalMemory_nav_weaponprop;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_nav_weaponprop weaponprop;
  uint32_T Spare01[97];
} T_Weapon_MCP_GlobalMemory_nav;
//---------------------------------------------------------------------------
typedef struct {
  real32_T Amax;
  real32_T Alin;
  real32_T Bele;
  uint32_T Spare01;
  uint32_T Spare02;
  uint32_T Spare03;
} T_Weapon_MCP_GlobalMemory_ttkf_seeker;
//---------------------------------------------------------------------------
typedef struct {
  real32_T horPos0[2];
  real32_T verPos0[2];
  real32_T horVel0[2];
  real32_T verVel0[2];
  real32_T horPosN;
  real32_T verPosN;
  real32_T horVelN;
  real32_T verVelN;
  uint32_T Spare01[10];
} T_Weapon_MCP_GlobalMemory_ttkf_setup_varnoise;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_ttkf_setup_varnoise MP;
  T_Weapon_MCP_GlobalMemory_ttkf_setup_varnoise Opp;
  uint32_T Spare01[10];
} T_Weapon_MCP_GlobalMemory_ttkf_setup;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_ttkf_seeker seeker[5];
  T_Weapon_MCP_GlobalMemory_ttkf_setup setup;
  uint32_T Spare01[100];
} T_Weapon_MCP_GlobalMemory_ttkf;
//---------------------------------------------------------------------------
typedef struct {
  real32_T Rarc;
  real32_T Xmrc;
  real32_T Xcg;
  real32_T Xref;
  real32_T Sref;
  real32_T mass;
  real32_T Jxx;
  real32_T machvec[7];
  real32_T alphavec[10];
  real32_T CMXdefl[7];
  real32_T CLCDtrim[70];
  real32_T CLtrim[70];
  real32_T CDtrim[70];
  uint32_T Spare01[10];
} T_Weapon_MCP_GlobalMemory_guid_AFprop;
//---------------------------------------------------------------------------
typedef struct {
  T_Weapon_MCP_GlobalMemory_guid_AFprop AFprop[3];
  uint32_T Spare01[1000];
} T_Weapon_MCP_GlobalMemory_guid;
//---------------------------------------------------------------------------
typedef struct {
  real32_T MachVec[6];
  real32_T AlphaVec[5];
  real32_T Kp_phi[30];
  real32_T Ki_phi[30];
  real32_T Kp_wbx[30];
  real32_T Kpa[30];
  real32_T Kiw[30];
  real32_T Kpw[30];
  real32_T SIGMAfilt;
  real32_T NCProll[30];
  real32_T RPLagZ[30];
  real32_T RPLagP[30];
  real32_T NCWroll[30];
  real32_T RRLeadZ[30];
  real32_T RRLeadP[30];
  real32_T RRLagZ[30];
  real32_T RRLagP[30];
  real32_T ALPHAfilt;
  real32_T NCAlat[30];
  real32_T LAlowpass[30];
  real32_T LALeadZ[30];
  real32_T LALeadP[30];
  real32_T LALagZ[30];
  real32_T LALagP[30];
  real32_T NCWlat[30];
  real32_T LWLeadZ[30];
  real32_T LWLeadP[30];
  real32_T LWLagZ[30];
  real32_T LWLagP[30];
} T_Weapon_MCP_GlobalMemory_autop_gains;
//---------------------------------------------------------------------------
typedef struct {
  real32_T defl_limit;
  real32_T t_delt;
  T_Weapon_MCP_GlobalMemory_autop_gains gains[6];
  uint32_T Spare01[100];
} T_Weapon_MCP_GlobalMemory_autop;
//---------------------------------------------------------------------------
typedef struct {
  real32_T psilim;
  real32_T psivec[8];
  real32_T altmin;
  real32_T altmax;
  real32_T altvec[6];
  real32_T alttgt[3];
  real32_T CX070[144];
  real32_T CX080[144];
  real32_T CX090[144];
  real32_T CY070[144];
  real32_T CY080[144];
  real32_T CY090[144];
  real32_T EXCL070[144];
  real32_T EXCL080[144];
  real32_T EXCL090[144];
  real32_T RAD070[144];
  real32_T RAD080[144];
  real32_T RAD090[144];
  real32_T RBAL070[36];
  real32_T RBAL080[36];
  real32_T RBAL090[36];
} T_Weapon_MCP_GlobalMemory_LARdata;
//---------------------------------------------------------------------------
typedef struct {
  int16_T TOF070[192000];
  int16_T TOF080[192000];
  int16_T TOF090[192000];
} T_Weapon_MCP_GlobalMemory_LARtraj;

//---------------------------------------------------------------------------
typedef struct {
  uint32_T BegOfBlockID;
  uint32_T SubCheckSum;
  uint8_T WeaponType;
  uint8_T BombType;
  uint8_T AirframeType;
  uint8_T SubVerNumber;
  uint32_T Spare01[50];
  T_Weapon_MCP_GlobalMemory_gnss gnss;
  T_Weapon_MCP_GlobalMemory_nav nav;
  T_Weapon_MCP_GlobalMemory_ttkf ttkf;
  T_Weapon_MCP_GlobalMemory_guid guid;
  T_Weapon_MCP_GlobalMemory_autop autop;
  T_Weapon_MCP_GlobalMemory_LARdata LARdata;
  T_Weapon_MCP_GlobalMemory_LARtraj LARtraj;
//  uint8_T Spare02[0x2000];
//  char GCAConfigData[0x1000];
} T_Weapon_MCP_GlobalMemory;
//---------------------------------------------------------------------------
typedef struct {
  uint32_T msgID;
  T_Cartesian wb;
  T_Cartesian ab;
  uint16_T Status1;
  uint16_T Status2;
} T_NAV_InertialMeasurementUnitAutopilot;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T BombType;
  uint8_T LaunchLinkPresent;
  uint8_T NoseFuzePresent;
  uint8_T WingsPresent;
  uint8_T PDU_28V_Present;
  uint8_T SAD_Launched;
  uint8_T BatteryOn;
  uint8_T WingsOpenRcvd;
  uint8_T WingsRelRcvd;
  uint8_T Spare01;
  uint8_T Spare02;
  uint8_T Spare03;
} T_Weapon_ITUC_MCP;
//---------------------------------------------------------------------------
typedef struct {
  real_T SeekerMode;
  real_T TrackStatus;
  real_T Detection;
  real_T AnglesValid;
  real_T GainStage;
  real_T TargetYaw;
  real_T TargetPitch;
  real_T TargetSNR;
  real_T EstPRI;
  real_T MeasureTime;
  real_T DetectorASat;
  real_T DetectorBSat;
  real_T DetectorCSat;
  real_T DetectorDSat;
} T_Weapon_SALSeeker_IF102;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T WeaponType;
  uint8_T BombType;
  uint8_T SeekerType;
  uint8_T HOBSPresent;
  uint8_T DTACPresent;
  uint8_T RTACPresent;
  uint8_T BatActMethod;
  uint8_T WingsPresent;
  uint8_T Spare01;
  uint8_T Spare02;
  uint8_T Spare03;
} T_GUID_WeaponConfig;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T GC_Mode;
  uint8_T WC_Log;
  uint8_T Spare01;
  uint8_T Spare02;
  int32_T LARalt;
  int32_T LARX;
  int32_T LARY;
  uint16_T LARradius;
  uint16_T LARtimeimp;
} T_GUID_TestConfig;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T umbilical;
  uint8_T WingsOpenRcvd;
  uint8_T WingsRelRcvd;
  uint8_T BatteryOn;
} T_GUID_Discretes;
//---------------------------------------------------------------------------
typedef struct {
  real_T PriTgtLatitude;
  real_T PriTgtLongitude;
  real_T SecTgtLatitude;
  real_T SecTgtLongitude;
  real32_T PriTgtAltitude;
  real32_T SecTgtAltitude;
  real32_T AttackHeading;
  real32_T DiveAngle;
  real32_T ImpactSpeed;
  uint16_T GNSS_Usage;
  uint8_T AttackHeadingValid;
  uint8_T SecondarySwitch;
  real32_T SAL_Frequency;
  uint32_T SAL_Activation;
  uint16_T SAL_Accuracy;
  uint16_T SAL_Jitter;
  uint8_T WeaponType;
  uint8_T MissionType;
  uint16_T Spare;
} T_GUID_MissionPlanning;
//---------------------------------------------------------------------------
typedef struct {
  uint16_T Altitude;
  uint16_T Speed;
  uint16_T Direction;
} T_GUID_WindEntry;
//---------------------------------------------------------------------------
typedef struct {
  uint16_T WindEntryCnt;
  T_GUID_WindEntry WindEntries[39];
  uint16_T Spare;
  uint16_T Checksum;
} T_GUID_WindParams;
//---------------------------------------------------------------------------
typedef struct {
  real_T Temp;
  real_T Psta;
  real_T Pdyn;
} T_GUID_FlightConditions;
//---------------------------------------------------------------------------
typedef struct {
  uint32_T valid;
  uint32_T Spare;
  real_T rgx;
  real_T rgy;
  real_T rgz;
  real_T rex;
  real_T rey;
  real_T rez;
  real_T alt;
  real_T dive;
  real_T arrive;
  real_T speed;
} T_GUID_Guid2DATA_tgtstruct;
//---------------------------------------------------------------------------
typedef struct {
  real_T lat;
  real_T lon;
  real_T alt;
  real_T rex;
  real_T rey;
  real_T rez;
  real_T Vgnd;
  real_T mach;
  real_T track;
  real_T wind_spd;
  real_T wind_dir;
} T_GUID_Guid2DATA_launchstruct;
//---------------------------------------------------------------------------
typedef struct {
  uint8_T umbilical;
  uint8_T NavActive;
  uint8_T OpportunityMode;
  uint8_T Spare01;
  uint32_T Spare02;
  T_GUID_Guid2DATA_tgtstruct tgt_data;
  T_GUID_Guid2DATA_launchstruct launch_data;
} T_GUID_Guid2DATA;
//---------------------------------------------------------------------------
#endif
