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
    <CPCMD> <PROJDIR>/dos/bitset         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/complex        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/deque          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/exceptio       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/fstream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/function       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iomanip        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ios            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iosfwd         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iostream       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/istream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/iterator       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/limits         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/list           <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/locale         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/map            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/memory         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/new            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/numeric        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ostream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/queue          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/set            <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/sstream        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/stack          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/stdexcep       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/streambu       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/string         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/strstrea       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/typeinfo       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/utility        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/valarray       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/vector         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/sys/*.h        <RELROOT>/rel2/h/sys/
    <CPCMD> <PROJDIR>/dos/win/*.h        <RELROOT>/rel2/h/win/
    <CPCMD> <PROJDIR>/linux/*.h*         <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/c*.          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/algorith     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/bitset       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/complex      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/deque        <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/exceptio     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/fstream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/function     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iomanip      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ios          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iosfwd       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iostream     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/istream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/iterator     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/limits       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/list         <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/locale       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/map          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/memory       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/new          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/numeric      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ostream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/queue        <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/set          <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/sstream      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/stack        <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/stdexcep     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/streambu     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/string       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/strstrea     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/typeinfo     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/utility      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/valarray     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/vector       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/arpa/*.h     <RELROOT>/rel2/lh/arpa/
    <CPCMD> <PROJDIR>/linux/sys/*.h      <RELROOT>/rel2/lh/sys/
    <CPCMD> <PROJDIR>/linux/netinet/*.h  <RELROOT>/rel2/lh/netinet/
#    <CPCMD> <PROJDIR>/qnx/*.h*           <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/c*             <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/algorith       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/bitset         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/complex        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/deque          <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/exceptio       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/fstream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/function       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iomanip        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/ios            <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iosfwd         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iostream       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/istream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/iterator       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/limits         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/list           <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/locale         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/memory         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/new            <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/numeric        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/ostream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/queue          <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/set            <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/sstream        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/stack          <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/stdexcep       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/streambu       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/string         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/strstrea       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/typeinfo       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/utility        <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/valarray       <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/vector         <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/sys/*.h        <RELROOT>/rel2/qh/sys/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
