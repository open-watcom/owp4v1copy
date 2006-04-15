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
* Description:  CodeView CV4 debug format dump routines.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>

#include "wdglb.h"
#include "dumpwv.h"
#include "wdfunc.h"
#include "cv4.h"


static  char    *cv4_dir_hdr_msg[] = {
    "2cbDirHeader - length of directory header      = ",
    "2cbDirEntry  - lentgh of each entry            = ",
    "4cDir        - number of directory entries     = ",
    "4lfoNextDir  - offset of next directory        = ",
    "4flags       - directory and sst flags         = ",
    NULL
};

static  char    *cv4_dir_entry_msg[] = {
    "2  subsection  - subsection index              = ",
    "2  iMod        - module index (1 based)        = ",
    "4  lfo         - offset from base              = ",
    "4  cb          - subsection size               = ",
    NULL
};

static  char    *cv4_sstModule_msg[] = {
    "2    ovlNumber  - overlay number               = ",
    "2    iLib       - index into sstLibraries      = ",
    "2    cSeg       - number of code segments      = ",
    "2    Style      - debugging style              = ",
    NULL
};

static  char    *cv_sstPublic_msg[] = {
    "2  offset       - symbol offset within segment = ",
    "2  seg          - segment index                = ",
    "2  type         - type index (0 if no data)    = ",
    NULL
};

/*
static  char    *cv_sstPublic32_msg[] = {
    "4  offset       - symbol offset within segment = ",
    "2  seg          - segment index                = ",
    "2  type         - type index (0 if no data)    = ",
    NULL
};
*/

static  char    *cv4_sstSrcLnSeg_msg[] = {
    "2  seg          - segment index                = ",
    "2  cPair        - number of line/offset pairs  = ",
    NULL
};

static  char    *cv_lnoff16_msg[] = {
    "2  line         - source line number           = ",
    "2  offset       - offset within segment        = ",
    NULL
};


/*
 * read_name - read length-prefixed name into 'buffer'
 */
static int read_name( char *buffer )
/**********************************/
{
    unsigned_8      len;

    Wread( &len, 1 );
    Wread( buffer, len );
    buffer[len] = '\0';
    return( len );
}


/*
 * dump_name - dump length-prefixed name, align to n-byte boundary
 * return number of bytes read
 */
static int dump_name( bool align )
/********************************/
{
    char        name[256];
    unsigned    len, pad = 0;

    len = read_name( name ) + 1;
    Wdputs( name );
    if( align ) {
        pad = align - (len & (align - 1));
    }
    if( pad ) {
        lseek( Handle, pad, SEEK_CUR );
    }
    return( len + pad + 1 );
}


/*
 * dump_cv4_sstPublic - dump CV4 sstPublic at 'offset'
 * from 'base 'containing 'size' bytes
 */
static void dump_cv4_sstPublic( unsigned_32 base, unsigned_32 offset,
                                                 unsigned_32 size )
/*******************************************************************/
{
    cv_sst_public_16    pub16;
    unsigned_32         read = 0;
    unsigned_8          name_len;
    char                name[256];

    Wlseek( base + offset );
    Wdputs( "==== sstPublic at offset " );
    Puthex( offset, 8 );
    Wdputslc( "\n" );
    while( read < size ) {
        Wread( &pub16, sizeof( pub16 ) );
        name_len = pub16.name[0];
        Dump_header( &pub16, cv_sstPublic_msg );
        read += sizeof( pub16 );
        Wread( name, name_len );
        name[name_len] = '\0';
        Wdputs( "  symbol name: \"" );
        Wdputs( name );
        read += name_len;
        Wdputslc( "\"\n" );
    }
    Wdputslc( "\n" );
}


/*
 * dump_cv4_sstLibraries - dump CV4 sstLibraries at 'offset'
 * from 'base 'containing 'size' bytes
 */
static void dump_cv4_sstLibraries( unsigned_32 base, unsigned_32 offset,
                                                     unsigned_32 size )
/**********************************************************************/
{
    unsigned        index = 0;
    unsigned_32     read = 0;

    Wlseek( base + offset );
    Wdputs( "==== sstLibraries at offset " );
    Puthex( offset, 8 );
    Wdputslc( "\n" );
    while( read < size ) {
        Wdputs( "  index: " );
        Puthex( index, 4 );
        Wdputs( "H  name: \"" );
        read += dump_name( 0 );
        Wdputslc( "\"\n" );
        ++index;
    }
    Wdputslc( "\n" );
}


