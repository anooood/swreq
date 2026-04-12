/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <AuxiliaryPort.h>
 *
 *  Description:
 *  ------------
 *  Header file for the AuxiliaryPort module.
 *
 ******************************************************************************/

#ifndef AUX_PORT_H
#define AUX_PORT_H

#define AUX_SERVICE_INTERVAL		((unsigned int)(10000)) // 10 milli-second


#define HelpStr \
"\n\nMAIN\n\
MLAN \n\
  WiFi  Rx Tx On Off\n\
  ETE   Rx Tx On Off\n\
  HILS  Rx Tx On Off\n\
  DTAC  Rx Tx On Off\n\
  Servo Rx Tx On Off\n\
  Tel   Rx Tx On Off\n\
HOBS Rx Tx On Off\n"


typedef struct
{
	TIME SystemTime;
	TIME OldSystemTime;
	unsigned int TimeDiff;
	char dest_str[AUX_FIFO_SIZE];
} sAUX_ServiceControl;

typedef struct
{
	sAUX_ServiceControl ServiceControls;
	int Main;
	int MLAN;
	int MPSAL;
	int WiFi;
	int ETE;
	int HILS;
	int DTAC;
	int Servo;
	int Tel;
	int HOBS;
	int On;
	int Off;
	int Rx;
	int Tx;
	unsigned short HILS_Mode;
	unsigned short rx_cnt;
	unsigned char tmp_rx_data[32];
	char AuxCmd[100];
	char AuxStr[UART_DATA_BUFFER_SIZE];		//
} sAUX_StateSpace;

extern sAUX_StateSpace AUX_SS;

char *PrnBinary(char *InVal,int Size);
void AUX_HandleCommand(sUART *UART, char *AuxCmd);
void AUXInit(char *AuxStr);
void ServiceAUXRx(sUART *UART);
void AUXSetMode(unsigned short HILS_Mode);
void AUXprintf ( sUART *UART, const char * format, ... );
void AUXsprintf ( const char * format, ... );
void ServiceAUXTx(sUART *UART);
void AUXformatFloatToStr(char * outputStr, float value);
char * AUXsprintfFloat(float value);
void AUXformatDoubleToStr(char * outputStr, double value);
char * AUXsprintfDouble(double value);
char * AUXsprintfDoubleDecimals(double value, unsigned int decimals);
char * AUXsprintfHexSignedShort(short value);
char * printBatteryCommand(unsigned char command);

#endif // AUXILLIARY_PORT_H
