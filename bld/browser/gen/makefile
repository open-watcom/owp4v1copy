dialogs = &
dgenums.$(ext) &
dgeddll.$(ext) &
dgfileed.$(ext) &
dggtroot.$(ext) &
dginhopt.$(ext) &
dgcllopt.$(ext) &
dglinedt.$(ext) &
dgmodlst.$(ext) &
dgmemflt.$(ext) &
dgnewdbr.$(ext) &
dgreport.$(ext) &
dgrxcfg.$(ext)  &
dgqrfilt.$(ext) &
dgquery.$(ext)  &
dgvwsym.$(ext)

ext = gpp
dialog_source = $+$(dialogs)$-

ext = gh
dialog_headers = $+$(dialogs)$-

dlg_dir = ../res/dlg

!ifdef __LINUX__
parse = ../dlgprs/o/dlgprs.exe $[@ ../bnd/$[&.bnd $[&.gh $^@
!else
parse = ..\dlgprs\o\dlgprs $[@ ..\bnd\$[&.bnd $[&.gh $^@
!endif

.BEFORE
    @set dos4g=quiet

.extensions:
.extensions: .gpp .gh .dlg .bnd

all : $(dialog_source) $(dialog_headers)

.dlg: $(dlg_dir)
.dlg.gpp:
    $(parse)

.dlg: $(dlg_dir)
.dlg.gh:
    $(parse)

clean: .SYMBOLIC
    rm -f *.gh *.gpp *.tim
