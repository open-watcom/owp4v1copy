#include "stdio.h"
#define MAX_SIZE 120
int main( int argc, char *argv[] )
{
    FILE *fp;
    int len;
    int offset = -2;
    char *buffer;
    char *ptr;
    char *loc;
    char linebuffer[MAX_SIZE];

    if( ( fp = fopen( argv[1], "r" )) == NULL )
        return 1;

    fseek( fp, -1, SEEK_END );
    len = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    buffer = (char*)malloc( len + 1 );
    if( buffer == NULL )
        return 2;

    buffer[len] = '\0';
    loc = buffer+len;

    for( ; ; ) {
        offset++;
        ptr = fgets( linebuffer, MAX_SIZE, fp );
        if( ptr == NULL ) break;
        loc -= strlen( ptr );
        strncpy( loc, ptr, strlen( ptr ) );
    }

    /* now dump it back out */

    fclose( fp );
    if(( fp = fopen( argv[1], "w" )) == NULL)
        return 3;

    fputs( buffer + offset, fp );

    fclose( fp );
    return 0;
}
