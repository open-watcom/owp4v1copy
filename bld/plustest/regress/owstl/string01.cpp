
#include <iostream>
#include <string>

// using namespace std;

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

  if( s1.size( ) != 0 ||
      s1         != "") {
    std::cout << "construct FAIL 0001\n"; rc = false;
  }
  if( s2.size( )     != 12         ||
      s2.capacity( ) <  s2.size( ) ||
      s2             != "Hello, World" ) {
    std::cout << "construct FAIL 0002\n"; rc = false;
  }
  if( s3.size( )     != 3         ||
      s3.capacity( ) < s3.size( ) ||
      s3             != "Hel" ) {
    std::cout << "construct FAIL 0003\n"; rc = false;
  }
  if( s4.size( )     != 12        ||
      s4.capacity( ) < s4.size( ) ||
      s4             != "Hello, World" ) {
    std::cout << "construct FAIL 0004\n"; rc = false;
  }
  if( s5.size( )     != 2         ||
      s5.capacity( ) < s5.size( ) ||
      s5             != "Wo" ) {
    std::cout << "construct FAIL 0005\n"; rc = false;
  }
  if( s6.size( )     != 5         ||
      s6.capacity( ) < s6.size( ) ||
      s6             != "World" ) {
    std::cout << "construct FAIL 0006\n"; rc = false;
  }
  if( s7.size( )     != 16        ||
      s7.capacity( ) < s7.size( ) ||
      s7             != "xxxxxxxxxxxxxxxx") {
    std::cout << "construct FAIL 0007\n"; rc = false;
  }
  return( rc );
}

bool assign_test( )
{
  bool rc = true;
  std::string s1( 16, 'x' );
  std::string s2( "Hello" );

  s1 = s2;
  if( s1.size( )     != 5          ||
      s1.capacity( ) <  s1.size( ) ||
      s1             != "Hello" ) {
    std::cout << "assign FAIL 0001\n"; rc = false;
  }

  std::string s3( 'x', 16 );
  s3 = "This string is longer than 16 characters";
  if( s3.size( )     != 40         ||
      s3.capacity( ) <  s3.size( ) ||
      s3             != "This string is longer than 16 characters" ) {
    std::cout << "assign FAIL 0002\n"; rc = false;
  }

  std::string s4( 'x', 16 );
  s4 = 'y';
  if( s4.size( )     != 1          ||
      s4.capacity( ) <  s4.size( ) ||
      s4             != "y" ) {
    std::cout << "assign FAIL 0003\n"; rc = false;
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

bool swap_test( )
{
  bool rc = true;

  std::string s1("ABC");
  std::string s2("XYZ");

  s1.swap( s2 );
  if( s1 != "XYZ" || s2 != "ABC" ) {
    std::cout << "swap FAIL 0001\n"; rc = false;
  }
  std::swap( s1, s2 );
  if( s1 != "ABC" || s2 != "XYZ" ) {
    std::cout << "swap FAIL 0002\n"; rc = false;
  }
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
      std::cout << "substr FAIL 0008: Exception expected!\n"; rc = false;
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

  try {
    if( !construct_test( )       ) rc = 1;
    if( !assign_test( )          ) rc = 1;
    if( !access_test( )          ) rc = 1;
    if( !relational_test( )      ) rc = 1;
    if( !capacity_test( )        ) rc = 1;
    if( !iterator_test( )        ) rc = 1;
    if( !append_test( )          ) rc = 1;
    if( !insert_test( )          ) rc = 1;
    if( !erase_test( )           ) rc = 1;
    if( !swap_test( )            ) rc = 1;
    if( !cstr_test( )            ) rc = 1;
    if( !find_test( )            ) rc = 1;
    if( !rfind_test( )           ) rc = 1;
    if( !find_first_of_test()    ) rc = 1;
    if( !find_last_of_test()     ) rc = 1;
    if( !find_first_not_of_test()) rc = 1;
    if( !find_last_not_of_test() ) rc = 1;
    if( !substr_test()           ) rc = 1;
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

  return( rc );
}

