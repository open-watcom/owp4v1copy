@echo off
call bmode
call docd toolkt2x
if [%1] == [] goto nocopy
set altdevdir=i:
for %%i in (%altdevdir%\toolkt21\*.*)                    do ifarch copy %%i %relroot%\%1\toolkt2x
for %%i in (%altdevdir%\toolkt21\asm\os2inc\*.*)         do ifarch copy %%i %relroot%\%1\h\os2
for %%i in (%altdevdir%\toolkt21\asm\samples\*.*)        do ifarch copy %%i %relroot%\%1\samples\toolkt2x\asm
for %%i in (%altdevdir%\toolkt21\asm\samples\pdd\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\asm\pdd
for %%i in (%altdevdir%\toolkt21\asm\samples\vdd\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\asm\vdd
for %%i in (%altdevdir%\toolkt21\book\*.*)               do ifarch copy %%i %relroot%\%1\binp\help
for %%i in (%altdevdir%\toolkt21\c\os2h\*.*)             do ifarch copy %%i %relroot%\%1\h\os2
for %%i in (%altdevdir%\toolkt21\c\os2h\dasd\*.*)        do ifarch copy %%i %relroot%\%1\h\os2
for %%i in (%altdevdir%\toolkt21\c\samples\*.*)          do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c
for %%i in (%altdevdir%\toolkt21\c\samples\animals\*.*)  do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\animals
for %%i in (%altdevdir%\toolkt21\c\samples\bidi\*.*)     do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\bidi
for %%i in (%altdevdir%\toolkt21\c\samples\clipbrd\*.*)  do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\clipbrd
for %%i in (%altdevdir%\toolkt21\c\samples\clock\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\clock
for %%i in (%altdevdir%\toolkt21\c\samples\dialog\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\dialog
for %%i in (%altdevdir%\toolkt21\c\samples\dllapi\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\dllapi
for %%i in (%altdevdir%\toolkt21\c\samples\dragdrop\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\dragdrop
for %%i in (%altdevdir%\toolkt21\c\samples\eas\*.*)      do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\eas
for %%i in (%altdevdir%\toolkt21\c\samples\graphic\*.*)  do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\graphic
for %%i in (%altdevdir%\toolkt21\c\samples\hanoi\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\hanoi
for %%i in (%altdevdir%\toolkt21\c\samples\hello\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\hello
for %%i in (%altdevdir%\toolkt21\c\samples\image\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\image
for %%i in (%altdevdir%\toolkt21\c\samples\ipf\*.*)      do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\ipf
for %%i in (%altdevdir%\toolkt21\c\samples\jigsaw\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\jigsaw
for %%i in (%altdevdir%\toolkt21\c\samples\npipe\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\npipe
for %%i in (%altdevdir%\toolkt21\c\samples\palette\*.*)  do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\palette
for %%i in (%altdevdir%\toolkt21\c\samples\print\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\print
for %%i in (%altdevdir%\toolkt21\c\samples\queues\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\queues
for %%i in (%altdevdir%\toolkt21\c\samples\semaph\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\semaph
for %%i in (%altdevdir%\toolkt21\c\samples\sort\*.*)     do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\sort
for %%i in (%altdevdir%\toolkt21\c\samples\style\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\style
for %%i in (%altdevdir%\toolkt21\c\samples\template\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\template
for %%i in (%altdevdir%\toolkt21\c\samples\tp\*.*)       do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\tp
for %%i in (%altdevdir%\toolkt21\c\samples\vmm\*.*)      do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\vmm
for %%i in (%altdevdir%\toolkt21\c\samples\worms\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\worms
for %%i in (%altdevdir%\toolkt21\c\samples\wpcar\*.*)    do ifarch copy %%i %relroot%\%1\samples\toolkt2x\c\wpcar
for %%i in (%altdevdir%\toolkt21\icon\*.*)               do ifarch copy %%i %relroot%\%1\toolkt2x\icon
for %%i in (%altdevdir%\toolkt21\ipfc\*.*)               do ifarch copy %%i %relroot%\%1\toolkt2x\ipfc
for %%i in (%altdevdir%\toolkt21\os2bin\*.*)             do ifarch copy %%i %relroot%\%1\binp
for %%i in (%altdevdir%\toolkt21\os2help\*.*)            do ifarch copy %%i %relroot%\%1\binp\help
for %%i in (%altdevdir%\toolkt21\os2lib\*.*)             do ifarch copy %%i %relroot%\%1\lib386\os2
for %%i in (%altdevdir%\toolkt21\dll\*.*)                do ifarch copy %%i %relroot%\%1\binp\dll
for %%i in (%altdevdir%\toolkt21\rexx\samples\*.*)       do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx
for %%i in (%altdevdir%\toolkt21\rexx\samples\callrexx\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\callrexx
for %%i in (%altdevdir%\toolkt21\rexx\samples\devinfo\*.*)  do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\devinfo
for %%i in (%altdevdir%\toolkt21\rexx\samples\pmrexx\*.*)   do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\pmrexx
for %%i in (%altdevdir%\toolkt21\rexx\samples\rexxutil\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\rexxutil
for %%i in (%altdevdir%\toolkt21\rexx\samples\rxmacdll\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\rxmacdll
for %%i in (%altdevdir%\toolkt21\rexx\samples\rxstring\*.*) do ifarch copy %%i %relroot%\%1\samples\toolkt2x\rexx\rxstring
for %%i in (%altdevdir%\toolkt21\sc\*.*)                    do ifarch copy %%i %relroot%\%1\h\os2
for %%i in (%altdevdir%\toolkt13\h\*.*)                     do ifarch copy %%i %relroot%\%1\h\os21x
for %%i in (%altdevdir%\toolkt13\progref\*.*)               do ifarch copy %%i %relroot%\%1\binp\help
ifarch copy %altdevdir%\toolkt13\lib\os2.lib %relroot%\%1\lib286\os2
set altdevdir=
:nocopy
cd ..
