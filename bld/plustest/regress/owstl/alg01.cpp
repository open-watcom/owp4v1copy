/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2006 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  This file contains the functional tests for the algorithms
*               in <algorithm>.
*
****************************************************************************/

#include <cstring>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include "sanity.cpp"

// Used in some of the tests.
void advance_char( char &ch )
{
  if( ++ch == 'z' ) ch = 'a';
}

// Used in some of the tests.
bool is_odd( int num )
{
  return( static_cast<bool>( num % 2 ) );
}

// Used in some of the tests.
bool is_divisible( int x, int y )
{
  return( !(x % y) );
}

bool for_each_test( )
{
  bool rc = true;

  char str[] = { 'h', 'e', 'l', 'l', 'o', '\0' };

  std::for_each( str, str, advance_char );
  if( std::strcmp( str, "hello" ) != 0 ) FAIL;

  std::for_each( str, str + 5, advance_char );
  if( std::strcmp( str, "ifmmp" ) != 0 ) FAIL;

  return( rc );
}

bool find_test( )
{
  bool rc = true;

  int  array[] = { 0, 1, 2, 3, 4 };
  int *p;

  p = std::find( array, array + 5, 2 );
  if( p == array + 5 || *p != 2 ) FAIL;

  p = std::find( array, array + 5, 5 );
  if( p != array + 5 ) FAIL;

  p = std::find( array, array, 1 );
  if( p != array ) FAIL;

  p = std::find_if(
        array, array + 5, std::bind2nd( std::greater< int >( ), 3 ) );
  if( p == array + 5 || *p != 4 ) FAIL;

  p = std::find_if( array, array + 5, is_odd );
  if( p == array + 5 || *p != 1 ) FAIL;

  return( rc );
}

bool find_end_test( )
{
  int a[] = { 1, 1, 1, 1, 1, 1 };
  int b[] = { 1, 1, 1 };
  int c[] = { 2, 4, 6, 8, 10, 12 };
  int d[] = { 6, 8 };
  int e[] = { 2, 4, 6, 8, 10, 12 };
  int f[] = { 3, 6, 6, 8, 6, 5, 8, 6, 8, 7 };    //10 elements
  int g[] = { 10 };
  //find last of multiple matches
  if( std::find_end( a, a+6, b, b+3 ) != a+3 ) FAIL
  //can't find because it subsequence is longer than sequence
  if( std::find_end( b, b+3, a, a+6 ) != b+3 ) FAIL
  //can't find because no match
  if( std::find_end( a, a+6, d, d+2 ) != a+6 ) FAIL
  //no match, same size sequences
  if( std::find_end( a, a+6, c, c+6 ) != a+6 ) FAIL
  //exact matching sequence
  if( std::find_end( c, c+6, e, e+6 ) != c ) FAIL
  //find in middle
  if( std::find_end( c, c+6, d, d+2 ) != c+2 ) FAIL
  //multi matches
  if( std::find_end( f, f+10, d, d+2 ) != f+7 ) FAIL
  //substring is only 1 long
  if( std::find_end( c, c+6, g, g+1 ) != c+4 ) FAIL

  //quick test with predictate, there should realy be more but it is
  //just a copy of the above, as so long as any updates are applied to
  //both functions it will be ok.
  int h[] = { 2,3 };
  if( std::find_end( c, c+6, h, h+2, is_divisible ) != c+4 ) FAIL

  return( true );
}

bool find_first_of_test( )
{
  using namespace std;
  string s("the cat sat\non the mat");
  char* s2 = "cmo";
  char* n1 = "xyz";
  string::iterator i, j, k;
  i = find_first_of( s.begin(), s.end(), s2, s2+3 );
  k = s.begin() + 4;
  if( *i != s[4] || i != k ) FAIL
  //don't find anything
  i = find_first_of( s.begin(), s.end(), n1, n1+3 );
  if( i != s.end() ) FAIL
  
  string s3("sat\non");
  string s4;
  string ws(" \n");
  //skip past 2 spaces
  i = find_first_of( s.begin(), s.end(), ws.begin(), ws.end() );
  ++i;
  i = find_first_of( i, s.end(), ws.begin(), ws.end() );
  ++i;
  //skip another 2 spaces
  j = find_first_of( i, s.end(), ws.begin(), ws.end() );
  ++j;
  j = find_first_of( j, s.end(), ws.begin(), ws.end() );
  while( i != j ) s4 += *i++;
  if( s3 != s4 ) FAIL
  
  // test binary predictate version:
  int x[] = { 3, 12, 17, 19 };
  int y[] = { 5, 4, 7, 11 };
  int* i2;
  //find a x that is evenly divisible by a y
  i2 = find_first_of( x, x+4, y, y+4, is_divisible );
  if( i2 != x+1 ) FAIL
  //don't find one
  i2 = find_first_of( x, x+4, y+2, y+4, is_divisible );
  if( i2 != x+4 ) FAIL
  
  return( true );
}

