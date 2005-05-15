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
* Description:  This program exercises the facilities in <memory>
*
****************************************************************************/

#include <iostream>
#include <memory>

#include "sanity.cpp"

typedef std::allocator< int > int_allocator;
typedef std::allocator< int >::rebind< long >::other lng_allocator;

bool rebind_test( )
{
  int_allocator ia;
  lng_allocator la;

  int_allocator::pointer p1 = ia.allocate( 1 );
  lng_allocator::pointer p2 = la.allocate( 1 );
  if( p1 == 0 ) FAIL
  if( p2 == 0 ) FAIL
  if( static_cast< void *>( p1 ) == static_cast< void* >( p2 ) ) FAIL
  ia.deallocate( p1, 1 );
  la.deallocate( p2, 1 );
  return( true );
}


int main( )
{
  int rc = 0;
  int original_count = heap_count( );

  try {
    if( !rebind_test( )  || !heap_ok( "t01" ) ) rc = 1;
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
