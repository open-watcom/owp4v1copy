#set +v 
# *****************************************************************
# setvars - Linux version
# *****************************************************************
# NOTE: Do not use this script directly, but copy it and
#       modify it as necessary for your own use!!

# Setup environment variables for Perforce
export P4PORT=perforce.scitechsoft.com:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

# root of source tree
export OWROOT=`cd \`dirname -- $0\` && pwd`

# Change this to point to an existing Watcom compiler (if applicable)
export WATCOM=$OWROOT/rel2

# placeholder
# placeholder

# Change this to the install location of GhostScript for PDF creation (optional)
export GHOSTSCRIPT=/usr/bin/gs

# Modifications beyond this point should not be necessary

# Set this variable to 1 to get debug build
export DEBUG_BUILD=0

# Set this variable to 1 to get default windowing support in clib
export DEFAULT_WINDOWING=0

# Set this variable to 0 to suppress documentation build
export DOC_BUILD=0      # Documentation building uses DOS programs.

# Documentation related variables - none likely to work in Linux
# set appropriate variables to blank for help compilers which you haven't installed
# export WIN95HC=hcrtf
# export OS2HC=ipfc

# Set up default path information variable
[ -n "$DEFPATH" ] || export DEFPATH=$PATH

# Subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# Subdirectory to be used for building prerequisite utilities
export PREOBJDIR=prebuild

# Stuff for the Open Watcom build environment
export BUILD_PLATFORM=linux386
export BLD_VER=17
export BLD_VER_STR=1.7
export BUILDER_CTL=lang.ctl
export DEVDIR=$OWROOT/bld

# Set the version numbers, which are stored in a batch file.  Use sed to convert
# the batch file to a shell script and then run it.  Immediately delete the shell
# script to prevent anyone from getting confused and changing the version number
# in it instead of in the batch file.
sed -e 's/@echo off//' -e 's/set/export/' -e 's/REM/#/' version.bat >version.sh
chmod 777 version.sh
./version.sh
rm version.sh

# Subdirectory to be used for bootstrapping/prebuild binaries
export OWBINDIR=$DEVDIR/build/binl
export DISTROOT=$OWROOT/distrib
export RELROOT=$OWROOT
export DWATCOM=$WATCOM
export DOC_ROOT=$OWROOT/docs
export INCLUDE=$WATCOM/lh
export EDPATH=$WATCOM/eddat
export PATH=$OWBINDIR:$OWROOT/bat:$WATCOM/binl:$DOC_ROOT/cmds:$DEFPATH

echo Open Watcom compiler build environment

# OS specifics

export WD_PATH=$WATCOM/binl
export MAKE=make
