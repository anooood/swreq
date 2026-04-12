/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Flash.c>
 *
 *  Description:
 *  ------------
 *  Implements the interface for non-volatile flash memory operations.
 *
 *  Function(s):
 *  ------------
 *  - FlashSendWriteEnableCommand : Sends a command to enable writing to flash memory.
 *  - FlashReadStatusRegister     : Reads the status register from the flash device.
 *  - FlashWaitForWriteEnableLatch: Waits until the write-enable latch is set in the flash status.
 *  - FlashWaitForOperationToFinish: Blocks until the current flash operation (write/erase) is complete.
 *  - FlashClearStatusErrorFlags  : Clears any error flags in the flash status register.
 *  - FlashGetRealAddr            : Calculates the physical memory address in flash based on logical address.
 *  - SendFlashBankSelect         : Sends a bank selection command to access specific flash memory regions.
 *  - FlashReadID                 : Reads the manufacturer and device ID from the flash chip.
 *  - SetupQuadMode               : Configures the flash for Quad I/O mode to enable higher data rates.
 *  - FlashInit                   : Initializes the flash memory interface and configures required settings.
 *  - FlashReadData               : Reads a block of data from a specified flash address.
 *  - FlashWriteData              : Writes a block of data to a specified flash address.
 *  - FlashEraseDevice            : Erases the entire flash memory chip.
 *  - FlashEraseSector            : Erases a single sector of the flash memory.
 *  ...
 *
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sleep.h>
// #include "xqspips_hw.h"
// #include "xqspips.h"
#include "Flash.h"
#include "FPGA.h"
#include "Timer.h"


// The following constants define the commands which may be sent to the Flash
// device.
#define QUAD_WRITE_CMD    0x32
#define QUAD_READ_CMD     0x6B
#define READ_ID_CMD       0x9F
#define WRITE_STATUS_CMD  0x01
#define WRITE_CMD         0x02
#define READ_CMD          0x03
#define WRITE_DISABLE_CMD 0x04
#define READ_STATUS_CMD   0x05
#define WRITE_ENABLE_CMD  0x06
#define FAST_READ_CMD     0x0B
#define DUAL_READ_CMD     0x3B
#define QUAD_READ_CMD     0x6B
#define BULK_ERASE_CMD    0xC7
#define SEC_ERASE_CMD     0xD8
#define SUBSEC_ERASE_CMD  0x20
#define READ_ID           0x9F
#define READ_CONFIG_CMD   0x35
#define WRITE_CONFIG_CMD  0x01
#define BANK_REG_RD       0x16
#define BANK_REG_WR       0x17
#define CLEAR_STATUS_REG  0x30
// Bank register is called Extended Address Register in Micron
#define EXTADD_REG_RD     0xC8
#define EXTADD_REG_WR     0xC5
#define DIE_ERASE_CMD     0xC4
#define READ_FLAG_STATUS_CMD  0x70

// The following constants define the offsets within a FlashBuffer data
// type for each kind of data.  Note that the read data offset is not the
// same as the write data because the QSPI driver is designed to allow full
// duplex transfers such that the number of bytes received is the number
// sent and received.
#define DUMMY_SIZE        1 // Number of dummy bytes for fast, dual & quad reads
#define BANK_SEL_SIZE     2 // BRWR or EARWR command + 1 byte bank value

// The following constants specify the extra bytes which are sent to the
// Flash on the QSPI interface, that are not data, but control information
// which includes the command and address
#define OVERHEAD_SIZE   4
// Base address of Flash1
#define FLASH1BASE 0x0000000
// Sixteen MB
#define SIXTEENMB 0x1000000
// Mask for quad enable bit in Flash configuration register
#define FLASH_QUAD_EN_MASK 0x02
#define FLASH_SRWD_MASK 0x80
// Bank mask
#define BANKMASK 0xF000000

#define FOURKB      0x00001000
#define FOURKB_MASK   0xFFFFF000

// Identification of Flash
// Micron:
//  Byte 0 is Manufacturer ID;
//  Byte 1 is first byte of Device ID - 0xBB or 0xBA
//  Byte 2 is second byte of Device ID describes flash size:
//  128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
#define MICRON_ID_BYTE0     0x20
#define MICRON_ID_BYTE2_128 0x18
#define MICRON_ID_BYTE2_256 0x19
#define MICRON_ID_BYTE2_512 0x20
#define MICRON_ID_BYTE2_1G  0x21

