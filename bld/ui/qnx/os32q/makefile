#pmake: all build cpu_386 os_qnx
host_CPU = 386
host_OS = qnx

MODEL = S
!ifdef comp386
WCCEXE=$(comp386)
!else
WCCEXE=wcc386
!endif
WCPPEXE=wcc386
EXTRA=$(deffp)/4r/bt=qnx
FORM=qnx386

!include ..\makefile
