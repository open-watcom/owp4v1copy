# setupgui Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
# C/C++
    <CPCMD> <PROJDIR>/os2386/csetup.exe <distroot>/wc/csetup/install.exe
    <CPCMD> <PROJDIR>/os2386/setup.sym <distroot>/wc/csetup/install.sym

    <CPCMD> <PROJDIR>/nt386/csetup.exe <distroot>/wc/csetup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/csetup.sym <distroot>/wc/csetup/setup32.sym

    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/csetup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/csetup/dossetup.sym

    <CPCMD> <PROJDIR>/wini86/csetup.exe <distroot>/wc/csetup/setup.exe
    <CPCMD> <PROJDIR>/wini86/setup.sym <distroot>/wc/csetup/setup.sym

    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/wc/csetup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/wc/csetup/setupl.sym

#fortran
    <CPCMD> <PROJDIR>/os2386/f77setup.exe <distroot>/wc/f77setup/install.exe
    <CPCMD> <PROJDIR>/os2386/setup.sym <distroot>/wc/f77setup/install.sym

    <CPCMD> <PROJDIR>/nt386/f77setup.exe <distroot>/wc/f77setup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/f77setup.sym <distroot>/wc/f77setup/setup32.sym

    <CPCMD> <PROJDIR>/wini86/f77setup.exe <distroot>/wc/f77setup/setup.exe
    <CPCMD> <PROJDIR>/wini86/setup.sym <distroot>/wc/f77setup/setup.sym

    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/wc/f77setup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/wc/f77setup/dossetup.sym

    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/wc/f77setup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/wc/f77setup/setupl.sym

    <CPCMD> <PROJDIR>/mkdisk/*.exe <distroot>/supp/
    
[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
