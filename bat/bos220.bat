@echo off
call bmode
call docd os220\wfc
wmake /h /i
call docd ..\wcc
wmake /h /i
call docd ..\wpp
wmake /h /i
call docd ..\wvideo
wmake /h /i
call docd ..\wmake
wmake /h /i
call docd ..\wlink
wmake /h /i
cd ..\..


if [%1] == [] goto nocopy
ifarch copy os220\wfc\wf77386.prf %relroot%\%1\wf77386.prf
ifarch copy os220\wfc\wfcopts.dll %relroot%\%1\binp\dll\wfcopts.dll
ifarch copy os220\wfc\wfcopts.hlp %relroot%\%1\binp\help\wfcopts.hlp
rem wfcerrs.hlp now comes from "doc" project
rem ifarch copy os220\wfc\wfcerrs.hlp %relroot%\%1\binp\help\wfcerrs.hlp
ifarch copy os220\wcc\wcc386.prf  %relroot%\%1\wcc386.prf
ifarch copy os220\wcc\wccopts.dll %relroot%\%1\binp\dll\wccopts.dll
ifarch copy os220\wcc\wccopts.hlp %relroot%\%1\binp\help\wccopts.hlp
rem wccerrs.hlp now comes from "doc" project
rem ifarch copy os220\wcc\wccerrs.hlp %relroot%\%1\binp\help\wccerrs.hlp
ifarch copy os220\wpp\wpp386.prf  %relroot%\%1\wpp386.prf
ifarch copy os220\wpp\wppopts.dll %relroot%\%1\binp\dll\wppopts.dll
rem wpperrs.hlp now comes from "doc" project
rem ifarch copy os220\wpp\wpperrs.hlp %relroot%\%1\binp\help\wpperrs.hlp
ifarch copy os220\wvideo\wvidopts.dll %relroot%\%1\binp\dll\wvidopts.dll
ifarch copy os220\wvideo\wvidopts.hlp %relroot%\%1\binp\help\wvidopts.hlp
ifarch copy os220\wmake\wmakopts.dll %relroot%\%1\binp\dll\wmakopts.dll
ifarch copy os220\wmake\wmakopts.hlp %relroot%\%1\binp\help\wmakopts.hlp
ifarch copy os220\wlink\wlopts.dll %relroot%\%1\binp\dll\wlopts.dll
ifarch copy os220\wlink\wlopts.hlp %relroot%\%1\binp\help\wlopts.hlp
ifarch copy watcom\lib\os2386.lib %relroot%\%1\lib386\os2\os2386.lib
:nocopy
