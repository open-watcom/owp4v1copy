set TOOLDR=f:
if exist %1\bin\vi.exe set editor=%1\bin\vi
if not exist %1\bin\vi.exe set editor=%1\binw\vi
set edpath=%tooldr%\tools\vi;%1\eddat
set watcom=%1
set toolkit=i:\toolkt20
set lang=f:\l
rem
rem COMSPEC
rem
set comspec=f:\dos\command.com
rem
rem BUILD stuff
rem
set ramd=p
set ramdisk=%RAMD%:
set devdir=g:\bld
set devdr=g
set BLDDR=g:
set TNTDIR=i:\tnt6
set reldr=h
set relroot=%reldr%:
set distroot=d:\DISTRIB
set patroot=d:/PAT
set patproj=d:/LPAT
set MKS_DIR=F:/BIN
REM set patproj=%relroot%/LPAT
set DEFREL=rel2
set DEFNET=r:\lang
set qnx_key=wcc
set testmode=
rem
rem PATH
rem
set path=
if not [%2] == [] set path=%2;
set wbin=
if not exist %watcom%\bin\*.* goto nobin
    set wbin=%watcom%\bin;%watcom%\binb
    goto endbin
:nobin
    set wbin=%watcom%\binw
    goto endbin
:endbin
set path=%path%%wbin%;%tooldr%\tools;c:\;f:\cmds;f:\bin;f:\rcs
set wbin=
set path=f:\dos;%path%;c:\qemm;f:\nu;
rem
rem WATCOM
rem
set libdos=%watcom%\lib286\dos;%watcom%\lib286;%devdir%\watcom\lib
set libos2=%watcom%\lib286\os2;%watcom%\lib286;%devdir%\watcom\lib
set libphar=%watcom%\lib386\dos;%watcom%\lib386;%devdir%\watcom\lib
set libqnx=%watcom%\lib286\qnx;%watcom%\lib286;%devdir%\watcom\lib
set libwin=%watcom%\lib286\win;%watcom%\lib286;%devdir%\watcom\lib
set lib=%libdos%
set lib286=%libdos%
set lib386=%libphar%
set include=%watcom%\h;%watcom%\h\win;%devdir%\watcom\h
set finclude=%watcom%\src\fortran
rem
rem MKS toolkit
rem
set home=f:
set glob=f:\etc\glob.exe
set help=f:\etc
set ROOTDIR=f:\
set TMPDIR=h:\tmp
rem
rem MISC
rem
set prompt=[10.0]$p$g
set wcgblipon=ALLERRS
set cge=%editor% pagedown pageup end
rem set wvideo=/reg#20/invoke#jdprof.dbg
set f77test=h:\f77test
set objpad=/f.07 /l25 /u250
set penpoint_path=f:\tools\penpoint
rem
rem WPROJ
rem
set whatsout=-b
set whatsnew=-b
set checkin=-q
set checkout=-q
set pout=*.c *.asm *.bat *.h *.inc *.2sm *.2c *.nc *.nsm *.mif *.
