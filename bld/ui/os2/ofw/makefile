#pmake: all build cpu_386 os_os2
host_CPU = 386
host_OS  = os2

!include ../os2.mif

sample2.$(EXE) : sample.obj sampdial.obj ui.lib
    %create sample.lnk
    %append sample.lnk  system os2v2
    %append sample.lnk  option map
    %append sample.lnk  name sample2.exe
    %append sample.lnk  debug all
    %append sample.lnk  file sample
    %append sample.lnk  file sampdial
    %append sample.lnk  library ../$(LIBNAME)
    set lib=$(watcomlib);$(%lib)
    wlinkp @sample.lnk
