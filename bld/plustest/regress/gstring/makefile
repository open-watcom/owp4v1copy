plustest_name = gstring
!include ..\environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq -i.

cflags_386 = $(cflags) -mf
cflags_i86 = $(cflags) -ml
cflags_axp = $(cflags)

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

objs = &
    string.obj &
    strngrep.obj &
    strtest1.obj
    
test : .symbolic strtest.$(exe)
    $(run) strtest >strtest.out
    diff strtest.out strtest.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

strtest.$(exe) : $(objs) strtest.lnk
    $(linker) @$]@

strtest.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ DEBUG line
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION map, caseexact, stack=8k, quiet
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
