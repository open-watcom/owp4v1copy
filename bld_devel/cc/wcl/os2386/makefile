#pmake: build os_os2 cpu_386

host_os  = os2
host_cpu = 386

!include cproj.mif
!include deftarg.mif

wl_opt = option quiet

all: wcl.exe wcl386.exe wclaxp.exe

dependents = ..\wcl.c ..\wclhelp.h ..\wclmsg.h

# We cannot use default rules since we need to build the same file several
# times with different switches.

wcl.exe : $(dependents)
        $(comp386) $[@ -dWCLI86 -d__OS2 -fo=wcl -os -s -zq -d1 -bt=os2 -i$(watcom_dir)\h
        wlink $(wl_opt) system os2v2 name $^@ file wcl

wcl386.exe : $(dependents)
        $(comp386) $[@ -dWCL386 -d__OS2 -fo=wcl386 -os -s -zq -d1 -bt=os2 -i$(watcom_dir)\h
        wlink $(wl_opt) system os2v2 name $^@ file wcl386

wclaxp.exe : $(dependents)
        $(comp386) $[@ -dWCLAXP -d__OS2 -fo=wclaxp -os -s -zq -d1 -bt=os2 -i$(watcom_dir)\h
        wlink $(wl_opt) system os2v2 name $^@ file wclaxp
