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
* Description:  Convert WATFOR-77 argument list to WFC argument list.
*
****************************************************************************/


#include <dos.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "ftnstd.h"
#include "parmtype.h"
#include "structio.h"
#include "rcb.h"
#include "fcenv.h"

#ifndef __alloca
#define __alloca _alloca
#endif

#define ARG_INDIRECT    0x8000
#define ARG_VALUE       0x4000
#define ARG_DATA_REF    0x2000
#define TYPE_MASK       0x000f
#define CODE_MASK       0x0f00
#define ARG_COUNT_MASK  0xff00
#define VAR_LEN_CHR     0x0080

#define DEF_CALL        0x0000
#define SYS_CALL        0x0001
#define STD_CALL        0x0002
#define CDECL_CALL      0x0003
#define PASCAL_CALL     0x0004
#define CALL_MASK       0x0007

#define FULL_REGS       sizeof( int ) * 4

#pragma off (check_stack);

#define MAX_ARG_SIZE sizeof( extended )
#define MAX_REG_IDX 3
#if _TARGET == _80386
    typedef long int REGISTER;
    #define ADDR( _x )  *(void PGM * PGM *)(_x)
    #define ADDR_SIZE   sizeof( void *)
    #pragma aux WF77_to_WFC "*" parm caller [edi] [esi];
    #pragma aux WF77_to_SYSCALL "*" parm caller [edi] [esi];
    extern      obj_ptr         CallerDataBase;
    extern      obj_ptr         CalledDataBase;
    #pragma aux (lg_sym) CallerDataBase;
    #pragma aux (lg_sym) CalledDataBase;
    #define ADD_CALLER( __p )   (void PGM *)( (obj_ptr)(__p) + CallerDataBase )
    #define ADD_CALLED( __p )   (void PGM *)( (obj_ptr)(__p) + CalledDataBase )
    #define RET_STRUCT_PTR      PGM
#else
    typedef short int REGISTER;
    #define ADDR( _x )  MK_FP( FP_SEG( _x ), *(unsigned_16 PGM *)(_x) )
    #define ADDR_SIZE   sizeof( unsigned_16 )
    #pragma aux WF77_to_WFC "*" parm caller [es di] [ds si];
    #pragma aux WF77_to_SYSCALL "*" parm caller [es di] [ds si];
    #define ADD_CALLER( __p )   (void PGM *)(__p)
    #define ADD_CALLED( __p )   (void PGM *)(__p)
    #define RET_STRUCT_PTR      near
#endif

#pragma aux WFC_to_WF77 "*";
#pragma aux SYSCALL_to_WF77 "*";

static  string PGM *ArraySCB;

#define LGADV( arg ) \
        ((lg_adv PGM *)arg)

#define _ArgCode( arginfo ) \
        ( ( arginfo & CODE_MASK ) >> 8 )

#define _ArgCount( arginfo ) \
        ( ( arginfo & ARG_COUNT_MASK ) >> 8 )

#define _ArgType( arginfo ) \
        ( arginfo & TYPE_MASK )

#define _CallType( call_info ) \
        ( call_info & CALL_MASK)

#define _RetStruct( typ, call ) \
    ( (PT_CPLX_32 == typ) || (PT_CPLX_16 == typ) || (PT_CPLX_8 == typ)  || \
      (PT_REAL_16 == typ) || (PT_STRUCT == typ)  || \
      ( (call != SYS_CALL) && ( (PT_REAL_8 == typ)  || (PT_REAL_4 == typ) ) ) )

static  void PGM *ArgAddr( unsigned_8 PGM *arg_list ) {
//=====================================================

// Get argument address.

    void PGM *          arg_addr;
    int                 arg_code;
    int                 arg_type;
    unsigned_16         arg_info;

    for(;;) {
        arg_info = *(unsigned_16 *)arg_list;
        arg_list += sizeof( unsigned_16 );
        arg_code = _ArgCode(arg_info );
        arg_type = _ArgType( arg_info );
        if( arg_code != PC_STATEMENT ) break;
    }
    arg_addr = ADD_CALLER( ADDR( arg_list ) );
    arg_list += ADDR_SIZE;
    if( ( arg_info & ARG_INDIRECT ) ||
        ( arg_code == PC_PROCEDURE ) ||
        ( arg_code == PC_FN_OR_SUB ) ) {
        arg_addr = *(void PGM * PGM *)arg_addr;
    } else if( arg_code == PC_ARRAY_NAME ) {
        if( arg_type == PT_CHAR ) {
            arg_addr = ArraySCB;
            --ArraySCB;
        } else {
            arg_addr = LGADV( arg_addr )->origin;
        }
    } else if( ( arg_code == PC_SS_ARRAY ) || ( arg_code == PC_ARRAY_ELT ) ) {
        if( arg_type != PT_CHAR ) {
            arg_addr = *(void PGM * PGM *)arg_addr;
        }
    } else if( arg_type == PT_STRUCT ) {
        arg_addr = *(void PGM * PGM *)arg_addr;
    }
    return( arg_addr );
}


enum { AX = 1, DX = 2, BX = 4, CX = 8 } used_regs;

