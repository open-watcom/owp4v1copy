@echo off
call bmode
call docd c\sqlpp\obj
wmake /h /i
call docd ..\o386
wmake /h /i
call docd ..\..\csqlpp
wmake /h /i
call docd ..\csql386
wmake /h /i
call docd ..\csql386p
wmake /h /i
call docd ..\csql
wmake /h /i
call docd ..\csqlp
wmake /h /i
call docd ..\csqlos2
wmake /h /i
cd ..\..
if [%1] == [] goto nocopy
ifarch copy c\csqlpp\wcsqlpp.exe %relroot%\%1\binnov\wcsqlpp.exe
ifarch copy c\csql386\csql386.exe %relroot%\%1\binnov\wcc386.exe
ifarch copy c\csql386p\csql386p.exe %relroot%\%1\binnov\wcc386p.exe
ifarch copy c\csql\csql.exe %relroot%\%1\binnov\wcc.exe
ifarch copy c\csqlp\csqlp.exe %relroot%\%1\binnov\wccp.exe
ifarch copy c\csqlos2\csqlos2.exe %relroot%\%1\binnov\os2\wcc.exe
:nocopy
