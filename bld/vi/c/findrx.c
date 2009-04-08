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
#include "win.h"
#include "rxsupp.h"

static char wrapMsg[] = "Wrapped past %s of file";
static bool wrapMsgPrinted;

/*
 * FindRegularExpression - do a forward search for a regular expression
 */
int FindRegularExpression( char *pat, i_mark *pos1, char **linedata,
                                                    linenum termline, int sw )
{
    int         rc;
    int         found;
    linenum     ilineno;
    bool        wrapped = FALSE;
    char        *data;
    line        *cline;
    fcb         *cfcb;
    int         scol;

    /*
     * initialize for search
     */
    if( wrapMsgPrinted ) {
        wrapMsgPrinted = FALSE;
        ClearWindow( MessageWindow );
    }
    if( sw ) {
        ilineno = pos1->line;
    }
    rc = CGimmeLinePtr( pos1->line, &cfcb, &cline );
    if( rc ) {
        return( rc );
    }
    scol = pos1->column;
    if( pat != NULL ) {
        rc = CurrentRegComp( pat );
        if( rc ) {
            return( rc );
        }
    }

    /*
     * loop until string found
     */
    while( TRUE ) {

        data = &cline->data[ scol ];
        found = RegExec( CurrentRegularExpression, data, (data == cline->data) );
        if( RegExpError != ERR_NO_ERR ) {
            return( RegExpError );
        }

        if( found ) {
            *linedata = cline->data;
            pos1->column = GetCurrRegExpColumn( cline->data );
            return( ERR_NO_ERR );
        }

        /*
         * get next line
         */
        rc = CGimmeNextLinePtr( &cfcb, &cline );
        if( rc ) {
            if( rc == ERR_NO_MORE_LINES ) {
                if( !sw ) {
                    return( ERR_FIND_END_OF_FILE );
                } else {
                    Message1( wrapMsg, "bottom" );
                    MyBeep();
                    wrapMsgPrinted = TRUE;
                }
                if( wrapped ) {
                    return( ERR_FIND_NOT_FOUND );
                }
                pos1->line = 1;
                rc = CGimmeLinePtr( pos1->line, &cfcb, &cline );
                if( rc ) {
                    return( rc );
                }
                pos1->line = 0;
                wrapped = TRUE;
            } else {
                return( rc );
            }
        }
        scol = 0;
        pos1->line += 1;
        if( pos1->line > termline ) {
            return( ERR_FIND_PAST_TERM_LINE );
        }
        if( wrapped ) {
            if( pos1->line > ilineno ) {
                return( ERR_FIND_NOT_FOUND );
            }
        }
    }
} /* FindRegularExpression */

/*
 * FindRegularExpressionBackwards - do a reverse search for a regular expression
 */
int FindRegularExpressionBackwards( char *pat, i_mark *pos1, char **linedata,
                                                    linenum termline, int sw )
{
    int         rc;
    char        *data;
    bool        wrapped = FALSE;
    bool        found;
    linenum     ilineno;
    line        *cline;
    fcb         *cfcb;
    regexp      rcpy;
    int         scol;

    /*
     * initialize for search
     */
    rc = CGimmeLinePtr( pos1->line, &cfcb, &cline );
    if( rc ) {
        return( rc );
    }
    if( sw ) {
        ilineno = pos1->line;
    }
    scol = pos1->column;
    if( pat != NULL ) {
        rc = CurrentRegComp( pat );
        if( rc ) {
            return( rc );
        }
    }

    /*
     * loop until string found
     */
    while( TRUE ) {

        data = cline->data;
        found = FALSE;
        /*
         * run through all possible matches on the line, accepting
         * only the last one
         */
        while( TRUE ) {
            rc = RegExec( CurrentRegularExpression, data, (data == cline->data) );
            if( RegExpError != ERR_NO_ERR ) {
                return( RegExpError );
            }
            if( rc ) {
                int     col, len;
                col = GetCurrRegExpColumn( cline->data );
                len = GetCurrRegExpLength();
                if( col + len - 1 > scol ) {
                    break;
                }
                found = TRUE;
                memcpy( &rcpy, CurrentRegularExpression, sizeof( regexp ) );
                data = &(cline->data[ col + 1 ]);
                if( *data == 0 ) {
                    break;
                }
            } else {
                break;
            }
        }

        if( found ) {
            *linedata = cline->data;
            memcpy( CurrentRegularExpression, &rcpy, sizeof( regexp ) );
            pos1->column = GetCurrRegExpColumn( cline->data );
            return( ERR_NO_ERR );
        }

        /*
         * get next line
         */
        rc = GimmePrevLinePtr( &cfcb, &cline );
        if( rc ) {
            if( rc == ERR_NO_MORE_LINES ) {
                if( !sw ) {
                    return( ERR_FIND_TOP_OF_FILE );
                } else {
                    Message1( wrapMsg, "top" );
                    MyBeep();
                    wrapMsgPrinted = TRUE;
                }
                if( wrapped ) {
                    return( ERR_FIND_NOT_FOUND );
                }
                rc = CFindLastLine( &pos1->line );
                if( rc ) {
                    return( rc );
                }
                rc = CGimmeLinePtr( pos1->line, &cfcb, &cline );
                if( rc ) {
                    return( rc );
                }
                pos1->line += 1;
                wrapped = TRUE;
            } else {
                return( rc );
            }
        }
        scol = cline->len - 1;
        pos1->line -= 1;
        if( pos1->line < termline ) {
            return( ERR_FIND_PAST_TERM_LINE );
        }
        if( wrapped ) {
            if( pos1->line < ilineno ) {
                return( ERR_FIND_NOT_FOUND );
            }
        }
    }
} /* FindRegularExpressionBackwards */
