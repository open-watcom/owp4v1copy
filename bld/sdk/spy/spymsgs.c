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


#include "spy.h"
#include <stdio.h>
#include <string.h>
#include <dde.h>

static message *userMsg;

/*
 * GetMessageDataFromID - use message id to look up message structure
 */
message *GetMessageDataFromID( int msgid, char *class_name )
{
    int i;

    for( i=0;i< MessageArraySize;i++ ) {
        if( msgid == MessageArray[i].id ) return( &MessageArray[i] );
    }
    if( !stricmp( class_name, "edit" ) ) {
        for( i = 0; i < EditMessageArraySize; i++ ) {
            if( msgid == EditMessageArray[i].id ) {
                return( &EditMessageArray[i] );
            }
        }
    } else if( !stricmp( class_name, "button" ) ) {
        for( i = 0; i < ButtonMessageArraySize; i++ ) {
            if( msgid == ButtonMessageArray[i].id ) {
                return( &ButtonMessageArray[i] );
            }
        }
    } else if( !stricmp( class_name, "static" ) ) {
        for( i = 0; i < StaticMessageArraySize; i++ ) {
            if( msgid == StaticMessageArray[i].id ) {
                return( &StaticMessageArray[i] );
            }
        }
    } else if( !stricmp( class_name, "listbox" ) ) {
        for( i = 0; i < ListBoxMessageArraySize; i++ ) {
            if( msgid == ListBoxMessageArray[i].id ) {
                return( &ListBoxMessageArray[i] );
            }
        }
#ifdef NT_MSGS
    } else if( !stricmp( class_name, "combobox" ) ||
               !stricmp( class_name, WC_COMBOBOXEX ) ) {
#else
    } else if( !stricmp( class_name, "combobox" ) ) {
#endif
        for( i = 0; i < ComboBoxMessageArraySize; i++ ) {
            if( msgid == ComboBoxMessageArray[i].id ) {
                return( &ComboBoxMessageArray[i] );
            }
        }
#ifdef NT_MSGS
    } else if( !stricmp( class_name, "scrollbar" ) ) {
        for( i = 0; i < ScrollBarMessageArraySize; i++ ) {
            if( msgid == ScrollBarMessageArray[i].id ) {
                return( &ScrollBarMessageArray[i] );
            }
        }
#endif
    }
    return( NULL );

} /* GetMessageDataFromID */

/*
 * ProcessIncomingMessage - get a string associated with a message id
 */
void ProcessIncomingMessage( int msgid, char *class_name, char *res )
{
    message     *msg;
    char        *fmtstr;
    char        buf[256];

    res[0] = 0;
    msg = GetMessageDataFromID( msgid, class_name );
    if( msg != NULL ) {
        if( msg->bits[M_WATCH] ) {
            strcpy( res, msg->str );
        }
        if( msg->bits[M_STOPON] ) {
            SetSpyState( OFF );
            RCsprintf( buf, STR_SPYING_STOPPED, msg->str );
            MessageBox( SpyMainWindow, buf, SpyName,
                        MB_OK | MB_ICONINFORMATION );
        }
        msg->count++;
    } else if( msgid > WM_USER ) {
        userMsg->count++;
        if( userMsg->bits[M_WATCH] ) {
            fmtstr = GetRCString( STR_WM_USER_PLUS );
            sprintf( res, fmtstr, msgid - WM_USER );
        }
        if( userMsg->bits[M_STOPON] ) {
            SetSpyState( OFF );
            fmtstr = GetRCString( STR_WM_USER_PLUS );
            sprintf( res, fmtstr, msgid - WM_USER );
            RCsprintf( buf, STR_SPYING_STOPPED, res );
            MessageBox( SpyMainWindow, buf, SpyName,
                        MB_OK | MB_ICONINFORMATION );
        }
    } else {
        if( Filters.filts.unknown.flag[M_WATCH] ) {
            fmtstr = GetRCString( STR_UNKNOWN_MSG );
            sprintf( res, fmtstr, msgid );
        }
        if( Filters.filts.unknown.flag[M_STOPON] ) {
            SetSpyState( OFF );
            fmtstr = GetRCString( STR_UNKNOWN_MSG );
            RCsprintf( buf, STR_SPYING_STOPPED, res );
            MessageBox( SpyMainWindow, buf, SpyName,
                        MB_OK | MB_ICONINFORMATION );
        }
    }

} /* ProcessIncomingMessage */


/*
 * SetFilterMsgs
 */
void SetFilterMsgs( MsgClass type, BOOL val, int bit ) {
    int i;

    for( i=0; i< MessageArraySize; i++ ) {
        if( MessageArray[i].type == type ) {
            MessageArray[i].bits[bit] = val;
        }
    }
}

/*
 * SetFilterSaveBitsMsgs
 */
void SetFilterSaveBitsMsgs( MsgClass type, BOOL val, char *bits ) {
    int i;

    for( i=0; i< MessageArraySize; i++ ) {
        if( MessageArray[i].type == type ) {
            bits[i] = val;
        }
    }
}

/*
 * InitMessages - init. messages structres
 */
void InitMessages( void )
{
    userMsg = GetMessageDataFromID( WM_USER, NULL );

} /* InitMessages */

/*
 * SaveBitState - save current watch/stopon state
 */
char *SaveBitState( int x )
{
    char        *data;
    int         i;

    data = MemAlloc( MessageArraySize );
    if( data == NULL ) return( NULL );
    for( i=0;i<MessageArraySize;i++) {
        data[i] = MessageArray[i].bits[x];
    }
    return( data );

} /* SaveBitState */

/*
 * CloneBitState - make a copy of a saved bitstate
 */
char *CloneBitState( char *old ) {
    char        *data;

    data = MemAlloc( MessageArraySize );
    if( data == NULL ) return( NULL );
    memcpy( data, old, MessageArraySize );
    return( data );
}

/*
 * RestoreBitState - put back watch/stopon state
 */
void RestoreBitState( char *data, int x )
{
    int         i;

    if( data == NULL ) return;
    for( i=0;i<MessageArraySize;i++) {
        MessageArray[i].bits[x] = data[i];
    }
    MemFree( data );

} /* RestoreBitState */

/*
 * FreeBitState - release memory used by a saved bit state
 *              - should not be called for states for which RestoreBitState
 *                has been called
 */
void FreeBitState( char *data ) {
    MemFree( data );
}

void CopyBitState( char *dst, char *src ) {
    memcpy( dst, src, MessageArraySize );
}

/*
 * ClearMessageCount - clear count of each type of message
 */
void ClearMessageCount( void )
{
    int i;

    for( i=0;i<MessageArraySize;i++) {
        MessageArray[i].count = 0L;
    }

} /* ClearMessageCount */
