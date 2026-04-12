/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Util.c>
 *
 *  Description:
 *  ------------
 *  Util module.
 *
 *  Function(s):
 *  ------------
 *  - ChangeEndian                            : Changes the Endianess of a variable.
 *  - MirrorUSHORT                            : Reverse the order of bits of a uint16_t.
 *  - CalculateMACH_Number                    : Calculate the MACH number at altitude.
 *  - ConvertRadAngleToPositiveDegAngle       : Converts a angle in rad to a angle in deg between 0 and 360.
 *  ...
 *
 *
 ******************************************************************************/

#include <String.h>
#include "Util.h"
#include "TypeDefines.h"

uint16_t MirrorUSHORT(uint16_t Val)
{
  uint8_t index;
  uint16_t tmp = 0;


  for (index = 0; index < UTIL_CONST_16; index++) {
    tmp |= (((Val >> (UTIL_CONST_15 - index)) & UTIL_CONST_1) << index);
  }

  return tmp;
}

uint32_t ConvertRadToDegPos(double AngleRad)
{
  int32_t ang_deg;
  double deg_value;

  if (AngleRad < 0.0) {
	deg_value = RAD2DEG(AngleRad) - UTIL_CONST_FLOAT_0_5;
  }
  else {
	deg_value = RAD2DEG(AngleRad) + UTIL_CONST_FLOAT_0_5;
  }

  /* Round to nearest integer explicitly */
  if (deg_value >= 0.0)
  {
      ang_deg = (int32_t)(deg_value + 0.5);
  }
  else
  {
      ang_deg = (int32_t)(deg_value - 0.5);
  }

  /* Normalize angle to [0, 359] range */
  ang_deg %= UTIL_CONST_360;
  if (ang_deg < 0)
  {
      ang_deg += UTIL_CONST_360;
  }

  return (uint32_t)ang_deg;
}