// Spansion:
//  Byte 0 is Manufacturer ID = 0x01;
//  Byte 1 is Device ID - Memory Interface type - 0x20 or 0x02
//  Byte 2 is second byte of Device ID describes flash size:
//  128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
#define SPANSION_ID_BYTE0     0x01
#define SPANSION_ID_BYTE2_128 0x18
#define SPANSION_ID_BYTE2_256 0x19
#define SPANSION_ID_BYTE2_512 0x20

// Winbond:
//  Byte 0 is Manufacturer ID = 0xEF;
//  Byte 1 is Device ID - Memory Interface type - 0x20 or 0x02
//  Byte 2 is second byte of Device ID describes flash size:
//  128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
#define WINBOND_ID_BYTE0              0xEF
#define WINBOND_ID_BYTE2_128          0x18

// Flash defines
#define FLASH_SIZE_ID_128M            0x18
#define FLASH_SIZE_ID_256M            0x19
#define FLASH_SIZE_ID_512M            0x20
#define FLASH_SIZE_ID_1G              0x21

// Size in bytes
#define FLASH_SIZE_128M               0x1000000
#define FLASH_SIZE_256M               0x2000000
#define FLASH_SIZE_512M               0x4000000
#define FLASH_SIZE_1G                 0x8000000

// The index for Flash config table
// Spansion
#define SPANSION_INDEX_START          0
#define FLASH_CFG_TBL_SINGLE_128_SP   SPANSION_INDEX_START
#define FLASH_CFG_TBL_STACKED_128_SP  (SPANSION_INDEX_START + 1)
#define FLASH_CFG_TBL_PARALLEL_128_SP (SPANSION_INDEX_START + 2)
#define FLASH_CFG_TBL_SINGLE_256_SP   (SPANSION_INDEX_START + 3)
#define FLASH_CFG_TBL_STACKED_256_SP  (SPANSION_INDEX_START + 4)
#define FLASH_CFG_TBL_PARALLEL_256_SP (SPANSION_INDEX_START + 5)
#define FLASH_CFG_TBL_SINGLE_512_SP   (SPANSION_INDEX_START + 6)
#define FLASH_CFG_TBL_STACKED_512_SP  (SPANSION_INDEX_START + 7)
#define FLASH_CFG_TBL_PARALLEL_512_SP (SPANSION_INDEX_START + 8)
// Micron
#define MICRON_INDEX_START            (FLASH_CFG_TBL_PARALLEL_512_SP + 1)
#define FLASH_CFG_TBL_SINGLE_128_MC   MICRON_INDEX_START
#define FLASH_CFG_TBL_STACKED_128_MC  (MICRON_INDEX_START + 1)
#define FLASH_CFG_TBL_PARALLEL_128_MC (MICRON_INDEX_START + 2)
#define FLASH_CFG_TBL_SINGLE_256_MC   (MICRON_INDEX_START + 3)
#define FLASH_CFG_TBL_STACKED_256_MC  (MICRON_INDEX_START + 4)
#define FLASH_CFG_TBL_PARALLEL_256_MC (MICRON_INDEX_START + 5)
#define FLASH_CFG_TBL_SINGLE_512_MC   (MICRON_INDEX_START + 6)
#define FLASH_CFG_TBL_STACKED_512_MC  (MICRON_INDEX_START + 7)
#define FLASH_CFG_TBL_PARALLEL_512_MC (MICRON_INDEX_START + 8)
#define FLASH_CFG_TBL_SINGLE_1GB_MC   (MICRON_INDEX_START + 9)
#define FLASH_CFG_TBL_STACKED_1GB_MC  (MICRON_INDEX_START + 10)
#define FLASH_CFG_TBL_PARALLEL_1GB_MC (MICRON_INDEX_START + 11)
// Winbond
#define WINBOND_INDEX_START           (FLASH_CFG_TBL_PARALLEL_1GB_MC + 1)
#define FLASH_CFG_TBL_SINGLE_128_WB   WINBOND_INDEX_START
#define FLASH_CFG_TBL_STACKED_128_WB  (WINBOND_INDEX_START + 1)
#define FLASH_CFG_TBL_PARALLEL_128_WB (WINBOND_INDEX_START + 2)

// The following constants map to the XPAR parameters created in the
// xparameters.h file. They are defined here such that a user can easily
// change all the needed parameters in one place.
#define QSPI_DEVICE_ID                XPAR_XQSPIPS_0_DEVICE_ID

// The following defines are for dual flash stacked mode interface.
#define LQSPI_CR_FAST_QUAD_READ       0x0000006B // Fast Quad Read output
#define LQSPI_CR_1_DUMMY_BYTE         0x00000100 // 1 Dummy Byte between address and return data

