#pmake: all build serv lcl os_dos dsx cpu_386 x86
host_cpu = 386
host_OS = dos

#NOTE: This file is also included from the ..\DOSPED.SRV makefile

os = pls
srv = pls
cpu = x86
!ifndef name
name = plshelp
!endif
which = SERVER

extra_c_flags = -fi=wcc.h -dDOSXHELP $(compflags_$[&)

extra_a_flags_plsintr = -fpi87

inc_dirs = ../h;$(plsdir);

extra_objs = $(dosx_serv_objs) plsacc.obj plsintr.obj x86cpu.obj dilglue.obj
extra_srcs = $(plsdir)

extra_c_flags_dilglue = -3s

!ifndef extra_linker
extra_linker = &
        op maxdata=0 runtime maxreal=1023K,maxibuf=4K,flag=2 &
        lib $(plsdir)/wdos32,$(plsdir)/wdosx32 &
        file $(plsdir)/dilseg
!endif

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif
