compile = $(comp286) $[* -d1 -mm -oax
auilib = $(auidir)/dosi86/aui.lib
auilib2 = $(auidir)/os2i86/aui.lib

!ifdef %os2box
target=app2.exe
!else
target=app.exe
!endif

all : .symbolic $(target)
        @%null

app2.exe : $(objs) app.lnk $(auilib2)
        wlink op stack=12k name app2 sys os2 pmc @app lib $(guidir)/dosi86/gui.lib,$(auilib2)

app.exe : $(objs) app.lnk $(auilib)
        wlink sys dos @app lib $(guidir)/dosi86/gui.lib,$(auilib)

!include ../makefile
