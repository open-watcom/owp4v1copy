rem @echo off
set gmlopt=wbls
if [%1] == [c] set gmlinc=cl;gml;whelp
if [%1] == [C] set gmlinc=cl;gml;whelp
if [%2] == [html] set gmlopt=html
if [%2] == [help] set gmlopt=nbhlp
if [%2] == [os2help] set gmlopt=nbhlpos2
if [%gmlopt%] == [wbls] goto default
if not [%3] == [] set gmlswt=%3 %4 %5 %6 %7
goto fmtit
:default
if not [%2] == [] set gmlswt=%2 %3 %4 %5 %6
:fmtit
call fmt %1 %gmlswt%
set gmlopt=
set gmlswt=
