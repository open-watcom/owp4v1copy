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
* Description:  Instruction decoding for MIPS R4000 architecture.
*
****************************************************************************/


#include <string.h>
#include <ctype.h>
#include "distypes.h"
#include "dis.h"

extern long SEX( unsigned long v, unsigned bit );

extern const dis_range          MIPSRangeTable[];
extern const int                MIPSRangeTablePos[];
extern const unsigned char      MIPSMaxInsName;

typedef union {
    unsigned_32     full;
#ifdef __BIG_ENDIAN__
    struct {
        unsigned_32 op          : 6;
        unsigned_32 rs          : 5;
        unsigned_32 rt          : 5;
        unsigned_32 immediate   : 16;
    }           itype;
    struct {
        unsigned_32 op          : 6;
        unsigned_32 target      : 26;
    }           jtype;
    struct {
        unsigned_32 op          : 6;
        unsigned_32 rs          : 5;
        unsigned_32 rt          : 5;
        unsigned_32 rd          : 5;
        unsigned_32 sa          : 5;
        unsigned_32 funct       : 6;
    }           rtype;
    struct {
        unsigned_32 op          : 6;
        unsigned_32 code        : 20;
        unsigned_32 funct       : 6;
    }           break_t;
    struct {
        unsigned_32 op          : 6;
        unsigned_32 rs          : 5;
        unsigned_32 rt          : 5;
        unsigned_32 code        : 10;
        unsigned_32 op2         : 6;
    }           trap_t;
#else
    struct {
        unsigned_32 immediate   : 16;
        unsigned_32 rt          : 5;
        unsigned_32 rs          : 5;
        unsigned_32 op          : 6;
    }           itype;
    struct {
        unsigned_32 target      : 26;
        unsigned_32 op          : 6;
    }           jtype;
    struct {
        unsigned_32 funct       : 6;
        unsigned_32 sa          : 5;
        unsigned_32 rd          : 5;
        unsigned_32 rt          : 5;
        unsigned_32 rs          : 5;
        unsigned_32 op          : 6;
    }           rtype;
    struct {
        unsigned_32 funct       : 6;
        unsigned_32 code        : 20;
        unsigned_32 op          : 6;
    }           break_t;
    struct {
        unsigned_32 op2         : 6;
        unsigned_32 code        : 10;
        unsigned_32 rt          : 5;
        unsigned_32 rs          : 5;
        unsigned_32 op          : 6;
    }           trap_t;
#endif
} mips_ins;


dis_handler_return MIPSNull( dis_handle *h, void *d, dis_dec_ins *ins )
{
    ins->num_ops = 0;
    return( DHR_DONE );
}

dis_handler_return MIPSJType( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_IMMED;
    ins->op[0].value = code.jtype.target;
    ins->num_ops = 1;
    return( DHR_DONE );
}

dis_handler_return MIPSCode( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_IMMED;
    ins->op[0].value = code.break_t.code;
    ins->num_ops = 1;
    return( DHR_DONE );
}

