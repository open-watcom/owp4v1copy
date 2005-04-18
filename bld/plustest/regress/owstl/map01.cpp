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
* Description:  Tests for std::map
*
****************************************************************************/

#include <iostream>
#include <map>
#include <string>
#include "sanity.cpp"
#include <cstdlib>
#include "allocxtr.hpp"

/* ------------------------------------------------------------------
 * To do:
 *      * user allocator - low memory allocator
 *      * user comparator
 *      * other functions not yet implemented in map
 */


/* ------------------------------------------------------------------
 * construct_test( )
 * Construct different maps in different ways.
 */
bool construct_test( )
{
    bool pass = true;
    typedef std::map< int, int > mapii_t;
    mapii_t m1;
    std::map< char, char const * > m2;
    std::map< char*, double > m3;
    mapii_t *m4 = new mapii_t();
    
    if( INSANE(m1) || m1.size() || !m1.empty() ){
        std::cout << "construct fail 0001\n";
        pass = false;
    }
    if( INSANE(m2) || m2.size() || !m2.empty() ){
        std::cout << "construct fail 0002\n";
        pass = false;
    }
    if( INSANE(m3) || m3.size() || !m3.empty() ){
        std::cout << "construct fail 0003\n";
        pass = false;
    }
    if( INSANE(*m4) || m4->size() || !m4->empty() ){
        std::cout << "construct fail 0004\n";
        pass = false;
    }
    delete m4;
    
    return( pass );
}

/* ------------------------------------------------------------------
 * access_test( )
 * check the tree is working with different inserts / finds / deletes /
 * repeat inserts
 */
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
    std::map< int, int > m1;
    
    //insert
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        std::pair< std::map< int, int >::iterator, bool > ans;
        ans = m1.insert( std::map< int, int >::value_type( num[i], num[i]*num[i] ) );
        if( INSANE( m1 ) || m1.size() != (i+1) || m1.empty() || !ans.second){
            std::cout << "access failure 0001 insert " << i << "\n";
            pass = false;
        }
    }
    //find inserted
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        std::map< int, int >::iterator it = m1.find( num[i] );
        if( INSANE( m1 ) || m1.size() != (sizeof(num) / sizeof(int)) ||
            m1.empty() || it == m1.end() ){
            std::cout << "access failure 0002 find " << i << "\n";
            pass = false;
        }
    }
    //find not inserted
    for( int i = 0; i < (sizeof(notnum) / sizeof(int)); i++){
        std::map< int, int >::iterator it = m1.find( notnum[i] );
        if( INSANE( m1 ) || m1.size() != (sizeof(num) / sizeof(int)) ||
            m1.empty() || it != m1.end() ){
            std::cout << "access failure 0003 find " << i << "\n";
            pass = false;
        }
    }
    //insert again
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        std::pair< std::map< int, int >::iterator, bool > ans;
        ans = m1.insert( std::map< int, int >::value_type( num[i], -1 ) );
        if( INSANE( m1 ) || m1.size() != (sizeof(num) / sizeof(int)) ||
            m1.empty() || ans.second ){
            std::cout << "access failure 0004 insert again " << i << "\n";
            pass = false;
        }
    }
    //use subscript to find inserted again: shouldn't have been over-written
    for( int i = 0; i < (sizeof(num) / sizeof(int)); i++){
        if( INSANE( m1 ) || m1.size() != (sizeof(num) / sizeof(int)) ||
            m1.empty() || m1[num[i]] != num[i]*num[i] ){
            std::cout << "access failure 0005 subscript find " << i << "\n";
            pass = false;
        }
    }
    //use subscript to insert
    size = sizeof(num) / sizeof(int);
    for( int i = 0; i < totsize; i++){
        m1[i] = -i;
        for( int j = 0; j < (sizeof(notnum) / sizeof(int)); j++)
            if( notnum[j] == i ) ++size;
        if( INSANE( m1 ) || m1.size() != size || m1.empty() ){
            std::cout << "access failure 0006 subscript insert " << i << "\n";
            pass = false;
        }
    }
    //use subscript to find: should have been over-written this time
    for( int i = 0; i < totsize; i++){
        if( INSANE( m1 ) || m1.size() != totsize || m1.empty() ||
            m1[i] != -i ){
            std::cout << "access failure 0007 subscript find all " << i << "\n";
            pass = false;
        }
    }
    //delete the elements
    size = totsize;
    for( int i = 0; i < (sizeof(delnum) / sizeof(int)); i++){
        m1.erase( delnum[i] );
        --size;
        if( INSANE( m1 ) || m1.size() != size ){
            std::cout << "access failure 0008 erase " << i << "\n";
            pass = false;
        }
    }
    if( !m1.empty() ){
        std::cout << "access failure 0010\n";
        pass = false;
    }
    //try finding the elements now they are deleted
    size = totsize;
    for( int i = 0; i < totsize; i++){
        std::map< int, int >::iterator it = m1.find( i );
        if( INSANE( m1 ) || m1.size() || !m1.empty() || it != m1.end() ){
            std::cout << "access failure 0011 find after erase " << i << "\n";
            pass = false;
        }
    }
    return( pass );
}

