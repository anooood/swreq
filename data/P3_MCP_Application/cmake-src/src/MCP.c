/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <MCP.c>
 *
 *  Description:
 *  ------------
 *  Mission Control Processor Unit functions
 *
 *  Function(s):
 *  ------------
 *  - MCP_Init                      : Initializes the MCP related functionality
 *  - MCP_KickWatchdog              : Kicks watchdog timer.
 *  - MCP_Reset                     : Reset MCP.
 *  - MCP_TimerEvent                : Timer interrupt handler
 *  - MCP_ReadTemperature           : Reads the MCP PCB temperature from the one-wire temperature sensor
 *  - MCP_UpdateVoltages            : Updates monitored voltages obtained from I2C devices
 *  - MCP_UpdatePressureSensorData  : Updates pressure obtained from I2C device
 *  - MCP_WeaponBatteryCtrl         : Weapon battery controller
 *  - MCP_SetWaitForLaunch          : Set the STC's wait for launch
 *  - MCP_SelectGNSS_Antenna        : Select the GNSS antenna to use
 *  - MCP_SetLEDs                   : Set the LED's on or off
 *  - MCP_ServiceHILS_ADS           : Service HILS
 *  ...
 *
 *
 ******************************************************************************/

#include <xil_io.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include "TypeDefines.h"
#include "Timer.h"
#include "UART.h"
#include "FIFO.h"
#include "UART_PR.h"
#include "AuxiliaryPort.h"
#include "AuxControls.h"
#include "Wind.h"
#include "FPGA.h"
#include "I2C.h"
#include "I2C_Controller.h"
#include "PressureSensor.h"
#include "Flash.h"
#include "Fuze.h"
#include "MCP.h"
#include "WIAC82_Comms.h"
#include "AircraftInterface.h"
#include "Weapon.h"
#include "WeaponSettings.h"

#define MCP_TEMPERATURE_SF      0.0625

#define DLE                     0x10
#define STX                     0x02
#define ETX                     0x03


sMCP MCP;
unsigned int HILS_ADS_RxMsgCnt;
sHILS_ADS_RxMsg HILS_ADS_RxMsg;
sMCP_StateSpace MCP_SS;
sUART HILS_ADS_RxUART;

static unsigned char PressureSensorChecked;
static unsigned char HILS_ADS_DLE_Received = FALSE;
static unsigned char HILS_ADS_ReceivingMsg = FALSE;
static unsigned char HILS_ADS_RxMessageIndex = 0;
//static sUART HILS_ADS_RxUART;
static unsigned char HILS_ADS_RxData[sizeof(sHILS_ADS_RxMsg)];

static void MCP_TimerEvent(unsigned int Input);

