#pmake: all build serv remote os_win cpu_i86 cpu_386 net x86

gui = 1
os = win
srv = net
cpu = x86

extra_objs = $(remote_objs)
resource_file = ..\h\win_srv.rc

!include $(trap_dir)\serv.mif

serv.res : ..\h\win_srv.rc $(trap_dir)\common\win\winserv.rc
