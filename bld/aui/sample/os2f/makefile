compile = $(comp386) $[* -d2 -mf
auilib = $(auidir)/os2386/aui.lib
guilib = $(guidir)/ui/os2386/gui.lib
uilib = $(uidir)/os2/os2386/ui.lib
reslib = $(wres_dir)/lib/wresf.lib
resfile = appui.res

app.exe : $(objs) app.lnk $(auilib) $(guilib) $(uilib) $(reslib) $(resfile)
        wlink sys os2v2 @app lib $(auilib),$(guilib),$(uilib),$(reslib)
        wstrip -q -a -r app.exe . $(resfile)

!include ../makefile
