:cmt. test nested lists :ol, :sl, :ul 
:cmt. item tags :LI and :LP
:cmt. TBD  not yet :DL, :GL and subtags
:LAYOUT.
:LP
        left_indent = 4
        right_indent = 3 
        line_indent = 2 
        pre_skip = 1 
        post_skip = 1 
        spacing = 1 
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
        right_indent = 15
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
:p.
&gml.SL follows.
:sl compact.
    :li.simple 1 compact
    :li.simple 2 compact
:lp.
&gml.OL follows.
    :ol.
.*      This is text before first &gml.li which extends over more 
.*         than one line to see how it looks.
        :li.ordered 1 no compact spacing 2 and some text to hit the right margin
to see how a multi line list item looks
        :li.ordered 2 no compact
        :note. This is a note which extends over more than one line to see how it looks.
    :lp.list part
still list part still list part still list part still list part still list part
still list part still list part still list part still list part still list part
        :li.ordered 3 no compact
    :ul
        :li.ul with eul in txl2 include file
.im txl2.gml
    :eol.
:LP.
&gml.OL end.
:LP.
&gml.UL follows.
    :ul.
        :li.unordered 1 no compact
        :li.unordered 2 no compact
    :eul.
:LP.&gml.UL end.
    :li.simple 3 compact
:ul   compact.
:lp. ul compact started with &gml.LP as first item.
:li. ul item 1
:li. ul item 2
:eul.
:esl.
&gml.SL end.
:EGDOC.
