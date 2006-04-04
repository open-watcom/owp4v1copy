plustest_name = splash
!include ..\environ.mif

objs = &
assoc.obj &
chgfnt.obj &
cislog.obj &
justify.obj &
regex.obj &
splash.obj &
spltest.obj &
tracer.obj &
tstio.obj &
xcl.obj

!ifdef test1
test_cflags = -d2
!else ifdef test2
test_cflags = -ox-d1
!else
test_cflags = -ox-d1 -oe1000
!endif
test_cflags += -zq-w0-dTEST

test_cflags_386 = $(test_cflags) -3s
test_cflags_i86 = $(test_cflags) -mm
test_cflags_axp = $(test_cflags)
    
stack_386=opt stack=64k
stack_i86=opt stack=8k
stack_axp=opt stack=64k

.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch))

exes = &
    splassoc.$(exe) &
    justify.$(exe) &
    spltest.$(exe)

test : .symbolic $(objs) $(exes)
    $(run) splassoc >assoc.out
    diff assoc.out assoc.chk
    $(run) justify 13 <justify.c >justify.out
    diff justify.out justify.chk
    $(run) spltest >spltest.out
    diff spltest.out spltest.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
spltest.$(exe) : spltest.obj splash.obj regex.obj test.lnk
    $(linker) @test.lnk NAME $^@ FILE spltest, splash, regex
    
splassoc.$(exe) : assoc.obj splash.obj regex.obj test.lnk
    $(linker) @test.lnk NAME $^@ FILE assoc, splash, regex
    
justify.$(exe) : justify.obj splash.obj regex.obj test.lnk
    $(linker) @test.lnk NAME $^@ FILE justify, splash, regex
    
regex.obj : regex.c
    $(wcc_$(arch)) $[* $(test_cflags_$(arch))
    
test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    
global : .symbolic
    @%make common_clean
