/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <UBLOX.h>
 *
 *  Description:
 *  ------------
 *  Header file for the UBLOX module.
 *
 ******************************************************************************/

#ifndef UBLOX_H_
#define UBLOX_H_

#include "GNSS.h"
#include "Timer.h"


#pragma pack(1)

typedef struct
{
  unsigned short CRC;
  unsigned char Identifier;
  unsigned char Id;
} sUBLOX_TelHeader;

typedef struct
{
  unsigned short Time;
  unsigned short PacketCnt;
} sUBLOX_TelTimestamp;

typedef struct
{
  double TOW;
  unsigned short Week;
  signed char LeapS;
  unsigned char NumMeas;
  unsigned char RecStat;
  unsigned char Reserved1;
  unsigned short Reserved2;
} sUBLOX_TelRXM_RAWX_Header;

typedef struct
{
  double PrMes;
  double CpMes;
  float DoMes;
  unsigned char GNSS_Id;
  unsigned char SV_Id;
  unsigned char Reserved1;
  unsigned char FreqId;
  unsigned short Locktime;
  unsigned char Cn0;
  unsigned char PrStdev;
  unsigned char CpStdev;
  unsigned char DoStdev;
  unsigned char TrkStat;
  unsigned char Reserved2;
} sUBLOX_TelRXM_RAWX_SV;

typedef struct
{
  unsigned int I_TOW;
  int F_TOW;
  short Week;
  signed char LeapS;
  unsigned char Valid;
  unsigned int T_Acc;
} sUBLOX_TelNAV_TimeGPS;

typedef struct
{
  unsigned int I_TOW;
  unsigned int TOD;
  int F_TOW;
  unsigned short Nt;
  unsigned char N4;
  unsigned char Valid;
  unsigned int T_Acc;
} sUBLOX_TelNAV_TimeGLO;

typedef struct {
  unsigned int towMS;
  unsigned int towSubMS;
  int qErr;
  unsigned short week;
  unsigned char flags;
  unsigned char refInfo;
} sUBLOX_TelTIM_TP;

typedef struct
{
  unsigned int TimeStamp;
  unsigned int PacketCnt;
  unsigned char GNSS_Id;
  unsigned char SV_Id;
  unsigned char Reserved1;
  unsigned char FreqId;
  unsigned char NumWords;
  unsigned char Reserved2;
  unsigned char Version;
  unsigned char Reserved3;
  unsigned int DWRD_01;
  unsigned int DWRD_02;
  unsigned int DWRD_03;
  unsigned int DWRD_04;
} sUBLOX_TelSRFBX_GLO_SV;

typedef struct
{
  unsigned int TimeStamp;
  unsigned int PacketCnt;
  unsigned char GNSS_Id;
  unsigned char SV_Id;
  unsigned char Reserved1;
  unsigned char FreqId;
  unsigned char NumWords;
  unsigned char Reserved2;
  unsigned char Version;
  unsigned char Reserved3;
  unsigned int DWRD_01;
  unsigned int DWRD_02;
  unsigned int DWRD_03;
  unsigned int DWRD_04;
  unsigned int DWRD_05;
  unsigned int DWRD_06;
  unsigned int DWRD_07;
  unsigned int DWRD_08;
  unsigned int DWRD_09;
  unsigned int DWRD_10;
} sUBLOX_TelSRFBX_GPS_SV;

typedef struct
{
  unsigned int iTOW;
  int PosECEF_X;
  int PosECEF_Y;
  int PosECEF_Z;
  unsigned int pAcc;
} sUBLOX_TelNavPOSECEF;

typedef struct
{
  unsigned int iTOW;
  int VelECEF_X;
  int VelECEF_Y;
  int VelECEF_Z;
  unsigned int pAcc;
} sUBLOX_TelNavVELECEF;

typedef struct
{
  unsigned int iTOW;
  unsigned short gDOP;
  unsigned short pDOP;
  unsigned short tDOP;
  unsigned short vDOP;
  unsigned short hDOP;
  unsigned short nDOP;
  unsigned short eDOP;
} sUBLOX_TelNAV_DOP;

