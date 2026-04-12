#ifndef GNSS_CBIT_H
#define GNSS_CBIT_H
//---------------------------------------------------------------------------
//  Errors
//    Bit[ 0]: UBX-RXM-RAWX.numMeas > 28
//    Bit[ 1]: UBX-RXM-RAWX.gnssId: Invalid
//    Bit[ 2]: UBX-RXM-RAWX.svId: Invalid
//    Bit[ 3]: UBX-NAV-SAT.numSvs > 28
//    Bit[ 4]: UBX-NAV-SAT.gnssId: Invalid
//    Bit[ 5]: UBX-NAV-SAT.svId: Invalid
//    Bit[16]: GNSS constellation not supported
//    Bit[17]: GNSS_TroposModel: Delay > 100m
//    Bit[18]: GPS_IonosModel: Delay > 150m
//---------------------------------------------------------------------------
//  Warnings
//    Bit[0]: Least Square: Rank of H < 4
//    Bit[1]: GNSS_TroposModel: SV elevation below 3 deg
//---------------------------------------------------------------------------
typedef struct {
  uint16_T CountErrors;
  uint16_T CountWarnings;
  uint32_T Errors;
  uint32_T Warnings;
} T_GNSS_CBIT;
//---------------------------------------------------------------------------
#endif
