compile = $(comp386) $[* -d2
auilib = $(auidir)/nt386/aui.lib
reslib = $(wres_dir)/lib/wresf.lib
guilib = $(guidir)/ui/nt386/gui.lib
uilib = $(uidir)/nt/nt386/ui.lib
resfile = appui.res

app.exe : $(objs) app.lnk $(auilib) $(guilib) $(uilib) $(reslib) $(resfile)
        $(linker) sys nt @app lib $(auilib),$(guilib),$(uilib),$(reslib)
        wstrip -q -a -r app.exe . $(resfile)

!include ../makefile
