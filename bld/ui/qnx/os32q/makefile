#pmake: os_qnx cpu_386

host_os  = qnx
host_cpu = 386

MODEL = S
!ifdef comp386
WCCEXE=$(comp386)
!else
WCCEXE=wcc386
!endif
WCPPEXE=wcc386
EXTRA=$(deffp)/4r/bt=qnx
FORM=qnx386

!include ../makefile
