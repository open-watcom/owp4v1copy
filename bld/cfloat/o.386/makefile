#pmake: all build cpu_386 os_nt os_os2 os_dos os_linux

host_CPU = 386
host_OS  = nt

!ifeq bootstrap 1
all: libcfloat.a
!endif

!include ../master.mif

!ifeq bootstrap 1
CC=cc
CFLAGS=-c -o $@ -Wall -O2 -g -D__386__ -DNDEBUG -I../../watcom/h -I../h

libcfloat.a: $(CF_OBJS)
	ar rs libcfloat.a $(CF_OBJS)
!endif
