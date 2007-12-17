plustest_name = claslist
!include ..\environ.mif

!ifdef test1
test_cflags_386 = -d2~
test_cflags_i86 = -d2~
test_cflags_axp = -d2~
alt_error=01
!else
test_cflags_386 = -ox-d1
test_cflags_i86 = -ox-d1
test_cflags_axp = -ox-d1
alt_error=00
!endif

test_cflags_386 += -zq-mf
test_cflags_i86 += -zq-mc
test_cflags_axp += -zq

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch))

objs = pcfistt.obj

test : .symbolic pcfistt.$(exe)
    $(run) pcfistt >pcfistt.out
    diff pcfistt.out pcfist.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

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