/*
 * dump_cv4_sstFileIndex - dump CV4 sstFileIndex at 'offset'
 * from 'base 'containing 'size' bytes
 */
static void dump_cv4_sstFileIndex( unsigned_32 base, unsigned_32 offset,
                                                     unsigned_32 size )
/**********************************************************************/
{
    unsigned_16     *file_index;
    unsigned_16     cMod;
    unsigned_16     cRef;
    unsigned_16     *ModStart;
    unsigned_16     *cRefCnt;
    unsigned_32     *NameRef;
    char            *Names;
    unsigned        mod, file;
    char            name[256];
    unsigned_8      *len;

    Wlseek( base + offset );
    Wdputs( "==== sstFileIndex at offset " );
    Puthex( offset, 8 );
    Wdputslc( "\n\n" );
    file_index = Wmalloc( size );
    Wread( file_index, size );
    cMod     = file_index[0];
    cRef     = file_index[1];
    ModStart = &file_index[2];
    cRefCnt  = &file_index[2 + cMod];
    NameRef  = (unsigned_32 *)(cRefCnt + cMod);
    Names    = (char *)(NameRef + cRef);
    Wdputslc( "  Module  File    Name\n" );
    for( mod = 0; mod < cMod; ++mod ) {
        for( file = 0; file < cRefCnt[mod]; ++file ) {
            /* CV spec says names are zero terminated, but actual NB11 format
             * data from MSVC 6 clearly uses length prefixed names!?!
             */
            Wdputs( "  " );
            Puthex( mod + 1, 4 );
            Wdputs( "H   " );
            Puthex( file, 4 );
            Wdputs( "H   \"" );
            len = (unsigned_8 *)(Names + NameRef[ModStart[mod] + file]);
            memcpy( name, (char *)len + 1, *len );
            name[*len] = '\0';
            Wdputs( name );
            Wdputslc( "\"\n" );
        }
    }
    Wdputslc( "\n" );
}


/*
 * dump_cv4_seg - dump CV4 cv_seginfo structure
 */
static void dump_cv4_seg( cv_seginfo *seg )
{
    Wdputs( "      " );
    Puthex( seg->Seg, 4 );
    Wdputs( "H         " );
    Puthex( seg->offset, 8 );
    Wdputs( "H   " );
    Puthex( seg->cbSeg, 8 );
    Wdputslc( "H\n" );
}


/*
 * dump_cv4_sstModule - dump CV4 sstModule at 'offset' from 'base'
 */
static void dump_cv4_sstModule( unsigned_32 base, unsigned_32 offset )
/********************************************************************/
{
    cv_sst_module       mod;
    cv_seginfo          seg;

    Wlseek( base + offset );
    Wdputs( "==== sstModule at offset " );
    Puthex( offset, 8 );
    Wdputslc( "\n" );
    Wread( &mod, sizeof( mod ) );
    Dump_header( &mod, cv4_sstModule_msg );
    if( mod.cSeg-- ) {
        Wdputslc( "      Segment idx   Offset      Code size\n" );
        dump_cv4_seg( mod.SegInfo );
        while( mod.cSeg-- ) {
            Wread( &seg, sizeof( seg ) );
            dump_cv4_seg( &seg );
        }
    } else {
        /* seek back in case there are no code segments in module */
        Wlseek( base + offset + sizeof( mod ) - sizeof( seg ) );
    }
    Wdputs( "    module name: " );
    dump_name( 0 );
    Wdputslc( "\n\n" );
}


typedef struct {
    unsigned_16     line;
    unsigned_16     offset;
} cv_srcln_off_16;


/*
 * dump_cv4_sstSrcLnSeg - dump sstSrcLnSeg at 'offset' from 'base'
 */
