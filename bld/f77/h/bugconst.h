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


// The following defines all debugger commands. The tables BugCmds,
// BugRtns and CmdEnd depend on this ordering:

enum {
    DB_JUNK,            // bad command
    DB_NULL,            // null command
    DB_BREAKPOINT,      // breakpoint
    DB_GO,              // go
    DB_HELP,            // help
    DB_QUIT,            // quit
    DB_TRACE,           // trace
    DB_WHERE,           // where
    DB_DISPLAY,         // display
    DB_UNIT,            // unit
    DB_LOGIO,           // logio
    DB_SYSTEM,          // system
    DB_MODIFY,          // modify
    DB_PRINT,           // print
};

#define DB_FIRST_CMD    DB_BREAKPOINT

// Formatting the PRINT command

#define NAME_COL        (10 + 8 + 1)    // room CHARACTER*n where n is 3 MByte
#define VALUE_COL       (NAME_COL + 12) // leaves room for complex*16

#define MAX_ISN         65535   // largest ISN
#define DISPLAY_WINDOW  5       // # of lines before and after current
                                // line displayed

// Trace bits

#define TRACE           0x01    // tracing in effect
#define TRACE_INTO      0x02    // trace into calls
#define TRACE_DEPTH     0x04    // keep track of subroutine depth

// Format codes for displaying data

#define DEFAULT_FMT     0       // use default format
#define Z_FMT           1       // use Z-format
#define A_FMT           2       // use A-format