static  int     GrabRegs( int num_regs ) {
//========================================

    if( num_regs == 1 ) {
        if( ( used_regs & AX ) == 0 ) {
            used_regs |= AX;
            return( 0 );
        } else if( ( used_regs & DX ) == 0 ) {
            used_regs |= DX;
            return( 1 );
        } else if( ( used_regs & BX ) == 0 ) {
            used_regs |= BX;
            return( 2 );
        } else if( ( used_regs & CX ) == 0 ) {
            used_regs |= CX;
            return( 3 );
        } else {
            return( -1 );
        }
    } else if( num_regs == 2 ) {
        if( ( used_regs & ( AX | DX ) ) == 0 ) {
            used_regs |= AX | DX;
            return( 0 );
        } else if( ( used_regs & ( BX | CX ) ) == 0 ) {
            used_regs |= BX | CX;
            return( 2 );
        } else {
            return( -1 );
        }
#if _TARGET == _8086
    } else if( num_regs == 4 ) {
        if( used_regs != 0 ) return( -1 );
        used_regs |= AX | BX | CX | DX;
        return( 0 );
#endif
    }
    return( -1 );
}

//                            !! L1 L4 I1 I2 I4 R4 R8 RA C8 C0 CF CH ST
#if _TARGET == _80386
  #if defined( __FPI__ )
    static char NumRegs[] = { 0, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 1, 0 };
  #else
    static char NumRegs[] = { 0, 1, 1, 1, 1, 1, 1, 2, 4, 2, 4, 0, 1, 0 };
  #endif
#else
  #if defined( __FPI__ )
    static char NumRegs[] = { 0, 1, 2, 1, 1, 2, 2, 4, 0, 0, 0, 0, 2, 0 };
  #else
    static char NumRegs[] = { 0, 1, 2, 1, 1, 2, 2, 4, 0, 4, 0, 0, 2, 0 };
  #endif
#endif

static  int     AssignArg( REGISTER *regs,
                           ftn_type PGM *arg_addr, unsigned_16 arg_info ) {
//=========================================================================

    int         idx;

    if( arg_info & ARG_VALUE ) {
        idx = GrabRegs( NumRegs[ _ArgType( arg_info ) ] );
        if( idx == -1 ) return( FALSE );
        switch( _ArgType( arg_info ) ) {
        case PT_LOG_1:
        case PT_INT_1:
            regs[ idx ] = (REGISTER)(arg_addr->intstar1);
            break;
        case PT_INT_2:
            regs[ idx ] = (REGISTER)(arg_addr->intstar2);
            break;
        case PT_INT_4:
            #if _TARGET == _80386
                regs[ idx ] = (REGISTER)(arg_addr->intstar4);
            #else
                regs[ idx ] = (REGISTER)(arg_addr->intstar4);
                regs[ idx+1 ] = (REGISTER)((arg_addr->intstar4) >> 16);
            #endif
            break;
        case PT_REAL_4:
            #if _TARGET == _80386
                    regs[ idx ] = (REGISTER)(arg_addr->intstar4);
            #else
                    regs[ idx ] = (REGISTER)(arg_addr->intstar4);
                    regs[ idx+1 ] = (REGISTER)((arg_addr->intstar4) >> 16);
            #endif
            break;
        case PT_REAL_8:
            #if _TARGET == _80386
                    regs[ idx ] = (REGISTER)(arg_addr->intstar4);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar4 ));
                    regs[ idx+1 ] = (REGISTER)(arg_addr->intstar4);
            #else
                    regs[ idx+3 ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx+2 ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx+1 ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx ] = (REGISTER)(arg_addr->intstar2);
            #endif
            break;
        case PT_CPLX_8:
            #if _TARGET == _80386
                    regs[ idx ] = (REGISTER)(arg_addr->intstar4);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar4 ));
                    regs[ idx+1 ] = (REGISTER)(arg_addr->intstar4);
            #else
                    regs[ idx ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx+1 ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx+2 ] = (REGISTER)(arg_addr->intstar2);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar2 ));
                    regs[ idx+3 ] = (REGISTER)(arg_addr->intstar2);
            #endif
            break;
        case PT_CHAR:
            arg_addr = (ftn_type *)(((string *)arg_addr)->strptr);
            #if _TARGET == _80386
                regs[ idx ] = (REGISTER)arg_addr;
            #else
                regs[ idx ] = (REGISTER)FP_OFF( arg_addr );
                regs[ idx+1 ] = (REGISTER)FP_SEG( arg_addr );
            #endif
            break;
#if _TARGET == _80386
        case PT_CPLX_16:
                    regs[ idx ] = (REGISTER)(arg_addr->intstar4);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar4 ));
                    regs[ idx+1 ] = (REGISTER)(arg_addr->intstar4);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar4 ));
                    regs[ idx+2 ] = (REGISTER)(arg_addr->intstar4);
                    arg_addr = (ftn_type *)((char *)arg_addr+sizeof( intstar4 ));
                    regs[ idx+3 ] = (REGISTER)(arg_addr->intstar4);
                    break;
#endif
        }
    } else {
        #if _TARGET == _80386
            idx = GrabRegs( 1 );
            if( idx == -1 ) return( FALSE );
            regs[ idx ] = (REGISTER)arg_addr;
        #else
            idx = GrabRegs( 2 );
            if( idx == -1 ) return( FALSE );
            regs[ idx ] = (REGISTER)FP_OFF( arg_addr );
            regs[ idx+1 ] = (REGISTER)FP_SEG( arg_addr );
        #endif
    }
    return( TRUE );
}

