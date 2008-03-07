#include "fail.h"


#if (defined(__386__) || defined(__I86__) || defined(_M_I86)) && !defined(__LINUX__)

// verify __interrupt implies default __far

typedef void __interrupt __far (*FAR_INT)( void );

extern void __interrupt int1( void );

void __interrupt __export int2( void )
{
}

void set_vec( void ( __interrupt *p )( void ) )
{
    if( sizeof( p ) != sizeof( FAR_INT ) ) fail(__LINE__);
    if( sizeof( &int1 ) != sizeof( FAR_INT ) ) fail(__LINE__);
    if( sizeof( &int2 ) != sizeof( FAR_INT ) ) fail(__LINE__);
}

void __far *Ptr;

// verify that far pointer <-> long long conversions can generate code
// and test other fun pointer <-> integer conversions
void ptr_cvt( void )
{
#ifdef _M_I86
    void __near *n_ptr;
    long        l_tmp;
#endif
    void __far  *f_ptr;
#if 0
    long long   tmp;

    f_ptr = Ptr;
    if( f_ptr ) fail(__LINE__); // Ptr had to be initialized to NULL
    tmp = (long long)f_ptr;
    if( Ptr != (void __far *)tmp ) fail(__LINE__);
    ++tmp;
    f_ptr = (void __far *)tmp;
    Ptr = f_ptr;
    if( Ptr != (void __far *)tmp ) fail(__LINE__);
    if( Ptr != (void __far *)1 ) fail(__LINE__);
#endif

#ifdef _M_I86
    /* 16-bit tests - assuming near pointer is 2 int/short sized, far pointer is long sized */
    /* The code is deliberately unclean and doesn't use explicit casts! */
#if 0
    n_ptr = NULL;
    l_tmp = n_ptr;
    f_ptr = n_ptr;
    if( l_tmp == 0 ) fail( __LINE__ );      /* must be DS:0, not zero */
    if( n_ptr != f_ptr ) fail( __LINE__ );  /* both conversions must yield the same result */
#endif

    f_ptr = 0x55;
    n_ptr = 0x55;
    if( n_ptr == f_ptr ) fail( __LINE__ );  /* DS:55 is not 0:55 */
    if( f_ptr == n_ptr ) fail( __LINE__ );  /* same as above */
    if( f_ptr != 0x55 ) fail( __LINE__ );   /* 0:55 is 0:55 */
    if( 0x55 != f_ptr ) fail( __LINE__ );   /* same as above */

    n_ptr = 0x123456;
    if( n_ptr != 0x3456 ) fail( __LINE__ );

    f_ptr = 0x123456;
    if( f_ptr != 0x123456 ) fail( __LINE__ );
#endif
}

#else

void set_vec( int i )
{
    i = i;
}

void ptr_cvt( void )
{
}

#endif

int main( void )
{
    set_vec( 0 );
    ptr_cvt();
    _PASS;
}
