plustest_name = jam
!include ..\environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -otexan-d1
!endif

cflags += -zq-w0-i..

cflags_386 += $(cflags) -mf
cflags_i86 += $(cflags) -mm
cflags_axp += $(cflags)

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

objs = &
    assert.obj &
    testftor.obj
    
test : .symbolic jam.$(exe)
    $(run) jam >jam.out
    diff jam.out jam.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

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