static char typeSize[] = { 0,
/* logicals */             sizeof(int), sizeof(long),
/* integers */             sizeof(int), sizeof(int), sizeof(long),
/* reals    */             sizeof(float), sizeof(double), sizeof(extended),
/* complex  */             sizeof(complex), sizeof(dcomplex), sizeof(xcomplex),
/* struct & char */        sizeof( void PGM * ), sizeof( void PGM * ) };


static int      preProcessArguments( unsigned_8 * arg_list, int list_count,
                                     int *st_space, int *total_size ) {
//=====================================================================

    unsigned_16         arg_info;
    int                 arg_code;
    int                 arg_type;
    string PGM          *scb;
    void PGM            *arg;
    int                 struct_space = 0;
    int                 arg_count = 0;
    int                 total = 0;
    sdefn PGM           *sd;

    while( list_count != 0 ) {
        arg_info = *(unsigned_16*)arg_list;
        arg_list += sizeof( unsigned_16 );
        arg_code = _ArgCode( arg_info );
        arg_type = _ArgType( arg_info );

        if( arg_code != PC_STATEMENT ) { // alternate returns are not passed
            if( arg_type == PT_CHAR && arg_code == PC_ARRAY_NAME ) {
                scb = __alloca( sizeof( string ) );
                arg = ADD_CALLER( ADDR( arg_list ) );
                scb->strptr = LGADV( arg )->origin;
                scb->len = LGADV( arg )->elt_size;
                if( ArraySCB == NULL ) {
                    ArraySCB = scb;
                }
                total += typeSize[ PT_CHAR ];
            } else if( arg_info & ARG_VALUE ) {
                switch( arg_type ) {
                case( PT_STRUCT ):
                    arg = ADD_CALLER( ADDR( arg_list ) );
                    sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                        ((rcb PGM *)arg)->defn );
                    sd = ADD_CALLER( sd ); // crucial, otherwise we have a problem
                    struct_space += __ALLOCA_ALIGN( sd->hd.total_size );
                    total += __ALLOCA_ALIGN( sd->hd.total_size );
                    break;
                case PT_CPLX_8:
                    struct_space += sizeof( complex );
                    break;
                case PT_CPLX_16:
                    struct_space += sizeof( dcomplex );
                    break;
                case PT_CPLX_32:
                    struct_space += sizeof( xcomplex );
                    break;
                }
                if( arg_type != PT_STRUCT ) {
                    total += typeSize[ arg_type ];
                }
            }
            ++arg_count;
            arg_list += ADDR_SIZE;     // skip the offset
        }
        --list_count;
    }
    *total_size = total;
    *st_space = struct_space;
    return( arg_count );
}


ftn_type PGM *  loadStackWithArgs( unsigned_8 * arg_list, int arg_count,
                                   ftn_type PGM *sp ) {
//=====================================================

    unsigned_16         arg_info;
    uint                size;
    void PGM            *arg;
    sdefn PGM           *sd;

    while( arg_count > 0 ) {
        arg_info = *(unsigned_16 *)arg_list;
        if( _ArgCode( arg_info ) == PC_STATEMENT ) { // alternate returns are not passed
            arg_list += sizeof( unsigned_16 );
            continue;
        }
        if( arg_info & ARG_VALUE ) {
            switch( _ArgType( arg_info ) ) {
            case PT_LOG_1:
                sp->logstar1 = *(logstar1 *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( inttarg ));
                break;
            case PT_LOG_4:
                sp->logstar4 = *(logstar4 *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( logstar4 ));
                break;
            case PT_INT_1:
                sp->intstar1 = *(intstar1 *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( inttarg ));
                break;
            case PT_INT_2:
                sp->intstar2 = *(intstar2 *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( inttarg ));
                break;
            case PT_INT_4:
                sp->intstar4 = *(intstar4 *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( intstar4 ));
                break;
            case PT_REAL_4:
                sp->single = *(single *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( single ));
                break;
            case PT_REAL_8:
                sp->dble = *(double *)ArgAddr( arg_list );
                sp = (ftn_type *)((char *)sp + sizeof( double ));
                break;
            case PT_CHAR:
                sp->pgm_ptr = ((string *)ArgAddr( arg_list ))->strptr;
                sp = (ftn_type *)((char *)sp + sizeof( void PGM * ));
                break;
            default:
                switch( _ArgType( arg_info ) ) {
                case PT_CPLX_8:
                    size = sizeof( complex );
                    break;
                case PT_CPLX_16:
                    size = sizeof( dcomplex );
                    break;
                case PT_CPLX_32:
                    size = sizeof( xcomplex );
                    break;
                case PT_STRUCT:
                    arg_list += sizeof( unsigned_16 ); // point to offset
                    arg = ADD_CALLER( ADDR( arg_list ) );
                    sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                        ADD_CALLER( ((rcb PGM *)arg)->defn ) );
                    size = sd->hd.total_size;
                    arg_list -= sizeof( unsigned_16 ); // point back to info
                    break;
                default:
                    size = 0;
                }
#if _TARGET == _80386
                memcpy( sp, ArgAddr( arg_list ), size );
#else
                _fmemcpy( sp, ArgAddr( arg_list ), size );
#endif
                sp = (ftn_type *)((char *)sp + size );
                break;
            }
        } else {
            sp->pgm_ptr = ArgAddr( arg_list );
            if( arg_info & ARG_DATA_REF ) {
                if( _ArgType( arg_info ) == PT_CHAR ) {
                    sp->pgm_ptr = ((string *)sp->pgm_ptr)->strptr;
                }
            }
            sp = (ftn_type *)((char *)sp + sizeof( void PGM * ));
        }
        arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
        --arg_count;
    }
    return( sp );
}


