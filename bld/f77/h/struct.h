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


// Define FORTRAN 77 structures:
// =============================

typedef struct fstruct {
    struct fstruct      *link;                  // next structure definition
#if _OPT_CG == _OFF
    obj_ptr             reloc_chain;            // head of relocation chain
#endif
    struct field        *fields;                // fields of structure
    intstar4            size;                   // size of structure
#if _OPT_CG == _ON
    unsigned short      cg_typ;                 // type for code generator
    unsigned short      dbi;                    // for debugging information
#endif
    unsigned_32         dbh;                    // browsing info handle
    byte                name_len;               // length of structure name
    char                name[STD_SYMLEN];       // structure name
} fstruct;

// Note: the fields of "fmap" must match the fields in "fstruct".

typedef struct fmap {
    struct fmap         *link;                  // next map definition
#if _OPT_CG == _OFF
    obj_ptr             reloc_chain;            // head of relocation chain
#endif
    struct field        *fields;                // fields of map
    intstar4            size;                   // size of map
} fmap;

typedef struct field {
    struct field        *link;                  // next field
    byte                typ;                    // type of field
    union {
        uint            size;                   // size of field
        struct fstruct  *record;                // pointer to structure
    } xt;
    struct act_dim_list *dim_ext;               // dimension information
    byte                name_len;               // length of name of field
    char                name[STD_SYMLEN];       // name of field
} field;

// Note: the fields of "funion" must match the fields in "field".

typedef struct funion {
    struct field        *link;                  // next field
    byte                typ;                    // type of field
    struct fmap         *record;                // pointer to map
//  struct fmap         *biggest;               // pointer to biggest map
} funion;
