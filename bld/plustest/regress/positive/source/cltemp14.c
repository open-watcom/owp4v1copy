#include "fail.h"

int g() {
    return 1;
}

int h() {
    return 0;
}


template <class T> struct A;

template <class T> struct A<T *>;

template< > struct A<long> { };


A<int *> *ptr;

typedef A<long> B;

template< > struct A<short> { };


template <class T>
struct A {
    int f() {
        return g();
    }
};

template <class T>
struct A<T *> {
    int f() {
        return h();
    }
};


int main()
{
    A<int *> a;
    if( a.f() != 0 ) fail( __LINE__ );

    ptr = &a;
    if( ptr->f() != 0 ) fail( __LINE__ );

    _PASS;
}
