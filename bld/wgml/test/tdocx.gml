.* test document with some sections changes
.* later test all sections
.se fil=&'left(&sysfnam,&'pos('.',&sysfnam)-1)
:layout
:abstract
        header = yes
:BANNER
        left_adjust = 0
        right_adjust = 0
        depth = 4
        place = topodd
        docsect = abstract
:BANREGION
        indent = 0
        hoffset = left
        width = extend
        voffset = 3
        depth = 1
        font = 0
        refnum = 1
        region_position = centre
        pouring = last
        script_format = yes
        contents = '//Abstract ODD &$amp.$pgnumr./'
:eBANREGION
:eBANNER
:BANNER
        left_adjust = 0
        right_adjust = 0
        depth = 4
        place = topeven
        docsect = abstract
:BANREGION
        indent = 0
        hoffset = left
        width = extend
        voffset = 3
        depth = 1
        font = 0
        refnum = 1
        region_position = centre
        pouring = last
        script_format = yes
        contents = '//Abstract EVEN &$amp.$pgnumr./'
:eBANREGION
:eBANNER
:BANNER
        left_adjust = 0
        right_adjust = 0
        depth = 1
        place = top
        docsect = body
:BANREGION
        indent = 0
        hoffset = left
        width = extend
        voffset = 0
        depth = 1
        font = 2
        refnum = 1
        region_position = left
        pouring = last
        script_format = yes
        contents = '/&$amp.date./-top body &$amp.$htext1. b-/ Page &$amp.$pgnuma./'
:eBANREGION
:eBANNER
:BANNER
        left_adjust = 0
        right_adjust = 0
        depth = 1
        place = bottom
        docsect = body
:BANREGION
        indent = 0
        hoffset = left
        width = extend
        voffset = 0
        depth = 1
        font = 0
        refnum = 1
        region_position = left
        pouring = last
        script_format = yes
        contents = '/&$amp.$htext1./bottom body/ &$amp.$pgnuma./'
:eBANREGION
:eBANNER
:convert file="&fil..lay".
:elayout
:gdoc
:frontm
:titlep
:title stitle='sect doc'.Section Test Document
:title stitle='sect doc2'.Title line 2
.ty title sysin=&$in  sysinr=&$inr sysir=&$ir sysll=&$ll lm=&$pagelm rm=&$pagerm
.ty title sysin=&$in  sysinr=&$inr sysir=&$ir sysll=&$ll  syslc=&$lc syslst=&$lst
:docnum.1234567
:date.Date was 25.08.2010
:author.First Author
:author.My Second Personality
:address.
:aline.address 1 1
:aline.address 1 2
:eaddress.
:address.
:aline.address 2 1
:aline.address 2 2
:eaddress.
:address.
:eaddress.
:etitlep
:abstract
:p.Text in abstract.
.pe 100
        Text in Abstract to create a page overflow to see wgml4.0 page overflow.
:body
:cmt. :h1 id='meinlangernamealsId'.Is was?
:p.
:hp1.HP1 text in input line one.:ehp1.
:hp2.HP2 text:ehp2.
:hp3.HP3 text:ehp3.
.br
.us us text us
no more us text
.br
More text in another line two
More text in another line three
More text in another line four
.pa nostart
More text in another line five
.pa odd
More text in another line six
.pa odd
:hp1.HP1 text in input line.:ehp1.
More text in another line six
More text in another line seven
More text in another line eight
More text in another line nine
.br
.se pe=0
.pe 100
.se pe=&pe.+1;   (&pe.). Performed line with a count of 100.
.cp 18
.br
After cp
:backm
:egdoc
