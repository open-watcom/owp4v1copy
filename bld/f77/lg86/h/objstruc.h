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


#define hash_entry lib_hash_entry
#include <pcobj.h>

typedef struct file_list {
    struct file_list    *next_file;
    struct obj_entry    *obj_files;
    char                name[1];
} file_list;

typedef struct obj_entry {
    struct obj_entry    *next_obj;
    struct pubnode      *publist;
    struct seg_entry    *seglist;
    struct grpnode      *grplist;
    unsigned_32         location;
    char                *member;
} obj_entry;

typedef struct group_entry {
    struct group_entry  *next_group;
    targ_addr           grp_addr;
    struct grp_members  *members;
    char                *name;
} group_entry;

typedef struct class_entry {
    struct class_entry  *next_class;
    struct seg_entry    *segs;
    bool                above;
    char                name[1];
} class_entry;

typedef struct seg_entry {
    struct seg_entry    *obj_seg;
    struct seg_entry    *next_seg;
    struct seg_entry    *addseg;
    sym_id              common;
    targ_addr           seg_addr;
    unsigned_16         size;
    unsigned_16         info;
    void                *segname;
} seg_entry;

//======================================================================
//
//   NOTES : during pass 1, addseg will point to the class name
//         : The info field is used as follows:
//
//  x x x x      x x x x      x x x x      x x x x
//                   | |      | | |        | |   |
//                   | |      | | |        | +---+---> combination
//                   | |      | | |        |
//                   | |      | | +--------+---------> alignment
//                   | |      | |
//                   | |      | +--------------------> in group
//                   | |      |
//                   | |      +----------------------> added segment
//                   | |
//                   | +-----------------------------> above all segment
//                   |
//                   +-------------------------------> non-combinable
//
//======================================================================

typedef struct node {
    pointer     next;
    pointer     entry;
} node;

typedef struct symnode {
    struct symnode      *next;
    pointer             entry;
} symnode;

typedef struct grpnode {
    struct grpnode      *next;
    group_entry         *entry;
} grpnode;

typedef struct grp_members {
    struct grp_members  *next;
    unsigned_16         num_segs;
    pointer             segs[1];
} grp_members;

typedef struct pubnode {
    struct pubnode      *next;
    unsigned_16         group;
    unsigned_16         seg;
    unsigned_16         frame;
    unsigned_16         offset;
    pointer             entry;
} pubnode;

typedef struct lib_proc {
    unsigned_32 dict_start;
    unsigned_16 dict_pages;
    unsigned_16 rec_length;
    byte        *buffer;
    unsigned_32 offset;
    char        *name;
} lib_proc;

typedef struct list_of_names {
    struct list_of_names        *next_name;
    bool                        keep;
    char                        name[1];
} list_of_names;

typedef struct liobject_data {
    targ_addr   addr;
    int         buff_len;
    byte        buffer[1];
} liobject_data;

typedef struct thread {
    pointer     next;
    byte        num;
    unsigned_8  flags;
    targ_addr   value;
} thread;

#define TRD_DEFINED             0x01    // FRAME or TARGET is defined
#define TRD_ABS_TARGET          0x02    // TARGET is absolute
#define TRD_FRAME_TARGET        0x04    // FRAME is same as segment of TARGET
