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


int main() {
    int (*ai1)[2] = 0;
    const int (*ai2)[2] = 0;
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


    _PASS;
}
