.if &e'&nobox eq 0 .do begin
.boxdef
..if '&format' eq '7x9' ..th ..do begin
.  .boxcol 12
.  .boxcol 10
.  .boxcol 10
..do end
..el ..do begin
.  .boxcol 15
.  .boxcol 12
.  .boxcol 12
..do end
.boxbeg
›Type         ›Smallest Value›Largest Value
.boxline
.monoon
›signed char  ›-128  ›127
›unsigned char›0     ›255
›signed int   ›-32768›32767
›unsigned int ›0     ›65535
.monooff
.boxend
.do end
.el .do begin
.millust begin
               Smallest  Largest
Type            Value     Value
-------------  --------  --------
signed char       -128       127
unsigned char        0       255
signed int      -32768     32767
unsigned int         0     65535
.millust end
.do end