ftn_type PGM *  loadStackWithRevArgs( unsigned_8 * arg_list, int arg_count,
                                      ftn_type PGM *sp, int total_size ) {
//========================================================================

    unsigned_16         arg_info;
    uint                size;
    void PGM            *arg;
    sdefn PGM           *sd;
    ftn_type            *finger;

    finger = sp;
    sp = (ftn_type *)((char *)sp + total_size );
    while( arg_count > 0 ) {
        arg_info = *(unsigned_16 *)arg_list;
        if( _ArgCode( arg_info ) == PC_STATEMENT ) { // alternate returns are not passed
            arg_list += sizeof( unsigned_16 );
            continue;
        }
        if( arg_info & ARG_VALUE ) {
            switch( _ArgType( arg_info ) ) {
            case PT_LOG_1:
                sp = (ftn_type *)((char *)sp - sizeof( inttarg ));
                sp->logstar1 = *(logstar1 *)ArgAddr( arg_list );
                break;
            case PT_LOG_4:
                sp = (ftn_type *)((char *)sp - sizeof( logstar4 ));
                sp->logstar4 = *(logstar4 *)ArgAddr( arg_list );
                break;
            case PT_INT_1:
                sp = (ftn_type *)((char *)sp - sizeof( inttarg ));
                sp->intstar1 = *(intstar1 *)ArgAddr( arg_list );
                break;
            case PT_INT_2:
                sp = (ftn_type *)((char *)sp - sizeof( inttarg ));
                sp->intstar2 = *(intstar2 *)ArgAddr( arg_list );
                break;
            case PT_INT_4:
                sp = (ftn_type *)((char *)sp - sizeof( intstar4 ));
                sp->intstar4 = *(intstar4 *)ArgAddr( arg_list );
                break;
            case PT_REAL_4:
                sp = (ftn_type *)((char *)sp - sizeof( single ));
                sp->single = *(single *)ArgAddr( arg_list );
                break;
            case PT_REAL_8:
                sp = (ftn_type *)((char *)sp - sizeof( double ));
                sp->dble = *(double *)ArgAddr( arg_list );
                break;
            case PT_CHAR:
                sp = (ftn_type *)((char *)sp - sizeof( void PGM * ));
                sp->pgm_ptr = ((string *)ArgAddr( arg_list ))->strptr;
                break;
            default:
                switch( _ArgType( arg_info ) ) {
                case PT_CPLX_8:
                    size = sizeof( complex );
                    break;
                case PT_CPLX_16:
                    size = sizeof( dcomplex );
                    break;
                case PT_CPLX_32:
                    size = sizeof( xcomplex );
                    break;
                case PT_STRUCT:
                    arg_list += sizeof( unsigned_16 ); // point to offset
                    arg = ADD_CALLER( ADDR( arg_list ) );
                    sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                        ADD_CALLER( ((rcb PGM *)arg)->defn ) );
                    size = sd->hd.total_size;
                    arg_list -= sizeof( unsigned_16 ); // point back to info
                    break;
                default:
                    size = 0;
                }
                sp = (ftn_type *)((char *)sp - size );
#if _TARGET == _80386
                memcpy( sp, ArgAddr( arg_list ), size );
#else
                _fmemcpy( sp, ArgAddr( arg_list ), size );
#endif
                break;
            }
        } else {
            sp = (ftn_type *)((char *)sp - sizeof( void PGM * ));
            sp->pgm_ptr = ArgAddr( arg_list );
            if( arg_info & ARG_DATA_REF ) {
                if( _ArgType( arg_info ) == PT_CHAR ) {
                    sp->pgm_ptr = ((string *)sp->pgm_ptr)->strptr;
                }
            }
        }
        arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
        --arg_count;
    }
    assert( sp == finger );
    return( (ftn_type *)((char *)sp + total_size ) );

}


#if _OPSYS == _OS2
  #define       OSYSCALL        __syscall
#else
  #define       OSYSCALL        __pascal
#endif

