@echo off
call bmode
call docd vi
call docd bind
wmake /h /i
call docd ..\ctags
wmake /h /i
call docd ..\obj286v
wmake /h /i bind=1
rem call docd vi\obj286
rem wmake /h /i bind=1
call docd ..\obj386
wmake /h /i bind=1
rem call docd ..\objos2
rem wmake /h /i  bind=1
call docd ..\objqnx
wmake /h /i bind=1
call docd ..\objnt
wmake /h /i bind=1
call docd ..\obj2
wmake /h /i bind=1
call docd ..\objwin
wmake /h /i
rem call docd ..\objwinp
rem wmake /h /i
call docd ..\objwinnt
wmake /h /i
call docd ..\objaxp
wmake /h /i
call docd ..\objwinap
wmake /h /i
cd ..\..

if [%1] == [] goto nocopy
rem ifarch copy %devdir%\vi\obj286\vi.exe %relroot%\%1\binw\vi286.exe
ifarch copy %devdir%\vi\objwin\viw.exe %relroot%\%1\binw\viw.exe
rem ifarch copy %devdir%\vi\objwinp\viwp.exe %relroot%\%1\binw\viwp.exe
ifarch copy %devdir%\vi\objwinnt\vintw.exe %relroot%\%1\binnt\viw.exe
rem ifarch copy %devdir%\vi\objos2\vi2r.exe %relroot%\%1\binp\vi16.exe
ifarch copy %devdir%\vi\obj2\vi2.exe %relroot%\%1\binp\vi.exe
ifarch copy %devdir%\vi\obj386\vi386.exe %relroot%\%1\binw\vi.exe
ifarch copy %devdir%\vi\obj286v\viv.exe %relroot%\%1\binw\vir.exe
ifarch copy %devdir%\vi\objqnx\viq.qnx %relroot%\%1\qnx\binq\vi.
ifarch copy %devdir%\vi\objnt\vint.exe %relroot%\%1\binnt\vi.exe
ifarch copy %devdir%\vi\objaxp\viaxp.exe %relroot%\%1\axpnt\vi.exe
ifarch copy %devdir%\vi\objwinap\viaxpw.exe %relroot%\%1\axpnt\viw.exe
ifarch copy %devdir%\vi\ctags\ctags.exe %relroot%\%1\binw\ctags.exe
rem ifarch copy %devdir%\vi\ctags\ctagsp.exe %relroot%\%1\binw\ctagsp.exe
rem ifarch copy %devdir%\vi\ctags\ctagsnt.exe %relroot%\%1\binnt\ctags.exe
ifarch copy %devdir%\vi\bind\edbind.exe %relroot%\%1\binw\edbind.exe
rem ifarch copy %devdir%\vi\bind\edbindnt.exe %relroot%\%1\binnt\edbind.exe

cd vi\dat
rem rcs stuff
for %%f in (v*.cmd) do ifarch copy %devdir%\vi\dat\%%f %relroot%\%1\binp
for %%f in (v*.bat) do ifarch copy %devdir%\vi\dat\%%f %relroot%\%1\binw
for %%f in (*.vi *.cfg *.dat *.ini) do ifarch copy %devdir%\vi\dat\%%f %relroot%\%1\eddat\%%f
for %%f in (bindvi.* ) do ifarch copy %devdir%\vi\dat\%%f %relroot%\%1\eddat\%%f
rem ifarch copy %devdir%\vi\dat\weditor.ini %relroot%\%1\binw
rem copy %relroot%\%1\weditor.ini %relroot%\%1\binnt >nul

cd ..\doc
for %%f in (*.hlp) do ifarch copy %devdir%\vi\doc\%%f %relroot%\%1\eddat\%%f

cd ..\..

:nocopy
