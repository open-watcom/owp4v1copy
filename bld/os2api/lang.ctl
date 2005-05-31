# os2api Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#16-bit stuff
    <CPCMD> incl16/*.h          <RELROOT>/rel2/h/os21x/
    <CPCMD> os2286/*.lib        <RELROOT>/rel2/lib286/os2/
#32-bit stuff
    <CPCMD> incl32/*.h          <RELROOT>/rel2/h/os2/
    <CPCMD> incl32/arpa/*.h     <RELROOT>/rel2/h/os2/arpa/
    <CPCMD> incl32/machine/*.h  <RELROOT>/rel2/h/os2/machine/
    <CPCMD> incl32/net/*.h      <RELROOT>/rel2/h/os2/net/
    <CPCMD> incl32/netinet/*.h  <RELROOT>/rel2/h/os2/netinet/
    <CPCMD> incl32/protocol/*.h <RELROOT>/rel2/h/os2/protocol/
    <CPCMD> incl32/sys/*.h      <RELROOT>/rel2/h/os2/sys/
    <CPCMD> incl32/GL/*.h       <RELROOT>/rel2/h/os2/GL/
    <CPCMD> os2386/*.lib        <RELROOT>/rel2/lib386/os2/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