void    WF77_to_SYSCALL( ftn_type (PGM *rtn)(), unsigned_8 PGM *arg_list,
                         ftn_type PGM *ret_val ) {
//===================================================================

// Call a non wfc subprogram, mainly system and pascal
// All arguments from WATFOR-77 are passed by reference.

    int                 arg_count;
    int                 ret_type;
    unsigned short      arg_info;
    unsigned short      call_type;
    ftn_type            PGM *sp;
    void PGM            *struct_storage;
    void PGM            *ret_struct;
    sdefn PGM           *sd;
    int                 extra_space;
    int                 total_space;
    int                 ret_struct_size = 0;

    ArraySCB = NULL;
    extra_space = 0;
    arg_info = *(unsigned_16 *)arg_list;
    arg_list += sizeof( unsigned_16 );
    call_type = _CallType( *(unsigned_16 *)arg_list ); // Get Type of system call.
    arg_list += sizeof( unsigned_16 );
    ret_type = _ArgType( arg_info );

    arg_count = preProcessArguments( arg_list, _ArgCount( arg_info ),
                                     &extra_space, &total_space );

    // calculate the amount of stack we will need
    extra_space += arg_count * MAX_ARG_SIZE;

    if( ( call_type != CDECL_CALL ) && _RetStruct( ret_type, call_type ) ) {
        if( ret_type == PT_STRUCT ) {
            // find size of structure
            sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                ADD_CALLED( ((rcb PGM *)ret_val)->defn ) );
            ret_struct_size = sd->hd.total_size;
        } else {
            ret_struct_size = typeSize[ ret_type ];
        }
        // allocate memory for it on stack
        struct_storage = __alloca( __ALLOCA_ALIGN( ret_struct_size ) );

        // allocate argument space
        sp = __alloca( extra_space + sizeof( void RET_STRUCT_PTR * ) );

        // make it the first argument to be popped off
        if( call_type == PASCAL_CALL ) {
            sp->pgm_ptr = (void RET_STRUCT_PTR *)struct_storage;
            sp = (ftn_type *)((char *)sp + sizeof( void RET_STRUCT_PTR * ));
        } else {
            sp->pgm_ptr = (void PGM *)struct_storage;
            sp = (ftn_type *)((char *)sp + sizeof( void PGM * ));
        }
    } else {
        sp = __alloca( extra_space );
    }
    if( call_type == PASCAL_CALL ) {
        sp = loadStackWithRevArgs( arg_list, arg_count, sp, total_space );
    } else {
        sp = loadStackWithArgs( arg_list, arg_count, sp );
    }

    switch( ret_type ) {
    case PT_NOTYPE :
        *(int *)ret_val = ((int OSYSCALL (PGM *)())rtn)();
        break;
    case PT_LOG_1 :
        ret_val->logstar4 = ((logstar1 OSYSCALL (PGM *)())rtn)();
        break;
    case PT_LOG_4 :
        ret_val->logstar4 = ((logstar4 OSYSCALL (PGM *)())rtn)();
        break;
    case PT_INT_1 :
        ret_val->intstar4 = ((intstar1 OSYSCALL (PGM *)())rtn)();
        break;
    case PT_INT_2 :
        ret_val->intstar4 = ((intstar2 OSYSCALL (PGM *)())rtn)();
        break;
    case PT_INT_4 :
        ret_val->intstar4 = ((intstar4 OSYSCALL (PGM *)())rtn)();
        break;
    case PT_CHAR :
        ret_val->string.strptr = ((char OSYSCALL *(PGM *)())rtn)();
        break;
    default:
        // execute call
        switch( call_type ) {
#if _TARGET == _80386 && _OPSYS == _OS2
        case( SYS_CALL ):
            if( ( ret_type != PT_REAL_4 ) && ( ret_type != PT_REAL_8 ) ) {
                ((void __syscall (PGM *)())rtn)();
            }
            break;
#endif
        case( PASCAL_CALL ):
            ((void __pascal (PGM *)())rtn)();
            break;
        }
        switch( ret_type ) {
        case PT_REAL_4 :
            switch( call_type ) {
#if _TARGET == _80386 && _OPSYS == _OS2
            case( SYS_CALL ):
                ret_val->single = ((single __syscall (PGM *)())rtn)();
                return;
#endif
            case( CDECL_CALL ):
                ret_val->single = ((single __cdecl (PGM *)())rtn)();
                return;
            default:
                ret_struct = &(ret_val->single);
            }
            break;
        case PT_REAL_8 :
            switch( call_type ) {
#if _TARGET == _80386 && _OPSYS == _OS2
            case( SYS_CALL ):
                ret_val->dble = ((double __syscall (PGM *)())rtn)();
                return;
#endif
            case( CDECL_CALL ):
                ret_val->dble = ((double __cdecl (PGM *)())rtn)();
                return;
            default:
                ret_struct = &(ret_val->dble);
            }
            break;
        case PT_CPLX_8 :
            if( call_type == CDECL_CALL ) {
                ret_val->complex = ((complex __cdecl (PGM *)())rtn)();
                return;
            }
            ret_struct = &(ret_val->complex);
            break;
        case PT_CPLX_16 :
            if( call_type == CDECL_CALL ) {
                ret_val->dcomplex = ((dcomplex __cdecl (PGM *)())rtn)();
                return;
            }
            ret_struct = &(ret_val->dcomplex);
            break;
        case PT_CPLX_32 :
            if( call_type == CDECL_CALL ) {
                ret_val->xcomplex = ((xcomplex __cdecl (PGM *)())rtn)();
                return;
            }
            ret_struct = &(ret_val->xcomplex);
            break;
        case PT_STRUCT :
            if( call_type == CDECL_CALL ) {
                // find size of structure
                sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                ADD_CALLED( ((rcb PGM *)ret_val)->defn ) );
                ret_struct_size = sd->hd.total_size;
                struct_storage = ((void near * __cdecl (PGM *)())rtn)();
            }
            ret_struct = SDEFN_REF_PGM(FP_SEG(((rcb PGM *)ret_val)->origin),
                                        ((rcb PGM *)ret_val)->origin );
            break;
        }
#if _TARGET == _80386
        memcpy( ret_struct, struct_storage, ret_struct_size );
#else
        _fmemcpy( ret_struct, struct_storage, ret_struct_size );
