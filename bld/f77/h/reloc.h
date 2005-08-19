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


// Relocation definitions:
// =======================

#if _CPU != 8086 && _CPU != 386
typedef struct sym_reloc {
    struct sym_reloc    *link;          // pointer to next relocation chain
    pointer             head;           // head of relocation chain
} sym_reloc;
#endif

typedef struct com_reloc {
    struct com_reloc    *link;          // pointer to common relocation entry
    targ_addr           reloc_chain;    // relocation chain
    unsigned_32         offset;         // offset into common block
#if _CPU == 370
    bool                in_data;        // data initialized
#endif
} com_reloc;

typedef union reloc_head {
    targ_addr           gr;             // head of global relocation chain
#if _CPU == 8086 || _CPU == 386
    obj_ptr             lr;             // head of local relocation chain
#else
    struct sym_reloc    *lr;            // head of local relocation chain
#endif
    com_reloc           *cr;            // head of common block relocation chain
} reloc_head;

typedef union obj_addr {
    obj_ptr             la;             // local address
    targ_addr           ga;             // global address
} obj_addr;
