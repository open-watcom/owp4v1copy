#pmake: cpu_i86 i86 intel
#
# WATCOM Debugger makefile (RFX)
#
!include $(wvdir)\make.wv
dig_os = $(os)
!include $(dig_dir)/mif/trpcli.mif

wvdir  = $+$(mdir)$-
mdir   = $(wvdir)\rfx
cdir   = $(mdir)
odir   = $(mdir)
ddir   = $(wvdir)\obj
dddir  = $(wvdir)\$(os)\obj

cflags = -s -w3 -ml -zq
!ifdef on_build_machine
cflags += -d1 -oas
!else
cflags += -d2
!endif

compile  = $(comp286) $[* -fo$^@ $(cflags)
assemble = optasm $[@;

!ifeq bin fmrbin
objs  = ..\rfx.fbj
!else ifeq bin fmrbinp
objs  = ..\rfx.fbj
!else ifeq bin bin.nec
objs  = ..\rfx.nbj
!else ifeq bin binp.nec
objs  = ..\rfx.nbj
!else
objs  = ..\rfx.obj
!endif

objs += ..\rfxacc.obj &
        ..\stubs.obj &
        local$(os).obj &
        $(dddir)\remmisc.obj &
        $(dddir)\remfile.obj &
        $(dddir)\dbgfile.obj &
        $(dddir)\doserr.obj &
        $(dddir)\$(os)filio.obj &
        $(dddir)\$(os)lkup.obj &
        $(trap_objs)

!ifeq os dos
objs += inthdlrs.obj
!endif

d : .SYMBOLIC $(bdir)\dfx.exe
        @%null

$(bdir)\dfx.exe : $(objs) $(__MAKEFILES__)
        @%make additional
        wlink sys $(form) debug all name $(bdir)\dfx @rfx

w : .SYMBOLIC $(bdir)\rfx.exe
        @%null

$(bdir)\rfx.exe : $(objs) $(__MAKEFILES__)
        @%make additional
        wlink sys $(form) name $(bdir)\rfx @rfx

additional : .PROCEDURE
    @%write rfx.lnk $# rfx link file
    @%append rfx.lnk OPTION quiet,map
    @for %i in ($(objs)) do @%append rfx.lnk file %i

.NOCHECK
.ERASE

.BEFORE
        @set include=..\;$(wvdir)\$(os)\h;$(wvdir)\h;$(dig_dir)\h;$(lang_root)\h\os21x;$(%include)

.EXTENSIONS
.EXTENSIONS : .trp .exe .obj .fbj .nbj .c .asm

.c: $(cdir);$(dig_srcs)
.c.obj:
        $(compile)

.c.fbj:
        $(compile) -d_FMR_PC

.c.nbj:
        $(compile) -d_NEC_PC

.asm: $(odir)
.asm.obj:
        $(assemble)
