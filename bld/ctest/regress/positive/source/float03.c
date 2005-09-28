#include "fail.h"

/*
 * NB: It should be possible to use two more digits on non-x86,
 * since DEC/MS compiler on Alpha can do it.
 */
 
double c[] = {
    1.2 - .2,
    .9 + .1,
    .3 * 10,
    3 / .2,
    .1111111111111111 <
    .1111111111111112,
    .1111111111111111 <=
    .1111111111111112,
    .1111111111111111 >
    .1111111111111112,
    .1111111111111111 >=
    .1111111111111112,
    .1111111111111111 ==
    .1111111111111112,
    .1111111111111111 !=
    .1111111111111112,
#ifdef __WATCOMC__
    /* This isn't standard C! */
    ( .23 , 1.25 )
#else
    1.25
#endif
};

unsigned n[] = {
    3,
    2,
    6,
    60,
    1,
    1,
    0,
    0,
    0,
    1,
    10,
};
unsigned d[] = {
    3,
    2,
    2,
    4,
    1,
    1,
    1,
    1,
    1,
    1,
    8,
};

int main() {
    unsigned *nn;
    unsigned *dd;
    double *p;

#ifdef __X86__
    if( (unsigned)34.99999999999999999999999999999999999999 != 34 ) fail(__LINE__);
#else
    /* Same as above - non-x86 platforms don't have enough accuracy */
    if( (unsigned)34.99999999999999 != 34 ) fail(__LINE__);
#endif
    if( ((double)2)/2/2*2*2 != 2 ) fail(__LINE__);
    nn = n;
    dd = d;
    for( p = c; p < &c[ sizeof(c) / sizeof(c[0]) ]; ++p ) {
        if( *p != ( (double)*nn ) / *dd ) {
            printf( "%f != %u/%u\n", *p, *nn, *dd );
            fail(__LINE__);
        }
        ++nn;
        ++dd;
    }
    _PASS;
}
