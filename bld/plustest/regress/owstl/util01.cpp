
#include <utility>
#include <iostream>

bool pair_test( )
{
  bool rc = true;

  std::pair< int, char > object( 1, 'H' );
  if( !( object == std::make_pair( 1, 'H' ) ) ) {
    std::cout << "pair FAIL 0001\n"; rc = false;
  }

  return rc;
}

int main()
{
  int rc = 0;
  try {
    if( !pair_test( ) ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return rc;
}
