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
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq-w0

cflags_386 += $(cflags)
cflags_i86 += $(cflags) -mm
cflags_axp += $(cflags)

.cpp.obj:
    $(wpp_$(arch)) $[* $(cflags_$(arch)) -dTEST
    
.c.obj:
    $(wcc_$(arch)) $[* $(cflags_$(arch))

test : .symbolic $(objs) $(exes)
    $(run) perltest >perltest.out
    diff perltest.out perltest.chk
    $(run) perlasso >perlasso.out
    diff perlasso.out perlasso.chk
    $(run) tstio <tstio.in >tstio.out
    diff tstio.out tstio.chk
    diff t2.txt t2.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
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
    @%append $^@ OPTION map, quiet, stack=32k
    
global : .symbolic
    @%make common_clean
