@echo off
call bmode
call docd dwarf\dw\o
wmake /h
call docd ..\ol
wmake /h
call docd ..\o386s
wmake /h
call docd ..\axp
wmake /h
call docd ..\..\dr\os2
wmake /h
call docd ..\win
wmake /h
call docd ..\386
wmake /h
call docd ..\axp
wmake /h
cd ..\..\..
