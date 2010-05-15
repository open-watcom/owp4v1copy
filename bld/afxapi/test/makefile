.EXTENSIONS : .res .rc

afxapitest_name = test
!include environ.mif

opt_EH_yes = -xs
alt_error = 00

afxapitest_dir = source

!include $(afxapitest_dir)/test.mif

test_cflags_386 += -we -w0 -zq -fh -D_CONSOLE -I../nt

.error
    @%append $(%error_file) $(%error_msg)

.cpp: $(afxapitest_dir)
.cpp.obj:
    @set error_msg=failure to compile $^@
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch)) $(opt_$^*) $(opt_EH_$(EH_$^*)) -fo=.obj

.rc: $(afxapitest_dir)
.rc.res:
    @set error_msg=failure to compile $^@
    wrc -q -r -fo=$@ $[@

.obj.$(exe):
    @set error_msg=failure to link $^@
    $(linker) @test.lnk $(opt_link_$^*) NAME $@ FILE $[@
    @set error_msg=failure to execute $^@
    @%append exec.out PASS executing $^@
    $(run) $(exec_prefix)$@ >>exec.out
    rm $[@

./chk_exec.exe : chk_exec.c
    $(bld_cl) $< $(wcl_util_opts)

test : .symbolic start_test $(test_exes) ./chk_exec.exe
    @set error_file=error.out
    @set error_msg=failure to complete test stream
    %write error.out OK
    $]@ exec.out >>error.out
    diff -b error.out error.chk
    @%make global
    %append $(log_file) PASS $(%__CWD__)

string04.$(exe) : string04.obj string04.res
    @set error_msg=failure to link $^@
    $(linker) @test.lnk NAME $@ FILE $[@
    @set error_msg=failure to add resources to $^@
    wrc -q $]@
    @set error_msg=failure to execute $^@
    @%append exec.out PASS executing $^@
    $(run) $(exec_prefix)$^@ >>exec.out

start_test : .symbolic test.lnk
    %create exec.out
    @set error_file=exec.out

test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION quiet, noredef
    @%append $^@ LIBPATH ../nt386

global : .symbolic
    @%make common_clean
