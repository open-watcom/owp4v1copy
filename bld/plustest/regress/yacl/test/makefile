plustest_name = yacl
!include ..\..\environ.mif

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

!ifdef test1
test_cflags = -ot-op-d1
alt_error=01
!else ifdef test2
test_cflags = -ot-op-d1-xds
alt_error=02
!else ifdef test3
test_cflags = -op-d2-xds
alt_error=03
!else
test_cflags = -op-d2
alt_error=00
!endif

test_cflags += -ei -zq -i.. -w0 -d__OS2__

test_cflags_i86 = -ml

stack_386=opt stack=32k
stack_i86=opt stack=24k
stack_axp=opt stack=32k

.obj.$(exe):
    $(linker) @ex.lnk FILE $[@ op verbose
    
.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))
    
test : .symbolic ex.lnk $(exes)
    if exist *.dat del *.dat
    del ..\base.lib
    del ..\io.lib
    %create test.out
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
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
ex.lnk : makefile
    %create $^@
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ LIBRARY ..\base.lib, ..\io.lib
    
global : .symbolic
    @if exist *.dat del *.dat
    @%make common_clean
