
#ifndef __MOSBUS_APP_H
#define __MOSBUS_APP_H

#include <plctypes.h>

/* Datatypes and datatypes of function blocks */
typedef struct MDB_type
{	plcbit WriteCmdRgisterAsyn;
	plcbit WriteCmdRgisterSect2;
	plcbit WriteCmdRgister;
	plcbit WriteCmdCoils;
	plcbit ReadCmdRgister;
	plcbit ReadCmdCoils;
	plcbit MCGSCoilsWr[50];
	signed short MCGSRegWrSect2[10];
	signed short MCGSRegWr[100];
	plcbit MCGSCoils[100];
	signed short MCGSReg[100];
	plcbit ModbusCommOK;
} MDB_type;


extern struct MDB_type MDB;

void _CYC_MDB_TASK(void);
void _CYC_RW(void);

#endif


