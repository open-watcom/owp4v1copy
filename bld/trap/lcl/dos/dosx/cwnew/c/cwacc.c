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
* Description:  CauseWay trap file protected mode request handling.
*
****************************************************************************/


//#define DEBUG_TRAP

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "trpimp.h"
#include "trperr.h"
#include "madregs.h"
#include "dpmi.h"
#include "x86cpu.h"
#include "misc7386.h"
#include "ioports.h"
#include "dosredir.h"
#define ERR_CODES
#include "dosmsgs.h"
#undef  ERR_CODES

#define MAX_WATCHES     256

#define ST_EXECUTING    0x01
#define ST_BREAK        0x02
#define ST_TRACE        0x04
#define ST_WATCH        0x08
#define ST_KEYBREAK     0x10
#define ST_TERMINATE    0x20

#ifdef DEBUG_TRAP
extern void dos_printf( const char *format, ... );

#define _DBG( ... )   dos_printf( __VA_ARGS__ )
#define _DBG1( ... )   dos_printf( __VA_ARGS__ )
#define _DBG2( ... )   dos_printf( __VA_ARGS__ )
#else
#define _DBG( ... )
#define _DBG1( ... )
#define _DBG2( ... )
#endif

#define GetModuleHandle GetSelBase
#define GetLinAddr(x)   GetSelBase(x.segment)+x.offset

#define IDX_TO_SEL(x) ((epsp->SegBase+segidx*8)|3)

typedef unsigned_16 selector;
typedef unsigned_16 segment;

#include "pushpck1.h"
typedef struct seg_t {
    unsigned_32 base;
    unsigned    limit   :20;
    unsigned    granul  :1;
    unsigned    class   :4;
    unsigned    is16bit :1;
    unsigned    is32bit :1;
    unsigned    isFlat  :1;
    unsigned    reserved:2;
    unsigned    nospeed :1;
    unsigned    compres :1;
} seg_t;

typedef struct epsp_t {
    char            PSP[256];
    selector        parent;
    selector        next;
    void            *resource;
    void            *mcbHead;
    void            *mcbMaxAlloc;
    char            far *DTA;
    selector        TransProt;
    segment         TransReal;
    unsigned_32     TransSize;
    void            far *SSESP;
    void            *INTMem;
    selector        DPMIMem;
    unsigned_32     MemBase;
    unsigned_32     MemSize;
    selector        SegBase;
    unsigned_16     SegSize;
    unsigned_32     NearBase;
    segment         RealENV;
    void            *NextPSP;
    void            *LastPSP;
    void            *Exports;
    void            *Imports;
    unsigned_32     Links;
    void            *ExecCount;
    void            far *EntryCSEIP;
    selector        PSPSel;
    char            FileName[256];
} epsp_t;
#include "poppck.h"

typedef struct hbrk_t {
    unsigned_32     address;
    unsigned_16     handle;
    char            type;
    unsigned_8      size;
    unsigned        inuse     :1;
    unsigned        installed :1;
} hbrk_t;

typedef struct watch_t {
    unsigned_32     address;
    unsigned_32     check;
    unsigned_8      length;
    unsigned        inuse     :1;
} watch_t;

void dos_print( char *s );
#pragma aux dos_print = \
    "mov  ah,9" \
    "int  21h" \
    parm [edx];

extern unsigned_32 GetSelBase( unsigned_16 );
#pragma aux GetSelBase = \
    "mov  ax,0FF08h" /* GetSelDet32 */ \
    "int  31h" \
    parm [bx] modify [eax ebx ecx] value [edx];

extern int RelSel( unsigned_16 );
#pragma aux RelSel = \
    "mov  ax,0FF04h" /* RelSel */ \
    "int  31h" \
    "sbb  eax,eax" \
    parm [bx] value [eax];

extern int IsSel32bit( unsigned_16 );
#pragma aux IsSel32bit = \
    "movzx eax,ax" \
    "lar   eax,eax" \
    "and   eax,400000h" \
    parm [ax] value [eax];

extern unsigned     MemoryCheck( unsigned_32, unsigned, unsigned );
extern unsigned     MemoryRead( unsigned_32, unsigned, void *, unsigned );
extern unsigned     MemoryWrite( unsigned_32, unsigned, void *, unsigned );
extern unsigned     Execute( bool );
extern int          DebugLoad( char *prog_name, char *cmdl );
extern unsigned     ExceptionText( unsigned, char * );
extern int          GrabVectors( void );
extern void         ReleaseVectors( void );

