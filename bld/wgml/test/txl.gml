:cmt. test for nested lists :ol, :sl, :ul, :lp
:LAYOUT.
:DT
        font = 2
:GT
        font = 2
:DTHD
        font = 1
:DD
        line_left = '0.5i'
        font = 0
:GD
        font = 0
:DDHD
        font = 1
:SL
        level = 1
        left_indent = 2
        right_indent = 0
        pre_skip = 0
        skip = 0
        spacing = 1
        post_skip = 0
        font = 0
:OL
        level = 1
        left_indent = 2
        right_indent = 0
        pre_skip = 0
        skip = 0
        spacing = 1
        post_skip = 0
        font = 0
        align = '0.4i'
        number_style = hd
        number_font = 0
:UL
        level = 1
        left_indent = 2
        right_indent = 0
        pre_skip = 0
        skip = 0
        spacing = 1
        post_skip = 0
        font = 0
        align = '0.4i'
        bullet = '*'
        bullet_translate = yes
        bullet_font = 0
:DL
        level = 1
        left_indent = 2
        right_indent = 0
        pre_skip = 0
        skip = 0
        spacing = 1
        post_skip = 0
        align = '1i'
        line_break = no
:GL
        level = 1
        left_indent = 2
        right_indent = 0
        pre_skip = 0
        skip = 0
        spacing = 1
        post_skip = 0
        align = 0
        delim = ':'
:BANNER
        left_adjust = 0
        right_adjust = 0
        depth = 3
        place = top
        docsect = body
:BANREGION
        indent = 0
        hoffset = left
        width = extend
        voffset = 2
        depth = 1
        font = 0
        refnum = 1
        region_position = left
        pouring = last
        script_format = yes
        contents = '/&$amp.$htext1.// &$amp.$pgnuma./'
:eBANREGION
:eBANNER
:eLAYOUT.
:GDOC.
:BODY.
SL follows.
:sl compact.
:li.simple 1
:li.simple 2
.br
OL follows.
:ol.
:li.ordered 1
:li.ordered 2
:lp.list part
still list part
still list part
still list part
still list part
:li.ordered 3
.im txl2.gml

:eol.
OL end.
.br
UL follows.
:ul.
:li.unordered 1
:li.unordered 2
:eul.
UL end.
:li.simple 3
:esl.
SL end.
:EGDOC.
