@echo off
call bmode
call docd som
if [%1] == [] goto nocopy
set altdevdir=i:
for %%i in (%altdevdir%\som\*.*)                                do ifarch copy %%i %relroot%\%1\som
del %relroot%\%1\som\reados2.203
for %%i in (%altdevdir%\som\bin\*.*)                            do ifarch copy %%i %relroot%\%1\som\bin
for %%i in (%altdevdir%\som\etc\*.*)                            do ifarch copy %%i %relroot%\%1\som\etc
del %relroot%\%1\som\etc\somnew.ir
for %%i in (%altdevdir%\som\etc\dsom\*.*)                       do ifarch copy %%i %relroot%\%1\som\etc\dsom
for %%i in (%altdevdir%\som\include\*.*)                        do ifarch copy %%i %relroot%\%1\som\include
for %%i in (%altdevdir%\som\lib\*.*)                            do ifarch copy %%i %relroot%\%1\som\lib
for %%i in (%altdevdir%\som\msg\*.*)                            do ifarch copy %%i %relroot%\%1\som\msg
for %%i in (%altdevdir%\som\samples\*.*)                        do ifarch copy %%i %relroot%\%1\som\samples
del %relroot%\%1\som\samples\readme
for %%i in (%altdevdir%\som\samples\somd\*.*)                   do ifarch copy %%i %relroot%\%1\som\samples\somd
del %relroot%\%1\som\samples\somd\readme
for %%i in (%altdevdir%\som\samples\somd\c\animal\*.*)          do ifarch copy %%i %relroot%\%1\som\samples\somd\c\animal
del %relroot%\%1\som\samples\somd\c\animal\readme
del %relroot%\%1\som\samples\somd\c\animal\initterm.c
for %%i in (%altdevdir%\som\samples\somd\c\dii\*.*)             do ifarch copy %%i %relroot%\%1\som\samples\somd\c\dii
del %relroot%\%1\som\samples\somd\c\dii\readme
for %%i in (%altdevdir%\som\samples\somd\c\event\*.*)           do ifarch copy %%i %relroot%\%1\som\samples\somd\c\event
del %relroot%\%1\som\samples\somd\c\event\readme
for %%i in (%altdevdir%\som\samples\somd\c\proxy\*.*)           do ifarch copy %%i %relroot%\%1\som\samples\somd\c\proxy
del %relroot%\%1\som\samples\somd\c\proxy\readme
for %%i in (%altdevdir%\som\samples\somd\c\sompserv\*.*)        do ifarch copy %%i %relroot%\%1\som\samples\somd\c\sompserv
del %relroot%\%1\som\samples\somd\c\sompserv\readme
del %relroot%\%1\som\samples\somd\c\sompserv\initterm.c
for %%i in (%altdevdir%\som\samples\somd\c\stack\*.*)           do ifarch copy %%i %relroot%\%1\som\samples\somd\c\stack
del %relroot%\%1\som\samples\somd\c\stack\readme
del %relroot%\%1\som\samples\somd\c\stack\initterm.c
for %%i in (%altdevdir%\som\samples\somd\cpp\animal\*.*)        do ifarch copy %%i %relroot%\%1\som\samples\somd\cpp\animal
del %relroot%\%1\som\samples\somd\cpp\animal\daniinit.c
del %relroot%\%1\som\samples\somd\cpp\animal\danimal.c
del %relroot%\%1\som\samples\somd\cpp\animal\dbdog.c
del %relroot%\%1\som\samples\somd\cpp\animal\ddog.c
del %relroot%\%1\som\samples\somd\cpp\animal\dldog.c
del %relroot%\%1\som\samples\somd\cpp\animal\dloc.c
del %relroot%\%1\som\samples\somd\cpp\animal\initterm.c
del %relroot%\%1\som\samples\somd\cpp\animal\readme
for %%i in (%altdevdir%\som\samples\somd\cpp\dii\*.*)           do ifarch copy %%i %relroot%\%1\som\samples\somd\cpp\dii
del %relroot%\%1\som\samples\somd\cpp\dii\foo.c
del %relroot%\%1\som\samples\somd\cpp\dii\readme
for %%i in (%altdevdir%\som\samples\somem\c\emdemo\*.*)         do ifarch copy %%i %relroot%\%1\som\samples\somem\c\emdemo
del %relroot%\%1\som\samples\somem\c\emdemo\readme
for %%i in (%altdevdir%\som\samples\somi\c\irdump\*.*)          do ifarch copy %%i %relroot%\%1\som\samples\somi\c\irdump
del %relroot%\%1\som\samples\somi\c\irdump\readme
for %%i in (%altdevdir%\som\samples\somk\c\animals\*.*)         do ifarch copy %%i %relroot%\%1\som\samples\somk\c\animals
del %relroot%\%1\som\samples\somk\c\animals\readme
for %%i in (%altdevdir%\som\samples\somk\c\tp\*.*)              do ifarch copy %%i %relroot%\%1\som\samples\somk\c\tp
del %relroot%\%1\som\samples\somk\c\tp\readme
for %%i in (%altdevdir%\som\samples\somk\cpp\*.*)               do ifarch copy %%i %relroot%\%1\som\samples\somk\cpp
del %relroot%\%1\som\samples\somk\cpp\readme
for %%i in (%altdevdir%\som\samples\somk\cpp\derived\*.*)       do ifarch copy %%i %relroot%\%1\som\samples\somk\cpp\derived
for %%i in (%altdevdir%\som\samples\somk\cpp\simple\*.*)        do ifarch copy %%i %relroot%\%1\som\samples\somk\cpp\simple
for %%i in (%altdevdir%\som\samples\somp\c\phone\*.*)           do ifarch copy %%i %relroot%\%1\som\samples\somp\c\phone
del %relroot%\%1\som\samples\somp\c\phone\readme
for %%i in (%altdevdir%\som\samples\somp\c\subclass\*.*)        do ifarch copy %%i %relroot%\%1\som\samples\somp\c\subclass
del %relroot%\%1\som\samples\somp\c\subclass\readme
for %%i in (%altdevdir%\som\samples\somp\cpp\phone\*.*)         do ifarch copy %%i %relroot%\%1\som\samples\somp\cpp\phone
del %relroot%\%1\som\samples\somp\cpp\phone\readme
for %%i in (%altdevdir%\som\samples\somr\c\menudemo\*.*)        do ifarch copy %%i %relroot%\%1\som\samples\somr\c\menudemo
del %relroot%\%1\som\samples\somr\c\menudemo\readme
for %%i in (%altdevdir%\som\samples\somr\cpp\menudemo\*.*)      do ifarch copy %%i %relroot%\%1\som\samples\somr\cpp\menudemo
del %relroot%\%1\som\samples\somr\cpp\menudemo\readme
set altdevdir=
:nocopy
cd ..
