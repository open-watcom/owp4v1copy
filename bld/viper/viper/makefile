#pmake: build os_nt os_win os_os2 os_linux cpu_386 cpu_axp
 
all : .symbolic IDE PBIDE
IDE : win/idex.cfg nt/idex.cfg ide.cfg .SYMBOLIC
PBIDE : win/pbidex.cfg nt/pbidex.cfg win/pbide.cfg nt/pbide.cfg .SYMBOLIC
 
win/idex.cfg : win/idex.mfg
        wsplice -r C_FOR_PB $[@ $^@
 
nt/idex.cfg : nt/idex.mfg
        wsplice -r C_FOR_PB $[@ $^@
 
win/pbidex.cfg : win/idex.mfg
        wsplice -k C_FOR_PB -r C_FOR_PB_NT $[@ $^@
 
nt/pbidex.cfg : nt/idex.mfg
        wsplice -k C_FOR_PB -k C_FOR_PB_NT $[@ $^@
 
ide.cfg : ide.mfg
        wsplice -r C_FOR_PB $[@ $^@
 
win/pbide.cfg : ide.mfg
        wsplice -k C_FOR_PB -r C_FOR_PB_NT $[@ $^@
 
nt/pbide.cfg : ide.mfg
        wsplice -k C_FOR_PB -k C_FOR_PB_NT $[@ $^@
 
clean: .SYMBOLIC
        rm -f win/*.cfg
        rm -f nt/*.cfg
        rm -f ide.cfg
