#pmake: all build cpu_386 os_nt os_os2 os_dos os_linux

host_CPU = 386

!ifeq bootstrap 1
all: libowl.a
!endif

!include ../master.mif

!ifeq bootstrap 1
cc = cc
cflags = -g -D_WCUNALIGNED= -I../../watcom/h -I../h -D__386__ -c -o $@

libowl.a: $(owl_objs)
	ar rs $@ $<
!endif
