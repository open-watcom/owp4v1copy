#pmake: all build cpu_386 os_nt
host_CPU = 386
host_OS = nt
sys_dll = 1

op_sys=nt dll
os=nt
ccopt =-mf -bd -d__DLL__ -bt=nt -i"../h" -i"$(lang_root)/h/nt"
name=pbide
compiler=wcc386
objs = pbide.obj errdll.obj
ext = dll

!include ../mif/master.mif
