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


/* set this macro to 0 if you do NOT want to allow structures to be defined
 * ie:
 *    struct foo
 *      ...
 *    foo ends
 *    ...
 *    bar foo < 4, 2, 3 >
 */
#define ALLOW_STRUCT_INIT 1

#include "asmglob.h"

#include "asminsd.h"
#include "asmdefs.h"
#include "asmexpnd.h"
#include "asmfixup.h"
#include "asmeval.h"

#ifdef _WASM_
#include "directiv.h"
#include "myassert.h"
#endif

extern int              match_phase_1( void );
extern int              ptr_operator( memtype, uint_8 );
extern int              jmp( struct asm_sym *sym );

unsigned char           More_Array_Element = FALSE;
unsigned char           Last_Element_Size;

static struct asm_code  Code_Info;
struct asm_code         *Code = &Code_Info;

unsigned char           Opnd_Count;

extern int              MakeLabel( char *, memtype );
extern int              dup_array( struct asm_sym *, char, char );
extern int              data_init( int, int );

static void             SizeString( unsigned op_size );

#ifdef _WASM_

extern void             InputQueueLine( char * );
extern int              directive( int , long );
extern int              SymIs32( struct asm_sym *sym );
extern void             find_use32( void );

void                    check_assume( struct asm_sym *, enum prefix_reg );

extern  int_8           DefineProc;     // TRUE if the definition of procedure
                                        // has not ended

uint_8                  CheckSeg;       // if checking of opened segment is needed
int_8                   Frame;          // Frame of current fixup
uint_8                  Frame_Datum;    // Frame datum of current fixup
extern char             *CurrString;    // Current Input Line
dir_node                *SegOverride;

static int              in_epilogue = 0;

#else

#define     directive( i, value )   cpu_directive( value )

#endif

extern void             make_inst_hash_table( void );

static int              curr_ptr_type;
static char             ConstantOnly;

static int              mem2code( char, int, int, asm_sym * );

/* moved here from asmline */
static struct asm_tok   tokens[MAX_TOKEN];

struct asm_tok  *AsmBuffer[MAX_TOKEN];  // buffer to store token

#ifdef _WASM_
void find_frame( struct asm_sym *sym )
/*******************************************/
{
    if( SegOverride != NULL ) {
        sym = (struct asm_sym *)SegOverride;
        if( sym->grpidx != 0 ) {
            Frame = FRAME_GRP;
            Frame_Datum = sym->grpidx;
        } else if( sym->segidx != 0 ) {
            Frame = FRAME_SEG;
            Frame_Datum = sym->segidx;
        }
    } else {
        switch( sym->state ) {
        case SYM_INTERNAL:
        case SYM_PROC:
            if( sym->grpidx != 0 ) {
                Frame = FRAME_GRP;
                Frame_Datum = sym->grpidx;
            } else if( sym->segidx != 0 ) {
                Frame = FRAME_SEG;
                Frame_Datum = sym->segidx;
            }
            break;
        case SYM_GRP:
            Frame = FRAME_GRP;
            Frame_Datum = sym->grpidx;
            break;
        case SYM_SEG:
            Frame = FRAME_SEG;
            Frame_Datum = sym->segidx;
            break;
        }
    }
}
#endif

static int comp_mem( int reg1, int reg2 )
/***************************************/
/*
- compare and return the r/m field encoding of 16-bit address mode;
- call by mem2code() only;
*/
{
    switch( reg1 ) {
    case T_BX:
        switch( reg2 ) {
        case T_SI: return( MEM_BX_SI );
        case T_DI: return( MEM_BX_DI );
        }
        break;
    case T_BP:
        switch( reg2 ) {
        case T_SI: return( MEM_BP_SI );
        case T_DI: return( MEM_BP_DI );
        }
        break;
    }
    AsmError( INVALID_MEMORY_POINTER );
    return( ERROR );
}

static void seg_override( int seg_reg, asm_sym *sym )
/***************************************************/
/*
- determine if segment override is necessary with the current address mode;
*/
{
    enum prefix_reg     default_seg;
#ifdef _WASM_
    enum assume_reg     assume_seg;

    switch( seg_reg ) {
        case T_SS:
        case T_BP:
        case T_EBP:
        case T_ESP:
            default_seg = PREFIX_SS;
            break;
        default:
            default_seg = PREFIX_DS;
    }
    if( sym != NULL ) {
        if( Code->prefix.seg == EMPTY ) {
            if( Code->info.token == T_LEA ) {
                check_assume( sym, EMPTY );
            } else {
                check_assume( sym, default_seg );
            }
        } else {
            switch( Code->prefix.seg ) {
            case PREFIX_ES:
                assume_seg = ASSUME_ES;
                break;
            case PREFIX_CS:
                assume_seg = ASSUME_CS;
                break;
            case PREFIX_SS:
                assume_seg = ASSUME_SS;
                break;
            case PREFIX_DS:
                assume_seg = ASSUME_DS;
                break;
            case PREFIX_FS:
                assume_seg = ASSUME_FS;
                break;
            case PREFIX_GS:
                assume_seg = ASSUME_GS;
                break;
            }
            if( GetPrefixAssume( sym, assume_seg ) == ASSUME_NOTHING ) {
                AsmWarn( 3, CANNOT_ADDRESS_WITH_ASSUMED_REGISTER );
            }
        }
    }
    
    if( Code->prefix.seg == default_seg ) {
        Code->prefix.seg = EMPTY;
    }
#else
    if( Code->prefix.seg != EMPTY ) {
        switch( seg_reg ) {
        case T_SS:
        case T_BP:
        case T_EBP:
        case T_ESP:
            default_seg = PREFIX_SS;
            break;
        default:
            default_seg = PREFIX_DS;
        }
        if( Code->prefix.seg == default_seg ) {
            Code->prefix.seg = EMPTY;
        }
    }
#endif
}

#ifdef _WASM_

static void check_assume( struct asm_sym *sym, enum prefix_reg default_reg )
/**************************************************************************/
/* Check if an assumed register is found, and prefix a register if necessary */
{
    enum assume_reg     reg;
    enum assume_reg     def_reg;

    /**/myassert( sym != NULL );
    if( sym->state == SYM_UNDEFINED ) return;

    switch( default_reg ) {
        case PREFIX_SS:
            def_reg = ASSUME_SS;
            break;
        case PREFIX_DS:
            def_reg = ASSUME_DS;
            break;
        case EMPTY:
            def_reg = ASSUME_NOTHING;
            break;
    }

    reg = GetAssume( sym, def_reg );

    if( reg == ASSUME_NOTHING ) {
        if( sym->state != SYM_EXTERNAL && sym->state != SYM_PROC ) {
            AsmWarn( 3, CANNOT_ADDRESS_WITH_ASSUMED_REGISTER );
        } else {
            Code->prefix.seg = default_reg;
        }
    } else if(  default_reg != EMPTY ) {
        switch( reg ) {
            case ASSUME_ES:
                Code->prefix.seg = PREFIX_ES;
                break;
            case ASSUME_CS:
                Code->prefix.seg = PREFIX_CS;
                break;
            case ASSUME_DS:
                Code->prefix.seg = PREFIX_DS;
                break;
            case ASSUME_GS:
                Code->prefix.seg = PREFIX_GS;
                break;
            case ASSUME_FS:
                Code->prefix.seg = PREFIX_FS;
                break;
            case ASSUME_SS:
                Code->prefix.seg = PREFIX_SS;
                break;
        }
    }
}

int check_override( int *i )
/**************************/
/* Check if there is a register, segment or group override */
{
    int         index;

    index = *i;

    if( ( index+2 ) < Token_Count ) {
        if( AsmBuffer[index+1]->token == T_COLON ) {
            switch( AsmBuffer[index]->token ) {
            case T_REG:
                Code->prefix.seg =
                    AsmOpTable[AsmOpcode[AsmBuffer[index]->value].position].opcode;
                (*i) += 2;
                if( *i >= Token_Count ) {
                    AsmError( LABEL_EXPECTED_AFTER_COLON );
                    return ERROR;
                }
                break;
            case T_ID:      // Segment or Group override
                if( FixOverride(*i) != NOT_ERROR ) {
                    return ERROR;
                }
                (*i) += 2;
                if( *i >= Token_Count ) {
                    AsmError( LABEL_EXPECTED_AFTER_COLON );
                    return ERROR;
                }
                break;
            default:
                break;
            }
        }
    }
    return NOT_ERROR;
}
#endif

