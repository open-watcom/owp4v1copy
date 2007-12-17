plustest_name = bits64
!include ../environ.mif

!ifdef test1
test_cflags_386 = -d2-3r
test_cflags_i86 = -d2
test_cflags_axp = -d2
alt_error=01
!else
test_cflags_386 = -ox-d1-3s
test_cflags_i86 = -ox-d1
test_cflags_axp = -ox-d1
alt_error=00
!endif

test_cflags_386 += -zq-mf
test_cflags_i86 += -zq-ml
test_cflags_axp += -zq

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.cpp: ..\bits
.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch)) -d_USE_INT64

objs = tbits.obj
    
test : .symbolic tbits.$(exe)
    $(run) tbits >tbits.out
    diff tbits.out ..\bits\tbits.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

tbits.$(exe) : $(objs) tbits.lnk
    $(linker) @$]@

tbits.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
