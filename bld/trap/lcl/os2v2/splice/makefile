#pmake: all build os_os2 lcl 32bit support x86
#
# Builds WDSPLICE.DLL
#

host_os  = os2
host_cpu = 386
sys_dll  = 1

!include cproj.mif
!include defrule.mif
!include deftarg.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

bdir = $(bin_root)/binp

objs = splice.obj

extra_l_flags = op oneautodata, map

.c: ../c

$(bdir)/dll/wdsplice.dll : $(objs)
    $(linker) $(lflags) name $@ file { $< } l $(os2386)
