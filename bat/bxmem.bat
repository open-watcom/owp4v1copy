@echo off
call bmode
call docd xmem\scbd
wmake /h /i
call docd ..\bcbd
wmake /h /i
cd ..\..
