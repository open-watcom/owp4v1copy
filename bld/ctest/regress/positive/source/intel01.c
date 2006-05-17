#include "fail.h"


#if (defined(__386__) || defined(__I86__)) && !defined(__LINUX__)

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
void ptr_cvt( void )
{
    void __far  *ptr;
    long long   tmp;

    ptr = Ptr;
    tmp = (long long)ptr;
    ++tmp;
    ptr = (void __far *)tmp;
    Ptr = ptr;
    if( Ptr != (void __far *)1 ) fail(__LINE__);
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
