#pmake: build os_dos cpu_i86

host_os  = dos
host_cpu = i86

!include cproj.mif
!include deftarg.mif

wl_opt = option quiet

all: wcl.exe wcl386.exe wclaxp.exe

dependents = ..\wcl.c ..\wclhelp.h ..\wclmsg.h swchar.obj

# We cannot use default rules since we need to build the same file several
# times with different switches.

wcl.exe : $(dependents)
        $(comp286) $[@ -dWCLI86 -d__DOS -fo=wcl -os -s -zq -d1 -bt=dos -i$(watcom_dir)\h
        wlink $(wl_opt) system dos name $^@ file wcl,swchar

wcl386.exe : $(dependents)
        $(comp286) $[@ -dWCL386 -d__DOS -fo=wcl386 -os -s -zq -d1 -bt=dos -i$(watcom_dir)\h
        wlink $(wl_opt) system dos name $^@ file wcl386,swchar

wclaxp.exe : $(dependents)
        $(comp286) $[@ -dWCLAXP -d__DOS -fo=wclaxp -os -s -zq -d1 -bt=dos -i$(watcom_dir)\h
        wlink $(wl_opt) system dos name $^@ file wclaxp,swchar

swchar.obj: ..\..\c\swchar.c
        $(comp286) $[@ -zq -os -s -d1 -i$(watcom_dir)\h;..;..\..\h
