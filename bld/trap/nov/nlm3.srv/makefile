#pmake: all build serv remote os_nov cpu_386 nov x86
host_cpu = 386

!ifndef nlm_ver
nlm_ver = 3
!endif
os = nlm
srv = nov
cpu = x86

extra_objs = $(remote_objs) $(nlm_objs) novnlm.obj

!include $(trap_dir)/lcl/nlm/nlm.mif
!include $(trap_dir)/serv.mif
