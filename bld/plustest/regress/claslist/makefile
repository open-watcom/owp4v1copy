plustest_name = claslist
!include ..\environ.mif

!ifdef test1
cflags_386 = -d2~
cflags_i86 = -d2~
cflags_axp = -d2~
!else
cflags_386 = -ox-d1
cflags_i86 = -ox-d1
cflags_axp = -ox-d1
!endif

cflags_386 += -zq-mf
cflags_i86 += -zq-mc
cflags_axp += -zq

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))

objs = pcfistt.obj

test : .symbolic pcfistt.$(exe)
    $(run) pcfistt >pcfistt.out
    diff pcfistt.out pcfist.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

pcfistt.$(exe) : $(objs) pcfistt.lnk
    $(linker) @$]@

pcfistt.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ DEBUG line
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