static void dump_cv4_sstSrcLnSeg( unsigned_32 base, unsigned_32 offset )
/**********************************************************************/
{
    bool                first = TRUE;
    cv_sst_src_lne_seg  src_ln;
    cv_srcln_off_16     lo_16;

    Wlseek( base + offset );
    Wdputs( "==== sstSrcLnSeg at offset " );
    Puthex( offset, 8 );
    Wdputslc( "\n" );
    Wdputs( "  source file: \"" );
    dump_name( 2 );
    Wdputslc( "\"\n" );
    Wread( &src_ln, sizeof( src_ln ) );
    Dump_header( &src_ln, cv4_sstSrcLnSeg_msg );
    while( src_ln.cPair-- ) {
        Wread( &lo_16, sizeof( lo_16 ) );
        Dump_header( &lo_16, cv_lnoff16_msg );
        first = FALSE;
    }
    Wdputslc( "\n" );
}


/*
 * dump_cv4_subsection - dump any CV4 subsection
 */
static void dump_cv4_subsection( unsigned_32 base, cv_directory_entry *dir )
/**************************************************************************/
{
    sst         sst_index;

    sst_index = dir->subsection;
    switch( sst_index ) {
    case sstModule:
        if( Debug_options & MODULE_INFO ) {
            dump_cv4_sstModule( base, dir->lfo );
        }
        break;
    case sstPublic:
        if( Debug_options & GLOBAL_INFO ) {
            dump_cv4_sstPublic( base, dir->lfo, dir->cb );
        }
        break;
    case sstSrcLnSeg:
        if( Debug_options & LINE_NUMS ) {
            dump_cv4_sstSrcLnSeg( base, dir->lfo );
        }
        break;
    case sstLibraries:
        if( Debug_options & MODULE_INFO ) {
            dump_cv4_sstLibraries( base, dir->lfo, dir->cb );
        }
        break;
    case sstFileIndex:
        if( Debug_options & MODULE_INFO ) {
            dump_cv4_sstFileIndex( base, dir->lfo, dir->cb );
        }
        break;
    }
}


/*
 * dump_cv4 - dump CV4 data at offset 'base' from start of file
 */
static void dump_cv4( unsigned_32 base )
/**************************************/
{
    cv_trailer                  header;
    cv_subsection_directory     sst_dir_hdr;
    cv_directory_entry          sst_dir_entry;
    int                         i;

    Wlseek( base );
    Wread( &header, sizeof( header ) );
    if( memcmp( header.sig, CV4_NB09, CV_SIG_SIZE ) == 0 ) {
        Wdputslc( "Signature NB09\n\n" );
    } else if( memcmp( header.sig, CV4_NB11, CV_SIG_SIZE ) == 0 ) {
        Wdputslc( "Signature NB11\n\n" );
    } else {
        return;
    }
    Wlseek( base + header.offset );
    Wread( &sst_dir_hdr, sizeof( sst_dir_hdr ) );
    Dump_header( &sst_dir_hdr , cv4_dir_hdr_msg );
    Wdputslc( "\n" );
    for( i = 0; i < sst_dir_hdr.cDir; ++i ) {
        Wlseek( base + header.offset + sst_dir_hdr.cbDirHeader + i * sst_dir_hdr.cbDirEntry );
        Wread( &sst_dir_entry, sizeof( sst_dir_entry ) );
        Dump_header( &sst_dir_entry, cv4_dir_entry_msg );
        Wdputslc( "\n" );
        dump_cv4_subsection( base, &sst_dir_entry );
    }
}


/*
 * Dmp_cv_head - dump CodeView debugging information
 */
bool Dmp_cv_head( void )
/**********************/
{
    off_t           end_off;
    off_t           dbg_off;
    cv_trailer      trailer;

    end_off = lseek( Handle, 0, SEEK_END );
    Wlseek( end_off - sizeof( trailer ) );
    Wread( &trailer, sizeof( trailer ) );
    dbg_off = end_off - trailer.offset;
    if( memcmp( trailer.sig, CV4_NB09, CV_SIG_SIZE ) == 0
     || memcmp( trailer.sig, CV4_NB11, CV_SIG_SIZE ) == 0 ) {
        Banner( "CodeView debugging information (CV4)" );
        Wdputs( "debugging information base  = " );
        Puthex( dbg_off, 8 );
        Wdputslc( "H\n" );
        Wdputs( "subsection directory offset = " );
        Puthex( trailer.offset, 8 );
        Wdputslc( "H\n" );
        dump_cv4( dbg_off );
        return( 1 );
    }
    return( 0 );
} /* Dmp_cv_head */
