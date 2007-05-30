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
* Description:  DOS/16M executable dumping routines.
*
****************************************************************************/


#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

#include "wdglb.h"
#include "wdfunc.h"

static  char    *dos16m_exe_msg[] = {
    "2length of load module mod 200H                       = ",
    "2number of 200H pages in load module                  = ",
    "2reserved1                                            = ",
    "2reserved2                                            = ",
    "2minimum memory allocation in KB                      = ",
    "2maximum memory allocation in KB                      = ",
    "2initial value of SS                                  = ",
    "2initial value of SP                                  = ",
    "2selector for start of huge reloc list                = ",
    "2initial value of IP                                  = ",
    "2initial value of CS                                  = ",
    "2runtime GDT size in bytes less one                   = ",
    "2MAKEPM/GLU version * 100                             = ",
    "4offset of possible next spliced .exp                 = ",
    "4offset of debug info                                 = ",
    "2last selector value used                             = ",
    "2private memory allocation in KB                      = ",
    "2allocation increment (ExtReserve) in KB              = ",
    "4reserved3                                            = ",
    "2reserved4                                            = ",
    "2runtime options bit mask                             = ",
    "2selector of transparent stack                        = ",
    "2module flags                                         = ",
    "2program size in paras                                = ",
    "2file size of GDT in bytes less one                   = ",
    "2first user selector (0 -> default of 0x80)           = ",
    "1default mem strategy                                 = ",
    "1reserved5                                            = ",
    "2transfer buffer size in bytes (0 -> default of 8192) = ",
    NULL
};

#if 0
static  char    *dos16m_gdt_msg[] = {
    "2gdtlen      = ",
    "2gdtaddr     = ",
    "1gdtaddr_hi  = ",
    "1gdtaccess   = ",
    "2gdtreserved = ",
    NULL
};
#endif

/*
 * dump the DOS/16M GDT file data
 */
static void dmp_dos16m_gdt( dos16m_exe_header *d16m_head )
/********************************************************/
{
    int                             i, num_gdts;
    gdt_info                        gdt;
    unsigned_32                     selector;
    unsigned_32                     file_size;
    unsigned_32                     mem_size;
    unsigned_32                     segdata_offset;
    unsigned                        access;

    Wdputslc( "GDT selectors:\n" );
    Wlseek( New_exe_off + sizeof( dos16m_exe_header ) );
    selector = d16m_head->first_selector ? d16m_head->first_selector : 0;
    /* Calculate offset of the first segment image in file */
    segdata_offset = New_exe_off + sizeof( dos16m_exe_header ) + 1
                   + d16m_head->gdtimage_size - 16 * sizeof( gdt_info );

    /* Calculate the number of GDT entries in file. Guesswork! */
    if( d16m_head->last_sel_used && d16m_head->first_selector )
        num_gdts = (d16m_head->last_sel_used - d16m_head->first_selector) / sizeof( gdt_info ) + 1;
    else
        num_gdts = (d16m_head->gdtimage_size + 1) / sizeof( gdt_info ) - 17;
    Wdputslc( "                                 Size in    Size in\n" );
    Wdputslc( "Sel #    Access    File offset   File       Memory     DPL    Present    Flags\n" );
    Wdputslc( "-----    ------    -----------   -------    -------    ---    -------    -----\n" );
    for( i = 0; i < num_gdts; ++i ) {
        Wread( &gdt, sizeof( gdt_info ) );
//        Dump_header( &gdt, dos16m_gdt_msg );
        Puthex( selector, 4 );
        selector += 8;
        Wdputs( "      " );
        access = gdt.gdtaccess & 0x0F;
        if( access & 0x08 ) {   /* code seg */
            Wdputs( (access & 0x02) ? "ER" : "EO" );
        } else {                /* data seg */
            Wdputs( (access & 0x02) ? "RW" : "RO" );
        }
        Wdputs( "         " );
        Puthex( segdata_offset, 6 );
        Wdputs( "       " );
        file_size = gdt.gdtlen ? gdt.gdtlen + 1 : 0;
        Puthex( file_size, 5 );
        segdata_offset += file_size;
        Wdputs( "      " );
        mem_size = ((unsigned_32)gdt.gdtreserved & 0x1FFF) << 4;
        if( mem_size == 0 ) mem_size = file_size;
        Puthex( mem_size, 5 );
        Wdputs( "     " );
        Putdec( (gdt.gdtaccess >> 5) & 0x03 );
        Wdputs( "         " );
        Putdec( (gdt.gdtaccess & 0x80) != 0 );
        Wdputs( "       " );
        if( gdt.gdtreserved & 0x2000 ) Wdputs( "Z" );
        Wdputslc( "\n" );
    }
}

