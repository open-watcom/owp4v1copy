rem call ..\setup11.cmd
call .\setup.cmd
copy %doc_root%\doc\gs\*.bmp
call ..\mkit c_readme
call ..\mkit f_readme
erase *.bmp
copy %doc_root%\doc\cl\*.bmp
call ..\mkit clr
erase *.bmp
copy %doc_root%\doc\gui\snaps\*.bmp
call ..\mkit ide
erase *.bmp
copy %doc_root%\doc\gui\snaps\brow*.bmp
call ..\mkit wbrw
erase brow*.bmp
copy %doc_root%\doc\gui\snaps\wprof*.bmp
call ..\mkit wprof
erase wprof*.bmp
copy %doc_root%\doc\cg\*.bmp
call ..\mkit cguide
erase *.bmp
copy %doc_root%\doc\fg\*.bmp
call ..\mkit fuguide
erase *.bmp
copy %doc_root%\doc\pg\*.bmp
call ..\mkit pguide
call ..\mkit fpguide
erase *.bmp
call ..\mkit lguide
call ..\mkit tools
copy %doc_root%\doc\wd\*.bmp
call ..\mkit wd
erase *.bmp
call ..\mkit rescomp
copy %doc_root%\doc\lr\ge\*.bmp
call ..\mkit clib
call ..\mkit f77graph
erase *.bmp
call ..\mkit cpplib
call ..\mkit wccerrs
call ..\mkit wpperrs
call ..\mkit ftools
call ..\mkit f77lr
call ..\mkit wfcerrs
rem copy %doc_root%\doc\isv\*.bmp
call ..\mkit isv
erase *.bmp
erase cmix.*
call ..\mkit cmix
call ..\mkit fmix
