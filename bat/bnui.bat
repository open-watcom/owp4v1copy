@echo off
call bmode
call docd nui\dos\osw
wmake /h /i
call docd ..\omw
wmake /h /i
call docd ..\olw
wmake /h /i
call docd ..\ofw
wmake /h /i
call docd ..\..\fmrdos\omw
wmake /h /i
call docd ..\olw
wmake /h /i
call docd ..\..\necdos\omw
wmake /h /i
call docd ..\olw
wmake /h /i
call docd ..\..\os2\olw
wmake /h /i
call docd ..\ofw
wmake /h /i
call docd ..\..\win\olw
wmake /h /i
call docd ..\..\necwin\olw
wmake /h /i
call docd ..\..\nt\ofw
wmake /h /i
call docd ..\..\..\uiforms\util
wmake /h /i
cd ..\..
