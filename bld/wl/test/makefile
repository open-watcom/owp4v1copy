OBJS = ovl1.obj ovl2.obj ovl3.obj

.c.obj:
    wcc $[@ -ml -d2 -w4 -zq

main.exe : main.obj $(OBJS) $(__MAKEFILES__)
    %create tmp.lnk
    %append tmp.lnk debug all
    %append tmp.lnk name main
    %append tmp.lnk system dos
    %append tmp.lnk option quiet,dynamic
    %append tmp.lnk option map,verbose
    %append tmp.lnk sort global
    %append tmp.lnk file main
    %append tmp.lnk begin autosection
    @for %i in ($(OBJS)) do %append tmp.lnk file %i
    %append tmp.lnk end
    %append tmp.lnk library ..\ovlldr\dovl.lib
    %append tmp.lnk libpath \ow1\lib286
    %append tmp.lnk libpath \ow1\lib286\dos
    %append tmp.lnk option area=0
    wln @tmp.lnk

clean: .symbolic
    rm -f *.obj main.exe
