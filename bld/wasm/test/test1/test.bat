if '%1' == '' goto std
wmake -h asm=%1
goto end
:std
wmake -h
:end
