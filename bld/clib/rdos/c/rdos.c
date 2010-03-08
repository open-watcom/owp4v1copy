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
* Description:  RDOS routines that cannot be inlined
*
****************************************************************************/

#include <memory.h>
#include <process.h>
#include "rdos.h"

#define FALSE 0

typedef struct RdosPtr48
{
    const char *offset;
    int         sel;
} TRdosPtr48;

typedef struct RdosSpawnParam
{
    TRdosPtr48 param;
    TRdosPtr48 startdir;
    TRdosPtr48 env;
    TRdosPtr48 options;
} TRdosSpawnParam;

int RdosCarryToBool();

#pragma aux RdosCarryToBool = \
    CarryToBool \
    value [eax];

void RdosBlitBase();

#pragma aux RdosBlitBase = \
    CallGate_blit;

void RdosDrawMaskBase();

#pragma aux RdosDrawMaskBase = \
    CallGate_draw_mask;

void RdosGetBitmapInfoBase();

#pragma aux RdosGetBitmapInfoBase = \
    CallGate_get_bitmap_info;

void RdosReadDirBase();

#pragma aux RdosReadDirBase = \
    CallGate_read_dir;

void RdosGetModuleResourceBase();

#pragma aux RdosGetModuleResourceBase = \
    CallGate_get_module_resource;

void RdosCreateThread( void (*Startup)(void *Param), const char *Name, void *Param, int StackSize )
{
    _beginthread( Startup, Name, StackSize, Param );
}

void RdosCreatePrioThread( void (*Start)(void *Param), int Prio, const char *Name, void *Param, int StackSize )
{
    _beginthread( Start, Name, StackSize, Param );
}

void RdosSpawnBase();

#pragma aux RdosSpawnBase = \
    CallGate_spawn_exe;

void RdosBlit( int SrcHandle, int DestHandle, int width, int height, int SrcX, int SrcY, int DestX, int DestY )
{
    _asm {
        mov esi,SrcX
        mov eax,SrcY
        shl eax,16
        or esi,eax
        mov edi,DestX
        mov eax,DestY
        shl eax,16
        or edi,eax
        mov eax,SrcHandle
            mov ebx,DestHandle
        mov ecx,width
        mov edx,height
    }
    RdosBlitBase();
}

void RdosDrawMask( int handle, void *mask, int RowSize, int width, int height, int SrcX, int SrcY, int DestX, int DestY )
{
    _asm {
        mov ebx,handle
        mov edi,mask
        mov eax,RowSize
        mov esi,height
        mov eax,width
        shl eax,16
        or esi,eax
        mov ecx,SrcX
        mov eax,SrcY
        shl eax,16
        or ecx,eax
        mov edx,DestX
        mov eax,DestY
        shl eax,16
        or edx,eax
    }
    RdosDrawMaskBase();
}

void RdosGetBitmapInfo( int handle, int *BitPerPixel, int *width, int *height, int *linesize, void **buffer )
{
    _asm {
        mov ebx,handle
    }
    RdosGetBitmapInfoBase();
    _asm {
        mov ebx,BitPerPixel
        movzx eax,al
        mov [ebx],eax
        mov ebx,width
        movzx ecx,cx
        mov [ebx],ecx
        mov ebx,height
        movzx edx,dx
        mov [ebx],edx
        mov ebx,linesize
        movzx esi,si
        mov [ebx],esi
        mov ebx,buffer
        mov [ebx],edi
    }
}

int RdosReadDir( int Handle, int EntryNr, int MaxNameSize, char *PathName, long *FileSize, int *Attribute, unsigned long *MsbTime, unsigned long *LsbTime )
{
    int val;
    
    _asm {
        mov ebx,Handle
        mov edx,EntryNr
        mov ecx,MaxNameSize
        mov edi,PathName
    }
    RdosReadDirBase();
    _asm {
        mov esi,FileSize
        mov [esi],ecx
        movzx ebx,bx
        mov esi,Attribute
        mov [esi],ebx
        mov esi,MsbTime
        mov [esi],edx
        mov esi,LsbTime
        mov [esi],eax
    }
    val = RdosCarryToBool();
    return( val );
}

