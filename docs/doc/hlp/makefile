suppress_just_enough=1

!ifeq hbook foo
book_title=foo
book_name=foo
book_incs=..\..\cmn;..\..\gml;..\..
book_winhc=whc

!else ifeq hbook devguide
book_title=Open Watcom Developer's Guide
book_name=devguide
book_incs=..\..\devguide;..\..\cmn;..\..\gml

!else ifeq hbook cbooks
book_title=Watcom C/C++ On-line Documentation
book_name=cbooks
book_incs=..\..\gs;..\..\gml

!else ifeq hbook fbooks
book_title=Watcom FORTRAN 77 On-line Documentation
book_name=fbooks
book_incs=..\..\gs;..\..\gml

!else ifeq hbook ebooks
book_title=Watcom Editor On-line Documentation
book_name=ebooks
book_incs=..\..\gs;..\..\gml

!else ifeq hbook isv
book_title=Sybase Partners Program
book_name=isv
book_incs=..\..\isv;..\..\gml

!else ifeq hbook isvcpp
book_title=Watcom C/C++ 11.0 Add-In Tools Guide
book_name=isvcpp
book_incs=..\..\isv;..\..\gml
multi_chap=true

!else ifeq hbook isvfor
book_title=Watcom F77 11.0 Add-In Tools Guide
book_name=isvfor
book_incs=..\..\isv;..\..\gml
multi_chap=true

!else ifeq hbook c_readme
book_title=Watcom C/C++ Getting Started
book_name=c_readme
book_incs=..\..\gs;..\..\cg;..\..\gml
book_winhc=hc31
multi_chap=true

!else ifeq hbook f_readme
book_title=Watcom FORTRAN 77 Getting Started
book_name=f_readme
book_incs=..\..\gs;..\..\gml
book_winhc=hc31
multi_chap=true

!else ifeq hbook tools
book_title=Watcom C/C++ Tools Guide
book_name=tools
book_incs=..\..\cmn;..\..\lg;..\..\cg;..\..\gml
multi_chap=true

!else ifeq hbook ftools
book_title=Watcom FORTRAN 77 Tools Guide
book_name=ftools
book_incs=..\..\cmn;..\..\lg;..\..\fg;..\..\gml
multi_chap=true

!else ifeq hbook lguide
book_title=Watcom Linker Guide
book_name=lguide
book_incs=..\..\lg;..\..\gml;..\..
multi_chap=true

!else ifeq hbook cguide
book_title=Watcom C/C++ User's Guide
book_name=cguide
book_incs=..\..\cg;..\..\lg;..\..\cmn;..\..\plusplus;..\..\gml;..\..
multi_chap=true

!else ifeq hbook cguideq
book_title=Watcom C/C++ User's Guide for QNX
book_name=cguideq
book_incs=..\..\cg;..\..\lg;..\..\cmn;..\..\plusplus;..\..\gml;..\..
multi_chap=true

!else ifeq hbook clr
book_title=Watcom C Language Reference
book_name=c
book_incs=..\..\cl;..\..\gml;..\..
multi_chap=true

!else ifeq hbook fuguide
book_title=Watcom FORTRAN 77 User's Guide
book_name=fuguide
book_incs=..\..\fg;..\..\cg;..\..\lg;..\..\cmn;..\..\gml;..\..
multi_chap=true

!else ifeq hbook wfcerrs
book_title=Watcom FORTRAN 77 Diagnostic Messages
book_name=wfcerrs
book_incs=..\..\fg;..\..\gml;..\..
book_os2_help=$(hbook).hlp

!else ifeq hbook wccerrs
book_title=Watcom C Diagnostic Messages
book_name=wccerrs
book_incs=..\..\cg;..\..\plusplus;..\..\gml;..\..
book_os2_help=$(hbook).hlp

!else ifeq hbook wpperrs
book_title=Watcom C++ Diagnostic Messages
book_name=wpperrs
book_incs=..\..\cg;..\..\plusplus;..\..\gml;..\..
book_os2_help=$(hbook).hlp

!else ifeq hbook wjcerrs
book_title=Sybase Java Compiler Diagnostic Messages
book_name=wjcerrs
book_incs=..\..\java;..\..\gml;..\..
book_os2_help=$(hbook).hlp


!else ifeq hbook wlnkerrs
book_title=Watcom Linker Diagnostic Messages
book_name=wlinkerr
book_incs=..\..\lg;..\..\gml;..\..
book_os2_help=$(hbook).hlp

!else ifeq hbook wrcerrs
book_title=Watcom Resource Compiler Diagnostic Messages
book_name=rcerrs
book_incs=..\..\gui;..\..\gml;..\..
book_os2_help=$(hbook).hlp

!else ifeq hbook pguide
book_title=Watcom C/C++ Programmer's Guide
book_name=pguide
book_incs=..\..\pg;..\..\cmn;..\..\cg;..\..\rsi;..\..\win386;..\..\gml;..\..\lr\src
multi_chap=true

!else ifeq hbook fpguide
book_title=Watcom FORTRAN 77 Programmer's Guide
book_name=fpguide
book_incs=..\..\pg;..\..\cmn;..\..\fg;..\..\rsi;..\..\win386;..\..\gml;..\..\lr\src
multi_chap=true

!else ifeq hbook f77lr
book_title=Watcom FORTRAN 77 Language Reference
book_name=f77lr
book_incs=..\..\fl;..\..\gml;..\..
multi_chap=true