void MCP_Init(char *AuxStr)
{
  sLoaderMetaData meta_data;
  volatile unsigned int *on_wire_device = (volatile unsigned int *) FPGA_GPIO_ONE_WIRE_ADDR;


  MCP.PLD_StatusReg = *((unsigned int *) FPGA_PLD_CONTROL_INTERFACE_ADDR);

  PressureSensorChecked = FALSE;

  FlashReadData(MCP_FW_META_FLASH_ADDR, (unsigned char *) &meta_data, sizeof(sLoaderMetaData));
  MCP.FW_CRC = meta_data.CRC;
  MCP.FW_Version = meta_data.Version;

  FlashReadData(MCP_SW_META_FLASH_ADDR, (unsigned char *) &meta_data, sizeof(sLoaderMetaData));
  MCP.SW_CRC = meta_data.CRC;
  MCP.SW_Version = meta_data.Version;

  MCP.ETI = I2C_ControllerReadETI();
  MCP.SerialNo = (unsigned long long) (on_wire_device[1] << 16) + (on_wire_device[0] >> 16);
  MCP.Temperature = (on_wire_device[0] & 0xFFFF) * MCP_TEMPERATURE_SF;
  MCP.PressureSensToUse = pressNONE;
  MCP.PressSensPressure = 0;
  MCP.PressSensTemperature = 0;
  MCP.PressSensTimestamp = 0;

  HILS_ADS_RxMsgCnt = 0;

  if (Weapon.Discretes.Bits.HILS_Mode)
	UART_Init(&HILS_ADS_RxUART, FPGA_UART_ADS_ADDR, br460800, parNONE, 1, 0);
  else
	UART_PR_Init((sUART *) &HILS_ADS_RxUART, FPGA_UART_ADS_ADDR, br3000000, parNONE, 1, 0, 0, 0, 0);

  MCP_SelectGNSS_Antenna(antFRONT);

  // Start timer
  StartTimer(MCP_TimerEvent, 250000);

  if (!WeaponSettings.DataFileValid) {
	  AUXCTRL_SS.MCPControls.AuxOn 				= 1;
	  AUXCTRL_SS.MCPControls.SchedulerAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.MCPPresAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.MCPVoltAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.MCPAnteAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.MCPBattAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.MCPTempAuxOn		= 0;
	  AUXCTRL_SS.MCPControls.ErrDetectOn		= 1;
  }

  MCP_SS.ServiceControls.StartTime		= GetSystemTime();

  TIM_Init(AuxStr,MCP_SS.ServiceControls.StartTime);

  if (AUXCTRL_SS.MCPControls.AuxOn) {
	  sprintf(AuxStr,"%s%u:MCP Initialization:\n",AuxStr,GetTimeUs());
	  sprintf(AuxStr,"%s SW Version: %d.%02d CRC: 0x%x\n",AuxStr,(MCP.SW_Version/100),(MCP.SW_Version%100),MCP.SW_CRC);
	  sprintf(AuxStr,"%s FW Version: %d.%02d CRC: 0x%x\n",AuxStr,(MCP.FW_Version/100),(MCP.FW_Version%100),MCP.FW_CRC);
  }

}

void MCP_KickWatchdog(void)
{
  *((volatile unsigned int *) (FPGA_PLD_CONTROL_INTERFACE_ADDR + 0x0C)) = 1;
}

void MCP_Reset(void)
{
  // Unlock SLCR register
  Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x008, 0xDF0D);
  // Perform PSS reset
  Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x200, 1);
}

void MCP_TimerEvent(unsigned int Input)
{
  static uLED_Control leds = {0};


  leds.LED1_On = !leds.LED1_On;
  leds.LED2_On = !leds.LED1_On;

  MCP_SetLEDs(leds);
}

void MCP_ReadTemperature(void)
{
  short val;


  val = *((volatile unsigned int *) FPGA_GPIO_ONE_WIRE_ADDR) & 0xFFFF;
  MCP.Temperature = val * MCP_TEMPERATURE_SF;

  if (AUXCTRL_SS.MCPControls.MCPTempAuxOn) {
	  sprintf(MCP_SS.AuxStr,"%s%u:MCP Temperature = ",MCP_SS.AuxStr,GetTimeUs());
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.Temperature);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
  }
}

void MCP_UpdateVoltages(void)
{
  short raw_value;
  unsigned char data[2];


  if (I2C_ControllerGetRecievedData(data, i2cPSU_5V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_5V = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_1V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_1V = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_1V35) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_1V35 = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_1V8) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_1V8 = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_3V3) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_3V3 = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_28V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_28V = (float) (raw_value >> 3) * 0.004 * 2.13;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_GCA_6V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_GCA_6V = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_IMU_5V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_IMU_5V = (float) (raw_value >> 3) * 0.004;
  }

  if (I2C_ControllerGetRecievedData(data, i2cPSU_EFUZE_5V) > 0) {
    raw_value = (data[0] << 8) + data[1];
    MCP.PSU_EFUZE_5V = (float) (raw_value >> 3) * 0.004;
  }

  if (AUXCTRL_SS.MCPControls.MCPVoltAuxOn) {
	  sprintf(MCP_SS.AuxStr,"%s%u:MCP Voltage values:\n",MCP_SS.AuxStr,GetTimeUs());
	  sprintf(MCP_SS.AuxStr,"%s PSU 5V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_5V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU 1V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_1V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU 1V35 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_1V35);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU 1V8 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_1V8);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU 3V3 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_3V3);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU 28V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_28V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU GCA 6V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_GCA_6V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU IMU 5V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_IMU_5V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s PSU EFUZE 5V = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, MCP.PSU_EFUZE_5V);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
  }
}

