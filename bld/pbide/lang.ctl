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

    cdsay <devdir>/viper/viper
    wmake -i -h PBIDE

    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>/pbide/run/wini86/run.exe <relroot>/rel2/binw/watrun.exe
    <CPCMD> <devdir>/pbide/run/nt386/runnt.exe <relroot>/rel2/binnt/watrun.exe
    <CPCMD> <devdir>/pbide/dlldbg/wini86/dlldbg.exe <relroot>/rel2/binw/dlldbg.exe
    <CPCMD> <devdir>/pbide/dlldbg/nt386/dlldbg.exe <relroot>/rel2/binnt/dlldbg.exe
    <CPCMD> <devdir>/pbide/dlldbg/pbend.dbg <relroot>/rel2/binw/pbend.dbg
    <CPCMD> <devdir>/pbide/dlldbg/pbstart.dbg <relroot>/rel2/binw/pbstart.dbg
    <CPCMD> <devdir>/pbide/dlldbg/pbwd.dbg <relroot>/rel2/binw/pbwd.dbg
    <CPCMD> <devdir>/pbide/dlldbg/pbwdnt.dbg <relroot>/rel2/binnt/pbwdnt.dbg
    <CPCMD> <devdir>/pbide/wig/owindll/pbide.dll <relroot>/rel2/binw/pbide.dll
    <CPCMD> <devdir>/pbide/wig/ontdll/pbide.dll <relroot>/rel2/binnt/pbide.dll
    <CPCMD> <devdir>/pbide/pbdll.h <relroot>/rel2/h/pbdll.h
    <CPCMD> <devdir>/viper/viper/win/pbide.cfg <relroot>/rel2/binw/pbide.cfg
    <CPCMD> <devdir>/viper/viper/nt/pbide.cfg <relroot>/rel2/binnt/pbide.cfg
    <CPCMD> <devdir>/viper/viper/idepb.cfg <relroot>/rel2/binw/idepb.cfg
    <CPCMD> <devdir>/viper/viper/idepbnt.cfg <relroot>/rel2/binnt/idepbnt.cfg
    <CPCMD> <devdir>/viper/viper/win/pbidex.cfg <relroot>/rel2/binw/pbidex.cfg
    <CPCMD> <devdir>/viper/viper/nt/pbidex.cfg <relroot>/rel2/binnt/pbidex.cfg
    <CPCMD> <devdir>/pbide/fscopy/wfscopy.exe <relroot>/rel2/binw/wfscopy.exe

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
