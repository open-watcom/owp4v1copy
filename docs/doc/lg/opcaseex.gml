.*
.*
.option CASEEXACT
.*
.np
The "CASEEXACT" option tells the &lnkname to respect case when
resolving references to global symbols.
That is, "ScanName" and "SCANNAME" represent two different symbols.
By default, the linker is case insensitive; "ScanName" and
"SCANNAME" represent the same symbol.
The format of the "CASEEXACT" option (short form "C") is as follows.
.mbigbox
    OPTION CASEEXACT
.embigbox
.np
.ix 'default directive file' 'wlink.lnk'
.ix 'wlink.lnk' 'default directive file'
If you have specified the "CASEEXACT" option in the default directive
files :FNAME.wlink.lnk:eFNAME. or :FNAME.wlsystem.lnk:eFNAME., it is
possible to override this option by using the "NOCASEEXACT" option.
The "NOCASEEXACT" option turns off case-sensitive linking.
The format of the "NOCASEEXACT" option (short form "NOCASE") is as
follows.
.mbigbox
    OPTION NOCASEEXACT
.embigbox
.*
.im wlinklnk
.*
