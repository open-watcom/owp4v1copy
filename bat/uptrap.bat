set ___x___=%1
if [%1]==[] set ___x___=r:\dlang
copy %devdir%\bin\???.trp %___x___%\binw
copy %devdir%\bin\???serv.exe %___x___%\binw
copy %devdir%\binp\dll\???.dll %___x___%\binp\dll
copy %devdir%\binp\dll\std??.dll %___x___%\binp\dll
copy %devdir%\binp\dll\*.exe %___x___%\binp\dll
copy %devdir%\binp\wvpmhelp.exe %___x___%\binp
copy %devdir%\binp\???serv.exe %___x___%\binp
copy %devdir%\binp\nmpbind.exe %___x___%\binp
copy %devdir%\win\???.dll %___x___%\binw
copy %devdir%\win\wint32.dll %___x___%\binw
copy %devdir%\win\???servw.exe %___x___%\binw
copy %devdir%\nt\std.dll %___x___%\binnt
copy %devdir%\nlm\*.nlm %___x___%\nlm
copy %devdir%\qnx\*.trp %___x___%\qnx\vid
copy %devdir%\qnx\???serv.qnx %___x___%\qnx
set ___x___=
