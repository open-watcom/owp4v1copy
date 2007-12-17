plustest_name = perl
!include ..\environ.mif

objs = &
chgfnt.obj &
cislog.obj &
perlasso.obj &
perlclas.obj &
perltest.obj &
regex.obj &
tstio.obj

exes = &
    perltest.$(exe) &
    perlasso.$(exe) &
    tstio.$(exe)

!ifdef test1
test_cflags = -d2
alt_error=01
!else
test_cflags = -ox-d1
alt_error=00
!endif

test_cflags += -zq-w0

test_cflags_386 =
test_cflags_i86 = -mm
test_cflags_axp =

stack_386=opt stack=32k
stack_i86=opt stack=16k
stack_axp=opt stack=32k

.cpp.obj:
    $(wpp_$(arch)) $[* $(test_cflags) $(test_cflags_$(arch)) -dTEST
    
.c.obj:
    $(wcc_$(arch)) $[* $(test_cflags) $(test_cflags_$(arch))

test : .symbolic $(objs) $(exes)
    $(run) perltest >perltest.out
    diff perltest.out perltest.chk
    $(run) perlasso >perlasso.out
    diff perlasso.out perlasso.chk
    $(run) tstio <tstio.in >tstio.out
    diff tstio.out tstio.chk
    diff t2.txt t2.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
perltest.$(exe) : perltest.obj perlclas.obj regex.obj test.lnk
    $(linker) @test.lnk FILE perltest, perlclas, regex.obj
    
perlasso.$(exe) : perlasso.obj perlclas.obj regex.obj test.lnk
    $(linker) @test.lnk FILE perlasso, perlclas, regex.obj
    
tstio.$(exe) : tstio.obj perlclas.obj regex.obj test.lnk
    $(linker) @test.lnk FILE tstio, perlclas, regex.obj
    
test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    
global : .symbolic
    @%make common_clean
