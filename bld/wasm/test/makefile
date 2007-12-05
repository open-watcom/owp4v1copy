#pmake: test

wasmtest_name = result

!include environ.mif

test : .symbolic clean_all start_test finish_test

start_test: .symbolic
    set WASMTEST_NAME=$(wasmtest_name)
    cd test1
    -wmake -h
    cd ../test2
    -wmake -h
    cd ../test3
    -wmake -h
    cd ../diag
    -wmake -h
!ifdef %EXTRA_ARCH
    cd ../test1
    -wmake -h arch=$(%EXTRA_ARCH)
    cd ../test2
    -wmake -h arch=$(%EXTRA_ARCH)
    cd ../test3
    -wmake -h arch=$(%EXTRA_ARCH)
    cd ../diag
    -wmake -h arch=$(%EXTRA_ARCH)
!endif
    cd ..

clean_all : .symbolic
    rm -f *.sav
    %make global

finish_test : .symbolic ./chktest.exe
    @cdsay >> $(log_file)
    $< $(log_file)
    %append $(log_file) PASS $(%__CWD__)

./chktest.exe : c/chktest.c
   $(bld_cl) $< -ox-d1

global : .symbolic
    %make common_clean
