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

wdebug.386: wdebug.obj $(emu386)\win386\emu387w.obj $(LNK)
        wlink @$(LNK)

wemu387.386: wdebug.obj $(emu386)\win386\emu387w.obj $(LNKEMU)
        wlink @$(LNKEMU)

$(LNK) : makefile
        %create $(LNK)
        @%append $(LNK) file wdebug.obj
        @%append $(LNK) file $(emu386)\win386\emu387w.obj
        @%append $(LNK) form windows vxd
        @%append $(LNK) name WDEBUG.386
        @%append $(LNK) export WDEBUG_DDB.1
        @%append $(LNK) option map
        @%append $(LNK) option description 'Win386 WDEBUG Device  (Version 1.0)'
        @%append $(LNK) segment '_LTEXT'  preload nondiscardable
        @%append $(LNK) segment '_LDATA'  preload nondiscardable
        @%append $(LNK) segment '_ITEXT'  discardable
        @%append $(LNK) segment '_IDATA'  discardable
        @%append $(LNK) segment '_TEXT'   nondiscardable
        @%append $(LNK) segment '_DATA'   nondiscardable

$(LNKEMU) : makefile
        %create $(LNKEMU)
        @%append $(LNKEMU) file wdebug.obj
        @%append $(LNKEMU) file $(emu386)\win386\emu387w.obj
        @%append $(LNKEMU) form windows vxd
        @%append $(LNKEMU) name WEMU387.386
        @%append $(LNKEMU) option modname=WDEBUG
        @%append $(LNKEMU) export WDEBUG_DDB.1
        @%append $(LNKEMU) option map
        @%append $(LNKEMU) option description 'Win386 WEMU387 Device  (Version 1.0)'
        @%append $(LNKEMU) segment '_LTEXT'  preload nondiscardable
        @%append $(LNKEMU) segment '_LDATA'  preload nondiscardable
        @%append $(LNKEMU) segment '_ITEXT'  discardable
        @%append $(LNKEMU) segment '_IDATA'  discardable
        @%append $(LNKEMU) segment '_TEXT'   nondiscardable
        @%append $(LNKEMU) segment '_DATA'   nondiscardable

.asm.obj :
        $(as) -mf -i..\..\emu\inc $[@

