@echo off
call bmode
call docd wdisasm\osi
wmake /h /i wdisasm.exe
call docd ..\qnx
wmake /h /i wdisasm.qnx
call docd %devdir%\ndisasm\stand\osi.x86
wmake /h /i
call docd ..\nt.axp
wmake /h /i
cd %devdir%
if [%1] == [] goto nocopy
ifarch copy wdisasm\osi\wdisasm.exe %relroot%\%1\binw
ifarch copy wdisasm\qnx\wdisasm.qnx %relroot%\%1\qnx\binq\wdisasm.
ifarch copy ndisasm\stand\osi.x86\wdis.exe %relroot%\%1\binw
ifarch copy ndisasm\stand\nt.axp\wdis.exe %relroot%\%1\axpnt
:nocopy
