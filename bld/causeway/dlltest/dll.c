#include <stdio.h>
#include <stdlib.h>
#include "cwdllfnc.h"

extern char *_LpPgmName;

int main( int reason, char **dummy )
{
    int result;

    if( !reason ) {

        /*
        ** DLL initialisation.
        */
        printf( "DLL startup...\n" );
        printf( "DLL File name: %s\n", GetModuleFileName( _psp ) );
        printf( "Program name: %s\n", _LpPgmName );
        fflush( stdout );

        /* return zero to let the load continue */
        result = 0;

    } else {

        /*
        ** DLL clean up.
        */
        printf( "DLL shutdown...\n" );
        fflush( stdout );

        result = 0;
    }
    return( result );
}


void __export _cdecl SayHello( char *message )
{
    printf( "Recieved DLL Message: %s\n", message );
    fflush( stdout );
}
