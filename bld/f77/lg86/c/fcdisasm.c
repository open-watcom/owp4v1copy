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
// FCDISASM     : disassemble f-codes
//

#include <stdio.h>
#include <i86.h>

#include <ftnstd.h>
#include <fcodes.h>

extern targ_addr        SProgStart;
extern ( __NEAR * __FAR FCTab[] ) ();

static unsigned_16 __FAR        *disasmPtr;
#define GETWORD( __v ) \
    __v = *disasmPtr++;
#define WORDATOFF( __o ) \
    (*(unsigned_16 __FAR *)MK_FP( FP_SEG( disasmPtr ), (__o) ))
#define DISASM_OFF() \
    FP_OFF( disasmPtr )
#define DISASM_SEG() \
    FP_SEG( disasmPtr )

static unsigned_16              argListOffset;

// Define F-Code disassembly info
//===============================

static const char __FAR * const __FAR fcNames[] = {
#include "fcnames.inc"
};

static int fcodeToEnum( unsigned_16 fc ) {
//========================================

    int         i;

    i = 0;
    while( i < LAST_FCODE ) {
        if( (short)FCTab[ i ] == fc ) {
            return( i );
        }
        ++i;
    }
    return( -1 );
}

static void doFCode( int fcode ) {
//================================

    unsigned_16                         arg1;
    unsigned_16                         arg2;
    unsigned_16                         arg3;

    switch( fcode ) {
        /* fcodes with no args */
    case RT_SET_UNIT:
    case RT_SET_REC:
    case RT_SET_IOS:
    case RT_SET_ACC:
    case RT_SET_BLNK:
    case RT_SET_FILE:
    case RT_SET_FORM:
    case RT_SET_LEN:
    case RT_SET_STAT:
    case RT_SET_DIR:
    case RT_SET_CCTRL:
    case RT_SET_FMTD:
    case RT_SET_NAME:
    case RT_SET_SEQ:
    case RT_SET_UFMTD:
    case RT_SET_EXST:
    case RT_SET_NMD:
    case RT_SET_NREC:
    case RT_SET_NUMB:
    case RT_SET_OPEN:
    case RT_SET_RECL:
    case RT_SET_NOFMT:
    case RT_SET_RECTYPE:
    case RT_SET_ACTION:
    case RT_SET_NML:
    case RT_SET_BLOCKSIZE:
    case RT_INQ_BLOCKSIZE:
    case RT_EX_READ:
    case RT_EX_WRITE:
    case RT_EX_OPEN:
    case RT_EX_CLOSE:
    case RT_EX_BACK:
    case RT_EX_ENDF:
    case RT_EX_REW:
    case RT_EX_INQ:
    case RT_OUT_LOG1:
    case RT_OUT_LOG4:
    case RT_OUT_INT1:
    case RT_OUT_INT2:
    case RT_OUT_INT4:
    case RT_OUT_REAL:
    case RT_OUT_DBLE:
    case RT_OUT_CPLX:
    case RT_OUT_DBCX:
    case RT_OUT_CHAR:
    case RT_INP_LOG1:
    case RT_INP_LOG4:
    case RT_INP_INT1:
    case RT_INP_INT2:
    case RT_INP_INT4:
    case RT_INP_REAL:
    case RT_INP_DBLE:
    case RT_INP_CPLX:
    case RT_INP_DBCX:
    case RT_INP_CHAR:
    case RT_ENDIO:
    case RT_FMT_SCAN:
    case RT_SUBSTRING:
    case RT_CHAR_SUBSCR:
    case RT_EPILOGUE:
    case RT_RTN_COUNT:
    case II_ADD:
    case RI_ADD:
    case DI_ADD:
    case CI_ADD:
    case QI_ADD:
    case IR_ADD:
    case RR_ADD:
    case DR_ADD:
    case CR_ADD:
    case QR_ADD:
    case ID_ADD:
    case RD_ADD:
    case DD_ADD:
    case CD_ADD:
    case QD_ADD:
    case IC_ADD:
    case RC_ADD:
    case DC_ADD:
    case CC_ADD:
    case QC_ADD:
    case IQ_ADD:
    case RQ_ADD:
    case DQ_ADD:
    case CQ_ADD:
    case QQ_ADD:
    case II_SUB:
    case RI_SUB:
    case DI_SUB:
    case CI_SUB:
    case QI_SUB:
    case IR_SUB:
    case RR_SUB:
    case DR_SUB:
    case CR_SUB:
    case QR_SUB:
    case ID_SUB:
    case RD_SUB:
    case DD_SUB:
    case CD_SUB:
    case QD_SUB:
    case IC_SUB:
    case RC_SUB:
    case DC_SUB:
    case CC_SUB:
    case QC_SUB:
    case IQ_SUB:
    case RQ_SUB:
    case DQ_SUB:
    case CQ_SUB:
    case QQ_SUB:
    case II_MUL:
    case RI_MUL:
    case DI_MUL:
    case CI_MUL:
    case QI_MUL:
    case IR_MUL:
    case RR_MUL:
    case DR_MUL:
    case CR_MUL:
    case QR_MUL:
    case ID_MUL:
    case RD_MUL:
    case DD_MUL:
    case CD_MUL:
    case QD_MUL:
    case IC_MUL:
    case RC_MUL:
    case DC_MUL:
    case CC_MUL:
    case QC_MUL:
    case IQ_MUL:
    case RQ_MUL:
    case DQ_MUL:
    case CQ_MUL:
    case QQ_MUL:
    case II_DIV:
    case RI_DIV:
    case DI_DIV:
    case CI_DIV:
    case QI_DIV:
    case IR_DIV:
    case RR_DIV:
    case DR_DIV:
    case CR_DIV:
    case QR_DIV:
    case ID_DIV:
    case RD_DIV:
    case DD_DIV:
    case CD_DIV:
    case QD_DIV:
    case IC_DIV:
    case RC_DIV:
    case DC_DIV:
    case CC_DIV:
    case QC_DIV:
    case IQ_DIV:
    case RQ_DIV:
    case DQ_DIV:
    case CQ_DIV:
    case QQ_DIV:
    case II_EXP:
    case RI_EXP:
    case DI_EXP:
    case CI_EXP:
    case QI_EXP:
    case IR_EXP:
    case RR_EXP:
    case DR_EXP:
    case CR_EXP:
    case QR_EXP:
    case ID_EXP:
    case RD_EXP:
    case DD_EXP:
    case CD_EXP:
    case QD_EXP:
    case IC_EXP:
    case RC_EXP:
    case DC_EXP:
    case CC_EXP:
    case QC_EXP:
    case IQ_EXP:
    case RQ_EXP:
    case DQ_EXP:
    case CQ_EXP:
    case QQ_EXP:
    case II_CNV:
    case RI_CNV:
    case DI_CNV:
    case CI_CNV:
    case QI_CNV:
    case IR_CNV:
    case RR_CNV:
    case DR_CNV:
    case CR_CNV:
    case QR_CNV:
    case ID_CNV:
    case RD_CNV:
    case DD_CNV:
    case CD_CNV:
    case QD_CNV:
    case IC_CNV:
    case RC_CNV:
    case DC_CNV:
    case CC_CNV:
    case QC_CNV:
    case IQ_CNV:
    case RQ_CNV:
    case DQ_CNV:
    case CQ_CNV:
    case QQ_CNV:
    case NEGI:
    case NEGR:
    case NEGD:
    case NEGC:
    case NEGQ:
    case II_CMP:
    case RI_CMP:
    case DI_CMP:
    case CI_CMP:
    case QI_CMP:
    case IR_CMP:
    case RR_CMP:
    case DR_CMP:
    case CR_CMP:
    case QR_CMP:
    case ID_CMP:
    case RD_CMP:
    case DD_CMP:
    case CD_CMP:
    case QD_CMP:
    case IC_CMP:
    case RC_CMP:
    case DC_CMP:
    case CC_CMP:
    case QC_CMP:
    case IQ_CMP:
    case RQ_CMP:
    case DQ_CMP:
    case CQ_CMP:
    case QQ_CMP:
    case CHAR_CMP:
    case EQ2LOGIC:
    case NE2LOGIC:
    case LT2LOGIC:
    case GE2LOGIC:
    case LE2LOGIC:
    case GT2LOGIC:
    case FLIP_EQ2LOGIC:
    case FLIP_NE2LOGIC:
    case FLIP_LT2LOGIC:
    case FLIP_GE2LOGIC:
    case FLIP_LE2LOGIC:
    case FLIP_GT2LOGIC:
    case EQV:
    case NEQV:
    case OR:
    case AND:
    case NOT:
    case VAL_LOG1:
    case VAL_LOG4:
    case VAL_INT1:
    case VAL_INT2:
    case VAL_INT4:
    case VAL_REAL4:
    case VAL_REAL8:
    case VAL_CPLX8:
    case VAL_CPLX16:
    case STOR_LOG1:
    case STOR_LOG4:
    case STOR_INT1:
    case STOR_INT2:
    case STOR_INT4:
    case STOR_REAL4:
    case STOR_REAL8:
    case STOR_CPLX8:
    case STOR_CPLX16:
    case ASGN_GOTO_WITH_LIST:
    case POP_ARR_ELT_PARM:
    case END_WARP:
    case PUSH_SCB_LEN:
    case PUSH_ARRAY_DATA:
    case II_FLIP:
    case RI_FLIP:
    case DI_FLIP:
    case CI_FLIP:
    case QI_FLIP:
    case IR_FLIP:
    case RR_FLIP:
    case DR_FLIP:
    case CR_FLIP:
    case QR_FLIP:
    case ID_FLIP:
    case RD_FLIP:
    case DD_FLIP:
    case CD_FLIP:
    case QD_FLIP:
    case IC_FLIP:
    case RC_FLIP:
    case DC_FLIP:
    case CC_FLIP:
    case QC_FLIP:
    case IQ_FLIP:
    case RQ_FLIP:
    case DQ_FLIP:
    case CQ_FLIP:
    case QQ_FLIP:
    case RT_END_DATA_VARS:
    case RT_ISN_DBUG:
    case RT_END_DBUG:
    case RT_DB_PROLOGUE:
    case RT_DB_EPILOGUE:
    case UVAL_LOG1:
    case UVAL_LOG4:
    case UVAL_INT1:
    case UVAL_INT2:
    case UVAL_INT4:
    case UVAL_REAL4:
    case UVAL_REAL8:
    case UVAL_CPLX8:
    case UVAL_CPLX16:
    case UCHAR_CMP:
    case DB_SFEPI:
    case LOC_FROM_ADDR:
    case LOC_FROM_SCB:
    case BITEQV:
    case BITNEQV:
    case BITOR:
    case BITAND:
    case BITNOT:
        printf( "\n" );
        break;

        /* fcodes with one argument */
    case RT_PAUSE:
    case RT_STOP:
    case RT_SET_IOCB:
    case RT_SET_FMT:
    case RT_SET_ERR:
    case RT_SET_END:
    case RT_FMT_ARR_SCAN:
    case RT_CAT:
    case RT_SUBSCRIPT:
    case RT_ADV_CONST:
    case RT_ADV_VAR:
    case RT_ADV_ASSUME:
    case RT_SET_LINE:
    case PUSH_LOG1:
    case PUSH_LOG4:
    case PUSH_INT1:
    case PUSH_INT2:
    case PUSH_INT4:
    case PUSH_REAL4:
    case PUSH_REAL8:
    case PUSH_CPLX8:
    case PUSH_CPLX16:
    case PUSH_SCB:
    case PUSH_IND_LOG1:
    case PUSH_IND_LOG4:
    case PUSH_IND_INT1:
    case PUSH_IND_INT2:
    case PUSH_IND_INT4:
    case PUSH_IND_REAL4:
    case PUSH_IND_REAL8:
    case PUSH_IND_CPLX8:
    case PUSH_IND_CPLX16:
    case PUSH_IND_ADDR:
    case PUSH_ADDR:
    case PUSH_INLINE:
    case POP_LOG1:
    case POP_LOG4:
    case POP_INT1:
    case POP_INT2:
    case POP_INT4:
    case POP_REAL4:
    case POP_REAL8:
    case POP_CPLX8:
    case POP_CPLX16:
    case POP_SCB:
    case POP_IND_LOG1:
    case POP_IND_LOG4:
    case POP_IND_INT1:
    case POP_IND_INT2:
    case POP_IND_INT4:
    case POP_IND_REAL4:
    case POP_IND_REAL8:
    case POP_IND_CPLX8:
    case POP_IND_CPLX16:
    case BC_FALSE:
    case BC_TRUE:
    case I1DO_LOOP:
    case I2DO_LOOP:
    case I4DO_LOOP:
    case R4DO_LOOP:
    case R8DO_LOOP:
    case EXEC_RB:
    case END_RB:
    case ASGN_GOTO:
    case SF_CALL:
    case SF_RETURN:
    case S_L1_FILL_IN:
    case S_L2_FILL_IN:
    case S_L3_FILL_IN:
    case S_L4_FILL_IN:
    case S_L5_FILL_IN:
    case S_L6_FILL_IN:
    case S_L7_FILL_IN:
    case S_H1_FILL_IN:
    case S_H2_FILL_IN:
    case S_H3_FILL_IN:
    case S_H4_FILL_IN:
    case S_H5_FILL_IN:
    case S_H6_FILL_IN:
    case S_H7_FILL_IN:
    case GO_WARP:
    case UPSH_LOG1:
    case UPSH_LOG4:
    case UPSH_INT1:
    case UPSH_INT2:
    case UPSH_INT4:
    case UPSH_REAL4:
    case UPSH_REAL8:
    case UPSH_CPLX8:
    case UPSH_CPLX16:
    case UPSH_IND_LOG1:
    case UPSH_IND_LOG4:
    case UPSH_IND_INT1:
    case UPSH_IND_INT2:
    case UPSH_IND_INT4:
    case UPSH_IND_REAL:
    case UPSH_IND_REAL8:
    case UPSH_IND_CPLX8:
    case UPSH_IND_CPLX16:
    case RT_USUBSCRIPT:
    case RT_UCAT:
    case DB_SFPRO:
    case RT_PSUBSCRIPT:
    case PUSH_PARM_SCB:
    case CHK_VALID_PARM:
    case RT_P1_SUBSCR:
    case RT_U1_SUBSCR:
    case RT_1_SUBSCR:
    case ADD_L_I4:
    case SUB_L_I4:
    case MUL_L_I4:
    case DIV_L_I4:
    case BC_EQ:
    case BC_NE:
    case BC_GT:
    case BC_GE:
    case BC_LT:
    case BC_LE:
    case CHK_RET_VAL:
    case ASSIGN:
    case ADD_L_R8:
    case SUB_L_R8:
    case MUL_L_R8:
    case DIV_L_R8:
    case ADD_I_R8:
    case SUB_I_R8:
    case MUL_I_R8:
    case DIV_I_R8:
    case ADD_L_R4:
    case SUB_L_R4:
    case MUL_L_R4:
    case DIV_L_R4:
    case SAVERETIDX:
    case PH_U1_SUBSCR:
    case SIMPLE_OUT_I4:
    case SIMPLE_OUT_R4:
    case SIMPLE_OUT_R8:
    case ADD_UL_I4:
    case SUB_UL_I4:
    case MUL_UL_I4:
    case DIV_UL_I4:
    case ADD_UL_R4:
    case SUB_UL_R4:
    case MUL_UL_R4:
    case DIV_UL_R4:
    case ADD_UL_R8:
    case SUB_UL_R8:
    case MUL_UL_R8:
    case DIV_UL_R8:
    case SIMPLE_INP_I4:
    case SIMPLE_INP_R4:
    case SIMPLE_INP_R8:
    case FLUSH_ENTRIES:
    case DUMP_ENTRIES:
    case LOAD_ENTRIES:
    case ALLOCATED:
    case PUSH_PGM:
        GETWORD( arg1 );
        printf( " %04x\n", arg1 );
        break;

        /* fcodes with 2 arguments */
    case RT_ALLOCATE:
    case RT_DEALLOCATE:
    case RT_FMT_ASSIGN:
    case I1DO_INIT:
    case I2DO_INIT:
    case I4DO_INIT:
    case R4DO_INIT:
    case R8DO_INIT:
    case MOVE_LL_I4:
    case MOVE_LL_R8:
    case MOVE_IL_R8:
    case MOVE_LL_R4:
    case PH_1_SUBSCR:
    case PH_P1_SUBSCR:
    case MOVE_ULL_I4:
    case MOVE_ULL_R4:
    case MOVE_ULL_R8:
    case IO_ARRAY:
    case PRT_CARRAY:
    case INP_CARRAY:
    case NEXT_ARR_ELT:
    case NEXT_PRT_CARR:
    case NEXT_INP_CARR:
        GETWORD( arg1 );
        GETWORD( arg2 );
        printf( " %04x %04x\n", arg1, arg2 );
        break;

        /* fcodes with 3 args */
    case THREE_WAY_BRANCH:
        GETWORD( arg1 );
        GETWORD( arg2 );
        GETWORD( arg3 );
        printf( " %04x %04x %04x\n", arg1, arg2, arg3 );
        break;

        /* fcodes with variable # of args */
    case I2_SELECT_SINGLES:
    case I2_SELECT_LOW_HIGH:
    case I4_SELECT_SINGLES:
    case I4_SELECT_LOW_HIGH:
    case ALT_RET:
        {
            unsigned_16                 num_args;
            uint                        u;

            GETWORD( num_args );
            u = 0;
            while( u < num_args ) {
                GETWORD( arg1 );
                printf( " %04x", arg1 );
                ++u;
            }
            printf( "\n" );
        }
        break;

        /* special cases */
    case RT_PROLOGUE:
        {
            unsigned_16                         calling;
            unsigned_16                         arg_info;
            unsigned_16                         ainfo;
            unsigned_16                         aoffset;
            uint                                u;

            GETWORD( calling );
            GETWORD( arg_info );
            printf( " calling %04x(%04x:%04x) args %04x",
                calling, WORDATOFF( calling+2 ), WORDATOFF( calling ),
                arg_info );
            u = 0;
            while( u < ( arg_info >> 8 ) ) {
                GETWORD( ainfo );
                GETWORD( aoffset );
                printf( " %04x/%04x", ainfo, aoffset );
                ++u;
            }
            printf( "\n" );
        }
        break;

    case BC_ALWAYS:
        GETWORD( arg1 );
        printf( " %04x\n", arg1 );
        if( WORDATOFF( arg1 ) == (unsigned)FCTab[ RT_SET_FMT ]
            && WORDATOFF( arg1+2 ) == DISASM_OFF() ) {
            /* aha! a format directive! */
            printf( "%04x: FORMAT:", DISASM_OFF() );
            while( DISASM_OFF() < arg1 ) {
                GETWORD( arg2 );
                printf( " %02x %02", arg2 & 0xff, arg2 >> 8 );
            }
            printf( "\n" );
        }
        break;

    case RT_SET_INTL:
        GETWORD( arg1 );
        if( arg1 == 0 ) {
            GETWORD( arg2 );
            GETWORD( arg3 );
            printf( " %04x %04x %04x\n", arg1, arg2, arg3 );
        } else {
            printf( " %04x\n", arg1 );
        }
        break;
    }
}

