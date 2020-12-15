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
* Description:  A simple file name pattern matching routine.
*
****************************************************************************/


static int my_fnmatch( char *pattern, char *string )
/**************************************************/
{
    char    *p;
    int     len;
    int     star_char;
    int     i;

    /*
     * check pattern section with wildcard characters
     */
    star_char = 0;
    while( ( *pattern == '*' ) || ( *pattern == '?' ) ) {
        if( *pattern == '?' ) {
            if( *string == 0 ) {
                return( 0 );
            }
            string++;
        } else {
            star_char = 1;
        }
        pattern++;
    }
    if( *pattern == 0 ) {
        if( ( *string == 0 ) || star_char ) {
            return( 1 );
        } else {
            return( 0 );
        }
    }
    /*
     * check pattern section with exact match
     * ( all characters except wildcards )
     */
    p = pattern;
    len = 0;
    do {
        if( star_char ) {
            if( string[ len ] == 0 ) {
                return( 0 );
            }
            len++;
        } else {
            if( *pattern != *string ) {
                return( 0 );
            }
            string++;
        }
        pattern++;
    } while( *pattern && ( *pattern != '*' ) && ( *pattern != '?' ) );
    if( star_char == 0 ) {
        /*
         * match is OK, try next pattern section
         */
        return( my_fnmatch( pattern, string ) );
    } else {
        /*
         * star pattern section, try locate exact match
         */
        while( *string ) {
            if( *p == *string ) {
                for( i = 1; i < len; i++ ) {
                    if( *( p + i ) != *( string + i ) ) {
                        break;
                    }
                }
                if( i == len ) {
                    /*
                     * if rest doesn't match, find next occurence
                     */
                    if( my_fnmatch( pattern, string + len ) ) {
                        return( 1 );
                    }
                }
            }
            string++;
        }
        return( 0 );
    }
}