/* ------------------------------------------------------------------
 * string_test( )
 * some quick tests with a more complicated type
 */
bool string_test( )
{
    bool pass = true;
    using namespace std;
    typedef map< string, __int64 > m_t;
    string eejit = "Dan";
    string place = "here";
    m_t &m1 = *(new m_t);

    if( INSANE(m1) || !m1.empty() ){
        std::cout << "map<string> failure 0001\n"; pass = false;
    }
    m1.insert( m_t::value_type(eejit,0x123456789abcdefUI64) );
    if( INSANE(m1) || m1.empty() || m1.size() != 1){
        std::cout << "map<string> failure 0002\n"; pass = false;
    }
    m1.insert( m_t::value_type("was",0x123456789abcdef0UI64) );
    if( INSANE(m1) || m1.empty() || m1.size() != 2){
        std::cout << "map<string> failure 0003\n"; pass = false;
    }
    m1.insert( m_t::value_type(place,0x5a5a5a5a5a5a5a5aUI64) );
    if( INSANE(m1) || m1.empty() || m1.size() != 3){
        std::cout << "map<string> failure 0004\n"; pass = false;
    }
    __int64 sum = (*m1.find( "Dan" )).second +
                  (*m1.find( "was" )).second + m1["here"];
    if( INSANE(m1) || m1.empty() || m1.size() != 3 ||
        sum != 0x6db1f63a7ec30739ui64){
        std::cout << "map<string> failure 0005\n"; pass = false;
    }
    m1.erase( m1.find("Dan") );
    m1.erase( m1.find("was") );
    m1.erase( m1.find("here") );
    if( INSANE(m1) || !m1.empty() || m1.size() ){
        std::cout << "map<string> failure 0006\n"; pass = false;
    }
    delete &m1;
    return( pass );
}

/* ------------------------------------------------------------------
 * torturer( int )
 * Run lots of inserts/deletes so statistically good chance of 
 * finding a problem if haven't covered all possibilities in access test
 */
bool torturer( int maxsize )
{
    using namespace std;
    typedef map< int, int > m_t;
    m_t m;
    bool growing = true;
    int p = 0.9 * RAND_MAX;
    int size = 0;
    int c = 0;
    
    while( growing || m.size() > 0 ){
        if( m.size() > maxsize ){
            growing = false;
            p = 0.1 * RAND_MAX;
        }
        if( (rand() < p) || (size == 0) ){
            int v = rand() + RAND_MAX * rand();
            while( !m.insert( m_t::value_type(v, v) ).second ) v++;
            ++size;
        }else{
            int n = (double)rand() / (double)RAND_MAX * (double)(m.size() - 1);
            m_t::iterator it = m.begin();
            if ( n >= m.size() ) { cout<< "internal err 1"<<n<<" "<<maxsize<<" "<<c<<"\n"; return ( false ); }
            advance(it, n);
            m.erase( it );
            --size;
        }
        if( !heap_ok( "x" ) ) { cout<<"heap err\n"; return false;}
        if( INSANE(m) || m.size() != size ){
        std::cout << "torture failure 0001 count " << c << 
                     " maxsize " << maxsize << " error " << m.mError <<"\n";
        return( false );
        }
        ++c;
    }
    if( INSANE(m) || m.size() || !m.empty() ){
        std::cout << "torture failure 0002\n" ;
        return( false );
    }
    return( true );
}
/* ------------------------------------------------------------------
 * torture_test( )
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
    int const biggest = 2048; //65536
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
    typedef std::map< int, int > m_t;
    m_t m1;
    m_t::iterator it;
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
