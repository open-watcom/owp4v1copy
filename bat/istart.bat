@rem
@rem initialize installation make procedure
@rem
@if [%1] == [] goto help
@if [%2] == [] goto help
@if [%3] == [] goto help
@set IDATE=%1
@set ITIME=%2
@set ISRC=%relroot%\%3
@fd *.me /d%IDATE% /t%ITIME%
@goto done
:help
@echo Usage: ISTART month/day/year hour:min[:sec] release_dir
@echo   e.g. ISTART 5/28/90 8:00 rel1
:done
