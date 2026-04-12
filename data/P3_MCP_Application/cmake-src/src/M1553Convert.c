/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <M1553Convert.c>
 *
 *  Description:
 *  ------------
 *  Module for WIAC-82 M1553 Conversions
 *
 *  Function(s):
 *  ------------
 *  - GetFloatInt                : Converts an integer value to float representation.
 *  - GetFloatUInt               : Converts an unsigned integer to float representation.
 *  - Get1WordFloatU             : Converts a 1-word unsigned value to float.
 *  - Get1WordFloatS             : Converts a 1-word signed value to float.
 *  - Get1WordDoubleS            : Converts a 1-word signed value to double precision float.
 *  - Get1WordDoubleU            : Converts a 1-word unsigned value to double precision float.
 *  - Get2WordFloatU             : Converts a 2-word unsigned value to float.
 *  - Get2WordFloatS             : Converts a 2-word signed value to float.
 *  - Get2WordDoubleS            : Converts a 2-word signed value to double precision float.
 *  - Get2WordDoubleu            : Converts a 2-word unsigned value to double precision float.
 *  - Convert1WordToShortU       : Converts a 1-word value to unsigned short.
 *  - Convert1WordToShortS       : Converts a 1-word value to signed short.
 *  - Convert2WordToShortU       : Converts a 2-word value to unsigned short.
 *  - Convert2WordToShortS       : Converts a 2-word value to signed short.
 *  - Convert2WordToIntS         : Converts a 2-word value to signed int.
 *  - Convert2WordToIntU         : Converts a 2-word value to unsigned int.
 *  ...
 *
 *
 ******************************************************************************/

#include <stdint.h>
#include "ScaleFactors.h"
#include "M1553Convert.h"