/*
 * dump the DOS/16M header information
 */
static void dmp_dos16m_head_info( dos16m_exe_header *d16m_head )
/**************************************************************/
{
    unsigned_32                     load_start;
    unsigned_32                     load_end;
    unsigned_32                     load_len;
    unsigned_16                     i;
    unsigned_16                     ver;
    struct { unsigned_16 offset;
             unsigned_16 segment; } reloc;

    Banner( "DOS/16M EXE Header" );
    Dump_header( &d16m_head->last_page_bytes, dos16m_exe_msg );
    load_len = (unsigned_32)d16m_head->pages_in_file * 0x200 + d16m_head->last_page_bytes;
    Wdputs( "\nload module length                                   = " );
    Puthex( load_len, 8 );
    Wdputslc( "H\n" );
    ver = d16m_head->MAKEPM_version;
    if( ver > 10 * 100 ) {
        Wdputs( "GLU version                                          = " );
        Putdec( ver / 1000 );
        Wdputs( "." );
        Putdecl( ver % 1000, 3 );
    } else {
        Wdputs( "MAKEPM version                                       = " );
        Putdec( ver / 100 );
        Wdputs( "." );
        Putdecl( ver % 100, 2 );
    }
    Wdputslc( "\n" );
    Wdputs( "original name: " );
    Wdputs( d16m_head->EXP_path );
    Wdputslc( "\n" );
    Wdputslc( "\n" );

    dmp_dos16m_gdt( d16m_head );

    // TODO: change fixup & segment data dumping to work with DOS/16M images
    if( Options_dmp & FIX_DMP ) {
        if( Dos_head.num_relocs != 0 ) {
            Wlseek( Dos_head.reloc_offset );
            Wdputslc( "segment:offset\n  " );
            for( i = 0; i < Dos_head.num_relocs; i++ ) {
                Wread( &reloc, sizeof( reloc ) );
                Puthex( reloc.segment, 4 );
                Wdputc( ':' );
                Puthex( reloc.offset, 4 );
                if( (i+1) % 6 ) {
                    Wdputs( "   " );
                } else {
                    Wdputslc( "\n  " );
                }
            }
            Wdputslc( "\n" );
            Wdputslc( "\n" );
        }
    }
    if( Options_dmp & DOS_SEG_DMP ) {
        load_start = Dos_head.hdr_size << 4;
        load_end = Load_len - load_start;
        Wdputslc( "load module =\n" );
        Dmp_seg_data( load_start, load_end );
        Wdputslc( "\n" );
    }
}

/*
 * Dump the DOS/16M Executable Header, if any.
 */
bool Dmp_d16m_head( void )
/************************/
{
    dos16m_exe_header   dos16m_head;

    Wlseek( New_exe_off );
    Wread( &dos16m_head, sizeof( dos16m_head ) );
    if( dos16m_head.signature == DOS16M_SIGNATURE ) {
        Banner( "DOS/16M EXE Header - BW" );
    } else {
        return( 0 );
    }
    Wdputs( "file offset = " );
    Puthex( New_exe_off, 8 );
    Wdputslc( "H\n" );
    Wdputslc( "\n" );
    dmp_dos16m_head_info( &dos16m_head );
    return( 1 );
}
