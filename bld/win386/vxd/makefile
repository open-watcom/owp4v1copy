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

mlopts = -c -Cx -DMASM6 -I..\..\emu\inc
# -Sf -Fl$^*.lst

all : wdebug.386 wemu387.386 .SYMBOLIC
        @%null

wdebug.386: wdebug.def $(OBJS) $(OBJL) $(LNK)
        mslink -vxd @$(LNK)

wemu387.386: wemu387.def $(OBJS) $(OBJL) $(LNKEMU)
        mslink -vxd @$(LNKEMU)

$(LNK) : makefile
        %create $(LNK)
        @%append $(LNK) -def:wdebug.def
        @%append $(LNK) -out:wdebug.386
        @%append $(LNK) -map
        @for %i in ($(objs)) do @%append $(lnk) %i
        @%append $(LNK) $(objl)

$(LNKEMU) : makefile
        %create $(LNKEMU)
        @%append $(LNKEMU) -def:wemu387.def
        @%append $(LNKEMU) -out:wemu387.386
        @%append $(LNKEMU) -map
        @for %i in ($(objs)) do @%append $(LNKEMU) %i
        @%append $(LNKEMU) $(objl)

.obj : $(OBJ)

.asm.obj :
#        ml $(mlopts) $[@ 
	wasm -w2 -i..\..\emu\inc $[@
