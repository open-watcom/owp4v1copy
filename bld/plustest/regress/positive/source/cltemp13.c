#include "fail.h"

template< typename T >
struct A {
    typedef T D;
};

template< typename C >
struct B {
    typename C::D t;
};


int main()
{
    B< A< int > > b;

    int &i = b.t;
    i = 0;

    _PASS;
}
