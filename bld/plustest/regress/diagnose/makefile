plustest_name = diagnose
!include ../environ.mif

diagnose_dir = source

!include $(diagnose_dir)/diagnose.mif

diagnose_opts += -ew -we -wx -e1000 -zq -eq
archdep_opts = -dARCH=$(arch) -fr=.ser
alt_error=00

# The check files for x86 are a bit different.
!ifeq arch i86
diagchk0 = di860000.chk
diagchk1 = di860100.chk
diagchk2 = di860200.chk
diagchk3 = di860300.chk
!else
diagchk0 = diag0000.chk
diagchk1 = diag0100.chk
diagchk2 = diag0200.chk
diagchk3 = diag0300.chk
dosystest = testsys
!endif

.c: $(diagnose_dir)
.c.err:
    -$(wpp_$(arch)) $[@ @diag -na $(opt_$^&)
    @if not exist $^&.err echo I did not generate errors for $^& > $^&.err
.c.ser:
    -$(wpp_$(arch)) $[@ @diag -na $(opt_$^&) $(archdep_opts)
    @if not exist $^&.ser echo I did not generate errors for $^& > $^&.ser

test : .symbolic initopts test0000 test0100 test0200 test0300 $(dosystest) test_special
    @%make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)

initopts : .symbolic
    set diag=$(diagnose_opts)

test0000 : .symbolic diag0000.out
    -diff -b diag0000.out $(diagchk0) >diff0000.out
    diff -b diff0000.out diff0000.chk
    @rm diff0000.out

test0100 : .symbolic diag0100.out
    -diff -b diag0100.out $(diagchk1) >diff0100.out
    diff -b diff0100.out diff0100.chk
    @rm diff0100.out

test0200 : .symbolic diag0200.out
    -diff -b diag0200.out $(diagchk2) >diff0200.out
    diff -b diff0200.out diff0200.chk
    @rm diff0200.out

test0300 : .symbolic diag0300.out
    -diff -b diag0300.out $(diagchk3) >diff0300.out
    diff -b diff0300.out diff0300.chk
    @rm diff0300.out

testsys : .symbolic diag$(arch).out
    -diff -b diag$(arch).out diag$(arch).chk >diff$(arch).out
    diff -b diff$(arch).out diff$(arch).chk
    @rm diff$(arch).out

test_special : .symbolic
    -$(wpp_$(arch)) $(diagnose_dir)\diag0001.c @diag $(opt_diag0001) -eq >diagspec.out
    -diff -b diagspec.out diagspec.chk >diffspec.out
    @%make diag_clean
    diff -b diffspec.out diffspec.chk
    @rm diffspec.out

diag0000.out : ./catfile.exe $(diags_0099)
    $[@ -a *.err >diag0000.out
    @%make diag_clean

diag0100.out : ./catfile.exe $(diags_0199)
    $[@ -a *.err >diag0100.out
    @%make diag_clean

diag0200.out : ./catfile.exe $(diags_0299)
    $[@ -a *.err >diag0200.out
    @%make diag_clean

diag0300.out : ./catfile.exe $(diags_0399)
    $[@ -a *.err >diag0300.out
    @%make diag_clean

diag386.out : ./catfile.exe $(diags_sys)
    $[@ -a *.ser >diag386.out
    @%make diag_clean

diagaxp.out : ./catfile.exe $(diags_sys)
    $[@ -a *.ser >diagaxp.out
    @%make diag_clean

./catfile.exe: source/catfile.c
    $(bld_cl) $[@

diag_clean : .procedure
    @if exist *.err rm -f *.err
    @if exist *.ser rm -f *.ser
    @if exist *.pch rm -f *.pch

save : .symbolic
    @if exist diff0000.out cp diag0000.out s$(arch)0000.sav
    @if exist diff0100.out cp diag0100.out s$(arch)0100.sav
    @if exist diff0200.out cp diag0200.out s$(arch)0200.sav
    @if exist diff0300.out cp diag0300.out s$(arch)0300.sav
    @if exist diff$(arch).out cp diag$(arch).out s$(arch)_$(arch).sav
    @if exist diffspec.out cp diagspec.out s$(arch)spec.sav

global : .symbolic
    @%make diag_clean
    @%make common_clean
    @if exist catfile.* rm -f catfile.*

