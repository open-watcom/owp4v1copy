@echo off
call bmode
call docd gui\ui\obj
wmake /h /i
call docd ..\objw
wmake /h /i
call docd ..\objq
wmake /h /i
call docd ..\obj2
wmake /h /i
call docd ..\objf
wmake /h /i
call docd ..\objnt
wmake /h /i
call docd ..\objnt.axp
wmake /h /i
call docd ..\..\win\obj
wmake /h /i
call docd ..\objpmf
wmake /h /i
call docd ..\objpm
wmake /h /i
call docd ..\objnt
wmake /h /i
call docd ..\objaxp
wmake /h /i
cd ..\..\..
