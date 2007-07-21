/*
 *  Change 31737
 *  Previously, a static prototype combined with a pragma aux definition would have been seen 
 *  as not defined and caused this error:
 *
 *      Error! E043: col(1) static function 'foo' has not been defined
 *      Note! N392: col(12) definition: 'int __pragma("foo") foo( void )'
 */
#include "fail.h"

static int foo(void);

#pragma aux foo = \
    "mov  eax,1"  \
    parm caller [eax]
    
int main (void)
{
    int x = foo();
    
    _PASS;
     
    return x;
}
