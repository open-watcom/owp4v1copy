
// Use this file as a template for creating new tests.

#include <iostream>

bool some_test( )
{
  bool rc = true;

  // Exercise some facility here.

  return( rc );
}


int main( )
{
  int rc = 0;
  try {
    if( !some_test( )       ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
