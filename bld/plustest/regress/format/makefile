plustest_name = format
!include ..\environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq

cflags_axp = $(cflags)
cflags_386 = $(cflags) -mf
cflags_i86 = $(cflags) -mc

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

i_objs = iformat.obj itest.obj
f_objs = fformat.obj ftest.obj

test : .symbolic testi.$(exe) testf.$(exe)
    $(run) testi >testi.out
    diff testi.out icheck.chk
    $(run) testf >testf.out
    diff testf.out fcheck.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

testi.$(exe) : $(i_objs) testi.lnk
    $(linker) @$]@

testf.$(exe) : $(f_objs) testf.lnk
    $(linker) @$]@

testi.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION verbose
    @for %i in ( $(i_objs) ) do @%append $^@ FILE %i

testf.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @for %i in ( $(f_objs) ) do @%append $^@ FILE %i

global : .SYMBOLIC
    @%make common_clean
