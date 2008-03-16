ctest_name = regress
!include environ.mif

./chktest.exe : $(watcom_dir)/c/chktest.c
    $(bld_cl) $< $(wcl_util_opts)

test : .symbolic ./chktest.exe
    $]@ $(log_file)
    %make global
    %append $(log_file) PASS $(%__CWD__)

del_log : .symbolic
    rm -f $(log_file)

global : .symbolic
    %make common_clean
