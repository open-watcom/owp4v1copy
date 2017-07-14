.if &'lower(&syspdev) NE whelp .do begin
:LAYOUT
    :CONVERT file='layout0.txt'
    :DEFAULT
        binding = 0 columns = 1 font = 0 gutter = '0.5i' input_esc = '~'
        justify = no spacing = 1
    :PAGE
        depth = '8.5i' left_margin = '1.25i' right_margin = '7.25i'
        top_margin = '.30i'
    :WIDOW
        threshold = 1
    :TITLEP
        columns = 1 spacing = 1
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
        abstract_string = "ABSTRACT"
        columns = 1 font = 11 header = no page_eject = even page_reset = yes
        post_skip = 1 pre_top_skip = 0 spacing = 1
    :PREFACE
        preface_string = "Preface"
        columns = 1 font = 11 header = no page_eject = odd page_reset = no
        post_skip = 1 pre_top_skip = 0 spacing = 1
    :BODY
        body_string = "Document Body"
        font = 11 header = no page_eject = yes page_reset = no
        post_skip = 0 pre_top_skip = 0
    :APPENDIX
        columns = 1 page_reset = no section_eject = even spacing = 1
        appendix_string = "Appendix "
        align = 0 case = mixed display_heading = yes font = 11 header = yes
        indent = 0 line_break = yes number_font = 11 number_form = new 
        number_reset = yes number_style = bd page_eject = no page_position = left 
        post_skip = 3 pre_skip = 0 pre_top_skip = 0
    :BACKM
        backm_string = "Additional Notes"
        columns = 1 font = 11 header = no page_eject = even page_reset = no
        post_skip = 0 pre_top_skip = 0

    :FIG
        default_frame = box default_place = inline font = 4 left_adjust = 0
        post_skip = 0 pre_skip = 0 right_adjust = 0  spacing = 1
    :FIGCAP
        figcap_string = "FredFigure "
        delim = '.' font = 1 pre_lines = 0 string_font = 3
    :FIGDESC
        font = 2 pre_lines = 0
    :FIGLIST
        fill_string = "." 
        columns = 1 left_adjust = 0 right_adjust = 0
        skip = 0 spacing = 1
    :FLPGNUM
        font = 0 size = '0.4i'
    :FN
        align = '0.3i' font = 1 frame = rule skip = 0
    :OL
        align = '0.3i' left_indent = '0.3i' number_style = hd
        post_skip = 0 pre_skip = 1 spacing = 1
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
    :HEADING
        delim = '.' max_group = 10 para_indent = no stop_eject = no
        threshold = 2
    :H0
        align = 0 case = mixed display_heading = yes font = 12 group = 0
        indent = 0 line_break = yes number_font = 3 number_form = none
        number_reset = yes number_style = h page_eject = odd page_position = right
        post_skip = 3 pre_skip = 0 pre_top_skip = 4 spacing = 1
    :H1
        align = 0 case = mixed display_heading = yes font = 11 group = 0
        indent = 0 line_break = yes number_font = 11 number_form = new
        number_reset = yes number_style = h page_eject = odd page_position = left
        post_skip = 2 pre_skip = 0 pre_top_skip = 0 spacing = 1
    :H2
        align = 0 case = mixed display_heading = yes font = 10 group = 0
        indent = 0 line_break = yes number_font = 10 number_form = prop
        number_reset = yes number_style = h page_eject = no page_position = left
        post_skip = 1 pre_skip = 1 pre_top_skip = 0 spacing = 1
    :H3
        align = 0 case = mixed display_heading = yes font = 9 group = 0
        indent = 0 line_break = yes number_font = 9 number_form = prop
        number_reset = yes number_style = h page_eject = no page_position = left
        post_skip = 0 pre_skip = 1 pre_top_skip = 0 spacing = 1
    :H4
        align = 0 case = mixed display_heading = yes font = 8 group = 0
        indent = 0 line_break = yes number_font = 8 number_form = prop
        number_reset = yes number_style = h page_eject = no page_position = left
        post_skip = 0 pre_skip = 1 pre_top_skip = 0 spacing = 1
    :H5
        align = 0 case = mixed display_heading = yes font = 8 group = 0
        indent = 0 line_break = yes number_font = 3 number_form = none
        number_reset = yes number_style = h page_eject = no page_position = right
        post_skip = 0 pre_skip = 1 pre_top_skip = 0 spacing = 1
    :H6
        align = 0 case = mixed display_heading = yes font = 8 group = 0
        indent = 0 line_break = yes number_font = 3 number_form = none
        number_reset = yes number_style = h page_eject = no page_position = centre
        post_skip = 0 pre_skip = 1 pre_top_skip = 0 spacing = 1
    :INDEX
        index_string = "Index"
        see_also_string = "See also " see_string = "See "
        columns = 2 font = 11 header = no left_adjust = 0 page_eject = odd
        page_reset = no post_skip = 0 pre_top_skip = 0 right_adjust = 0
        spacing = 1
    :IXPGNUM
        font = 1
    :IXMAJOR
        font = 2
    :IXHEAD
        font = 8 frame = box header = yes indent = 0 post_skip = 0
        pre_skip = 0
    :I1
        index_delim = "  "
        font = 1 indent = 1i post_skip = 0 pre_skip = 0 skip = 0
        string_font = 2 wrap_indent = '0.4i'
    :I2
        index_delim = "  " 
        font = 4 indent = '0.4cm' post_skip = 0 pre_skip = 0 skip = 0
        string_font = 3 wrap_indent = '0.4i'
    :I3
        index_delim = "  "
        font = 3 indent = '0.4cm' post_skip = 0 pre_skip = 0 skip = 0 
        wrap_indent = '0.4i'
    :NOTE
        note_string = "Notes: "
    :P
        line_indent = 0 post_skip = 1 pre_skip = 1
    :PC
        line_indent = 0 post_skip = 0 pre_skip = 1
    :TOC
        fill_string = "." 
        columns = 1 left_adjust = 0 right_adjust = 0
        spacing = 1 toc_levels = 7
    :TOCH0
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 4 post_skip = 1 pre_skip = 2 skip = 2
    :TOCH1
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 4 post_skip = 0 pre_skip = 1 skip = 1
    :TOCH2
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH3
        align = 0 display_in_toc = yes font = 0 group = 0
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH4
        align = 0 display_in_toc = no font = 0 group = 0
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH5
        align = 0 display_in_toc = no font = 0 group = 0
        indent = 4 post_skip = 0 pre_skip = 0 skip = 0
    :TOCH6
        align = 0 display_in_toc = no font = 0 group = 0
        indent = '0.2i' post_skip = 0 pre_skip = 0 skip = 0
    :TOCPGNUM
        font = 0 size = '0.4i'
    :XMP
        font = 4 left_indent = '0.3i' post_skip = 0 pre_skip = 0
        
    :BANNER
        depth = 4 docsect = abstract left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = ABSTRACT depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = abstract left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = preface left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = PREFACE depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre
        script_format = no voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = preface left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre
        script_format = no voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = body left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = tophead depth = '0.23i' hoffset = left indent = 0 font = 8 
        pouring = last refnum = 1  region_position = center script_format = no 
        voffset = 0 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 2 docsect = body left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 hoffset = left indent = 0 font = 8
        pouring = last refnum = 1 region_position = center script_format = no
        voffset = 1 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = appendix left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = headnum1 depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = appendix left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = backm left_adjust = 0 place = top
        right_adjust = 0
    :BANREGION
        contents = "BACK MATTER" depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = backm left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre script_format = no
        voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = figlist left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre
        script_format = no voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 4 docsect = toc left_adjust = 0 place = bottom
        right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0
        pouring = last refnum = 1 region_position = centre
        script_format = no voffset = 3 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = index left_adjust = 0 place = bottom right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0 
        pouring = last refnum = 1 region_position = left 
        script_format = no voffset = 2 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = head0 left_adjust = 0 place = top right_adjust = 0
    :BANREGION
        contents = headtext0 depth = 1 font = 0 hoffset = left indent = 0 
        pouring = last refnum = 1 region_position = left 
        script_format = no voffset = 2 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = head0 left_adjust = 0 place = bottom right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0 
        pouring = last refnum = 1 region_position = left 
        script_format = no voffset = 2 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = head1 left_adjust = 0 place = top right_adjust = 0
    :BANREGION
        contents = headtext1 depth = 1 font = 0 hoffset = left indent = 0 
        pouring = last refnum = 1 region_position = left 
        script_format = no voffset = 2 width = extend
    :eBANREGION
    :eBANNER
    :BANNER
        depth = 3 docsect = head1 left_adjust = 0 place = bottom right_adjust = 0
    :BANREGION
        contents = pgnuma depth = 1 font = 0 hoffset = left indent = 0 
        pouring = last refnum = 1 region_position = left 
        script_format = no voffset = 2 width = extend
    :eBANREGION
    :eBANNER
    :CONVERT file='layout1.txt'
