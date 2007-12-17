plustest_name = ehmix
!include ..\environ.mif

!ifdef test1
comp_flags = -d2-xs-hd -br
alt_error=01
!else ifdef test2
comp_flags = -d2-xss-hd
alt_error=02
!else ifdef test3
comp_flags = -d2-xst-hd
alt_error=03
!else ifdef test4
comp_flags = -ox-d1-xst
alt_error=04
!else ifdef test5
comp_flags = -ox-d1-xss
alt_error=05
!else
comp_flags = -ox-d1-xs -br
alt_error=00
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
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
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