extern unsigned_8       Exception;
extern int              XVersion;
extern trap_cpu_regs    DebugRegs;
extern unsigned_16      DebugPSP;
extern seg_t            *DebugSegs;

char                UtilBuff[BUFF_SIZE];
int                 WatchCount = 0;
bool                FakeBreak = FALSE;

static unsigned_8   RealNPXType;
static hbrk_t       HBRKTable[4];
static watch_t      WatchPoints[MAX_WATCHES];

static char *DosErrMsgs[] = {
#include "dosmsgs.h"
};
#define MAX_ERR_CODE (sizeof( DosErrMsgs ) / sizeof( char * ) - 1)

#ifdef DEBUG_TRAP
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
#endif

static void HBRKInit( void )
/**************************/
{
    int     i;

    for( i = 0; i < 4; ++i ) {
        HBRKTable[i].inuse = FALSE;
    }
}

void SetHBRK( void )
/******************/
{
    int     i;

    // Install hardware break points.
    for( i = 0; i < 4; ++i ) {
        if( HBRKTable[i].inuse ) {
            dpmi_watch_handle   wh;

            wh = _DPMISetWatch( HBRKTable[i].address, HBRKTable[i].size, HBRKTable[i].type );
            if( wh >= 0 ) {
                HBRKTable[i].installed = TRUE;
                HBRKTable[i].handle = wh;
                _DPMIResetWatch( wh );
            }
        }
    }
}

void ResetHBRK( void )
/********************/
{
    int     i;

    // Uninstall hardware break points.
    for( i = 0; i < 4; ++i ) {
        if( HBRKTable[i].inuse && HBRKTable[i].installed ) {
            _DPMIClearWatch( HBRKTable[i].handle );
            HBRKTable[i].installed = FALSE;
        }
    }
}

int IsHardBreak( void )
/*********************/
{
    int     i;

    for( i = 0; i < 4; ++i ) {
        if( HBRKTable[i].inuse && HBRKTable[i].installed ) {
            if( _DPMITestWatch( HBRKTable[i].handle ) ) {
                return( TRUE );
            }
        }
    }
    return( FALSE );
}

int CheckWatchPoints( void )
/**************************/
{
    int         i;
    int         j;
    unsigned_8  *p;
    unsigned_32 sum;

    for( i = 0; i < MAX_WATCHES; ++i ) {
        if( WatchPoints[i].inuse ) {
            p = (unsigned_8 *)WatchPoints[i].address;
            sum = 0;
            for( j = 0; j < WatchPoints[i].length; ++j ) {
                sum += *(p++);
            }
            if( sum != WatchPoints[i].check ) {
                return( TRUE );
            }
        }
    }
    return( FALSE );
}

static int MapStateToCond( unsigned state )
/*****************************************/
{
    int     rc;

    if( state & ST_TERMINATE ) {
        _DBG( "Condition: TERMINATE\r\n" );
        rc = COND_TERMINATE;
    } else if( state & ST_KEYBREAK ) {
        _DBG( "Condition: KEYBREAK\r\n" );
        rc = COND_USER;
    } else if( state & ST_WATCH ) {
        _DBG( "Condition: WATCH\r\n" );
        rc = COND_WATCH;
    } else if( state & ST_BREAK ) {
        _DBG( "Condition: BREAK\r\n" );
        rc = COND_BREAK;
    } else if( state & ST_TRACE ) {
        _DBG( "Condition: TRACE\r\n" );
        rc = COND_TRACE;
    } else {
        _DBG( "Condition: EXCEPTION\r\n" );
        rc = COND_EXCEPTION;
    }
    return( rc );
}

