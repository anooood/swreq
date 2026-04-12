/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <Main.h>
 *
 *  Description:
 *  ------------
 *  Header file for the Main.
 *
 ******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#define FIRMWARE_VERSION 			0x56ac0004
#define MAIN_NODE_CHECK_SERVICE		((unsigned int)(1000000)) // 1 second

typedef struct
{
	unsigned int 					FW_VersionRead;
}sMAIN_DeviceControls;

typedef struct
{
	unsigned int SystemTime;
	unsigned int OldSystemTime;
	unsigned int TimeDiff;
} sMAIN_ServiceControl;

typedef struct
{
	sMAIN_DeviceControls 	DeviceControls;				// Device control
	sMAIN_ServiceControl	ServiceControls;
	char 					AuxStr[AUX_FIFO_SIZE];		//
} sMAIN_StateSpace;

extern sMAIN_StateSpace MAIN_SS;




#endif // AUX_PORT_H
