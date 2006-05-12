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
* Description: Benchmark program that exercises std::sort and compares
*              its behavior with the C library's qsort().
*
****************************************************************************/

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#define SORT       std::sort  // Name of the STL-like sorting function.
#define N          1000000    // Size of array to sort.
#define TEST_COUNT 20         // Number of times array is sorted.

// Make a pseudo-random array of integers.
void make_random( int *p1, int *p2 )
{
  srand( 0 );
  while( p1 != p2 ) {
    *p1++ = rand();
  }
}

// Make a sorted array of integers.
void make_sorted( int *p1, int *p2 )
{
  int value = 0;
  while( p1 != p2 ) {
    *p1++ = value++;
  }
}

// Make a pseudo-random array of std::strings.
void make_random_strings( std::string *p1, std::string *p2 )
{
  srand( 0 );
  while( p1 != p2 ) {
    for( int i = 0; i < 50; ++i ) {
      p1->push_back( static_cast<char>( 'a' + ( rand() % 26 ) ) );
    }
    ++p1;
  }
}

// Comparison function for qsort( ).
int int_compare( const void *left, const void *right )
{
  const int *x = ( const int * )left;
  const int *y = ( const int * )right;
  if( *x < *y ) return -1;
  if( *x > *y ) return +1;
  return 0;
}

// Use the C++ library std::sort (or something similar) to sort array of Ts.
template< class T >
void cpp_sort(T *working, T *holding, int size, char *caption)
{
  clock_t interval = clock();

  for(int i = 0; i < TEST_COUNT; ++i) {
    memcpy( working, holding, size * sizeof(int) );
    SORT( working, working + size );
  }
  interval = clock() - interval;
  std::cout << "std::sort => " << caption << ": "
            << interval/static_cast<double>( CLOCKS_PER_SEC ) << "\n";
}

// Use the C++ library std::sort (or something similar) to sort strings.
void cpp_string_sort( int size, char *caption)
{
  std::string *working = new std::string[ size ];
  make_random_strings( working, working + size );

  clock_t interval = clock();
  SORT( working, working + size );
  interval = clock() - interval;
  std::cout << "std::sort => " << caption << ": "
            << TEST_COUNT * ( interval/static_cast<double>( CLOCKS_PER_SEC ) ) << "\n";
  
  delete [] working;
}

// Use C library qsort() to sort arrays of integers.
void c_sort(int *working, int *holding, int size, char *caption)
{
  clock_t interval = clock();

  for(int i = 0; i < TEST_COUNT; ++i) {
    memcpy( working, holding, size * sizeof(int) );
    qsort( working, size, sizeof(int), int_compare );
  }
  interval = clock( ) - interval;
  std::cout << "qsort     => " << caption << ": "
            << interval/static_cast<double>( CLOCKS_PER_SEC ) << "\n";
}

int main( )
{
  int *holding = new int[N];
  int *working = new int[N];

  make_random( holding, holding + N );
  cpp_sort( working, holding, N, "random" );
  c_sort( working, holding, N, "random" );

  make_sorted( holding, holding + N );
  cpp_sort( working, holding, N, "sorted" );
  c_sort( working, holding, N, "sorted" );

  cpp_string_sort( N, "string" );

  delete [] working;
  delete [] holding;
  return 0;
}
