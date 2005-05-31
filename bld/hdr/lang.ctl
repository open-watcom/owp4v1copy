# HDR Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
    <CPCMD> <PROJDIR>/dos/*.h*           <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/*.cnv          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/sys/*.h        <RELROOT>/rel2/h/sys/
    <CPCMD> <PROJDIR>/dos/win/*.h        <RELROOT>/rel2/h/win/
    <CPCMD> <PROJDIR>/dos/algorith       <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/bitset         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cassert        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cctype         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cerrno         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cfloat         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ciso646        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/climits        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/clocale        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cmath          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/complex        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/csetjmp        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/csignal        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cstdarg        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cstddef        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cstdio         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cstdlib        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cstring        <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/ctime          <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cwchar         <RELROOT>/rel2/h/
    <CPCMD> <PROJDIR>/dos/cwctype        <RELROOT>/rel2/h/
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

    <CPCMD> <PROJDIR>/linux/*.h*         <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/arpa/*.h     <RELROOT>/rel2/lh/arpa/
    <CPCMD> <PROJDIR>/linux/sys/*.h      <RELROOT>/rel2/lh/sys/
    <CPCMD> <PROJDIR>/linux/netinet/*.h  <RELROOT>/rel2/lh/netinet/
    <CPCMD> <PROJDIR>/linux/algorith     <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/bitset       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cassert      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cctype       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cerrno       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cfloat       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ciso646      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/climits      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/clocale      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cmath        <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/complex      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/csetjmp      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/csignal      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cstdarg      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cstddef      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cstdio       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cstdlib      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cstring      <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/ctime        <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cwchar       <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/cwctype      <RELROOT>/rel2/lh/
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

    <CPCMD> <PROJDIR>/linux/arch/i386/*.h       <RELROOT>/rel2/lh/arch/i386/
    <CPCMD> <PROJDIR>/linux/arch/i386/sys/*.h   <RELROOT>/rel2/lh/arch/i386/sys/
    <CPCMD> <PROJDIR>/linux/arch/mips/*.h       <RELROOT>/rel2/lh/arch/mips/
    <CPCMD> <PROJDIR>/linux/arch/mips/sys/*.h   <RELROOT>/rel2/lh/arch/mips/sys/

#    <CPCMD> <PROJDIR>/qnx/*.h*           <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/sys/*.h        <RELROOT>/rel2/qh/sys/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
