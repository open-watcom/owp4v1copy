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
* Description: This file contains the functional tests for smart pointers.
*
****************************************************************************/

#include <memory>
#include <iostream>
#include <string>

#include "sanity.cpp"

class A { 
public:
    int tag;
    A( ) : tag( 0 ) { }
    virtual ~A( ) { }
};

class B : public A {
public:
    B( bool& _deleted ) : deleted( _deleted ) { }
    ~B( ) { deleted = true; }
private:
    bool& deleted;
};

class D { 
public:
    template< class T >
    void operator( ) ( T *p ) const
        { delete p; }
};

bool construction_destruction_test( )
{
    // shared_ptr( )
    {
        std::shared_ptr< A > s1;
        if( !(s1.use_count( ) == 0 && s1.get( ) == 0) ) FAIL;
    }

    // template< class Y > explicit shared_ptr( Y *p )
    {
        bool deleted = false;
        B *p = new B( deleted );
        {
            std::shared_ptr< A > s( p );
            if( !(s.use_count( ) == 1 && s.get( ) == p) ) FAIL;
        }
        if( !deleted ) FAIL;
    }

    // template< class Y, class D > shared_ptr( Y *p, D d )
    {
        bool deleted = false;
        B *p = new B( deleted );
        {
            std::shared_ptr< A > s( p, D( ) );
            if( !(s.use_count( ) == 1 && s.get( ) == p) ) FAIL;
        }
        if( !deleted ) FAIL;
    }

    // shared_ptr( shared_ptr const &r )
    {
        std::shared_ptr< A > s1;
        std::shared_ptr< A > s2 = s1;
        if( !(s2.use_count( ) == 0 && s2.get( ) == 0) ) FAIL;

        std::shared_ptr< A > s3( new A );
        std::shared_ptr< A > s4 = s3;
        if( !(s3.use_count( ) == s4.use_count( ) && s3.get( ) == s4.get( )) ) FAIL;
        if( s3.use_count( ) != 2 ) FAIL;
    }

    // template< class Y > shared_ptr( shared_ptr< Y > const &r )
    {
        std::shared_ptr< B > s1;
        std::shared_ptr< A > s2( s1 );
        if( !(s2.use_count( ) == 0 && s2.get( ) == 0) ) FAIL;

        bool deleted = false;
        {
            std::shared_ptr< B > s3( new B( deleted ) );
            {
                std::shared_ptr< A > s4( s3 );
                if( !(s3.use_count( ) == s4.use_count( ) && s3.get( ) == s4.get( )) ) FAIL;
                if( s3.use_count( ) != 2 ) FAIL;
            }
            if( s3.use_count( ) != 1 ) FAIL;
            if( deleted ) FAIL;
        }
        if( !deleted ) FAIL;
    }

    // template< class Y > explicit shared_ptr( weak_ptr< Y > const &r )
    {
        bool deleted = false;
        std::weak_ptr< B > w1;
        {
            std::shared_ptr< B > s1( new B( deleted ) );
            w1 = s1;
            std::shared_ptr< A > s2( w1 );
            if( !(s2.use_count( ) == w1.use_count( ) && s2.get( ) == s1.get( )) ) FAIL;
            if( s2.use_count( ) != 2 ) FAIL;
        }
        if( !deleted ) FAIL;

        bool caught_bad_weak_ptr = false;
        try {
            std::shared_ptr< A > s3( w1 );
        }
        catch( std::bad_weak_ptr& e ) {
            caught_bad_weak_ptr = true;
            if( std::string( e.what( ) ) != "bad_weak_ptr" ) FAIL; 
        }
        if( !caught_bad_weak_ptr ) FAIL;
    }
    
    // template< class Y > explicit shared_ptr( auto_ptr< Y > &r ) : ptr( r.get( ) )
    {
        bool deleted = false;
        std::auto_ptr< B > a1( new B( deleted ) );
        {
            A *a1_ptr = a1.get( );
            std::shared_ptr< A > s1( a1 );
            if( s1.get( ) != a1_ptr ) FAIL;
            if( !(s1.use_count( ) == 1 && a1.get( ) == 0) ) FAIL;
            if( deleted ) FAIL;
        }
        if( !deleted ) FAIL;
    }

    return( true );
}

bool assignment_test( )
{
    // shared_ptr& operator=( shared_ptr const& r )
    {
        bool deleted1 = false;
        bool deleted2 = false;
        {
            std::shared_ptr< B > s1( new B( deleted1 ) );
            std::shared_ptr< B > s2( new B( deleted2 ) );
            s1 = s2;
            if( !deleted1 ) FAIL;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;
            s1 = s2;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;
            s1 = s1;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;    
        }
        if( !deleted2 ) FAIL;
    }

    // template< class Y > shared_ptr& operator=( shared_ptr< Y > const &r )
    {
        bool deleted1 = false;
        bool deleted2 = false;
        {
            std::shared_ptr< A > s1( new B( deleted1 ) );
            std::shared_ptr< B > s2( new B( deleted2 ) );
            s1 = s2;
            if( !deleted1 ) FAIL;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;
            s1 = s2;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;
            s1 = s1;
            if( s1.get( ) != s2.get( ) || s1.use_count( ) != 2 ) FAIL;
        }
        if( !deleted2 ) FAIL;
    }

    // template< class Y > shared_ptr& operator=( auto_ptr< Y > &r )
    {
        bool deleted1 = false;
        bool deleted2 = false;
        {
            std::shared_ptr< A > s1( new B( deleted1 ) );
            std::auto_ptr< B > a1( new B( deleted2 ) );
            s1 = a1;
            if( !deleted1 ) FAIL;
            if( deleted2 ) FAIL;
            if( s1.use_count( ) != 1 ) FAIL;
            if( a1.get( ) ) FAIL;
        }
        if( !deleted2 ) FAIL;
    }
    
    return( true );
}

