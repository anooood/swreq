/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <AuxiliaryPort.c>
 *
 *  Description:
 *  ------------
 *  AuxiliaryPort.c implements the interface for the auxiliary port used by the diagnostic tool.
 *  It provides functions for outputting debug information, printing status messages, and
 *  facilitating real-time monitoring of the system during development and testing.
 *
 *  Function(s):
 *  ------------
 *  - AUXInit:                     Initialization
 *  - PrnBinaryByte:               Print Binary Functionality
 *  - AUXSetMode:                  Set HILS Mode
 *  - AUX_HelpCommand:             Help command functionality
 *  - AUX_MainCommand:             Auxiliary functionality for Main
 *  - AUX_Handle_WiFi_Cmd:         Auxiliary functionality for Wifi
 *  - AUX_Handle_ETE_Cmd:          Auxiliary functionality for ETE
 *  - AUX_Handle_HILS_Cmd          Auxiliary functionality for HILS
 *  - AUX_Handle_DTAC_Cmd:         Auxiliary functionality for DTAC
 *  - AUX_Handle_Servo_Cmd:        Auxiliary functionality for Servo
 *  - AUX_Handle_Tel_Cmd:          Auxiliary functionality for Telemetry
 *  - AUX_MLANCommand:             Auxiliary functionality for MLAN
 *  - AUX_HandleCommand:           Auxiliary functionality for handling any command
 *  - AUXprintf:                   Auxiliary functionality for printing on the AUX PORT
 *  - ServiceAUXRx:                Function that serves the Rx messages on the AUX PORT
 *  - ServiceAUXTx:                Function that serves the Tx messages on the AUX PORT
 *  - AUXsprintf:                  Auxiliary functionality for sprinting (sprinft) on the AUX PORT
 *  - AUXsprintfFloatToStr:        Format Float to Str for printing capability
 *  - AUXsprintfFloat:             Format Float for printing capability
 *  - AUXsprintfDoubleToStr:       Format Double to Str for printing capability
 *  - AUXsprintfDouble:            Format Double for printing capability
 *  - AUXsprintfDoubleDecimals:    Format Double with more decimals for printing capability
 *  - AUXsprintfHexSignedShort:    Format HEX Signed short for printing capability
 *  - printBatteryCommand:         Format Battery Commands to be printed
 *  ...
 *
 *
 ******************************************************************************/


#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "Timer.h"
#include "FIFO.h"
#include "UART.h"
#include "UART_PR.h"
#include "WeaponSettings.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "BatteryPack.h"
#include "MLAN.h"
#include "Main.h"

sCharAUX_FIFO		AuxPortRxFIFO;

sAUX_StateSpace AUX_SS;

void AUXInit(char *AuxStr) {

	AUX_SS.ServiceControls.OldSystemTime = GetTimeUs();
	AUX_SS.AuxCmd[0] = '\0'; //clear buffer
	AUX_SS.AuxStr[0] = '\0'; //clear buffer
	memset(AUX_SS.ServiceControls.dest_str,0,sizeof(AUX_SS.ServiceControls.dest_str));

	if (!WeaponSettings.DataFileValid) {
		AUXCTRL_SS.AuxiliaryControls.AuxOn			= 1;
		AUXCTRL_SS.AuxiliaryControls.SchedulerAuxOn	= 0;
		AUXCTRL_SS.AuxiliaryControls.Enabled		= 1;
		AUXCTRL_SS.AuxiliaryControls.ErrDetectOn	= 1;
	}

	AUX_SS.ServiceControls.SystemTime = GetTimeUs();
	AUX_SS.ServiceControls.TimeDiff = AUX_SS.ServiceControls.SystemTime - AUX_SS.ServiceControls.OldSystemTime;

	INIT_FIFO(AuxPortRxFIFO);
	if (AUXCTRL_SS.AuxiliaryControls.AuxOn){
		sprintf(AuxStr,"%s%llu:Auxiliary Port Initialization\n",AuxStr,AUX_SS.ServiceControls.SystemTime);
	}
}

typedef struct
{
	char		Bit0	:1;
	char		Bit1	:1;
	char		Bit2	:1;
	char		Bit3	:1;
	char		Bit4	:1;
	char		Bit5	:1;
	char		Bit6	:1;
	char		Bit7	:1;
}s8Bit;

char *PrnBinaryByte(char InVal)
{
	static char Result[1000];
	char Value;
	s8Bit *Val8Bit;

	Result[0]= 0;
	Value=InVal;
	Val8Bit = (s8Bit *)&Value;
	if (Val8Bit->Bit7) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit6) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit5) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit4) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	sprintf(Result,"%s%c",Result,' ');
	if (Val8Bit->Bit3) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit2) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit1) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	if (Val8Bit->Bit0) sprintf(Result,"%s%c",Result,'1');
	else sprintf(Result,"%s%c",Result,'0');
	sprintf(Result,"%s%s",Result," : ");

	Result[12]=0;
	return(Result);
}

