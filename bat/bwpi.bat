@echo off
call bmode

call docd %devdir%\wpi\o
wmake /h /i
call docd %devdir%\wpi\ont
wmake /h /i
call docd %devdir%\wpi\ontaxp
wmake /h /i
call docd %devdir%\wpi\objos2
wmake /h /i
call docd %devdir%\wpi\objos2f
wmake /h /i
call docd %devdir%\wpi\opm16
wmake /h /i
cd %devdir%

