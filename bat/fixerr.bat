@echo off
if not exist *.err goto done
echo DOING THIS ONE
ls *.err | sed -e 's/^/erase /' -e 's/\.err/.?bj/' > __tmp.bat
call __tmp
del __tmp.bat
:done
