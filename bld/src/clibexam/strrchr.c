#include <stdio.h>
#include <string.h>

void main()
{
    printf( "%s\n", strrchr( "abcdeaaedcb", 'a' ) );
    if( strrchr( "abcdeaaedcb", 'x' ) == NULL )
        printf( "NULL\n" );
}