static void doEpilog( void ) {
//============================

    unsigned_16                 tmp;
    unsigned_16                 tmp2;
    unsigned_16                 ret_arg_count;
    uint                        u;

    GETWORD( tmp );
    printf( "%04x: offset of current arg list: %04x\n",
        DISASM_OFF() - 2, tmp );
    GETWORD( tmp );
    GETWORD( tmp2 );
    printf( "%04x: return address:             %04x:%04x\n",
        DISASM_OFF() - 4, tmp2, tmp );
    if( DISASM_OFF() < argListOffset ) {
        printf( "%04x: function value:", DISASM_OFF() );
        while( DISASM_OFF() < argListOffset ) {
            GETWORD( tmp );
            printf( " %04x", tmp );
        }
        printf( "\n" );
    }
    GETWORD( ret_arg_count );
    printf( "%04x: return type/arg count:      %04x\n",
        DISASM_OFF() - 2, ret_arg_count );
    u = 0;
    while( u < ( ret_arg_count >> 8 ) ) {
        GETWORD( tmp );
        GETWORD( tmp2 );
        printf( "%04x: arg %u: info:%04x offs:%04x\n",
            DISASM_OFF() - 4, u, tmp, tmp2 );
        ++u;
    }
}


static int doHeader( void ) {
//===========================

    unsigned_16         tmp;

    printf( "%04x: subprogram header: (segment %04x)\n",
        DISASM_OFF() - 2, DISASM_SEG() );
    GETWORD( tmp );     /* skip over call far Ext_to_F77 */
    GETWORD( tmp );
    GETWORD( argListOffset );
    printf( "%04x: offset of arg list info:    %04x\n",
        DISASM_OFF() - 2, argListOffset );
    GETWORD( tmp );
    printf( "%04x: offset of epilogue fcode:   %04x\n",
        DISASM_OFF() - 2, tmp );
    GETWORD( tmp );
    printf( "%04x: offset to traceback struct: %04x\n",
        DISASM_OFF() - 2, tmp );
    GETWORD( tmp );
    printf( "%04x: offset to entry name:       %04x (%Fs)\n",
        DISASM_OFF() - 2, tmp, MK_FP( DISASM_SEG(), tmp ) );
    return( 0 );
}

