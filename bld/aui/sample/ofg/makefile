compile = $(comp386) $[* -d2 -mf -dDOS
auilib = $(auidir)/dos386/aui.lib

app.exe : $(objs) app.lnk $(auilib)
        wlink sys dos4g @app lib $(guidir)/ui/dos386/gui.lib,$(uidir)/dos/dos386/ui.lib

!include ../makefile
