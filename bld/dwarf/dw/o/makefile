#pmake: build all os_dos os_nt os_os2 os_linux cpu_386 dwarf
model = f
host_CPU = 386
host_OS = nt

!ifeq bootstrap 1
all: libdwarf.a
host_OS = linux
!endif

!include ../dw.mif

!ifeq bootstrap 1

CC=cc
COPTS=-o $@ -Wall -O2 -g -D__386__ -DNDEBUG -I../../../watcom/h -I. -I../h
CFLAGS=-c $(COPTS)
dwtjbflags = -c $(COPTS) -o $@
dwmakeabflags = dwmakeab.tbj dwutils.tbj -o $<
bld_cc=$(cc)
bld_link=$(cc)

libdwarf.a: dwabenum.i dwabinfo.i $(OBJS)
	ar rs libdwarf.a $(OBJS)

!endif

