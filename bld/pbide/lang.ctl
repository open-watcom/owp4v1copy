# TODO: Restructure directories, redo makefiles. This project is a mess.
#       Parts of it seem obsolete (wfscopy).
#
# PBIDE Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]

    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

    cdsay <DEVDIR>/viper/viper
    wmake -i -h PBIDE

    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/pbide/run/wini86/run.exe <relroot>/rel2/binw/watrun.exe
    <CPCMD> <DEVDIR>/pbide/run/nt386/run.exe <relroot>/rel2/binnt/watrun.exe
    <CPCMD> <DEVDIR>/pbide/dlldbg/wini86/dlldbg.exe <relroot>/rel2/binw/dlldbg.exe
    <CPCMD> <DEVDIR>/pbide/dlldbg/nt386/dlldbg.exe <relroot>/rel2/binnt/dlldbg.exe
    <CPCMD> <DEVDIR>/pbide/dlldbg/pbend.dbg <relroot>/rel2/binw/pbend.dbg
    <CPCMD> <DEVDIR>/pbide/dlldbg/pbstart.dbg <relroot>/rel2/binw/pbstart.dbg
    <CPCMD> <DEVDIR>/pbide/dlldbg/pbwd.dbg <relroot>/rel2/binw/pbwd.dbg
    <CPCMD> <DEVDIR>/pbide/dlldbg/pbwdnt.dbg <relroot>/rel2/binnt/pbwdnt.dbg
    <CPCMD> <DEVDIR>/pbide/wig/wini86.dll/pbide.dll <relroot>/rel2/binw/pbide.dll
    <CPCMD> <DEVDIR>/pbide/wig/nt386.dll/pbide.dll <relroot>/rel2/binnt/pbide.dll
    <CPCMD> <DEVDIR>/pbide/pbdll.h <relroot>/rel2/h/pbdll.h
    <CPCMD> <DEVDIR>/viper/viper/win/pbide.cfg <relroot>/rel2/binw/pbide.cfg
    <CPCMD> <DEVDIR>/viper/viper/nt/pbide.cfg <relroot>/rel2/binnt/pbide.cfg
    <CPCMD> <DEVDIR>/viper/viper/idepb.cfg <relroot>/rel2/binw/idepb.cfg
    <CPCMD> <DEVDIR>/viper/viper/idepbnt.cfg <relroot>/rel2/binnt/idepbnt.cfg
    <CPCMD> <DEVDIR>/viper/viper/win/pbidex.cfg <relroot>/rel2/binw/pbidex.cfg
    <CPCMD> <DEVDIR>/viper/viper/nt/pbidex.cfg <relroot>/rel2/binnt/pbidex.cfg
    <CPCMD> <DEVDIR>/pbide/fscopy/wfscopy.exe <relroot>/rel2/binw/wfscopy.exe

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
