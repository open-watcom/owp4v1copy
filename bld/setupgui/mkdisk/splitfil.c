/*
	SPLITFIL.C  - program to split a file into pieces so that
		      it will fit onto a diskette.

    Modified:   By:             Reason:
    ---------   ---             -------
    25-feb-90   F.W.Crigger     Initial implementation
    27-aug-92   H.W.Hillis      modified to split into two pieces

*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <conio.h>


#define BUFFER_SIZE     512

#if !defined( SPLITZIP )
void SplitFile( long size, long max_size, char * file );

void main( int argc, char *argv[] )
//=================================

{

    if( argc != 4 ) {
	printf( "Usage: SPLITFIL filename size max-size\n" );
	exit( 1 );
    }
    SplitFile( atol( argv[ 2 ] ), atol( argv[ 3 ] ), argv[ 1 ] );
}
#endif

void SplitFile( long size, long max_size, char * input_file )
/***********************************************************/
{
    char                fullname[ _MAX_PATH ], drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ], fname[ _MAX_FNAME ], ext[ _MAX_EXT ];
    FILE                *bigfile, *smallfile;
    char                *buffer;
    int                 len_read;
    unsigned		idx;
    
    bigfile = fopen( input_file, "rb" );
    if( bigfile == NULL ) {
	printf( "Unable to open '%s'\n", input_file );
	getch();
	exit( 2 );
    }
    buffer = malloc( BUFFER_SIZE );
    if( buffer == NULL ) {
	printf( "Unable to allocate buffer\n" );
	getch();
	exit( 3 );
    }

    _splitpath( input_file, drive, dir, fname, NULL );

    idx = 0;
    for( ;; ) {
	sprintf( ext, "%u", ++idx );
	_makepath( fullname, drive, dir, fname, ext );
	smallfile = fopen( fullname, "wb" );
	if( smallfile == NULL ) {
	    printf( "Unable to create '%s'\n", fullname );
	    exit( 4 );
	}
	while( size > 0 ) {
	    len_read = read( fileno( bigfile ), buffer, BUFFER_SIZE );
	    if( len_read == 0 ) break;
	    if( write( fileno( smallfile ), buffer, len_read ) != len_read ) {
		printf( "Error writing '%s'\n", fullname );
		getch();
	    }
	    size -= len_read;
	}
	fclose( smallfile );
	if( len_read == 0 ) break;
	size = max_size;
    }
    fclose( bigfile );
}