!else ifeq hbook f77graph
book_title=Watcom FORTRAN 77 Graphics Library Reference
book_name=f77graph
book_incs=..\..\lr\ge;..\..\lr;..\..\lr\src;..\..\lr\gr;..\..\gml
multi_chap=true

!else ifeq hbook clbtst
book_title=Watcom C Library Reference
book_name=clib_tst
book_incs=..\..\lr\ge;..\..\lr;..\..\lr\src;..\..\lr\gr;..\..\gml
multi_chap=true

!else ifeq hbook clib
book_title=Watcom C Library Reference
book_name=clib_dos
book_incs=..\..\lr\ge;..\..\lr;..\..\lr\src;..\..\lr\gr;..\..\gml
multi_chap=true

!else ifeq hbook clibnt
book_title=Watcom C Library Reference
book_name=clib_nt
book_incs=..\..\lr\ge;..\..\lr;..\..\lr\src;..\..\lr\gr;..\..\gml
multi_chap=true

!else ifeq hbook clibqnx
book_title=Watcom C Library Reference for QNX
book_name=clib_qnx
book_incs=..\..\lr\ge;..\..\lr;..\..\lr\qnx;..\..\lr\src;..\..\lr\gr;..\..\gml
multi_chap=true

!else ifeq hbook cpplib
book_title=Watcom C++ Class Library Reference
book_name=cpplib
book_incs=..\..\cpplib;..\..\gml
book_winhc=hc31
multi_chap=true

!else ifeq hbook wd
book_title=Watcom Debugger Guide
book_name=wdbg
book_incs=..\..\wd;..\..\gml;..\..
book_os2_help=$(hbook).hlp
multi_chap=true

!else ifeq hbook ide
book_title=Watcom IDE
book_os2_help=$(hbook).hlp

!else ifeq hbook wbrw
book_title=Watcom Source Browser
book_os2_help=$(hbook).hlp

!else ifeq hbook wprof
book_title=Watcom Profiler
book_os2_help=$(hbook).hlp
multi_chap=true

!else ifeq hbook editor
book_title=Watcom Editor
multi_chap=true

!else ifeq hbook resedt
book_title=Watcom Resource Editor

!else ifeq hbook resacc
book_title=Watcom Accelerator Editor

!else ifeq hbook resdlg
book_title=Watcom Dialog Editor

!else ifeq hbook resimg
book_title=Watcom Image Editor

!else ifeq hbook resmnu
book_title=Watcom Menu Editor

!else ifeq hbook resstr
book_title=Watcom String Table Editor

!else ifeq hbook rescomp
book_title=Watcom Resource Compiler

!else ifeq hbook zoom
book_title=Watcom Zoom

!else ifeq hbook heapwalk
book_title=Watcom Heap Walker

!else ifeq hbook spy
book_title=Watcom Spy

!else ifeq hbook ddespy
book_title=Watcom DDE Spy

!else ifeq hbook drwatcom
book_title=Dr. Watcom

!else ifeq hbook drnt
book_title=Dr. Watcom NT

!else ifeq hbook cmix
book_title=Watcom C/C++ Master Index
book_name=cmix
book_incs=.;..\..\gml;..\..
book_winhc=hc31

!else ifeq hbook fmix
book_title=Watcom FORTRAN 77 Master Index
book_name=fmix
book_incs=.;..\..\gml;..\..
book_winhc=hc31

!endif

!ifndef book_name
book_name=guitool
!endif

!ifndef book_incs
book_incs=..\..\gui\snaps;..\..\gui;..\..\cg;..\..\gml
!endif

!ifndef book_winhc
book_winhc=whc
!endif

!ifndef multi_chap
multi_chap=false
!endif

!ifndef GML_ROOT
GML_ROOT = $(%GML_ROOT)
!endif
!ifndef GMLLIB
GMLLIB = $(%GMLLIB)
!endif
!ifndef WHELP
WHELP = $(%WHELP)
!endif


.EXTENSIONS: .whp .gml .blt .idx .cop .pcd

rootdir= $+ $(%cwd)\.. $-
projdir= $+ $(%cwd) $-

.before
    @set GMLINC=$(projdir);$(WHELP);..\;$(book_incs)
    @set GMLLIB=$(projdir);$(WHELP);$(GMLLIB)

.after
    @cd $(projdir)

.error
    @cd $(projdir)

.gml : $(book_incs)
.idx : $(projdir)
.blt : $(projdir)
.pcd : $(WHELP)
.cop : $(WHELP)

!ifndef gml_opts
gml_opts=
!endif

gml_opts += set book $(hbook) set dohelp 1

!ifdef on_build_machine
gml_opts += nowa
!else
gml_opts += verb stat
!endif

# The 'append' lines below are to ensure that the files exist. If they do
# exist, no real harm is done by appending to them. 'WHPCVT', which
# is run after this, just ends up recreating them anyway
$(hbook).whp : $(rootdir)\makefile $(WHELP)\whelp.cop $(WHELP)\whelpdrv.cop
    @%append $(hbook).idx
    @%append $(hbook).tbl
    @%append $(hbook).kw
!ifndef %OS2_TK
    -$(GML_ROOT)\wgml $(book_name) (pass 2 output (t:2200)$(hbook).whp file whelp.opt $(gml_opts)
!else
    -$(GML_ROOT)\wgml $(book_name) (pass 2 output (t:2200)$(hbook).whp file whelp.opt $(gml_opts) | tee $(hbook).log
!endif
    copy sysusr01.gml $(hbook).mix
    del  sysusr01.gml

.pcd.cop :
    cd $(WHELP)
    -$(GML_ROOT)\gendev $[*.pcd
    cd $(projdir)

