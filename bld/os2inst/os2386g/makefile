
host_os  = os2
host_cpu = 386

sys_windowed = 1

.extensions: .rc

.c: ../c
.h: ../h
.rc: ..

inst_obj        = install.obj instutil.obj globals.obj uac_comm.obj uac_crc.obj &
                  uac_crt.obj uac_dcpr.obj uac_sys.obj unace.obj
uninst_obj      = uninstal.obj

!ifdef debug
extra_c_flags   += -DDEBUG
!endif

all: install.exe # uninstal.exe

install.exe: $(inst_obj) ..\install.rc
    $(linker) $(lflags) f { $(inst_obj) }
    rc -i $(%watcom)\h\os2 -i ..\h ..\install.rc -r install.res
    rc install.res

uninstal.exe: $(uninst_obj) ..\uninstal.rc
    $(linker) $(lflags) f { $(uninst_obj) }
    rc -i $(%watcom)\h\os2 -i ..\h ..\uninstal.rc uninstal.exe

!include cproj.mif
!include defrule.mif
!include deftarg.mif
