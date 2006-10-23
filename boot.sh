#!/bin/sh
#
# Build script to build a subset of the Open Watcom tools
# using the GNU C/C++ compiler tools.

if [ -f setvars ]; then
    . setvars
else
    . setvars.sh
fi
if [ ! -f $DEVDIR/build/binl/wtouch ]; then
    cp -p `which touch` $DEVDIR/build/binl/wtouch
fi
cd bld/wmake
$MAKE -f gnumake
mkdir ../builder/$OBJDIR
cd ../builder/$OBJDIR
wmake -h -f ../linux386/makefile builder.exe bootstrap=1
cd ../..
builder -s -c boot.ctl boot 
