#pmake: build cpu_i86 os_win

host_cpu = i86
host_os  = win
sys_dll = 1

op_sys=windows dll libfile libentry.obj export WEP RESIDENT
os=windows
ccopt = -zw -ml -zu -bd -d__DLL__
name=pbide
compiler=wcc
objs = pbide.obj errdll.obj
libs = ddeml.lib
ext = dll

!include ../mif/master.mif
