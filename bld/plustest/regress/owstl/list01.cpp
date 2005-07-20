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
* Description:  This file contains the functional tests for std::list.
*
****************************************************************************/

#include <algorithm>
#include <iostream>
#include <list>
#include "sanity.cpp"
#include "allocxtr.hpp"

/* ------------------------------------------------------------------
 * access_test
 * test insert, push_front, push_back, erase, pop_front, pop_back 
 */
bool access_test( )
{
    std::list< int > lst;
    std::list< int >::iterator it;
    int i;
    //test insert
    for( i = 0; i < 10 ; i++ ){
        lst.insert( lst.begin(), i );
    }//now contains 9...0
    for( it = lst.begin(), i = 9; i >= 0; --i, ++it ){
        //std::cout<<*it<<",";
        if( INSANE(lst) || *it != i ) FAIL
    }
    //test push_front
    for( i = 10; i < 20 ; i++ ){
        lst.push_front( i );
    }//now contains 19...0
    for( it = lst.begin(), i = 19; i >= 0; --i, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    //test push_back
    for( i = -1; i > -11; i-- ){
        lst.push_back( i );
    }//now contains 19...-10
    for( it = lst.begin(), i = 19; i >= -10; --i, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    //test erase
    it = lst.begin();
    for( i = 0; i < 30; i+=2 ){
        lst.erase( it++ );
        ++it;
    }//now contains evens 18...-10
    for( it = lst.begin(), i = 18; i >= -10; i-=2, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    //test pop_front
    for( i = 18; i > 8; i-=2 ){
        if( lst.front() != i ) FAIL
        lst.pop_front( );
    }//now contains evens 8...-10
    for( it = lst.begin(), i = 8; i >= -10; i-=2, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    //test pop_back
    for( i = -10; i < 4; i+=2 ){
        if( lst.back() != i ) FAIL
        lst.pop_back( );
    }//now contains evens 8...4
    for( it = lst.begin(), i = 8; i >= 4; i-=2, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    lst.front() = 5;
    lst.back() = 7;
    for( it = lst.begin(), i = 5; i <= 7; ++i, ++it ){
        if( INSANE(lst) || *it != i ) FAIL
    }
    return( true );
}

/* ------------------------------------------------------------------
 * assign_test
 * test list assignment methods
 */
bool assign_test( )
{
    std::list< int > lst1, lst2;
    for( int i =  1; i <= 10; ++i ) lst1.push_back( i );
    for( int i = 11; i <= 15; ++i ) lst2.push_back( i );
    lst1 = lst2;
    //check it
    if( INSANE( lst1 ) || INSANE( lst2 ) ) FAIL
    if( lst1.size( ) != 5 || lst2.size( ) != 5 ) FAIL
    int i = 11;
    std::list< int >::iterator it = lst1.begin( );
    while( it != lst1.end( ) ) {
        if( *it != i ) FAIL
        ++i; ++it;
    }
    //try a different way
    lst1.assign( 10, -1 );
    //check it
    if( INSANE( lst1 ) || lst1.size( ) != 10 ) FAIL
    for( it = lst1.begin( ); it != lst1.end( ); ++it ) {
        if( *it != -1 ) FAIL
    }
    return( true );
}

/* ------------------------------------------------------------------
 * clear_test
 * test removal of elements, clear and destructor
 */
bool clear_test()
{
    std::list< int > lst, lst2, lst3;
    typedef std::list< int > list_t;
    list_t* lstp = new list_t;
    
    for( int i = 0; i < 10; i++ ){
        lst.push_front( i );
        lst2.push_front( i );
        lst3.push_front( i );
        lstp->push_front( i );
        if( INSANE( lst ) || lst.front() != i || lst.size() != i+1 ) FAIL
        if( INSANE( lst2 ) || lst.front() != i || lst.size() != i+1 ) FAIL
        if( INSANE( lst3 ) || lst.front() != i || lst.size() != i+1 ) FAIL
        if( INSANE( *lstp ) || lstp->front() != i || lst.size() != i+1 ) FAIL
    }
    //test clear
    lst.clear();
    if( INSANE( lst ) || lst.size() || !lst.empty() ) FAIL
    lst.clear();
    if( INSANE( lst ) || lst.size() || !lst.empty() ) FAIL
    //test removal
    for( int i = 0; i < 10; i++ ) lst2.pop_front();
    if( INSANE( lst2 ) || lst2.size() || !lst2.empty() ) FAIL
    //test destructor call
    delete lstp;
    //lst3 is cleared automatically 
    //(leak detect will fire if destructor is wrong)
    return( true );
}

/* ------------------------------------------------------------------
 * erase_test
 * test two argument form of erase
 */
bool erase_test( )
{
    using namespace std;  //try this for something different.
    //build a list.
    list< int > lst;
    for( int i = 1; i <= 20; ++i ) lst.push_back( i );
    list< int >::iterator it = find( lst.begin( ), lst.end( ), 11 );
    //do the deed
    lst.erase( lst.begin( ), it );
    if( INSANE( lst ) || lst.size( ) != 10 ) FAIL
    it = lst.begin( );
    for( int i = 1; i <= 10; ++i ) {
        if( *it != i + 10 ) FAIL
        ++it;
    }
    //erase everything
    lst.erase( lst.begin( ), lst.end( ) );
    if( INSANE( lst ) || lst.size( ) != 0 ) FAIL
    return( true );
}

/* ------------------------------------------------------------------
 * swap_test
 * test list swapping method
 */
bool swap_test( )
{
    std::list< int > lst_1, lst_2;
    lst_1.push_back( 1 );
    //try a swap and then check the result
    lst_1.swap( lst_2 );
    if( INSANE( lst_1 )    || INSANE( lst_2 ) ||
        lst_1.size( ) != 0 || lst_2.size( ) != 1) FAIL
    if( lst_2.front( ) != 1 ) FAIL
    //add some things to lst_1 and swap again
    lst_1.push_back( 10 );
    lst_1.push_back( 11 );
    lst_1.swap( lst_2 );
    if( INSANE( lst_1 )    || INSANE( lst_2 ) ||
        lst_1.size( ) != 1 || lst_2.size( ) != 2) FAIL
    if( lst_1.front( ) != 1 ) FAIL
    if( lst_2.front( ) != 10 ) FAIL
    lst_2.pop_front( );
    if( lst_2.front( ) != 11 ) FAIL
    return( true );
}

/* ------------------------------------------------------------------
 * remove_test( )
 * test the remove methods
 */
bool remove_test( )
{
    typedef std::list< int > l_t;
    l_t l;
    //prepare the list
    l.push_back( 0 );
    for( int i = 1; i <= 10; i++ ) {
        l.push_back( i );
    }
    l.push_back( 0 );
    l.push_back( 0 );
    for( int i = 11; i <= 20; i++ ) {
        l.push_back( i );
    }
    l.push_back( 0 );
    //do the deed
    l.remove( 0 );
    //did it work?
    if( INSANE( l ) || l.size( ) != 20 ) FAIL
    l_t::iterator it( l.begin( ) );
    for( int i = 1; i <= 20; i++ ) {
        if( *it != i ) FAIL
        ++it;
    }
    return( true );
}

/* ------------------------------------------------------------------
 * iterator_test( )
 * Test the iterator functionality
 */
bool iterator_test( )
{
    typedef std::list< int > l_t;
    l_t l;
    l_t::iterator it;
    l_t::const_iterator cit;
    for( int i = 0; i < 20; i++ ){
        l.push_back( i );
    }
    //test increment and dereferencing
    it = l.begin();
    int ans = *it;
    for( int i = 0; i < 20 ; i++ ){
        if( INSANE( l ) || ans != i || *it != i ) FAIL
        if( i%2 ) ans = (*(it++)) + 1;
        else ans = *(++it);
    }
    //and again with const iterator
    cit = l.begin();
    ans = *cit;
    for( int i = 0; i < 20 ; i++ ){
        if( INSANE( l ) || ans != i || *cit != i ) FAIL
        if( i%2 ) ans = *(cit++) + 1;
        else ans = *(++cit);
    }
    //test decrement
    it = l.end();
    for( int i = 19; i > 0 ; i-- ){
        int ans;
        if( i%2 ) ans = *(--it);
        else ans = *(it--) - 1;
        if( INSANE( l ) || ans != i || *it != i ) FAIL
    }
    //and again with const iterator
    cit = l.end();
    for( int i = 19; i > 0 ; i-- ){
        int ans;
        if( i%2 ) ans = *(--cit);
        else ans = *(cit--) - 1;
        if( INSANE( l ) || ans != i || *cit != i ) FAIL
    }
    
    return( true );
}

/* ------------------------------------------------------------------
 * reverse_iterator_test
 * make sure reverse iterators do something reasonable
 */
bool reverse_iterator_test( )
{
    std::list< int > lst;
    for( int i = 1; i <= 10; ++i ) lst.push_back( i );

    std::list< int >::reverse_iterator rit = lst.rbegin( );
    for( int i = 10; i >= 1; --i ) {
      if( *rit != i ) FAIL
      ++rit;
    }
    if( rit != lst.rend( ) ) FAIL
    return( true );
}

/* ------------------------------------------------------------------
 * copy_test
 * test copy constructor
 */
bool copy_test()
{
    std::list<int> lst1;
    for( int i = 0; i < 20; i++ ){
        lst1.push_front( -i );
    }
    std::list<int> lst2(lst1);
    if( INSANE( lst1 ) || lst1.size() != 20 ) FAIL
    if( INSANE( lst2 ) || lst2.size() != 20 ) FAIL
    for( int i = 0; i < 20; i++ ){
        if( lst2.back() != -i ) FAIL
        lst2.pop_back();
    }
    return( true );
}

/* ------------------------------------------------------------------
 * allocator_test
 * test stateful allocators and exception handling
 */
bool allocator_test( )
{
    typedef std::list< int, LowMemAllocator<int> > list_t;
    LowMemAllocator<int> mem(100);
    mem.SetTripOnAlloc();
    list_t lst( mem );
    bool thrown = false;
    
    //LowMemAllocator is set to trip after 100 allocations
    try{
        for( int i=0; i<101; i++ ){
            lst.push_front( i );
        }
    }catch( std::bad_alloc const & ){
        mem = lst.get_allocator();
        if( mem.GetNumAllocs() != 101 ) FAIL       //should fail on 101st
        if( INSANE(lst) || lst.size() != 99 ) FAIL //one alloc for sentinel
        thrown = true;
    }
    if( !thrown ) FAIL  //exception should have been thrown
    
    lst.clear();
    mem.Reset(100);
    mem.SetTripOnConstruct();
    thrown = false;
    //LowMemAllocator is set to trip after 100 allocations
    try{
        for( int i=0; i<101; i++ ){
            lst.push_back( i );
        }
    }catch( std::bad_alloc const & ){
        mem = lst.get_allocator();
        if( mem.GetNumConstructs() != 101 ) FAIL
        //should have cleaned up last one and left only 100 allocated items
        if( mem.GetNumAllocs() != 101 || mem.GetNumDeallocs() != 1 ) FAIL    
        if( INSANE(lst) || lst.size() != 100 ) FAIL
        thrown = true;
    }
    if( !thrown ) FAIL  //exception should have been thrown
    //if container didn't deal with the exception and clean up the allocated 
    //memory then the leak detector will also trip later
    
    lst.clear();
    mem.Reset(100);
    thrown = false;
    for( int i = 0; i < 70; i++ ){
        lst.push_back( i );
    }
    //now reset the allocator so it trips at a lower threshold
    //and test the copy mechanism works right
    mem.Reset( 50 );
    mem.SetTripOnAlloc();
    try{
        list_t lst2( lst );
    }catch( std::bad_alloc ){
        if( mem.GetNumConstructs() != 49 ) FAIL  //sentinel not constructed
        if( mem.GetNumAllocs()     != 51 ) FAIL
        if( mem.GetNumDestroys()   != 49 ) FAIL  //sentinel not destroyed
        if( mem.GetNumDeallocs()   != 50 ) FAIL
        if( INSANE( lst ) || lst.size() != 70 ) FAIL
        thrown = true;
    }
    if( !thrown ) FAIL
    
    return( true );
}

int main( )
{
    int rc = 0;
    int original_count = heap_count( );
    //heap_dump();

    try {
        //if( !construct_test( )        || !heap_ok( "t01" ) ) rc = 1;
        if( !access_test( )           || !heap_ok( "t02" ) ) rc = 1;
        //if( !string_test( )           || !heap_ok( "t03" ) ) rc = 1;
        //if( !torture_test( )          || !heap_ok( "t04" ) ) rc = 1;
        if( !assign_test( )           || !heap_ok( "t05" ) ) rc = 1;
        if( !clear_test( )            || !heap_ok( "t06" ) ) rc = 1;
        if( !erase_test( )            || !heap_ok( "t07" ) ) rc = 1;
        if( !swap_test( )             || !heap_ok( "t08" ) ) rc = 1;
        if( !remove_test( )           || !heap_ok( "t09" ) ) rc = 1;
        if( !iterator_test( )         || !heap_ok( "t10" ) ) rc = 1;
        if( !reverse_iterator_test( ) || !heap_ok( "t11" ) ) rc = 1;
        if( !copy_test( )             || !heap_ok( "t12" ) ) rc = 1;
        if( !allocator_test( )        || !heap_ok( "t13" ) ) rc = 1;
    }
    catch( ... ) {
        std::cout << "Unexpected exception of unexpected type.\n";
        rc = 1;
    }

    if( heap_count( ) != original_count ) {
        std::cout << "Possible memory leak!\n";
        heap_dump();
        rc = 1;
    }
    return( rc );
}
