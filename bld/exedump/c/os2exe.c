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
* Description:  NE/LE/LX dumping routines.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#include "wdglb.h"
#include "wdfunc.h"

char * os2_exe_msg[] = {
    "2link version                                         = ",
    "2offset of entry table                                = ",
    "2length of entry table                                = ",
    "432-bit chksum                                        = ",
    "2module flag                                          = ",
    "2segment number of auto data segment                  = ",
    "2initial size of local heap added to auto data seg    = ",
    "2initial size of stack to be added to auto data seg   = ",
    "2initial value of IP on entry                         = ",
    "2initial segment number for setting CS on entry       = ",
    "2initial value of SP on entry                         = ",
    "2segment number for setting SS on entry               = ",
    "2number of entries in segment table                   = ",
    "2number of entries in module reference table          = ",
    "2number of bytes in nonresident names table           = ",
    "2offset of beg of segment table( rel NH )             = ",
    "2offset of beg of resource table                      = ",
    "2offset of beg of resident names table                = ",
    "2offset of beg of module reference table              = ",
    "2offset of beg of imported names table                = ",
    "4offset of nonresident names table ( rel file )       = ",
    "2number of movable entry points listed in entry table = ",
    "2alignment shift count( 0 => 9 )                      = ",
    "2number of reserved segments                          = ",
    "1target OS (1==OS2, 2==Windows, 3==DOS4, 4==Win386)   = ",
    "1other flags                                          = ",
    "2offset of gangload area  (Windows only)              = ",
    "2length of gangload area  (Windows only)              = ",
    "2swap area                (Windows only)              = ",
    "2expected Windows version (Windows only)              = ",
    NULL
};

char *os2_386_msg[] = {
    "1byte order (0==little endian, 1==big endian)      = ",
    "1word order       \"                \"               = ",
    "4linear EXE format level                           = ",
    "2cpu type                                          = ",
    "2os type (1==OS2, 2==Windows, 3==DOS4, 4==Win386)  = ",
    "4module version                                    = ",
    "4module flags                                      = ",
    "4# module pages                                    = ",
    "4object # for initial EIP                          = ",
    "4initial EIP                                       = ",
    "4object # for initial ESP                          = ",
    "4initial ESP                                       = ",
    "4page size                                         = ",
    "4last page size (LE)/page shift (LX)               = ",
    "4fixup section size                                = ",
    "4fixup section checksum                            = ",
    "4loader section size                               = ",
    "4loader section checksum                           = ",
    "4object table offset                               = ",
    "4# of objects in module                            = ",
    "4object page map offset                            = ",
    "4object iterated data map offset                   = ",
    "4offset of resource table                          = ",
    "4# of resource entries                             = ",
    "4offset of resident name table                     = ",
    "4offset of entry table                             = ",
    "4offset of module directive table                  = ",
    "4# of module directives                            = ",
    "4offset of fixup page table                        = ",
    "4offset of fixup record table                      = ",
    "4offset of import module name table                = ",
    "4# of entries in import module name table          = ",
    "4offset of import procedure name table             = ",
    "4offset of per-page checksum table                 = ",
    "4offset of enumerated data pages                   = ",
    "4# of pre-load pages                               = ",
    "4offset of non-resident names table (rel file)     = ",
    "4size of non-resident names table                  = ",
    "4non-resident names table checksum                 = ",
    "4object # for automatic data object                = ",
    "4offset of the debugging information               = ",
    "4size of the debugging information                 = ",
    "4# of instance pages in the preload section        = ",
    "4# of instance pages in the demand load section    = ",
    "4size of heap (for 16-bit apps)                    = ",
    NULL
};

char *os2_obj_msg[] = {
    "4virtual memory size              = ",
    "4          relocation base address          = ",
    "4          object flag bits                 = ",
    "4          object page table index          = ",
    "4          # of object page table entries   = ",
    "4          reserved                         = ",
    NULL
};

char *map_flgs[] = { "VALID", "ITERATED", "INVALID", "ZEROED", "RANGE" };

/*
 * Dump the New Executable Header, if any.
 */
