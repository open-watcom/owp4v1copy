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
// DBMODIFY     : debugger "modify" command
//

#include "ftnstd.h"
#include "buginfo.h"
#include "rundat.h"
#include "xfflags.h"
#include "errcod.h"

extern  void            ParseBlanks(void);
extern  bool            ParseChr(char);
extern  char            *ParsePtr(void);
extern  int             ParseName(void);
extern  void            DbMsg(uint);
extern  db_item         PGM *FindVar(char *,int);
extern  int             ItemType(db_item PGM *);
extern  int             ItemSubscripts(db_item PGM *);
extern  bool            SubScr(int);
extern  bool            SubStr(string *);
extern  void            DoFreeIn(void);
extern  int             Spawn();
extern  void            DbClearEOF(void);
extern  void            LimError(void);

extern  void            PGM *DbInAddr;
extern  const byte __FAR ItemSize[];

static  uint            DbInType;
static  unsigned_32     DbInCount;


static  io_type_rtn     DbIOType;

static  int     DbIOType() {
//==========================

// Get the type of an input item.

    if( __XcptFlags & XF_LIMIT_ERR ) {
        LimError();
    }
    if( DbInCount == 0 ) return( PT_NOTYPE );
    --DbInCount;
    if( DbInType == PT_CHAR ) {
        IORslt.string.len = ((string PGM *)DbInAddr)->len;
        IORslt.string.strptr = ((string PGM *)DbInAddr)->strptr;
        ((string PGM *)DbInAddr)->strptr += ((string PGM *)DbInAddr)->len;
    } else { // numeric or logical
        IORslt.pgm_ptr = DbInAddr;
        DbInAddr = (char PGM *)DbInAddr + ItemSize[ DbInType ];
    }
    return( DbInType );
}


void    DbModify() {
//==================

// Perform "modify" command.

    int         num_ss;
    unsigned_16 elt_size;
    db_item     PGM *item;
    char        *name;
    int         len;
    string      scb;
    io_type_rtn *save_iotype_rtn;

    ParseBlanks();
    name = ParsePtr();
    len = ParseName();
    if( len == 0 ) {
        DbMsg( MS_DB_SYNTAX_ERR );
        return;
    }
    item = FindVar( name, len );
    if( item == NULL ) {
        DbMsg( MS_DB_VAR_NOT_FOUND );
        return;
    }
    DbInCount = 1;
    DbInType = ItemType( item );
    num_ss = ItemSubscripts( item );
    if( num_ss != 0 ) {
        if( ((lg_adv PGM *)DbInAddr )->num_ss & ADV_NOT_INIT ){
            DbMsg( MS_DB_ARR_NOT_DIMMED );
            return;
        }
        elt_size = ((lg_adv PGM *)DbInAddr)->elt_size;
        if( ParseChr( '(' ) ) {
            if( !SubScr( num_ss ) ) {
                return;
            }
            if( DbInType == PT_CHAR ) {
                scb.len = elt_size;
                scb.strptr = DbInAddr;
                DbInAddr = &scb;
            }
            num_ss = 0;
        }
    }
    if( num_ss != 0 ) {
        DbInCount = ((lg_adv PGM *)DbInAddr)->num_elts;
        DbInAddr = ((lg_adv PGM *)DbInAddr)->origin;
        if( DbInType == PT_CHAR ) {
            scb.len = elt_size;
            scb.strptr = DbInAddr;
            DbInAddr = &scb;
        }
    } else if( DbInType == PT_CHAR ) { // character variable
        if( ParseChr( '(' ) ) {
            if( !SubStr( &scb ) ) {
                return;
            }
        }
        DbInAddr = &scb;
    }
    save_iotype_rtn = IOTypeRtn;
    IOTypeRtn = &DbIOType;
    Spawn( &DoFreeIn );
    IOTypeRtn = save_iotype_rtn;
    DbClearEOF();
    return;
}
