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


#include "vi.h"

/*
 * YankLineRange - yank a specified line range in current file
 */
vi_rc YankLineRange( linenum s, linenum e )
{
    vi_rc   rc;
    fcb     *s1fcb, *e1fcb;

    rc = GetCopyOfLineRange( s, e, &s1fcb, &e1fcb );
    if( rc != ERR_NO_ERR ) {
        return( rc );
    }
    AddFcbsToSavebuf( s1fcb, e1fcb, FALSE );
    LineYankMessage( s, e );
    return( ERR_NO_ERR );

} /* YankLineRange */

/*
 * GetCopyOfLineRange - yank a specified line range in current file
 */
vi_rc GetCopyOfLineRange( linenum s, linenum e, fcb **s1fcb, fcb **e1fcb )
{
    file        *cfile;
    linenum     ll;
    fcb         *sfcb, *efcb;
    fcb         *head = NULL, *tail = NULL;
    vi_rc       rc;
    vi_rc       rc1;
    vi_rc       rc2;

    /*
     * check line range
     */
    if( EditFlags.Modeless == FALSE ) {
        UnselectRegion();
    }
    if( s < 1 ) {
        return( ERR_NO_SUCH_LINE );
    }
    if( s > e ) {
        ll = s;
        s = e;
        e = ll;
    }

    /*
     * find start and ending fcbs
     */
    rc = FindFcbWithLine( s, CurrentFile, &sfcb );
    if( rc != ERR_NO_ERR ) {
        return( rc );
    }
    rc = FindFcbWithLine( e + 1, CurrentFile, &efcb );
    if( rc != ERR_NO_ERR ) {
        if( rc != ERR_NO_SUCH_LINE ) {
            return( rc );
        }
        if( IsPastLastLine( e ) ) {
            return( ERR_NO_SUCH_LINE );
        }
        rc = FindFcbWithLine( e, CurrentFile, &efcb );
        if( rc != ERR_NO_ERR ) {
            return( rc );
        }
    }

    /*
     * duplicate this list
     */
    *e1fcb = efcb->next;
    efcb->next = NULL;
    CreateDuplicateFcbList( sfcb, &head, &tail );
    efcb->next = *e1fcb;
    cfile = FileAlloc( NULL );
    cfile->fcb_head = head;
    cfile->fcb_tail = tail;

    /*
     * split head and tail fcbs
     */
    rc1 = SplitFcbAtLine( s, cfile, head );
    if( rc1 > 0 ) {
        return( rc1 );
    }
    rc = FindFcbWithLine( e + 1, cfile, &efcb );
    if( rc != ERR_NO_ERR ) {
        if( rc != ERR_NO_SUCH_LINE )  {
            return( rc );
        }
        rc = FindFcbWithLine( e, cfile, &efcb );
        if( rc != ERR_NO_ERR ) {
            return( rc );
        }
    }
    rc2 = SplitFcbAtLine( e + 1, cfile, efcb );
    if( rc2 > 0 ) {
        return( rc2 );
    }

    /*
     * select appropriate part of split fcbs
     */
    if( rc1 == NO_SPLIT_CREATED_AT_START_LINE ) {
        *s1fcb = head;
    } else {
        *s1fcb = head->next;
        FreeEntireFcb( head );
    }
    if( rc2 != NO_SPLIT_CREATED_AT_START_LINE ) {
        *e1fcb = efcb;
    } else {
        *e1fcb = efcb->prev;
        FreeEntireFcb( efcb );
        (*e1fcb)->next = NULL;
    }

    /*
     * tidy up and return
     */
    if( (*e1fcb)->next != NULL ) {
        FreeEntireFcb( (*e1fcb)->next );
    }
    (*s1fcb)->prev = (*e1fcb)->next = NULL;
    sfcb = *s1fcb;
    while( sfcb != NULL ) {
        sfcb->f = NULL;
        sfcb = sfcb->next;
    }
    FileFree( cfile );
    return( ERR_NO_ERR );

} /* GetCopyOfLineRange */

/*
 * LineYankMessage - display line deleted message
 */
void LineYankMessage( linenum s, linenum e )
{
#ifdef __WIN__
    if( LastSavebuf == 0 ) {
        Message1( "%l lines copied into the clipboard", e - s + 1 );
    } else {
#endif
        Message1( "%l lines yanked into buffer %c", e - s + 1, LastSavebuf );
#ifdef __WIN__
    }
#endif

} /* LineYankMessage */
