plustest_name = yacl
!include ..\..\environ.mif

!ifneq arch i86
exes = &
bittest.$(exe) &
btreetst.$(exe) &
bytstrm.$(exe) &
dkbtrtst.$(exe) &
format1.$(exe) &
gseqtest.$(exe) &
holidays.$(exe) &
maptest.$(exe) &
perstest.$(exe) &
seqtest.$(exe) &
settest.$(exe) &
slotftst.$(exe) &
strset.$(exe) &
strtest.$(exe) &
treeprnt.$(exe) &
treetest.$(exe) &
wordcnt.$(exe)
!endif

!ifdef test1
cflags = -op-d2
!else ifdef test2
cflags = -ot-op-d1-xds
!else ifdef test3
cflags = -op-d2-xds
!else
cflags = -ot-op-d1
!endif

cflags += -ei -zq -i.. -w0 -d__OS2__

.obj.$(exe):
    $(linker) @ex.lnk FILE $[@
    
.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags)
    
test : .symbolic ex.lnk $(exes)
    if exist *.dat del *.dat
    del ..\base.lib
    del ..\io.lib
    %create test.out
!ifneq arch i86
    %append test.out bittest.$(exe)
    $(run) bittest.$(exe) >>test.out
    %append test.out btreetst.$(exe)
    $(run) btreetst.$(exe) >>test.out
    %append test.out bytstrm.$(exe)
    $(run) bytstrm.$(exe) >>test.out
    %append test.out dkbtrtst.$(exe)
    $(run) dkbtrtst.$(exe) >>test.out
    %append test.out dkbtrtst.$(exe)
    $(run) dkbtrtst.$(exe) >>test.out
    %append test.out format1.$(exe)
    $(run) format1.$(exe) <format1.cpp >>test.out
    %append test.out gseqtest.$(exe)
    $(run) gseqtest.$(exe) >>test.out
    %append test.out holidays.$(exe)
    $(run) holidays.$(exe) 1999 >>test.out
    %append test.out maptest.$(exe)
    $(run) maptest.$(exe) >>test.out
    %append test.out perstest.$(exe)
    $(run) perstest.$(exe) >>test.out
    %append test.out perstest.$(exe)
    $(run) perstest.$(exe) >>test.out
    %append test.out seqtest.$(exe)
    $(run) seqtest.$(exe) >>test.out
    %append test.out settest.$(exe)
    $(run) settest.$(exe) >>test.out
    %append test.out slotftst.$(exe)
    $(run) slotftst.$(exe) >>test.out
    %append test.out strset.$(exe)
    $(run) strset.$(exe) >>test.out
    %append test.out strtest.$(exe)
    $(run) strtest.$(exe) >>test.out
    %append test.out treeprnt.$(exe)
    $(run) treeprnt.$(exe) >>test.out
    %append test.out treetest.$(exe)
    $(run) treetest.$(exe) >>test.out
    %append test.out wordcnt.$(exe)
    $(run) wordcnt.$(exe) <wordcnt.cpp >>test.out
    diff test.out test.chk
!endif
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
ex.lnk : makefile
    %create $^@
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION quiet, stack=32k
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ LIBRARY ..\base.lib, ..\io.lib
    
global : .symbolic
    @if exist *.dat del *.dat
    @%make common_clean
