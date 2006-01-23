#include "fail.h"
#include <stdlib.h>
#include <string.h>

/* Test initialization of _Bool objects */

// any non-zero scalar gets converted to 1
_Bool ab[] = {
    32,
    0x20,
    3.1415,
    2.2f,
    (void *)4,
    0x2000000000,
};

int fni( int a )
{
    _Bool   b = a;

    return( b );
}

int fnd( double a )
{
    _Bool   b = a;

    return( b );
}

int fnp( void *a )
{
    _Bool   b = a;

    return( b );
}

int main( void )
{
    char    *b;

    // note - ISO standard doesn't guarantee that sizeof( _Bool ) is 1
    b = (char *)ab;
    for( int i = 0; i < sizeof( ab ) / sizeof( ab[0] ); ++i ) {
        if( b[i] != 1 ) fail( __LINE__ );
    }

    if( fni( 42 ) + fnd( 6.28 ) + fnp( &main ) != 3 ) fail( __LINE__ );
    _PASS;
}
