OBJS = ovl1.obj ovl2.obj ovl3.obj

.c.obj:
    wcc $[@ -ml -of -s -w4 -zq -d1 -oax

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
    %append tmp.lnk library ..\ovlldr\wovl.lib
    %append tmp.lnk option area=0
    wlink @tmp.lnk

clean: .symbolic
    rm -f *.obj main.exe
