#!/bin/sh
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
builder build os_linux
