echo %1 ================================================================
set cmplr=%2
if exist %1.c rm %1.c
vi -d -q -k ":r %1.asm\n:so toc.vi\n:0r cinit.vi\n:w %1.c\n:q!\n"
%cmplr% %1 -s -ot -zm -zq
wdis -a -l %1
vi -d -q -k ":so fixlst.vi\nZZ" %1.lst
reverse %1.lst
diff -b %1.lst %1.asm
rem cat %1.lst >> %3
set cmplr=
