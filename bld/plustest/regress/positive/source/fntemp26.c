#include "fail.h"


struct A{
    struct B1{
        B1() {}
    };

    struct B2{
        B2(){}
        B2( B1 ) {}
    };

    template< class T >
    void fn( B2, T ) {}

    template< class T >
    void fn( int, T ) {}

    template< class T >
    void fn( short, T ) {}
};

int main(void)
{
    A a;
    A::B1 b1;
    A::B2 b2;

    a.fn(b1,1);
    a.fn(b2,1);

    _PASS;
}
