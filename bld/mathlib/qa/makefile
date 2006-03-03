#
# Math libraray test program makefile (see mathtest.c)
#

# code  platform            invocation
# ====  ========            ==========
# DOS   16-bit DOS          dos_fpi
#                           dos_fpc
# PLS   PharLap             run386 pls_fpi
#                           run386 pls_fpc
# RSI   Rational Systems    rsi_fpi
#                           rsi_fpc
# OS2   OS/2 16-bit         os2_fpi
#                           os2_fpc
# OS3   OS/2 32-bit         os3_fpi
#                           os2_fpc
# WIN   Windows 16-bit      create icons ; click
# W32   Windows 32-bit      create icon; click
# WNT   Windows NT          wnt_fpi
#                           wnt_fpc

!include cproj.mif
!include deftarg.mif

.erase
.extensions:
.extensions: .exe .rex .exp .obj .c .asm

mathtest = $(new_mlib)\c\mathtest.c

comp_opts = -za99 -ox -d2 -zq -fo=$^*.obj
link_opts = NAME $^* OPTION quiet, map DEBUG all

exes = &
    dos_fpi.exe dos_fpc.exe &
    pls_fpi.exp pls_fpc.exp &
    rsi_fpi.exe rsi_fpc.exe &
    os2_fpi.exe os2_fpc.exe &
    os3_fpi.exe os3_fpc.exe &
    win_fpi.exe win_fpc.exe &
    w32_fpi.exe w32_fpc.exe &
    wnt_fpi.exe wnt_fpc.exe

all : .symbolic $(exes)

dos_fpi.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpi
    wlink SYSTEM dos $(link_opts) FILE $^*

dos_fpc.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpc
    wlink SYSTEM dos $(link_opts) FILE $^*

pls_fpi.exp : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpi
    wlink SYSTEM pharlap option offset=4k $(link_opts) FILE $^*

pls_fpc.exp : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpc
    wlink SYSTEM pharlap option offset=4k $(link_opts) FILE $^*

rsi_fpi.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpi
    wlink SYSTEM dos4g $(link_opts) FILE $^*

rsi_fpc.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpc
    wlink SYSTEM dos4g $(link_opts) FILE $^*

os2_fpi.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpi
    wlink SYSTEM os2 pmcompatible $(link_opts) FILE $^*

os2_fpc.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpc
    wlink SYSTEM os2 pmcompatible $(link_opts) FILE $^*

os3_fpi.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpi
    wlink SYSTEM os2v2 pmcompatible $(link_opts) FILE $^*

os3_fpc.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpc
    wlink SYSTEM os2v2 pmcompatible $(link_opts) FILE $^*

win_fpi.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpi -bw -zw
    wlink SYSTEM windows $(link_opts) FILE $^*

win_fpc.exe : $(mathtest)
    $(cc_i86) $[@ $(comp_opts) -fpc -bw -zw
    wlink SYSTEM windows $(link_opts) FILE $^*

w32_fpi.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpi -bw -zw
    wlink SYSTEM win386 $(link_opts) FILE $^*
    wbind -q -s $(lang_root)\binw\win386.ext -n $^*
    rm $^*.rex

w32_fpc.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpc -bw -zw
    wlink SYSTEM win386 $(link_opts) FILE $^*
    wbind -q -s $(lang_root)\binw\win386.ext -n $^*
    rm $^*.rex

wnt_fpi.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpi -bt=nt
    wlink SYSTEM nt $(link_opts) FILE $^*

wnt_fpc.exe : $(mathtest)
    $(cc_386) $[@ $(comp_opts) -fpc -bt=nt
    wlink SYSTEM nt $(link_opts) FILE $^*
