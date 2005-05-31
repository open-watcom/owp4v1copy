# setupgui Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
# Note: this is at the moment only set up for one platform at the time. The <distroot>/wc directory
# should be split into subdirs per platform, like the mkdisk dir. This is why the "" is not present
# in the IFDEF lines. Later. 20040201 Mat Nieuwenhoven
  [ IFDEF (os_os2) <2*> ]
    <CPCMD> <PROJDIR>/os2386/csetup.exe <distroot>/wc/csetup/install.exe
    <CPCMD> <PROJDIR>/os2386/virgin.sym <distroot>/wc/csetup/install.sym
    <CPCMD> <PROJDIR>/os2386/f77setup.exe <distroot>/wc/f77setup/install.exe
    <CPCMD> <PROJDIR>/os2386/virgin.sym <distroot>/wc/f77setup/install.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
  [ IFDEF (os_nt) <2*> ]
    <CPCMD> <PROJDIR>/nt386/csetup.exe <distroot>/wc/csetup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/virgin.sym <distroot>/wc/csetup/setup32.sym
    <CPCMD> <PROJDIR>/nt386/f77setup.exe <distroot>/wc/f77setup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/virgin.sym <distroot>/wc/f77setup/setup32.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
  [ IFDEF (os_dos) <2*> ]
    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/csetup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/csetup/dossetup.sym
    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/f77setup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/f77setup/dossetup.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
  [ IFDEF (os_win) <2*> ]
    <CPCMD> <PROJDIR>/wini86/csetup.exe <distroot>/wc/csetup/setup.exe
    <CPCMD> <PROJDIR>/wini86/virgin.sym <distroot>/wc/csetup/setup.sym
    <CPCMD> <PROJDIR>/wini86/f77setup.exe <distroot>/wc/f77setup/setup.exe
    <CPCMD> <PROJDIR>/wini86/virgin.sym <distroot>/wc/f77setup/setup.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
  [ IFDEF (os_linux) <2*> ]
    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/wc/csetup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/wc/csetup/setupl.sym
    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/wc/f77setup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/wc/f77setup/setupl.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/


[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
