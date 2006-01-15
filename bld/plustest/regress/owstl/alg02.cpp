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
* Description:  This file tests sorting and searching algorithms.
****************************************************************************/

#include <algorithm>
#include <iostream>

bool heap_test( )
{
  bool rc = true;

  // Exercise heaps here.

  // Currently std::sort is implemented in terms of heaps so perhaps we
  // can get by without testing anything here for right now (obviously
  // this needs to be fixed at some point).

  return( rc );
}

//
// Test cases can contain no more than this many elements.
//
const int MAX_SIZE = 9;

//
// The following structure holds information about a single test case.
//
struct test_case {
  char *title;              // Something to print when reporting results.
  char  input[MAX_SIZE];    // The array to sort.
  char  expected[MAX_SIZE]; // The expected result.
  int   size;               // The number of elements in input[] I care about.
};


//
// The test cases.
//
struct test_case tests[] = {

  { "An empty sequence ................",
    { '@' },
    { '@' }, 0 },

  { "One element ......................", 
    { 'a' },
    { 'a' }, 1 },

  { "Two elements .....................",
    { 'b', 'a' },
    { 'a', 'b' }, 2 },

  { "Three elements ...................",
    { 'b', 'c', 'a' },
    { 'a', 'b', 'c' }, 3 },

  { "Random elements, size 2^n ........",
    { 'd', 'b', 'h', 'e', 'c', 'g', 'f', 'a' },
    { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' }, 8 },

  { "Random elements, size 2^n + 1 ....",
    { 'a', 'h', 'c', 'b', 'd', 'i', 'e', 'g', 'f' },
    { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' }, 9 },

  { "Random elements, size 2^n - 1 ....",
    { 'd', 'b', 'e', 'c', 'a', 'g', 'f' },
    { 'a', 'b', 'c', 'd', 'e', 'f', 'g' }, 7 },

  { "Already sorted ...................",
    { 'a', 'b', 'c', 'd', 'e' },
    { 'a', 'b', 'c', 'd', 'e' }, 5 },

  { "Already reverse sorted ...........",
    { 'e', 'd', 'c', 'b', 'a' },
    { 'a', 'b', 'c', 'd', 'e' }, 5 },

  { "One pair of duplicate elements ...",
    { 'a', 'b', 'd', 'b', 'c' },
    { 'a', 'b', 'b', 'c', 'd' }, 5 },

  { "All duplicate elements ...........",
    { 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a' }, 5 }
};

bool sort_test( )
{
  bool rc = true;

  // For each test...
  for( int i = 0; i < sizeof(tests)/sizeof(struct test_case); ++i ) {

    // Do the test.
    std::cout << tests[i].title << " " << std::flush;
    std::sort( &tests[i].input[0], &tests[i].input[tests[i].size] );

    // Did it work?
    bool worked = true;
    for( int j = 0; j < tests[i].size; ++j ) {
      if ( tests[i].input[j] != tests[i].expected[j] ) worked = false;
    }
    if ( worked )
      std::cout << "passed.\n";
    else {
      std::cout << "FAILED!\n";
      rc = false;
    }
  }

  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    if( !heap_test( )       ) rc = 1;
    if( !sort_test( )       ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