bool modifiers_test( )
{
    // void swap( shared_ptr &r )
    {
        std::shared_ptr< A > s1( new A );
        std::shared_ptr< A > s1_1 = s1; // to increase use_count
        A *ptr1 = s1.get( );
        long use_count1 = s1.use_count( );

        bool deleted = false;
        std::shared_ptr< A > s2( new B( deleted ) );
        A *ptr2 = s2.get( );
        long use_count2 = s2.use_count( );

        s1.swap( s2 );
        if( deleted ) FAIL;
        if( !(s1.get( ) == ptr2 && s1.use_count( ) == use_count2 && 
            s2.get( ) == ptr1 && s2.use_count( ) == use_count1) ) FAIL;
    }

    // void reset( )
    {
        bool deleted = false;
        std::shared_ptr< B > s1( new B( deleted ) );
        s1.reset( );
        if( !(s1.get( ) == 0 && s1.use_count( ) == 0 && deleted) ) FAIL;
    }

    // template< class Y > void reset( Y *p )
    {
        bool deleted1 = false;
        bool deleted2 = false;
        {
            std::shared_ptr< A > s1( new B( deleted1 ) );
            B *b = new B( deleted2 );
            s1.reset( b );
            if( !deleted1 || deleted2 ) FAIL;
            if( !(s1.get( ) == b && s1.use_count( ) == 1) ) FAIL;
        }
        if( !deleted2 ) FAIL;
    }

    // template< class Y, class D > void reset( Y *p, D d )
    {
        bool deleted1 = false;
        bool deleted2 = false;
        {
            std::shared_ptr< A > s1( new B( deleted1 ) );
            B *b = new B( deleted2 );
            s1.reset( b, D( ) );
            if( !deleted1 || deleted2 ) FAIL;
            if( !(s1.get( ) == b && s1.use_count( ) == 1) ) FAIL;
        }
        if( !deleted2 ) FAIL;
    }

    return( true );
}

bool observers_test( )
{
    // T *get( ) const                    
    {
        A *a = new A;
        std::shared_ptr< A > s1( a );
        std::shared_ptr< A > s2;
        if( !(s1.get( ) == a && s2.get( ) == 0) ) FAIL;
    }

    // T &operator*( ) const
    {
        A *a = new A;
        std::shared_ptr< A > s1( a );
        A& b = *s1;
        b.tag = 5;
        if( !(a->tag == 5) ) FAIL;
    }

    // T *operator->( ) const
    {
        A *a = new A;
        std::shared_ptr< A > s1( a );
        a->tag = 5;
        if( s1->tag != 5 ) FAIL;
    }

    // long use_count( ) const
    {
        std::shared_ptr< A > s1;
        if( s1.use_count( ) != 0 ) FAIL;
        std::shared_ptr< A> s2( new A );
        if( s2.use_count( ) != 1 ) FAIL;
        s1 = s2;
        if( s2.use_count( ) != 2 || s1.use_count( ) != 2 ) FAIL;
    }

    // bool unique( ) const
    {
        std::shared_ptr< A > s1;
        if( s1.unique( ) ) FAIL;
        std::shared_ptr< A> s2( new A );
        if( !s2.unique( ) ) FAIL;
        s1 = s2;
        if( s1.unique( ) || s2.unique( ) ) FAIL;
    }

    // operator bool( ) const
    {
        std::shared_ptr< A > s1;
        if( s1 ) FAIL;
        std::shared_ptr< A> s2( new A );
        if( !s2 ) FAIL;
    }
    
    return( true );
}

int main( )
{
    int rc = 0;
    int original_count = heap_count( );

    try {
        if( !construction_destruction_test( )       || !heap_ok( "t01" ) ) rc = 1;
        if( !assignment_test( )                     || !heap_ok( "t02" ) ) rc = 1;
        if( !modifiers_test( )                      || !heap_ok( "t03" ) ) rc = 1;
        if( !observers_test( )                      || !heap_ok( "t04" ) ) rc = 1;
        // to be continued...
    }
    catch( ... ) {
        std::cout << "Unexpected exception of unexpected type.\n";
        rc = 1;
    }

    if( heap_count( ) != original_count ) {
        std::cout << "Possible memory leak!\n";
        rc = 1;
    }

    if( rc == 0 ) {
        std::cout << "success\n";
    }

    return( rc );
}
