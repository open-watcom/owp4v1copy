@echo off
md s:\jack\doc
md s:\jack\doc\whelp
md s:\jack\doc\hlp
md s:\jack\doc\hlp\os2
md s:\jack\hlp
md s:\jack\hlp\os2
xcopy \w110\toolkt2x\*.* s:\jack\toolkt2x\ /s/e
xcopy \doc\whelp\*.* s:\jack\doc\whelp
xcopy \doc\hlp\*.* s:\jack\doc\hlp
xcopy \doc\hlp\os2\*.* s:\jack\doc\hlp\os2
@ECHO NOTE: The IPF Compiler may need to be transferred to the Watcom BINP directory
copy d:\w110\binp\ipfc.exe s:\jack\doc\hlp\os2
xcopy \doc\gs\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\cl\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\gui\snaps\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\gui\snaps\brow*.bmp s:\jack\doc\hlp\os2
xcopy \doc\gui\snaps\wprof*.bmp s:\jack\doc\hlp\os2
xcopy \doc\cg\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\fg\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\pg\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\wd\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\lr\ge\*.bmp s:\jack\doc\hlp\os2
xcopy \doc\isv\*.bmp s:\jack\doc\hlp\os2