:eLAYOUT.
.do end
:GDOC.
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:cmt.:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
:FRONTM.
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:cmt.:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
.if &'lower(&syspdev) NE whelp .do begin
:TITLEP.
:TITLE.Test Document
:TITLE.Second Title Line
:DOCNUM.765
:DATE.September 25, 1985
.sk 20
:AUTHOR.The Invincible Mr Rabbit
:AUTHOR.The Incredible Ms Mouse
.sk 10
:ADDRESS.
:ALINE.A Lovely Street
:ALINE.Lined with Elms
:ALINE.A Small Town
:ALINE.A Decent Country
:eADDRESS.
:eTITLEP.
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:cmt.:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
.do end
:ABSTRACT.
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:cmt.:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
:cmt.text on line after BINCLUDE
.ix '"aa'
.ix #aa
.ix $aa
.ix %aa
.ix &aa
.ix "'aa"
.ix (aa
.ix )aa
.ix *aa
.ix +aa
.ix ,aa
.ix -aa
.ix .aa
.ix 0aa
.ix 1aa
.ix 2aa
.ix 3aa
.ix 4aa
.ix 5aa
.ix 6aa
.ix 7aa
.ix 8aa
.ix 9aa
.ix :aa
.'ix ;aa
.ix <aa
.ix =aa
.ix >aa
.ix ?aa
.ix @aa
.ix caa
.ix daa
.ix eaa
.ix gaa
.ix haa
.ix jaa
.ix kaa
.ix laa
.ix naa
.ix oaa
.ix qaa
.ix raa
.ix uaa
.ix vaa
.ix waa
.ix xaa
.ix yaa
.ix zaa
.ix [aa
.ix \aa
.ix ]aa
.ix _aa
.ix {aa
.ix }aa
.ix ~aa
:I1. aaa
:I2. sub entry
:I2. to test
:I2. pagination
:I2. more testing 1!
:I2. more testing 2!
:I2. more testing 3!
:I2. more testing 4!
:I2. more testing 5!
:I2. more testing 6!
:I2. more testing 7!
:I2. more testing 8!
:I2. more testing 9!
:IH1 id = id_ih0 . IH1 For parsing test
:IH1 id = iaab print = aax. aab
:I1 pg = start. aab
:IREF refid = iaab pg = 'tim' see = john seeid = iaab
:IREF refid = iaab pg = fred
:I1 pg = 'fred'. aab
:I2 IH2 after IH1 with blank print
:IH1 id = id_steve 

 steve 
