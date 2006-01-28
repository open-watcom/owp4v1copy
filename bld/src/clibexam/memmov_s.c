#define __STDC_WANT_LIB_EXT1__  1
#include <string.h>

void main( void )
{
    char buffer[80];

    memcpy_s( buffer + 1, sizeof( buffer ), buffer, 79 );
    buffer[0] = '*';
}
