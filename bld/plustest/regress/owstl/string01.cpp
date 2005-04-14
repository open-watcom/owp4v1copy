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
* Description:  This file contains functional tests for std::string.
*               Ideally it should carefully exercise every method.
*               Although currently most methods are checked for basic
*               operation, many more tests could (and should) be added.
*
****************************************************************************/

#include <iostream>
#include <string>

#include "sanity.cpp"

bool construct_test( )
{
  bool rc = true;
  std::string s1;
  std::string s2( "Hello, World" );
  std::string s3( "Hello, World", 3 );
  std::string s4( s2 );
  std::string s5( s2, 7, 2 );
  std::string s6( s2, 7, 1024 );
  std::string s7( 16, 'x' );

  if( s1.size( ) != 0 || s1 != "" || INSANE( s1 ) ) {
    std::cout << "construct FAIL 0001\n"; rc = false;
  }
  if( s2.size( ) != 12  || s2 != "Hello, World" || INSANE( s2 ) ) {
    std::cout << "construct FAIL 0002\n"; rc = false;
  }
  if( s3.size( ) != 3 || s3 != "Hel" || INSANE( s3 ) ) {
    std::cout << "construct FAIL 0003\n"; rc = false;
  }
  if( s4.size( ) != 12 || s4 != "Hello, World" || INSANE( s4 ) ) {
    std::cout << "construct FAIL 0004\n"; rc = false;
  }
  if( s5.size( ) != 2 || s5 != "Wo" || INSANE( s5 ) ) {
    std::cout << "construct FAIL 0005\n"; rc = false;
  }
  if( s6.size( ) != 5 || s6 != "World" || INSANE( s6 ) ) {
    std::cout << "construct FAIL 0006\n"; rc = false;
  }
  if( s7.size( ) != 16 || s7 != "xxxxxxxxxxxxxxxx" || INSANE( s7 ) ) {
    std::cout << "construct FAIL 0007\n"; rc = false;
  }
  return( rc );
}

bool assign_test( )
{
  bool rc = true;
  std::string s1( 15, 'x' );
  std::string s2( "Hello" );
  std::string s3( "This string is longer than 15 characters" );

  s1 = s2;
  if( s1.size( )     != 5          ||
      s1.capacity( ) <  s1.size( ) ||
      s1             != "Hello" ) {
    std::cout << "assign FAIL 0001\n"; rc = false;
  }

  s1 = s3;
  if( s1.size( )     != 40         ||
      s1.capacity( ) < s1.size( )  ||
      s1             != "This string is longer than 15 characters" ) {
    std::cout << "assign FAIL 0002\n"; rc = false;
  }

  std::string s4( 'x', 15 );
  s4 = "This string is longer than 15 characters";
  if( s4.size( )     != 40         ||
      s4.capacity( ) <  s4.size( ) ||
      s4             != "This string is longer than 15 characters" ) {
    std::cout << "assign FAIL 0003\n"; rc = false;
  }

  std::string s5( 'x', 15 );
  s5 = 'y';
  if( s5.size( )     != 1          ||
      s5.capacity( ) <  s5.size( ) ||
      s5             != "y" ) {
    std::cout << "assign FAIL 0004\n"; rc = false;
  }

  // Need to add tests for the assign() methods as well.

  return( rc );
}

bool access_test( )
{
  bool rc = true;
  std::string s1( "Hello, World" );

  if( s1[0] != 'H' ) {
    std::cout << "access FAIL 0001\n"; rc = false;
  }
  if( s1[11] != 'd' ) {
    std::cout << "access FAIL 0002\n"; rc = false;
  }

  s1[0] = 'x';
  if( s1 != "xello, World" ) {
    std::cout << "access FAIL 0003\n"; rc = false;
  }

  return( rc );
}

