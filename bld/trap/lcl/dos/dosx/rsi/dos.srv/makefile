#pmake: all build serv lcl os_dos dsx cpu_386 x86
host_CPU = i86
host_OS = dos

os = rsi
srv = rsi
cpu = x86
name = rsihelp
which = SERVER

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

extra_c_flags = -dDOS4G -dDOSXHELP -zdp -zu -wcd=1177
extra_c_flags_rsiacc = -I"$(rsilib)"
extra_c_flags_realmode = -I"$(rsilib)"
extra_c_flags_dbglib = -I"$(rsilib)"
extra_a_flags_rsi87 = -fpi87
extra_a_flags_segs_16m = -I"$(rsilib)" -Dcompact
extra_a_flags_dbgliba = -I"$(rsilib)" -Dcompact
extra_a_flags_rmhdlr = -I"$(rsilib)" -Dcompact

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
    @%append $(lf) $(rsilib)/$(libnam).lib $(lang_root)/lib286/dos/clibc.lib
    !$(tools_root)\glu @$(lf)

#until wasm gets fixed up

assemble = $(tools_root)\masm -mx -s -zd -I$(rsilib) -Dcompact $[@;

segs_16m.obj: segs_16m.asm
        $(assemble)

dbgliba.obj: dbgliba.asm
        $(assemble)

rmhdlr.obj: rmhdlr.asm
        $(assemble)
