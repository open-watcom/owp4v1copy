ctest_name = diagnose
!include ../environ.mif

diagnose_dir = source

!include $(diagnose_dir)/diagnose.mif

diagnose_opts += -we -wx -e1000 -zq
archdep_opts = -dARCH=$(arch) -fr=.ser

.c: $(diagnose_dir)
.c.err:
    -$(wcc_$(arch)) $[@ $(diagnose_opts) $(opt_$^&)
    @if not exist $^&.err echo I did not generate errors for $^& > $^&.err
.c.ser:
    -$(wcc_$(arch)) $[@ $(diagnose_opts) $(opt_$^&) $(archdep_opts)
    @if not exist $^&.ser echo I did not generate errors for $^& > $^&.ser

test : .symbolic test0000
    %make global
    %append $(log_file) PASS $(%__CWD__)

test0000 : .symbolic diag0000.out
    -diff -b diag0000.out diag0000.chk >diff0000.out
    diff -b diff0000.out diff0000.chk

diag0000.out : ./catfile.exe $(diags_0099)
    $[@ -a *.err >diag0000.out
    %make diag_clean

./catfile.exe: source/catfile.c
        $(bld_cl) $[@

diag_clean : .procedure
    if exist *.err del *.err
    if exist *.ser del *.ser
    if exist *.pch del *.pch

save : .symbolic
    if exist diff0000.out cp diff0000.out s$(arch)_0000.sav

global : .symbolic
    %make diag_clean
    %make common_clean
    if exist catfile.* del catfile.*

