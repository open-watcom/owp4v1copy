@echo off
call bmode

call docd diff\nt
wmake /i /h
copy diff.exe c:\wproj\nt\diffnt.exe

call docd ..\o
wmake /i /h
copy diff.exe c:\wproj\dos\diffr.exe

call docd ..\os2
wmake /i /h
copy diff.exe c:\wproj\dos\diff2p.exe
call docd ..\..

call docd wrcs\nt
wmake /i /h
copy *.exe c:\wproj\nt

call docd ..\o
wmake /i /h
copy *.exe c:\wproj\dos
copy *.exe c:\wproj\os2

call docd ..\..

call docd wproj\nt
wmake /i /h
copy *.exe c:\wproj\nt

call docd ..\o
wmake /i /h
call docd ..\o2
wmake /i /h
copy *.exe c:\wproj\dos

call docd ..\os2_20
wmake /i /h
copy *.exe c:\wproj\os2

call docd ..\c
copy psweep.prf c:\wproj\dos
copy psweep.prf c:\wproj\nt
copy psweep.prf c:\wproj\os2

call docd ..\cp
copy nttouch.exe c:\wproj\nt\rcstouch.exe
copy wtouch.exe c:\wproj\dos\rcstouch.exe
copy wtouch.exe c:\wproj\os2\rcstouch.exe

call docd ..
copy read.me c:\wproj
copy tstwprj.bat c:\wproj


call docd ..
