.sr docnum='BA0614'
.* diskspace req'd for 8K clusters and 16K clusters
.sr diskspace = '441 MB'
.sr clusspace = '8K clusters (257MB to 512MB)'
.sr diskspc16 = '514 MB'
.sr clusspc16 = '16K clusters (513MB to 1GB)'
.sr readme = 1
.* .sr beta = 1
.*
:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:set symbol="lang"       value="C/C++".
:INCLUDE file='DEFS'.
:INCLUDE file='XDEFS'.
.*
:GDOC.
.*
.if &e'&dohelp eq 0 .do begin
.*
:FRONTM.
:TITLEP.
:TITLE stitle="Getting Started".&product
:TITLE.Getting Started
.if &e'&beta eq 1 .do begin
:TITLE.(&ver..&rev Beta)
.do end
:INCLUDE file='DOCTITLE'.
:eTITLEP.
:ABSTRACT.
:INCLUDE file='COPYRITE'.
:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
.pa odd
:TOC.
:cmt.:FIGLIST.
.pa odd
.do end
.*
:BODY.
.*
.if &e'&dohelp eq 1 .do begin
:exhelp
:include file='&book..idx'
:include file='&book..tbl'
:include file='&book..kw'
.do end
.*
:include file='intro'
:include file='install'
:include file='tutorial'
:include file='docson'
:include file='c10mfc'
:include file='som'
:include file='bench'
:include file='redist'
:include file='diffs'
:cmt. :include file='lic'
:cmt. :include file='tshoot'
.if &e'&dohelp eq 0 .do begin
:BACKM.
.cd set 2
:INDEX.
.do end
.cntents end_of_book
:eGDOC.
