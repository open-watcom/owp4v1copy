# OS/2 SFX Installer Builder Control file
# =======================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>\os2inst\os2386\packinst.exe <relroot>\bld\build\binp\packinst.exe
    <CPCMD> <devdir>\os2inst\os2386g\install.exe <relroot>\distrib\os2inst\install.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
