#pmake: os_nt cpu_axp

host_os  = nt
host_cpu = axp

!include cproj.mif
!include deftarg.mif

wl_opt = option quiet

all: wcl.exe wcl386.exe wclaxp.exe

dependents = ..\wcl.c ..\wclhelp.h ..\wclmsg.h

# We cannot use default rules since we need to build the same file several
# times with different switches.

wcl.exe : $(dependents)
        $(compaxp) $[@ -dWCLI86 -d__NT -fo=wcl -os -s -zq -d1 -bt=nt -i$(watcom_dir)\h
        wlink $(wl_opt) system ntaxp name $^@ file wcl

wcl386.exe : $(dependents)
        $(compaxp) $[@ -dWCL386 -d__NT -fo=wcl386 -os -s -zq -d1 -bt=nt -i$(watcom_dir)\h
        wlink $(wl_opt) system ntaxp name $^@ file wcl386

wclaxp.exe : $(dependents)
        $(compaxp) $[@ -dWCLAXP -d__NT -fo=wclaxp -os -s -zq -d1 -bt=nt -i$(watcom_dir)\h
        wlink $(wl_opt) system ntaxp name $^@ file wclaxp