bool Dmp_os2_head( void )
/***********************/
{
    Wlseek( New_exe_off );
    Wread( &Os2_head, sizeof( struct os2_exe_header ) );
    if( Os2_head.signature != OS2_SIGNATURE_WORD ) {
        return( 0 );
    }
    if( Os2_head.align == 0 ) {
        Os2_head.align = 9;
    }
    Banner( "New EXE Header (OS/2 or Windows)" );
    Wdputs( "file offset = " );
    Puthex( New_exe_off, 8 );
    Wdputslc( "H\n" );
    Wdputslc( "\n" );
    Dump_header( (char *)&Os2_head.version, os2_exe_msg );
    dmp_mod_flag( Os2_head.info );
    Dmp_seg_tab();
    Dmp_resrc_tab();
    Dmp_ne_tbls();
    if( Options_dmp & OS2_SEG_DMP ) {
        Dmp_segments();
    }
    if( Int_seg_tab != NULL ) {
        free( Int_seg_tab );
    }
    return( 1 );
}

/*
 * Dump the 386 Executable Header, if any.
 */
bool Dmp_386_head( void )
/***********************/
{
    Wlseek( New_exe_off );
    Wread( &Os2_386_head, sizeof( Os2_386_head ) );
    if( Os2_386_head.signature == OSF_FLAT_SIGNATURE ) {
        Form = FORM_LE;
        Banner( "Linear EXE Header (OS/2 V2.0) - LE" );
    } else if ( Os2_386_head.signature == OSF_FLAT_LX_SIGNATURE ) {
        Form = FORM_LX;
        Banner( "Linear EXE Header (OS/2 V2.0) - LX" );
    } else {
        return( 0 );
    }
    Wdputs( "file offset = " );
    Puthex( New_exe_off, 8 );
    Wdputslc( "H\n" );
    Wdputslc( "\n" );
    Dump_header( (char *)&Os2_386_head.byte_order, os2_386_msg );
    dmp_mod_flag( Os2_386_head.flags );
    dmp_obj_table();
    Dmp_resrc2_tab();
    Dmp_le_lx_tbls();
    return( 1 );
}

/*
 * dump the module flag word
 */
void dmp_mod_flag( unsigned_16 flag )
/***********************************/
{
    Wdputs( "Module flag Word = " );
    if( flag & OS2_IS_DLL ) {
        Wdputs( "LIBRARYMODE" );
    } else {
        Wdputs( "PROGRAMMODE" );
    }
    if( flag & WIN_USES_EMS_DIRECT ) {
        Wdputs( " | NONCONFORMING" );
    }
    if( flag & OS2_LINK_ERROR ) {
        Wdputs( " | LINKERRORSDETECTED" );
    }
    switch( flag & OS2_COMPATIBILITY_MASK ) {
    case OS2_NOT_PM_COMPATIBLE:
        Wdputs( " | NOT_PM_COMPATABLE" );
        break;
    case OS2_PM_COMPATIBLE:
        Wdputs( " | PM_COMPATABLE" );
        break;
    case OS2_PM_APP:
        Wdputs( " | PM_APPLICATION" );
        break;
    }
    if( flag & OS2_PROT_MODE_ONLY ) {
        Wdputs( " | RUNSPROTECTMODE" );
    }
    if( flag & OS2_INIT_INSTANCE ) {
        Wdputs( " | RUNSREALMODE" );
    }
    if( ( flag & OS2_MULT_AUTO ) && !( flag & OS2_IS_DLL ) ) {
        Wdputs( " | MULTIPLEDATA" );
    }
    if( flag & OS2_IS_DLL ) {
        if( flag & OS2_SINGLE_AUTO ) {
            Wdputs( " | SINGLEDATA" );
        } else {
            Wdputs( " | NOAUTODATA" );
        }
    }
    if( flag & OSF_INTERNAL_FIXUPS_DONE ) {
        Wdputs( " | NO_INT_FIXUPS" );
    }
    if( flag & OSF_EXTERNAL_FIXUPS_DONE ) {
        Wdputs( " | NO_EXT_FIXUPS" );
    }
    Wdputslc( "\n" );
    Wdputslc( "\n" );
}

/*
 * dump the module flag word
 */
static void dmp_obj_table( void )
/*******************************/
{
    unsigned_16     i;
    object_record   os_obj;

    Banner( "Object Table" );
    for( i = 0; i < Os2_386_head.num_objects; i++ ) {
        Wlseek( New_exe_off + Os2_386_head.objtab_off
                            + i * sizeof( object_record ) );
        Wread( &os_obj, sizeof( object_record ) );
        Wdputs( "object " );
        Putdec( i );
        Wdputs( ": " );
        Dump_header( (char *)&os_obj.size, os2_obj_msg );
        Wdputs( "          flags = " );
        dmp_obj_flags( os_obj.flags );
        dmp_obj_page( os_obj );
        Wdputslc( "\n" );
    }
}

