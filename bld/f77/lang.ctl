# WFC Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    cdsay <PROJDIR>/lg86/utils
    wmake -i -h
    cd <PROJDIR>
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#    <CPCMD> <devdir>/f77/cg86/cp/osi386.i86/wfc.exe <relroot>/rel2/binw/wfc.exe
#    <CPCMD> <devdir>/f77/cg86/cp/osi386.386/wfc386.exe <relroot>/rel2/binw/wfc386.exe

    #intel compilers
    <CPCMD> <devdir>/f77/cg86/cp/dos386.i86/wfc.exe <relroot>/rel2/binw/wfc.exe
    <CPCMD> <devdir>/f77/cg86/cp/dos386.i86/wfc.sym <relroot>/rel2/binw/wfc.sym
    <CPCMD> <devdir>/f77/cg86/cp/dos386.386/wfc386.exe <relroot>/rel2/binw/wfc386.exe
    <CPCMD> <devdir>/f77/cg86/cp/dos386.386/wfc386.sym <relroot>/rel2/binw/wfc386.sym
    <CPCMD> <devdir>/f77/cg86/cp/os2386.i86/wfc.exe <relroot>/rel2/binp/wfc.exe
    <CPCMD> <devdir>/f77/cg86/cp/os2386.i86/wfc.sym <relroot>/rel2/binp/wfc.sym
    <CPCMD> <devdir>/f77/cg86/cp/os2386.386/wfc386.exe <relroot>/rel2/binp/wfc386.exe
    <CPCMD> <devdir>/f77/cg86/cp/os2386.386/wfc386.sym <relroot>/rel2/binp/wfc386.sym
    <CPCMD> <devdir>/f77/cg86/cp/nt386.i86/wfc.exe <relroot>/rel2/binnt/wfc.exe
    <CPCMD> <devdir>/f77/cg86/cp/nt386.i86/wfc.sym <relroot>/rel2/binnt/wfc.sym
    <CPCMD> <devdir>/f77/cg86/cp/nt386.386/wfc386.exe <relroot>/rel2/binnt/wfc386.exe
    <CPCMD> <devdir>/f77/cg86/cp/nt386.386/wfc386.sym <relroot>/rel2/binnt/wfc386.sym

    #axp compilers
#    <CPCMD> <devdir>/f77/cg86/cp/os2386.axp/wfcaxp.exe <relroot>/rel2/binp/wfcaxp.exe
#    <CPCMD> <devdir>/f77/cg86/cp/nt386.axp/wfcaxp.exe <relroot>/rel2/binnt/wfcaxp.exe
#    <CPCMD> <devdir>/f77/cg86/cp/ntaxp.axp/wfcaxp.exe <relroot>/rel2/axpnt/wfcaxp.exe

    #intel link utilities
    <CPCMD> <devdir>/f77/wfl/dosi86.i86/wfl.exe <relroot>/rel2/binw/wfl.exe
    <CPCMD> <devdir>/f77/wfl/dosi86.i86/wfl.sym <relroot>/rel2/binw/wfl.sym
    <CPCMD> <devdir>/f77/wfl/dosi86.386/wfl386.exe <relroot>/rel2/binw/wfl386.exe
    <CPCMD> <devdir>/f77/wfl/dosi86.386/wfl386.sym <relroot>/rel2/binw/wfl386.sym
    <CPCMD> <devdir>/f77/wfl/nt386.i86/wfl.exe <relroot>/rel2/binnt/wfl.exe
    <CPCMD> <devdir>/f77/wfl/nt386.i86/wfl.sym <relroot>/rel2/binnt/wfl.sym
    <CPCMD> <devdir>/f77/wfl/nt386.386/wfl386.exe <relroot>/rel2/binnt/wfl386.exe
    <CPCMD> <devdir>/f77/wfl/nt386.386/wfl386.sym <relroot>/rel2/binnt/wfl386.sym
    <CPCMD> <devdir>/f77/wfl/os2386.i86/wfl.exe <relroot>/rel2/binp/wfl.exe
    <CPCMD> <devdir>/f77/wfl/os2386.i86/wfl.sym <relroot>/rel2/binp/wfl.sym
    <CPCMD> <devdir>/f77/wfl/os2386.386/wfl386.exe <relroot>/rel2/binp/wfl386.exe
    <CPCMD> <devdir>/f77/wfl/os2386.386/wfl386.sym <relroot>/rel2/binp/wfl386.sym

    #alpha link utilities
