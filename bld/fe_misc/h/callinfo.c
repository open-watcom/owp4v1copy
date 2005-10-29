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
* Description:  front end caling info structure and initialization
*
****************************************************************************/

#if defined( BY_C_FRONT_END ) || defined( BY_CPP_FRONT_END ) || defined( BY_FORTRAN_FRONT_END )

#if _INTEL_CPU

#include <string.h>

static  hw_reg_set  StackParms[] = {
    HW_D( HW_EMPTY )
};

#if _CPU == 386
static  hw_reg_set  OptlinkParms[] = {
    HW_D_4( HW_EAX, HW_ECX, HW_EDX, HW_FLTS ),
    HW_D( HW_EMPTY )
};
static  hw_reg_set  FastcallParms[] = {
    HW_D( HW_ECX ), HW_D( HW_EDX ),
    HW_D( HW_EMPTY )
};
#else
static  hw_reg_set  FastcallParms[] = {
    HW_D( HW_AX ), HW_D( HW_DX ), HW_D( HW_BX ),
    HW_D( HW_EMPTY )
};
#endif

#if defined( BY_C_FRONT_END )
#define MEMALLOC CMemAlloc
#define STRALLOC CStrSave
#elif defined( BY_CPP_FRONT_END )
#define MEMALLOC CMemAlloc
#define STRALLOC strsave
#elif defined( BY_FORTRAN_FRONT_END )
#define MEMALLOC FMemAlloc
#define STRALLOC CMemAlloc
#else
#endif