#endif
    }
}

#ifdef __386__
  #pragma aux (default) other_call_conv parm [esi] [eax ebx ecx edx];
#else
  #pragma aux (default) other_call_conv parm [si] [ax bx cx dx];
#endif
typedef void (alt_call_conv)();
#pragma aux (alt_call_conv, other_call_conv);

void    WF77_to_WFC( ftn_type (PGM *rtn)(), unsigned_8 PGM *arg_list,
                     ftn_type PGM *ret_val ) {
//============================================

// Call a subprogram compiled by WATCOM FORTRAN Optimizing compiler.
// All arguments from WATFOR-77 are passed by reference.

    int                 arg_count;
    int                 ret_type;
    unsigned short      arg_info;
    REGISTER            *regs;
    ftn_type            PGM *sp;
    sdefn PGM           *sd;
    void PGM            *struct_storage;
    int                 idx;
    bool                assigned;
    int                 extra_space;
    int                 total_space;

    ArraySCB = NULL;
    arg_info = *(unsigned_16 *)arg_list;
    arg_list += sizeof( unsigned_16 );
    ret_type = _ArgType( arg_info );
    arg_count = preProcessArguments( arg_list, _ArgCount( arg_info ),
                                      &extra_space, &total_space );
    regs = __alloca( ( MAX_REG_IDX + 1 ) * sizeof( REGISTER ) );
    idx = 0;
    used_regs = 0;
    assigned = TRUE;
    if( arg_count > 0 ) {
        assigned = AssignArg( regs, ArgAddr( arg_list ),
                              *(unsigned_16 *)arg_list );
        if( assigned ) {
            arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
            --arg_count;
        }
        if( ( arg_count > 0 ) && assigned ) {
            assigned = AssignArg( regs,
                                  ArgAddr( arg_list ),
                                  *(unsigned_16 *)arg_list );
            if( assigned ) {
                arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
                --arg_count;
            }
            if( ( arg_count > 0 ) && assigned ) {
                assigned = AssignArg( regs,
                                      ArgAddr( arg_list ),
                                      *(unsigned_16 *)arg_list );
                if( assigned ) {
                    arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
                    --arg_count;
                }
                if( arg_count > 0 ) {
                    assigned = AssignArg( regs,
                                          ArgAddr( arg_list ),
                                          *(unsigned_16 *)arg_list );
                    if( assigned ) {
                        arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
                        --arg_count;
                    }
                }
            }
        }
    }
    if( (ret_type == PT_CHAR) && assigned ) {
        assigned = AssignArg( regs, ret_val, PT_CHAR );
    }
    if( ret_type == PT_STRUCT ) {
        sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                            ADD_CALLED( ((rcb PGM *)ret_val)->defn ) );
#if _TARGET == _8086
        switch( sd->hd.total_size ) {
        case 1:
        case 2:
        case 4:
            struct_storage = ((rcb PGM *)ret_val)->origin;
            break;
        default:
            // allocate space on callee's stack since SS:SI is pointer to
            // return value
            struct_storage = __alloca( __ALLOCA_ALIGN( sd->hd.total_size ) );
        }
#else
        struct_storage = ((rcb PGM *)ret_val)->origin;
#endif
    }
    // calculate required stack space
    extra_space += arg_count * MAX_ARG_SIZE;
    if( ( ret_type == PT_CHAR ) && !assigned ) {
        sp = __alloca( extra_space + sizeof( void PGM * ) );
    } else {
        sp = __alloca( extra_space );
    }
    sp = loadStackWithArgs( arg_list, arg_count, sp );

#define ARGS    regs[0], regs[1], regs[2], regs[3]
    switch( ret_type ) {
    case PT_NOTYPE :
        *(int *)ret_val = ((int (PGM *)())rtn)( ARGS );
        break;
    case PT_LOG_1 :
        ret_val->logstar4 = ((logstar1 (PGM *)())rtn)( ARGS );
        break;
    case PT_LOG_4 :
        ret_val->logstar4 = ((logstar4 (PGM *)())rtn)( ARGS );
        break;
    case PT_INT_1 :
        ret_val->intstar4 = ((intstar1 (PGM *)())rtn)( ARGS );
        break;
    case PT_INT_2 :
        ret_val->intstar4 = ((intstar2 (PGM *)())rtn)( ARGS );
        break;
    case PT_INT_4 :
        ret_val->intstar4 = ((intstar4 (PGM *)())rtn)( ARGS );
        break;
    case PT_REAL_4 :
        ret_val->single = ((single (PGM *)())rtn)( ARGS );
        break;
    case PT_REAL_8 :
        ret_val->dble = ((double (PGM *)())rtn)( ARGS );
        break;
    case PT_CPLX_8 :
        ret_val->complex = ((complex (PGM *)())rtn)( ARGS );
        break;
    case PT_CPLX_16 :
        ret_val->dcomplex = ((dcomplex (PGM *)())rtn)( ARGS );
        break;
    case PT_CHAR :
        if( !assigned ) {
            sp->pgm_ptr = ret_val;
        }
        rtn( ARGS );
        break;
    case PT_STRUCT :
        {
            // We have to check if the structure size is 1, 2, or 4 bytes
            // since it will be return in al/ax/eax in those cases.


            switch( sd->hd.total_size ) {
            case 1:
                *(intstar1 PGM *)struct_storage =
                                        ((intstar1 (PGM *)())rtn)( ARGS );
                break;
            case 2:
                *(intstar2 PGM *)struct_storage =
                                        ((intstar2 (PGM *)())rtn)( ARGS );
                break;
            case 4:
                *(intstar4 PGM *)struct_storage =
                                        ((intstar4 (PGM *)())rtn)( ARGS );
                break;
            default:
                ((alt_call_conv PGM *)rtn)( FP_OFF( struct_storage ), ARGS);
#if _TARGET == _8086
                _fmemcpy( ((rcb PGM *)ret_val)->origin, struct_storage,
                    sd->hd.total_size );
#endif
                break;
            }
        }
    }
}


