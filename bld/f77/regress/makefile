#pmake: test

ftest_name = result

all_tests = test0 test1 test2 test3 test4 test5 test6 test7 test8 test9

!include environ.mif

test : .symbolic global start_test $(all_tests) finish_test global

start_test: .symbolic
    @if exist *.sav rm -f *.sav
    @if exist *.log rm -f *.log
    set FTEST_NAME=$(ftest_name)
    cd nist

cd_back : .symbolic
    cd ..

./chktest.exe : $(watcom_dir)/c/chktest.c
    $(bld_cl) $< $(wcl_util_opts)

finish_test : .symbolic cd_back ./chktest.exe
    @cdsay >> $(log_file)
    $]@ $(log_file)
    %append $(log_file) TEST
    %append $(log_file) PASS $(%__CWD__)

$(all_tests) : .procedure
    -@wmake -h global
    -wmake -h $@=
    -@wmake -h $@= save
    -@wmake -h global
!ifdef %EXTRA_ARCH
    -wmake -h arch=$(%EXTRA_ARCH) $@=
    -@wmake -h arch=$(%EXTRA_ARCH) $@= save
    -@wmake -h global
!endif

global : .procedure
    @if exist *.obj rm *.obj
    @if exist *.err rm *.err
    @if exist *.exe rm *.exe