void PragmaAuxCallInfoInit( call_class call_type, int flag_stdatnum )
{
    hw_reg_set  full_no_segs;

    HW_CAsgn( full_no_segs, HW_FULL );
    HW_CTurnOff( full_no_segs, HW_SEGS );


/*************************************************
 *  __fortran calling convention
 *************************************************/
    FortranInfo.objname = STRALLOC( "^" );

/*************************************************
 *  __cdecl calling convention
 *************************************************/
    CdeclInfo.cclass =    call_type |
#if _CPU == 8086
                         LOAD_DS_ON_CALL |
#endif
                         //REVERSE_PARMS |
                         CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         NO_FLOAT_REG_RETURNS |
                         NO_STRUCT_REG_RETURNS |
                         ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    CdeclInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( CdeclInfo.parms, StackParms, sizeof( StackParms ) );
    CdeclInfo.objname = STRALLOC( "_*" );

    HW_CAsgn( CdeclInfo.returns, HW_EMPTY );
    HW_CAsgn( CdeclInfo.streturn, HW_EMPTY );
    HW_TurnOn( CdeclInfo.save, full_no_segs );
    HW_CTurnOff( CdeclInfo.save, HW_FLTS );
#if _CPU == 386
    HW_CAsgn( CdeclInfo.streturn, HW_EAX );
    HW_CTurnOff( CdeclInfo.save, HW_EAX );
//    HW_CTurnOff( CdeclInfo.save, HW_EBX );
    HW_CTurnOff( CdeclInfo.save, HW_ECX );
    HW_CTurnOff( CdeclInfo.save, HW_EDX );
#else
    HW_CAsgn( CdeclInfo.streturn, HW_AX );
    HW_CTurnOff( CdeclInfo.save, HW_ABCD );
    HW_CTurnOff( CdeclInfo.save, HW_ES );
#endif

/*************************************************
 *  __pascal calling convention
 *************************************************/
    PascalInfo.cclass =   call_type |
                         REVERSE_PARMS |
                         //CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         NO_FLOAT_REG_RETURNS |
                         NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    PascalInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( PascalInfo.parms, StackParms, sizeof( StackParms ) );
    PascalInfo.objname = STRALLOC( "^" );

    HW_CAsgn( PascalInfo.returns, HW_EMPTY );
    HW_CAsgn( PascalInfo.streturn, HW_EMPTY );
    HW_TurnOn( PascalInfo.save, full_no_segs );
    HW_CTurnOff( PascalInfo.save, HW_FLTS );
#if _CPU == 386
    HW_CTurnOff( PascalInfo.save, HW_EAX );
    HW_CTurnOff( PascalInfo.save, HW_EBX );
    HW_CTurnOff( PascalInfo.save, HW_ECX );
    HW_CTurnOff( PascalInfo.save, HW_EDX );
#else
    HW_CTurnOff( PascalInfo.save, HW_ABCD );
    HW_CTurnOff( PascalInfo.save, HW_ES );
#endif


/*************************************************
 *  __stdcall calling convention
 *************************************************/
    StdcallInfo.cclass =  call_type |
                         //REVERSE_PARMS |
                         //CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         //NO_FLOAT_REG_RETURNS |
                         //NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    StdcallInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( StdcallInfo.parms, StackParms, sizeof( StackParms ) );
#if _CPU == 386
    if( flag_stdatnum ) {
        StdcallInfo.objname = STRALLOC( "_*#" );
    } else {
        StdcallInfo.objname = STRALLOC( "_*" );
    }
#else
    StdcallInfo.objname = STRALLOC( "_*" );
#endif

    HW_CAsgn( StdcallInfo.returns, HW_EMPTY );
    HW_CAsgn( StdcallInfo.streturn, HW_EMPTY );
    HW_TurnOn( StdcallInfo.save, full_no_segs );
    HW_CTurnOff( StdcallInfo.save, HW_FLTS );
#if _CPU == 386
//    HW_CAsgn( StdcallInfo.streturn, HW_EAX );
    HW_CTurnOff( StdcallInfo.save, HW_EAX );
//    HW_CTurnOff( StdcallInfo.save, HW_EBX );
    HW_CTurnOff( StdcallInfo.save, HW_ECX );
    HW_CTurnOff( StdcallInfo.save, HW_EDX );
#else
    HW_CAsgn( StdcallInfo.streturn, HW_AX );
    HW_CTurnOff( StdcallInfo.save, HW_ABCD );
    HW_CTurnOff( StdcallInfo.save, HW_ES );
#endif


/*************************************************
 *  __fastcall calling convention
 *************************************************/
#if _CPU == 386
    FastcallInfo.cclass = call_type |
                         //REVERSE_PARMS |
                         //CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         //NO_FLOAT_REG_RETURNS |
                         //NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    FastcallInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( FastcallParms ) );
    memcpy( FastcallInfo.parms, FastcallParms, sizeof( FastcallParms ) );
    FastcallInfo.objname = STRALLOC( "@*#" );

    HW_CAsgn( FastcallInfo.returns, HW_EMPTY );
    HW_CAsgn( FastcallInfo.streturn, HW_EMPTY );
    HW_TurnOn( FastcallInfo.save, full_no_segs );
    HW_CTurnOff( FastcallInfo.save, HW_FLTS );
    HW_CTurnOff( FastcallInfo.save, HW_EAX );
//    HW_CTurnOff( FastcallInfo.save, HW_EBX );
    HW_CTurnOff( FastcallInfo.save, HW_ECX );
    HW_CTurnOff( FastcallInfo.save, HW_EDX );
#else
    FastcallInfo.cclass = call_type |
                         //REVERSE_PARMS |
                         //CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         //NO_FLOAT_REG_RETURNS |
                         //NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    FastcallInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( FastcallParms ) );
    memcpy( FastcallInfo.parms, FastcallParms, sizeof( FastcallParms ) );
    FastcallInfo.objname = STRALLOC( "@*" );

    HW_CAsgn( FastcallInfo.returns, HW_EMPTY );
    HW_CAsgn( FastcallInfo.streturn, HW_EMPTY );
    HW_TurnOn( FastcallInfo.save, full_no_segs );
    HW_CTurnOff( FastcallInfo.save, HW_FLTS );
    HW_CTurnOff( StdcallInfo.save, HW_ABCD );
    HW_CTurnOff( StdcallInfo.save, HW_ES );
#endif

/*************************************************
 *  _Optlink calling convention
 *************************************************/
    OptlinkInfo.cclass =  call_type |
#ifdef PARMS_STACK_RESERVE
                         PARMS_STACK_RESERVE |
#endif
                         //REVERSE_PARMS |
                         CALLER_POPS  |
                         //GENERATE_STACK_FRAME |
                         //NO_FLOAT_REG_RETURNS |
                         NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
#if _CPU == 386
    OptlinkInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( OptlinkParms ) );
    memcpy( OptlinkInfo.parms, OptlinkParms, sizeof( OptlinkParms ) );
