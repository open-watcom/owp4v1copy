#pmake: all build lcl os_win cpu_i86 cpu_386 support x86
host_cpu = i86
host_os = win

NAME = wint32

memory_model_i86 = c

sys_dll = 1

OBJS =  intmain.obj ints.obj int32.obj

!include cproj.mif
!include deftarg.mif
!include defrule.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

#CFLAGS = -zq -s -oas -fpc -mc -w3 -d1 -i=$(%lang)\h;$(%lang)\h\win;$(watcom_dir)\h
extra_c_flags = -s -fpc -zu -zw

extra_l_flags_win = initinstance memory
extra_l_flags_win += option heapsize=1k
extra_l_flags_win += export WEP.1 RESIDENT
extra_l_flags_win += segment CLASS 'CODE' PRELOAD FIXED
extra_l_flags_win += segment CLASS 'FAR_DATA' PRELOAD FIXED
extra_l_flags_win += segment CLASS 'BEGDATA' PRELOAD FIXED
extra_l_flags_win += segment CLASS 'DATA' PRELOAD FIXED
extra_l_flags_win += segment CLASS 'BSS' PRELOAD FIXED
extra_l_flags_win += library windows,clibc,toolhelp
extra_l_flags_win += file {$(objs)}

w : .symbolic $(bin_root)/bin/$(name).dll
        @%null

$(bin_root)/bin/$(name).dll: $(OBJS)
       $(linker) $(lflags) name $^@

.c : ../c
#.c.obj :
#        $(comp286) $[* $(CFLAGS) -zu -zw
#        $(cc) $[* $(CFLAGS) -zu -zw

extra_a_flags = -fpc -mc

.asm : ../asm
#.asm.obj :
#        $(as) $[* -fpc -mc -w3 -d1 -i=$(%lang)\h;$(%lang)\h\win;$(watcom_dir)\h
