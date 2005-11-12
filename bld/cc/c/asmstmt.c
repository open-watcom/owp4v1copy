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
* Description:  asm statement processing
*
****************************************************************************/

#include "cvars.h"
#include "pragdefn.h"
#include "asmstmt.h"

#if _INTEL_CPU
#include "asminlin.h"
#else
#include "asinline.h"
#endif

static int EndOfAsmStmt( void )
/*****************************/
{
    if( CurToken == T_SEMI_COLON ) {
        // ; .ASM comment
        for( ;; ) {
            NextToken();
            if( ( CurToken == T_EOF )
              || ( CurToken == T_NULL ) ) {
                break;
            }
        }
        return( TRUE );
    }
    if( CurToken == T_EOF )
        return( TRUE );
    if( CurToken == T_NULL )
        return( TRUE );
    if( CurToken == T___ASM )
        return( TRUE );
    if( CurToken == T_RIGHT_BRACE )
        return( TRUE );
    return( FALSE );
}

static void GetAsmLine( void )
/****************************/
{
    int         AsmLineNo;
    enum TOKEN  LastToken = T_DOT;
    char        buf[256] = { '\0' };
   
    CompFlags.pre_processing = 1;
    AsmLineNo = TokenLine;
    if( strcmp( Buffer, "_emit" ) == 0 ) {
        strcpy( buf, AsmSysDefineByte() );
        NextToken();
    }
    for( ;; ) {
        if( EndOfAsmStmt() )
            break;
        if(( LastToken != T_DOT )
          && ( LastToken != T_BAD_CHAR )
          && ( CurToken != T_XOR ))
            strncat( buf, " ", 255 );
        strncat( buf, Buffer, 255 );
        LastToken = CurToken;
        NextToken();
    }
    TokenLine = AsmLineNo;
    if( *buf != '\0' ) {
        AsmLine( buf );
    }
    CompFlags.pre_processing = 0;
}

void AsmStmt( void )
/******************/
{
    int                 too_many_bytes;
    auto unsigned char  buff[ MAXIMUM_BYTESEQ + 32 ];
    enum TOKEN          skip_token;

    // indicate that we are inside an __asm statement so scanner will
    // allow tokens unique to the assembler. e.g. 21h
    CompFlags.inside_asm_stmt = 1;

    NextToken();
    AsmSysInit( buff );
    too_many_bytes = 0;
    if( CurToken == T_LEFT_BRACE ) {
        NextToken();
        for(;;) {               // grab assembler lines
            GetAsmLine();
            if( AsmCodeAddress > MAXIMUM_BYTESEQ ) {
                if( ! too_many_bytes ) {
                    CErr1( ERR_TOO_MANY_BYTES_IN_PRAGMA );
                    too_many_bytes = 1;
                }
                // reset index to we don't overrun buffer
                AsmCodeAddress = 0;
            }
            if( CurToken == T_RIGHT_BRACE )
                break;
            if( CurToken == T_EOF )
                break;
            NextToken();
        }
        skip_token = T_RIGHT_BRACE;
    } else {
        GetAsmLine();           // grab single assembler instruction
        skip_token = T_NULL;
    }
    CompFlags.pre_processing = 0;
    CompFlags.inside_asm_stmt = 0;
    AsmSysMakeInlineAsmFunc( too_many_bytes );
    AsmSysFini();
    if( CurToken == skip_token ) {
        NextToken();
    }
}