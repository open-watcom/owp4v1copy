# Debugger Trap Files Control file
# ================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#dos
    <CPCMD> <devdir>/trap/lcl/dos/dosr/dos.std/std.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/rsi/dos.trp/rsi.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/rsi/dos.srv/rsihelp.exp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/pls/dos.trp/pls.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/pls/dos.srv/plshelp.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/pls/dosped.srv/pedhelp.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/cw/dos.trp/cw.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/cw/dos.srv/cwhelp.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/net/dos.trp/net.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/net/dos.srv/netserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nmp/dos.trp/nmp.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nmp/dos.srv/nmpserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nov/dos.trp/nov.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nov/dos.srv/novserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/par/dos.trp/par.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/par/dos.srv/parserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/ser/dos.trp/ser.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/ser/dos.srv/serserv.exe <relroot>/rel2/binw/
#    <CPCMD> <devdir>/trap/tcp/dos.trp/tcp.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/tcp/dos.srv/tcpserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/vdm/dos.srv/vdmserv.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/win/dos.trp/win.trp <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/win/dos.srv/winserv.exe <relroot>/rel2/binw/
#nec98
    <CPCMD> <devdir>/trap/lcl/dos/dosr/nec.std/std.trp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/rsi/nec.trp/rsi.trp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/rsi/nec.srv/rsihelp.exp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/lcl/dos/dosx/pls/nec.trp/pls.trp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/net/dosnec.trp/net.trp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/net/dosnec.srv/netserv.exe <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/nov/dosnec.trp/nov.trp <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/nov/dosnec.srv/novserv.exe <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/vdm/dosnec.srv/vdmserv.exe <relroot>/rel2/n98bw/
    <CPCMD> <devdir>/trap/win/dosnec.srv/winserv.exe <relroot>/rel2/n98bw/
#os2v2
    <CPCMD> <devdir>/trap/lcl/os2v2/std/std.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/net/os2v2.trp/net.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/net/os2v2.srv/netserv.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/nmp/os2v2.trp/nmp.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/nmp/os2v2.srv/nmpserv.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/par/os2v2.trp/par.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/par/os2v2.srv/parserv.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/ser/os2v2.trp/ser.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/ser/os2v2.srv/serserv.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/tcp/os2v2.trp/tcp.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/tcp/os2v2.srv/tcpserv.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/vdm/os2v2.trp/vdm.d32 <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/lcl/os2v2/wvpmhelp/wdpmhook.dll <relroot>/rel2/binp/dll/
    <CPCMD> <devdir>/trap/lcl/os2v2/wvpmhelp/wdpmhelp.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/lcl/os2v2/splice/wdsplice.dll <relroot>/rel2/binp/dll/
    <CPCMD> <devdir>/trap/nmp/nmpbind/nmpbind.exe <relroot>/rel2/binp/
    <CPCMD> <devdir>/trap/par/os2v2.pio/wdio.dll <relroot>/rel2/binp/dll/
#os2
#    <CPCMD> <devdir>/trap/nov/os2.trp/nov.dll <relroot>/rel2/binp/dll/
#    <CPCMD> <devdir>/trap/nov/os2.srv/novserv1.exe <relroot>/rel2/binp/
#    <CPCMD> <devdir>/bin/binp/dll/???.dll <relroot>/rel2/binp/dll/
#    <CPCMD> <devdir>/bin/binp/dll/std??.dll <relroot>/rel2/binp/dll/
#    <CPCMD> <devdir>/bin/binp/dll/???serv.exe <relroot>/rel2/binp/dll/
#   <CPCMD> <devdir>/bin/binp/???serv1.exe <relroot>/rel2/binp/
#windows
    <CPCMD> <devdir>/trap/lcl/win/std/std.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/net/win.trp/net.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/net/win.srv/netservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nmp/win.trp/nmp.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nmp/win.srv/nmpservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nov/win.trp/nov.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/nov/win.srv/novservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/par/win.trp/par.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/par/win.srv/parservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/tcp/win.trp/tcp.dll <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/tcp/win.srv/tcpservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/vdm/win.srv/vdmservw.exe <relroot>/rel2/binw/
    <CPCMD> <devdir>/trap/lcl/win/int32/wint32.dll <relroot>/rel2/binw/
#nt
    <CPCMD> <devdir>/trap/lcl/nt/std/std.dll <relroot>/rel2/binnt/
#    <CPCMD> <devdir>/trap/nmp/nt.trp/nmp.dll <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/nov/nt.trp/nov.dll <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/nov/nt.srv/novserv.exe <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/par/nt.trp/par.dll <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/par/nt.srv/parserv.exe <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/ser/nt.trp/ser.dll <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/ser/nt.srv/serserv.exe <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/tcp/nt.trp/tcp.dll <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/tcp/nt.srv/tcpserv.exe <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/vdm/nt.trp/vdm.dll <relroot>/rel2/binnt/
#axp
#    <CPCMD> <devdir>/trap/lcl/nt/stdaxp/std.dll <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/trap/nov/ntaxp.trp/nov.dll <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/trap/nov/ntaxp.srv/novserv.exe <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/trap/tcp/ntaxp.trp/tcp.dll <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/trap/tcp/ntaxp.srv/tcpserv.exe <relroot>/rel2/axpnt/
#nlm
    <CPCMD> <devdir>/trap/nov/nlm3.srv/novserv3.nlm <relroot>/rel2/nlm/
    <CPCMD> <devdir>/trap/nov/nlm4.srv/novserv4.nlm <relroot>/rel2/nlm/
    <CPCMD> <devdir>/trap/par/nlm3.srv/parserv3.nlm <relroot>/rel2/nlm/
    <CPCMD> <devdir>/trap/par/nlm4.srv/parserv4.nlm <relroot>/rel2/nlm/
    <CPCMD> <devdir>/trap/ser/nlm3.srv/serserv3.nlm <relroot>/rel2/nlm/
    <CPCMD> <devdir>/trap/ser/nlm4.srv/serserv4.nlm <relroot>/rel2/nlm/
#qnx
#    <CPCMD> <devdir>/trap/lcl/qnx/pmd/pmd.trp <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/trap/lcl/qnx/std/std.trp <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/trap/par/qnx.trp/par.trp <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/trap/par/qnx.srv/parserv.qnx <relroot>/rel2/qnx/binq/parserv
#    <CPCMD> <devdir>/trap/ser/qnx.trp/ser.trp <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/trap/ser/qnx.srv/serserv.qnx <relroot>/rel2/qnx/binq/serserv
#    <CPCMD> <devdir>/trap/tcp/qnx.trp/tcp.trp <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/trap/tcp/qnx.srv/tcpserv.qnx <relroot>/rel2/qnx/binq/tcpserv
# NT parallel port device driver and installer
    <CPCMD> <devdir>/trap/par/ntsupp/dbgport.sys <relroot>/rel2/binnt/
    <CPCMD> <devdir>/trap/par/ntsupp/dbginst.exe <relroot>/rel2/binnt/
#linux
    <CPCMD> <devdir>/trap/lcl/linux/std/std.trp <relroot>/rel2/binl/
    <CPCMD> <devdir>/trap/par/linux.trp/par.trp <relroot>/rel2/binl/
    <CPCMD> <devdir>/trap/par/linux.srv/parserv.elf <relroot>/rel2/binl/parserv
    <CPCMD> <devdir>/trap/tcp/linux.trp/tcp.trp <relroot>/rel2/binl/
    <CPCMD> <devdir>/trap/tcp/linux.srv/tcpserv.elf <relroot>/rel2/binl/tcpserv

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
