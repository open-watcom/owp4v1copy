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

objs = splice.obj

extra_l_flags = op oneautodata, map

.c: ../c

wdsplice.dll : $(objs)
    $(linker) $(lflags) name $@ file { $< } l $(os2386)
