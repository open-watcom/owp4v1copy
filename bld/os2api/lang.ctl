# os2api Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 acprel2 ]
#16-bit stuff
    <CPCMD> incl16\*.h   <relroot>\rel2\h\os21x\
    <CPCMD> os2286\*.lib <relroot>\rel2\lib286\os2\
#32-bit stuff
#   <CPCMD> incl32\*.h   <relroot>\rel2\h\os2\
    <CPCMD> os2386\*.lib <relroot>\rel2\lib386\os2\

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
