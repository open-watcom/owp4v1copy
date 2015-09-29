.if &'lower(&syspdev) NE whelp .do begin
:LAYOUT
    :PAGE
        depth = '8.5i' left_margin = '1.25i' right_margin = '7.25i'
        top_margin = '.30i'
    :DEFAULT
        binding = 0 input_esc = '~' justify = no
    :TITLEP
        spacing = 1
    :TITLE
        font = 11 left_adjust = 0 page_position = centre pre_top_skip = 3
        right_adjust = 0 skip = 1
    :DOCNUM
        docnum_string = ""
        font = 9 left_adjust = 0 page_position = centre pre_skip = 2
        right_adjust = 0
    :DATE
        font = 2 left_adjust = 0 page_position = centre pre_skip = 2
        right_adjust = 0
    :AUTHOR
        font = 2 left_adjust = 0 page_position = centre pre_skip = 12
        right_adjust = 0 skip = 0
    :ADDRESS
        font = 2 left_adjust = 0 page_position = centre pre_skip = 1
        right_adjust = 0
    :ALINE
        skip = 0
    :ABSTRACT
        font = 2 header = no page_reset = no
    :PREFACE
        preface_string = "" font = 11 header = no page_eject = odd
        pre_top_skip = 0 spacing = 1
    :P
        line_indent = 0 post_skip = 0 pre_skip = 1
    :PC
        line_indent = 0 post_skip = 0 pre_skip = 1
    :XMP
        font = 4 left_indent = '0.3i' post_skip = 0 pre_skip = 0
    :FIG
        default_frame = none default_place = inline font = 4
        left_adjust = 7 post_skip = 1 pre_skip = 1
    :FIGCAP
        figcap_string = "Figure "
        delim = '.' font = 1 pre_lines = 0 string_font = 3
    :FIGDESC
        font = 1 pre_lines = 0
    :FN
        align = '0.3i' font = 1 frame = rule skip = 0
    :OL
        align = '0.3i' left_indent = '0.3i' number_style = hd
        post_skip = 0 spacing = 1
    :UL
        align = '0.05i' left_indent = '0.3i' post_skip = 0 pre_skip = 1
        skip = 1 spacing = 1
    :SL
        left_indent = '0.3i' post_skip = 0 pre_skip = 1 skip = 0
        spacing = 1
    :DL
        align = '0.76i' left_indent = '0.0i' post_skip = 0 pre_skip = 1
    :DTHD
        font = 3
    :DDHD
        font = 3
    :DT
        font = 1
    :DD
        font = 0
    :H0
        font = 12 indent = 0 number_form = none number_reset = no
        page_eject = odd page_position = right
    :H1
        display_heading = yes font = 11 number_font = 12 number_form = new
        page_eject = odd page_position = left post_skip = 2
        pre_skip = 0 pre_top_skip = 0
    :H2
        font = 10 number_font = 10 number_form = prop number_style = h
        page_eject = no post_skip = 0 pre_skip = 1 pre_top_skip = 0
    :H3
        font = 9 number_font = 9 number_form = prop number_style = h
        page_eject = no post_skip = 0 pre_skip = 1 pre_top_skip = 0
    :H4
        font = 8 number_font = 8 number_form = prop number_style = h
        page_eject = no post_skip = 0 pre_skip = 1 pre_top_skip = 0
    :H5
        font = 8 line_break = yes number_form = none page_eject = no
        post_skip = 0 pre_skip = 1 pre_top_skip = 0
    :H6
        font = 10 line_break = yes number_form = none page_eject = no
        page_position = centre post_skip = 0 pre_skip = 1 pre_top_skip = 0
    :NOTE
        note_string = "Notes: "
    :TOC
        spacing = 1 toc_levels = 5
    :TOCH0
        indent = 4 post_skip = 1 pre_skip = 2 skip = 2
    :TOCH1
        indent = 4 post_skip = 0 pre_skip = 1 skip = 1
    :TOCH2
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH3
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH4
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH5
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :APPENDIX
        appendix_string = ""
        font = 11 header = yes number_font = 11 number_style = bd
        page_eject = odd post_skip = 3 pre_skip = 0 pre_top_skip = 0
        section_eject = yes
    :INDEX
        columns = 2 spacing = 1 page_eject = odd
    :IXHEAD
        font = 8 post_skip = 1 pre_skip = 1
    :I1
        post_skip = 0 pre_skip = 0 skip = 0
    :I2
        indent = '0.4cm' post_skip = 0 pre_skip = 0 skip = 0
    :I3
        indent = '0.4cm' post_skip = 0 pre_skip = 0 skip = 0
    :BANNER
        depth = 3 docsect = body left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = 'Body Top &$amp.$pgnuma.'
        depth = '0.23i' hoffset = left indent = 0 font = 8 pouring = last refnum = 1
        region_position = center script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 2 docsect = body left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'Body Bottom &$amp.$pgnuma.'
        depth = 1 hoffset = left indent = 0 font = 8 pouring = last refnum = 1
        region_position = center script_format = no voffset = 1 width = extend
    :eBANREGION
    :eBANNER
:eLAYOUT.
.do end
:GDOC.
:FRONTM.
:cmt.:PREFACE.
:BODY.
.co on
.ju off

.im tbsyms.gml
.im tbmacs.gml
.im tbtext.gml

:eGDOC.


