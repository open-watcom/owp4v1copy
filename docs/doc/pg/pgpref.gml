.preface Preface
.*
The
.book &product Programmer's Guide
includes the following major components:
.begbull
.*
.if '&target' eq 'DOS' .do begin
.*
.bull
DOS Programming Guide
.bull
The DOS/4GW DOS Extender
.bull
Windows 3.x Programming Guide
.*
.do end
.*
.bull
Windows NT Programming Guide
.*
.if '&target' eq 'DOS' .do begin
.*
.bull
OS/2 Programming Guide
.*
.do end
.*
.if '&target' eq 'QNX' .do begin
.*
.bull
QNX Programming Guide
.*
.do end
.*
.if '&target' eq 'DOS' .do begin
.*
.bull
AutoCAD ADS Programming Guide
.bull
Novell NLM Programming Guide
.bull
Mixed Language Programming
.*
.do end
.*
.bull
Common Problems
.endbull
.*
.sect Acknowledgements
.*
.im gmlack
.*
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
.np
The Plum Hall Validation Suite for &lang has been invaluable in
verifying the conformance of the &product compilers to the
.if '&lang' eq 'C' .do begin
ANSI C Language Standard.
.do end
.if '&lang' eq 'C/C++' .do begin
ANSI C Language Standard and the Draft Proposed C++ Language Standard.
.do end
.do end
.np
Many users have provided valuable feedback on earlier versions of
the &product compilers and related tools.
Their comments were greatly appreciated.
If you find problems in the documentation or have some good
suggestions, we would like to hear from you.
.np
&pubdate..
.*
.sect Trademarks Used in this Manual
.*
.if '&target' eq 'AXPNT' .do begin
.np
Alpha AXP, AXP, DEC and Digital
are trademarks of Digital Equipment Corporation.
.do end
.*
.if '&target' eq 'DOS' .do begin
.np
AutoCAD Development System is a trademark of Autodesk, Inc.
.*
.np
DOS/4G and DOS/16M are trademarks of Tenberry Software, Inc.
.*
.np
OS/2 is a trademark of International Business Machines Corp.
IBM Developer's WorkFrame/2, Presentation Manager, and OS/2 are
trademarks of International Business Machines Corp.
.do end
IBM is a registered trademark of International Business Machines Corp.
.*
.np
Intel and Pentium are registered trademarks of Intel Corp.
.*
.np
Microsoft, Windows and Windows 95 are registered trademarks of
Microsoft Corp.
Windows NT is a trademark of Microsoft Corp.
.*
.if '&target' eq 'DOS' .do begin
.np
NetWare, NetWare 386, and Novell are registered trademarks of Novell,
Inc.
.*
.np
Phar Lap, 286|DOS-Extender and 386|DOS-Extender are trademarks of
Phar Lap Software, Inc.
.do end
.*
.if '&target' eq 'QNX' .do begin
.np
QNX is a registered trademark of QNX Software Systems Ltd.
.do end
.*
.np
UNIX is a registered trademark of UNIX System Laboratories, Inc.
.*
.np
.im wtrdmrk
