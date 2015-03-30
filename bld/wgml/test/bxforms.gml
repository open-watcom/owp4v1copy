:LAYOUT
   :PAGE
      depth = '9.66i' left_margin = '1i' right_margin = '7i' top_margin = 0
   :DEFAULT
      binding = 0 columns = 1 font = 0 gutter = '0.5i' input_esc = '~'
      justify = no spacing = 1
   :WIDOW
      threshold = 1
   :BODY
      body_string = ''
      font = 1 header = no post_skip = 0 pre_top_skip = 0
      page_eject = yes page_reset = yes
   :BANNER
      docsect = body place = bottom
   :eBANNER
:eLAYOUT.
:GDOC.
:BODY.
.co on
.ju off

.bx 20 40
.bx new 1 26
.bx off
.bx off

BX default operand tests

.bx 1 26 
.bx 32 62
.bx off

.bx 32 62
.bx 1 26 
.bx off

.bx 1 26
.bx 20 40
.bx off

.bx 20 40
.bx 1 26
.bx off

.bx 1 26
.bx 26 40
.bx off

.bx 26 40
.bx 1 26
.bx off

.bx 1 / 10 15 / 40
.bx 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx 1 / 10 15 / 40
.bx off

.bx 1 / 10 25 / 40
.bx 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx 1 / 10 25 / 40
.bx off

.bx 1 / 10 15 / 30 33 / 40
.bx 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx 1 / 10 15 / 30 33 / 40
.bx off

.bx 1 / 10 15 / 22 28 / 33 37 / 45
.bx 8 12 / 20 25 / 35 40
.bx off

.bx 8 12 / 20 25 / 35 40
.bx 1 / 10 15 / 22 28 / 33 37 / 45
.bx off

.bx 1 / 10 15 / 22 37 / 45 50 / 55
.bx 20 25 / 35 40
.bx off

.bx 20 25 / 35 40
.bx 1 / 10 15 / 22 37 / 45 50 / 55
.bx off

.bx 1 / 10 15 / 22 37 / 45 50
.bx 20 25 / 35 40 / 55
.bx off

.bx 20 25 / 35 40 / 55
.bx 1 / 10 15 / 22 37 / 45 50
.bx off

.pa
BX ON tests

.bx 1 26 
.bx on 32 62
.bx off

.bx 32 62
.bx on 1 26 
.bx off

.bx 1 26
.bx on 20 40
.bx off

.bx 20 40
.bx on 1 26
.bx off

.bx 1 26
.bx on 26 40
.bx off

.bx 26 40
.bx on 1 26
.bx off

.bx 1 / 10 15 / 40
.bx on 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx on 1 / 10 15 / 40
.bx off

.bx 1 / 10 25 / 40
.bx on 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx on 1 / 10 25 / 40
.bx off

.bx 1 / 10 15 / 30 33 / 40
.bx on 20 25 / 30 35
.bx off

.bx 20 25 / 30 35
.bx on 1 / 10 15 / 30 33 / 40
.bx off

.bx 1 / 10 15 / 22 28 / 33 37 / 45
.bx on 8 12 / 20 25 / 35 40
.bx off

.bx 8 12 / 20 25 / 35 40
.bx on 1 / 10 15 / 22 28 / 33 37 / 45
.bx off

.bx 1 / 10 15 / 22 37 / 45 50 / 55
.bx on 20 25 / 35 40
.bx off

.bx 20 25 / 35 40
.bx on 1 / 10 15 / 22 37 / 45 50 / 55
.bx off

.bx 1 / 10 15 / 22 37 / 45 50
.bx on 20 25 / 35 40 / 55
.bx off

.bx 20 25 / 35 40 / 55
.bx on 1 / 10 15 / 22 37 / 45 50
.bx off

.pa
BX OFF tests

.bx 1 26 
.bx off 32 62

.bx 32 62
.bx off 1 26 

.bx 1 26
.bx off 20 40

.bx 20 40
.bx off 1 26

.bx 1 26
.bx off 26 40

.bx 26 40
.bx off 1 26

.bx 1 / 10 15 / 40
.bx off 20 25 / 30 35

.bx 20 25 / 30 35
.bx off 1 / 10 15 / 40

.bx 1 / 10 25 / 40
.bx off 20 25 / 30 35

.bx 20 25 / 30 35
.bx off 1 / 10 25 / 40

.bx 1 / 10 15 / 30 33 / 40 
.bx off 20 25 / 30 35

.bx 20 25 / 30 35
.bx off 1 / 10 15 / 30 33 / 40

.bx 1 / 5 / 10 15 / 18 / 23 / 30 33 / 40 / 43 45 / 50 55
.bx off 20 25 / 30 35

