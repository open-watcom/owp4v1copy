proj_name = ifbuild

host_CPU = 386
!ifdef __LINUX__
host_OS = linux
!else ifdef __OS2__
host_OS = os2
!else ifdef __NT__
host_OS = nt
!else
host_OS = dos
!endif

!include cproj.mif
!include deftarg.mif
!include defrule.mif

ifbuild_objs = &
    ifbuild.obj &
    ifnames.obj &
    ifargtyp.obj &
    ifflags.obj &
    ifargct.obj &
    ifselect.obj

.c : c;../../cp;

inc_dirs = ../h;../../h;

extra_c_flags = -zp1 -d__RT__

extra_l_flags = op map

ifbuild.exe : $(ifbuild_objs)
        $(linker) $(lflags) name $^@ file {$(ifbuild_objs)}
