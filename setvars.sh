# It may be best to copy this file to "setvars" and make that writable before
# adjusting these settings

# root of source tree
export OWROOT=`cd \`dirname $0\` && pwd`

# Change this to point to an existing Watcom compiler (if applicable)
# or alternatively to the release directory
export WATCOM=$OWROOT/rel2

export MAKE=make

# Subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# Set this variable to 1 to get debug build
export DEBUG_BUILD=0

# Set this variable to 1 to get default windowing support in clib
export DEFAULT_WINDOWING=0

# Set this variable to 0 to suppress documentation build
export DOC_BUILD=0

# Stuff for the Open Watcom build environment
export BATDIR=$OWROOT
export BUILDER_CTL=lang.ctl
export BUILD_PLATFORM=linux386
export BLD_VER=12
export BLD_VER_STR=1.2
export DEFPATH=$PATH
export DEFREL=rel2
export DEVDIR=$OWROOT/bld
export DWATCOM=$WATCOM
export DOC_ROOT=$OWROOT/docs
export INCLUDE=$WATCOM/lh:$DEVDIR/watcom/h
export LIB=$OWROOT/bld/watcom/lib
export LANG_BLD=$OWROOT/bat
export PATH=$OWROOT/bin:$DEVDIR/build/binl:$OWROOT/bat:$WATCOM/binl:$DOC_ROOT/cmds:$DEFPATH
export RELROOT=$OWROOT
export WD_PATH=$WATCOM/binl

# Setup environment variables for Perforce
export P4PORT=perforce.scitechsoft.com:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

