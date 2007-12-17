plustest_name = bitset
!include ../environ.mif

!ifdef test1
test_cflags_386 = -d2-4r
test_cflags_i86 = -d2
test_cflags_axp = -d2
alt_error=01
!else
test_cflags_386 = -ox-d1-4s
test_cflags_i86 = -ox-d1
test_cflags_axp = -ox-d1
alt_error=00
!endif

test_cflags_386 += -j-zq-mf-4r
test_cflags_i86 += -j-zq-ml
test_cflags_axp += -j-zq

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch))

objs = &
    tbitset.obj &
    bitset.obj
    
test : .symbolic bitset.$(exe)
    $(run) bitset >bitset.out
    diff bitset.out bitset.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

bitset.$(exe) : $(objs) bitset.lnk
    $(linker) @$]@

bitset.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
