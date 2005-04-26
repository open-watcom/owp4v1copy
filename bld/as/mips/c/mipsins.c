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
* Description:  MIPS instruction opcodes.
*
****************************************************************************/


#include "as.h"

static bool insErrFlag = FALSE;    // to tell whether we had problems or not

#define INS( a, b, c, d, e ) { a, b, c, d, e, NULL }

ins_table MIPSTable[] = {
 /* INS( name,      opcode, fncode, template,               method ), */
 // Memory Format Instructions
    INS( "lb",      0x20,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lh",      0x21,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lwl",     0x22,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lw",      0x23,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lbu",     0x24,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lhu",     0x25,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lwr",     0x26,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "sb",      0x28,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "sh",      0x29,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "swl",     0x2a,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "sw",      0x2b,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "swr",     0x2d,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),

    INS( "lda",     0x08,   0x00,   IT_MEMORY_LDA,          ENUM_NONE ),
    INS( "ldah",    0x09,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "ldf",     0x20,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "ldg",     0x21,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "ldl",     0x28,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "ldl_l",   0x2A,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "ldq",     0x29,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "ldq_l",   0x2B,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "ldq_u",   0x0B,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "lds",     0x22,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "ldt",     0x23,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "stf",     0x24,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "stg",     0x25,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "stl",     0x2C,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "stl_c",   0x2E,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "stq",     0x2D,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "stq_c",   0x2F,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "stq_u",   0x0F,   0x00,   IT_MEMORY_ALL,          ENUM_NONE ),
    INS( "sts",     0x26,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
    INS( "stt",     0x27,   0x00,   IT_FP_MEMORY_ALL,       ENUM_NONE ),
 // Memory Format Instructions with a function code
    INS( "fetch",   0x18,   0x80,   IT_MEMORY_B,            ENUM_NONE ),
    INS( "fetch_m", 0x18,   0xA0,   IT_MEMORY_B,            ENUM_NONE ),
    INS( "mb",      0x18,   0x40,   IT_MEMORY_NONE,         ENUM_NONE ),
    INS( "wmb",     0x18,   0x44,   IT_MEMORY_NONE,         ENUM_NONE ),
    INS( "rc",      0x18,   0xE0,   IT_MEMORY_A,            ENUM_NONE ),
    INS( "rpcc",    0x18,   0xC0,   IT_MEMORY_A,            ENUM_NONE ),
    INS( "rs",      0x18,   0xF0,   IT_MEMORY_A,            ENUM_NONE ),
    INS( "trapb",   0x18,   0x00,   IT_MEMORY_NONE,         ENUM_NONE ),
    INS( "excb",    0x18,   0x04,   IT_MEMORY_NONE,         ENUM_NONE ),
 // Jump Instructions
    INS( "j",       0x02,   0x00,   IT_JUMP,                ENUM_NONE ),
    INS( "jal",     0x03,   0x00,   IT_JUMP,                ENUM_NONE ),
    INS( "jr",      0x00,   0x08,   IT_REG_JUMP,            ENUM_NONE ),
    INS( "jalr",    0x00,   0x09,   IT_REG_JUMP,            ENUM_NONE ),
 // Memory Branch Instructions
    INS( "ret",     0x1A,   0x02,   IT_RET,                 ENUM_NONE ),
 // Branch Format Instructions
    INS( "beq",     0x04,   0x00,   IT_BRANCH_TWO,          ENUM_NONE ),
    INS( "bne",     0x05,   0x00,   IT_BRANCH_TWO,          ENUM_NONE ),

    INS( "br",      0x30,   0x00,   IT_BR,                  ENUM_NONE ),
    INS( "fbeq",    0x31,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "fblt",    0x32,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "fble",    0x33,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "bsr",     0x34,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "fbne",    0x35,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "fbge",    0x36,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "fbgt",    0x37,   0x00,   IT_FP_BRANCH,           ENUM_NONE ),
    INS( "blbc",    0x38,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "blt",     0x3A,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "ble",     0x3B,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "blbs",    0x3C,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "bge",     0x3E,   0x00,   IT_BRANCH,              ENUM_NONE ),
    INS( "bgt",     0x3F,   0x00,   IT_BRANCH,              ENUM_NONE ),
 // Operate Format Instructions
    INS( "add",     0x00,   0x20,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "addu",    0x00,   0x21,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "sub",     0x00,   0x22,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "subu",    0x00,   0x23,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "and",     0x00,   0x24,   IT_OPERATE,             ENUM_NONE ),
    INS( "or",      0x00,   0x25,   IT_OPERATE,             ENUM_NONE ),
    INS( "nor",     0x00,   0x27,   IT_OPERATE,             ENUM_NONE ),
    INS( "xor",     0x00,   0x26,   IT_OPERATE,             ENUM_NONE ),
    INS( "slt",     0x00,   0x2a,   IT_OPERATE,             ENUM_NONE ),
    INS( "addi",    0x08,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "addiu",   0x09,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "andi",    0x0c,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "ori",     0x0d,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "xori",    0x0e,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "sll",     0x00,   0x00,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "srl",     0x00,   0x02,   IT_OPERATE_IMM,         ENUM_NONE ),
    INS( "sllv",    0x00,   0x04,   IT_OPERATE,             ENUM_NONE ),
    INS( "srlv",    0x00,   0x06,   IT_OPERATE,             ENUM_NONE ),
    INS( "srav",    0x00,   0x07,   IT_OPERATE,             ENUM_NONE ),

    INS( "cmpbge",  0x10,   0x0F,   IT_OPERATE,             ENUM_NONE ),
    INS( "cmpeq",   0x10,   0x2D,   IT_OPERATE,             ENUM_NONE ),
    INS( "subl",    0x10,   0x09,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "subq",    0x10,   0x29,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "bic",     0x11,   0x08,   IT_OPERATE,             ENUM_NONE ),
    INS( "bis",     0x11,   0x20,   IT_OPERATE,             ENUM_NONE ),
    INS( "mull",    0x13,   0x00,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "mulq",    0x13,   0x20,   IT_OPERATE,             ENUM_OF_ADDL ),
    INS( "umulh",   0x13,   0x30,   IT_OPERATE,             ENUM_NONE ),
 // Two-operand operate instructions
    INS( "mult",    0x00,   0x18,   IT_MUL_DIV,             ENUM_NONE ),
    INS( "multu",   0x00,   0x19,   IT_MUL_DIV,             ENUM_NONE ),
    INS( "div",     0x00,   0x1a,   IT_MUL_DIV,             ENUM_NONE ),
    INS( "divu",    0x00,   0x1b,   IT_MUL_DIV,             ENUM_NONE ),
 // Moves to/from special registers
    INS( "mfhi",    0x00,   0x10,   IT_MOV_SPECIAL,         ENUM_NONE ),
    INS( "mthi",    0x00,   0x11,   IT_MOV_SPECIAL,         ENUM_NONE ),
    INS( "mflo",    0x00,   0x12,   IT_MOV_SPECIAL,         ENUM_NONE ),
    INS( "mtlo",    0x00,   0x13,   IT_MOV_SPECIAL,         ENUM_NONE ),
 // Moves to/from FPU
    INS( "mfc1",    0x11,   0x00,   IT_MOV_FP,              ENUM_NONE ),
    INS( "mtc1",    0x11,   0x04,   IT_MOV_FP,              ENUM_NONE ),

 // Floating-Point Operate Format Instructions - Data Type Independent
    INS( "cpys",    0x17,   0x20,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "cpyse",   0x17,   0x22,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "cpysn",   0x17,   0x21,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "cvtlq",   0x17,   0x10,   IT_FP_CONVERT,          ENUM_NONE ),
    INS( "cvtql",   0x17,   0x30,   IT_FP_CONVERT,          ENUM_DTI_CVTQL ),
    INS( "fcmoveq", 0x17,   0x2A,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "fcmovge", 0x17,   0x2D,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "fcmovgt", 0x17,   0x2F,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "fcmovle", 0x17,   0x2E,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "fcmovlt", 0x17,   0x2C,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "fcmovne", 0x17,   0x2B,   IT_FP_OPERATE,          ENUM_NONE ),
    INS( "mf_fpcr", 0x17,   0x25,   IT_MT_MF_FPCR,          ENUM_NONE ),
    INS( "mt_fpcr", 0x17,   0x24,   IT_MT_MF_FPCR,          ENUM_NONE ),
 // Floating-Point Operate Format Instructions - IEEE
    INS( "adds",    0x16,   0x80,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "addt",    0x16,   0xA0,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "cmpteq",  0x16,   0xA5,   IT_FP_OPERATE,          ENUM_IEEE_CMPTEQ ),
    INS( "cmptlt",  0x16,   0xA6,   IT_FP_OPERATE,          ENUM_IEEE_CMPTEQ ),
    INS( "cmptle",  0x16,   0xA7,   IT_FP_OPERATE,          ENUM_IEEE_CMPTEQ ),
    INS( "cmptun",  0x16,   0xA4,   IT_FP_OPERATE,          ENUM_IEEE_CMPTEQ ),
    INS( "cvtqs",   0x16,   0xBC,   IT_FP_CONVERT,          ENUM_IEEE_CVTQS ),
    INS( "cvtqt",   0x16,   0xBE,   IT_FP_CONVERT,          ENUM_IEEE_CVTQS ),
    INS( "cvtts",   0x16,   0xAC,   IT_FP_CONVERT,          ENUM_IEEE_ADDS ),
    INS( "divs",    0x16,   0x83,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "divt",    0x16,   0xA3,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "muls",    0x16,   0x82,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "subs",    0x16,   0x81,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "subt",    0x16,   0xA1,   IT_FP_OPERATE,          ENUM_IEEE_ADDS ),
    INS( "cvttq",   0x16,   0xAF,   IT_FP_CONVERT,          ENUM_IEEE_CVTTQ ),

    INS( "mov.s",   0x10,   0x06,   IT_FP_CONVERT,          ENUM_NONE ),
    INS( "mov.d",   0x11,   0x06,   IT_FP_CONVERT,          ENUM_NONE ),
 // Instruction with optional embedded code
    INS( "syscall", 0x00,   0x0C,   IT_SYSCODE,             ENUM_NONE ),
    INS( "break",   0x00,   0x0D,   IT_SYSCODE,             ENUM_NONE ),
 // Stylized Code Forms
    // nop is in directiv.c since it doesn't require much parsing
    INS( "clr",     0x11,   0x20,   IT_PSEUDO_CLR,          ENUM_NONE ),
    INS( "fclr",    0x17,   0x20,   IT_PSEUDO_FCLR,         ENUM_NONE ),
    INS( "fmov",    0x17,   0x20,   IT_PSEUDO_FMOV,         ENUM_NONE ),
    INS( "negl",    0x10,   0x09,   IT_PSEUDO_NOT,          ENUM_OF_ADDL ),
    INS( "negq",    0x10,   0x29,   IT_PSEUDO_NOT,          ENUM_OF_ADDL ),
    INS( "negs",    0x16,   0x81,   IT_PSEUDO_NEGF,         ENUM_IEEE_ADDS ),
    INS( "negt",    0x16,   0xA1,   IT_PSEUDO_NEGF,         ENUM_IEEE_ADDS ),
    INS( "fneg",    0x17,   0x21,   IT_PSEUDO_FNEG,         ENUM_NONE ),
    INS( "fnegf",   0x17,   0x21,   IT_PSEUDO_FNEG,         ENUM_NONE ),
    INS( "fnegg",   0x17,   0x21,   IT_PSEUDO_FNEG,         ENUM_NONE ),
    INS( "fnegs",   0x17,   0x21,   IT_PSEUDO_FNEG,         ENUM_NONE ),
    INS( "fnegt",   0x17,   0x21,   IT_PSEUDO_FNEG,         ENUM_NONE ),
    INS( "not",     0x11,   0x28,   IT_PSEUDO_NOT,          ENUM_NONE ),
    INS( "andnot",  0x11,   0x08,   IT_OPERATE,             ENUM_NONE ),
    INS( "xornot",  0x11,   0x48,   IT_OPERATE,             ENUM_NONE ),
    INS( "fabs",    0x17,   0x20,   IT_PSEUDO_NEGF,         ENUM_NONE ),
    INS( "sextl",   0x10,   0x00,   IT_PSEUDO_NOT,          ENUM_NONE ),

    INS( "move",    0x00,   0x25,   IT_PSEUDO_MOV,          ENUM_NONE ),

 // The following pseudo-instructions might emit multiple real instructions
    INS( "li",      0x00,   0x00,   IT_PSEUDO_LIMM,         ENUM_NONE ),
    INS( "la",      0x00,   0x00,   IT_PSEUDO_LADDR,        ENUM_NONE ),
    // abs pseudo ins (opcode & funccode are from subl/v, subq/v)
    INS( "absl",    0x10,   0x49,   IT_PSEUDO_ABS,          ENUM_NONE ),
    INS( "absq",    0x10,   0x69,   IT_PSEUDO_ABS,          ENUM_NONE ),
};

#define MAX_NAME_LEN    20  // maximum length of a MIPS instruction mnemonic (TODO)

static void addInstructionSymbol( qualifier_flags flags, ins_table *table_entry )
//*******************************************************************************
// Given an instruction name for which the optional bits in flags
// are turned on, add a symbol for it to the symbol table.
{
    sym_handle  sym;
    ins_symbol  *entry;
    char        buffer[MAX_NAME_LEN];

    strcpy( buffer, table_entry->name );
    if( flags ) {
        strcat( buffer, "/" );
        if( flags & QF_S ) strcat( buffer, "s" );

        if( flags & QF_U ) strcat( buffer, "u" );   // u & v are mutually
        if( flags & QF_V ) strcat( buffer, "v" );   // exclusive

        if( flags & QF_I ) strcat( buffer, "i" );

        if( flags & QF_C ) strcat( buffer, "c" );   // c, m, & d are
        if( flags & QF_M ) strcat( buffer, "m" );   // mutually exclusive
        if( flags & QF_D ) strcat( buffer, "d" );
    }

    entry = MemAlloc( sizeof( ins_symbol ) );
    entry->table_entry = table_entry;
    entry->flags = flags;

    // link it into our list of symbols for this table entry
    entry->next = table_entry->symbols;
    table_entry->symbols = entry;

    sym = SymAdd( buffer, SYM_INSTRUCTION );
    SymSetLink( sym, (void *)entry );
}

static void enum_NONE( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//***********************************************************************************************************************************
{
    method = method;
    level = level;
    mask = mask;
    func( QF_NONE, parm );
}

static void enum_OF_ADDL( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//**************************************************************************************************************************************
{
    method = method;
    level = level;
    mask = mask;
    func( QF_NONE, parm );
    func( QF_V, parm );
}

static void enum_DTI_CVTQL( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//****************************************************************************************************************************************
{
    method = method;
    level = level;
    mask = mask;
    func( QF_NONE, parm );
    func( QF_V, parm );
    func( QF_S | QF_V, parm );
}

static void enum_IEEE_CMPTEQ( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//******************************************************************************************************************************************
{
    method = method;
    level = level;
    mask = mask;
    func( QF_NONE, parm );
    func( QF_S | QF_U, parm );
}

static void enum_IEEE_CVTQS( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//*****************************************************************************************************************************************
{
    assert( level < 2 );
    switch( level ) {
    case 0:
        enum_IEEE_CVTQS( method, mask, level + 1, func, parm );
        mask |= ( QF_S | QF_U | QF_I );
        enum_IEEE_CVTQS( method, mask, level + 1, func, parm );
        break;
    case 1:
        func( mask, parm );
        mask |= QF_C;
        func( mask, parm );
        mask &= ~QF_C;
        mask |= QF_M;
        func( mask, parm );
        mask &= ~QF_M;
        mask |= QF_D;
        func( mask, parm );
        break;
    }
}

static void enum_IEEE_ADDS_or_CVTTQ( ins_enum_method method, uint_32 mask, uint_8 level, void (*func)( qualifier_flags, ins_table * ), void *parm )
//*************************************************************************************************************************************************
{
    assert( level < 4 );
    switch( level ) {
    case 0:
        enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        mask |= QF_S;
        enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        break;
    case 1:
        if( !mask ) {
            enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        }
        mask |= ( ( method == ENUM_IEEE_ADDS ) ? QF_U : QF_V );
        enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        break;
    case 2:
        enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        if( mask & QF_S ) {
            mask |= QF_I;
            enum_IEEE_ADDS_or_CVTTQ( method, mask, level + 1, func, parm );
        }
        break;
    case 3:
        func( mask, parm );
        mask |= QF_C;
        func( mask, parm );
        mask &= ~QF_C;
        mask |= QF_M;
        func( mask, parm );
        mask &= ~QF_M;
        mask |= QF_D;
        func( mask, parm );
        break;
    }
}

typedef void (*enumFunc_t)( ins_enum_method, uint_32, uint_8, void (*func)( qualifier_flags, ins_table * ), void * );

#define PICK( a, b )    b,
static enumFunc_t enumFunc[] = {
#include "insenum.inc"
};
#undef PICK

static void enumInstructions( ins_enum_method method, void (*func)( qualifier_flags set, ins_table *parm ), void *parm )
//**********************************************************************************************************************
// Depending on which enum_method it belongs to, different instruction-
// enumeration functions will be called to generate all the possible
// instructions with the different qualifiers attached.
{
    enumFunc[method]( method, QF_NONE, 0, func, parm );
}

#ifdef _STANDALONE_
#ifndef NDEBUG
static char *itStrings[] = {
#define PICK( a, b, c, d, e ) #a,
#include "mipsfmt.inc"
#undef PICK
};

extern void DumpITString( ins_template template )
{
    printf( itStrings[template] );
}

static char *insEnumStrings[] = {
#define PICK( a, b ) #a,
#include "insenum.inc"
#undef PICK
};

extern void DumpInsEnumMethod( ins_enum_method method )
//*****************************************************
{
    printf( insEnumStrings[method] );
}

extern void DumpInsTableEntry( ins_table *table_entry )
//*****************************************************
{
    ins_symbol  *symbol;

    printf( "%s: 0x%x(0x%x) ", table_entry->name, table_entry->opcode, table_entry->funccode );
    DumpITString( table_entry->template );
    printf( ", " );
    DumpInsEnumMethod( table_entry->method );
    printf( "\n\tSymbol entries: " );
    symbol = table_entry->symbols;
    while( symbol != NULL ) {
        printf( " %x", symbol );
        symbol = symbol->next;
    }
    printf( "\n" );
}

extern void DumpInsTables()
//*************************
{
    ins_table   *curr;
    int         i, n;

    n = sizeof( MIPSTable ) / sizeof( MIPSTable[0] );
    for( i = 0; i < n; i++ ) {
        curr = &MIPSTable[i];
        DumpInsTableEntry( curr );
    }
}
#endif
#endif

extern void InsInit()
//*******************
{
    ins_table   *curr;
    int         i, n;

    n = sizeof( MIPSTable ) / sizeof( MIPSTable[0] );
    for( i = 0; i < n; i++ ) {
        curr = &MIPSTable[i];
        /* for each possible symbol generated by this instruction, add a symbol table entry */
        enumInstructions( curr->method, addInstructionSymbol, curr );
    }
#ifndef NDEBUG
    #ifdef _STANDALONE_
    if( _IsOption( DUMP_INS_TABLE ) ) DumpInsTables();
    #endif
#endif
}

extern instruction *InsCreate( sym_handle op_sym )
//************************************************
// Allocate an instruction and initialize it.
{
    instruction *ins;

    ins = MemAlloc( sizeof( instruction ) );
    ins->opcode_sym = op_sym;
    ins->format = SymGetLink( op_sym );
    ins->num_operands = 0;
    return( ins );
}

extern void InsAddOperand( instruction *ins, ins_operand *op )
//************************************************************
// Add an operand to the given instruction.
{
    if( ins->num_operands == MAX_OPERANDS ) {
        if( !insErrFlag ) {
            Error( MAX_NUMOP_EXCEEDED );
            insErrFlag = TRUE;
        }
        MemFree( op );
        return;
    }
    if( insErrFlag) insErrFlag = FALSE;
    ins->operands[ins->num_operands++] = op;
}

extern void InsEmit( instruction *ins )
//*************************************
// Check an instruction to make sure operands match
// and encode it. The encoded instruction is emitted
// to the current OWL section.
{
#ifndef NDEBUG
    #ifdef _STANDALONE_
    if( _IsOption( DUMP_INSTRUCTIONS ) ) {
        DumpIns( ins );
    }
    #endif
#endif
    if( insErrFlag == FALSE && MIPSValidate( ins ) ) {
#ifdef _STANDALONE_
        MIPSEmit( CurrentSection, ins );
#else
        MIPSEmit( ins );
#endif
    }
}

extern void InsDestroy( instruction *ins )
//****************************************
// Free up an instruction and all operands which
// are hanging off of it.
{
    int         i;

    for( i = 0; i < ins->num_operands; i++ ) {
        MemFree( ins->operands[i] );
    }
    MemFree( ins );
}

extern void InsFini()
//*******************
{
    ins_table   *curr;
    ins_symbol  *next;
    ins_symbol  *entry;
    int         i, n;
    extern instruction *AsCurrIns; // from as.y

    if( AsCurrIns != NULL ) {
        InsDestroy( AsCurrIns );
        AsCurrIns = NULL;
    }
    n = sizeof( MIPSTable ) / sizeof( MIPSTable[0] );
    for( i = 0; i < n; i++ ) {
        curr = &MIPSTable[i];
        for( entry = curr->symbols; entry != NULL; entry = next ) {
            next = entry->next;
            MemFree( entry );
        }
        curr->symbols = NULL;   // need to reset this pointer
    }
}

#ifdef _STANDALONE_
#ifndef NDEBUG
extern void DumpIns( instruction *ins )
//*************************************
{
    int         i;

    printf( "%-11s", SymName( ins->opcode_sym ) );
    for( i = 0; i < ins->num_operands; i++ ) {
        if( i != 0 ) printf( ", " );
        DumpOperand( ins->operands[i] );
    }
    printf( "\n" );
}
#endif
#endif
