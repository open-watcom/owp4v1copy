/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Win32 Serial handling routines
*
****************************************************************************/


#ifndef ZDP
   #define ZDP
#endif

#include <windows.h>
#undef NO_DATA		// Clashes with define in serial.h

#include <stdio.h>
#include <stddef.h>
#include "trpimp.h"
#include "trperr.h"
#include "serial.h"
//#include "tinyio.h"

#define BREAK_TIME      4		// In DOS timer ticks, I think (55ms)

int ErrorFlag;
int BreakFlag;
static int currentBaudRateIndex;
static DWORD lastTickReset;

static HANDLE hSerial = INVALID_HANDLE_VALUE;
static int comPortNumber = 1;

//////////////////////////////////////////////////////////////////////////
// Read and write caches to reduce the number of ReadFile and WriteFile 
// calls which have to occur.
//
static BYTE writeCache[1024];
static size_t writeCacheLevel;
static bool bBlockWriteMode;

static BYTE readCache[1024];
static size_t readCacheIndex;
static DWORD readCacheLevel;


void Trace(const char* fmt, ...)
{
	va_list va;
	static char traceBuffer[1000];
	
	va_start(va, fmt);

	vsprintf(traceBuffer, fmt, va);
	OutputDebugString(traceBuffer);

	va_end(va);
}

void ZeroWaitCount()
{
    lastTickReset = GetTickCount();
}


unsigned WaitCount()
{
    return( (GetTickCount() - lastTickReset) / 55 );
}


char *InitSys( void )
{
	DCB devCon;
	char deviceFileName[20];
	COMMTIMEOUTS timeouts = { MAXDWORD, MAXDWORD, 1, 0, 0 };

	sprintf(deviceFileName, "\\\\.\\COM%d", comPortNumber);

	Trace("InitSys: '%s'\n", deviceFileName);

	currentBaudRateIndex = -1;

	ZeroWaitCount();

	hSerial = CreateFile(deviceFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, 
		NULL, 
		OPEN_EXISTING,
		NULL, 
		NULL);

	if(INVALID_HANDLE_VALUE == hSerial)
	{
		Trace("InitSys: CreateFile failed '%s'\n", deviceFileName);
		return NULL;
	}

	// Set up a big RX buffer
	if(!SetupComm(hSerial, 1000, 1000))
	{
		// This odd circumstance seems to occur if the port has been assigned to a printer
		Trace("InitSys: Setupcom failed '%s'\n", deviceFileName);
		CloseHandle(hSerial);
		hSerial = INVALID_HANDLE_VALUE;
		return NULL;
	}

	// Configure the serial port
	GetCommState(hSerial, &devCon);

	devCon.BaudRate = 9600;
	devCon.ByteSize = 8;
	devCon.Parity = NOPARITY;
	devCon.StopBits = ONESTOPBIT;
	devCon.fParity = FALSE;

	devCon.fDsrSensitivity = FALSE;
	devCon.fDtrControl = FALSE;
	devCon.fRtsControl = RTS_CONTROL_DISABLE;
	devCon.fOutxCtsFlow = FALSE;
	devCon.fOutxDsrFlow = FALSE;
	devCon.fInX = FALSE;
	devCon.fOutX = FALSE;
	SetCommState(hSerial, &devCon);

	SetCommTimeouts(hSerial, &timeouts);

	EscapeCommFunction(hSerial, SETDTR);
	EscapeCommFunction(hSerial, SETRTS);



#if 0
    int i8259_val;                  /* storing I8259 value */
    int i8259m_val;                 /* storing I8259 mask register value */

    inp( IOBase );                 /* base com port: clear any characters */
    outp( IER, 0 );                /* ier <= 0 : no ints for now */
    inp( MSR );                    /* MSR : reset MSR now */

    InitInts();

    outp( LCR, LCR_SETUP );                /* set com line characteristics */
    outp( MCR, MCR_SETUP );                /* set MCR value */
    outp( IER, 0x05 );                     /* set ier, allow lsr & rx ints */

    _disable();
    i8259m_val = inp( I8259M );            /* set 8259 mask register    */
    I8259mBit = i8259m_val & ( 1 << IntVector );
    i8259m_val &= ~( 1 << IntVector );    /*    to allow com interrupts */
    outp( I8259M, i8259m_val );
    _enable();
    i8259_val = IntVector;                 /* set 8259 priority */
    --i8259_val;
    i8259_val |= 0xc0;
    outp( I8259, i8259_val );
    CurrentBaud = -1;
    Modem = 0;
#endif
    return( NULL );
}


