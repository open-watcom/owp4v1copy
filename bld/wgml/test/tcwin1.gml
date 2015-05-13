:CMT.This file uses TY since many sections do not allow text
:CMT.This can cause problems with the DOS version of wgml 4.0
:CMT.Uncomment/recomment TY lines until the output fits in the DOS console window
:CMT.Also uncomment TOC, FIGLIST, and INDEX and add the appropriate items so that
:CMT.they will have some content when our wgml can generate them
:CMT.There are no BODY banners because, when a header is specified, wgml 4.0
:CMT.insists on using the default banners, at least on the first page of the BODY
:LAYOUT
    :DEFAULT
        binding = 0 columns = 1 font = 0 gutter = 0 input_esc = '~'
        justify = no spacing = 1
    :PAGE
        depth = '9.66i' left_margin = '1i' right_margin = '7i' top_margin = 0
    :WIDOW
        threshold = 1
    :ABSTRACT
        abstract_string = "Test Abstract"
        columns = 1 font = 1 header = yes page_eject = yes page_reset = yes
        post_skip = 0 pre_top_skip = 0 spacing = 1
    :ADDRESS
        left_adjust = 0 font = 0 page_position = left pre_skip = 0
        right_adjust = 0
    :ALINE
        skip = 0
    :APPENDIX
        appendix_string = "Test Appendix "
        align = 0 case = mixed columns = 1 display_heading = yes font = 0
        header = yes indent = 0 line_break = yes number_font = 0
        number_form = new number_reset = yes number_style = b page_eject = yes
        page_position = left page_reset = no post_skip = 0 pre_skip = 0
        pre_top_skip = 0 section_eject = yes spacing = 1
    :AUTHOR
        font = 0 left_adjust = 0 page_position = left pre_skip = 0
        right_adjust = 0 skip = 0
    :BACKM
        backm_string = "Test Back Matter"
        columns = 1 font = 0 header = yes page_eject = yes page_reset = no
        post_skip = 0 pre_top_skip = 0
    :BODY
        body_string = "Test Body"
        font = 0 header = yes post_skip = 0 pre_top_skip = 0
        page_eject = yes page_reset = yes
    :DATE
        date_form = "$ml $dsn, $yl"
        font = 0 left_adjust = 0 page_position = right pre_skip = 0
        right_adjust = 0
    :DOCNUM 
        docnum_string = "Document Number "
        left_adjust = 0 right_adjust = 0 page_position = right font = 0
        pre_skip = 2 
    :FIG
        default_frame = box default_place = inline font = 0 left_adjust = 0
        post_skip = 0 pre_skip = 0 right_adjust = 0 spacing = 1
    :FIGCAP
        figcap_string = "Figure "
        delim = '.' font = 0 pre_lines = 0 string_font = 0
    :FIGDESC
        font = 0 pre_lines = 0
    :FIGLIST
        columns = 1 fill_string = "." left_adjust = 0 right_adjust = 0
        skip = 0 spacing = 1
    :FLPGNUM
        font = 0 size = 0
    :H0
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H1
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H2
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H3
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H4
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H5
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :H6
        align = 0 case = mixed display_heading = yes font = 0 group = 0
        indent = 0 line_break = yes number_font = 0 number_form = none
        number_reset = yes number_style = h page_eject = no
        page_position = left post_skip = 0 pre_skip = 0 pre_top_skip = 0
        spacing = 1
    :I1
        font = 0 indent = 0 index_delim = "  " post_skip = 0 pre_skip = 0
        skip = 0 string_font = 0 wrap_indent = 0
    :I2
        font = 0 indent = 0 index_delim = "  " post_skip = 0 pre_skip = 0
        skip = 0 string_font = 0 wrap_indent = 0
    :I3
        font = 0 indent = 0 index_delim = "  " post_skip = 0 pre_skip = 0
        skip = 0 wrap_indent = 0
    :INDEX
        index_string = "Test Index"
        see_string = "See "
        see_also_string = "See also "
        columns = 1 font = 0 header = yes left_adjust = 0 page_eject = yes
        page_reset = no post_skip = 0 pre_top_skip = 0 right_adjust = 0
        spacing = 1
    :IXHEAD
        font = 0 frame = box header = yes indent = 0 post_skip = 0
        pre_skip = 0
    :IXMAJOR
        font = 0
    :IXPGNUM
        font = 0
    :PREFACE
        preface_string = "Test Preface"
        columns = 1 font = 0 header = yes page_eject = yes page_reset = no
        post_skip = 0 pre_top_skip = 0 spacing = 1
    :TITLE
        font = 0 left_adjust = 0 page_position = right pre_top_skip = 0
        right_adjust = 0 skip = 0
    :TITLEP
        columns = 1 spacing = 1
    :TOC
        columns = 1 fill_string = "." left_adjust = 0 right_adjust = 0
        spacing = 1 toc_levels = 7
    :TOCH0
        align = 0 display_in_toc = no font = 0 group = 0 
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH1
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH2
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH3
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH4
        align = 0 display_in_toc = no font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH5
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH6
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 0 post_skip = 0 pre_skip = 0 skip = 0
    :TOCPGNUM
        font = 0 size = 0
    :BANNER
        depth = 1 docsect = abstract left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = 'Abstract Top &$amp.$pgnuma.'
        depth = 1 hoffset = left font = 0 indent = 0 pouring = last refnum = 1
        region_position = centre script_format = no voffset = 0 width = extend 
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = abstract left_adjust = 1 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'Abstract Bottom &$amp.$pgnuma.'
        depth = 1 hoffset = left indent = 0 font = 0 pouring = last refnum = 1
        region_position = center script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = appendix left_adjust = 0 place = top right_adjust = 0
    :BANREGION
        contents = 'Appendix Top &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = 0 indent = 0 pouring = last refnum = 1
        region_position = left script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = appendix left_adjust = 0 place = bottom right_adjust = 0
    :BANREGION
        contents = 'Appendix Bottom &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = 0 indent = 0 pouring = last refnum = 1
        region_position = left script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = preface left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = 'Preface Top &$amp.$pgnuma.'
        depth = 1 hoffset = left font = 0 indent = 0 pouring = last refnum = 1
        region_position = centre script_format = no voffset = 0 width = extend 
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = preface left_adjust = 1 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'Preface Bottom &$amp.$pgnuma.'
        depth = 1 hoffset = left indent = 0 font = 0 pouring = last refnum = 1
        region_position = center script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = backm left_adjust = 0 place = top right_adjust = 0
    :BANREGION
        contents = 'Back Matter &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = 0 indent = 0 pouring = last refnum = 1
        region_position = left script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = backm left_adjust = 0 place = bottom right_adjust = 0
    :BANREGION
        contents = 'Back Matter Bottom &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = 0 indent = 0 pouring = last refnum = 1
        region_position = left script_format = no voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = figlist left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = 'List of Figures Top &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 refnum = 1
        region_position = centre pouring = last script_format = no
        voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = figlist left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'List of Figures Bottom &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 refnum = 1
        region_position = centre pouring = last script_format = no
        voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = index left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = 'Index Top &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 pouring = last
        script_format = no refnum = 1 region_position = centre
        voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = index left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'Index Bottom &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 pouring = last
        script_format = no refnum = 1 region_position = centre
        voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = toc left_adjust = 0 place = top 
        right_adjust = 0
    :BANREGION
        contents = 'Table of Contents Top &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 pouring = last
        refnum = 1 region_position = centre script_format = no voffset = 0
        width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 1 docsect = toc left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = 'Table of Contents Bottom &$amp.$pgnuma.'
        depth = 1 font = 0 hoffset = left indent = 0 pouring = last
        refnum = 1 region_position = centre script_format = no voffset = 0
        width = extend
    :eBANREGION
    :eBANNER
