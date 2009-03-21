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
* Description:  CauseWay trap file main loop.
*
****************************************************************************/


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "watcom.h"
#include "trpimp.h"
#include "dpmi.h"

extern void ReadConfig( void );
#pragma aux ReadConfig "*";
extern void Int01Handler( void );
#pragma aux Int01Handler "*";
extern void Int03Handler( void );
#pragma aux Int03Handler "*";
extern void Int21Handler( void );
#pragma aux Int21Handler "*";
extern void Int31Intercept( void );
#pragma aux Int31Intercept "*";
extern void EInt01Handler( void );
#pragma aux EInt01Handler "*";
extern void EInt03Handler( void );
#pragma aux EInt03Handler "*";
extern void Exc00Handler( void );
#pragma aux Exc00Handler "*";
extern void Exc12Handler( void );
#pragma aux Exc12Handler "*";
extern void Exc13Handler( void );
#pragma aux Exc13Handler "*";
extern void Exc14Handler( void );
#pragma aux Exc14Handler "*";
extern void BreakChecker( void );
#pragma aux BreakChecker "*";

extern unsigned_16  _psp;
extern unsigned_16  PSPSegment;
#pragma aux PSPSegment "*";
extern unsigned_16  cwMajorVersion;
#pragma aux cwMajorVersion "*";

extern void __far *OldInt01;
#pragma aux OldInt01 "*";
extern void __far *OldInt03;
#pragma aux OldInt03 "*";
extern void __far *OldInt09;
#pragma aux OldInt09 "*";
extern void __far *OldInt21h;
#pragma aux OldInt21h "*";
extern void __far *OldInt31;
#pragma aux OldInt31 "*";
extern void __far *OldEInt01;
#pragma aux OldEInt01 "*";
extern void __far *OldEInt03;
#pragma aux OldEInt03 "*";
extern void __far *OldExc00;
#pragma aux OldExc00 "*";
extern void __far *OldExc12;
#pragma aux OldExc12 "*";
extern void __far *OldExc13;
#pragma aux OldExc13 "*";
extern void __far *OldExc14;
#pragma aux OldExc14 "*";

int CW_FarCallReal( rm_call_struct __far *call_st );
#pragma aux CW_FarCallReal =    \
    "push   es"                 \
    "mov    es,dx"              \
    "mov    ax,0ff02h"          \
    "int    0x31"               \
    "pop    es"                 \
    "sbb    eax,eax"            \
    parm [dx edi] value [eax];

void dos_print( char *s );
#pragma aux dos_print =     \
    "mov    ah,9h"          \
    "int    21h"            \
    parm [edx];

void dos_delete( char *filename );
#pragma aux dos_delete =    \
    "mov    ah,41h"         \
    "int    21h"            \
    parm [edx];

// Needed because assembly stuff expects ES==DS
void fix_es( void );
#pragma aux fix_es =    \
    "push   ds"         \
    "pop    es";

// fuck stupid assembly interfaces
void *get_edi( void );
#pragma aux get_edi =   \
    "mov    eax,edi";

struct trap_info {
    unsigned_32     signature;
    void            *req_addr;
    unsigned_32     real_call;
};

struct req_header {
    char            *buffer;
    unsigned_32     length;
};

/* C trap request handlers */
extern unsigned (* const CoreRequests[])( void );

/* Assembly trap request handlers */
typedef unsigned_32 (req_fn)( void *req_in, void *req_out, unsigned_32 req_len );
#pragma aux req_fn parm [esi] [edi] [ecx] value [ecx] modify [eax];

typedef req_fn *preq_fn;

extern preq_fn   ReqTable[ 128 ];
#pragma aux ReqTable "*";


static struct req_header    *ReqAddress;
static char                 ReqBuffer[ 1024 ];
static unsigned_32          ReqLength;
static rm_call_struct       RealModeRegs;

extern unsigned_32  DebugLevel;
#pragma aux DebugLevel "*";

char    banner[] =
    "-------------------------------------------------------------------------------\r\n"
    "CauseWay Trap helper v4.00.  Helper code is public domain.  No rights reserved.\r\n"
    "-------------------------------------------------------------------------------\r\n"
    "\r\n$";

#if 0
#define DBG( ... )   dos_printf( __VA_ARGS__ )
#else
#define DBG( ... )
#endif

