#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d16info.h"

extern d16info_t __far  _d16info;
extern char __far       _env16M[]; 

/* Exercise basic DOS/16M functionality, especially the special
 * DOS/16M kernel selectors (which require the linker to handle
 * absolute segment relocations).
 */
int main( int argc, char **argv )
{
    char __far  *f_env;
    char        **env;
    int         i;

    printf( "DOS/16M version: %Ws\n", _d16info.versionp );
    printf( "Executed as: %s\n", argv[0] );

    /* Environment variables in the DOS/16M kernel. */
    f_env = _env16M;
    i = 4;
    puts( "Contents of the DOS/16M environment segment (partial):" );
    while( *f_env ) {
        printf( "%Ws\n", f_env );
        f_env += _fstrlen( f_env ) + 1;
        if( i-- == 0 )
            break;  /* Limit the number of strings printed. */
    }

    /* Environment variables in the clib. */
    env = environ;
    i = 4;
    puts( "\nC library environment (partial):" );
    while( *env ) {
        puts( *env );
        ++env;
        if( i-- == 0 )
            break;  /* Limit the number of strings printed. */
    }

    return( EXIT_SUCCESS );
}
