#include "stdio.h"
#include "stdlib.h"
#include "process.h"
#include "string.h"
#include "conio.h"
#include "sys\stat.h"

#define BUF_SIZE	4096
#define MAX_PACK_SIZE	(3L*(1024*1024)/4)	// half of 1.44 meg
char	buff[BUF_SIZE];
char	prev_cond[BUF_SIZE];
char	prev_dir[_MAX_DIR];
char	name[_MAX_PATH];
char	new_pack[_MAX_FNAME];

int main( int argc, char **argv )
{
    FILE	*in,*out;
    char	*n;
    char	*dir;
    char	*file;
    char	*rel_file;
    char	*pack_file;
    char	*cond;
    struct stat	sbuf;
    long	packsize;
    int		packnum;
    int		i;

    int rc = 0;
    in = fopen( "sizem.lst", "r" );
    out = fopen( "sizem.out", "w" );
    packsize = 0;
    packnum = 0;
    i = 0;
    if( argc != 2 ) {
	printf( "usage: %s <reldir>\n", argv[0] );
	exit( -1 );
    }
    for( ;; ) {
	cprintf( "%d\r", i++ );
	if( fgets( buff, sizeof( buff ), in ) == NULL ) break;
	buff[strlen(buff)-1] = '\0';
	n = strtok( buff, " \t" );
	dir = strtok( NULL, " \t" );
	file = strtok( NULL, " \t" );
	rel_file = strtok( NULL, " \t" );
	pack_file = strtok( NULL, " \t" );
	cond = strtok( NULL, "\0" );
	if( cond == NULL ) cond = pack_file+strlen(pack_file);
	strcpy( name, argv[1] );
	strcat( name, "\\" );
	if( *rel_file == '.' ) {
	    strcat( name, dir );
	    strcat( name, "\\" );
	    strcat( name, file );
	} else {
	    strcat( name, rel_file );
	}
	if( stat( name, &sbuf ) ) {
	    printf( "\r\nFile '%s' does not exist - putting in own packfile\r\n", name );
	    ++packnum;
	    packsize = 0;
	} else {
	    packsize += sbuf.st_size;
	    if( strcmp( prev_cond, cond ) != 0 ||
		strcmp( prev_dir, dir ) != 0 ||
		packsize > MAX_PACK_SIZE ) {
		++packnum;
		packsize = 0;
	    }
	}
	sprintf( new_pack, "pck%05d", packnum );
	fprintf( out, "%-*s" "%-*s" "%-*s" "%-*s" "%-*s" "%s" "\n",
		dir-n,n,
		file-dir,dir,
		rel_file-file,file,
		pack_file-rel_file,rel_file,
		cond-pack_file,new_pack,
		cond );
	strcpy( prev_cond, cond );
	strcpy( prev_dir, dir );
    }
    fclose( in );
    fclose( out );
    return( rc );
}
