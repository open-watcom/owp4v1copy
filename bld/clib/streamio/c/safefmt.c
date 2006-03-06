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
* Description:  Non-exhaustive test of Safer C Library formatted I/O.
*
****************************************************************************/


#define __STDC_WANT_LIB_EXT1__  1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <stdarg.h>
#include <wchar.h>

#ifdef __SW_BW
    #include <wdefwin.h>
#endif

/* Test macros */

#define VERIFY( exp )   if( !(exp) ) {                                      \
                           printf( "%s: ***FAILURE*** at line %d of %s.\n", \
                                   ProgramName, __LINE__,                   \
                                   strlwr( __FILE__ ) );                    \
                           NumErrors++;                                     \
                           exit( -1 );                                      \
                       }

char    ProgramName[FILENAME_MAX];  /* executable filename */
int     NumErrors = 0;              /* number of errors */
int     NumViolations = 0;          /* runtime-constraint violation counter */


/* Runtime-constraint handler for tests; doesn't abort program. */
void my_constraint_handler( const char *msg, void *ptr, errno_t error )
{
#ifdef DEBUG_MSG
    fprintf( stderr, "Runtime-constraint message: %s", msg );
#endif
    ++NumViolations;
}

/**********************/
/* Main test routines */
/**********************/

int Test_scan( void )
/*******************/
{
    int     number;

    VERIFY( sscanf( "123", "%d", &number ) == 1 );
    VERIFY( number == 123 );

    return( 1 );
}


