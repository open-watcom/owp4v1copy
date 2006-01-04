#include "fail.h"

#if defined( M_I86 ) || defined( M_I386 )

struct S {
    int s;
};

typedef S __based(__segname("_STACK")) *SS_PTR;

void set_to_one( SS_PTR p )
{
    p->s = 1;
}

__segment get_seg( SS_PTR p )
{
    return( __segment( p ) );
}

int main()
{
    S x;

    x.s = 0;
    set_to_one( &x );
    if( x.s != 1 ) fail(__LINE__);
#ifdef M_I86
    // this only works in 16-bit mode
    if( get_seg( &x ) != ((unsigned long)((void __far *)&x) >> 16)) fail(__LINE__);
#endif
    _PASS;
}

#else

ALWAYS_PASS

#endif