static int mem( int i )
/*********************/
/*
  parse the memory reference operand
*/
{
    char                ss = SCALE_FACTOR_1;
    char                count = Token_Count;
    int                 index = EMPTY;
    int                 base = EMPTY;
    struct asm_sym      *sym;
    struct asm_sym      *field;
    struct asmfixup     *fixup;
    char                base_lock = FALSE;
    char                *string_ptr;
    char                id_flag = 0;
    enum fixup_types    fixup_type;
    int                 flag;
#ifdef _WASM_
    int                 type;
    char                field_flag = 0;

#endif
    sym = NULL;
    fixup = NULL;
    ConstantOnly = TRUE;
    if( Code->prefix.seg != EMPTY ) {
        //fixme
        /* temporary fix -- we will probably have to change the structure of
         * the asm_code structure, to allow multiple seg. overrides per line
         * for "MOVS ds:si, es:di" for example
         *
         * we need to have Constantonly set IFF we have a number which does NOT
         * have a seg override
         * for now check if the token before this one is a colon
         * or if the prev. token was a [ and a : before that
         */
        if( ( AsmBuffer[i-1]->token == T_COLON ) ||
            ( ( AsmBuffer[i-1]->token == T_OP_SQ_BRACKET ) &&
              ( AsmBuffer[i-2]->token == T_COLON ) ) ) {
            ConstantOnly = FALSE;
        }
    }
    while( AsmBuffer[i]->token != T_FINAL && AsmBuffer[i]->token != T_COMMA ) {
        switch( AsmBuffer[i]->token ) {
        case T_REG:
            ConstantOnly = FALSE;
            switch( AsmBuffer[i]->value ) {     // check for index registers
            case T_EAX:
            case T_EBX:
            case T_ECX:
            case T_EDX:
            case T_ESP:
            case T_EBP:
            case T_ESI:
            case T_EDI:
                if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
                    // 286 and down cannot use 386 registers
                    AsmError( CANNOT_USE_386_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                    return( ERROR );
                }
                if( !Code->use32 ) Code->prefix.adrsiz = TRUE;
                break;
            case T_BX:
            case T_BP:
            case T_SI:
            case T_DI:
                if( Code->use32 ) Code->prefix.adrsiz = TRUE;
                break;
            default:
                AsmError( INVALID_MEMORY_POINTER );
                return( ERROR );
            }
            switch( AsmBuffer[i + 1]->token ) {
            case T_TIMES :  // 386, cur reg is index
                if( (Code->info.cpu&P_CPU_MASK) >= P_386 ) {
                    if( !Code->use32 ) Code->prefix.adrsiz = TRUE;
                    index = AsmBuffer[i]->value;
                    switch( index ) {
                    case T_ESP:
                    case T_BX:
                    case T_BP:
                    case T_SI:
                    case T_DI:
                        // cannot use ESP or 16-bit reg as index
                        AsmError( INVALID_INDEX_REGISTER );
                        return( ERROR );
                    default:
                        if( !Code->use32 ) Code->prefix.adrsiz = TRUE;
                        i += 2;
                        if( AsmBuffer[i]->token == T_NUM ) {
                            switch( AsmBuffer[i]->value ) {
                            case 1:
                                // ss = 00
                                break;
                            case 2:
                                // ss = 01
                                ss = SCALE_FACTOR_2;
                                break;
                            case 4:
                                // ss = 10
                                ss = SCALE_FACTOR_4;
                                break;
                            case 8:
                                // ss = 11
                                ss = SCALE_FACTOR_8;
                                break;
                            default: // must be * 1, 2, 4 or 8
                                AsmError( SCALE_FACTOR_MUST_BE_1_2_4_OR_8 );
                                return( ERROR );
                            }
                        } else {
                            // '*' must be followed by a number
                            AsmError( EXPECTING_NUMBER );
                            return( ERROR );
                        }
                    }
                } else {
                    // 286 and down cannot use this memory mode
                    AsmError( INVALID_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                    return( ERROR );
                }
                break;
            case T_CL_SQ_BRACKET:
                if( base == EMPTY ) {
                    // cur reg is base
                    base = AsmBuffer[i]->value;
                } else if( index == EMPTY ) {
                    if( AsmBuffer[i]->value == T_ESP ) {
                        if( base_lock == TRUE ) {
                            AsmError( ESP_CANNOT_BE_USED_AS_INDEX );
                            return( ERROR );
                        } else {
                            index = base;
                            base = AsmBuffer[i]->value;
                        }
                    } else {
                        index = AsmBuffer[i]->value;
                    }
                } else {
                    AsmError( TOO_MANY_BASE_INDEX_REGISTERS );
                    return( ERROR );
                }
                i++;
                break;
            default:
                AsmError( INVALID_MEMORY_POINTER );
                return( ERROR );
            }
            break;
        case T_MINUS:
            if( AsmBuffer[i+1]->token != T_NUM ) {
                AsmError( ILLEGAL_USE_OF_REGISTER );
                return( ERROR );
            }
            i++;
            Code->data[Opnd_Count] -= AsmBuffer[i]->value;
            break;
        case T_NUM: // direct address
            Code->data[Opnd_Count] += AsmBuffer[i]->value;
            break;
        case T_COMMA:
            count = i;
            i--;
            break;
        case T_OP_SQ_BRACKET:
        case T_CL_SQ_BRACKET:
        case T_DOT:
        case T_PLUS:
            break;
        case T_ID:
            // ConstantOnly = FALSE;
            string_ptr = AsmBuffer[i]->string_ptr;
            if( *string_ptr == '+' ) {
                string_ptr++;
            } else if( *string_ptr == '-' ) {
                AsmError( OFFSET_MUST_BE_RELOCATABLE );
                return( ERROR );
            }
            field = sym;
            sym = AsmLookup( string_ptr );
            if( sym == NULL )
                return ERROR;
            ConstantOnly = FALSE;
            if( id_flag > 0 ) {
                AsmError( MEMORY_OFFSET_CANNOT_REFERENCE_TO_MORE_THAN_ONE_LABEL );
                return( ERROR );
            } else {
                id_flag++;
            }
            
            switch( sym->state ) {
            case SYM_UNDEFINED:
                // forward reference
                break;
            case SYM_STACK:
                if( base != EMPTY ) {
                    if( base_lock == TRUE ) {
                        // [reg + data][reg + data] is not allowed
                        AsmError( TOO_MANY_BASE_REGISTERS );
                        return( ERROR );
                    } else {
                        index = base;
                    }
                }
                if( Code->use32 ) {
                    base = T_EBP;
                } else {
                    base = T_BP;
                }
                base_lock = TRUE;   // add lock
                /* fall through */
            default:
                if( Code->mem_type == EMPTY ) {
#ifdef _WASM_
                    if( !Modend ) {
#endif
                        if( ptr_operator( sym->mem_type, FALSE ) == ERROR )
                            return( ERROR );
                        if( ptr_operator( T_PTR, FALSE ) == ERROR ) {
                            return( ERROR );
                        }
#ifdef _WASM_
                    }
#endif
                }
                break;
            }

            fixup_type = FIX_OFF16; /* might be changed to OFF32 later */

#ifdef _WASM_
            switch( sym->state ) {
            case SYM_GRP:
            case SYM_SEG:
                ConstantOnly = TRUE;
                fixup_type = FIX_SEG;
                Code->info.opnd_type[Opnd_Count] = OP_I16;
                Code->info.opcode |= W_BIT;
                break;
            }
#endif
            break;
        default:
            AsmError( INVALID_INDIRECT_MEMORY_OPERAND );
            return( ERROR );
        }
        i++;
    }
    flag = 0;
    if( ( index != EMPTY ) || ( base != EMPTY ) ) {
        // indirect
        if( sym != NULL ) {
            // indirect with symbol
            flag = 1;
        } else {
            // indirect only reg + const
            flag = 2;
        }
    } else if( sym != NULL ) {
        // direct symbol
        flag = 3;
    } else {
        // constant
        flag = 4;
    }
    if( flag == 1 || flag == 2 || flag == 3 ) {
//    if( flag == 3 ) {
        if( IS_ANY_BRANCH( Code->info.token ) ) {
            int temp;
            /* might be a direct control transfer */
            temp = jmp( sym );
            switch( temp ) {
            case ERROR:
                return( ERROR );
            case SCRAP_INSTRUCTION:
                return( EMPTY );
            case INDIRECT_JUMP:
                break;
            default:
                if( flag == 1 || flag == 2 ) {
                    break;
                } else {
                    return( i - 1 );
                }
            }
        }
    }
    if( sym != NULL ) {
        fixup = AddFixup( sym, fixup_type, OPTJ_NONE );
    }
#ifdef _WASM_
    type = 0;
    if( ( sym != NULL ) && ( index == EMPTY ) && ( base == EMPTY ) ) {
        switch( Code->mem_type ) {
        case T_NEAR:
        case T_FAR:
            type = 1;
            break;
        case EMPTY:
            switch( sym->mem_type ) {
            case T_NEAR:
            case T_FAR:
                type = 1;
                break;
            }
            break;
        }
        if( Code->info.token == T_LEA ) {
            type = 0;
        }
        if( type == 0 ) {
            if( !ConstantOnly ) {
                SET_ADRSIZ( Code, SymIs32( sym ));
            }
        }
    }
    if( fixup != NULL && fixup->fixup_type == FIX_OFF16 && addr_32( Code ) ) {
        fixup->fixup_type = FIX_OFF32;
    }
    if( Modend ) {
        if( sym == NULL ) {
            AsmError( SYNTAX_ERROR );
            return( ERROR );
        }
        GetAssume( sym, ASSUME_NOTHING );
    } else if( ConstantOnly ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else if( type == 0 ) {
        if( mem2code( ss, index, base, sym ) == ERROR ) {
            return( ERROR );
        }
    }
    if( type > 0 ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else if( sym != NULL ) {
        add_frame();
    } else if( field_flag ) {
        if( ConstantOnly ) {
            if( idata( Code->data[Opnd_Count] ) == ERROR ) {
                return( ERROR );
            }
        } else {
            if( ptr_operator( T_PTR, FALSE ) == ERROR ) {
                return( ERROR );
            }
        }
    }
    return( i - 1 );
#else
    if( fixup != NULL && fixup->fixup_type == FIX_OFF16 && addr_32( Code ) ) {
        fixup->fixup_type = FIX_OFF32;
    }
    if( ConstantOnly ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else {
        if( mem2code( ss, index, base, sym ) == ERROR ) {
            return( ERROR );
        }
    }
    return( i - 1 );
#endif
}

static int Reg386( int reg_token )
/********************************/
{
    switch( reg_token ) {
    case T_EAX:         return( 0 );
    case T_ECX:         return( 1 );
    case T_EDX:         return( 2 );
    case T_EBX:         return( 3 );
    case T_ESP:         return( 4 );
    case T_EBP:         return( 5 );
    case T_ESI:         return( 6 );
    case T_EDI:         return( 7 );
    }
    /* I don't think this should happen */
    return( 0 );
}

int OperandSize( enum operand_type opnd )
/***************************************/
{
    if( opnd == OP_NONE || opnd & OP_SPECIAL ) {
        return( 0 );
    } else if( opnd == OP_M ) {
        /* fixme */
        switch( Code->mem_type ) {
        case EMPTY:     return( 0 );
#ifdef _WASM_
        case T_SBYTE:
#endif
        case T_BYTE:    return( 1 );
#ifdef _WASM_
        case T_SWORD:
#endif
        case T_WORD:    return( 2 );
#ifdef _WASM_
        case T_SDWORD:
#endif
        case T_DWORD:   return( 4 );
        case T_FWORD:   return( 6 );
        case T_QWORD:   return( 8 );
        case T_TBYTE:   return( 10 );
        case T_OWORD:   return( 16 );
        }
    } else if( opnd & ( OP_M8_R8 | OP_M_B | OP_I8 | OP_I_1 | OP_I_3 | OP_I8_U ) ) {
        return( 1 );
    } else if( opnd & ( OP_M16_R16 | OP_M_W | OP_I16 | OP_SR ) ) {
        return( 2 );
    } else if( opnd & ( OP_R32 | OP_M_DW | OP_I32 | OP_J32 | OP_SPEC_REG ) ) {
        return( 4 );
    } else if( opnd & ( OP_I | OP_J48 ) ) {
        return( 6 );
    } else if( opnd & ( OP_M_QW | OP_MMX ) ) {
        return( 8 );
    } else if( opnd & ( OP_STI | OP_M_TB ) ) {
        return( 10 );
    } else if( opnd & ( OP_M_OW | OP_XMM ) ) {
        return( 16 );
    }
    return( 0 );
}

int InRange( unsigned long val, unsigned bytes )
/**********************************************/
/*
     Can 'val' be represented in 'bytes' bytes?
*/
{
    unsigned long max;
    unsigned long mask;

    max = (1UL << (bytes*8)) - 1;
    if( val <= max ) /* absolute value fits */
        return( 1 );
    mask = ~(max >> 1);
    if( (val & mask) == mask ) /* just a sign extension */
        return( 1 );
    return( 0 );

}

static int mem2code( char ss, int index, int base, asm_sym *sym )
/***************************************************************/
/*
  encode the memory operand to machine code
*/
{
    struct asm_code     *rCode = Code;
    int                 temp;
    unsigned char       mod_field;
    unsigned char       rm_field;

    // clear mod
    rm_field = 0;
    if( InsFixups[Opnd_Count] != NULL ) {
        mod_field = MOD_10;
    } else if( rCode->data[Opnd_Count] == 0 ) {
        mod_field = MOD_00;
    } else if( rCode->data[Opnd_Count] > SCHAR_MAX ||
               rCode->data[Opnd_Count] < SCHAR_MIN ) {
        mod_field = MOD_10;
    } else {
        mod_field = MOD_01;
    }
    if( index == EMPTY  &&  base == EMPTY ) {
        // direct memory
        // clear the rightmost 3 bits
        mod_field = MOD_00;
        if( !addr_32( rCode )) {
            if( !InRange( rCode->data[Opnd_Count], 2 ) ) {
                // expect 16-bit but got 32-bit address
                AsmError( DISPLACEMENT_OUT_OF_RANGE );
                return( ERROR );
            }
            rm_field = D16;
        } else {
            rm_field = D32;
        }
        // default is DS:[], DS: segment override is not needed
        seg_override( T_DS, sym );
    } else if( index == EMPTY  &&  base != EMPTY ) {
        switch( base ) {
        case T_SI:
            rm_field = 0x04; // SI
            // default is DS:[], DS: segment override is not needed
            break;
        case T_DI:
            rm_field = 0x05; // DI
            // default is DS:[], DS: segment override is not needed
            break;
        case T_BP:
            rm_field = BP;
            if( mod_field == MOD_00 ) {
                mod_field = MOD_01;
            }
            // default is SS:[], SS: segment override is not needed
            break;
        case T_BX:
            rm_field = 0x07; // BX
            // default is DS:[], DS: segment override is not needed
            break;
        case T_EBP:
            rm_field = EBP;
            if( mod_field == MOD_00 ) {
                mod_field = MOD_01;
            }
            // default is SS:[], SS: segment override is not needed
            break;
        case T_ESP:
            rm_field = ESP;
            // ss = 00, index = 100 ( no index ), base = 100 ( ESP )
            rCode->sib = 0x24;
            // default is SS:[], SS: segment override is not needed
            break;
        default: // for 386 and up
            rm_field = Reg386( base );
            // default is DS:[], DS: segment override is not needed
        }
        seg_override( base, sym );
    } else if( index != EMPTY  &&  base == EMPTY ) {
        // mod field is 00
        mod_field = MOD_00;
        // s-i-b is present ( r/m = 100 )
        rm_field = S_I_B;
        // scale factor, index, base ( 0x05 => no base reg )
        rCode->sib = ( ss | ( Reg386(index) << 3 ) | 0x05 );
        // default is DS:[], DS: segment override is not needed
        seg_override( T_DS, sym );
    } else {
        // base != EMPTY && index != EMPTY
        switch( index ) {
        case T_BX:
        case T_BP:
            if( ( temp = comp_mem( index, base ) ) == ERROR )
                return( ERROR );
            rm_field = temp;
            seg_override( index, sym );
            break;
        case T_SI:
        case T_DI:
            if( ( temp = comp_mem( base, index ) ) == ERROR )
                return( ERROR );
            rm_field = temp;
            seg_override( base, sym );
            break;
        case T_ESP:
            AsmError( ESP_CANNOT_BE_USED_AS_INDEX );
            return( ERROR );
        default:
            if( base < T_EAX ) {
                AsmError( CANNOT_MIX_16_AND_32_BIT_REGISTERS );
                return( ERROR );
            } else if( base == T_EBP ) {
                if( mod_field == MOD_00 ) {
                    mod_field = MOD_01;
                }
            }
            // s-i-b is present ( r/m = 100 )
            rm_field |= S_I_B;
            rCode->sib = ( ss | (Reg386(index) << 3) | Reg386(base) );
            seg_override( base, sym );
        }
    }
    if( Opnd_Count == OPND2 ) {
        // shift the register field to left by 3 bit
        rCode->info.rm_byte = mod_field | ( rm_field << 3 ) | ( rCode->info.rm_byte & BIT_012 );
    } else if( Opnd_Count == OPND1 ) {
        rCode->info.rm_byte = mod_field | rm_field;
    }
    return( NOT_ERROR );
}

static int comp_opt( uint direct )
/********************************/
/*
  Compare function for CPU directive
*/
{
    // follow Microsoft MASM
    switch( direct ) {
    case T_DOT_NO87:
        return( P_NO87 );
    case T_DOT_8086:
        return( P_86 );
    case T_DOT_8087:
        return( P_87 );
    case T_DOT_186:
        return(  P_186 );
    case T_DOT_286:
        return(  P_286 );
    case T_DOT_287:
        return(  P_287 );
    case T_DOT_286P:
        return( P_286p );
    case T_DOT_386:
        return(  P_386 );
    case T_DOT_387:
        return(  P_387 );
    case T_DOT_386P:
        return( P_386p );
    case T_DOT_486:
        return(  P_486 );
    case T_DOT_486P:
        return( P_486p );
    case T_DOT_586:
        return(  P_586 );
    case T_DOT_586P:
        return( P_586p );
    case T_DOT_686:
        return(  P_686 );
    case T_DOT_686P:
        return( P_686p );
    case T_DOT_MMX:
        return( P_MMX );
    case T_DOT_K3D:
        return( P_K3D | P_MMX );
    case T_DOT_XMM:
        return( P_SSE | P_MMX );
    case T_DOT_XMM2:
        return( P_SSE2 | P_SSE | P_MMX );
    case T_DOT_XMM3:
        return( P_SSE3 | P_SSE2 | P_SSE | P_MMX );
    default:
        // not found
        return( EMPTY );
    }
}

#ifdef _WASM_
static void MakeCPUConstant( int i )
/**********************************/
{
    MakeConstantUnderscored( i );

    switch( i ) {
    // fall right through
    case T_DOT_686P:
    case T_DOT_686:
        MakeConstantUnderscored( T_DOT_686 );
    case T_DOT_586P:
    case T_DOT_586:
        MakeConstantUnderscored( T_DOT_586 );
    case T_DOT_486P:
    case T_DOT_486:
        MakeConstantUnderscored( T_DOT_486 );
    case T_DOT_386P:
    case T_DOT_386:
        MakeConstantUnderscored( T_DOT_386 );
        break;
    case T_DOT_286P:
    case T_DOT_286:
        MakeConstantUnderscored( T_DOT_286 );
    }
    return;
}
#endif

int cpu_directive( int i )
/************************/
{
    int                 temp;

    if( ( temp = comp_opt( i ) ) != EMPTY ) {
        if( i == T_DOT_NO87 ) {
            Code->info.cpu &= ~P_FPU_MASK;              // turn off FPU bits
        } else if( temp & P_FPU_MASK ) {
            Code->info.cpu &= ~P_FPU_MASK;              // turn off FPU bits
            Code->info.cpu |= temp & P_FPU_MASK;        // turn on desired bit(s)
        } else if( temp & P_EXT_MASK ) {
            Code->info.cpu |= temp & P_EXT_MASK;        // turn on desired bit(s)
        } else {
            Code->info.cpu &= ~(P_CPU_MASK | P_PM);
            Code->info.cpu |= temp & (P_CPU_MASK | P_PM);
        }
    } else {
        AsmError( UNKNOWN_DIRECTIVE );
        return( ERROR );
    }

#ifdef _WASM_
    MakeCPUConstant( i );
    switch( i ) {
    case T_DOT_686P:
    case T_DOT_686:
    case T_DOT_586P:
    case T_DOT_586:
    case T_DOT_486P:
    case T_DOT_486:
    case T_DOT_386P:
    case T_DOT_386:
        SetModuleDefSegment32( TRUE );
        find_use32();
        break;
    case T_DOT_286P:
    case T_DOT_286:
    case T_DOT_186:
    case T_DOT_8086:
        SetModuleDefSegment32( FALSE );
        find_use32();
        break;
    default:
        // set FPU
        break;
    }
#endif

    return( NOT_ERROR );
}


static int idata( long value )
/****************************/
/*
  determine the correct data size of immediate operand;
*/
{
    enum operand_type   op_type;

    /* note that we now have code DISTANCE ( short / near / far ) and
     *                            mem_type ( byte / word / dword ... )
     */

    switch( Code->mem_type ) {
    case EMPTY:
        if( Code->info.token == T_PUSH ) {
            if( Code->use32 ) {
                if( (int_8)value == (int_32)value ) {
                    op_type = OP_I8;
                } else {
                    op_type = OP_I32;
                }
            } else {
                if( (unsigned long)value > USHRT_MAX ) {
                    SET_OPSIZ_32( Code );
                    if( (int_8)value == (int_32)value ) {
                        op_type = OP_I8;
                    } else {
                        op_type = OP_I32;
                    }
                } else {
                    if( (int_8)value == (int_16)value ) {
                        op_type = OP_I8;
                    } else {
                        op_type = OP_I16;
                    }
                }
            }
            break;
        } else if( Code->info.token == T_PUSHW ) {
            SET_OPSIZ_16( Code );
            if( (int_8)value == (int_16)value ) {
                op_type = OP_I8;
            } else {
                op_type = OP_I16;
            }
            break;
        } else if( Code->info.token == T_PUSHD ) {
            SET_OPSIZ_32( Code );
            if( (int_8)value == (int_32)value ) {
                op_type = OP_I8;
            } else {
                op_type = OP_I32;
            }
            break;
        }
        // fall through
    case T_FAR:
        if( value > SHRT_MAX  ||  value < SHRT_MIN ) {
            op_type = OP_I32;
        } else if( value > SCHAR_MAX  ||  value < SCHAR_MIN ) {
            op_type = OP_I16;
        } else {
            op_type = OP_I8;
        }
        break;
    case T_NEAR:
        if( !Code->use32 ) {
            op_type = OP_I16;
        } else {
            op_type = OP_I32;
        }
        break;
    case T_SHORT:
        if( value > SCHAR_MAX  ||  value < SCHAR_MIN ) {
            // expect 8-bit but got 16 bit
            AsmError( JUMP_OUT_OF_RANGE );
            return( ERROR );
        } else {
            op_type = OP_I8;
        }
        break;
    case T_BYTE:
        if( !InRange( value, 1 ) ) {
            // expect 8-bit but got 16 bit
            AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
            return( ERROR );
        } else {
            op_type = OP_I8;
        }
        break;
#ifdef _WASM_
    case T_SBYTE:
        if( value > SCHAR_MAX || value < SCHAR_MIN ) {
            AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
            return( ERROR );
        } else {
            op_type = OP_I8;
        }
        break;
    case T_SWORD:
        if( value > SHRT_MAX  ||  value < SHRT_MIN ) {
            AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
            return( ERROR );
        } else if( value > SCHAR_MAX  ||  value < SCHAR_MIN ){
            op_type = OP_I16;
        } else {
            op_type = OP_I8;
        }
        // set w-bit
        Code->info.opcode |= W_BIT;
        break;
#endif
    case T_WORD:
#ifdef _WASM_
        if( Options.sign_value ) {
            if( !InRange( value, 2 ) ) {
                AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
                return( ERROR );
            } else if( value > UCHAR_MAX ){
                op_type = OP_I16;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
        } else {
#endif
            if( !InRange( value, 2 ) ) {
                AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
                return( ERROR );
            } else if( value > SCHAR_MAX  ||  value < SCHAR_MIN ){
                op_type = OP_I16;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
#ifdef _WASM_
        }
#endif
        break;
#ifdef _WASM_
   case T_SDWORD:
        if( value > SCHAR_MAX  ||  value < SCHAR_MIN ){
            op_type = OP_I32;
        } else {
            op_type = OP_I8;
        }
        // set w-bit
        Code->info.opcode |= W_BIT;
        break;
#endif
    case T_DWORD:
#ifdef _WASM_
        if( Options.sign_value ) {
            if( value > UCHAR_MAX ) {
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
        } else {
#endif
            if( value > SCHAR_MAX  ||  value < SCHAR_MIN ){
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
#ifdef _WASM_
        }
#endif
        break;
    case T_QWORD:
#ifdef _WASM_
        if( Options.sign_value ) {
            if( value > UCHAR_MAX ) {
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
        } else {
#endif
            if( value > SCHAR_MAX  ||  value < SCHAR_MIN ){
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            Code->info.opcode |= W_BIT;
#ifdef _WASM_
        }
#endif
        break;
    }
    Code->info.opnd_type[Opnd_Count] = op_type;
    Code->data[Opnd_Count] = value;
    return( NOT_ERROR );
}

static int idata_float( long value )
/**********************************/
/*
  check the correct operand/data size for float immediate operand;
*/
{
    switch( Code->mem_type ) {
    case EMPTY:
        if( Code->info.token == T_PUSH ) { // sigh. another special case
            if( !Code->use32 ) {
                // expect 32-bit code but get 16-bit
                AsmError( IMMEDIATE_DATA_OUT_OF_RANGE );
                return( ERROR );
            }
        }
        break;
    case T_FAR:
    case T_NEAR:
    case T_SHORT:
        AsmError( SYNTAX_ERROR );
        return( ERROR );
#ifdef _WASM_
    case T_SBYTE:
    case T_SWORD:
#endif
    case T_BYTE:
    case T_WORD:
        AsmError( OPERANDS_MUST_BE_THE_SAME_SIZE );
        return( ERROR );
#ifdef _WASM_
    case T_SDWORD:
#endif
    case T_DWORD:
        // set w-bit
        Code->info.opcode |= W_BIT;
        break;
    }
    Code->info.opnd_type[Opnd_Count] = OP_I32;
    Code->data[Opnd_Count] = value;
    return( NOT_ERROR );
}

static unsigned char get_sr_rm_byte( enum prefix_reg seg_prefix )
/***************************************************************/
{
    switch( seg_prefix ) {
    case PREFIX_ES:
        return( 0 );
    case PREFIX_CS:
        return( 1 );
    case PREFIX_SS:
        return( 2 );
    case PREFIX_DS:
        return( 3 );
    case PREFIX_FS:
        return( 4 );
    case PREFIX_GS:
        return( 5 );
    }
    #ifdef _WASM_
        /**/myassert( 0 );
    #endif
    return( 0 );
}

static int reg( int i )
/*********************/
/*
- parse and encode the register operand;
*/
{
    int                 temp;
    int                 reg;

    temp = AsmOpcode[AsmBuffer[i]->value].position;
    reg = AsmOpTable[temp].opcode;
    Code->info.opnd_type[Opnd_Count] = AsmOpTable[temp].opnd_type[OPND2];
    switch( AsmOpTable[temp].opnd_type[OPND2] ) {
    case OP_AL:
    case OP_R8:
        Code->info.opcode &= NOT_W_BIT;         // clear w-bit
        break;
    case OP_CL: /* only appears in "shift opnd,CL" instructions */
        break;
    case OP_AX:
    case OP_DX: /* only appears in "in" and "out" instructions  */
    case OP_R16:
        Code->info.opcode |= W_BIT;             // set w-bit
        if( Code->use32 ) Code->prefix.opsiz = TRUE;
        break;
    case OP_MMX:
        break;
    case OP_XMM:
        break;
    case OP_ST:
        switch( AsmBuffer[i + 1]->token ) {
        case T_OP_BRACKET:
            i++;
            /* fall through ... "st 1" will be treated the same as "st(1)" */
        case T_NUM:
            i++;
            switch( AsmBuffer[i]->token ) {
            case T_NUM:
                temp = AsmBuffer[i]->value & BIT_012;
                Code->info.rm_byte |= temp;
                if( temp != 0 ) {
                    Code->info.opnd_type[Opnd_Count] = OP_ST_REG;
                }
                if( AsmBuffer[i+1]->token == T_CL_BRACKET ) {
                    // the next token should be ')'
                    i++;
                }
                break;
            default:
                AsmError( EXPECTING_NUMBER );
                return( ERROR );
            }
            break;
        case T_COMMA:
        case T_FINAL:
            break;
        default:
            AsmError( EXPECTING_NUMBER );
            return( ERROR );
        }
        break;
    case OP_SR3:                        // 386 segment register
        if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
            // 8086 ins cannot use 80386 segment register
            AsmError( CANNOT_USE_386_SEGMENT_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( ERROR );
        }
    case OP_SR:                                 // any seg reg
    case OP_SR2:                                // 8086 segment register
        if( AsmBuffer[i + 1]->token == T_COLON ) {
            /* segment override */
            Code->prefix.seg = AsmOpTable[temp].opcode;
            /* fixme - setup the frame here */
            return( i );
        }
        if( AsmBuffer[i]->value == T_CS ) {
            // POP CS is not allowed
            if( Code->info.token == T_POP ) {
                AsmError( POP_CS_IS_NOT_ALLOWED );
                return( ERROR );
            }
        }
        reg = get_sr_rm_byte( AsmOpTable[temp].opcode );
        break;
    case OP_EAX:
    case OP_R32:
        if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
            // 8086 ins cannot use 386 register
            AsmError( CANNOT_USE_386_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( ERROR );
        }
        Code->info.opcode |= W_BIT;             // set w-bit
        if( !Code->use32 ) Code->prefix.opsiz = TRUE;
        break;
    case OP_TR:                 // Test registers
        switch( AsmBuffer[i]->value ) {
        case T_TR3:
        case T_TR4:
        case T_TR5:
            if( ( (Code->info.cpu&P_CPU_MASK) < P_486
               || (Code->info.cpu&P_CPU_MASK) >= P_686 )
                &&  (AsmOpTable[temp].cpu&P_CPU_MASK) >= P_486 ) {
                // TR3, TR4, TR5 are available on 486 only
                AsmError( CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( ERROR );
            }
            break;
        case T_TR6:
        case T_TR7:
            if( ( (Code->info.cpu&P_CPU_MASK) < P_386
               || (Code->info.cpu&P_CPU_MASK) >= P_686 )
                &&  (AsmOpTable[temp].cpu&P_CPU_MASK) >= P_386 ) {
                // TR6, TR7 are available on 386...586 only
                AsmError( CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( ERROR );
            }
            break;
        }
    case OP_CR:                 // Control registers
    case OP_DR:                 // Debug registers
        if( Code->info.token != T_MOV ) {
            AsmError( ONLY_MOV_CAN_USE_SPECIAL_REGISTER );
            return( ERROR );
        }
        break;
    }
    if( Opnd_Count == OPND1 ) {
        // the first operand
        // r/m is treated as a 'reg' field
        Code->info.rm_byte |= MOD_11;
        // fill the r/m field
        Code->info.rm_byte |= reg;
        // special case for IMUL ( it has 3 opnds )
        if( Code->info.token == T_IMUL ) {
            /* if IMUL has 2 opnds only, and we are
               now parsing the second opnd, then fill bit-345 field */
            if( AsmBuffer[i + 1]->token == T_COMMA  &&  i+3 >= Token_Count ) {
                Code->info.rm_byte |= ( reg << 3 );
            }
        }
    } else {
        // the second operand
        if( ( Code->info.token == T_XCHG ) 
            && ( ( Code->info.opnd_type[OPND1] == OP_AX )
            || ( Code->info.opnd_type[OPND1] == OP_EAX ) ) ) {
            // XCHG can use short form if op1 is AX or EAX
            Code->info.rm_byte = ( Code->info.rm_byte & BIT_67 ) | reg;
        } else {
            // fill reg field with reg
            Code->info.rm_byte = ( Code->info.rm_byte & ~BIT_345 ) | ( reg << 3 );
        }
    }
    return( i );
}

#ifdef _WASM_

static int proc_check( void )
/***************************/
/* Check if we are inside a procedure and write prologue statements if the
   current line is the first instruction line following the procedure
   declaration */
{
    if( CurrProc == NULL || Token_Count == 0 || !DefineProc )
        return FALSE;

    if( AsmBuffer[0]->token == T_DIRECTIVE )
        return FALSE;

    if( Token_Count > 1 ) {
        if( AsmBuffer[1]->token == T_DIRECTIVE
            || AsmBuffer[1]->token == T_DIRECT_EXPR ) {
            return FALSE;
        }
    }

    /* 1st instruction following a procedure definition */
    if( WritePrologue() == ERROR )
        return( ERROR );
    InputQueueLine( CurrString );
    DefineProc = FALSE;
    return TRUE;
}

int check_override_x( expr_list *opndx )
/***************************************/
/* Check if there is a register, segment or group override */
{
    int         index;

    index = opndx->override;
    if( index == EMPTY )
        return( NOT_ERROR );
    switch( AsmBuffer[index]->token ) {
    case T_REG:
//        Code->prefix.seg = AsmOpTable[AsmOpcode[AsmBuffer[index]->value].position].opcode;
        break;
    case T_ID:      // Segment or Group override
        if( FixOverride(index) != NOT_ERROR )
            return( ERROR );
        break;
    default:
        break;
    }
    return( NOT_ERROR );
}

#endif

static int process_address( expr_list *opndx )
/********************************************/
/*
  parse the memory reference operand
*/
{
    char                ss = SCALE_FACTOR_1;
    int                 index = EMPTY;
    int                 base = EMPTY;
    struct asm_sym      *sym;
    struct asmfixup     *fixup;
    char                base_lock = FALSE;
    enum fixup_types    fixup_type;
    int                 flag;
#ifdef _WASM_
    int                 type;
    char                field_flag = 0;
#endif

    fixup = NULL;
    ConstantOnly = FALSE;

    Code->data[Opnd_Count] = opndx->value;
    sym = opndx->sym;

    if( opndx->instr != EMPTY ) {
        if( ( opndx->base_reg != EMPTY )
            || ( opndx->idx_reg != EMPTY ) ) {
            AsmError( INVALID_MEMORY_POINTER );
            return( ERROR );
        }
        switch( AsmBuffer[opndx->instr]->value ) {
        case T_SEG:
            if( sym->state == SYM_STACK ) {
                AsmError( CANNOT_SEG_AUTO );
                return( ERROR );
            }
            switch( Code->mem_type ) {
            case EMPTY:
                Code->mem_type = T_WORD;
                // no break
#ifdef _WASM__
            case T_SWORD:
#endif
            case T_WORD:
                Code->info.opnd_type[Opnd_Count] = OP_I16;
                break;
#ifdef _WASM_
            case T_SDWORD:
#endif
            case T_DWORD:
                Code->info.opnd_type[Opnd_Count] = OP_I32;
                break;
            }
            fixup_type = FIX_SEG;
            break;
        case T_OFFSET:
            if( MEM_TYPE( Code->mem_type, BYTE ) ) {
                AsmError( OFFSET_TOO_SMALL );
                return( ERROR );
            }
#ifdef _WASM_
            if( check_override_x( opndx ) == ERROR ) {
                return ERROR;
            }
#endif
            if( sym != NULL ) {
                if( sym->state == SYM_STACK ) {
                    AsmError( CANNOT_OFFSET_AUTO );
                    return( ERROR );
#ifdef _WASM_
                } else if( sym->state == SYM_GRP ) {
                    AsmError( CANNOT_OFFSET_GRP );
                    return( ERROR );
#endif
                }
            }

            if( MEM_TYPE( Code->mem_type, DWORD ) ) {
                fixup_type = 1;
            } else if( MEM_TYPE( Code->mem_type, WORD ) ) {
                fixup_type = 0;
            } else {
                fixup_type = Code->use32;
            }
            if( fixup_type ) {
                fixup_type = FIX_OFF32;
                Code->mem_type = T_DWORD;
                Code->info.opnd_type[Opnd_Count] = OP_I32;
            } else {
                fixup_type = FIX_OFF16;
                Code->mem_type = T_WORD;
                Code->info.opnd_type[Opnd_Count] = OP_I16;
            }
            break;
        }
        ConstantOnly = TRUE;
        Code->info.opcode |= W_BIT;
        if( sym == NULL )
            return( NOT_ERROR );
#ifdef _WASM_
        find_frame( sym );
#endif
        fixup = AddFixup( sym, fixup_type, OPTJ_NONE );
        if( fixup == NULL ) {
            return( ERROR );
        } else {
            return( NOT_ERROR );
        }
    } else {
        if( opndx->override != EMPTY ) {
            if( AsmBuffer[opndx->override]->token == T_REG ) {
                Code->prefix.seg = AsmOpTable[AsmOpcode[AsmBuffer[opndx->override]->value].position].opcode;
            } else {
#ifdef _WASM_
                if( check_override_x( opndx ) == ERROR ) {
                    return ERROR;
                }
#endif
            }
        } else {
        }
        Code->info.opnd_type[Opnd_Count] = OP_M;
        flag = ( opndx->explicit ) ? TRUE : FALSE ;
        if( ptr_operator( opndx->expr_type, flag ) == ERROR )
            return( ERROR );
        if( ptr_operator( T_PTR, flag ) == ERROR ) {
            return( ERROR );
        }
        if( opndx->base_reg != EMPTY ) {
            base = AsmBuffer[opndx->base_reg]->value;
            switch( base ) {     // check for base registers
            case T_EAX:
            case T_EBX:
            case T_ECX:
            case T_EDX:
            case T_ESP:
            case T_EBP:
            case T_ESI:
            case T_EDI:
                if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
                    // 286 and down cannot use 386 registers
                    AsmError( CANNOT_USE_386_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                    return( ERROR );
                }
                SET_ADRSIZ_32( Code );
                break;
            case T_BX:
            case T_BP:
            case T_SI:
            case T_DI:
                SET_ADRSIZ_16( Code );
                break;
            default:
                AsmError( INVALID_MEMORY_POINTER );
                return( ERROR );
            }
        }
        if( opndx->idx_reg != EMPTY ) {
            index = AsmBuffer[opndx->idx_reg]->value;
            switch( index ) {     // check for index registers
            case T_EAX:
            case T_EBX:
            case T_ECX:
            case T_EDX:
            case T_ESP:
            case T_EBP:
            case T_ESI:
            case T_EDI:
                if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
                    // 286 and down cannot use 386 registers
                    AsmError( CANNOT_USE_386_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                    return( ERROR );
                }
                SET_ADRSIZ_32( Code );
                break;
            case T_BX:
            case T_BP:
            case T_SI:
            case T_DI:
                SET_ADRSIZ_16( Code );
                break;
            default:
                AsmError( INVALID_MEMORY_POINTER );
                return( ERROR );
            }
            if( AsmBuffer[opndx->idx_reg]->value == T_ESP ) {
                if( opndx->scale == 1 ) {
                    index = base;
                    base = AsmBuffer[opndx->idx_reg]->value;
                } else {
                    AsmError( ESP_CANNOT_BE_USED_AS_INDEX );
                    return( ERROR );
                }
            }
            if( addr_32( Code ) ) {
                if( (Code->info.cpu&P_CPU_MASK) >= P_386 ) {
                    if( !Code->use32 )
                        Code->prefix.adrsiz = TRUE;
                    switch( index ) {
                    case T_ESP:
                    case T_BX:
                    case T_BP:
                    case T_SI:
                    case T_DI:
                        // cannot use ESP or 16-bit reg as index
                        AsmError( INVALID_INDEX_REGISTER );
                        return( ERROR );
                    default:
                        if( !Code->use32 )
                            Code->prefix.adrsiz = TRUE;
                        switch( opndx->scale ) {
                        case 1:
                            // ss = 00
                            break;
                        case 2:
                            // ss = 01
                            ss = SCALE_FACTOR_2;
                            break;
                        case 4:
                            // ss = 10
                            ss = SCALE_FACTOR_4;
                            break;
                        case 8:
                            // ss = 11
                            ss = SCALE_FACTOR_8;
                            break;
                        default: // must be * 1, 2, 4 or 8
                            AsmError( SCALE_FACTOR_MUST_BE_1_2_4_OR_8 );
                            return( ERROR );
                        }
                    }
                } else {
                    // 286 and down cannot use this memory mode
                    AsmError( INVALID_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                    return( ERROR );
                }
            }
        }
    }

    if( sym != NULL ) {
        switch( sym->state ) {
        case SYM_UNDEFINED:
            // forward reference
            break;
        case SYM_STACK:
            if( base != EMPTY ) {
                if( base_lock == TRUE ) {
                    // [reg + data][reg + data] is not allowed
                    AsmError( TOO_MANY_BASE_REGISTERS );
                    return( ERROR );
                } else {
                    index = base;
                }
            }
            if( Code->use32 ) {
                base = T_EBP;
            } else {
                base = T_BP;
            }
            base_lock = TRUE;   // add lock
            /* fall through */
        default:
            if( Code->mem_type == EMPTY ) {
                if( ptr_operator( sym->mem_type, FALSE ) == ERROR )
                    return( ERROR );
                if( ptr_operator( T_PTR, FALSE ) == ERROR ) {
                    return( ERROR );
                }
            }
            break;
        }

        fixup_type = FIX_OFF16; /* might be changed to OFF32 later */

#ifdef _WASM_
        switch( sym->state ) {
        case SYM_GRP:
        case SYM_SEG:
            ConstantOnly = TRUE;
            fixup_type = FIX_SEG;
            Code->info.opnd_type[Opnd_Count] = OP_I16;
            Code->info.opcode |= W_BIT;
            break;
        }
#endif
    }
    flag = 0;
    if( ( index != EMPTY ) || ( base != EMPTY ) ) {
        // indirect
        if( sym != NULL ) {
            // indirect with symbol
            flag = 1;
        } else {
            // indirect only reg + const
            flag = 2;
        }
    } else if( sym != NULL ) {
        // direct symbol
        flag = 3;
    } else {
        // constant
        flag = 4;
    }
    if( flag == 1 || flag == 2 || flag == 3 ) {
//    if( flag == 3 ) {
        if( IS_ANY_BRANCH( Code->info.token ) ) {
            int temp;
            /* might be a direct control transfer */
            temp = jmp( sym );
            switch( temp ) {
            case ERROR:
                return( ERROR );
            case SCRAP_INSTRUCTION:
                return( EMPTY );
            case INDIRECT_JUMP:
                break;
            default:
                if( flag == 1 || flag == 2 ) {
                    break;
                } else {
                    return( NOT_ERROR );
                }
            }
        }
    }
    if( sym != NULL ) {
        fixup = AddFixup( sym, fixup_type, OPTJ_NONE );
    }
#ifdef _WASM_
    type = 0;
    if( ( sym != NULL ) && ( index == EMPTY ) && ( base == EMPTY ) ) {
        switch( Code->mem_type ) {
        case T_NEAR:
        case T_FAR:
            type = 1;
            break;
        case EMPTY:
            switch( sym->mem_type ) {
            case T_NEAR:
            case T_FAR:
                type = 1;
                break;
            }
            break;
        }
        if( Code->info.token == T_LEA ) {
            type = 0;
        }
        if( type == 0 ) {
            if( !ConstantOnly ) {
                SET_ADRSIZ( Code, SymIs32( sym ));
            }
        }
    }
    if( fixup != NULL && fixup->fixup_type == FIX_OFF16 && addr_32( Code ) ) {
        fixup->fixup_type = FIX_OFF32;
    }
    if( Modend ) {
        if( sym == NULL ) {
            AsmError( SYNTAX_ERROR );
            return( ERROR );
        }
        GetAssume( sym, ASSUME_NOTHING );
    } else if( ConstantOnly ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else if( type == 0 ) {
        if( mem2code( ss, index, base, sym ) == ERROR ) {
            return( ERROR );
        }
    }
    if( type > 0 ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else if( sym != NULL ) {
        add_frame();
    } else if( field_flag ) {
        if( ConstantOnly ) {
            if( idata( Code->data[Opnd_Count] ) == ERROR ) {
                return( ERROR );
            }
        } else {
            if( ptr_operator( T_PTR, FALSE ) == ERROR ) {
                return( ERROR );
            }
        }
    }
#else
    if( fixup != NULL && fixup->fixup_type == FIX_OFF16 && addr_32( Code ) ) {
        fixup->fixup_type = FIX_OFF32;
    }
    if( ConstantOnly ) {
        if( idata( Code->data[Opnd_Count] ) == ERROR ) {
            return( ERROR );
        }
    } else {
        if( mem2code( ss, index, base, sym ) == ERROR ) {
            return( ERROR );
        }
    }
#endif
    return( NOT_ERROR );
}

static int process_const( expr_list *opndx )
/******************************************/
{
    if( ( Code->info.token == T_IMUL )
        && ( Code->info.opnd_type[OPND1] & OP_R ) ) {
        if( Opnd_Count == OPND2 ) {
            Code->info.rm_byte = ( Code->info.rm_byte & ~BIT_345 )
                          | ( ( Code->info.rm_byte & BIT_012 ) << 3 );
        } else if( Opnd_Count == OPND3 ) {
            Code->info.opnd_type[OPND1] = Code->info.opnd_type[OPND2];
            Code->info.opnd_type[OPND2] = OP_NONE;
            Code->data[OPND1] = Code->data[OPND2];
            Code->data[OPND2] = 0;
            InsFixups[OPND1] = InsFixups[OPND2];
            InsFixups[OPND2] = NULL;
            Opnd_Count = OPND2;
        }
    }
    Code->data[Opnd_Count] = opndx->value;
    Code->info.opnd_type[Opnd_Count] = OP_I;
    if( idata( Code->data[Opnd_Count] ) == ERROR )
        return( ERROR );
    return( NOT_ERROR );
}

static int process_reg( expr_list *opndx )
/****************************************/
/*
- parse and encode the register operand;
*/
{
    int                 temp;
    int                 reg;

    if( opndx->indirect )
        return( process_address( opndx ) );
    temp = AsmOpcode[AsmBuffer[opndx->base_reg]->value].position;
    reg = AsmOpTable[temp].opcode;
    Code->info.opnd_type[Opnd_Count] = AsmOpTable[temp].opnd_type[OPND2];
    switch( AsmOpTable[temp].opnd_type[OPND2] ) {
    case OP_AL:
    case OP_R8:
        Code->info.opcode &= NOT_W_BIT;         // clear w-bit
        break;
    case OP_CL: /* only appears in "shift opnd,CL" instructions */
        break;
    case OP_AX:
    case OP_DX: /* only appears in "in" and "out" instructions  */
    case OP_R16:
        Code->info.opcode |= W_BIT;             // set w-bit
        if( Code->use32 ) Code->prefix.opsiz = TRUE;
        break;
    case OP_MMX:
        break;
    case OP_XMM:
        break;
    case OP_ST:
        temp = opndx->idx_reg & BIT_012;
        Code->info.rm_byte |= temp;
        if( temp != 0 )
            Code->info.opnd_type[Opnd_Count] = OP_ST_REG;
        break;
    case OP_SR3:                        // 386 segment register
        if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
            // 8086 ins cannot use 80386 segment register
            AsmError( CANNOT_USE_386_SEGMENT_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( ERROR );
        }
    case OP_SR:                                 // any seg reg
    case OP_SR2:                                // 8086 segment register
#if 0    
        if( AsmBuffer[i + 1]->token == T_COLON ) {
            /* segment override */
            Code->prefix.seg = AsmOpTable[temp].opcode;
            /* fixme - setup the frame here */
            return( i );
        }
#endif        
        if( AsmBuffer[opndx->base_reg]->value == T_CS ) {
            // POP CS is not allowed
            if( Code->info.token == T_POP ) {
                AsmError( POP_CS_IS_NOT_ALLOWED );
                return( ERROR );
            }
        }
        reg = get_sr_rm_byte( AsmOpTable[temp].opcode );
        break;
    case OP_EAX:
    case OP_R32:
        if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
            // 8086 ins cannot use 386 register
            AsmError( CANNOT_USE_386_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( ERROR );
        }
        Code->info.opcode |= W_BIT;             // set w-bit
        if( !Code->use32 ) Code->prefix.opsiz = TRUE;
        break;
    case OP_TR:                 // Test registers
        switch( AsmBuffer[opndx->base_reg]->value ) {
        case T_TR3:
        case T_TR4:
        case T_TR5:
            if( ( (Code->info.cpu&P_CPU_MASK) < P_486
               || (Code->info.cpu&P_CPU_MASK) >= P_686 )
                &&  (AsmOpTable[temp].cpu&P_CPU_MASK) >= P_486 ) {
                // TR3, TR4, TR5 are available on 486 only
                AsmError( CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( ERROR );
            }
            break;
        case T_TR6:
        case T_TR7:
            if( ( (Code->info.cpu&P_CPU_MASK) < P_386
               || (Code->info.cpu&P_CPU_MASK) >= P_686 )
                &&  (AsmOpTable[temp].cpu&P_CPU_MASK) >= P_386 ) {
                // TR6, TR7 are available on 386...586 only
                AsmError( CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( ERROR );
            }
            break;
        }
    case OP_CR:                 // Control registers
    case OP_DR:                 // Debug registers
        if( Code->info.token != T_MOV ) {
            AsmError( ONLY_MOV_CAN_USE_SPECIAL_REGISTER );
            return( ERROR );
        }
        break;
    }
    if( Opnd_Count == OPND1 ) {
        // the first operand
        // r/m is treated as a 'reg' field
        Code->info.rm_byte |= MOD_11;
        // fill the r/m field
        Code->info.rm_byte |= reg;
    } else {
        // the second operand
        if( ( Code->info.token == T_XCHG ) 
            && ( ( Code->info.opnd_type[OPND1] == OP_AX )
            || ( Code->info.opnd_type[OPND1] == OP_EAX ) ) ) {
            // XCHG can use short form if op1 is AX or EAX
            Code->info.rm_byte = ( Code->info.rm_byte & BIT_67 ) | reg;
        } else {
            // fill reg field with reg
            Code->info.rm_byte = ( Code->info.rm_byte & ~BIT_345 ) | ( reg << 3 );
        }
    }
    return( NOT_ERROR );
}

int AsmParse( void )
/******************/
/*
- co-ordinate the parsing process;
- it is a basically a big loop to loop through all the tokens and identify them
  with the switch statement;
*/
{
    int                 i;
    int                 temp;
    char                *char_ptr;
    struct asm_sym      *sym = NULL;
    struct asmfixup     *fixup;
    enum operand_type   cur_opnd = OP_NONE;
    enum operand_type   last_opnd = OP_NONE;
    struct asm_code     *rCode = Code;
    int                 operator_loc;
    expr_list           opndx;

#ifdef _WASM_
    if( Use32 ) {
        Code->use32 = TRUE;
    } else {
        Code->use32 = FALSE;
    }
    i = proc_check();
    if( i == ERROR ) return( ERROR );
    if( i == TRUE ) return( NOT_ERROR );
#endif

    //init
    rCode->info.token   = T_NULL;
    rCode->info.opcode  = 0;
    rCode->info.rm_byte = 0;
    rCode->prefix.ins   = EMPTY;
    rCode->prefix.seg   = EMPTY;
    rCode->prefix.adrsiz = FALSE;
    rCode->prefix.opsiz = FALSE;
    rCode->mem_type     = EMPTY;
    rCode->mem_type_fixed = FALSE;
    rCode->extended_ins = EMPTY;
    rCode->sib          = 0;            // assume ss is *1
    rCode->indirect     = FALSE;
    for( i = 0; i < 3; i++ ) {
        rCode->info.opnd_type[i] = OP_NONE;
        rCode->data[i] = 0;
        InsFixups[i] = NULL;
    }
    Opnd_Count = 0;
    curr_ptr_type = EMPTY;

    // check if continue initializing array
    if( More_Array_Element == TRUE ) {
        // drop flag
        More_Array_Element = FALSE;
        // action
        return( dup_array( NULL, 0, Last_Element_Size ) );
    }

#ifdef _WASM_
    CheckSeg = TRUE;
    Frame = EMPTY;
    SegOverride = NULL;
#endif

    for( i = 0; i < Token_Count; i++ ) {
        switch( AsmBuffer[i]->token ) {
        case T_INSTR:
//            ExpandTheWorld( i, FALSE, TRUE );
#ifdef _WASM_
            if( ExpandAllConsts( i, FALSE ) == ERROR )
                return( ERROR );
#endif
            if( last_opnd != OP_NONE ) {
                // illegal operand is put before instruction
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            cur_opnd = OP_NONE;
            switch( AsmBuffer[i]->value ) {
            // prefix
            case T_LOCK:
            case T_REP:
            case T_REPE:
            case T_REPNE:
            case T_REPNZ:
            case T_REPZ:
                rCode->prefix.ins = AsmBuffer[i]->value;
                // prefix has to be followed by an instruction
                if( AsmBuffer[i+1]->token != T_INSTR ) {
                    AsmError( PREFIX_MUST_BE_FOLLOWED_BY_AN_INSTRUCTION );
                    return( ERROR );
                }
                continue;
#ifdef _WASM_
            case T_RET:
                if(( CurrProc != NULL ) && ( in_epilogue == 0 )) {
                    in_epilogue = 1;
                    return Ret( i, Token_Count, FALSE );
                }
            case T_RETN:
            case T_RETF:
                in_epilogue = 0;
                rCode->info.token = AsmBuffer[i]->value;
                break;
            case T_IRET:
            case T_IRETD:
                if(( CurrProc != NULL ) && ( in_epilogue == 0 )) {
                    in_epilogue = 1;
                    return Ret( i, Token_Count, TRUE );
                }
            case T_IRETF:
            case T_IRETDF:
                in_epilogue = 0;
                rCode->info.token = AsmBuffer[i]->value;
                break;
#endif
            default:
                rCode->info.token = AsmBuffer[i]->value;
                break;
            }
#ifdef _WASM_
            if( AsmBuffer[i+1]->token == T_DIRECTIVE ) {
                AsmBuffer[i]->token = T_ID;
                i--;
                continue;
            }
#endif
            i++;
            if( EvalOperand( &i, Token_Count, &opndx, TRUE ) == ERROR )
                return( ERROR );
            if( opndx.empty )
                break;
            switch( opndx.type ) {
            case EXPR_ADDR:
                if( process_address( &opndx ) == EMPTY )
                    return( SCRAP_INSTRUCTION );
                break;
            case EXPR_CONST:
                process_const( &opndx );
                break;
            case EXPR_REG:
                process_reg( &opndx );
                break;
            }
            i--;
            break;
        case T_UNARY_OPERATOR:
            operator_loc = i;
            if( MEM_TYPE( rCode->mem_type, BYTE ) ) {
                AsmError( OFFSET_TOO_SMALL );
                return( ERROR );
            }
#ifdef _WASM_
            i++;
            if( check_override( &i ) == ERROR ) {
                return ERROR;
            }
            i--;
#endif
            if( i + 1 < Token_Count ) {
                if( AsmBuffer[++i]->token == T_ID ) {
                    sym = AsmLookup( AsmBuffer[i]->string_ptr );
                    if( sym == NULL ) return ERROR;

                    switch( AsmBuffer[operator_loc]->value ) {
                    case T_OFFSET:
                        if( sym->state == SYM_STACK ) {
                            AsmError( CANNOT_OFFSET_AUTO );
                            return( ERROR );
#ifdef _WASM_
                        } else if( sym->state == SYM_GRP ) {
                            AsmError( CANNOT_OFFSET_GRP );
                            return( ERROR );
#endif
                        }
                        break;
                    case T_SEG:
                        if( sym->state == SYM_STACK ) {
                            AsmError( CANNOT_SEG_AUTO );
                        }
                    }

                    switch( AsmBuffer[operator_loc]->value ) {
                    case T_OFFSET:
                    case T_SEG:
                        if( AsmBuffer[operator_loc]->value == T_OFFSET ) {
                            int use32 = rCode->use32;

                            if( MEM_TYPE( rCode->mem_type, DWORD ) ) {
                                use32 = 1;
                            } else if( MEM_TYPE( rCode->mem_type, WORD ) ) {
                                use32 = 0;
                            }
                            if( use32 ) {
                                temp = FIX_OFF32;
                                rCode->mem_type = T_DWORD;
                            } else {
                                temp = FIX_OFF16;
                                rCode->mem_type = T_WORD;
                            }
                        } else {
                            temp = FIX_SEG;
                        }
#ifdef _WASM_
                        if( sym->state != SYM_STRUCT_FIELD ) {
                            find_frame( sym );
#endif
                            fixup = AddFixup( sym, temp, OPTJ_NONE );
                            if( fixup == NULL ) return( ERROR );
#ifdef _WASM_
                        }
#endif
                        // fall through
#ifdef _WASM_
                    case T_LENGTH:
                    case T_LENGTHOF:
                    case T_SIZE:
                    case T_SIZEOF:
#endif
                        switch( rCode->mem_type ) {
                        case EMPTY:
                            rCode->mem_type = T_WORD;
                            // no break
#ifdef _WASM__
                        case T_SWORD:   // 20-Aug-92
#endif
                        case T_WORD:
                            rCode->info.opcode |= W_BIT;
                            rCode->info.opnd_type[Opnd_Count] = OP_I16;
                            break;
#ifdef _WASM_
                        case T_SDWORD:  // 20-Aug-92
#endif
                        case T_DWORD:
                            rCode->info.opcode |= W_BIT;
                            rCode->info.opnd_type[Opnd_Count] = OP_I32;
                            break;
                        }
                    }

                    switch( AsmBuffer[operator_loc]->value ) {
                    /* specifics */
                    case T_OFFSET:
#ifdef _WASM_
                        if( sym->state == SYM_STRUCT_FIELD ) {
                            Code->data[Opnd_Count] = sym->offset;
                        }
                        break;
#endif
                    case T_SEG:
                        break;
#ifdef _WASM_
                    case T_LENGTH:
                        Code->data[Opnd_Count] = sym->first_length;
                        break;
                    case T_LENGTHOF:
                        Code->data[Opnd_Count] = sym->total_length;
                        break;
                    case T_SIZE:
                        Code->data[Opnd_Count] = sym->first_size;
                        break;
                    case T_SIZEOF:
                        Code->data[Opnd_Count] = sym->total_size;
                        break;
#endif
                    default:
                        AsmError( SYNTAX_ERROR );
                        return( ERROR );
                    }

                } else {
                    AsmError( SYNTAX_ERROR );
                    return( ERROR );
                }
            }
            cur_opnd = OP_I;
            break;
        case T_RES_ID:
            if( rCode->info.token == T_NULL ) {
                int name;
                if( i == 0 ) {
                    name = -1;
                } else if( i == 1 ) {
                    name = 0;
                } else if( i == 2 ) {
                    name = 0;
                }
                return( ( data_init( name, i ) == ERROR ) ? ERROR : NOT_ERROR );
            } else if( last_opnd != OP_NONE && last_opnd != OP_M ) {
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            if( AsmBuffer[i]->value == T_PTR ) {
                if( AsmBuffer[i - 1]->token == T_RES_ID ) {
                    if( AsmBuffer[i - 1]->value != T_PTR ) {
                        if( ptr_operator( T_PTR, TRUE ) == ERROR ) {
                            return( ERROR );
                        }
                        break;
                    }
                }
                // find 'ptr' but no 'byte', 'word' etc in front of it
                AsmError( NO_SIZE_GIVEN_BEFORE_PTR_OPERATOR );
                return( ERROR );
            } else if( ptr_operator( AsmBuffer[i]->value, TRUE ) == ERROR ) {
                return( ERROR );
            }
            cur_opnd = OP_NONE;
            break;
        case T_DIRECTIVE:
            return( directive( i, AsmBuffer[i]->value ) );
            break;
#ifdef _WASM_
        case T_DIRECT_EXPR:
            if( Parse_Pass != PASS_1 ) {
                Modend = TRUE;
                if( Token_Count > 1 ) {
                    if( mem( i+1 ) == ERROR ) {
                        return ERROR;
                    }
                }
            }
            return( directive( i, AsmBuffer[i]->value ) );
            break;
#endif
        case T_ID:
#ifdef _WASM_
            if( !( AsmBuffer[i+1]->token == T_DIRECTIVE &&
                ( AsmBuffer[i+1]->value == T_EQU ||
                  AsmBuffer[i+1]->value == T_EQU2 ||
                  AsmBuffer[i+1]->value == T_TEXTEQU ) ) ) {
                switch( ExpandSymbol( i, FALSE ) ) {
                case ERROR:
                    return( ERROR );
                case STRING_EXPANDED:
                    // restart token processing
                    i--;
                    continue;
                }
            }
#endif
            // if( last_opnd != OP_NONE && last_opnd != OP_M_ANY )
            if( last_opnd != OP_NONE && last_opnd != OP_M && last_opnd != OP_I ) {
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            if( i == 0 ) {   // a new label
#if ALLOW_STRUCT_INIT
#ifdef _WASM_
                sym = AsmGetSymbol( AsmBuffer[i]->string_ptr );
                if( sym != NULL && sym->state == SYM_STRUCT &&
                    AsmBuffer[i+1]->token != T_DIRECTIVE ) {
                    AsmBuffer[i]->token = T_DIRECTIVE;
                    AsmBuffer[i]->value = T_STRUCT;
                    return( ( data_init( -1,0 ) == ERROR ) ? ERROR : NOT_ERROR );
                }
#endif
#endif

                switch( AsmBuffer[i + 1]->token ) {
                case T_COLON:
                    cur_opnd = OP_LABEL;
                    break;
#if ALLOW_STRUCT_INIT
#ifdef _WASM_
                case T_ID:
                    /* structure declaration */
                    sym = AsmGetSymbol( AsmBuffer[i+1]->string_ptr );
                    if( sym == NULL || sym->state != SYM_STRUCT ) {
                        AsmError( SYNTAX_ERROR );
                        return( ERROR );
                    } else {
                        AsmBuffer[i + 1]->token = T_DIRECTIVE;
                        AsmBuffer[i + 1]->value = T_STRUCT;
                    }
#endif
#endif
                case T_RES_ID:
                    if( data_init( i, i+1 ) == ERROR ) {
                        return( ERROR );
                    } else {
                        return( NOT_ERROR );
                    }
                    break;
#ifdef _WASM_
                case T_DIRECTIVE:
                    return( directive( i+1, AsmBuffer[i+1]->value ) );
                    break;
#endif
                default:
                    AsmError( SYNTAX_ERROR );
                    return( ERROR );
                }
            } else {
                // data label
#ifdef _WASM_
                if( check_override( &i ) == ERROR ) {
                    return ERROR;
                }
                if( AsmBuffer[i]->token != T_ID ) {
                    AsmError( SYNTAX_ERROR );
                    return( ERROR );
                }
#endif
                temp = mem( i );
                if( temp == ERROR ) {
                    return( ERROR );
                } else if( temp == EMPTY ) {
                    return( SCRAP_INSTRUCTION );
                } else {
                    i = temp;
                }
                cur_opnd = OP_NONE;
                if( rCode->info.opnd_type[Opnd_Count] == OP_NONE ) {
                    rCode->info.opnd_type[Opnd_Count] = OP_M;
                }
            }
            break;
        case T_STRING:
            if( last_opnd != OP_NONE ) {
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            if( AsmBuffer[i]->value > ( (rCode->info.cpu&P_CPU_MASK) >= P_386 ? BYTE_4 : BYTE_2 ) ) {
                AsmError( IMMEDIATE_DATA_TOO_BIG );
                return( ERROR );
            } else {
                unsigned len;
                unsigned long int value = 0;

                len = AsmBuffer[i]->value;
                temp = 0;
                char_ptr = AsmBuffer[i]->string_ptr;
                while( temp < len ) {
                    value <<= 8;
                    value |= ( *char_ptr );
                    temp++;
                    char_ptr++;
                }
                if( idata( value ) == ERROR ) {
                    return( ERROR );
                }
            }
            break;
        case T_COMMA:
            cur_opnd = OP_NONE;
            curr_ptr_type = EMPTY;
            if( Opnd_Count == OPND2 ) {
                switch( rCode->info.token ) {
                case T_SHLD:
                case T_SHRD:
                    switch( AsmBuffer[i + 1]->token ) {
                    case T_NUM:
                        break;
                    case T_REG:
                        i++;
                        if( AsmBuffer[i]->value == T_CL ) {
                            break;
                        }
                    default:
                        AsmError( INVALID_SHLD_SHRD_FORMAT );
                        return( ERROR );
                    }
                    break;
                case T_NULL:
                    break;
                default:
                    Opnd_Count++;
                    break;
//                    AsmError( TOO_MANY_COMMAS );
//                    return( ERROR );
                }
            } else if( Opnd_Count == OPND3 ) {
                AsmError( TOO_MANY_COMMAS );
                return( ERROR );
            } else {
                Opnd_Count++;
                #ifdef _WASM_
                    Frame = EMPTY;
                    SegOverride = NULL;
                #endif
            }
            i++;
            if( EvalOperand( &i, Token_Count, &opndx, TRUE ) == ERROR )
                return( ERROR );
            if( opndx.empty )
                break;
            switch( opndx.type ) {
            case EXPR_ADDR:
                if( process_address( &opndx ) == EMPTY )
                    return( SCRAP_INSTRUCTION );
                break;
            case EXPR_CONST:
                process_const( &opndx );
                break;
            case EXPR_REG:
                process_reg( &opndx );
                break;
            }
            i--;
            break;
        case T_COLON:
            if( rCode->info.opnd_type[Opnd_Count] & OP_SR ) {
                // rCode->info.opnd_type[Opnd_Count] = OP_NONE;
                rCode->info.opnd_type[Opnd_Count] = OP_M;
                cur_opnd = OP_M;
            } else if ( last_opnd == OP_LABEL ) {
                if( AsmBuffer[i+1]->token != T_RES_ID ) {
                    if( MakeLabel( AsmBuffer[i-1]->string_ptr, T_NEAR )==ERROR ) {
                         return( ERROR );
                    }
                }
                cur_opnd = OP_NONE;
            } else if( rCode->info.token == T_CALLF ||
                       rCode->info.token == T_JMPF ) {
                if( AsmBuffer[i - 1]->token == T_NUM ) {
                    // last opnd is segment
                    rCode->info.opnd_type[Opnd_Count] = OP_I16;
                    Opnd_Count++;
                    cur_opnd = OP_NONE;
                } else {
                    AsmError( SYNTAX_ERROR_UNEXPECTED_COLON );
                    return( ERROR );
                }
            } else {
                AsmError( SYNTAX_ERROR_UNEXPECTED_COLON );
                return( ERROR );
            }
            break;
        case T_DOT:
            switch( last_opnd ) {
            case OP_M:
            case OP_I:
                break;
            case OP_NONE:
                return( directive( i, AsmBuffer[i]->value ) );
            default :
                AsmError( UNKNOWN_DIRECTIVE );
                return( ERROR );
            }
            break;
        case T_OP_SQ_BRACKET:
            i++;
            /* fall through */
        case T_NUM:
            switch( last_opnd ) {
            case OP_I8:
            case OP_I16:
            case OP_I32:
            case OP_J32:
            case OP_J48:
            case OP_NONE:
            case OP_M:
            case OP_I: // fixme - is this necessary?
                temp = mem( i );
                if( temp == EMPTY ) {
                    return( SCRAP_INSTRUCTION );
                } else if( temp == ERROR ) {
                    // if i is ERROR , cur_opnd is ERROR
                    return( ERROR );
                } else {
                    i = temp;
                    cur_opnd = OP_NONE;
                    if( ConstantOnly ) {
                        // 20-Aug-92
                        if( idata( rCode->data[Opnd_Count] ) == ERROR ) {
                            return( ERROR );
                        }
                    } else {
                        rCode->info.opnd_type[Opnd_Count] = OP_M;
                    }
                }
                break;
            default :
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            break;
        case T_FLOAT:
            if( idata_float( AsmBuffer[i]->value ) == ERROR ) {
                return( ERROR );
            }
            if( AsmBuffer[i-1]->token == T_MINUS ) {
                rCode->data[Opnd_Count] *= -1;
            }
#ifdef _WASM_
            AsmWarn( 4, FLOAT_OPERAND );
#endif
            break;
        case T_CL_SQ_BRACKET:
            break;
        case T_REG:
            switch( last_opnd ) {
            case OP_NONE:
                if( ( i = reg( i ) ) != ERROR ) {
                    cur_opnd = OP_R;
                } else {
                    // if i is ERROR , cur_opnd is ERROR
                    return( ERROR );
                }
                break;
            default:
                AsmError( SYNTAX_ERROR );
                return( ERROR );
            }
            break;
#ifdef _WASM_
        case T_PERCENT:
            /* expansion operator */
            cur_opnd = OP_NONE;
            ExpandTheWorld( i, FALSE, TRUE );
            break;
#endif
        case T_NOOP:
            break;
        }
        last_opnd = cur_opnd;
    }
    switch( rCode->info.token ) {
    case T_LODS:
    case T_SCAS:
    case T_STOS:
        SizeString( OperandSize( Code->info.opnd_type[OPND1] ) );
        break;
    }
#if 0
    if( match_phase_1() == ERROR ) {
        if( Opnd_Count > OPND1 ) {
            check_size();
        }
        return( ERROR );
    }
    return( NOT_ERROR );
#else
    if( Opnd_Count > OPND1 ) {
        if( check_size() == ERROR ) {
            return( ERROR );
        }
    }
    return( match_phase_1() );
#endif
}

static void SizeString( unsigned op_size )
/****************************************/
{
    /* size an string instruction based on it's operand's size */
    switch( op_size ) {
    case 1:
        Code->mem_type = T_BYTE;
        Code->info.opcode &= NOT_W_BIT;
        if( Code->use32 ) Code->prefix.opsiz = FALSE;
        break;
    case 2:
        Code->mem_type = T_WORD;
        Code->info.opcode |= W_BIT;
        Code->prefix.opsiz = Code->use32 ? TRUE : FALSE;
        break;
    case 4:
        Code->mem_type = T_DWORD;
        Code->info.opcode |= W_BIT;
        Code->prefix.opsiz = Code->use32 ? FALSE : TRUE;
        break;
    }
}

static int check_size( void )
/***************************/
/*
- use to make sure the size of first operand match the size of second operand;
- optimize MOV instruction;
*/
{
    enum operand_type   op1 = Code->info.opnd_type[OPND1];
    enum operand_type   op2 = Code->info.opnd_type[OPND2];
    int                 state = NOT_ERROR;
    int                 temp;
    int                 op1_size;
    int                 op2_size;
    int                 op_size = 0;

    switch( Code->info.token ) {
#if 0
    case T_PSLLW:
    case T_PSLLD:
    case T_PSLLQ:
    case T_PSRLW:
    case T_PSRLD:
    case T_PSRLQ:
    case T_PSRAW:
    case T_PSRAD:
        // check was wrong - instructions take a m64 OR an 8 bit immediate
        if( op2 & OP_I ) {
            op_size = OperandSize( op2 );
            if( op_size >= 2 ) {
                AsmError( OP_2_TOO_BIG );
                state = ERROR;
            }
        }
        break;
#endif
    case T_IN:
        if( op2 == OP_DX ) {
            switch( op1 ) {
            case OP_AX:
                break;
            case OP_AL:
                Code->info.opcode &= NOT_W_BIT;         // clear w-bit
            case OP_EAX:
                if( Code->use32 ) Code->prefix.opsiz = FALSE;
            }
        }
        break;
    case T_OUT:
        if( op1 == OP_DX ) {
            switch( op2 ) {
            case OP_AX:
                break;
            case OP_AL:
                Code->info.opcode &= NOT_W_BIT;         // clear w-bit
            case OP_EAX:
                if( Code->use32 ) Code->prefix.opsiz = FALSE;
            }
        }
        break;
    case T_INS:
    case T_CMPS:
        op_size = OperandSize( op1 );
        /* fall through */
    case T_MOVS:
    case T_OUTS:
        if( op_size == 0 ) op_size = OperandSize( op2 );

        SizeString( op_size );
        break;
    case T_LEA:
        switch( OperandSize( op1 ) ) {
        case 2:
        case 4:
            break;
        default:
            AsmError( OPERANDS_MUST_BE_THE_SAME_SIZE );
            state = ERROR;
        }
        break;
    case T_RCL:
    case T_RCR:
    case T_ROL:
    case T_ROR:
    case T_SAL:
    case T_SAR:
    case T_SHL:
    case T_SHR:
    case T_LDS:
    case T_LES:
    case T_LFS:
    case T_LGS:
    case T_LSS:
        // checking will be done later
        break;
    case T_ENTER:
        // ENTER has to be OP_I16, OP_I8
        if( op1 == OP_I32 ) {
            //parse_phase_1 will treat 16-bit data as OP_I32 if CPU is 386
            if( Code->data[OPND1] > (signed long)USHRT_MAX ) {
                // if op1 is really 32-bit data, then error
                AsmError( IMMEDIATE_DATA_TOO_BIG );
                state = ERROR;
            }
        }
        // type cast op1 to OP_I16
        Code->info.opnd_type[OPND1] = OP_I16;
        // op2 have to be 8-bit data
        if( op2 >= OP_I16 ) {
            if( Code->data[OPND2] > UCHAR_MAX ) {
                AsmError( IMMEDIATE_DATA_TOO_BIG );
                state = ERROR;
            }
            Code->info.opnd_type[OPND2] = OP_I8;
        }
        break;
    case T_CALLF:
    case T_JMPF:
        // segment can only be 16-bit
        if( op1 > OP_I16 ) {
            AsmError( SEGMENT_TOO_BIG );
            state = ERROR;
        }
        if( (Code->info.cpu&(P_CPU_MASK|P_PM)) <= P_286p ) {
        // offset can only be 16-bit if CPU is 286 and down
            if( op2 > OP_I16 ) {
            AsmError( OFFSET_TOO_BIG );
            state = ERROR;
        }
        }
        // swap the 2 opnds to make output easier
        if( InsFixups[OPND2] != NULL ) {
            // absolute segment + offset nnnn,offset
            Code->info.opnd_type[OPND1] = Code->info.opnd_type[OPND2];
            InsFixups[OPND1] = InsFixups[OPND2];
            InsFixups[OPND2] = NULL;
        } else {
            // absolute ptr nnnn,nnnn
            Code->info.opnd_type[OPND1] = Code->use32 ? OP_I32 : OP_I16;
        }
        temp = Code->data[OPND1];
        Code->data[OPND1] = Code->data[OPND2];
        Code->data[OPND2] = temp;
        Code->info.opnd_type[OPND2] = OP_I16;
        break;
    case T_MOVSX:
    case T_MOVZX:
        Code->info.opcode &= NOT_W_BIT;
        op2_size = OperandSize( op2 );
        switch( OperandSize( op1 ) ) {
        case 4:
            switch( op2_size ) {
            case 2:
                Code->info.opcode |= W_BIT;
                break;
            case 4:
                AsmError( OP_2_TOO_BIG );
                state = ERROR;
            }
            if( Code->use32 ) {
                Code->prefix.opsiz = FALSE;     // - don't need opnd size prefix
            }
            break;
        case 2:
            if( op2_size >= 2 ) {
                AsmError( OP_2_TOO_BIG );
                state = ERROR;
            }
            break;
        default:
            // op1 have to be r16/r32
            AsmError( OP_1_TOO_SMALL );
            state = ERROR;
        }
        break;
    case T_LSL:                                 /* 19-sep-93 */
        op1_size = OperandSize( op1 );
        switch( op1_size ) {
        case 2:
            if( Code->use32 ) Code->prefix.opsiz = TRUE;
            break;
        case 4:
            if( Code->use32 ) Code->prefix.opsiz = FALSE;
            break;
        default:
            AsmError( INVALID_SIZE );
            return( ERROR );
        }
        op2_size = OperandSize( op2 );
        switch( op2_size ) {
        case 2:
        case 4:
            break;
        default:
            AsmError( INVALID_SIZE );
            state = ERROR;
            break;
        }
        break;
    case T_CVTSD2SI:
    case T_CVTTSD2SI:
    case T_CVTSS2SI:
    case T_CVTTSS2SI:
    case T_MOVNTI:
        break;
    case T_MOVD:
#if 0    
        op1_size = OperandSize( op1 );
        op2_size = OperandSize( op2 );
        if( op1_size != 0 && op1_size != 4 || op2_size != 0 && op2_size != 4 ) {
            AsmError( OPERANDS_MUST_BE_THE_SAME_SIZE );
            state = ERROR;
        }
#endif        
        break;
    case T_MOV:
        // to optimize MOV
        if(  op1 == OP_M || op2 == OP_M ) {
            temp = Code->info.rm_byte;
            if( Code->info.opnd_type[OPND1] & OP_A ) {
                temp = ( temp & BIT_67 ) | ( ( temp & BIT_012 ) << 3 ) | ( ( temp & BIT_345 ) >> 3 );
                if( ( addr_32( Code ) && temp == D32 ) ||
                    ( !addr_32( Code ) && temp == D16 ) ) {
                    // DS:[d32] or DS:[d16] can use MOV Mem with Acc (short form)
                } else {
                    // we have to change OP_A to OP_R
                    Code->info.opnd_type[OPND1] &= ~OP_A;
                }
            } else if( Code->info.opnd_type[OPND2] & OP_A ) {
                if( ( addr_32( Code ) && temp == D32 ) ||
                    ( !addr_32( Code ) && temp == D16 ) ) {
                    // DS:[d32] or DS:[d16] can use MOV Mem with Acc (short form)
                    temp = EMPTY;
                } else {
                    // we have to change OP_A to OP_R
                    Code->info.opnd_type[OPND2] &= ~OP_A;
                }
            }
        } else if( ( op1 & OP_SR ) && ( op2 & OP_R32 ) ) {
            return( state );
        } else if( ( op1 & OP_R32 ) && ( op2 & OP_SR ) ) {
            return( state );
        } else if( ( op1 & OP_SPEC_REG ) || ( op2 & OP_SPEC_REG ) ) {
            Code->prefix.opsiz = FALSE;
            return( state );
        }
        // no break;
    default:
        // make sure the 2 opnds are of the same type
        op1_size = OperandSize( op1 );
        op2_size = OperandSize( op2 );
        if( op1_size > op2_size ) {
            if( op2 >= OP_I8  &&  op2 <= OP_I32 ) {     /* immediate */
                op2_size = op1_size;    /* promote to larger size */
            }
        }
        if( op1_size == 1 && op2 == OP_I16 && Code->data[OPND2] <= UCHAR_MAX ) {
            return( state ); // OK cause no sign extension
        }
        if( op1_size == 2 && op2 == OP_I32 && Code->data[OPND2] <= USHRT_MAX ) {
            return( state ); // OK cause no sign extension
        }
        if( op1_size != op2_size ) {
            /* if one or more are !defined, set them appropriately */
            if( ( op1 | op2 ) & ( OP_MMX | OP_XMM ) ) {
            } else if( op1_size != 0  &&  op2_size != 0 ) {
                AsmError( OPERANDS_MUST_BE_THE_SAME_SIZE );
//                state = ERROR;
            }
            if( op1_size == 0 ) {
                if( ( op1 & OP_M_ANY ) && ( op2 & OP_I ) ) {
                    if( (unsigned long)Code->data[OPND2] > USHRT_MAX ) {
                         Code->mem_type = T_DWORD;
                         Code->info.opcode |= W_BIT;
                         Code->info.opnd_type[OPND2] = OP_I32;
#ifdef _WASM_
                         if( Parse_Pass == PASS_1 ) {
                             AsmWarn( 1, ASSUMING_DWORD );
                         }
#endif
                    } else if( (unsigned long)Code->data[OPND2] > UCHAR_MAX ) {
                         Code->mem_type = T_WORD;
                         Code->info.opcode |= W_BIT;
                         Code->info.opnd_type[OPND2] = OP_I16;
#ifdef _WASM_
                         if( Parse_Pass == PASS_1 ) {
                             AsmWarn( 1, ASSUMING_WORD );
                         }
#endif
                    } else {
                         Code->mem_type = T_BYTE;
                         Code->info.opnd_type[OPND2] = OP_I8;
#ifdef _WASM_
                         if( Parse_Pass == PASS_1 ) {
                             AsmWarn( 1, ASSUMING_BYTE );
                         }
#endif
                    }
                } else if( ( op1 & OP_M_ANY ) && ( op2 & ( OP_R | OP_SR ) ) ) {
                } else if( ( op1 & ( OP_MMX | OP_XMM ) ) && ( op2 & OP_I ) ) {
                    if( (unsigned long)Code->data[OPND2] > USHRT_MAX ) {
                         Code->info.opnd_type[OPND2] = OP_I32;
                    } else if( (unsigned long)Code->data[OPND2] > UCHAR_MAX ) {
                         Code->info.opnd_type[OPND2] = OP_I16;
                    } else {
                         Code->info.opnd_type[OPND2] = OP_I8;
                    }
                } else if( ( op1 | op2 ) & ( OP_MMX | OP_XMM ) ) {
                } else {
#ifdef _WASM_
                    AsmIntErr( 1 );
#endif
                    switch( op2_size ) {
                    case 1:
                        Code->mem_type = T_BYTE;
                        #ifdef _WASM_
                            if( Parse_Pass == PASS_1 && ( op2 & OP_I ) ) {
                                AsmWarn( 1, ASSUMING_BYTE );
                            }
                        #endif
                        break;
                    case 2:
                        Code->mem_type = T_WORD;
                        Code->info.opcode |= W_BIT;
                        #ifdef _WASM_
                            if( Parse_Pass == PASS_1 && ( op2 & OP_I ) ) {
                                AsmWarn( 1, ASSUMING_WORD );
                            }
                        #endif
                        if( Code->use32 ) Code->prefix.opsiz = TRUE;
                        break;
                    case 4:
                        Code->mem_type = T_DWORD;
                        Code->info.opcode |= W_BIT;
                        #ifdef _WASM_
                            if( Parse_Pass == PASS_1 && ( op2 & OP_I ) ) {
                                AsmWarn( 1, ASSUMING_DWORD );
                            }
                        #endif
                        break;
                    }
                }
            }
        }
    }
    return( state );
}

void AsmInit( int cpu, int fpu, int use32, int extn )
/***************************************************/
{
    int         pos = 0;
    enum asm_token  token_value = 1;
    int         size = sizeof( AsmOpTable ) / sizeof( AsmOpTable[0] );
    int         count;

    for( count = 0; count < MAX_TOKEN; count ++ ) {
        AsmBuffer[count] = &tokens[count];
    }

    if( use32 < 0 ) use32 = 0; // default is 16-bit segment
    if( cpu < 0 ) cpu = 0;     // default is 8086 CPU
    if( fpu < 0 ) fpu = 1;     // default is FPU use
    if( extn < 0 ) extn = 0;   // default is no CPU extension instructions
    switch( use32 ) {
    case 0:
        Code->use32 = 0;
        break;
    case 1:
        Code->use32 = 1;
        break;
    }
    switch( cpu ) {
    case 0:
        Code->info.cpu |= P_86;
        if( fpu ) Code->info.cpu |= P_87;
        break;
    case 1:
        Code->info.cpu |= P_186;
        if( fpu ) Code->info.cpu |= P_87;
        break;
    case 2:
        Code->info.cpu |= P_286p;
        if( fpu ) Code->info.cpu |= P_287;
        break;
    case 3:
        Code->info.cpu |= P_386p;
        if( fpu ) Code->info.cpu |= P_387;
        break;
    case 4:
        Code->info.cpu |= P_486p;
        if( fpu ) Code->info.cpu |= P_387;
        break;
    case 5:
        Code->info.cpu |= P_586p;
        if( fpu ) Code->info.cpu |= P_387;
        if( extn ) Code->info.cpu |= P_K3D | P_MMX;
        break;
    case 6:
        Code->info.cpu |= P_686p;
        if( fpu ) Code->info.cpu |= P_387;
        if( extn ) Code->info.cpu |= P_K3D | P_MMX | P_SSE | P_SSE2 | P_SSE3;
        break;
    }

    // initialize AsmOpcode table to point to entry in AsmOpTable
    // token has its own value, e.g. T_AAA is 0, T_ADD is 1, etc.
    
    if( AsmOpcode[1].position == 0 ) {  // if not initialized
        while( AsmOpcode[token_value].len != 0 ) {
            do {
                pos++;
            } while ( AsmOpTable[pos].token != token_value && pos <= size );
            if( pos > size ) {
                AsmError( INTERNAL_ERROR_1 );
                exit( -1 );
            }
            AsmOpcode[token_value].position = pos;
            token_value++;
        }
        make_inst_hash_table();
    }
}
