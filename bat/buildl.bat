%devdr%:
call docd %devdir%
dobuildl 2>&1 | tee buildl.log
