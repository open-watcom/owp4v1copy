Note: This information and much more is now in the Open Watcom Developer's
Guide. To produce a PostScript version go to docs\ps and run

 wmake hbook=devguide

If you have everything set up correctly, you should end up with devguide.ps
which you can print or view.
-------------------------------------------------------------------------------


  Getting up to Speed on Open Watcom
  ==================================

Here's a quick intro on the build layout and build process as
they used to be at Watcom/Sybase. The Open Watcom layout is
slightly different and many parts of the build process are broken
currently due to removal of third-party tools, headers and libraries.
The original Watcom/Sybase setup placed all directories in d:\. It
is however perfectly possible to place them wherever you wish, for
instance w:\ow. Where this file refers to d:\, substitute your Open
Watcom root location (stored in the owroot environment variable).

1) Default build machine layout:

d:\
    bld
          - this is the root of the build tree
            each project we build has a subdirectory under d:\bld
            for example:
            d:\bld\cg - code generator
            d:\bld\cc - c compiler
            d:\bld\plusplus - c++ compiler
            etc. (see projects.txt for details)

    rel2  - this is where the software we actually ship gets copied
            after it is built - it matches the directory structure of our
            shipping watcom c/c++ tools

    bat   - batch files, many of which aren't used anymore
            of most interest are the .ctl files - scripts for the "builder"
            tool (see below)  and make init files

    tools - utilities and 3rd party tools - not in Open Watcom

    lang  - used to hold Watcom 10.6 compiler

    nlang - used to hold intermediate version of 11.0 compiler;
            for OpenWatcom these both point to the same place
            where Open Watcom 1.x binaries live

    docs  - here is all for documentation, sources and tools


------------------------------
2) To set up a new machine:

  - Modify setvars.bat/setvars.cmd to reflect your setup. See comments
    within the file for additional information.

    d:\bat\makeinit, d:\bat\makecomm may have some machine specific info,
    but should be ok if you don't change any locations

    Your path should look something like this if you are on NT:

    PATH=C:\WINNT\system32;
    C:\WINNT;
    d:\bat;
    d:\lang\binnt;
    d:\lang\binw;
    d:\bin;
    d:\tools;
    d:\bld\build\binnt

  - Create directories not included in source archives/Perforce. This is
    primarily the rel2 tree where finished binaries, libraries, include
    files etc. end up. To do this, simply run mkrel2.bat/mkrel2.cmd which
    should care of everything provided that you have your environment
    variables set up correctly.

------------------------------
3) Priming the pump:

Besides the Open Watcom C/C++ 1.x binaries you'll need a few other executables
used by the build process, primarily builder. The source is in \bld\builder.
Run wmake in the directory corresponding to your host platform and if you set
up everything properly, the executables will be built and copied to the
right place. Perhaps someone will fully automate this process one day. For
now please consider it an aptitude test.

------------------------------
4) Build process:

We use the Open Watcom C/C++ compilers and Open Watcom wmake to build our
tools, but at the top level we have a tool which oversees traversing the
build tree, deciding which projects to build for what platforms, logging
the results to a file, and copying the finished software into the release
tree (rel2), making fully automated builds a possibility.

This tool is called builder.

See d:\bld\builder\builder.doc for detailed info on the tool and the source
if the documentation doesn't satisfy you.

Here's how we use it:

Each project has a "lang.ctl" builder script file.
If you go to a project directory and run builder, it will make only that
project; if you go to \bld and run builder, it will build everything
the overall build uses \bat\lang.ctl which includes all of the individual
project lang.ctl files that we use. Note that if you run builder, it will
traverse directories upwards until it finds a lang.ctl (or it hits the
root and still doesn't find anything, but then you must have done something
wrong).

Results are logged to "build.log" in the current project directory (or d:\bld),
the previous build.log file is copied to build.lo1.

Common commands:

builder build - build the software
builder rel2  - build the software, and copy it into the "rel2" release tree
builder clean - erase object files, exe's, etc. so you can build from scratch

Many of the projects use the "pmake" features of builder (see builder.doc).
To determine what to build, pmake source is in \bld\pmake.

Each makefile has a comment line at the top of the file which is read by pmake.
Most of our lang.ctl files will have a line similar to this:

pmake -d build -h ...

this will cause wmake to be run in every subdirectory where the makefile
contains "build" on the #pmake line.

You can also specify more parmeters to build a smaller subset of files. This
is especially useful if you do not have all required tools/headers/libraries
for all target platforms.

For example:

builder rel2 os_nt

will (generally) build only the NT version of the tools.

A word of warning: running a full build may take upwards of two hours on
a ~1GHz machine. There is a LOT to build! This is not your ol' OS kernel
or a single-host, single-target C/C++ compiler.

It is generally possible to build specific binaries/libraries by going to
their directory and running wmake. For instance to build the OS/2 version
of WLINK you can go to \bld\wl\os2386 and run wmake there (note that the
process won't successfully finish unless several required libraries are built).
Builder is useful for making full builds while running wmake in the right spot
is handy during development.

------------------------------
5) Getting more information:

If you have any further questions (and you will if you're serious), visit
http://www.openwatcom.org/ and subscribe to the Open Watcom newsgroups at
news://news.openwatcom.org/, particularly openwatcom.contributors.
