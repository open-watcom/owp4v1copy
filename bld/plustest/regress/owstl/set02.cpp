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
* Description:  Tests for std::multimap
*
****************************************************************************/

//#include <cstdlib>
#include <iostream>
#include <string>
#include "sanity.cpp"
#include "allocxtr.hpp"
//#include <_rbtree.h>
#include <set>

/* ------------------------------------------------------------------
 * construct_test( )
 * Construct different maps in different ways.
 */
bool construct_test( )
{
    typedef std::multiset< int, int > msii_t;
    msii_t s1;
    int i;
    for( i = 0;  i < 100;  i++ ){
        s1.insert( 1 );
        s1.insert( 2 );
    }
    return( true );
}
/* ------------------------------------------------------------------
 * access_test( )
 * insert, find, erase, count
 */
bool access_test( )
{
    typedef std::multiset< int, int > msii_t;
    typedef msii_t::iterator msiter_t;
    msii_t s1;
    
    int num[] = {11,11,11,10,4,8,8,2,1,12,13,19,7,18,4,2,2,17,11};
    int notnum[] = {3,5,6,9,14,15,16};
    int const num_size = sizeof(num) / sizeof(int);
    int const notnum_size = sizeof(notnum) / sizeof(int);
        
    //insert
    for( int i = 0; i < num_size; i++ ){
        msiter_t it;
        it = s1.insert( num[i] );
        if( INSANE( s1 ) || s1.size() != (i+1) || s1.empty() ||
            it == s1.end() || *it != num[i] ) FAIL
    }
    //find inserted
    for( int i = 0; i < num_size; i++ ){
        msiter_t it = s1.find( num[i] );
        if( INSANE( s1 ) || s1.size() != num_size || s1.empty() ||
            it == s1.end() || *it != num[i] ) FAIL
    }
    //find not inserted
    for( int i = 0; i < notnum_size; i++ ){
        msiter_t it = s1.find( notnum[i] );
        if( INSANE( s1 ) || s1.size() != num_size || s1.empty() ||
            it != s1.end() ) FAIL
    }
    //insert again
    for( int i = 0; i < num_size; i++ ){
        msiter_t it;
        it = s1.insert( num[i] );
        if( INSANE( s1 ) || s1.size() != (num_size+i+1) ||
            s1.empty() || it == s1.end() || *it != num[i] ) FAIL
    }
    //spot checks on contents
    if( s1.count( 11 ) != 8 ) FAIL      //count the number of 11's
    msiter_t it = s1.find( 11 );
    for( int i = 0; i < 4; i++ ){
        if( INSANE( s1 ) || s1.size() != num_size*2 ||
            s1.empty() || it == s1.end() || *it != 11 ) FAIL
        ++it;
    }
    //erase tests
    //erase first of multiple keys
    s1.erase( s1.find( 11 ) );
    if( INSANE( s1 ) || s1.size() != num_size*2-1 || s1.count( 11 ) != 7 ) FAIL
    //erase last of multiple keys
    s1.erase( --( s1.find( 12 ) ) );
    if( INSANE( s1 ) || s1.size() != num_size*2-2 || s1.count( 11 ) != 6 ) FAIL
    //erase middle of multiple keys
    s1.erase( --(--(--( s1.find( 12 ) ))) );
    if( INSANE( s1 ) || s1.size() != num_size*2-3 || s1.count( 11 ) != 5 ) FAIL
    //erase all keys
    s1.erase( s1.find(12) );
    s1.erase( s1.find(12) );
    if( INSANE( s1 ) || s1.size() != num_size*2-5 || s1.count( 12 ) != 0 ) FAIL
    //erase all keys other method
    if( s1.erase( 2 ) != 6 ) FAIL;
    if( INSANE( s1 ) || s1.size() != num_size*2-11 || s1.count( 2 ) != 0 ) FAIL
    
    return( true );
}

int main( )
{
    int rc = 0;
    //heap_dump();
    int original_count = heap_count( );
    
    try {
        if( !construct_test( )  || !heap_ok( "t1" ) ) rc = 1;
        if( !access_test( )     || !heap_ok( "t2" ) ) rc = 1;
        //if( !string_test( )     || !heap_ok( "t3" ) ) rc = 1;
        //if( !torture_test( )    || !heap_ok( "t4" ) ) rc = 1;
        //if( !clear_test( )      || !heap_ok( "t5" ) ) rc = 1;
        //if( !iterator_test( )   || !heap_ok( "t6" ) ) rc = 1;
        //if( !copy_test( )       || !heap_ok( "t6" ) ) rc = 1;
    }
    catch( ... ) {
        std::cout << "Unexpected exception of unexpected type.\n";
        rc = 1;
    }
    int heap_diff = heap_count( ) - original_count;
    if( heap_diff ) {
        heap_dump();
        std::cout << "Possible memory leak! " << heap_diff << " " << original_count << "\n";
        rc = 1;
    }
    
    return( rc );
}
