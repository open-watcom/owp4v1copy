#pmake: serv lcl os_dos dsx cpu_386 x86
host_CPU = 386
host_OS = dos

os = ads
srv = ads
cpu = x86
name = adshelp
which = SERVER

extra_objs = $(dosx_serv_objs) adsacc.obj adsintr.obj x86cpu.obj misc7386.obj

extra_c_flags = -DACAD

extra_a_flags = -ms -3s -DACAD

extra_linker = op nocaseexact

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif
