/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
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

bool for_each_test( )
{
  bool rc = true;

  char str[] = { 'h', 'e', 'l', 'l', 'o', '\0' };

  std::for_each( str, str, advance_char );
  if( std::strcmp( str, "hello" ) != 0 ) {
    std::cout << "for_each FAIL 0001\n"; rc = false;
  }

  std::for_each( str, str + 5, advance_char );
  if( std::strcmp( str, "ifmmp" ) != 0 ) {
    std::cout << "for_each FAIL 0002\n"; rc = false;
  }

  return( rc );
}

bool find_test( )
{
  bool rc = true;

  int  array[] = { 0, 1, 2, 3, 4 };
  int *p;

  p = std::find( array, array + 5, 2 );
  if( p == array + 5 || *p != 2 ) {
    std::cout << "find FAIL 0001\n"; rc = false;
  }
  p = std::find( array, array + 5, 5 );
  if( p != array + 5 ) {
    std::cout << "find FAIL 0002\n"; rc = false;
  }
  p = std::find( array, array, 1 );
  if( p != array ) {
    std::cout << "find FAIL 0003\n"; rc = false;
  }
  p = std::find_if(
        array, array + 5, std::bind2nd( std::greater< int >( ), 3 ) );
  if( p == array + 5 || *p != 4 ) {
    std::cout << "find FAIL 0004\n"; rc = false;
  }
  p = std::find_if( array, array + 5, is_odd );
  if( p == array + 5 || *p != 1 ) {
    std::cout << "find FAIL 0005\n"; rc = false;
  }

  return( rc );
}


