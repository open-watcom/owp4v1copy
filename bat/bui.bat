@echo off
call bmode
call docd ui\dos\ofw
wmake /h /i
call docd ..\..\os2\olw
wmake /h /i
call docd ..\..\qnx\olwq
wmake /h /i
call docd ..\..\win\olw
wmake /h /i
call docd ..\..\nt\ofw
wmake /h /i
call docd ..\axp
wmake /h /i
call docd ..\..\necdos\ofw
wmake /h /i
call docd ..\..\necwin\olw
wmake /h /i
rem call docd ..\..\..\uiforms\util
rem wmake /h /i
cd ..\..
