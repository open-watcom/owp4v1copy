# Graphlib Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ INCLUDE prereq.ctl ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>/../pgchart
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/fix
    pmake -d buildnext <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/graphlib/fix/dosi86/graph.lib    <RELROOT>/rel2/lib286/dos/graph.lib
    <CPCMD> <DEVDIR>/graphlib/fix/dos386/graph.lib    <RELROOT>/rel2/lib386/dos/graph.lib
    <CPCMD> <DEVDIR>/graphlib/fix/os2i86/graphp.obj   <RELROOT>/rel2/lib286/os2/graphp.obj
#    <CPCMD> <DEVDIR>/graphlib/fix/qnxi86/graph.lib   <RELROOT>/rel2/lib286/qnx/graph.lib
#    <CPCMD> <DEVDIR>/graphlib/fix/qnx386/graph3r.lib  <RELROOT>/rel2/lib386/qnx/graph3r.lib
#    <CPCMD> <DEVDIR>/graphlib/fix/qnx386/graph3s.lib  <RELROOT>/rel2/lib386/qnx/graph3s.lib

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>/../pgchart
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/fix
    pmake -d buildnext <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>
