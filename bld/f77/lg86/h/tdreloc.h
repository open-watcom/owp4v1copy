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
* Description:  Symbol table relocation.
*
****************************************************************************/


extern  unsigned_32     ArrNumElts(lg_adv PGM *);
extern  void            LitSCBReloc(sym_id);
extern  void            TmpSCBReloc(sym_id);
extern  void            TmpRCBReloc(sym_id);
extern  void            SetGblOrg(sym_id);
extern  unsigned_32     GblOffset(sym_id);
extern  void            StartGD(void);
extern  void            AlignSeg(void);
extern  void            SetComAddr(sym_id);
extern  void            SetIFAddr(int,sym_id);
extern  void            SetIFName(sym_id);
extern  void            MakeGblAddr(sym_id,sym_id);
extern  void            GblReloc(sym_id);
extern  void            RelocAddr(targ_addr *,targ_addr *);
extern  void            ComVarReloc(com_reloc *,obj_addr *);
extern  void            GblVarReloc(targ_addr *,targ_addr *,signed_32);
extern  void            LinkComReloc(sym_id,com_reloc *);
extern  void            LinkGblSym(sym_id,sym_id);
extern  void            LinkGblChain(obj_ptr,targ_addr *);
extern  void            GblUndefined(unsigned_32);
extern  void            ConstReloc(sym_id,obj_ptr);
extern  void            VarReloc(sym_id,obj_ptr);
extern  void            DataReloc(sym_id,obj_ptr);
extern  void            CodeReloc(sym_id,obj_ptr);
extern  void            LclReloc(obj_ptr,obj_ptr);
extern  obj_ptr         LinkReloc(obj_ptr *);
extern  obj_ptr         LinkSym(sym_id);
extern  void            SymRef(sym_id);
extern  void            CodeSymRef(sym_id);
extern  void            PackGblData(unsigned long);
extern  void            SetCurrentGblAddr(sym_id,unsigned_32 *);
extern  void            SetCommonBlockSize(unsigned_32);
extern  void            CloseCommonBlock(void);
extern  void            SetEQVariablesAddr(sym_id,sym_id,unsigned_32);
extern void             SetEQLeadersAddr(sym_id);
