#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <io.h>

extern int __nonIBM();

int main( int argc, char **argv )
{
    char    buff[_MAX_PATH];
    char    drive[_MAX_DRIVE];
    char    path[_MAX_PATH];
    char    name[_MAX_FNAME];
    char    ext[_MAX_EXT];

    _splitpath( argv[0], drive, path, name, ext );
#if !defined( WSQL )
#if NECCHECK
    if( __nonIBM() ) {
#else
    if( 0 ) {
#endif
    _makepath( buff, drive, path, "98SETUP", ext );
    } else {
    _makepath( buff, drive, path, "DOSSETUP", ext );
    }
#else
    _makepath( buff, drive, path, "DOSSETUP", ext );
#endif
    if( access( buff, F_OK ) == 0 ) {
    argv[0] = buff;
    execv( buff, (const char * const *) argv );
    return( 0 );
    }
    cputs( "Cannot find '" );
    cputs( buff );
    cputs( "'\r\n" );
    return( 0 );
}
