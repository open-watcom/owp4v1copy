#include "fail.h"

struct A {
    static const int val1 = 0;
    static const int val2 = val1 + 1;
    static const int val3 = val1 + val2;
};

int main() {
    A a;

    if( A::val2 != A::val3 ) fail(__LINE__);
    if( a.val2 != a.val3 ) fail(__LINE__);

    _PASS;
}