#ifdef NEVER
bool count_test( )
{
  bool rc = true;

  char array[] = { 'a', 'b', 'b', 'd', 'e' };
  int n = -1;

  n = std::count( array, array + 5, 'b' );
  if( n != 2 ) FAIL;

  n = std::count( array, array + 5, 'c' );
  if( n != 0) FAIL;

  n = std::count( array, array, 'a' );
  if( n != 0 ) FAIL;

  n = std::count_if( array, array + 5, is_odd );
  if( n != 2 ) FAIL;

  n = std::count_if(
        array, array + 5, std::bind1st( std::less< int >( ), 'b' ) );
  if( n != 2 ) FAIL;

  return( rc );
}
#endif

bool equal_test( )
{
  bool rc = true;

  int array1[] = { 0,  1,  2,  3,  4 };
  int array2[] = { 0,  1,  2,  3,  4 };
  int array3[] = { 0,  1,  3,  3,  4 };
  int array4[] = { 0, -1, -2, -3, -4 };

  if( !std::equal( array1, array1 + 5, array2 ) ) FAIL;
  if(  std::equal( array1, array1 + 3, array3 ) ) FAIL;
  if( !std::equal( array1, array1, array2 ) ) FAIL;
  if( !std::equal(
         array1, array1 + 5, array4, std::greater_equal< int >( ) ) ) FAIL;

  return( rc );
}