unsigned ReqGet_sys_config( void )
/********************************/
{
    get_sys_config_ret  *ret;

    _DBG( "AccGetConfig\r\n" );
    ret = GetOutPtr( 0 );
    ret->sys.os = OS_RATIONAL;      // Pretend we're DOS/4G
    ret->sys.osmajor = _osmajor;
    ret->sys.osminor = _osminor;
    ret->sys.cpu = X86CPUType();
    ret->sys.huge_shift = 12;
    ret->sys.fpu = NPXType(); //RealNPXType;
    ret->sys.mad = MAD_X86;
    _DBG( "os = %d, cpu=%d, fpu=%d, osmajor=%d, osminor=%d\r\n",
        ret->sys.os, ret->sys.cpu, ret->sys.fpu, ret->sys.osmajor, ret->sys.osminor );
    return( sizeof( *ret ) );
}

unsigned ReqMap_addr( void )
/**************************/
{
    map_addr_req    *acc;
    map_addr_ret    *ret;
    epsp_t          *epsp;
    unsigned_32     limit;
    signed_16       segidx;

    _DBG1( "AccMapAddr\r\n" );
    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    ret->out_addr.offset = 0;
    ret->out_addr.segment = 0;
    ret->lo_bound = 0;
    ret->hi_bound = 0;
    epsp = (epsp_t *)GetModuleHandle( DebugPSP );
    if( (epsp_t *)acc->handle == epsp ) {
        // get segment index
        segidx = acc->in_addr.segment;
        if( segidx < 0 )
            segidx = -segidx;
        --segidx;
        // convert segment index to selector
        ret->out_addr.segment = IDX_TO_SEL( segidx );
        // convert offset
        ret->out_addr.offset = acc->in_addr.offset + DebugSegs[segidx].base + epsp->MemBase;
        // horrible hackery to fix offset + code size passed for symbol offset in global vars
        if( acc->in_addr.segment == MAP_FLAT_DATA_SELECTOR ) {
            // 2nd segment(hopefully DGROUP) base offset round up to next 64K
            ret->out_addr.offset -= ( DebugSegs[1].base + 0xFFFF ) & 0xFFFF0000;
        }
        // Set the bounds.
        limit = DebugSegs[segidx].limit;
        if( DebugSegs[segidx].granul ) {
            limit <<= 12;
            limit |= 0xFFF;
        }
        if( limit != -1 && limit != 0 ) {
            --limit;
        }
        ret->lo_bound = 0;
        ret->hi_bound = limit;
    }
    return( sizeof( *ret ) );
}

//OBSOLETE - use ReqMachine_data
unsigned ReqAddr_info( void )
/***************************/
{
    addr_info_req       *acc;
    addr_info_ret       *ret;

    _DBG( "AccAddrInfo\r\n" );
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->is_32 = 0;
    if( IsSel32bit( acc->in_addr.segment ) ) {
        ret->is_32 = 1;
    }
    return( sizeof( *ret ) );
}

static unsigned ReadMemory( addr48_ptr *addr, void *data, unsigned len )
/**********************************************************************/
{
    return( MemoryRead( addr->offset, addr->segment, data, len ) );
}

static unsigned WriteMemory( addr48_ptr *addr, void *data, unsigned len )
/***********************************************************************/
{
    return( MemoryWrite( addr->offset, addr->segment, data, len ) );
}

unsigned ReqChecksum_mem( void )
/******************************/
{
    unsigned            len;
    int                 i;
    unsigned            read;
    checksum_mem_req    *acc;
    checksum_mem_ret    *ret;

    _DBG1(( "AccChkSum\n" ));

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    len = acc->len;
    ret->result = 0;
    while( len >= BUFF_SIZE ) {
        read = ReadMemory( &acc->in_addr, &UtilBuff, BUFF_SIZE );
        for( i = 0; i < read; ++i ) {
            ret->result += UtilBuff[ i ];
        }
        if( read != BUFF_SIZE )
            return( sizeof( *ret ) );
        len -= BUFF_SIZE;
        acc->in_addr.offset += BUFF_SIZE;
    }
    if( len != 0 ) {
        read = ReadMemory( &acc->in_addr, &UtilBuff, len );
        for( i = 0; i < read; ++i ) {
            ret->result += UtilBuff[ i ];
        }
    }
    return( sizeof( ret ) );
}

unsigned ReqRead_mem( void )
/**************************/
{
    read_mem_req        *acc;

    acc = GetInPtr( 0 );
    return( ReadMemory( &acc->mem_addr, GetOutPtr( 0 ), acc->len ) );
}