typedef struct
{
  unsigned int iTOW;
  int Lon;
  int Lat;
  int Height;
  int hMSL;
  unsigned int hAcc;
  unsigned int vAcc;
} sUBLOX_TelNAV_POSLLH;

typedef struct
{
  unsigned int iTOW;
  unsigned char Version;
  unsigned char NumSvs;
  unsigned short Reserved;
} sUBLOX_TelNAV_SAT_Header;

typedef struct
{
  unsigned char GNSS_Id;
  unsigned char SV_Id;
  unsigned char CN0;
  signed char Elev;
  short Azim;
  short PR_Res;
  unsigned int Flags;
} sUBLOX_TelNAV_SAT_SV;

typedef struct
{
  unsigned int TimeStamp;
  unsigned int PacketCnt;
  T_UBX_NAV_SOL NAV_SOL;
} sUBLOX_TelNAV_SOL;

typedef struct
{
  unsigned short GDOP;
  unsigned short PDOP;
  unsigned short HDOP;
  unsigned short VDOP;
  unsigned short TDOP;
} sGNSS_TelResultDOP;

typedef struct
{
  float Pre_ResMag;
  float Pre_ResStd;
  float Pre_ResRMS;
  unsigned short Pre_CntSVs;
  unsigned short Pre_Reserved;
  float Post_ResMag;
  float Post_ResStd;
  float Post_ResRMS;
  unsigned short Post_CntSVs;
  unsigned short Post_Reserved;
  unsigned int SuspectSVsGPS;
  unsigned int SuspectSVsGLO;
} sGNSS_TelResultRAIM;

typedef struct
{
  unsigned char ID_System;
  unsigned char ID_SV;
  unsigned short Status;
  float Elev;
  float Azim;
  float Tropos_m;
  float Ionos_m;
  float Residual;
} sGNSS_TelResultSV_User;

typedef struct
{
  unsigned char ID_System;
  unsigned char ID_SV;
  unsigned short Reserved;
  int Pos_X;
  int Pos_Y;
  int Pos_Z;
  float Vel_X;
  float Vel_Y;
  float Vel_Z;
  int Range;
  float RangeRate;
} sGNSS_TelResultNAV_SV;