void dos_printf( const char *format, ... )
{
    static char     dbg_buf[ 256 ];
    va_list         args;

    va_start( args, format );
    vsnprintf( dbg_buf, sizeof( dbg_buf ), format, args );
    // Convert to DOS string
    dbg_buf[ strlen( dbg_buf ) ] = '\$';
    dos_print( dbg_buf );
    va_end( args );
}

// Print nicely formatted contents of a buffer. Useful for dumping
// debug requests and responses.
void HexDumpBuffer( const char *buf, int length )
{
    int     i, j;
    int     line_len;

#define DUMP_LINE_LEN   16

    i = 0;
    while( length > 0 ) {
        line_len = length < DUMP_LINE_LEN ? length : DUMP_LINE_LEN;
        for( j = 0; j < line_len; ++j )
            dos_printf( "%02x ", buf[i + j] );
        for( ; j < DUMP_LINE_LEN; ++j )
            dos_printf( "   " );
        for( j = 0; j < line_len; ++j )
            dos_printf( "%c", buf[i + j] );
        dos_printf( "\r\n" );
        length -= DUMP_LINE_LEN;
        i += DUMP_LINE_LEN;
    }
}

// Takes care of low-level link to the trap file and dishes out control
// to appropriate functions.
void Dispatcher( void )
{
    int         len;
    unsigned_8  request;
    char        *out_ptr;

    DBG( "Entered trap dispatcher\r\n" );
    for( ;; ) {
        // Transfer back to real mode
        DBG( "Calling real mode..." );
        CW_FarCallReal( &RealModeRegs );
        DBG( "...back from RM\r\n" );

        len = ReqAddress->length;
        if( len > sizeof( ReqBuffer ) ) {
            dos_printf( "Request buffer too long (%d bytes)!\r\n", len );
            len = sizeof( ReqBuffer );
        }
        memcpy( ReqBuffer, ReqAddress->buffer, len );
        ReqLength = len;

        if( DebugLevel )
            HexDumpBuffer( ReqBuffer, len );

        // Check if this is a get lost message
        request = ReqBuffer[0];
        if( request == 0 )
            break;

        // Check if any results are needed and skip processing on
        // this pass if they are
        if( !(request & 0x80) ) {
            // Pass control back to real mode ready for result transfer
            DBG( "Calling real mode with results..." );
            CW_FarCallReal( &RealModeRegs );
            DBG( "...back from RM\r\n" );
        }

        // Go through all commands in the buffer
        out_ptr = ReqAddress->buffer;
        do {
            int     new_len;

            request = ReqBuffer[0];
            request &= 0x7f;

            DBG( "Processing request %d\r\n", request );
            if( CoreRequests[request] != NULL ) {
                unsigned    ret_len;
                mx_entry    in_entry;
                mx_entry    out_entry;

                in_entry.ptr = (void *)&ReqBuffer;
                in_entry.len = len;
                In_Mx_Num = 1;
                In_Mx_Ptr = &in_entry;
                out_entry.ptr = out_ptr;
                out_entry.len = 0;
                Out_Mx_Num = 1;
                Out_Mx_Ptr = &out_entry;

                DBG( "Calling C trap request handler..." );
                ret_len = CoreRequests[request]();
                DBG( "...back from request\r\n" );
                out_ptr += ret_len;
                new_len = 0;

            } else if( ReqTable[request] == NULL ) {
                dos_printf( "Unimplemented request! (%d)\r\n", request );
                // Display request buffer if we don't understand it
                HexDumpBuffer( ReqBuffer, len );
            } else {
                DBG( "Calling assembly trap request handler..." );
                // TODO: fix_es() and get_edi() need to go once all requests
                // are handled in C
                fix_es();
                new_len = ReqTable[request]( &ReqBuffer, out_ptr, len );
                out_ptr = get_edi();
                DBG( "...back from request\r\n" );
                DBG( "out_ptr: %p\r\n", out_ptr );
            }
            DBG( "new_len = %d\r\n", new_len );

            if( new_len < 0 ) {
                // If this happened, we're pretty much fucked
                dos_printf( "cwhelp: underflow error in dispatcher!\r\n" );
                dos_printf( "request = %d, len = %d, new_len = %d\r\n",
                            request, len, new_len );
                HexDumpBuffer( ReqBuffer, 16 );
                break;
            }
            len = new_len;
        } while( len );

        ReqAddress->length = out_ptr - ReqAddress->buffer;

        if( DebugLevel )
            HexDumpBuffer( ReqAddress->buffer, ReqAddress->length );

    }
    DBG( "Exiting trap dispatcher\r\n" );
}


