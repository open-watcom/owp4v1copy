#pmake: build os_linux cpu_386

host_os  = linux
host_cpu = 386

!ifeq bootstrap 1
all: wasm
!endif

!include ../mif/master.mif

!ifeq bootstrap 1

cc = cc
cflags = -funsigned-char -c -D_WASM_ -D__UNIX__ -D__LINUX__ -D__386__ -D__FLAT__ -D_WCUNALIGNED= -I$(%PWD) -I../h -I$(womp_dir)/h -I$(womp_dir) -I$(trmem_dir)/h -I../../watcom/h -o $@
force_include = -include
bld_cl = cc
mkopcode_opts = -o $^@ -Dstricmp=strcasecmp $(LIBS)

LIBS = -L../../clib/bootstrp -lwatcom -L../../sdk/rc/wres/bootstrp -lwres 

wasm : ../h/fullops.h ../h/inlnops.h $(link_name) $(proj_name).res $(proj_obj)
    @%make usage.u
    $(cc) -o $^@ $(proj_obj) $(LIBS)
    wstrip -a -r -q $@ . $(proj_name)

!endif
