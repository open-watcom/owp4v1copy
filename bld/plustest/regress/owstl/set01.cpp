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
* Description:  Tests for std::set
*
****************************************************************************/

#include <iostream>
#include <set>
#include <string>
#include "sanity.cpp"
#include <cstdlib>
#include "allocxtr.hpp"

bool construct_test( )
{
    bool pass = true;
    typedef std::set< int > seti_t;
    seti_t s1;
    std::set< char > s2;                // to do: construct with different alloc/comp
    std::set< char* > s3;
    seti_t *s4 = new seti_t();
    
    if( INSANE(s1) || s1.size() || !s1.empty() ){
        std::cout << "construct fail 0001\n";
        pass = false;
    }
    if( INSANE(s2) || s2.size() || !s2.empty() ){
        std::cout << "construct fail 0002\n";
        pass = false;
    }
    if( INSANE(s3) || s3.size() || !s3.empty() ){
        std::cout << "construct fail 0003\n";
        pass = false;
    }
    if( INSANE(*s4) || s4->size() || !s4->empty() ){
        std::cout << "construct fail 0004\n";
        pass = false;
    }
    delete s4;
    
    return( pass );
}

bool access_test( )
{
    bool pass = true;

    // todo... choose an array of numbers that excercise all the different
    // interals of insert and delete (eg different tree transforms)
    int num[] = {10,5,3,4,6,2,7,8,11,12,14,13,19,17,16}; 
    int notnum[] = {0,1,9,15,18}; //numbers not in num[]
    int delnum[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    int totsize = (sizeof(num) + sizeof(notnum)) / sizeof(int);
    int size;
    std::set< int > s1;
    
    //insert
    for( int i = 0; i < (sizeof( num ) / sizeof( int )); i++ ){
        std::pair< std::set< int >::iterator, bool > ans;
        ans = s1.insert( num[i] );
        if( INSANE( s1 ) || s1.size() != (i+1) || s1.empty() || !ans.second ){
            std::cout << "access failure 0001 insert " << i << "\n";
            pass = false;
        }
    }
    //find inserted
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        std::set< int >::iterator it = s1.find( num[i] );
        if( INSANE( s1 ) || s1.size() != (sizeof( num ) / sizeof( int )) ||
            s1.empty() || it == s1.end() ){
            std::cout << "access failure 0002 find " << i << "\n";
            pass = false;
        }
    }
    //find not inserted
    for( int i = 0; i < (sizeof(notnum) / sizeof(int)); i++){
        std::set< int >::iterator it = s1.find( notnum[i] );
        if( INSANE( s1 ) || s1.size() != (sizeof( num ) / sizeof( int )) ||
            s1.empty() || it != s1.end() ){
            std::cout << "access failure 0003 find " << i << "\n";
            pass = false;
        }
    }
    //insert again
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        std::pair< std::set< int >::iterator, bool > ans;
        ans = s1.insert( num[i] );
        if( INSANE( s1 ) || s1.size() != (sizeof(num) / sizeof(int)) ||
            s1.empty() || ans.second ){
            std::cout << "access failure 0004 insert again " << i << "\n";
            pass = false;
        }
    }
    //insert all
    size = sizeof( num ) / sizeof( int );
    for( int i = 0; i < totsize; i++){
        std::pair< std::set< int >::iterator, bool > ans;
        ans = s1.insert( i );
        bool should_have_inserted = false;
        for( int j = 0; j < (sizeof( notnum ) / sizeof( int )); j++)
            if( notnum[j] == i ) should_have_inserted = true;
        if( should_have_inserted ){
            ++size;
            if( !ans.second ) pass = false;
        }else{ // shouldn't have inserted
            if( ans.second ) pass = false;
        }
        if( !pass || INSANE( s1 ) || s1.size() != size || s1.empty() ){
            std::cout << "access failure 0005 insert all " << i << "\n";
            pass = false;
        }
    }
    //delete the elements
    size = totsize;
    for( int i = 0; i < (sizeof( delnum ) / sizeof( int )); i++){
        s1.erase( delnum[i] );
        --size;
        if( INSANE( s1 ) || s1.size() != size ){
            std::cout << "access failure 0006 erase " << i << "\n";
            pass = false;
        }
    }
    if( !s1.empty() ){
        std::cout << "access failure 0007\n";
        pass = false;
    }
    //try finding the elements now they are deleted
    size = totsize;
    for( int i = 0; i < totsize; i++){
        std::set< int >::iterator it = s1.find( i );
        if( INSANE( s1 ) || s1.size() || !s1.empty() || it != s1.end() ){
            std::cout << "access failure 0008 find after erase " << i << "\n";
            pass = false;
        }
    }

    return( pass );
}

