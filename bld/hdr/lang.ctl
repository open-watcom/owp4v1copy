# HDR Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
    <CPCMD> <PROJDIR>/dos/*.h*           <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/*.cnv          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/c*.            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/algorith       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/fstream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/function       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iomanip        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ios            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iosfwd         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iostream       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/istream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iterator       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/limits         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/memory         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ostream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/streambu       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/string         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/strstrea       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/utility        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/sys/*.h        <RELROOT>/rel2/h/sys/
    <CPCMD> <PROJDIR>/dos/win/*.h        <RELROOT>/rel2/h/win/
    <CPCMD> <PROJDIR>/linux/*.h*         <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/c*.          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/algorith     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/fstream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/function     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iomanip      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ios          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iosfwd       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iostream     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/istream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iterator     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/limits       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/memory       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ostream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/streambu     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/string       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/strstrea     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/utility      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/arpa/*.h     <RELROOT>/rel2/lh/arpa/
    <CPCMD> <PROJDIR>/linux/sys/*.h      <RELROOT>/rel2/lh/sys/
    <CPCMD> <PROJDIR>/linux/netinet/*.h  <RELROOT>/rel2/lh/netinet/
#    <CPCMD> <PROJDIR>/qnx/*.h*           <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/c*             <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/algorith       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/fstream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/function       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iomanip        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/ios            <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iosfwd         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iostream       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/istream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iterator       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/limits         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/memory         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/ostream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/streambu       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/string         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/strstrea       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/utility        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/sys/*.h        <RELROOT>/rel2/qh/sys/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
