# Debugger Trap Files Control file
# ================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/dos/dosr/dos.std/std.trp              <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/lcl/dos/dosx/rsi/dos.trp/rsi.trp          <RELROOT>/rel2/binw/
    <CCCMD> <DEVDIR>/trap/lcl/dos/dosx/rsi/dos.srv/rsihelp.exp      <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/lcl/dos/dosx/pls/dos.trp/pls.trp          <RELROOT>/rel2/binw/
    <CCCMD> <DEVDIR>/trap/lcl/dos/dosx/pls/dos.srv/plshelp.exp      <RELROOT>/rel2/binw/
    <CCCMD> <DEVDIR>/trap/lcl/dos/dosx/pls/dosped.srv/pedhelp.exp   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/lcl/dos/dosx/cw/dos.trp/cw.trp            <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/lcl/dos/dosx/cw/dos.srv/cwhelp.exe        <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/net/dos.trp/net.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/net/dos.srv/netserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nmp/dos.trp/nmp.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nmp/dos.srv/nmpserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nov/dos.trp/nov.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nov/dos.srv/novserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/par/dos.trp/par.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/par/dos.srv/parserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/ser/dos.trp/ser.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/ser/dos.srv/serserv.exe                   <RELROOT>/rel2/binw/
#    <CPCMD> <DEVDIR>/trap/tcp/dos.trp/tcp.trp                      <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/tcp/dos.srv/tcpserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/vdm/dos.srv/vdmserv.exe                   <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/win/dos.trp/win.trp                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/win/dos.srv/winserv.exe                   <RELROOT>/rel2/binw/

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/os2v2/std/std.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/net/os2v2.trp/net.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/net/os2v2.srv/netserv.exe                 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/nmp/os2v2.trp/nmp.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/nmp/os2v2.srv/nmpserv.exe                 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/par/os2v2.trp/par.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/par/os2v2.srv/parserv.exe                 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/ser/os2v2.trp/ser.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/ser/os2v2.srv/serserv.exe                 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/tcp/os2v2.trp/tcp.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/tcp/os2v2.srv/tcpserv.exe                 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/vdm/os2v2.trp/vdm.d32                     <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/lcl/os2v2/wvpmhelp/wdpmhook.dll           <RELROOT>/rel2/binp/dll/
    <CPCMD> <DEVDIR>/trap/lcl/os2v2/wvpmhelp/wdpmhelp.exe           <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/lcl/os2v2/splice/wdsplice.dll             <RELROOT>/rel2/binp/dll/
    <CPCMD> <DEVDIR>/trap/nmp/nmpbind/nmpbind.exe                   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/trap/par/os2v2.pio/wdio.dll                    <RELROOT>/rel2/binp/dll/
#os2
#    <CPCMD> <DEVDIR>/trap/nov/os2.trp/nov.dll                      <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/trap/nov/os2.srv/novserv1.exe                 <RELROOT>/rel2/binp/
#    <CPCMD> <DEVDIR>/bin/binp/dll/???.dll                          <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/bin/binp/dll/std??.dll                        <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/bin/binp/dll/???serv.exe                      <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/bin/binp/???serv1.exe                         <RELROOT>/rel2/binp/

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/win/std/std.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/net/win.trp/net.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/net/win.srv/netservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nmp/win.trp/nmp.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nmp/win.srv/nmpservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nov/win.trp/nov.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/nov/win.srv/novservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/par/win.trp/par.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/par/win.srv/parservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/tcp/win.trp/tcp.dll                       <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/tcp/win.srv/tcpservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/vdm/win.srv/vdmservw.exe                  <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/trap/lcl/win/int32/wint32.dll                  <RELROOT>/rel2/binw/

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/nt/std/std.dll                        <RELROOT>/rel2/binnt/
#    <CPCMD> <DEVDIR>/trap/nmp/nt.trp/nmp.dll                       <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/nov/nt.trp/nov.dll                        <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/nov/nt.srv/novserv.exe                    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/par/nt.trp/par.dll                        <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/par/nt.srv/parserv.exe                    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/ser/nt.trp/ser.dll                        <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/ser/nt.srv/serserv.exe                    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/tcp/nt.trp/tcp.dll                        <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/tcp/nt.srv/tcpserv.exe                    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/vdm/nt.trp/vdm.dll                        <RELROOT>/rel2/binnt/
# NT parallel port device driver and installer
    <CPCMD> <DEVDIR>/trap/par/ntsupp/dbgport.sys                    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/trap/par/ntsupp/dbginst.exe                    <RELROOT>/rel2/binnt/

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/nt/stdaxp/std.dll                     <RELROOT>/rel2/axpnt/
    <CPCMD> <DEVDIR>/trap/nov/ntaxp.trp/nov.dll                     <RELROOT>/rel2/axpnt/
    <CPCMD> <DEVDIR>/trap/nov/ntaxp.srv/novserv.exe                 <RELROOT>/rel2/axpnt/
    <CPCMD> <DEVDIR>/trap/tcp/ntaxp.trp/tcp.dll                     <RELROOT>/rel2/axpnt/
    <CPCMD> <DEVDIR>/trap/tcp/ntaxp.srv/tcpserv.exe                 <RELROOT>/rel2/axpnt/

  [ IFDEF (os_nov "") <2*> ]
    <CPCMD> <DEVDIR>/trap/nov/nlm3.srv/novserv3.nlm                 <RELROOT>/rel2/nlm/
    <CPCMD> <DEVDIR>/trap/nov/nlm4.srv/novserv4.nlm                 <RELROOT>/rel2/nlm/
    <CPCMD> <DEVDIR>/trap/par/nlm3.srv/parserv3.nlm                 <RELROOT>/rel2/nlm/
    <CPCMD> <DEVDIR>/trap/par/nlm4.srv/parserv4.nlm                 <RELROOT>/rel2/nlm/
    <CPCMD> <DEVDIR>/trap/ser/nlm3.srv/serserv3.nlm                 <RELROOT>/rel2/nlm/
    <CPCMD> <DEVDIR>/trap/ser/nlm4.srv/serserv4.nlm                 <RELROOT>/rel2/nlm/

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/qnx/pmd/pmd.trp                       <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/trap/lcl/qnx/std/std.trp                       <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/trap/par/qnx.trp/par.trp                       <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/trap/par/qnx.srv/parserv.qnx                   <RELROOT>/rel2/qnx/binq/parserv
    <CPCMD> <DEVDIR>/trap/ser/qnx.trp/ser.trp                       <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/trap/ser/qnx.srv/serserv.qnx                   <RELROOT>/rel2/qnx/binq/serserv
    <CPCMD> <DEVDIR>/trap/tcp/qnx.trp/tcp.trp                       <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/trap/tcp/qnx.srv/tcpserv.qnx                   <RELROOT>/rel2/qnx/binq/tcpserv

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <DEVDIR>/trap/lcl/linux/std/std.trp                     <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/trap/par/linux.trp/par.trp                     <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/trap/par/linux.srv/parserv.elf                 <RELROOT>/rel2/binl/parserv
    <CPCMD> <DEVDIR>/trap/tcp/linux.trp/tcp.trp                     <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/trap/tcp/linux.srv/tcpserv.elf                 <RELROOT>/rel2/binl/tcpserv

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
