plustest_name = jam
!include ..\environ.mif

!ifdef test1
test_cflags = -d2
alt_error=01
!else
test_cflags = -otexan-d1
alt_error=00
!endif

test_cflags += -zq-w0-i..

test_cflags_386 += $(test_cflags) -mf
test_cflags_i86 += $(test_cflags) -mm
test_cflags_axp += $(test_cflags)

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch))

objs = &
    assert.obj &
    testftor.obj
    
test : .symbolic jam.$(exe)
    $(run) jam >jam.out
    diff jam.out jam.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

jam.$(exe) : $(objs) jam.lnk
    $(linker) @$]@

jam.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ DEBUG line
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .symbolic
    @%make common_clean
