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
* Description:  decorate/truncate object file symbol names
*
****************************************************************************/


#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "standard.h"
#include "sysmacro.h"
#include "cg.h"
#include "bckdef.h"
#include "cgaux.h"
#include "objrep.h"
#include "system.h"
#include "model.h"
#include "hostsys.h"
#include "ocentry.h"
#include "import.h"
#include "feprotos.h"
#include "targsys.h"

#include "langenvd.h"
#if _TARGET & _TARG_PPC
  #define   __TGT_SYS   __TGT_SYS_AXP_PPC
#elif _TARGET & _TARG_AXP
  #define   __TGT_SYS   __TGT_SYS_AXP_NT
#else
  #define   __TGT_SYS   __TGT_SYS_X86
#endif
#include "langenv.h"

extern  uint            Length(pointer);
extern  char            *CopyStr(char*,char*);

#define SPEC_PREFIX             ".PREFIX_DATA."
#define DLLIMPORT_PREFIX        "__imp_"
#define PIC_RW_PREFIX           "__rw_"

#define TRUNC_SYMBOL_HASH_LEN        4
#define TRUNC_SYMBOL_LEN_WARN        120

static uint_32 objNameHash( uint_32 h, char *s )
/**********************************************/
{
    uint_32 c;
    uint_32 g;

    // don't change this in a patch
    for(;;) {
        /* ( h & ~0x0ffffff ) == 0 is always true here */
        c = *s;
        if( c == 0 ) break;
        h = (h << 4) + c;
        g = h & ~0x0ffffff;
        h ^= g;
        h ^= g >> 24;
        ++s;
    }
    return( h );
}

static char *createFourCharHash( char *mangle, char *buff, int ucase )
/********************************************************************/
{
    uint_32 mangle_hash;
    int     i;
    char    c;

    c = ( ucase ) ? 'A' : 'a';
    mangle_hash = objNameHash( 0, mangle );
    for( i = 0; i < TRUNC_SYMBOL_HASH_LEN; i++ ) {
        int x;

        x = ( mangle_hash % 36 ) + '0';
        if( x > '9' ) {
            x += c - '0' - 10;
        }
        buff[ TRUNC_SYMBOL_HASH_LEN - 1 - i ] = x;
        mangle_hash /= 36;
    }
    buff[ i ] = '\0';
    return( buff );
}

#if 0
static char *reduceMangledName( char *mangle )
{
    auto char buff[MANGLED_HASH_LEN+1];

    /* change "W?xxx" to "T?hhhhxxx" */
    createFourCharHash( mangle, buff );
    memmove( mangle + 2 + MANGLED_HASH_LEN, mangle + 2,
             MANGLED_MAX_LEN - MANGLED_HASH_LEN );
    memcpy( mangle + 2, buff, MANGLED_HASH_LEN );
    mangle[MANGLED_MAX_LEN] = '\0';
    mangle[0] = IN_TRUNCATE;
    return( mangle );
}

static char *reduceNonMangledName( char *mangle )
{
    char *hash_name;
    auto char buff[MANGLED_HASH_LEN+1];

    /* change "xxx" to "xxxhhhh" */
    hash_name = createFourCharHash( mangle, buff );
    memmove( mangle + ( MANGLED_MAX_LEN - MANGLED_HASH_LEN ), buff,
             MANGLED_HASH_LEN );
    mangle[MANGLED_MAX_LEN] = '\0';
    return( mangle );
}
#endif

static int truncateSymbolName( char fce, char *dst, char *src, int src_len, int max_dst_len )
/*******************************************************************************************/
{
    int     len;
    int     i;
    char    *p;
    char    c;

    len = 0;
    p = src;
    for( i = 0; i < src_len; i++ ) {
        if( len >= max_dst_len ) {
            break;
        } else if( len == max_dst_len - TRUNC_SYMBOL_HASH_LEN ) {
            p = src + i;
        }
        c = src[i];
        if( fce == '\0' ) {
            if( c == '\\' )
                continue;
            dst[ len++ ] = c;
        } else if( fce == '*' ) {
            dst[ len++ ] = c;
        } else if( fce == '!' ) {
            dst[ len++ ] = tolower( c );
        } else if( fce == '^' ) {
            dst[ len++ ] = toupper( c );
        }
    }
    if( len >= max_dst_len ) {
        char    buff[ TRUNC_SYMBOL_HASH_LEN + 1 ];

        createFourCharHash( p, buff, ( fce == '^' ) );
        if( ( dst[0] == 'W' ) && ( dst[1] == '?' ) ) {
            assert( len >= TRUNC_SYMBOL_HASH_LEN + 2 );
            dst[0] = 'T';
            memmove( dst + TRUNC_SYMBOL_HASH_LEN + 2, dst + 2, len - TRUNC_SYMBOL_HASH_LEN );
            dst[ len ] = 0;
            strncpy( dst + 2, buff, TRUNC_SYMBOL_HASH_LEN );
        } else {
            assert( len >= TRUNC_SYMBOL_HASH_LEN );
            strncpy( dst + len - TRUNC_SYMBOL_HASH_LEN, buff, TRUNC_SYMBOL_HASH_LEN );
        }
        return( -len );
    } else {
        return( len );
    }
}

