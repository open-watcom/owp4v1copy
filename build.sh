#!/bin/sh
if [ -f setvars ]; then
    . setvars
else
    . setvars.sh
fi
if [ ! -d $WATCOM/lib386/linux ]; then
    mkdir -p $WATCOM/lib386/linux $WATCOM/binw
    ln -s $OWROOT/bld/wl/wlsystem.lnk $WATCOM/binw/wlsystem.lnk
    ln -s $OWROOT/bld/hdr/linux $WATCOM/lh
    ln -s $OWROOT/bld/hdr/dos $WATCOM/h
    ln -s $OWROOT/bld/mathlib/library/msdos.387/ms_r/maths.lib $WATCOM/lib386/math387r.lib
    ln -s $OWROOT/bld/mathlib/library/msdos.387/ms_s/maths.lib $WATCOM/lib386/math387s.lib
    ln -s $OWROOT/bld/mathlib/library/msdos.386/ms_r/maths.lib $WATCOM/lib386/math3r.lib
    ln -s $OWROOT/bld/mathlib/library/msdos.386/ms_s/maths.lib $WATCOM/lib386/math3s.lib
    ln -s $OWROOT/bld/clib/library/linux.386/ms_r/clibs.lib $WATCOM/lib386/linux/clib3r.lib
    ln -s $OWROOT/bld/clib/library/linux.386/ms_s/clibs.lib $WATCOM/lib386/linux/clib3s.lib
fi
mkdir -p $DEVDIR/build/binl
if [ ! -f $DEVDIR/build/binl/wtouch ]; then
    ln -s `which touch` $DEVDIR/build/binl/wtouch
fi
cd bld/clib
$MAKE -f gnumake
cd ../builder
$MAKE -f gnumake
cp $OBJDIR/builder ../build/binl
cd ..
builder build os_linux
