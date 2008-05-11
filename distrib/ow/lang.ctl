# Installer builder control file
# ==============================

set INSTDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <DISTROOT>/<LOGFNAME>.<LOGEXT> ]

cdsay .

echo Installer Build: <1> <2> <3> <4> <5>

[ BLOCK . . ]

# First build installer executables
[ INCLUDE <DEVDIR>/setupgui/lang.ctl ]

# Build installer self-extracting archives
[ BLOCK <1> build rel2 ]
    cdsay .
    rm -f setup.exe
    rm -f instarch.lst
    echo Creating C/C++ setup.inf
    langdat c

  [ IFDEF (os_nt "") <2*> ]
    cp <DEVDIR>/setupgui/nt386.gui/csetup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM c filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-c-win32-<BLD_VER_STR>.exe

  [ IFDEF (os_os2 "") <2*> ]
    cp <DEVDIR>/setupgui/os2386.gui/csetup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM c filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-c-os2-<BLD_VER_STR>.exe

  [ IFDEF (os_dos "") <2*> ]
    cp <DEVDIR>/setupgui/dos386/setup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM c filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-c-dos-<BLD_VER_STR>.exe

  [ IFDEF (os_linux "") <2*> ]
    cp <DEVDIR>/setupgui/linux386/setup.exe setup.exe
    mksetup -i../include -dDstDir= c filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-c-linux-<BLD_VER_STR>

[ BLOCK <1> build rel2 ]
    echo Creating C/C++ installer archive
    # Have to switch to rel2 to ensure paths inside archive are correct
    cp setup.inf <RELROOT>/setup.inf
    rm -f <RELROOT>/csetup.zip
    uzip <RELROOT>/csetup.zip instarch.lst <RELROOT>
    mkexezip <INSTALLER> <RELROOT>/csetup.zip setup.exe

[ BLOCK <1> build rel2 ]
    rm -f setup.exe
    rm -f instarch.lst
    echo Creating Fortran setup.inf
    langdat f77

  [ IFDEF (os_nt "") <2*> ]
    cp <DEVDIR>/setupgui/nt386.gui/f77setup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM f77 filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-f77-win32-<BLD_VER_STR>.exe

  [ IFDEF (os_os2 "") <2*> ]
    cp <DEVDIR>/setupgui/os2386.gui/f77setup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM f77 filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-f77-os2-<BLD_VER_STR>.exe

  [ IFDEF (os_dos "") <2*> ]
    cp <DEVDIR>/setupgui/dos386/setup.exe setup.exe
    mksetup -i../include -dDstDir=C:\WATCOM f77 filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-f77-dos-<BLD_VER_STR>.exe

  [ IFDEF (os_linux "") <2*> ]
    cp <DEVDIR>/setupgui/linux386/setup.exe setup.exe
    mksetup -i../include -dDstDir= f77 filelist <RELROOT>
    set INSTALLER=<RELROOT>/open-watcom-f77-linux-<BLD_VER_STR>

[ BLOCK <1> build rel2 ]
    echo Creating Fortran installer archive
    # Have to switch to rel2 to ensure paths inside archive are correct
    cp setup.inf <RELROOT>/setup.inf
    rm -f <RELROOT>/fsetup.zip
    uzip <RELROOT>/fsetup.zip instarch.lst <RELROOT>
    mkexezip <INSTALLER> <RELROOT>/fsetup.zip setup.exe

[ BLOCK <1> missing ]
    langdat c
    mksetup -x -i../include c filelist <RELROOT>
    langdat f77
    mksetup -x -i../include f77 filelist <RELROOT>

[ BLOCK . . ]
    # Get setup.inf out of the way
    rm -f <RELROOT>/setup.inf   
    # Do cdsay to see end time
    cdsay <INSTDIR>

[ BLOCK <1> clean ]
    # Clean up
    rm -f filelist instarch.lst setup.inf setup.exe
    rm -f <RELROOT>/setup.inf
    rm -f <RELROOT>/csetup.zip
    rm -f <RELROOT>/fsetup.zip
