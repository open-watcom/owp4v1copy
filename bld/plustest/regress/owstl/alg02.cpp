
#include <algorithm>
#include <iostream>

bool heap_test( )
{
  bool rc = true;

  // Exercise heaps here.

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

  #ifdef _WATCOM_UNSUPPORTED

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

  #endif

  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    // Heaps don't work because of a compiler bug. See bug02.cpp.
    // NOTE: When fixed uncomment body of sort_test and fill in heap_test.
    // if( !heap_test( )       ) rc = 1;
    // if( !sort_test( )       ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
