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


gbl_defn label_id    EpiLabel;       // label for start of epilogue

gbl_defn unsigned_32 CodeSize;       // size of program object code
gbl_defn unsigned_32 DataSize;       // size of local data area(s)
gbl_defn unsigned_32 GlobalSize;     // size of global data area

gbl_defn targ_addr   SProgStart;

gbl_defn obj_ptr     LocalData;

gbl_defn targ_addr   PrevDChain;     // pointer to previous data statement

gbl_defn obj_ptr     EndEqLabel;     // points to label for ATEND code

gbl_defn label_id    TBLabel;        // label for traceback struct
gbl_defn bool        TBLabelPatched; // have we backpatched the TBLabel?
gbl_defn label_id    EpiFCode;       // label for epilogue F-Code

gbl_defn obj_ptr     ObjPtr;         // address (segment offset) of fcode data

gbl_defn char        *LinkFile;      // name of .LNK file to use

#if _TARGET == _8086
gbl_defn unsigned_16 CurrSeg;        // segment address
#endif
