compile = $(comp386) $[*
auilib = $(auidir)/nt386/aui.lib
resfile = app.res
rcopt = -bt=nt
addinc = -i"$(lang_root)/h/nt"

app.exe : auistr.gh $(objs) app.lnk $(auilib) $(resfile)
        $(linker) sys nt_win @app lib $(guidir)/win/nt386/gui.lib,nt
        wrc -k $(rcopt) $(resfile) app.exe

!include ../makefile
