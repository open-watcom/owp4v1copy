#include "fail.h"

int foo( int b, int t, int f )
{
    return b ? t : f;
}

int main() {
    if( foo( 1, -1, -2 ) != -1 ) fail(__LINE__);
    if( foo( 0, -1, -2 ) != -2 ) fail(__LINE__);
    _PASS;
}