float GetFloatInt(int Word,float ScaleFactor) {
	float FloatOut;
	int IntVal;
	IntVal=Word;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
float GetFloatUInt(unsigned int Word,float ScaleFactor) {
	float FloatOut;
	unsigned int IntVal;
	IntVal=Word;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
float Get1WordFloatU(uint16_t Word,float ScaleFactor) {
	float FloatOut;
	uint16_t IntVal;
	IntVal=Word;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
float Get1WordFloatS(uint16_t Word,float ScaleFactor) {
	float FloatOut;
	int16_t IntVal;
	IntVal=Word;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
double Get1WordDoubleS(uint16_t Word,double ScaleFactor) {
	double DoubleOut;
	int IntVal;
	IntVal=Word;
	DoubleOut=(double)IntVal;
	return(DoubleOut/ScaleFactor);
}
double Get1WordDoubleU(uint16_t Word,double ScaleFactor) {
	double DoubleOut;
	unsigned int IntVal;
	IntVal=Word;
	DoubleOut=(double)IntVal;
	return(DoubleOut/ScaleFactor);
}
float Get2WordFloatU(uint16_t *Word1,float ScaleFactor) {
	float FloatOut;
	unsigned int IntVal;
	s1553_Double_Word WordSwap;

	WordSwap.MSWord = Word1[0];
	WordSwap.LSWord = Word1[1];
	IntVal=(WordSwap.MSWord<<16)|WordSwap.LSWord;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
float Get2WordFloatS(uint16_t *Word1,float ScaleFactor) {
	float FloatOut;
	int IntVal;
	s1553_Double_Word WordSwap;

	WordSwap.MSWord = Word1[0];
	WordSwap.LSWord = Word1[1];
	IntVal=(WordSwap.MSWord<<16)|WordSwap.LSWord;
	FloatOut=(float)IntVal;
	return(FloatOut/ScaleFactor);
}
double Get2WordDoubleS(uint16_t *Word1,double ScaleFactor) {
	double DoubleOut;
	int IntVal;
	s1553_Double_Word WordSwap;
	WordSwap.MSWord = Word1[0];
	WordSwap.LSWord = Word1[1];
	IntVal=(WordSwap.MSWord<<16)|WordSwap.LSWord;
	DoubleOut=(double)IntVal;
	return(DoubleOut/ScaleFactor);
}
double Get2WordDoubleU(uint16_t *Word1,double ScaleFactor) {
	double DoubleOut;
	unsigned int IntVal;
	s1553_Double_Word WordSwap;
	WordSwap.MSWord = Word1[0];
	WordSwap.LSWord = Word1[1];
	IntVal=(WordSwap.MSWord<<16)|WordSwap.LSWord;
	DoubleOut=(double)IntVal;
	return(DoubleOut/ScaleFactor);
}

uint16_t Convert1WordToShortU(uint16_t Word,float SF1, float SF2, float SF3){
	float FloatN;
	float FloatN1;
	float FloatN2;
	uint16_t ShortN;
	FloatN  = Get1WordFloatU(Word,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	ShortN = (short)FloatN2;
	return(ShortN);
}
int16_t Convert1WordToShortS(int16_t Word,float SF1, float SF2, float SF3){
	float FloatN;
	float FloatN1;
	float FloatN2;
	int16_t ShortN;
	FloatN  = Get1WordFloatS(Word,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	if (FloatN2>=FLOAT_2POWER_15){
		FloatN2=(FLOAT_2POWER_15 - 1);
	}
	else if (FloatN2<=(FLOAT_MINUS_1*FLOAT_2POWER_15))
		FloatN2=(FLOAT_MINUS_1*FLOAT_2POWER_15)+1;
	ShortN = (short)FloatN2;
	return(ShortN);
}
unsigned short Convert2WordToShortU(uint16_t *Word1,float SF1, float SF2, float SF3){
	float FloatN;
	float FloatN1;
	float FloatN2;
	unsigned short ShortN;
	FloatN  = Get2WordFloatU(Word1,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	if (FloatN2>=FLOAT_2POWER_16)
		FloatN2=(FLOAT_2POWER_16 - 1);
	ShortN = (unsigned short)FloatN2;
	return(ShortN);
}
short Convert2WordToShortS(uint16_t *Word1,float SF1, float SF2, float SF3){
	float FloatN;
	float FloatN1;
	float FloatN2;
	short ShortN;
	FloatN  = Get2WordFloatS(Word1,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	if (FloatN2>=FLOAT_2POWER_15){
		FloatN2=(FLOAT_2POWER_15 - 1);
	}
	else if (FloatN2<=(FLOAT_MINUS_1*FLOAT_2POWER_15))
		FloatN2=(FLOAT_MINUS_1*FLOAT_2POWER_15)+1;
	ShortN = (short)FloatN2;
	return(ShortN);
}
int Convert2WordToIntS(uint16_t *Word1,float SF1, float SF2, float SF3){
	double FloatN;
	double FloatN1;
	double FloatN2;
	int IntN;
	FloatN  = Get2WordFloatU(Word1,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	if (FloatN2>=FLOAT_2POWER_32){
		FloatN2=(FLOAT_2POWER_32 - 1);
	}
	else if (FloatN2<=(FLOAT_MINUS_1*FLOAT_2POWER_32))
		FloatN2=(FLOAT_MINUS_1*FLOAT_2POWER_32)+1;
	IntN = (int)FloatN2;
	return(IntN);
}
unsigned int Convert2WordToIntU(uint16_t *Word1,float SF1, float SF2, float SF3){
	float FloatN;
	float FloatN1;
	float FloatN2;
	unsigned int IntN;
	FloatN  = Get2WordFloatS(Word1,SF1);
	FloatN1 = (FloatN * SF2);
	FloatN2 = (FloatN1 * SF3);
	if (FloatN2>=FLOAT_2POWER_31){
		FloatN2=(FLOAT_2POWER_31 - 1);
	}
	else if (FloatN2<=(FLOAT_MINUS_1*FLOAT_2POWER_31))
		FloatN2=(FLOAT_MINUS_1*FLOAT_2POWER_31)+1;
	IntN = (short)FloatN2;
	return(IntN);
}