int RdosReadResource( int handle, int ID, char *Buf, int Size )
{
    char *RcPtr = 0;
    int RcSize;
    int ok;
    int i;
    char *src;
    char *dst;
    
    if( handle == 0 ) {
        _asm {
            mov eax,fs:[0x24]
            mov handle,eax
        }
    }

    _asm {
        push ds
        mov ebx,handle
        mov eax,ID
        mov edx,6
    }
    RdosGetModuleResourceBase();
    _asm {
        pop ds
        mov RcSize,ecx
        mov RcPtr,esi
    }
    ok = RdosCarryToBool();

    if( !ok )
        RcSize = 0;

    if( RcSize ) {
        if (RcSize > Size)
            RcSize = Size;

        src = RcPtr;
        dst = Buf;
        for( i = 0; i < RcSize; i++ ) {
            *dst = *src;
            dst++;
            src += 2;
        }            
    }    

    return( RcSize );
}

int RdosReadBinaryResource( int handle, int ID, char *Buf, int Size )
{
    char *RcPtr = 0;
    int RcSize;
    int ok;
    
    if( handle == 0 ) {
        _asm {
            mov eax,fs:[0x24]
            mov handle,eax
        }
    }

    _asm {
        push ds
        mov ebx,handle
        mov eax,ID
        mov edx,10
    }
    RdosGetModuleResourceBase();
    _asm {
        pop ds
        mov RcSize,ecx
        mov RcPtr,esi
    }
    ok = RdosCarryToBool();
    if( !ok )
        RcSize = 0;

    if( RcSize ) {
        if( RcSize > Size )
            RcSize = Size;
        memcpy( Buf, RcPtr, RcSize );            
    }    

    return( RcSize );
}

int RdosSpawn( const char *prog, const char *param, const char *startdir, const char *env, const char *options, int *thread )
{
    TRdosSpawnParam p;
    TRdosSpawnParam *pp;    
    int flatdata = 0;
    int ok = 0;
    int threadid = 0;
    int handle = 0;

    _asm {
        mov eax,ds
        mov flatdata,eax
    }

    if( param ) {
        p.param.offset = param;
        p.param.sel = flatdata;
    }
    else {
        p.param.offset = 0;
        p.param.sel = 0;
    }

    if( startdir ) {
        p.startdir.offset = startdir;
        p.startdir.sel = flatdata;
    }
    else {
        p.startdir.offset = 0;
        p.startdir.sel = 0;
    }

    if( env ) {
        p.env.offset = env;
        p.env.sel = flatdata;
    }
    else {
        p.env.offset = 0;
        p.env.sel = 0;
    }

    if( options ) {
        p.options.offset = options;
        p.options.sel = flatdata;
    }
    else {
        p.options.offset = 0;
        p.options.sel = 0;
    }

    pp = &p; 

    _asm {
        mov esi,prog
        mov edi,pp
        xor edx,edx
    }
    RdosSpawnBase();
    _asm {
        movzx eax,ax
        mov threadid,eax
        movzx edx,dx
        mov handle,edx
    }
    RdosCarryToBool();
    _asm {
        mov ok,eax
    }    

    if( ok ) {
        *thread = threadid;
        return( handle );
    }        
    else
        return( 0 );
}

int RdosSpawnDebug( const char *prog, const char *param, const char *startdir, const char *env, const char *options, int *thread )
{
    TRdosSpawnParam p;
    TRdosSpawnParam *pp;    
    int flatdata = 0;
    int ok = 0;
    int threadid = 0;
    int handle = 0;

    _asm {
        mov eax,ds
        mov flatdata,eax
    }

    if( param ) {
        p.param.offset = param;
        p.param.sel = flatdata;
    }
    else {
        p.param.offset = 0;
        p.param.sel = 0;
    }

    if( startdir ) {
        p.startdir.offset = startdir;
        p.startdir.sel = flatdata;
    }
    else {
        p.startdir.offset = 0;
        p.startdir.sel = 0;
    }

    if( env ) {
        p.env.offset = env;
        p.env.sel = flatdata;
    }
    else {
        p.env.offset = 0;
        p.env.sel = 0;
    }

    if( options ) {
        p.options.offset = options;
        p.options.sel = flatdata;
    }
    else {
        p.options.offset = 0;
        p.options.sel = 0;
    }

    pp = &p; 

    _asm {
        mov esi,prog
        mov edi,pp
        mov edx,fs:[0x24]
    }
    RdosSpawnBase();
    _asm {
        movzx eax,ax
        mov threadid,eax
        movzx edx,dx
        mov handle,edx
    }
    RdosCarryToBool();
    _asm {
        mov ok,eax
    }    

    if( ok ) {
        *thread = threadid;
        return( handle );
    }        
    else
        return( 0 );
}

