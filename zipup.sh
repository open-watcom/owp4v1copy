#! /bin/sh
RELEASE=1.2.1
LABEL=open_watcom_devel_$RELEASE
PREFIX=open_watcom_devel_$RELEASE
P4OPT=-f
P4PORT=-pp4.scitechsoft.com:3488
ARCHIVES=/archives
cd $ARCHIVES
rm -f $PREFIX-src.tar.bz2

#############################################################################
# Archive all Open Watcom source files
rm -rf $ACHIVES/ow_devel_src
p4 $P4PORT -uanonymous -Panonymous -cOPENWATCOM_DEVEL_SRC sync $P4OPT @$LABEL
cd $ARCHIVES/ow_devel_src
tar -cv * | bzip2 -9 > ../$PREFIX-src.tar.bz2

cd ~/ow

