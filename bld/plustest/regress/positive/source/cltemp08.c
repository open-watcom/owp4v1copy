#include "fail.h"

template<class T> struct S;

template<>
struct S<char> {
    friend struct S<int>;
};

template<>
struct S<int> {
};

template<>
struct S<short>;

template<>
struct S<short> {
};

int main() {
    S<char> s1;
    S<int> s2;
    S<short> s3;

    _PASS;
}
