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
    <CPCMD> <PROJDIR>/dos/sys/*.h        <RELROOT>/rel2/h/sys/
    <CPCMD> <PROJDIR>/dos/win/*.h        <RELROOT>/rel2/h/win/
    <CPCMD> <PROJDIR>/dos/algorith.gh    <RELROOT>/rel2/h/algorith
    <CPCMD> <PROJDIR>/dos/bitset.gh      <RELROOT>/rel2/h/bitset
    <CPCMD> <PROJDIR>/dos/cassert.gh     <RELROOT>/rel2/h/cassert
    <CPCMD> <PROJDIR>/dos/cctype.gh      <RELROOT>/rel2/h/cctype
    <CPCMD> <PROJDIR>/dos/cerrno.gh      <RELROOT>/rel2/h/cerrno
    <CPCMD> <PROJDIR>/dos/cfloat.gh      <RELROOT>/rel2/h/cfloat
    <CPCMD> <PROJDIR>/dos/ciso646.gh     <RELROOT>/rel2/h/ciso646
    <CPCMD> <PROJDIR>/dos/climits.gh     <RELROOT>/rel2/h/climits
    <CPCMD> <PROJDIR>/dos/clocale.gh     <RELROOT>/rel2/h/clocale
    <CPCMD> <PROJDIR>/dos/cmath.gh       <RELROOT>/rel2/h/cmath
    <CPCMD> <PROJDIR>/dos/complex.gh     <RELROOT>/rel2/h/complex
    <CPCMD> <PROJDIR>/dos/csetjmp.gh     <RELROOT>/rel2/h/csetjmp
    <CPCMD> <PROJDIR>/dos/csignal.gh     <RELROOT>/rel2/h/csignal
    <CPCMD> <PROJDIR>/dos/cstdarg.gh     <RELROOT>/rel2/h/cstdarg
    <CPCMD> <PROJDIR>/dos/cstddef.gh     <RELROOT>/rel2/h/cstddef
    <CPCMD> <PROJDIR>/dos/cstdio.gh      <RELROOT>/rel2/h/cstdio
    <CPCMD> <PROJDIR>/dos/cstdlib.gh     <RELROOT>/rel2/h/cstdlib
    <CPCMD> <PROJDIR>/dos/cstring.gh     <RELROOT>/rel2/h/cstring
    <CPCMD> <PROJDIR>/dos/ctime.gh       <RELROOT>/rel2/h/ctime
    <CPCMD> <PROJDIR>/dos/cwchar.gh      <RELROOT>/rel2/h/cwchar
    <CPCMD> <PROJDIR>/dos/cwctype.gh     <RELROOT>/rel2/h/cwctype
    <CPCMD> <PROJDIR>/dos/deque.gh       <RELROOT>/rel2/h/deque
    <CPCMD> <PROJDIR>/dos/exceptio.gh    <RELROOT>/rel2/h/exceptio
    <CPCMD> <PROJDIR>/dos/fstream.gh     <RELROOT>/rel2/h/fstream
    <CPCMD> <PROJDIR>/dos/function.gh    <RELROOT>/rel2/h/function
    <CPCMD> <PROJDIR>/dos/iomanip.gh     <RELROOT>/rel2/h/iomanip
    <CPCMD> <PROJDIR>/dos/ios.gh         <RELROOT>/rel2/h/ios
    <CPCMD> <PROJDIR>/dos/iosfwd.gh      <RELROOT>/rel2/h/iosfwd
    <CPCMD> <PROJDIR>/dos/iostream.gh    <RELROOT>/rel2/h/iostream
    <CPCMD> <PROJDIR>/dos/istream.gh     <RELROOT>/rel2/h/istream
    <CPCMD> <PROJDIR>/dos/iterator.gh    <RELROOT>/rel2/h/iterator
    <CPCMD> <PROJDIR>/dos/limits.gh      <RELROOT>/rel2/h/limits
    <CPCMD> <PROJDIR>/dos/list.gh        <RELROOT>/rel2/h/list
    <CPCMD> <PROJDIR>/dos/locale.gh      <RELROOT>/rel2/h/locale
    <CPCMD> <PROJDIR>/dos/map.gh         <RELROOT>/rel2/h/map
    <CPCMD> <PROJDIR>/dos/memory.gh      <RELROOT>/rel2/h/memory
    <CPCMD> <PROJDIR>/dos/new.gh         <RELROOT>/rel2/h/new
    <CPCMD> <PROJDIR>/dos/numeric.gh     <RELROOT>/rel2/h/numeric
    <CPCMD> <PROJDIR>/dos/ostream.gh     <RELROOT>/rel2/h/ostream
    <CPCMD> <PROJDIR>/dos/queue.gh       <RELROOT>/rel2/h/queue
    <CPCMD> <PROJDIR>/dos/set.gh         <RELROOT>/rel2/h/set
    <CPCMD> <PROJDIR>/dos/sstream.gh     <RELROOT>/rel2/h/sstream
    <CPCMD> <PROJDIR>/dos/stack.gh       <RELROOT>/rel2/h/stack
    <CPCMD> <PROJDIR>/dos/stdexcep.gh    <RELROOT>/rel2/h/stdexcep
    <CPCMD> <PROJDIR>/dos/streambu.gh    <RELROOT>/rel2/h/streambu
    <CPCMD> <PROJDIR>/dos/string.gh      <RELROOT>/rel2/h/string
    <CPCMD> <PROJDIR>/dos/strstrea.gh    <RELROOT>/rel2/h/strstrea
    <CPCMD> <PROJDIR>/dos/typeinfo.gh    <RELROOT>/rel2/h/typeinfo
    <CPCMD> <PROJDIR>/dos/utility.gh     <RELROOT>/rel2/h/utility
    <CPCMD> <PROJDIR>/dos/valarray.gh    <RELROOT>/rel2/h/valarray
    <CPCMD> <PROJDIR>/dos/vector.gh      <RELROOT>/rel2/h/vector

    <CPCMD> <PROJDIR>/linux/*.h*         <RELROOT>/rel2/lh/
    <CPCMD> <PROJDIR>/linux/arpa/*.h     <RELROOT>/rel2/lh/arpa/
    <CPCMD> <PROJDIR>/linux/sys/*.h      <RELROOT>/rel2/lh/sys/
    <CPCMD> <PROJDIR>/linux/netinet/*.h  <RELROOT>/rel2/lh/netinet/
    <CPCMD> <PROJDIR>/linux/algorith.gh  <RELROOT>/rel2/lh/algorith
    <CPCMD> <PROJDIR>/linux/bitset.gh    <RELROOT>/rel2/lh/bitset
    <CPCMD> <PROJDIR>/linux/cassert.gh   <RELROOT>/rel2/lh/cassert
    <CPCMD> <PROJDIR>/linux/cctype.gh    <RELROOT>/rel2/lh/cctype
    <CPCMD> <PROJDIR>/linux/cerrno.gh    <RELROOT>/rel2/lh/cerrno
    <CPCMD> <PROJDIR>/linux/cfloat.gh    <RELROOT>/rel2/lh/cfloat
    <CPCMD> <PROJDIR>/linux/ciso646.gh   <RELROOT>/rel2/lh/ciso646
    <CPCMD> <PROJDIR>/linux/climits.gh   <RELROOT>/rel2/lh/climits
    <CPCMD> <PROJDIR>/linux/clocale.gh   <RELROOT>/rel2/lh/clocale
    <CPCMD> <PROJDIR>/linux/cmath.gh     <RELROOT>/rel2/lh/cmath
    <CPCMD> <PROJDIR>/linux/complex.gh   <RELROOT>/rel2/lh/complex
    <CPCMD> <PROJDIR>/linux/csetjmp.gh   <RELROOT>/rel2/lh/csetjmp
    <CPCMD> <PROJDIR>/linux/csignal.gh   <RELROOT>/rel2/lh/csignal
    <CPCMD> <PROJDIR>/linux/cstdarg.gh   <RELROOT>/rel2/lh/cstdarg
    <CPCMD> <PROJDIR>/linux/cstddef.gh   <RELROOT>/rel2/lh/cstddef
    <CPCMD> <PROJDIR>/linux/cstdio.gh    <RELROOT>/rel2/lh/cstdio
    <CPCMD> <PROJDIR>/linux/cstdlib.gh   <RELROOT>/rel2/lh/cstdlib
    <CPCMD> <PROJDIR>/linux/cstring.gh   <RELROOT>/rel2/lh/cstring
    <CPCMD> <PROJDIR>/linux/ctime.gh     <RELROOT>/rel2/lh/ctime
    <CPCMD> <PROJDIR>/linux/cwchar.gh    <RELROOT>/rel2/lh/cwchar
    <CPCMD> <PROJDIR>/linux/cwctype.gh   <RELROOT>/rel2/lh/cwctype
    <CPCMD> <PROJDIR>/linux/deque.gh     <RELROOT>/rel2/lh/deque
    <CPCMD> <PROJDIR>/linux/exceptio.gh  <RELROOT>/rel2/lh/exceptio
    <CPCMD> <PROJDIR>/linux/fstream.gh   <RELROOT>/rel2/lh/fstream
    <CPCMD> <PROJDIR>/linux/function.gh  <RELROOT>/rel2/lh/function
    <CPCMD> <PROJDIR>/linux/iomanip.gh   <RELROOT>/rel2/lh/iomanip
    <CPCMD> <PROJDIR>/linux/ios.gh       <RELROOT>/rel2/lh/ios
    <CPCMD> <PROJDIR>/linux/iosfwd.gh    <RELROOT>/rel2/lh/iosfwd
    <CPCMD> <PROJDIR>/linux/iostream.gh  <RELROOT>/rel2/lh/iostream
    <CPCMD> <PROJDIR>/linux/istream.gh   <RELROOT>/rel2/lh/istream
    <CPCMD> <PROJDIR>/linux/iterator.gh  <RELROOT>/rel2/lh/iterator
    <CPCMD> <PROJDIR>/linux/limits.gh    <RELROOT>/rel2/lh/limits
    <CPCMD> <PROJDIR>/linux/list.gh      <RELROOT>/rel2/lh/list
    <CPCMD> <PROJDIR>/linux/locale.gh    <RELROOT>/rel2/lh/locale
    <CPCMD> <PROJDIR>/linux/map.gh       <RELROOT>/rel2/lh/map
    <CPCMD> <PROJDIR>/linux/memory.gh    <RELROOT>/rel2/lh/memory
    <CPCMD> <PROJDIR>/linux/new.gh       <RELROOT>/rel2/lh/new
    <CPCMD> <PROJDIR>/linux/numeric.gh   <RELROOT>/rel2/lh/numeric
    <CPCMD> <PROJDIR>/linux/ostream.gh   <RELROOT>/rel2/lh/ostream
    <CPCMD> <PROJDIR>/linux/queue.gh     <RELROOT>/rel2/lh/queue
    <CPCMD> <PROJDIR>/linux/set.gh       <RELROOT>/rel2/lh/set
    <CPCMD> <PROJDIR>/linux/sstream.gh   <RELROOT>/rel2/lh/sstream
    <CPCMD> <PROJDIR>/linux/stack.gh     <RELROOT>/rel2/lh/stack
    <CPCMD> <PROJDIR>/linux/stdexcep.gh  <RELROOT>/rel2/lh/stdexcep
    <CPCMD> <PROJDIR>/linux/streambu.gh  <RELROOT>/rel2/lh/streambu
    <CPCMD> <PROJDIR>/linux/string.gh    <RELROOT>/rel2/lh/string
    <CPCMD> <PROJDIR>/linux/strstrea.gh  <RELROOT>/rel2/lh/strstrea
    <CPCMD> <PROJDIR>/linux/typeinfo.gh  <RELROOT>/rel2/lh/typeinfo
    <CPCMD> <PROJDIR>/linux/utility.gh   <RELROOT>/rel2/lh/utility
    <CPCMD> <PROJDIR>/linux/valarray.gh  <RELROOT>/rel2/lh/valarray
    <CPCMD> <PROJDIR>/linux/vector.gh    <RELROOT>/rel2/lh/vector

#    <CPCMD> <PROJDIR>/qnx/*.h*           <RELROOT>/rel2/qh/
#    <CPCMD> <PROJDIR>/qnx/sys/*.h        <RELROOT>/rel2/qh/sys/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