#define SINGLE_QSPI_CONFIG_WRITE      (LQSPI_CR_1_DUMMY_BYTE | LQSPI_CR_FAST_QUAD_READ)
#define DUAL_STACK_CONFIG_WRITE       (XQSPIPS_LQSPI_CR_TWO_MEM_MASK | LQSPI_CR_1_DUMMY_BYTE | LQSPI_CR_FAST_QUAD_READ)
#define DUAL_QSPI_CONFIG_WRITE        (XQSPIPS_LQSPI_CR_TWO_MEM_MASK | XQSPIPS_LQSPI_CR_SEP_BUS_MASK | LQSPI_CR_1_DUMMY_BYTE | LQSPI_CR_FAST_QUAD_READ)


typedef struct
{
  unsigned int  SectSize;          // Individual sector size or combined sector size in case of parallel config
  unsigned int  NumSect;           // Total no. of sectors in one/two flash devices
  unsigned int  PageSize;          // Individual page size or combined page size in case of parallel config
  unsigned int  NumPage;           // Total no. of pages in one/two flash devices
  unsigned int  FlashDeviceSize;   // This is the size of one flash device NOT the combination of both devices, if present
  unsigned char ManufacturerID;    // Manufacturer ID - used to identify make
  unsigned char DeviceIDMemSize;   // Byte of device ID indicating the memory size
  unsigned int  SectMask;          // Mask to get sector start address
  unsigned int  PageMask;          // Mask to get page start address
  unsigned char NumDie;            // No. of die forming a single flash
} sFlashDeviceInfo;

typedef struct {
  unsigned int MakeId;
  unsigned int SizeId;
  unsigned int Size;
  unsigned int SectorCount;
  unsigned int SectorSize;
} sFlash;


static sFlash Flash;

// The instances to support the device drivers are global such that they
// are initialized to zero each time the program runs. They could be local
// but should at least be static so they are zeroed.
static XQspiPs QspiInstance;
static XQspiPs_Config *QspiConfig;
static XQspiPs *QspiPtr = &QspiInstance;
static unsigned int FCTIndex;    // Flash configuration table index
static unsigned char FlashReadBuf[5004];
static unsigned char FlashWriteBuf[1032];
static sFlashDeviceInfo Flash_Config_Table[24] = {
    // Spansion
    {0x10000, 0x100, 256, 0x10000, 0x1000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x10000, 0x200, 256, 0x20000, 0x1000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x20000, 0x100, 512, 0x10000, 0x1000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFE0000, 0xFFFFFE00, 1},
    {0x10000, 0x200, 256, 0x20000, 0x2000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x10000, 0x400, 256, 0x40000, 0x2000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x20000, 0x200, 512, 0x20000, 0x2000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFE0000, 0xFFFFFE00, 1},
    {0x40000, 0x100, 512, 0x20000, 0x4000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFFC0000, 0xFFFFFE00, 1},
    {0x40000, 0x200, 512, 0x40000, 0x4000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFFC0000, 0xFFFFFE00, 1},
    {0x80000, 0x100, 1024, 0x20000, 0x4000000,
        SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFF80000, 0xFFFFFC00, 1},
    // Spansion 1Gbit is handled as 512Mbit stacked
    // Micron
    {0x10000, 0x100, 256, 0x10000, 0x1000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x10000, 0x200, 256, 0x20000, 0x1000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x20000, 0x100, 512, 0x10000, 0x1000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFE0000, 0xFFFFFE00, 1},
    {0x10000, 0x200, 256, 0x20000, 0x2000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x10000, 0x400, 256, 0x40000, 0x2000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x20000, 0x200, 512, 0x20000, 0x2000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFE0000, 0xFFFFFE00, 1},
    {0x10000, 0x400, 256, 0x40000, 0x4000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFF0000, 0xFFFFFF00, 2},
    {0x10000, 0x800, 256, 0x80000, 0x4000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFF0000, 0xFFFFFF00, 2},
    {0x20000, 0x400, 512, 0x40000, 0x4000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFE0000, 0xFFFFFE00, 2},
    {0x10000, 0x800, 256, 0x80000, 0x8000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFF0000, 0xFFFFFF00, 4},
    {0x10000, 0x1000, 256, 0x100000, 0x8000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFF0000, 0xFFFFFF00, 4},
    {0x20000, 0x800, 512, 0x80000, 0x8000000,
        MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFE0000, 0xFFFFFE00, 4},
    // Winbond
    {0x10000, 0x100, 256, 0x10000, 0x1000000,
        WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x10000, 0x200, 256, 0x20000, 0x1000000,
        WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFF0000, 0xFFFFFF00, 1},
    {0x20000, 0x100, 512, 0x10000, 0x1000000,
        WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFE0000, 0xFFFFFE00, 1}
};