#else
    OptlinkInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( OptlinkInfo.parms, StackParms, sizeof( StackParms ) );
#endif
    OptlinkInfo.objname = STRALLOC( "*" );

    HW_CAsgn( OptlinkInfo.returns, HW_EMPTY );
//    HW_CAsgn( OptlinkInfo.returns, HW_FLTS );
    HW_CAsgn( OptlinkInfo.streturn, HW_EMPTY );
    HW_TurnOn( OptlinkInfo.save, full_no_segs );
    HW_CTurnOff( OptlinkInfo.save, HW_FLTS );
#if _CPU == 386
    HW_CTurnOff( OptlinkInfo.save, HW_EAX );
//    HW_CTurnOff( OptlinkInfo.save, HW_EBX );
    HW_CTurnOff( OptlinkInfo.save, HW_ECX );
    HW_CTurnOff( OptlinkInfo.save, HW_EDX );
#else
    HW_CTurnOff( OptlinkInfo.save, HW_ABCD );
    HW_CTurnOff( OptlinkInfo.save, HW_ES );
#endif

/*************************************************
 *  __syscall calling convention
 *************************************************/
    SyscallInfo.cclass =  call_type |
                         //REVERSE_PARMS |
                         CALLER_POPS |
                         //GENERATE_STACK_FRAME |
                         //NO_FLOAT_REG_RETURNS |
                         NO_STRUCT_REG_RETURNS |
                         //ROUTINE_RETURN |
                         //NO_8087_RETURNS |
                         //SPECIAL_RETURN |
                         SPECIAL_STRUCT_RETURN;
    SyscallInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( SyscallInfo.parms, StackParms, sizeof( StackParms ) );
    SyscallInfo.objname = STRALLOC( "*" );

    HW_CAsgn( SyscallInfo.returns, HW_EMPTY );
    HW_CAsgn( SyscallInfo.streturn, HW_EMPTY );
    HW_TurnOn( SyscallInfo.save, full_no_segs );
    HW_CTurnOff( SyscallInfo.save, HW_FLTS );
#if _CPU == 386
    HW_CTurnOff( SyscallInfo.save, HW_EAX );
//    HW_CTurnOff( SyscallInfo.save, HW_EBX );
    HW_CTurnOff( SyscallInfo.save, HW_ECX );
    HW_CTurnOff( SyscallInfo.save, HW_EDX );
#else
    HW_CTurnOff( SyscallInfo.save, HW_ABCD );
    HW_CTurnOff( SyscallInfo.save, HW_ES );
#endif

#if _CPU == 386
/****************************************************
 *  OS/2 32-bit->16-bit calling convention ( _Far16 )
 ****************************************************/
    /* these are internal, and will never be pointed to by
     * an aux_entry, so we don't have to worry about them
     */

    Far16CdeclInfo = CdeclInfo;
    Far16CdeclInfo.cclass |= FAR16_CALL;
    Far16CdeclInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( Far16CdeclInfo.parms, StackParms, sizeof( StackParms ) );
    Far16CdeclInfo.objname = STRALLOC( CdeclInfo.objname );
    // __far16 __cdecl depends on EBX being trashed in __cdecl
    // but NT 386 __cdecl preserves EBX
    HW_CTurnOff( Far16CdeclInfo.save, HW_EBX );

    Far16PascalInfo = PascalInfo;
    Far16PascalInfo.cclass |= FAR16_CALL;
    Far16PascalInfo.parms = (hw_reg_set *)MEMALLOC( sizeof( StackParms ) );
    memcpy( Far16PascalInfo.parms, StackParms, sizeof( StackParms ) );
    Far16PascalInfo.objname = STRALLOC( PascalInfo.objname );
#endif
}

#endif
#endif

