#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

#include "fail.h"

typedef void * __stdcall f1( char p1, short p2, int p3, long p4, long long p5 );
typedef __declspec(__stdcall) void * f2( char p1, short p2, int p3, long p4, long long p5 );

void * __stdcall test1( char p1, short p2, int p3, long p4, long long p5 );
__declspec(__stdcall) void * test1( char p1, short p2, int p3, long p4, long long p5 );

extern void **(__stdcall * x3)( char p1, short p2, int p3, long p4, long long p5 );
__declspec(__stdcall) void **(* x3)( char p1, short p2, int p3, long p4, long long p5 );

int res[5];

f1 *x1;
f2 *x2;

__declspec(__stdcall) void * test1( char p1, short p2, int p3, long p4, long long p5 )
{
    res[0] = p1;
    res[1] = p2;
    res[2] = p3;
    res[3] = p4;
    res[4] = p5;
    return 0;
}

__declspec(__stdcall) void ** test2( char p1, short p2, int p3, long p4, long long p5 )
{
    res[0] = p1;
    res[1] = p2;
    res[2] = p3;
    res[3] = p4;
    res[4] = p5;
    return 0;
}

int main( void )
{
    x1 = test1;
    x1( 1, 2, 3, 4, 5 );
    if( res[0] != 1 ) fail( __LINE__ );
    if( res[1] != 2 ) fail( __LINE__ );
    if( res[2] != 3 ) fail( __LINE__ );
    if( res[3] != 4 ) fail( __LINE__ );
    if( res[4] != 5 ) fail( __LINE__ );
    x2 = test1;
    x2( 1, 2, 3, 4, 5 );
    if( res[0] != 1 ) fail( __LINE__ );
    if( res[1] != 2 ) fail( __LINE__ );
    if( res[2] != 3 ) fail( __LINE__ );
    if( res[3] != 4 ) fail( __LINE__ );
    if( res[4] != 5 ) fail( __LINE__ );
    x3 = test2;
    x3( 1, 2, 3, 4, 5 );
    if( res[0] != 1 ) fail( __LINE__ );
    if( res[1] != 2 ) fail( __LINE__ );
    if( res[2] != 3 ) fail( __LINE__ );
    if( res[3] != 4 ) fail( __LINE__ );
    if( res[4] != 5 ) fail( __LINE__ );
    _PASS;
}

#ifdef __cplusplus
}
#endif