/*
 * dump the object page table
 */
static void dmp_obj_page( object_record obj )
/*******************************************/
{
    unsigned_32     j;
    unsigned_32     offset;
    map_entry       map;

    if( Form == FORM_LX ) {
        offset = ( obj.mapidx-1 ) * sizeof( lx_map_entry );
    } else {
        offset = ( obj.mapidx-1 ) * sizeof( le_map_entry );
    }
    offset += New_exe_off + Os2_386_head.objmap_off;
    for( j = 0; j < obj.mapsize; ++j ) {
        Data_count++;
        Wdputs( "    page # " );
        Putdec( Data_count );
        Wdputs( "  map page = " );
        switch( Form ) {
        case FORM_LE:
            Wlseek( offset );
            Wread( &map, sizeof( le_map_entry ) );
            offset += sizeof( le_map_entry );
            Puthex( map.le.page_num[0], 2 );
            Puthex( map.le.page_num[1], 2 );
            Puthex( map.le.page_num[2], 2 );
            Wdputs( "H flgs = " );
            Puthex( map.le.flags, 2 );
            Wdputs( "H " );
            Wdputs( map_flgs[ map.le.flags ] );
            if( Options_dmp & OS2_SEG_DMP ) {
                Dmp_le_page_seg();
            }
            break;
        case FORM_LX:
            Wlseek( offset );
            Wread( &map, sizeof( lx_map_entry ) );
            offset += sizeof( lx_map_entry );
            Puthex( map.lx.page_offset, 8 );
            Wdputs( "H size = " );
            Puthex( map.lx.data_size, 4 );
            Wdputs( "H flgs = " );
            Puthex( map.le.flags, 2 );
            Wdputs( "H " );
            Wdputs( map_flgs[ map.lx.flags ] );
            if( Options_dmp & OS2_SEG_DMP ) {
                Dmp_lx_page_seg( map );
            }
            break;
        }
        Wdputslc( "\n" );
    }
}

/*
 * dump the object flag word
 */
static void dmp_obj_flags( unsigned_32 flags )
/********************************************/
{
    char    name[256];

    name[0] = 0;
    if( flags & OBJ_READABLE ) {
        strcat( name, "READABLE|" );
    }
    if( flags & OBJ_WRITEABLE ) {
        strcat( name, "WRITABLE|" );
    }
    if( flags & OBJ_EXECUTABLE ) {
        strcat( name, "EXECUTABLE|" );
    }
    if( flags & OBJ_RESOURCE ) {
        strcat( name, "RESOURCE|" );
    }
    if( flags & OBJ_DISCARDABLE ) {
        strcat( name, "DISCARDABLE|" );
    }
    if( flags & OBJ_SHARABLE ) {
        strcat( name, "SHARABLE|" );
    }
    if( flags & OBJ_HAS_PRELOAD ) {
        strcat( name, "HAS_PRELOAD|" );
    }
    if( flags & OBJ_HAS_INVALID ) {
        strcat( name, "HAS_INVALID|" );
    }
    if( flags & OBJ_PERM_SWAPPABLE ) {
        strcat( name, "PERM_SWAPPABLE|" );
    }
    if( flags & OBJ_PERM_RESIDENT ) {
        strcat( name, "PERM_RESIDENT|" );
    }
    if( flags & OBJ_PERM_LOCKABLE ) {
        strcat( name, "PERM_LOCKABLE|" );
    }
    if( flags & OBJ_ALIAS_REQUIRED ) {
        strcat( name, "16:16_ALIAS|" );
    }
    if( flags & OBJ_BIG ) {
        strcat( name, "BIG|" );
    }
    if( flags & OBJ_CONFORMING ) {
        strcat( name, "CONFORMING|" );
    }
    if( flags & OBJ_IOPL ) {
        strcat( name, "IOPL|" );
    }
    if( name[strlen(name)-1] == '|' ) {
        name[strlen(name)-1] = 0;
    }
    Wdputs( name );
    Wdputslc( "\n" );
}