bool relational_test( )
{
  bool rc = true;
  std::string s1( "abcd" );
  std::string s2( "abcd" );
  std::string s3( "abcc" );
  std::string s4( "abce" );
  std::string s5( "abc" );
  std::string s6( "abcde" );

  // Operator==
  if( !( s1 == s2 ) ) {
    std::cout << "relational FAIL 0001\n"; rc = false;
  }
  if(  ( s1 == s5 ) ) {
    std::cout << "relational FAIL 0002\n"; rc = false;
  }

  // Operator !=
  if(  ( s1 != s2 ) ) {
    std::cout << "relational FAIL 0003\n"; rc = false;
  }
  if( !( s5 != s1 ) ) {
    std::cout << "relational FAIL 0004\n"; rc = false;
  }

  // Operator<
  if(  ( s1 < s2 ) ) {
    std::cout << "relational FAIL 0005\n"; rc = false;
  }
  if( !( s3 < s1 ) ) {
    std::cout << "relational FAIL 0006\n"; rc = false;
  }
  if(  ( s4 < s1 ) ) {
    std::cout << "relational FAIL 0007\n"; rc = false;
  }
  if( !( s5 < s1 ) ) {
    std::cout << "relational FAIL 0008\n"; rc = false;
  }
  if(  ( s4 < s6 ) ) {
    std::cout << "relational FAIL 0009\n"; rc = false;
  }

  // Operator>
  if( !( s4 > s3 ) ) {
    std::cout << "relational FAIL 0010\n"; rc = false;
  }
  if(  ( s1 > s6 ) ) {
    std::cout << "relational FAIL 0011\n"; rc = false;
  }
  
  // Operator <=
  if( !( s1 <= s2 && s3 <= s2 ) ) {
    std::cout << "relational FAIL 0012\n"; rc = false;
  }
  if(  ( s2 <= s3 || s6 <= s5 ) ) {
    std::cout << "relational FAIL 0013\n"; rc = false;
  }

  // Operator>=
  if( !( s2 >= s1 && s6 >= s5 ) ) {
    std::cout << "relational FAIL 0014\n"; rc = false;
  }
  if(  ( s3 >= s4 || s5 >= s4 ) ) {
    std::cout << "relatioanl FAIL 0015\n"; rc = false;
  }

  return( rc );
}

bool capacity_test( )
{
  bool rc = true;

  std::string s1;
  std::string s2("Hello, World!");

  if( s1.capacity( ) == 0) {
    std::cout << "capacity FAIL 0001\n"; rc = false;
  }
  if( s2.size( ) > s2.capacity( ) ) {
    std::cout << "capacity FAIL 0002\n"; rc = false;
  }
  if( s1.empty( ) == false ) {
    std::cout << "capacity FAIL 0003\n"; rc = false;
  }
  if( s2.empty( ) == true ) {
    std::cout << "capacity FAIL 0004\n"; rc = false;
  }

  s2.clear( );
  if( s2.empty( ) == false ) {
    std::cout << "capacity FAIL 0005\n"; rc = false;
  }

  std::string s3("Hello");
  s3.resize( 2 );
  if( s3.size( ) != 2 || s3 != "He" ) {
    std::cout << "capacity FAIL 0006\n"; rc = false;
  }
  s3.resize( 5, 'x' );
  if( s3.size( ) != 5 || s3 != "Hexxx" ) {
    std::cout << "capacity FAIL 0007\n"; rc = false;
  }
  s3.resize( 40, 'y' );
  if( s3.size( ) != 40 ||
      s3 != "Hexxxyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" ) {
    std::cout << "capacity FAIL 0008\n"; rc = false;
  }

  std::string s4("Hello");
  std::string::size_type new_s4capacity = ( 7 * s4.capacity( ) ) / 2;
  s4.reserve( new_s4capacity );
  if( s4.capacity( ) < new_s4capacity ||
      s4.size( ) != 5 ||
      s4 != "Hello" ) {
    std::cout << "capacity FAIL 0009\n"; rc = false;
  }
  return( rc );
}

