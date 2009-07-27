#pmake: build serv remote os_nov cpu_386 nov x86
host_CPU = 386
host_OS = nov

!ifndef nlm_ver
nlm_ver = 3
!endif
os = nlm
srv = nov
cpu = x86
which = SERVER
type  = remote_link

extra_objs = $(nlm_objs) novnlm.obj

!include $(trap_dir)/lcl/nlm/nlm.mif
!include $(trap_dir)/master.mif
