#include "fail.h"

template< class T >
struct A { };

template< >
struct A<int> {
    static const int val = 0;
};

template< class T, class U = A< T > >
struct B {
    static const int val = U::val;
};

int main()
{
    if( B< int >::val != 0) fail(__LINE__);

    _PASS;
}