void MCP_UpdatePressureSensorData(void)
{
  unsigned char sensor1_ok = FALSE;
  unsigned char sensor2_ok = FALSE;
  unsigned char press[3];
  unsigned char temp[3];
  float expected_pressure;

  if (I2C_ControllerGetRecievedData(press, i2cPRESSURE_SENSOR_1_PRESS) > 0) {
    if (I2C_ControllerGetRecievedData(temp, i2cPRESSURE_SENSOR_1_TEMP) > 0) {
      PressureSensorUpdate(&PressureSensor1, press, temp);
      sensor1_ok = TRUE;
    }
  }

  if (I2C_ControllerGetRecievedData(press, i2cPRESSURE_SENSOR_2_PRESS) > 0) {
    if (I2C_ControllerGetRecievedData(temp, i2cPRESSURE_SENSOR_2_TEMP) > 0) {
      PressureSensorUpdate(&PressureSensor2, press, temp);
      sensor2_ok = TRUE;
    }
  }

  // Verify that pressure sensors are correct (when in full nav) before using it.
  if (!PressureSensorChecked) {
    if ((Weapon.Status.Bits.NAV_Mode >= 30) && (Weapon.Status.Bits.NAV_Mode < 39)) {
      expected_pressure = (100 - (Weapon.Altitude / 1300.0) * 15.0) * 1000;

      if (sensor2_ok && (PressureSensor2.Pressure > (expected_pressure - 5000)) && (PressureSensor2.Pressure < (expected_pressure + 5000))) {
        MCP.PressureSensToUse = pressREAR;
      }
      else if (sensor1_ok && (PressureSensor1.Pressure > (expected_pressure - 5000)) && (PressureSensor1.Pressure < (expected_pressure + 5000))) {
        MCP.PressureSensToUse = pressFRONT;
      }
      else {
        MCP.PressureSensToUse = pressNONE;
      }

      PressureSensorChecked = TRUE;
    }
  }

  if (sensor2_ok && (MCP.PressureSensToUse == pressREAR)) {
    MCP.PressSensPressure = PressureSensor2.Pressure;
    MCP.PressSensTemperature = PressureSensor2.Temperature;
    MCP.PressSensTimestamp = Weapon.SystemTime;
  }
  else if (sensor1_ok && (MCP.PressureSensToUse == pressFRONT)) {
    MCP.PressSensPressure = PressureSensor1.Pressure;
    MCP.PressSensTemperature = PressureSensor1.Temperature;
    MCP.PressSensTimestamp = Weapon.SystemTime;
  }

  if (AUXCTRL_SS.MCPControls.MCPPresAuxOn) {
	  sprintf(MCP_SS.AuxStr,"%s%u:MCP Pressure values:\n",MCP_SS.AuxStr,GetTimeUs());
	  sprintf(MCP_SS.AuxStr,"%s Pressure Sensor To Use = %d\n",MCP_SS.AuxStr,MCP.PressureSensToUse);
	  sprintf(MCP_SS.AuxStr,"%s Pressure Sensor 1 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, PressureSensor1.Pressure);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s Pressure Sensor 2 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, PressureSensor2.Pressure);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s Pressure Temperature 1 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, PressureSensor1.Temperature);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
	  sprintf(MCP_SS.AuxStr,"%s Pressure Temperature 2 = ",MCP_SS.AuxStr);
	  AUXformatFloatToStr(MCP_SS.AuxStr, PressureSensor2.Temperature);
	  sprintf(MCP_SS.AuxStr,"%s\n",MCP_SS.AuxStr);
  }
}

void MCP_WeaponBatteryCtrl(unsigned char BatteryOn)
{
  volatile sMCP_ControlRegister mcp_ctrl_register;


  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);
  mcp_ctrl_register.SwitchOnBattery = BatteryOn;

  *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR) = mcp_ctrl_register;

  if (AUXCTRL_SS.MCPControls.MCPBattAuxOn) {
	  sprintf(MCP_SS.AuxStr,"%s%u:MCP Switch On Battery = %d \n",MCP_SS.AuxStr,GetTimeUs(),mcp_ctrl_register.SwitchOnBattery);
  }
}

