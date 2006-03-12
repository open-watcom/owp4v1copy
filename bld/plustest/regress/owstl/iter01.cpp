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
* Description:  This file contains the functional tests for the generic
*               iterator support in the library.
*
****************************************************************************/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "sanity.cpp"

// Use a namespace to exercise that ability.
namespace xyz {
  int X, Y;

  // Simple structured iterator gives random access to X or Y.
  class RandomToggleIterator :
    public std::iterator< std::random_access_iterator_tag, int, int > {

  public:
    RandomToggleIterator( bool first_flag )
      { if( first_flag ) isX = true; else isX = false; }

    int &operator*( )
      { if( isX ) return X; else return Y; }

  private:
    bool isX;
  };
}

char raw_array[] = { 'a', 'b', 'c', 'd' };
const char const_raw_array[] = { 'w', 'x', 'y', 'z' };

template< class Iterator >
void exchange( Iterator x, Iterator y )
{
  // Check operation of iterator_traits.
  typename std::iterator_traits< Iterator >::value_type temp;
  temp = *x;
  *x   = *y;
  *y   = temp;
}

template< class Iterator >
bool read_element( Iterator first )
{
  // Used for exercising partial specialization for pointers to const.
  typename std::iterator_traits< Iterator >::value_type temp( *first );
  if( temp != 'w' ) FAIL
  return true;
}

bool traits_test( )
{
  bool rc = true;
  xyz::RandomToggleIterator p1( true );
  xyz::RandomToggleIterator p2( false );

  xyz::X = 1;
  xyz::Y = 2;
  exchange( p1, p2 );
  if( xyz::X != 2 || xyz::Y != 1 ) FAIL

  exchange( raw_array, raw_array + 3 );
  if( raw_array[0] != 'd' || raw_array[3] != 'a' ) FAIL

  if( !read_element( const_raw_array ) ) rc = false;
  return( rc );
}

bool advance_test( )
{
  bool rc = true;

  char *p = raw_array;
  std::advance( p, 2 );
  if( *p != 'c' ) FAIL

  return( rc );
}

#ifdef NEVER
bool distance_test( )
{
  bool rc = true;

  std::ptrdiff_t d =  std::distance( raw_array, raw_array + 2 );
  if( d != 2 ) FAIL

  return( rc );
}
#endif

char reverse_array[] = { 'a', 'b', 'c', 'd' };

bool reverse_test( )
{
  bool rc = true;

  std::reverse_iterator< char * > p1( reverse_array + 2 );
  std::reverse_iterator< char * > p2( reverse_array );
  if( *p1 != 'b' ) FAIL

  ++p1;
  if( *p1 != 'a' ) FAIL

  ++p1;
  if( p1 != p2 ) FAIL
  return( rc );
}

bool back_inserter_test( )
{
  bool rc = true;
  char raw[] = { 'a', 'b', 'c', 'd' };

  std::string s1( "xyz" );
  std::copy( raw, raw + 4, std::back_inserter( s1 ) );
  if( s1 != "xyzabcd" ) FAIL

  return( rc );
}

bool front_inserter_test( )
{
  bool rc = true;

  // Finish me once we have a container with pop_front().
  return( rc );
}

bool inserter_test( )
{
  bool rc = true;
  char raw[] = { 'a', 'b', 'c', 'd' };

  std::string s1( "xyz" );
  std::copy( raw, raw + 4, std::inserter( s1, s1.begin( ) + 1 ) );
  if( s1 != "xabcdyz" ) FAIL

  return( rc );
}

int main( )
{
  int rc = 0;
  int original_count = heap_count( );

  try {
    if( !traits_test( )         || !heap_ok( "t01" )  ) rc = 1;
    if( !advance_test( )        || !heap_ok( "t02" )  ) rc = 1;
    // if( !distance_test( )       || !heap_ok( "t03" )  ) rc = 1;
    if( !reverse_test( )        || !heap_ok( "t04" )  ) rc = 1;
    if( !back_inserter_test( )  || !heap_ok( "t05" )  ) rc = 1;
    if( !front_inserter_test( ) || !heap_ok( "t06" )  ) rc = 1;
    if( !inserter_test( )       || !heap_ok( "t07" )  ) rc = 1;
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
