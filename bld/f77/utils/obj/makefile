__none : .symbolic
        @echo use "WMAKE forceout" or "WMAKE checkout" or "WMAKE checked_out"
        @echo or "WMAKE errsrc" or "WMAKE checkin= checkin"
        @%abort

checkin : .symbolic
        set check_out=checkin
        @%make __err

forceout : .symbolic
        set check_out=forceout
        @%make __err

checkout : .symbolic
        set check_out=checkout
        @%make __err

checked_out : .symbolic
        set check_out=@echo
        @%make __err

__err : errsrc.exe .symbolic
        $(%check_out) $(f77_dir)\cg86\rt\h\errcod.h
        $(%check_out) $(f77_dir)\cg86\rt\c\errmsg.c
        $(%check_out) $(f77_dir)\cg86\rt\c\errmsg.rc
        $(%check_out) $(f77_dir)\cg86\rt\c\errgrp.c
#       $(%check_out) $(f77_dir)\cg86\rt\c\jerrmsg.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\errcodi.h
        $(%check_out) $(f77_dir)\cg86\cp\h\errgrpi.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsgi.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsgi.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\errcod3.h
        $(%check_out) $(f77_dir)\cg86\cp\h\errgrp3.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsg3.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsg3.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\errcodr.h
        $(%check_out) $(f77_dir)\cg86\cp\h\errgrpr.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsgr.c
        $(%check_out) $(f77_dir)\cg86\cp\h\errmsgr.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\jerrmsgi.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\jerrmsg3.rc
        $(%check_out) $(f77_dir)\cg86\cp\h\jerrmsgr.rc
        $(%check_out) $(f77_dir)\lg86\h\errcod.h
        $(%check_out) $(f77_dir)\lg86\c\errmsg.c
        $(%check_out) $(f77_dir)\lg86\h\errmsgw.rc
        $(%check_out) $(f77_dir)\lg86\c\errgrp.c
        $(%check_out) $(f77_dir)\lg86\rt\asm\errcod.inc
        $(%check_out) $(f77_dir)\lg86\rt\h\errcod.h
        $(%check_out) $(f77_dir)\lg86\rt\c\errmsg.c
        $(%check_out) $(f77_dir)\lg86\rt\c\errmsg.rc
        $(%check_out) $(f77_dir)\lg86\rt\c\errgrp.c
#       $(%check_out) $(f77_dir)\lg86\rt\c\jerrmsg.rc
        $(%check_out) $(f77_dir)\wfl\h\errcodi.h
        $(%check_out) $(f77_dir)\wfl\h\errmsgi.c
        $(%check_out) $(f77_dir)\wfl\h\errmsgi.rc
        $(%check_out) $(f77_dir)\wfl\h\errgrpi.c
        $(%check_out) $(f77_dir)\wfl\h\jerrmsgi.rc
        $(%check_out) $(f77_dir)\wfl\h\errcod3.h
        $(%check_out) $(f77_dir)\wfl\h\errmsg3.c
        $(%check_out) $(f77_dir)\wfl\h\errmsg3.rc
        $(%check_out) $(f77_dir)\wfl\h\errgrp3.c
        $(%check_out) $(f77_dir)\wfl\h\jerrmsg3.rc
        $(%check_out) $(f77_dir)\wfl\h\errcodr.h
        $(%check_out) $(f77_dir)\wfl\h\errmsgr.c
        $(%check_out) $(f77_dir)\wfl\h\errmsgr.rc
        $(%check_out) $(f77_dir)\wfl\h\errgrpr.c
        $(%check_out) $(f77_dir)\wfl\h\jerrmsgr.rc
        $(%check_out) $(f77_dir)\lg86\nec\errmsgw.rc
