plustest_name = torture
!include ..\environ.mif

objs = &
    assert.obj &
    jamstr.obj &
    newlog.obj &
    ordkeyma.obj &
    pool.obj
    
!ifdef test1
cflags = -d2-j
!else
cflags = -ox-d1-j
!endif

cflags += -zq-w0-i.

cflags_386 += $(cflags) -mf
cflags_i86 += $(cflags) -ml -fi=noinline.h
cflags_axp += $(cflags)

.cpp.obj:
    $(wpp_$(arch)) $[* $(cflags_$(arch))
    
test : .symbolic mdarr.$(exe) diarr.$(exe)
    $(run) mdarr >exec.out
    diff output output.chk
    del output
    $(run) diarr >>exec.out
    diff exec.out exec.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
diarr.$(exe) : dim2arr.obj tort.lnk tort.lib
    $(linker) @tort NAME $^@ FILE $[@ LIB $]@
    
mdarr.$(exe) : mdarrtst.obj tort.lnk tort.lib
    $(linker) @tort NAME $^@ FILE $[@ LIB $]@
    
tort.lib : tort.lbc $(objs)
    $(librarian_$(arch)) /b /q /n @$[@
    
tort.lbc : $(__MAKEFILES__)
    %create $^@
    @%append $^@ $^*.lib
    @for %i in ( $(objs) ) do @%append $^@ -+%i
    
tort.lnk : $(__MAKEFILES__)
    %create $^@
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION stack=16k, quiet, map
    @%append $^@ $(ldebug_$(arch))
    
global : .symbolic
    @%make common_clean