// This function should probably be some kind of template so it can be
// readily used to check iterators from any kind of sequence container.
//
bool iterator_test( )
{
  bool rc = true;
  std::string s1( "Hello, World" );
  std::string::iterator p1;
  std::string::iterator p2;

  p1 = s1.begin( );
  if( *p1 != 'H' ) {
    std::cout << "iterator FAIL 0001\n"; rc = false;
  }
  ++p1;
  if( *p1 != 'e' ) {
    std::cout << "iterator FAIL 0002\n"; rc = false;
  }
  --p1;
  if( *p1 != 'H' ) {
    std::cout << "iterator FAIL 0003\n"; rc = false;
  }
  p1++;
  if( *p1 != 'e' ) {
    std::cout << "iterator FAIL 0004\n"; rc = false;
  }
  p1--;
  if( *p1 != 'H' ) {
    std::cout << "iterator FAIL 0005\n"; rc = false;
  }

  p2 = s1.end( );
  --p2;
  if( *p2 != 'd' ) {
    std::cout << "iterator FAIL 0006\n"; rc = false;
  }
  ++p2;
  if( p2 != s1.end( ) ) {
    std::cout << "iterator FAIL 0007\n"; rc = false;
  }

  if ( p2 < p1 ) {
    std::cout << "iterator FAIL 0008\n"; rc = false;
  }

  const std::string s2( "Hello, World" );
  std::string::const_iterator p3;
  std::string::const_iterator p4;

  p3 = s2.begin( );
  if( *p3 != 'H' ) {
    std::cout << "iterator FAIL 0009\n"; rc = false;
  }
  ++p3;
  if( *p3 != 'e' ) {
    std::cout << "iterator FAIL 0010\n"; rc = false;
  }
  --p3;
  if( *p3 != 'H' ) {
    std::cout << "iterator FAIL 0011\n"; rc = false;
  }
  p3++;
  if( *p3 != 'e' ) {
    std::cout << "iterator FAIL 0012\n"; rc = false;
  }
  p3--;
  if( *p3 != 'H' ) {
    std::cout << "iterator FAIL 0013\n"; rc = false;
  }

  p4 = s2.end( );
  --p4;
  if( *p4 != 'd' ) {
    std::cout << "iterator FAIL 0014\n"; rc = false;
  }
  ++p4;
  if( p4 != s2.end( ) ) {
    std::cout << "iterator FAIL 0015\n"; rc = false;
  }

  if ( p4 < p3 ) {
    std::cout << "iterator FAIL 0016\n"; rc = false;
  }

  return( rc );
}

bool append_test( )
{
  bool rc = true;
  std::string s1( "Hello" );
  std::string s2( "World" );

  s1 += s2;
  if( s1         != "HelloWorld" ||
      s1.size( ) != 10 ) {
    std::cout << "append FAIL 0001\n"; rc = false;
  }
  s1 += "x";
  if( s1         != "HelloWorldx" ||
      s1.size( ) != 11 ) {
    std::cout << "append FAIL 0002\n"; rc = false;
  }
  s1 += 'x';
  if( s1         != "HelloWorldxx" ||
      s1.size( ) != 12 ) {
    std::cout << "append FAIL 0003\n"; rc = false;
  }
  s1.append( s2, 2, 2 );
  if( s1         != "HelloWorldxxrl" ||
      s1.size( ) != 14 ) {
    std::cout << "append FAIL 0004\n"; rc = false;
  }
  s1.append( "fizzle", 4 );
  if( s1         != "HelloWorldxxrlfizz" ||
      s1.size( ) != 18 ) {
    std::cout << "append FAIL 0005\n"; rc = false;
  }
  s1.append( "fizzle" );
  if( s1         != "HelloWorldxxrlfizzfizzle" ||
      s1.size( ) != 24 ) {
    std::cout << "append FAIL 0006\n"; rc = false;
  }
  s1.append( 16, 'x' );
  if( s1         != "HelloWorldxxrlfizzfizzlexxxxxxxxxxxxxxxx" ||
      s1.size( ) != 40 ) {
    std::cout << "append FAIL 0007\n"; rc = false;
  }
  s1.push_back( 'z' );
  if( s1         != "HelloWorldxxrlfizzfizzlexxxxxxxxxxxxxxxxz" ||
      s1.size( ) != 41 ) {
    std::cout << "append FAIL 0008\n"; rc = false;
  }

  return( rc );
}

