plustest_name = stl94
!include ..\..\environ.mif

!ifdef test1
alt_error = 01
test_cflags = -oxa-d1
!else ifdef test2
alt_error = 02
test_cflags = -oxa-d1-xs
!else ifdef test3
alt_error = 03
test_cflags = -d2-xs
!else
alt_error = 00
test_cflags = -d2
!endif

test_cflags += -w0-zq -i.;.. -fi=..\pch.h -fhq

test_cflags_i86 = -mm -zm

extra_istmit1 = <istmit1.in

stack_386=opt stack=64k
stack_i86=opt stack=16k
stack_axp=opt stack=64k

.cpp: .;..
.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))
    
.obj.$(exe):
    wlink @test FILE $[@, tempbuf.obj, random.obj
    del $[@
    %append exec.out @@@ $^@ @@@
    $(run) $^@ $(extra_$^*) >>exec.out
    
!include stl_test.mif

test : .symbolic test.lnk tempbuf.obj random.obj start_test $(tests)
!ifeq arch i86
    diff exec.out execi86.chk
!else
    diff exec.out exec.chk
!endif
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
start_test : .symbolic
    %create exec.out
    
test.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    
save : .symbolic
    if exist exec.out copy exec.out s$(arch)_$(alt_error).sav
    
global : .symbolic
    @%make common_clean
