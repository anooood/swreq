/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Telemetry.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Telemetry module.
 *
 ******************************************************************************/

#ifndef TELEMETRY_H
#define TELEMETRY_H


extern unsigned int FixedTelemetry;

void TelemetryInit(void);
void TelemetryBuild600Hz(void);
void TelemetryBuild2HzBlock1(void);
void TelemetryBuild2HzBlock2(void);
void TelemetryBuild2HzBlock3(void);
void TelemetryBuild2HzBlock4(void);
void TelemetryBuild2HzBlock5(void);
void TelemetryBuild2HzBlock7(void);
void TelemetryBuild2HzBlock8(void);
void TelemetryBuild2HzBlock10(void);
void TelemetrySend(unsigned char Tel100HzIndex);
void TelUseFixedData(unsigned int FixedTelOn);

#endif
