.func FILENAME_MAX
#include <stdio.h>
#define FILENAME_MAX 123
.funcend
.desc begin
The &func macro expands to an integral constant expression that is the
size needed for an array of char large enough to hold the
longest file name string that the implementation guarantees can be opened;
If the implementation imposes no practical limit on the length of file name
strings, the value of &func should instead be the recommended size of an array
intended to hold a file name string. Of course, file name string contents
are subject to other system-specific constraints; therefore all possible
strings of length &func cannot be expected to be opened successfully.
.np
The &func macro is typically used to dimension an array to hold a file name.
.np
.us ANSI C:
.desc end
.return begin
The &func macro returns a positive integral value.
.return end
.exmp begin
#include <stdio.h>
#include <string.h>

.exmp break
int main( int argc, char *argv[] )
{
    if( argc ) {
      char fname[FILENAME_MAX];

      strcpy( fname, argv[0] );
      puts( fname );
    }
    return( 0 );
}
.exmp end
.class ANSI
.system
