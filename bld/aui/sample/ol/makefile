compile = $(comp286) $[* -d2 -ml -dDOS
auilib  = $(auidir)/dosi86/aui.lib
auilib2 = $(auidir)/os2i86/aui.lib
guilib  = $(guidir)/ui/dosi86/gui.lib
guilib2 = $(guidir)/ui/os2i86/gui.lib
reslib = $(wres_dir)/lib/wresl.lib
resfile = appui

app.exe appj.exe : $(objs) app.lnk $(auilib2) $(reslib) $(guilib2) $(resfile).res $(resfile)j.res
        $(linker) op stack=12k name app sys os2 @app &
                lib $(reslib),$(guilib2),$(uidir)/os2/os2i86/ui.lib,$(auilib2)
        cp app.exe appj.exe
        wstrip -q -a -r app.exe . $(resfile).res
        wstrip -q -a -r appj.exe . $(resfile)j.res

appdos.exe : $(objs) app.lnk $(auilib) $(reslib) $(guilib) $(resfile).res $(resfile)j.res
        $(linker) op stack=12k name appdos sys dos @app &
                lib $(reslib),$(guilib),$(uidir)/dos/dosi86/ui.lib,$(auilib)
        wstrip -q -a -r appdos.exe . $(resfile).res

!include ../makefile