bool insert_test( )
{
  bool rc = true;

  std::string s0( "INSERTED" );
  std::string s1( "Hello, World!" );
  s1.insert( 2, s0 );
  if( s1         != "HeINSERTEDllo, World!" ||
      s1.size( ) != 21 ) {
    std::cout << "insert FAIL 0001\n"; rc = false;
  }

  std::string s2( "Hello, World!" );
  s2.insert( 0, s0 );
  if( s2         != "INSERTEDHello, World!" ||
      s2.size( ) != 21 ) {
    std::cout << "insert FAIL 0002\n"; rc = false;
  }

  std::string s3( "Hello, World!" );
  s3.insert( 13, s0 );
  if( s3         != "Hello, World!INSERTED" ||
      s3.size( ) != 21 ) {
    std::cout << "insert FAIL 0003\n"; rc = false;
  }

  std::string s4( "Hello, World!" );
  s4.insert( 0, s0, 2, 2 );
  if( s4         != "SEHello, World!" ||
      s4.size( ) != 15 ) {
    std::cout << "insert FAIL 0004\n"; rc = false;
  }

  std::string s5( "Hello, World!" );
  s5.insert( 0, s0, 2, 128 );
  if( s5         != "SERTEDHello, World!" ||
      s5.size( ) != 19 ) {
    std::cout << "insert FAIL 0005\n"; rc = false;
  }

  std::string s6( "Hello, World!" );
  s6.insert( s6.begin( ), 2, 'x' );
  if( s6         != "xxHello, World!" ||
      s6.size( ) != 15 ) {
    std::cout << "insert FAIL 0006\n"; rc = false;
  }

  // Need to test other insert methods.

  return( rc );
}

bool erase_test( )
{
  bool rc = true;

  std::string s1( "Hello, World!" );
  s1.erase( );
  if( s1         != "" ||
      s1.size( ) != 0 ) {
    std::cout << "erase FAIL 0001\n"; rc = false;
  }

  std::string s2( "Hello, World!" );
  s2.erase( 2, 3 );
  if( s2         != "He, World!" ||
      s2.size( ) != 10 ) {
    std::cout << "erase FAIL 0002\n"; rc = false;
  }

  std::string s3( "Hello, World!" );
  s3.erase( 7, 6 );
  if( s3         != "Hello, " ||
      s3.size( ) != 7 ) {
    std::cout << "erase FAIL 0003\n"; rc = false;
  }

  std::string s4( "Hello, World!" );
  s4.erase( s4.begin( ) );
  if( s4         != "ello, World!" ||
      s4.size( ) != 12 ) {
    std::cout << "erase FAIL 0004\n"; rc = false;
  }

  std::string s5( "Hello, World!" );
  s5.erase( s5.begin( ) + 2, s5.begin( ) + 5 );
  if( s5         != "He, World!" ||
      s5.size( ) != 10 ) {
    std::cout << "erase FAIL 0005\n"; rc = false;
  }

  return( rc );
}

