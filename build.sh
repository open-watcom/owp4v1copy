#!/bin/sh
#
# Build script to bootstrap build the Linux version of the compiler
# using the GNU C/C++ compiler tools. If you already have a working
# Open Watcom compiler, you do not need to use the bootstrap process

if [ -f setvars ]; then
    . setvars
else
    . setvars.sh
fi
if [ ! -f $DEVDIR/build/binl/wtouch ]; then
    cp -p `which touch` $DEVDIR/build/binl/wtouch
fi
cd bld/clib
$MAKE -f gnumake
cd ../builder
$MAKE -f gnumake
cp $OBJDIR/builder ../build/binl
cd ..
export BUILDMODE=bootstrap
builder rel2 os_linux
unset BUILDMODE