#ifdef _NEVER
bool count_test( )
{
  bool rc = true;

  char array[] = { 'a', 'b', 'b', 'd', 'e' };
  int n = -1;

  n = std::count( array, array + 5, 'b' );
  if( n != 2 ) {
    std::cout << "count FAIL 0001\n"; rc = false;
  }
  n = std::count( array, array + 5, 'c' );
  if( n != 0) {
    std::cout << "count FAIL 0002\n"; rc = false;
  }
  n = std::count( array, array, 'a' );
  if( n != 0 ) {
    std::cout << "count FAIL 0003\n"; rc = false;
  }
  n = std::count_if( array, array + 5, is_odd );
  if( n != 3 ) {
    std::cout << "count FAIL 0004\n"; rc = false;
  }
  n = std::count_if(
        array, array + 5, std::bind1st( std::less< int >( ), 'b' ) );
  if( n != 2 ) {
    std::cout << "count FAIL 0005\n"; rc = false;
  }

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

  if( !std::equal( array1, array1 + 5, array2 ) ) {
    std::cout << "equal FAIL 0001\n"; rc = false;
  }
  if(  std::equal( array1, array1 + 3, array3 ) ) {
    std::cout << "equal FAIL 0002\n"; rc = false;
  }
  if( !std::equal( array1, array1, array2 ) ) {
    std::cout << "equal FAIL 0003\n"; rc = false;
  }
  if( !std::equal(
         array1, array1 + 5, array4, std::greater_equal< int >( ) ) ) {
    std::cout << "equal FAIL 0004\n"; rc = false;
  }

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
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "copy FAIL 0001\n"; rc = false;
  }
  int *p = std::copy( array1, array1 + 3, array3 );
  std::copy( array1 + 3, array1 + 5, p );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "copy FAIL 0002\n"; rc = false;
  }
  std::copy( array1 + 1, array1 + 5, array1 );
  if( std::memcmp( array1, array4, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "copy FAIL 0003\n"; rc = false;
  }

  int array5[] = { 0, 1, 2, 3, 4 };
  int array6[] = { 0, 0, 1, 2, 3 };

  std::copy_backward( array5, array5 + 4, array5 + 5 );
  if( std::memcmp( array5, array6, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "copy FAIL 0004\n"; rc = false;
  }

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
  if( std::memcmp( array1, result_array1, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "swap FAIL 0001\n"; rc = false;
  }
  if( std::memcmp( array2, result_array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "swap FAIL 0002\n"; rc = false;
  }

  std::string sarray1[] = { "abc", "xyz", "123" };
  std::string sarray2[] = { "123", "xyz", "abc" };

  std::swap_ranges( sarray1, sarray1 + 3, sarray2 );
  if( sarray1[0] != "123" || sarray1[1] != "xyz" || sarray1[2] != "abc" ) {
    std::cout << "swap FAIL 0003\n"; rc = false;
  }
  if( sarray2[0] != "abc" || sarray2[1] != "xyz" || sarray2[2] != "123" ) {
    std::cout << "swap FAIL 0004\n"; rc = false;
  }

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
  if( std::memcmp( array2, result1, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "transform FAIL 0001\n"; rc = false;
  }
  std::transform(
    array1, array1 + 5, array2, std::bind1st( std::multiplies< int >( ), 2 ) );
  if( std::memcmp( array2, result2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "transform FAIL 0002\n"; rc = false;
  }

  std::string sarray1[] = { "hello", "fizzle" };
  std::string sarray2[] = { "world", "sticks" };
  std::string sarray3[2];
  std::string sresult[] = { "helloworld", "fizzlesticks" };

  std::transform( sarray1, sarray1 + 2, sarray2, sarray3, join_strings );
  if( sarray3[0] != sresult[0] || sarray3[1] != sresult[1] ) {
    std::cout << "transform FAIL 0003\n"; rc = false;
  }

  return( rc );
}


bool replace_test( )
{
  bool rc = true;

  int array1[] = { 0, 1,  2, 3, 4 };
  int array2[] = { 0, 1, -1, 3, 4 };

  std::replace( array1, array1 + 5, 2, -1 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "replace FAIL 0001\n"; rc = false;
  }

  int array3[] = {  0, 1,  2, 3,  4 };
  int array4[] = { -1, 1, -1, 3, -1 };

  std::replace_if(
    array3, array3 + 5, std::not1( std::ptr_fun( is_odd ) ), -1 );
  if( std::memcmp( array3, array4, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "replace FAIL 0002\n"; rc = false;
  }

  int array5[] = { 0, 1, 2,  3, 4 };
  int array6[5];
  int array7[] = { 0, 1, 2, -1, 4 };

  std::replace_copy( array5, array5 + 5, array6, 3, -1 );
  if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "replace FAIL 0003\n"; rc = false;
  }

  int array8[] = {  0,  1,  2, 3, 4 };
  int array9[5];
  int arrayA[] = { -1, -1, -1, 3, 4 };

  std::replace_copy_if(
    array8, array8 + 5, array9, std::bind2nd( std::less_equal< int >( ), 2 ), -1 );
  if( std::memcmp( array9, arrayA, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "replace FAIL 0004\n"; rc = false;
  }

  return( rc );
}


bool fill_test( )
{
  bool rc = true;

  int array1[5];
  int array2[] = { -2, -2, -2, -2, -2 };
  int array3[] = {  1,  1,  1, -2, -2 };

  std::fill( array1, array1 + 5, -2 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "fill FAIL 0001\n"; rc = false;
  }

  std::fill_n( array1, 3, 1 );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "fill FAIL 0002\n"; rc = false;
  }

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
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "generate FAIL 0001\n"; rc = false;
  }

  std::generate_n( array1, 2, powers_of_two );
  if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "generate FAIL 0002\n"; rc = false;
  }

  return( rc );
}


bool reverse_test( )
{
  bool rc = true;

  int array1[] = { 0, 1, 2, 3, 4 };
  int array2[] = { 4, 3, 2, 1, 0 };

  std::reverse( array1, array1 + 5 );
  if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "reverse FAIL 0001\n"; rc = false;
  }

  int array3[] = { 0, 1, 2, 3 };
  int array4[] = { 3, 2, 1, 0 };

  std::reverse( array3, array3 + 4 );
  if( std::memcmp( array3, array4, 4 * sizeof( int ) ) != 0 ) {
    std::cout << "reverse FAIL 0002\n"; rc = false;
  }

  int array5[] = { 0, 1, 2, 3, 4 };
  int array6[5];
  int array7[] = { 4, 3, 2, 1, 0 };

  std::reverse_copy( array5, array5 + 5, array6 );
  if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) {
    std::cout << "reverse FAIL 0003\n"; rc = false;
  }

  int array8[] = { 0, 1, 2, 3 };
  int array9[4];
  int arrayA[] = { 3, 2, 1, 0 };

  std::reverse_copy( array8, array8 + 4, array9 );
  if( std::memcmp( array9, arrayA, 4 * sizeof( int ) ) != 0 ) {
    std::cout << "reverse FAIL 0004\n"; rc = false;
  }

  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    if( !for_each_test( )   ) rc = 1;
    if( !find_test( )       ) rc = 1;
    // The count test fails because of a compiler bug.
    // if( !count_test( )      ) rc = 1;
    if( !equal_test( )      ) rc = 1;
    if( !copy_test( )       ) rc = 1;
    if( !swap_test( )       ) rc = 1;
    if( !transform_test( )  ) rc = 1;
    if( !replace_test( )    ) rc = 1;
    if( !fill_test( )       ) rc = 1;
    if( !generate_test( )   ) rc = 1;
    if( !reverse_test( )    ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
