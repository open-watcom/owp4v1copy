#!/bin/sh

function usage() {
    echo usage: $0 prgname errorfile
    exit
}

function print_header() {
    echo \# -----------------------------
    echo \#   Test $TEST
    echo \# -----------------------------
}

function do_check() {
    if [ "$?" == "0" ]; then
        echo \# Test $TEST successful
    else
        echo \#\# PREPROCESS \#\# >> $LOGFILE
        echo Error: Test $TEST unsuccessful!!! | tee -a $LOGFILE
        test
    fi
}

if [ "$2" == "" ]; then 
    usage
fi

echo \# ===========================
echo \# Start Preprocessor Test
echo \# ===========================

TEST=1
print_header
rm -f tmp.out
$1 -h -f prep01 > tmp.out 2>&1
diff -b prep01.cmp tmp.out
do_check

TEST=2
print_header
rm -f tmp.out
$1 -h -f prep02 -ms > tmp.out 2>&1
diff prep02.cmp tmp.out
do_check

rm -f tmp.out
