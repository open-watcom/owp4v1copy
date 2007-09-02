/*
 * OW 1.7 failure in &
 */

#include "fail.h"

int test_function( int a )
{
    return( (a & 0xffff0001) != 0xffff0001);
}

int main( void )
{
    if( test_function( 0xffffffff ) ) fail( __LINE__ );
    _PASS;
}