.bx 20 25 / 30 35
.bx off 1 / 5 / 10 15 / 18 / 23 / 30 33 / 40 / 43  45 / 50 55

.bx 1 / 10 15 / 22 28 / 33 37 / 45
.bx off 8 12 / 20 25 / 35 40

.bx 8 12 / 20 25 / 35 40
.bx off 1 / 10 15 / 22 28 / 33 37 / 45

.bx 1 / 10 15 / 22 37 / 45 50 / 55
.bx off 20 25 / 35 40

.bx 20 25 / 35 40
.bx off 1 / 10 15 / 22 37 / 45 50 / 55

.bx 1 / 10 15 / 22 37 / 45 50
.bx off 20 25 / 35 40 / 55

.bx 20 25 / 35 40 / 55
.bx off 1 / 10 15 / 22 37 / 45 50

.bx 1 10 / 15 26 / 45 50 / 55
.bx off 20 40

.bx 20 40
.bx off 1 10 / 15 26 / 45 50 / 55

.bx 1 / 5 / 10 15 / 18 / 23 / 30 33 / 40 / 43
.bx off 25 30 / 35 40

.bx 25 30 / 35 40
.bx off 1 / 5 / 10 15 / 18 / 23 / 30 33 / 40 / 43

.pa
BX NEW tests

.bx 1 26 
.bx new 32 62
.bx off
.bx off

.bx 32 62
.bx new 1 26 
.bx off
.bx off

.bx 1 26
.bx new 20 40
.bx off
.bx off

.bx 20 40
.bx new 1 26
.bx off
.bx off

.bx 1 26
.bx new 26 40
.bx off
.bx off

.bx 26 40
.bx new 1 26
.bx off
.bx off

.bx 1 40
.bx new 20 30
.bx off
.bx off

.bx 20 30
.bx new 1 40
.bx off
.bx off

.bx 1 10 15 20 30
.bx new 10 20
.bx off
.bx off

.bx 1 10 15 20 30
.bx new 8 12 / 18 22
.bx off
.bx off

.bx 1 10 15 20 30
.bx new 10 20
.bx 12 18
.bx off
.bx off

.bx 1 10 15 20 30
.bx new 10 20
.bx 8 16 / 18 22
.bx off
.bx off

.bx 1 10 15 20 30
.bx new 4 8 / 14 18
.bx off
.bx off

.pa
BX CAN tests in a complex box
.tb set \
.tb 4 15 27 39 51
.*
\\text bx 2 13 49
.bx 2 13 49
\\text bx new 13 25 37 49
.bx new 13 25 37 49
\\text bx new 25 / 37 49 (1)
.bx new 25 / 37 49
\\text bx off 25 37 49
.bx off 25 37 49
\\text bx new 25 / 37 49 (2)
.bx new 25 / 37 49
\\text bx 13 49
.bx 13 49
\\text bx can (1)
.bx can
\\text bx can (2)
.bx can
\\text bx can (3)
.bx can
\\text off 2 13 49
.bx off 2 13 49
.sk
.*
OFF/CAN reversed
\\text bx 2 13 49
.bx 2 13 49
\\text bx new 13 25 37 49
.bx new 13 25 37 49
\\text bx new 25 / 37 49 (1)
.bx new 25 / 37 49
\\text bx off 25 37 49
.bx off 25 37 49
\\text bx new 25 / 37 49 (2)
.bx new 25 / 37 49
\\text bx 13 49
.bx 13 49
\\text bx off (1)
.bx off
\\text bx off (2)
.bx off
\\text bx off (3)
.bx off
\\text can
.bx can
.pa
BX CAN tests in a shorter complex box
.tb set \
.tb 4 15 27 39 51
.*
\\text bx 2 13 49
.bx 2 13 49
\\text bx new 13 25 37 49
.bx new 13 25 37 49
\\text bx new 25 / 37 49 (1)
.bx new 25 / 37 49
\\text bx off 25 37 49
.bx off 25 37 49
\\text bx new 25 / 37 49 (2)
.bx new 25 / 37 49
\\text bx 13 49
.bx 13 49
\\text bx can (1)
.bx can
.bx can
.bx can
.bx off 2 13 49
.sk
.*
OFF/CAN reversed
\\text bx 2 13 49
.bx 2 13 49
\\text bx new 13 25 37 49
.bx new 13 25 37 49
\\text bx new 25 / 37 49 (1)
.bx new 25 / 37 49
\\text bx off 25 37 49
.bx off 25 37 49
\\text bx new 25 / 37 49 (2)
.bx new 25 / 37 49
\\text bx 13 49
.bx 13 49
\\text bx off (1)
.bx off
.bx off
.bx off
.bx can
:eGDOC.

