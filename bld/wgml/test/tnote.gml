.* test for &gml.note and &gml.fn tags
.se fil=&'left(&sysfnam,&'pos('.',&sysfnam)-1)
:layout
:convert file="&fil..lay".
:elayout
:gdoc sec='top secret'
:body
:p.
Some text.

:note.
Text on note tag.

:hp1.HP1 text for note in input line one.:ehp1.
More text for note in another line two
More text for note in another line three
More text for note in another line four
More text for note in another line five
More text for note in another line six
:p
Paragraph
:fn.fussnote:efn.
following note.
:egdoc
