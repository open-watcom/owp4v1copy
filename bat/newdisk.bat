@rem Usage: @call newdisk disk_num label
@rem   e.g. @call newdisk 2 "Library & Linker Disk"
@rem
@echo @call idisk %1                            >>mkdisks.bat
@echo #>>%INSTALL%
@echo # install diskette #%1>>%INSTALL%
@echo #>>%INSTALL%
@set NEWDISK=disk %1, labelled
:loop
@set NEWDISK=%NEWDISK% %2
@shift
@if not [%2]==[] goto loop