:IREF refid = id_steve pg = major
:i2 refid = id_steve first i2
:i2 refid = id_steve pg = major second i2
:I1 id = id_i1. I1 ID test
.ix "I1/IX test"
:I2 id = id_i2. I2 ID test
:I3 id = id_i3. I3 ID test
:cmt.:IH1 id = id_ih1 seeid = id_i1 print = 'zya'. IH1 PRINT test
:IH1 id = id_ih1 seeid = id_i1. IH1 PRINT test
:IH2 id = id_ih2 seeid = id_ih1. IH2 SEEID test
:cmt.:IH3 seeid = id_ih2. IH3 SEEID test
:IREF refid = id_ih2 pg = 'level 2 comma test'.
:IREF refid = id_ih2 seeid = id_ih0 pg = 'IREF SEEID test'.
:i1 id = i1_1. first i1tag entry
:iref refid = i1_1 see = tom.
.ix mary tim tom 'see fred'
.ix mary tim tom 'see joe'
.ix abs
.ix aaa
:cmt..ix abstract
.ix abstract *
.ix abstract *tom
.ix abstract term1 *
.ix abstract term1 *tom
.ix abstract term1 term2 *
.ix abstract term1 term2 tom
:IH1 abstract1
:I2 *
:IH1 abstract1 
:I2 *tom
:IH1 abstract1 
:IH2 term1 
:I3 pg = major. *
:IH1 abstract1 
:IH2 term1
:I3 *tom
:IH1 abstract1
:IH2 term1
:I3 id = I3_1. term2
:IREF refid = I3_1.
:IH1 abstract1
:IH2 term1
:I3 id = I3_2. term2
:IREF refid = I3_2.
.ix fred tom . 'see mary'
.ix mary tom fred . 'see also tim'
.ix mary tim tom 'see fred'
.ix mary tim tom 'see joe'
.ix "abstract one" . 0
.ix "abstract two" . 5
.ix "abstract three" . 10
.co on
.ju off
:cmt..cd set 2
:P.Input text line with tag P in ABSTRACT!
.in 2 -2
.bx 1 26
.br;More test lines in ABSTRACT 1
.br;More test lines in ABSTRACT 2
.br;More test lines in ABSTRACT 3
.br;More test lines in ABSTRACT 4
.br;More test lines in ABSTRACT 5
.br;More test lines in ABSTRACT 6
.br;More test lines in ABSTRACT 7
.br;More test lines in ABSTRACT 8
.br;More test lines in ABSTRACT 9
.br;More test lines in ABSTRACT 10
.br;More test lines in ABSTRACT 11
.br;More test lines in ABSTRACT 12
.br;More test lines in ABSTRACT 13
.br;More test lines in ABSTRACT 14
.br;More test lines in ABSTRACT 15
.br;More test lines in ABSTRACT 16
.br;More test lines in ABSTRACT 17
.br;More test lines in ABSTRACT 18
.br;More test lines in ABSTRACT 19
.br;More test lines in ABSTRACT 20
.br;More test lines in ABSTRACT 21
.br;More test lines in ABSTRACT 22
.br;More test lines in ABSTRACT 23
.br;More test lines in ABSTRACT 24
.br;More test lines in ABSTRACT 25
.br;More test lines in ABSTRACT 26
.br;More test lines in ABSTRACT 27
.br;More test lines in ABSTRACT 28
.br;More test lines in ABSTRACT 29
.br;More test lines in ABSTRACT 30
.br;More test lines in ABSTRACT 31
.br;More test lines in ABSTRACT 32
.br;More test lines in ABSTRACT 33
.br;More test lines in ABSTRACT 34
.br;More test lines in ABSTRACT 35
.br;More test lines in ABSTRACT 36
.br;More test lines in ABSTRACT 37
.br;More test lines in ABSTRACT 38
:cmt..br;More test lines in ABSTRACT 39
:cmt..br;More test lines in ABSTRACT 40
.in
.bx off
:P.And now for a few dozen lines to test columns in ABSTRACT and PREFACE. 
If possible, to verify those prior results that led to the use of "pane".
What follows is text from forWiki:
Our wgml sets system symbol LL to 60 just as wgml 4.0 does, even for device WHELP, 
even when the margins define a wider line length - and our code ignores the value 
(except when it is involved in returning a symbol value as a string). This seems 
like a resonable thing to do, so our code now sets the value of system symbol CD 
to 1 and that of system symbol CL to 60 regardless of the LAYOUT.
This was tested only with the DEFAULT block; other blocks that define columns may 
eventually require different treatment, something that can be dealt with if and 
when it comes up.
When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a 
heading after ejecting one or more pages, the FIG appears at the top of the next 
column, with the section heading at the top of the first column. This was unexpected 
when encountered, as it had been thought that the heading appeared alone at the top 
of the current page and the FIG would be moved to the next page.
When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a heading 
after ejecting one or more pages, the FIG appears at the top of the next column, with 
the section heading at the top of the first column. This was unexpected when encountered, 
as I had expected that the heading would appear alone at the top of the current page, 
even though the figure appeared at the top of the next column.
When BINCLUDE and/or GRAPHIC are used following EGDOC, our wgml worked only when the 
number of columns was (re-)set to 1 (as opposed to the value in the LAYOUT DEFAULT tag). 
The width of the GRAPHIC in wgml 4.0 supported the theory that this was the correct number 
of columns.
wgml 4.0 failed to output a lot of text in the second column (of 2) of ABSTRACT that it had 
no trouble with when ABSTRACT had only one column – and no trouble with in the second 
column (of 3) in PREFACE. Removing such distractors as the section heading and the IN/BX 
lines did not change this behavior.
This line occurs in whelp.gml (included by whelplay.gml, which is specified by whelp.opt, 
which is processed when "file whelp" is used on the command line).
It is supposed to suppress emitting a second space after stops (such as ":" or "."). It may 
well do so when SCRIPT is specified on the command line; however, when WSCRIPT is used 
(as all of the OW docs do), then it has no effect at all. 
The implementation was identifying it, and ignoring it, but then dropping through to the 
default case, causing a superfluous error message to be emitted. Although technically 
correct, since nothing happens when ".dc stop" is encountered, since this is only used 
with ".dc stop off" and that has no effect, the implementation is, in fact, correct. 
The error message was, therefore, suppressed.
Changing the default font (that is, the value of attribute FONT of tag DEFAULT in the 
LAYOUT section) from "0" to "8" revealed a lot of discrepancies. The OW docs  use "0" 
exclusively, but, if our wgml is intended to work as described, these problems need 
to be resolved.
The strangest discrepancy was found testing FIG in the ABSTRACT section: text on the 
same line as the eFIG was done in font "8", but text on subsequent lines was done in 
font "0". Font "8" was also seen inside a CO OFF/CO ON block. While not complete, 
this should give some feeling for the problem.
In some ways, this is similar to the behavior inside tables, where the fonts are 
forced to font 0.
As might be imagined, this makes it very difficult to tell, in any particular place, 
whether the current font or font 0 is being used.
.pa
:FIG place = top width = page.
test text in top page-width fig
:eFIG.
:FIG place = bottom width = column.
test text in bottom column-width fig 1
:eFIG.
:FIG place = bottom width = column.
test text in bottom column-width fig 2
:eFIG.
:FIG place = top width = column.
test text in top column-width fig 1
:eFIG.
:FIG place = top width = column.
test text in top column-width fig 2
:eFIG.
:P.Some text at the top of the page.
.if &'lower(&syspdev) EQ ps .do begin
:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000 width = page.
.do end
:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
.tb set $
.tb 2 15 30
$item one 01$item two 01$item three 01
$item one 02$item two 02$item three 02
$item one 03$item two 03$item three 03
$item one 04$item two 04$item three 04
$item one 05$item two 05$item three 05
$item one 06$item two 06$item three 06
$item one 07$item two 07$item three 07
$item one 08$item two 08$item three 08
$item one 09$item two 09$item three 09
$item one 10$item two 10$item three 10
.pa
:P.Some text at the top of the page.
.if &'lower(&syspdev) EQ ps .do begin
:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000 width = page.
.do end
:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
$item four 01$item five 01$item six 01
$item four 02$item five 02$item six 02
$item four 03$item five 03$item six 03
$item four 04$item five 04$item six 04
$item four 05$item five 05$item six 05
$item four 06$item five 06$item six 06
$item four 07$item five 07$item six 07
$item four 08$item five 08$item six 08
$item four 09$item five 09$item six 09
$item four 10$item five 10$item six 10
.tb
.tb set
:PREFACE.
:cmt..cd set 3
:P.And now for a few dozen lines to test columns in ABSTRACT and PREFACE. 
If possible, to verify those prior results that led to the use of "pane".
What follows is text from forWiki:
Our wgml sets system symbol LL to 60 just as wgml 4.0 does, even for device WHELP, 
even when the margins define a wider line length - and our code ignores the value 
(except when it is involved in returning a symbol value as a string). This seems 
like a resonable thing to do, so our code now sets the value of system symbol CD 
to 1 and that of system symbol CL to 60 regardless of the LAYOUT.
This was tested only with the DEFAULT block; other blocks that define columns may 
eventually require different treatment, something that can be dealt with if and 
when it comes up.
When the value of tag FIG attribute place is "top" and the default width is used or 
the value of attribute width is given is "column", and the section displays a 
heading after ejecting one or more pages, the FIG appears at the top of the next 
column, with the section heading at the top of the first column. This was unexpected 
when encountered, as it had been thought that the heading appeared alone at the top 
of the current page and the FIG would be moved to the next page - which happens when 
the value of attribute width is "page".
:cmt..cd set 1
:I1 pg = major. aab
:IREF refid = iaab pg = judith
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:cmt.:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.
:cmt.text on line after BINCLUDE
.ix preface
.ix preface *
.ix preface two *
.ix preface two three *
:P.Input text line with tag P in PREFACE!
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 5118dv scale = 1000.
.do end
.pa
:P.Text line at top of page.
:FIG.
.br;More test lines in PREFACE 1
.br;More test lines in PREFACE 2
.br;More test lines in PREFACE 3
.br;More test lines in PREFACE 4
.br;More test lines in PREFACE 5
.br;More test lines in PREFACE 6
.br;More test lines in PREFACE 7
.br;More test lines in PREFACE 8
.br;More test lines in PREFACE 9
.br;More test lines in PREFACE 10
.br;More test lines in PREFACE 11
.br;More test lines in PREFACE 12
.br;More test lines in PREFACE 13
.br;More test lines in PREFACE 14
.br;More test lines in PREFACE 15
.br;More test lines in PREFACE 16
.br;More test lines in PREFACE 17
.br;More test lines in PREFACE 18
.br;More test lines in PREFACE 19
.br;More test lines in PREFACE 20
.br;More test lines in PREFACE 21
.br;More test lines in PREFACE 22
.br;More test lines in PREFACE 23
.br;More test lines in PREFACE 24
.br;More test lines in PREFACE 25
.br;More test lines in PREFACE 26
.br;More test lines in PREFACE 27
.br;More test lines in PREFACE 28
.br;More test lines in PREFACE 29
.br;More test lines in PREFACE 30
:eFIG.
:TOC.
:FIGLIST.
:cmt..do end
:BODY.
:H0 Start with A HEADING!
.ix body
.ix body *
.ix body two *
.ix body two three *
.br
:cmt.:I1 pg = end . aab
.if &'lower(&syspdev) EQ ps .do begin
:cmt.:GRAPHIC file = 'acc4.ps' depth = 3322dv scale = 1000.
.do end
.ty before FIGREF  line
.br
This is what FIGREF produces for a forward reference:
The first one: :FIGREF refid = Sue page = yes. with more text after it.
.br
The second one: :FIGREF refid = Sue page = no., -- for the same fig.
.br
The third one, extended to push the fig onto the next page in pass 2: 
.pa
:P.Text line at top of page.
:FIGREF refid = FRed. -- well, still a bit more yet
.br;Test line for the top of the bottom box heading 1
.br;Test line for the top of the bottom box heading 2
.br;Test line for the top of the bottom box heading 3
.br;Test line for the top of the bottom box heading 4
.br;Test line for the top of the bottom box heading 5
.br;Test line for the top of the bottom box heading 6
.br;Test line for the top of the bottom box heading 7
.br;Test line for the top of the bottom box heading 8
.br;Test line for the top of the bottom box heading 9
.br;Test line for the top of the bottom box heading 10
.br
This is what FIGREF produces for a forward reference:
The first one: :FIGREF refid = Sue page = yes. with more text after it.
.br
The second one: :FIGREF refid = Sue page = no., -- for the same fig.
.br
The third one, extended to push the fig onto the next page in pass 2: 
:I1 id = ih1_fig. test preceding FIG fred
:FIGREF refid = FRed. -- well, still a bit more yet
.br;Test line for placing fig ambiguously 1
.br;Test line for placing fig ambiguously 2
.br;Test line for placing fig ambiguously 3
.br;Test line for placing fig ambiguously 4
.br;Test line for placing fig ambiguously 5
.br;Test line for placing fig ambiguously 6
.br;Test line for placing fig ambiguously 7
.br;Test line for placing fig ambiguously 8
.br;Test line for placing fig ambiguously 9
:FIG
Simple test FIG
:eFIG
.br;Test line for placing fig ambiguously 10
.br;Test line for placing fig ambiguously 11
.br;Test line for placing fig ambiguously 12
.br;Test line for placing fig ambiguously 13
.br;Test line for placing fig ambiguously 14
.br;Test line for placing fig ambiguously 15
.br;Test line for placing fig ambiguously 16
.br;Test line for placing fig ambiguously 17
.br;Test line for placing fig ambiguously 18
.br;Test line for placing fig ambiguously 19
.br;Test line for placing fig ambiguously 20
.br;Test line for placing fig ambiguously 21
.br;Test line for placing fig ambiguously 22
.br;Test line for placing fig ambiguously 23
.br;Test line for placing fig ambiguously 24
.br;Test line for placing fig ambiguously 25
.br;Test line for placing fig ambiguously 26
.br;Test line for placing fig ambiguously 27
.br;Test line for placing fig ambiguously 28
.br;Test line for placing fig ambiguously 29
.br;Test line for placing fig ambiguously 30
.br;Test line for placing fig ambiguously 31
.br;Test line for placing fig ambiguously 32
.br;Test line for placing fig ambiguously 33
.br;Test line for placing fig ambiguously 34
.br;Test line for placing fig ambiguously 35
.br;Test line for placing fig ambiguously 36
.br;Test line for placing fig ambiguously 37
.br;Test line for placing fig ambiguously 38
.br;Test line for placing fig ambiguously 39
.br;Test line for placing fig ambiguously 40
:IREF refid = ih1_fig pg = major.
.pa
:P.Text line at top of page.
:FIG id = fred place = inline width = column.This :hp1.text:ehp1. follows FIG
This is test line one.
This is test line two.
This is a test of the processing of really long lines.    And this is some more text to make it a really long line.
This line is to extend the FIG vertically 1
This line is to extend the FIG vertically 2
This line is to extend the FIG vertically 3
:cmt.This line is to extend the FIG vertically 4
:cmt.This line is to extend the FIG vertically 5
:cmt.This line is to extend the FIG vertically 6
:cmt.This line is to extend the FIG vertically 7
:cmt.This line is to extend the FIG vertically 8
:cmt.This line is to extend the FIG vertically 9
:cmt.This line is to extend the FIG vertically 10
:FIGCAP. This is FIGCAP caption text. 
:FIGDESC. This is FIGDESC :hp1.text:ehp1. on the same line as the FIGDESC tag.
This is some more :hp1.figdesc:ehp1. text.
:eFIG.This text follows eFIG.
This is what FIGREF produces: :FIGREF refid = FRed. after the FIG.
:FIG id = sue place = inline width = column.This text follows FIG
This is test line one.
This is test line two.
:FIGCAP. This is FIGCAP caption text. 
:eFIG.This text follows eFIG.

