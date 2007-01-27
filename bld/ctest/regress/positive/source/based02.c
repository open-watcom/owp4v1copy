#include "fail.h"

#if defined( _M_IX86 )

struct S {
    int s;
};

typedef struct S __based(__segname("_STACK")) *SS_PTR;

void set_to_one( SS_PTR p )
{
    p->s = 1;
}

int main()
{
    struct S x;

    x.s = 0;
    set_to_one( &x );
    if( x.s != 1 ) fail(__LINE__);
    _PASS;
}

#else

ALWAYS_PASS

#endif