//=======================  Function Header  ===================================
//                void FlashSendWriteEnableCommand(void)
//
// Coded by       :  MC Wessels
// Date           :  2014/09/13
// Description    :
//=============================================================================
static void FlashSendWriteEnableCommand(void)
{
	unsigned char write_buf[2];
	unsigned char status[2];


	write_buf[0] = WRITE_ENABLE_CMD;
	XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 1);
}


//=======================  Function Header  ===================================
//                unsigned char FlashReadStatusRegister(void)
//
// Coded by       :  MC Wessels
// Date           :  2014/09/13
// Description    :
//=============================================================================
static unsigned char FlashReadStatusRegister(void)
{
	unsigned char write_buf[4] = {READ_STATUS_CMD,0,0,0};
	unsigned char status[4];


	// Must send at least 2 bytes
	XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 2);

	return status[1];
}

//=======================  Function Header  ===================================
//                void FlashWaitForWriteEnableLatch(void)
//
// Coded by       :  MC Wessels
// Date           :  2014/09/13
// Description    :
//=============================================================================
static void FlashWaitForWriteEnableLatch(void)
{
	unsigned char status;


	do {
		status = FlashReadStatusRegister();
	} while ((status & 0x02) == 0);
}

//=======================  Function Header  ===================================
//                void FlashWaitForOperationToFinish(void)
//
// Coded by       :  MC Wessels
// Date           :  2014/09/13
// Description    :
//=============================================================================
static void FlashWaitForOperationToFinish(void)
{
	unsigned char status;


	do {
		status = FlashReadStatusRegister();
	} while ((status & 0x01) == 0x01);
}

//=======================  Function Header  ===================================
//                void FlashClearStatusErrorFlags(void)
//
// Coded by       :  MC Wessels
// Date           :  2014/09/23
// Description    :
//=============================================================================
static void FlashClearStatusErrorFlags(void)
{
	unsigned char write_buf[2] = {0, 0};
	unsigned char status[2];


	// Check if any of the Status register bits, SRWD, P_ERR, E_ERR, WEL or WIP is
	// set and clear if necessary
	if (FlashReadStatusRegister() & 0xE3) {
		// Clear the status registers
		write_buf[0] = CLEAR_STATUS_REG;
		XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 1);

		// Read the status register and check if WEL are set
		if ((FlashReadStatusRegister() & 0x03) == 0x02) {
			write_buf[0] = WRITE_DISABLE_CMD;
			XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 2);
		}
	}
}

//=======================  Function Header  ===================================
// This functions translates the address based on the type of interconnection.
// In case of stacked, this function asserts the corresponding slave select.
//
// @param  QspiPtr is a pointer to the QSPI driver component to use.
// @param  Address which is to be accessed (for erase, write or read)
//
// @return RealAddr is the translated address - for single it is unchanged;
//       for stacked, the lower flash size is subtracted;
//       for parallel the address is divided by 2.
//
// @note   None.
//=============================================================================
static unsigned int FlashGetRealAddr(XQspiPs *QspiPtr, unsigned int Address)
{
	unsigned int lqspi_cr;
	unsigned int real_addr = Address;


	// Check the flash configuration
	switch (QspiPtr->Config.ConnectionMode) {
  	// Single flash -
  	case XQSPIPS_CONNECTION_MODE_SINGLE  : real_addr = Address; break;
		// Dual stacked -
		case XQSPIPS_CONNECTION_MODE_STACKED : {
      //  Get the current LQSPI Config reg value
      lqspi_cr = XQspiPs_GetLqspiConfigReg(QspiPtr);

      // Select lower or upper Flash based on sector address
      if (Address & Flash_Config_Table[FCTIndex].FlashDeviceSize) {
        // Set selection to U_PAGE
        XQspiPs_SetLqspiConfigReg(QspiPtr, lqspi_cr | XQSPIPS_LQSPI_CR_U_PAGE_MASK);
        // Subtract first flash size when accessing second flash
        real_addr = Address & (~Flash_Config_Table[FCTIndex].FlashDeviceSize);
      }
      else {
        // Set selection to L_PAGE
        XQspiPs_SetLqspiConfigReg(QspiPtr, lqspi_cr & (~XQSPIPS_LQSPI_CR_U_PAGE_MASK));
        real_addr = Address;
      }

      // Assert the Flash chip select.
      XQspiPs_SetSlaveSelect(QspiPtr);
    }  break;
	  // Dual parallel -
	  // The effective address in each flash is the actual address / 2
  	case XQSPIPS_CONNECTION_MODE_PARALLEL: real_addr = Address / 2;   break;
  	// Other - RealAddr wont be assigned in this case;
    default:  break;
	}

	return real_addr;
}

