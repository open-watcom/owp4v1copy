@rem
@rem  Usage:  @call ifile  directory   filename  conditions ...
@rem
@if not exist %ISRC%\%1\%2 echo %ISRC%\%1\%2>>missing.log
@echo copy %ISRC%\%1\%2 %%ITRG%%  >>mkdisks.bat
@if [%NEWDISK%]==[] goto skip
@echo enter %%1%2 %NEWDISK%>>%INSTALL%
@set NEWDISK=
:skip
@if [%3]==[]  echo unpack %%2%1\ %%1%2>>%INSTALL%
@if [%3]==[]  goto done
@if [%4]==[]  echo if %%%3 unpack %%2%1\ %%1%2>>%INSTALL%
@if not [%4]==[] echo if %%%3 if %%%4 unpack %%2%1\ %%1%2>>%INSTALL%
:done
