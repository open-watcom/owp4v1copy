#include "fail.h"

namespace ns {
template< class T >
struct A {
    int f();
    int g();
};

template< class T >
int A<T>::f() {
    return 1;
}
}

#if 0
// doesn't work with OW 1.3
template< class T >
int ns::A<T>::g() {
    return 1;
}
#else
namespace ns {
template< class T >
int A<T>::g() {
    return 1;
}
}
#endif

int main()
{
    ns::A<int> a;

    if( ! a.f() ) fail(__LINE__);
    if( ! a.g() ) fail(__LINE__);

    _PASS;
}
