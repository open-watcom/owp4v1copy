%DEVDR%:
cd %DEVDIR%
freshall sweep killobjs %1
cd %DEVDIR%\bin
sweep killobjs %1
cd %DEVDIR%\hdr
sweep if exist *.h* del *.h*
