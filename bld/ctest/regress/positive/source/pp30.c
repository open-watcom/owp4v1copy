#include "fail.h"
#include <stdio.h>
#include <string.h>

/* C99 __VA_ARGS__ example */
char buff[1000];

#define debug(...) sprintf(buff, __VA_ARGS__)
#define showlist(...) strcpy(buff, #__VA_ARGS__)
#define report(test, ...) ((test)?(int)strcpy(buff,#test):\
                       sprintf(buff, __VA_ARGS__))

int main() {
    int x = 2, y = 3;
    debug( "Flag" );
    if ( strcmp( buff, "Flag" ))
        fail(__LINE__);
    debug("X = %d\n", x);
    if ( strcmp( buff, "X = 2\n" ))
        fail(__LINE__);
    showlist(The first, second, and third items.);
    if ( strcmp( buff, "The first, second, and third items." ))
        fail(__LINE__);
    report(x>y, "x is %d but y is %d\n", x, y);
    if ( strcmp( buff, "x is 2 but y is 3\n" ))
        fail(__LINE__);
    _PASS;
}
