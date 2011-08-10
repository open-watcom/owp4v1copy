#include "fail.h"
#include <stdbool.h>

/* 
 * Test of conversion from / to _Bool bit-fields. Note that this test is written
 * such that it will fail unless it's built with -za99.
 */

#define TRUE    1
#define FALSE   0

static bool func_bol( const bool x ){ return x; }

struct bit2 {
    unsigned int uibf : 7;
      signed int sibf : 7;
             int pibf : 7;
            bool bobf : 1;
            bool bobx : 2;
} bits = { -1, -1, -1, -1, -1 };

struct bit3 {
    bool    b1;
    bool    b2  : 1;
    bool    b3  : 1;
    int     ibf : 1;
} bitz = { true, true, false, -1 };

int main( void ) 
{
    bool                bol;
    double              d;
    signed long long    slli;
    unsigned long long  ulli;
    signed long         sli;
    unsigned long       uli;
    signed              si;
    unsigned            ui;
    signed char         sc;
    unsigned char       uc;
    char                c;

    if( func_bol( FALSE ) == TRUE ) fail( __LINE__ );
    if( func_bol( TRUE ) != TRUE ) fail( __LINE__ );
    if( func_bol( -3 ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.uibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.sibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.pibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.bobf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.bobx ) != TRUE ) fail( __LINE__ );
    d = 4.33;
    bol = d;
    if( func_bol( d ) != TRUE ) fail( __LINE__ );
    bits.uibf = bol;
    bits.sibf = bol;
    bits.pibf = bol;
    bits.bobf = bol;
    bits.bobx = bol;
    if( bol != TRUE ) fail( __LINE__ );
    if( bits.uibf != TRUE ) fail( __LINE__ );
    if( bits.sibf != TRUE ) fail( __LINE__ );
    if( bits.pibf != TRUE ) fail( __LINE__ );
    if( bits.bobf != TRUE ) fail( __LINE__ );
    if( bits.bobx != TRUE ) fail( __LINE__ );
    if( func_bol( bol ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.uibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.sibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.pibf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.bobf ) != TRUE ) fail( __LINE__ );
    if( func_bol( bits.bobx ) != TRUE ) fail( __LINE__ );

    if( bitz.ibf != -1 ) fail( __LINE__ );
    if( bitz.b2  != 1 ) fail( __LINE__ );
    if( bitz.b2 == bitz.ibf ) fail( __LINE__ );

    slli = (  signed long long int)(bool)1u;
    ulli = (unsigned long long int)(bool)1u;
    if( slli != ulli ) fail( __LINE__ );
    slli = (  signed long long int)bitz.b1;
    ulli = (unsigned long long int)bitz.b1;
    if( slli != ulli ) fail( __LINE__ );
    slli = (  signed long long int)bitz.b2;
    ulli = (unsigned long long int)bitz.b2;
    if( slli != ulli ) fail( __LINE__ );
    slli = bitz.b2;
    ulli = bitz.b2;
    if( slli != ulli ) fail( __LINE__ );
    slli = bitz.b3;
    if( slli == ulli ) fail( __LINE__ );

    sli = (  signed long int)(bool)1u;
    uli = (unsigned long int)(bool)1u;
    if( sli != uli ) fail( __LINE__ );
    sli = (  signed long int)bitz.b1;
    uli = (unsigned long int)bitz.b1;
    if( sli != uli ) fail( __LINE__ );
    sli = (  signed long int)bitz.b2;
    uli = (unsigned long int)bitz.b2;
    if( sli != uli ) fail( __LINE__ );
    sli = bitz.b2;    
    uli = bitz.b2;
    if( sli != uli ) fail( __LINE__ );
    sli = bitz.b3;
    if( sli == uli ) fail( __LINE__ );

    si = (  signed int)(bool)1u;
    ui = (unsigned int)(bool)1u;
    if( si != ui ) fail( __LINE__ );
    si = (  signed int)bitz.b1;
    ui = (unsigned int)bitz.b1;
    if( si != ui ) fail( __LINE__ );
    si = (  signed int)bitz.b2;
    ui = (unsigned int)bitz.b2;
    if( si != ui ) fail( __LINE__ );
    si = bitz.b2;    
    ui = bitz.b2;
    if( si != ui ) fail( __LINE__ );
    ui = bitz.b3;
    if( si == ui ) fail( __LINE__ );

    sc = (  signed char)(bool)1u;
    uc = (unsigned char)(bool)1u;
    if( sc != uc ) fail( __LINE__ );
    sc = (  signed char)bitz.b1;
    uc = (unsigned char)bitz.b1;
    if( sc != uc ) fail( __LINE__ );
    sc = (  signed char)bitz.b2;
    uc = (unsigned char)bitz.b2;
    if( sc != uc ) fail( __LINE__ );
    sc = bitz.b2;    
    uc = bitz.b2;
    if( sc != uc ) fail( __LINE__ );
    uc = bitz.b3;
    if( sc == uc ) fail( __LINE__ );

    sc = (  signed char)(bool)1u;
    uc = (unsigned char)(bool)1u;
     c = (         char)(bool)1u;
    if( sc != c ) fail( __LINE__ );
    if( uc != c ) fail( __LINE__ );
    sc = (  signed char)bitz.b1;
    uc = (unsigned char)bitz.b1;
     c = (         char)bitz.b1;
    if( sc != c ) fail( __LINE__ );
    if( uc != c ) fail( __LINE__ );
    sc = (  signed char)bitz.b2;
    uc = (unsigned char)bitz.b2;
     c = (         char)bitz.b2;
    if( sc != c ) fail( __LINE__ );
    if( uc != c ) fail( __LINE__ );
    sc = bitz.b2;    
    uc = bitz.b2;
     c = bitz.b2;
    if( sc != c ) fail( __LINE__ );
    if( uc != c ) fail( __LINE__ );
     c = bitz.b3;
    if( c == uc ) fail( __LINE__ );
    if( c == sc ) fail( __LINE__ );

    _PASS;
}