static int loadArg( unsigned_8 PGM *arg_list, va_list args, int size,
                    int regsize, unsigned_16 arg_info ) {
//=======================================================

    void PGM            *d_arg;
    void PGM            *a_arg;
    sdefn PGM           *sd;
    int                 arg_type;
    int                 arg_code;

    arg_code = _ArgCode( arg_info );
    arg_type = _ArgType( arg_info );
    d_arg = ADD_CALLED( ADDR( arg_list ) );
    if( arg_info & ARG_VALUE ) {
        if( ( size == regsize ) && ( size < FULL_REGS ) ) {
            // skip unallocated regs
            *args += FULL_REGS - regsize;
            size = FULL_REGS;
        }
        // big kludge right here to get address of argument on the
        // stack, there is currently no other way to do it, this should
        // be changed in stdarg.h
        a_arg = *args;
        switch( arg_type ) {
        case PT_LOG_1 :
        case PT_INT_1 :
        case PT_INT_2 :
            *args += sizeof( int );
            size += sizeof( int );
            *(void PGM * PGM *)d_arg = a_arg;
            break;
        case PT_LOG_4 :
        case PT_INT_4 :
        case PT_REAL_4 :
            *args += sizeof( long );
            size += sizeof( long );
            *(void PGM * PGM *)d_arg = a_arg;
            break;
        case PT_REAL_8 :
        case PT_CPLX_8 :
            *args += sizeof( double );
            size += sizeof( double );
            *(void PGM * PGM *)d_arg = a_arg;
            break;
        case PT_REAL_16 :
        case PT_CPLX_16 :
            *args += sizeof( dcomplex );
            size += sizeof( dcomplex );
            *(void PGM * PGM *)d_arg = a_arg;
            break;
        case PT_CPLX_32 :
            *args += sizeof( xcomplex );
            size += sizeof( xcomplex );
            *(void PGM * PGM *)d_arg = a_arg;
            break;
        case PT_CHAR :
           a_arg = va_arg( args, void PGM * );
           size += sizeof( void PGM * );
           ((string PGM *)d_arg)->strptr = a_arg;
           if( ( arg_info & VAR_LEN_CHR ) && ( a_arg != NULL ) ) {
                ((string PGM *)d_arg)->len = strlen( a_arg );
           }
           break;
           case PT_STRUCT :
                sd = SDEFN_REF_PGM( FP_SEG(arg_list),
                                    ADD_CALLED(((rcb PGM*)d_arg)->defn));
                *(void PGM * PGM *)d_arg = a_arg;

                *args += __ALLOCA_ALIGN( sd->hd.total_size );
                size += __ALLOCA_ALIGN( sd->hd.total_size );
        }
    } else {
        a_arg = va_arg( args, void PGM * );
        size += sizeof( void PGM * );
        if( arg_type == PT_CHAR ) {
            if( arg_code == PC_ARRAY_NAME ) {
                LGADV( d_arg )->origin = ((string PGM *)a_arg)->strptr;
                if( arg_info & VAR_LEN_CHR ) {
                    LGADV( d_arg )->elt_size=((string PGM *)a_arg)->len;
                }
            } else {
                ((string PGM *)d_arg)->strptr = ((string PGM *)a_arg)->strptr;
                if( arg_info & VAR_LEN_CHR ) {
                    ((string PGM *)d_arg)->len = ((string PGM *)a_arg)->len;
                }
            }
        } else {
            *(void PGM * PGM *)d_arg = a_arg;
        }
        if( arg_code == PC_ARRAY_NAME ) {
            LGADV( d_arg )->adv_link = NULL;
        }
    }
    return( size );
}


static int processIncomingArguments( unsigned_8 PGM *arg_list, va_list args,
                                     int list_count, int regsize ) {
//==================================================================

    int                 arg_type;
    int                 arg_code;
    unsigned short      arg_info;
    int                 size = 0;

    while( list_count != 0 ) {
        arg_info = *(unsigned_16 PGM *)arg_list;
        arg_list += sizeof( unsigned_16 );
        arg_code = _ArgCode( arg_info );
        arg_type = _ArgType( arg_info );
        if( arg_code != PC_STATEMENT ) {
            size = loadArg( arg_list, args, size, regsize, arg_info );
            arg_list += ADDR_SIZE;
        }
        --list_count;
    }
    return( size );
}


