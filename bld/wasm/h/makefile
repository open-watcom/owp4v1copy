#pmake: build

!ifdef __OS2__
_OS     = OS2
!else
_OS     = DOS
!endif
sys     = none
comp    = echo error
targ_n  = dummy
targ_e  =

target : ..\h\inlnops.h ..\h\fullops.h .SYMBOLIC
        @%null

!include ..\gen.mif

clean: .SYMBOLIC
    @if exist inlnops.h  del inlnops.h
    @if exist fullops.h  del fullops.h
    @if exist msg.gh     del msg.gh
    @if exist wasmmsg.gh del wasmmsg.gh
    @if exist ..\mkopcode.exe del ..\mkopcode.exe
