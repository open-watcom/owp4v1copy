#include "fail.h"
//this was reduced down from stlport - it was crashing compiler
//simular test case to cltemp19.c but crashing in different place

template <class P>
class A {
};

template<>
class A<void> {
};

template <class Q, class R>
struct B{
};

template <class S, class T>
struct B<S, A<T> > {
  typedef int K;
};

struct C{
  typedef typename B<bool, A<bool> >::K L;
  typedef typename B<int, A<bool> >::K M;
};

int main() {
    C c;
    _PASS;
}
