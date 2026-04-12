/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <M1553Convert.h>
 *
 *  Description:
 *  ------------
 *  Header file for the M1553Convert module.
 *
 ******************************************************************************/

#ifndef M1553_CONVERT_H_
#define M1553_CONVERT_H_

typedef struct
{
	uint16_t MSWord;
	uint16_t LSWord;
} s1553_Double_Word;

float GetFloatInt(int Word,float ScaleFactor);
float GetFloatUInt(unsigned int Word,float ScaleFactor);
float Get1WordFloatS(uint16_t Word,float ScaleFactor);
float Get1WordFloatU(uint16_t Word,float ScaleFactor);
double Get1WordDoubleS(uint16_t Word,double ScaleFactor);
double Get1WordDoubleU(uint16_t Word,double ScaleFactor);
float Get2WordFloatS(uint16_t *Word1,float ScaleFactor);
float Get2WordFloatU(uint16_t *Word1,float ScaleFactor);
double Get2WordDoubleS(uint16_t *Word1,double ScaleFactor);
double Get2WordDoubleU(uint16_t *Word1,double ScaleFactor);
uint16_t Convert1WordToShortU(uint16_t Word,float SF1, float SF2, float SF3);
int16_t Convert1WordToShortS(int16_t Word,float SF1, float SF2, float SF3);
unsigned short Convert2WordToShortU(uint16_t *Word1,float SF1, float SF2, float SF3);
short Convert2WordToShortS(uint16_t *Word1,float SF1, float SF2, float SF3);
int Convert2WordToIntS(uint16_t *Word1,float SF1, float SF2, float SF3);


#endif /* M1553_CONVERT_H_ */
