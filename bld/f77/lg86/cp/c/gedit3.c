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
// GEDIT3       : editor interface for 386 version (pharlap only)
//

#include "ftnstd.h"
#include "edit.h"

#include <process.h>
#include <dos.h>
#include <string.h>
#include <tinyio.h>

static char             *EdtName = { "weditf.exe" };
static unsigned_32      oldRealVect;
static char __far       *protPtr;
static unsigned_16      realSeg;

#define REAL_PARAS      0x100   /* allocate 4k of real mode memory */
#define STK_SIZE        256     /* stack for switching to 16-bit editor */
static const char       editStub[] = {
    0x2e, 0x8c, 0x16, 36, 0,                    /* mov cs:L1,ss         */
    0x2e, 0x89, 0x26, 38, 0,                    /* mov cs:L2,ss         */
    0xfa,                                       /* cli                  */
    0x8e, 0xd7,                                 /* mov ss,di            */
    0xbc, STK_SIZE & 0xff, STK_SIZE >> 8,       /* mov sp,STK_SIZE      */
    0xfb,                                       /* sti                  */
    0x9c,                                       /* pushf                */
#define HOOK_OFFS       19
    0x9a, 0, 0, 0, 0,                           /* call far hook        */
    0xfa,                                       /* cli                  */
    0x2e, 0x8e, 0x16, 36, 0,                    /* mov ss,cs:L1         */
    0x2e, 0x8b, 0x26, 38, 0,                    /* mov sp,cs:L2         */
    0xfb,                                       /* sti                  */
    0xcf,                                       /* iret                 */
    0, 0,                                       /* L1 dw 0              */
    0, 0,                                       /* L2 dw 0              */
#define SIG_OFFS        40
    'W', 'A', 'T', 'C', 'O', 'M', ' ',          /* signature            */
    'S', 'y', 's', 't', 'e', 'm', 's',
    ' ', 'I', 'n', 'c', '.', 0
};


tiny_ret_t DOSAlloc( unsigned_16 );
#pragma aux DOSAlloc = \
    _MOV_AH 0x25 \
    _MOV_AL 0xc0 \
    _INT_21 \
    _RCL_AX_1   \
    _ROR_AX_1   \
    parm [bx] value [eax] modify exact [eax ebx];

tiny_ret_t DOSFree( unsigned_16 );
#pragma aux DOSFree = \
    _MOV_AH 0x25 \
    _MOV_AL 0xc1 \
    _INT_21 \
    _SBB_AX_AX \
    parm [cx] value [eax] modify exact [eax ecx];


unsigned_32 GetRealVect( unsigned_8 );
#pragma aux GetRealVect = \
    _MOV_AH 0x25 \
    _MOV_AL 0x03 \
    _INT_21 \
    parm [cl] value [ebx];


void SetRealVect( unsigned_8, unsigned_32 );
#pragma aux SetRealVect = \
    _MOV_AH 0x25 \
    _MOV_AL 0x05 \
    _INT_21 \
    parm [cl] [ebx];


int     InitEditor() {
//====================

// Initialize editor.

    tiny_ret_t  r;

    // allocate some real mode memory for our use

    r = DOSAlloc( REAL_PARAS );
    if( TINY_ERROR( r ) ) return( 0 );
    realSeg = TINY_INFO( r );
    protPtr = MK_FP( 0x34, (unsigned_32)realSeg * 16 );

    // copy our stub program to the bottom of that real mode memory

    _fmemcpy( protPtr, editStub, sizeof( editStub ) );

    // Before spawning the editor, set the interrupt vector to point at the
    // copyright string so that the editor knows that it is being spawned.

    oldRealVect = GetRealVect( EDIT_INTERRUPT );
    SetRealVect( EDIT_INTERRUPT, ( (unsigned_32)realSeg << 16 ) + SIG_OFFS );
    if( spawnlp( P_WAIT, EdtName, NULL ) == 0x0300 ) {
        *(unsigned_32 __far *)(protPtr + HOOK_OFFS)
                = GetRealVect( EDIT_INTERRUPT );
        SetRealVect( EDIT_INTERRUPT, (unsigned_32)realSeg << 16 );
        return( 0x0300 );
    }

    // could not invoke editor
    DOSFree( realSeg );
    SetRealVect( EDIT_INTERRUPT, oldRealVect );

    return( 0 );
}


void    FiniEditor() {
//====================

// Terminate editor.

    if( realSeg ) {
        EditInt1( realSeg, ED_FINIEDITOR );
        DOSFree( realSeg );
    }

    // restore old interrupt handler
    SetRealVect( EDIT_INTERRUPT, oldRealVect );
}


#define MAX_CMD 127

int     DoEdit( char *str, int line_num ) {
//=========================================

    int rc;

// Invoke editor.

    _fmemcpy( protPtr + STK_SIZE, str, MAX_CMD + 1 );
    rc = EditInt3( realSeg, ED_EDITFILE, STK_SIZE, line_num );
    _fmemcpy( str, protPtr + STK_SIZE, MAX_CMD + 1 );
    return( rc );
}


int     ResumeEditor( char *str, int line_num ) {
//===============================================

    int rc;

// Resume editor.

    _fmemcpy( protPtr + STK_SIZE, str, MAX_CMD + 1 );
    rc = EditInt3( realSeg, ED_RESUMEEDITOR, STK_SIZE, line_num );
    _fmemcpy( str, protPtr + STK_SIZE, MAX_CMD + 1 );
    return( rc );
}


int     EOpenf( char *buffname ) {
//================================

// Open editor buffer.

    _fstrcpy( protPtr + STK_SIZE, buffname );
    return( EditInt2( realSeg, ED_OPENF, STK_SIZE ) );
}


int     EGetRec( unsigned_16 handle, char *buff, int len ) {
//==========================================================

// Get record from editor buffer.

    len = EditInt4( realSeg, ED_GETREC, handle, STK_SIZE, len );
    _fmemcpy( buff, protPtr + STK_SIZE, len );
    return( len );
}


void    EClosef( unsigned_16 handle ) {
//=====================================

// Close editor buffer.

    EditInt2( realSeg, ED_CLOSEF, handle );
}


int     EEof( unsigned_16 handle ) {
//==================================

// Check for end of buffer.

    return( EditInt2( realSeg, ED_EOF, handle ) );
}
