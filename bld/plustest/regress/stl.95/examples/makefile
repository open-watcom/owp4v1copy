plustest_name = stl95
!include ..\..\environ.mif

!ifdef test1
alt_error = 01
cflags = -oxa-d1
!else ifdef test2
alt_error = 02
cflags = -oxa-d1-xs
!else ifdef test3
alt_error = 03
cflags = -d2-xs
!else ifdef test4
alt_error = 04
cflags = -oxa-d1-xs -br
!else ifdef test5
alt_error = 05
cflags = -d2-xs -br
!else
alt_error = 00
cflags = -d2
!endif

cflags += -w0-zq -i.;.. -fi=..\pch.h -fh

extra_istmit1 = <istmit1.in

.cpp: ..\..\stl.94\examples;..
.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags)
    
.obj.$(exe):
    wlink @test FILE $[@, tempbuf.obj, random.obj
    del $[@
    %append exec.out @@@ $^@ @@@
    $(run) $^@ $(extra_$^*) >>exec.out
    
!include ..\..\stl.94\examples\stl_test.mif

test : .symbolic test.lnk tempbuf.obj random.obj start_test $(tests)
    diff exec.out exec.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
start_test : .symbolic
    %create exec.out
    
test.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION caseexact, stack=64k, quiet
    
save : .symbolic
    if exist exec.out copy exec.out s$(arch)_$(alt_error).sav
    
global : .symbolic
    @%make common_clean
