plustest_name = getlong
!include ../environ.mif

!ifdef test1
cflags = -d2
!else
cflags = -ox-d1
!endif

cflags += -zq

stack_386=opt stack=8k
stack_i86=opt stack=4k
stack_axp=opt stack=8k

.cc.obj:
    $(wpp_$(arch)) $[@ $(cflags)

objs = &
    getlongo.obj &
    readme.obj
    
test : .symbolic opt.$(exe)
    $(run) opt >opt.out
    $(run) opt -h >>opt.out
    -$(run) opt -hopeless >>opt.out
    $(run) opt -vers >>opt.out
    $(run) opt -v >>opt.out
    -$(run) opt -o >>opt.out
    -$(run) opt -v=1 >>opt.out
    diff opt.out opt.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

opt.$(exe) : $(objs) opt.lnk
    $(linker) @$]@
    
opt.lnk : makefile
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .SYMBOLIC
    @%make common_clean
