set __dir__=%1
:loop
    if [%2] == [] goto endloop
    if not exist %ramd%:\%2 copy %__dir__%\%2 %ramd%:\%2
    shift
    goto loop
:endloop
set __dir__=
