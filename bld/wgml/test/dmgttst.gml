:LAYOUT
   :PAGE
      depth = '9.66i' left_margin = '1i' right_margin = '7i' top_margin = 0
   :DEFAULT
      binding = 0 columns = 1 font = 0 gutter = '0.5i' input_esc = '~'
      justify = no spacing = 1
   :WIDOW
      threshold = 1
   :ABSTRACT
      abstract_string = 'ABSTRACT'
      columns = 1 font = 1 header = no page_eject = yes page_reset = yes
      pre_top_skip = 0 post_skip = 0 spacing = 1
   :BODY
      body_string = 'BODY'
      font = 1 header = no post_skip = 0 pre_top_skip = 0
      page_eject = yes page_reset = yes
   :PREFACE
      preface_string = 'PREFACE'
      columns = 1 font = 5 header = no page_eject = yes page_reset = yes
      pre_top_skip = 0 post_skip = 0 spacing = 1
   :P
      line_indent = '0.5i' post_skip = 0 pre_skip = 1
   :PC
      line_indent = 0 post_skip = 0 pre_skip = 0
   :NOTE
      font = 2 left_indent = 0 note_string = 'NOTE: ' pre_skip = 1 post_skip = 1
      right_indent = 0 spacing = 1
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
:FRONTM.
:PREFACE.
:BODY.
.co on
.ju off

.*
.dm @italbeg begin
.  :SF font=1.&*.
.dm @italbeg end
.*
.dm @italend begin
.  :eSF.&*.
.dm @italend end
.*
.gt ITAL  add @italbeg continue
.gt eITAL add @italend continue
.*
.gt ITALX  add @italbeg
.gt eITALX add @italend
.*

This is a test of :ITAL.the ITAL user tag:eITAL..
.br
This is a test of :ITALX.the ITALX user tag:eITALX..
.br
This is another test of
:ITAL.the ITAL user tag:eITAL..
.br
This is another test of
:ITALX.the ITALX user tag:eITALX..
.br
Here is yet another test of 
:ITAL.the ITAL user tag:eITAL.
.
.br
Here is yet another test of 
:ITALX.the ITALX user tag:eITALX.
.
.br
And here is test of the macros underlying
.@italbeg the ITAL user tag
.@italend .
.br
And here is the final test of the use:ITAL.r ta:eITAL.g ITAL!
.br
And here is the final test of the use:ITALX.r ta:eITALX.g ITALX!
.br
This compares user tag ITAL to the re:hp1.al t:ehp1.ag HT1!
.br

..sr firsttab=&SYSIN.+5
..tb set $
..tb &firsttab. +5 +5 +5 +5 +5 +5 +5 +5 +5 +5
.br
token
$$keyword
$or$identifier
$or$constant
$or$string-literal
$or$operator
$or$punctuator
.br
token
.ct $$keyword
$or$identifier
$or$constant
$or$string-literal
$or$operator
$or$punctuator
.br
:ITAL.token:eITAL.
$$keyword
$or$identifier
$or$constant
$or$string-literal
$or$operator
$or$punctuator
.br
:ITALX.token:eITALX.
$$keyword
$or$identifier
$or$constant
$or$string-literal
$or$operator
$or$punctuator
:eGDOC.

