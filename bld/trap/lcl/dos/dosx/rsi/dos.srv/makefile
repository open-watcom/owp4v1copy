#pmake: all build serv lcl os_dos dsx cpu_386 x86
host_CPU = i86
host_OS = dos

os = rsi
srv = rsi
cpu = x86
name = rsihelp
which = SERVER

rsidir = $(tools_root)

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

extra_a_flags = -Dcompact
extra_a_flags_npxtype = -fpi87
extra_a_flags_rsi87 = -fpi87

extra_srcs = $(rsilib);
inc_dirs = $(rsilib);

!define substitute_linker

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif

objs += $(lang_root)/lib286/dos/dos16m.obj &
        segs_16m.obj &
        magicstr.obj

lf = tmp.lnk

$(target) : $(extra_libs) $(objs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp($^@) map($^&) log($^&.err)
    @%append $(lf) -mpl -nod -DPMI -map -acg -farcall -auto -quiet -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/d16libc.lib
    @%append $(lf) $(lang_root)/lib286/dos/clibc.lib $(extra_libs)
    !$(rsidir)\glu @$(lf)
