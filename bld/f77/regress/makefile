#pmake: test

ftest_name = result

!include environ.mif

test : .symbolic clean_all start_test finish_test

start_test: .symbolic
    set FTEST_NAME=$(ftest_name)
    cd nist
    -wmake -h
    -wmake -h test1=
    -wmake -h test2=
    -wmake -h test3=
    -wmake -h test4=
    -wmake -h test5=
    -wmake -h test6=
    -wmake -h test7=
    -wmake -h test8=
    -wmake -h test9=
!ifdef %EXTRA_ARCH
    cd ../nist
    -wmake -h arch=$(%EXTRA_ARCH)
    -wmake -h arch=$(%EXTRA_ARCH) test1=
    -wmake -h arch=$(%EXTRA_ARCH) test2=
    -wmake -h arch=$(%EXTRA_ARCH) test3=
    -wmake -h arch=$(%EXTRA_ARCH) test4=
    -wmake -h arch=$(%EXTRA_ARCH) test5=
    -wmake -h arch=$(%EXTRA_ARCH) test6=
    -wmake -h arch=$(%EXTRA_ARCH) test7=
    -wmake -h arch=$(%EXTRA_ARCH) test8=
    -wmake -h arch=$(%EXTRA_ARCH) test9=
!endif
    cd ..

clean_all : .symbolic
    %make global
    rm -f *.sav
    rm -f $(log_file)

finish_test : .symbolic ./chktest.exe
    @cdsay >> $(log_file)
    $< $(log_file)
    %append $(log_file) PASS $(%__CWD__)

./chktest.exe : c/chktest.c
    $(bld_cl) $< $(wcl_util_opts)

global : .procedure
    %make common_clean
