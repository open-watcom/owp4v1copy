OBJS = ovl1.obj ovl2.obj ovl3.obj

!ifdef dwarf
cform = -hd
lform = debug dwarf
!else
cform = -hw
lform = debug watcom all
!endif

.c.obj:
    wcc $[@ -ml -of -s -w4 -zq -d2 $(cform)

main.exe : main.obj $(OBJS) $(__MAKEFILES__)
    %create tmp.lnk
    %append tmp.lnk $(lform)
    %append tmp.lnk name main
    %append tmp.lnk system dos
    %append tmp.lnk option map,verbose,symfile
    %append tmp.lnk sort global
    %append tmp.lnk file main
    %append tmp.lnk option dynamic
    %append tmp.lnk begin autosection
    @for %i in ($(OBJS)) do %append tmp.lnk file %i
    %append tmp.lnk end
    %append tmp.lnk library ..\ovlldr\wovl.lib
    %append tmp.lnk option area=0
    wlink @tmp.lnk

clean: .symbolic
    rm -f *.obj main.exe *.lnk *.sym *.map *.err
