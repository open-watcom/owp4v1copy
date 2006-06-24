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
* Description:  This file contains the functional tests for most of the
*               algorithms in <algorithm>. The sorting and related
*               algorithms are tested in alg02.cpp.
*
* To-Do: The non-mutating algorithms should work on input sequences that
*        are constants. To test this the iteratory catagory templates
*        need to be smarter.
*
****************************************************************************/

#include <cstring>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include "itcat.h"
#include "sanity.cpp"

// "Little" functions used in some of the tests.
// ---------------------------------------------

void advance_char( char &ch )
{
    if( ++ch == 'z' ) ch = 'a';
}

// Make some internally linked to see if that causes any problems.
static bool is_odd( int num )
{
    return( static_cast<bool>( num % 2 ) );
}

bool both_oddeven( int num1, int num2 )
{
    if( is_odd( num1 ) && is_odd( num2 ) ) return true;
    if( !is_odd( num1 ) && !is_odd( num2 ) ) return true;
    return false;
}

bool is_divisible( int x, int y )
{
    return( !(x % y) );
}

static int square( int num )
{
    return( num * num );
}

std::string join_strings( const std::string &left,
                          const std::string &right )
{
    return( left + right );
}

static int powers_of_two( )
{
    static int current = 1;

    int temp = current;
    current *= 2;
    return( temp );
}

// Non-modifying sequence operations
// ---------------------------------

bool for_each_test( )
{
    char str[] = { 'h', 'e', 'l', 'l', 'o', '\0' };

    // Use FwdIt instead of InpIt because operation modifies sequence.
    std::for_each( FwdIt<char>(str), FwdIt<char>(str), advance_char );
    if( std::strcmp( str, "hello" ) != 0 ) FAIL;

    std::for_each
      ( FwdIt<char>(str), FwdIt<char>(str + 5), advance_char);
    if( std::strcmp( str, "ifmmp" ) != 0 ) FAIL;

    return( true );
}


bool find_test( )
{
    int array[] = { 0, 1, 2, 3, 4 };

    InpIt<int> p1( std::find( InpIt<int>(array), InpIt<int>(array + 5), 2 ) );
    if( p1 == InpIt<int>(array + 5) || *p1 != 2 ) FAIL;

    InpIt<int> p2( std::find( InpIt<int>(array), InpIt<int>(array + 5), 5 ) );
    if( p2 != InpIt<int>(array + 5) ) FAIL;

    InpIt<int> p3( std::find( InpIt<int>(array), InpIt<int>(array), 1 ) );
    if( p3 != InpIt<int>(array) ) FAIL;

    InpIt<int> p4( std::find_if( InpIt<int>(array), InpIt<int>(array + 5),
                                 std::bind2nd( std::greater< int >( ), 3 ) ) );
    if( p4 == InpIt<int>(array + 5) || *p4 != 4 ) FAIL;

    InpIt<int> p5( std::find_if( InpIt<int>(array), InpIt<int>(array + 5),
                                 is_odd ) );
    if( p5 == InpIt<int>(array + 5) || *p5 != 1 ) FAIL;

    return( true );
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
    if( std::find_end( FwdIt<int>(a), FwdIt<int>(a+6),
                       FwdIt<int>(b), FwdIt<int>(b+3) ) != FwdIt<int>(a+3) ) FAIL
    //can't find because subsequence is longer than sequence
    if( std::find_end( FwdIt<int>(b), FwdIt<int>(b+3),
                       FwdIt<int>(a), FwdIt<int>(a+6) ) != FwdIt<int>(b+3) ) FAIL
    //can't find because no match
    if( std::find_end( FwdIt<int>(a), FwdIt<int>(a+6),
                       FwdIt<int>(d), FwdIt<int>(d+2) ) != FwdIt<int>(a+6) ) FAIL
    //no match, same size sequences
    if( std::find_end( FwdIt<int>(a), FwdIt<int>(a+6),
                       FwdIt<int>(c), FwdIt<int>(c+6) ) != FwdIt<int>(a+6) ) FAIL
    //exact matching sequence
    if( std::find_end( FwdIt<int>(c), FwdIt<int>(c+6),
                       FwdIt<int>(e), FwdIt<int>(e+6) ) != FwdIt<int>(c) ) FAIL
    //find in middle
    if( std::find_end( FwdIt<int>(c), FwdIt<int>(c+6),
                       FwdIt<int>(d), FwdIt<int>(d+2) ) != FwdIt<int>(c+2) ) FAIL
    //multi matches
    if( std::find_end( FwdIt<int>(f), FwdIt<int>(f+10),
                       FwdIt<int>(d), FwdIt<int>(d+2) ) != FwdIt<int>(f+7) ) FAIL
    //substring is only 1 long
    if( std::find_end( FwdIt<int>(c), FwdIt<int>(c+6),
                       FwdIt<int>(g), FwdIt<int>(g+1) ) != FwdIt<int>(c+4) ) FAIL

    //quick test with predictate, there should realy be more but the
    //predicate version of find_end is just a copy of the non-predicate
    //version, as so long as any updates are applied to both functions it
    //will be ok.
    int h[] = { 2,3 };
    if( std::find_end( FwdIt<int>(c), FwdIt<int>(c+6),
                       FwdIt<int>(h), FwdIt<int>(h+2), is_divisible ) !=
                                                         FwdIt<int>(c+4) ) FAIL

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
    FwdIt<int> i2;
    //find a x that is evenly divisible by a y
    i2 = find_first_of( FwdIt<int>(x), FwdIt<int>(x+4),
                        FwdIt<int>(y), FwdIt<int>(y+4), is_divisible );
    if( i2 != FwdIt<int>(x+1) ) FAIL
    //don't find one
    i2 = find_first_of( FwdIt<int>(x), FwdIt<int>(x+4),
                        FwdIt<int>(y+2), FwdIt<int>(y+4), is_divisible );
    if( i2 != FwdIt<int>(x+4) ) FAIL
  
    return( true );
}


