plustest_name = stl94
!include ..\..\environ.mif

objs = demo.obj tempbuf.obj

!ifdef test1
cflags = -oxa-d1-hd
!else ifdef test2
cflags = -oxa-d1-xs-hd
!else ifdef test3
cflags = -d2-xs-hd
!else
cflags = -d2-hd
!endif

cflags += -w0-zq -i..

cflags_i86 = -mm -zm

stack_386=opt stack=128k
stack_i86=opt stack=32k
stack_axp=opt stack=128k

.cpp: .;..
.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags) $(cflags_$(arch))

#
# function code overflow 16-bit segment
# don't use it for i86 target
#
!ifeq arch i86
test : .symbolic
!else    
test : .symbolic demo.$(exe)
    $(run) $[@ <demo.in >demo.out
    diff demo.out demo.chk
!endif
    %make global
    %append $(log_file) PASS $(%__CWD__)

demo.$(exe) : $(objs) demo.lnk
    $(linker) @$]@
    
demo.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    
global : .symbolic
    @%make common_clean
