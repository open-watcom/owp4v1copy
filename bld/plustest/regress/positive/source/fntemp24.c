#include "fail.h"

template< class T >
void g( T i, T j = T() )
{ }

template< class T >
struct A
{
    template< class U >
    void f( U i, U j = U() )
    { }
};


int main()
{
    g( 1, 2 );

    A< int > a;
    a.f( 1, 2 );

    _PASS;
}