bool adjacent_find_test( )
{
    int a[] = { 1, 2, 3, 4 };
    int b[] = { 1, 1, 2, 3 };
    int c[] = { 1, 2, 3, 3 };
    int d[] = { 5, 9, 3, 2 };
    FwdIt<int> p;

    // Sequence of length 0.
    p = std::adjacent_find( FwdIt<int>(a), FwdIt<int>(a) );
    if( p != FwdIt<int>(a) ) FAIL;
    // Sequence of length 1.
    p = std::adjacent_find( FwdIt<int>(a), FwdIt<int>(a + 1) );
    if( p != FwdIt<int>(a + 1) ) FAIL;
    // Sequence of length 2, no match.
    p = std::adjacent_find( FwdIt<int>(a), FwdIt<int>(a + 2) );
    if( p != FwdIt<int>(a + 2) ) FAIL;
    // Long sequence, no match.
    p = std::adjacent_find( FwdIt<int>(a), FwdIt<int>(a + 4) );
    if( p != FwdIt<int>(a + 4) ) FAIL;
    // Sequence of length two, with match.
    p = std::adjacent_find( FwdIt<int>(b), FwdIt<int>(b + 2) );
    if( p != FwdIt<int>(b) ) FAIL;
    // Long sequence, match early.
    p = std::adjacent_find( FwdIt<int>(b), FwdIt<int>(b + 4) );
    if( p != FwdIt<int>(b) ) FAIL;
    // Long sequence, match late.
    p = std::adjacent_find( FwdIt<int>(c), FwdIt<int>(c + 4) );
    if( p != FwdIt<int>(c + 2) ) FAIL;
    // Predicate check.
    p = std::adjacent_find( FwdIt<int>(d), FwdIt<int>(d + 4), is_divisible );
    if( p != FwdIt<int>(d + 1) ) FAIL;

    return( true );
}


#ifdef NEVER
bool count_test( )
{
    char a[] = { 'a', 'b', 'b', 'd', 'e' };
    int n = -1;

    n = std::count( InpIt<int>(a), InpIt<int>(a + 5), 'b' );
    if( n != 2 ) FAIL;

    n = std::count( InpIt<int>(a), InpIt<int>(a + 5), 'c' );
    if( n != 0) FAIL;

    n = std::count( InpIt<int>(a), InpIt<int>(a), 'a' );
    if( n != 0 ) FAIL;

    n = std::count_if( InpIt<int>(a), InpIt<int>(a + 5), is_odd );
    if( n != 2 ) FAIL;

    n = std::count_if(InpIt<int>(a), InpIt<int>(a + 5),
                      std::bind1st( std::less< int >( ), 'b' ) );
    if( n != 2 ) FAIL;

    return( true );
}
#endif


