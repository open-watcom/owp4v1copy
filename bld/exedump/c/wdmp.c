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
* Description:  Executable dumper mainline.
*
****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <unistd.h>
#include <process.h>

#include "wdglb.h"
#include "wdfunc.h"

char *Exts[] = { ".exe", ".dll", ".exp", ".nlm", ".qnx", ".elf", ".lib", "obj",
                NULL };
char  Fname[ _MAX_FNAME ];

static int find_file( char * file_path )
/**************************************/
{
    int     access_rc;
    char    path[ _MAX_PATH ];      /* this allows file_path to remain */
                                    /* unaltered if we don't find anything */
    char    drive[ _MAX_DRIVE ];
    char    dir[ _MAX_DIR ];
    char    given_ext[ _MAX_EXT ];
    char    **ext;

    _splitpath( file_path, drive, dir, Fname, given_ext );
    /* check if the given file name exists */
    access_rc = access( file_path, R_OK );
    if (access_rc == 0) {
        return( TRUE );
    } else {
        if( given_ext[0] == '\0' ) {
            /* if no extention was given try the ones in Exts */
            for (ext = Exts; *ext != NULL && access_rc != 0; ext++) {
                _makepath( path, drive, dir, Fname, *ext );
                access_rc = access( path, R_OK );
            }
        }
        if( access_rc == 0 ) {
            strcpy( file_path, path );
            return( TRUE );
        } else {
            return( FALSE );
        }
    }
}

/*
 * handles file to be dumped and .lst file if required
 */
