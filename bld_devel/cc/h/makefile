#pmake/50: build headers cpu_386 cpu_axp os_nt os_os2 os_dos os_win os_osi os_qnx

!include cproj.mif

all: weights.gh keywords.gh .symbolic
        @%null

weights.gh keywords.gh : c.key pc.key seh.key dummy.key
        $(bld_cl) $(fe_misc_dir)\c\findhash.c $(wcl_util_opts)
        findhash -q  c.key pc.key seh.key dummy.key
        del findhash.*


clean: .SYMBOLIC
        @if exist *.gh del *.gh
        @if exist cerror.gml del cerror.gml