static int my_printf_s( const char *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vprintf_s( fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_fprintf_s( FILE *fp, const char *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vfprintf_s( fp, fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_sprintf_s( char * buf, rsize_t n, const char *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vsprintf_s( buf, n, fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_snprintf_s( char * buf, rsize_t n, const char *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vsnprintf_s( buf, n, fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_wprintf_s( const wchar_t *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vwprintf_s( fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_fwprintf_s( FILE *fp, const wchar_t *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vfwprintf_s( fp, fmt, arg );
    va_end( arg );
    return( rc );
}


static int my_swprintf_s( wchar_t * buf, rsize_t n, const wchar_t *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vswprintf_s( buf, n, fmt, arg );
    va_end( arg );
    return( rc );
}

static int my_snwprintf_s( wchar_t * buf, rsize_t n, const wchar_t *fmt, ... )
{
    va_list     arg;
    int         rc;

    va_start( arg, fmt );
    rc = vsnwprintf_s( buf, n, fmt, arg );
    va_end( arg );
    return( rc );
}


int Test_print( void )
/********************/
{
    char    buf[128];
    int     n;
    int     violations = NumViolations;

    VERIFY( printf_s( NULL ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( printf_s( "%n", &n ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( printf_s( "%s", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );


    VERIFY( fprintf_s( NULL, "hello" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( fprintf_s( stdout, NULL, 3 ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( fprintf_s( stdout, "%hhn", &n ) < 0 );
    VERIFY( ++violations == NumViolations );


    VERIFY( my_printf_s( NULL, 3, 6 ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_printf_s( "%jn", &n ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_printf_s( "%S", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );


    VERIFY( my_fprintf_s( NULL, "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_fprintf_s( stdout, NULL ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_fprintf_s( stdout, "%-8.3zn", &n ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_fprintf_s( stdout, "%S", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );


    VERIFY( sprintf_s( buf, 2, "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( sprintf_s( buf, sizeof( buf ), "%%n%d", 1 ) == 3 );
    VERIFY( !strcmp( buf, "%n1" ) );

    *buf = 'z';
    VERIFY( sprintf_s( buf, sizeof( buf ), NULL ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    *buf = 'z'; /* preinit output buffer ... */
    VERIFY( sprintf_s( NULL, sizeof( buf ), "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( sprintf_s( buf, 0, "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( sprintf_s( buf, ~0, "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
#endif


    VERIFY( my_sprintf_s( NULL, sizeof( buf ), "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_sprintf_s( buf, 0, "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( my_sprintf_s( buf, ~0, "hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
#endif

    VERIFY( *buf == 'z' );  /* ... and make sure no one touched it */

    VERIFY( my_sprintf_s( buf, 5, NULL, 'z', "oo", 36 ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_sprintf_s( buf, 6, "%c%s%d", 'z', "oo", 36 ) == 5 );
    VERIFY( !strcmp( buf, "zoo36" ) );

    VERIFY( my_sprintf_s( buf, 5, "%c%s%d", 'z', "oo", 36 ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_sprintf_s( buf, sizeof( buf ), "hi%#6.3n", &n ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_sprintf_s( buf, sizeof( buf ), "%%n%s", "Y" ) == 3 );
    VERIFY( !strcmp( buf, "%nY" ) );

    VERIFY( my_sprintf_s( buf, sizeof( buf ), "hi%s", NULL ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );


    VERIFY( my_snprintf_s( buf, sizeof( buf ), "hi%#6.3n", &n ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_snprintf_s( buf, 1, "hi%s", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    memset( buf, 'Q', 10 );
    VERIFY( my_snprintf_s( buf, 4, "%c%s%d", 'z', "oo", 3676 ) == 7 );
    VERIFY( !memcmp( buf, "zoo\0QQQQQQ", 10 ) );

    VERIFY( my_snprintf_s( buf, sizeof( buf ), "%c%s%d", 'z', "oo", 36 ) == 5 );
    VERIFY( !strcmp( buf, "zoo36" ) );

    VERIFY( my_snprintf_s( NULL, sizeof( buf ), "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_snprintf_s( buf, 0, "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( my_snprintf_s( buf, ~0, "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );
#endif


    VERIFY( snprintf_s( NULL, sizeof( buf ), "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( snprintf_s( buf, 0, "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( snprintf_s( buf, ~0, "hi" ) < 0 );
    VERIFY( ++violations == NumViolations );
#endif

    VERIFY( *buf == 'z' );

    VERIFY( snprintf_s( buf, sizeof( buf ), NULL, 3, 5 ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( snprintf_s( buf, sizeof( buf ), "hi%#3.55n", &n ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( snprintf_s( buf, sizeof( buf ), "hi%s", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    return( 1 );
}


int Test_print_wide( void )
/*************************/
{
    wchar_t     buf[128];
    int         n;
    int         violations = NumViolations;

    /* we can't do any actual output to stdout, because it's
     * byte oriented by now. Do just the most basic tests to
     * verify all the functions are there.
     */
    VERIFY( wprintf_s( NULL ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( fwprintf_s( NULL, L"hello" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( fwprintf_s( stdout, NULL, 3 ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_wprintf_s( NULL, 3, 6 ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_fwprintf_s( NULL, L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_fwprintf_s( stdout, NULL ) < 0 );
    VERIFY( ++violations == NumViolations );


    /* But we can properly test the string output functions... */
    VERIFY( swprintf_s( buf, 2, L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( swprintf_s( buf, sizeof( buf ), L"%%n%d", 1 ) == 3 );
    VERIFY( !wcscmp( buf, L"%n1" ) );

    *buf = 'z';
    VERIFY( swprintf_s( buf, sizeof( buf ), NULL ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    *buf = 'z'; /* preinit output buffer ... */
    VERIFY( swprintf_s( NULL, sizeof( buf ), L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( swprintf_s( buf, 0, L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( swprintf_s( buf, ~0, L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
#endif


    VERIFY( my_swprintf_s( NULL, sizeof( buf ), L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_swprintf_s( buf, 0, L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( my_swprintf_s( buf, ~0, L"hi" ) == 0 );
    VERIFY( ++violations == NumViolations );
#endif

    VERIFY( *buf == 'z' );  /* ... and make sure no one touched it */

    VERIFY( my_swprintf_s( buf, 5, NULL, 'z', "oo", 36 ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_swprintf_s( buf, 6, L"%c%s%d", 'z', L"oo", 36 ) == 5 );
    VERIFY( !wcscmp( buf, L"zoo36" ) );

    VERIFY( my_swprintf_s( buf, 5, L"%c%s%d", 'z', L"oo", 36 ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_swprintf_s( buf, sizeof( buf ), L"hi%#6.3n", &n ) == 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_swprintf_s( buf, sizeof( buf ), L"%%n%s", L"Y" ) == 3 );
    VERIFY( !wcscmp( buf, L"%nY" ) );

    VERIFY( my_swprintf_s( buf, sizeof( buf ), L"hi%s", NULL ) == 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );


    VERIFY( my_snwprintf_s( buf, sizeof( buf ), L"hi%#6.3n", &n ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( my_snwprintf_s( buf, 1, L"hi%s", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    wmemset( buf, 'Q', 10 );
    VERIFY( my_snwprintf_s( buf, 4, L"%c%s%d", 'z', L"oo", 3676 ) == 7 );
    VERIFY( !wmemcmp( buf, L"zoo\0QQQQQQ", 10 ) );

    VERIFY( my_snwprintf_s( buf, sizeof( buf ), L"%c%s%d", 'z', L"oo", 36 ) == 5 );
    VERIFY( !wcscmp( buf, L"zoo36" ) );

    VERIFY( my_snwprintf_s( NULL, sizeof( buf ), L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( my_snwprintf_s( buf, 0, L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( my_snwprintf_s( buf, ~0, L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );
#endif


    VERIFY( snwprintf_s( NULL, sizeof( buf ), L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

    VERIFY( snwprintf_s( buf, 0, L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );

#if RSIZE_MAX != SIZE_MAX
    VERIFY( snwprintf_s( buf, ~0, L"hi" ) < 0 );
    VERIFY( ++violations == NumViolations );
#endif

    VERIFY( *buf == 'z' );

    VERIFY( snwprintf_s( buf, sizeof( buf ), NULL, 3, 5 ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( snwprintf_s( buf, sizeof( buf ), L"hi%#3.55n", &n ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    VERIFY( snwprintf_s( buf, sizeof( buf ), L"hi%s", NULL ) < 0 );
    VERIFY( ++violations == NumViolations );
    VERIFY( *buf == '\0' );

    return( 1 );
}


int main( int argc, char *argv[] )
/********************************/
{
#ifdef __SW_BW
    FILE    *my_stdout;

    my_stdout = freopen( "tmp.log", "a", stdout );
    if( my_stdout == NULL ) {
        fprintf( stderr, "Unable to redirect stdout\n" );
        exit( -1 );
    }
#endif
    /*** Initialize ***/
    strcpy( ProgramName, strlwr( argv[0] ) );   /* store filename */

    /****************************/
    /*  set constraint-handler  */
    /****************************/

    set_constraint_handler_s( my_constraint_handler );

    /******************/
    /* Start of tests */
    /******************/

//    Test_scan();
    Test_print();
    Test_print_wide();

    /****************/
    /* End of tests */
    /****************/

    /*** Print a pass/fail message and quit ***/
    if( NumErrors != 0 ) {
        printf( "%s: FAILURE (%d errors).\n", ProgramName, NumErrors );
        return( EXIT_FAILURE );
    }
    printf( "Tests completed (%s).\n", strlwr( argv[0] ) );
#ifdef __SW_BW
    fprintf( stderr, "Tests completed (%s).\n", strlwr( argv[0] ) );
    fclose( my_stdout );
    _dwShutDown();
#endif

    return( 0 );
}
