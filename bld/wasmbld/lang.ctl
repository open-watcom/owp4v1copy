# WASM Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    [ INCLUDE prereq.ctl ]

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    @rm -f <RELROOT>/bld/build/bin/wasm.*
    @rm -f <RELROOT>/bld/build/binp/wasm.*
    @rm -f <RELROOT>/bld/build/binnt/wasm.*
    @rm -f <RELROOT>/bld/build/binl/wasm.*
