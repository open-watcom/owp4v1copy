.chap Windows NT Programming Overview
.*
.np
.ix 'Windows NT' 'programming overview'
Windows NT supports both non-windowed character-mode applications and
windowed Graphical User Interface (GUI) applications.
In addition, Windows NT supports Dynamic Link Libraries and
applications with multiple threads of execution.
.np
.ix 'Windows NT'
.ix 'NT development'
We have supplied all the necessary tools for native development on
Windows NT.
You can also cross develop for Windows NT using either the DOS-hosted
compilers and tools, the Windows 95-hosted compilers and tools, or the
OS/2-hosted compilers and tools.
Testing and debugging of your Windows NT application must be done on
Windows NT or Windows 95.
.np
.ix 'Phar Lap TNT'
.ix 'TNT'
If you are creating a character-mode application, you may also be
interested in a special DOS extender from Phar Lap (TNT) that can run
your Windows NT character-mode application under DOS.
.*
.if '&lang' eq 'C/C++' .do begin
.*
.section Windows NT Programming Note
.*
.np
.ix 'Windows NT' 'programming notes'
.ix 'structure alignment'
.ix 'enums'
When doing Win32 programming, you should use the /ei and /zp4 options
to compile C and C++ code with the &company compilers since this
adjusts the compilers to match the default Microsoft compiler
behaviour.
Some Microsoft software relies on the default behaviour of their own
compiler regarding the treatment of enums and structure packing
alignment.
.do end
.*
.section Windows NT Character-mode Versus GUI
.*
.np
.ix 'Windows NT' 'character-mode applications'
.ix 'Windows NT' 'GUI applications'
Basically, there are two classes of &lang applications that can run in
a windowed environment like Windows NT.
.np
The first are those &lang applications that do not use any of the
Win32 API functions; they are strictly &lang applications that do
not rely on the features of a particular operating system.
&company gives you two choices when porting these kinds of
applications to Windows NT.
.if '&defwin' eq 'Windows NT' .do begin
You may choose to create a character-mode application that makes no
use of the windowing capabilities of the system (the remainder of this
chapter will deal with these kinds of applications).
Or, you may choose to make use of &company's default windowing system
in which application output will be directed to one or more windows.
The latter can give somewhat of a GUI look-and-feel to an application
what wasn't designed for the GUI environment.
A subsequent chapter deals with the creation of applications that make
use of the default windowing system.
.do end
.np
The second class of &lang applications are those that actually call
Win32 API functions directly.
These are applications that have been tailored for the Win32
operating environment.
.if '&lang' eq 'C/C++' .do begin
A subsequent chapter deals with the creation of applications that make
use of the Win32 API.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
&product does not provide direct support for these types of
applications.
While we do provide include files that map out 16-bit Windows
structures and the interface to 16-bit Windows API calls, we do not
provide this for Win32 API.
The Win32 application developer must create these as required.
.np
An alternate solution, for those so-inclined, is to develop the GUI
part of the interface in C and call these functions from FORTRAN code.
.do end
.*
.section Windows NT Character-mode Applications
.*
:set symbol="oprompt"    value=&prompt.
:set symbol="prompt"     value="C:\>".
.*
.np
.ix 'Windows NT' 'character-mode applications'
.ix 'NT character-mode applications'
.ix 'character-mode applications'
Suppose you have a set of &lang applications that previously ran under
DOS and you now wish to run them under Windows NT.
To achieve this, simply recompile your application and link with the
appropriate libraries.
Depending on the method with which you linked your application, it can
run in a Windows NT character-mode environment, or as a Windows NT GUI
application.
A Windows NT GUI application has full access to the complete set of
user-interface tools such as menus, icons, scroll bars, etc.
.if '&defwin' eq 'Windows NT' .do begin
.ix 'NT default windowing system'
An application that was not designed as a windowed application (such
as a DOS application) can run as a GUI application.
This is achieved by a default windowing system that is optionally
linked with your application.
The creation of default windowing applications is described in a later
chapter.
.do end
.np
.ix 'NT character-mode applications'
Very little effort is required to port an existing &lang application
to Windows NT.
Let us try to run the following sample program (contained in the file
.fi hello.&langsuff
.ct ).
.if '&lang' eq 'FORTRAN 77' .do begin
.millust begin
    program hello
    print *, 'Hello world'
    end
.millust end
.do end
.if '&lang' eq 'C/C++' .do begin
.millust begin
#include <stdio.h>

int main( void )
{
    printf( "Hello world\n" );
    return( 0 );
}
.millust end
.np
An equivalent C++ program follows:
.millust begin
#include <iostream.h>

int main( void )
{
    cout << "Hello world" << endl;
    return( 0 );
}
.millust end
.do end
.np
First we must compile the file
.fi hello.&langsuff
by issuing the following command.
.millust begin
&prompt.&ccmd32 hello
.millust end
.np
Once we have successfully compiled the file, we can link it by issuing
the following command.
.millust begin
&prompt.&lnkcmd system nt file hello
.millust end
.np
This will create a character-mode application.
.*
:set symbol="prompt"     value=&oprompt.
