#pmake: build wgml

#
# Watcom GML/Script Test Device Library Makefile
#
# This uses the existing (legacy) gendev, either 
# the DOS version or the OS/2 version
#

!ifdef __UNIX__
RENCMD=mv
!else
RENCMD=ren
!endif

docs_dir = $(%OWROOT)/docs
src_dir = $(%OWROOT)/bld

gendev_cmdl_params = $[@

!include bhost.mif
!include wgmlcmd.mif

!include wgmlcmd.mif

#-------------------------------------------------------
# Set the extensions, paths, and groups
#-------------------------------------------------------

.extensions :                           # reset extensions
.extensions : .inc .cop .pcd .fon

.pcd: ../testsrc
.fon: ../testsrc
.inc: ../testsrc

dev_common =	&
	dev01.inc	&
	dev03.inc	&
	dev07.inc

dev_core =	&
	dev02.inc	&
	dev04.inc	&
	dev05.inc	&
	dev06.inc

drv_common =	&
	drv01.inc	&
	drv02.inc

drv_core =	&
	drv03.inc

drv_page =	&
	drv04.inc

#-------------------------------------------------------
# Where the action is
#-------------------------------------------------------

default : test01.cop test02.cop pstest.cop psspec.cop tdrv01.cop &
          tdrv02.cop pstdrv.cop pssdrv.cop pssfon00.cop pssfon01.cop &
	  pssfon02.cop pssfon03.cop pssfon04.cop pssfon05.cop pssfon06.cop &
	  pssfon07.cop pssfon08.cop pssfon09.cop tfon00.cop tfon01.cop &
	  tfon02.cop tfon03.cop tfon04.cop tfon05.cop tfon06.cop tfon07.cop &
	  tfon08.cop tfon09.cop .SYMBOLIC

test01.cop : test01.pcd $(dev_common) $(dev_core)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TEST01.COP $@
!endif

test02.cop : test02.pcd $(dev_common) $(dev_core)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TEST02.COP $@
!endif

pstest.cop : pstest.pcd $(dev_common) $(dev_core)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSTEST.COP $@
!endif

psspec.cop : psspec.pcd $(dev_common)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSPEC.COP $@
!endif

tdrv01.cop : tdrv01.pcd $(drv_common) $(drv_core)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TDRV01.COP $@
!endif

tdrv02.cop : tdrv02.pcd $(drv_common) $(drv_core) $(drv_page)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TDRV02.COP $@
!endif

pstdrv.cop : pstdrv.pcd $(drv_common) $(drv_core) $(drv_page)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSTDRV.COP $@
!endif

pssdrv.cop : pssdrv.pcd $(drv_common) $(drv_page)
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSDRV.COP $@
!endif

pssfon00.cop : pssfon00.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON00.COP $@
!endif

pssfon01.cop : pssfon01.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON01.COP $@
!endif

pssfon02.cop : pssfon02.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON02.COP $@
!endif

pssfon03.cop : pssfon03.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON03.COP $@
!endif

pssfon04.cop : pssfon04.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON04.COP $@
!endif

pssfon05.cop : pssfon05.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON05.COP $@
!endif

pssfon06.cop : pssfon06.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON06.COP $@
!endif

pssfon07.cop : pssfon07.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON07.COP $@
!endif

pssfon08.cop : pssfon08.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON08.COP $@
!endif

pssfon09.cop : pssfon09.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) PSSFON09.COP $@
!endif

tfon00.cop : tfon00.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON00.COP $@
!endif

tfon01.cop : tfon01.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON01.COP $@
!endif

tfon02.cop : tfon02.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON02.COP $@
!endif

tfon03.cop : tfon03.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON03.COP $@
!endif

tfon04.cop : tfon04.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON04.COP $@
!endif

tfon05.cop : tfon05.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON05.COP $@
!endif

tfon06.cop : tfon06.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON06.COP $@
!endif

tfon07.cop : tfon07.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON07.COP $@
!endif

tfon08.cop : tfon08.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON08.COP $@
!endif

tfon09.cop : tfon09.fon
        @%make gencop
!ifdef wgml_ucase
        @$(RENCMD) TFON09.COP $@
!endif

genall : genall.pcd .symbolic
        @%make gencop

gencop : .procedure
!ifeq use_dosemu dosemu
        %create $(dosemu_gendev_batch)
        @%append $(dosemu_gendev_batch) lredir w: linux\fs$(docs_dir)
        @%append $(dosemu_gendev_batch) set GMLLIB=..\testlib
        @%append $(dosemu_gendev_batch) set GMLINC=..\testsrc
        @%append $(dosemu_gendev_batch) w:\gml\dos\gendev.exe $(gendev_cmdl_params)
        @%append $(dosemu_gendev_batch) exitemu
        $(GENDEVCMD)
!else ifeq use_dosemu dosbox
        %create $(dosemu_gendev_batch)
        @%append $(dosemu_gendev_batch) d:
        @%append $(dosemu_gendev_batch) set GMLLIB=e:\testlib
        @%append $(dosemu_gendev_batch) set GMLINC=e:\testsrc
        @%append $(dosemu_gendev_batch) c:\gml\dos\gendev.exe $(gendev_cmdl_params)
        @%append $(dosemu_gendev_batch) exit
        $(GENDEVCMD)
!else
        set GMLLIB=..\testlib
        set GMLINC=..\testsrc
        $(GENDEVCMD)
!endif

clean   : .SYMBOLIC
        rm -f *.bat *.cop *.COP
        rm -f stdout.txt stderr.txt