unsigned ReqWrite_mem( void )
/***************************/
{
    write_mem_req       *acc;
    write_mem_ret       *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->len = WriteMemory( &acc->mem_addr, GetInPtr( sizeof( *acc ) ), GetTotalSize() - sizeof( *acc ) );
    return( sizeof( *ret ) );
}

unsigned ReqRead_io( void )
/*************************/
{
    read_io_req         *acc;
    void                *data;

    acc = GetInPtr(0);
    data = GetOutPtr(0);
    if( acc->len == 1 ) {
        *(byte *)data = In_b( acc->IO_offset );
    } else if( acc->len == 2 ) {
        *(word *)data = In_w( acc->IO_offset );
    } else {
        *(dword *)data = In_d( acc->IO_offset );
    }
    return( acc->len );
}

unsigned ReqWrite_io( void )
/**************************/
{
    unsigned            len;
    write_io_req        *acc;
    write_io_ret        *ret;
    void                *data;

    acc = GetInPtr(0);
    data = GetInPtr( sizeof( *acc ) );
    len = GetTotalSize() - sizeof( *acc );
    ret = GetOutPtr(0);
    if( len == 1 ) {
        Out_b( acc->IO_offset, *(byte *)data );
    } else if( len == 2 ) {
        Out_w( acc->IO_offset, *(word *)data );
    } else {
        Out_d( acc->IO_offset, *(dword *)data );
    }
    ret->len = len;
    return( sizeof( *ret ) );
}

unsigned ReqRead_cpu( void )
/**************************/
{
    trap_cpu_regs       *regs;

    regs = GetOutPtr( 0 );
    *regs = DebugRegs;
    return( sizeof( *regs ) );
}

unsigned ReqRead_fpu( void )
/**************************/
{
    trap_fpu_regs       *regs;

    regs = GetOutPtr( 0 );
    Read387( regs );
    return( sizeof( *regs ) );
}

unsigned ReqWrite_cpu( void )
/***************************/
{
    trap_cpu_regs       *regs;

    regs = GetInPtr( sizeof( write_cpu_req ) );
    DebugRegs = *regs;
    return( 0 );
}

unsigned ReqWrite_fpu( void )
/***************************/
{
    trap_fpu_regs       *regs;

    regs = GetInPtr( sizeof( write_fpu_req ) );
    Write387( regs );
    return( 0 );
}

static unsigned ProgRun( bool step )
/**********************************/
{
    prog_go_ret *ret;

    _DBG1( "AccRunProg %X:%X\n", DebugRegs.CS, DebugRegs.EIP );
    ret = GetOutPtr( 0 );
    ret->conditions = MapStateToCond( Execute( step ) );
    ret->conditions |= COND_CONFIG;
    // Now setup return value to reflect why we stopped execution.
    ret->program_counter.offset = DebugRegs.EIP;
    ret->program_counter.segment = DebugRegs.CS;
    ret->stack_pointer.offset = DebugRegs.ESP;
    ret->stack_pointer.segment = DebugRegs.SS;
    return( sizeof( *ret ) );
}

unsigned ReqProg_go( void )
/*************************/
{
    return( ProgRun( FALSE ) );
}

unsigned ReqProg_step( void )
/***************************/
{
    return( ProgRun( TRUE ) );
}

unsigned ReqProg_load( void )
/***************************/
{
    char            *src;
    char            *dst;
    char            *name;
    char            ch;
    prog_load_ret   *ret;
    unsigned        len;
    int             rc;

    _DBG1( "AccLoadProg\r\n" );
    ret = GetOutPtr( 0 );
    src = GetInPtr( sizeof( prog_load_req ) );
    name = src;
    while( *src != '\0' )
        ++src;
    ++src;
    len = GetTotalSize() - ( src - name ) - sizeof( prog_load_req );
    dst = UtilBuff;
    for( ; len > 0; --len ) {
        ch = *src;
        if( ch == '\0' )
            ch = ' ';
        *dst = ch;
        ++src;
        ++dst;
    }
    if( dst > UtilBuff )
        --dst;
    *dst = '\0';
    
    rc = DebugLoad( name, UtilBuff );
    _DBG1( "back from debugload - %d\r\n", rc );
    ret->flags = LD_FLAG_IS_32 | LD_FLAG_IS_PROT | LD_FLAG_DISPLAY_DAMAGED;
    if( rc == 0 ) {
        ret->err = 0;
        ret->task_id = DebugPSP;
        ret->mod_handle = GetModuleHandle( DebugPSP );
    } else {
        ret->task_id = 0;
        ret->mod_handle = 0;
        if( rc == 1 ) {
            ret->err = ERR_ACCESS_DENIED;
        } else if( rc == 2 ) {
            ret->err = ERR_INVALID_FORMAT;
        } else if( rc == 3 ) {
            ret->err = ERR_INSUFFICIENT_MEMORY;
        } else {
            ret->err = rc;
        }
    }
    _DBG1( "done AccLoadProg\r\n" );
    return( sizeof( *ret ) );
}

