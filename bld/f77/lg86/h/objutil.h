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
// OBJUTIL   : object memory access routines
//

extern  void    InitObj();
extern  uint_32 AvailMem();
extern  void    RelUnused();
extern  void    RelMem();
extern  void    NewSPUnit();
extern  void    NewGItem();
extern  void    AlignEven( inttarg size );
extern  void    BumpPtr( uint size );
extern  void    OutByte( char value );
extern  void    OutBytes( const char *src, uint len );
extern  void    OutU16( unsigned_16 value );
extern  void    OutInt( inttarg value );
extern  void    OutObjPtr( obj_ptr value );
extern  void    OutLocalRef( obj_ptr value );
extern  void    OutU32( unsigned_32 value );
extern  void    OutIIPtr();
extern  void    OutRTPtr();
extern  void    OutSCB( inttarg len, int, int );
extern  void    OutRCB( sym_id sd, int );
extern  void    PutU16( obj_ptr ptr, unsigned_16 val );
extern  obj_ptr PutPtr( obj_ptr ptr, obj_ptr val );
extern  void    Undefined( inttarg size );
extern  void    PutAddr( targ_addr *loc, targ_addr *link );
extern  obj_ptr ObjTell();
extern  int     ObjOffset( obj_ptr prev_obj );
extern  void    OutCodePtr( obj_ptr value );
extern  void    OutDataPtr( obj_ptr value );
