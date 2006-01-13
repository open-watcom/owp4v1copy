#include "fail.h"
#include <stdio.h>
#include <string.h>

/* C99 macro argument placement example */

#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,), t(10,,), t(,11,), t(,,12), t(,,) };

#define g(x,...) x##__VA_ARGS__

int ap, ple, apple;

/*
The above should expand to:
int j[] = { 123, 45, 67, 89, 10, 11, 12, };
*/

int main() {
    if( sizeof( j ) / sizeof( j[0] ) != 7 )
        fail(__LINE__);

    if( (j[0] != 123) || j[1] != 45 )
        fail(__LINE__);
    if( (j[2] != 67) || j[3] != 89 )
        fail(__LINE__);
    if( (j[4] != 10) || j[5] != 11 )
        fail(__LINE__);
    if( j[6] != 12 )
        fail(__LINE__);

    /* Some of the following used to crash the compiler */
    g(ap,ple) = apple;
    g(ap,)    = ap;
    g(,ple)   = ple;
//  The following is broken - macro expansion gets confused and returns
//  T_BAD_CHAR token which the parser is none too happy about; bug has
//  been there at least since version 10.6.
//    g(,)

    _PASS;
}
