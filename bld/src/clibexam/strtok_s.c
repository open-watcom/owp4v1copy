#define _ _STDC_WANT_LIB_EXT1_ _ 1
#include <stdio.h>
#include <string.h>
.exmp break
void main()
{
    char    *p;
    char    *buffer;
    char    *delims = { " .," };
    size_t  buflen;
    char    *ptr;

    buffer = strdup( "Find words, all of them." );
    printf( "%s\n", buffer );
    buflen = strlen( buffer );
    p = strtok_s( buffer, &buflen, delims, &ptr );
    while( p != NULL ) {
      printf( "word: %s\n", p );
      p = strtok_s( NULL, &buflen, delims, &ptr );
    }
    printf( "%s\n", buffer );
}
