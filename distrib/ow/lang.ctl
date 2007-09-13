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
  rm -f setup.inf
  rm -f instarch.lst

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
     mkexezip <RELROOT>/rel2/open-watcom-c-win32-<BLD_VER_STR>.exe <RELROOT>/rel2/csetup.zip <DEVDIR>/setupgui/nt386.gui/csetup.exe

  [ IFDEF (os_os2 "") <2*> ]
     mkexezip <RELROOT>/rel2/open-watcom-c-os2-<BLD_VER_STR>.exe   <RELROOT>/rel2/csetup.zip <DEVDIR>/setupgui/os2386.gui/csetup.exe

  [ IFDEF (os_dos "") <2*> ]
     mkexezip <RELROOT>/rel2/open-watcom-c-dos-<BLD_VER_STR>.exe   <RELROOT>/rel2/csetup.zip <DEVDIR>/setupgui/dos386/setup.exe

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
     mkexezip <RELROOT>/rel2/open-watcom-f77-win32-<BLD_VER_STR>.exe <RELROOT>/rel2/fsetup.zip <DEVDIR>/setupgui/nt386.gui/f77setup.exe

  [ IFDEF (os_os2 "") <2*> ]
     mkexezip <RELROOT>/rel2/open-watcom-f77-os2-<BLD_VER_STR>.exe   <RELROOT>/rel2/fsetup.zip <DEVDIR>/setupgui/os2386.gui/f77setup.exe

  [ IFDEF (os_dos "") <2*> ]
     mkexezip <RELROOT>/rel2/open-watcom-f77-dos-<BLD_VER_STR>.exe   <RELROOT>/rel2/fsetup.zip <DEVDIR>/setupgui/dos386/setup.exe

[ BLOCK . . ]
  # Get setup.inf out of the way
  rm -f <RELROOT>/rel2/setup.inf   
  # Do cdsay to see end time
  cdsay <INSTDIR>

[ BLOCK <1> clean ]
# Clean up

    rm -f filelist instarch.lst setup.inf
    rm -f <RELROOT>/rel2/setup.inf
    rm -f <RELROOT>/rel2/csetup.zip
    rm -f <RELROOT>/rel2/fsetup.zip
