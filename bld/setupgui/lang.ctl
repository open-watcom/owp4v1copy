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
    <CPCMD> <PROJDIR>/mkdisk/mkexezip.exe <OWBINDIR>/
    <CPCMD> <PROJDIR>/mkdisk/uzip.exe <OWBINDIR>/

    <CPCMD> <PROJDIR>/os2386.gui/csetup.exe <distroot>/ow/os2386.gui/csetup/install.exe
    <CPCMD> <PROJDIR>/os2386.gui/virgin.sym <distroot>/ow/os2386.gui/csetup/install.sym
    <CPCMD> <PROJDIR>/os2386.gui/f77setup.exe <distroot>/ow/os2386.gui/f77setup/install.exe
    <CPCMD> <PROJDIR>/os2386.gui/virgin.sym <distroot>/ow/os2386.gui/f77setup/install.sym

    <CPCMD> <PROJDIR>/nt386/csetup.exe <distroot>/ow/nt386/csetup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/virgin.sym <distroot>/ow/nt386/csetup/setup32.sym
    <CPCMD> <PROJDIR>/nt386/f77setup.exe <distroot>/ow/nt386/f77setup/setup32.exe
    <CPCMD> <PROJDIR>/nt386/virgin.sym <distroot>/ow/nt386/f77setup/setup32.sym

    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/ow/dos386/csetup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/ow/dos386/csetup/dossetup.sym
    <CPCMD> <PROJDIR>/dos386/setup.exe <distroot>/ow/dos386/f77setup/dossetup.exe
    <CPCMD> <PROJDIR>/dos386/setup.sym <distroot>/ow/dos386/f77setup/dossetup.sym

    <CPCMD> <PROJDIR>/wini86/csetup.exe <distroot>/ow/wini86/csetup/setup.exe
    <CPCMD> <PROJDIR>/wini86/virgin.sym <distroot>/ow/wini86/csetup/setup.sym
    <CPCMD> <PROJDIR>/wini86/f77setup.exe <distroot>/ow/wini86/f77setup/setup.exe
    <CPCMD> <PROJDIR>/wini86/virgin.sym <distroot>/ow/wini86/f77setup/setup.sym

    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/ow/linux386/csetup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/ow/linux386/csetup/setupl.sym
    <CPCMD> <PROJDIR>/linux386/setup.exe <distroot>/ow/linux386/f77setup/setupl
    <CPCMD> <PROJDIR>/linux386/setup.sym <distroot>/ow/linux386/f77setup/setupl.sym

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
