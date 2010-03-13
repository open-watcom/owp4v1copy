# *****************************************************************
# CMNVARS.SH - common environment variables
# *****************************************************************
# NOTE: All scripts to set the environment must call this script at
#       the end.

# Set the version numbers
export BLD_VER=19
export BLD_VER_STR=1.9

# Set up default path information variable
[ -n "$DEFPATH" ] || export DEFPATH=$PATH

# Stuff for the Open Watcom build environment
export BUILD_PLATFORM=linux386
export BUILDER_CTL=lang.ctl
export DEVDIR=$OWROOT/bld

# Subdirectory to be used for bootstrapping/prebuild binaries
export OWBINDIR=$DEVDIR/build/binl
export DISTROOT=$OWROOT/distrib
export RELROOT=$OWROOT/rel2
export DWATCOM=$WATCOM
export DOC_ROOT=$OWROOT/docs
export INCLUDE=$WATCOM/lh
export EDPATH=$WATCOM/eddat
export WIPFC=$WATCOM/wipfc
export PATH=$OWBINDIR:$OWROOT/bat:$WATCOM/binl:$DOC_ROOT/cmds:$DEFPATH

echo Open Watcom compiler build environment

# OS specifics

export WD_PATH=$WATCOM/binl
export MAKE=make
