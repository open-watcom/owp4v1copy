.func _dwDeleteOnClose
#include <wdefwin.h>
int _dwDeleteOnClose( int handle );
.funcend
.desc begin
The &func function tells the console window that it should close
itself when the corresponding file is closed.
The argument
.arg handle
is the handle associated with the opened console.
.np
The &func function is one of the support functions that can be called
from an application using &company's default windowing support.
.desc end
.return begin
The &func function returns 1 if it was successful and 0 if not.
.return end
.see begin
.seelist &function. _dwDeleteOnClose _dwSetAboutDlg _dwSetAppTitle _dwSetConTitle _dwShutDown _dwYield
.see end
.exmp begin
#include <wdefwin.h>
#include <stdio.h>

void main()
  {
    FILE *sec;
.exmp break
    _dwSetAboutDlg( "Hello World About Dialog",
                    "About Hello World\n"
                    "Copyright 1994 by WATCOM\n" );
    _dwSetAppTitle( "Hello World Application Title" );
    _dwSetConTitle( 0, "Hello World Console Title" );
    printf( "Hello World\n" );
    sec = fopen( "CON", "r+" );
    _dwSetConTitle( fileno( sec ),
                    "Hello World Second Console Title" );
    _dwDeleteOnClose( fileno( sec ) );
    fprintf( sec, "Hello to second console\n" );
    fprintf( sec, "Press Enter to close this console\n" );
    fflush( sec );
    fgetc( sec );
    fclose( sec );
  }
.exmp end
.class WATCOM
.system
