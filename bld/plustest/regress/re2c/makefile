plustest_name = re2c
!include ../environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq

cflags_386 += $(cflags) -mf-3s
cflags_i86 += $(cflags) -ml -3
cflags_axp += $(cflags)

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

.c.2bj:
    $(wpp_i86) $[@ $(cflags_286)
    
objs = &
    code.obj &
    dfa.obj &
    main.obj &
    parser.obj &
    scanner.obj &
    useful.obj &
    asc2ebc.obj &
    ebc2asc.obj &
    getopt.obj
    
test : .symbolic re2c.$(exe)
    $(run) re2c -d plusplus.lex
    diff debug.out debug.chk | wc -w >word.out
    diff word.out word.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

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
