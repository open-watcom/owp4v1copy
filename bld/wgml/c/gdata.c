
/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Initialze global variables
*
****************************************************************************/
#include    "wgml.h"

#define global
#include    "gvars.h"

#include    "swchar.h"

/***************************************************************************/
/*  Read environment variable and return content in allocated buffer       */
/***************************************************************************/

char *GMLGetEnv( char *name )
{
    errno_t     rc;
    size_t      len;
    char       *value;
    size_t      maxsize;

    maxsize = BUF_SIZE;
    value = GMemAlloc( maxsize );
    rc = getenv_s( &len, value, maxsize, name );
    if( rc ) {
        GMemFree( value );
        value = NULL;
        if( len ) {   /*  we need more space */
            maxsize = len + 1;
            value = GMemAlloc( maxsize );
            rc = getenv_s( &len, value, maxsize, name );
        }
    }
    if( len == 0 ) {
        GMemFree( value );
        value = NULL;
    }
    return( value );
}


void GetEnvVars( void )
{
    Pathes  = GMLGetEnv( "PATH" );
    GMLlibs = GMLGetEnv( GMLLIB );
    GMLincs = GMLGetEnv( GMLINC );
}

/* initialize globals     */
void InitGlobalVars( void )
{

    memset( &GlobalFlags, 0, sizeof( GlobalFlags ) );
    GlobalFlags.wscript = TRUE;         // wscript extension set default


    TryFileName = NULL;
    Pathes = NULL;                      // content of PATH environment var
    GMLlibs = NULL;                     // content of GMLLIB environment var
    GMLincs = NULL;                     // content of GMLINC environment var

    MasterFName = NULL;                 // Master input file name
    MasterFNameAttr = NULL;             // Master input file name attributes

    OPTFiles = NULL;                    // Command (option) file names
    FileCbs = NULL;                     // list of GML files
    OutFile = NULL;                     // output file name
    SwitchChar = _dos_switch_char();
    AltExt = GMemAlloc( 5 );            // alternate extension
    *AltExt = '\0';

    ErrCount = 0;                       // total error count
    WngCount = 0;                       // total warnig count

    GMLChar = GML_CHAR_DEFAULT;         // GML start char
    SCRChar = SCR_CHAR_DEFAULT;         // Script start char
    CWSepChar = CW_SEP_CHAR_DEFAULT;    // control word seperator

    CPI = 10;                           // chars per inch
    CPI_units = SU_chars;
    LPI = 6;                            // lines per inch
    LPI_units = SU_lines;
    Bind = 0;                           // Bind value
    Bind_units = SU_undefined;          // indicate 0 = no value
    BindOdd = 0;                        // Bind value
    BindOdd_units = SU_undefined;       // indicate 0 = no value

}

