#pmake: os_dos os_nt os_os2 os_linux cpu_i86 intel
host_os = dos
host_cpu = i86

proj_name = ovlldr

!ifndef ovlldr_autodepends
ovlldr_autodepends = .AUTODEPEND
!endif

sobjs = sovlret.obj sovlldr.obj sovlmain.obj sovldbg.obj

lobjs = lovlret.obj lovlldr.obj lovlmain.obj lovldbg.obj

commobjs = ovlio.obj ovlboth.obj ovldata.obj ovlstd.obj ovlshare.obj &
           ngrablow.obj naddarea.obj

debugobjs = dovlboth.obj dovldata.obj

regobjs = novlldr.obj novlmain.obj novldbg.obj

mtobjs  = movlldr.obj movlmain.obj movldbg.obj

allobjs = $(lobjs) $(sobjs) $(commobjs) $(regobjs)

!include cproj.mif
!include defrule.mif
!include deftarg.mif

lib_opts = -b

extra_c_flags = -r -d1 -zu -zl -nc=_OVLCODE -nt=_TEXT -nm=$^& -Fo=$^@

extra_c_flags_sovlldr  = -mc -dOVL_SMALL
extra_c_flags_dsovlldr = -mc -dOVL_SMALL -dOVL_DEBUG
extra_c_flags_lovlldr  =
extra_c_flags_dlovlldr = -dOVL_DEBUG
extra_c_flags_novlldr  =
extra_c_flags_dnovlldr = -dOVL_DEBUG
extra_c_flags_movlldr  =
extra_c_flags_dmovlldr = -dOVL_DEBUG

extra_c_flags_sovldbg  = -mc -dOVL_SMALL
extra_c_flags_lovldbg  =
extra_c_flags_novldbg  = -dOVL_WHOOSH
extra_c_flags_movldbg  = -dOVL_WHOOSH -dOVL_MULTITHREAD

extra_c_flags_dovlboth = -dOVL_DEBUG

extra_a_flags = -i$(watcom_h) -Fo=$^@

extra_a_flags_ovldata  =
extra_a_flags_dovldata = -DOVL_DEBUG
extra_a_flags_sovlmain = -DOVL_SMALL
extra_a_flags_lovlmain =
extra_a_flags_novlmain =
extra_a_flags_movlmain =
extra_a_flags_sovlret  = -DOVL_SMALL
extra_a_flags_lovlret  =

wovl : .symbolic wovl.lib wmovl.lib
    @%null

all : .symbolic wovl.lib wmovl.lib dovl.lib dovls.lib dovll.lib dovln.lib
    @%null

wovl.lib : $(allobjs)
    @%make makelib

wmovl.lib : $(commobjs) $(mtobjs)
    @%make makelib

dovl.lib : $(allobjs)
    @%make makedbglib

dovls.lib : $(debugobjs) dsovlldr.obj sovlmain.obj sovldbg.obj
    @%make makedbglib

dovll.lib : $(debugobjs) dlovlldr.obj lovlmain.obj lovldbg.obj
    @%make makedbglib

dovln.lib : $(debugobjs) dnovlldr.obj novlmain.obj novldbg.obj &
         naddarea.obj ngrablow.obj
    @%make makedbglib

dsovlldr.obj : ovlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

sovlldr.obj : ovlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dlovlldr.obj : ovlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

lovlldr.obj : ovlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dnovlldr.obj : novlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dmovlldr.obj : movlldr.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dovlboth.obj : ovlboth.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

sovldbg.obj : ovldbg.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

lovldbg.obj : ovldbg.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

novldbg.obj : ovldbg.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

movldbg.obj : ovldbg.c
    $(cc) $(cflags) $(extra_c_flags_$^&) $[@

dovldata.obj : ovldata.asm
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
