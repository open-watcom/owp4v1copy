ctest_name = diagnose
!include ../environ.mif

diagnose_dir = source

!include $(diagnose_dir)/diagnose.mif

diagnose_opts += -we -wx -e1000 -eq -zq
archdep_opts = -dARCH=$(arch) -fr=.ser
alt_error=00

.c: $(diagnose_dir)
.c.err:
    -$(wcc_$(arch)) $< $(diagnose_opts) $(opt_$^&) -fo=.obj
    @if not exist $@ %write $@ I did not generate errors for $^&
.c.ser:
    -$(wcc_$(arch)) $< $(diagnose_opts) $(opt_$^&) $(archdep_opts) -fo=.obj
    @if not exist $@ %write $@ I did not generate errors for $^&

test : .symbolic start_test diag0000.out
    -diff -b diag0000.out diag0000.chk >diff0000.out
    diff -b diff0000.out diff0000.chk
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

./catfile.exe: source/catfile.c
    $(bld_cl) $< $(wcl_util_opts)

diag0000.out : $(diags_0099) ./catfile.exe
    $]@ -a *.err | sed -e "s/source\//source\\\\/g" >diag0000.out

start_test : .symbolic
    @if exist s$(arch)0000.sav rm s$(arch)0000.sav

save : .symbolic
    @if exist diff0000.out cp diff0000.out s$(arch)0000.sav

global : .symbolic
    @if exist *.ser del *.ser
    @if exist catfile.* rm catfile.*
    @%make common_clean
