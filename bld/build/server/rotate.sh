#!/bin/sh
#
# Path configuration
# ==================
owbuildpath=/OW/pass1
wwwpath=/www

#
# Initialization
# ==============
cd $wwwpath
if [ ! -d $owbuildpath ]; then
    echo "Missing $owbuildpath. Can't continue with rotation."
    exit -1
fi

# Backup old snapshot
# ===================
if [ -d snapshot ]; then
    if ! mv snapshot snapshot.bak; then
        echo "Existing snapshot backup failed. Can't continue with rotation."
        exit -1
    fi
fi

# Move pass1 build
# =================
mv $owbuildpath/ snapshot

# Build Archives
# ==============
if [ -f ss.zip ]; then rm ss.zip; fi
if [ -f ss.7z ]; then rm ss.7z; fi
cd snapshot
7za a -tzip -r ../ss.zip *
7za a -t7z -r ../ss.7z *
cd ..

# Move Archives
# =============
if [ -f ow-snapshot.zip ]; then rm ow-snapshot.zip; fi
if [ -f ow-snapshot.7z ]; then rm ow-snapshot.7z; fi
mv ss.zip ow-snapshot.zip
mv ss.7z ow-snapshot.7z

# Final Cleanup
# =============
if [ -d snapshot.bak ]; then rm -r snapshot.bak; fi