bool copy_test( )
{
  bool rc = true;

  int array1[] = { 0, 1, 2, 3, 4 };
  int array2[] = { 0, 0, 0, 0, 0 };
  int array3[] = { 0, 0, 0, 0, 0 };
  int array4[] = { 1, 2, 3, 4, 4 };

  std::copy( array1, array1 + 5, array2 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  int *p = std::copy( array1, array1 + 3, array3 );
  std::copy( array1 + 3, array1 + 5, p );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::copy( array1 + 1, array1 + 5, array1 );
  if( std::memcmp( array1, array4, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array5[] = { 0, 1, 2, 3, 4 };
  int array6[] = { 0, 0, 1, 2, 3 };

  std::copy_backward( array5, array5 + 4, array5 + 5 );
  if( std::memcmp( array5, array6, 5 * sizeof( int ) ) != 0 ) FAIL;

  return( rc );
}


bool swap_test( )
{
  bool rc = true;

  int array1[] = { 0, 1, 2, 3, 4 };
  int array2[] = { 4, 3, 2, 1, 0 };

  int result_array1[] = { 4, 3, 2, 1, 0 };
  int result_array2[] = { 0, 1, 2, 3, 4 };

  std::swap_ranges( array1, array1 + 5, array2 );
  if( std::memcmp( array1, result_array1, 5 * sizeof( int ) ) != 0 ) FAIL;
  if( std::memcmp( array2, result_array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::string sarray1[] = { "abc", "xyz", "123" };
  std::string sarray2[] = { "123", "xyz", "abc" };

  std::swap_ranges( sarray1, sarray1 + 3, sarray2 );
  if( sarray1[0] != "123" || sarray1[1] != "xyz" || sarray1[2] != "abc" ) FAIL;
  if( sarray2[0] != "abc" || sarray2[1] != "xyz" || sarray2[2] != "123" ) FAIL;

  return( rc );
}


int square( int num )
{
  return( num * num );
}

std::string join_strings( const std::string &left,
                                 const std::string &right )
{
  return( left + right );
}

bool transform_test( )
{
  bool rc = true;

  int array1[]  = { 0, 1, 2, 3,  4 };
  int array2[]  = { 0, 0, 0, 0,  0 };
  int result1[] = { 0, 1, 4, 9, 16 };
  int result2[] = { 0, 2, 4, 6,  8 };

  std::transform( array1, array1 + 5, array2, square );
  if( std::memcmp( array2, result1, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::transform(
    array1, array1 + 5, array2, std::bind1st( std::multiplies< int >( ), 2 ) );
  if( std::memcmp( array2, result2, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::string sarray1[] = { "hello", "fizzle" };
  std::string sarray2[] = { "world", "sticks" };
  std::string sarray3[2];
  std::string sresult[] = { "helloworld", "fizzlesticks" };

  std::transform( sarray1, sarray1 + 2, sarray2, sarray3, join_strings );
  if( sarray3[0] != sresult[0] || sarray3[1] != sresult[1] ) FAIL;

  return( rc );
}


bool replace_test( )
{
  bool rc = true;

  int array1[] = { 0, 1,  2, 3, 4 };
  int array2[] = { 0, 1, -1, 3, 4 };

  std::replace( array1, array1 + 5, 2, -1 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array3[] = {  0, 1,  2, 3,  4 };
  int array4[] = { -1, 1, -1, 3, -1 };

  std::replace_if(
    array3, array3 + 5, std::not1( std::ptr_fun( is_odd ) ), -1 );
  if( std::memcmp( array3, array4, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array5[] = { 0, 1, 2,  3, 4 };
  int array6[5];
  int array7[] = { 0, 1, 2, -1, 4 };

  std::replace_copy( array5, array5 + 5, array6, 3, -1 );
  if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array8[] = {  0,  1,  2, 3, 4 };
  int array9[5];
  int arrayA[] = { -1, -1, -1, 3, 4 };

  std::replace_copy_if(
    array8, array8 + 5, array9, std::bind2nd( std::less_equal< int >( ), 2 ), -1 );
  if( std::memcmp( array9, arrayA, 5 * sizeof( int ) ) != 0 ) FAIL;

  return( rc );
}


bool fill_test( )
{
  bool rc = true;

  int array1[5];
  int array2[] = { -2, -2, -2, -2, -2 };
  int array3[] = {  1,  1,  1, -2, -2 };

  std::fill( array1, array1 + 5, -2 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::fill_n( array1, 3, 1 );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

  return( rc );
}


int powers_of_two( )
{
  static int current = 1;

  int temp = current;
  current *= 2;
  return( temp );
}


bool generate_test( )
{
  bool rc = true;

  int array1[5];
  int array2[] = {  1,  2, 4, 8, 16 };
  int array3[] = { 32, 64, 4, 8, 16 };

  std::generate( array1, array1 + 5, powers_of_two );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  std::generate_n( array1, 2, powers_of_two );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

  return( rc );
}

bool remove_test( )
{
  int* o;
  int ref[] = { 1,2,3,4,3,2,1 };
  int a1[] = { 1,2,3,4,3,2,1 };
  //---- remove ----
  //don't remove anything
  o = std::remove( a1, a1 + 7, 0 );
  if( std::memcmp( a1, ref, sizeof( ref ) ) || (o != a1 + 7) ) FAIL
  //remove 2s
  o = std::remove( a1, a1 + 7, 2 );
  int ref2[] = { 1,3,4,3,1 };
  if( std::memcmp( a1, ref2, sizeof( ref2 ) ) || (o != a1 + 5) ) FAIL
  //remove 1s
  o = std::remove( a1, a1 + 5, 1 );
  int ref3[] = { 3,4,3 };
  if( std::memcmp( a1, ref3, sizeof( ref3 ) ) || (o != a1 + 3) ) FAIL
  //remove 4
  o = std::remove( a1, a1 + 3, 4 );
  int ref4[] = { 3,3 };
  if( std::memcmp( a1, ref4, sizeof( ref4 ) ) || (o != a1 + 2) ) FAIL
  //remove 3
  o = std::remove( a1, a1 + 2, 3 );
  if( o != a1 ) FAIL
  
  //---- remove_copy_if ----
  int out[9];
  int a2[] = { 1,2,3,4,5,4,3,2,1 };
  int ref5[] = { 2,4,4,2 };
  o = std::remove_copy_if( a2, a2 + 9, out, is_odd ); //removes if !false
  if( std::memcmp( out, ref5, sizeof( ref5 ) ) || (o != out + 4 ) ) FAIL
  
  //---- remove_if ----
  //just a quick test because it is really the same as remove_copy_if
  o = std::remove_if( a2, a2 + 9, is_odd ); //removes if !false
  if( std::memcmp( out, ref5, sizeof( ref5 ) ) || (o != a2 + 4 ) ) FAIL
  
  //---- remove_copy ----
  //just a quick test because it is really the same as remove
  int a3[] = { 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, 6, 9, 7, 9 };
  int ref6[] = { 1, 2, 3, 4, 5, 6, 7 };
  o = std::remove_copy( a3, a3+15, out, 9 );
  if( std::memcmp( out, ref6, sizeof( ref6 ) ) || (o != out + 7 ) ) FAIL
  
  return( true );
}



bool reverse_test( )
{
  bool rc = true;

  int array1[] = { 0, 1, 2, 3, 4 };
  int array2[] = { 4, 3, 2, 1, 0 };

  std::reverse( array1, array1 + 5 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array3[] = { 0, 1, 2, 3 };
  int array4[] = { 3, 2, 1, 0 };

  std::reverse( array3, array3 + 4 );
  if( std::memcmp( array3, array4, 4 * sizeof( int ) ) != 0 ) FAIL;

  int array5[] = { 0, 1, 2, 3, 4 };
  int array6[5];
  int array7[] = { 4, 3, 2, 1, 0 };

  std::reverse_copy( array5, array5 + 5, array6 );
  if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) FAIL;

  int array8[] = { 0, 1, 2, 3 };
  int array9[4];
  int arrayA[] = { 3, 2, 1, 0 };

  std::reverse_copy( array8, array8 + 4, array9 );
  if( std::memcmp( array9, arrayA, 4 * sizeof( int ) ) != 0 ) FAIL;

  return( rc );
}

// This function verifies that each value from 0..size is in the given
// array once and only once. This function is used to check the sanity
// of the output of random_shuffle.
//
static bool check_values( int *array, int size )
{
  bool all_good = true;

  // Let's use good old malloc here... just to see if it works for us.
  int *counters = static_cast< int * >( std::malloc( size*sizeof( int ) ) );
  for( int i = 0; i < size; ++i ) counters[i] = 0;
  for( int i = 0; i < size; ++i ) counters[ array[i] ]++;
  for( int i = 0; i < size; ++i )
    if( counters[i] != 1 ) all_good = false;

  std::free( counters );
  return( all_good );
}

// This function is a simple random number generator given to the three
// argument form of random_shuffle. It is intentionally not very random
// so that we can force certain effects.
//
static int simple_generator( int )
{
  return 0;
}

bool random_shuffle_test( )
{
  bool rc = true;

  int array[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  // Let's just try a few shuffles... er... at random.

  std::srand( 0 );
  std::random_shuffle( array, array + 10 );
  if( !check_values( array, 10 ) ) FAIL;

  std::srand( 10 );
  std::random_shuffle( array, array + 10 );
  if( !check_values( array, 10 ) ) FAIL;

  std::srand( 100 );
  std::random_shuffle( array, array + 10 );
  if( !check_values( array, 10 ) ) FAIL;

  std::random_shuffle( array, array + 10, simple_generator );
  if( !check_values( array, 10 ) ) FAIL; 

  return( rc );
}


int main( )
{
  int rc = 0;
  int original_count = heap_count( );

  try {
    if( !for_each_test( )       || !heap_ok( "t01" ) ) rc = 1;
    if( !find_test( )           || !heap_ok( "t02" ) ) rc = 1;
    if( !find_end_test( )       || !heap_ok( "t03" ) ) rc = 1;
    if( !find_first_of_test( )  || !heap_ok( "t04" ) ) rc = 1;
    // if( !count_test( )       || !heap_ok( "t05" ) ) rc = 1;
    if( !equal_test( )          || !heap_ok( "t06" ) ) rc = 1;
    if( !copy_test( )           || !heap_ok( "t07" ) ) rc = 1;
    if( !swap_test( )           || !heap_ok( "t08" ) ) rc = 1;
    if( !transform_test( )      || !heap_ok( "t09" ) ) rc = 1;
    if( !replace_test( )        || !heap_ok( "t10" ) ) rc = 1;
    if( !fill_test( )           || !heap_ok( "t11" ) ) rc = 1;
    if( !generate_test( )       || !heap_ok( "t12" ) ) rc = 1;
    if( !remove_test( )         || !heap_ok( "t13" ) ) rc = 1;
    if( !reverse_test( )        || !heap_ok( "t14" ) ) rc = 1;
    if( !random_shuffle_test( ) || !heap_ok( "t15" ) ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  if( heap_count( ) != original_count ) {
    std::cout << "Possible memory leak!\n";
    rc = 1;
  }
  return( rc );
}
