plustest_name = winnt
!include ../environ.mif

!ifdef test1
test_cflags = -oxa-d1
alt_error=01
!else ifdef test2
test_cflags = -oxa-d1 -br
alt_error=02
!else ifdef test3
test_cflags = -d2 -br
alt_error=03
!else
test_cflags = -d2
alt_error=00
!endif

test_cflags += -w0-zq -i$(%watcom)\h\nt

.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags)
    
test : .symbolic start_test tomexe.exe tomdll.dll
    diff exec.out exec.chk
    @%make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

tomexe.exe : tomexe.obj tomdll.dll tomexe.lnk
    $(linker) @$]@
    $(run) tomexe

start_test : .symbolic
    %create exec.out
    @if exist s$(arch)_$(alt_error).sav rm s$(arch)_$(alt_error).sav

tomdll.dll : tomdll.obj tomdll.lnk
    $(linker) @$]@
    
tomdll.obj : tomdll.c
    $(wpp_$(arch)) $[@ $(test_cflags) -bd
    
tomexe.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION map, caseexact, stack=128k, quiet
    @%append $^@ FILE tomexe
    
tomdll.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ SYSTEM nt dll initinstance terminstance reference __DLLstart_
    @%append $^@ OPTION map, caseexact, stack=128k, quiet
    @%append $^@ FILE tomdll
    
save : .symbolic
    @if exist exec.out cp exec.out s$(arch)_$(alt_error).sav

global : .symbolic
    @%make common_clean
