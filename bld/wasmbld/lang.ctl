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
    @rm -f <relroot>/bld/build/bin/wasm.*
    @rm -f <relroot>/bld/build/binp/wasm.*
    @rm -f <relroot>/bld/build/binnt/wasm.*
    @rm -f <relroot>/bld/build/binl/wasm.*
