!include deftarg.mif
!ifeq host_OS win

# windows version
weditcw.dll : weditcw.obj
    wlink @..\weditcw.lnk
    wlib -b -n weditcw.lib +weditcw.dll

weditcw.obj : ../weditcw.c
    wcc $< /i=..\..;$(%watcom)\h\win /mc /d1 /oaxt /zc /zw /zu /zq /we

!else ifeq host_OS nt

# NT version
weditcwn.dll : weditcwn.obj
    wlink debug all @..\weditcwn.lnk
    wlib -b -n weditcwn.lib +weditcwn.dll

weditcwn.obj : ../weditcw.c
    wcc386 $< /i=..\..;$(%watcom)\h\nt -bt=nt /d1 /oaxt /we -fo=weditcwn.obj

!endif