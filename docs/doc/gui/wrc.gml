.chap The &wrcname
.*
.if &e'&dohelp eq 0 .do begin
.section Introduction
.do end
.*
.np
The &wrcname (&wrccmdup) performs two functions. It converts human
readable resource script files (usually with the extension ".RC") into
machine readable resource files (usually with the extension ".RES").
We call this process pass one. The &wrcname then combines this ".RES"
file with an existing executable file or dynamic link library. We call
this process pass two. Pass two may also be run without a ".RES" file
to set flags or to produce a fastload section.
.np
The &wrcname can process resources for the Windows and Windows NT
operating systems.
.np
The &wrcname command line syntax is the following.
.ix '&wrccmdup' 'command line format'
.ix 'command line format' '&wrccmdup'
.ix '&wrcname' 'command line format'
.mbigbox
&wrccmdup {options} input-filename {options} [output-filename] {options}
.embigbox
.np
The square brackets [ ] denote items which are optional.
.np
.begpoint $break
.point &wrccmdup
is the name of the &wrcname..
.point input-filename
The filename specification of the resource file to compile.
.np
If no extension is specified for input-filename, then the extension
"rc" is assumed. If the period (.) is specified but not the extension,
the file is assumed to have no file extension.
.point output-filename
The filename specification for the output of the compilation.
.np
The meaning of this filename and its default extension is dependent on
the actions being performed.
See the section entitled :HDREF refid='wrcinfo'. for information on
this.
.point options
A list of valid options, each preceded by a slash (/) or a dash (&minus.).
.np
Options may be specified in any order.
.begpoint $compact $setptnt 11
:DTHD.Options:
:DDHD.Description:
.point &sw.?
print this help summary
.point &sw.q
.ix 'resource compiler options' 'q'
operate quietly
.point &sw.30
.ix 'resource compiler options' '30'
mark file as requiring Windows 3.0
.point &sw.31
.ix 'resource compiler options' '31'
mark file as requiring Windows 3.1 (default)
.point &sw.dNAME=value
.ix 'resource compiler options' 'd'
behave as if
.mono #define NAME value
was at top of file
.point &sw.ad
.ix 'resource compiler options' 'ad'
generate auto dependency information for use by the &makname utility
.point &sw.bt=<target>
.ix 'resource compiler options' 'bt'
build target is one of the following:
.begpoint $compact
.point windows
build a WIN16 resource file (default for the non-NT hosted resource
compiler)
.point nt
build a WIN32 resource file (default for the Windows NT-hosted or
Windows 95-hosted resource compiler)
.endpoint
.point &sw.c=name
set the code page conversion file
.point &sw.e
for a DLL, global memory above EMS line
.point &sw.fe=name
set the output EXE file to name
.point &sw.fo=name
set the output RES file to name
.point &sw.fr=name
specify an additional input RES file.
.point &sw.i=path
look in path for include files
.point &sw.k
don't sort segments (same as /s0)
.point &sw.l
program uses LIM 3.2 EMS directly
.point &sw.m
each instance of program has its own EMS bank
.point &sw.p
private DLL
.point &sw.r
only build the RES file
.point &sw.s{0,1,2}
segment and resource sorting method:
.begpoint $compact
.point 0:
no sorting, leave segments in the linker order
.point 1:
move preload segments to front and mark for fast load if possible
.point 2:
(default) move preload, data, and non-discardable segments to front
and mark for fast load if possible
.endpoint
.point &sw.t
protected mode only
.point &sw.v
verbose: print tokens as they are scanned
.point &sw.v1
verbose: print grammar rules as they are reduced
.point &sw.v2
verbose: print both tokens and grammar rules
.point &sw.x
ignore the INCLUDE environment variable
.point &sw.zk{0,1,2}
double-byte character support:
.begpoint $compact
.point 0:
(default) Kanji
.point 1:
Chinese/Taiwanese (for Windows only)
.point 2:
Korean (for Windows only)
.endpoint
.point &sw.zm
output Microsoft format
.fi &sysper.RES
files
.point &sw.zn
don't preprocess the file
.endpoint
.endpoint
.*
.section Resource Definition Files
.*
.ix '&wrcname' 'resource definition files'
.np
A resource definition file (".RC" file) lists all resources that your
application will use.
You should refer to your Windows or Windows NT programmer's
documentation for information on the script language used in resource
definition files.
.*
.beglevel
.*
.section Sample .RC file
.*
.np
The following example is the resource script file from an application
called Life:
.tinyexam begin
#include "windows.h"
#include "life.h"