:XMP.This :hp1.text:ehp1. follows FIG
This is test line one.
This is test line two.
This is a test of the processing of really long lines.      And this is some more text to make it a really long line.
This line is to extend the XMP vertically 1
This line is to extend the XMP vertically 2
This line is to extend the XMP vertically 3
:eXMP.This text follows eXMP.
:FIG place = inline width = column.This text follows FIG
This is test line one.
This is test line two.
:FIGCAP. This is FIGCAP caption text. 
:eFIG.This text follows eFIG.
:cmt.:I1. test after FIG declarations
.pa
This is a test of FIGREF on a different page from the FIG: :FIGREF refid = freD. 
on a new page
This is a test of FIGREF on a different page from the FIG: :FIGREF refid = freD page = no. 
on a new page
.ty after FIGREF  line
:H0 id = ted. Test header0
:P.some text (using tag P)
some text
:FIG.
3rd FIG body text
:FIGDESC.
:eFIG.
:P.some text (using tag P)
more text
:H1.Test header1
.pa
.ty before HDREF  line
This is what HDREF produces for a forward reference:
The first one: :HDREF refid = Sue page = yes. with more text after it.
.br
The second one: :HDREF refid = Sue page = no.
.br
The third one: :HDREF refid = Bill., -- for a different heading.
.sk 34
This is an HDREF in a very long line designed to test page changes: :HDREF refid = Sue. more text
:H2 positioning test.
:P.some text (using tag P)
some text between headings
:H2 id = Sue. Test header2 Number One
:P.some text (using tag P)
some text between headings, augmented with an awful lot of text to see what happens, and then with even more text
:H3. Sub-header one/one
:P.some text (using tag P)
some text between headings
:H3. Sub-header one/two
:P.some text (using tag P)
some text between headings
.br
this is an HDREF before the header: :HDREF refid = Bill.
.pa
.ix ixtest *
.ix ixtest two *
.ix ixtest two three *
:H2 id = Bill. Test header2
:P.some text (using tag P)
some text between headings
:H3. Sub-header two/one
:P.some text (using tag P)
some text between headings
:H3. Sub-header two/two
:P.some text (using tag P)
some text between headings
:H0. New top-level header
:cmt.:I2. test after H2 declarations
this is text immediately after the header
this is an HDREF after the header: :HDREF refid = Bill page = yes. which should show it!
this is an HDREF after the header: :HDREF refid = Bill. which should not!
.pa
this is an HDREF after the header: :HDREF refid = Bill. which should show it!
this is an HDREF after the header: :HDREF refid = Sue page = no. which should not!
.ty after HDREF  line
.*
:H0.Appendices
:APPENDIX.
:H1.First H1 header in APPENDIX.
.ix appendix
.ix appendix '*'
.ix appendix two '*'
.ix appendix two three '*'
This is some initial text in APPENDIX.
This line is to explore line spacing after the second fig 1
.br
This line is to explore line spacing after the second fig 2
.br
This line is to explore line spacing after the second fig 3
.br
This line is to explore line spacing after the second fig 4
.br
This line is to explore line spacing after the second fig 5
.br
This line is to explore line spacing after the second fig 6
.br
This line is to explore line spacing after the second fig 7
.br
This line is to explore line spacing after the second fig 8
.br
This line is to explore line spacing after the second fig 9
.br
This line is to explore line spacing after the second fig 10
.br
some text between headings
:H2.First H2 header in APPENDIX.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
.pa
:H2.Second H2 header in APPENDIX.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
:H1.Second H1 header in APPENDIX.
:P.some text (using tag P)
some text between headings
:H2.First H2 header in APPENDIX.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
:H2.Second H2 header in APPENDIX.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
:BACKM.
.ix backm
.ix backm *
.ix backm two *
.ix backm two three *
.if 1 = 0 .do begin
:H1.H1 header in BACKM.
This is some initial text in BACKM.
:FIG id = backm1 place = inline.
The first FIG in BACKM.
:FIGCAP.BACKM FIG 1.
:eFIG.Text after eFIG on same line
:FIG id = backm2 place = inline.
The second FIG in BACKM.
:FIGCAP.BACKM FIG 2.
:eFIG.Text after eFIG on same line
.sk 41
This is an HDREF in a very long line designed to test page changes: :HDREF refid = Sue2. more text
:H2 positioning test.
:P.some text (using tag P)
some text between headings
:H2 id = Sue2. Test header2 Number One, augmented with an awful lot of text to see what happens
:P.some text (using tag P)
some text between headings
:H2.First H2 header in BACKM.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
:H2.Second H2 header in BACKM.
:P.some text (using tag P)
some text between headings
:H3.First H3 header.
:P.some text (using tag P)
some text between headings
:H3.Second H3 header.
:P.some text (using tag P)
some text between headings
:cmt..if &'lower(&syspdev) NE whelp .do begin
.do end
:IREF refid = iaab
:IREF refid = iaab pg =  1a
:IREF refid = iaab pg = fred
:iref refid = i1_1.
:I1 pg = end. aab
Here is some text using tabs with fill chars:
.tb set $
.tb ./10r
Some$text
More$text
.tb
.tb set
.cd set 2
:INDEX.
:cmt.:INDEX ix = 2.
.cd set 1
:cmt..do end
.ty after last line/before eGDOC
.*
:eGDOC.
.if &'lower(&syspdev) EQ ps .do begin
:GRAPHIC file = 'acc4.ps' depth = 3i scale = 1000.
.do end
:BINCLUDE file = 'rule.eps' depth = 1i reposition = start.

