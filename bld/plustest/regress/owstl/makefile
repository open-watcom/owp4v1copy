plustest_name = owstl
!include ..\environ.mif

!ifdef test1
test_cflags_386 = -d2-fpd-xs
test_cflags_i86 = -d2-fpd-xs
test_cflags_axp = -d2-xs
!else ifdef test2
test_cflags_386 = -os-d1-3s-xss-br
test_cflags_i86 = -os-d1-xss
test_cflags_axp = -os-d1-xss
!else ifdef test3
test_cflags_386 = -os-d1-3s-xss-xr
test_cflags_i86 = -os-d1-xss-xr
test_cflags_axp = -os-d1-xss-xr
!else ifdef test4
test_cflags_386 = -ot-d1-3r-fpd-xs -oe1000
test_cflags_i86 = -ot-d1-fpd-xs -oe100
test_cflags_axp = -ot-d1-xs -oe1000
!else ifdef test5
test_cflags_386 = -d2-br-xs
test_cflags_i86 = -d2-xs
test_cflags_axp = -d2-xs
!else
test_cflags_386 = -os-d1-xst
test_cflags_i86 = -os-d1-xst
test_cflags_axp = -os-d1-xst
!endif

test_cflags_386 += -zq-wx-wcd433-fpi87-fp5-mf
test_cflags_i86 += -zq-wx-wcd433-fpi87-fp5-ml-zm
test_cflags_axp += -zq-wx-wcd433

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

tests = &
	alg01.$(exe)    &
	alg02.$(exe)    &
	cmplx01.$(exe)	&
        fun01.$(exe)    &
        iter01.$(exe)   &
        lim01.$(exe)    &
        numer01.$(exe)  &
        string01.$(exe) &
        string02.$(exe) &
        util01.$(exe)   &
        vector01.$(exe) &

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch)) -e1000

.obj.$(exe):
    $(linker) @test.lnk NAME $^* FILE $^*

test : .symbolic test.lnk $(tests)
    %create test.out
# Note that there must be no space before the redirection symbol - otherwise
# echo will emit the space character and subsequent diff will fail. This
# way is more transparent than keeping trailing blanks in the check file.
    @echo ----------alg01---------->> test.out
    $(run) alg01 >> test.out
    @echo ----------alg02---------->> test.out
    $(run) alg02 >> test.out
    @echo ----------cmplx01---------->> test.out
    $(run) cmplx01 >> test.out
    @echo ----------fun01---------->> test.out
    $(run) fun01 >> test.out
    @echo ----------iter01---------->> test.out
    $(run) iter01 >> test.out
    @echo ----------lim01---------->> test.out
    $(run) lim01 >> test.out
    @echo ----------numer01---------->> test.out
    $(run) numer01 >> test.out
    @echo ----------string01---------->> test.out
    $(run) string01 >> test.out
    @echo ----------string02---------->> test.out
    $(run) string02 >> test.out
    @echo ----------util01---------->> test.out
    $(run) util01 >> test.out
    @echo ----------vector01---------->> test.out
    $(run) vector01 >> test.out
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
