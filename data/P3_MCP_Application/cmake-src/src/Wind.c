/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Wind.c>
 *
 *  Description:
 *  ------------
 *  Wind module.
 *
 *  Function(s):
 *  ------------
 *  - WindSet                            : Set wind data.
 *  ...
 *
 *
 ******************************************************************************/

#include <math.h>
#include "TypeDefines.h"
#include "CRC.h"
#include "Weapon.h"
#include "Wind.h"
#include "Telemetry.h"

uint8_t WindSet(sWind *WindData)
{
  uint16_t crc;

  /* Calculate MIL STD CRC from byte 0 to the byte before the CRC in the sWind structure */
  crc = CalculateMIL_STD_1760D_Crc((uint16_t *) WindData, (sizeof(sWind) - WIND_CONST_6) / WIND_CONST_2);

  if (crc == WindData->CRC) {
    Weapon.Flags.Bits.WindOk = 1U; /* TRUE */
    Weapon.Wind = *WindData;
  }
  else {
    Weapon.Flags.Bits.WindOk = 0U; /* FALSE */
  }
  TelemetryBuild2HzBlock2();
  return (uint8_t)Weapon.Flags.Bits.WindOk;
}



