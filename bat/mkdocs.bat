@echo off
rem do some error checking here before we start creating gobs of directories
if .%doc_root%. == .. goto error
md %doc_root%\hlp
md %doc_root%\hlp\ib
md %doc_root%\hlp\os2
md %doc_root%\hlp\win
md %doc_root%\hlp\win95
goto end
:error
@echo The doc_root environment variable needs to be set!
:end
