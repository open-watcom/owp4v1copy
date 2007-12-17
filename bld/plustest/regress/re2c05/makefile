plustest_name = re2c05
!include ../environ.mif

!ifdef test1
test_cflags = -d2
alt_error=01
!else
test_cflags = -ox-d1
alt_error=00
!endif

test_cflags += -zq

test_cflags_386 += $(test_cflags) -mf-zc
test_cflags_i86 += $(test_cflags) -ml-zc
test_cflags_axp += $(test_cflags)

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch))

objs = &
code.obj &
dfa.obj &
main.obj &
memchk.obj &
parser.obj &
scanner.obj &
substr.obj &
translat.obj &
actions.obj

test : .symbolic re2c.$(exe)
    $(run) re2c.$(exe) -b scanner.re >debug.out
    diff debug.out debug.chk | wc -l >line.out
    diff line.out line.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

re2c.$(exe) : $(objs) re2c.lnk
    $(linker) @$]@

re2c.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i

global : .SYMBOLIC
    @%make common_clean
