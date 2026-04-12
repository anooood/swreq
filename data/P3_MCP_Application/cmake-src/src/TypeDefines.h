/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <TypeDefines.h>
 *
 *  Description:
 *  ------------
 *  Type Definition header.
 *
 ******************************************************************************/

#ifndef TYPEDEFINES_H
#define TYPEDEFINES_H

#ifndef TRUE
#define TRUE                            1
#endif

#ifndef FALSE
#define FALSE                           0
#endif

#ifndef ON
#define ON                              1
#endif

#ifndef OFF
#define OFF                             0
#endif

#ifndef NULL
#define NULL                            0
#endif

#ifndef PI
#define PI                              3.141592653590
#endif

#ifndef TWO_PI
#define TWO_PI                          6.283185307180
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)                      (x * 0.017453292520)
#endif

#ifndef RAD2DEG
#define RAD2DEG(x)                      (x * 57.295779513082)
#endif

#ifndef FT2METER
#define FT2METER(x)                     (x * 0.3048)
#endif

#ifndef METER2FT
#define METER2FT(x)                     (x * 3.28084)
#endif

#ifndef MBAR2PA
#define MBAR2PA(x)                      (x * 100.0)
#endif

#ifndef PA2MBAR
#define PA2MBAR(x)                      (x * 0.01)
#endif

#ifndef ISNAN
#define ISNAN(x)                        (x != x)
#endif

#ifndef WITHIN_LIMITS
#define WITHIN_LIMITS(x, Low, High)     ((x >= Low) && (x <= High))
#endif

#endif
