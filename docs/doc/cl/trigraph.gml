.ix 'trigraphs'
..sk 1 c
.if &e'&nobox eq 0 .do begin
.boxdef
.boxcol 8 c
.boxcol 8 c
.boxbeg
›         ›Trigraph
›Character›Sequence
.boxline
.monoon
›[  ›??(
›]  ›??)
›{  ›??<
›}  ›??>
›|  ›??!
›#  ›??=
›\  ›??/
›^  ›??'
›~~ ›??-
.monooff
.boxend
.do end
.el .do begin
.millust begin
            Trigraph
Character   Sequence
---------   --------
    [         ??(
    ]         ??)
    {         ??<
    }         ??>
    |         ??!
    #         ??=
    \         ??/
    ^         ??'
    ~~         ??-
.millust end
.do end