//=======================  Function Header  ===================================
// This functions selects the current bank
//
// @param  QspiPtr is a pointer to the QSPI driver component to use.
// @param  Pointer to the write buffer which contains data to be transmitted
// @param  BankSel is the bank to be selected in the flash device(s).
//
// @return XST_SUCCESS if bank selected, otherwise XST_FAILURE.
//
// @note   None.
//=============================================================================
static int SendFlashBankSelect(XQspiPs *QspiPtr, unsigned int BankSel)
{
	unsigned int status;


	// Bank select commands for Micron and Spansion are different
	if (Flash_Config_Table[FCTIndex].ManufacturerID == MICRON_ID_BYTE0 ) {
		// For micron command WREN should be sent first
		// except for some specific feature set
		FlashWriteBuf[0] = WRITE_ENABLE_CMD;
		status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, NULL, 1);

		if (status != XST_SUCCESS) {
		  return XST_FAILURE;
		}

		// Send the Extended address register write command
		// written, no receive buffer required
		FlashWriteBuf[0]   = EXTADD_REG_WR;
		FlashWriteBuf[1] = BankSel;

		status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, NULL, BANK_SEL_SIZE);

		if (status != XST_SUCCESS) {
		  return XST_FAILURE;
		}
	}

	if (Flash_Config_Table[FCTIndex].ManufacturerID == SPANSION_ID_BYTE0) {
		FlashWriteBuf[0] = BANK_REG_WR;
		FlashWriteBuf[1] = BankSel;

		// Send the Extended address register write command
		// written, no receive buffer required
		status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, NULL, BANK_SEL_SIZE);

		if (status != XST_SUCCESS) {
		  return XST_FAILURE;
		}
	}

	// For testing - Read bank to verify
	if (Flash_Config_Table[FCTIndex].ManufacturerID == SPANSION_ID_BYTE0) {
		FlashWriteBuf[0] = BANK_REG_RD;
		FlashWriteBuf[1] = 0x00;

		// Send the Extended address register write command
		// written, no receive buffer required

		status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf,	BANK_SEL_SIZE);

		if (status != XST_SUCCESS) {
		  return XST_FAILURE;
		}
	}

	if (Flash_Config_Table[FCTIndex].ManufacturerID == MICRON_ID_BYTE0) {
		FlashWriteBuf[0] = EXTADD_REG_RD;
		FlashWriteBuf[1] = 0x00;

		// Send the Extended address register write command
		// written, no receive buffer required
		status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf,	BANK_SEL_SIZE);

		if (status != XST_SUCCESS) {
		  return XST_FAILURE;
		}
	}

	if (FlashReadBuf[1] != BankSel) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

//=======================  Function Header  ===================================
// This function reads serial FLASH ID connected to the SPI interface.
// It then deduces the make and size of the flash and obtains the
// connection mode to point to corresponding parameters in the flash
// configuration table. The flash driver will function based on this and
// it presently supports Micron and Spansion - 128, 256 and 512Mbit and
// Winbond 128Mbit
//
// @param  none
//
// @return XST_SUCCESS if read id, otherwise XST_FAILURE.
//
// @note   None.
//=============================================================================
static unsigned int FlashReadID(XQspiPs *QspiPtr)
{
	unsigned int Status;


	// Read ID in Auto mode.
	FlashWriteBuf[0] = READ_ID_CMD;
	FlashWriteBuf[1] = 0x00;   /* 3 dummy bytes */
	FlashWriteBuf[2] = 0x00;
	FlashWriteBuf[3] = 0x00;

	Status = XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf, 128);

	if (Status != XST_SUCCESS) {
	  return XST_FAILURE;
	}

	// Deduce flash make
	Flash.MakeId = FlashReadBuf[1];

	// Deduce flash Size
	Flash.SizeId = FlashReadBuf[3];

	switch (Flash.SizeId) {
    case 0x18: FCTIndex = QspiPtr->Config.ConnectionMode + 0; break;
    case 0x19: FCTIndex = QspiPtr->Config.ConnectionMode + 3; break;
    case 0x20: FCTIndex = QspiPtr->Config.ConnectionMode + 6; break;
    case 0x21: FCTIndex = QspiPtr->Config.ConnectionMode + 9; break;
  }

  switch (Flash.MakeId) {
    case SPANSION_ID_BYTE0: {FCTIndex += SPANSION_INDEX_START;} break;
    case MICRON_ID_BYTE0  : {FCTIndex += MICRON_INDEX_START;  } break;
    case WINBOND_ID_BYTE0 : {FCTIndex += WINBOND_INDEX_START; } break;
  }

  if (QspiPtr->Config.ConnectionMode == XQSPIPS_CONNECTION_MODE_PARALLEL) {
    Flash.Size = 2 * Flash_Config_Table[FCTIndex].FlashDeviceSize;
  }
  else {
    Flash.Size = Flash_Config_Table[FCTIndex].FlashDeviceSize;
  }

  Flash.SectorSize = Flash_Config_Table[FCTIndex].SectSize;
  Flash.SectorCount = Flash.Size / Flash.SectorSize;

	return XST_SUCCESS;
}

