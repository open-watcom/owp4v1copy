#include "fail.h"
#include <stdlib.h>
#include <string.h>

/* Test C99 style declarations */

int foo( int arg )
{
    ++arg;
    int a = 0;
    --arg;
    volatile int i = 4;
    ++arg;
    const char *s;

    for( int i = 2, j = 1; i < 10; ++i )
        a += i + j;     // i in scope opened by 'for'

    return( a - i );    // back to the i in function scope
}

int main( void )
{
    if( foo( 1 ) != 48 ) fail( __LINE__ );
    _PASS;
}
