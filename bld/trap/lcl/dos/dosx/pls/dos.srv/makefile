#pmake: serv lcl os_dos dsx cpu_386 x86 pls
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

pharlap_c_flags = -dWATCOM=7 -dCMPLR=7 -d__STKCALL=cdecl -d_Far=__far -d_Near=__Near -I"$(plsdir)"

extra_c_flags = -dPHARLAP $(compflags_$[&)

extra_c_flags_dilglue = -3s $(pharlap_c_flags)
extra_c_flags_dosxlink = $(pharlap_c_flags)
extra_c_flags_plsacc = $(pharlap_c_flags)

extra_a_flags = -dPHARLAP -ms

inc_dirs = -I"../h"

extra_objs = &
    $(dosx_serv_objs) &
    plsacc.obj &
    plsintr.obj &
    dosxrmod.obj &
    dilglue.obj

extra_srcs = $(plsdir)

!ifndef extra_linker
extra_linker = &
        op maxdata=0 runtime maxreal=1023K,maxibuf=4K,flag=2 &
        lib $(plsdir)/wdos32.lib,$(plsdir)/wdosx32.lib &
        file $(plsdir)/dilseg.obj
!endif

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif
