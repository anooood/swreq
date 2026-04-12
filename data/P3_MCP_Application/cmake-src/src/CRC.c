/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <CRC.c>
 *
 *  Description:
 *  ------------
 *  CRC Calculation.
 *
 *  Function(s):
 *  ------------
 *  - InitCRC:                     Initializes lookup table for CRC calculation
 *  - Calculate16BitCRC:           Calculates 16-bit on an array of data
 *  - Calculate32BitCRC:           Calculates 32-bit on an array of data
 *  - CalculateXOR_Checksum:       Calculates XOR checksum on an array of data
 *  - CalculateMIL_STD_1760D_Crc:  Calculates MIL STD 1760D CRC
 *  ...
 *
 *
 ******************************************************************************/

#define CRC_16BIT_GEN           0x8408
#define CRC_32BIT_GEN           0x04C11DB7  //0xEDB88320


static unsigned short CRC16_Lookup[256];
static unsigned int CRC32_Lookup[256];

void InitCRC(void)
{
  unsigned int index;
  unsigned int bit;
  unsigned int next_byte;
  unsigned int next_polynomial;
  unsigned int msb_set;


  // This lookup generator assumes that the polynomial is reflected

  for (index = 0; index < 256; index++) {
    next_polynomial = 0;
    next_byte = index;

    for (bit = 0; bit < 8; bit++) {
      msb_set = next_byte & 0x01;   // Because the polynomial is reflected, the MSB is now the first bit
      next_byte >>= 1;
      next_polynomial >>= 1;

      if (msb_set) {
        next_byte ^= CRC_16BIT_GEN;
        next_polynomial ^= CRC_16BIT_GEN;
      }
    }

    CRC16_Lookup[index] = next_polynomial;
  }

  // Do the same for the 32-bit lookup
  for (index = 0; index < 256; index++) {
    next_polynomial = 0;
    next_byte = index;

    for (bit = 0; bit < 8; bit++) {
      msb_set = next_byte & 0x01;   // Because the polynomial is reflected, the MSB is now the first bit
      next_byte >>= 1;
      next_polynomial >>= 1;

      if (msb_set) {
        next_byte ^= CRC_32BIT_GEN;
        next_polynomial ^= CRC_32BIT_GEN;
      }
    }

    CRC32_Lookup[index] = next_polynomial;
  }
}

unsigned short Calculate16BitCRC(unsigned char *Data, unsigned int Length)
{
  unsigned int index;
  unsigned short crc = 0;


  for (index = 0; index < Length; index++) {
    crc = CRC16_Lookup[Data[index] ^ (crc & 0xFF)] ^ (crc >> 8);
  }

  return crc;
}

unsigned int Calculate32BitCRC(unsigned char *Data, unsigned int Length)
{
  unsigned int index;
  unsigned int crc = 0xFFFFFFFF;


  for (index = 0; index < Length; index++) {
    crc = CRC32_Lookup[Data[index] ^ (crc & 0xFF)] ^ (crc >> 8);
  }

  crc ^= 0xFFFFFFFF;

  return crc;
}

unsigned char CalculateXOR_Checksum(unsigned char *Data, unsigned int Length)
{
  unsigned int index;
  unsigned char checksum = 0;


  for (index = 0; index < Length; index++) {
    checksum ^= Data[index];
  }

  return checksum;
}

unsigned short CalculateMIL_STD_1760D_Crc(unsigned short *Words, unsigned int WordCnt)
{
  unsigned int index;
  unsigned short crc = 0;

  crc = 0;
  for (index = 0; index < WordCnt; index++) {
    crc = Words[index] ^ ((crc << 1) | ((crc >> 15) & 0x01));
  }

  crc = ((crc << 1) | ((crc >> 15) & 0x01));

  return crc;
}
