#pmake: build os_linux cpu_386

host_os  = linux
host_cpu = 386

!include cproj.mif
!include deftarg.mif

wl_opt = option quiet

all: wcl.exe wcl386.exe # wclaxp.exe

dependents = ../wcl.c ../wclhelp.h ../wclmsg.h

# We cannot use default rules since we need to build the same file several
# times with different switches.

wcl.exe : $(dependents)
        $(comp386) $[@ -dWCLI86 -fo=wcl -ox -zq -d1 -bt=linux -i"$(inc_path)"
        wlink $(wl_opt) system linux name $^@ file wcl

wcl386.exe : $(dependents)
        $(comp386) $[@ -dWCL386 -fo=wcl386 -ox -zq -d1 -bt=linux -i"$(inc_path)"
        wlink $(wl_opt) system linux name $^@ file wcl386
        cp $^@ ../../../build/binl/wcl386

wclaxp.exe : $(dependents)
        $(comp386) $[@ -dWCLAXP -fo=wclaxp -ox -zq -d1 -bt=linux -i"$(inc_path)"
        wlink $(wl_opt) system linux name $^@ file wclaxp
