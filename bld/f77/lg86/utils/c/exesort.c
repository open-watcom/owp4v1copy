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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


//
// EXESORT      : sort the relocation items in an exe
//                also does some specific things that the overlaid version
//                of WATFOR77 requires.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <watcom.h>
#include <stddef.h>
#include "exedos.h"
typedef struct {
    unsigned_16 off;
    unsigned_16 seg;
} dos_addr;
#include "ovltab.h"

static  char            exeName[ _MAX_PATH ];
static  FILE            *exeFP;

static void ioError( const char *doing ) {

    printf( "Error %s %s: %s\n", doing, exeName, strerror( errno ) );
    exit( -1 );
}

static void safeRead( void *buf, size_t len ) {

    size_t      len_read;

    len_read = fread( buf, 1, len, exeFP );
    if( len_read != len ) {
        ioError( "reading" );
    }
}

static void safeWrite( const void *buf, size_t len ) {

    size_t      len_written;

    len_written = fwrite( buf, 1, len, exeFP );
    if( len_written != len ) {
        ioError( "writing" );
    }
}

static void safeSeek( long int offset ) {

    if( fseek( exeFP, offset, SEEK_SET ) != 0 ) {
        ioError( "seeking on" );
    }
}

#pragma aux usage aborts;
static void usage( void ) {
    printf( "usage: EXESORT exe_file [/o]\n" );
    printf( "EXESORT sorts the relocations in a DOS exe file.\n" );
    printf( "/o indicates the exe is overlaid and the overlay relocations\n" );
    printf( "   should also be sorted\n" );
    exit( EXIT_FAILURE );
}


static int compReloc( dos_addr *p1, dos_addr *p2 ) {

    unsigned_32 v1;
    unsigned_32 v2;

    v1 = (unsigned_32)p1->seg * 16 + (unsigned_32)p1->off;
    v2 = (unsigned_32)p2->seg * 16 + (unsigned_32)p2->off;
    if( v1 > v2 ) {
        return( 1 );
    } else if( v1 == v2 ) {
        return( 0 );
    }
    return( -1 );
}


static void sortRelocs( long int offset, unsigned_16 num_relocs ) {

    dos_addr *relocs;

    relocs = malloc( num_relocs * sizeof( dos_addr ) );
    if( relocs == NULL ) {
        printf( "not enough memory\n" );
        fclose( exeFP );
        exit( -1 );
    }
    safeSeek( offset );
    safeRead( relocs, num_relocs * sizeof( dos_addr ) );
    qsort( relocs, num_relocs, sizeof( dos_addr ), compReloc );
    safeSeek( offset );
    safeWrite( relocs, num_relocs * sizeof( dos_addr ) );
    free( relocs );
}


static void doWatfor( long int offset, unsigned_16 num_relocs,
    unsigned_16 ovlldr_seg ) {

    dos_addr    *root_relocs;
    dos_addr    *ovlldr_relocs;
    dos_addr    *root_high;
    dos_addr    *ovlldr_high;
    dos_addr    *p;
    unsigned_16 non_ovl_relocs;

    root_relocs = malloc( 2 * num_relocs * sizeof( dos_addr ) );
    if( root_relocs == NULL ) {
        printf( "not enough memory\n" );
        fclose( exeFP );
        exit( -1 );
    }
    // read the relocs
    safeSeek( offset );
    safeRead( root_relocs, num_relocs * sizeof( dos_addr ) );

    // separate the root and the overlay loader relocations
    ovlldr_relocs = root_relocs + num_relocs;
    root_high = root_relocs;
    ovlldr_high = ovlldr_relocs;
    p = root_relocs;
    while( p < ovlldr_relocs ) {
        if( p->seg == ovlldr_seg ) {
            *ovlldr_high++ = *p;
        } else {
            *root_high++ = *p;
        }
        ++p;
    }
    non_ovl_relocs = root_high - root_relocs;
    // sort the relocations
    qsort( root_relocs, non_ovl_relocs, sizeof( dos_addr ), compReloc );
    qsort( ovlldr_relocs, num_relocs - non_ovl_relocs,
        sizeof( dos_addr ), compReloc );
    safeSeek( offset );
    safeWrite( root_relocs, non_ovl_relocs * sizeof( dos_addr ) );
    safeWrite( ovlldr_relocs, ( num_relocs - non_ovl_relocs ) *
        sizeof( dos_addr ) );
    safeSeek( offsetof( dos_exe_header, chk_sum ) );
    safeWrite( &non_ovl_relocs, sizeof( non_ovl_relocs ) );
    free( root_relocs );
}


static void getName( const char *arg ) {
    char        d[ _MAX_DRIVE ];
    char        p[ _MAX_DIR ];
    char        f[ _MAX_FNAME ];
    char        e[ _MAX_EXT ];

    _splitpath( arg, d, p, f, e );
    _makepath( exeName, d, p, f, ( e[0] == 0 ) ? ".exe" : e );
}


int main( int argc, char **argv ) {
//=================================

    dos_exe_header      exe_hdr;
    long int            ovltab;
    int                 do_overlays;
    int                 do_watfor;

    if( argc < 2 ) {
        usage();
    }
    do_overlays = 0;
    do_watfor = 0;
    if( argc > 2 ) {
        int                     i;

        for( i = 2; i < argc; ++i ) {
            if( strcmp( argv[i], "/o" ) == 0 ) {
                do_overlays = 1;
            } else if( strcmp( argv[i], "/w" ) == 0 ) {
                do_watfor = 1;
            } else {
                usage();
            }
        }
    }
    getName( argv[1] );
    exeFP = fopen( exeName, "r+b" );
    if( exeFP == NULL ) {
        printf( "error opening %s for update: %s\n", exeName,
            strerror( errno ) );
        return( -1 );
    }
    safeRead( &exe_hdr, sizeof( exe_hdr ) );
    if( exe_hdr.signature != 0x5a4d ) {
        printf( "%s is not a valid DOS executable!\n", exeName );
        fclose( exeFP );
        return( -1 );
    }
    if( do_overlays ) {
        if( exe_hdr.reloc_offset == sizeof( exe_hdr ) ) {
            /* WLINK hasn't stuffed the ovltab offset into the file for us */
            printf("%s wasn't linked with a recent enough WLINK!\n", exeName );
            fclose( exeFP );
            return( -1 );
        }
        safeSeek( (long int)exe_hdr.reloc_offset - 4 );
        safeRead( &ovltab, sizeof( ovltab ) );
    }
    if( do_watfor ) {
        doWatfor( (long int)exe_hdr.reloc_offset, exe_hdr.num_relocs,
            exe_hdr.CS_offset );
    } else {
        sortRelocs( (long int)exe_hdr.reloc_offset, exe_hdr.num_relocs );
    }
    if( do_overlays ) {
        ovltab_entry            ovlent;
        unsigned                cur_overlay;

        cur_overlay = 0;
        for(;;) {
            safeSeek( ovltab
                + offsetof( ovl_table, entries )
                + sizeof( ovltab_entry ) * cur_overlay );
            safeRead( &ovlent.flags_anc, sizeof( ovlent.flags_anc ) );
            if( ovlent.flags_anc == OVLTAB_TERMINATOR ) break;
            safeRead( &ovlent.relocs, sizeof( ovlent )
                - sizeof( ovlent.flags_anc ) );
            sortRelocs( ovlent.disk_addr + (unsigned_32)ovlent.num_paras * 16,
                ovlent.relocs );
            ++cur_overlay;
        }
    }
    fclose( exeFP );
    return( 0 );
}
