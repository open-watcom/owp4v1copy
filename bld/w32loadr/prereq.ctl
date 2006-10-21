# W32LOADR Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f <OWBINDIR>/w32bind
    rm -f <OWBINDIR>/w32bind.exe
    rm -f <OWBINDIR>/os2ldr.exe
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
  [ IFDEF (os_os2 "") <2*> ]
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    wmake -h -f ../os2ldr/makefile prebuild=1
    <CPCMD> w32bind.exe <OWBINDIR>/w32bind.exe
    <CPCMD> os2ldr.exe <OWBINDIR>/os2ldr.exe

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
  [ IFDEF (os_os2 "") <2*> ]
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    wmake -h -f ../os2ldr/makefile prebuild=1
    <CPCMD> w32bind.exe <OWBINDIR>/w32bind.exe
    <CPCMD> os2ldr.exe <OWBINDIR>/os2ldr.exe

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
  [ IFDEF (os_os2 "") <2*> ]
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    wmake -h -f ../os2ldr/makefile prebuild=1
    <CPCMD> w32bind.exe <OWBINDIR>/w32bind.exe
    <CPCMD> os2ldr.exe <OWBINDIR>/os2ldr.exe

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
  [ IFDEF (os_os2 "") <2*> ]
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../linux386/makefile prebuild=1
    wmake -h -f ../os2ldr/makefile prebuild=1
    <CPCMD> w32bind.exe <OWBINDIR>/w32bind
    <CPCMD> os2ldr.exe <OWBINDIR>/os2ldr.exe

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