//============================= FUNCTION HEADER ================================
//                          unsigned int SetupQuadMode(void)
//==============================================================================
static unsigned int SetupQuadMode(void)
{
	// Read configuration register to see if Cr Bit 1 = 1
  FlashWriteBuf[0] = WRITE_ENABLE_CMD;
  FlashWriteBuf[1] = 0;
	FlashWriteBuf[2] = 2;
	FlashWriteBuf[3] = 0;

	XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, NULL, 1);

	FlashWriteBuf[0] = WRITE_CONFIG_CMD;

	XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf, 3);

	FlashWriteBuf[0] = READ_CONFIG_CMD;

	XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf, 2);

	return ((FlashReadBuf[1] & 0x2) == 2) ? XST_SUCCESS : XST_FAILURE;
}

//=========================== FUNCTION HEADER ==================================
//                    unsigned int FlashInitInterface(sFlashInfo *Flash)
//==============================================================================
unsigned int FlashInit(void)
{
	int status;


	// Select dual parallel flash
	*((volatile unsigned int *) (FPGA_PLD_CONTROL_INTERFACE_ADDR + 0x10)) = 3;
	//*((volatile unsigned int *) FPGA_PLD_CTRL_REG) = Select & 0x3;

	Delay(2000);

  // Get config and initialise the QSPI driver so that it's ready to use
	QspiConfig = XQspiPs_LookupConfig(QSPI_DEVICE_ID);

	if (NULL == QspiConfig) {
	  return FALSE;
	}

	status = XQspiPs_CfgInitialize(QspiPtr, QspiConfig, QspiConfig->BaseAddress);

	if (status != XST_SUCCESS) {
	  return FALSE;
	}

	// Perform a self-test to check hardware build
	status = XQspiPs_SelfTest(QspiPtr);

	if (status != XST_SUCCESS) {
	  return FALSE;
	}

	// Set the pre-scaler for QSPI clock
	XQspiPs_SetClkPrescaler(QspiPtr, XQSPIPS_CLK_PRESCALE_4);

	// Set Manual Start and Manual Chip select options and drive the HOLD_B high.
	XQspiPs_SetOptions(QspiPtr, XQSPIPS_FORCE_SSELECT_OPTION | XQSPIPS_MANUAL_START_OPTION | XQSPIPS_HOLD_B_DRIVE_OPTION);

	if(QspiConfig->ConnectionMode == XQSPIPS_CONNECTION_MODE_SINGLE) {
		// Setup for single flash memory
		// XQspiPs_SetLqspiConfigReg(QspiPtr, SINGLE_QSPI_CONFIG_WRITE);
	}

	if(QspiConfig->ConnectionMode == XQSPIPS_CONNECTION_MODE_STACKED) {
		// Enable two flash memories, Shared bus (NOT separate bus), L_PAGE selected by default
		XQspiPs_SetLqspiConfigReg(QspiPtr, DUAL_STACK_CONFIG_WRITE);
	}

	if(QspiConfig->ConnectionMode == XQSPIPS_CONNECTION_MODE_PARALLEL) {
  	// Enable two flash memories on separate buses
		XQspiPs_SetLqspiConfigReg(QspiPtr, DUAL_QSPI_CONFIG_WRITE);
	}

	// Assert the Flash chip select.
	XQspiPs_SetSlaveSelect(QspiPtr);

	// Read flash ID and obtain all flash related information
	// It is important to call the read id function before
	// performing proceeding to any operation, including
	// preparing the write_buf to get the FCTIndex
	FlashReadID(QspiPtr);

	FlashClearStatusErrorFlags();
	SetupQuadMode();
	FlashClearStatusErrorFlags();

	usleep(100);

	return TRUE;
}

