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
* Description:  CALL/JMP processing
*
****************************************************************************/


#include "asmglob.h"

#include "asmfixup.h"
#include "asmeval.h"

#if defined( _STANDALONE_ )
  #include "directiv.h"
  #include "asminput.h"
#endif

/* prototypes */
int ptr_operator( memtype mem_type, uint_8 fix_mem_type );
int jmp( expr_list *opndx );

#if defined( _STANDALONE_ )

extern void             GetInsString( enum asm_token, char *, int );
extern int              SymIs32( struct asm_sym *sym );
extern void             check_assume( struct asm_sym *sym, enum prefix_reg default_reg );
extern void             find_frame( struct asm_sym *sym );

static enum asm_token getJumpNegation( enum asm_token instruction )
/*****************************************************************/
{
    switch( instruction ) {
    case T_JA:          return( T_JNA );
    case T_JAE:         return( T_JNAE );
    case T_JB:          return( T_JNB );
    case T_JBE:         return( T_JNBE );
    case T_JC:          return( T_JNC );
    case T_JE:          return( T_JNE );
    case T_JG:          return( T_JNG );
    case T_JGE:         return( T_JNGE );
    case T_JL:          return( T_JNL );
    case T_JLE:         return( T_JNLE );
    case T_JNA:         return( T_JA );
    case T_JNAE:        return( T_JAE );
    case T_JNB:         return( T_JB );
    case T_JNBE:        return( T_JBE );
    case T_JNC:         return( T_JC );
    case T_JNE:         return( T_JE );
    case T_JNG:         return( T_JG );
    case T_JNGE:        return( T_JGE );
    case T_JNL:         return( T_JL );
    case T_JNLE:        return( T_JLE );
    case T_JNO:         return( T_JO );
    case T_JNP:         return( T_JP );
    case T_JNS:         return( T_JS );
    case T_JNZ:         return( T_JZ );
    case T_JO:          return( T_JNO );
    case T_JP:          return( T_JNP );
    case T_JPE:         return( T_JPO );
    case T_JPO:         return( T_JPE );
    case T_JS:          return( T_JNS );
    case T_JZ:          return( T_JNZ );
    default:
        return( (enum asm_token)ERROR );
    }
}

static void jumpExtend( int far_flag )
/*************************************/
{
    unsigned i;
    unsigned next_ins_size;
    enum asm_token negation;
    char buffer[MAX_LINE_LEN];

    /* there MUST be a conditional jump instruction in asmbuffer */
    for( i = 0; ; i++ ) {
        if( ( AsmBuffer[i]->token == T_INSTR )
            && IS_JMP( AsmBuffer[i]->u.value ) ) {
            break;
        }
    }

    AsmWarn( 4, EXTENDING_JUMP );

    negation = getJumpNegation( AsmBuffer[i]->u.value );
    GetInsString( negation, buffer, MAX_LINE_LEN );
    if( far_flag ) {
        next_ins_size = Code->use32 ? 7 : 5;
    } else {
        next_ins_size = Code->use32 ? 5 : 3;
    }
    sprintf( buffer + strlen( buffer ), " SHORT $+%d ", next_ins_size+2 );
    InputQueueLine( buffer );
    if( far_flag ) {
        strcpy( buffer, "jmpf " );
    } else {
        strcpy( buffer, "jmp " );
    }
    for( i++; AsmBuffer[i]->token != T_FINAL; i++ ) {
        switch( AsmBuffer[i]->token ) {
        case T_NUM:
        case T_DEC_NUM:
        case T_OCT_NUM:
        case T_HEX_NUM_0:
        case T_HEX_NUM:
        case T_BIN_NUM:
            itoa( AsmBuffer[i]->u.value, buffer + strlen( buffer ), 10 );
            break;
        case T_OP_SQ_BRACKET:
            strcat( buffer, "[" );
            break;
        case T_CL_SQ_BRACKET:
            strcat( buffer, "]" );
            break;
        default:
            strcat( buffer, AsmBuffer[i]->string_ptr );
            break;
        }
    }
    InputQueueLine( buffer );
    return;
}

