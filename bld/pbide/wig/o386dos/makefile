#pmake: all cpu_386 os_dos
op_sys=dos4g
os=dos
host_CPU=386
bind=1
name=wig
compiler=wcc386
objs = error.obj

!ifeq release 0
ccopt += -dTRMEM -of+
objs += trmemcvr.obj trmem.obj
!endif

!include ../mif/master.mif
