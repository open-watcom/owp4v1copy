#pmake: all cpu_386 os_os2

host_cpu = 386
host_os  = os2

op_sys=os2v2
os=os2
name=wig
compiler=wcc386
objs = error.obj

!ifeq release 0
ccopt += -dTRMEM -of+
objs += $(trmem_dir)\trmemcvr.obj $(trmem_dir)\trmem.obj
!endif

!include ../mif/master.mif
