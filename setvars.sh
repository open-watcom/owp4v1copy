set +v
# *****************************************************************
# setvars - Linux version
# *****************************************************************
# NOTE: Do not use this script directly, but copy it and
#       modify it as necessary for your own use!!

# root of source tree
export OWROOT=`cd \`dirname $0\` && pwd`

# Change this to point to an existing Watcom compiler (if applicable)
# or alternatively to the release directory
export WATCOM=$OWROOT/rel2

# Set this variable to 1 to get debug build
export DEBUG_BUILD=0

# Set this variable to 1 to get default windowing support in clib
export DEFAULT_WINDOWING=0

# Set this variable to 0 to suppress documentation build
export DOC_BUILD=0      # Documentation building uses DOS programs.

# Setup environment variables for Perforce
export P4PORT=perforce.scitechsoft.com:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

# Documentation related variables - none likely to work in Linux
# export appropriate variables to blank for help compilers which you haven't installed
# export WIN95HC=hcrtf
# export WIN31HC=hc31
# export OS2HC=ipfc

# Set up default path information variable
[ -n "$DEFPATH" ] || export DEFPATH=$PATH

# Subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# Stuff for the Open Watcom build environment
export BUILD_PLATFORM=linux386
export BATDIR=$OWROOT
export BLD_VER=14
export BLD_VER_STR=1.4
export BUILDER_CTL=lang.ctl
export DEFREL=rel2
export DEVDIR=$OWROOT/bld
# export DISTROOT=%OWROOT%/distrib
export RELROOT=$OWROOT
export DWATCOM=$WATCOM
export DOC_ROOT=$OWROOT/docs
# export LANG=%WATCOM%
export INCLUDE=$WATCOM/lh:$DEVDIR/watcom/h
export LANG_BLD=$OWROOT/bat
export LIB=$OWROOT/bld/watcom/lib

echo Open Watcom compiler build environment

# split up path for easy checking.
export PATH=$OWROOT/bin
export PATH=$PATH:$DEVDIR/build/binl
export PATH=$PATH:$OWROOT/bat
export PATH=$PATH:$WATCOM/binl
export PATH=$PATH:$DOC_ROOT/cmds
export PATH=$PATH:$DEFPATH

export WD_PATH=$WATCOM/binl

export MAKE=make