ABOUTBOX DIALOG LOADONCALL MOVEABLE DISCARDABLE 7, 15,
147, 87
    CAPTION "About Life"
    STYLE WS_BORDER | WS_CAPTION | WS_DLGFRAME | WS_SYSMENU |
                DS_MODALFRAME | WS_POPUP
    BEGIN
        CONTROL "&company Life Program for Windows", -1, "static",
                SS_CENTER | WS_GROUP | WS_CHILD, 0, 5, 144, 8
        CONTROL "Version 1.0", -1, "static", SS_CENTER | WS_GROUP
                | WS_CHILD, 0, 14, 144, 8
        CONTROL "OK", 1, "button", BS_DEFPUSHBUTTON | WS_GROUP
                | WS_TABSTOP | WS_CHILD, 55, 64, 32, 14
        CONTROL "LifeIcon", -1, "static", SS_ICON | WS_CHILD,
                58, 28, 27, 22
    END

.tinyexam break
CellBitMap BITMAP cell.bmp
MenuBitMap BITMAP menu.bmp

LifeIcon ICON life.ico

LifeMenu MENU
    BEGIN
        POPUP "&File"
            BEGIN
                MENUITEM "&Save Selected Region ...", MENU_SAVE
                MENUITEM "&Load New Pattern ...", MENU_LOAD
                MENUITEM SEPARATOR
                MENUITEM "&About Life ...", MENU_ABOUT
            END
        POPUP "&Boundary"
            BEGIN
                MENUITEM "&Wrap Around", MENU_WRAP_AROUND
                MENUITEM "&Bounded Edges", MENU_BOUNDED_EDGES
            END
    END
