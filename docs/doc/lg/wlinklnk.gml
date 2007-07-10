.np
.ix 'default directive file'
.ix 'wlink.lnk' 'default directive file'
The file :FNAME.wlink.lnk:eFNAME. is a special linker directive file
that is automatically processed by the &lnkname before processing any
other directives.
On a DOS, OS/2, or Windows-hosted system, this file must be located in
one of the paths specified in the
.ev PATH
environment variable.
On a QNX-hosted system, this file should be located in the
.id /etc
directory.
A default version of this file is located in
the :FNAME.\watcom\binw:eFNAME. directory on DOS-hosted systems,
the :FNAME.\watcom\binp:eFNAME. directory on OS/2-hosted systems,
the
.id /etc
directory on QNX-hosted systems, and
the :FNAME.\watcom\binnt:eFNAME. directory on Windows 95 or
Windows NT-hosted systems.
.ix 'wlsystem.lnk' 'directive file'
Note that the file :FNAME.wlink.lnk:eFNAME. includes the
file :FNAME.wlsystem.lnk:eFNAME. which is located in
the :FNAME.\watcom\binw:eFNAME. directory on DOS, OS/2, or
Windows-hosted systems and the
.id /etc
directory on QNX-hosted systems.
.np
The files :FNAME.wlink.lnk:eFNAME. and :FNAME.wlsystem.lnk:eFNAME.
reference the
.ev &pathvarup.
environment variable which must be set to the directory in which you
installed your software.
.np
Default directive file name (:FNAME.wlink.lnk:eFNAME.) can be overridden by 
.ev WLINK_LNK 
environment variable. If new file is not found then default file name is used 
as usual.