bool mismatch_test( )
{
    using namespace std;

    // Does this local typedef cause a problem?
    typedef pair< InpIt<int>, InpIt<int> > result_type;

    int a[] = { 1, 2, 3, 4 };
    int b[] = { 0, 2, 3, 4 };
    int c[] = { 1, 2, 3, 5 };
    int d[] = { 5, 4, 5, 8 };

    result_type result;

    // First sequence of zero length.
    result = mismatch( InpIt<int>(a), InpIt<int>(a), InpIt<int>(a) );
    if( result.first != InpIt<int>(a) ) FAIL;

    // First sequence of length one, no mismatch.
    result = mismatch( InpIt<int>(a), InpIt<int>(a + 1), InpIt<int>(a) );
    if( result.first != InpIt<int>(a + 1) ) FAIL;
    // First sequence long, no mismatch.
    result = mismatch( InpIt<int>(a), InpIt<int>(a + 4), InpIt<int>(a) );
    if( result.first != InpIt<int>(a + 4) ) FAIL;
    // First sequence of length one, with mismatch.
    result = mismatch( InpIt<int>(b), InpIt<int>(b + 1), InpIt<int>(a) );
    if( result.first != InpIt<int>(b) && result.second != InpIt<int>(a) ) FAIL;
    // First sequence long, mismatch in first position.
    result = mismatch( InpIt<int>(b), InpIt<int>(b + 4), InpIt<int>(a) );
    if( result.first != InpIt<int>(b) && result.second != InpIt<int>(a) ) FAIL;
    // First sequence long, mismatch at the end.
    result = mismatch( InpIt<int>(c), InpIt<int>(c + 4), InpIt<int>(a) );
    if( result.first != InpIt<int>(c + 3) &&
        result.second != InpIt<int>(a + 3) ) FAIL;
    // Predicate check.
    result = mismatch( InpIt<int>(d), InpIt<int>(d + 4), InpIt<int>(a),
                       is_divisible );
    if( result.first != InpIt<int>(d + 2) &&
        result.second != InpIt<int>(a + 2) ) FAIL;

    return( true );
}


bool equal_test( )
{
    int a1[] = { 0,  1,  2,  3,  4 };
    int a2[] = { 0,  1,  2,  3,  4 };
    int a3[] = { 0,  1,  3,  3,  4 };
    int a4[] = { 0, -1, -2, -3, -4 };

    if( !std::equal( InpIt<int>(a1), InpIt<int>(a1 + 5),
                     InpIt<int>(a2) ) ) FAIL;
    if(  std::equal( InpIt<int>(a1), InpIt<int>(a1 + 3),
                     InpIt<int>(a3) ) ) FAIL;
    if( !std::equal( InpIt<int>(a1), InpIt<int>(a1),
                     InpIt<int>(a2) ) ) FAIL;
    if( !std::equal( InpIt<int>(a1), InpIt<int>(a1 + 5),
                     InpIt<int>(a4), std::greater_equal< int >( ) ) ) FAIL;

    return( true );
}