void ResetSys( void )
{
	if(hSerial != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSerial);
		hSerial = INVALID_HANDLE_VALUE;
	}
#if 0
    int i8259m_val;         /* storing I8259 mask register value */

    outp( I8259, 0xc7 );              /* reset normal 8259 priority */

    i8259m_val = inp( I8259M );               /* set 8259 mask register  */
    i8259m_val |= I8259mBit;                  /*  to previous status for */
    outp( I8259M, i8259m_val );               /*  com:-level interrupts  */

    outp( IER, 0 );                   /* ier <= 0 : all ints off */
    outp( MCR, 0 );                   /* MCR : set OUT2 & DTR off */

    FiniInts();
#endif
}

bool Terminate( void )
{
	ResetSys();
	return( TRUE );
}

static void FlushWriteCache()
{
	if(writeCacheLevel > 0)
	{
		DWORD nBytesWritten;
		WriteFile(hSerial, writeCache, writeCacheLevel, &nBytesWritten, NULL);
		writeCacheLevel = 0;
	}
}

static void InsertWriteCacheByte(BYTE newByte)
{
	if(writeCacheLevel >= sizeof(writeCache))
	{
		FlushWriteCache();
	}
	writeCache[writeCacheLevel++] = newByte;
}

void SendByte( int value )
{
	InsertWriteCacheByte(value);
	if(!bBlockWriteMode)
	{
		FlushWriteCache();
	}

	//	Trace("Ser: Wrote 0x%x\n", outByte);

#if 0
    do {
        ;
    } while( (inp( LSR ) & LSR_THRE) == 0 );
    if( Modem ) {
        /* talking over a modem - check the data set ready line */
        do {
            ;
        } while( (inp( MSR ) & MSR_CTS) == 0 );
    }
    outp( IOBase, value );
#endif
}

void StartBlockTrans()
{
	bBlockWriteMode = TRUE;
}

void StopBlockTrans()
{
	bBlockWriteMode = FALSE;
	FlushWriteCache();

#if 0
    if( Modem ) {
        /* talking over a modem - check the data set ready line */
        do {
            ;
        } while( (inp( MSR ) & MSR_CTS) == 0 );
    }
#endif
}


int GetByte()
{
	if(readCacheIndex < readCacheLevel)
	{
		return readCache[readCacheIndex++];
	}
	else
	{
		// Cache is empty
		readCacheIndex = readCacheLevel = 0;
		if(ReadFile(hSerial, readCache, sizeof(readCache), &readCacheLevel, NULL))
		{
			if(readCacheLevel > 0)
			{
				return readCache[readCacheIndex++];
			}
		}
	}
	return -1;
}


void ClearCom()
{
//    ClearBuffer();
	writeCacheLevel = 0;
}


void SendABreak( void )
{
	EscapeCommFunction(hSerial, SETBREAK);
	Sleep(BREAK_TIME * 55);
	EscapeCommFunction(hSerial, CLRBREAK);

#if 0
    int lcr_value;             /* storing line control register value */

    lcr_value = inp( LCR );                /* LCR contents */
    outp( LCR, lcr_value | 0x40 );         /* set break bit on */
    SendByte( 0 );                         /* interrupt other side */
    Wait( BREAK_TIME );                    /* hold it there */
    lcr_value &= (~0x40);                  /* assure break bit is off */
    outp( LCR, lcr_value );                /* restore lcr content */
#endif
}

