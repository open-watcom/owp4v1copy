#pmake/50: build headers cpu_386 cpu_axp os_nt os_os2 os_dos os_win os_osi os_qnx os_linux

!include cproj.mif

all: weights.gh keywords.gh .symbolic
        @%null

./findhash.exe : $(fe_misc_dir)/c/findhash.c
        $(bld_cl) $< $(wcl_util_opts) -fe=$@

.BEFORE rm -f findhash.exe
weights.gh keywords.gh : ./findhash.exe c.key pc.key seh.key dummy.key
        $[@ -q  c.key pc.key seh.key dummy.key

clean: .SYMBOLIC
        rm -f *.gh *.exe cerror.gml
