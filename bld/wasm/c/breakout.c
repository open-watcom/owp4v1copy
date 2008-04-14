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


/* stuff broken out of asmins.c
 * used only by WASM not the inline assembler
 */

#include "asmglob.h"

#include "directiv.h"
#include "condasm.h"
#include "asmexpnd.h"
#include "asmlabel.h"
#include "asmstruc.h"

/* prototypes */
extern int              OrgDirective( int );
extern int              AlignDirective( uint_16, int );
extern int              ForDirective( int, enum irp_type );

int directive( int i, long direct )
/* Handle all directives */
{
    int         ret;

    /* no expansion on the following */
    switch( direct ) {
    case T_DOT_286C:
        direct = T_DOT_286;
    case T_DOT_8086:
    case T_DOT_186:
    case T_DOT_286:
    case T_DOT_286P:
    case T_DOT_386:
    case T_DOT_386P:
    case T_DOT_486:
    case T_DOT_486P:
    case T_DOT_586:
    case T_DOT_586P:
    case T_DOT_686:
    case T_DOT_686P:
    case T_DOT_8087:
    case T_DOT_287:
    case T_DOT_387:
    case T_DOT_NO87:
    case T_DOT_K3D:
    case T_DOT_MMX:
    case T_DOT_XMM:
    case T_DOT_XMM2:
    case T_DOT_XMM3:
        ret = cpu_directive(direct);
        if( Parse_Pass != PASS_1 ) ret = NOT_ERROR;
        return( ret );
    case T_DOT_DOSSEG:
    case T_DOSSEG:
        Globals.dosseg = TRUE;
        return( NOT_ERROR );
    case T_PUBLIC:
        /* special case - expanded inside iff it is an EQU to a symbol */
        return( Parse_Pass == PASS_1 ? PubDef(i+1) : NOT_ERROR );
    case T_ELSE:
    case T_ELSEIF:
    case T_ENDIF:
    case T_IF:
    case T_IF1:
    case T_IF2:
    case T_IFB:
    case T_IFDEF:
    case T_IFE:
    case T_IFNB:
    case T_IFNDEF:
    case T_IFDIF:
    case T_IFDIFI:
    case T_IFIDN:
    case T_IFIDNI:
        return( conditional_assembly_directive( i ) );
    case T_DOT_ERR:
    case T_DOT_ERRB:
    case T_DOT_ERRDEF:
    case T_DOT_ERRDIF:
    case T_DOT_ERRDIFI:
    case T_DOT_ERRE:
    case T_DOT_ERRIDN:
    case T_DOT_ERRIDNI:
    case T_DOT_ERRNB:
    case T_DOT_ERRNDEF:
    case T_DOT_ERRNZ:
        return( conditional_error_directive( i ) );
    case T_ENDS:
        if( Definition.struct_depth != 0 ) {
            return( StructDef( i-1 ) );
        }
        // else fall through to T_SEGMENT
    case T_SEGMENT:
        return( Parse_Pass == PASS_1 ? SegDef(i-1) : SetCurrSeg(i-1) );
    case T_GROUP:
        return( Parse_Pass == PASS_1 ? GrpDef(i-1) : NOT_ERROR );
    case T_PROC:
        return( ProcDef( i-1, TRUE ) );
    case T_ENDP:
        return( ProcEnd(i-1) );
    case T_DOT_CODE:
    case T_DOT_STACK:
    case T_DOT_DATA:
    case T_DOT_DATA_UN:
    case T_DOT_FARDATA:
    case T_DOT_FARDATA_UN:
    case T_DOT_CONST:
        return( SimSeg(i) );
    case T_DOT_ALPHA:
    case T_DOT_SEQ:
    case T_DOT_LIST:
    case T_DOT_LISTALL:
    case T_DOT_LISTIF:
    case T_DOT_LISTMACRO:
    case T_DOT_LISTMACROALL:
    case T_DOT_NOLIST:
    case T_DOT_XLIST:
    case T_DOT_TFCOND:
    case T_DOT_SFCOND:
    case T_DOT_LFCOND:
    case T_PAGE:
    case T_TITLE:
    case T_SUBTITLE:
    case T_SUBTTL:
    case T_DOT_CREF:
    case T_DOT_XCREF:
    case T_DOT_NOCREF:
    case T_DOT_SALL:
        AsmWarn( 4, IGNORING_DIRECTIVE );
        return( NOT_ERROR );
    case T_ECHO:
    case T_HIGH:
    case T_HIGHWORD:
    case T_LOW:
    case T_LOWWORD:
        /* these could possibly be useful */
    case T_DOT_BREAK:
    case T_DOT_CONTINUE:
    case T_DOT_ELSE:
    case T_DOT_ENDIF:
    case T_DOT_ENDW:
    case T_DOT_IF:
    case T_DOT_RADIX:
    case T_DOT_REPEAT:
    case T_DOT_UNTIL:
    case T_DOT_WHILE:
    case T_ADDR:
    case T_BOUND:
    case T_CASEMAP:
    case T_CATSTR:
    case T_INVOKE:
    case T_LROFFSET:
    case T_MASK:
    case T_OPATTR:
    case T_OPTION:
    case T_POPCONTEXT:
    case T_PROTO:
    case T_PURGE:
    case T_PUSHCONTEXT:
    case T_RECORD:
    case T_THIS:
    case T_TYPEDEF:
    case T_UNION:
    case T_WIDTH:
        AsmError( NOT_SUPPORTED );
        return( ERROR );
    case T_ORG:
        ExpandTheWorld( 0, FALSE, TRUE );
        break;
    case T_EQU2:    // =
        /* expand any constants and simplify any expressions */
//        if( Parse_Pass == PASS_1 ) {
            ExpandTheConstant( 0, FALSE, TRUE );
//        }
        break;
    case T_EQU:     // EQU
        /* expand any constants and simplify any expressions */
//        if( Parse_Pass == PASS_1 ) {
            ExpandTheConstant( 0, FALSE, TRUE );
//        }
        break;
    case T_TEXTEQU: // TEXTEQU
        /* expand any constants and simplify any expressions */
//        if( Parse_Pass == PASS_1 ) {
            ExpandTheConstant( 0, FALSE, TRUE );
//        }
        break;
    case T_NAME:
        // no expand parameters
        break;
    case T_DOT_STARTUP:
    case T_DOT_EXIT:
    default:
        /* expand any constants in all other directives */
        //if( Parse_Pass == PASS_1 ) {
            ExpandAllConsts( 0, FALSE );
        //}
        break;
    }

    switch( direct ) {
    case T_ALIAS:
        return( Parse_Pass == PASS_1 ? AddAlias( i-1 ) : NOT_ERROR );
    case T_GLOBAL:
    case T_EXTERNDEF:
        return( Parse_Pass == PASS_1 ? ExtDef( i+1, TRUE ) : NOT_ERROR );
    case T_EXTERN:
    case T_EXTRN:
        return( Parse_Pass == PASS_1 ? ExtDef( i+1, FALSE ) : NOT_ERROR );
    case T_COMM:
        return( Parse_Pass == PASS_1 ? CommDef(i+1) : NOT_ERROR );
    case T_DOT_MODEL:
        return( Model(i) );
    case T_INCLUDE:
        return( Include(i+1) );
    case T_INCLUDELIB:
        return( Parse_Pass == PASS_1 ? IncludeLib(i+1) : NOT_ERROR );
    case T_ASSUME:
        return( SetAssume(i) );
    case T_END:
        return( ModuleEnd(Token_Count) );
    case T_EQU:
        return( DefineConstant( i-1, FALSE, FALSE ) );
    case T_EQU2:
        return( DefineConstant( i-1, TRUE, FALSE ) );
    case T_TEXTEQU:
        return( DefineConstant( i-1, TRUE, TRUE ) );
    case T_MACRO:
        return( MacroDef(i-1, FALSE ) );
    case T_ENDM:
        return( MacroEnd( FALSE ) );
    case T_EXITM:
        return( MacroEnd( TRUE ) );
    case T_LOCAL:
        return( Parse_Pass == PASS_1 ? LocalDef(i) : NOT_ERROR );
    case T_COMMENT:
        return( Comment( START_COMMENT, i ) );
    case T_STRUC:
    case T_STRUCT:
        return( StructDef( i-1 ) );
    case T_NAME:
        return( Parse_Pass == PASS_1 ? NameDirective(i) : NOT_ERROR );
    case T_LABEL:
        return( LabelDirective( i ) );
    case T_ORG:
        return( OrgDirective( i ) );
    case T_ALIGN:
    case T_EVEN:
        return( AlignDirective( direct, i ) );
    case T_FOR:
    case T_IRP:
        return( ForDirective ( i+1, IRP_WORD ) );
    case T_FORC:
    case T_IRPC:
        return( ForDirective ( i+1, IRP_CHAR ) );
    case T_REPT:
    case T_REPEAT:
        return( ForDirective ( i+1, IRP_REPEAT ) );
    case T_DOT_STARTUP:
    case T_DOT_EXIT:
        return( Startup ( i ) );
    }
    AsmError( UNKNOWN_DIRECTIVE );
    return( ERROR );
}
