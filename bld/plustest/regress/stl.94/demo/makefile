plustest_name = stl94
!include ..\..\environ.mif

objs = demo.obj tempbuf.obj

!ifdef test1
test_cflags = -oxa-d1-hd
alt_error=01
!else ifdef test2
test_cflags = -oxa-d1-xs-hd
alt_error=02
!else ifdef test3
test_cflags = -d2-xs-hd
alt_error=03
!else
test_cflags = -d2-hd
alt_error=00
!endif

test_cflags += -w0-zq -i..

test_cflags_i86 = -mm -zm

stack_386=opt stack=128k
stack_i86=opt stack=32k
stack_axp=opt stack=128k

.cpp: .;..
.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))

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
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

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
