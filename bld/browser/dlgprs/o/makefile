.IGNORE

!include bhost.mif

scan_files = &
bind.obj &
dialog.obj &
main.obj &
scanner.obj &
styles.obj &
prsbnd.obj &
prsdlg.obj

browse_files = &
chfile.obj  &
chbffile.obj &
mempool.obj

c_objs = $+$(scan_files) $(browse_files)$-

lnk = dlgprs.lnk

yacc = $(bld_bin)\yacc -b

wppdbgf = -d1-oaxt
wldbgf = pa .

.BEFORE
#   @set compflags=-zq-mf-xs-w4 -bt=dos $(wppdbgf) -i=$(browse_dir)\contain\h;$(browse_dir)\h;..\h;..\gen;$(lang_root)\h\win
!ifdef __NT__
    @set compflags=-zq-mf-xs-w4 -bt=nt  $(wppdbgf) -i=$(browse_dir)\contain\h;$(browse_dir)\h;..\h;..\gen;$(lang_root)\h\nt
!else
    @set compflags=-zq-mf-xs-w4         $(wppdbgf) -i=$(browse_dir)\contain\h;$(browse_dir)\h;..\h;..\gen;$(lang_root)\h\nt
!endif

.extensions:
.extensions: .exe .obj .gh .c .cpp .gpp .y

!ifdef __NT__
compile.tim : dlgprsnt.exe
!else ifdef __OS2__
compile.tim : dlgprsp.exe
!else
compile.tim : dlgprs.exe
!endif
    wtouch $^@

dlgprs.exe : $(c_objs) $(browse_objs)
    @%create $(lnk)
    @%append $(lnk) name $^@
    @%append $(lnk) $(wldbgf)
    @%append $(lnk) op map=dlgprs,quiet,show,art,static sort sys dos4g
    @for %i in ($(c_objs)) do @%append $(lnk) file %i
    wlink @$(lnk)

dlgprsnt.exe : $(c_objs) $(browse_objs)
    @%create $(lnk)
    @%append $(lnk) name $^@
    @%append $(lnk) $(wldbgf)
    @%append $(lnk) op map=dlgprsnt,quiet,show,art,static sort sys nt
    @for %i in ($(c_objs)) do @%append $(lnk) file %i
    wlink @$(lnk)

dlgprsp.exe : $(c_objs) $(browse_objs)
    @%create $(lnk)
    @%append $(lnk) name $^@ system os2v2
    @%append $(lnk) debug all
    @%append $(lnk) op map=dlgprsp,quiet,show,art,static sort
    @for %i in ($(c_objs)) do @%append $(lnk) file %i
    wlink @$(lnk)

..\gen\prsdlg.gh ..\gen\prsdlg.gpp : ..\y\prsdlg.y ..\cpp\yydriver.cpp
    $(yacc) $[@ $]@ >y.out
    @if exist ..\gen\prsdlg.gh erase ..\gen\prsdlg.gh >nul
    @copy ytab.h ..\gen\prsdlg.gh >nul
    @erase ytab.h >nul
    @if exist ..\gen\prsdlg.gpp erase ..\gen\prsdlg.gpp >nul
    @copy ytab.c ..\gen\prsdlg.gpp >nul
    @erase ytab.c >nul

..\gen\prsbnd.gh ..\gen\prsbnd.gpp : ..\y\prsbnd.y ..\cpp\yydriver.cpp
    $(yacc) $[@ $]@ >y.out
    @if exist ..\gen\prsbnd.gh erase ..\gen\prsbnd.gh >nul
    @copy ytab.h ..\gen\prsbnd.gh >nul
    @erase ytab.h >nul
    @if exist ..\gen\prsbnd.gpp erase ..\gen\prsbnd.cpp >nul
    @copy ytab.c ..\gen\prsbnd.gpp >nul
    @erase ytab.c >nul

scanner.obj : ..\cpp\scanner.cpp ..\gen\prsdlg.gh ..\gen\prsbnd.gh .AUTODEPEND
    wpp386 @compflags $[@

.cpp: ..\cpp;$(browse_dir)\cpp
.cpp.obj: .AUTODEPEND
    wpp386 @compflags $[@

.gpp: ..\gen
.gpp.obj: .AUTODEPEND
    wpp386 @compflags $[@
