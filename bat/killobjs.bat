@echo off
if exist lastdef. erase lastdef.
if exist *.pch erase *.pch
if exist *.map erase *.map
if exist *.tmp erase *.tmp
if exist tmp.* erase tmp.*
if exist *.lib erase *.lib
if exist *.lbc erase *.lbc
if exist *.386 erase *.386
if exist *.?bj erase *.?bj
if exist *.?es erase *.?es
if exist *.com erase *.com
if exist *.exe erase *.exe
if exist *.ext erase *.ext
if exist *.exp erase *.exp
if exist *.trp erase *.trp
if exist *.dll erase *.dll
if exist *.nlm erase *.nlm
if exist *.qnx erase *.qnx
if exist *.gh  erase *.gh
if exist *.gc  erase *.gc
if exist *.gpp erase *.gpp
if exist *.sym erase *.sym
if [%1] == [all] if exist *.osz erase *.osz
