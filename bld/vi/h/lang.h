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
* Description:  Languages defined for syntax highlighting.
*
****************************************************************************/


#ifndef _LANG_INCLUDED
#define _LANG_INCLUDED

enum {
    LANG_NONE,          // LANG_NONE always first
    LANG_C,
    LANG_CPP,
    LANG_FORTRAN,
    LANG_JAVA,
    LANG_SQL,
    LANG_BAT,
    LANG_BASIC,
    LANG_PERL,
    LANG_HTML,
    LANG_WML,
    LANG_GML,
    LANG_DBTEST,
    LANG_MAKEFILE,
    LANG_RC,
    LANG_USER,
    LANG_MAX            // LANG_MAX always last
};

typedef struct tag_hash_entry {
    bool                    real;
    char                    *keyword;
    struct tag_hash_entry   *next;
} hash_entry;

typedef struct tag_lang_info {
    hash_entry          *keyword_table;
    int                 table_entries;
    int                 ref_count;
    char                *read_buf;
} lang_info;

bool    IsKeyword( char *keyword, bool case_ignore );
bool    IsPragma( char *pragma );
void    LangInit( int );
void    LangFini( int );
void    LangFiniAll( void );

#endif