:eLAYOUT.
:GDOC.
:cmt. &SYSIN. &SYSINR. pre-FRONTM1/IN +3 -3
:cmt..in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-FRONTM2
:FRONTM.
.co on
.ju on
:cmt..ty &SYSIN. &SYSINR. pre-TITLEP1/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-TITLEP2
:TITLEP.
:cmt..ty &SYSIN. &SYSINR. pre-TITLE11/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-TITLE12
:TITLE.Test Document
:cmt..ty &SYSIN. &SYSINR. pre-TITLE22/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-TITLE22
:TITLE.Second Title Line
:cmt..ty &SYSIN. &SYSINR. pre-DOCNUM1/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-DOCNUM2
:DOCNUM.765
:cmt..ty &SYSIN. &SYSINR. pre-DATE1/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-DATE2
:DATE.September 25, 1985
:cmt..ty &SYSIN. &SYSINR. pre-AUTHOR11/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-AUTHOR12
:AUTHOR.The Invincible Mr Rabbit
:cmt..ty &SYSIN. &SYSINR. pre-AUTHOR21/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-AUTHOR22
:AUTHOR.The Incredible Ms Mouse
:cmt..ty &SYSIN. &SYSINR. pre-ADDRESS1/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-ADDRESS2
:ADDRESS.
:cmt..ty &SYSIN. &SYSINR. pre-ALINE12/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-ALINE12
:ALINE.A Lovely Street
:cmt..ty &SYSIN. &SYSINR. pre-ALINE21/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-ALINE22
:ALINE.Lined with Elms
:cmt..ty &SYSIN. &SYSINR. pre-ALINE32/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-ALINE32
:ALINE.A Small Town
:cmt..ty &SYSIN. &SYSINR. pre-ALINE41/IN +3 -3
.in +3 -3
:cmt..ty &SYSIN. &SYSINR. pre-ALINE42
:ALINE.A Decent Country
:cmt..ty &SYSIN. &SYSINR. pre-eADDRESS1/IN -2 2
.in -2 2
:cmt..ty &SYSIN. &SYSINR. pre-eADDRESS2
:eADDRESS.
:cmt..ty &SYSIN. &SYSINR. pre-eTITLEP
:eTITLEP.
:cmt..ty &SYSIN. &SYSINR. pre-ABSTRACT
:ABSTRACT.
:cmt..ty &SYSIN. &SYSINR. Abstract start
.br
some Abstract text
.in 3 -3
.br
some Abstract Preface text
:cmt..ty &SYSIN. &SYSINR. Abstract end
:PREFACE.
:cmt..ty &SYSIN. &SYSINR. Preface start
.br
some Preface text
.in -2 2
.br
some more Preface text
:cmt..ty &SYSIN. &SYSINR. Preface end
:cmt.:TOC.
:cmt..ty &SYSIN. &SYSINR. post-TOC
:cmt.:FIGLIST.
:cmt..ty &SYSIN. &SYSINR. post-FIGLIST
:BODY.
:cmt..ty &SYSIN. &SYSINR. Body start
.br
:CMT.This is the only way to get our wgml to match wgml 4.0 with the default
:CMT."body bottom" banner: wgml4 does NOT set $htext1 at all and the "$htext1"
:CMT.in the BANREGION, when H0 and H1 are defined, matches the H0 text, not the
:CMT.H1 text
.sr $htext1=Test Body
some Body text
.in 3 -3
.br
some more Body text
:cmt..ty &SYSIN. &SYSINR. Body end
:APPENDIX.
:cmt..ty &SYSIN. &SYSINR. Appendix start
.br
some Appendix text
.in -2 2
.br
some more Appendix text
:cmt..ty &SYSIN. &SYSINR. Appendix end
:BACKM.
:cmt..ty &SYSIN. &SYSINR. Back Matter start
.br
some Back Matter text
.in 3 -3
.br
some more Back Matter text
:cmt..ty &SYSIN. &SYSINR. Back Matter end
:cmt.:INDEX.
:cmt..ty &SYSIN. &SYSINR. post-INDEX
:eGDOC.
:cmt..ty &SYSIN. &SYSINR. post-eGDOC

