.* FAMILY OF PROCESS REFERENCES
.seelist &*1 abort atexit _bgetcmd
.if '&machsys' eq 'QNX' .do begin
.seelist &*1 close
.do end
.seelist &*1 exec Functions
.seelist &*1 exit _Exit _exit getcmd getenv main onexit putenv
.if '&machsys' eq 'QNX' .do begin
.seelist &*1 sigaction signal
.do end
.seelist &*1 spawn Functions
.seelist &*1 system
.if '&machsys' eq 'QNX' .do begin
.seelist &*1 wait waitpid
.do end