bool replace_test( )
{
  bool rc = true;
  const std::string s1( "Insert me!" );

  std::string t1( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t1.replace( 0, std::string::npos, s1, 0, std::string::npos );
  if( t1 != "Insert me!" ) {
    std::cout << "replace FAIL 0001\n"; rc = false;
  }

  std::string t2( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t2.replace( 0, 1, s1, 0, std::string::npos );
  if( t2 != "Insert me!BCDEFGHIJKLMNOPQRSTUVWXYZ" ) {
    std::cout << "replace FAIL 0002\n"; rc = false;
  }

  std::string t3( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t3.replace( 1, 0, s1, 0, 1 );
  if( t3 != "AIBCDEFGHIJKLMNOPQRSTUVWXYZ" ) {
    std::cout << "replace FAIL 0003\n"; rc = false;
  }

  std::string t4( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t4.replace( 26, 0, s1, 0, std::string::npos );
  if( t4 != "ABCDEFGHIJKLMNOPQRSTUVWXYZInsert me!" ) {
    std::cout << "replace FAIL 0004\n"; rc = false;
  }

  std::string t5( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t5.replace( 4, 3, s1, 2, 5 );
  if (t5 != "ABCDsert HIJKLMNOPQRSTUVWXYZ" ) {
    std::cout << "replace FAIL 0005\n"; rc = false;
  }

  return( rc );
}

bool iterator_replace_test( )
{
  bool rc = true;
  const std::string s1( "Insert me!" );

  std::string t1( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t1.replace( t1.begin( ), t1.end( ), s1 );
  if( t1 != "Insert me!" ) {
    std::cout << t1.c_str( ) << "\n";
    std::cout << "iterator_replace FAIL 0001\n"; rc = false;
  }

  std::string t2( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t2.replace( t2.begin( ), t2.begin( ) + 1, s1 );
  if( t2 != "Insert me!BCDEFGHIJKLMNOPQRSTUVWXYZ" ) {
    std::cout << "iterator_replace FAIL 0002\n"; rc = false;
  }

  std::string t3( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t3.replace( t3.end( ), t3.end( ), s1 );
  if( t3 != "ABCDEFGHIJKLMNOPQRSTUVWXYZInsert me!" ) {
    std::cout << "iterator_replace FAIL 0003\n"; rc = false;
  }

  std::string t4( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  t4.replace( t4.begin( ) + 4, t4.begin( ) + 7, s1 );
  if (t4 != "ABCDInsert me!HIJKLMNOPQRSTUVWXYZ" ) {
    std::cout << "iterator_replace FAIL 0004\n"; rc = false;
  }
  return( rc );
}

bool copy_test( )
{
  bool rc = true;

  char buffer[128];
  std::string s1( "Hello, World!" );

  buffer[ s1.copy( buffer, 2, 0 ) ] = '\0';
  if( std::strcmp( buffer, "He" ) != 0 ) {
    std::cout << "copy FAIL 0001\n"; rc = false;
  }

  buffer[ s1.copy( buffer, s1.size( ), 0 ) ] = '\0';
  if( std::strcmp( buffer, "Hello, World!" ) != 0 ) {
    std::cout << "copy FAIL 0002\n"; rc = false;
  }

  buffer[ s1.copy( buffer, 3, 2 ) ] = '\0';
  if( std::strcmp( buffer, "llo" ) != 0 ) {
    std::cout << "copy FAIL 0003\n"; rc = false;
  }

  buffer[ s1.copy( buffer, 10, 7 ) ] = '\0';
  if( std::strcmp( buffer, "World!" ) != 0 ) {
    std::cout << "copy FAIL 0004\n"; rc = false;
  }

  buffer[ s1.copy( buffer, 0, 13 ) ] = '\0';
  if( std::strcmp( buffer, "" ) != 0 ) {
    std::cout << "copy FAIL 0005\n"; rc = false;
  }

  try {
    buffer[ s1.copy( buffer, 1, 14 ) ] = '\0';
    std::cout << "copy FAIL 0006\n"; rc = false;
  }
  catch( std::out_of_range ) {
    // Okay
  }
  return( rc );
}

bool swap_test( )
{
  bool rc = true;

  std::string s1("ABC");
  std::string s2("XYZ");

  s1.swap( s2 );
  if( s1 != "XYZ" || s2 != "ABC" ) {
    std::cout << "swap FAIL 0001\n"; rc = false;
  }

  #ifdef __NEVER
  std::swap( s1, s2 );
  if( s1 != "ABC" || s2 != "XYZ" ) {
    std::cout << "swap FAIL 0002\n"; rc = false;
  }
  #endif

  return( rc );
}

bool cstr_test( )
{
        bool  rc = true;
  const char *p;

  std::string s1( "Hello, World!" );
  p = s1.c_str( );
  if( std::strcmp( p, "Hello, World!" ) != 0 ) {
    std::cout << "cstr FAIL 0001\n"; rc = false;
  }

  std::string s2;
  p = s2.c_str( );
  if ( std::strcmp( p, "" ) != 0 ) {
    std::cout << "cstr FAIL 0002\n"; rc = false;
  }

  return( rc );
}

bool find_test( )
{
  bool rc = true;

  std::string s1( "Hello, World" );
  if( s1.find( "Hello", 0 ) != 0 ) {
    std::cout << "find FAIL 0001\n"; rc = false;
  }
  if( s1.find( "Hello", 1 ) != std::string::npos ) {
    std::cout << "find FAIL 0002\n"; rc = false;
  }
  if( s1.find( "World", 0 ) != 7 ) {
    std::cout << "find FAIL 0003\n"; rc = false;
  }
  if( s1.find( "Hello", 11 ) != std::string::npos ) {
    std::cout << "find FAIL 0004\n"; rc = false;
  }
  if( s1.find( "Hello", 12 ) != std::string::npos ) {
    std::cout << "find FAIL 0005\n"; rc = false;
  }
  if( s1.find( "Hello", 13 ) != std::string::npos ) {
    std::cout << "find FAIL 0006\n"; rc = false;
  }
  if( s1.find( "", 12 ) != 12 ) {
    std::cout << "find FAIL 0007\n"; rc = false;
  }

  return( rc );
}

bool rfind_test( )
{
  bool rc = true;

  std::string s1( "Hello, World" );
  if( s1.rfind( "World" ) != 7 ) {
    std::cout << "rfind FAIL 0001\n"; rc = false;
  }
  if( s1.rfind( "Hello" ) != 0 ) {
    std::cout << "rfind FAIL 0002\n"; rc = false;
  }
  if( s1.rfind( "ell", 7 ) != 1 ) {
    std::cout << "rfind FAIL 0003\n"; rc = false;
  }
  if( s1.rfind( "Fizzle" ) != std::string::npos ) {
    std::cout << "rfind FAIL 0004\n"; rc = false;
  }
  if( s1.rfind( "Hello, World..." ) != std::string::npos ) {
    std::cout << "rfind FAIL 0005\n"; rc = false;
  }
  return( rc );
}

bool find_first_of_test()
{
  bool rc = true;
  std::string s1( "Hello, World!" );
  if( s1.find_first_of( "eoW" ) != 1 ) {
    std::cout << "find_first_of FAIL 0001\n"; rc = false;
  }
  if( s1.find_first_of( "eoW", 1 ) != 1 ) {
    std::cout << "find_first_of FAIL 0002\n"; rc = false;
  }
  if( s1.find_first_of( "eoW", 2 ) != 4 ) {
    std::cout << "find_first_of FAIL 0003\n"; rc = false;
  }
  if( s1.find_first_of( "!" ) != 12 ) {
    std::cout << "find_first_of FAIL 0004\n"; rc = false;
  }
  if( s1.find_first_of( "!", 13 ) != std::string::npos ) {
    std::cout << "find_first_of FAIL 0005\n"; rc = false;
  }
  if( s1.find_first_of( "z#+" ) != std::string::npos ) {
    std::cout << "find_first_of FAIL 0006\n"; rc = false;
  }

  return( rc );
}

bool find_last_of_test()
{
  bool rc = true;
  std::string s1( "Hello, World!" );
  if( s1.find_last_of( "eoW" ) != 8 ) {
    std::cout << "find_last_of FAIL 0001\n"; rc = false;
  }
  if( s1.find_last_of( "eoW", 1 ) != 1 ) {
    std::cout << "find_last_of FAIL 0002\n"; rc = false;
  }
  if( s1.find_last_of( "oWe", 2 ) != 1 ) {
    std::cout << "find_last_of FAIL 0003\n"; rc = false;
  }
  if( s1.find_last_of( "!" ) != 12 ) {
    std::cout << "find_last_of FAIL 0004\n"; rc = false;
  }
  if( s1.find_last_of( "!", 13 ) != 12 ) {
    std::cout << "find_last_of FAIL 0005\n"; rc = false;
  }
  if( s1.find_last_of( "z#+" ) != std::string::npos ) {
    std::cout << "find_last_of FAIL 0006\n"; rc = false;
  }

  return( rc );
}

bool find_first_not_of_test()
{
  bool rc = true;

  std::string good_chars("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  std::string s1("xAWQKSMQIFJJWXGV"); // Be sure both 'A' and 'Z' represented.
  std::string s2("KFNNAWEZxZNEKGDW");
  std::string s3("QMVVXZKRIGJJWTAx");
  std::string s4("UUDHHAKVVLZVFLQP");
  std::string s5("");

  if( s1.find_first_not_of( good_chars ) != 0 ) {
    std::cout << "find_first_not_of FAIL 0001\n"; rc = false;
  }
  if( s2.find_first_not_of( good_chars ) != 8 ) {
    std::cout << "find_first_not_of FAIL 0002\n"; rc = false;
  }
  if( s3.find_first_not_of( good_chars ) != 15 ) {
    std::cout << "find_first_not_of FAIL 0003\n"; rc = false;
  }
  if( s4.find_first_not_of( good_chars ) != std::string::npos ) {
    std::cout << "find_first_not_of FAIL 0004\n"; rc = false;
  }
  if( s2.find_first_not_of( good_chars, 8 ) != 8 ) {
    std::cout << "find_first_not_of FAIL 0005\n"; rc = false;
  }
  if( s3.find_first_not_of( good_chars, 15) != 15 ) {
    std::cout << "find_first_not_of FAIL 0006\n"; rc = false;
  }
  if( s3.find_first_not_of( good_chars, 16) != std::string::npos ) {
    std::cout << "find_first_not_of FAIL 0007\n"; rc = false;
  }
  if( s3.find_first_not_of( good_chars, 17) != std::string::npos ) {
    std::cout << "find_first_not_of FAIL 0008\n"; rc = false;
  }
  if( s5.find_first_not_of( good_chars ) != std::string::npos ) {
    std::cout << "find_first_not_of FAIL 0009\n"; rc = false;
  }
  if( s4.find_first_not_of( s5 ) != 0 ) {
    std::cout << "find_first_not_of FAIL 0010\n"; rc = false;
  }
  return( rc );
}


bool find_last_not_of_test()
{
  bool rc = true;

  std::string good_chars("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  std::string s1("xAWQKSMQIFJJWXGV");  // Be sure both 'A' and 'Z' represented.
  std::string s2("KFNNAWEZxZNEKGDW");
  std::string s3("QMVVXZKRIGJJWTAx");
  std::string s4("UUDHHAKVVLZVFLQP");
  std::string s5("");

  if( s1.find_last_not_of( good_chars ) != 0 ) {
    std::cout << "find_last_not_of FAIL 0001\n"; rc = false;
  }
  if( s2.find_last_not_of( good_chars ) != 8 ) {
    std::cout << "find_last_not_of FAIL 0002\n"; rc = false;
  }
  if( s3.find_last_not_of( good_chars ) != 15 ) {
    std::cout << "find_last_not_of FAIL 0003\n"; rc = false;
  }
  if( s4.find_last_not_of( good_chars ) != std::string::npos ) {
    std::cout << "find_last_not_of FAIL 0004\n"; rc = false;
  }
  if( s2.find_last_not_of( good_chars, 8 ) != 8 ) {
    std::cout << "find_last_not_of FAIL 0005\n"; rc = false;
  }
  if( s3.find_last_not_of( good_chars, 15) != 15 ) {
    std::cout << "find_last_not_of FAIL 0006\n"; rc = false;
  }
  if( s3.find_last_not_of( good_chars, 16) != 15 ) {
    std::cout << "find_last_not_of FAIL 0007\n"; rc = false;
  }
  if( s3.find_last_not_of( good_chars, 17) != 15 ) {
    std::cout << "find_last_not_of FAIL 0008\n"; rc = false;
  }
  if( s5.find_last_not_of( good_chars ) != std::string::npos ) {
    std::cout << "find_last_not_of FAIL 0009\n"; rc = false;
  }
  if( s4.find_last_not_of( s5 ) != 15 ) {
    std::cout << "find_last_not_of FAIL 0010\n"; rc = false;
  }
  return( rc );
}

bool substr_test()
{
  bool rc = true;

  std::string s1("Hello, World!");

  if( s1.substr( ) != "Hello, World!" ) {
    std::cout << "substr FAIL 0001\n"; rc = false;
  }
  if( s1.substr( 1 ) != "ello, World!" ) {
    std::cout << "substr FAIL 0002\n"; rc = false;
  }
  if( s1.substr( 0, 1 ) != "H" ) {
    std::cout << "substr FAIL 0003\n"; rc = false;
  }
  if( s1.substr( 0, 0 ) != "" ) {
    std::cout << "substr FAIL 0004\n"; rc = false;
  }
  if( s1.substr( 12 ) != "!" ) {
    std::cout << "substr FAIL 0005\n"; rc = false;
  }
  if( s1.substr( 3, 4 ) != "lo, " ) {
    std::cout << "substr FAIL 0006\n"; rc = false;
  }
  if( s1.substr( 13 ) != "" ) {
    std::cout << "substr FAIL 0007\n"; rc = false;
  }
  try {
    if( s1.substr( 14 ) != "" ) {
      std::cout << "substr FAIL 0008\n"; rc = false;
    }
  }
  catch( std::out_of_range ) {
    // Ok.
  }
  return( rc );
}


// Main Program
// ============

int main( )
{
  int rc = 0;
  int original_count = heap_count( );

  try {
    if( !construct_test( )         || !heap_ok( "t01" ) ) rc = 1;
    if( !assign_test( )            || !heap_ok( "t02" ) ) rc = 1;
    if( !access_test( )            || !heap_ok( "t03" ) ) rc = 1;
    if( !relational_test( )        || !heap_ok( "t04" ) ) rc = 1;
    if( !capacity_test( )          || !heap_ok( "t05" ) ) rc = 1;
    if( !iterator_test( )          || !heap_ok( "t06" ) ) rc = 1;
    if( !append_test( )            || !heap_ok( "t07" ) ) rc = 1;
    if( !insert_test( )            || !heap_ok( "t08" ) ) rc = 1;
    if( !erase_test( )             || !heap_ok( "t09" ) ) rc = 1;
    if( !replace_test( )           || !heap_ok( "t10" ) ) rc = 1;
    if( !iterator_replace_test( )  || !heap_ok( "t11" ) ) rc = 1;
    if( !copy_test( )              || !heap_ok( "t12" ) ) rc = 1;
    if( !swap_test( )              || !heap_ok( "t13" ) ) rc = 1;
    if( !cstr_test( )              || !heap_ok( "t14" ) ) rc = 1;
    if( !find_test( )              || !heap_ok( "t15" ) ) rc = 1;
    if( !rfind_test( )             || !heap_ok( "t16" ) ) rc = 1;
    if( !find_first_of_test( )     || !heap_ok( "t17" ) ) rc = 1;
    if( !find_last_of_test( )      || !heap_ok( "t18" ) ) rc = 1;
    if( !find_first_not_of_test( ) || !heap_ok( "t19" ) ) rc = 1;
    if( !find_last_not_of_test( )  || !heap_ok( "t20" ) ) rc = 1;
    if( !substr_test( )            || !heap_ok( "t21" ) ) rc = 1;
  }
  catch( std::out_of_range e ) {
    std::cout << "Unexpected out_of_range exception: " << e.what( ) << "\n";
    rc = 1;
  }
  catch( std::length_error e ) {
    std::cout << "Unexpected length_error exception: " << e.what( ) << "\n";
    rc = 1;
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
