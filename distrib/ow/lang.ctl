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

[ BLOCK <1> build rel2 ]
# Build installer self-extracting archives 

  [ IFDEF (cinst "") <2*> ]
     echo Creating C/C++ setup.inf
     langdat c
     mksetup -i../include c filelist <RELROOT>/rel2
     echo Creating C/C++ installer archive
     # Have to switch to rel2 to ensure paths inside archive are correct
     cd <RELROOT>/rel2
     copy <INSTDIR>/setup.inf setup.inf
     rm -f csetup.zip
     uzip csetup.zip <INSTDIR>/instarch.lst
     cd <INSTDIR>

  [ IFDEF (os_nt "") <2*> ]
     mkexezip <INSTDIR>/nt386/csetup/setup32.exe <RELROOT>/rel2/csetup.zip

  [ IFDEF (os_os2 "") <2*> ]
     mkexezip <INSTDIR>/os2386.gui/csetup/install.exe <RELROOT>/rel2/csetup.zip

  [ IFDEF (finst "") <2*> ]
     echo Creating Fortran setup.inf
     langdat f77
     mksetup -i../include f77 filelist <RELROOT>/rel2
     echo Creating Fortran installer archive
     # Have to switch to rel2 to ensure paths inside archive are correct
     cd <RELROOT>/rel2
     copy <INSTDIR>/setup.inf setup.inf
     rm -f fsetup.zip
     uzip fsetup.zip <INSTDIR>/instarch.lst
     cd <INSTDIR>

  [ IFDEF (os_nt "") <2*> ]
     mkexezip <INSTDIR>/nt386/f77setup/setup32.exe <RELROOT>/rel2/fsetup.zip

  [ IFDEF (os_os2 "") <2*> ]
     mkexezip <INSTDIR>/os2386.gui/f77setup/install.exe <RELROOT>/rel2/fsetup.zip

[ BLOCK <1> rel2 ]
# Copy installers to final location

  [ IFDEF (os_nt "") <2*> ]
     <CPCMD> <INSTDIR>/nt386/csetup/setup32.exe            <RELROOT>/rel2/open-watcom-c-win32-<BLD_VER_STR>.exe
     <CPCMD> <INSTDIR>/nt386/f77setup/setup32.exe          <RELROOT>/rel2/open-watcom-f77-win32-<BLD_VER_STR>.exe

  [ IFDEF (os_os2 "") <2*> ]
     <CPCMD> <INSTDIR>/os2386.gui/csetup/install.exe       <RELROOT>/rel2/open-watcom-c-os2-<BLD_VER_STR>.exe
     <CPCMD> <INSTDIR>/os2386.gui/f77setup/install.exe     <RELROOT>/rel2/open-watcom-f77-os2-<BLD_VER_STR>.exe

[ BLOCK . . ]
# Do cdsay to see end time
cdsay <INSTDIR>

[ BLOCK <1> clean ]
# Clean up

    rm -f filelist instarch.lst setup.inf
    rm -f <RELROOT>/rel2/setup.inf
    rm -f <RELROOT>/rel2/csetup.zip
    rm -f <RELROOT>/rel2/fsetup.zip