/* ------------------------------------------------------------------
 * some quick tests with a more complicated type
 */
bool string_test( )
{
    bool pass = true;
    using namespace std;
    typedef set< string > s_t;
    string eejit = "Dan";
    string place = "here";
    s_t &s1 = *(new s_t);

    if( INSANE(s1) || !s1.empty() ){
        std::cout << "set<string> failure 0001\n"; pass = false;
    }
    s1.insert( eejit );
    if( INSANE(s1) || s1.empty() || s1.size() != 1){
        std::cout << "set<string> failure 0002\n"; pass = false;
    }
    s1.insert( "here" );
    if( INSANE(s1) || s1.empty() || s1.size() != 2){
        std::cout << "set<string> failure 0003\n"; pass = false;
    }
    s1.insert( place );
    if( INSANE(s1) || s1.empty() || s1.size() != 2){
        std::cout << "set<string> failure 0004\n"; pass = false;
    }
    if( s1.find( "Dan" ) == s1.end() || s1.find( "here" ) == s1.end() ||
        *s1.find( eejit ) != "Dan" || *s1.find( place ) != place ||
        INSANE(s1) || s1.empty() || s1.size() != 2 ){
        std::cout << "set<string> failure 0005\n"; pass = false;
    }
    s1.erase( s1.find("Dan") );
    s1.erase( "was" );
    s1.erase( "here" );
    if( INSANE(s1) || !s1.empty() || s1.size() ){
        std::cout << "set<string> failure 0006\n"; pass = false;
    }
    return( pass );
}

/* ------------------------------------------------------------------
 * Run lots of inserts/deletes so statistically good chance of 
 * finding a problem if haven't covered all possibilities in access test
 */
bool torturer( int maxsize )
{
    using namespace std;
    typedef set< int > m_t;
    m_t s;
    bool growing = true;
    int p = 0.9 * RAND_MAX;
    int size = 0;
    int c = 0;
    
    while( growing || s.size() > 0 ){
        if( s.size() > maxsize ){
            growing = false;
            p = 0.1 * RAND_MAX;
        }
        if( (rand() < p) || (size == 0) ){
            int v = rand() + RAND_MAX * rand();
            while( !s.insert( v ).second ) v++;
            ++size;
        }else{
            int n = (double)rand() / (double)RAND_MAX * (double)(s.size() - 1);
            m_t::iterator it = s.begin();
            if ( n >= s.size() ) { cout<< "internal err 1 "<<n<<" "<<maxsize<<" "<<c<<"\n"; return ( false ); }
            advance(it, n);
            s.erase( it );
            --size;
        }
        if( !heap_ok( "x" ) ) { cout<<"heap err\n"; return false;}
        if( INSANE(s) || s.size() != size ){
        std::cout << "torture failure 0001 count " << c << 
                     " maxsize " << maxsize << " error " << s.mError <<"\n";
        return( false );
        }
        ++c;
    }
    if( INSANE(s) || s.size() || !s.empty() ){
        std::cout << "torture failure 0002\n" ;
        return( false );
    }
    return( true );
}
/* ------------------------------------------------------------------
 * _very_ rough timings just to give an idea of how long you have
 * to wait for the test to complete before suspecting it has bust
 * P4 3GHz debug build
 * biggest = 2048   3 sec
 * biggest = 4096   9 sec
 * biggest = 8192   37 sec
 * biggest = 16384  164 sec
 * ~~x^2 time because we run _Sane at every element as it is inserted and 
 * deleted and _Sane runs through every element currently in the tree.
 * Some actual benchmarks should probably be created in the appropriate place.
 */
bool torture_test( )
{
    // no point in this test really as the base RedBlackTree is 
    // tested under map
    int const biggest = 256; 
    for( int i = 1; i <= biggest; i *= 2 ){
        //std::cout<<i<<", ";
        if( !torturer( i ) ) return false;
    }
    return( true );
}

/* ------------------------------------------------------------------
 * iterator_test( )
 * Test the iterator functionality
 */
bool iterator_test( )
{
    typedef std::set< int > s_t;
    s_t s1;
    s_t::iterator it;
    //to do
    return( true );
}

int main( )
{
    int rc = 0;
    int original_count = heap_count( );

    try {
        if( !construct_test( )  || !heap_ok( "t1" ) ) rc = 1;
        if( !access_test( )     || !heap_ok( "t2" ) ) rc = 1;
        if( !string_test( )     || !heap_ok( "t3" ) ) rc = 1;
        if( !torture_test( )    || !heap_ok( "t4" ) ) rc = 1;
        if( !iterator_test( )   || !heap_ok( "t5" ) ) rc = 1;
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
