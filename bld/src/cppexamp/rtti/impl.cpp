//
// This module contains member function definitions. It is provided
// by a library vendor and customers do not have access to its source code.
//

#include <new.h>
#include <string.h>
#include "impl.h"

employee::employee( char *n, int hr ) : hourRate( hr )
{
    Name = new char[strlen( n ) + 1];
    strcpy( Name, n );
}

employee::~employee()
{
    delete[] Name;
}

int manager::salary()
{
    return( hourRate * 40 + Experience * 10 );
}

int programmer::salary()
{
    return( hourRate * 40 );
}
