#include "fail.h"

template< typename T >
struct A {
    typedef T D;
};

template< typename C >
struct B {
    typename C::D t;

    typename C::D f() {
        return typename C::D();
    }
};


int main()
{
    B< A< int > > b;

    int &i = b.t;
    i = b.f();

    _PASS;
}
