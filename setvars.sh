# It may be best to copy this file to "setvars" and make that writable before
# adjusting these settings

# root of source tree
export OWROOT=$HOME/watcom/src

# Change this to point to an existing Watcom compiler (if applicable)
# or alternatively to the release directory
export WATCOM=$OWROOT/rel2

export MAKE=make

#subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# Unset this variable to get more debug info
export ON_BUILD_MACHINE=1

# Stuff for the Open Watcom build environment
export BATDIR=$OWROOT
export BUILDER_CTL=lang.ctl
export BUILD_PLATFORM=linux386
export build_platform=$BUILD_PLATFORM
export DEFPATH=$PATH
export DEVDIR=$OWROOT/bld
export devdir=$DEVDIR
export DWATCOM=$WATCOM
export INCLUDE=$WATCOM/lh:$DEVDIR/watcom/h
export LIB=$OWROOT/bld/watcom/lib
export LANG_BLD=$OWROOT/bat
export owroot=$OWROOT
export PATH=$OWROOT/bin:$DEVDIR/build/binl:$OWROOT/bat:$WATCOM/binl:$DOC_ROOT/cmds:$DEFPATH

# Setup environment variables for Perforce
export P4PORT=perforce.scitechsoft.com:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