.tinyexam end
.np
The two
.bd #include
statements for
.fi &sysper.h
files are used to include definitions necessary to compile the
resource file.
The
.fi windows.h
header file contains general definitions for Windows, and the
.fi life.h
header file contains definitions specific to the LIFE application.
.np
The
.bd DIALOG
statement defines the "ABOUT" dialog box used by the LIFE application.
.np
The two
.bd BITMAP
statements define a pair of bitmap resources found in the files
.fi cell.bmp
and
.fi menu.bmp.
The bitmaps are identified by the names
.us CellBitMap
and
.us MenuBitMap
respectively.
.np
The
.bd ICON
statement defines an icon resource found in the file
.fi life.ico.
The icon is identified by the name
.us LifeIcon.
.np
The
.bd MENU
statement defines the various menu items that are in the menu, and
what identifiers are sent to the application when the menu item is
selected.
.*
.endlevel
.*
.section Resource Compiler Options
.*
.np
.ix '&wrccmdup options'
.ix '&wrcname' 'options'
.begpoint $break
:DTHD.Options:
:DDHD.Description:
.*
.point &sw.?
Displays a summary of &wrcname command line options.
.*
.point &sw.q
.ix 'resource compiler options' 'q'
Causes the &wrcname to operate quietly.
No text is displayed as the &wrcname runs except for warning and
error messages.
.*
.point &sw.30
.ix 'resource compiler options' '30'
Marks the executable file as being able to run on Windows 3.0 or
Windows 3.1.
By default, &wrccmdup marks the executable file as able to run on
Windows 3.1 only. This option affects pass two only.
.*
.point &sw.31
.ix 'resource compiler options' '31'
Marks the executable file as being able to run on Windows 3.1 only.
This is the default setting. This option affects pass two only.
.*
.point &sw.ad
.ix 'resource compiler options' 'ad'
Allows the use of the WMAKE .AUTODEPEND directive with your
.fi &sysper.RES
files.
If you do the first and second passes separately and use this option,
you should specify it for both passes. This options may not be used
with the -zm switch.
.*
.point &sw.dNAME=value
.ix 'resource compiler options' 'd'
Defines a macro NAME.  This is the same as adding the line
.millust begin
#define NAME    value
.millust end
to the top of your resouce script file.  If value is not specified
then a compiler generated value is provided.  This option affects pass
one only.
.*
.point &sw.bt=<target>
.ix 'resource compiler options' 'bt'
This is the build target directive.
It is used to specify whether you are building a resource file for
Windows or Windows NT.
The target may be one of the following:
.begpoint
.point windows
build a WIN16 (Windows) resource file (default for the non-NT hosted
resource compiler)
.point nt
build a WIN32 (Windows NT) resource file (default for the NT hosted
resource compiler)
.endpoint
.np
This option affects pass one only.  During pass two the target is
inferred from the format of the input files.
.*
.point &sw.e
Specifies that global memory is above the EMS line in a Windows 3.0
DLL.
This option has no effect with Windows 3.1 or Windows NT.  This option
affects pass two only.
.*
.point &sw.fe=name
Specifies a new name for the resulting executable file after the
resources are added to the executable.
.*
.point &sw.fo=name
Specifies the name of the output resource file (default is the name of
the
.fi &sysper.RC
file with a
.fi &sysper.RES
extension).
The option affects pass one only.
.*
.point &sw.fr=name
Specifies the name of an additional input resource file.
The option affects pass one only.
This option may be specified multiple times.
.*
.point &sw.i=path
Specifies an include path.
This include path is searched before the directories specified in the
.ev INCLUDE
environment variable.  This option affects pass one only.
.*
.point &sw.k
Disables the segment sorting feature (load optimization).
If this option is not specified, the &wrcname arranges all pre-load
segments and resources so that they are at the start of the
executable. This option has no effect with Windows NT. This option
affects pass two only.
.*
.point &sw.l
Mark the application as using LIM 3.2 EMS directly.
This option has no effect with Windows 3.1 or Windows NT.
This option affects pass two only.
.*
.point &sw.m
Each instance of the application has its own EMS bank, when Windows is
running with EMS 4.0 (by default, all instances share the same EMS
bank). This option has no effect with Windows 3.1 or Windows NT.
This option affects pass two only.
.*
.point &sw.p
Mark a dynamic link library as a private DLL that is called by only one
application. This option has no effect with Windows 3.1 or Windows NT.
This option affects pass two only.
.*
.point &sw.r
Only build the
.fi &sysper.RES
file.
This option is used when you do not want to add the compiled resources
to the executable file.  It prevents pass two from being performed.
.*
.point &sw.s{0,1,2}
Specifies the segment and resource sorting method.
Possible settings are:
.begpoint
.point 0:
No sorting, leave segments in the linker order.
.point 1:
Move preload segments to front and mark for fast load if possible.
.point 2:
Move preload, data, and non-discardable segments to front
and mark for fast load if possible (the default).
.endpoint
.np
This option has no effect with Windows NT. This option affects pass two
only.
.*
.point &sw.t
Marks the application as able to run in a protected-mode Windows
environment (standard mode or enhanced mode) only.
This option has no effect with Windows 3.1 or Windows NT. This option
affects pass two only.
.*
.point &sw.v
This is a debugging option.
It causes &wrccmdup to print tokens as they are scanned during pass
one.
.*
.point &sw.v1
This is a debugging option.
It causes &wrccmdup to print grammar rules as they are reduced during
pass one.
.*
.point &sw.v2
This is a debugging option.
It causes &wrccmdup to print both tokens and grammar rules while pass
one is performed.
.*
.point &sw.x
This option causes &wrccmdup to ignore the
.ev INCLUDE
environment variable.
Normally, the &wrcname searches the directories specified in the
.ev INCLUDE
environment variable for files specified in a #include directive in
the
.fi &sysper.RC
file. This option affects pass one only.
.*
.point &sw.zk{0,1,2}
Strings contained in resources in the
.fi &sysper.RC
file are assumed to contain double byte characters from the
appropriate character sets.
Although these options allow strings to contain double byte
characters the names of resources in the resource script file must
contain only standard ASCII characters in the range 0-127 (inclusive).
This option affects pass one only.
Possible settings are:
.begpoint
.point 0:
(default) Kanji
.point 1:
Chinese/Taiwanese (for Windows only)
.point 2:
Korean (for Windows only)
.endpoint
.*
.point &sw.zm
This option causes &wrccmdup to output a Microsoft format
.fi &sysper.RES
file.
This is useful when using a dialog editor or other resource tool that
understands Microsoft
.fi &sysper.RES
files.  This option affects pass one only.
This option may not be specified when creating a
.fi &sysper.RES
file for NT (i.e., when the -bt=NT switch has been specified).
.*
.point &sw.zn
&wrccmdup will not pre-process the
.fi &sysper.RC
file if this option is specified.
This option affects pass one only.
.*
.endpoint
.*
.section *refid=wrcinfo Using the &wrcname
.*
.np
The command &wrccmdup is used to start the &wrcname..
The &wrcname may be used in a number of different ways:
.begbull
.bull
To compile a resource definition file and add the resulting resources
to the executable.
.np
&wrccmdup is used as follows to accomplish this:
.millust begin
&wrccmd [options] resource-definition-file [executable-file]
.millust end
.np
The resource-definition-file must not have the extension ".RES",
".EXE", ".SCR", ".DLL" or ".DRV". If no extension is specified for the
executable-file then ".EXE" is assumed. If executable-file is
specified without an extension but is terminated with a period (".")
then the default extension is not added. If the executable-file is not
specified then its value defaults to the filename specified for the
resource-definition-file with its extension changed to ".EXE".
.exam begin
&wrccmd life.rc
.exam end
.bull
To compile resources separately.
.np
This causes the &wrcname to create a
.fi &sysper.RES
file only.
&wrccmdup is used as follows to accomplish this:
.millust begin
&wrccmd -r [options] resource-definition-file [output-file]
.millust end
.np
The resource-definition-file must not have the extension ".RES",
".EXE", ".SCR", ".DLL" or ".DRV". If no extension is specified for the
output-file then ".RES" is assumed. If output-file is specified
without an extension but is terminated with a period (".") then the
default extension is not added. If the output-file is not specified
then its value defaults to the filename specified for the
resource-definition-file with its extension changed to ".RES".
.exam begin
&wrccmd -r -bt=NT life.rc
.exam end
.bull
To add a compiled resource file to an executable file.
.np
&wrccmdup is used as follows to accomplish this:
.millust begin
&wrccmd [options] resource-file [executable-file]
.millust end
.np
The resource-file must have the extension ".RES".  The defaults for
the executable-file parameter are the same as in the case of compiling
a resource definition file and adding the results to an executable.
.exam begin
&wrccmd life.res
.exam end
.bull
To add multiple compiled resource files to an executable file.
.np
&wrccmdup is used as follows to accomplish this:
.millust begin
&wrccmd [options] res-file [executable-file] -fr=res-file-2 ...
.millust end
.np
The resource files must have the extension ".RES".  The defaults for
the executable-file parameter are the same as in the case of compiling
a resource definition file and adding the results to an executable.
.exam begin
&wrccmd life.res life.exe -fr wave.res
.exam end
.bull
To compile an application that does not have a resource file.
.np
This is useful if you wish to set the Windows or Windows NT version
number, or sort load the segments (Windows), or use any of the other
executable modifying features of &wrcname..
&wrccmdup is used as follows to accomplish this:
.millust begin
&wrccmd [options] executable-file
.millust end
.np
The file specified for executable-file specified must have a
".EXE", ".SCR", ".DLL" or ".DRV" extension.
.exam begin
&wrccmd -30 life.exe
.exam end
.endbull
.*
.section Warning and Error Messages
.*
.im wrcmacs
.im wrcerrs
