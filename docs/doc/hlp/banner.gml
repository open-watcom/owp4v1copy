.dm topsect begin
.cp 10
.sr *tag=&*
.if '&*1' ne '&*tag.' .do begin
.   .sr *tag=&'strip(&*1,'T',',')
.do end
:ZH2 ctx='&*tag.'.&*
.pu 1 .ixsect &*
.se headtxt0$=&*
.se headtxt1$=&*
.se SCTlvl=1
.cntents *ctx='&*tag.' &*
.dm topsect end
