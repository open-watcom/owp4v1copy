plustest_name = torture
!include ..\environ.mif

objs = &
    assert.obj &
    jamstr.obj &
    newlog.obj &
    ordkeyma.obj &
    pool.obj
    
!ifdef test1
test_cflags = -d2-j
alt_error=01
!else
test_cflags = -ox-d1-j
alt_error=00
!endif

test_cflags += -zq-w0-i.

test_cflags_386 = -mf
test_cflags_i86 = -ml -fi=noinline.h
test_cflags_axp =

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.cpp.obj:
    $(wpp_$(arch)) $[* $(test_cflags) $(test_cflags_$(arch))
    
test : .symbolic mdarr.$(exe) diarr.$(exe)
    $(run) mdarr >exec.out
    diff output output.chk
    del output
    $(run) diarr >>exec.out
    diff exec.out exec.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
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
    @%append $^@ $(stack_$(arch))
    @%append $^@ $(ldebug_$(arch))
    
global : .symbolic
    @%make common_clean
