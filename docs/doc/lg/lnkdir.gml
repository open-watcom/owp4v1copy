.*
.*
.dm dir begin
.sr *$$sys=''
.se *$$fnd=&'wordpos('dos',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., DOS
.do end
.se *$$fnd=&'wordpos('elf',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., ELF
.do end
.se *$$fnd=&'wordpos('netware',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., NetWare
.do end
.se *$$fnd=&'wordpos('os2',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., OS/2
.do end
.se *$$fnd=&'wordpos('pharlap',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., PharLap
.do end
.se *$$fnd=&'wordpos('qnx',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., QNX
.do end
.se *$$fnd=&'wordpos('win16',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., Win16
.do end
.se *$$fnd=&'wordpos('win32',&*,3)
.if &*$$fnd. ne 0 .do begin
.   .sr *$$sys=&*$$sys., Win32
.do end
.sr *$$ban=&'upper(&*1)
.sr suppsys='All'
.if '&*$$sys' ne '' .do begin
.sr *$$ban=&*$$ban. (&'substr(&*$$sys.,3))
.sr suppsys=&'substr(&*$$sys.,3)
.do end
.if '&target' eq 'QNX' .do begin
.   .se *$$fnd=&'wordpos('QNX',&suppsys)
.   .if &*$$fnd. ne 0 or '&suppsys' eq 'All' .do begin
.   .   .pa
.   .   .se headtxt0$=&*$$ban
.   .   .se headtxt1$=&*$$ban
.   .   .im &*2
.   .do end
.do end
.el .do begin
.   .pa
.   .se headtxt0$=&*$$ban
.   .se headtxt1$=&*$$ban
.   .im &*2
.do end
.dm dir end
.*
.dm dirctv begin
.sr *$$rfs=&'left(&*,6)
.if "&*$$rfs" eq "#" .do begin
.sr *$$rfs='commen'
.do end
.el .if "&*$$rfs" eq "@" .do begin
.sr *$$rfs='includ'
.do end
.section *refid=x&*$$rfs. The &* Directive
.if '&target' ne 'QNX' .do begin
.np
:SF font=9.Formats: &suppsys:eSF.
.do end
.ix '&* directive'
.ix 'directives' '&*'
.dm dirctv end
.*
.dm option begin
.sr *$$rfs=&'left(&*,6)
.section *refid=x&*$$rfs. The &* Option
.if '&target' ne 'QNX' .do begin
.np
:SF font=9.Formats: &suppsys:eSF.
.do end
.ix '&* option'
.ix 'options' '&*'
.dm option end
.*
.* The directives below that are commented out are "overlay" related.
.*
.dir alias              ldalias.gml     all
.dir alignment          opalignm.gml    elf os2 win16 win32
.dir anonymousexport    ldanonym.gml    win16 win32
.*   area               oparea.gml      dos
.dir artificial         opartifi.gml    all
.*   autosection        ldautose.gml    dos
.dir autounload         opautoun.gml    netware
.*   begin              ldbegin.gml     dos
.dir cache              opcache.gml     all
.dir caseexact          opcaseex.gml    all
.dir check              opcheck.gml     netware
.dir comment            ldcmt.gml       all
.dir commit             ldcommit.gml    win32
.dir copyright          opcopyri.gml    netware
.dir custom             opcustom.gml    netware
.dir cvpack             opcvpack.gml    all
.dir debug              lddebug.gml     all
.dir description        opdescri.gml    os2 win16 win32
.dir disable            lddisabl.gml    all
.*   distribute         opdistri.gml    dos
.dir dosseg             opdosseg.gml    all
.*   dynamic            opdynami.gml    dos
.dir eliminate          opelimin.gml    all
.*   end                ldend.gml       dos
.dir endlink            ldendlin.gml    all
.dir exit               opexit.gml      netware
.dir export             ldexport.gml    elf netware os2 win16 win32
.dir file               ldfile.gml      all
.*   fixedlib           ldfixedl.gml    dos
.*   forcevector        ldforcev.gml    dos
.dir format             ldformat.gml    all
.dir heapsize           opheap.gml      os2 qnx win16 win32
.dir help               ophelp.gml      netware
.dir impfile            opimpfil.gml    netware os2 win16 win32
.dir implib             opimplib.gml    netware os2 win16 win32
.dir import             ldimport.gml    elf netware os2 win16 win32
.dir include            ldinclud.gml    all
.dir incremental        opincrem.gml    elf os2 pharlap qnx win16 win32
.dir internalrelocs     opintern.gml    os2
.dir language           ldlangua.gml    all
.dir libfile            ldlibfil.gml    all
.dir libpath            ldlibpat.gml    all
.dir library            ldlibrar.gml    all
.dir linearrelocs       oplinear.gml    qnx
.dir longlived          oplongli.gml    qnx
.dir manglednames       opmangle.gml    all
.dir manyautodata       opmanyau.gml    os2 win16
.dir map                opmap.gml       all
.dir maxdata            opmaxdat.gml    pharlap
.dir maxerrors          opmaxerr.gml    all
.dir messages           opmessag.gml    netware
.dir mindata            opmindat.gml    pharlap
.dir modname            opmodnam.gml    os2 win16 win32
.dir modfile            ldmodfil.gml    all
.dir modtrace           ldmodtrc.gml    all
.dir module             ldmodule.gml    elf netware
.dir multiload          opmultil.gml    netware
.dir name               ldname.gml      all
.dir namelen            opnamele.gml    all
.dir newfiles           opnewfil.gml    os2
.dir newsegment         ldnewseg.gml    dos os2 qnx win16
.dir nlmflags           opnlmfla.gml    netware
.dir noautodata         opnoauto.gml    os2 win16
.dir nodefaultlibs      opnodefa.gml    all
.*   noindirect         opnoindi.gml    dos
.dir norelocs           opnorelo.gml    qnx win32
.dir nostdcall          opnostdc.gml    win32
.*   novector           ldnovect.gml    dos
.dir objalign           opobjali.gml    win32
.dir oldlibrary         opoldlib.gml    os2 win16 win32
.dir offset             opoffset.gml    os2 pharlap qnx win32
.dir oneautodata        oponeaut.gml    os2 win16
.dir option             ldoption.gml    all
.dir optlib             ldoptlib.gml    all
.dir osdomain           oposdoma.gml    netware
.dir pseudopreemption   oppseudo.gml    netware
.dir osname             oposname.gml    all
.*   overlay            ldoverla.gml    dos
.dir packcode           oppackco.gml    dos os2 qnx win16
.dir packdata           oppackda.gml    dos os2 qnx win16
.dir path               ldpath.gml      all
.dir privilege          opprivil.gml    qnx
.dir protmode           opprotmo.gml    os2
.dir quiet              opquiet.gml     all
.dir redefsok           opredefs.gml    all
.dir reentrant          opreentr.gml    netware
.dir reference          ldrefere.gml    all
.dir resource           opresour.gml    os2 qnx win16 win32
.dir runtime            ldruntim.gml    pharlap win32
.dir rwreloccheck       oprwrelo.gml    win16
.dir screenname         opscreen.gml    netware
.*   section            ldsectio.gml    dos
.dir segment            ldsegmen.gml    os2 qnx win16 win32
.dir sharelib           opsharel.gml    netware
.dir showdead           opshowde.gml    all
.*   small              opsmall.gml     dos
.dir sort               ldsort.gml      all
.dir stack              opstack.gml     all
.*   standard           opstanda.gml    dos
.dir start              opstart.gml     all
.dir startlink          ldstartl.gml    all
.dir statics            opstatic.gml    all
.dir stub               opstub.gml      os2 win16 win32
.dir symfile            opsymfil.gml    all
.dir symtrace           ldsymtrc.gml    all
.dir synchronize        opsynchr.gml    netware
.dir system             ldsystem.gml    all
.dir threadname         opthread.gml    netware
.dir togglerelocs       optoggle.gml    os2
.dir undefsok           opundefs.gml    all
.*   vector             ldvector.gml    dos
.dir verbose            opverbos.gml    all
.dir version            opversio.gml    netware os2 win16 win32
.dir vfremoval          opvfremo.gml    all
.dir xdcdata            opxdcdat.gml    netware
.*
:set symbol="headtxt0$" value="The Open Watcom Linker".
.*