dis_handler_return MIPSImmed1( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rt + DR_MIPS_r0;
    ins->op[1].type = DO_IMMED;
    ins->op[1].value = code.itype.immediate;
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSImmed2( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rt + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.itype.rs + DR_MIPS_r0;
    ins->op[2].type = DO_IMMED;
    ins->op[2].value = SEX( code.itype.immediate, 15 );
    ins->num_ops = 3;
    return( DHR_DONE );
}

dis_handler_return MIPSShift( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rd + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.rtype.rt + DR_MIPS_r0;
    ins->op[2].type = DO_IMMED;
    ins->op[2].value = code.rtype.sa;
    ins->num_ops = 3;
    return( DHR_DONE );
}

dis_handler_return MIPSTrap1( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rs + DR_MIPS_r0;
    ins->op[1].type = DO_IMMED;
    ins->op[1].value = SEX( code.itype.immediate, 15 );
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSTrap2( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.trap_t.rs + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.trap_t.rt + DR_MIPS_r0;
// What do we do with 'code'?
//    ins->op[2].type = DO_IMMED;
//    ins->op[2].value = code.trap_t.code;
//    ins->num_ops = 3;
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSReg1( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rd + DR_MIPS_r0;
    ins->num_ops = 1;
    return( DHR_DONE );
}

dis_handler_return MIPSReg2( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rt + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.rtype.rd + DR_MIPS_r0;
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSReg3( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rd + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.rtype.rs + DR_MIPS_r0;
    ins->op[2].type = DO_REG;
    ins->op[2].base = code.rtype.rt + DR_MIPS_r0;
    ins->num_ops = 3;
    return( DHR_DONE );
}

dis_handler_return MIPSMulDiv( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rs + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.rtype.rt + DR_MIPS_r0;
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSCache( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_IMMED;
    ins->op[0].value = code.itype.rt;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.itype.rt + DR_MIPS_r0;
    ins->op[2].type = DO_MEMORY_ABS;
    ins->op[2].value = SEX( code.itype.immediate, 15 );
    ins->op[2].base = code.itype.rs + DR_MIPS_r0;
    ins->num_ops = 3;
    return( DHR_DONE );
}

dis_handler_return MIPSMemory( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rt + DR_MIPS_r0;
    ins->op[1].type = DO_MEMORY_ABS;
    ins->op[1].value = SEX( code.itype.immediate, 15 );
    ins->op[1].base = code.itype.rs + DR_MIPS_r0;
    ins->num_ops = 2;
#if 0
    switch( code.memory.opcode & 0x0b ) {
    case 0x00:
        ins->op[1].ref_type = DRT_AXP_FFLOAT;
        break;
    case 0x01:
        ins->op[1].ref_type = DRT_AXP_GFLOAT;
        break;
    case 0x02:
        ins->op[1].ref_type = DRT_AXP_SFLOAT;
        break;
    case 0x03:
        ins->op[1].ref_type = DRT_AXP_TFLOAT;
        break;
    case 0x08:
    case 0x0a:
        ins->op[1].ref_type = DRT_AXP_LWORD;
        break;
    case 0x09:
    case 0x0b:
        ins->op[1].ref_type = DRT_AXP_QWORD;
        break;
    }
#endif
    return( DHR_DONE );
}

dis_handler_return MIPSJump1( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rs + DR_MIPS_r0;
    ins->num_ops = 1;
    return( DHR_DONE );
}

dis_handler_return MIPSJump2( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.rtype.rd + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.rtype.rs + DR_MIPS_r0;
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSBranch1( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rs + DR_MIPS_r0;
    ins->op[1].type = DO_RELATIVE;
    ins->op[1].value = (SEX( code.itype.immediate, 15 ) + 1) * sizeof( unsigned_32 );
    ins->num_ops = 2;
    return( DHR_DONE );
}

dis_handler_return MIPSBranch2( dis_handle *h, void *d, dis_dec_ins *ins )
{
    mips_ins    code;

    code.full = ins->opcode;
    ins->op[0].type = DO_REG;
    ins->op[0].base = code.itype.rs + DR_MIPS_r0;
    ins->op[1].type = DO_REG;
    ins->op[1].base = code.itype.rt + DR_MIPS_r0;
    ins->op[2].type = DO_RELATIVE;
    ins->op[2].value = (SEX( code.itype.immediate, 15 ) + 1) * sizeof( unsigned_32 );
    ins->num_ops = 3;
    return( DHR_DONE );
}

static unsigned MIPSInsHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, char *name )
{
    const char  *new;

    if( !(flags & DFF_PSEUDO) ) return( 0 );
    new = NULL;
    switch( ins->type ) {
    case DI_MIPS_SLL:
        if( ins->op[0].base == DR_MIPS_r0 ) {
            new = "nop";
            ins->num_ops = 0;
        }
        break;
    case DI_MIPS_OR:
        if( ins->op[2].base == DR_MIPS_r0 ) {
            new = "move";
            ins->num_ops = 2;
        }
        break;
    case DI_MIPS_ADDIU:
        if( ins->op[1].base == DR_MIPS_r0 ) {
            new = "li";
            ins->op[1].type  = ins->op[2].type;
            ins->op[1].value = ins->op[2].value;
            ins->num_ops = 2;
        }
        break;
    case DI_MIPS_JALR:
        if( ins->op[0].base == DR_MIPS_r31 ) {
            ins->op[0].type = ins->op[1].type;
            ins->op[0].base = ins->op[1].base;
            ins->num_ops = 1;
        }
        break;
    default:
        break;
    }
    if( name != NULL && new != NULL ) {
        strcpy( name, new );
        return( strlen( name ) );
    }
    return( 0 );
}

static unsigned MIPSFlagHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, char *name )
{
#if 0
    char        *p;

    p = name;
    if( ins->flags != DIF_NONE ) {
        *p++ = '/';
        if( ins->flags & DIF_AXP_C ) *p++ = 'c';
        if( ins->flags & DIF_AXP_D ) *p++ = 'd';
        if( ins->flags & DIF_AXP_I ) *p++ = 'i';
        if( ins->flags & DIF_AXP_M ) *p++ = 'm';
        if( ins->flags & DIF_AXP_S ) *p++ = 's';
        if( ins->flags & DIF_AXP_U ) *p++ = 'u';
        if( ins->flags & DIF_AXP_V ) *p++ = 'v';
        *p = '\0';
    }
    return( p - name );
#else
    return( NULL );
#endif
}

static unsigned MIPSOpHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, unsigned op_num, char *op_buff )
{
    dis_operand *op;

    if( flags & DFF_SYMBOLIC_REG ) {
        op = &ins->op[op_num];
        if( op->base >= DR_MIPS_r0 && op->base <= DR_MIPS_r31 ) {
            op->base += DR_MIPS_zero - DR_MIPS_r0;
        }
        if( op->index >= DR_MIPS_r0 && op->index <= DR_MIPS_r31 ) {
            op->index += DR_MIPS_zero - DR_MIPS_r0;
        }
    }
    if( flags & DFF_ASM ) {
        op = &ins->op[op_num];
        if( op->base >= DR_MIPS_r0 && op->base <= DR_MIPS_ra ) {
            op->base += DR_MIPS_ar0 - DR_MIPS_r0;
        }
        if( op->index >= DR_MIPS_r0 && op->base <= DR_MIPS_ra ) {
            op->index += DR_MIPS_ar0 - DR_MIPS_r0;
        }
    }
    return( 0 );
}

static dis_handler_return MIPSDecodeTableCheck( int page, dis_dec_ins *ins )
{
    return( DHR_DONE );
}

static void ByteSwap( dis_handle *h, void *d, dis_dec_ins *ins )
{
    if( h->need_bswap ) {
        SWAP_32( ins->opcode );
    }
}

static void MIPSPreprocHook( dis_handle *h, void *d, dis_dec_ins *ins )
{
    ByteSwap( h, d, ins );
}

static unsigned MIPSPostOpHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, unsigned op_num, char *op_buff )
{
    // Nothing to do
    return( 0 );
}

const dis_cpu_data MIPSData = {
    MIPSRangeTable, MIPSRangeTablePos, MIPSPreprocHook, MIPSDecodeTableCheck, MIPSInsHook, MIPSFlagHook, MIPSOpHook, MIPSPostOpHook, &MIPSMaxInsName, 4
};
