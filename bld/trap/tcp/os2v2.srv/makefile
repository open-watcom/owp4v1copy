#pmake: all build serv remote os_os2 cpu_i86 cpu_386 tcp x86

os  = os2v2
srv = tcp
cpu = x86

extra_objs = $(remote_objs)
extra_libs = $(%os2tkroot)\lib\tcpip32.lib
extra_incs = $(%os2tkroot)\h

!include $(trap_dir)\serv.mif