typedef struct
{
  unsigned short ErrorCnt;
  unsigned short WaringCnt;
  unsigned int Errors;
  unsigned int Warnings;
} sGNSS_CBIT;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_Header RXM_RAW_Header;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV1;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV2;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV3;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV4;
  sUBLOX_TelTimestamp TIM_TP_Timestamp;
  sUBLOX_TelTIM_TP TIM_TP;
  sUBLOX_TelTimestamp NAV_TimeGPS_Timestamp;
  sUBLOX_TelNAV_TimeGPS NAV_TimeGPS;
  unsigned int Reserved;
  unsigned int PPS_Timestamp;
} sUBLOX_GNSS_2HzTelBlock1Slot1;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV5;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV6;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV7;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV8;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV9;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV10;
} sUBLOX_GNSS_2HzTelBlock1Slot2And7;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV11;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV12;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV13;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV14;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV15;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV16;
} sUBLOX_GNSS_2HzTelBlock1Slot3And8;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV17;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV18;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV19;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV20;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV21;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV22;
} sUBLOX_GNSS_2HzTelBlock1Slot4And9;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV23;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV24;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV25;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV26;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV27;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV28;
} sUBLOX_GNSS_2HzTelBlock1Slot5And10;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp RXM_RAW_Timestamp;
  sUBLOX_TelRXM_RAWX_Header RXM_RAW_Header;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV1;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV2;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV3;
  sUBLOX_TelRXM_RAWX_SV RXM_RAWX_SV4;
  sUBLOX_TelTimestamp TIM_TP_Timestamp;
  sUBLOX_TelTIM_TP TIM_TP;
  sUBLOX_TelTimestamp NAV_TimeGLO_Timestamp;
  sUBLOX_TelNAV_TimeGLO NAV_TimeGLO;
  unsigned int PPS_Timestamp;
} sUBLOX_GNSS_2HzTelBlock1Slot6;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV01;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV02;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV03;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV04;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV05;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV06;
  unsigned int Reserved;
} sUBLOX_GNSS_2HzTelBlock2Slot1;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV07;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV08;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV09;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV10;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV11;
  sUBLOX_TelSRFBX_GLO_SV SRFBX_GLO_SV12;
  unsigned int Reserved;
} sUBLOX_GNSS_2HzTelBlock2Slot2;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV01;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV02;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV03;
  sUBLOX_TelTimestamp NAV_DOP_Timestamp;
  sUBLOX_TelNAV_DOP NAV_DOP;
  unsigned short Reserved1;
  unsigned int Reserved2;
} sUBLOX_GNSS_2HzTelBlock2Slot3;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV04;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV05;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV06;
  sUBLOX_TelTimestamp NavPOSECEF_Timestamp;
  sUBLOX_TelNavPOSECEF NavPOSECEF;
  unsigned int Reserved;
} sUBLOX_GNSS_2HzTelBlock2Slot4;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV07;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV08;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV09;
  sUBLOX_TelTimestamp NavVELECEF_Timestamp;
  sUBLOX_TelNavVELECEF NavVELECEF;
  unsigned int Reserved;
} sUBLOX_GNSS_2HzTelBlock2Slot5;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV10;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV11;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV12;
  unsigned int Reserved[7];
} sUBLOX_GNSS_2HzTelBlock2Slot6;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV13;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV14;
  sUBLOX_TelNAV_SOL NAV_SOL;
  unsigned int Reserved[6];
} sUBLOX_GNSS_2HzTelBlock2Slot7;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV15;
  sUBLOX_TelSRFBX_GPS_SV SRFBX_GPS_SV16;
  unsigned int Reserved[21];
} sUBLOX_GNSS_2HzTelBlock2Slot8;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp NAV_SAT_Timestamp;
  sUBLOX_TelNAV_SAT_Header NAV_SAT_Header;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV01;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV02;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV03;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV04;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV05;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV06;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV07;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV08;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV09;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV10;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV11;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV12;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV13;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV14;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV15;
  unsigned int Reserved;
} sUBLOX_GNSS_2HzTelBlock2Slot9;

typedef struct
{
  sUBLOX_TelHeader Header;
  sUBLOX_TelTimestamp NAV_SAT_Timestamp;
  unsigned int NAV_SAT_iTOW;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV16;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV17;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV18;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV19;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV20;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV21;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV22;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV23;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV24;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV25;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV26;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV27;
  sUBLOX_TelNAV_SAT_SV NAV_SAT_SV28;
  sUBLOX_TelTimestamp NAV_POSLLH_Timestamp;
  sUBLOX_TelNAV_POSLLH NAV_POSLLH;
} sUBLOX_GNSS_2HzTelBlock2Slot10;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int Timestamp;
  unsigned int PacketCnt;
  T_GNSS_Result GNSS_Result;
  /*unsigned char GNNS_ResultValid;
  unsigned char GNNS_ResultReserved0 ;
  unsigned short GNNS_ResultNoSVsInFix;
  double GNNS_ResultGPS_TimeOfMeas;
  double GNNS_ResultGLO_TimeOfMeas;
  double GNNS_ResultSysTimeOfMeas;
  double GNNS_ResultPos[3];
  float GNNS_ResultVel[3];*/
} sUBLOX_GNSS_2HzTelBlock3Slot1And6;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot2;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot3;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot4;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot5;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot7;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot8;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot9;

typedef struct
{
  sUBLOX_TelHeader Header;
  unsigned int PacketCnt;
} sUBLOX_GNSS_2HzTelBlock3Slot10;

