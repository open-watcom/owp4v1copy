host_os  = dos
host_cpu = i86

proj_name = testovl

testovl_debugfmt = watcom

!include cproj.mif
!include defrule.mif
!include deftarg.mif

extra_c_flags = -of -s

OBJS = ovl1.obj ovl2.obj ovl3.obj

all : main.exe smain.exe .symbolic

main.exe : main.obj $(OBJS) $(__MAKEFILES__)
    $(linker) n $@ $(lflags) op map, dynamic, area=0 file main.obj begin autosection file { $(OBJS) } end libpath ../ovlldr/dosi86

smain.exe : main.obj $(OBJS) $(__MAKEFILES__)
    $(linker) n $@ $(lflags) op map, standard file main.obj begin s f ovl1 s f ovl2 s f ovl3 end libpath ../ovlldr/dosi86
