plustest_name = cool
!include ../../environ.mif

exps = &
ex2_4.$(exe) &
ex2_7.$(exe) &
ex2_9.$(exe) &
ex3_4.$(exe) &
ex3_6.$(exe) &
ex3_8.$(exe) &
ex3_10.$(exe) &
ex4_7.$(exe) &
ex4_9.$(exe) &
ex5_8.$(exe) &
ex6_4.$(exe) &
ex6_6.$(exe) &
ex6_8.$(exe) &
ex6_10.$(exe) &
ex7_4.$(exe) &
ex7_7.$(exe) &
ex7_9.$(exe) &
ex8_4.$(exe) &
ex8_6.$(exe) &
ex9_5.$(exe) &
ex9_7.$(exe) &
ex9_11.$(exe)

!ifdef test1
test_cflags = -op-d2
alt_error=01
!else ifdef test2
test_cflags = -ot-op-d1-xds
alt_error=02
!else ifdef test3
test_cflags = -op-d2-xds
alt_error=03
!else
test_cflags = -ot-op-d1
alt_error=00
!endif

test_cflags += -zq -I../.. -w0

test_cflags_i86 = -ml -zm

stack_386=opt stack=64k
stack_i86=opt stack=48k
stack_axp=opt stack=64k

.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))
    
.obj.$(exe):
    $(linker) @ex.lnk NAME $^* FILE $[@
    
test : .symbolic ex.lnk $(exps)
    %create exec.out
    @for %i in ( $(exps) ) do $(run) %i >>exec.out
    diff exec.out exec.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
ex.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(stack_$(arch))
    @%append $^@ LIBRARY ..\cool.lib
    
global : .symbolic
    @%make common_clean
