#pmake: build os_win cpu_i86

all : idex.cfg pbidex.cfg pbide.cfg .symbolic

idex.cfg : ../idex.mfg
    wsplice -k FOR_WIN $[@ $^@

pbidex.cfg : ../idex.mfg
    wsplice -k C_FOR_PB -k FOR_WIN $[@ $^@

pbide.cfg : ../ide.mfg
    wsplice -k C_FOR_PB -k FOR_WIN $[@ $^@

clean : .symbolic
    rm -f *.cfg
