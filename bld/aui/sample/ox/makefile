compile = $(comp286) $[* -d1 -oaxt -ml
auilib = $(auidir)/ox/aui.lib

!ifdef %os2box
target=app2.exe
!else
target=app.exe
!endif

all : .symbolic $(target)
        @%null

app2.exe : $(objs) app.lnk $(auilib)
        wlink op stack=12k name app2 sys os2 pmc @app lib $(guidir)/ui/os2i86/gui.lib,$(uidir)/os2/os2i86/ui.lib,$(auilib)

app.exe : $(objs) app.lnk $(auilib)
        wlink sys dos @app lib $(guidir)/ui/dosi86/gui.lib lib $(uidir)/dos/dosi86/ui.lib,$(auilib)

!include ../makefile
