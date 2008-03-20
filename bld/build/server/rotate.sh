#!/bin/sh
#
# Initialization
# ==============
cd /www
if [ ! -d /OW/pass1 ]; then
    echo "Missing /OW/pass1. Can't continue with rotation."
    exit -1
fi

# Backup old snapshot
# ===================
if [ -d snapshot -a ! mv snapshot snapshot.bak ]; then
    echo "Existing snapshot backup failed. Can't continue with rotation."
    exit -1
fi

# Move pass1 build
# =================
mv /OW/pass1 snapshot

# Build Archives
# ==============
test -f ss.zip -a rm ss.zip
test -f ss.7z -a rm ss.7z
cd snapshot
7za a -tzip -r ../ss.zip *
7za a -t7z -r ../ss.7z *
cd ..

# Move Archives
# =============
test -f ow-snapshot.zip -a rm ow-snapshot.zip
test -f ow-snapshot.7z -a rm ow-snapshot.7z
mv ss.zip ow-snapshot.zip
mv ss.7z ow-snapshot.7z

# Final Cleanup
# =============
rm -r snapshot.bak
