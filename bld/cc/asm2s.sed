s/;/#/g
s/_loop/0:   /g
s/db[ ]*/.asciz /g
s/^_//g
s/_quif e /je    1f/g
s/public  _/.global /g
s/ _/  /g
s/.asciz 0/.asciz ""/g
s/.asciz \([A-Za-z]\)/.byte \1/g
s/include.*/.intel_syntax noprefix/
s/,0\([^a-zA-Z]*\)$/\1/g
s/^.*assume.*$//g
s/^.*286p.*$/.code16/g
s/^.*386.*$//g
s/^[ ]*name.*$//g
s/^DATA.*$//g
s/^DGROUP.*$//g
s/^\(.*\)macro/.macro \1/g
s/ endm/ .endm/g
s/^[ ]*end//g
s/ dw / .long /g
s/ dd / .long /g
s/[[:cntrl:]]//g
s/until  e/jne   0b\
        1:/g
s/func[ ]*\([A-Za-z_]*\)/.long \1_defs,\1_name,\1/g
