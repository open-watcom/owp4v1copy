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
* Description: This file contains the functional tests for std::vector.
*
****************************************************************************/

#include <algorithm>  // I use std::reverse in pushback_test.
#include <iostream>
#include <string>
#include <vector>

bool construction_test( )
{
  bool rc = true;
  std::vector< int > v1;
  std::vector< int > v2(10);
  std::vector< int > v3(v1);
  std::vector< int > v4(v2);
  std::vector< int > v5(10, 1);

  if( v1.size( ) != 0 || !v1.empty( ) ) {
    std::cout << "construct FAIL 0001\n"; rc = false;
  }
  if( v2.size( ) != 10 || v2.empty( ) ) {
    std::cout << "construct FAIL 0002\n"; rc = false;
  }
  if( v3.size( ) != 0 || !v3.empty( ) ) {
    std::cout << "construct FAIL 0003\n"; rc = false;
  }
  if( v4.size( ) != 10 || v4.empty( ) ) {
    std::cout << "construct FAIL 0004\n"; rc = false;
  }
  if( v5.size( ) != 10 || v5.empty( ) ) {
    std::cout << "construct FAIL 0005\n"; rc = false;
  }

  // Use checked access so that we get an exception if something's wrong.
  for( std::vector< int >::size_type i = 0; i < v5.size( ); ++i ) {
    if( v5.at( i ) != 1 ) {
      std::cout << "construct FAIL 0006\n"; rc = false;
    }
  }

  return( rc );
}

bool access_test( )
{
  using std::out_of_range;

  bool rc = true;
  std::vector< int > v(10);

  for (int i = 0; i < 10; ++i ) {
    v[i] = i;
  }
  for (int i = 0; i < 10; ++i ) {
    if( v[i] != i ) {
      std::cout << "access FAIL 0001\n";
    }
  }

  for (int i = 0; i < 10; ++i ) {
    v.at( i ) = i;
  }
  for (int i = 0; i < 10; ++i ) {
    if( v.at( i ) != i ) {
      std::cout << "access FAIL 0002\n";
    }
  }

  try {
    v.at( 10 ) = 0;
    std::cout << "access FAIL 0003\n";
  }
  catch( out_of_range ) {
    // Okay
  }

  return( rc );
}

bool assign_test( )
{
  bool rc = true;
  std::vector< int > v1(10, 1);
  std::vector< int > v2( 5, 2);
  std::vector< int > v3(15, 3);

  v1 = v2;
  if( v1.size( ) != 5 ) {
    std::cout << "assign FAIL 0001\n"; rc = false;
  }
  for( std::vector< int >::size_type i = 0; i < v1.size( ); ++i ) {
    if( v1.at( i ) != 2 ) {
      std::cout << "assign FAIL 0002\n"; rc = false;
    }
  }

  v1 = v3;
  if( v1.size( ) != 15 ) {
    std::cout << "assign FAIL 0003\n"; rc = false;
  }
  for( std::vector< int >::size_type i = 0; i < v1.size( ); ++i ) {
    if( v1.at( i ) != 3 ) {
      std::cout << "assign FAIL 0004\n"; rc = false;
    }
  }

  v1.assign( 10, 4 );
  if( v1.size( ) != 10 ) {
    std::cout << "assign FAIL 0005\n"; rc = false;
  }
  for( std::vector< int >::size_type i = 0; i < v1.size( ); ++i ) {
    if( v1.at( i ) != 4 ) {
      std::cout << "assign FAIL 0006\n"; rc = false;
    }
  }

  v1.assign( 20, 5 );
  if( v1.size( ) != 20 ) {
    std::cout << "assign FAIL 0007\n"; rc = false;
  }
  for( std::vector< int >::size_type i = 0; i < v1.size( ); ++i ) {
    if( v1.at( i ) != 5 ) {
      std::cout << "assign FAIL 0008\n"; rc = false;
    }
  }


  return( rc );
}

template< class Type >
bool pushback_test(Type *check, std::size_t check_size )
{
  using std::vector;  // Make sure this works.

  bool rc = true;
  vector< Type > vec;

  for( vector< Type >::size_type i = 0; i < check_size; ++i ) {
    vec.push_back( check[i] );
    if( vec.back( ) != check[i] ) {
      std::cout << "pushback FAIL 0001\n"; rc = false;
    }
    if( vec.front( ) != check[0] ) {
      std::cout << "pushback FAIL 0002\n"; rc = false;
    }
  }
  if( vec.size( ) != check_size ) {
    std::cout << "pushback FAIL 0003\n"; rc = false;
  }

  std::reverse( check, check + check_size );
  for( vector< Type >::size_type i = 0; i < check_size; ++i ) {
    if( vec.back( ) != check[i] ) {
      std::cout << "pushback FAIL 0004\n"; rc = false;
    }
    if( vec.front( ) != check[check_size - 1] ) {
      std::cout << "pushback FAIL 0005\n"; rc = false;
    }
    vec.pop_back( );
  }
  if( vec.size( ) != 0 ) {
    std::cout << "pushback FAIL 0006\n"; rc = false;
  }

  return( rc );
}

bool iterator_test( )
{
  bool rc = true;
  std::vector< int > vec(10);
  int counter;

  for(std::vector< int >::size_type i = 0; i < 10; ++i ) {
    vec[i] = i;
  }

  std::vector< int >::iterator p;

  counter = 0;
  for( p = vec.begin( ); p != vec.end( ); ++p ) {
    if( *p != counter ) {
      std::cout << "iterator FAIL 0001\n"; rc = false;
    }
    ++counter;
  }

  // Use p++ (probably a more exhaustive test should be created).
  counter = 0;
  for( p = vec.begin( ); p != vec.end( ); p++ ) {
    if( *p != counter ) {
      std::cout << "iterator FAIL 0002\n"; rc = false;
    }
    ++counter;
  }

  // Also run iterators backwards, do pointer arithmetic, compare, etc.

  return( rc );
}

int main( )
{
  // Probably the entire test suite should be a template so that all
  // tests can be run using both structured and unstructed types. For
  // now this is better than nothing.
  //
  static int int_check[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
  const std::size_t int_check_size =
    sizeof( int_check )/sizeof( int );

  static std::string string_check[] = {
    "abc", "def", "ghi", "jkl", "mno", "pqr", "stu", "vwx", "yz" };
  const std::size_t string_check_size =
    sizeof( string_check )/sizeof( std::string );

  int rc = 0;
  try {
    if( !construction_test( )       ) rc = 1;
    if( !access_test( )             ) rc = 1;
    if( !assign_test( )             ) rc = 1;
    if( !pushback_test( int_check, int_check_size ) ) rc = 1;
    if( !pushback_test( string_check, string_check_size ) ) rc = 1;
    if( !iterator_test( )           ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