int main( int argc, char **argv )
{
    void                **trap_ptr;
    struct trap_info    *trap_header;
    unsigned_16 __far   *cw_version;

    PSPSegment = _psp;
    dos_print( banner );

    // Look for trap file header off interrupt vector 6
    // (refer to LINK_SIGNATURE in dosxlink.c)
    trap_ptr = (void *)(6 * sizeof( unsigned_32 ));
    trap_header = *trap_ptr;
    if( trap_header->signature != 0xdeb0deb0 ) {
        dos_printf( "CauseWay trap file signature not found!\r\n" );
        return( 1 );
    }

    // Get the transfer buffer address and real mode callback address
    ReqAddress = trap_header->req_addr;
    RealModeRegs.ip = trap_header->real_call & 0xFFFF;
    RealModeRegs.cs = trap_header->real_call >> 16;

    // Process the configuration file
    ReadConfig();

    // Delete debug log if in debug mode
    if( DebugLevel )
        dos_delete( "cwhelp.log" );

    // Install debug interrupt (single step) handler
    OldInt01 = DPMIGetPMInterruptVector( 1 );
    DPMISetPMInterruptVector( 1, Int01Handler );

    // Install debug breakpoint handler
    OldInt03 = DPMIGetPMInterruptVector( 3 );
    DPMISetPMInterruptVector( 3, Int03Handler );

    // Install debug exception handler
    OldEInt01 = DPMIGetPMExceptionVector( 1 );
    DPMISetPMExceptionVector( 1, EInt01Handler );

    // Install debug breakpoint exception handler
    OldEInt03 = DPMIGetPMExceptionVector( 3 );
    DPMISetPMExceptionVector( 3, EInt03Handler );

    // Install exception 0 (divide by zero) handler
    OldExc00 = DPMIGetPMExceptionVector( 0 );
    DPMISetPMExceptionVector( 0, Exc00Handler );

    // Install exception 12 (stack fault) handler
    OldExc12 = DPMIGetPMExceptionVector( 12 );
    DPMISetPMExceptionVector( 12, Exc12Handler );

    // Install exception 13 (general protection fault) handler
    OldExc13 = DPMIGetPMExceptionVector( 13 );
    DPMISetPMExceptionVector( 13, Exc13Handler );

    // Install exception 14 (page fault) handler
    OldExc14 = DPMIGetPMExceptionVector( 14 );
    DPMISetPMExceptionVector( 14, Exc14Handler );

    // Install int 21h handler to catch termination
    OldInt21h = DPMIGetPMInterruptVector( 0x21 );
    DPMISetPMInterruptVector( 0x21, Int21Handler );

    // Install keyboard interrupt handler so we can break in
    OldInt09 = DPMIGetPMInterruptVector( 9 );
    DPMISetPMInterruptVector( 9, BreakChecker );

    // Install DPMI API handler
    OldInt31 = DPMIGetPMInterruptVector( 0x31 );
    // Save off the old CauseWay API version
    cw_version = OldInt31;
    --cw_version;
    dos_printf( "CauseWay API version = %#04x\r\n", *cw_version );
    DPMISetPMInterruptVector( 0x31, Int31Intercept );

    Dispatcher();

    // Restore old interrupt/exception handlers
    DPMISetPMInterruptVector( 9, OldInt09 );
    DPMISetPMInterruptVector( 0x21, OldInt21h );
    DPMISetPMInterruptVector( 1, OldInt01 );
    DPMISetPMInterruptVector( 3, OldInt03 );
    DPMISetPMExceptionVector( 1, OldEInt01 );
    DPMISetPMExceptionVector( 3, OldEInt03 );
    DPMISetPMExceptionVector( 0, OldExc00 );
    DPMISetPMExceptionVector( 12, OldExc12 );
    DPMISetPMExceptionVector( 13, OldExc13 );
    DPMISetPMExceptionVector( 14, OldExc14 );

    return( 0 );
}
