%devdr%:
call docd %devdir%
dobuild 2>&1 | tee build.log