//=======================  Function Header  ===================================
//      void FlashReadData(unsigned int Address, unsigned char *ReadBfrPtr, unsigned int ByteCount)
//
// Coded by       :  MC Wessels/Yolindi du Jongh
// Date           :  11/3/2014
// Description    :  This function reads data from the flash and places it first
//                   in the temporary ReadBuffer. The size is limited to 4KB.
//                   Hence the function is split up into 4096 byte reads.
//=============================================================================
void FlashReadData(unsigned int Address, unsigned char *Data, unsigned int ByteCount)
{
	unsigned int real_addr;
	unsigned int real_byte_cnt;
	unsigned int bank_sel = 0;
	unsigned char shift_size = OVERHEAD_SIZE + DUMMY_SIZE;
	unsigned char bank_init_flag = 1;
	unsigned int bytes_written = 0;
	XQspiPs *qspi_ptr = &QspiInstance;


	while (ByteCount > 0) {
		// Translate address based on type of connection
		// If stacked assert the slave select based on address
		real_addr = FlashGetRealAddr(qspi_ptr, Address);

		if ((bank_sel != (real_addr/SIXTEENMB)) || (bank_init_flag)) {
			bank_sel = real_addr/SIXTEENMB;
			SendFlashBankSelect(qspi_ptr, bank_sel);
			bank_init_flag = 0;
		}

		// If data to be read spans beyond the current 4KB, then calculate
		// RealByteCnt in current subsector. Else RealByteCnt is the same as ByteCount
		if ((Address & FOURKB_MASK) != ((Address+ByteCount) & FOURKB_MASK)) {
			real_byte_cnt = (Address & FOURKB_MASK) + FOURKB - Address;
		}
		else {
			real_byte_cnt = ByteCount;
		}

		// Send the read command to the Flash to read the specified number
		// of bytes from the Flash, send the read command and address and
		// receive the specified number of bytes of data in the data buffer
		FlashWriteBuf[0] = QUAD_READ_CMD;
		FlashWriteBuf[1] = (real_addr >> 16) & 0xFF; //(unsigned char)((RealAddr & 0xFF0000) >> 16);
		FlashWriteBuf[2] = (real_addr >> 8) & 0xFF; //(unsigned char)((RealAddr & 0xFF00) >> 8);
		FlashWriteBuf[3] = real_addr & 0xFF; //(unsigned char)(RealAddr & 0xFF);

		XQspiPs_PolledTransfer(qspi_ptr, FlashWriteBuf, FlashReadBuf, real_byte_cnt + shift_size);

		// To discard the first 5 dummy bytes, shift the data in read buffer
		memcpy(&Data[bytes_written], &FlashReadBuf[shift_size], real_byte_cnt);

		// Increase address to next bank
		Address = (Address & FOURKB_MASK) + FOURKB;
		bytes_written += real_byte_cnt;

		// Decrease byte count by bytes already read.
		ByteCount -= real_byte_cnt;
	}
}


void FlashWriteData(unsigned int Address, unsigned char *Data, unsigned int ByteCount)
{
  unsigned int real_addr;
  unsigned int bank_sel = 0;
  //unsigned int index;
  unsigned int remaining;
  unsigned int cnt;
  unsigned int page_end_addr;
  unsigned int offset;


  remaining = ByteCount;
  offset = 0;

  while (remaining > 0) {
    // Translate address based on type of connection
    real_addr = FlashGetRealAddr(QspiPtr, Address + offset);

    // Bank Select
    if (bank_sel != (real_addr / SIXTEENMB)) {
      bank_sel = real_addr / SIXTEENMB;
      SendFlashBankSelect(QspiPtr, bank_sel);
    }

    // Send the write enable command to the Flash so that it can be
    // written to, this needs to be sent as a separate transfer before
    // the write
    FlashSendWriteEnableCommand();
    FlashWaitForWriteEnableLatch();

    page_end_addr = (Address + offset) | 0x1FF;
    cnt = page_end_addr - (Address + offset) + 1;

    if (cnt > remaining) {
      cnt = remaining;
    }

    // Setup the write command with the specified address and data for the Flash
    FlashWriteBuf[0] = WRITE_CMD;
    FlashWriteBuf[1] = (real_addr >> 16) & 0xFF;
    FlashWriteBuf[2] = (real_addr >> 8) & 0xFF;
    FlashWriteBuf[3] = real_addr & 0xFF;

    // Put the data in the write buffer
    //for (index = 0; index < cnt; index++) {
    //  FlashWriteBuf[index + 4] = WriteBfrPtr[offset + index];
    //}
    memcpy(&FlashWriteBuf[4], &Data[offset], cnt);

    // Send the write command, address, and data to the Flash to be written
    XQspiPs_PolledTransfer(QspiPtr, FlashWriteBuf, FlashReadBuf, cnt + 4);

    // Wait for the write command to the Flash to be completed, it takes
    // some time for the data to be written
    FlashWaitForOperationToFinish();

    offset += cnt;
    remaining -= cnt;
  }
}


