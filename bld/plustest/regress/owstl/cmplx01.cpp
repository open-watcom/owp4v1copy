
// These tests do not exercise the precision of complex calculations.
// The checks against expected values are done to three significant
// figures. They are only intended to verify that the correct algorithm
// is being used in the computation.

#include <cmath>
#include <complex>
#include <iostream>

typedef std::complex<double> complex;

#define CLOSE_ENOUGH(x, y) (std::abs(((x)/(y)) - 1.0) <= 1.0e-3)

static bool close_enough( const complex &x, const complex &y )
{
  if (CLOSE_ENOUGH( x.real( ), y.real( ) ) &&
      CLOSE_ENOUGH( x.imag( ), y.imag( ) ) )
    return( true );
  return( false );
}


bool construct_test( )
{
  bool rc = true;

  complex z1;
  complex z2( 1.0 );
  complex z3( 1.0, -1.0 );
  complex z4( z3 );

  // Don't use the close_enough() function until basic construction checks.
  if( !CLOSE_ENOUGH( z1.real( ), 0.0 ) ||
      !CLOSE_ENOUGH( z1.imag( ), 0.0 ) ) {
    std::cout << "basic FAIL 0001\n"; rc = false;
  }

  if( !CLOSE_ENOUGH( z2.real( ), 1.0 ) ||
      !CLOSE_ENOUGH( z2.imag( ), 0.0 ) ) {
    std::cout << "basic FAIL 0002\n"; rc = false;
  }

  if( !CLOSE_ENOUGH( z3.real( ),  1.0 ) ||
      !CLOSE_ENOUGH( z3.imag( ), -1.0 ) ) {
    std::cout << "basic FAIL 0003\n"; rc = false;
  }

  if( !CLOSE_ENOUGH( z4.real( ),  1.0 ) ||
      !CLOSE_ENOUGH( z4.imag( ), -1.0 ) ) {
    std::cout << "basic FAIL 0004\n"; rc = false;
  }

  return( rc );
}


bool add_test( )
{
  bool rc = true;

  complex z( 1.0, 1.0 );
  complex x;

  x = z + complex( 1.0, 1.0 );
  if( !close_enough( x, complex( 2.0, 2.0 ) ) ) {
    std::cout << "add FAIL 0001\n"; rc = false;
  }

  x = z + complex( 1.0, -2.0 );
  if( !close_enough( x, complex( 2.0, -1.0 ) ) ) {
    std::cout << "add FAIL 0002\n"; rc = false;
  }

  x = z + complex( -2.0, 1.0 );
  if( !close_enough( x, complex( -1.0, 2.0 ) ) ) {
    std::cout << "add FAIL 0003\n"; rc = false;
  }

  x = z + complex( -2.0, -2.0 );
  if( !close_enough( x, complex( -1.0, -1.0 ) ) ) {
    std::cout << "add FAIL 0004\n"; rc = false;
  }

  return( rc );
}


bool subtract_test( )
{
  bool rc = true;

  complex z( 1.0, 1.0 );
  complex x;

  x = z - complex( 0.5, 0.5 );
  if( !close_enough( x, complex( 0.5, 0.5 ) ) ) {
    std::cout << "subtract FAIL 0001\n"; rc = false;
  }

  x = z - complex( 2.0, -0.5 );
  if( !close_enough( x, complex( -1.0, 1.5 ) ) ) {
    std::cout << "subtract FAIL 0002\n"; rc = false;
  }

  x = z - complex( 2.0, 2.0 );
  if( !close_enough( x, complex( -1.0, -1.0 ) ) ) {
    std::cout << "subtract FAIL 0003\n"; rc = false;
  }

  x = z - complex( -0.5, 2.0 );
  if( !close_enough( x, complex( 1.5, -1.0 ) ) ) {
    std::cout << "subtract FAIL 0004\n"; rc = false;
  }

  return( rc );
}


bool multiply_test( )
{
  bool rc = true;

  complex z( 2.0, 2.0 );
  complex x;

  x = z * complex( 1.0, 1.5 );
  if( !close_enough( x, complex( -1.0, 5.0 ) ) ) {
    std::cout << "multiply FAIL 0001\n"; rc = false;
  }

  x = z * complex( 1.0, 0.1 );
  if( !close_enough( x, complex( 1.8, 2.2 ) ) ) {
    std::cout << "multiply FAIL 0002\n"; rc = false;
  }

  x = z * complex( -0.175, -0.425 );
  if( !close_enough( x, complex( 0.5, -1.2 ) ) ) {
    std::cout << "multiply FAIL 0003\n"; rc = false;
  }

  x = z * complex( -0.25, 0.0 );
  if( !close_enough( x, complex( -0.5, -0.5 ) ) ) {
    std::cout << "multiply FAIL 0004\n"; rc = false;
  }

  return( rc );
}


bool divide_test( )
{
  bool rc = true;

  complex z( 2.0, 2.0 );
  complex x;

  x = z / complex( 1.0, 1.5 );
  if( !close_enough( x, complex( 1.5385, -0.3077 ) ) ) {
    std::cout << "divide FAIL 0001\n"; rc = false;
  }

  x = z / complex( 0.9, 0.1 );
  if( !close_enough( x, complex( 2.4390, 1.9512 ) ) ) {
    std::cout << "divide FAIL 0002\n"; rc = false;
  }

  x = z / complex( -1.2235, 0.0941 );
  if( !close_enough( x, complex( -1.5, -1.75 ) ) ) {
    std::cout << "divide FAIL 0003\n"; rc = false;
  }

  x = z / complex( 0.0941, -1.2235 );
  if( !close_enough( x, complex( -1.5, 1.75 ) ) ) {
    std::cout << "divide FAIL 0004\n"; rc = false;
  }

  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    if( !construct_test( )   ) rc = 1;
    if( !add_test( )         ) rc = 1;
    if( !subtract_test( )    ) rc = 1;
    if( !multiply_test( )    ) rc = 1;
    if( !divide_test( )      ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
