compile = $(comp386) $[* -4s -d2 -mf -dNLM -zq -s -fpc
auilib = $(auidir)/of386n/aui.lib

app.nlm : $(objs) app.lnk $(auilib)
        @%append app.lnk name app.nlm
        @%append app.lnk format novell 'app nlm'
        @%append app.lnk module clib
        @%append app.lnk option caseexact,quiet
        @%append app.lnk library $(guidir)/ui/guinlmf
        @%append app.lnk library $(uidir)/nlm/nov386/ui.lib
        @%append app.lnk libpath %WATCOM%/lib386
        @%append app.lnk libpath %WATCOM%/lib386/netware
        @%append app.lnk import  @%WATCOM%/novi/clib.imp
        @%append app.lnk import  @$(uidir)/nlm/uinlm.imp
        wlink debug novell @app
        rm -f app.lnk

!include ../makefile
