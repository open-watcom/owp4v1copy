#pmake: build os_linux cpu_386

host_os  = linux
host_cpu = 386

!ifeq bootstrap 1
all: wlink
!endif

release_wlink = 1

!include ../wlink.mif

!ifeq bootstrap 1
cc = cc
cflags = -funsigned-char -g -c -D__UNIX__ -D__LINUX__ -D__386__ -D__FLAT__ -D_WCUNALIGNED= -I../linux386 -I$(%PWD) -I../h -I../../sdk/rc/rc/h -I../../lib_misc/h -I../../watcom/h -o $@
force_include = -include

LIBS = -L../../clib/$(%OBJDIR) -lwatcom -L../../sdk/rc/wres/$(%OBJDIR) -lwres -L../../dwarf/dw/$(%OBJDIR) -ldwarf -L../../orl/$(%OBJDIR) -lorl

wlink : global.chk $(c_objs) $(extra_objs) wlink.res
    @%make wlink.res
    $(cc) -o $^@ $(c_objs) $(extra_objs) $(LIBS)
    wstrip -q -a -r $@ . wlink.res
!endif
