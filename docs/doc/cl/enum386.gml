.if &e'&nobox eq 0 .do begin
.boxdef
..if '&format' eq '7x9' ..th ..do begin
.  .boxcol 13
.  .boxcol 11
.  .boxcol 11
..do end
..el ..do begin
.  .boxcol 15
.  .boxcol 12
.  .boxcol 12
..do end
.boxbeg
›Type          ›Smallest Value›Largest Value
.boxline
.monoon
›signed char   ›-128       ›127
›unsigned char ›0          ›255
›signed short  ›-32768     ›32767
›unsigned short›0          ›65535
›signed int    ›-2147483648›2147483647
›unsigned int  ›0          ›4294967295
.monooff
.boxend
.do end
.el .do begin
.millust begin
                  Smallest      Largest
Type               Value         Value
-------------  ------------  ------------
signed char            -128           127
unsigned char             0           255
signed short         -32768         32767
unsigned short            0         65535
signed int      -2147483648    2147483647
unsigned int              0    4294967295
.millust end
.do end
