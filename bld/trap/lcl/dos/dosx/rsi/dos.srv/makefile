#pmake: all build serv lcl os_dos dsx cpu_386 x86
host_CPU = i86
host_OS = dos

os = rsi
srv = rsi
cpu = x86
name = rsihelp
which = SERVER

masmdir = $(tools_root)
rsidir = $(tools_root)

#until wasm gets fixed up

assemble = $(masmdir)\masm -mx -s -I$(rsilib) -Dcompact $[@; # masm 5
#assemble = ml -nologo -c -Cx -Zm -I$(rsilib) -Dcompact $[@ # masm 6

extra_objs = &
        $(dosx_serv_objs) &
        dosredir.obj &
        squish87.obj &
        x86cpu.obj &
        npxtype.obj &
        rsiacc.obj &
        rsi87.obj &
        realmode.obj &
        dbglib.obj &
        dbgliba.obj &
        rmhdlr.obj &
        sbrk.obj

extra_c_flags = -dDOS4G -dDOSXHELP -zdp -zu
extra_c_flags_rsiacc = -I"$(rsilib)"
extra_c_flags_realmode = -I"$(rsilib)"
extra_c_flags_dbglib = -I"$(rsilib)" -wcd=1177

extra_a_flags = -Dcompact
extra_a_flags_npxtype = -fpi87
extra_a_flags_rsi87 = -fpi87
extra_a_flags_segs_16m = -I"$(rsilib)"
extra_a_flags_dbgliba = -I"$(rsilib)"
extra_a_flags_rmhdlr = -I"$(rsilib)"

extra_srcs = $(rsilib)

!define substitute_linker

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif

objs = &
        $(%WATCOM)/lib286/dos/dos16m.obj &
        segs_16m.obj &
        magicstr.obj &
        $+$(objs)$-

lf = tmp.lnk
libnam  = d16libc

$(target) $(dtarget) : $(objs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp($^@) map($^&) log($^&.err)
    @%append $(lf) -mpl -nod -DPMI -map -acg -farcall -auto -quiet -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/$(libnam).lib
    @%append $(lf) $(lang_root)/lib286/dos/clibc.lib
    !$(rsidir)\glu @$(lf)

!ifdef assemble
segs_16m.obj : segs_16m.asm
    $(assemble)

dbgliba.obj : dbgliba.asm
    $(assemble)

rmhdlr.obj : rmhdlr.asm
    $(assemble)
!endif