char *PrnBinary(char *InVal,int Size)
{
	static char Result[100];
	int Count;
	Result[0]= 0;
	for (Count=0;Count<Size;Count++) {
		sprintf(Result,"%s%s",Result,PrnBinaryByte(InVal[Count]));
	}
	sprintf(Result,"%s%c",Result,'\0');
	return(Result);
}

void AUXSetMode(unsigned short HILS_Mode) {
	AUX_SS.HILS_Mode=HILS_Mode;
}

void AUX_HelpCommand (){
	sprintf(AUX_SS.AuxStr,"%s",HelpStr);
}

void AUX_MainCommand (){
	AUX_SS.Main=1;
	sprintf(AUX_SS.AuxStr,"%s Main Command Received\n",AUX_SS.AuxStr);
	AUXCTRL_SS.MAINControls.NodeCheckOn	= 1;
}

void AUX_Handle_WiFi_Cmd(void) {
	AUX_SS.WiFi=1;
	sprintf(AUX_SS.AuxStr,"%s WiFi Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s WiFi Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.WiFi_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s WiFi Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.WiFi_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s WiFi Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.WiFi_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s WiFi Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.WiFi_AuxOn=0;
	}
}
void AUX_Handle_ETE_Cmd(void) {
	AUX_SS.ETE=1;
	sprintf(AUX_SS.AuxStr,"%s ETE Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s ETE Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.ETE_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s ETE Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.ETE_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s ETE Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.ETE_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s ETE Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.ETE_AuxOn=0;
	}
}

void AUX_Handle_HILS_Cmd(void) {
	AUX_SS.HILS=1;
	sprintf(AUX_SS.AuxStr,"%s HILS Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s HILS Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.HILS_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s HILS Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.HILS_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s HILS Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.HILS_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s HILS Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.HILS_AuxOn=0;
	}
}

void AUX_Handle_DTAC_Cmd(void) {
	AUX_SS.DTAC=1;
	sprintf(AUX_SS.AuxStr,"%s DTAC Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s DTAC Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.DTAC_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s DTAC Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.DTAC_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s DTAC Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.DTAC_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s DTAC Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.DTAC_AuxOn=0;
	}
}

void AUX_Handle_Servo_Cmd(void) {
	AUX_SS.Servo=1;
	sprintf(AUX_SS.AuxStr,"%s Servo Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Servo Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.Servo_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Servo Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.Servo_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Servo Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.Servo_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Servo Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.Servo_AuxOn=0;
	}
}

