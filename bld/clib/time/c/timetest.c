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
* Description:  Non-exhaustive test of the C library time functions.
*
****************************************************************************/


#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __SW_BW
    #include <wdefwin.h>
#endif

#define VERIFY( exp )   if( !(exp) ) {                                      \
                            printf( "%s: ***FAILURE*** at line %d of %s.\n",\
                                    ProgramName, __LINE__,                  \
                                    strlwr(__FILE__) );                     \
                            NumErrors++;                                    \
                            exit( -1 );                                     \
                        }


char ProgramName[128];                          /* executable filename */
int NumErrors = 0;                              /* number of errors */



/****
***** Program entry point.
****/

int main( int argc, char *argv[] )
{
    clock_t	clocktime = clock();
    time_t	tt1, tt2;
    double      dtime;
    struct tm	tm1, tm2, *gmt;
    char	*datestr = "Wed Aug 14 17:23:31 2002\n";
    char	buf[64];
    
    #ifdef __SW_BW
        FILE *my_stdout;
        my_stdout = freopen( "tmp.log", "a", stdout );
        if( my_stdout == NULL ) {
            fprintf( stderr, "Unable to redirect stdout\n" );
            exit( -1 );
        }
    #endif
    /*** Initialize ***/
    strcpy( ProgramName, strlwr(argv[0]) );     /* store filename */

    /*** Test various functions ***/
    tt1 = time( &tt2 );
    /* Unfortunately we have no good way to verify that the current time
     * is being returned correctly. So let's just see if time() behaves
     * consistently.
     */
    VERIFY( tt1 == tt2 )
 
    tm1.tm_sec   = 31;
    tm1.tm_min   = 23;
    tm1.tm_hour  = 17;
    tm1.tm_mday  = 14;
    tm1.tm_mon   = 7;
    tm1.tm_year  = 102;
    tm1.tm_isdst = -1;
    tt1 = mktime( &tm1 );
    /* See if mktime() works properly */
    VERIFY( tm1.tm_wday == 3 );
    VERIFY( tm1.tm_yday == 225 );
    
    tm1.tm_sec++;
    tt2 = mktime( &tm1 );
    /* Test difftime() */
    dtime = difftime( tt1, tt2 );
    VERIFY( dtime == -1.0 );
    /* Test localtime() and asctime() */
    VERIFY( strcmp( asctime( localtime( &tt1 ) ), datestr ) == 0 );


    /* Test gmtime() and strtime() */
    gmt = gmtime( &tt2 );
    VERIFY( strftime( buf, sizeof( buf ), "%Y", gmt ) == 4 );
    VERIFY( strcmp( buf, "2002" ) == 0 );
    VERIFY( strftime( buf, sizeof( buf ), "%m", gmt ) == 2 );
    VERIFY( strcmp( buf, "08" ) == 0 );

    /* Make sure clock() isn't going backwards */
    VERIFY( clocktime <= clock() );
    
    /*** Print a pass/fail message and quit ***/
    if( NumErrors!=0 ) {
        printf( "%s: FAILURE (%d errors).\n", ProgramName, NumErrors );
        return( EXIT_FAILURE );
    }
    printf( "Tests completed (%s).\n", strlwr( argv[0] ) );
    #ifdef __SW_BW
    {
        fprintf( stderr, "Tests completed (%s).\n", strlwr( argv[0] ) );
        fclose( my_stdout );
        _dwShutDown();
    }
    #endif
    return( 0 );
}
