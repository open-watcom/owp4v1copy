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
* Description: Benchmark program that does a lot of std::string copying.
*
****************************************************************************/

#include <iostream>
#include <string>
#include "timer.h"

#define LONG_SIZE     1000    // Base size of a 'long' string.
#define LONGDUMP_SIZE 1000    // Number of strings in long string dump.
#define LONG_COUNT    10000   // Number of times long string loop executes.

#define SHRT_SIZE     3       // Same as above except for short strings.
#define SHRTDUMP_SIZE 10000
#define SHRT_COUNT    10000

std::string long_dump[LONGDUMP_SIZE];
std::string shrt_dump[SHRTDUMP_SIZE];

std::string long_f( std::string v )
{
  v.append( LONG_SIZE, 'y' );  // Cause reallocation.
  return( v );
}

std::string shrt_f( std::string v )
{
  v.append( SHRT_SIZE, 'y' );  // No reallocation.
  return( v );
}

int main( )
{
  TimerOn( );
  for( int i = 0; i < LONG_COUNT; ++i ) {
    std::string long_s( LONG_SIZE, 'x' );
    long_s = long_f( long_s );
    if( long_s.size( ) != 2*LONG_SIZE ) {
      std::cout << "Internal error!\n";
    }
    for( int j = 0; j < LONGDUMP_SIZE; ++j ) {
      long_dump[j] = long_s;
    }
  }
  TimerOff( );
  std::cout << "Average time per pass of long string loop: ";
  std::cout << ( TimerElapsed( )/LONG_COUNT ) * 1000 << " ms\n\n";

  TimerOn();
  for( int i = 0; i < SHRT_COUNT; ++i ) {
    std::string shrt_s( SHRT_SIZE, 'x' );
    shrt_s = shrt_f( shrt_s );
    if( shrt_s.size( ) != 2*SHRT_SIZE ) {
      std::cout << "Internal error!\n";
    }
    for( int j = 0; j < SHRTDUMP_SIZE; ++j ) {
      shrt_dump[j] = shrt_s;
    }
  }
  TimerOff();
  std::cout << "Average time per pass of short string loop: ";
  std::cout << ( TimerElapsed( )/SHRT_COUNT ) * 1000 << " ms\n\n";

  return( 0 );
}
