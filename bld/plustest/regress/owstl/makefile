plustest_name = owstl
!include ..\environ.mif

!ifdef test1
cflags_386 = -d2-fpd-xs
cflags_i86 = -d2-fpd-xs
cflags_axp = -d2-xs
!else ifdef test2
cflags_386 = -os-d1-3s-xss-br
cflags_i86 = -os-d1-xss
cflags_axp = -os-d1-xss
!else ifdef test3
cflags_386 = -os-d1-3s-xss-xr
cflags_i86 = -os-d1-xss-xr
cflags_axp = -os-d1-xss-xr
!else ifdef test4
cflags_386 = -ot-d1-3r-fpd-xs -oe1000
cflags_i86 = -ot-d1-fpd-xs -oe1000
cflags_axp = -ot-d1-xs -oe1000
!else ifdef test5
cflags_386 = -d2-br-xs
cflags_i86 = -d2-xs
cflags_axp = -d2-xs
!else
cflags_386 = -os-d1-xst
cflags_i86 = -os-d1-xst
cflags_axp = -os-d1-xst
!endif

cflags_386 += -zq-fpi87-fp5-mf
cflags_i86 += -zq-fpi87-fp5-ml
cflags_axp += -zq

tests = &
	alg01.$(exe)    &
	alg02.$(exe)    &
	cmplx01.$(exe)	&
        fun01.$(exe)    &
        lim01.$(exe)    &
        string01.$(exe) &
        util01.$(exe)   &

.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch)) -e1000

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
    @echo ----------lim01---------->> test.out
    $(run) lim01 >> test.out
    @echo ----------string01---------->> test.out
    $(run) string01 >> test.out
    @echo ----------util01---------->> test.out
    $(run) util01 >> test.out
    diff test.out test.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION map, caseexact, stack=16k, quiet

global : .SYMBOLIC
    @%make common_clean
