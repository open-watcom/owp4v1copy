plustest_name = ehmix
!include ..\environ.mif

!ifdef test1
comp_flags = -d2-xs-hd -br
!else ifdef test2
comp_flags = -d2-xss-hd
!else ifdef test3
comp_flags = -d2-xst-hd
!else ifdef test4
comp_flags = -ox-d1-xst
!else ifdef test5
comp_flags = -ox-d1-xss
!else
comp_flags = -ox-d1-xs -br
!endif

comp_flags += -zq -xr

stack_386=opt stack=64k
stack_i86=opt stack=16k
stack_axp=opt stack=64k

.cpp.obj:
    $(wpp_$(arch)) $[@ $(comp_flags)
    
.obj.exe:
    wlink @ehmix file $[@ 

# not posible to compile it for 16-bit due to 
# ehmix.c requires more than 64kB memory for code
#
!ifeq arch i86
test : .symbolic
!else
test : .symbolic exec.out
    -diff exec.out exec.chk >diff.out
!endif
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
exec.out : ehmix.lnk ehmix.exe
    ehmix.exe >exec.out
    
ehmix.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    
global : .SYMBOLIC
    %make common_clean
