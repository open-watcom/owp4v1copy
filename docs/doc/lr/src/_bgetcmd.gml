.func _bgetcmd
#include <process.h>
int _bgetcmd( char *cmd_line, int len );
.ixfunc2 '&Process' &func
.ixfunc2 '&Direct' &func
.funcend
.desc begin
The &func function causes the command line information, with the program
name removed, to be copied to
.arg cmd_line.
The argument
.arg len
specifies the size of
.arg cmd_line.
The information is terminated with a
.mono '\0'
character.
.if '&machsys' eq 'QNX' .do begin
This provides a method of obtaining the original parameters to a program
as a single string of text.
.do end
.el .do begin
This provides a method of obtaining the original parameters to a program
unchanged (with the white space intact).
.do end
.np
This information can also be obtained by examining the vector of program
parameters passed to the main function in the program.
.desc end
.return begin
.if &version ge 106 .do begin
The number of bytes required to store the entire command line,
excluding the terminating null character, is returned.
.do end
.el .do begin
If
.arg cmd_line
is
.mono NULL
then the number of bytes required to store the command line, excluding
the terminating null character, is returned; otherwise the number of
bytes stored in
.arg cmd_line,
excluding the terminating null character, is returned.
.do end
.return end
.see begin
.im seeproc &function.
.see end
.exmp begin
.blktext begin
Suppose a program were invoked with the command line
.millust begin
myprog arg-1 ( my   stuff ) here
.millust end
where that program contains
.blktext end
.blkcode begin
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

void main()
  {
    char *cmdline;
    int   cmdlen;
.exmp break
    cmdlen = _bgetcmd( NULL, 0 ) + 1;
    cmdline = malloc( cmdlen );
    if( cmdline != NULL ) {
      cmdlen = _bgetcmd( cmdline, cmdlen );
      printf( "%s\n", cmdline );
    }
  }
.blkcode end
.exmp output
.if '&machsys' eq 'QNX' .do begin
arg-1 ( my stuff ) here
.do end
.el .do begin
  arg-1 ( my   stuff ) here
.do end
.exmp end
.class WATCOM
.system