void MCP_SetWaitForLaunch(unsigned char On)
{
  /*volatile sMCP_ControlRegister mcp_ctrl_register;


  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);
  mcp_ctrl_register.STC1_WaitForLaunch = On;
  mcp_ctrl_register.STC2_WaitForLaunch = On;

  *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR) = mcp_ctrl_register;*/
}

void MCP_SelectGNSS_Antenna(eGNSS_Antenna GNSS_Antenna)
{
  volatile sMCP_ControlRegister mcp_ctrl_register;


  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);
  mcp_ctrl_register.GNSS_Antenna = GNSS_Antenna;

  *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR) = mcp_ctrl_register;

  if (AUXCTRL_SS.MCPControls.MCPAnteAuxOn) {
	  sprintf(MCP_SS.AuxStr,"%s%u:MCP GNSS Antenna Selected = %d  (0 - Rear , 1 - Front) \n",MCP_SS.AuxStr,GetTimeUs(),mcp_ctrl_register.GNSS_Antenna);
  }
}

void MCP_SetLEDs(uLED_Control LEDs)
{
  volatile sMCP_ControlRegister mcp_ctrl_register;


  mcp_ctrl_register = *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR);

  mcp_ctrl_register.LED_1_On = LEDs.LED1_On;
  mcp_ctrl_register.LED_2_On = LEDs.LED2_On;
  mcp_ctrl_register.LED_3_On = LEDs.LED3_On;
  mcp_ctrl_register.LED_4_On = LEDs.LED4_On;

  *((volatile sMCP_ControlRegister *) FPGA_CONTROL_REG_ADDR) = mcp_ctrl_register;
}


void MCP_ServiceHILS_ADS(void)
{
  unsigned short index;
  unsigned short rx_cnt;
  unsigned char tmp_rx_data[32];


  // Read received data
  rx_cnt = UART_Read(&HILS_ADS_RxUART, tmp_rx_data, sizeof(tmp_rx_data));

  // Build message with received data
  for (index = 0; index < rx_cnt; index++) {
    if (tmp_rx_data[index] == DLE) {
      if (HILS_ADS_DLE_Received && HILS_ADS_ReceivingMsg) {
        // A DLE character followed by another DLE character indicates that a DLE character was sent as part of the message
        HILS_ADS_RxData[HILS_ADS_RxMessageIndex] = DLE;
        HILS_ADS_RxMessageIndex++;
        HILS_ADS_DLE_Received = FALSE;
      }
      else {
        HILS_ADS_DLE_Received = TRUE;
      }
    }
    else {
      // If the previous received byte was a DLE character, check if it is the start or end of a message
      if (HILS_ADS_DLE_Received) {
        HILS_ADS_DLE_Received = FALSE;

        switch (tmp_rx_data[index]) {
          case STX : {
            // A DLE character followed by a STX character indicates start of a new message
            HILS_ADS_ReceivingMsg = TRUE;
          } break;
          case ETX : {
            // A DLE character followed by a ETX character indicates end of message
            if (HILS_ADS_RxMessageIndex == sizeof(sHILS_ADS_RxMsg)) {
              HILS_ADS_RxMsgCnt++;
              memcpy((void *) &HILS_ADS_RxMsg, HILS_ADS_RxData, sizeof(sHILS_ADS_RxMsg));
            }

            HILS_ADS_ReceivingMsg = FALSE;
          } break;
          default : {
            // Error - a DLE character should always be followed by either a STX, ETX or another DLE
            HILS_ADS_ReceivingMsg = FALSE;
          } break;

          HILS_ADS_RxMessageIndex = 0;
        }
      }
      else if (HILS_ADS_ReceivingMsg) {
        // Data byte
        if (HILS_ADS_RxMessageIndex < sizeof(sHILS_ADS_RxMsg)) {
          HILS_ADS_RxData[HILS_ADS_RxMessageIndex] = tmp_rx_data[index];
          HILS_ADS_RxMessageIndex++;
        }
        else {
          // Error - more data bytes received than expected
          HILS_ADS_ReceivingMsg = FALSE;
          HILS_ADS_RxMessageIndex = 0;
        }
      }
    }
  }
}
