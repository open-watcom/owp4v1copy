#pmake: build cpu_386

!include cproj.mif

all: cwdllr.lib cwdlls.lib

dllfuncr.obj: dllfunc.asm
    wasm dllfunc.asm $(mode_aflags) -bt=dos -mf -3r -w4 -zq -fo=dllfuncr.obj

cwdllr.lib: dllfuncr.obj cwapi.obj
    wlib -b -c -n -q -p=512 cwdllr.lib +dllfuncr.obj +cwapi.obj

dllfuncs.obj: dllfunc.asm
    wasm dllfunc.asm $(mode_aflags) -bt=dos -mf -3s -w4 -zq -fo=dllfuncs.obj

cwdlls.lib: dllfuncs.obj cwapi.obj
    wlib -b -c -n -q -p=512 cwdlls.lib +dllfuncs.obj +cwapi.obj

clean: .SYMBOLIC
    rm *.lib
    rm dllfuncr.obj
    rm dllfuncs.obj
