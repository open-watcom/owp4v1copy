set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building the wcc386 bootstrap
    mkdir -p <PROJDIR>/linux386.<OBJDIR>
    cdsay <PROJDIR>/linux386.<OBJDIR>
    cp -f ../linux386/target.h .
    wmake -h -f ../linux386/makefile bootstrap=1
    <CPCMD> wcc386 <devdir>/build/binl/wcc386
    cdsay <PROJDIR>

    echo Building the wcc bootstrap
    mkdir -p <PROJDIR>/linuxi86.<OBJDIR>
    cdsay <PROJDIR>/linuxi86.<OBJDIR>
    cp -f ../linuxi86/target.h .
    wmake -h -f ../linuxi86/makefile bootstrap=1
    <CPCMD> wcci86 <devdir>/build/binl/wcc
    cdsay <PROJDIR>
