#include "fail.h"

#if defined( _M_I86 ) || defined( __386__ ) && !defined( __NT__ ) && !defined( __UNIX__ )

#define MK_FP(seg,off) (((__segment)(seg)):>((void __based(void) *)(off)))

// #pragma on (dump_init)
char __based(__segname("foo_TEXT")) bar[] = "abc";

/* Now const-fold the :> operator. */
/* NB: This doesn't work in 32-bit mode because the data initializer 
 * can't handle static initialization of 16:32 far pointers!
 */
void __far *lp = MK_FP( 0x1234, 0x5678 );

int __based(__segname("foo_TEXT")) main()
{
    if( bar[0] == 'a'
     && bar[1] == 'b'
     && bar[2] == 'c' ) {
    } else {
        fail( __LINE__ );
    }
    if( (unsigned long)lp != 0x12345678 ) fail( __LINE__ );
    _PASS;
}

#else

ALWAYS_PASS

#endif
