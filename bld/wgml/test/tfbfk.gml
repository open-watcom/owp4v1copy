.if &'lower(&syspdev) NE whelp .do begin
:LAYOUT
    :CONVERT file='layout0.txt'
:eLAYOUT.
:cmt..im pslay.gml   
:LAYOUT
    :CONVERT file='layout1.txt'
:eLAYOUT.
.do end
:GDOC.
:BODY.
.co on
.ju off
text before tag P with no text following
:P.
.br;text before FB/FK start
.br;text before first FB block
.fb begin
.br;This is floating block 1 line 01.
.br;This is floating block 1 line 02.
.br;This is floating block 1 line 03.
.br;This is floating block 1 line 04.
.br;This is floating block 1 line 05.
.br;This is floating block 1 line 06.
.br;This is floating block 1 line 07.
.br;This is floating block 1 line 08.
.br;This is floating block 1 line 09.
.br;This is floating block 1 line 10.
.fb end
text after first FB block
.br;test line for large FK block 01
.br;test line for large FK block 02
.br;text before first FK block
.fk begin
.br;This is floating keep 1 line 01.
.br;This is floating keep 1 line 02.
.br;This is floating keep 1 line 03.
.br;This is floating keep 1 line 04.
.br;This is floating keep 1 line 05.
.br;This is floating keep 1 line 06.
.br;This is floating keep 1 line 07.
.br;This is floating keep 1 line 08.
.br;This is floating keep 1 line 09.
.br;This is floating keep 1 line 10.
.br;This is floating keep 1 line 11.
.br;This is floating keep 1 line 12.
.br;This is floating keep 1 line 13.
.br;This is floating keep 1 line 14.
.br;This is floating keep 1 line 15.
.br;This is floating keep 1 line 16.
.br;This is floating keep 1 line 17.
.br;This is floating keep 1 line 18.
.br;This is floating keep 1 line 19.
.br;This is floating keep 1 line 20.
.br;This is floating keep 1 line 21.
.br;This is floating keep 1 line 22.
.br;This is floating keep 1 line 23.
.br;This is floating keep 1 line 24.
.br;This is floating keep 1 line 25.
.br;This is floating keep 1 line 26.
.br;This is floating keep 1 line 27.
.br;This is floating keep 1 line 28.
.br;This is floating keep 1 line 29.
.br;This is floating keep 1 line 30.
.br;This is floating keep 1 line 31.
.br;This is floating keep 1 line 32.
.br;This is floating keep 1 line 33.
.br;This is floating keep 1 line 34.
.br;This is floating keep 1 line 35.
.br;This is floating keep 1 line 36.
.br;This is floating keep 1 line 37.
.br;This is floating keep 1 line 38.
.br;This is floating keep 1 line 39.
.br;This is floating keep 1 line 40.
.br;This is floating keep 1 line 41.
.br;This is floating keep 1 line 42.
.br;This is floating keep 1 line 43.
.br;This is floating keep 1 line 44.
.br;This is floating keep 1 line 45.
.br;This is floating keep 1 line 46.
.br;This is floating keep 1 line 47.
.br;This is floating keep 1 line 48.
.br;This is floating keep 1 line 49.
.br;This is floating keep 1 line 50.
.fk end
text after first FK block
:P.And now for a few dozen lines to test columns in ABSTRACT and PREFACE. 
If possible, to verify those prior results that led to the use of "pane".
What follows is text from forWiki:
.fb begin
.br;This is floating block 2 line 01.
.br;This is floating block 2 line 02.
.br;This is floating block 2 line 03.
.br;This is floating block 2 line 04.
.br;This is floating block 2 line 05.
.br;This is floating block 2 line 06.
.br;This is floating block 2 line 07.
.br;This is floating block 2 line 08.
.br;This is floating block 2 line 09.
.br;This is floating block 2 line 10.
.fb end
.fk begin
.br;This is floating keep 2 line 01.
.br;This is floating keep 2 line 02.
.br;This is floating keep 2 line 03.
.br;This is floating keep 2 line 04.
.br;This is floating keep 2 line 05.
.br;This is floating keep 2 line 06.
.br;This is floating keep 2 line 07.
.br;This is floating keep 2 line 08.
.br;This is floating keep 2 line 09.
.br;This is floating keep 2 line 10.
.fk end
:P.Our wgml sets system symbol LL to 60 just as wgml 4.0 does, even for device WHELP, 
even when the margins define a wider line length - and our code ignores the value 
(except when it is involved in returning a symbol value as a string). This seems 
like a resonable thing to do, so our code now sets the value of system symbol CD 
to 1 and that of system symbol CL to 60 regardless of the LAYOUT.
:P.This was tested only with the DEFAULT block; other blocks that define columns may 
eventually require different treatment, something that can be dealt with if and 
when it comes up.
.fb begin
.br;This is floating block 3 line 01.
.br;This is floating block 3 line 02.
.br;This is floating block 3 line 03.
.br;This is floating block 3 line 04.
.br;This is floating block 3 line 05.
.br;This is floating block 3 line 06.
.br;This is floating block 3 line 07.
.br;This is floating block 3 line 08.
.br;This is floating block 3 line 09.
.br;This is floating block 3 line 10.
.fb end
.fk begin
.br;This is floating keep 3 line 01.
.br;This is floating keep 3 line 02.
.br;This is floating keep 3 line 03.
.br;This is floating keep 3 line 04.
.br;This is floating keep 3 line 05.
.br;This is floating keep 3 line 06.
.br;This is floating keep 3 line 07.
.br;This is floating keep 3 line 08.
.br;This is floating keep 3 line 09.
.br;This is floating keep 3 line 10.
.br;This is floating keep 3 line 11.
.br;This is floating keep 3 line 12.
.br;This is floating keep 3 line 13.
.br;This is floating keep 3 line 14.
.br;This is floating keep 3 line 15.
.br;This is floating keep 3 line 16.
.br;This is floating keep 3 line 17.
.br;This is floating keep 3 line 18.
.br;This is floating keep 3 line 19.
.br;This is floating keep 3 line 20.
.br;This is floating keep 3 line 21.
.br;This is floating keep 3 line 22.
.br;This is floating keep 3 line 23.
.br;This is floating keep 3 line 24.
.br;This is floating keep 3 line 25.
.br;This is floating keep 3 line 26.
.br;This is floating keep 3 line 27.
.br;This is floating keep 3 line 28.
.br;This is floating keep 3 line 29.
.br;This is floating keep 3 line 30.
.br;This is floating keep 3 line 31.
.br;This is floating keep 3 line 32.
.br;This is floating keep 3 line 33.
.br;This is floating keep 3 line 34.
.br;This is floating keep 3 line 35.
.br;This is floating keep 3 line 36.
.br;This is floating keep 3 line 37.
.br;This is floating keep 3 line 38.
.br;This is floating keep 3 line 39.
.br;This is floating keep 3 line 40.
.br;This is floating keep 3 line 41.
.br;This is floating keep 3 line 42.
.br;This is floating keep 3 line 43.
.br;This is floating keep 3 line 44.
.br;This is floating keep 3 line 45.
.br;This is floating keep 3 line 46.
.br;This is floating keep 3 line 47.
.br;This is floating keep 3 line 48.
.br;This is floating keep 3 line 49.
.br;This is floating keep 3 line 50.
.fk end
:P.When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a 
heading after ejecting one or more pages, the FIG appears at the top of the next 
column, with the section heading at the top of the first column. This was unexpected 
when encountered, as it had been thought that the heading appeared alone at the top 
of the current page and the FIG would be moved to the next page.
:P.When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a heading 
after ejecting one or more pages, the FIG appears at the top of the next column, with 
the section heading at the top of the first column. This was unexpected when encountered, 
as I had expected that the heading would appear alone at the top of the current page, 
even though the figure appeared at the top of the next column.
:P.When BINCLUDE and/or GRAPHIC are used following EGDOC, our wgml worked only when the 
number of columns was (re-)set to 1 (as opposed to the value in the LAYOUT DEFAULT tag). 
:P.The width of the GRAPHIC in wgml 4.0 supported the theory that this was the correct number 
of columns.
:P.wgml 4.0 failed to output a lot of text in the second column (of 2) of ABSTRACT that it had 
no trouble with when ABSTRACT had only one column – and no trouble with in the second 
column (of 3) in PREFACE. Removing such distractors as the section heading and the IN/BX 
lines did not change this behavior.
:P.This line occurs in whelp.gml (included by whelplay.gml, which is specified by whelp.opt, 
which is processed when "file whelp" is used on the command line).
:P.It is supposed to suppress emitting a second space after stops (such as ":" or "."). It may 
well do so when SCRIPT is specified on the command line; however, when WSCRIPT is used 
(as all of the OW docs do), then it has no effect at all. 
:P.The implementation was identifying it, and ignoring it, but then dropping through to the 
default case, causing a superfluous error message to be emitted. Although technically 
correct, since nothing happens when ".dc stop" is encountered, since this is only used 
with ".dc stop off" and that has no effect, the implementation is, in fact, correct. 
:P.The error message was, therefore, suppressed.
:P.Changing the default font (that is, the value of attribute FONT of tag DEFAULT in the 
LAYOUT section) from "0" to "8" revealed a lot of discrepancies. The OW docs  use "0" 
exclusively, but, if our wgml is intended to work as described, these problems need 
to be resolved.
:P.The strangest discrepancy was found testing FIG in the ABSTRACT section: text on the 
same line as the eFIG was done in font "8", but text on subsequent lines was done in 
font "0". Font "8" was also seen inside a CO OFF/CO ON block. While not complete, 
this should give some feeling for the problem.
:P.In some ways, this is similar to the behavior inside tables, where the fonts are 
forced to font 0.
:P.As might be imagined, this makes it very difficult to tell, in any particular place, 
whether the current font or font 0 is being used.
.fb dump
.fk dump
:P.And now for a few dozen lines to test columns in ABSTRACT and PREFACE. 
If possible, to verify those prior results that led to the use of "pane".
What follows is text from forWiki:
:P.Our wgml sets system symbol LL to 60 just as wgml 4.0 does, even for device WHELP, 
even when the margins define a wider line length - and our code ignores the value 
(except when it is involved in returning a symbol value as a string). This seems 
like a resonable thing to do, so our code now sets the value of system symbol CD 
to 1 and that of system symbol CL to 60 regardless of the LAYOUT.
:P.This was tested only with the DEFAULT block; other blocks that define columns may 
eventually require different treatment, something that can be dealt with if and 
when it comes up.
.fb begin
.br;This is floating block 4 line 01.
.br;This is floating block 4 line 02.
.br;This is floating block 4 line 03.
.br;This is floating block 4 line 04.
.br;This is floating block 4 line 05.
.br;This is floating block 4 line 06.
.br;This is floating block 4 line 07.
.br;This is floating block 4 line 08.
.br;This is floating block 4 line 09.
.br;This is floating block 4 line 10.
.fb end
.fk begin
.br;This is floating keep 4 line 01.
.br;This is floating keep 4 line 02.
.br;This is floating keep 4 line 03.
.br;This is floating keep 4 line 04.
.br;This is floating keep 4 line 05.
.br;This is floating keep 4 line 06.
.br;This is floating keep 4 line 07.
.br;This is floating keep 4 line 08.
.br;This is floating keep 4 line 09.
.br;This is floating keep 4 line 10.
.fk end
:P.When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a 
heading after ejecting one or more pages, the FIG appears at the top of the next 
column, with the section heading at the top of the first column. This was unexpected 
when encountered, as it had been thought that the heading appeared alone at the top 
of the current page and the FIG would be moved to the next page.
:P.When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a heading 
after ejecting one or more pages, the FIG appears at the top of the next column, with 
the section heading at the top of the first column. This was unexpected when encountered, 
as I had expected that the heading would appear alone at the top of the current page, 
even though the figure appeared at the top of the next column.
:P.When BINCLUDE and/or GRAPHIC are used following EGDOC, our wgml worked only when the 
number of columns was (re-)set to 1 (as opposed to the value in the LAYOUT DEFAULT tag). 
:P.The width of the GRAPHIC in wgml 4.0 supported the theory that this was the correct number 
of columns.
.fb begin
.br;This is floating block 5 line 01.
.br;This is floating block 5 line 02.
.br;This is floating block 5 line 03.
.br;This is floating block 5 line 04.
.br;This is floating block 5 line 05.
.br;This is floating block 5 line 06.
.br;This is floating block 5 line 07.
.br;This is floating block 5 line 08.
.br;This is floating block 5 line 09.
.br;This is floating block 5 line 10.
.fb end
.fk begin
.br;This is floating keep 5 line 01.
.br;This is floating keep 5 line 02.
.br;This is floating keep 5 line 03.
.br;This is floating keep 5 line 04.
.br;This is floating keep 5 line 05.
.br;This is floating keep 5 line 06.
.br;This is floating keep 5 line 07.
.br;This is floating keep 5 line 08.
.br;This is floating keep 5 line 09.
.br;This is floating keep 5 line 10.
.fk end
:P.wgml 4.0 failed to output a lot of text in the second column (of 2) of ABSTRACT that it had 
no trouble with when ABSTRACT had only one column – and no trouble with in the second 
column (of 3) in PREFACE. Removing such distractors as the section heading and the IN/BX 
lines did not change this behavior.
:P.This line occurs in whelp.gml (included by whelplay.gml, which is specified by whelp.opt, 
which is processed when "file whelp" is used on the command line).
:P.It is supposed to suppress emitting a second space after stops (such as ":" or "."). It may 
well do so when SCRIPT is specified on the command line; however, when WSCRIPT is used 
(as all of the OW docs do), then it has no effect at all. 
:P.The implementation was identifying it, and ignoring it, but then dropping through to the 
default case, causing a superfluous error message to be emitted. Although technically 
correct, since nothing happens when ".dc stop" is encountered, since this is only used 
with ".dc stop off" and that has no effect, the implementation is, in fact, correct. 
:P.The error message was, therefore, suppressed.
:P.Changing the default font (that is, the value of attribute FONT of tag DEFAULT in the 
LAYOUT section) from "0" to "8" revealed a lot of discrepancies. The OW docs  use "0" 
exclusively, but, if our wgml is intended to work as described, these problems need 
to be resolved.
:P.The strangest discrepancy was found testing FIG in the ABSTRACT section: text on the 
same line as the eFIG was done in font "8", but text on subsequent lines was done in 
font "0". Font "8" was also seen inside a CO OFF/CO ON block. While not complete, 
this should give some feeling for the problem.
:P.In some ways, this is similar to the behavior inside tables, where the fonts are 
forced to font 0.
:P.As might be imagined, this makes it very difficult to tell, in any particular place, 
whether the current font or font 0 is being used.
.fb begin
.br;This is floating block 6 line 01.
.br;This is floating block 6 line 02.
.br;This is floating block 6 line 03.
.br;This is floating block 6 line 04.
.br;This is floating block 6 line 05.
.br;This is floating block 6 line 06.
.br;This is floating block 6 line 07.
.br;This is floating block 6 line 08.
.br;This is floating block 6 line 09.
.br;This is floating block 6 line 10.
.fb end
.fk begin
.br;This is floating keep 6 line 01.
.br;This is floating keep 6 line 02.
.br;This is floating keep 6 line 03.
.br;This is floating keep 6 line 04.
.br;This is floating keep 6 line 05.
.br;This is floating keep 6 line 06.
.br;This is floating keep 6 line 07.
.br;This is floating keep 6 line 08.
.br;This is floating keep 6 line 09.
.br;This is floating keep 6 line 10.
.fk end
.fb dump
.fk dump
Text after final DUMP lines
:eGDOC.
 
