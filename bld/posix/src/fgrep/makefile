cpcmd = copy>nul

fgrep.exe: egrep.exe
        $(cpcmd) $[@ $^.

egrep.exe: $(posix_dir)/src/egrep/egrep.c
        wmake $(__makeopts__) -f $(posix_dir)/src/egrep/makefile

clean: .symbolic
        wmake $(__makeopts__) -f $(posix_dir)/src/egrep/makefile clean
