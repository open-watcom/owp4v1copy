host_os = dos
host_cpu = i86

proj_name = testovl

!include cproj.mif
!include defrule.mif
!include deftarg.mif

extra_c_flags = -of -s

OBJS = ovl1.obj ovl2.obj ovl3.obj

main.exe : main.obj $(OBJS) $(__MAKEFILES__)
    %create tmp.lnk
    %append tmp.lnk $(lflags)
    %append tmp.lnk option map,verbose
    %append tmp.lnk sort global
    %append tmp.lnk option dynamic, area=0
    %append tmp.lnk file main.obj
    %append tmp.lnk begin autosection
    @for %i in ($(OBJS)) do %append tmp.lnk file %i
    %append tmp.lnk end
    %append tmp.lnk library ..\ovlldr\wovl.lib
    $(linker) @tmp.lnk
