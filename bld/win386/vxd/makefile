#pmake: all build os_win cpu_386
host_OS=win
host_CPU=386

!include cproj.mif
!include deftarg.mif
!include defrule.mif

LNK = wdebug.lnk
LNKEMU = wemu387.lnk

all : wdebug.386 wemu387.386 .SYMBOLIC
        @%null

wdebug.386: wdebug.obj $(emu386)\win386\emu387.obj $(LNK)
        $(linker) @$(LNK)

wemu387.386: wdebug.obj $(emu386)\win386\emu387.obj $(LNKEMU)
        $(linker) @$(LNKEMU)

$(LNK) : makefile
        %create $(LNK)
        @%append $(LNK) option description 'Win386 WDEBUG Device (Version 1.1 Open Watcom)'
        @%append $(LNK) name WDEBUG.386
        @%append $(LNK) file $(emu386)\win386\emu387.obj
        @%append $(LNK) @..\vxd.lnk

$(LNKEMU) : makefile
        %create $(LNKEMU)
        @%append $(LNKEMU) option description 'Win386 WEMU387 Device (Version 1.1 Open Watcom)'
        @%append $(LNKEMU) name WEMU387.386
        @%append $(LNKEMU) file $(emu386)\win386\emu387.obj
        @%append $(LNKEMU) @..\vxd.lnk

wdebug.obj : wdebug.asm wdebug.inc $(emu386)\inc\win30vxd.inc
        $(as) $[@ -mf -i$(emu386)\inc