//=======================  Function Header  ===================================
//                    void FlashEraseDevice(void)
//
// Coded by       :  MC Wessels
// Date           :  11/3/2014
// Description    :
//=============================================================================
void FlashEraseDevice(void)
{
	unsigned char write_buf[32];
	unsigned char status[4];


	// Send the write enable command to the SEEPOM so that it can be written
	// to, this needs to be sent as a separate transfer before the write
	// Verify that Write Enable Latch (WEL) in status register is set
	FlashSendWriteEnableCommand();
	FlashWaitForWriteEnableLatch();

	// Do a device erase on the flash device
	write_buf[0] = BULK_ERASE_CMD;
	write_buf[1] = 0;

	// Send the sector erase command and address
	XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 1);

	usleep(300000);

	// Wait for the sector erase command to the Flash to be completed,
	// check the Write in Progress (WIP) in the status register
	FlashWaitForOperationToFinish();
}

//=======================  Function Header  ===================================
//     void FlashEraseData(unsigned int Address)
//
// Coded by       :  MC Wessels
// Date           :  11/3/2014
// Description    :
//=============================================================================
unsigned char FlashEraseSector(unsigned int Address)
{
	unsigned char write_buf[32];
	unsigned char status[4];
	unsigned int real_addr = 0;
	unsigned int bank_sel= 0;
	unsigned int index;
	unsigned int sector;
	unsigned int remaining;
	unsigned int offset;
	unsigned int dword_cnt;
	unsigned int byte_cnt;
	unsigned int dword_buf[0x400];


	// Translate address based on type of connection
	// If stacked assert the slave select based on address
	real_addr = FlashGetRealAddr(QspiPtr, Address);

	// Check bank and send bank select if new bank
	if (bank_sel != (real_addr/SIXTEENMB)) {
		bank_sel = real_addr/SIXTEENMB;
		SendFlashBankSelect(QspiPtr, bank_sel);
	}

	// Send the write enable command to the SEEPOM so that it can be written
	// to, this needs to be sent as a separate transfer before the write
	// Verify that Write Enable Latch (WEL) in status register is set
	FlashSendWriteEnableCommand();
	FlashWaitForWriteEnableLatch();

	// Setup the write command with the specified address and data for the Flash
	// This ensures 3B address is sent to flash even with address
	// greater than 128Mb.
	write_buf[0] = SEC_ERASE_CMD;
	write_buf[1] = (real_addr >> 16) & 0xFF;
  write_buf[2] = (real_addr >> 8) & 0xFF;
  write_buf[3] = real_addr & 0xFF;

	// Send the sector erase command and address
	XQspiPs_PolledTransfer(QspiPtr, write_buf, status, 4);

	// Wait for the sector erase command to the Flash to be completed,
	// check the Write in Progress (WIP) in the status register
	FlashWaitForOperationToFinish();

	// Verify that the sector is successfully erased by checking that all bytes are 0xFF
  sector = Address / Flash.SectorSize;
  offset = sector * Flash.SectorSize;
  remaining = (Flash.SectorSize + 3) / 4;

  while (remaining > 0) {
    dword_cnt = remaining / 4;

    // Limit the number of words to read
    if (dword_cnt > 0x400) {
      dword_cnt = 0x400;
    }

    byte_cnt = dword_cnt * 4;

    FlashReadData(offset, (unsigned char *) dword_buf, byte_cnt);

    for (index = 0; index < dword_cnt; index++) {
      if (dword_buf[index] != 0xFFFFFFFF) {
        return FALSE;
      }
    }

    offset += byte_cnt;
    remaining -= byte_cnt;
  }

  return TRUE;
}
