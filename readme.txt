
  Getting up to Speed on OpenWatcom
  =================================

Here's a quick intro on the build layout and build process as
they used to be at Watcom/Sybase. The OpenWatcom layout is
slightly different and many parts of the build process are broken
due to removal of third-party tools, headers and libraries.

1) build machine layout:

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

    tools - utilities and 3rd party tools - not in OpenWatcom

    lang  - used to hold Watcom 10.6 compiler

    nlang - used to hold intermediate version of 11.0 compiler;
            for OpenWatcom these both point to the same place
            where Watcom 11.0c lives


------------------------------
2) to set up a new machine:

  - modify setvars.bat/setvars.cmd to reflect your setup

d:\bat\makeinit, d:\bat\makecomm may have some machine specific info,
    but should be ok if you don't change any locations

    your path should look something like this if you are on NT:

    PATH=C:\WINNT\system32;
    C:\WINNT;
    d:\bat;
    d:\lang\BINNT;
    d:\lang\BINW;
    d:\bin;
    d:\tools;
    d:\bld\build\binnt

------------------------------
3) priming the pump

Besides the Watcom C/C++ 11.0c binaries you'll need a few other executables
used by the build process, primarily builder and pmake. Their source is in
\bld\builder and \bld\pmake respectively. Both can be built by running wmake
in the appropriate directory. The executables should be copied to a
subdirectory of \bld\build appropriate to your host platform. Perhaps
someone will automate this process. For now consider it an aptitude test.

------------------------------
4) build process

We use the watcom c/c++ compilers and watcom wmake to build our tools,
but at the top level we have a tool which oversees traversing the build tree,
deciding which projects to build for what platforms, logging the results to
a file, and copying the finished software into the release tree (rel2)

This tool is BUILDER.EXE

see d:\bld\builder\builder.doc for detailed info on the tool

here's how we use it:

each project has a "lang.ctl" builder script file
if you go to a project directory and run builder, it will make only that project
if you go to \bld and run builder, it will build everything
the overall build uses \bat\lang.ctl which includes all of the individual
project lang.ctl files that we use

results are logged to "build.log" in the current project directory (or d:\bld)
the previous build.log file is copied to build.lo1

builder build - build the software
builder rel2  - build the software, and copy it into the "rel2" release tree
builder clean - erase object files, exe's, etc. so you can build from scratch

many of the projects use the "pmake" features of builder (see builder.doc)
to determine what to build, pmake source is in \bld\pmake.

Each makefile has a comment line at the top of the file which is read by pmake

most of our lang.ctl files will have a line similar to this:

pmake -d build -h ...

this will cause wmake to be run in every subdirectory where the makefile
contains "build" on the #pmake line

you can also specify more parmeters to build a smaller subset of files.

For example:

builder rel2 os_nt will (generally) build only the NT version of the tools

It is generally possible to build specific binaries/libraries by going to
their directory and running wmake. For instance to build the OS/2 version
of WLINK you can go to \bld\wl\os2386 and run wmake there (note that the
process won't successfully finish unless several needed libraries are built).
Builder is useful for making full builds while running wmake in the right spot
is useful during development.
