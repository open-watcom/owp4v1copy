#
# WDISASM is dead, replaced by WDIS. We still have to keep the project on
# the 11.0 build machine for the time being because the source is still being
# used by the X86 MAD. That will change in 11.5.
#
#### # WDISASM Builder Control file
#### # ============================
####
#### set PROJDIR=<CWD>
####
#### [ INCLUDE <LANG_BLD>\master.ctl ]
#### [ INCLUDE <LANG_BLD>\wproj.ctl ]
#### [ LOG <LOGFNAME>.<LOGEXT> ]
####
#### cdsay .
####
#### [ BLOCK <1> build rel2 ]
####     pmake -d build -h
####     cd <PROJDIR>
####
#### [ BLOCK <1> rel2 cprel2 ]
#### #========================
#### <CPCMD> os2.x86\wdisasm.exe <relroot>\rel2\binp\
#### <CPCMD> dos.x86\wdisasm.exe <relroot>\rel2\binw\
#### <CPCMD> qnx.x86\wdisasm.qnx <relroot>\rel2\qnx\binq\wdisasm.
#### <CPCMD> nt.x86\wdisasm.exe <relroot>\rel2\binnt\
#### <CPCMD> nt.axp\wdisasm.exe <relroot>\rel2\axpnt\
####
#### [ BLOCK <1> clean ]
#### #==================
####     pmake -d build -h clean