!ifndef checkin
        errsrc eo r
        copy errcod.h $(f77_dir)\cg86\rt\h\errcod.h
        del errcod.h
        copy errmsg.c $(f77_dir)\cg86\rt\c\errmsg.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\cg86\rt\c\errmsg.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\cg86\rt\c\errgrp.c
        del errgrp.c
        del errmsg.msg
        errsrc eoic
        copy errcod.h $(f77_dir)\cg86\cp\h\errcodi.h
        del errcod.h
        copy errmsg.c $(f77_dir)\cg86\cp\h\errmsgi.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\cg86\cp\h\errmsgi.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\cg86\cp\h\errgrpi.c
        del errgrp.c
        del errmsg.msg
        errsrc eo3c
        copy errcod.h $(f77_dir)\cg86\cp\h\errcod3.h
        del errcod.h
        copy errmsg.c $(f77_dir)\cg86\cp\h\errmsg3.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\cg86\cp\h\errmsg3.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\cg86\cp\h\errgrp3.c
        del errgrp.c
        del errmsg.msg
        errsrc eorc
        copy errcod.h $(f77_dir)\cg86\cp\h\errcodr.h
        del errcod.h
        copy errmsg.c $(f77_dir)\cg86\cp\h\errmsgr.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\cg86\cp\h\errmsgr.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\cg86\cp\h\errgrpr.c
        del errgrp.c
        del errmsg.msg
        errsrc ew
        copy errcod.h $(f77_dir)\lg86\h\errcod.h
        del errcod.h
        copy errmsg.c $(f77_dir)\lg86\c\errmsg.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\lg86\h\errmsgw.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\lg86\c\errgrp.c
        del errgrp.c
        sed -e 's/^$#define    //' -e 's/ / EQU /' $(f77_dir)\lg86\h\errcod.h >$(f77_dir)\lg86\rt\asm\errcod.inc
        del errmsg.msg
        errsrc ew r
        copy errcod.h $(f77_dir)\lg86\rt\h\errcod.h
        del errcod.h
        copy errmsg.c $(f77_dir)\lg86\rt\c\errmsg.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\lg86\rt\c\errmsg.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\lg86\rt\c\errgrp.c
        del errgrp.c
        del errmsg.msg
        errsrc elic
        copy errcod.h $(f77_dir)\wfl\h\errcodi.h
        del errcod.h
        copy errmsg.c $(f77_dir)\wfl\h\errmsgi.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\wfl\h\errmsgi.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\wfl\h\errgrpi.c
        del errgrp.c
        del errmsg.msg
        errsrc el3c
        copy errcod.h $(f77_dir)\wfl\h\errcod3.h
        del errcod.h
        copy errmsg.c $(f77_dir)\wfl\h\errmsg3.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\wfl\h\errmsg3.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\wfl\h\errgrp3.c
        del errgrp.c
        del errmsg.msg
        errsrc elrc
        copy errcod.h $(f77_dir)\wfl\h\errcodr.h
        del errcod.h
        copy errmsg.c $(f77_dir)\wfl\h\errmsgr.c
        del errmsg.c
        copy errmsg.rc $(f77_dir)\wfl\h\errmsgr.rc
        del errmsg.rc
        copy errgrp.c $(f77_dir)\wfl\h\errgrpr.c
        del errgrp.c
        del errmsg.msg
#       errsrc jo r
#       copy errmsg.rc $(f77_dir)\cg86\rt\c\jerrmsg.rc
#       del errmsg.rc
#       del errcod.h
#       del errmsg.c
#       del errgrp.c
#       del errmsg.msg
        errsrc joic
        copy errmsg.rc $(f77_dir)\cg86\cp\h\jerrmsgi.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
        errsrc jo3c
        copy errmsg.rc $(f77_dir)\cg86\cp\h\jerrmsg3.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
        errsrc jorc
        copy errmsg.rc $(f77_dir)\cg86\cp\h\jerrmsgr.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
        errsrc jw
        copy errmsg.rc $(f77_dir)\lg86\nec\errmsgw.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
#       errsrc jw r
#       copy errmsg.rc $(f77_dir)\lg86\rt\c\jerrmsg.rc
#       del errmsg.rc
#       del errcod.h
#       del errmsg.c
#       del errgrp.c
#       del errmsg.msg
        errsrc jlic
        copy errmsg.rc $(f77_dir)\wfl\h\jerrmsgi.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
        errsrc jl3c
        copy errmsg.rc $(f77_dir)\wfl\h\jerrmsg3.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
        errsrc jlrc
        copy errmsg.rc $(f77_dir)\wfl\h\jerrmsgr.rc
        del errmsg.rc
        del errcod.h
        del errmsg.c
        del errgrp.c
        del errmsg.msg
!endif

errsrc : errsrc.exe .symbolic
        @%null

errsrc.exe : errsrc.obj
        wlink d a f errsrc

errsrc.obj : ..\c\errsrc.c
        wcc386 $[@ /w3 /d1 /i=$(lang_root)\h;$(c_dir)h /dstatic=extern /zs /v >nul
        wcc386 $[@ /w3 /d1 /i=$(lang_root)\h;$(c_dir)h

makedoc : errdoc.exe .symbolic
        errdoc

errdoc.exe : errdoc.obj
        wlink d a f errdoc

errdoc.obj : ..\c\errdoc.for
        wfc386 $[@ /d1