void AUX_Handle_Tel_Cmd(void) {
	AUX_SS.Tel=1;
	sprintf(AUX_SS.AuxStr,"%s Tel Command Received\n",AUX_SS.AuxStr);
	if ((AUX_SS.On==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Tel Tx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.Tel_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Tx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Tel Tx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.TxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.Tel_AuxOn=0;
	}
	if ((AUX_SS.On==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Tel Rx Setting On\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 1;
		AUXCTRL_SS.MLANControls.Tel_AuxOn=1;
	}
	else if ((AUX_SS.Off==1)&&(AUX_SS.Rx==1)) {
		sprintf(AUX_SS.AuxStr,"%s Tel Rx Setting Off\n",AUX_SS.AuxStr);
		AUXCTRL_SS.MLANControls.RxAuxOn	= 0;
		AUXCTRL_SS.MLANControls.Tel_AuxOn=0;
	}
}

void AUX_MLANCommand(){
	sprintf(AUX_SS.AuxStr,"%s\nMLAN Command Received:%s\n",AUX_SS.AuxStr,AUX_SS.AuxCmd);
	if (strstr(AUX_SS.AuxCmd, "wifi")!=NULL) {
		AUX_Handle_WiFi_Cmd();
	}
	else if (strstr(AUX_SS.AuxCmd, "ETE")!=NULL) {
		AUX_Handle_ETE_Cmd();
	}
	else if (strstr(AUX_SS.AuxCmd, "HILS")!=NULL) {
		AUX_Handle_HILS_Cmd();
	}
	else if (strstr(AUX_SS.AuxCmd, "DTAC")!=NULL) {
		AUX_Handle_DTAC_Cmd();
	}
	else if (strstr(AUX_SS.AuxCmd, "Servo")!=NULL) {
		AUX_Handle_Servo_Cmd();
	}
	else if (strstr(AUX_SS.AuxCmd, "Tel")!=NULL) {
		AUX_Handle_Tel_Cmd();
	}

}

void AUX_HandleCommand(sUART *UART, char *AuxCmd) {
	if (strstr(AuxCmd, "On")!=NULL) {
		AUX_SS.On=1;
		AUX_SS.Off=0;
	}
	else if (strstr(AuxCmd, "Off")!=NULL) {
		AUX_SS.Off=1;
		AUX_SS.On=0;
	}
	if (strstr(AuxCmd, "Tx")!=NULL) {
		AUX_SS.Tx=1;
		AUX_SS.Rx=0;
	}
	if (strstr(AuxCmd, "Rx")!=NULL) {
		AUX_SS.Tx=0;
		AUX_SS.Rx=1;
	}
	if ((strcmp(AuxCmd,"help"))==0) {
		AUX_HelpCommand();
	}
	else if (strstr(AuxCmd, "MAIN")!=NULL) {
		AUX_SS.Main=1;
		AUX_MainCommand();
	}
	else if (strstr(AuxCmd, "MLAN")!=NULL) {
		AUX_SS.MLAN=1;
		AUX_MLANCommand();
	}
	else if (strstr(AuxCmd, "MPSAL")!=NULL) {
		AUX_SS.MPSAL=1;

	}
	else if (strstr(AuxCmd, "HOBS")!=NULL) {
		AUX_SS.HOBS=1;
	}

}

void AUXprintf ( sUART *UART, const char * format, ... )
{
	char dest_str[5000];
	int Len;

  memset(dest_str,0,sizeof(dest_str));
  // the arglist holds list of all arguments fed to this function
  va_list arglist;
  // initializing the arglish using the vs_start macro
  va_start (arglist, format);
  // putting the arguments into the arglist using vsprintf
  vsprintf (dest_str, format, arglist);
  // releasing the arglish variable using the va_end macro
  va_end (arglist);
  Len=strlen(dest_str);
  if (!AUX_SS.HILS_Mode) {
	  if (Len>0) {
		  UART_Write(UART, (unsigned char *)dest_str, Len);
	  }
  }
}

void ServiceAUXRx(sUART *UART){
	// Read received data
	AUX_SS.rx_cnt = UART_Read(UART, AUX_SS.tmp_rx_data, sizeof(AUX_SS.tmp_rx_data));
	if (AUX_SS.rx_cnt){
		if (AUX_SS.tmp_rx_data[0]==0xd) {
			AUX_HandleCommand(UART,AUX_SS.AuxCmd);
			AUX_SS.AuxCmd[0] = '\0';
			sprintf(AUX_SS.AuxStr,"%s\nMCP>",AUX_SS.AuxStr);
			AUXprintf(UART,AUX_SS.AuxStr);
			AUX_SS.AuxStr[0] = '\0';
		}
		else {
			AUXprintf(UART,"%c",AUX_SS.tmp_rx_data[0]);
			sprintf(AUX_SS.AuxCmd,"%s%c",AUX_SS.AuxCmd,AUX_SS.tmp_rx_data[0]);
		}
	}
}

void ServiceAUXTx(sUART *UART){
	int Length;
	int Count;
	char dest_str[AUX_FIFO_SIZE];

	if (AUXCTRL_SS.AuxiliaryControls.Enabled) {
		Length = 0;
		AUX_SS.ServiceControls.SystemTime = GetTimeUs();
		AUX_SS.ServiceControls.TimeDiff = AUX_SS.ServiceControls.SystemTime - AUX_SS.ServiceControls.OldSystemTime;
		if (AUX_SS.ServiceControls.TimeDiff > AUX_SERVICE_INTERVAL) {
			AUX_SS.ServiceControls.OldSystemTime = AUX_SS.ServiceControls.SystemTime;
			if (BYTES_IN_AUX_FIFO(AuxPortRxFIFO)) {
				NUMBER_BYTES_IN_AUX_FIFO(AuxPortRxFIFO, Length);
				if (Length>(UART_DATA_BUFFER_SIZE/2)) {
					if (AUXCTRL_SS.AuxiliaryControls.ErrDetectOn)
						AUXsprintf("%d:WARNING AUX FIFO Bytes=0x%x > UART Buffer 0x%x \n",GetTimeUs(), Length,UART_DATA_BUFFER_SIZE/2);
					Length = (UART_DATA_BUFFER_SIZE/2)-1;
				}
				for (Count=0;Count<Length;Count++) {
					GET_AUX_FIFO(AuxPortRxFIFO,dest_str[Count]);
				}
				UART_Write(UART, (unsigned char *)dest_str, Length);
			}
			if (AUXCTRL_SS.AuxiliaryControls.SchedulerAuxOn)
				AUXsprintf("%d:Servicing AUXILLIARY Port - Bytes in FIFO = %d\n",GetTimeUs(), Length);
		}
	}
}

void AUXsprintf ( const char * format, ... )
{
	int Len;
	int Count;

  //memset(AUX_SS.ServiceControls.dest_str,0,sizeof(AUX_SS.ServiceControls.dest_str));
  // the arglist holds list of all arguments fed to this function
  va_list arglist;
  // initializing the arglish using the vs_start macro
  va_start (arglist, format);
  // putting the arguments into the arglist using vsprintf
  vsprintf (AUX_SS.ServiceControls.dest_str, format, arglist);
  // releasing the arglish variable using the va_end macro
  va_end (arglist);
  Len=strlen(AUX_SS.ServiceControls.dest_str);
  if (Len>0) {
	  for (Count=0;Count<Len;Count++) {
		  PUT_AUX_FIFO(AuxPortRxFIFO,AUX_SS.ServiceControls.dest_str[Count]);
	  }
  }
}

void AUXformatFloatToStr(char * outputStr, float value)
{
    int integerPart = (int)value;
    int fractionalPart = (int)((value - integerPart) * 100);

    sprintf(outputStr,"%s%d.%02d",outputStr, integerPart, fractionalPart);
}

char * AUXsprintfFloat(float value)
{
	static char outputStr[15];
	unsigned char negativeFlag = 0;

    if (value < 0) {
        value = -value;
        negativeFlag = 1;
    }

    int integerPart = (int)value;
    int fractionalPart = (int)((value - integerPart) * 10000);

    if (negativeFlag)
    	snprintf(outputStr,sizeof(outputStr),"-%d.%04d", integerPart, fractionalPart);
    else
    	snprintf(outputStr,sizeof(outputStr),"%d.%04d", integerPart, fractionalPart);

    return outputStr;
}


void AUXformatDoubleToStr(char * outputStr, double value)
{
    int integerPart = (int)value;
    int fractionalPart = (int)((value - integerPart) * 10000);

    sprintf(outputStr,"%s%d.%04d",outputStr, integerPart, fractionalPart);
}

char * AUXsprintfDouble(double value)
{
	static char outputStr[15];
	unsigned char negativeFlag = 0;

    if (value < 0) {
        value = -value;
        negativeFlag = 1;
    }

    int integerPart = (int)value;
    int fractionalPart = (int)((value - integerPart) * 10000);

    if (negativeFlag)
    	snprintf(outputStr,sizeof(outputStr),"-%d.%04d", integerPart, fractionalPart);
    else
    	snprintf(outputStr,sizeof(outputStr),"%d.%04d", integerPart, fractionalPart);

    return outputStr;
}

char * AUXsprintfDoubleDecimals(double value, unsigned int decimals)
{
	static char outputStr[30];
	unsigned char negativeFlag = 0;

	// Int 32 max number of digits (4,294,967,295)
    if (decimals <= 0) {
        decimals = 4;  // Default to 4 decimals if input is invalid (<= 0)
    } else if (decimals > 9) {
        decimals = 9;  // Limit decimals to a maximum of 9
    }

    if (value < 0) {
        value = -value;
        negativeFlag = 1;
    }

    int integerPart = (int)value;
    int fractionalPart = (int)((value - integerPart) * pow(10,decimals));

    if (negativeFlag)
    	snprintf(outputStr,sizeof(outputStr),"-%d.%0*d", integerPart, decimals, fractionalPart);
    else
    	snprintf(outputStr,sizeof(outputStr),"%d.%0*d", integerPart, decimals, fractionalPart);

    return outputStr;
}

char * AUXsprintfHexSignedShort(short value)
{
	static char outputStr[30];
	char TempStr[30];

	sprintf(TempStr,"%8.8x",value);
	sprintf(outputStr,"0x%s",&TempStr[4]);
    return outputStr;
}

char * printBatteryCommand(unsigned char command)
{
	static char outputStr[45];

    switch (command)
    {
        case 0x00:
        	snprintf(outputStr,sizeof(outputStr),"0x00 - Set heater control mode");
            break;
        case 0x55:
        	snprintf(outputStr,sizeof(outputStr),"0x55 - Set control temperature");
            break;
        case 0xAA:
        	snprintf(outputStr,sizeof(outputStr),"0xAA - (Re)set cumulative battery usage");
            break;
        case 0xBB:
        	snprintf(outputStr,sizeof(outputStr),"0xBB - (Re)set number battery activations");
            break;
        case 0xCC:
        	snprintf(outputStr,sizeof(outputStr),"0xCC - Set the total battery Capacity");
            break;
        default:
        	snprintf(outputStr,sizeof(outputStr),"Unknown command");
            break;
    }

    return outputStr;
}
