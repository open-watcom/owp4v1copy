
host_os  = os2
host_cpu = 386

.c: ../c
.h: ../h

packinst_obj    = packinst.obj globals.obj uac_comm.obj uac_crc.obj &
                  puaccrt.obj uac_dcpr.obj uac_sys.obj punace.obj

!ifdef debug
extra_c_flags   += -DDEBUG
!endif

all: packinst.exe

packinst.exe: $(packinst_obj)
    $(linker) $(lflags) f { $(packinst_obj) }

!include cproj.mif
!include defrule.mif
!include deftarg.mif
