:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:INCLUDE file='XDEFS'.
:GDOC.
.*
.if &e'&dohelp eq 0 .do begin
.*
:FRONTM.
:TITLEP.
:TITLE stitle="Partners".Sybase Partners Program
:set symbol="isbn" value="".
:graphic depth='5.0i' xoff='-1.5i' yoff='0.0i' scale=100 file='pwrs.eps'.
:eTITLEP.
:ABSTRACT.
:INCLUDE file='COPYRITE'.
:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
:PREFACE.
:INCLUDE file='PREFACE'.
.pa odd
:TOC.
:cmt.   :FIGLIST.
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
.section Sybase Home on the World Wide Web
.*
.np
To get the most out of your use of this product, you may want to
download other files or read other information on the Sybase Web site
located at:
.sk 1 c
.bd http://www.sybase.com/
.*
.section Becoming a Partner
.*
.np
Sybase Inc. has dedicated substantial resources to creating strong,
mutually beneficial, noncompetitive partnerships. By partnering with
Sybase, we'll provide you with the resources you need to be
successful. You'll receive the software, tools, updates, training, and
support you need to start developing and marketing products and
services based on Sybase Inc.'s technologies. Select the program
that's right for you, after all, your success is our success!
.np
The Sybase Partner Programs page is located at:
.sk 1 c
.bd http://www.sybase.com/partners/
.*
.section Looking for a Solution
.*
The CODE/ISV Program is the Sybase ISV partner program supporting all
of our products from tools to database and middleware. The Program's
focus is to engage ISV technology partners to help extend Sybase
products into new and exciting market segments, through cooperative
marketing, development and technical support programs.
The CODE/ISV Partner Program page is located at:
.sk 1 c
.bd http://www.sybase.com/partners/code/
.sk 1 c
Select "Search Partners" to go to the CODE/ISV Partners listing.
From this page, select the search method that you want to use
to locate the information you seek.
.*
.if &e'&dohelp eq 0 .do begin
:BACKM.
.cd set 2
:INDEX.
.do end
.*
.cd set 1
.cntents end_of_book
:eGDOC.
