plustest_name = arith
!include ..\environ.mif

!ifdef test1
cflags_386 = -d2-fpd
cflags_i86 = -d2-fpd
cflags_axp = -d2
!else ifdef test2
cflags_386 = -os-d1-3s-xds-br
cflags_i86 = -os-d1-xds
cflags_axp = -os-d1-xds
!else ifdef test3
cflags_386 = -os-d1-3s-xss-fpd
cflags_i86 = -os-d1-xss-fpd
cflags_axp = -os-d1-xss
!else ifdef test4
cflags_386 = -ot-d1-3r-fpd -oe1000
cflags_i86 = -ot-d1-fpd -oe1000
cflags_axp = -ot-d1 -oe1000
!else ifdef test5
cflags_386 = -d2-br
cflags_i86 = -d2
cflags_axp = -d2
!else
cflags_386 = -os-d1-xst
cflags_i86 = -os-d1-xst
cflags_axp = -os-d1-xst
!endif

cflags_386 += -zq-fpi87-fp5-mf
cflags_i86 += -zq-fpi87-fp5-ml
cflags_axp += -zq

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

tests = &
        bench.$(exe)    &
        bitcount.$(exe) &
        calc_e.$(exe)   &
        calc_pi.$(exe)  &
        calc_pi2.$(exe) &
        calc_pi3.$(exe) &
        dmc.$(exe)      &
        mldiv.$(exe)    &
        rsa.$(exe)      &
        waste.$(exe)    &


.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch)) -e1000 -fi=arith.h

.obj.$(exe):
    $(linker) @test.lnk NAME $^* FILE $^*

test : .symbolic test.lnk $(tests)
    $(run) bench >bench.out
    diff bench.out bench.chk
    %create test.out
# Note that there must be no space before the redirection symbol - otherwise
# echo will emit the space character and subsequent diff will fail. This
# way is more transparent than keeping trailing blanks in the check file.
    @echo -------bench--------->> test.out
    $(run) bench >>test.out
    @echo -------bitcount--------->> test.out
    $(run) bitcount >>test.out
    @echo -------calc_e----------->> test.out
    $(run) calc_e >>test.out
    @echo -------calc_pi---------->> test.out
    $(run) calc_pi >>test.out
    @echo -------calc_pi2--------->> test.out
    $(run) calc_pi2 >>test.out
    @echo -------calc_pi3--------->> test.out
    $(run) calc_pi3 >>test.out
    @echo -------dmc-------------->> test.out
    $(run) dmc c dmc.cpp dmc.out >>test.out
    $(run) dmc d dmc.out cpp.out >>test.out
    diff cpp.out dmc.cpp >>test.out
    @echo -------mldiv------------>> test.out
    $(run) mldiv >>test.out
    @echo -------rsa------------>> test.out
    $(run) rsa >>test.out
    @echo -------waste------------>> test.out
    $(run) waste >>test.out
    diff test.out test.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))

global : .SYMBOLIC
    @%make common_clean