static void FarCallToNear( void )
/*******************************/
{
    unsigned i;
    char buffer[MAX_LINE_LEN];

    /* there MUST be a call instruction in asmbuffer */
    for( i = 0; ; i++ ) {
        if( ( AsmBuffer[i]->token == T_INSTR )
            && ( AsmBuffer[i]->u.value == T_CALL ) ) {
            break;
        }
    }
    if( Parse_Pass == PASS_2 )
        AsmWarn( 4, CALL_FAR_TO_NEAR );
    InputQueueLine( "PUSH CS" );
#if defined( _STANDALONE_ )
    if( Options.mode & MODE_IDEAL ) {
        strcpy( buffer, "CALL NEAR " );
    } else {
        strcpy( buffer, "CALL NEAR PTR " );
    }
#else
    strcpy( buffer, "CALL NEAR PTR " );
#endif
    for( i++; AsmBuffer[i]->token != T_FINAL; i++ ) {
        switch( AsmBuffer[i]->token ) {
        case T_NUM:
        case T_DEC_NUM:
        case T_OCT_NUM:
        case T_HEX_NUM_0:
        case T_HEX_NUM:
        case T_BIN_NUM:
            itoa( AsmBuffer[i]->u.value, buffer+strlen( buffer ), 10 );
            break;
        case T_OP_SQ_BRACKET:
            strcat( buffer, "[" );
            break;
        case T_CL_SQ_BRACKET:
            strcat( buffer, "]" );
            break;
        default:
            strcat( buffer, AsmBuffer[i]->string_ptr );
            break;
        }
    }
    InputQueueLine( buffer );
    return;
}
#endif

