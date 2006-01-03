#pmake: build os_dos os_nt os_os2 os_linux cpu_i86 intel
host_os = dos
host_cpu = i86

proj_name = ovlldr
ovlldr_autodepends = .autodepend

sobjs = sovlret.obj sovlmain.obj sovldbg.obj ovlstd.obj ovlshare.obj
lobjs = lovlret.obj lovlmain.obj lovldbg.obj ovlstd.obj ovlshare.obj
nobjs = novlmain.obj novldbg.obj naddarea.obj ngrablow.obj
mtobjs  = movlmain.obj movldbg.obj naddarea.obj
commobjs = ovlio.obj

regobjs = ovlboth.obj ovldata.obj
debugobjs = dovlboth.obj dovldata.obj

!include cproj.mif
!include defrule.mif
!include deftarg.mif

lib_opts = -b -p16

extra_c_flags = $(inc_path) -Fo=$^@ -r -d1 -zu -zl -nc=_OVLCODE -nt=_TEXT -nm=$^&

extra_c_flags_sovlldr  = -mc -DOVL_SMALL
extra_c_flags_dsovlldr = -mc -DOVL_SMALL -DOVL_DEBUG
extra_c_flags_lovlldr  =
extra_c_flags_dlovlldr = -DOVL_DEBUG
extra_c_flags_novlldr  =
extra_c_flags_dnovlldr = -DOVL_DEBUG
extra_c_flags_movlldr  = -DOVL_MULTITHREAD
extra_c_flags_dmovlldr = -DOVL_MULTITHREAD -DOVL_DEBUG

extra_c_flags_sovldbg  = -mc -DOVL_SMALL
extra_c_flags_lovldbg  =
extra_c_flags_novldbg  = -DOVL_WHOOSH
extra_c_flags_movldbg  = -DOVL_WHOOSH -DOVL_MULTITHREAD

extra_c_flags_dovlboth = -DOVL_DEBUG

extra_a_flags = -Fo=$^@ -i$(watcom_h)

extra_a_flags_ovldata  =
extra_a_flags_dovldata = -DOVL_DEBUG

extra_a_flags_sovlmain = -DOVL_SMALL
extra_a_flags_lovlmain =
extra_a_flags_novlmain =
extra_a_flags_movlmain = -DOVL_MULTITHREAD

extra_a_flags_sovlret  = -DOVL_SMALL
extra_a_flags_lovlret  =

wovl : .symbolic wovl.lib wmovl.lib
    @%null

all : .symbolic wovl dovl.lib dovls.lib dovll.lib dovln.lib dovlmt.lib
    @%null

wovl.lib : lovlldr.obj sovlldr.obj novlldr.obj &
        $(lobjs) $(sobjs) $(nobjs) $(commobjs) $(regobjs)
    @%make makelib

wmovl.lib : movlldr.obj $(mtobjs) $(commobjs) $(regobjs)
    @%make makelib

dovl.lib : dlovlldr.obj dsovlldr.obj dnovlldr.obj &
        $(lobjs) $(sobjs) $(nobjs) $(commobjs) $(debugobjs)
    @%make makedbglib

dovls.lib : dsovlldr.obj $(sobjs) $(commobjs) $(debugobjs)
    @%make makedbglib

dovll.lib : dlovlldr.obj $(lobjs) $(commobjs) $(debugobjs)
    @%make makedbglib

dovln.lib : dnovlldr.obj $(nobjs) $(commobjs) $(debugobjs)
    @%make makedbglib

dovlmt.lib : dmovlldr.obj $(mtobjs) $(commobjs) $(debugobjs)
    @%make makedbglib

.h : $(watcom_h)
.inc : $(watcom_h)

dsovlldr.obj : ovlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

sovlldr.obj : ovlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dlovlldr.obj : ovlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

lovlldr.obj : ovlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dnovlldr.obj : novlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dmovlldr.obj : movlldr.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dovlboth.obj : ovlboth.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

sovldbg.obj : ovldbg.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

lovldbg.obj : ovldbg.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

novldbg.obj : ovldbg.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

movldbg.obj : ovldbg.c $($(proj_name)_autodepends)
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dovldata.obj : ovldata.asm $($(proj_name)_autodepends)
    $(as) $(aflags) $(extra_a_flags_$^&) $[@


makedbglib: .procedure
    %create $*.lbc
    @for %i in ($?) do @%append $*.lbc -+%i
    $(librarian) $(lib_opts) $@ @$*.lbc

makelib: .procedure
    %create $*.lbc
    @for %i in ($?) do @%append $*.lbc -+%i
!ifeq release 0
    $(librarian) $(lib_opts) $@ @$*.lbc
!else
    $(librarian) $(lib_opts) -s -t $@ @$*.lbc
!endif
