set __batfile__=tmp.bat
if exist %__batfile__% del %__batfile__%
call synchdrv c %__batfile__% out
call synchdrv d %__batfile__% out
set __batfile__=
