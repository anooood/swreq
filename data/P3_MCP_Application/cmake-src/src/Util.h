/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Util.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Util module.
 *
 ******************************************************************************/

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define UTIL_CONST_1                0x01u
#define UTIL_CONST_15               (uint8_t)(15)
#define UTIL_CONST_16               (uint8_t)(16)
#define UTIL_CONST_360              (int8_t)(360)
#define UTIL_CONST_FLOAT_0_5        (0.5f)

uint16_t MirrorUSHORT(uint16_t Val);
uint32_t ConvertRadToDegPos(double AngleRad);

#endif /* UTIL_H_ */