int jmp( expr_list *opndx )
/*
  determine the displacement of jmp;
*/
{
    int_32              addr;
    enum fixup_types    fixup_type;
    enum fixup_options  fixup_option;
    enum sym_state      state;
    struct asm_sym      *sym;
#if defined( _STANDALONE_ )
    dir_node            *seg;
#endif

    Code->data[Opnd_Count] = opndx->value;
    sym = opndx->sym;
    if( sym == NULL ) {
        if( IS_JMPCALLN( Code->info.token ) )
            Code->info.token++;
        if( Code->data[Opnd_Count] > USHRT_MAX )
            Code->info.opnd_type[Opnd_Count] = OP_I32;
        else
            Code->info.opnd_type[Opnd_Count] = OP_I16;
        return( NOT_ERROR );
    }

#if defined( _STANDALONE_ )
    if( sym->mem_type == MT_ERROR ) {
        AsmError( LABEL_NOT_DEFINED );
        return( ERROR );
    }
#endif
    state = sym->state;
#if defined( _STANDALONE_ )
    seg = GetSeg( sym );
    if( seg == NULL || CurrSeg == NULL || CurrSeg->seg != seg ) {
        /* jumps to another segment are just like to another file */
        state = SYM_EXTERNAL;
    }
#endif

    if( !Code->mem_type_fixed ) {
        Code->mem_type = MT_EMPTY;
    }
    fixup_option = OPTJ_NONE;
    fixup_type = FIX_RELOFF8;
    switch( state ) {
    case SYM_INTERNAL:
#if defined( _STANDALONE_ )
    case SYM_PROC:
#endif
        if(  ( Code->mem_type == MT_EMPTY || Code->mem_type == MT_SHORT
                || Code->mem_type == MT_NEAR )
            && sym->mem_type != MT_WORD
            && sym->mem_type != MT_DWORD
            && sym->mem_type != MT_FWORD
            && !IS_JMPCALLF( Code->info.token ) ) {
#if defined( _STANDALONE_ )
            if( ( Code->info.token == T_CALL )
                && ( Code->mem_type == MT_EMPTY )
                && ( sym->mem_type == MT_FAR ) ) {
                FarCallToNear();
                return( SCRAP_INSTRUCTION );
            }
            addr = sym->offset;
#else
            addr = sym->addr;
#endif
            addr -= ( AsmCodeAddress + 2 );  // calculate the displacement
            addr += Code->data[Opnd_Count];
            switch( Code->info.token ) {
            case T_JCXZ:
            case T_LOOPW:
            case T_LOOPEW:
            case T_LOOPZW:
            case T_LOOPNEW:
            case T_LOOPNZW:
                if( Code->use32 ) {
                    // 1 extra byte for OPNSIZ
                    addr--;
                }
                break;
            case T_JECXZ:
            case T_LOOPD:
            case T_LOOPED:
            case T_LOOPZD:
            case T_LOOPNED:
            case T_LOOPNZD:
                if( !Code->use32 ) {
                    // 1 extra byte for OPNSIZ
                    addr--;
                }
                break;
            }
            if( Code->info.token == T_CALL && Code->mem_type == MT_EMPTY ) {
                Code->mem_type = MT_NEAR;
            }
            if( Code->mem_type != MT_NEAR && Code->info.token != T_CALL
                && ( addr >= SCHAR_MIN && addr <= SCHAR_MAX ) ) {
                Code->info.opnd_type[Opnd_Count] = OP_I8;
            } else {
                /* near jmp */
                if( Code->use32 ) {
                    Code->info.opnd_type[Opnd_Count] = OP_I32;
                    addr -= 3; // 32 bit displacement
                } else {
                    Code->info.opnd_type[Opnd_Count] = OP_I16;
                    addr -= 1; // 16 bit displacement
                }
                if( IS_JMP( Code->info.token ) ) {
                    switch( Code->info.token ) {
                    case T_JMP:
                    case T_JMPF:
                    case T_JCXZ:
                    case T_JECXZ:
                        break;
                    default:
                        // 1 extra byte for opcode ( 0F )
                        addr--;
                        break;
                    }
                }
            }

            /* store the displacement */
            Code->data[Opnd_Count] = addr;

            switch( Code->info.token ) {
            case T_JCXZ:
            case T_JECXZ:
            case T_LOOP:
            case T_LOOPE:
            case T_LOOPNE:
            case T_LOOPNZ:
            case T_LOOPZ:
            case T_LOOPD:
            case T_LOOPED:
            case T_LOOPNED:
            case T_LOOPNZD:
            case T_LOOPZD:
            case T_LOOPW:
            case T_LOOPEW:
            case T_LOOPNEW:
            case T_LOOPNZW:
            case T_LOOPZW:
#if defined( _STANDALONE_ )
    #define GOOD_PHASE  !PhaseError &&
#else
    #define GOOD_PHASE
#endif
                if( GOOD_PHASE (Code->info.opnd_type[Opnd_Count] != OP_I8) ) {
                    AsmError( JUMP_OUT_OF_RANGE );
                    return( ERROR );
                }
                Code->info.opnd_type[Opnd_Count] = OP_I8;
                break;
            }

            if( (Code->info.cpu&P_CPU_MASK) < P_386 && IS_JMP( Code->info.token ) ) {
                /* look into jump extension */
                switch( Code->info.token ) {
                case T_JMP:
                case T_JMPF:
                    break;
                default:
                    if( Code->info.opnd_type[Opnd_Count] != OP_I8 ) {
#if defined( _STANDALONE_ )
                        if( Code->mem_type == MT_EMPTY ) {
                            jumpExtend( 0 );
                            return( SCRAP_INSTRUCTION );
                        } else if( !PhaseError ) {
                            AsmError( JUMP_OUT_OF_RANGE );
                            return( ERROR );
                        }
#else
                        AsmError( JUMP_OUT_OF_RANGE );
                        return( ERROR );
#endif
                    }
                }
            }
            break;
        }
        /* otherwise fall through & get handled like external symbols */
    case SYM_UNDEFINED:
    case SYM_EXTERNAL:

        /* forward ref, or external symbol */
        if( Code->mem_type == MT_EMPTY && sym->mem_type != MT_EMPTY ) {
            switch( sym->mem_type ) {
            case MT_FAR:
                if( IS_JMPCALLN( Code->info.token ) ) {
                    Code->info.token++;
                }
                // fall through
            case MT_SHORT:
            case MT_NEAR:
                Code->mem_type = sym->mem_type;
                break;
#if defined( _STANDALONE_ )
            case MT_PROC:
                Code->mem_type = IS_PROC_FAR() ? MT_FAR : MT_NEAR;
                if( IS_JMPCALLN( Code->info.token )
                    && ( Code->mem_type == MT_FAR ) ) {
                    Code->info.token++;
                }
                break;
#endif
            case MT_FWORD:
                if( ptr_operator( MT_FWORD, TRUE ) == ERROR )
                    return( ERROR );
                break;
            default:
                Code->mem_type = sym->mem_type;
            }
        }
        if( ( Code->mem_type == MT_FAR ) && IS_JMPCALLN( Code->info.token ) ) {
            Code->info.token++;
        }
        switch( Code->info.token ) {
        case T_CALLF:
        case T_JMPF:
            switch( Code->mem_type ) {
            case MT_SHORT:
            case MT_NEAR:
                if( Opnd_Count == OPND1 && Code->mem_type_fixed ) {
                    AsmError( CANNOT_USE_SHORT_OR_NEAR );
                    return( ERROR );
                }
                /* fall through */
            case MT_FAR:
            case MT_EMPTY:
#if defined( _STANDALONE_ )
                SET_OPSIZ( Code, SymIs32( sym ));
                find_frame( sym );
#endif
                if( Opnd_Count == OPND2 ) {
                    if( oper_32( Code ) ) {
                        fixup_type = FIX_OFF32;
                        Code->info.opnd_type[Opnd_Count] = OP_I32;
                    } else {
                        fixup_type = FIX_OFF16;
                        Code->info.opnd_type[Opnd_Count] = OP_I16;
                    }
                } else {
                    if( oper_32( Code ) ) {
                        fixup_type = FIX_PTR32;
                        Code->info.opnd_type[Opnd_Count] = OP_J48;
                    } else {
                        fixup_type = FIX_PTR16;
                        Code->info.opnd_type[Opnd_Count] = OP_J32;
                    }
                }
                break;
            case MT_BYTE:
            case MT_WORD:
#if defined( _STANDALONE_ )
            case MT_SBYTE:
            case MT_SWORD:
#endif
                AsmError( INVALID_SIZE );
                return( ERROR );
            case MT_DWORD:
            case MT_FWORD:
#if defined( _STANDALONE_ )
            case MT_SDWORD:
#endif
                return( INDIRECT_JUMP );
            case MT_QWORD:
            case MT_TBYTE:
            case MT_OWORD:
                AsmError( INVALID_SIZE );
                return( ERROR );
            }
            break;
        case T_CALL:
            if( Code->mem_type == MT_SHORT ) {
                AsmError( CANNOT_USE_SHORT_WITH_CALL );
                return( ERROR );
            } else if( Code->mem_type == MT_EMPTY ) {
#if defined( _STANDALONE_ )
                fixup_option = OPTJ_CALL;
#else
                fixup_option = OPTJ_NONE;
#endif
                if( Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    Code->info.opnd_type[Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    Code->info.opnd_type[Opnd_Count] = OP_I16;
                }
                break;
            }
            /* fall through */
        case T_JMP:
            switch( Code->mem_type ) {
            case MT_SHORT:
                fixup_option = OPTJ_EXPLICIT;
                fixup_type = FIX_RELOFF8;
                Code->info.opnd_type[Opnd_Count] = OP_I8;
                break;
            case MT_FAR:
                AsmError( SYNTAX_ERROR );
                break;
            case MT_EMPTY:
                // forward reference
                // inline assembler jmp default distance is near
                // stand-alone assembler jmp default distance is short
                fixup_option = OPTJ_NONE;
#if defined( _STANDALONE_ )
                /* guess short if JMP, we will expand later if needed */
                fixup_type = FIX_RELOFF8;
                Code->info.opnd_type[Opnd_Count] = OP_I8;
#else
                if( Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    Code->info.opnd_type[Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    Code->info.opnd_type[Opnd_Count] = OP_I16;
                }
#endif
                break;
            case MT_NEAR:
                fixup_option = OPTJ_EXPLICIT;
                if( Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    Code->info.opnd_type[Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    Code->info.opnd_type[Opnd_Count] = OP_I16;
                }
                break;
            case MT_DWORD:
            case MT_WORD:
#if defined( _STANDALONE_ )
            case MT_SDWORD:
            case MT_SWORD:
#endif
                return( INDIRECT_JUMP );
#if defined( _STANDALONE_ )
            case MT_SBYTE:
#endif
            case MT_BYTE:
            case MT_FWORD:
            case MT_QWORD:
            case MT_TBYTE:
            case MT_OWORD:
                AsmError( INVALID_SIZE );
                return( ERROR );
            }
//            check_assume( sym, EMPTY );
            break;
        case T_JCXZ:
        case T_JECXZ:
            // JCXZ and JECXZ always require SHORT label
        case T_LOOP:
        case T_LOOPE:
        case T_LOOPNE:
        case T_LOOPNZ:
        case T_LOOPZ:
        case T_LOOPD:
        case T_LOOPED:
        case T_LOOPNED:
        case T_LOOPNZD:
        case T_LOOPZD:
        case T_LOOPW:
        case T_LOOPEW:
        case T_LOOPNEW:
        case T_LOOPNZW:
        case T_LOOPZW:
#if defined( _STANDALONE_ )
            if( ( Code->mem_type != MT_EMPTY ) &&
                ( Code->mem_type != MT_SHORT ) &&
                ( (Options.mode & MODE_IDEAL) == 0 ) ) {
#else
            if( ( Code->mem_type != MT_EMPTY ) &&
                ( Code->mem_type != MT_SHORT ) ) {
#endif
                AsmError( ONLY_SHORT_DISPLACEMENT_IS_ALLOWED );
                return( ERROR );
            }
            Code->info.opnd_type[Opnd_Count] = OP_I8;
            fixup_option = OPTJ_EXPLICIT;
            fixup_type = FIX_RELOFF8;
            break;
        default:
            if( (Code->info.cpu&P_CPU_MASK) >= P_386 ) {
                switch( Code->mem_type ) {
                case MT_SHORT:
                    fixup_option = OPTJ_EXPLICIT;
                    fixup_type = FIX_RELOFF8;
                    Code->info.opnd_type[Opnd_Count] = OP_I8;
                    break;
                case MT_EMPTY:
                    // forward reference
                    // inline assembler default distance is near
                    // stand-alone assembler default distance is short
#if defined( _STANDALONE_ )
                    fixup_option = OPTJ_JXX;
                    fixup_type = FIX_RELOFF8;
                    Code->info.opnd_type[Opnd_Count] = OP_I8;
                    break;
#endif
                case MT_NEAR:
                    fixup_option = OPTJ_EXPLICIT;
                    if( Code->use32 ) {
                        fixup_type = FIX_RELOFF32;
                        Code->info.opnd_type[Opnd_Count] = OP_I32;
                    } else {
                        fixup_type = FIX_RELOFF16;
                        Code->info.opnd_type[Opnd_Count] = OP_I16;
                    }
                    break;
                case MT_FAR:
#if defined( _STANDALONE_ )
                    jumpExtend( 1 );
                    return( SCRAP_INSTRUCTION );
#endif
                default:
                    AsmError( ONLY_SHORT_AND_NEAR_DISPLACEMENT_IS_ALLOWED );
                    return( ERROR );
                }
            } else {
                // the only mode in 8086, 80186, 80286 is
                // Jxx SHORT
                switch( Code->mem_type ) {
                case MT_EMPTY:
#if defined( _STANDALONE_ )
                    fixup_option = OPTJ_EXTEND;
                    fixup_type = FIX_RELOFF8;
                    Code->info.opnd_type[Opnd_Count] = OP_I8;
                    break;
#endif
                case MT_SHORT:
                    fixup_option = OPTJ_EXPLICIT;
                    fixup_type = FIX_RELOFF8;
                    Code->info.opnd_type[Opnd_Count] = OP_I8;
                    break;
                default:
                    AsmError( ONLY_SHORT_DISPLACEMENT_IS_ALLOWED );
                    return( ERROR );
                }
            }
        }
        AddFixup( sym, fixup_type, fixup_option );
        break;
    default: /* SYM_STACK */
        AsmError( NO_JUMP_TO_AUTO );
        return( ERROR );
    }
    return( NOT_ERROR );
}

int ptr_operator( memtype mem_type, uint_8 fix_mem_type )
/***********************************************/
/*
  determine what should be done with SHORT, NEAR, FAR, BYTE, WORD, DWORD, PTR
  operator;
*/
{
    /* new idea:
     * when we get a near/far/dword/etc, just set distance / mem_type
     * operator will be called again with PTR, then we set the opsiz, etc.
     */

    if( Code->info.token == T_LEA )
        return( NOT_ERROR );
    if( Code->info.token == T_SMSW )
        return( NOT_ERROR );
    if( mem_type == MT_PTR ) {
        /* finish deciding what type to make the inst NOW
         * ie: decide size overrides etc.
         */
        if( Code->use32 && MEM_TYPE( Code->mem_type, WORD ) ) {
            // if we are in use32 mode, we have to add OPSIZ prefix for
            // most of the 386 instructions ( except MOVSX and MOVZX )
            // when we find WORD PTR

            if( !IS_BRANCH( Code->info.token ) ) {
                if( Code->info.opnd_type[OPND1] == OP_MMX ) {
                /* JBS 2001/02/19
                no WORD operands for MMX instructions, only 64-bit or 128-bit
                so no WORD override needed
                    */
                } else {
                    switch( Code->info.token ) {
                    case T_MOVSX:
                    case T_MOVZX:
                        break;
                    default:
                        Code->prefix.opsiz = TRUE;
                        break;
                    }
                }
            }

        } else if( !Code->use32 && MEM_TYPE( Code->mem_type, DWORD ) ) {

            /* if we are not in use32 mode, we have to add OPSIZ
             * when we find DWORD PTR
             * unless we have a LXS ins.
             * which moves a DWORD ptr into SR:word reg
             * fixme  - can this be done by breaking up the LXS instructions in
             *          asmins.h, and then putting F_32 or F_16 to append
             *      opsize bytes when necessary ?
             */
            if( !IS_BRANCH( Code->info.token ) ) {

                if( Code->info.opnd_type[OPND1] == OP_MMX ) {
                    /* JBS 2001/02/19
                       no WORD operands for MMX instructions, only 64-bit or 128-bit
                       so no DWORD override needed
                     */
                } else {
                    switch( Code->info.token ) {
                    case T_LDS:
                    case T_LES:
                    case T_LFS:
                    case T_LGS:
                    case T_LSS:
                        /* in these cases, opsize does NOT need to be changed  */
                        break;
                    default:
                        // OPSIZ prefix
                        Code->prefix.opsiz = TRUE;
                    }
                }
            }
        }
    } else {
        if( ( mem_type != MT_EMPTY ) && ( Code->mem_type_fixed == FALSE ) ) {
#if defined( _STANDALONE_ )
            if( mem_type != MT_STRUCT ) {
#endif
                Code->mem_type = mem_type;
                if( fix_mem_type ) {
                    Code->mem_type_fixed = TRUE;
                    if( IS_JMPCALLN( Code->info.token ) && ( mem_type == MT_FAR ) ) {
                        Code->info.token++;
                    }
                }

#if defined( _STANDALONE_ )
            }
#endif
        }
    }
    return( NOT_ERROR );
}
