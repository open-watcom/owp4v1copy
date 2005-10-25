plustest_name = field64
!include ../environ.mif

!ifdef test1
test_cflags_386 = -d2-fpd
test_cflags_i86 = -d2-fpd-zm
test_cflags_axp = -d2
!else ifdef test2
test_cflags_386 = -os-d1-3s-xds
test_cflags_i86 = -os-d1-xds
test_cflags_axp = -os-d1-xds
!else ifdef test3
test_cflags_386 = -os-d1-3s-xss-fpd
test_cflags_i86 = -os-d1-xss-fpd
test_cflags_axp = -os-d1-xss
!else
test_cflags_386 = -os-d1-xst
test_cflags_i86 = -os-d1-xst
test_cflags_axp = -os-d1-xst
!endif

test_cflags_386 += -zq-fpi87-fp5-mf
test_cflags_i86 += -zq-fpi87-fp5-ml-s
test_cflags_axp += -zq

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch)) -e1000
    
test : .symbolic test1.$(exe) test2.$(exe) test3.$(exe)
    $(run) test1 >test.out
    $(run) test2 >>test.out
    $(run) test3 1000 1000 >>test.out
    diff test.out test.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
test1.$(exe) : field.obj test1.obj field.lnk
    $(linker) @$]@ NAME $^* FILE test1.obj, field.obj
    
test2.$(exe) : field.obj test2.obj field.lnk
    $(linker) @$]@ NAME $^* FILE test2.obj, field.obj
    
test3.$(exe) : field.obj test3.obj field.lnk
    $(linker) @$]@ NAME $^* FILE test3.obj, field.obj

field.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))

global : .SYMBOLIC
    @%make common_clean
