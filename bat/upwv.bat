@echo off
fastdel h:\dv
xcopy h:\rel2\*.foo h:\dv\ /s/e
g:
cd \bld\wv
call bdbg dv
xcopy h:\dv r:\dlang /s
