plustest_name = exercise
!include ../environ.mif

exes = &
chkcl.$(exe) &
ctordtor.$(exe) &
mp.$(exe) &
ovldall.$(exe) &
switcher.$(exe) &

!ifdef test1
test_cflags += -d2-xds
!else ifdef test2
test_cflags += -d2-xss
!else ifdef test3
test_cflags += -d2-xdt
!else ifdef test4
test_cflags += -d2-xst
!else ifdef test5
test_cflags += -ox-d1-xds
!else ifdef test6
test_cflags += -ox-d1-xss
!else ifdef test7
test_cflags += -ox-d1-xdt
!else ifdef test8
test_cflags += -d2-xs
add_br_ = -br
!else ifdef test9
test_cflags += -ox-d1-xs
add_br_ = -br
!else
test_cflags += -ox-d1-xst
!endif

test_cflags += -zq-w0

!ifdef %plustest_bad_br
no_br_ctordtor = no
no_br_switcher = no
!endif

!ifeq arch 386
test_cflags += $(add_br_$(no_br_$^*))
!endif

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags)
    
.obj.$(exe):
    wlink @test FILE $[@

test : .symbolic test.lnk $(exes)
    %create exercise.out
    @for %i in ( $(exes) ) do $(run) %i >>exercise.out
    diff exercise.out exercise.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
test.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ DEBUG line
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))

global : .SYMBOLIC
    @%make common_clean