static int open_files( void )
/***************************/
{
    int                 ret;
    char                listfile[ _MAX_PATH ];

    Handle = open( Name, O_RDONLY | O_BINARY, 0 );
    if( Handle != -1 ) {
        ret = setjmp( Se_env );
        if( ret == 0 ) {
            if( Options_dmp & LST_REQD ) {
                if( Lstf == NULL ) {
                    _makepath( listfile, NULL, NULL, Fname, ".lst" );
                    Lhandle = open( listfile, O_WRONLY | O_CREAT |
                            O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
                } else {
                    Lhandle = open( Lstf, O_WRONLY | O_CREAT | O_TRUNC,
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
                }
                if( Lhandle == NULL ) {
                    Options_dmp &= ~LST_REQD;
                    Wdputslc( "Unable to open list file\n" );
                }
                Wdputs( "Module:  " );
                Wdputs( Name );
                Wdputs( "\n\n" );
                dmp_exe();
                close( Lhandle );
            } else {
                dmp_exe();
            }
        }
        close( Handle );
        return( ret );
    }
    return( 1 );
}

/*
 * The Segmented .EXE File Header Dump Utility
 * for WLINK and WVIDEO
 */
int main( void )
/**************/
{
    int                 found_file;

    Line_count = 0;
    WSize = 0;
    Write_buff[WSize] = 0;
    Cmd = Wmalloc( BUFFERSIZE );
    getcmd( Cmd );
    Parse_option();
    if( !(Options_dmp & QUIET) ) {
        if( Options_dmp & LST_REQD ) {
            Options_dmp &= ~LST_REQD;
            wbanner();
            Options_dmp |= LST_REQD;
        } else {
            wbanner();
        }
    }
    if( *Name == '\0' ){
        Options_dmp |= PAGE_DMP;
        usage();
        return( 1 );
    }
    found_file = find_file( Name );
    if( found_file ) {
        return( open_files() );
    }
    Options_dmp &= ~LST_REQD;   // output usage to STDOUT
    Options_dmp |= PAGE_DMP;
    Wdputs( "Could not open '" );
    Wdputs( Name );
    Wdputs( "': " );
    Wdputslc( strerror( errno ) );
    Wdputslc( "\n\n" );
    usage();
    return( 1 );
}

/*
 * The copyright banner
 */
static void wbanner( void )
/*************************/
{
    Wdputs(
      "WATCOM Old and Segmented Image Dump Utitity Version 0.1\n"
      "Copyright by WATCOM Systems Inc. 1988.  All rights reserved.\n\n" );
}

/*
 * Usage
 */
static void usage( void )
/***********************/
{
    Wdputs( "Usage: wdmp [options] <pathname> [options]\n" );
    Wdputs( "  <pathname> is a DOS .EXE file," ); //or .COM file,"
    Wdputs( " a Windows application,\n" );
    Wdputs( "            or a new .EXE, .DLL, .EXP, .NLM, .QNX or .ELF file\n" ); //, .COM, or .SYS file\n"
    Wdputs( "  <pathname> has default extension of .EXE, .DLL, .EXP, .NLM, .QNX and .ELF\n" );
    Wdputs( "  options:\n" );
    Wdputs( "        -l[=<listfile>] information dumped in a listing file\n" );
    Wdputs( "        -p information dumped by page.  <enter>: next line,\n" );
    Wdputs( "            <esc> or 'q': quit, any other key: next page\n" );
    Wdputs( "        -q quiet dump - don't write banner\n" );
//            "        -m causes Microsoft debugging information to be dumped\n" );
    Wdputs( "        -s[=<segnum>] the segments'( resp. that segment's )\n" );
    Wdputs( "            info is dumped\n" );
    Wdputs( "        -r causes more resource information to be dumped\n" );
    Wdputs( "        -f causes fixup information to be dumped\n" );
    Wdputs( "        -a[=<segnum>] causes segment, resource and fixup info to be dumped\n" );
    Wdputs( "        -i dump export information for PE DLLs\n" );
    Wdputs( "        -b[=<hexoff>] causes binary dump [beginning at offset in hex]\n" );
    Wdputs( "        -d causes WATCOM debugging information to be dumped\n" );
    Wdputs( "           a : show addr infomation\n" );
    Wdputs( "           g : show global infomation\n" );
    Wdputs( "           l : show locals (only if m specified)\n" );
    Wdputs( "           m : show module informatiom\n" );
    Wdputs( "           n : show line numbers (only if m specified)\n" );
    Wdputs( "           t : show types (only if m specified)\n" );
    Wdputs( "           x : show all information\n" );
    Wdputs( "        -e causes executable infomation to be dumped as well\n" );
}

/*
 * parse the executable
 */
static void dmp_exe( void )
/*************************/
{
    bool            dos_dmp;
    unsigned_32     len;

    Form = FORM_NE;
    Data_count = 0;
    Sizeleft = 0;
    Num_buff = -1;
    if( Options_dmp & EXE_INFO ) {
        dos_dmp = Dmp_dos_head();
        if( dos_dmp == 2 ) {
            Options_dmp |= HAS_NEW_EXE;
            if( Dmp_os2_head() ) {
                /* done */
            } else if( Dmp_386_head() ) {
                /* done */
            } else if( Dmp_pe_head() ) {
                /* done */
            } else {
                Wdputslc( "Invalid OS/2, PE header\n" );
            }
        } else if( !dos_dmp ) {
            if( Dmp_dwarf() ) {
                /* done */
            } else if( Dmp_qnx_head() ) {
                /* done */
            } else if( Dmp_elf_head() ) {
                /* done */
                // elf debug info has already been handled
                Options_dmp &= ~DEBUG_INFO;
            } else if( Dmp_phar_head() ) {
                /* done */
            } else if( Dmp_nlm_head() ) {
                /* done */
            } else if( Dmp_ar_head() ) {
                /* done */
            } else if( Dmp_lib_head() ) {
                /* done */
            } else if( Dmp_coff_head() ) {
                /* done */
            } else {
                Wdputs( Name );
                Wdputslc( " is not a valid browser file.\n" );
                Wdputslc( "Invalid DOS, NLM, PHAR, QNX, ELF, AR or COFF file.\n" );
            }
        }
    }
    if( Options_dmp & DEBUG_INFO ) {
        Wdputslc( "\n" );
        if( !Dmp_mdbg_head() ) {
            Wdputslc( "Invalid symbol file!\n" );
        }
    }
    if( Options_dmp & IMPORT_LIB ) {
        if( !Dmp_pe_tab() ) {
            Wdputslc( "Invalid PE file\n" );
        }
    }
    if( Options_dmp & BINARY_DMP ) {
        len = lseek( Handle, -Hexoff, SEEK_END );
        Wdputs( "offset = " );
        Puthex( Hexoff, 8 );
        Wdputs( ",  length = " );
        Puthex( len, 8 );
        Wdputslc( "\n" );
        Dmp_seg_data( Hexoff, len );
    }
    if( WSize ) {
        write( Lhandle, Write_buff, WSize );
    }
}
