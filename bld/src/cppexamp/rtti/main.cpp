//
// Main module of the application.
//

#include "impl.h"
#include "user.h"
#include <iostream.h>

company     ACME;

int main( void )
{
    ACME.addManager( "Bill", 40, 15 );

    ACME.addProgrammer( "Bob", 25, 35 );

    ACME.addProgrammer( "Julie", 30, 35 );

    ACME.addOvertime( "Bob", 10 );

    if( ACME.payRoll() == 4300 ) {
        cout << endl << "Test Successful." << endl;
        return( 0 );
    } else {
        cout << endl << "Test Failed." << endl;
        return( 1 );
    }
}