unsigned ReqProg_kill( void )
/***************************/
{
    prog_kill_req       *acc;
    prog_kill_ret       *ret;

    _DBG( "AccKillProg\r\n" );
    acc = GetInPtr(0);
    ret = GetOutPtr( 0 );
    RedirectFini();
    ret->err = ( RelSel( acc->task_id ) ) ? ERR_INVALID_HANDLE : 0;
    return( sizeof( *ret ) );
}

unsigned ReqSet_watch( void )
/***************************/
{
    set_watch_req   *acc;
    set_watch_ret   *ret;
    int             i;
    int             j;
    unsigned_8      *p;
    unsigned_32     sum;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    if( acc->size == 1 || acc->size == 2 || acc->size == 4 ) {
        for( i = 0; i < 4; ++i ) {
            if( HBRKTable[i].inuse == FALSE ) {
                HBRKTable[i].inuse = TRUE;
                HBRKTable[i].installed = FALSE;
                HBRKTable[i].address = GetLinAddr( acc->watch_addr );
                HBRKTable[i].size = acc->size;
                HBRKTable[i].type = DPMI_WATCH_WRITE;
                ret->err = 0;
                ret->multiplier = 10 | USING_DEBUG_REG;
                return( sizeof( *ret ) );
            }
        }
    }
    if( WatchCount < MAX_WATCHES ) {
        for( i = 0; i < MAX_WATCHES; ++i ) {
            if( WatchPoints[i].inuse == FALSE ) {
                WatchPoints[i].inuse = TRUE;
                WatchPoints[i].address = GetLinAddr( acc->watch_addr );
                WatchPoints[i].length = acc->size;
                p = (unsigned_8 *)WatchPoints[i].address;
                sum = 0;
                for( j = 0; j < acc->size; ++j ) {
                    sum += *(p++);
                }
                WatchPoints[i].check = sum;
                ++WatchCount;
                ret->err = 0;
                ret->multiplier = 5000;
                return( sizeof( *ret ) );
            }
        }
    }
    ret->err = ERR_INVALID_DATA;
    ret->multiplier = 0;
    return( sizeof( *ret ) );
}

unsigned ReqClear_watch( void )
/*****************************/
{
    clear_watch_req     *acc;
    int                 i;
    unsigned_32         watch_addr;

    acc = GetInPtr( 0 );
    watch_addr = GetLinAddr( acc->watch_addr );
    for( i = 0; i < 4; i++ ) {
        if( HBRKTable[i].inuse ) {
            if( HBRKTable[i].address == watch_addr ) {
                if( HBRKTable[i].size == acc->size ) {
                    HBRKTable[i].inuse = FALSE;
                    return( 0 );
                }
            }
        }
    }
    if( WatchCount ) {
        for( i = 0; i < MAX_WATCHES; ++i ) {
            if( WatchPoints[i].inuse ) {
                if( WatchPoints[i].address == watch_addr ) {
                    if( WatchPoints[i].length == acc->size ) {
                        WatchPoints[i].inuse = FALSE;
                        --WatchCount;
                        break;
                    }
                }
            }
        }
    }
    return( 0 );
}

unsigned ReqSet_break( void )
/***************************/
{
    byte            ch;
    set_break_req   *acc;
    set_break_ret   *ret;

    _DBG( "AccSetBreak\r\n" );
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ReadMemory( &acc->break_addr, &ch, 1 );
    ret->old = ch;
    ch = 0xCC;
    WriteMemory( &acc->break_addr, &ch, 1 );
    return( sizeof( *ret ) );
}

