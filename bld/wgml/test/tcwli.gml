.* test for .li controlword
.* co off for one line in = one line out
.co off
.se var=outside li
.li 1
.se var=inside li
.li .ty &var.
&var. not within li
.li on
.li xxxx in li
&var line1 in .li  :cmt xxxyyy
&var line2 in .li
.li off
.li off;:cmt duplicate li off is accepted without errmsg
.li on
.li on;.* duplicate li on is treated as text
.li off
.li off
.se var=varvalueoff
.li
.ty &var
