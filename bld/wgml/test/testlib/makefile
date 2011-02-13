#
# Watcom GML/Script Test Device Library Makefile
#
# This uses the existing (legacy) gendev, either 
# the DOS version or the OS/2 version
#

#-------------------------------------------------------
# Set the extensions and paths; .inc is a default extension
#-------------------------------------------------------

.extensions : .cop .pcd .fon

.pcd: ../testsrc
.fon: ../testsrc
.inc: ../testsrc

#-------------------------------------------------------
# Set build_platform and define gendev
#-------------------------------------------------------

!ifeq %build_platform dos386
gendev = $(%DOC_ROOT)\gml\dos\gendev

!else ifeq %build_platform os2386
gendev = $(%DOC_ROOT)\gml\os2\gendev

!else ifeq %build_platform nt386
gendev = $(%DOC_ROOT)\gml\dos\gendev

!else ifeq %build_platform linux386

#-------------------------------------------------------
# set up to use dosemu to run gendev under Linux
# copied/modified from how wgml is handled under Linux
# the location for gendev will probably need to be modified
# since it is relative to a different directory
#-------------------------------------------------------

dosemu_gendev_batch=gendev.bat
$(dosemu_gendev_batch): $(__MAKEFILES__)
    @%create $@
    @%append $@ unix -s GMLLIB
    @%append $@ unix -s GMLINC
    @%append $@ ..\gml\dos\gendev.exe "%1 %2 %3 %4 %5 %6 %7 %8"
    @%append $@ exitemu
%make $(dosemu_gendev_batch)

gendev = dosemu -dumb $(dosemu_wgml_batch)

!else
!error Unsupported building platform !!!!!!

!endif

#-------------------------------------------------------
# Set the environment variables
#-------------------------------------------------------

#owroot=OWROOT
.before:
        set GMLLIB=$(%owroot)\bld\wgml\test\testlib
	set GMLINC=..\testsrc

#-------------------------------------------------------
# Where the action is
#-------------------------------------------------------

.pcd.cop:
	$(gendev) $^&

.fon.cop:
	$(gendev) $^&

default : test01.cop test02.cop pstest.cop psspec.cop tdrv01.cop tdrv02.cop &
          pstdrv.cop pssdrv.cop pssfon00.cop pssfon01.cop pssfon02.cop &
	  pssfon03.cop pssfon04.cop pssfon05.cop pssfon06.cop pssfon07.cop &
	  pssfon08.cop pssfon09.cop tfon00.cop tfon01.cop tfon02.cop &
	  tfon03.cop tfon04.cop tfon05.cop tfon06.cop tfon07.cop tfon08.cop &
	  tfon09.cop .SYMBOLIC

genall  : .SYMBOLIC
	  $(gendev) genall

clean   : .SYMBOLIC
          del *.cop
