#set +v
# *****************************************************************
# setvars - Linux version
# *****************************************************************
# NOTE: Do not use this script directly, but copy it and
#       modify it as necessary for your own use!!

# Setup environment variables for Perforce
export P4PORT=perforce.openwatcom.org:3488
export P4USER=YourName
export P4CLIENT=YOURCLIENT
export P4PASSWD=YourPassword

# root of source tree
export OWROOT=`cd \`dirname -- $0\` && pwd`

# Change this to point to an existing Watcom compiler (if applicable)
export WATCOM=$OWROOT/rel2

# Change this to the PATH required by GhostScript for PDF creation on used host OS (optional)
export GHOSTSCRIPTPATH=/usr/bin

# Set this variable to 1 to get debug build
export DEBUG_BUILD=0

# Set this variable to 1 to get default windowing support in clib
export DEFAULT_WINDOWING=0

# Set this variable to 0 to suppress documentation build
export DOC_BUILD=0      # Documentation building uses DOS programs.

# Documentation related variables
# To run Microsoft help compilers Wine must be installed on box
# set appropriate variables to point to Windows help compilers which you have installed in Wine
# export WIN95HC=hcrtf
# export HHC=hhc

# build process requires WGML utility which is available only as DOS executable
# it is necessary to have some DOS emulator installed
# DOSBOX emulator is available on most platforms
# DOSEMU emulator is available on Linux (Intel platform)
# if DOSBOX emulator is used then OWDOSBOX variable must be set
# Uncoment and set OWDOSBOX variable bellow to point to DOSBOX emulator executable
# Otherwise DOSEMU emulator is used on Linux by default (Intel platform)

# export OWDOSBOX=dosbox

# Subdirectory to be used for bootstrapping
export OBJDIR=bootstrp

# Subdirectory to be used for building prerequisite utilities
export PREOBJDIR=prebuild

# Invoke the script for the common environment
. $OWROOT/cmnvars.sh
