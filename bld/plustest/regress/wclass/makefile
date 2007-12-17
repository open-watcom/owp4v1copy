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

comp_flags += -zq -zw -i$(%watcom)\h\win -bt=windows

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_i86) $[* $(comp_flags) $(extra)
    
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

test : .symbolic test.exe
    cp test.exe test.ice
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    ren test.ice test.exe

test.exe : $(objs) test.lnk
    $(linker) @$]@

test.lnk : makefile $(objs)
    %create $^@
    @%append $^@ SYSTEM windows
    @%append $^@ OPTION rwreloccheck
    @%append $^@ NAME $^*
    @%append $^@ $(ldebug_$(arch))
    @for %i in ( $(objs) ) do @%append $^@ FILE %i
    @%append $^@ LIBRARY $(%watcom)\lib286\win\DDEML.LIB
    @%append $^@ LIBRARY $(%watcom)\lib286\win\COMMDLG.LIB

!else
    
test : .symbolic
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

!endif
    
global : .procedure
    %make common_clean
