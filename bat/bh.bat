@echo off
call bmode
call docd hdr\dos
wmake -h
call docd ..\qnx
wmake -h
call docd ..
if [%1] == [] goto nocopy
for %%i in (%devdir%\hdr\dos\*.h*) do ifarch copy %%i %relroot%\%1\h
for %%i in (%devdir%\hdr\dos\*.cnv) do ifarch copy %%i %relroot%\%1\h
for %%i in (%devdir%\hdr\dos\sys\*.h) do ifarch copy %%i %relroot%\%1\h\sys
for %%i in (%devdir%\hdr\dos\win\*.h) do ifarch copy %%i %relroot%\%1\h\win
for %%i in (%devdir%\hdr\qnx\*.h*) do ifarch copy %%i %relroot%\%1\qh
for %%i in (%devdir%\hdr\qnx\sys\*.h) do ifarch copy %%i %relroot%\%1\qh\sys
rem for %%i in (%devdir%\hdr\penpoint\*.h*) do ifarch copy %%i %relroot%\%1\ph
rem for %%i in (%devdir%\hdr\penpoint\sys\*.h) do ifarch copy %%i %relroot%\%1\ph\sys
rem for %%i in (%devdir%\watcom\nt_h\*.h*) do ifarch copy %%i %relroot%\%1\h\nt
rem for %%i in (%devdir%\watcom\nt_h\*.dlg) do ifarch copy %%i %relroot%\%1\h\nt
for %%i in (%devdir%\nwsdk\novi40\*.*) do ifarch copy %%i %relroot%\%1\novi
for %%i in (%devdir%\nwsdk\novh40\*.*) do ifarch copy %%i %relroot%\%1\novh
for %%i in (%devdir%\nwsdk\novh40\arpa\*.h) do ifarch copy %%i %relroot%\%1\novh\arpa
for %%i in (%devdir%\nwsdk\novh40\netinet\*.h) do ifarch copy %%i %relroot%\%1\novh\netinet
for %%i in (%devdir%\nwsdk\novh40\sys\*.h) do ifarch copy %%i %relroot%\%1\novh\sys
:nocopy
cd ..
