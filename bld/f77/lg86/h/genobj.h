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


extern  void            GObjInit(void);
extern  void            GObjFini(void);
extern  void            GObjSubInit(void);
extern  void            GObjSubFini(void);
extern  void            GObjDataItem(void);
extern  void            GObjStartLD(void);
extern  void            GObjStartGD(void);
extern  void            GObjEndGD(void);
extern  void            GObjEnable(void);
extern  void            GObjOutPtr(obj_ptr);
extern  void            GObjTargAddr(unsigned_32);
extern  void            GObjPutLink(targ_addr *,targ_addr *);
extern  int             GObjFindSeg(targ_addr);
extern  void            GObjPutAddr(targ_addr *,targ_addr *,int);
extern  obj_ptr         GObjPutPtr(obj_ptr,obj_ptr,int);
extern  void            GObjRelocExt(sym_id);
extern  void            GObjNoReloc( targ_addr );
extern  void            GObjComBlk(sym_id);
extern  void            GObjEndOfSPSeg( void );
extern  void            GObjFlushLEDATA( void );
extern  void            GObjSetLiteralSegmentLen( void );
extern  int             GObjPackedSegment( uint size );
extern  uint            GObjExistLiteralSegment( void );
extern  void            GObjMakeNewLiteralSeg( void );
extern  void            GObjSetGlobalSegmentLen( void );
extern  void            GObjSetGlobalSegFull( void );
extern  void            GObjSetCommonSegmentLen( void );
extern  void            GObjSetCommonSegFull( void );
extern  void            GObjMakeNewGlobalSeg( void );
extern  uint            GObjCalculateNumOfGblSegs( unsigned long size );
extern  int             GObjGlobalPackedSegment( uint size );
extern  uint            GObjGetGlobalVirtualPtr( void );
extern  void            GObjSetGlobalVirtualPtr( uint size );
extern  void            GObjIncGlobalVirtualPtr( uint size );
extern  unsigned long   GObjCalculatePrevSize( unsigned long offset );
extern  void            GObjMakeNewCommonSeg( void );
extern  void            GObjSetCommonBlockSize( unsigned_32 size );
extern  unsigned long   GObjGetCommonBlockSize( void );
extern  void            GObjSetCommonBlockLen( void );

