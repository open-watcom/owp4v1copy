#include "fail.h"

#ifdef _M_IX86    // x86 only!

/* This test is primarily for 16-bit environments, but applies to some
 * 32-bit environments as well. In flat model, this test should still
 * succeed but data segment references just collapse to 'DS'.
 * The test is meaningful in 386 small model, but only if the OS really
 * supports small model -- like DOS/4G or OS/2, but not NT or Linux whose
 * executable formats simply have no concept of segment relocations.
 *
 */

#if !(defined( __NT__ ) || defined( __LINUX__ ))

#if !defined( __FLAT__ )

/* This creates a MY_DATA segment and ends up there. */
unsigned __based( __segname( "MY_DATA" ) ) u_arr1[8] = { 0 };

typedef unsigned __based( __segname( "MY_DATA" ) ) *bpu;

/* This ends up in the default data segment (DGROUP). */
unsigned long u_arr2[8] = { 0 };

#pragma data_seg( MY_DATA )

/* This ends up in the MY_DATA segment again. */
unsigned u_arr3[8] = { 0 };

#pragma data_seg()

/* This creates a HIS_DATA segment and ends up there. */
#pragma data_seg( "HIS_DATA" )

unsigned u_arr4[8] = { 0 };

#pragma data_seg()

typedef unsigned __based( __segname( "HIS_DATA" ) ) *bp2;


unsigned __far *fn1a( void )
{
    return( &u_arr1[3] );   /* C compiler folds this to OPR_DOT */
}

unsigned __far *fn1b( int idx )
{                           /* This case was broken */
    return( &u_arr1[idx] ); /* C compiler can't fold, uses OPR_INDEX */
}

unsigned __far *fn1c( int idx )
{                           /* Explicit case worked */
    return( (bpu)&u_arr1[idx] );
}

unsigned __far *fn3a( void )
{
    return( &u_arr3[3] );
}

unsigned __far *fn3b( int idx )
{
    return( &u_arr3[idx] );
}

unsigned __far *fn3c( int idx )
{
    return( (bpu)&u_arr3[idx] );
}

unsigned __far *fn4a( void )
{
    return( &u_arr4[3] );
}

unsigned __far *fn4b( int idx )
{
    return( &u_arr4[idx] );
}

unsigned __far *fn4c( int idx )
{
    return( (bp2)&u_arr4[idx] );
}

unsigned gv1( int idx )
{
    return( u_arr1[idx] );
}

unsigned gv2( int idx )
{
    return( u_arr2[idx] );
}

unsigned gv3( int idx )
{
    return( u_arr3[idx] );
}

unsigned gv4( int idx )
{
    return( u_arr4[idx] );
}

__segment seg1( void )
{
    return( (__segment)u_arr1 );    /* MY_DATA */
}

__segment seg2( void )
{
    return( (__segment)u_arr2 );    /* DGROUP */
}

__segment seg3( void )
{
    return( (__segment)u_arr3 );    /* MY_DATA */
}

__segment seg4( void )
{
    return( (__segment)u_arr4 );    /* HIS_DATA */
}

int main( void )
{
    if( fn1a() != fn1b( 3 ) ) fail( __LINE__ );
    if( fn1a() != fn1c( 3 ) ) fail( __LINE__ );

    if( fn3a() != fn3b( 3 ) ) fail( __LINE__ );
    if( fn3a() != fn3c( 3 ) ) fail( __LINE__ );

    if( fn4a() != fn4b( 3 ) ) fail( __LINE__ );
    if( fn4a() != fn4c( 3 ) ) fail( __LINE__ );

    if( seg1() != seg3() )  fail( __LINE__ );
    if( seg1() == seg2() )  fail( __LINE__ );
    if( seg4() == seg2() )  fail( __LINE__ );

    u_arr1[5] = 1;
    u_arr2[5] = 2;
    u_arr3[5] = 3;
    u_arr4[5] = 4;
            
    if( gv1( 5 ) != 1 ) fail( __LINE__ );
    if( gv2( 5 ) != 2 ) fail( __LINE__ );
    if( gv3( 5 ) != 3 ) fail( __LINE__ );
    if( gv4( 5 ) != 4 ) fail( __LINE__ );
    
    _PASS;
}

#else /* !defined( __FLAT__ ) */

ALWAYS_PASS

#endif

#else /* !((defined( __NT__ ) || defined( __LINUX__ )) */

ALWAYS_PASS

#endif

#else /* defined( _M_IX86 ) */

ALWAYS_PASS

#endif
