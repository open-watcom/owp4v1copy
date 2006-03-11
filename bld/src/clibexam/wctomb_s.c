#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>

wchar_t wchar = { 0x0073 };
char    mbbuffer[2];
.exmp break
int main()
{
    int     len;
    int     status;
    errno_t rc;
.exmp break
    rc = wctomb_s( &status, NULL, 0, wchar );
    printf( "Character encodings are %sstate dependent\n",
            ( status ) ? "" : "not " );

    rc = wctomb_s( &status, mbbuffer, 2, wchar );
    if( rc != 0) {
        printf( "Character encoding error\n");
    } else {
        mbbuffer[len] = '\0';
        printf( "%s(%d)\n", mbbuffer, len );
    }
    return( rc );
}
