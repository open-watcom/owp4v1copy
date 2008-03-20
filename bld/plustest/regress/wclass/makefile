plustest_name = wclass
!include ../environ.mif

!ifeq arch 386
!define arch i86
!endif

!ifeq arch i86

!ifdef test1
comp_flags = -d2-ml
alt_error=01
!else ifdef test2
comp_flags = -ox-d1-xds-mc
alt_error=02
!else
comp_flags = -ox-d1-mc
alt_error=00
!endif

comp_flags += -zq -zw -I"$(%watcom)/h;$(%watcom)/h/win" -bt=windows

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_i86) $[* $(comp_flags) $(extra) -fo=.obj

objs = &
nullfunc.obj &
wboolsw.obj &
wbrush.obj &
wbutton.obj &
wcheckbx.obj &
wclient.obj &
wcombox.obj &
wcontrol.obj &
wdraw.obj &
weditbox.obj &
wfiledlg.obj &
wfont.obj &
wgroupbx.obj &
wlistbox.obj &
wmenu.obj &
wmenuitm.obj &
wobjmap.obj &
wpalette.obj &
wpen.obj &
wpopmenu.obj &
wport.obj &
wpshbttn.obj &
wradbttn.obj &
wrect.obj &
wserver.obj &
wstring.obj &
wtest.obj &
wtext.obj &
wvcllctn.obj &
wvlist.obj &
wwindow.obj &
wtimer.obj &
wmdiwndw.obj &
wmdichld.obj &
wmsgdlg.obj &
wwinmain.obj

test : .symbolic start_test test.exe
    @%make global
    %append $(log_file) PASS $(%__CWD__)

test.exe : $(objs) test.lnk
    $(linker) @$]@ NAME $@

test.lnk : makefile $(objs)
    %create $^@
    @%append $^@ SYSTEM windows
    @%append $^@ OPTION rwreloccheck
    @%append $^@ $(ldebug_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    @%append $^@ LIBRARY $(%watcom)/lib286/win/ddeml.lib
    @%append $^@ LIBRARY $(%watcom)/lib286/win/commdlg.lib

!else

test : .symbolic start_test
    @%make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

!endif

start_test : .symbolic
    %create exec.out
    @if exist s$(arch)_$(alt_error).sav rm s$(arch)_$(alt_error).sav

save : .symbolic
    @if exist exec.out cp exec.out s$(arch)_$(alt_error).sav

global : .procedure
    @%make common_clean
