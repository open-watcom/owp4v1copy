all: wstub.exe wstubq.exe

wstub.exe: wstub.obj
    wlink sys dos f $<

wstub.obj: wstub.asm
    wasm $<

wstubq.exe: wstubq.obj
    wlink sys dos f $<

wstubq.obj: wstub.asm
    wasm -dQUIET -fo=wstubq.obj $<

clean : .symbolic
        @if exist *.?bj del *.?bj
        @if exist *.exe del *.exe
        @if exist *.lst del *.lst
        @if exist *.err del *.err
        @if exist *.map del *.map
        @if exist *.dmp del *.dmp
        @if exist *.sym del *.sym
        @if exist *.tmp del *.tmp