char *xtoa( char *p, int x )
/**************************/
{
    if( x > 10 ) {
        p = xtoa( p, x/10 );
        x %= 10;
    }
    *p = x + '0';
    return( ++p );
}

static int GetExtName( sym_handle sym, char *buffer, int max_len )
/****************************************************************/
{
    char                 *src;
    char                 *dst;
    char                 *p;
    char                 *prefix;
    char                 *sufix;
    char                 c;
    int                  basename_len;
    int                  max_sym_len;
    char                 *pattern;

    pattern = FEExtName( sym, EXTN_PATTERN );
    c = 0;
    basename_len = 0;
    prefix = NULL;
    for( p = pattern; *p != '\0'; p++ ) {
        if(( *p == '*' ) || ( *p == '!' ) || ( *p == '^' )) {
            if( c == '\0' ) {
                prefix = p;
                c = *p;
            }
        } else if( *p == '#' ) {
            if( c == '\0' ) {
                basename_len = p - pattern;
            }
            break;
        } else if( *p == '\\' ) {
            if( c != '\0' )
                break;
            p++;
        } else if( c != '\0' ) {
            break;
        }
    }
    sufix = p;
    // add prefix to buffer
    dst = buffer;
    for( src = pattern; src < prefix; ++src ) {
        if( *src != '\\' ) {
            *(dst++) = *src;
        }
    }
    *dst = '\0';
    // add sufix to buffer
    p = dst;
    for( src = sufix; *src != '\0'; ++src ) {
        if( *src == '#' ) {
            int size;

            *(p++) = '@';
            size = (int)FEExtName( sym, EXTN_PRMSIZE );
            p = xtoa( p, size );
        } else {
            if( *src == '\\' )
                ++src;
            *(p++) = *src;
        }
    }
    *p = '\0';
    // calculate max base symbol length
    max_sym_len = max_len - ( p - buffer );
    if( max_sym_len > 0 ) {
        int     sufix_len;
        int     base_len;

        if( basename_len != 0 ) {
            // alias
            src = pattern;
        } else {
            src = FEExtName( sym, EXTN_BASENAME );
            basename_len = strlen( src );
        }
        // shift sufix to the end of buffer
        sufix_len = p - dst;
        sufix = buffer + max_len - sufix_len;
        memmove( sufix, dst, sufix_len + 1 );
        // copy + truncate base symbol name
        base_len = truncateSymbolName( c, dst, src, basename_len, max_sym_len );
        if( base_len < 0 ) {
//            char    symx[ TRUNC_SYMBOL_LEN_WARN + 1 ];
            
//            strncpy( symx, src, TRUNC_SYMBOL_LEN_WARN );
//            symx[ TRUNC_SYMBOL_LEN_WARN ] = 0;
//            SetSymLoc( &sym );
//            CWarn( WARN_SYMBOL_NAME_TOO_LONG, ERR_SYMBOL_NAME_TOO_LONG, symx );
        } else {
            // shift sufix to the end of truncated symbol name
            memcpy( dst + base_len, sufix, sufix_len + 1 );
        }
    } else {
       // TODO: error prefix + sufix >= max_len
       assert( 0 );
    }
    return( 0 );
}

extern  void    DoOutObjectName( sym_handle sym,
                                void (*outputter)( char *, void * ),
                                void *data,
                                import_type kind ) {
/*******************************************************************/

    char        *dst;
    char        buffer[TS_MAX_OBJNAME + TRUNC_SYMBOL_HASH_LEN];
    unsigned    pref_len;

    dst = buffer;
    switch( kind ) {
    case SPECIAL:
        pref_len = (sizeof( SPEC_PREFIX )-1);
        dst = CopyStr( SPEC_PREFIX, dst );
        break;
    case DLLIMPORT:
        pref_len = (sizeof( DLLIMPORT_PREFIX )-1);
        dst = CopyStr( DLLIMPORT_PREFIX, dst );
        break;
    case PIC_RW:
        pref_len = (sizeof( PIC_RW_PREFIX )-1);
        dst = CopyStr( PIC_RW_PREFIX, dst );
        break;
    default:
        pref_len = 0;
    }
    GetExtName( sym, dst, TS_MAX_OBJNAME - 1 - pref_len );
    outputter( buffer, data );
}

extern bool SymIsExported( sym_handle sym ) {
/*******************************************/

    bool        exported;

    exported = FALSE;
    if( sym != NULL ) {
        if( FEAttr( sym ) & FE_DLLEXPORT ) {
            exported = TRUE;
        } else if( *(call_class*)FEAuxInfo( FEAuxInfo( sym, AUX_LOOKUP ), CALL_CLASS ) & DLL_EXPORT ) {
            exported = TRUE;
        }
    }
    return( exported );
}
