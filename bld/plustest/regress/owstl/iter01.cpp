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
* Description:  This file contains the functional tests for the generic
*               iterator support in the library.
*
****************************************************************************/

#include <iostream>
#include <iterator>

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
  if( temp != 'w' ) {
    std::cout << "traits FAIL 0003\n"; return false;
  }
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
  if( xyz::X != 2 || xyz::Y != 1 ) {
    std::cout << "traits FAIL 0001\n"; rc = false;
  }

  exchange( raw_array, raw_array + 3 );
  if( raw_array[0] != 'd' || raw_array[3] != 'a' ) {
    std::cout << "traits FAIL 0002\n"; rc = false;
  }

  if( !read_element( const_raw_array ) ) rc = false;
  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    if( !traits_test( )       ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
