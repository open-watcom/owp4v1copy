#pmake: all build serv remote os_win cpu_i86 cpu_386 nmp x86

gui = 1
os = win
srv = nmp
cpu = x86

extra_objs = $(remote_objs) nmpdos.obj
resource_file = serv.res

!include $(trap_dir)\serv.mif

serv.res : serv.rc $(trap_dir)\common\win\winserv.rc
