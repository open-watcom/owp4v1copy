#
# make stubs for real-mode compilers and tools
#
all: .SYMBOLIC wcc386p.exe wccx.exe wfcp.exe wfc386p.exe wlinkp.exe
        @%null

wcc386p.exe: execpgm.c
        $(comp286) execpgm /ox /dPGM_NAME="wcc386.exe" /zq
        wlink system dos name wcc386p file execpgm
wfc386p.exe: execpgm.c
        $(comp286) execpgm /ox /dPGM_NAME="wfc386.exe" /zq
        wlink system dos name wfc386p file execpgm
wccx.exe: execpgm.c
        $(comp286) execpgm /ox /dPGM_NAME="wcc.exe" /zq
        wlink system dos name wccx file execpgm
wfcp.exe: execpgm.c
        $(comp286) execpgm /ox /dPGM_NAME="wfc.exe" /zq
        wlink system dos name wfcp file execpgm
wlinkp.exe: execpgm.c
        $(comp286) execpgm /ox /dPGM_NAME="wlink.exe" /zq
        wlink system dos name wlinkp file execpgm

