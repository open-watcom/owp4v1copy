compile = $(comp286) $[* -d2 -ml -zW
auilib = $(auidir)/wini86/aui.lib
guilib = $(guidir)/win/wini86/gui.lib
rcopt   = -bt=windows
resfile = app.res

app.exe : $(objs) app.lnk $(auilib) $(guilib) $(resfile)
        $(linker) sys windows @app lib $(guilib),$(auilib)
        cp app.exe appj.exe
#bug    segment CLASS 'CODE' LOADONCALL DISCARDABLE
        wrc -k $(rcopt) $(resfile)
        wrc -k $(rcopt) appj.res

!include ../makefile