bool TestForBreak( void )
{
	DWORD errors;
	COMSTAT comStat;
	if(ClearCommError(hSerial, &errors, &comStat))
	{
		if(errors & CE_BREAK)
		{
			return TRUE;
		}
	}
	return( FALSE );
#if 0
    _disable();
    if( BreakFlag || ( inp(LSR) & LSR_BRK ) ) {
        BreakFlag = 0;
        _enable();
        return( TRUE );
    }
    _enable();
#endif
}


int Divisor[] = { 1, 2, 3, 6, 12, 24, 48, 96, 0 };

bool Baud( int index )
{
	DCB devCon;

	ErrorFlag = 0;
	BreakFlag = 0;
	if( index == MIN_BAUD ) 
	{
//		Modem = 1;
		Trace("Ser: Modem flag set\n");
		return( TRUE );
	}
//	Modem = 0;

	if(index == currentBaudRateIndex) return( TRUE );

	GetCommState(hSerial, &devCon);
	devCon.BaudRate = 115200 / Divisor[index];
	Trace("Ser: Baud set: %d\n", devCon.BaudRate);
	SetCommState(hSerial, &devCon);

	currentBaudRateIndex = index;

	return( TRUE );

#if 0
    int lcr_value;

    ErrorFlag = 0;
    BreakFlag = 0;
    if( index == MIN_BAUD ) {
        Modem = 1;
        return( TRUE );
    }
    Modem = 0;
    if( index == CurrentBaud ) return( TRUE );

    /* don't change baud rate while a character is still being sent */
    do {} while( (inp( LSR ) & LSR_TSRE) == 0 );
    lcr_value = inp( LCR );              /* get LCR value */
    _disable();                          /* disable interrupt */
    outp( LCR, lcr_value | LCR_DLAB );   /* set Divisor Latch Access Bit(DLAB)*/
                                         /*  to enable setting of baud rate */
    outp( IOBase, Divisor[index] );      /* LSB portion of new divisor */
    outp( IOBase+1, 0 );                 /* MSB portion of new divisor */
    lcr_value = inp( LCR );
    outp( LCR, lcr_value & ~LCR_DLAB );  /* set off DLAB bit of LCR */
    _enable();                           /* re-enable interrupts */
    CurrentBaud = index;
#endif
}

char *ParsePortSpec( char * *spec )
{
    char    *parm;
    int     port;

    parm = (spec == NULL) ? "" : *spec;

    switch( *parm ) {
    case '1':
        ++parm;
        /* fall through */
    case '.':
    case '\0':
        port = 0;
		comPortNumber = 1;
//        IntVector = 4;
//        IOBase = 0x3f8;
        break;
    case '2':
        ++parm;
        port = 1;
		comPortNumber = 2;
//        IntVector = 3;
//        IOBase = 0x2f8;
        break;
    default:
        return( TRP_ERR_invalid_serial_port_number );
    }
    if( *parm != '\0' && *parm != '.' ) return( TRP_ERR_invalid_serial_port_number );
//    if( port >= Equipment().num_rs232s ) return( TRP_ERR_serial_port_does_not_exist );
    if( spec != NULL ) *spec = parm;
    return( NULL );
}


void DonePort(void)
{
}


bool CheckPendingError()
{
    int old_error;

//    _disable();
    old_error = ErrorFlag;
    ErrorFlag = 0;
//    _enable();
    return( old_error != 0 );
}


void ClearLastChar()
{
	// Wait for the output buffer to empty
	FlushWriteCache();
	for(;;)
	{
		DWORD errors;
		COMSTAT comStat;
		if(!ClearCommError(hSerial, &errors, &comStat))
		{
			break;
		}
		if(comStat.cbOutQue == 0)
		{
			break;
		}
		Sleep(0);
	}
}

