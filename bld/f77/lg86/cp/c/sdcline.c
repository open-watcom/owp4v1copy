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
// SDCLINE   : system dependent command line processing
//

#include <string.h>
#include "ftnstd.h"
#include "global.h"
#include "fio.h"
#include "posio.h"
#include "cpopt.h"
#include "cioconst.h"
#include "errcod.h"

extern  char            *SkipBlanks(char *);
extern  void            InitCompile(void);
extern  void            ProcOpts(char *[]);
extern  void            SetInitialOpts(void);
extern  file_handle     Openf(char *,int);
extern  void            Closef(file_handle);
extern  void            PutRec(char *,int);
extern  uint            FGetRec(file_handle,char *,int);
extern  uint            Errorf(file_handle);
extern  bool            TBreak(void);
extern  void            SetCmdParms(char *,int);
extern  char            *SDSrcExtn(char *);
extern  int             Shell(char *);
extern  bool            SDError(file_handle,char *);
extern  void            InfoError(int,...);
extern  void            MsgBuffer(uint,char *,...);
#if _EDITOR == _ON
extern  char            *EdBuffName(void);
#endif

extern  a_file          *FStdIn;
extern  a_file          *FStdOut;

static  a_file          *CmdIn = { NULL };
static  bool            CmdLineParsed;


bool    ParseCmdLine( char **fn, char **rest, char **opts, char *ptr ) {
//======================================================================

    uint        opt_num;

    if( CmdLineParsed ) {
        *opts = NULL;
        return( TRUE );
    }
    *fn = NULL;
    opt_num = 0;
    for(;;) {
        ptr = SkipBlanks( ptr );
        if( *ptr == '/' ) {
            *ptr = NULLCHAR;    // terminate previous option or filename
            ++ptr;
            if( opt_num < MAX_OPTIONS ) {
                *opts = ptr;
                ++opts;
            }
            ++opt_num;
        } else if( *fn == NULL ) {
            *fn = ptr;
        } else {
            break;
        }
        for(;;) {
            if( *ptr == NULLCHAR ) break;
            if( *ptr == ' ' ) {
                *ptr = NULLCHAR;
                ++ptr;
                break;
            }
            if( *ptr == '/' ) break;
            ++ptr;
        }
    }
    *opts = NULL;
    *rest = ptr;
#if _EDITOR == _ON
    if( ( Options & OPT_EDIT ) && ( **fn == NULLCHAR ) ) {
        *fn = EdBuffName();
    }
#endif
    SrcExtn = SDSrcExtn( *fn );
    SetCmdParms( ptr, strlen( ptr ) );
    return( opt_num <= MAX_OPTIONS );
}


static  bool    GetCmdLine( char *resp ) {
//========================================

    struct stat info;

    if( fstat( CmdIn->handle, &info ) == 0 ) {
        if( S_ISCHR( info.st_mode ) ) {
            FStdOut->attrs |= CC_NOCR;
            PutRec( "WATFOR>", 7 );
            FStdOut->attrs &= ~CC_NOCR;
        }
    }
    resp[ FGetRec( CmdIn, resp, 80 ) ] = NULLCHAR;
    if( ( Errorf( CmdIn ) != IO_OK ) || TBreak() ) {
        if( CmdIn != FStdIn ) {
            Closef( CmdIn );
        }
        CmdIn = NULL;
    }
    return( CmdIn != NULL );
}


bool    MainCmdLine( char **fn, char **rest, char **opts, char *ptr ) {
//=====================================================================

    char        *endptr;

    CmdLineParsed = FALSE;
    ptr = SkipBlanks( ptr );
    endptr = &ptr[ strlen( ptr ) - 3 ];
    if( strcmp( endptr, "..." ) == 0 ) {
        if( CmdIn == NULL ) {
            CmdIn = FStdIn;
        }
        GetCmdLine( endptr );
    }
    return( ParseCmdLine( fn, rest, opts, ptr ) );
}


static  bool    DoBatch( char *buffer, uint num ) {
//=================================================

    char        *ptr;
    bool        compile;
    char        buff[LFERR_BUFF_SIZE+1];

    compile = TRUE;
    if( num == 0 ) {
        CmdIn = NULL;
        if( *SrcName == NULLCHAR ) {
            CmdIn = FStdIn;
        } else if( *SrcName == '@' ) {
            ptr = SkipBlanks( SrcName + sizeof( char ) );
            CmdIn = Openf( ptr, REC_TEXT | RDONLY );
            if( CmdIn == NULL ) {
                SDError( NULL, buff );
                InfoError( SM_OPENING_FILE, ptr, buff );
                compile = FALSE;
            }
        }
    } else if( CmdIn == NULL ) {
        compile = FALSE;
    }
    if( CmdIn != NULL ) {

        struct stat     info;

        if( fstat( CmdIn->handle, &info ) == 0 ) {
            if( S_ISCHR( info.st_mode ) ) {
                MsgBuffer( MS_WATFOR_EXIT, buff );
                PutRec( &buff[1], strlen( &buff[1] ) );
            }
        }
        for(;;) {
            if( !GetCmdLine( buffer ) ) {
                compile = FALSE;
                break;
            }
            ptr = SkipBlanks( buffer );
            if( ( *ptr != '!' ) && ( *ptr != NULLCHAR ) ) break;
            if( *ptr != NULLCHAR ) {
                if( Shell( SkipBlanks( ptr + sizeof( char ) ) ) != 0 ) {
                    MsgBuffer( MS_CANT_EXEC, buff );
                    PutRec( &buff[1], strlen( &buff[1] ) );
                }
            }
        }
    }
    return( compile );
}


char    *Batch( char *buffer, uint num ) {
//========================================

    TBreak();
    if( ( num == 0 ) && ( *SrcName != NULLCHAR ) && ( *SrcName != '@' ) ) {
        // It's the first time and a file name was specifed on the command line;
        // go compile the file.
        CmdLineParsed = TRUE;
    } else if( DoBatch( buffer, num ) ) {
        InitCompile();
        SetInitialOpts();
        CmdLineParsed = FALSE;
    } else {
        buffer = NULL;
    }
    return( buffer );
}
