@set IDN=%1
@if not [%IDN%] == [1] goto not_first
@   set IOTRG=%ITRG%
@   set ILOG=%2
@   if exist %ILOG% del %ILOG%
@   if exist missing.log del missing.log
@   set IVL=%3
:set_lbl
@       if [%4] == [] goto next
@       set IVL=%IVL% %4
@       shift
@       goto set_lbl
:not_first
@   if [%IOTRG%]==[\tmp] goto sizing
@       vl %ITRG% "%IVL%"
@       fd %ITRG%\*.* /d%IDATE% /t%ITIME%
@       dir %ITRG%\*.* >>%ILOG%
@       fs  %ITRG%\*.* /t >>%ILOG%
@       goto next
:sizing
@   filesize %ITRG%\*.* >>%ILOG%
:next
@if [%IDN%] == [done] goto done
@   echo Insert disk #%IDN%
@   if not [%IOTRG%]==[\tmp] goto not_sizing
@       set ITRG=%IOTRG%\%IDN%
@       mkdir %IOTRG%\%IDN%
@       goto rest
:not_sizing
@       pause
:rest
@   del %ITRG%\*.* <%TOOLDR%\tools\yes
@   echo ================== DISK %IDN% =================== >>%ILOG%
:done