static void flipCmplx( unsigned_8 PGM *arg_list, int arg_type ) {
//===============================================================

    ftn_type PGM                *d_arg;
    single                      tsingle;
    double                      tdouble;
    extended                    textended;

    d_arg = *(ftn_type PGM * PGM *)ADD_CALLED( ADDR( arg_list ) );
    switch( arg_type ) {
    case( PT_CPLX_8 ):
        tsingle = d_arg->complex.realpart;
        d_arg->complex.realpart = d_arg->complex.imagpart;
        d_arg->complex.imagpart = tsingle;
        break;
    case( PT_CPLX_16 ):
        tdouble = d_arg->dcomplex.realpart;
        d_arg->dcomplex.realpart = d_arg->dcomplex.imagpart;
        d_arg->dcomplex.imagpart = tdouble;
        break;
    case( PT_CPLX_32 ):
        textended = d_arg->xcomplex.realpart;
        d_arg->xcomplex.realpart = d_arg->xcomplex.imagpart;
        d_arg->xcomplex.imagpart = textended;
        break;
    }
}


static int processIncomingRevArguments( unsigned_8 PGM *arg_list, va_list args,
                                        int list_count, int regsize ) {
//=====================================================================

    int                 arg_type;
    int                 arg_code;
    unsigned short      arg_info;
    int                 size = 0;

    if( list_count != 0 ) {
        list_count--;
        arg_info = *(unsigned_16 PGM *)arg_list;
        arg_list += sizeof( unsigned_16 );
        arg_code = _ArgCode( arg_info );
        if( arg_code == PC_STATEMENT ) {
            size = processIncomingRevArguments( arg_list, args, list_count,
                                                regsize );
        } else {
            size = processIncomingRevArguments( arg_list + ADDR_SIZE, args,
                                                list_count, regsize );
            size = loadArg( arg_list, args, size, regsize, arg_info );
            arg_type = _ArgType( arg_info );
            if( ( arg_info & ARG_VALUE ) && ( ( arg_type == PT_CPLX_8 ) ||
                ( arg_type == PT_CPLX_16 ) || ( arg_type == PT_CPLX_32 ) ) ) {
                flipCmplx( arg_list, arg_type );
            }
        }
    }
    return( size );
}


static int numOfRegsUsed( unsigned_8 PGM *arg_list ) {
//====================================================

    unsigned short      arg_info;
    int                 x;
    int                 arg_type;
    int                 size = 0;

    for( x = 0; x < 4; x++ ) {
        arg_info = *(unsigned_16 PGM *)arg_list;
        arg_type = _ArgType( arg_info );
        arg_list += sizeof( unsigned_16 ) + ADDR_SIZE;
        if( arg_info & ARG_VALUE ) {
            arg_type = NumRegs[ arg_type ];
            if( arg_type ) {
                size += NumRegs[ arg_type ];
                if( size > 4 ) {
                    size -= NumRegs[ arg_type ];
                    return( size );
                } else if( size == 4 ) {
                    return( size );
                }
            } else {
                return( size );
            }
        } else {
            size += NumRegs[ PT_INT_4 ];
            if( size >= 4 ) {
                return( size );
            }
        }
    }
    return( size );
}


int     WFC_to_WF77( unsigned_8 PGM *arg_list, string PGM **ret_scb, ... ) {
//==========================================================================

// Call a WATFOR-77 subprogram from a subprogram compiled using WFC.

    int                 ret_type;
    va_list             args;
    unsigned short      arg_info;
    int                 size;

    va_start( args, ret_scb );
    arg_info = *(unsigned_16 PGM *)arg_list;
    arg_list += sizeof( unsigned_16 );
    ret_type = _ArgType( arg_info );
    size = numOfRegsUsed( arg_list ) * sizeof( int );
    size = processIncomingArguments( arg_list, args, _ArgCount(arg_info), size );
    if( ret_type == PT_CHAR ) {
        *ret_scb = (string PGM *)va_arg( args, void PGM * );
        size += sizeof( void PGM * );
    }
    va_end( args );
    return( size );
}

static ftn_type cdeclRetVal;

int     SYSCALL_to_WF77( unsigned_8 PGM *arg_list, string PGM **ret_scb, ... ) {
//==============================================================================

// Call a WATFOR-77 subprogram from a system subprogram

    int                 ret_type;
    va_list             args;
    unsigned short      arg_info;
    unsigned short      call_type;
    int                 size = 0;

    va_start( args, ret_scb );
    arg_info = *(unsigned_16 PGM *)arg_list;
    arg_list += sizeof( unsigned_16 );
    call_type = _CallType( *(unsigned_16 PGM *)arg_list );
    arg_list += sizeof( unsigned_16 );
    ret_type = _ArgType( arg_info );
    if( call_type == CDECL_CALL ) {
        *ret_scb = &cdeclRetVal.string;
    } else if( _RetStruct( ret_type, call_type ) ) {
        *ret_scb = va_arg( args, void PGM * );
        size = sizeof( void PGM * );
    }

    if( call_type == PASCAL_CALL ) {
        size += processIncomingRevArguments(arg_list, args, _ArgCount(arg_info),
                                            FULL_REGS );
    } else {
        size += processIncomingArguments( arg_list, args, _ArgCount(arg_info),
                                          FULL_REGS );
    }

    if( ret_type == PT_CHAR ) {
        *ret_scb = (string PGM *)va_arg( args, void PGM * );
        size += sizeof( void PGM * );
    }
    va_end( args );
    if( call_type == PASCAL_CALL ) {
        return( size );                 // callee clears stack
    } else {
        return( 0 );                    // caller clears stack
    }
}