bool copy_test( )
{
    int array1[] = { 0, 1, 2, 3, 4 };
    int array2[] = { 0, 0, 0, 0, 0 };
    int array3[] = { 0, 0, 0, 0, 0 };
    int array4[] = { 1, 2, 3, 4, 4 };

    std::copy( InpIt<int>(array1),
               InpIt<int>(array1 + 5),
               OutIt<int>(array2) );
    if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    OutIt<int> p = std::copy( InpIt<int>(array1),
                              InpIt<int>(array1 + 3),
                              OutIt<int>(array3) );
    std::copy( InpIt<int>(array1 + 3), InpIt<int>(array1 + 5), p );
    if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::copy( InpIt<int>(array1 + 1),
               InpIt<int>(array1 + 5),
               OutIt<int>(array1) );
    if( std::memcmp( array1, array4, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array5[] = { 0, 1, 2, 3, 4 };
    int array6[] = { 0, 0, 1, 2, 3 };

    std::copy_backward( BidIt<int>(array5),
                        BidIt<int>(array5 + 4),
                        BidIt<int>(array5 + 5) );
    if( std::memcmp( array5, array6, 5 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
}


bool swap_test( )
{
    int array1[] = { 0, 1, 2, 3, 4 };
    int array2[] = { 4, 3, 2, 1, 0 };

    int result_array1[] = { 4, 3, 2, 1, 0 };
    int result_array2[] = { 0, 1, 2, 3, 4 };

    std::swap_ranges( FwdIt<int>(array1),
                      FwdIt<int>(array1 + 5),
                      FwdIt<int>(array2) );
    if( std::memcmp( array1, result_array1, 5 * sizeof( int ) ) != 0 ) FAIL;
    if( std::memcmp( array2, result_array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::string sarray1[] = { "abc", "xyz", "123" };
    std::string sarray2[] = { "123", "xyz", "abc" };

    std::swap_ranges( FwdIt<std::string>(sarray1),
                      FwdIt<std::string>(sarray1 + 3),
                      FwdIt<std::string>(sarray2) );
    if( sarray1[0] != "123" || sarray1[1] != "xyz" || sarray1[2] != "abc" ) FAIL;
    if( sarray2[0] != "abc" || sarray2[1] != "xyz" || sarray2[2] != "123" ) FAIL;

    return( true );
}


bool transform_test( )
{
    int array1[]  = { 0, 1, 2, 3,  4 };
    int array2[]  = { 0, 0, 0, 0,  0 };
    int result1[] = { 0, 1, 4, 9, 16 };
    int result2[] = { 0, 2, 4, 6,  8 };

    std::transform( InpIt<int>(array1),
                    InpIt<int>(array1 + 5),
                    OutIt<int>(array2),
                    square );
    if( std::memcmp( array2, result1, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::transform( InpIt<int>(array1),
                    InpIt<int>(array1 + 5),
                    OutIt<int>(array2),
                    std::bind1st( std::multiplies< int >( ), 2 ) );
    if( std::memcmp( array2, result2, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::string sarray1[] = { "hello", "fizzle" };
    std::string sarray2[] = { "world", "sticks" };
    std::string sarray3[2];
    std::string sresult[] = { "helloworld", "fizzlesticks" };

    std::transform( InpIt<std::string>(sarray1),
                    InpIt<std::string>(sarray1 + 2),
                    InpIt<std::string>(sarray2),
                    OutIt<std::string>(sarray3),
                    join_strings );
    if( sarray3[0] != sresult[0] || sarray3[1] != sresult[1] ) FAIL;

    return( true );
}


bool replace_test( )
{
    int array1[] = { 0, 1,  2, 3, 4 };
    int array2[] = { 0, 1, -1, 3, 4 };

    std::replace( FwdIt<int>(array1), FwdIt<int>(array1 + 5), 2, -1 );
    if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array3[] = {  0, 1,  2, 3,  4 };
    int array4[] = { -1, 1, -1, 3, -1 };

    std::replace_if( FwdIt<int>(array3),
                     FwdIt<int>(array3 + 5),
                     std::not1( std::ptr_fun( is_odd ) ),
                     -1 );
    if( std::memcmp( array3, array4, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array5[] = { 0, 1, 2,  3, 4 };
    int array6[5];
    int array7[] = { 0, 1, 2, -1, 4 };

    std::replace_copy( InpIt<int>(array5),
                       InpIt<int>(array5 + 5),
                       OutIt<int>(array6), 3,
                       -1 );
    if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array8[] = {  0,  1,  2, 3, 4 };
    int array9[5];
    int arrayA[] = { -1, -1, -1, 3, 4 };

    std::replace_copy_if( InpIt<int>(array8),
                          InpIt<int>(array8 + 5),
                          OutIt<int>(array9),
                          std::bind2nd( std::less_equal< int >( ), 2 ),
                          -1 );
    if( std::memcmp( array9, arrayA, 5 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
}


bool fill_test( )
{
    int array1[5];
    int array2[] = { -2, -2, -2, -2, -2 };
    int array3[] = {  1,  1,  1, -2, -2 };

    std::fill( FwdIt<int>(array1), FwdIt<int>(array1 + 5), -2 );
    if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::fill_n( OutIt<int>(array1), 3, 1 );
    if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
}


bool generate_test( )
{
    int array1[5];
    int array2[] = {  1,  2, 4, 8, 16 };
    int array3[] = { 32, 64, 4, 8, 16 };

    std::generate( FwdIt<int>(array1), FwdIt<int>(array1 + 5), powers_of_two );
    if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    std::generate_n( OutIt<int>(array1), 2, powers_of_two );
    if( std::memcmp( array1, array3, 5 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
}

bool remove_test( )
{
    FwdIt<int> o;
    int ref[] = { 1,2,3,4,3,2,1 };
    int a1[] = { 1,2,3,4,3,2,1 };
    //---- remove ----
    //don't remove anything
    o = std::remove( FwdIt<int>(a1), FwdIt<int>(a1 + 7), 0 );
    if( std::memcmp( a1, ref, sizeof( ref ) ) || (o.get() != a1 + 7) ) FAIL
    //remove 2s
    o = std::remove( FwdIt<int>(a1), FwdIt<int>(a1 + 7), 2 );
    int ref2[] = { 1,3,4,3,1 };
    if( std::memcmp( a1, ref2, sizeof( ref2 ) ) || (o.get() != a1 + 5) ) FAIL
    //remove 1s
    o = std::remove( FwdIt<int>(a1), FwdIt<int>(a1 + 5), 1 );
    int ref3[] = { 3,4,3 };
    if( std::memcmp( a1, ref3, sizeof( ref3 ) ) || (o.get() != a1 + 3) ) FAIL
    //remove 4
    o = std::remove( FwdIt<int>(a1), FwdIt<int>(a1 + 3), 4 );
    int ref4[] = { 3,3 };
    if( std::memcmp( a1, ref4, sizeof( ref4 ) ) || (o.get() != a1 + 2) ) FAIL
    //remove 3
    o = std::remove( FwdIt<int>(a1), FwdIt<int>(a1 + 2), 3 );
    if( o.get() != a1 ) FAIL
  
    //---- remove_copy_if ----
    int out[9];
    int a2[] = { 1,2,3,4,5,4,3,2,1 };
    int ref5[] = { 2,4,4,2 };
    OutIt<int> o2 = std::remove_copy_if( InpIt<int>(a2),
                                         InpIt<int>(a2 + 9),
                                         OutIt<int>(out),
                                         is_odd ); //removes if !false
    if( std::memcmp( out, ref5, sizeof( ref5 ) ) ) FAIL
  
    //---- remove_if ----
    //just a quick test because it is really the same as remove_copy_if
    o = std::remove_if( FwdIt<int>(a2), FwdIt<int>(a2 + 9), is_odd ); //removes if !false
    if( std::memcmp( out, ref5, sizeof( ref5 ) ) || (o.get() != a2 + 4 ) ) FAIL
  
    //---- remove_copy ----
    //just a quick test because it is really the same as remove
    int a3[] = { 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, 6, 9, 7, 9 };
    int ref6[] = { 1, 2, 3, 4, 5, 6, 7 };
    o2 = std::remove_copy( InpIt<int>(a3),
                           InpIt<int>(a3+15),
                           OutIt<int>(out),
                           9 );
    if( std::memcmp( out, ref6, sizeof( ref6 ) ) ) FAIL
  
    return( true );
}


bool unique_test( )
{
    int s1[] = { 0, 1, 2, 3 };
    int s2[] = { 0, 0, 0, 0 };
    int s3[] = { 1, 2, 2, 3, 3, 3, 4, 5, 5 };
    int s3_result[] = { 1, 2, 3, 4, 5 };

    FwdIt<int> p;
    p = std::unique( FwdIt<int>(s1), FwdIt<int>(s1 + 4) );
    if( p.get() != s1 + 4 ) FAIL;

    p = std::unique( FwdIt<int>(s2), FwdIt<int>(s2 + 4) );
    if( p.get() != s2 + 1 || *p.get() != 0 ) FAIL;

    p = std::unique( FwdIt<int>(s3), FwdIt<int>(s3 + 9) );
    if( p.get() != s3 + 5 ) FAIL;
    for( int i = 0; i < 5; ++i ) {
        if( s3[i] != s3_result[i] ) FAIL;
    }

    int s4[] = { 1, 3, 5, 2, 4, 6 };
    int s4_result[] = { 1, 2 };

    p = std::unique( FwdIt<int>(s4), FwdIt<int>(s4 + 6), both_oddeven );
    if( p.get() != s4 + 2 ) FAIL;
    for( int i = 0; i < 2; ++i ) {
        if( s4[i] != s4_result[i] ) FAIL;
    }

    int c1[] = { 0, 1, 2, 3 };
    int c2[] = { 0, 0, 0, 0 };
    int c3[] = { 1, 2, 2, 3, 3, 3, 4, 5, 5 };
    int c_result[ 16 ];

    int c1_result[] = { 0, 1, 2, 3 };
    int c2_result[] = { 0 };
    int c3_result[] = { 1, 2, 3, 4, 5 };

    OutIt<int> p1 = std::unique_copy( InpIt<int>(c1),
                                      InpIt<int>(c1 + 4),
                                      OutIt<int>(c_result) );
    if( p1.get() != c_result + 4 ) FAIL;
    if( std::memcmp( c_result, c1_result, 4 * sizeof( int ) ) != 0 ) FAIL;

    OutIt<int> p2 = std::unique_copy( InpIt<int>(c2),
                                      InpIt<int>(c2 + 4),
                                      OutIt<int>(c_result) );
    if( p2.get() != c_result + 1 ) FAIL;
    if( std::memcmp( c_result, c2_result, 1 * sizeof( int ) ) != 0 ) FAIL;

    OutIt<int> p3 = std::unique_copy( InpIt<int>(c3),
                                      InpIt<int>(c3 + 9),
                                      OutIt<int>(c_result) );
    if( p3.get() != c_result + 5 ) FAIL;
    if( std::memcmp( c_result, c3_result, 5 * sizeof( int ) ) != 0 ) FAIL;

    int c4[] = { 1, 3, 5, 2, 4, 6 };
    int c4_result[] = { 1, 2 };

    OutIt<int> p4 = std::unique_copy( InpIt<int>(c4),
                                      InpIt<int>(c4 + 6),
                                      OutIt<int>(c_result),
                                      both_oddeven );
    if( p4.get() != c_result + 2 ) FAIL;
    if( std::memcmp( c_result, c4_result, 2 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
}


bool reverse_test( )
{
    int array1[] = { 0, 1, 2, 3, 4 };
    int array2[] = { 4, 3, 2, 1, 0 };

    std::reverse( BidIt<int>(array1), BidIt<int>(array1 + 5) );
    if( std::memcmp( array1, array2, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array3[] = { 0, 1, 2, 3 };
    int array4[] = { 3, 2, 1, 0 };

    std::reverse( BidIt<int>(array3), BidIt<int>(array3 + 4) );
    if( std::memcmp( array3, array4, 4 * sizeof( int ) ) != 0 ) FAIL;

    int array5[] = { 0, 1, 2, 3, 4 };
    int array6[5];
    int array7[] = { 4, 3, 2, 1, 0 };

    std::reverse_copy( BidIt<int>(array5),
                       BidIt<int>(array5 + 5),
                       OutIt<int>(array6) );
    if( std::memcmp( array6, array7, 5 * sizeof( int ) ) != 0 ) FAIL;

    int array8[] = { 0, 1, 2, 3 };
    int array9[4];
    int arrayA[] = { 3, 2, 1, 0 };

    std::reverse_copy( BidIt<int>(array8),
                       BidIt<int>(array8 + 4),
                       OutIt<int>(array9) );
    if( std::memcmp( array9, arrayA, 4 * sizeof( int ) ) != 0 ) FAIL;

    return( true );
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

    return( true );
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
    if( !adjacent_find_test( )  || !heap_ok( "t05" ) ) rc = 1;
    // if( !count_test( )       || !heap_ok( "t06" ) ) rc = 1;
    if( !mismatch_test( )       || !heap_ok( "t07" ) ) rc = 1;
    if( !equal_test( )          || !heap_ok( "t08" ) ) rc = 1;
    if( !copy_test( )           || !heap_ok( "t09" ) ) rc = 1;
    if( !swap_test( )           || !heap_ok( "t10" ) ) rc = 1;
    if( !transform_test( )      || !heap_ok( "t11" ) ) rc = 1;
    if( !replace_test( )        || !heap_ok( "t12" ) ) rc = 1;
    if( !fill_test( )           || !heap_ok( "t13" ) ) rc = 1;
    if( !generate_test( )       || !heap_ok( "t14" ) ) rc = 1;
    if( !remove_test( )         || !heap_ok( "t15" ) ) rc = 1;
    if( !unique_test( )         || !heap_ok( "t16" ) ) rc = 1;
    if( !reverse_test( )        || !heap_ok( "t17" ) ) rc = 1;
    if( !random_shuffle_test( ) || !heap_ok( "t18" ) ) rc = 1;
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
