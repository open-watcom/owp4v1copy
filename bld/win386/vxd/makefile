#pmake: all build os_win cpu_386

!include deftarg.mif

LNK = wdebug.lnk
LNKEMU = wemu387.lnk

objs =  wdebug.obj
objl = $(emu386)\win386\emu387w.obj

##############################################################
# it uses still MS Windows 98 DDK tools for VxD creation
# ML.EXE (MASM V6) and LINK.EXE, don't require anything more
#
# ML.EXE should copy to bld\bin directory
# LINK.EXE should copy as MSLINK.EXE to bld\bin directory
##############################################################

#MASM = 1
mlopts = -c -Cx -DMASM6 -I..\..\emu\inc

all : wdebug.386 wemu387.386 .SYMBOLIC
        @%null

wdebug.386: wdebug.def $(OBJS) $(OBJL) $(LNK)
!ifdef MASM
        mslink -vxd @$(LNK)
!else
        wlink @$(LNK)
!endif

wemu387.386: wemu387.def $(OBJS) $(OBJL) $(LNKEMU)
!ifdef MASM
        mslink -vxd @$(LNKEMU)
!else
        wlink @$(LNKEMU)
!endif

$(LNK) : makefile
        %create $(LNK)
!ifdef MASM
        @%append $(LNK) -def:wdebug.def
        @%append $(LNK) -out:wdebug.386
        @%append $(LNK) -map
        @for %i in ($(objs)) do @%append $(lnk) %i
        @%append $(LNK) $(objl)
!else
        @for %i in ($(objs)) do @%append $(lnk) file %i
        @%append $(LNK) libfile $(objl)
        @%append $(LNK) form OS2 LE VIRT
        @%append $(LNK) name wdebug.386
        @%append $(LNK) export WDEBUG_DDB.1
        @%append $(LNK) option map
        @%append $(LNK) option description 'Win386 WDEBUG Device  (Version 1.0)'
        @%append $(LNK) segment '_LTEXT'  preload
        @%append $(LNK) segment '_LDATA'  preload
        @%append $(LNK) segment '_ITEXT'  discardable
        @%append $(LNK) segment '_IDATA'  discardable
!endif

$(LNKEMU) : makefile
        %create $(LNKEMU)
!ifdef MASM
        @%append $(LNKEMU) -def:wemu387.def
        @%append $(LNKEMU) -out:wemu387.386
        @%append $(LNKEMU) -map
        @for %i in ($(objs)) do @%append $(LNKEMU) %i
        @%append $(LNKEMU) $(objl)
!else
        @for %i in ($(objs)) do @%append $(LNKEMU) file %i
        @%append $(LNKEMU) libfile $(objl)
        @%append $(LNKEMU) form OS2 LE VIRT
        @%append $(LNKEMU) name wemu387.386
        @%append $(LNKEMU) option modname=WDEBUG
        @%append $(LNKEMU) export WDEBUG_DDB.1
        @%append $(LNKEMU) option map
        @%append $(LNKEMU) option description 'Win386 WEMU387 Device  (Version 1.0)'
        @%append $(LNKEMU) segment '_LTEXT'  preload
        @%append $(LNKEMU) segment '_LDATA'  preload
        @%append $(LNKEMU) segment '_ITEXT'  discardable
        @%append $(LNKEMU) segment '_IDATA'  discardable
!endif

.obj : $(OBJ)

.asm.obj :
!ifdef MASM
        ml $(mlopts) $[@ 
!else
        wasm -mf -i..\..\emu\inc $[@
!endif

