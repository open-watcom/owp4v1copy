#pmake: os_linux cpu_386

!ifeq bootstrap 1
all: nwlib
!endif

host_os  = linux
host_cpu = 386

!include ../master.mif

!ifeq bootstrap 1
bld_cl=cc
cc=cc
cflags=-funsigned-char -c -g -D__386__ -I. -I../h -I$(orl_dir)/h -I$(trmem_dir) -I$(lib_misc_dir)/h -I$(watcom_dir)/h -DIDE_PGM -D__stdcall= -D__UNIX__ -D__LINUX__ -o $@
wcl_flags = -o $@

nwlib: msg.gh $(wlib_objs) wlib.res
    $(cc) -g -o nwlib -L$(clib_dir)/$(%OBJDIR) -L$(wres_dir)/$(%OBJDIR) -L$(orl_dir)/$(%OBJDIR) $(wlib_objs) -lwatcom -lorl -lwres
    wstrip -q -a -r $^@ . wlib.res

!endif
