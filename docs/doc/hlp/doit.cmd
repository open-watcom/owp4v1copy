cd ib
call mkall clean
xcopy *.ihp o:\cproj\cdoc\hlp\ib
xcopy *.h o:\cproj\cdoc\hlp\ib
erase *.ihp
erase *.h
erase *.tbl
erase *.idx
erase *.kw
cd ..\os2
xcopy \doc\cg\*.bmp
xcopy \doc\fg\*.bmp
xcopy \doc\pg\*.bmp
xcopy \doc\wd\*.bmp
xcopy \doc\gui\snaps\*.bmp
xcopy \doc\lr\ge\*.bmp
xcopy \doc\isv\*.bmp
call mkall clean
xcopy *.inf o:\cproj\cdoc\hlp\os2
xcopy *.hlp o:\cproj\cdoc\hlp\os2
xcopy *.h o:\cproj\cdoc\hlp\os2
erase *.ipf
erase *.inf
erase *.hlp
erase *.h
erase *.tbl
erase *.idx
erase *.kw
erase *.bmp
cd ..\win
call mkall clean
xcopy *.hlp o:\cproj\cdoc\hlp\win
xcopy *.h o:\cproj\cdoc\hlp\win
erase *.rtf
erase *.hlp
erase *.h
erase *.tbl
erase *.idx
erase *.kw
rem cd ..\html
rem call mkall clean
