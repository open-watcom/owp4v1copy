.extensions:
.extensions: .exe .obj .c

!ifndef SYSTEM
!ifdef __OS2__
SYSTEM = OS2V2
!else
SYSTEM = DOS4G
!endif
!endif

compile = wcc386 /i=..\h;$(dwarfdir)\dr\h;$(dwarfdir)\dw\h

.c : $(dwarfdir)\util\c
.c.obj :
        $(compile) $[@ -oaxt-d1-w4

all : dump.exe hexit.exe build.exe
    wtouch all

dump.exe : main.obj testdump.obj
    @set OBJS=$<
    @set NAME=$^@
    @%make dolink

hexit.exe : hexit.obj
    @set OBJS=$<
    @set NAME=$^@
    @%make dolink

build.exe : build.obj
    @set OBJS=$<
    @set NAME=$^@
    @%make dolink

dolink : .PROCEDURE
    wlink system $(SYSTEM) debug all f { $(%OBJS) } name $(%NAME) op map

clean : .symbolic
        rm -f *.obj *.exe *.map *.lib *.lbc *.err *.i
