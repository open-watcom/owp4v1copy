set ___x___=%1
if [%1]==[] set ___x___=r:\dlang
copy %devdir%\wprof\pm\wprof.exe %___x___%\binp\wprof.exe
copy %devdir%\wprof\win\wprofw.exe %___x___%\binw\wprofw.exe
copy %devdir%\wprof\dos\wprof.exe %___x___%\binw\wprof.exe
copy %devdir%\wprof\nt\wprof.exe %___x___%\binnt\wprof.exe
copy %devdir%\wprof\qnx\wprof.qnx %___x___%\qnx\wprof.qnx
copy %devdir%\wprof\qnx\wprof.sym %___x___%\qnx\sym\wprof.sym
set ___x___=
