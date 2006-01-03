#include "fail.h"

#if defined( M_I86 ) || defined( M_I386 )

#include <i86.h>

int main( void )
{
    char                    c0, c1, c2, c3;
    char __based( void )    *off;
    __segment               seg;

    c0 = 0xAA;

    off = (char __based( void ) *)FP_OFF( &c0 );
    seg = FP_SEG( &c0 );

    // Test various ways of data access through based pointer
    c1 = *(seg:>off);
    c2 = *(char __based( seg ) *)off;
    c3 = ((char __based( seg ) *)off)[0];

    if( c0 != c1 || c1 != c2 || c2 != c3 ) fail(__LINE__);
    _PASS;
}

#else

ALWAYS_PASS

#endif
