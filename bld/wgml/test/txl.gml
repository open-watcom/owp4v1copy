:cmt. test nested lists :dl :gl :ol, :sl, :ul
:cmt. item tags :LI and :LP
:LAYOUT.
    :PAGE
        depth = '9.66i' left_margin = '1i' right_margin = '7i' top_margin = 0.
    :DEFAULT
        spacing = 1 font = 0 columns = 1 justify = no
    :DL
        align = '1i' left_indent = '0.5i' level = 1 line_break = no post_skip = 1
        pre_skip = 1 right_indent = '0.5i' skip = 1 spacing = 1
    :DL
        align = '1i' left_indent = '0.25i' level = 2 line_break = no post_skip = 1
        pre_skip = 1 right_indent = '0.25i' skip = 1 spacing = 1
    :DD
        line_left = '0.5i' font = 0
    :DDHD
        font = 1
    :DT
        font = 2
    :DTHD
        font = 1
    :GL
        align = 0 delim = ':' left_indent = '0.5i' level = 1 post_skip = 1 pre_skip = 1
        right_indent = '0.5i' skip = 1 spacing = 1
    :GL
        align = 0 delim = ':' left_indent = '0.25i' level = 2 post_skip = 1 pre_skip = 1
        right_indent = '0.25i' skip = 1 spacing = 1
    :GD
        font = 0
    :GT
        font = 2
    :LP
        left_indent = '0.25i' line_indent = 0 post_skip = 1 pre_skip = 1 right_indent = '0.25i'
        spacing = 1
    :OL
        align = '0.4i' font = 0 left_indent = '0.5i' level = 1 number_font = 0 number_style = rd
        post_skip = 1 pre_skip = 1 right_indent = '0.5i' skip = 1 spacing = 1
    :OL
        align = '0.4i' font = 0 left_indent = '0.5i' level = 2 number_font = 0 number_style = ad
        post_skip = 1 pre_skip = 1 right_indent = '0.5i' skip = 1 spacing = 1
    :OL
        align = '0.4i' font = 0 left_indent = '0.5i' level = 3 number_font = 0 
        post_skip = 1 pre_skip = 1 right_indent = '0.5i' skip = 1 spacing = 1
    :OL
        align = '0.4i' font = 0 left_indent = '0.5i' level = 1 number_font = 0 number_style = hd
        post_skip = 1 pre_skip = 1 right_indent = '0.5i' skip = 1 spacing = 1
    :P
        line_indent = 0 post_skip = 0 pre_skip = 1
    :PC
        line_indent = 0 post_skip = 0 pre_skip = 1
    :SL
        font = 0 left_indent = '0.5i' level = 1 post_skip = 1 pre_skip = 1 right_indent = '0.5i'
        skip = 1 spacing = 1
    :UL
        align = '0.4i' bullet = '@' bullet_translate = yes bullet_font = 0 font = 0
        left_indent = '0.5i' level = 1 post_skip = 1 pre_skip = 1 right_indent = '0.1i'
        skip = 1 spacing = 1
    :UL
        align = '0.4i' bullet = '|' bullet_translate = yes bullet_font = 0 font = 0
        left_indent = '0.5i' level = 2 post_skip = 1 pre_skip = 1 right_indent = '0.1i'
        skip = 1 spacing = 1
    :UL
        align = '0.4i' bullet_translate = yes bullet_font = 0 font = 0
        left_indent = '0.5i' level = 3 post_skip = 1 pre_skip = 1 right_indent = '0.1i'
        skip = 1 spacing = 1
    :UL
        align = '0.4i' bullet = '*' bullet_translate = yes bullet_font = 0 font = 0
        left_indent = '0.5i' level = 1 post_skip = 1 pre_skip = 1 right_indent = '0.1i'
        skip = 1 spacing = 1
    :convert file='txl.lay'.
:eLAYOUT.
:GDOC.
:BODY.
.im txl1.inc
.ty body  in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
:p.
&gml.SL follows.
.ty p     in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
:sl compact.
    :li.simple 1 compact
.ty li sl in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
    :li.simple 2 compact
:lp.
&gml.OL follows.
    :ol.
.*      This is text before first &gml.li which extends over more
.*         than one line to see how it looks.
        :li.ordered 1 no compact spacing 2 and some text to hit the right margin
to see how a multi line list item looks
.ty li ol in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
        :li.ordered 2 no compact
    :lp.list part
still list part still list part still list part still list 
.ty lp    in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
        :li.ordered 3 no compact
    :ul
        :li.ul with eul in txl2 include file
.im txl2.inc
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
:li. ul item 3
:li. ul item 4
:eul.
:esl.
&gml.SL end.
.ty end  in=&$in inr=&$inr ir=&$ir ll=&$ll lc=&$lc lst=&$lst pg=&$pgnuma
:EGDOC.
