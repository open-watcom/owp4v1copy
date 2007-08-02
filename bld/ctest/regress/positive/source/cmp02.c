#include "fail.h"
#include <stdio.h>
#include <limits.h>

/* Test const folding for comparisons against constants which are completely
 * outside the variable's type range. Was broken in 1.6.
 */

int cmp_U1_hi_eq( unsigned char x )
{
    if( x == UCHAR_MAX + 1 ) {
        return( 1 );
    }
    return( 0 );
}

int cmp_U2_hi_eq( unsigned short x )
{
    if( x == USHRT_MAX + 1L ) {
        return( 1 );
    }
    return( 0 );
}

int cmp_U4_hi_eq( unsigned long x )
{
    if( x == ULONG_MAX + 1LL ) {
        return( 1 );
    }
    return( 0 );
}

unsigned char       uc;
unsigned short      us;
unsigned long       ul;

int main( void )
{
    if( cmp_U1_hi_eq( uc ) ) fail( __LINE__ );
    if( cmp_U2_hi_eq( us ) ) fail( __LINE__ );
    if( cmp_U4_hi_eq( ul ) ) fail( __LINE__ );

    _PASS;
}
