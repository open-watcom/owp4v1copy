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
* Description:  Project type functions.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "gui.h"
#include "projtype.h"
#include "rcstr.gh"

#define PROJTYPE_CONFIG_FILE    "projtype.cfg"

typedef struct project_type {
    char                typename[128];
    char                friendlyname[128];
    struct project_type *next;
} project_type;

project_type    *projectTypes = NULL;
project_type    *lastProjectType = NULL;

static bool nextToken( FILE *fh, char *buf, int nbuf )
/****************************************************/
{
    int ch;
    ch = getc( fh );
    while( isspace( ch ) ) {
        ch = getc( fh );
    }
    if( isalpha( ch ) ) {
        while( isalnum( ch ) ) {
            *buf = ch;
            buf++;
            nbuf--;
            if( nbuf == 0 ) {
                return( FALSE );
            }
            ch = getc( fh );
        }
        ungetc( ch, fh );
        *buf = '\0';
        return( TRUE );
    }
    if( ch == '"' ) {
        ch = getc( fh );
        while( ch != '"' ) {
            *buf = ch;
            buf++;
            nbuf--;
            if( nbuf == 0 ) {
                return( FALSE );
            }
            ch = getc( fh );
        }
        *buf = '\0';
        return( TRUE );
    }
    if( ch == ',' ) {
        *buf = ch;
        buf++;
        *buf = '\0';
        return( TRUE );
    }
    return( FALSE );
}

static void addProjectType( char *typename, char *friendlyname )
/**************************************************************/
{
    if( lastProjectType == NULL ) {
        projectTypes = (project_type *)malloc( sizeof( project_type ) );
        lastProjectType = projectTypes;
    } else {
        lastProjectType->next = (project_type *)malloc( sizeof( project_type ) );
        lastProjectType = lastProjectType->next;
    }
    strcpy( lastProjectType->typename, typename );
    strcpy( lastProjectType->friendlyname, friendlyname );
    lastProjectType->next = NULL;
}

static void showError( int errcode, ... )
/***************************************/
{
    char    fmt[128];
    char    msg[128];
    va_list args;

    va_start( args, errcode );
    GUILoadString( errcode, fmt, 128 );
    vsprintf( msg, fmt, args );
    GUIDisplayMessage( NULL, msg, "", GUI_OK );
    va_end( args );
}

bool ReadProjectTypes()
/*********************/
{
    FILE    *fh;
    char    filepath[PATH_MAX];
    char    token[128];
    char    typename[128];
    char    friendlyname[128];
    
    _searchenv( PROJTYPE_CONFIG_FILE, "PATH", filepath );
    if( filepath[0] == '\0' ) {
        showError( APPWIZ_CFG_MISSING, PROJTYPE_CONFIG_FILE );
        return( FALSE );
    }
    fh = fopen( filepath, "r" );
    while( !feof( fh ) ) {
        if( nextToken( fh, token, 128 ) && strcmp( token, "ProjectType" ) == 0 ) {
            if( !nextToken( fh, typename, 128 ) || !isalpha( typename[0] ) ||
                !nextToken( fh, token, 128 ) || strcmp( token, "," ) != 0 ||
                !nextToken( fh, friendlyname, 128 ) ) {
                showError( APPWIZ_INVALID_PROJTYPES );
                return( FALSE );
            }
            addProjectType( typename, friendlyname );
        } else if( !feof( fh ) ) {
            showError( APPWIZ_INVALID_PROJTYPES );
            return( FALSE );
        }
    }
    fclose( fh );

    return( TRUE );
}

void FreeProjectTypes()
/*********************/
{
    project_type    *this_type;
    project_type    *next_type;

    next_type = projectTypes;
    while( next_type != NULL ) {
        this_type = next_type;
        next_type = this_type->next;
        free( this_type );
    }
    projectTypes = NULL;
    lastProjectType = NULL;
}

project_type_iterator GetFirstProjectType()
/*****************************************/
{
    return( (project_type_iterator)projectTypes );
}

void GetNextProjectType( project_type_iterator *iter, char *typename,
                         char *friendlyname )
/*******************************************************************/
{
    project_type    *type;

    if( *iter != NULL ) {
        type = (project_type *)*iter;
        if( typename != NULL ) {
            strcpy( typename, type->typename );
        }
        if( friendlyname != NULL ) {
            strcpy( friendlyname, type->friendlyname );
        }
        *iter = (project_type_iterator)type->next;
    }
}

