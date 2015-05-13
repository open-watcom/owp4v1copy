:LAYOUT
    :PAGE
        depth = '9.66i' left_margin = '1i' right_margin = '7i' top_margin = 0
    :DEFAULT
        binding = 0 columns = 1 font = 0 gutter = '0.5i' input_esc = '~'
        justify = no spacing = 1
    :WIDOW
        threshold = 1
    :BODY
        body_string = 'BODY'
        font = 1 header = no post_skip = 0 pre_top_skip = 0
        page_eject = yes page_reset = yes
    :P
        line_indent = '1.5i' post_skip = 0 pre_skip = 0
    :PC
        line_indent = '0.5i' post_skip = 0 pre_skip = 0
    :NOTE
        font = 2 left_indent = 0 note_string = 'NOTE: ' pre_skip = 1 post_skip = 1
        right_indent = 0 spacing = 1
    :LP
        left_indent = '0.5i' right_indent = '0.5i' line_indent = 0 pre_skip = 1
        post_skip = 1 spacing = 1
    :OL
        level = 1 left_indent = '0.1i' right_indent = '0.1i' pre_skip = 1 skip = 1 
        spacing = 1 post_skip = 1 font = 0 align = '0.3i' number_style = hd 
        number_font = 0
    :SL
        level = 1 left_indent = '0.3i' right_indent = '0.3i' pre_skip = 1 skip = 1 
        spacing = 1 post_skip = 1 font = 0
    :UL
        level = 1 left_indent = '0.2i' right_indent = '0.2i' pre_skip = 1 skip = 1 
        spacing = 1 post_skip = 1 font = 0 align = '0.4i' bullet = '*' 
        bullet_translate = yes bullet_font = 0
    :TITLEP
        columns = 1 spacing = 1
    :TITLE
        font = 11 left_adjust = 0 page_position = centre pre_top_skip = 3
        right_adjust = 0 skip = 1
    :DOCNUM
        docnum_string = "" font = 9 left_adjust = 0 page_position = centre
        pre_skip = 2 right_adjust = 0
    :BANNER
        docsect = body place = bottom
    :eBANNER
:eLAYOUT.
:GDOC.
:BODY.
.co on
.ju off
.*
Single-list tests with LI
.sk
.in
IN reset for OL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before `
:ol.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LI2
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before EOL
:eol. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eOL
.sk
.in
IN reset for SL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before SL/LI1
:sl.
:li. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LI2
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before eSL
:esl. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eSL
.sk
.in
IN reset for UL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before UL/LI1
:ul.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LI2
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before EUL
:eul. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eUL
.pa
.*
Single-list tests with LP
.sk
.in
IN reset for OL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before OL/LP1
:ol.
:lp. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LP2
:lp. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before EOL
:eol. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eOL
.sk
.in
IN reset for SL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before SL/LP1
:sl.
:lp. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LP2
:lp. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before eSL
:esl. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eSL
.sk
.in
IN reset for UL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before UL/LP1
:ul.
:lp. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before LP2
:lp. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before EUL
:eul. &SYSIN. &SYSINR. inline text
.br
&SYSIN. &SYSINR. after eUL
.pa
.*
Embedded list test
.sk
.in
IN reset for OL inside UL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before UL/LI
:ul.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
:PC.test of PC inside list
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before OL/LI
:ol.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eOL
:eol. &SYSIN. &SYSINR. inline after eOL
.br
&SYSIN. &SYSINR. after eOL
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before OL/LI
:ol.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eOL
:eol. &SYSIN. &SYSINR. inline after eOL
.br
&SYSIN. &SYSINR. after eOL
.br
&SYSIN. &SYSINR. before eUL
:eul. &SYSIN. &SYSINR. inline after eUL
.br
&SYSIN. &SYSINR. after eUL
.sk
.in
IN reset for SL inside OL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before OL/LI
:ol.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before SL/LI
:sl.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eSL
:esl. &SYSIN. &SYSINR. inline after eSL
.br
&SYSIN. &SYSINR. after eSL
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before SL/LI
:sl.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eSL
:esl. &SYSIN. &SYSINR. inline after eSL
.br
&SYSIN. &SYSINR. after eSL
.br
&SYSIN. &SYSINR. before eOL
:eol. &SYSIN. &SYSINR. inline after eOL
.br
&SYSIN. &SYSINR. after eOL
.sk
.in
IN reset for UL inside SL
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before SL/LI
:sl.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before UL/LI
:ul.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eUL
:eul. &SYSIN. &SYSINR. inline after eUL
.br
&SYSIN. &SYSINR. after eUL
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before IN +3 -3
.in +3 -3
.br
&SYSIN. &SYSINR. before UL/LI
:ul.
:li. &SYSIN. &SYSINR. inline text
&SYSIN. &SYSINR. after inline text
.br
&SYSIN. &SYSINR. before IN -1 1
.in -1 1
.br
&SYSIN. &SYSINR. before eUL
:eul. &SYSIN. &SYSINR. inline after eUL
.br
&SYSIN. &SYSINR. after eUL
.br
&SYSIN. &SYSINR. before eSL
:esl. &SYSIN. &SYSINR. inline after eSL
.br
&SYSIN. &SYSINR. after eSL
.*
:eGDOC.