typedef struct
{
  sUBLOX_GNSS_2HzTelBlock1Slot1 Slot1;
  sUBLOX_GNSS_2HzTelBlock1Slot2And7 Slot2;
  sUBLOX_GNSS_2HzTelBlock1Slot3And8 Slot3;
  sUBLOX_GNSS_2HzTelBlock1Slot4And9 Slot4;
  sUBLOX_GNSS_2HzTelBlock1Slot5And10 Slot5;
  sUBLOX_GNSS_2HzTelBlock1Slot6 Slot6;
  sUBLOX_GNSS_2HzTelBlock1Slot2And7 Slot7;
  sUBLOX_GNSS_2HzTelBlock1Slot3And8 Slot8;
  sUBLOX_GNSS_2HzTelBlock1Slot4And9 Slot9;
  sUBLOX_GNSS_2HzTelBlock1Slot5And10 Slot10;
} sUBLOX_GNSS_20HzBlock1;

typedef struct
{
  sUBLOX_GNSS_2HzTelBlock2Slot1 Slot1;
  sUBLOX_GNSS_2HzTelBlock2Slot2 Slot2;
  sUBLOX_GNSS_2HzTelBlock2Slot3 Slot3;
  sUBLOX_GNSS_2HzTelBlock2Slot4 Slot4;
  sUBLOX_GNSS_2HzTelBlock2Slot5 Slot5;
  sUBLOX_GNSS_2HzTelBlock2Slot6 Slot6;
  sUBLOX_GNSS_2HzTelBlock2Slot7 Slot7;
  sUBLOX_GNSS_2HzTelBlock2Slot8 Slot8;
  sUBLOX_GNSS_2HzTelBlock2Slot9 Slot9;
  sUBLOX_GNSS_2HzTelBlock2Slot10 Slot10;
} sUBLOX_GNSS_20HzBlock2;

typedef struct
{
  sUBLOX_GNSS_2HzTelBlock3Slot1And6 Slot1;
  sUBLOX_GNSS_2HzTelBlock3Slot2 Slot2;
  sUBLOX_GNSS_2HzTelBlock3Slot3 Slot3;
  sUBLOX_GNSS_2HzTelBlock3Slot4 Slot4;
  sUBLOX_GNSS_2HzTelBlock3Slot5 Slot5;
  sUBLOX_GNSS_2HzTelBlock3Slot1And6 Slot6;
  sUBLOX_GNSS_2HzTelBlock3Slot7 Slot7;
  sUBLOX_GNSS_2HzTelBlock3Slot8 Slot8;
  sUBLOX_GNSS_2HzTelBlock3Slot9 Slot9;
  sUBLOX_GNSS_2HzTelBlock3Slot10 Slot10;
} sUBLOX_GNSS_20HzBlock3;

typedef struct
{
  TIME RxTimestamp;
  unsigned int RxCnt;
  unsigned short ErrCnt;
  unsigned char GPS_SatUsedCnt;
  unsigned char GLO_SatUsedCnt;
  float GPS_AveSignalStrenth;
  float GLO_AveSignalStrenth;
  T_GNSS_Result GNSS_Result;
} sUBLOX;

#pragma pack()


extern volatile sUBLOX UBLOX;
extern volatile sUBLOX_GNSS_20HzBlock1 UBLOX_GNSS_20HzBlock1;
extern volatile sUBLOX_GNSS_20HzBlock2 UBLOX_GNSS_20HzBlock2;
extern volatile sUBLOX_GNSS_20HzBlock3 UBLOX_GNSS_20HzBlock3;

void UBLOX_Init(void);
void UBLOX_EnableSystems(unsigned short Systems);
void UBLOX_DisableSystems(unsigned short Systems);
void UBLOX_Service(void);
void UBLOX_SetMessageRate(unsigned char Class, unsigned char Id, unsigned char Rate);
void UBLOX_PollMessage(unsigned char Class, unsigned char Id);
void UBLOX_RequestVersion(void);
unsigned char UBLOX_Configure(void);
unsigned char UBLOX_ConfigureReceiver(unsigned char UseGPS, unsigned char UseGlonass);

#endif /* UBLOX_H */
