.* MACROS USED WITH DESCRIPTIONS OF GRAPHIC FUNCTIONS
.*
.sr grfun = "PC Graphics"
.sr grindex = "PC Graphics Functions"
.sr graphics = 1
.*
.dm coord begin
.  .mono (&*1,&*2)&*3
.dm coord end
.*
.dm termhead begin
:DTHD.&*1
:DDHD.&*2
.sk
.dm termhead end
.*
.dm picture begin
.sk
:GRAPHIC depth='3.6i' file='&*1..ps'.
.dm picture end
.*
.dm gfunc begin
:H5._&*
:set symbol="function" value="_&*".
:set symbol="func" value="_&*".
:set symbol="headtxt0$" value="_&*".
:set symbol="headtxt1$" value="_&*".
.uindex &*
.newcode Synopsis:
.if '&lang' eq 'C' .do begin
.if '&'substr(&*,1,2)' eq 'pg' .do begin
#include <pgchart.h>
.do end
.el .do begin
#include <graph.h>
.do end
.do end
.dm gfunc end
.*
.dm gfuncw begin
:H5._&*, _&*._w
:set symbol="function" value="_&*".
:set symbol="func" value="_&*".
:set symbol="func2" value="_&*._w".
:set symbol="headtxt0$" value="_&* &rroutines".
:set symbol="headtxt1$" value="_&* &rroutines".
.uindex &*
.uindex &*._w
.newcode Synopsis:
.if '&lang' eq 'C' .do begin
#include <graph.h>
.do end
.dm gfuncw end
.*
.dm gfuncwxy begin
:H5._&*, _&*._w, _&*._wxy
:set symbol="function" value="_&*".
:set symbol="func" value="_&*".
:set symbol="func2" value="_&*._w".
:set symbol="func3" value="_&*._wxy".
:set symbol="headtxt0$" value="_&* &rroutines".
:set symbol="headtxt1$" value="_&* &rroutines".
.uindex &*
.uindex &*._w
.uindex &*._wxy
.newcode Synopsis:
.if '&lang' eq 'C' .do begin
#include <graph.h>
.do end
.dm gfuncwxy end
.*
.dm gfuncms begin
:H5._&*, _&*.ms
:set symbol="function" value="_&*".
:set symbol="func" value="_&*".
:set symbol="func2" value="_&*.ms".
:set symbol="headtxt0$" value="_&* &rroutines".
:set symbol="headtxt1$" value="_&* &rroutines".
.uindex &*
.uindex &*.ms
.newcode Synopsis:
.if '&lang' eq 'C' .do begin
#include <pgchart.h>
.do end
.dm gfuncms end
.*
.dm gfuncend begin
.endcode
:set symbol="func" value=";.sf4 &function;.esf ".
.if '&'substr(&function,1,3)' eq '_pg' .do begin
:set symbol="func2" value=";.sf4 &function.ms;.esf ".
.do end
.el .do begin
:set symbol="func2" value=";.sf4 &function._w;.esf ".
:set symbol="func3" value=";.sf4 &function._wxy;.esf ".
.do end
.dm gfuncend end
.*
.dm hex begin
.if '&lang' eq 'C' .do begin
.mono 0x&*1&*2
.do end
.el .do begin
.mono '&*1'x&*2
.do end
.dm hex end
.*
.dm grexam begin
.if '&*1' eq 'begin' .do begin
. .im gr_sizes &*2
. .in 0
. :DL &NTEphi..:DT.Example::DD.
:cmt. . .in 7
. .in 0.46i
. :XMP.:SF font=4.
. .sk -1
. .im &*2
. :set symbol="SF_set" value="on".
.do end
.if '&*' eq 'output' .do begin
. :eSF.:eXMP.
. :set symbol="SF_set" value="off".
. .cp 24
. .in &INDlvl
. :PC.produces the following:
.do end
.if '&*' eq 'end' .do begin
. .if '&SF_set' eq 'on' .do begin
. :eSF.:eXMP.
. .do end
. :eDL.
. .in &INDlvl
.do end
.dm grexam end
.*
.dm gfuncref begin
.uindex &*1
(see the
.kw _&*1
&routine)&*2
.dm gfuncref end
.*
.dm gfuncsref begin
.uindex &*1
(see the
.kw _&*1
&routines)&*2
.dm gfuncsref end
.*
.dm uterm begin
.note _&*
.uindex &*
.dm uterm end
.*
:cmt..dm udesc begin
:cmt.:SF font=3._&*:eSF.
:cmt..uindex &*
:cmt..dm udesc end
.*
.dm uindex begin
:IH1 print='_&*'.&*
:I1 &*.
.dm uindex end
.*
.dm class begin
:CMT.   NULL macro
.dm class end
.*
.dm seelist begin
.se *i=2
.pe &*0.-1
. .seekw &*1 &*&*i. &*&*i.+1;.se *i=&*i.+1
.dm seelist end
.*
.dm seekw begin
.if '&*2' ne '' .do begin
. .if &seecnt ne 0 .ct ,
. .ct
. .se seecnt=1
. .mono &*2
.do end
.dm seekw end
.*
.dm kw begin
.sr $$kw="&'strip(&*,'T',',')"
.sr $$kw="&'strip(&$$kw.,'T','.')"
.sr $$kw="&'strip(&$$kw.,'L','_')"
:I1.&$$kw.
:SF font=4.&*:eSF.
.dm kw end
.*
.* fd... macros are used for function definitions
.*
.dm fdbeg begin
The following &routines are defined:
:DL compact termhi=1 tsize=20 break.
.dm fdbeg end
.*
.dm fd begin
:DT.&*
:DD.
.dm fd end
.*
.dm fdend begin
:eDL.
.dm fdend end
