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
* Description:  MIPS specific branch/jump instruction encoding.
*
****************************************************************************/


#include "standard.h"
#include "coderep.h"
#include "opcodes.h"
#include "pattern.h"
#include "ocentry.h"
#include "mipsenc.h"
#include "reloc.h"
#include "offset.h"
#include "optopts.h"
#include "optlbl.h"

extern void ObjBytes( const char *, unsigned );
extern void OutReloc( pointer, owl_reloc_type, unsigned );


void EncodeRet( oc_ret *oc )
/**************************/
{
    mips_ins            encoding;

    oc = oc;
    // 'jr ra'
    encoding = _Opcode( 0 ) | _Rs( MIPS_RETURN_ADDR ) | _Function( 0x08 );
    ObjBytes( (char *)&encoding, sizeof( encoding ) );
    // TODO: Handle delay slot better
    encoding = MIPS_NOP;
    ObjBytes( (char *)&encoding, sizeof( encoding ) );
}


static void doBranch( mips_ins opcode, pointer lbl, uint reg )
/************************************************************/
{
    opcode = _Opcode( opcode ) | _Rs( reg ) | _Rt( MIPS_ZERO_SINK );
    OutReloc( lbl, OWL_RELOC_BRANCH_REL, 0 );
    ObjBytes( (char *)&opcode, sizeof( opcode ) );
    // TODO: Handle delay slot better
    opcode = MIPS_NOP;
    ObjBytes( (char *)&opcode, sizeof( opcode ) );
}


void EncodeJump( oc_handle *oc )
/******************************/
{
    doBranch( 0x04, oc->handle, MIPS_ZERO_SINK );
}


void EncodeCall( oc_handle *oc )
/******************************/
{
    mips_ins            encoding;

    // TODO: PIC call
    // Call to absolute address
    // 'jal target'
    encoding = _Opcode( 0x03 );
    OutReloc( oc->handle, OWL_RELOC_JUMP_ABS, 0 );
    ObjBytes( (char *)&encoding, sizeof( encoding ) );
    // TODO: Handle delay slot better
    encoding = MIPS_NOP;
    ObjBytes( (char *)&encoding, sizeof( encoding ) );
}


static  uint_8  BranchOpcodes[][2] = {
    { 0x04, 0x31 },                     /* OP_CMP_EQUAL */
    { 0x05, 0x35 },                     /* OP_CMP_NOT_EQUAL */
    { 0x07, 0x37 },                     /* OP_CMP_GREATER */
    { 0x06, 0x33 },                     /* OP_CMP_LESS_EQUAL */
    { 0x01, 0x32 },                     /* OP_CMP_LESS */
    { 0x00, 0x36 },                     /* OP_CMP_GREATER_EQUAL */
};

void EncodeCond( oc_jcond *oc )
/*****************************/
{
    mips_ins    opcode;
    int         floating;

    floating = 0;
    if( oc->op.class & ATTR_FLOAT ) {
        floating = 1;
    }
    opcode = BranchOpcodes[oc->cond - FIRST_COMPARISON][floating];
    doBranch( opcode, oc->handle, oc->index );
}
