.func isatty
#include <&iohdr>
int isatty( int &fd );
.ixfunc2 '&OsIo' &func
.funcend
.desc begin
The &func function tests if the opened file or device referenced by
the file &handle
.arg &fd
is a character device (for example, a console, printer or port).
.desc end
.return begin
The &func function returns zero if the device or file is not
a character device; otherwise, a non-zero value is returned.
.im errnoref
.return end
.see begin
.seelist isatty open
.see end
.exmp begin
#include <stdio.h>
#include <&iohdr>

void main()
  {
    printf( "stdin is a %stty\n",
            ( isatty( fileno( stdin ) ) )
            ? "" : "not " );
  }
.exmp end
.class POSIX 1003.1
.system
