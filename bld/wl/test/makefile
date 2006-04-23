host_os = dos
host_cpu = i86

proj_name = testovl

testovl_debugfmt = watcom

!include cproj.mif
!include defrule.mif
!include deftarg.mif

extra_c_flags = -of -s

OBJS = ovl1.obj ovl2.obj ovl3.obj

main.exe : main.obj $(OBJS) $(__MAKEFILES__)
    %create tmp.lnk
    %append tmp.lnk $(lflags)
    %append tmp.lnk option map
    %append tmp.lnk option dynamic, area=0
    %append tmp.lnk file main.obj
    %append tmp.lnk begin autosection
    @for %i in ($(OBJS)) do %append tmp.lnk file %i
    %append tmp.lnk end
    %append tmp.lnk libpath ..\ovlldr
    $(linker) @tmp.lnk
