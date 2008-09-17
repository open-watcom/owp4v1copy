#include "fail.h"


typedef int Array[2];

void f1( int (*ai)[2] )
{ }

void f2( const int (*ai)[2] )
{ }

void f3( Array *ai )
{ }

void f4( const Array *ai )
{ }


int g( char *a[][1] )
{
    return 0;
}

int g( char * const a[][1] )
{
    return 1;
}

int g( const char * const a[][1] )
{
    return 2;
}


int main() {
    int ( *ai1 )[2] = 0;
    const int ( *ai2 )[2] = 0;
    Array *ai3 = 0;
    const Array *ai4 = 0;


    f1( ai1 );
    // error: ai1 = ai2;
    // error: f1( ai2 );
    ai1 = ai3;
    f1( ai3 );
    // error: ai1 = ai4;
    // error: f1( ai4 );

    ai2 = ai1;
    f2( ai1 );
    ai2 = ai3;
    f2( ai3 );
    ai2 = ai4;
    f2( ai4 );

    ai3 = ai1;
    f3( ai1 );
    // error: ai3 = ai2;
    // error: f3( ai2 );
    f3( ai3 );
    // error: ai3 = ai4;
    // error: f3( ai4 );

    ai4 = ai1;
    f4( ai1 );
    ai4 = ai2;
    f4( ai2 );
    ai4 = ai3;
    f4( ai3 );
    f4( ai4 );


    char * ( *ac0 )[1] = 0;
    char * const ( *ac1 )[1] = 0;
    const char * const ( *ac2 )[1] = 0;

    if( g( ac0 ) != 0 ) fail( __LINE__ );
    if( g( ac1 ) != 1 ) fail( __LINE__ );
    if( g( ac2 ) != 2 ) fail( __LINE__ );


    _PASS;
}
