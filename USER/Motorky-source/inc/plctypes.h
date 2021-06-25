/*
 * bur/plctypes.h
 * IEC data types
 *
 * Automation Studio
 * Copyright Bernecker&Rainer 1998-2005
 */
#ifndef _PLCTYPES_H_
#define _PLCTYPES_H_
#ifdef __cplusplus
extern "C" {
#endif

/* the names plcbit,plcbyte,plcword,plcdword,plctime,plcdt,plcdate,plctod,plcstring,plcwstring  
	are reserved words and must not be used in user-code */

typedef unsigned char  plcbit;
typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;
typedef signed char    SINT;
typedef signed short   INT;
typedef signed long    DINT;
typedef unsigned char  USINT;
typedef unsigned short UINT;
typedef unsigned long  UDINT;
typedef float          REAL;
typedef double         LREAL;
typedef unsigned char  plcbyte;
#ifndef	_BYTE_DEFINED_
	#define	_BYTE_DEFINED_
	typedef plcbyte        BYTE;
#endif
typedef unsigned short plcword;
#ifndef	_WORD_DEFINED_
	#define	_WORD_DEFINED_
	typedef plcword        WORD;
#endif
typedef unsigned long  plcdword;
#ifndef	_DWORD_DEFINED_
	#define	_DWORD_DEFINED_
	typedef plcdword       DWORD;
#endif
typedef signed long    plctime;
//typedef plctime        TIME;
typedef unsigned long  plcdt;
//typedef plcdt          DT;
//typedef plcdt          DATE_AND_TIME;
typedef unsigned long  plcdate;
//typedef plcdate        DATE;
typedef unsigned long  plctod;
//typedef plctod         TOD;
//typedef plctod         TIME_OF_DAY;
typedef char           plcstring;
typedef unsigned short plcwstring;
//typedef plcstring      STRING;
//typedef plcwstring     WSTRING;

#ifdef __cplusplus
}
#endif
#endif /* _PLCTYPES_H_ */

