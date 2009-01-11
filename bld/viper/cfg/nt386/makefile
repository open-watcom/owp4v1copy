#pmake: build os_nt cpu_386

all : idex.cfg pbidex.cfg pbide.cfg .symbolic

idex.cfg : ../idex.mfg
    wsplice -k FOR_NT $[@ $^@

pbidex.cfg : ../idex.mfg
    wsplice -k C_FOR_PB -k FOR_NT $[@ $^@

pbide.cfg : ../ide.mfg
    wsplice -k C_FOR_PB -k FOR_NT $[@ $^@

clean : .symbolic
    rm -f *.cfg