unsigned ReqClear_break( void )
/*****************************/
{
    byte            ch;
    clear_break_req *acc;

    _DBG( "AccClearBreak\r\n" );
    acc = GetInPtr( 0 );
    ch = acc->old;
    WriteMemory( &acc->break_addr, &ch, 1 );
    return( 0 );
}

unsigned ReqGet_next_alias( void )
/********************************/
{
    get_next_alias_req  *acc;
    get_next_alias_ret  *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->seg = 0;
    ret->alias = 0;
    return( sizeof( *ret ) );
}

unsigned ReqGet_lib_name( void )
/******************************/
{
    char                *name;
    get_lib_name_ret    *ret;

    ret = GetOutPtr( 0 );
    ret->handle = 0;
    name = GetOutPtr( sizeof( *ret ) );
    *name = '\0';
    return( sizeof( *ret ) + 1 );
}

unsigned ReqGet_err_text( void )
/******************************/
{
    get_err_text_req    *acc;
    char                *err_txt;

    _DBG( "AccErrText\r\n" );
    acc = GetInPtr( 0 );
    err_txt = GetOutPtr( 0 );
    if( acc->err > MAX_ERR_CODE ) {
        _DBG( "After acc->error_code > MAX_ERR_CODE" );
        strcpy( (char *)err_txt, TRP_ERR_unknown_system_error );
        ultoa( acc->err, (char *)err_txt + strlen( err_txt ), 16 );
        _DBG( "After utoa()\r\n" );
    } else {
        strcpy( (char *)err_txt, DosErrMsgs[ acc->err ] );
        _DBG( "After strcpy\r\n" );
    }
    return( strlen( err_txt ) + 1 );
}

unsigned ReqGet_message_text( void )
/**********************************/
{
    get_message_text_ret    *ret;
    char                    *err_txt;

    ret = GetOutPtr( 0 );
    err_txt = GetOutPtr( sizeof(*ret) );
    ExceptionText( Exception, err_txt );
    ret->flags = MSG_NEWLINE | MSG_ERROR;
    return( sizeof( *ret ) + strlen( err_txt ) + 1 );
}

unsigned ReqRead_regs( void )
/***************************/
{
    mad_registers       *mr;

    mr = GetOutPtr( 0 );
    *(&mr->x86.cpu) = DebugRegs;
    Read387( &mr->x86.fpu );
    return( sizeof( mr->x86 ) );
}

unsigned ReqWrite_regs( void )
/****************************/
{
    mad_registers       *mr;

    mr = GetInPtr( sizeof( write_regs_req ) );
    DebugRegs = *(&mr->x86.cpu);
    Write387( &mr->x86.fpu );
    return( 0 );
}

unsigned ReqMachine_data( void )
/******************************/
{
    machine_data_req    *acc;
    machine_data_ret    *ret;
    union {
        unsigned_8      charact;
    } *data;
    unsigned            len;

    _DBG( "AccMachineData\r\n" );
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->cache_start = 0;
    ret->cache_end = 0;
    len = 0;
    if( acc->info_type == X86MD_ADDR_CHARACTERISTICS ) {
        ret->cache_end = ~(addr_off)0;
        data = GetOutPtr( sizeof( *ret ) );
        len = sizeof( data->charact );
        data->charact = 0;
        if( IsSel32bit( acc->addr.segment ) ) {
            data->charact = X86AC_BIG;
        }
    }
    _DBG( "address %x:%x is %s\r\n", acc->addr.segment, acc->addr.offset, *data ? "32-bit" : "16-bit" );
    return( sizeof( *ret ) + len );
}

trap_version TRAPENTRY TrapInit( char *parm, char *err, bool remote )
/*******************************************************************/
{
    trap_version    ver;

    err[0] = '\0'; /* all ok */
    ver.major = TRAP_MAJOR_VERSION;
    ver.minor = TRAP_MINOR_VERSION;
    ver.remote = FALSE;
    RedirectInit();
    RealNPXType = NPXType();
    HBRKInit();
    WatchCount = 0;
    FakeBreak = FALSE;
    XVersion = GrabVectors();
    _DBG( "CauseWay API version = %d.%02d\r\n", XVersion / 256, XVersion % 256 );
    return( ver );
}

void TRAPENTRY TrapFini( void )
/*****************************/
{
    // Restore old interrupt/exception handlers
    ReleaseVectors();
}
