.chap Build Architecture

In an effort to clean up the build process, make it easier for projects to
compile on various people's machines and allow for easier ports to other
architectures, every project which is developed under the Open Watcom Project
should follow certain conventions as far as makefile layout is concerned.
This section describes the conventions and requirements for these makefiles,
as well as the steps needed to get projects to compile on a "build" machine.
.np
For those who do not desire a lecture on the preparation and maintenance of
makefiles, feel free to skip straight to the Executive Summary at the end.
.np
Every build machine must have the mif project (
.us bld\build\mif
) installed. That is taken care of by uncompressing the Open Watcom source
archive and/or syncing up with Perforce. For the purposes of this document,
every machine where a project can be built is considered a build machine,
not just the "official" build server.

.section Makeinit
.*
.np
.ix 'makeinit'
All the magic starts with
.us makeinit.
Every build machine must have a
.us makeinit
file with the following defined therein:
.begnote
.note mif_dir:
        must point to the directory in which the mif project has been installed
.note lang_root:
        the location of the installed (Open) Watcom compiler
.endnote

For each project with name X you wish to have on the build machine,
.id X_dir
must be set to the directory containing the project. That is, if you want the
code generator on your machine (and who wouldn't?), it is officially
named cg (see Project Names below) and so you would define
.id cg_dir.
.np
Alternatively, if all of your projects are in directories which correspond
to their project names under a common directory, you can set dev_dir and
.us !include cdirs.mif
in your
.us makeinit.
This is the recommended setup and default for Open Watcom. You do not have
to take any extra action to use it.
.np
Alternatively, you can do the above and then redefine
.id X_dir
for any projects which are not under the
.id dev_dir.

.section Project Names
.*
.np
.ix 'names'

Each project must be given a unique name, which should also be a valid
directory name under FAT file systems (8.3 convention).
.np

.section Makefiles
.*
.np
Each makefile should be located in the object file directory - ie. no more
of this silly cd'ing into the object directory based on crystal-balls and
what not. The makefile must define the following:
.begnote
.note host_CPU:
        architecture which the resulting executable code will run on.
.note host_OS:
        os which the resulting executable code will run on
.note proj_name:
        the project name
.endnote
.np
Valid values for
.id host_CPU
are 386, i86, axp, ppc. These should be self-explanatory.
Valid values for
.id host_OS
are dos, nt, os2, nov, qnx, win, osi. These should be self-explanatory for
the most part, with one possible exception: osi stands for OS Independent,
the executables can run on multiple OSes if appropriate loader stub is
provided.
.np
The makefile must then include
.us cproj.mif.
This will define all sorts of make
variables, which can then be used to build the project. A list of the
most important of these variables and what they can be used for is
included below.
.np
A makefile can also include
.us deftarg.mif,
for definition of the required clean target, and
.us defrule.mif,
which has the default compilation rules. A makefile is free to override
these defaults as long as it follows the following conventions:
.np
.autonote
.note
Tools which have macros defined for them must be referred to by the
macros - these are currently (any additions should be brought to my attention):
.begnote
.note $(CC):
The C compiler
.note $(CPP):
The C++ compiler
.note $(LINKER):
The linker
.note $(LIBRARIAN):
The librarian
.note $(AS):
The assembler, if applicable
.note $(RC):
The resource compiler
.note $(EDIT):
Our VI editor
.note $(YACC):
Our version of yacc
.note $(RE2C):
The regular-expression to C compiler
.endnote
.np
note
After the
.us cproj.mif
is included and before a command is run, the
.id INCLUDE
environment variable must be set to
.id inc_path.
A project is not allowed to set
.id INCLUDE
to something else - it may redefine certain elements of
.id inc_path
 - see Include Paths below. This ensures that include paths can be
easily controlled and tracked.
.np
.note
When referring to other projects, a makefile should use the
.id X_dir
macro, where X is the name of the project.
.endnote


.section Requirements To Build
.*
.np
.ix 'build requirements'
A project should be able to build either a -d2 (if
.id release_$(proj_name)
!= 1)
or releaseable (if
.id release_$(proj_name)
== 1 ) executable providing the following are done:
.begbull
.bull
the project is uptodate and
.id $(proj_name)_dir
is set correctly
.bull
the mif project is uptodate and make knows to look for .mif files in there
.bull
lang_root is set
.bull
all depended upon projects are uptodate and have
.id $(proj_name)_dir
set correctly
.bull
all depended upon projects have been built
.bull
any required executables from
.us bld\build\bin
are in the path
.endbull
.np
Note that there are no other requirements here &mdash it is very annoying when
a project requires you to define handles for tools, create directories in
which it can deposit stuff, scrounge up obscure tools from who knows where
or pretend to be Jim Welch
.fn One of the original Watcom compiler developers
in order to get a debuggable version of the executable.
.np
Perhaps it should be noted that "releasable" build still contains debugging
information, but only at the -d1 level and in a separate .sym file. In case
of crashes or other highly unusual behaviour, release build should be enough
to point you in the right direction but usually not sufficient to fully
diagnose and rectify the problem.
.np
Now, if you wish to allow certain abberant behaviours based upon cryptic
make variables, that is fine, as long as the project can build both a
debuggable (ie full -d2) version as well as a release (ie no -d2, -d1 only
and no memory tracker) version without these things being set. That is, if
you want stupid stuff in your
.us makeinit
&mdash fine, but don't require others to do this in order to build the project.
.np
Any non-standard makefile variables which you do use should be prepended
by the name of your project and an underscore, to prevent namespace clutter
and clashes.
.np
Tools required to build are an issue that will have to be handled on a
case-by-case basis. For example, stuff to bind up DOS protected mode apps
will likely be added to the standard suite of tools available, and macros
made for them. Before we do this, we should standardize on one extender and
use it wherever possible. Any small special purpose tools should be checked
in along with the project and built as part of the build process (so that
we don't have to check in zillions of binaries for all supported platforms). An
important future consideration will be the ability to build on a different
architecture. Please try and avoid weirdo tools that have no hope of running
on an Alpha or PPC running NT. More general tools (yacc, re2c, w32bind) that
are likely to be used by several projects should be copied up into the bin
directories under
.us bld\build
&mdash bin for DOS, binp for OS/2 and binnt for some other OS, forget which.
These tools should be referenced from the makefile as
.id $(bld_dir)\tool.
If your tool cannot run under a particular OS, you should at least put
a batchfile in that bin which echoes a message to that effect (to alert
people to the fact that you've just made their life difficult).


.section The Runtime DLL Libraries
.*
.np
.ix 'DLL runtime'
If you set
.id $(proj_name)_rtdll
= 1, the -br switch should be thrown for you automatically, providing the
target os supports it.


.section Memory Trackers
.*
.np
.ix 'memory trackers'
The memory tracker is an useful development aid &mdash it tracks all dynamic
memory allocations and deallocations, making it easy to spot memory leaks and
helping to pinpoint heap corruption or other unsociable behaviour that so
rarely happens in our code.
.np
If the memory tracker is an optional part of your project, and independant
of the release mode, it is suggested that you enable it if
.id $(proj_name)_trmem
is set to 1, and disable it otherwise.
.np
The source to the memory tracker can be found in
.us bld\trmem.


.section The Clean Target
.*
.np
.ix 'clean target'
Each makefile should support a clean target. This should not be part of the
default target list, and should delete every makefile generated file. Which
means that after running "wmake clean", the directory should look exactly
like a new installation of the project on a bare drive. !including
.us deftarg.mif
should do for most people who do not get creative with file extensions or
generated source files.
.np
Do not underestimate the importance of proper cleanup. It guarantees that
every part of a project can be built from scratch, ensuring that there
will be no nasty surprises when stuff breaks for people after a clean
install just because you had a generated file hanging around and never
discovered that it can no longer be made.


.section Pmake Support
.*
.np
.ix 'pmake'
Every makefile should contain a pmake line at the top. Pmake is a tool which
was invented in order to make life easier with the clib project. Pmake, when
run from a root directory, crawls down all subdirectories looking for files
called
.us makefile.
When it finds one, it checks to see if there is a wmake comment which looks
like:
.millust begin
#pmake: <some identifiers>
.millust end
If there is such a comment, and any identifiers in the list given to pmake
appear in the list after the colon, then wmake is invoked in that directory.
This provides a handy way to control selective builds and destroys. Some
tokens which should be used by the appropriate makefiles are:
.begnote $break
.note all
should be in every makefile
.note build
indicates that wmake should be run in this directory as part of the build process
.note os_x
for each x in the list of the valid host_OS tokens
.note cpu_x
for each x in the list of the valid host_CPU tokens
.note target_x
for each x in the list of valid host_CPU tokens (for compilers and targetted apps)
.note tiny, small, compact, medium, large, huge, flat, nomodel
the memory model
.note inline, calls
whether an app uses inline 8087 stuff or fp calls
.endnote
.np
For example, an executable which is going to run on the PPC version of NT
should have a pmake line which contains, at a minimum:
.millust begin
#pmake: all build os_nt cpu_ppc
.millust end
You are free to add as many mnemonic identifiers as you want, of course,
but anything which you feel is an abstract classification that would apply
to other projects, please bring to our collective attention and if deemed
useful, it will get added to the appropriate places (and the list above).
.np
For an example of where this is useful, if we suddenly found out that our
NT headers were bogus and everything including them needed a recompile, we
could do the following on the build machine: "pmake os_nt -h clean & pmake
os_nt -h".
.np
Another very useful property of this setup is that it allows people to
build libraries/binaries only for their host platform. This is especially
convenient if they don't have all the necessary SDKs, Toollkits and
whatnot installed and/or cannot run some or all of the platform specific
tools required during builds. And this situation is the norm rather than
exception &mdash only dedicated build servers usually have all necessary
files in place.


.section Misc Conventions
.*
.np
.ix 'conventions'
To make it easy to see what projects are required to make a given project,
all needed projects should be listed in a makefile comment in the main
makefile of the dependant project. Hopefully, this main makefile should
be called
.us master.mif
and be in the root directory, or a mif subdirectory thereof, of the project.
.np
Also, it is suggested that the object file directory name be a combination
of the
.id host_OS
followed by the
.id host_CPU,
if convenient. For example, NT versions for the PPC should be genned into
a ntppc directory. If a directory structure which is different than this
is used for some reason, then comments explaining exactly what is built
where would be nice in the
.us master.mif
file.
.np
Things get more interesting if cross compilers are thrown into the mix.
In that case three components are required in the name: for instance a
.us nt386.axp
directory can hold the Alpha AXP NT compiler producing 386 code.


.section DLLs and Windowed Apps
.*
.np
.ix 'DLLs'
.ix 'windowed applications'
Set
.id host_OS
and
.id host_CPU
as normal, and then, if creating a windowed app, set
.id sys_windowed
= 1. If creating a DLL, set
.id sys_dll
= 1. Delightfully simple.


.section Include Paths
.*
.np
.ix 'include paths'
The
.id inc_path
macro, which every project should set the include environment variable to
before executing any build commands, is composed of several other variables.
Projects are able to hook any of these variables by redefining them after
.us cproj.mif
is included. The current, and likely to change, structure looks like this:
.millust begin
inc_path = inc_dirs | inc_dirs_$(host_OS) | inc_dirs_sys
inc_dirs_sys = inc_dirs_lang | inc_dirs_sys_$(host_OS)
inc_dirs_lang = $(lang_root)\h
.millust end
So, a project should put any include directories it needs into inc_dirs - this
includes
.id $(watcom_dir)\h
&mdash no project should assume anything will be set except the location of the
(Open) Watcom C compiler header files and the appropriate system header files.
.np
If it needs to, a project can override any and all of these &mdash for instance,
the clib needs to be built with the next release header files, and so would
redefine
.id inc_dirs_lang.
.np
Any OS-specific header files needed by the project can be set in
.id inc_dirs_$(host_OS)
&mdash again, this should not include the standard system
header files, which will be defined in
.id inc_dirs_sys_$(host_OS).


.section Executive Summary
.*
.np
.ix 'summary'
In order to convert a project to this new structure or create a new (and
conforming) project, do the following:
.np
.autonote
.note
Create an object file directory for each combination of host_OS/host_CPU
under your project.
.note
Give your project a name, for instance Foo.
.note
Create a
.us master.mif
in the root of your project.
.note
Put all the normal makefile gear in this
.us master.mif.
.note
Add
.id proj_name
 = Foo to the top of master.mif
.note
Include the following files (in this order)
.us cproj.mif, deftarg.mif, defrule.mif
in
.us master.mif
.note
Add
.id inc_dirs
= {list of directories, separated by semi-colons, which your project needs
in include path - this does not include OS-specific includes (ie
.us \lang\h\win
)}
.note
Add
.id extra_c_flags
= {list of c flags, not including optimization, /w4, /zq. /we and model info,
needed to compile your application} These should be host_OS/host_CPU independant.
.note
Add
.id extra_l_flags
 = {list of linker directives, not incuding system or debug directives}
 Should be host_OS/host_CPU independant.
.note
Use following to compile:
.id $(cc) $(cflags)
filename etc...
.note
Use following to link:
.id $(linker) $(lflags)
file { list of obj files }
.note
Use following to create libraries:
.id $(librarian)
.note
In each object file directory, create a makefile which looks like the following:
.millust begin
#pmake: all build os_X cpu_Y
host_OS=X
host_CPU=Y
!include ..\master.mif
.millust end
.endnote
That's it! It should be apparent that sticking to these guidelines will make
everyone's life easier.

