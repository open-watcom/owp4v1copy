# setupgui Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    cdsay <PROJDIR>/dos386
    wmake setup.exe
    cdsay <PROJDIR>/os2386
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>/nt386
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>/wini86
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>/mkdisk
    wmake
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
# C/C++
    <CPCMD> <PROJDIR>/os2386/csetup.exe <distroot>/wc/csetup/install.exe
    <CPCMD> <PROJDIR>/nt386/csetup.exe <distroot>/wc/csetup/setup32.exe
    <CPCMD> <PROJDIR>/wini86/csetup.exe <distroot>/wc/csetup/setup.exe
    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/csetup/dossetup.exe
    <CPCMD> <PROJDIR>/os2386/setup.sym <distroot>/wc/csetup/install.sym
    <CPCMD> <PROJDIR>/nt386/setup.sym <distroot>/wc/csetup/setup32.sym
    <CPCMD> <PROJDIR>/wini86/setup.sym <distroot>/wc/csetup/setup.sym
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/csetup/dossetup.sym
#fortran
    <CPCMD> <PROJDIR>/os2386/f77setup.exe <distroot>/wc/f77setup/install.exe
    <CPCMD> <PROJDIR>/nt386/f77setup.exe <distroot>/wc/f77setup/setup32.exe
    <CPCMD> <PROJDIR>/wini86/f77setup.exe <distroot>/wc/f77setup/setup.exe
    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/f77setup/dossetup.exe
    <CPCMD> <PROJDIR>/os2386/setup.sym <distroot>/wc/f77setup/install.sym
    <CPCMD> <PROJDIR>/nt386/setup.sym <distroot>/wc/f77setup/setup32.sym
    <CPCMD> <PROJDIR>/wini86/setup.sym <distroot>/wc/f77setup/setup.sym
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/f77setup/dossetup.sym
    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
    
[ BLOCK <1> clean ]
#==================
    sweep killobjs
