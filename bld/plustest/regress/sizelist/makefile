plustest_name = sizelist
!include ../environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq-w0

cflags_386 += $(cflags) -zp4-ms
cflags_i86 += $(cflags) -zp2-ml
cflags_axp += $(cflags) -zp8

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

objs = &
    psfistt.obj
    
test : .symbolic psfistt.$(exe)
    $(run) psfistt.$(exe) >psfistt.out
    diff psfistt.out psfistt.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

psfistt.$(exe) : $(objs) psfistt.lnk
    $(linker) @$]@
    
psfistt.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
