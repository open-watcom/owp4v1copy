@echo off
set PTH=%1
if [%1] == [] set PTH=.
:lup
   shift
   if [%1] == [] goto done
   set NAME=%1
   if [%NAME%] == [-] set NAME=..
   set PTH=%PTH%\%NAME%
goto lup
:done
cd %PTH%\.
set PTH=
set NAME=
