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
        echo \#\# IMPLICIT \#\# >> $LOGFILE
        echo Error: Test $TEST unsuccessful!!! | tee -a $LOGFILE
        exit
    fi
}

if [ "$2" == "" ]; then 
    usage
fi

LOGFILE=$2

wtouch hello.obj
wtouch hello.cpp

echo \# ===========================
echo \# Implicit Rules Test
echo \# ===========================

TEST=1
print_header
rm -f err1.out
$1 -h -f makefile.u -l err1.out > tst1.out
do_check

TEST=2A
print_header
sleep 1
touch hello.h
rm -f tst1.out
$1 -h -c -f imp02a > tst1.out
diff imp02.cmp tst1.out
do_check

TEST=2B
print_header
sleep 1
touch hello.h
rm -f tst1.out
$1 -c -h -f imp02b > tst1.out
diff imp02.cmp tst1.out
do_check

TEST=2C
print_header
sleep 1
touch hello.h
rm -f tst1.out
$1 -h -c -f imp02c -ms > tst1.out
diff imp02.cmp tst1.out
do_check

rm -f *.obj
rm -f err1.out
rm -f tst1.out
rm -f tst2.out
rm -f app.exe
rm -f hello.cpp
rm -f hello.h
