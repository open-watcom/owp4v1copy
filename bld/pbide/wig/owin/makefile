#pmake: all cpu_i86 os_win

host_cpu = i86
host_os  = win

op_sys=windows
os=windows
ccopt = -zW -ml
name=wig
compiler=wcc
objs = error.obj

!ifeq release 0
ccopt += -dTRMEM -of+
objs += $(trmem_dir)\trmemcvr.obj $(trmem_dir)\trmem.obj
!endif

!include ../mif/master.mif
