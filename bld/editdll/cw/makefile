!include deftarg.mif

lnk = tmp.lnk

!ifeq host_OS win

# windows version
weditcw.dll : weditcw.obj
    @%create $(lnk)
    @%append $(lnk) system windows dll debug all
    @%append $(lnk) name wedit
    @%append $(lnk) option modname=wedit
    @%append $(lnk) option description 'Open Watcom Editor DLL for Codewriter'
    @%append $(lnk) option map, elim, verbose, static
    @%append $(lnk) library ddeml
    @%append $(lnk) option stack=8k, symfile
    @%append $(lnk) segment  _TEXT PRELOAD FIXED
    @%append $(lnk) segment  CLASS 'CODE' LOADONCALL MOVEABLE DISCARDABLE
    @%append $(lnk) export   WEP RESIDENT
    @%append $(lnk) file %WATCOM%\lib286\win\libentry.obj, weditcw.obj
    wlink @$(lnk)
    wlib -b -n wedit.lib +wedit.dll
    @del $(lnk)

weditcw.obj : ../weditcw.c
    wcc $< /i=..\..;$(%watcom)\h\win /mc /d1 /oaxt /zc /zw /zu /zq /we

!else ifeq host_OS nt

# NT version
weditcw.dll : weditcw.obj
    @%create $(lnk)
    @%append $(lnk) system nt_dll debug all
    @%append $(lnk) name wedit
    @%append $(lnk) option modname=wedit
    @%append $(lnk) option description 'Open Watcom Editor DLL for Codewriter'
    @%append $(lnk) option map, elim, verbose, static
    @%append $(lnk) file weditcw.obj
    wlink @$(lnk)
    wlib -b -n wedit.lib +wedit.dll
    @del $(lnk)

weditcw.obj : ../weditcw.c
    wcc386 $< /i=..\..;$(%watcom)\h\nt -bt=nt /d1 /oaxt /we

!endif