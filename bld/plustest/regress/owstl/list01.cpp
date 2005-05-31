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
* Description:  Use this file as a template for creating new tests.
*
****************************************************************************/

#include <iostream>
#include <list>
#include "sanity.cpp"

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
 * 
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


int main( )
{
    int rc = 0;
    int original_count = heap_count( );

    try {
        //if( !construct_test( )  || !heap_ok( "t1" ) ) rc = 1;
        if( !access_test( )     || !heap_ok( "t2" ) ) rc = 1;
        //if( !string_test( )     || !heap_ok( "t3" ) ) rc = 1;
        //if( !torture_test( )    || !heap_ok( "t4" ) ) rc = 1;
        if( !clear_test( )      || !heap_ok( "t5" ) ) rc = 1;
        //if( !iterator_test( )   || !heap_ok( "t6" ) ) rc = 1;
        //if( !copy_test( )       || !heap_ok( "t6" ) ) rc = 1;
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