void FCDisasm( unsigned_16 seg, unsigned_16 start ) {
//===================================================
    FCDisRange( seg, start, 0xffff );
}

void FCDisRange( unsigned_16 seg, unsigned_16 start, unsigned_16 end ) {
//======================================================================

    unsigned_16                         fcode_code_label;
    int                                 fcode;
    int                                 stop_at_epilog;

    disasmPtr = (unsigned_16 __FAR *)MK_FP( seg, start );
    stop_at_epilog = 0;
    for(;;) {
        GETWORD( fcode_code_label );
        if( fcode_code_label == 0x9a2e ) {
            doHeader();
            stop_at_epilog = 1;
            continue;
        }
        fcode = fcodeToEnum( fcode_code_label );
        if( fcode == -1 ) {
            printf( "%04x: (%04x)UNKNOWN OPCODE\n",
                DISASM_OFF() - 2, fcode_code_label );
        } else {
            printf( "%04x: (%04x)%Fs",
                DISASM_OFF() - 2, fcode_code_label, fcNames[ fcode ] );
            doFCode( fcode );
            if( fcode == RT_EPILOGUE && stop_at_epilog ) {
                doEpilog();
                break;
            }
        }
        if( DISASM_OFF() >= end ) {
            break;
        }
    }
    fflush( stdout );
}
