#pmake: all build os_nov cpu_386
host_cpu = 386
host_os = nov

suppress_zc = 1

!include cproj.mif
!include defrule.mif

!ifdef __LINUX__
description = \'OpenWatcom Execution Sampler Version 1.0\'
!else
description = 'OpenWatcom Execution Sampler Version 1.0'
!endif

objects = realmode.obj nwhooks.obj sample.obj sampdata.obj sampnlm.obj &
          io.obj sysio.obj settime.obj wmsg.obj pitstuff.obj cpuspeed.obj

extra_c_flags = -3s -dREPORT_TYPE= -dFAR_PTR= -dNETWARE -fi=watcom.h -zp1

inc_dirs=$(ow_nwoss_root)/common/h;$(ow_nwoss_root)/nw4/h;$(nwsrc);$(dig_dir)/h;

imports = &
    FileServerMajorVersionNumber &
    AllocateResourceTag &
    ScheduleSleepAESProcessEvent &
    MapAbsoluteAddressToDataOffset &
    Abend  &
    Pause  &
    Disable  &
    Enable  &
    RegisterForEventNotification  &
    UnRegisterEventNotification  &
    systemConsoleScreen  &
    OutputToScreen  &
    LoadModule  &
    LoadedList &
    Alloc &
    delay &
    GetCurrentTime &
    ImportSymbol &
    Free

xname = wsample

../bin/$(xname).nlm : $(xname).res  $objects makefile
    @%write $(%debug) tmp.lnk name $^@
    @%append tmp.lnk option osname='Novell NLM'
    @%append tmp.lnk format novell nlm $(description)
    @%append tmp.lnk debug novell
    @%append tmp.lnk debug all
    @%append tmp.lnk option map
    @%append tmp.lnk option caseexact
    @%append tmp.lnk option stack=10k
    @%append tmp.lnk option nodefault
    @for %i in ($objects) do @%append tmp.lnk file %i
    @for %i in ($(imports)) do @%append tmp.lnk import %i
    @%append tmp.lnk import @$(ow_nwoss_root)/common/imp/owclib.imp
    @%append tmp.lnk libpath $(wres_dir)/lib
    @%append tmp.lnk lib wresfs
    @%append tmp.lnk lib $(%devdir)/clib/library/nw_clib.386/ms_s/clib3s.lib
    @%append tmp.lnk module clib
    $(linker) @tmp.lnk
    wstrip -q -a -r $^@ . $(xname).res

.c: .;../c

.asm: .

$(xname).res : ../h/wsample.rc ../h/wmsg.h .AUTODEPEND
    $(rc_aui) -dNLM -fo=$^. $[@

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map *.err
    @rm -f ../bin/$(xname).nlm
