# Graphlib Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>/../pgchart
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/fix
    pmake -d buildnext <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>/graphlib/fix/dosi86/graph.lib    <relroot>/rel2/lib286/dos/graph.lib
    <CPCMD> <devdir>/graphlib/fix/dos386/graph386.lib <relroot>/rel2/lib386/dos/graph.lib
    <CPCMD> <devdir>/graphlib/fix/os2i86/graphp.2bj   <relroot>/rel2/lib286/os2/graphp.obj
    <CPCMD> <devdir>/graphlib/fix/dosi86/graph98.lib  <relroot>/rel2/lib286/dos/graph98.lib
    <CPCMD> <devdir>/graphlib/fix/dos386/graph983.lib <relroot>/rel2/lib386/dos/graph98.lib
    <CPCMD> <devdir>/graphlib/fix/qnxi86/graphq.lib   <relroot>/rel2/lib286/qnx/graph.lib
    <CPCMD> <devdir>/graphlib/fix/qnx386/graphq3r.lib  <relroot>/rel2/lib386/qnx/graph3r.lib
    <CPCMD> <devdir>/graphlib/fix/qnx386/graphq3s.lib  <relroot>/rel2/lib386/qnx/graph3s.lib

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>/../pgchart
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/fix
    pmake -d buildnext <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>
