# It may be best to copy this file to "setvars" and make that writable before
# adjusting these settings

# root of source tree
export OWROOT=`cd \`dirname $0\` && pwd`

# Change this to point to an existing Watcom compiler (if applicable)
# or alternatively to the release directory
export WATCOM=$OWROOT/rel2

export MAKE=make

#subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# set this variable to get debug build
export DEBUG_BUILD=0

# set this variable if you want default windowing support
export default_windowing=0

# Stuff for the Open Watcom build environment
export BATDIR=$OWROOT
export BUILDER_CTL=lang.ctl
export BUILD_PLATFORM=linux386
export build_platform=$BUILD_PLATFORM
export BLD_VER=12
export DEFPATH=$PATH
export DEFREL=rel2
export DEVDIR=$OWROOT/bld
export devdir=$DEVDIR
export DWATCOM=$WATCOM
export INCLUDE=$WATCOM/lh:$DEVDIR/watcom/h
export LIB=$OWROOT/bld/watcom/lib
export LANG_BLD=$OWROOT/bat
export owroot=$OWROOT
export PATH=$OWROOT/bin:$DEVDIR/build/binl:$OWROOT/bat:$WATCOM/binl:$DOC_ROOT/cmds:$DEFPATH
export RELROOT=$OWROOT
export doc_root=$OWROOT/docs

# Setup environment variables for Perforce
export P4PORT=perforce.scitechsoft.com:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