#    <CPCMD> <devdir>/f77/wfl/os2386.axp/wflaxp.exe <relroot>/rel2/binp/wflaxp.exe
#    <CPCMD> <devdir>/f77/wfl/nt386.axp/wflaxp.exe <relroot>/rel2/binnt/wflaxp.exe
#    <CPCMD> <devdir>/f77/wfl/ntaxp.axp/wflaxp.exe <relroot>/rel2/axpnt/wflaxp.exe


    # Libraries, lots and lots of libraries, basic platforms are OS/2, DOS, NT, NETWARE, WIN

    # 32-bit Libraries
    # 32-bit DOS Libraries
    <CPCMD> <devdir>/f77/cg86/rt/386/flib.lib           <relroot>/rel2/lib386/dos/flib.lib
    <CPCMD> <devdir>/f77/cg86/rt/387/flib7.lib          <relroot>/rel2/lib386/dos/flib7.lib
    <CPCMD> <devdir>/f77/cg86/rt/386s/flibs.lib         <relroot>/rel2/lib386/dos/flibs.lib
    <CPCMD> <devdir>/f77/cg86/rt/387s/flib7s.lib        <relroot>/rel2/lib386/dos/flib7s.lib

    # 32-bit NETWARE Libraries
    <CPCMD> <devdir>/f77/cg86/rt/net386s/flibs.lib      <relroot>/rel2/lib386/netware/flibs.lib
    <CPCMD> <devdir>/f77/cg86/rt/net387s/flib7s.lib     <relroot>/rel2/lib386/netware/flib7s.lib

    # NT Libraries
    <CPCMD> <devdir>/f77/cg86/rt/ntr/flib.lib           <relroot>/rel2/lib386/nt/flib.lib
    <CPCMD> <devdir>/f77/cg86/rt/ntr7/flib7.lib         <relroot>/rel2/lib386/nt/flib7.lib
    <CPCMD> <devdir>/f77/cg86/rt/nts/flibs.lib          <relroot>/rel2/lib386/nt/flibs.lib
    <CPCMD> <devdir>/f77/cg86/rt/nts7/flib7s.lib        <relroot>/rel2/lib386/nt/flib7s.lib

    # 32-bit OS/2 Libraries
    <CPCMD> <devdir>/f77/cg86/rt/23r/flib.lib           <relroot>/rel2/lib386/os2/flib.lib
    <CPCMD> <devdir>/f77/cg86/rt/23r7/flib7.lib         <relroot>/rel2/lib386/os2/flib7.lib
    <CPCMD> <devdir>/f77/cg86/rt/23s/flibs.lib          <relroot>/rel2/lib386/os2/flibs.lib
    <CPCMD> <devdir>/f77/cg86/rt/23s7/flib7s.lib        <relroot>/rel2/lib386/os2/flib7s.lib

    # 32-bit Windows Libraries
    <CPCMD> <devdir>/f77/cg86/rt/w386/flib.lib          <relroot>/rel2/lib386/win/flib.lib
    <CPCMD> <devdir>/f77/cg86/rt/w387/flib7.lib         <relroot>/rel2/lib386/win/flib7.lib
    <CPCMD> <devdir>/f77/cg86/rt/w386s/flibs.lib        <relroot>/rel2/lib386/win/flibs.lib
    <CPCMD> <devdir>/f77/cg86/rt/w387s/flib7s.lib       <relroot>/rel2/lib386/win/flib7s.lib

    # 16-bit Libraries
    # 16-bit DOS Libraries
    <CPCMD> <devdir>/f77/cg86/rt/bcbd/flibl.lib         <relroot>/rel2/lib286/dos/flibl.lib
    <CPCMD> <devdir>/f77/cg86/rt/bcbd7/flib7l.lib       <relroot>/rel2/lib286/dos/flib7l.lib
    <CPCMD> <devdir>/f77/cg86/rt/bcsd/flibm.lib         <relroot>/rel2/lib286/dos/flibm.lib
    <CPCMD> <devdir>/f77/cg86/rt/bcsd7/flib7m.lib       <relroot>/rel2/lib286/dos/flib7m.lib

    # 16-bit OS/2 Libraries
    <CPCMD> <devdir>/f77/cg86/rt/2bcbd/flibl.lib        <relroot>/rel2/lib286/os2/flibl.lib
    <CPCMD> <devdir>/f77/cg86/rt/2bcbd7/flib7l.lib      <relroot>/rel2/lib286/os2/flib7l.lib
    <CPCMD> <devdir>/f77/cg86/rt/2bcsd/flibm.lib        <relroot>/rel2/lib286/os2/flibm.lib
    <CPCMD> <devdir>/f77/cg86/rt/2bcsd7/flib7m.lib      <relroot>/rel2/lib286/os2/flib7m.lib

    # 16-bit Windows Libraries
    <CPCMD> <devdir>/f77/cg86/rt/wbcbd/flibl.lib        <relroot>/rel2/lib286/win/flibl.lib
    <CPCMD> <devdir>/f77/cg86/rt/wbcbd7/flib7l.lib      <relroot>/rel2/lib286/win/flib7l.lib
    <CPCMD> <devdir>/f77/cg86/rt/wbcsd/flibm.lib        <relroot>/rel2/lib286/win/flibm.lib
    <CPCMD> <devdir>/f77/cg86/rt/wbcsd7/flib7m.lib      <relroot>/rel2/lib286/win/flib7m.lib

[ BLOCK <1> clean ]
#==================
    cdsay <PROJDIR>/lg86/utils
    wmake -i -h clean
    cd <PROJDIR>
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
